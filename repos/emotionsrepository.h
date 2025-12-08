#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class EmotionsRepository : public QObject
{
    Q_OBJECT
public:
    explicit EmotionsRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createEmotion(const QString &name, const QString &classification);
    QVariantMap getEmotion(int id);
    QVariantList getAllEmotions();
    bool updateEmotion(int id, const QString &name, const QString &classification);
    bool deleteEmotion(int id);
    bool createOrUpdateEmotion(const QString &name,
                               const QString &classification);
    int getIdByName(const QString &name);
private:
    QSqlDatabase m_db;
};
