#include "processjsonresponse.h"
#include <QDebug>

ProcessJsonResponse::ProcessJsonResponse(QObject* parent) : QObject(parent) {}

bool ProcessJsonResponse::process(const QString& json,
                                  int userId,
                                  ConsumedRepository& consumedRepo,
                                  EmotionsRepository& emotionsRepo,
                                  ExerciseEntriesRepository& exercisesRepo,
                                  DailyStatisticsRepository& statsRepo,
                                  ProductsRepository& productsRepo,
                                  RecipesRepository& recipesRepo,
                                  RecipeIngredientsRepository& recipeIngredientsRepo,
                                  AchievementsRepository& achievementsRepo,
                                  UsersRepository& usersRepo)
{
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isObject()) {
        emit errorOccurred(QStringLiteral("Invalid JSON"));
        emit processed(false, QStringLiteral("Invalid JSON"));
        return false;
    }

    const QJsonObject root = doc.object();
    const QString day = todayISO();
    const QString ts = nowISO();

    // emotions
    if (root.contains("emotions") && root["emotions"].isArray())
        handleEmotions(root["emotions"].toArray(), emotionsRepo);

    // consumed
    if (root.contains("consumed") && root["consumed"].isArray())
        handleConsumed(root["consumed"].toArray(), userId, day, ts, consumedRepo);

    // exercises
    if (root.contains("exercises") && root["exercises"].isArray())
        handleExercises(root["exercises"].toArray(), userId, day, ts, exercisesRepo);

    // products (optional block)
    if (root.contains("products") && root["products"].isArray())
        handleProducts(root["products"].toArray(), productsRepo);

    // users (optional block)
    if (root.contains("users") && root["users"].isArray())
        handleUsers(root["users"].toArray(), usersRepo);

    // achievements (optional block)
    if (root.contains("achievements") && root["achievements"].isArray())
        handleAchievements(root["achievements"].toArray(), achievementsRepo);

    // recipes with ingredients (optional)
    if (root.contains("recipes") && root["recipes"].isArray())
        handleRecipes(root["recipes"].toArray(), recipesRepo, recipeIngredientsRepo);

    // water/statistics
    const double waterMl = root["waterMl"].toDouble();
    updateDailyStats(userId, day, waterMl, consumedRepo, statsRepo);

    emit processed(true, QStringLiteral("JSON processed successfully"));
    return true;
}

void ProcessJsonResponse::handleEmotions(const QJsonArray& arr, EmotionsRepository& emotionsRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name", "classification"})) {
            qWarning() << "Skipping invalid emotion entry:" << o;
            continue;
        }
        emotionsRepo.createEmotion(o["name"].toString(), o["classification"].toString());
    }
}

void ProcessJsonResponse::handleConsumed(const QJsonArray& arr, int userId, const QString& dayISO, const QString& tsISO,
                                         ConsumedRepository& consumedRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name", "quantity", "unit"})) {
            qWarning() << "Skipping invalid consumed entry:" << o;
            continue;
        }
        /*consumedRepo.createConsumedEntry(userId, dayISO,
                                         o["name"].toString(),
                                         o["quantity"].toDouble(),
                                         o["unit"].toString(),
                                         o["proteins"].toDouble(),
                                         o["fats"].toDouble(),
                                         o["carbs"].toDouble(),
                                         o["calories"].toDouble(),
                                         tsISO);*/
    }
}

void ProcessJsonResponse::handleExercises(const QJsonArray& arr, int userId, const QString& dayISO, const QString& tsISO,
                                          ExerciseEntriesRepository& exercisesRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name", "duration", "caloriesBurned"})) {
            qWarning() << "Skipping invalid exercise entry:" << o;
            continue;
        }
        exercisesRepo.createExerciseEntry(userId, dayISO,
                                          o["name"].toString(),
                                          o["duration"].toDouble(),
                                          o["caloriesBurned"].toDouble(),
                                          tsISO);
    }
}

void ProcessJsonResponse::handleProducts(const QJsonArray& arr, ProductsRepository& productsRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name", "quantity", "unit"})) {
            qWarning() << "Skipping invalid product entry:" << o;
            continue;
        }
        productsRepo.createProduct( o.toVariantMap());
    }
}

void ProcessJsonResponse::handleUsers(const QJsonArray& arr, UsersRepository& usersRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name"})) {
            qWarning() << "Skipping invalid user entry:" << o;
            continue;
        }
        usersRepo.createUser(o["name"].toString(),
                             o["age"].toInt(),
                             o["gender"].toString(),
                             o["height"].toDouble(),
                             o["weight"].toDouble(),
                             o["goal"].toString(),
                             o["activity_level"].toInt());
    }
}

void ProcessJsonResponse::handleAchievements(const QJsonArray& arr, AchievementsRepository& achievementsRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name"})) {
            qWarning() << "Skipping invalid achievement entry:" << o;
            continue;
        }
        achievementsRepo.createAchievement(o["name"].toString(),
                                           o["description"].toString(),
                                           o["unlocked"].toBool());
    }
}

void ProcessJsonResponse::handleRecipes(const QJsonArray& arr, RecipesRepository& recipesRepo, RecipeIngredientsRepository& ingredientsRepo)
{
    for (const QJsonValue& v : arr) {
        const QJsonObject o = v.toObject();
        if (o.isEmpty() || !hasKeys(o, {"name"})) {
            qWarning() << "Skipping invalid recipe entry:" << o;
            continue;
        }

        int rid = recipesRepo.createRecipe(o["name"].toString(),
                                           o["quantity"].toDouble(),
                                           o["unit"].toString(),
                                           o["proteins"].toDouble(),
                                           o["fats"].toDouble(),
                                           o["carbs"].toDouble());

        const QJsonArray ings = o["ingredients"].toArray();
        for (const QJsonValue& iv : ings) {
            const QJsonObject io = iv.toObject();
            if (io.isEmpty() || !hasKeys(io, {"name"})) {
                qWarning() << "Skipping invalid ingredient entry:" << io;
                continue;
            }
            ingredientsRepo.createOrUpdate(rid,io.toVariantMap());
        }
    }
}

bool ProcessJsonResponse::hasKeys(const QJsonObject& o, std::initializer_list<const char*> keys)
{
    for (const char* k : keys) {
        if (!o.contains(QLatin1String(k))) return false;
    }
    return true;
}

QString ProcessJsonResponse::todayISO()
{
    return QDate::currentDate().toString("yyyy-MM-dd");
}

QString ProcessJsonResponse::nowISO()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

void ProcessJsonResponse::updateDailyStats(int userId, const QString& dayISO, double waterMl,
                                           ConsumedRepository& consumedRepo, DailyStatisticsRepository& statsRepo)
{
    double totalProteins = 0.0, totalFats = 0.0, totalCarbs = 0.0, totalCalories = 0.0;
    const QVariantList items = consumedRepo.getConsumedEntriesByUser(userId, dayISO);
    for (const QVariant& v : items) {
        const QVariantMap m = v.toMap();
        totalProteins += m.value("proteins").toDouble();
        totalFats     += m.value("fats").toDouble();
        totalCarbs    += m.value("carbs").toDouble();
        totalCalories += m.value("calories").toDouble();
    }
   /* statsRepo.createOrUpdateDailyStatistics(userId, dayISO, waterMl,
                                            totalProteins, totalFats, totalCarbs, totalCalories);*/
}

