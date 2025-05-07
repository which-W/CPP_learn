#include "Register.h"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextCodec>
#include "login.h"
Register::Register(QWidget* parent)
    : QDialog(parent), ui(new Ui::RegisterClass())
{
    ui->setupUi(this);
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    this->setMaximumSize(QSize(400, 500));
    this->setMinimumSize(QSize(400, 500)); // 限制窗口

    // 总的垂直布局
    QVBoxLayout* vbox_layout = new QVBoxLayout(); // 不需要传递 this 作为参数

    // 第一个弹簧
    QSpacerItem* verticalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vbox_layout->addItem(verticalSpacer1);

    // 邮箱部分
    QLabel* name_label = new QLabel(this);
    name_label->setText(codec->toUnicode("邮箱："));
    QLineEdit* name_edit = new QLineEdit(this);
    QHBoxLayout* name_layout = new QHBoxLayout();
    name_layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    name_layout->addWidget(name_label);
    name_layout->addWidget(name_edit);
    name_layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    vbox_layout->addLayout(name_layout);

    // 第二个弹簧
    vbox_layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Maximum, QSizePolicy::Maximum));

    // 密码部分
    QLabel* pwd_label = new QLabel(this);
    pwd_label->setText(codec->toUnicode("密码："));
    QLineEdit* pwd_line = new QLineEdit(this);
    QHBoxLayout* pwd_layout = new QHBoxLayout();
    pwd_layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    pwd_layout->addWidget(pwd_label);
    pwd_layout->addWidget(pwd_line);
    pwd_layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum));
    vbox_layout->addLayout(pwd_layout);

    // 第三个弹簧
    vbox_layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Expanding));

    // 注册按钮
    QPushButton* _res_btn = new QPushButton(this);
    _res_btn->setText(codec->toUnicode("注册"));
    QHBoxLayout* _res_layout = new QHBoxLayout();
    _res_layout->addWidget(_res_btn, 5);
    vbox_layout->addLayout(_res_layout);

    // 最后一个弹簧
    vbox_layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Expanding));

    // 设置布局
    this->setLayout(vbox_layout);


    connect(_res_btn, &QPushButton::clicked, this, &Register::Switch_Dialog_Login);
}

Register::~Register()
{
    delete ui;
}

void Register::get_login(const std::weak_ptr<login>& login) {

    _login = login;
}

void Register::Switch_Dialog_Login() {
    this->close();
    auto shared_lock = _login.lock();
    shared_lock->show();
}