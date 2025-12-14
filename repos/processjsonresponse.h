/*#pragma once
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QDateTime>

#include "repos/consumedrepository.h"
#include "repos/emotionsrepository.h"
#include "repos/exerciseentriesrepository.h"
#include "repos/dailystatisticsrepository.h"
#include "repos/achievementsrepository.h"
#include "repos/usersrepository.h"
#include "repos/productsrepository.h"
#include "repos/recipesrepository.h"
#include "repos/recipeingredientsrepository.h"

class ProcessJsonResponse : public QObject
{
    Q_OBJECT
public:
    explicit ProcessJsonResponse(QObject* parent = nullptr);

    // Entry point: process full JSON payload
    bool process(const QString& json,
                 int userId,
                 ConsumedRepository& consumedRepo,
                 EmotionsRepository& emotionsRepo,
                 ExerciseEntriesRepository& exercisesRepo,
                 DailyStatisticsRepository& statsRepo,
                 ProductsRepository& productsRepo,
                 RecipesRepository& recipesRepo,
                 RecipeIngredientsRepository& recipeIngredientsRepo,
                 AchievementsRepository& achievementsRepo,
                 UsersRepository& usersRepo);

signals:
    void processed(bool success, const QString& message);
    void errorOccurred(const QString& message);

private:
    // per-block handlers
    void handleEmotions(const QJsonArray& arr, EmotionsRepository& emotionsRepo);
    void handleConsumed(const QJsonArray& arr, int userId, const QString& dayISO, const QString& tsISO,
                        ConsumedRepository& consumedRepo);
    void handleExercises(const QJsonArray& arr, int userId, const QString& dayISO, const QString& tsISO,
                         ExerciseEntriesRepository& exercisesRepo);
    void handleProducts(const QJsonArray& arr, ProductsRepository& productsRepo);
    void handleUsers(const QJsonArray& arr, UsersRepository& usersRepo);
    void handleAchievements(const QJsonArray& arr, AchievementsRepository& achievementsRepo);
    void handleRecipes(const QJsonArray& arr, RecipesRepository& recipesRepo, RecipeIngredientsRepository& ingredientsRepo);

    // helpers
    static bool hasKeys(const QJsonObject& o, std::initializer_list<const char*> keys);
    static QString todayISO();
    static QString nowISO();

    // statistics accumulation
    void updateDailyStats(int userId, const QString& dayISO, double waterMl,
                          ConsumedRepository& consumedRepo, DailyStatisticsRepository& statsRepo);
};
*/
