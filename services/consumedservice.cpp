#include "consumedservice.h"

ConsumedService::ConsumedService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent),
    m_consumed(new ConsumedRepository(this)),
    m_stats(new DailyStatisticsRepository(this)) {
    m_consumed->setDatabase(db);
    m_stats->setDatabase(db);
}

QVariantMap ConsumedService::addConsumedEntry(int userId, const QString& date,
                                              const QString& productName, double quantity,
                                              const QString& unit, double proteins,
                                              double fats, double carbs, double calories,
                                              const QString& timestamp) {
    if (!beginTransaction()) return makeResponse(false, "Ошибка транзакции");

    int id = m_consumed->createConsumedEntry(userId, date, productName, quantity, unit,
                                             proteins, fats, carbs, calories, timestamp);
    if (id == -1) {
        rollbackTransaction();
        return makeResponse(false, "Не удалось создать запись");
    }

    // Получаем текущую статистику
    QVariantMap stats = m_stats->getDailyStatisticsRecord(userId, date);
    double newProteins = stats["proteins"].toDouble() + proteins;
    double newFats     = stats["fats"].toDouble() + fats;
    double newCarbs    = stats["carbs"].toDouble() + carbs;
    double newCalories = stats["calories"].toDouble() + calories;

    bool ok = m_stats->createOrUpdateDailyStatistics(userId, date,
                                                     stats["waterMl"].toDouble(),
                                                     newProteins, newFats, newCarbs, newCalories);
    if (!ok) {
        rollbackTransaction();
        return makeResponse(false, "Не удалось обновить статистику");
    }

    commitTransaction();

    QVariantMap payload;
    payload["id"] = id;
    payload["date"] = date;
    payload["calories"] = newCalories;
    payload["proteins"] = newProteins;
    payload["fats"] = newFats;
    payload["carbs"] = newCarbs;

    return makeResponse(true, "Запись добавлена", payload);
}
