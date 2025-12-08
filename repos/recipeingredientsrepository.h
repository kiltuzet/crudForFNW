#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class RecipeIngredientsRepository : public QObject
{
    Q_OBJECT
public:
    explicit RecipeIngredientsRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createIngredient(int recipeId, const QString &name, double quantity, const QString &unit,
                         double proteins, double fats, double carbs);
    QVariantMap getIngredient(int id);
    QVariantList getIngredientsByRecipe(int recipeId);
    bool updateIngredient(int id, const QString &name, double quantity, const QString &unit,
                          double proteins, double fats, double carbs);
    bool deleteIngredient(int id);
    bool deleteAllForRecipe(int recipeId);
    bool createOrUpdateIngredient(int id,
                                  int recipeId,
                                  const QString &name,
                                  double quantity,
                                  const QString &unit,
                                  double proteins,
                                  double fats,
                                  double carbs);
private:
    QSqlDatabase m_db;
};
