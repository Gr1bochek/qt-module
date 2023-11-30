#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager
{
public:
    ~DbManager();

    bool isOpen() const;

    bool createTable();

    bool addTypingResult(double wordsPerMinute, int numErrors, int totalWords, int timeSpent);

    void printAllTypingResults() const;

    bool removeAllTypingResults() const;

    static DbManager* getInstance(const QString& path);

    QSqlDatabase getDatabase() const;

private:
    DbManager(const QString& path);

private:
    static DbManager* instance;
    QSqlDatabase db;
};

#endif // DBMANAGER_H
