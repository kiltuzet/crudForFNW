#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class UsersRepository : public QObject
{
    Q_OBJECT
public:
    explicit UsersRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createUser(const QString &name, int age, const QString &gender,
                   double height, double weight, const QString &goal, int activityLevel);
    QVariantMap getUser(int id);
    QVariantList getAllUsers();
    bool updateUser(int id, const QString &name, int age, const QString &gender,
                    double height, double weight, const QString &goal, int activityLevel);
    bool deleteUser(int id);

private:
    QSqlDatabase m_db;
};
