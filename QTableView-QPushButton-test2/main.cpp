#include "mainwindow.h"

#include <QApplication>
#include "mydelegate.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    QStandardItemModel model(4, 2);
    QTableView tableView;
    tableView.setModel(&model);

    MyDelegate delegate;
    tableView.setItemDelegate(&delegate);

    tableView.horizontalHeader()->setStretchLastSection(true);
    tableView.show();
    return app.exec();
}
