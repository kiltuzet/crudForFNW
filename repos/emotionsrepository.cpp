#include "emotionsrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

EmotionsRepository::EmotionsRepository(QObject *parent) : QObject(parent) {}
void EmotionsRepository::setDatabase(const QSqlDatabase &db) { m_db = db; }

int EmotionsRepository::createEmotion(const QString &name, const QString &classification)
{
    if (!m_db.isValid() || !m_db.isOpen()) return -1;
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO emotions (name, classification) VALUES (:name, :classification)");
    q.bindValue(":name", name);
    q.bindValue(":classification", classification);
    if (!q.exec()) {
        qWarning() << "createEmotion failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap EmotionsRepository::getEmotion(int id)
{
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, classification FROM emotions WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec() && q.next()) {
        r["id"] = q.value(0);
        r["name"] = q.value(1);
        r["classification"] = q.value(2);
    }
    return r;
}

QVariantList EmotionsRepository::getAllEmotions()
{
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;
    QSqlQuery q(m_db);
    if (q.exec("SELECT id, name, classification FROM emotions ORDER BY id ASC")) {
        while (q.next()) {
            QVariantMap item;
            item["id"] = q.value(0);
            item["name"] = q.value(1);
            item["classification"] = q.value(2);
            out.append(item);
        }
    }
    return out;
}

bool EmotionsRepository::updateEmotion(int id, const QString &name, const QString &classification)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("UPDATE emotions SET name=:name, classification=:classification WHERE id=:id");
    q.bindValue(":name", name);
    q.bindValue(":classification", classification);
    q.bindValue(":id", id);
    return q.exec();
}

int EmotionsRepository::getIdByName(const QString &name)
{
    if (name.isEmpty() || !m_db.isValid() || !m_db.isOpen())
        return -1;

    QSqlQuery q(m_db);
    q.prepare("SELECT id FROM emotions WHERE LOWER(name)=LOWER(:name)");
    q.bindValue(":name", name);
    if (q.exec() && q.next())
        return q.value(0).toInt();
    return -1;
}

bool EmotionsRepository::createOrUpdateEmotion(const QString &name,
                                               const QString &classification)
{
    if (name.isEmpty()) return false;

    int id = getIdByName(name);
    if (id != -1) {
        return updateEmotion(id, name, classification);
    } else {
        return createEmotion(name, classification) != -1;
    }
}

bool EmotionsRepository::deleteEmotion(int id)
{
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM emotions WHERE id = :id");
    q.bindValue(":id", id);
    return q.exec();
}
