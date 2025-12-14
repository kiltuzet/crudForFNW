#include "recipesrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RecipesRepository::RecipesRepository(QObject *parent)
    : BaseRepository("recipes",
                     {"id","name","quantity","unit","proteins","fats","carbs"},
                     parent) {}

int RecipesRepository::createRecipe(const QString &name, double quantity,
                                    const QString &unit,
                                    double proteins, double fats, double carbs) {
    QVariantMap values;
    values["name"] = name;
    values["quantity"] = quantity;
    values["unit"] = unit;
    values["proteins"] = proteins;
    values["fats"] = fats;
    values["carbs"] = carbs;
    return create(values);
}

QVariantMap RecipesRepository::getRecipeByName(const QString &name) {
    return getByField("name", name);
}

bool RecipesRepository::deleteRecipeWithIngredients(int id) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;

    // Сначала удаляем ингредиенты рецепта
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :rid");
    q.bindValue(":rid", id);
    if (!q.exec()) {
        qWarning() << "deleteRecipeWithIngredients: failed to delete ingredients:"
                   << q.lastError().text();
        return false;
    }

    // Затем удаляем сам рецепт
    return deleteById(id);
}
