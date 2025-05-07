#pragma once

#include <QDialog>
#include "ui_childDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class childDialogClass; };
QT_END_NAMESPACE

class childDialog : public QDialog
{
	Q_OBJECT

public:
	childDialog(QWidget *parent = nullptr);
	~childDialog();
public slots:
	void on_enterMaibtn_clicked();
private:
	Ui::childDialogClass *ui;
};
