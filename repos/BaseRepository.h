#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>

class BaseRepository : public QObject {
    Q_OBJECT
protected:
    QSqlDatabase m_db;
    QString m_table;
    QStringList m_fields;

public:
    explicit BaseRepository(const QString &table,
                            const QStringList &fields,
                            QObject *parent=nullptr)
        : QObject(parent), m_table(table), m_fields(fields) {}

    void setDatabase(const QSqlDatabase &db) { m_db = db; }

    // CREATE
    int create(const QVariantMap &values);

    // READ
    QVariantMap getById(int id);
    QVariantList getAll();

    // UPDATE
    bool update(int id, const QVariantMap &values);

    // DELETE
    bool deleteById(int id);

    // UPSERT
    bool createOrUpdate(int id, const QVariantMap &values);
};
