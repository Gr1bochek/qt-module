#ifndef TABLEVIEWWINDOW_H
#define TABLEVIEWWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "dbmanager.h"
#include "qdialog.h"

class TableViewWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TableViewWindow(QWidget *parent = nullptr);

private:
    QTableView *tableView;
    DbManager *dbManager;
};

#endif // TABLEVIEWWINDOW_H
