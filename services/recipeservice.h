#include "BaseService.h"
#include "repos/recipesrepository.h"
#include "repos/recipeingredientsrepository.h"
#include "repos/productsrepository.h"
class RecipesService : public BaseService {
    Q_OBJECT
public:
    explicit RecipesService(const QSqlDatabase& db, QObject* parent = nullptr)
        : BaseService(db),
        m_recipes(new RecipesRepository(this)),
        m_ingredients(new RecipeIngredientsRepository(this)),
        m_products(new ProductsRepository(this)) {
        m_recipes->setDatabase(db);
        m_ingredients->setDatabase(db);
        m_products->setDatabase(db);
    }

    Q_INVOKABLE QVariantMap createRecipe(const QString& name, const QList<int>& productIds);
    Q_INVOKABLE bool deleteRecipe(int recipeId);

private:
    RecipesRepository* m_recipes;
    RecipeIngredientsRepository* m_ingredients;
    ProductsRepository* m_products;
};
