#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>
#include <QString>

class BaseService : public QObject {
    Q_OBJECT
protected:
    QSqlDatabase m_db;

public:
    explicit BaseService(const QSqlDatabase& db, QObject* parent = nullptr);

    // Транзакции
    Q_INVOKABLE bool beginTransaction();
    Q_INVOKABLE bool commitTransaction();
    Q_INVOKABLE bool rollbackTransaction();

    // Логирование
    Q_INVOKABLE void logInfo(const QString& msg);
    Q_INVOKABLE void logError(const QString& msg);

    // Унифицированный ответ для QML
    Q_INVOKABLE QVariantMap makeResponse(bool success,
                                         const QString& message,
                                         const QVariantMap& payload = {});
};
