#include "EmotionsService.h"

EmotionsService::EmotionsService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent), m_repo(new EmotionsRepository(this)) {
    m_repo->setDatabase(db);
}

QVariantMap EmotionsService::addEmotion(const QString& name, const QString& classification) {
    int id = m_repo->createEmotion(name, classification);
    QVariantMap payload;
    payload["id"] = id;
    payload["name"] = name;
    return makeResponse(id != -1, id != -1 ? "Эмоция добавлена" : "Ошибка добавления", payload);
}

QVariantMap EmotionsService::getEmotionByName(const QString& name) {
    QVariantMap emotion = m_repo->getEmotionByName(name);
    return makeResponse(!emotion.isEmpty(), emotion.isEmpty() ? "Не найдена" : "ОК", emotion);
}
