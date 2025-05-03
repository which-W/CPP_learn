#pragma once

#include <QDialog>
#include "ui_RegisterDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterDialogClass; };
QT_END_NAMESPACE

class RegisterDialog : public QDialog
{
	Q_OBJECT

public:
	RegisterDialog(QWidget *parent = nullptr);
	~RegisterDialog();
	void showTip(QString str, bool b_ok);
public slots:
	void get_code_func();
private:
	Ui::RegisterDialogClass *ui;
	QTextCodec* codec;
};
