#pragma once

#include <QtWidgets/QDialog>
#include "ui_login.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class loginClass; };
QT_END_NAMESPACE

class Register;
class login : public QDialog,public std::enable_shared_from_this<login> 
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();
    void InitRegister();
public slots:

    void Switch_Dialog_Register();
private:
    Ui::loginClass *ui;
    //两个页面互引用的操作
    std::shared_ptr<Register> _Register;
};
