// thread10_no_mutex_queue.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "CircularQuelk.h"
#include "Myclass.h"
#include "CircularQueligth.h"
#include "CircularQueSeq.h"
#include "CircularQueMseq.h"
void TestCircularQue() {
    //最大容量为10
    CircularQueLk<MyClass, 5> cq_lk;
    MyClass mc1(1);
    MyClass mc2(2);
    cq_lk.push(mc1);
    cq_lk.push(std::move(mc2));
    for (int i = 3; i <= 5; i++) {
        MyClass mc(i);
        auto res = cq_lk.push(mc);
        if (res == false) {
            break;
        }
    }
    cq_lk.push(mc2);
    for (int i = 0; i < 5; i++) {
        MyClass mc1;
        auto res = cq_lk.pop(mc1);
        if (!res) {
            break;
        }
        std::cout << "pop success, " << mc1 << std::endl;
    }
    auto res = cq_lk.pop(mc1);
}

void TestCircularQueSeq()
{
    CircularQueSeq<MyClass, 3> cq_seq;
    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }
}

void TestCircularQueLight()
{
    CircularQueLight<MyClass, 3> cq_seq;
    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }
}

void TestCircularQueMseq() {
    CircularQueMseq<MyClass, 3> cq_seq;
    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1(i);
        auto res = cq_seq.push(mc1);
        if (!res)
        {
            break;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        MyClass mc1;
        auto res = cq_seq.pop(mc1);
        if (!res)
        {
            break;
        }

        std::cout << "pop success, " << mc1 << std::endl;
    }
}
int main()
{
    std::cout << "Hello World!\n";
}
