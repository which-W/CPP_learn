#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qdialog.h>
//ģ̬���ģ̬�Ի�������������²����ĶԻ����Ƿ�����ƶ�
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
