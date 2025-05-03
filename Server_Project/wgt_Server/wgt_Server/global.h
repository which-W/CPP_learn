#pragma once
#include<qwidget.h>
#include<functional>
#include"qstyle.h"
#include<qregularexpression.h>
#include<QTextCodec>
//用来刷新Qss的
extern std::function<void(QWidget*)> repolish;