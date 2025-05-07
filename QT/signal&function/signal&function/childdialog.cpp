#include "childdialog.h"

childdialog::childdialog(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::childdialogClass())
{
	ui->setupUi(this);
	connect(ui->pushButton, &QPushButton::clicked, this, &childdialog::showparentwin);
}

childdialog::~childdialog()
{
	delete ui;
}


void childdialog::showparentwin() {
	this->hide();
	emit showMainwin();
}