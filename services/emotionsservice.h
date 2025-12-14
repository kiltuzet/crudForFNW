#pragma once
#include "BaseService.h"
#include "repos/emotionsrepository.h"

class EmotionsService : public BaseService {
    Q_OBJECT
public:
    explicit EmotionsService(const QSqlDatabase& db, QObject* parent = nullptr);

    Q_INVOKABLE QVariantMap addEmotion(const QString& name, const QString& classification);

    Q_INVOKABLE QVariantMap getEmotionByName(const QString& name);

private:
    EmotionsRepository* m_repo;
};
