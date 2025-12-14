#include "exerciseservice.h"

ExerciseService::ExerciseService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent),
    m_exercises(new ExerciseEntriesRepository(this)),
    m_stats(new DailyStatisticsRepository(this)) {
    m_exercises->setDatabase(db);
    m_stats->setDatabase(db);
}

QVariantMap ExerciseService::addExerciseEntry(int userId, const QString& date,
                                              const QString& exerciseName,
                                              double duration, double caloriesBurned,
                                              const QString& timestamp) {
    if (!beginTransaction()) return makeResponse(false, "Ошибка транзакции");

    int id = m_exercises->createExerciseEntry(userId, date, exerciseName,
                                              duration, caloriesBurned, timestamp);
    if (id == -1) {
        rollbackTransaction();
        return makeResponse(false, "Не удалось создать запись упражнения");
    }

    // Обновляем статистику (например, калории)
    QVariantMap stats = m_stats->getDailyStatisticsRecord(userId, date);
    double newCalories = stats["calories"].toDouble() - caloriesBurned; // вычитаем сожжённые калории

    bool ok = m_stats->createOrUpdateDailyStatistics(userId, date,
                                                     stats["waterMl"].toDouble(),
                                                     stats["proteins"].toDouble(),
                                                     stats["fats"].toDouble(),
                                                     stats["carbs"].toDouble(),
                                                     newCalories);
    if (!ok) {
        rollbackTransaction();
        return makeResponse(false, "Не удалось обновить статистику");
    }

    commitTransaction();

    QVariantMap payload;
    payload["id"] = id;
    payload["exerciseName"] = exerciseName;
    payload["caloriesBurned"] = caloriesBurned;
    payload["date"] = date;
    payload["caloriesAfterExercise"] = newCalories;

    return makeResponse(true, "Упражнение добавлено", payload);
}
