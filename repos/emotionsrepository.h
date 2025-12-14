#pragma once
#include "BaseRepository.h"

class EmotionsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit EmotionsRepository(QObject *parent = nullptr);

    // Специфичные методы
    int createEmotion(const QString &name, const QString &classification);
    QVariantMap getEmotionByName(const QString &name);
};
