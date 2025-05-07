#include "login.h"
#include <QtWidgets/QApplication>
#include "Register.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<login> w = std::make_shared<login>();
    w->InitRegister();
    w->show();
    return a.exec();
}
