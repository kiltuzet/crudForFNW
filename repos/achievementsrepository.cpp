#include "achievementsrepository.h"

AchievementsRepository::AchievementsRepository(QObject *parent)
    : BaseRepository("achievements", {"id","name","unlocked","description"}, parent) {}

int AchievementsRepository::createAchievement(const QString &name, const QString &description, bool unlocked) {
    QVariantMap values;
    values["name"] = name;
    values["description"] = description;
    values["unlocked"] = unlocked ? 1 : 0;
    return create(values);
}

QVariantMap AchievementsRepository::getAchievement(int id) { return getById(id); }
QVariantList AchievementsRepository::getAllAchievements() { return getAll(); }

bool AchievementsRepository::updateAchievement(int id, const QString &name, const QString &description, bool unlocked) {
    QVariantMap values;
    values["name"] = name;
    values["description"] = description;
    values["unlocked"] = unlocked ? 1 : 0;
    return update(id, values);
}

bool AchievementsRepository::deleteAchievement(int id) { return deleteById(id); }

bool AchievementsRepository::createOrUpdateAchievement(int id, const QString &name, const QString &description, bool unlocked) {
    QVariantMap values;
    values["name"] = name;
    values["description"] = description;
    values["unlocked"] = unlocked ? 1 : 0;
    return createOrUpdate(id, values);
}
