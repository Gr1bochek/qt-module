#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlDatabase>

DbManager* DbManager::instance = nullptr;

DbManager* DbManager::getInstance(const QString& path) {
    if(instance == nullptr){
        instance = new DbManager(path);
    }
    return instance;
}

QSqlDatabase DbManager::getDatabase() const {
    return db;
}

DbManager::DbManager(const QString &path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Error: connection with database fail";
    }

    else
    {
        qDebug() << "Database: connection ok";
    }

    createTable();
}

DbManager::~DbManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool DbManager::isOpen() const
{
    return db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE typing_results("
                  "id INTEGER PRIMARY KEY, "
                  "words_per_minute REAL, "
                  "num_errors INTEGER, "
                  "total_words INTEGER, "
                  "time_spent INTEGER);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'typing_results': one might already exist.";
        success = false;
    }
    else
    {
        success = true;
    }

    return success;
}

bool DbManager::addTypingResult(double wordsPerMinute, int numErrors, int totalWords, int timeSpent)
{
    bool success = false;

    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO typing_results (words_per_minute, num_errors, total_words, time_spent) "
                     "VALUES (:wordsPerMinute, :numErrors, :totalWords, :timeSpent)");
    queryAdd.bindValue(":wordsPerMinute", wordsPerMinute);
    queryAdd.bindValue(":numErrors", numErrors);
    queryAdd.bindValue(":totalWords", totalWords);
    queryAdd.bindValue(":timeSpent", timeSpent);

    if (queryAdd.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "add typing result failed: " << queryAdd.lastError();
    }

    return success;
}

void DbManager::printAllTypingResults() const
{
    qDebug() << "Typing results in db:";
    QSqlQuery query("SELECT * FROM typing_results");
    int idWPM = query.record().indexOf("words_per_minute");
    int idNumErrors = query.record().indexOf("num_errors");
    int idTotalWords = query.record().indexOf("total_words");
    int idTimeSpent = query.record().indexOf("time_spent");

    while (query.next())
    {
        double wordsPerMinute = query.value(idWPM).toDouble();
        int numErrors = query.value(idNumErrors).toInt();
        int totalWords = query.value(idTotalWords).toInt();
        int timeSpent = query.value(idTimeSpent).toInt();

        qDebug() << "=== Words Per Minute:" << wordsPerMinute
                 << "Num Errors:" << numErrors
                 << "Total Words:" << totalWords
                 << "Time Spent:" << timeSpent;
    }
}

bool DbManager::removeAllTypingResults() const
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM typing_results");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all typing results failed: " << removeQuery.lastError();
    }

    return success;
}
