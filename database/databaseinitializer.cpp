#include "databaseinitializer.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QStringDecoder>
#else
#include <QTextCodec>
#endif

DatabaseInitializer::DatabaseInitializer(const QSqlDatabase& db,
                                         const QString& dbFilePath)
    : m_db(db)
    , m_dbFilePath(dbFilePath)
{
}

bool DatabaseInitializer::initializeIfNotExists(const QString& sqlFilePath)
{
    // 🔹 Если файл БД уже есть — ничего не делаем
    if (QFile::exists(m_dbFilePath)) {
        qDebug() << "Database already exists:" << m_dbFilePath;
        return true;
    }

    qDebug() << "Database not found, creating:" << m_dbFilePath;

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    // 🔹 Создание структуры через SQL
    if (!executeSqlFile(sqlFilePath, m_db)) {
        qCritical() << "Failed to execute schema SQL";
        return false;
    }

    qDebug() << "Database successfully initialized";
    return true;
}
QString DatabaseInitializer::readSqlFileWithEncodingDetection(const QString &sqlFilePath)
{
    QFile file(sqlFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open sql file:" << sqlFilePath;
        return {};
    }
    QByteArray raw = file.readAll();
    file.close();

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QStringDecoder dUtf8(QStringDecoder::Encoding::Utf8);
    QString s = dUtf8(raw);
    if (!s.contains(QChar::ReplacementCharacter)) return s;
    QStringDecoder dCp1251(QStringDecoder::Encoding::System);
    return dCp1251(raw);
#else
    auto codecUtf8 = QTextCodec::codecForName("UTF-8");
    QString s = codecUtf8->toUnicode(raw);
    if (!s.contains(QChar::ReplacementCharacter)) return s;
    auto codecWin = QTextCodec::codecForName("Windows-1251");
    if (codecWin) return codecWin->toUnicode(raw);
    return QString::fromLocal8Bit(raw);
#endif
}

bool DatabaseInitializer::executeSqlFile(const QString &sqlFilePath, const QSqlDatabase &db)
{
    if (!db.isValid() || !db.isOpen()) {
        qWarning() << "executeSqlFile: db not open";
        return false;
    }

    QString sqlAll = readSqlFileWithEncodingDetection(sqlFilePath);
    if (sqlAll.isEmpty()) {
        qWarning() << "executeSqlFile: sql file empty or unreadable";
        return false;
    }

    // Run in transaction
    QSqlQuery q(db);
    if (!q.exec("BEGIN;")) {
        qWarning() << "BEGIN failed:" << q.lastError().text();
        return false;
    }

    QStringList stmts = sqlAll.split(';', Qt::SkipEmptyParts);
    bool ok = true;
    for (QString piece : stmts) {
        QString stmt = piece.trimmed();
        if (stmt.isEmpty()) continue;
        if (!q.exec(stmt)) {
            qWarning() << "SQL error:" << q.lastError().text() << "stmt:" << stmt.left(200);
            ok = false;
            // continue trying others
        }
    }

    if (ok) {
        if (!q.exec("COMMIT;")) {
            qWarning() << "COMMIT failed:" << q.lastError().text();
            q.exec("ROLLBACK;");
            return false;
        }
    } else {
        q.exec("ROLLBACK;");
    }

    return ok;
}
