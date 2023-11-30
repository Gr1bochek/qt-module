#include "result.h"
#include "ui_result.h"

Result::Result(int time, int count, float speed, int errors, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Result)
{    
    ui->setupUi(this);
    QString text = QString("SPEED: %1 words/min\n\nTOTAL WORD COUNT: %2 \n\nERROR COUNT: %3 \n\nTIME TAKEN: %4 sec")
                       .arg(speed * 60).arg(count).arg(errors).arg(time);
    ui->textBrowser->setText(text);
}

Result::~Result()
{
    delete ui;
}

void Result::on_pushButton_3_clicked()
{
    qApp->quit();
}
