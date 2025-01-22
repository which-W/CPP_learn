---
title: 零基础C++(7) 引用类型
date: 2024-09-21 11:37:12
tags: C++ cppbase
categories: C++ cppbase
---

## `const`限定符

### 1 `const` 的定义与作用

`const` 是 C++ 关键字，用于指示变量的值不可修改。通过使用 `const`，可以提高代码的安全性与可读性，防止无意中修改变量的值。

### 2 `const` 在变量声明中的位置

`const` 关键字通常放在变量类型之前，例如：

``` cpp
const int a = 10;
```

也可以放在类型之后，但这种用法较少见：

``` cpp
int const a = 10;
```

可以用一个变量初始化常量， 也可以将一个常量赋值给一个变量

``` cpp
//可以用一个变量初始化常量
int i1 = 10;
const int i2 = i1;
//也可以将一个常量赋值给一个变量
int i3 = i2;
```

`const`变量必须初始化

``` cpp
//错误用法，const变量必须初始化
//const int i4;
```

### 3 编译器如何处理 `const` 修饰的变量

`const` 修饰的变量在编译时会被视为只读，尝试修改其值会导致编译错误。此外，编译器可能会对 `const` 变量进行优化，如将其存储在只读内存区域。

**注意**

> 默认状态下，`const`对象仅在文件内有效

当以编译时初始化的方式定义一个`const`对象时，就如对`bufSize`的定义一样：

``` cpp
const int bufSize = 512;
```

编译器将在编译过程中把用到该变量的地方都替换成对应的值。也就是说，编译器会找到代码中所有用到`bufSize`的地方，然后用512替换。

为了执行上述替换，编译器必须知道变量的初始值。

如果程序包含多个文件，则每个用了`const`对象的文件都必须得能访问到它的初始值才行。要做到这一点，就必须在每一个用到变量的文件中都有对它的定义.

为了支持这一用法，同时避免对同一变量的重复定义，默认情况下，`const`对象被设定为仅在文件内有效。当多个文件中出现了同名的`const`变量时，其实等同于在不同文件中分别定义了独立的变量。

我们创建一个`global.h`文件和`global.cpp`文件, 我们知道头文件只做变量的声明，之前我们在头文件添加变量的定义会导致连接错误。

那如果我们添加`const`变量的定义

``` cpp
#ifndef DAY08_CONST_GLOBAL_H
#define DAY08_CONST_GLOBAL_H
const int bufSize = 100;
#endif //DAY08_CONST_GLOBAL_H
```

在`main.cpp`和`global.cpp`中包含`global.h`，发现可以编译通过，虽然`main.cpp`和`global.cpp`中包含了同名的`bufSize`，但却是不同的变量，运行程序可以编译通过。

有时候我们不想定义不同的`const`变量，可以在`global.h`中用`extern`声明`bufSize`

``` cpp
extern const int bufSize2;
```

在`global.cpp`中定义

``` cpp
const int bufSize2 = 10;
```

同样可以编译通过。

为了验证我们的说法，我们可以在`global.h`中声明一个函数,用来打印两个变量的地址

``` cpp
//打印bufSize地址和bufSize2地址
extern void PrintBufAddress();
```

在`global.cpp`中实现`PrintBufAddress()`

``` cpp
void PrintBufAddress(){
    std::cout << "global.cpp buf address: " << &bufSize << std::endl;
    std::cout << "global.cpp buf2 address: " << &bufSize2 << std::endl;
}
```

然后我们在`main.cpp`中调用`PrintBufAddress()`函数，并且在`main.cpp`中打印两个变量地址

``` cpp
PrintBufAddress();
//输出bufSize地址
std::cout << "main.cpp buf address is " << &bufSize << std::endl;
//输出bufSize2地址
std::cout << "main.cpp buf2 address is " << &bufSize2 << std::endl;
```

程序输出

``` bash
global.cpp buf address: 0x7ff67a984040
global.cpp buf2 address: 0x7ff67a984044
main.cpp buf address is 0x7ff67a984000
main.cpp buf2 address is 0x7ff67a984044
```

可以看出`global.cpp`中的`bufSize`和`main.cpp`中的`bufSize`不是同一个变量

**技巧**

> 如果想在多个文件之间共享`const`对象，必须在变量的定义之前添加extern关键字。

## `const`的引用

可以把引用绑定到`const`对象上，就像绑定到其他对象上一样，我们称之为对常量的引用（`reference to const`）。与普通引用不同的是，对常量的引用不能被用作修改它所绑定的对象：

``` cpp
//定义常量
const int ci = 1024;
//用常量引用绑定常量
const int &r1 = ci;
```

不能修改常量引用的值

