#pragma once

#include <QDialog>
#include "ui_Register.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterClass; };
QT_END_NAMESPACE
class login;
class Register : public QDialog
{
	Q_OBJECT

public:
	Register(QWidget *parent = nullptr);
	~Register();
	void get_login(const std::weak_ptr<login>& login);
public slots:
	void Switch_Dialog_Login();
private:
	Ui::RegisterClass *ui;

	QTextCodec* codec;

	std::weak_ptr<login> _login;


};
