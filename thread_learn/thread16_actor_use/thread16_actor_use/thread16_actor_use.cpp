// thread16_actor_use.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ClassA.h"
int main()
{
    //CRTP的方式 A->B->C的信息处理流程的模型
    MsgClassA msga;
    msga.name = "wgt";
    ClassA::Inst().PostMsg(msga);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "main process exited!\n";
}


