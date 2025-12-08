#include "nutritionconverter.h"
#include <QDebug>

NutritionConverter::NutritionConverter(QObject *parent)
    : QObject(parent)
{
}

NutritionConverter::~NutritionConverter()
{
    closeDatabase();
}

bool NutritionConverter::initDatabase(const QString &dbPath, const QString &sqlFile)
{
    if (!m_dbConn.open(dbPath)) {
        emit errorOccurred(QStringLiteral("Failed to open DB"));
        emit isConnectedChanged();
        return false;
    }

    // Привязываем все репозитории
    QSqlDatabase db = m_dbConn.database();
    m_products.setDatabase(db);
    m_consumed.setDatabase(db);
    m_stats.setDatabase(db);
    m_emotions.setDatabase(db);
    m_exercises.setDatabase(db);
    m_achievements.setDatabase(db);
    m_users.setDatabase(db);
    m_recipes.setDatabase(db);
    m_recipeIngredients.setDatabase(db);

    // Инициализация SQL
    if (!sqlFile.isEmpty()) {
        if (!m_dbInit.executeSqlFile(sqlFile, db)) {
            emit errorOccurred(QStringLiteral("Failed to execute init SQL"));
            emit isConnectedChanged();
            return false;
        }
    }

    emit isConnectedChanged();
    return true;
}

void NutritionConverter::closeDatabase()
{
    m_dbConn.close();
    emit isConnectedChanged();
}
bool NutritionConverter::consumed(int userId,
                                  const QString &date,
                                  const QString &itemName,
                                  double quantity,
                                  const QString &unit,
                                  bool isRecipe)
{
    if (isConnected() || itemName.isEmpty() || quantity <= 0) {
        emit errorOccurred("Invalid consumed entry");
        return false;
    }

    // Получение базовых данных
    double baseQty = 0.0;
    double baseProteins = 0.0, baseFats = 0.0, baseCarbs = 0.0;
    int itemId = -1;
    QString itemUnit;

    if (isRecipe) {
        QVariantMap recipe = m_recipes.getRecipeByName(itemName);
        if (recipe.isEmpty()) { qWarning() << "Recipe not found:" << itemName; return false; }
        itemId       = recipe["id"].toInt();
        baseQty      = recipe["quantity"].toDouble();
        itemUnit     = recipe["unit"].toString();
        baseProteins = recipe["proteins"].toDouble();
        baseFats     = recipe["fats"].toDouble();
        baseCarbs    = recipe["carbs"].toDouble();
    } else {
        QVariantMap product = m_products.getProductByName(itemName);
        if (product.isEmpty()) { qWarning() << "Product not found:" << itemName; return false; }
        itemId       = product["id"].toInt();
        baseQty      = product["quantity"].toDouble();
        itemUnit     = product["unit"].toString();
        baseProteins = product["proteins"].toDouble();
        baseFats     = product["fats"].toDouble();
        baseCarbs    = product["carbs"].toDouble();
    }

    // Рассчитываем нутриенты пропорционально потреблённому количеству
    // Если базовое количество 0 или некорректное, нутриенты считаем от 100 г/мл (как в processConsumed), или защищаемся
    const double divisor = (baseQty > 0.0 ? baseQty : 100.0);
    const double factor = quantity / divisor;
    const double proteins = baseProteins * factor;
    const double fats     = baseFats     * factor;
    const double carbs    = baseCarbs    * factor;
    const double calories = 0;//calculateCalories(proteins, fats, carbs);

    // Всегда пишем запись о потреблении
    const QString ts = QDateTime::currentDateTime().toString(Qt::ISODate);
    m_consumed.createConsumedEntry(userId, date, itemName, quantity, unit.isEmpty() ? itemUnit : unit,
                                    proteins, fats, carbs, calories, ts);

    // Обновление инвентаря и статистики по условиям
    const double newQty = baseQty - quantity;

    // Если newQty < 0: клампим quantity до 0 в таблице и НЕ добавляем записи в статистику
    if (newQty < 0.0) {
        if (isRecipe) {
            m_recipes.updateRecipe(itemId, itemName, 0.0, itemUnit, baseProteins, baseFats, baseCarbs);
        } else {
            m_products.updateProduct(itemId, itemName, 0.0, itemUnit, baseProteins, baseFats, baseCarbs);
        }
        // Пропускаем обновление daily_statistics в этом случае
        emit dataProcessed(true, "Consumed entry processed; inventory clamped to zero; statistics not updated");
        return true;
    }

    // Если newQty >= 0: НЕ обновляем products/recipes, но обновляем статистику
    {
        QVariantMap stats = m_stats.getDailyStatisticsRecord(userId, date);
        const double totalProteins = stats.value("proteins").toDouble() + proteins;
        const double totalFats     = stats.value("fats").toDouble() + fats;
        const double totalCarbs    = stats.value("carbs").toDouble() + carbs;
        const double totalCalories = stats.value("calories").toDouble() + calories;
        const double waterMl       = stats.value("waterMl").toDouble();

        m_stats.createOrUpdateDailyStatistics(userId, date, waterMl,
                                               totalProteins, totalFats, totalCarbs, totalCalories);
    }

    emit dataProcessed(true, "Consumed entry processed; statistics updated");
    return true;
}


