#pragma once
#include "BaseRepository.h"

class RecipesRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit RecipesRepository(QObject *parent = nullptr);

    // Специфичные методы
    int createRecipe(const QString &name, double quantity, const QString &unit,
                     double proteins, double fats, double carbs);

    QVariantMap getRecipeByName(const QString &name);

    // Удаление рецепта вместе с ингредиентами
    bool deleteRecipeWithIngredients(int id);
};