``` cpp
//不能修改常量引用的值
//r1 = 2048;
```

也不能用非常量引用指向一个常量对象

``` cpp
//也不能用非常量引用指向一个常量对象
//int& r2 = ci;
```

**术语**

> 常量引用是对`const`的引用

企业中，`C++`程序员们经常把词组“对`const`的引用”简称为“常量引用

允许将`const`引用绑定一个非`const`变量

``` cpp
int i5 = 1024;
//允许将const int& 绑定到一个普通的int对象上
const int &r5 = i5;
```

常量引用绑定字面量

``` cpp
//常量引用绑定字面量
const int &r6 = 1024;
```

常量引用绑定表达式计算的值

``` cpp
//常量引用绑定表达式计算的值
const int &r7 = r6 * 2;
const int &r8 = i5 * 2 + 10;
```

**思考1**

下面的代码能编译通过吗？

``` cpp
double dval = 3.14;
int & rd = dval;
```

**答案**

``` cpp
//错误用法，类型不匹配
double dval = 3.14;
int & rd = dval;
```

**思考2**

下面的代码能编译通过吗？

``` cpp
double dval = 3.14;
const int & ri = dval;
```

**答案**

``` cpp
//编译通过
double dval = 3.14;
const int & ri = dval;
```

上面的代码相当于

``` cpp
//上面代码会做隐士转换,相当于下面代码
const int temp  = dval;
const int &rt = temp;
```

在这种情况下，ri绑定了一个临时量（temporary）对象。

所谓临时量对象就是当编译器需要一个空间来暂存表达式的求值结果时临时创建的一个未命名的对象。

C++程序员们常常把临时量对象简称为临时量。

对`const`的引用可能引用一个并非`const`的对象必须认识到，常量引用仅对引用可参与的操作做出了限定，对于引用的对象本身是不是一个常量未作限定。因为对象也可能是个非常量，所以允许通过其他途径改变它的值：

``` cpp
int i9 = 1024;
//非常量引用绑定i9
int &r9 = i9;
//常量引用绑定一个变量
const int &r10 = i9;
//可以同过非常量引用修改i9的值
r9 = 2048;
```

## 指针和`const`

### 指向常量的指针(pointer to const)

可以令指针指向常量或非常量。类似于常量引用，指向常量的指针（`pointer to const`）不能用于改变其所指对象的值。

要想存放常量对象的地址，只能使用指向常量的指针：

``` cpp
//PI 是一个常量,它的值不能改变
const double PI = 3.14;
//错误，ptr是一个普通指针
//double * ptr = &PI;
//正确,cptr可以指向一个双精度常量
const double *cptr = &PI;
//错误，不能给*ptr赋值
//*cptr = 3.14;
```

指针的类型必须与其所指对象的类型一致，但是允许令一个指向常量的指针指向一个非常量对象

``` cpp
//可以用指向常量的指针指向一个非常量
int i10 = 2048;
//ptr指向i10
int *cptr2 = &i10;
```

### `const`指针

指针是对象而引用不是，因此就像其他对象类型一样，允许把指针本身定为常量。

常量指针（`const pointer`）必须初始化，而且一旦初始化完成，则它的值（也就是存放在指针中的那个地址）就不能再改变了。

把＊放在`const`关键字之前用以说明指针是一个常量，这样的书写形式隐含着一层意味，即不变的是指针本身的值而非指向的那个值：

``` cpp
int errNumb = 0;
//curErr是一个常量指针，指向errNumb
int * const curErr = &errNumb;
const double pi2 = 3.14;
//pip 是一个指向常量对象的常量指针
const double *const pip = &pi2;
```

指针本身是一个常量并不意味着不能通过指针修改其所指对象的值，能否这样做完全依赖于所指对象的类型

``` cpp
//错误，pip是一个指向常量的指针
//*pip = 2.72;
//可以修改常量指针指向的内容
*curErr = 1024;
//可以修改常量指针指向的地址
//curErr = &i10;
```

## 顶层`const`

指针本身是一个对象，它又可以指向另外一个对象。因此，指针本身是不是常量以及指针所指的是不是一个常量就是两个相互独立的问题

用名词顶层`const（top-level const）`表示指针本身是个常量，而用名词底层`const（low-level const）`表示指针所指的对象是一个常量。

顶层`const`可以表示任意的对象是常量，这一点对任何数据类型都适用，如算术类型、类、指针等。

底层`const`则与指针和引用等复合类型的基本类型部分有关。比较特殊的是，指针类型既可以是顶层`const`也可以是底层`const`，这一点和其他类型相比区别明显：

