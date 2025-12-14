#include "productservice.h"

ProductsService::ProductsService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent), m_repo(new ProductsRepository(this)) {
    m_repo->setDatabase(db);
}

QVariantMap ProductsService::createProduct(const QString& name, double quantity,
                                           const QString& unit,
                                           double proteins, double fats, double carbs) {
    int id = m_repo->createProduct(name, quantity, unit, proteins, fats, carbs);

    QVariantMap payload;
    payload["id"] = id;
    payload["name"] = name;

    return makeResponse(id != -1,
                        id != -1 ? "Продукт создан" : "Ошибка создания продукта",
                        payload);
}

QVariantMap ProductsService::getProduct(int id) {
    QVariantMap product = m_repo->getProduct(id);
    return makeResponse(!product.isEmpty(),
                        product.isEmpty() ? "Продукт не найден" : "ОК",
                        product);
}

QVariantList ProductsService::getAllProducts() {
    return m_repo->getAll();
}
