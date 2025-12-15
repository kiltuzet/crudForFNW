#include "recipeingredientsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RecipeIngredientsRepository::RecipeIngredientsRepository(QObject *parent)
    : BaseRepository("recipe_ingredients", {"id","recipe_id","product_id"}, parent) {}

/*int RecipeIngredientsRepository::addProductToRecipe(int recipeId, int productId) {
    QVariantMap values;
    values["recipe_id"] = recipeId;
    values["product_id"] = productId;
    return create(values);
}*/

QVariantList RecipeIngredientsRepository::getProductsForRecipe(int recipeId) {
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    q.prepare("SELECT product_id FROM recipe_ingredients WHERE recipe_id = :rid");
    q.bindValue(":rid", recipeId);

    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["product_id"] = q.value(0);
            out.append(item);
        }
    }
    return out;
}

bool RecipeIngredientsRepository::deleteAllForRecipe(int recipeId) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :rid");
    q.bindValue(":rid", recipeId);
    return q.exec();
}

int RecipeIngredientsRepository::addProductToRecipe(int recipeId, int productId) {
    // 1. Создаём связку
    QVariantMap values;
    values["recipe_id"] = recipeId;
    values["product_id"] = productId;
    int linkId = create(values);

    if (linkId == -1) return -1;

    // 2. Получаем продукт
    ProductsRepository products;
    products.setDatabase(m_db);
    QVariantMap product = products.getById(productId);

    // 3. Обновляем агрегаты рецепта
    RecipesRepository recipes;
    recipes.setDatabase(m_db);

    QVariantMap recipe = recipes.getById(recipeId);
    double proteins = recipe["proteins"].toDouble() + product["proteins"].toDouble();
    double fats     = recipe["fats"].toDouble()     + product["fats"].toDouble();
    double carbs    = recipe["carbs"].toDouble()    + product["carbs"].toDouble();
    double calories = recipe["calories"].toDouble() +
                      (product["proteins"].toDouble()*4 +
                       product["fats"].toDouble()*9 +
                       product["carbs"].toDouble()*4);

    QVariantMap updateVals;
    updateVals["proteins"] = proteins;
    updateVals["fats"]     = fats;
    updateVals["carbs"]    = carbs;
    updateVals["calories"] = calories;

    recipes.update(recipeId, updateVals);

    return linkId;
}

bool RecipeIngredientsRepository::removeProductFromRecipe(int recipeId, int productId) {
    // 1. Получаем продукт
    ProductsRepository products;
    products.setDatabase(m_db);
    QVariantMap product = products.getById(productId);

    // 2. Вычитаем из агрегатов рецепта
    RecipesRepository recipes;
    recipes.setDatabase(m_db);

    QVariantMap recipe = recipes.getById(recipeId);
    double proteins = recipe["proteins"].toDouble() - product["proteins"].toDouble();
    double fats     = recipe["fats"].toDouble()     - product["fats"].toDouble();
    double carbs    = recipe["carbs"].toDouble()    - product["carbs"].toDouble();
    double calories = recipe["calories"].toDouble() -
                      (product["proteins"].toDouble()*4 +
                       product["fats"].toDouble()*9 +
                       product["carbs"].toDouble()*4);

    QVariantMap updateVals;
    updateVals["proteins"] = proteins;
    updateVals["fats"]     = fats;
    updateVals["carbs"]    = carbs;
    updateVals["calories"] = calories;

    recipes.update(recipeId, updateVals);

    // 3. Удаляем связку
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id=:rid AND product_id=:pid");
    q.bindValue(":rid", recipeId);
    q.bindValue(":pid", productId);
    return q.exec();
}

