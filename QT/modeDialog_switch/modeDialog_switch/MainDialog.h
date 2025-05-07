#pragma once

#include <QtWidgets/QDialog>
#include "ui_MainDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialogClass; };
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

public slots:
    void on_enterMaibtn_clicked();
    void exit_button_clicked();
private:
    Ui::MainDialogClass *ui;
};
