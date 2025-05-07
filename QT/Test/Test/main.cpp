#include "Test.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qdialog.h>
#include <qlabel.h>
int main(int argc, char *argv[])
{
    //负责消息轮询,F1可以查询文档的使用说明
    QApplication a(argc, argv);
    QDialog w;
    QLabel label(&w);
    label.setText("hello world");
    w.resize(460, 520);
    label.move(120, 120);
    w.show();
    return a.exec(); //阻塞的，不停轮询
}
