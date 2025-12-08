#include "dailystatisticsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DailyStatisticsRepository::DailyStatisticsRepository(QObject *parent) : QObject(parent) {}
void DailyStatisticsRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

bool DailyStatisticsRepository::createOrUpdateDailyStatistics(int userId, const QString &date, double waterMl,
                                                              double totalProteins, double totalFats,
                                                              double totalCarbs, double totalCalories)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    // Try update first
    QSqlQuery q(m_db);
    q.prepare("UPDATE daily_statistics SET water_ml=:water, total_proteins=:p, total_fats=:f, total_carbs=:c, total_calories=:cal WHERE user_id=:uid AND date=:date");
    q.bindValue(":water", waterMl);
    q.bindValue(":p", totalProteins);
    q.bindValue(":f", totalFats);
    q.bindValue(":c", totalCarbs);
    q.bindValue(":cal", totalCalories);
    q.bindValue(":uid", userId);
    q.bindValue(":date", date);
    if (!q.exec()) {
        qWarning() << "update daily_statistics failed:" << q.lastError().text();
        // continue to try insert
    }
    if (q.numRowsAffected() > 0) return true;

    // Insert
    QSqlQuery qi(m_db);
    qi.prepare("INSERT INTO daily_statistics (user_id, date, water_ml, total_proteins, total_fats, total_carbs, total_calories) "
               "VALUES (:uid, :date, :water, :p, :f, :c, :cal)");
    qi.bindValue(":uid", userId);
    qi.bindValue(":date", date);
    qi.bindValue(":water", waterMl);
    qi.bindValue(":p", totalProteins);
    qi.bindValue(":f", totalFats);
    qi.bindValue(":c", totalCarbs);
    qi.bindValue(":cal", totalCalories);
    if (!qi.exec()) {
        qWarning() << "insert daily_statistics failed:" << qi.lastError().text();
        return false;
    }
    return true;
}

QVariantMap DailyStatisticsRepository::getDailyStatisticsRecord(int userId, const QString &date)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT water_ml, total_proteins, total_fats, total_carbs, total_calories FROM daily_statistics WHERE user_id=:uid AND date=:date");
    q.bindValue(":uid", userId);
    q.bindValue(":date", date);
    if (q.exec() && q.next()) {
        r["waterMl"] = q.value(0).toDouble();
        r["proteins"] = q.value(1).toDouble();
        r["fats"] = q.value(2).toDouble();
        r["carbs"] = q.value(3).toDouble();
        r["calories"] = q.value(4).toDouble();
        r["date"] = date;
    }
    return r;
}

QVariantList DailyStatisticsRepository::getDailyStatisticsRange(int userId, const QString &startDate, const QString &endDate)
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    q.prepare("SELECT date, water_ml, total_proteins, total_fats, total_carbs, total_calories FROM daily_statistics WHERE user_id=:uid AND date BETWEEN :start AND :end ORDER BY date ASC");
    q.bindValue(":uid", userId);
    q.bindValue(":start", startDate);
    q.bindValue(":end", endDate);
    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["date"] = q.value(0);
            item["waterMl"] = q.value(1).toDouble();
            item["proteins"] = q.value(2).toDouble();
            item["fats"] = q.value(3).toDouble();
            item["carbs"] = q.value(4).toDouble();
            item["calories"] = q.value(5).toDouble();
            out.append(item);
        }
    }
    return out;
}

bool DailyStatisticsRepository::deleteDailyStatistics(int userId, const QString &date)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM daily_statistics WHERE user_id=:uid AND date=:date");
    q.bindValue(":uid", userId);
    q.bindValue(":date", date);
    return q.exec();
}
