#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QString>

class BaseService : public QObject {
    Q_OBJECT
public:
    explicit BaseService(QSqlDatabase db, QObject* parent = nullptr);

protected:
    QSqlDatabase m_db;

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    void logInfo(const QString& msg);
    void logError(const QString& msg);
};
