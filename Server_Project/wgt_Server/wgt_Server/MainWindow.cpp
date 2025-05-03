#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
     _loginDialog = new LoginDialog(this);
    setCentralWidget(_loginDialog);
    _loginDialog->show();

    connect(_loginDialog, &LoginDialog::switch_RegisterDialog, this, &MainWindow::slot_switch_Register);
    _RegisterDialog = new RegisterDialog(this);

    _loginDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _RegisterDialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _RegisterDialog->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_switch_Register()
{
    setCentralWidget(_RegisterDialog);
    _loginDialog->hide();
    _RegisterDialog->show();

}
