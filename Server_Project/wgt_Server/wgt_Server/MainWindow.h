#pragma once
/*
    author: which_w
    data:2025-3-8
    brief:Ö÷´°¿Ú

*/
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slot_switch_Register();
private:
    Ui::MainWindowClass *ui;
    LoginDialog* _loginDialog;
    RegisterDialog* _RegisterDialog;

};
