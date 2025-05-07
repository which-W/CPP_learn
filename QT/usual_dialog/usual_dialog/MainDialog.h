#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainDialog.h"
#include <qprogressdialog.h>
#include <qtimer.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainDialogClass; };
QT_END_NAMESPACE

class MainDialog : public QMainWindow
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

public slots:
    void colorDialogShow();
    void textDialog();
    void IntDialog();
    void floatDialog();
    void itemDialog();
    void msgDialog();
    void progessDialog();
    void on_progessDialog();
    void on_cancel_progessDialog();
    void wizadDialog();
private:
    Ui::MainDialogClass *ui;

    QProgressDialog* _progessDialog;

    QTimer* _timer;

    QTextCodec* codec;

    int _counter  ;
};
