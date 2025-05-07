#include "MainWindow.h"
#include <qdialog.h>
#include "childdialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{   
    
    ui->setupUi(this);
    //QT4的信号和槽
    //connect(ui->showButton, SIGNAL(clicked(bool)), this, SLOT(showchilddialog()));
    //QT4的SIGNAL,SLOT只是宏转义，不会检测编译，所以有可能正常运行但点击并不会生效

    //推荐的QT5的方式
    connect(ui->showButton, &QPushButton::clicked, this, &MainWindow::showchilddialog);

    _childdialog = new childdialog(this);

    connect(_childdialog, &childdialog::showMainwin, this, &MainWindow::showMaindialog);
}


void MainWindow::showchilddialog() {
    _childdialog->show();
    _childdialog->resize(800, 800);
    this->hide();
}

void MainWindow::showMaindialog() {

    this->show();
    _childdialog->hide();
}
MainWindow::~MainWindow()
{
    delete ui;
}
