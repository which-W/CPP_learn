#include "Test.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qdialog.h>
#include <qlabel.h>
int main(int argc, char *argv[])
{
    //������Ϣ��ѯ,F1���Բ�ѯ�ĵ���ʹ��˵��
    QApplication a(argc, argv);
    QDialog w;
    QLabel label(&w);
    label.setText("hello world");
    w.resize(460, 520);
    label.move(120, 120);
    w.show();
    return a.exec(); //�����ģ���ͣ��ѯ
}
