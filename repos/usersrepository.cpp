#include "UsersRepository.h"

UsersRepository::UsersRepository(QObject *parent)
    : BaseRepository("users", {"id","name","age","gender","height","weight","goal","activity_level"}, parent) {}

int UsersRepository::createUser(const QString &name, int age, const QString &gender,
                                double height, double weight, const QString &goal, int activityLevel) {
    QVariantMap values;
    values["name"] = name;
    values["age"] = age;
    values["gender"] = gender;
    values["height"] = height;
    values["weight"] = weight;
    values["goal"] = goal;
    values["activity_level"] = activityLevel;
    return create(values);
}

QVariantMap UsersRepository::getUser(int id) { return getById(id); }
QVariantList UsersRepository::getAllUsers() { return getAll(); }

bool UsersRepository::updateUser(int id, const QString &name, double weight) {
    QVariantMap values;
    values["name"] = name;
    values["weight"] = weight;
    return update(id, values);
}

bool UsersRepository::deleteUser(int id) { return deleteById(id); }

bool UsersRepository::createOrUpdateUser(int id, const QString &name, int age, const QString &gender,
                                         double height, double weight, const QString &goal, int activityLevel) {
    QVariantMap values;
    values["name"] = name;
    values["age"] = age;
    values["gender"] = gender;
    values["height"] = height;
    values["weight"] = weight;
    values["goal"] = goal;
    values["activity_level"] = activityLevel;
    return createOrUpdate(id, values);
}
