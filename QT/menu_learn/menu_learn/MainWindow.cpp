#include "MainWindow.h"
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);

    connect(ui->showdialog, &QAction::triggered, this, &MainWindow::show_debog_func);
    connect(ui->NewFile_N, &QAction::triggered, this, &MainWindow::on_actionnew_N_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_debog_func(void) {
    
    qDebug() << "show dedog_dialog";
    ui->mainwindow->show();
}

void MainWindow::on_actionnew_N_triggered(void)
{
    qDebug() << "�½��ļ�..." << endl;
    QTextEdit* textEdit = new QTextEdit(this);
    auto childWindow = ui->mdiArea->addSubWindow(textEdit);
    childWindow->setWindowTitle(tr("�ı��༭�Ӵ���"));
    childWindow->show();
}