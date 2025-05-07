#include "MainWindow.h"
#include <qdialog.h>
#include <qdebug.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
    //在一个对话框中再生成一个,因为有this所以可以不用手动去析构
    auto w = new QDialog(this);
    //设置它是否为模态对话框
    w->setModal(true);
    //也可以这样设置
   /* QDialog w;
    w->exec();*/
    //设置默认上方
  /*  auto w = new QDialog(this);
    w->setWindowFlags(Qt::WindowStaysOnTopHint);
    w->show();*/
    w->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
