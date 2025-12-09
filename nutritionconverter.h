#pragma once
#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>

#include "database/databaseconnection.h"
#include "database/databaseinitializer.h"
#include "repos/productsrepository.h"
#include "repos/consumedrepository.h"
#include "repos/dailystatisticsrepository.h"
#include "repos/emotionsrepository.h"
#include "repos/exerciseentriesrepository.h"
#include "repos/achievementsrepository.h"
#include "repos/usersrepository.h"
#include "repos/recipesrepository.h"
#include "repos/recipeingredientsrepository.h"
#include "repos/processjsonresponse.h"

class NutritionConverter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
public:
    explicit NutritionConverter(QObject *parent = nullptr);
    ~NutritionConverter() override;

    Q_INVOKABLE bool initDatabase(const QString &dbPath, const QString &sqlFile = QStringLiteral("bdext.sql"));
    Q_INVOKABLE void closeDatabase();

    // Products API
    Q_INVOKABLE QVariantList getAllProducts();
    Q_INVOKABLE QVariantList getAllUsers();
    Q_INVOKABLE QVariantMap getProduct(int id);
    Q_INVOKABLE int createProduct(const QString &name, double quantity, const QString &unit,
                                  double proteins, double fats, double carbs);
    Q_INVOKABLE bool updateProduct(int id, const QString &name, double quantity, const QString &unit,
                                   double proteins, double fats, double carbs);
    Q_INVOKABLE bool deleteProduct(int id);

    // JSON processing
    Q_INVOKABLE bool processJsonResponse(const QString &json, int userId);
    bool isValidJsonObject(const QJsonObject &obj, const QStringList &requiredKeys);


    // Statistics
    Q_INVOKABLE QVariantMap getDailyStatistics(int userId, const QString &date);
    Q_INVOKABLE QVariantList getConsumedToday(int userId);
    Q_INVOKABLE double getTotalCaloriesToday(int userId);

    Q_INVOKABLE bool consumed(int userId,
                              const QString &date,
                              const QString &itemName,
                              double quantity,
                              const QString &unit,
                              bool isRecipe);


    bool isConnected() const;

signals:
    void isConnectedChanged();
    void errorOccurred(const QString &msg);
    void dataProcessed(bool success, const QString &message);

private:
    DatabaseConnection m_dbConn;
    DatabaseInitializer m_dbInit;

    ProductsRepository m_products;
    ConsumedRepository m_consumed;
    DailyStatisticsRepository m_stats;
    EmotionsRepository m_emotions;
    ExerciseEntriesRepository m_exercises;
    AchievementsRepository m_achievements;
    UsersRepository m_users;
    RecipesRepository m_recipes;
    RecipeIngredientsRepository m_recipeIngredients;
    //ProcessJsonResponse m_jsonProc;
};
