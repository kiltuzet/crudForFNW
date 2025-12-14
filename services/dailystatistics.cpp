#include "dailystatistics.h"

DailyStatisticsService::DailyStatisticsService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent), m_repo(new DailyStatisticsRepository(this)) {
    m_repo->setDatabase(db);
}

QVariantMap DailyStatisticsService::getDailyStatistics(int userId, const QString& date) {
    QVariantMap stats = m_repo->getDailyStatisticsRecord(userId, date);
    return makeResponse(!stats.isEmpty(), stats.isEmpty() ? "Нет данных" : "ОК", stats);
}
