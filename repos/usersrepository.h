#pragma once
#include "BaseRepository.h"

class UsersRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit UsersRepository(QObject *parent=nullptr);

    int createUser(const QString &name, int age, const QString &gender,
                   double height, double weight, const QString &goal, int activityLevel);
    QVariantMap getUser(int id);
    QVariantList getAllUsers();
    bool updateUser(int id, const QString &name, double weight);
    bool deleteUser(int id);
    bool createOrUpdateUser(int id, const QString &name, int age, const QString &gender,
                            double height, double weight, const QString &goal, int activityLevel);
};
