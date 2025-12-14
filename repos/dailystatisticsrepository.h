#pragma once
#include "BaseRepository.h"

class DailyStatisticsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit DailyStatisticsRepository(QObject *parent = nullptr);

    // Специфичные методы
    bool createOrUpdateDailyStatistics(int userId, const QString &date,
                                       double waterMl,
                                       double totalProteins,
                                       double totalFats,
                                       double totalCarbs,
                                       double totalCalories);

    QVariantMap getDailyStatisticsRecord(int userId, const QString &date);
    QVariantList getDailyStatisticsRange(int userId, const QString &startDate, const QString &endDate);
};
