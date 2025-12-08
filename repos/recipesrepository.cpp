#include "recipesrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RecipesRepository::RecipesRepository(QObject *parent) : QObject(parent) {}

void RecipesRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int RecipesRepository::createRecipe(const QString &name, double quantity, const QString &unit,
                                    double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO recipes (name, quantity, unit, proteins, fats, carbs) VALUES (:name, :quantity, :unit, :proteins, :fats, :carbs)");//unique name
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    if (!q.exec()) {
        qWarning() << "createRecipe failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap RecipesRepository::getRecipe(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, quantity, unit, proteins, fats, carbs FROM recipes WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["name"] = q.value(1);
        r["quantity"] = q.value(2);
        r["unit"] = q.value(3);
        r["proteins"] = q.value(4);
        r["fats"] = q.value(5);
        r["carbs"] = q.value(6);
    }
    return r;
}

QVariantList RecipesRepository::getAllRecipes()
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (q.exec("SELECT id, name, quantity, unit, proteins, fats, carbs FROM recipes ORDER BY id ASC")) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["name"] = q.value(1);
            item["quantity"] = q.value(2);
            item["unit"] = q.value(3);
            item["proteins"] = q.value(4);
            item["fats"] = q.value(5);
            item["carbs"] = q.value(6);
            out.append(item);
        }
    }
    return out;
}

bool RecipesRepository::updateRecipe(int id, const QString &name, double quantity, const QString &unit,
                                     double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE recipes SET name=:name, quantity=:quantity, unit=:unit, proteins=:proteins, fats=:fats, carbs=:carbs WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    q.bindValue(":id", id);
    return q.exec();
}

int RecipesRepository::getIdByName(const QString &name)
{
    if (name.isEmpty() || !m_db.isValid() || !m_db.isOpen())
        return -1;

    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM products WHERE LOWER(name)=LOWER(:name)");
    q.bindValue(":name", name);
    if (q.exec() && q.next())
        return q.value(0).toInt();
    return -1;
}

bool RecipesRepository::createOrUpdateRecipe(const QString &name,
                                             double quantity,
                                             const QString &unit,
                                             double proteins,
                                             double fats,
                                             double carbs)
{
    if (name.isEmpty()) return false;

    int id = getIdByName(name);
    if (id != -1) {
        return updateRecipe(id, name, quantity, unit, proteins, fats, carbs);
    } else {
        return createRecipe(name, quantity, unit, proteins, fats, carbs) != -1;
    }
}

QVariantMap RecipesRepository::getRecipeByName(const QString &name)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen() || name.isEmpty()) return r;

    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, quantity, unit, proteins, fats, carbs "
              "FROM recipes WHERE LOWER(name)=LOWER(:name)");
    q.bindValue(":name", name);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["name"] = q.value(1);
        r["quantity"] = q.value(2);
        r["unit"] = q.value(3);
        r["proteins"] = q.value(4);
        r["fats"] = q.value(5);
        r["carbs"] = q.value(6);
    }
    return r;
}


bool RecipesRepository::deleteRecipe(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    // Delete ingredients first to avoid FK constraint issues if not set to cascade
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM recipe_ingredients WHERE recipe_id = :rid");
    q.bindValue(":rid", id);
    if (!q.exec()) {
        qWarning() << "deleteRecipe: failed to delete recipe ingredients:" << q.lastError().text();
        return false;
    }
    QSqlQuery q2(m_db);
    q2.prepare("DELETE FROM recipes WHERE id = :id");
    q2.bindValue(":id", id);
    return q2.exec();
}
