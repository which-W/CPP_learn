#include "MainDialog.h"
#include "childDialog.h"
MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialogClass())
{
    ui->setupUi(this);
    connect(ui->ShowButton, &QPushButton::clicked, this, &MainDialog::on_enterMaibtn_clicked);
    connect(ui->ExitButton, &QPushButton::clicked, this, &MainDialog::exit_button_clicked);
}

void MainDialog::on_enterMaibtn_clicked() {
    //并没有被回收只是掩藏
    close();
    childDialog childdialog;
    //子界面转主界面
    auto res = childdialog.exec();
    if (res == childDialog::Accepted) {
        this->show();
    }
    else
    {
        return;
    }
   
}

void MainDialog::exit_button_clicked() {
    close();
}

MainDialog::~MainDialog()
{
    delete ui;
}
