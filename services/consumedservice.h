#pragma once
#include "BaseService.h"
#include "repos/consumedrepository.h"
#include "repos/dailystatisticsrepository.h"
#include "repos/recipesrepository.h"
#include "repos/productsrepository.h"

class ConsumedService : public BaseService {
    Q_OBJECT
public:
    explicit ConsumedService(const QSqlDatabase& db, QObject* parent = nullptr);

    // Добавить запись о потреблении продукта и обновить статистику
    Q_INVOKABLE QVariantMap addConsumedEntry(QVariantMap values);

private:
    ConsumedRepository* m_consumed;
    DailyStatisticsRepository* m_stats;
    RecipesRepository *m_recipe;
    ProductsRepository *m_product;
};
