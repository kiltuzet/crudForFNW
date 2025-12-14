#pragma once
#include "BaseService.h"
#include "repos/dailystatisticsrepository.h"

class DailyStatisticsService : public BaseService {
    Q_OBJECT
public:
    explicit DailyStatisticsService(const QSqlDatabase& db, QObject* parent = nullptr);

    Q_INVOKABLE QVariantMap getDailyStatistics(int userId, const QString& date);

private:
    DailyStatisticsRepository* m_repo;
};
