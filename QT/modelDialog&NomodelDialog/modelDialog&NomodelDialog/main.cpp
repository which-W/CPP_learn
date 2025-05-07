#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qdialog.h>
//模态与非模态对话框的区别在于新产生的对话框是否可以移动
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
