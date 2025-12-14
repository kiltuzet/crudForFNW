#pragma once
#include "BaseService.h"
#include "repos/productsrepository.h"

class ProductsService : public BaseService {
    Q_OBJECT
public:
    explicit ProductsService(const QSqlDatabase& db, QObject* parent = nullptr);

    // Создание продукта
    Q_INVOKABLE QVariantMap createProduct(const QString& name, double quantity,
                                          const QString& unit,
                                          double proteins, double fats, double carbs);

    // Получение продукта по id
    Q_INVOKABLE QVariantMap getProduct(int id);

    // Получение всех продуктов
    Q_INVOKABLE QVariantList getAllProducts();

private:
    ProductsRepository* m_repo;
};
