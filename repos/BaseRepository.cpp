#include "BaseRepository.h"

int BaseRepository::create(const QVariantMap &values) {
    if (!m_db.isValid() || !m_db.isOpen()) return -1;

    QStringList cols, params;
    for (auto it = values.begin(); it != values.end(); ++it) {
        cols << it.key();
        params << ":" + it.key();
    }

    QSqlQuery q(m_db);
    q.prepare(QString("INSERT INTO %1 (%2) VALUES (%3)")
                  .arg(m_table, cols.join(", "), params.join(", ")));

    for (auto it = values.begin(); it != values.end(); ++it)
        q.bindValue(":" + it.key(), it.value());

    if (!q.exec()) {
        qWarning() << "Insert failed:" << q.lastError().text();
        return -1;
    }
    return q.lastInsertId().toInt();
}

QVariantMap BaseRepository::getById(int id) {
    QVariantMap r;
    if (!m_db.isValid() || !m_db.isOpen()) return r;

    QSqlQuery q(m_db);
    q.prepare(QString("SELECT %1 FROM %2 WHERE id=:id")
                  .arg(m_fields.join(", "), m_table));
    q.bindValue(":id", id);

    if (q.exec() && q.next()) {
        for (int i=0; i<m_fields.size(); ++i)
            r[m_fields[i]] = q.value(i);
    }
    return r;
}

QVariantList BaseRepository::getAll() {
    QVariantList out;
    if (!m_db.isValid() || !m_db.isOpen()) return out;

    QSqlQuery q(m_db);
    if (q.exec(QString("SELECT %1 FROM %2 ORDER BY id ASC")
                   .arg(m_fields.join(", "), m_table))) {
        while (q.next()) {
            QVariantMap item;
            for (int i=0; i<m_fields.size(); ++i)
                item[m_fields[i]] = q.value(i);
            out.append(item);
        }
    }
    return out;
}

bool BaseRepository::update(int id, const QVariantMap &values) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;

    QStringList sets;
    for (auto it = values.begin(); it != values.end(); ++it)
        sets << QString("%1=:%1").arg(it.key());

    QSqlQuery q(m_db);
    q.prepare(QString("UPDATE %1 SET %2 WHERE id=:id")
                  .arg(m_table, sets.join(", ")));

    for (auto it = values.begin(); it != values.end(); ++it)
        q.bindValue(":" + it.key(), it.value());
    q.bindValue(":id", id);

    return q.exec();
}

bool BaseRepository::deleteById(int id) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare(QString("DELETE FROM %1 WHERE id=:id").arg(m_table));
    q.bindValue(":id", id);
    return q.exec();
}

bool BaseRepository::createOrUpdate(int id, const QVariantMap &values) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;

    if (update(id, values)) {
        QSqlQuery check(m_db);
        check.exec("SELECT changes()");
        if (check.next() && check.value(0).toInt() > 0)
            return true;
    }

    QVariantMap vals = values;
    vals["id"] = id;
    return create(vals) != -1;
}
