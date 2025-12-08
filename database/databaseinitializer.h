#pragma once
#include <QObject>
#include <QString>
#include "databaseconnection.h"
class DatabaseInitializer : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseInitializer(QObject *parent = nullptr);

    // Execute sqlFilePath into provided db (by name). Returns true if all good.
    bool executeSqlFile(const QString &sqlFilePath, const QSqlDatabase &db);

private:
    QString readSqlFileWithEncodingDetection(const QString &sqlFilePath);
};
