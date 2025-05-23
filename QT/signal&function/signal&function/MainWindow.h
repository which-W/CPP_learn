#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "childdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void showchilddialog();
    void showMaindialog();
private:
    Ui::MainWindowClass *ui;
    childdialog* _childdialog;
};
