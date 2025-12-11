/*#pragma once
#include "BaseRepository.h"

class ProductsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ProductsRepository(QObject *parent=nullptr);

    int createProduct(const QString &name, double quantity, const QString &unit,
                      double proteins, double fats, double carbs);
    QVariantMap getProduct(int id);
    QVariantList getAllProducts();
    bool updateProduct(int id, const QString &name, double quantity);
    bool deleteProduct(int id);
    bool createOrUpdateProduct(int id, const QString &name, double quantity, const QString &unit,
                               double proteins, double fats, double carbs);
};
*/
#pragma once
#include "BaseRepository.h"

class ProductsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ProductsRepository(QObject *parent = nullptr);

    // Специфичные методы
    QVariantMap getProductByName(const QString &name);
};
