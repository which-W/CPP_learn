#include "Test.h"

Test::Test(QWidget *parent)
    : QMainWindow(parent),ui(new Ui::TestClass )
{
    ui->setupUi(this);
    ui->pushButton->isDefault();
}

Test::~Test()
{
    delete ui;
}
