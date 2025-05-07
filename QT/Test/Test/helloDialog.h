#pragma once

#include <QDialog>
#include "ui_helloDialog.h"

class helloDialog : public QDialog
{
	Q_OBJECT

public:
	helloDialog(QWidget *parent = nullptr);
	~helloDialog();

private:
	Ui::helloDialogClass ui;
};
