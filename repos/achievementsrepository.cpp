#include "achievementsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AchievementsRepository::AchievementsRepository(QObject *parent) : QObject(parent) {}

void AchievementsRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int AchievementsRepository::createAchievement(const QString &name, const QString &description, bool unlocked)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO achievements (name, unlocked, description) VALUES (:name, :unlocked, :description)");
    q.bindValue(":name", name);
    q.bindValue(":unlocked", unlocked ? 1 : 0);
    q.bindValue(":description", description);
    if (!q.exec()) {
        qWarning() << "createAchievement failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap AchievementsRepository::getAchievement(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, unlocked, description FROM achievements WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["name"] = q.value(1);
        r["unlocked"] = q.value(2).toBool();
        r["description"] = q.value(3);
    }
    return r;
}

QVariantList AchievementsRepository::getAllAchievements()
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (q.exec("SELECT id, name, unlocked, description FROM achievements ORDER BY id ASC")) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["name"] = q.value(1);
            item["unlocked"] = q.value(2).toBool();
            item["description"] = q.value(3);
            out.append(item);
        }
    }
    return out;
}

bool AchievementsRepository::updateAchievement(int id, const QString &name, const QString &description, bool unlocked)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE achievements SET name=:name, description=:description, unlocked=:unlocked WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":description", description);
    q.bindValue(":unlocked", unlocked ? 1 : 0);
    q.bindValue(":id", id);
    return q.exec();
}
bool AchievementsRepository::createOrUpdateAchievement(int id,
                                                       const QString &name,
                                                       const QString &description,
                                                       bool unlocked)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;

    QSqlQuery q(m_db);
    q.prepare("UPDATE achievements SET name=:name, description=:desc, unlocked=:unlocked WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":desc", description);
    q.bindValue(":unlocked", unlocked ? 1 : 0);
    q.bindValue(":id", id);

    if (!q.exec()) qWarning() << "update achievement failed:" << q.lastError().text();
    if (q.numRowsAffected() > 0) return true;

    QSqlQuery qi(m_db);
    qi.prepare("INSERT INTO achievements (id, name, description, unlocked) VALUES (:id, :name, :desc, :unlocked)");
    qi.bindValue(":id", id);
    qi.bindValue(":name", name);
    qi.bindValue(":desc", description);
    qi.bindValue(":unlocked", unlocked ? 1 : 0);

    if (!qi.exec()) {
        qWarning() << "insert achievement failed:" << qi.lastError().text();
        return false;
    }
    return true;
}


bool AchievementsRepository::deleteAchievement(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM achievements WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
