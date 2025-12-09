#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class ConsumedRepository : public QObject
{
    Q_OBJECT
public:
    explicit ConsumedRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createConsumedEntry(int userId, const QString &date, const QString &productName,
                            double quantity, const QString &unit, double proteins,
                            double fats, double carbs, double calories, const QString &timestamp);
    QVariantMap getConsumedEntry(int id);
    QVariantList getConsumedEntriesByUser(int userId, const QString &date);
    QVariantMap getConsumedEntry(int id, const QString &date);
    bool updateConsumedEntry(int id, const QString &productName, double quantity,
                             const QString &unit, double proteins, double fats,
                             double carbs, double calories);
    bool deleteConsumedEntry(int id);
    bool createOrUpdateConsumedEntry(int userId,
                                     const QString &date,const QString &productName,
                                     double quantity,const QString &unit, double proteins,
                                     double fats, double carbs, double calories,
                                     const QString &timestamp);
    int getIdByName(const QString &productName);
private:
    QSqlDatabase m_db;
};
