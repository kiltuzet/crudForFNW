#pragma once
#include "BaseRepository.h"

class ProductsRepository : public BaseRepository {
    Q_OBJECT
public:
    explicit ProductsRepository(QObject *parent = nullptr);

    // Специфичные методы
    QVariantMap getProductByName(const QString &name);
};
