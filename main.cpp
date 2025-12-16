#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "database/databaseconnection.h"
#include "database/databaseinitializer.h"
#include "services/emotionsservice.h"
#include "services/consumedservice.h"
#include "services/exerciseservice.h"
#include "services/dailystatisticsservice.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DatabaseConnection dbConn;
    dbConn.open("nutrition.db");

    DatabaseInitializer initializer(dbConn.database(), "nutrition.db");
    initializer.initializeIfNotExists(":/bdext.sql");
    if(initializer.isDatabaseEmpty()){
        initializer.executeSqlFile(":/bdext.sql",dbConn.database());
    }
    EmotionsService emotions(dbConn.database());
    ConsumedService consumed(dbConn.database());
    ExerciseService exercise(dbConn.database());
    DailyStatisticsService stats(dbConn.database());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("EmotionsService", &emotions);
    engine.rootContext()->setContextProperty("ConsumedService", &consumed);
    engine.rootContext()->setContextProperty("ExerciseService", &exercise);
    engine.rootContext()->setContextProperty("DailyStatisticsService", &stats);

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty()) return -1;

    return app.exec();
}
