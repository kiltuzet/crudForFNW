#pragma once
#include "BaseRepository.h"

class ExerciseEntriesRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ExerciseEntriesRepository(QObject *parent = nullptr);

    // Специфичные методы
    int createExerciseEntry(int userId, const QString &date, const QString &exerciseName,
                            double duration, double caloriesBurned, const QString &timestamp);

    QVariantList getExerciseEntriesByUser(int userId, const QString &date);
};
