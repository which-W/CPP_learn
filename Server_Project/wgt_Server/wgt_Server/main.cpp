#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss("./style/this_style.qss");
    if (qss.open(QFile::ReadOnly)) {
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();

    }
    else {
        qDebug("open qss failed");
    }
    MainWindow w;
    w.show();
    return a.exec();
}
