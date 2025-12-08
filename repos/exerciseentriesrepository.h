#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class ExerciseEntriesRepository : public QObject
{
    Q_OBJECT
public:
    explicit ExerciseEntriesRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createExerciseEntry(int userId, const QString &date, const QString &exerciseName,
                            double duration, double caloriesBurned, const QString &timestamp);
    QVariantMap getExerciseEntry(int id);
    QVariantList getExerciseEntriesByUser(int userId, const QString &date);
    bool updateExerciseEntry(int id, const QString &exerciseName, double duration, double caloriesBurned);
    bool deleteExerciseEntry(int id);

private:
    QSqlDatabase m_db;
};
