#include "MainWindow.h"
#include "qstandarditemmodel.h"
#include "qtableview.h"
#include "SpinBoxDelegate.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);

    //形成一个表格
    auto* model = new QStandardItemModel(7, 7, this);
    for (int row = 0; row < 7; row++) {
        for (int column = 0; column < 7; column++) {
            QStandardItem* item = new QStandardItem(QString("%1").arg(row * 4 + column));
            model->setItem(row, column, item);
        }
    }
    QTableView* table = new QTableView;
    table->setModel(model);
    setCentralWidget(table);
    this->resize(1200, 1200);

    //另一个不同方式产生的表格面板
    QTableView* _table_view2 = new QTableView;
    SpinBoxDelegate* delegate = new SpinBoxDelegate(this);
    QStandardItemModel* model2 = new QStandardItemModel(7, 4, this);
    for (int row = 0; row < 7; row++) {
        for (int column = 0; column < 4; column++) {
            QStandardItem* item = new QStandardItem(QString("%1").arg(row * 4 + column));
            model2->setItem(row, column, item);
        }
    }
    _table_view2->setModel(model2);
    _table_view2->setItemDelegate(delegate);
    _table_view2->show();
    _table_view2->resize(1200, 1200);
}

MainWindow::~MainWindow()
{
    delete ui;
}
