#pragma once
#include "BaseService.h"
#include "repos/exerciseentriesrepository.h"
#include "repos/dailystatisticsrepository.h"

class ExerciseService : public BaseService {
    Q_OBJECT
public:
    explicit ExerciseService(const QSqlDatabase& db, QObject* parent = nullptr);

    // Добавить запись об упражнении
    Q_INVOKABLE QVariantMap addExerciseEntry(int userId, const QString& date,
                                             const QString& exerciseName,
                                             double duration, double caloriesBurned,
                                             const QString& timestamp);

private:
    ExerciseEntriesRepository* m_exercises;
    DailyStatisticsRepository* m_stats;
};
