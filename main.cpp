#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>

#include "database/databaseconnection.h"
#include "database/databaseinitializer.h"

#include "services/emotionsservice.h"
#include "services/consumedservice.h"
#include "services/exerciseservice.h"
#include "services/dailystatisticsservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const QString dbPath =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
        + "/app.db";

    // 🔹 подключение
    DatabaseConnection dbConnection();
    QSqlDatabase db = dbConnection.database();

    if (!db.isOpen()) {
        qFatal("Failed to open database");
      //  return -1;
    }

    // 🔹 инициализация ТОЛЬКО при отсутствии файла
    DatabaseInitializer initializer(db, dbPath);
    if (!initializer.initializeIfNotExists(":/sql/schema.sql")) {
        qFatal("Database initialization failed");
        return -1;
    }

    // 3️⃣ Создание сервисов (КЛЮЧЕВО)
    EmotionsService emotionsService(db);
    ConsumedService consumedService(db);
    ExerciseService exerciseService(db);
    DailyStatisticsService dailyStatisticsService(db);

    QQmlApplicationEngine engine;

    // 4️⃣ Проброс сервисов в QML
    engine.rootContext()->setContextProperty(
        "EmotionsService", &emotionsService);
    engine.rootContext()->setContextProperty(
        "ConsumedService", &consumedService);
    engine.rootContext()->setContextProperty(
        "ExerciseService", &exerciseService);
    engine.rootContext()->setContextProperty(
        "DailyStatisticsService", &dailyStatisticsService);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
