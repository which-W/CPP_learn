#include "MainWindow.h"
#include <qdialog.h>
#include <qdebug.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
    //��һ���Ի�����������һ��,��Ϊ��this���Կ��Բ����ֶ�ȥ����
    auto w = new QDialog(this);
    //�������Ƿ�Ϊģ̬�Ի���
    w->setModal(true);
    //Ҳ������������
   /* QDialog w;
    w->exec();*/
    //����Ĭ���Ϸ�
  /*  auto w = new QDialog(this);
    w->setWindowFlags(Qt::WindowStaysOnTopHint);
    w->show();*/
    w->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
