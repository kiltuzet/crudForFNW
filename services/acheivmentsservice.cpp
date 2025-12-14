#include "acheivmentsservice.h"

AchievementsService::AchievementsService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent), m_repo(new AchievementsRepository(this)) {
    m_repo->setDatabase(db);
}

QVariantMap AchievementsService::addAchievement(const QString& name, const QString& description, bool unlocked) {
    int id = m_repo->createAchievement(name, description, unlocked);
    QVariantMap payload;
    payload["id"] = id;
    payload["name"] = name;
    return makeResponse(id != -1, id != -1 ? "Достижение добавлено" : "Ошибка добавления", payload);
}

QVariantList AchievementsService::getAllAchievements() {
    return m_repo->getAllAchievements();
}
