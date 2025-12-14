#include "databaseconnection.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlDriver>

DatabaseConnection::DatabaseConnection(QObject *parent)
    : QObject(parent),
    m_connectionName(QStringLiteral("app_connection"))
{
}

DatabaseConnection::~DatabaseConnection()
{
    close();
}

bool DatabaseConnection::open(const QString &databasePath)
{
    if (m_db.isValid() && m_db.isOpen()) {
        if (m_db.databaseName() == databasePath) return true;
        close();
    }

    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_db.setDatabaseName(databasePath);

    if (!m_db.open()) {
        qCritical() << "DatabaseConnection::open failed:" << m_db.lastError().text();
        return false;
    }

    // Pragmas
    QSqlQuery q(m_db);
    q.exec("PRAGMA foreign_keys = ON;");
    q.exec("PRAGMA journal_mode = WAL;");
    q.exec("PRAGMA synchronous = NORMAL;");

    qDebug() << "Database opened:" << databasePath << "driver:" << m_db.driverName();
    return true;
}

void DatabaseConnection::close()
{
    if (!m_db.isValid()) return;
    QString name = m_db.connectionName();
    m_db.close();
    QSqlDatabase::removeDatabase(name);
    m_db = QSqlDatabase();
}

bool DatabaseConnection::isOpen() const
{
    return m_db.isValid() && m_db.isOpen();
}

QSqlDatabase DatabaseConnection::database() const
{
    return m_db;
}

QString DatabaseConnection::connectionName() const
{
    return m_connectionName;
}
