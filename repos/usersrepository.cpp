#include "usersrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UsersRepository::UsersRepository(QObject *parent) : QObject(parent) {}

void UsersRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int UsersRepository::createUser(const QString &name, int age, const QString &gender,
                                double height, double weight, const QString &goal, int activityLevel)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO users (name, age, gender, height, weight, goal, activity_level) "
              "VALUES (:name, :age, :gender, :height, :weight, :goal, :activity_level)");
    q.bindValue(":name", name);
    q.bindValue(":age", age);
    q.bindValue(":gender", gender);
    q.bindValue(":height", height);
    q.bindValue(":weight", weight);
    q.bindValue(":goal", goal);
    q.bindValue(":activity_level", activityLevel);
    if (!q.exec()) {
        qWarning() << "createUser failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap UsersRepository::getUser(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, age, gender, height, weight, goal, activity_level FROM users WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["name"] = q.value(1);
        r["age"] = q.value(2);
        r["gender"] = q.value(3);
        r["height"] = q.value(4);
        r["weight"] = q.value(5);
        r["goal"] = q.value(6);
        r["activity_level"] = q.value(7);
    }
    return r;
}

QVariantList UsersRepository::getAllUsers()
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (q.exec("SELECT id, name, age, gender, height, weight, goal, activity_level FROM users ORDER BY id ASC")) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["name"] = q.value(1);
            item["age"] = q.value(2);
            item["gender"] = q.value(3);
            item["height"] = q.value(4);
            item["weight"] = q.value(5);
            item["goal"] = q.value(6);
            item["activity_level"] = q.value(7);
            out.append(item);
        }
    }
    return out;
}

bool UsersRepository::updateUser(int id, const QString &name, int age, const QString &gender,
                                 double height, double weight, const QString &goal, int activityLevel)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE users SET name=:name, age=:age, gender=:gender, height=:height, weight=:weight, "
              "goal=:goal, activity_level=:activity_level WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":age", age);
    q.bindValue(":gender", gender);
    q.bindValue(":height", height);
    q.bindValue(":weight", weight);
    q.bindValue(":goal", goal);
    q.bindValue(":activity_level", activityLevel);
    q.bindValue(":id", id);
    return q.exec();
}

bool UsersRepository::deleteUser(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM users WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
