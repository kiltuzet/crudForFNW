#include "consumedrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ConsumedRepository::ConsumedRepository(QObject *parent) : QObject(parent) {}
void ConsumedRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int ConsumedRepository::createConsumedEntry(int userId, const QString &date, const QString &productName,
                                            double quantity, const QString &unit, double proteins,
                                            double fats, double carbs, double calories, const QString &timestamp)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO consumed_entries (user_id, date, product_name, quantity, unit, proteins, fats, carbs, calories, timestamp) "
              "VALUES (:uid, :date, :pname, :qty, :unit, :pr, :fat, :carb, :cal, :ts)");
    q.bindValue(":uid", userId);
    q.bindValue(":date", date);
    q.bindValue(":pname", productName);
    q.bindValue(":qty", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":pr", proteins);
    q.bindValue(":fat", fats);
    q.bindValue(":carb", carbs);
    q.bindValue(":cal", calories);
    q.bindValue(":ts", timestamp);
    if (!q.exec()) {
        qWarning() << "createConsumedEntry failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap ConsumedRepository::getConsumedEntry(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, product_name, quantity, unit, proteins, fats, carbs, calories, timestamp FROM consumed_entries WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["user_id"] = q.value(1);
        r["date"] = q.value(2);
        r["product_name"] = q.value(3);
        r["quantity"] = q.value(4);
        r["unit"] = q.value(5);
        r["proteins"] = q.value(6);
        r["fats"] = q.value(7);
        r["carbs"] = q.value(8);
        r["calories"] = q.value(9);
        r["timestamp"] = q.value(10);
    }
    return r;
}

QVariantMap ConsumedRepository::getConsumedEntry(int userId, const QString &date)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, product_name, quantity, unit, proteins, fats, carbs, calories, timestamp FROM consumed_entries WHERE id = :id");
    q.bindValue(":uid", userId);
    q.bindValue(":dt", date);
    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["user_id"] = q.value(1);
            item["date"] = q.value(2);
            item["product_name"] = q.value(3);
            item["quantity"] = q.value(4);
            item["unit"] = q.value(5);
            item["proteins"] = q.value(6);
            item["fats"] = q.value(7);
            item["carbs"] = q.value(8);
            item["calories"] = q.value(9);
            item["timestamp"] = q.value(10);
        }
    }
    return r;
}

QVariantList ConsumedRepository::getConsumedEntriesByUser(int userId, const QString &date)
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, product_name, quantity, unit, proteins, fats, carbs, calories, timestamp "
              "FROM consumed_entries WHERE user_id = :uid AND date = :dt");
    q.bindValue(":uid", userId);
    q.bindValue(":dt", date);
    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["user_id"] = q.value(1);
            item["date"] = q.value(2);
            item["product_name"] = q.value(3);
            item["quantity"] = q.value(4);
            item["unit"] = q.value(5);
            item["proteins"] = q.value(6);
            item["fats"] = q.value(7);
            item["carbs"] = q.value(8);
            item["calories"] = q.value(9);
            item["timestamp"] = q.value(10);
            out.append(item);
        }
    }
    return out;
}


int ConsumedRepository::getIdByName(const QString &productName)
{
    if (productName.isEmpty() || !m_db.isValid() || !m_db.isOpen())
        return -1;

    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM consumed_entries WHERE LOWER(product_name)=LOWER(:pname)");
    q.bindValue(":pname", productName);
    if (q.exec() && q.next())
        return q.value(0).toInt();
    return -1;
}

bool ConsumedRepository::updateConsumedEntry(int id, const QString &productName, double quantity,
                                             const QString &unit, double proteins, double fats,
                                             double carbs, double calories)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE consumed_entries SET user_id=:uid, date=:date, product_name=:pname, "
              "quantity=:qty, unit=:unit, proteins=:pr, fats=:fat, carbs=:carb, calories=:cal, timestamp=:ts "
              "WHERE id=:id");
    q.bindValue(":pname", productName);
    q.bindValue(":qty", quantity);
    q.bindValue(":unit", unit);
    q.bindValue(":pr", proteins);
    q.bindValue(":fat", fats);
    q.bindValue(":carb", carbs);
    q.bindValue(":cal", calories);
    q.bindValue(":id", id);
    qDebug()<<q.executedQuery();
    qDebug()<<"update was triggered";
    return q.exec();
}

bool ConsumedRepository::createOrUpdateConsumedEntry( int userId, const QString &date,
                                                     const QString &productName, double quantity,
                                                     const QString &unit, double proteins,
                                                     double fats, double carbs, double calories,
                                                     const QString &timestamp)
{
    if (productName.isEmpty()) return false;

    int id = getIdByName(productName);
    if (id != -1) {
        return updateConsumedEntry(id, productName, quantity, unit, proteins, fats, carbs, calories);
            qDebug()<<"update consumed was triggered";
    } else {
            qDebug()<<"create consumed was triggered";
        return createConsumedEntry(userId, date, productName, quantity, unit,
                                   proteins, fats, carbs, calories, timestamp) != -1;
    }
}


bool ConsumedRepository::deleteConsumedEntry(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM consumed_entries WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
