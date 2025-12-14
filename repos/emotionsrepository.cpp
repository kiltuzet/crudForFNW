#include "emotionsrepository.h"

EmotionsRepository::EmotionsRepository(QObject *parent)
    : BaseRepository("emotions", {"id","name","classification"}, parent) {}

int EmotionsRepository::createEmotion(const QString &name, const QString &classification) {
    QVariantMap values;
    values["name"] = name;
    values["classification"] = classification;
    return create(values);
}

QVariantMap EmotionsRepository::getEmotionByName(const QString &name) {
    return getByField("name", name);
}
