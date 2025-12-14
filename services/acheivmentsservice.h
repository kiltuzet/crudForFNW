#pragma once
#include "BaseService.h"
#include "repos/achievementsrepository.h"

class AchievementsService : public BaseService {
    Q_OBJECT
public:
    explicit AchievementsService(const QSqlDatabase& db, QObject* parent = nullptr);

    Q_INVOKABLE QVariantMap addAchievement(const QString& name, const QString& description, bool unlocked);

    Q_INVOKABLE QVariantList getAllAchievements();

private:
    AchievementsRepository* m_repo;
};
