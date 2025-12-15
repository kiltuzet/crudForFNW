#pragma once
#include "BaseRepository.h"

class ProductsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ProductsRepository(QObject *parent = nullptr);

    // Специфичные методы
    QVariantMap getProductByName(const QString &name);
    bool createOrUpdateProduct(int id,QVariantMap values);
    int createProduct(QVariantMap value);
    bool updateProduct(int id,const QString& value, double quantity);
    QVariantMap getProduct(int id);
    QVariantList getAllProducts();
    bool deleteProduct(int id);
};
