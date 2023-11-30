#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTime>
#include <QtCore>
#include "ui_mainwindow.h"
#include "result.h"
#include "mainwindow.h"
#include "tableviewwindow.h"

QString MainWindow::dbPath = "typing.db";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), db(DbManager::getInstance(dbPath)) ,timer(new QTimer(this)),
    seconds(0), textLoaded(false), isStarted(false)
{
    ui->setupUi(this);
    ui->comboBox->addItem("1 min");
    ui->comboBox->setItemData(0, TimeSelectionEnum::ONE_MIN, Qt::UserRole);
    ui->comboBox->addItem("3 min");
    ui->comboBox->setItemData(1, TimeSelectionEnum::TREE_MIN, Qt::UserRole);
    ui->comboBox->addItem("5 min");
    ui->comboBox->setItemData(2, TimeSelectionEnum::FIVE_MIN, Qt::UserRole);
    ui->comboBox->addItem("Unlimited");
    ui->comboBox->setItemData(3, TimeSelectionEnum::UNLIMITED, Qt::UserRole);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimerLabel()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTimeIsOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimerLabel() {
    seconds++;

    int minutes = seconds / 60;
    int remainingSeconds = seconds % 60;

    ui->textTimeLbl->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(remainingSeconds, 2, 10, QLatin1Char('0')));
}

void MainWindow::checkTimeIsOut() {
    int currentIndex = ui->comboBox->currentIndex();
    TimeSelectionEnum selectedValue = static_cast<TimeSelectionEnum>(ui->comboBox->itemData(currentIndex, Qt::UserRole).toInt());
    switch (selectedValue) {
    case ONE_MIN:
        if(seconds == ONE_MIN) emit on_giveUp_clicked();
        break;
    case TREE_MIN:
        if(seconds == TREE_MIN) emit on_giveUp_clicked();
        break;
    case FIVE_MIN:
        if(seconds == FIVE_MIN) emit on_giveUp_clicked();
        break;
    case UNLIMITED:
        break;
    default:
        break;
    }
}

void MainWindow::on_upload_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", "(*.txt)");
    if (filename.isEmpty()) return;

    QFile inputFile(filename);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, "Error", inputFile.errorString());
    }

    QTextStream in(&inputFile);
    ui->textBrowser->setText(in.readAll());
    inputFile.close();

    textLoaded = true;
}

int MainWindow::checkErrors()
{
    int error = 0;
    QString main = ui->textBrowser->toPlainText();
    QString sub = ui->textEdit->toPlainText();
    QStringList main_list = main.split(' ', QString::SkipEmptyParts);
    QStringList sub_list = sub.split(' ', QString::SkipEmptyParts);
    
    for(int i = 0; i < sub_list.length(); i++)
    {
        if(main_list[i] != sub_list[i])
        {
            error++;
        }
    }
    
    return error;
}

void MainWindow::on_getRandom_clicked()
{
    QFile inputFile(":/sample.txt");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    ui->textBrowser->setText(in.readAll());
    inputFile.close();
    textLoaded = true;
}

void MainWindow::on_start_clicked()
{
    if (!textLoaded)
    {
        QMessageBox::information(this, "Error", "Please load the text first.");
        return;
    } else if(isStarted) return;

    ui->getRandom->setEnabled(false);
    ui->upload->setEnabled(false);

    isStarted = true;
    timer->start(1000);
    ui->start->setText("STARTED");
}

void MainWindow::on_giveUp_clicked()
{
    if (!isStarted) return;
    timer->stop();

    ui->giveUp->setText("STOP");
    ui->start->setText("START");

    int time = seconds;
    int errors = checkErrors();

    QString text = ui->textEdit->toPlainText();
    QStringList text_list = text.split(" ", QString::SkipEmptyParts);

    int count = text_list.count();
    float speed = (count-errors)*1.0/time ;

    seconds = 0;
    isStarted = false;

    ui->textEdit->clear();
    ui->getRandom->setEnabled(true);
    ui->upload->setEnabled(true);

    if (db->isOpen())
    {
        qDebug() << speed;
        db->addTypingResult(speed * 60,errors,count,time);
    }

    Result mDialog(time,count,speed,errors);
    mDialog.setModal(true);
    mDialog.exec();
}

void MainWindow::on_pushButton_clicked()
{
    TableViewWindow tableViewWindow;
    tableViewWindow.setModal(true);
    tableViewWindow.exec();
}

