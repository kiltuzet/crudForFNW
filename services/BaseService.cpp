#include "BaseService.h"
#include <QDebug>

BaseService::BaseService(const QSqlDatabase& db, QObject* parent)
    : QObject(parent), m_db(db) {}

bool BaseService::beginTransaction() {
    if (!m_db.isOpen()) {
        qWarning() << "[BaseService] Database is not open!";
        return false;
    }
    return m_db.transaction();
}

bool BaseService::commitTransaction() {
    return m_db.commit();
}

bool BaseService::rollbackTransaction() {
    return m_db.rollback();
}

void BaseService::logInfo(const QString& msg) {
    qDebug() << "[Service]" << msg;
}

void BaseService::logError(const QString& msg) {
    qWarning() << "[Service ERROR]" << msg;
}

QVariantMap BaseService::makeResponse(bool success,
                                      const QString& message,
                                      const QVariantMap& payload) {
    QVariantMap response;
    response["success"] = success;
    response["message"] = message;
    response["data"]    = payload;
    return response;
}
