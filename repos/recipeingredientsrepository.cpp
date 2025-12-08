#include "recipeingredientsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RecipeIngredientsRepository::RecipeIngredientsRepository(QObject *parent) : QObject(parent) {}
void RecipeIngredientsRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int RecipeIngredientsRepository::createIngredient(int recipeId, const QString &name, double quantity, const QString &unit,
                                                  double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO recipe_ingredients (recipe_id, name, quantity, unit, proteins, fats, carbs) "
              "VALUES (:recipe_id, :name, :quantity, :unit, :proteins, :fats, :carbs)");
    q.bindValue(":recipe_id", recipeId);
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    if (!q.exec()) {
        qWarning() << "createIngredient failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap RecipeIngredientsRepository::getIngredient(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, recipe_id, name, quantity, unit, proteins, fats, carbs FROM recipe_ingredients WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["recipe_id"] = q.value(1);
        r["name"] = q.value(2);
        r["quantity"] = q.value(3);
        r["unit"] = q.value(4);
        r["proteins"] = q.value(5);
        r["fats"] = q.value(6);
        r["carbs"] = q.value(7);
    }
    return r;
}

QVariantList RecipeIngredientsRepository::getIngredientsByRecipe(int recipeId)
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, recipe_id, name, quantity, unit, proteins, fats, carbs FROM recipe_ingredients WHERE recipe_id = :rid ORDER BY id ASC");
    q.bindValue(":rid", recipeId);
    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["recipe_id"] = q.value(1);
            item["name"] = q.value(2);
            item["quantity"] = q.value(3);
            item["unit"] = q.value(4);
            item["proteins"] = q.value(5);
            item["fats"] = q.value(6);
            item["carbs"] = q.value(7);
            out.append(item);
        }
    }
    return out;
}

bool RecipeIngredientsRepository::updateIngredient(int id, const QString &name, double quantity, const QString &unit,
                                                   double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE recipe_ingredients SET name=:name, quantity=:quantity, unit=:unit, proteins=:proteins, fats=:fats, carbs=:carbs WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    q.bindValue(":id", id);
    return q.exec();
}

bool RecipeIngredientsRepository::deleteIngredient(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}

bool RecipeIngredientsRepository::createOrUpdateIngredient(int id,
                                                           int recipeId,
                                                           const QString &name,
                                                           double quantity,
                                                           const QString &unit,
                                                           double proteins,
                                                           double fats,
                                                           double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;

    // UPDATE
    QSqlQuery q(m_db);
    q.prepare("UPDATE recipe_ingredients SET recipe_id=:rid, name=:name, quantity=:qty, unit=:unit, "
              "proteins=:pr, fats=:fat, carbs=:carb WHERE id=:id");
    q.bindValue(":rid", recipeId);
    q.bindValue(":name", name);
    q.bindValue(":qty", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":pr", proteins);
    q.bindValue(":fat", fats);
    q.bindValue(":carb", carbs);
    q.bindValue(":id", id);

    if (!q.exec()) qWarning() << "update ingredient failed:" << q.lastError().text();
    if (q.numRowsAffected() > 0) return true;

    // INSERT
    QSqlQuery qi(m_db);
    qi.prepare("INSERT INTO recipe_ingredients (id, recipe_id, name, quantity, unit, proteins, fats, carbs) "
               "VALUES (:id, :rid, :name, :qty, :unit, :pr, :fat, :carb)");
    qi.bindValue(":id", id);
    qi.bindValue(":rid", recipeId);
    qi.bindValue(":name", name);
    qi.bindValue(":qty", quantity);
    qi.bindValue(":unit", unit);
    qi.bindValue(":pr", proteins);
    qi.bindValue(":fat", fats);
    qi.bindValue(":carb", carbs);

    if (!qi.exec()) {
        qWarning() << "insert ingredient failed:" << qi.lastError().text();
        return false;
    }
    return true;
}


bool RecipeIngredientsRepository::deleteAllForRecipe(int recipeId)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :rid");
    q.bindValue(":rid", recipeId);
    return q.exec();
}
