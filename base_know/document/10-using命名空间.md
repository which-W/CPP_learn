---
title: 零基础C++(10) 命名空间
date: 2024-10-03 12:23:44
tags: C++ cppbase
categories: C++ cppbase
---



## 命名空间的using声明

目前为止，我们用到的库函数基本上都属于命名空间std，而程序也显式地将这一点标示了出来。

例如，`std::cin`表示从标准输入中读取内容。此处使用作用域操作符`(::)`的含义是：编译器应从操作符左侧名字所示的作用域中寻找右侧那个名字。

因此，`std::cin`的意思就是要使用命名空间std中的名字cin。

上面的方法显得比较烦琐，然而幸运的是，通过更简单的途径也能使用到命名空间中的成员。

本节将学习其中一种简单的方法，使用using声明（using declaration），有了using声明就无须专门的前缀（形如命名空间::）也能使用所需的名字了。using声明具有如下的形式：

``` cpp
using namespace::name;
```

一旦声明了上述语句，就可以直接访问命名空间中的名字:

``` cpp
using std::cin;
int main() {
    int i ;
    //正确,cin和std::cin含义相同
    cin >> i;
    //错误,没有对应的using声明，必须使用完整的名字
    //cout << i;
    //正确，显示地从std中使用cout
    std::cout << i;
    return 0;
}
```

## 每个名字都需要独立的using声明

按照规定，每个using声明引入命名空间中的一个成员。例如，可以把要用到的标准库中的名字都以using声明的形式表示出来，程序如下：

``` cpp
using std::cin;
using std::endl;
int main() {
    int i ;
    //正确,cin和std::cin含义相同
    cin >> i;
    //错误,没有对应的using声明，必须使用完整的名字
    //cout << i;
    //正确，显示地从std中使用cout
    std::cout << i << endl;
    return 0;
}
```



## 头文件不应包含using声明

位于头文件的代码一般来说不应该使用using声明。这是因为头文件的内容会拷贝到所有引用它的文件中去，如果头文件里有某个using声明，那么每个使用了该头文件的文件就都会有这个声明。对于某些程序来说，由于不经意间包含了一些名字，反而可能产生始料未及的名字冲突。

