#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "nutritionconverter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
//ИМЕНА СДЕЛАТЬ UNIQUE
    // Регистрируем тип для использования в QML
    qmlRegisterType<NutritionConverter>("com.nutrition.converter", 1, 0, "NutritionConverter");

    // Или создаем экземпляр и передаем в контекст
    NutritionConverter converter;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("nutritionConverter", &converter);

    engine.loadFromModule("untitled1", "Main");

    return app.exec();
}
