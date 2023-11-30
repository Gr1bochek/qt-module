#include "tableviewwindow.h"
#include <QSqlTableModel>
#include <QVBoxLayout>

TableViewWindow::TableViewWindow(QWidget *parent)
    : QDialog(parent)
{
    dbManager = DbManager::getInstance("typing.db");
    tableView = new QTableView(this);

    QSqlTableModel *model = new QSqlTableModel(this, dbManager->getDatabase());
    model->setTable("typing_results");
    model->setSort(model->fieldIndex("words_per_minute"), Qt::DescendingOrder);
    model->select();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    mainLayout->addWidget(tableView);

    setLayout(mainLayout);

    setWindowTitle("Typing Results");
    resize(800, 600);
}
