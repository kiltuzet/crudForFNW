#pragma once
#include <QObject>
#include <QVariantList>
#include <QSqlDatabase>

class ProductsRepository : public QObject
{
    Q_OBJECT
public:
    explicit ProductsRepository(QObject *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);

    int createProduct(const QString &name, double quantity, const QString &unit,
                      double proteins, double fats, double carbs);
    QVariantMap getProduct(int id);
    QVariantMap getProductByName(const QString &name);
    QVariantList getAllProducts();
    bool updateProduct(int id, const QString &name, double quantity, const QString &unit,
                       double proteins, double fats, double carbs);
    bool deleteProduct(int id);
    bool createOrUpdateProduct(const QString &name,
                               double quantity,
                               const QString &unit,
                               double proteins,
                               double fats,
                               double carbs);
    int getIdByName(const QString &name);
private:
    QSqlDatabase m_db;
};
