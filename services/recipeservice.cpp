#include "recipeservice.h"

RecipesService::RecipesService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent),
    m_recipes(new RecipesRepository(this)),
    m_ingredients(new RecipeIngredientsRepository(this)),
    m_products(new ProductsRepository(this)) {
    m_recipes->setDatabase(db);
    m_ingredients->setDatabase(db);
    m_products->setDatabase(db);
}

QVariantMap RecipesService::createRecipe(const QString& name, const QList<int>& productIds) {
    if (!beginTransaction()) return makeResponse(false, "Ошибка транзакции");

    // 1. Создаём рецепт
    int recipeId = m_recipes->createRecipe(name, 0, "", 0, 0, 0);
    if (recipeId == -1) {
        rollbackTransaction();
        return makeResponse(false, "Не удалось создать рецепт");
    }

    // 2. Добавляем продукты в рецепт
    double totalProteins = 0, totalFats = 0, totalCarbs = 0, totalCalories = 0;
    for (int productId : productIds) {
        m_ingredients->addProductToRecipe(recipeId, productId);

        QVariantMap product = m_products->getProduct(productId);
        totalProteins += product["proteins"].toDouble();
        totalFats     += product["fats"].toDouble();
        totalCarbs    += product["carbs"].toDouble();
        totalCalories += product["proteins"].toDouble()*4
                         + product["fats"].toDouble()*9
                         + product["carbs"].toDouble()*4;
    }

    // 3. Обновляем агрегаты рецепта
    QVariantMap updateVals;
    updateVals["proteins"] = totalProteins;
    updateVals["fats"]     = totalFats;
    updateVals["carbs"]    = totalCarbs;
    updateVals["calories"] = totalCalories;
    m_recipes->update(recipeId, updateVals);

    commitTransaction();

    QVariantMap payload;
    payload["id"] = recipeId;
    payload["name"] = name;
    payload["proteins"] = totalProteins;
    payload["fats"] = totalFats;
    payload["carbs"] = totalCarbs;
    payload["calories"] = totalCalories;

    return makeResponse(true, "Рецепт создан", payload);
}

QVariantMap RecipesService::deleteRecipe(int recipeId) {
    bool ok = m_recipes->deleteRecipeWithIngredients(recipeId);
    return makeResponse(ok, ok ? "Рецепт удалён" : "Ошибка удаления");
}