``` cpp
int i = 0;
//不能改变p1的值，这是一个顶层const
int * const pi = &i;
//不能改变ci的值，这是一个顶层const
const int ci  = 42;
//允许改变p2的值，这是一个底层const
const int *  p2 = &ci;
//靠右边的const是顶层const，靠左边的const是底层const
const int * const p3 = p2;
//用于声明引用的const都是底层const
const int &r = ci;
```

底层`const`的限制却不能忽视。当执行对象的拷贝操作时，拷入和拷出的对象必须具有相同的底层const资格，或者两个对象的数据类型必须能够转换

``` cpp
//指针赋值要注意关注底层const
//p2拥有底层const,p4无底层const，所以无法赋值
//int * p4 = p2;
```

## `constexpr`和常量表达式

常量表达式`（const expression）`是指值不会改变并且在编译过程就能得到计算结果的表达式。显然，字面值属于常量表达式，用常量表达式初始化的`const`对象也是常量表达式。后面将会提到，`C++`语言中有几种情况下是要用到常量表达式的。

我们先在global.h中声明一个全局函数返回固定大小

``` cpp
extern int GetSize();
```

在global.cpp中实现

``` cpp
int GetSize(){
    return 20;
}
```

然后我们用const定义一些常量表达式

一个对象（或表达式）是不是常量表达式由它的数据类型和初始值共同决定，例如

``` cpp
{
    //max_files是一个常量表达式
    const int max_files = 20;
    //limit是一个常量表达式
    const int limit = max_files + 10;
    //staff_size不是常量表达式,无const声明
    int staff_size = 20;
    //sz不是常量表达式,运行时计算才得知
    const int sz = GetSize();
}
```

尽管`staff_size`的初始值是个字面值常量，但由于它的数据类型只是一个普通`int`而非`const int`，所以它不属于常量表达式。

另一方面，尽管`sz`本身是一个常量，但它的具体值直到运行时才能获取到，所以也不是常量表达式。

在一个复杂系统中，很难（几乎肯定不能）分辨一个初始值到底是不是常量表达式。

当然可以定义一个`const`变量并把它的初始值设为我们认为的某个常量表达式，但在实际使用时，尽管要求如此却常常发现初始值并非常量表达式的情况。

**C++11新标准**

C++11新标准规定，允许将变量声明为`constexpr`类型以便由编译器来验证变量的值是否是一个常量表达式。声明为`constexpr`的变量一定是一个常量，而且必须用常量表达式初始化：

``` cpp
//20是一个常量表达式
constexpr int mf = 20;
//mf+1是一个常量表达式
constexpr int limit = mf + 10;
//错误，GetSize()不是一个常量表达式，需要运行才能返回
//constexpr int sz = GetSize();
```

尽管不能使用普通函数作为`constexpr`变量的初始值，新标准允许定义一种特殊的`constexpr`函数。

这种函数应该足够简单以使得编译时就可以计算其结果，这样就能用`constexpr`函数去初始化`constexpr`变量了。

我们在`global.h`中定义一个`constexpr`函数

``` cpp
inline constexpr int GetSizeConst() {
    return 1;
}
```

为了避免在多个源文件中包含同一个头文件而导致的多重定义错误，可以将 `constexpr` 函数声明为 `inline`。

`inline` 关键字允许在多个翻译单元中定义同一个函数，而不会引起链接错误。

接下来在定义一个`constexpr`变量就行了

``` cpp
constexpr int sz = GetSizeConst();
```

**指针和`constexpr`**

必须明确一点，在constexpr声明中如果定义了一个指针，限定符constexpr仅对指针有效，与指针所指的对象无关：

``` cpp
//p是一个指向整形常量的指针
const int * p = nullptr;
//q是一个指向整数的常量指针
constexpr int *q = nullptr;
```

一个`constexpr`指针的初始值必须是`nullptr`或者0，或者是存储于某个固定地址中的对象。

函数体内定义的变量一般来说并非存放在固定地址中，因此`constexpr`指针不能指向这样的变量。

定义于所有函数体之外的对象其地址固定不变，能用来初始化`constexpr`指针

global_i是一个全局变量

``` cpp
//constexpr指针只能绑定固定地址
//constexpr int *p = &mvalue;
constexpr int *p = nullptr;
//可以绑定全局变量，全局变量地址固定
constexpr  int *cp = &global_i;
```

可以修改`constexpr`指向的内容

``` cpp
constexpr int *p = &global_i;
//修改p指向的内容数据
*p = 1024;
```

**问题**

global_i是一个全局变量，下面这个指针是什么类型？能否修改`cp`指向的数据的内容(`*cp = 200`)？

``` cpp
constexpr const int * cp = &global_i;
```



