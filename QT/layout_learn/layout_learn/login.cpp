#include "login.h"
#include "Register.h"
login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginClass())
{
    ui->setupUi(this);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &login::Switch_Dialog_Register);
}

void login::InitRegister() {
    _Register = std::make_shared<Register>();
    _Register->get_login(shared_from_this());

}
void login::Switch_Dialog_Register() {

    this->hide();
    _Register->show();
}

login::~login()
{
    delete ui;
}


