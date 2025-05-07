#pragma once

#include <QMainWindow>
#include "ui_childdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class childdialogClass; };
QT_END_NAMESPACE

class childdialog : public QMainWindow
{
	Q_OBJECT
signals:
	void showMainwin();
public:
	childdialog(QWidget *parent = nullptr);
	~childdialog();
public slots:
	void showparentwin();
private:
	Ui::childdialogClass *ui;
	
};
