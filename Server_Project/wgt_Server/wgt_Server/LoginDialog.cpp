#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::LoginDialogClass())
{
	ui->setupUi(this);
	connect(ui->exit_button, &QPushButton::clicked, this, &LoginDialog::switch_RegisterDialog);
}

LoginDialog::~LoginDialog()
{
	delete ui;
}
