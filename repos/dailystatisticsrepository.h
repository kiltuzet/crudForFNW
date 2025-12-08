#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class DailyStatisticsRepository : public QObject
{
    Q_OBJECT
public:
    explicit DailyStatisticsRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    bool createOrUpdateDailyStatistics(int userId, const QString &date, double waterMl,
                                       double totalProteins, double totalFats,
                                       double totalCarbs, double totalCalories);
    QVariantMap getDailyStatisticsRecord(int userId, const QString &date);
    QVariantList getDailyStatisticsRange(int userId, const QString &startDate, const QString &endDate);
    bool deleteDailyStatistics(int userId, const QString &date);

private:
    QSqlDatabase m_db;
};
