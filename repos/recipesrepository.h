#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class RecipesRepository : public QObject
{
    Q_OBJECT
public:
    explicit RecipesRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createRecipe(const QString &name, double quantity, const QString &unit,
                     double proteins, double fats, double carbs);
    QVariantMap getRecipe(int id);
    QVariantList getAllRecipes();
    bool updateRecipe(int id, const QString &name, double quantity, const QString &unit,
                      double proteins, double fats, double carbs);
    bool deleteRecipe(int id);
    bool createOrUpdateRecipe(const QString &name,
                          double quantity,
                          const QString &unit,
                          double proteins,
                          double fats,
                              double carbs);
    int getIdByName(const QString &name);
    QVariantMap getRecipeByName(const QString &name);

private:
    QSqlDatabase m_db;
};
