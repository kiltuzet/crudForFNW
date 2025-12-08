#pragma once
#include <QObject>
#include <QSqlDatabase>

class DatabaseConnection : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConnection(QObject *parent = nullptr);
    ~DatabaseConnection() override;

    bool open(const QString &databasePath);
    void close();
    bool isOpen() const;
    QSqlDatabase database() const;

    // Useful convenience
    QString connectionName() const;

private:
    QSqlDatabase m_db;
    QString m_connectionName;
};
