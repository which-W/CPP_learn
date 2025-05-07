#include "MainWindow.h"
#include <qstring.h>
#include <QRegExpValidator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
    //IP地址
    QString IP_Addr = "000.000.000.000; _";
    ui->ip_lineEdit->setInputMask(IP_Addr);
    //MAC的地址
    QString MAC_Addr = "HH:HH:HH:HH:HH:HH;_";
    ui->mack_lineEdit->setInputMask(MAC_Addr);
    //密码
    ui->pwd_lineEdit->setEchoMode(QLineEdit::Password);
    //邮箱
    ui->mail_lineEdit->setEchoMode(QLineEdit::Normal);
    //设置正则表达式过滤  secondtonone23@163.com
    QRegExp regx("[a-zA-Z0-9_-]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+");
    QValidator* validator = new QRegExpValidator(regx, ui->mail_lineEdit);
    ui->mail_lineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}
