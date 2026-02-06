#pragma once
#include "BaseRepository.h"

class RecipeIngredientsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit RecipeIngredientsRepository(QObject *parent = nullptr);

    // Добавить продукт в рецепт
    int addProductToRecipe(int recipeId, int productId);

    // Получить все продукты для рецепта
    QVariantList getProductsForRecipe(int recipeId);

    // Удалить все продукты для рецепта
    bool deleteAllForRecipe(int recipeId);

    bool removeProductFromRecipe(int recipeId, int productId);

    //int addProductToRecipe(int recipeId, int productId);
};
