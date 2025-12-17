#include "consumedservice.h"

ConsumedService::ConsumedService(const QSqlDatabase& db, QObject* parent)
    : BaseService(db, parent),
    m_consumed(new ConsumedRepository(this)),
    m_stats(new DailyStatisticsRepository(this)),
    m_recipe(new RecipesRepository(this)),
    m_product(new ProductsRepository(this)){
    m_consumed->setDatabase(db);
    m_stats->setDatabase(db);
    m_recipe->setDatabase(db);
    m_product->setDatabase(db);
}

QVariantMap ConsumedService::addConsumedEntry(QVariantMap values)
{
    if (!beginTransaction())
        return makeResponse(false, "Ошибка транзакции");

    // 1️⃣ dostat' map
    QVariantList list = values["consumed"].toList();
    if (list.isEmpty())
        return makeResponse(false, "Нет потребленных продуктов");
    qDebug()<<"add consumed entry";
    QVariantMap val = list.first().toMap();

    QString name = val["name"].toString();
    double quantity = val["quantity"].toDouble();
    QString unit = val["unit"].toString();

    // 2️⃣ product or recipe search
    QVariantMap product = m_product->getProductByName(name);
    QVariantMap recipe  = m_recipe->getRecipeByName(name);
    qDebug()<<"id's";

    bool isProduct = product["id"].isValid();
    bool isRecipe  = recipe["id"].isValid();
    qDebug()<<"flags is recipe || product";
    qDebug()<<isProduct;
    qDebug()<<isRecipe;
    if (!isProduct && !isRecipe)
    {
        rollbackTransaction();
        return makeResponse(false, "Продукт или рецепт не найден");
    }

    // 3️⃣ calculate nutrition from DB
    double proteins = 0;
    double fats = 0;
    double carbs = 0;
    double calories = 0;

    if (isProduct)
    {
        proteins  = product["proteins"].toDouble() * quantity / 100.0;
        fats      = product["fats"].toDouble()     * quantity / 100.0;
        carbs     = product["carbs"].toDouble()    * quantity / 100.0;
        calories  = product["calories"].toDouble() * quantity / 100.0;
    }
    else
    {
        // рецепт: суммируем его ингредиенты
        auto ingredients = m_recipe->getRecipeByName(name);
        for (auto &i : ingredients)
        {
            QVariantMap ing = i.toMap();
            proteins += ing["proteins"].toDouble() * quantity / 100.0;
            fats     += ing["fats"].toDouble()     * quantity / 100.0;
            carbs    += ing["carbs"].toDouble()    * quantity / 100.0;
            calories += ing["calories"].toDouble() * quantity / 100.0;
        }
    }

    // 4️⃣ insert consumed entry
    QVariantMap insertValues;
    insertValues["user_id"] = values["userId"].toInt();
    insertValues["date"] = values["date"].toString();
    insertValues["name"] = name;
    insertValues["quantity"] = quantity;
    insertValues["unit"] = unit;
    insertValues["proteins"]=proteins;
    insertValues["fats"]=fats;
    insertValues["carbs"]=carbs;
    insertValues["calories"]=calories;
    insertValues["timestamp"] = values["timestamp"].toString();
//QList("id", "user_id", "date", "product_name", "quantity", "unit", "proteins", "fats", "carbs", "calories", "timestamp")
    int id = m_consumed->createConsumedEntry(insertValues);
    if (id == -1)
    {
        rollbackTransaction();
        return makeResponse(false, "Не удалось создать запись");
    }

    // 5️⃣ update stats
    QVariantMap stats = m_stats->getDailyStatisticsRecord(
        values["userId"].toInt(),
        values["date"].toString()
        );

    stats["proteins"]  = stats["proteins"].toDouble()  + proteins;
    stats["fats"]      = stats["fats"].toDouble()      + fats;
    stats["carbs"]     = stats["carbs"].toDouble()     + carbs;
    stats["calories"]  = stats["calories"].toDouble()  + calories;

    bool ok = m_stats->createOrUpdateDailyStatistics(
        values["userId"].toInt(),
        stats
        );

    if (!ok)
    {
        rollbackTransaction();
        return makeResponse(false, "Не удалось обновить статистику");
    }

    commitTransaction();

    QVariantMap payload;
    payload["id"] = id;
    payload["proteins"] = proteins;
    payload["fats"] = fats;
    payload["carbs"] = carbs;
    payload["calories"] = calories;

    return makeResponse(true, "Запись добавлена", payload);
}
