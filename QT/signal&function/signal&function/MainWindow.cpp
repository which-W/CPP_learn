#include "MainWindow.h"
#include <qdialog.h>
#include "childdialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{   
    
    ui->setupUi(this);
    //QT4���źźͲ�
    //connect(ui->showButton, SIGNAL(clicked(bool)), this, SLOT(showchilddialog()));
    //QT4��SIGNAL,SLOTֻ�Ǻ�ת�壬��������룬�����п����������е������������Ч

    //�Ƽ���QT5�ķ�ʽ
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
