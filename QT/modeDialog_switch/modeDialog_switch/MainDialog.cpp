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
    //��û�б�����ֻ���ڲ�
    close();
    childDialog childdialog;
    //�ӽ���ת������
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
