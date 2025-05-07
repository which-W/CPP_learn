#include "MainDialog.h"
#include <QtWidgets/QApplication>
#include "childDialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
    childDialog childDialog;
    auto res = childDialog.exec();
    if (res == QDialog::Accepted) {
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
    
}
