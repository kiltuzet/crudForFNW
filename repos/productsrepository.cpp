#include "productsrepository.h"
ProductsRepository::ProductsRepository(QObject *parent)
    : BaseRepository("products", {"id","name","quantity","unit","proteins","fats","carbs"}, parent) {}

int ProductsRepository::createProduct(QVariantMap values) {
  /*  QVariantMap values;
    values["name"] = name;
    values["quantity"] = quantity;
    values["unit"] = unit;
    values["proteins"] = proteins;
    values["fats"] = fats;
    values["carbs"] = carbs;
*/
return create(values);
}

QVariantMap ProductsRepository::getProduct(int id) { return getById(id); }
QVariantList ProductsRepository::getAllProducts() { return getAll(); }

bool ProductsRepository::updateProduct(int id, const QString &name, double quantity) {
    QVariantMap values;
    values["name"] = name;
    values["quantity"] = quantity;
    return update(id, values);
}

bool ProductsRepository::deleteProduct(int id) { return deleteById(id); }

bool ProductsRepository::createOrUpdateProduct(int id,QVariantMap values) {
  /*  QVariantMap values;
    values["name"] = name;
    values["quantity"] = quantity;
    values["unit"] = unit;
    values["proteins"] = proteins;
    values["fats"] = fats;
    values["carbs"] = carbs;*/
    return createOrUpdate(id, values);
}
