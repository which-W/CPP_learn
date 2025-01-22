## 变量

### 变量定义

在C++中，变量是用来存储数据值的一种实体。每个变量都有一个类型，这个类型决定了变量可以存储的数据的种类以及变量在内存中所占的空间大小。

通俗例子

我们家中会有收纳柜，收纳柜存储的是我们的物品。物品就相当于数据，收纳柜就相当于变量，不同的柜子有不同的类型，有的装内衣，有的装袜子。

![https://cdn.llfc.club/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_17245418515191.png](https://cdn.llfc.club/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_17245418515191.png)

装内衣的柜子一般不会装螺丝刀，一个柜子在定义好用来做什么，它存储的内容就确定了。

### 补充知识

计算机中数据是按照二进制存储的，一个字节占8bit, bit就是位的意思，比如数字2会转化为二进制00000010, 然后将这个00000010放到计算机为我们分配好的存储单元里，这个存储单元本身还有一个地址，假设存储单元的地址为5，转化为二进制就是00000101.

![https://cdn.llfc.club/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_17245430325039.png](https://cdn.llfc.club/%E4%BC%81%E4%B8%9A%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_17245430325039.png)

当我们想要取出数据00000010时，需要先访问地址00000101找到存储单元，然后取出存储单元存储的数据。

再次理解以下，存储单元可以理解为一个变量，存储了数据00000010，变量的地址为00000101。理解这个，之后我们会介绍指针的概念。



### 变量的声明与初始化

在C++中，你首先需要声明一个变量，然后（可选地）可以初始化它。声明变量时，你需要指定变量的类型和名称。

``` cpp
// 声明一个整型变量age，未初始化
int age; 
// 声明并初始化一个整型变量height
int height = 175; 
// 声明并初始化一个双精度浮点型变量weight
double weight = 65.5; 
// 声明并初始化一个字符型变量gender
char gender = 'M'; 
```

### 变量命名规则

在C++中，变量名可以包含字母、数字和下划线（_），**但不能以数字开头**。此外，C++是大小写敏感的，因此`age`和`Age`被视为两个不同的变量。

### 变量类型

C++支持多种基本数据类型，包括整型（`int`、`short`、`long`、`long long`）、浮点型（`float`、`double`、`long double`）、字符型（`char`）、布尔型（`bool`）等。此外，C++还支持枚举（`enum`）、结构体（`struct`）、联合体（`union`）和类（`class`）等复合数据类型。

### 示例：使用变量

``` cpp
#include <iostream>

int main() {
    //初始化变量a和b
    int a = 5, b = 10;
    //a+b的值赋值给sum
    int sum = a + b;
    //输出求和的结果
    std::cout << "The sum of " << a << " and " << b << " is " << sum << std::endl;
    return 0;
}
```

在这个示例中，我们声明了三个整型变量`a`、`b`和`sum`，并将`a`和`b`的值分别初始化为5和10。然后，我们计算`a`和`b`的和，并将结果存储在变量`sum`中。最后，我们使用`std::cout`输出计算结果。

``` bash
The sum of 5 and 10 is 15
```



### 输出运算符

在C++中，输出通常是通过标准输出流（`std::cout`）来完成的。`std::cout` 是C++标准库中的一个对象，它代表标准输出设备，通常是屏幕。为了使用 `std::cout` 来输出数据，你需要包含头文件 `<iostream>`。

下面是一个简单的C++程序示例，展示了如何使用 `std::cout` 来输出文本和数字

``` cpp
#include <iostream> // 包含标准输入输出流库

int main() {
    // 输出字符串
    std::cout << "Hello, World!" << std::endl;

    // 输出数字
    int number = 42;
    std::cout << "The number is: " << number << std::endl;

    // 输出浮点数
    double pi = 3.14159;
    std::cout << "The value of pi is approximately: " << pi << std::endl;

    // 输出字符
    char ch = 'A';
    std::cout << "The character is: " << ch << std::endl;

    // 输出布尔值（C++11及以后）
    bool truth = true;
    std::cout << std::boolalpha; // 启用布尔值的true/false输出
    std::cout << "The truth is: " << truth << std::endl;

    return 0;
}
```

在这个例子中，`<<` 是插入运算符，用于将右侧的数据发送到左侧的流对象（在这个例子中是 `std::cout`）。`std::endl` 是一个操纵符，用于在输出后插入换行符，并刷新输出缓冲区，确保立即在屏幕上显示输出。

``` bash
Hello, World!
The number is: 42
The value of pi is approximately: 3.14159
The character is: A
The truth is: true
```



**理解即可**

注意，为了输出布尔值 `true` 和 `false` 而不是整数 `1` 和 `0`，我们使用了 `std::boolalpha` 操纵符。这是C++11及以后版本中引入的，用于控制布尔值的输出格式。

此外，C++还支持格式化输出，但这通常涉及更复杂的语法，如使用 `std::iomanip` 头文件中定义的操纵符（如 `std::setw` 和 `std::setprecision`或使用C++20引入的新特性（如格式化库 `std::format`，尽管这不是通过 `std::cout` 直接使用的，而是用于生成格式化字符串）。

### 练习

定义初始化一个变量pai_val数值为 3.14， 初始化一个int类型的变量int_val变量值为5，分别输出这两个变量的值。

初始化一个bool变量数值为-100，输出bool变量的值，看看是true还是false。

### 赞赏

感谢支持

![https://cdn.llfc.club/dashang.jpg](https://cdn.llfc.club/dashang.jpg)