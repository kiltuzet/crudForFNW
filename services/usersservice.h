#pragma once
#include "BaseService.h"
#include "repos/usersrepository.h"

class UsersService : public BaseService {
    Q_OBJECT
public:
    explicit UsersService(const QSqlDatabase& db, QObject* parent = nullptr);

    Q_INVOKABLE QVariantMap createUser(const QString& name, int age, const QString& gender,
                                       double height, double weight, const QString& goal, int activityLevel);

    Q_INVOKABLE QVariantMap getUser(int id);

    Q_INVOKABLE QVariantList getAllUsers();

    Q_INVOKABLE QVariantMap updateUser(int id, const QString& name, double weight);

    Q_INVOKABLE QVariantMap deleteUser(int id);

private:
    UsersRepository* m_repo;
};
