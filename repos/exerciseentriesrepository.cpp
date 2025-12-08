#include "exerciseentriesrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ExerciseEntriesRepository::ExerciseEntriesRepository(QObject *parent) : QObject(parent) {}
void ExerciseEntriesRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int ExerciseEntriesRepository::createExerciseEntry(int userId, const QString &date, const QString &exerciseName,
                                                   double duration, double caloriesBurned, const QString &timestamp)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO exercise_entries (user_id, date, exercise_name, duration_minutes, calories_burned, timestamp) "
              "VALUES (:uid, :dt, :ename, :dur, :cal, :ts)");
    q.bindValue(":uid", userId);
    q.bindValue(":dt", date);
    q.bindValue(":ename", exerciseName);
    q.bindValue(":dur", duration);
    q.bindValue(":cal", caloriesBurned);
    q.bindValue(":ts", timestamp);
    if (!q.exec()) {
        qWarning() << "createExerciseEntry failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap ExerciseEntriesRepository::getExerciseEntry(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, exercise_name, duration_minutes, calories_burned, timestamp FROM exercise_entries WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["user_id"] = q.value(1);
        r["date"] = q.value(2);
        r["exercise_name"] = q.value(3);
        r["duration_minutes"] = q.value(4);
        r["calories_burned"] = q.value(5);
        r["timestamp"] = q.value(6);
    }
    return r;
}

QVariantList ExerciseEntriesRepository::getExerciseEntriesByUser(int userId, const QString &date)
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, date, exercise_name, duration_minutes, calories_burned, timestamp "
              "FROM exercise_entries WHERE user_id = :uid AND date = :dt");
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

bool ExerciseEntriesRepository::updateExerciseEntry(int id, const QString &exerciseName, double duration, double caloriesBurned)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE exercise_entries SET exercise_name=:ename, duration_minutes=:dur, calories_burned=:cal WHERE id=:id");
    q.bindValue(":ename", exerciseName);
    q.bindValue(":dur", duration);
    q.bindValue(":cal", caloriesBurned);
    q.bindValue(":id", id);
    return q.exec();
}

bool ExerciseEntriesRepository::deleteExerciseEntry(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM exercise_entries WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
