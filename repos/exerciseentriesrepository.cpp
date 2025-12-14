#include "exerciseentriesrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ExerciseEntriesRepository::ExerciseEntriesRepository(QObject *parent)
    : BaseRepository("exercise_entries",
                     {"id","user_id","date","exercise_name","duration_minutes","calories_burned","timestamp"},
                     parent) {}

int ExerciseEntriesRepository::createExerciseEntry(int userId, const QString &date,
                                                   const QString &exerciseName,
                                                   double duration, double caloriesBurned,
                                                   const QString &timestamp) {
    QVariantMap values;
    values["user_id"] = userId;
    values["date"] = date;
    values["exercise_name"] = exerciseName;
    values["duration_minutes"] = duration;
    values["calories_burned"] = caloriesBurned;
    values["timestamp"] = timestamp;
    return create(values);
}

QVariantList ExerciseEntriesRepository::getExerciseEntriesByUser(int userId, const QString &date) {
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, exercise_name, duration_minutes, calories_burned, timestamp "
              "FROM exercise_entries WHERE user_id = :uid AND date = :dt ORDER BY id ASC");
    q.bindValue(":uid", userId);
    q.bindValue(":dt", date);

    if (q.exec()) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["user_id"] = q.value(1);
            item["date"] = q.value(2);
            item["exercise_name"] = q.value(3);
            item["duration_minutes"] = q.value(4);
            item["calories_burned"] = q.value(5);
            item["timestamp"] = q.value(6);
            out.append(item);
        }
    }
    return out;
}
