#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class AchievementsRepository : public QObject
{
    Q_OBJECT
public:
    explicit AchievementsRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createAchievement(const QString &name, const QString &description, bool unlocked = false);
    QVariantMap getAchievement(int id);
    QVariantList getAllAchievements();
    bool updateAchievement(int id, const QString &name, const QString &description, bool unlocked);
    bool deleteAchievement(int id);
    bool createOrUpdateAchievement(int id,const QString &name,const QString &description,bool unlocked);
private:
    QSqlDatabase m_db;
};
