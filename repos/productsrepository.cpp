#include "productsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>
#include <QDebug>

ProductsRepository::ProductsRepository(QObject *parent) : QObject(parent) {}

void ProductsRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int ProductsRepository::createProduct(const QString &name, double quantity, const QString &unit,
                                      double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO products (name, quantity, unit, proteins, fats, carbs) "
              "VALUES (:name, :quantity, :unit, :proteins, :fats, :carbs)");
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    if (!q.exec()) {
        qWarning() << "createProduct failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap ProductsRepository::getProduct(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, quantity, unit, proteins, fats, carbs FROM products WHERE id = :id");
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

QVariantMap ProductsRepository::getProductByName(const QString &name)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, quantity, unit, proteins, fats, carbs FROM products WHERE LOWER(name)=LOWER(:name)");
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

QVariantList ProductsRepository::getAllProducts()
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (q.exec("SELECT id, name, quantity, unit, proteins, fats, carbs FROM products ORDER BY id ASC")) {
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

bool ProductsRepository::updateProduct(int id, const QString &name, double quantity, const QString &unit,
                                       double proteins, double fats, double carbs)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE products SET name=:name, quantity=:quantity, unit=:unit, proteins=:proteins, fats=:fats, carbs=:carbs WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":quantity", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":proteins", proteins);
    q.bindValue(":fats", fats);
    q.bindValue(":carbs", carbs);
    q.bindValue(":id", id);
    return q.exec();
}

int ProductsRepository::getIdByName(const QString &name)
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

bool ProductsRepository::createOrUpdateProduct(const QString &name,
                                               double quantity,
                                               const QString &unit,
                                               double proteins,
                                               double fats,
                                               double carbs)
{
    if (name.isEmpty()) return false;

    int id = getIdByName(name);
    if (id != -1) {
                qDebug()<<"update Product was triggered";
        return updateProduct(id, name, quantity, unit, proteins, fats, carbs);

    } else {
         qDebug()<<"update Product was triggered";
        return createProduct(name, quantity, unit, proteins, fats, carbs) != -1;
    }
}



bool ProductsRepository::deleteProduct(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM products WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
