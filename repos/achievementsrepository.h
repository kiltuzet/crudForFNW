#pragma once
#include "BaseRepository.h"

class AchievementsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit AchievementsRepository(QObject *parent=nullptr);

    int createAchievement(const QString &name, const QString &description, bool unlocked);
    QVariantMap getAchievement(int id);
    QVariantList getAllAchievements();
    bool updateAchievement(int id, const QString &name, const QString &description, bool unlocked);
    bool deleteAchievement(int id);
    bool createOrUpdateAchievement(int id, const QString &name, const QString &description, bool unlocked);
};
