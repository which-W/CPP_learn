#include "RegisterDialog.h"
#include "global.h"
RegisterDialog::RegisterDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::RegisterDialogClass())
{
	ui->setupUi(this);
	ui->psw_edit->setEchoMode(QLineEdit::Password);
	ui->psw_edit_2->setEchoMode(QLineEdit::Password);
	ui->err_tip->setProperty("state", "normal");
	repolish(ui->err_tip);
	codec = QTextCodec::codecForName("GBK");
	connect(ui->get_code, &QPushButton::clicked, this, &RegisterDialog::get_code_func);
}


void RegisterDialog::get_code_func() {
	auto email = ui->email_edit->text();
	QRegularExpression regex("^[a - zA - Z0 - 9._ % +-] + @[a - zA - Z0 - 9. - ] + \.[a - zA - Z] {2, }$)");
	bool match = regex.match(email).hasMatch();
	if (match) {
		//·¢ËÍÑéÖ¤Âë
	}
	else {
		showTip(codec->toUnicode("ÓÊÏäÊäÈë´íÎó"),false);
	}

}
RegisterDialog::~RegisterDialog()
{
	delete ui;
}

void RegisterDialog::showTip(QString  str ,bool b_ok)
{	
	if (b_ok) {
		ui->err_tip->setProperty("state", "normal");
	}
	else
	{
		ui->err_tip->setProperty("state", "err");
	}
	ui->err_tip->setText(str);
	repolish(ui->err_tip);
}
