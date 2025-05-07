#include "MainDialog.h"
#include <qcolordialog.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <string>
#include <qtextcodec.h>
#include <qinputdialog.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qwizard.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
MainDialog::MainDialog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainDialogClass())
{
    ui->setupUi(this);

    _counter = 0;

    connect(ui->colorButton, &QPushButton::clicked, this, &MainDialog::colorDialogShow);
    connect(ui->textButton, &QPushButton::clicked, this, &MainDialog::textDialog);
    connect(ui->IntButton, &QPushButton::clicked, this, &MainDialog::IntDialog);
    connect(ui->floatButton, &QPushButton::clicked, this, &MainDialog::floatDialog);
    connect(ui->itemButton, &QPushButton::clicked, this, &MainDialog::itemDialog);
    connect(ui->msgButton, &QPushButton::clicked, this, &MainDialog::msgDialog);
    connect(ui->progessButton, &QPushButton::clicked, this, &MainDialog::progessDialog);
    connect(ui->wizadButton, &QPushButton::clicked, this, &MainDialog::wizadDialog);
  
    codec = QTextCodec::codecForName("GBK");

}

void MainDialog::IntDialog() {
    bool ok;
    auto intdata = QInputDialog::getInt(this, codec->toUnicode("数字输入对话框"), codec->toUnicode("请输入"), 200, -200, 400, 10, &ok);
    if (ok) {
        qDebug() << "数字为" << intdata;
    }
}

void MainDialog::floatDialog() {
    bool ok;
    auto floatdata = QInputDialog::getDouble(this, codec->toUnicode("数字输入对话框"), codec->toUnicode("请输入"), 0.1, -2, 6, 2, &ok);
    if (ok) {
        qDebug() << "数字为" << floatdata;
    }
}

void MainDialog::itemDialog() {
    QStringList items;
    items << codec->toUnicode("条目一") << codec->toUnicode("条目二");
    bool ok = false;
    auto itemData = QInputDialog::getItem(this, codec->toUnicode("条目输入对话框"), codec->toUnicode("请输入"), items, 0, true, &ok);
    if (ok) {
        qDebug() << "条目为" << itemData;
    }
}

void MainDialog::colorDialogShow() {
    QColorDialog colorDlg(Qt::blue, this);
    colorDlg.setOption(QColorDialog::ShowAlphaChannel);
    colorDlg.exec();
    QColor color = colorDlg.currentColor();
    qDebug() << "color is" << color;
}

void MainDialog::textDialog() {

    QString path = QDir::currentPath();
    QString title = codec->toUnicode("文本对话框");
    QString filter = codec->toUnicode("文本文件(*.txt);;图片文件(*.png,*.jpg);;所有文件(*.*)");
    QString aFileName = QFileDialog::getOpenFileName(this, title, path, filter);
    qDebug() << "选择文件为：" << aFileName;
}


void MainDialog::msgDialog() {
    auto res = QMessageBox::question(this, codec->toUnicode("提问对话框"), codec->toUnicode("你是单身狗吗"));
    if (res == QMessageBox::Yes) {
        qDebug() << "你好单身狗";
    }
    auto res2 = QMessageBox::information(this, codec->toUnicode("回答对话框"), codec->toUnicode("你好单身狗"), QMessageBox::Yes);
    if (res2 == QMessageBox::Yes) {
        qDebug() << "res2 is" << res2;
    }
}

void MainDialog::progessDialog() {
    _progessDialog = new QProgressDialog(codec->toUnicode("正在复制"),codec->toUnicode("取消复制"),0,5000,this);
    _progessDialog->setWindowTitle(codec->toUnicode("进度对话框"));
    _progessDialog->setWindowModality(Qt::ApplicationModal);
    //创建定时器
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &MainDialog::on_progessDialog);
    connect(_progessDialog, &QProgressDialog::canceled, this, &MainDialog::on_cancel_progessDialog);
    _timer->start(2);
}

void MainDialog::on_cancel_progessDialog() {
    _timer->stop();
    delete _timer;
    _timer = nullptr;
    delete _progessDialog;
    _counter = 0;
    return;
}
void MainDialog::on_progessDialog() {
    _counter++;
    if (_counter > 5000) {
        _timer->stop();
        delete _timer;
        _timer = nullptr;
        delete _progessDialog;
        _counter = 0;
        return;
    }
    _progessDialog->setValue(_counter);
}

void MainDialog::wizadDialog() {
    QWizard wizard(this);
    wizard.setWindowTitle(codec->toUnicode("全城热恋"));

    auto page1 = new QWizardPage();
    page1->setTitle(codec->toUnicode("婚恋程序引导系统"));
    auto label1 = new QLabel();
    label1->setText(codec->toUnicode("寻找你人生的终极伴侣"));

    auto layout = new QVBoxLayout();
    layout->addWidget(label1);
    page1->setLayout(layout);
    wizard.addPage(page1);

    auto page2 = new QWizardPage();
    page2->setTitle(codec->toUnicode("选择你心动的类型"));
    auto btn_group = new QButtonGroup(page2);
    auto btn1 = new QRadioButton();
    btn1->setText(codec->toUnicode("A: 小矮人"));
    auto btn2 = new QRadioButton();
    btn2->setText(codec->toUnicode("B: 小旅人"));
    auto btn3 = new QRadioButton();
    btn3->setText(codec->toUnicode("C: 小飞人"));
    btn_group->addButton(btn1);
    btn_group->addButton(btn2);
    btn_group->addButton(btn3);

    auto layout2 = new QVBoxLayout();
    for (size_t i = 0; i < btn_group->buttons().size(); i++)
    {
        layout2->addWidget(btn_group->buttons()[i]);
    }
    page2->setLayout(layout2);
    wizard.addPage(page2);
    

    auto page3 = new QWizardPage();
    page3->setTitle(codec->toUnicode("感谢您的参与"));

    auto label2 = new QLabel();
    label2->setText(codec->toUnicode("您的缘分即将到来"));
    auto layout3 = new QVBoxLayout();
    layout3->addWidget(label2);
    page3->setLayout(layout3);
    wizard.addPage(page3);

    wizard.show();
    wizard.exec();
}
MainDialog::~MainDialog()
{
    delete ui;
}
