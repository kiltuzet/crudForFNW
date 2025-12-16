#include "consumedrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

ConsumedRepository::ConsumedRepository(QObject *parent)
    : BaseRepository("consumed_entries",
                     {"id","user_id","date","product_name","quantity","unit",
                      "proteins","fats","carbs","calories","timestamp"},
                     parent) {}

int ConsumedRepository::createConsumedEntry(int userId, const QString &date,
                                            const QString &productName,
                                            double quantity, const QString &unit,
                                            double proteins, double fats,
                                            double carbs, double calories,
                                            const QString &timestamp) {
    QVariantMap values;
    values["user_id"] = userId;
    values["date"] =  QDate::currentDate().toString("yyyy-MM-dd");
    values["product_name"] = productName;
    values["quantity"] = quantity;
    values["unit"] = unit;
    values["proteins"] = proteins;
    values["fats"] = fats;
    values["carbs"] = carbs;
    values["calories"] = calories;
    values["timestamp"] =;
    qDebug()<<"consumed entry data:";
    qDebug()<<values;
    return create(values);
}

QVariantList ConsumedRepository::getConsumedEntriesByUser(int userId, const QString &date) {
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, product_name, quantity, unit, proteins, fats, carbs, calories, timestamp "
              "FROM consumed_entries WHERE user_id = :uid AND date = :dt ORDER BY id ASC");
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
