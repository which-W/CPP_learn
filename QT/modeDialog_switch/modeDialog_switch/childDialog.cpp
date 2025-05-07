#include "childDialog.h"

childDialog::childDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::childDialogClass())
{
	ui->setupUi(this);
	connect(ui->showmainDialog, &QPushButton::clicked, this, &childDialog::on_enterMaibtn_clicked);
}

void childDialog::on_enterMaibtn_clicked() {
	accept();
}

childDialog::~childDialog()
{
	delete ui;
}
