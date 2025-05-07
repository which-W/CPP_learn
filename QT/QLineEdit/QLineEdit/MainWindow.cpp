#include "MainWindow.h"
#include <qstring.h>
#include <QRegExpValidator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
    //IP��ַ
    QString IP_Addr = "000.000.000.000; _";
    ui->ip_lineEdit->setInputMask(IP_Addr);
    //MAC�ĵ�ַ
    QString MAC_Addr = "HH:HH:HH:HH:HH:HH;_";
    ui->mack_lineEdit->setInputMask(MAC_Addr);
    //����
    ui->pwd_lineEdit->setEchoMode(QLineEdit::Password);
    //����
    ui->mail_lineEdit->setEchoMode(QLineEdit::Normal);
    //����������ʽ����  secondtonone23@163.com
    QRegExp regx("[a-zA-Z0-9_-]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+");
    QValidator* validator = new QRegExpValidator(regx, ui->mail_lineEdit);
    ui->mail_lineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}
