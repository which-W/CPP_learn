#include "MainDialog.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
    w.show();
    //�������ı���

    return a.exec();
}
