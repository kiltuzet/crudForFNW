#pragma once
#include "BaseRepository.h"

class DailyStatisticsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit DailyStatisticsRepository(QObject *parent = nullptr);

    // Специфичные методы
    bool createOrUpdateDailyStatistics(int userId, QVariantMap values);

    QVariantMap getDailyStatisticsRecord(int userId, const QString &date);
    QVariantList getDailyStatisticsRange(int userId, const QString &startDate, const QString &endDate);
};
