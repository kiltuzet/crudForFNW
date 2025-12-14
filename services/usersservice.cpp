#include "UsersService.h"

UsersService::UsersService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent), m_repo(new UsersRepository(this)) {
    m_repo->setDatabase(db);
}

QVariantMap UsersService::createUser(const QString& name, int age, const QString& gender,
                                     double height, double weight, const QString& goal, int activityLevel) {
    int id = m_repo->createUser(name, age, gender, height, weight, goal, activityLevel);
    QVariantMap payload;
    payload["id"] = id;
    payload["name"] = name;
    return makeResponse(id != -1, id != -1 ? "Пользователь создан" : "Ошибка создания", payload);
}

QVariantMap UsersService::getUser(int id) {
    QVariantMap user = m_repo->getUser(id);
    return makeResponse(!user.isEmpty(), user.isEmpty() ? "Не найден" : "ОК", user);
}

QVariantList UsersService::getAllUsers() {
    return m_repo->getAllUsers();
}

QVariantMap UsersService::updateUser(int id, const QString& name, double weight) {
    bool ok = m_repo->updateUser(id, name, weight);
    QVariantMap payload;
    payload["id"] = id;
    payload["name"] = name;
    payload["weight"] = weight;
    return makeResponse(ok, ok ? "Обновлено" : "Ошибка обновления", payload);
}

QVariantMap UsersService::deleteUser(int id) {
    bool ok = m_repo->deleteUser(id);
    return makeResponse(ok, ok ? "Удалено" : "Ошибка удаления");
}