// Products delegation
QVariantList NutritionConverter::getAllProducts() { return m_products.getAllProducts(); }
QVariantList NutritionConverter::getAllUsers(){return m_users.getAllUsers();}
QVariantMap NutritionConverter::getProduct(int id) { return m_products.getProduct(id); }
int NutritionConverter::createProduct(const QString &name, double quantity, const QString &unit,
                                      double proteins, double fats, double carbs)
{
    return m_products.createProduct(name, quantity, unit, proteins, fats, carbs);
}
bool NutritionConverter::updateProduct(int id, const QString &name, double quantity, const QString &unit,
                                       double proteins, double fats, double carbs)
{
    return m_products.updateProduct(id, name, quantity, unit, proteins, fats, carbs);
}
bool NutritionConverter::deleteProduct(int id) { return m_products.deleteProduct(id); }

// JSON processing
bool NutritionConverter::isValidJsonObject(const QJsonObject &obj, const QStringList &requiredKeys)
{
    if (obj.isEmpty()) return false; // пустой объект сразу пропускаем

    for (const QString &key : requiredKeys) {
        if (!obj.contains(key)) return false; // нет обязательного поля
        if (obj.value(key).isNull() || obj.value(key).toString().isEmpty()) return false;
    }
    return true;
}

bool NutritionConverter::processJsonResponse(const QString &json, int userId)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) {
        emit dataProcessed(false, "Invalid JSON");
        return false;
    }
    QJsonObject obj = doc.object();

    // Emotions
    QJsonArray emotions = obj["emotions"].toArray();
    for (auto e : emotions) {

        QJsonObject eo = e.toObject();
       // m_emotions.g
        if (eo.isEmpty()) {
            qWarning() << " co Skipping empty consumed entry";
            continue; // просто игнорируем
        }
        m_emotions.createOrUpdateEmotion(eo["name"].toString(), eo["classification"].toString());
    }

    // Consumed
    QJsonArray consumed = obj["consumed"].toArray();
    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    QString ts = QDateTime::currentDateTime().toString(Qt::ISODate);
    for (auto c : consumed) {
        QJsonObject co = c.toObject();
        if (c.toObject().isEmpty()) {
            qWarning() << " co Skipping empty consumed entry";
            continue; // просто игнорируем
        }
        m_consumed.createOrUpdateConsumedEntry(userId, today,
                                       co["name"].toString(),
                                       co["quantity"].toDouble(),
                                       co["unit"].toString(),
                                       co["proteins"].toDouble(),
                                       co["fats"].toDouble(),
                                       co["carbs"].toDouble(),
                                       co["calories"].toDouble(),
                                       ts);
    }

    // Exercises
    QJsonArray exercises = obj["exercises"].toArray();
    for (auto ex : exercises) {
        QJsonObject eo = ex.toObject();
        if (ex.toObject().isEmpty()) {
            qWarning() << "eo Skipping empty consumed entry";
            continue; // просто игнорируем
        }
        m_exercises.createExerciseEntry(userId, today,
                                        eo["name"].toString(),
                                        eo["duration"].toDouble(),
                                        eo["caloriesBurned"].toDouble(),
                                        ts);
    }

    // Water & stats
    double waterMl = obj["waterMl"].toDouble();
    double totalProteins = 0, totalFats = 0, totalCarbs = 0, totalCalories = 0;
    auto items = m_consumed.getConsumedEntriesByUser(userId, today);
    for (auto v : items) {
        QVariantMap m = v.toMap();
        totalProteins += m["proteins"].toDouble();
        totalFats += m["fats"].toDouble();
        totalCarbs += m["carbs"].toDouble();
        totalCalories += m["calories"].toDouble();
    }
    m_stats.createOrUpdateDailyStatistics(userId, today, waterMl,
                                          totalProteins, totalFats,
                                          totalCarbs, totalCalories);
    qDebug()<<"All finished";
    emit dataProcessed(true, "JSON processed successfully");
    return true;
}

// Statistics
QVariantMap NutritionConverter::getDailyStatistics(int userId, const QString &date)
{
    QString d = date.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : date;
    return m_stats.getDailyStatisticsRecord(userId, d);
}

QVariantList NutritionConverter::getConsumedToday(int userId)
{
    QString d = QDate::currentDate().toString("yyyy-MM-dd");
    return m_consumed.getConsumedEntriesByUser(userId, d);
}

double NutritionConverter::getTotalCaloriesToday(int userId)
{
    auto items = getConsumedToday(userId);
    double total = 0;
    for (auto v : items) {
        total += v.toMap()["calories"].toDouble();
    }
    return total;
}

bool NutritionConverter::isConnected() const { return m_dbConn.isOpen(); }
