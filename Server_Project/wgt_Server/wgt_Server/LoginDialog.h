#pragma once

#include <QDialog>
#include "ui_LoginDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialogClass; };
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = nullptr);
	~LoginDialog();
signals:
	void switch_RegisterDialog();
private:
	Ui::LoginDialogClass *ui;
};
