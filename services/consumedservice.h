#pragma once
#include "BaseService.h"
#include "repos/consumedrepository.h"
#include "repos/dailystatisticsrepository.h"

class ConsumedService : public BaseService {
    Q_OBJECT
public:
    explicit ConsumedService(const QSqlDatabase& db, QObject* parent = nullptr);

    // Добавить запись о потреблении продукта и обновить статистику
    Q_INVOKABLE QVariantMap addConsumedEntry(int userId, const QString& date,
                                             const QString& productName, double quantity,
                                             const QString& unit, double proteins,
                                             double fats, double carbs, double calories,
                                             const QString& timestamp);

private:
    ConsumedRepository* m_consumed;
    DailyStatisticsRepository* m_stats;
};
