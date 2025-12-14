#include "dailystatisticsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DailyStatisticsRepository::DailyStatisticsRepository(QObject *parent)
    : BaseRepository("daily_statistics",
                     {"id","user_id","date","water_ml","total_proteins","total_fats","total_carbs","total_calories"},
                     parent) {}

bool DailyStatisticsRepository::createOrUpdateDailyStatistics(int userId, const QString &date,
                                                              double waterMl,
                                                              double totalProteins,
                                                              double totalFats,
                                                              double totalCarbs,
                                                              double totalCalories) {
    QVariantMap values;
    values["user_id"] = userId;
    values["date"] = date;
    values["water_ml"] = waterMl;
    values["total_proteins"] = totalProteins;
    values["total_fats"] = totalFats;
    values["total_carbs"] = totalCarbs;
    values["total_calories"] = totalCalories;
    return createOrUpdate(userId, values);
}

QVariantMap DailyStatisticsRepository::getDailyStatisticsRecord(int userId, const QString &date) {
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;

    QSqlQuery q(m_db);
    q.prepare("SELECT water_ml, total_proteins, total_fats, total_carbs, total_calories "
              "FROM daily_statistics WHERE user_id=:uid AND date=:date");
    q.bindValue(":uid", userId);
    q.bindValue(":date", date);

    if (q.exec() && q.next()) {
        r["waterMl"]   = q.value(0).toDouble();
        r["proteins"]  = q.value(1).toDouble();
        r["fats"]      = q.value(2).toDouble();
        r["carbs"]     = q.value(3).toDouble();
        r["calories"]  = q.value(4).toDouble();
        r["date"]      = date;
    }
    return r;
}

QVariantList DailyStatisticsRepository::getDailyStatisticsRange(int userId, const QString &startDate, const QString &endDate) {
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    q.prepare("SELECT date, water_ml, total_proteins, total_fats, total_carbs, total_calories "
              "FROM daily_statistics WHERE user_id=:uid AND date BETWEEN :start AND :end ORDER BY date ASC");
    q.bindValue(":uid", userId);
    q.bindValue(":start", startDate);
    q.bindValue(":end", endDate);

    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["date"]      = q.value(0);
            item["waterMl"]   = q.value(1).toDouble();
            item["proteins"]  = q.value(2).toDouble();
            item["fats"]      = q.value(3).toDouble();
            item["carbs"]     = q.value(4).toDouble();
            item["calories"]  = q.value(5).toDouble();
            out.append(item);
        }
    }
    return out;
}
