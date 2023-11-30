#ifndef RESULT_H
#define RESULT_H

#include <QDialog>

namespace Ui {
class Result;
}

class Result : public QDialog
{
    Q_OBJECT

public:
    explicit Result(int time, int count, float speed, int errors, QWidget *parent = 0);
    ~Result();
    
private slots:
    void on_pushButton_3_clicked();

private:
    Ui::Result *ui;
};

#endif // RESULT_H
