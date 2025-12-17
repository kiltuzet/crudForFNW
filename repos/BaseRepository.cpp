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
    //отладка
    qDebug()<<QString("INSERT INTO %1 (%2) VALUES (%3)")
                    .arg(m_table, cols.join(", "), params.join(", "));
    for (auto it = values.begin(); it != values.end(); ++it)
        q.bindValue(":" + it.key(), it.value());
    qDebug()<<QString(q.executedQuery());
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

bool BaseRepository::insertWithKeys(const QString &tableName,
                                    const QVariantMap &values) {
    QStringList columns = values.keys();
    QStringList placeholders;
    for (const auto &col : std::as_const(columns)) {
        placeholders << ":" + col;
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                      .arg(tableName, columns.join(", "), placeholders.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sql);
    for (const auto &col : std::as_const(columns)) {
        query.bindValue(":" + col, values.value(col));
    }
    return query.exec();
}

int BaseRepository::getIdByField(const QString &tableName,
                                const QString &name) {
    QSqlQuery query(m_db);
    query.prepare(QString("SELECT id FROM %1 WHERE name = :name LIMIT 1")
                      .arg(tableName));
    query.bindValue(":name", name);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool BaseRepository::deleteById(int id) {
    if (!m_db.isValid() || !m_db.isOpen()) return false;
    QSqlQuery q(m_db);
    q.prepare(QString("DELETE FROM %1 WHERE id=:id").arg(m_table));
    q.bindValue(":id", id);
    return q.exec();
}

QVariantMap BaseRepository::getByField(const QString &fieldName, const QVariant &value)
{
    QVariantMap result;
    if (!m_db.isValid() || !m_db.isOpen()) {
        qWarning() << "[BaseRepository] Database is not open or invalid";
        return result;
    }

    // Формируем SQL-запрос
    QString sql = QString("SELECT %1 FROM %2 WHERE %3 = :val LIMIT 1")
                      .arg(m_fields.join(", "))
                      .arg(m_table)
                      .arg(fieldName);

    QSqlQuery query(m_db);
    query.prepare(sql);
    query.bindValue(":val", value);

    if (!query.exec()) {
        qWarning() << "[BaseRepository] getByField failed:" << query.lastError().text();
        return result;
    }

    if (query.next()) {
        for (int i = 0; i < m_fields.size(); ++i) {
            result[m_fields[i]] = query.value(i);
        }
    }

    return result;
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
