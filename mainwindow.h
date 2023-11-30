#ifndef TYPE_IT_H
#define TYPE_IT_H

#include <QMainWindow>
#include "dbmanager.h"

enum TimeSelectionEnum {
    ONE_MIN = 60,
    TREE_MIN = 180,
    FIVE_MIN = 300,
    UNLIMITED
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_upload_clicked();
    void on_getRandom_clicked();
    void on_start_clicked();
    void on_giveUp_clicked();
    void updateTimerLabel();
    void checkTimeIsOut();
    void on_pushButton_clicked();

private:
    int checkErrors();

private:
    Ui::MainWindow *ui;
    DbManager* db;
    QTimer *timer;
    int seconds;
    bool textLoaded;
    bool isStarted;
    static QString dbPath;
};

#endif // TYPE_IT_H
