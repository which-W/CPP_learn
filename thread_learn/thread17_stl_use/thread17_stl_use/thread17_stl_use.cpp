// thread17_stl_use.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "join_thread.h"
#include "parallen_each_for.h"
#include "parallen_find.h"
#include <random>
#include <vector>
#include <cmath>
#include "parallen_sum.h"

void TestParallenForEach() {

    std::vector<int> nvec;
    for (int i = 0; i < 26; i++) {
        nvec.push_back(i);
    }

    parallel_for_each(nvec.begin(), nvec.end(), [](int& i) {
        i *= i;
        });

    for (int i = 0; i < nvec.size(); i++) {
        std::cout << nvec[i] << " ";
    }

    std::cout << std::endl;
}


std::vector<int> TestAsyncForEach() {
    std::vector<int> nvec;
    for (int i = 0; i < 55; i++) {
        nvec.push_back(i);
    }

    async_for_each(nvec.begin(), nvec.end(), [](int& i) {
        i *= i;
        });

    for (int i = 0; i < nvec.size(); i++) {
        std::cout << nvec[i] << " ";
    }

    std::cout << std::endl;

    return nvec;
}


void TestParallenFind(std::vector<int>& nvec) {
    auto iter = parallel_find(nvec.begin(), nvec.end(), 25 * 25);

    if (iter == nvec.end()) {
        std::cout << "not find value sqrt value 25" << std::endl;
        return;
    }
    std::cout << "find iter value is " << *iter << std::endl;
}

void AsyncParallenFind(std::vector<int>& nvec) {
    auto iter = parallel_find_async(nvec.begin(), nvec.end(), 25 * 25);

    if (iter == nvec.end()) {
        std::cout << "not find value sqrt value 25" << std::endl;
        return;
    }
    std::cout << "find iter value is " << *iter << std::endl;
}


void TestParallelPartialSum() {
    std::vector<int> nvec = { 1,2,3,4,5,6,7,8,9 };
    parallel_partial_sum(nvec.begin(), nvec.end());
    for (int i = 0; i < nvec.size(); i++) {
        std::cout << nvec[i] << " ";
    }

    std::cout << std::endl;
}



int main()
{
    std::vector<int> necv{ 1,24,25,13,24,45,24,12,214,34,54,(25*25)};
    /*TestParallenForEach();*/
    TestParallenFind(necv);
}
