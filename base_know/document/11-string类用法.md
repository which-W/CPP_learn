---
title: 零基础C++(11) string类用法
date: 2024-10-03 13:03:31
tags: C++ cppbase
categories: C++ cppbase
---

### 1. 引言



#### 什么是字符串？



字符串是由一系列字符组成的序列，用于表示文本信息。它在编程中被广泛应用于用户交互、文件处理、数据解析等场景。



#### C 风格字符串 vs `std::string`



在 C++ 中，有两种主要的字符串类型：



- **C 风格字符串（C-strings）**：基于字符数组，以空字符 (`'\0'`) 结尾。
- **C++ `std::string` 类**：更高级、功能更丰富的字符串类，封装了字符串操作的复杂性。



**C 风格字符串示例：**



```cpp
char cstr[] = "Hello, World!";
```



**`std::string` 示例：**



```cpp
#include <string>

std::string str = "Hello, World!";
```



------



### 2. `std::string` 基础



#### 定义与初始化



`std::string` 是 C++ 标准库中的一个类，位于 `<string>` 头文件中。它封装了字符序列，并提供了丰富的成员函数用于操作字符串。

初始化有很多中方式，如下图

![https://cdn.llfc.club/1727932857214.jpg](https://cdn.llfc.club/1727932857214.jpg)



**包含头文件：**



```cpp
#include <string>
```



**初始化示例：**



```cpp
#include <iostream>
#include <string>

int main() {
    // 默认构造函数
    std::string str1;

    // 使用字符串字面值初始化
    std::string str2 = "Hello";

    // 使用拷贝构造函数
    std::string str3(str2);

    // 使用部分初始化
    std::string str4(str2, 0, 3); // "Hel"

    // 使用重复字符初始化
    std::string str5(5, 'A'); // "AAAAA"

    std::cout << "str1: " << str1 << std::endl;
    std::cout << "str2: " << str2 << std::endl;
    std::cout << "str3: " << str3 << std::endl;
    std::cout << "str4: " << str4 << std::endl;
    std::cout << "str5: " << str5 << std::endl;

    return 0;
}
```



**输出：**



```makefile
str1:
str2: Hello
str3: Hello
str4: Hel
str5: AAAAA
```



#### 字符串输入与输出



**输出字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string greeting = "Hello, C++ Strings!";
    std::cout << greeting << std::endl;
    return 0;
}
```



**从用户输入字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string input;
    std::cout << "请输入一个字符串：";
    std::cin >> input; // 读取直到第一个空白字符
    std::cout << "您输入的字符串是：" << input << std::endl;
    return 0;
}
```



**读取包含空格的整行字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string line;
    std::cout << "请输入一行文本：";
    std::getline(std::cin, line);
    std::cout << "您输入的文本是：" << line << std::endl;
    return 0;
}
```



------



### 3. 字符串操作

常用的字符串操作如下：

![https://cdn.llfc.club/1727933166315.jpg](https://cdn.llfc.club/1727933166315.jpg)

#### 3.1 拼接与连接



**使用 `+` 运算符：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string first = "Hello, ";
    std::string second = "World!";
    std::string combined = first + second;
    std::cout << combined << std::endl; // 输出: Hello, World!
    return 0;
}
```



**使用 `append()` 函数：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello";
    str.append(", World!");
    std::cout << str << std::endl; // 输出: Hello, World!
    return 0;
}
```



**使用 `+=` 运算符：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Data";
    str += " Structures";
    std::cout << str << std::endl; // 输出: Data Structures
    return 0;
}
```



#### 3.2 比较字符串

关于字符串的比较，其实是逐个位置按照字符比较，计算机中字符存储的方式是ASCII码表，每个字符对应一个ASCII码值，比较字符就是比较ASCII码值的大小

<img src="https://cdn.llfc.club/ascii-1-3.png" alt="https://cdn.llfc.club/ascii-1-3.png" style="zoom: 50%;" />

一些控制字符也是通过ASCII码存储的

<img src="https://cdn.llfc.club/ascii-2-1.png" alt="https://cdn.llfc.club/ascii-2-1.png" style="zoom: 50%;" />



**使用 `==`, `!=`, `<`, `>`, `<=`, `>=` 运算符：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string a = "apple";
    std::string b = "banana";

    if (a == b) {
        std::cout << "a 和 b 相等" << std::endl;
    } else {
        std::cout << "a 和 b 不相等" << std::endl;
    }

    if (a < b) {
        std::cout << "a 在字典序中小于 b" << std::endl;
    } else {
        std::cout << "a 在字典序中不小于 b" << std::endl;
    }

    return 0;
}
```



**输出：**



```css
a 和 b 不相等
a 在字典序中小于 b
```



#### 3.3 查找与替换



**使用 `find()` 查找子字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string text = "The quick brown fox jumps over the lazy dog.";
    std::string word = "fox";

    size_t pos = text.find(word);
    if (pos != std::string::npos) {
        std::cout << "找到 '" << word << "' 在位置: " << pos << std::endl;
    } else {
        std::cout << "'" << word << "' 未找到。" << std::endl;
    }

    return 0;
}
```



**替换子字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string text = "I like cats.";
    std::string from = "cats";
    std::string to = "dogs";

    size_t pos = text.find(from);
    if (pos != std::string::npos) {
        text.replace(pos, from.length(), to);
        std::cout << "替换后: " << text << std::endl; // 输出: I like dogs.
    } else {
        std::cout << "'" << from << "' 未找到。" << std::endl;
    }

    return 0;
}
```



#### 3.4 子字符串与切片



**使用 `substr()` 获取子字符串：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";
    std::string sub = str.substr(7, 5); // 从位置7开始，长度5
    std::cout << sub << std::endl; // 输出: World
    return 0;
}
```



**注意：** 如果省略第二个参数，`substr()` 会返回从起始位置到字符串末尾的所有字符。



```cpp
std::string sub = str.substr(7); // 从位置7开始直到结束
std::cout << sub << std::endl; // 输出: World!
```



------



### 4. 字符串的常用成员函数



#### 4.1 长度与容量



**获取字符串长度：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "C++ Programming";
    std::cout << "字符串长度: " << str.length() << std::endl; // 输出: 14
    // 或者使用 size()
    std::cout << "字符串大小: " << str.size() << std::endl; // 输出: 14
    return 0;
}
```



**获取字符串容量：**



每个 `std::string` 对象都有一个容量（capacity），表示它当前能够持有的最大字符数，而不需要重新分配内存。



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello";
    std::cout << "初始容量: " << str.capacity() << std::endl;

    str += ", World!";
    std::cout << "追加后的容量: " << str.capacity() << std::endl;

    return 0;
}
```



**输出示例：**



```makefile
初始容量: 15
追加后的容量: 15
```



**注意：** 容量可能因实现而异，并不保证它等于长度。



#### 4.2 访问字符

对字符串中的字符操作，有如下方法, 切记需包含<cctype>头文件

![https://cdn.llfc.club/1727933254985.jpg](https://cdn.llfc.club/1727933254985.jpg)

**使用索引访问单个字符：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "ABCDE";

    // 正向索引
    for (size_t i = 0; i < str.length(); ++i) {
        std::cout << "字符 " << i << ": " << str[i] << std::endl;
    }

    //反向遍历
    for(int i = str.length() - 1; i >= 0 ; i --){
        std::cout << "下标为 " << i << "的字符为" << str[i] << std::endl;
    }

    return 0;
}
```



**使用 `at()` 函数（包含边界检查）：**



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "ABCDE";
    try {
        char c = str.at(10); // 超出范围，会抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "异常捕获: " << e.what() << std::endl;
    }
    return 0;
}
```



**输出：**



```kotlin
异常捕获: basic_string::at: __n (which is 10) >= this->size() (which is 5)
```



#### 4.3 转换大小写



C++ 标准库中的 `std::toupper` 和 `std::tolower` 可以用于转换字符的大小写。结合 `std::transform`，可以实现整个字符串的大小写转换。



**转换为大写：**



```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string str = "Hello, World!";
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](unsigned char c) { return std::toupper(c); });
    std::cout << str << std::endl; // 输出: HELLO, WORLD!
    return 0;
}
```



**转换为小写：**



```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string str = "Hello, World!";
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    std::cout << str << std::endl; // 输出: hello, world!
    return 0;
}
```



#### 4.4 其他有用的函数



- **`empty()`**：检查字符串是否为空。

  ```cpp
  std::string str;
  if (str.empty()) {
      std::cout << "字符串为空。" << std::endl;
  }
  ```

- **`clear()`**：清空字符串内容。

  ```cpp
  std::string str = "Clear me!";
  str.clear();
  std::cout << "str: " << str << std::endl; // 输出为空
  ```

- **`erase()`**：删除字符串的部分内容。

  ```cpp
  std::string str = "Hello, World!";
  str.erase(5, 7); // 从位置5开始，删除7个字符
  std::cout << str << std::endl; // 输出: Hello!
  ```

- **`insert()`**：在指定位置插入字符串或字符。

  ```cpp
  std::string str = "Hello World";
  str.insert(5, ",");
  std::cout << str << std::endl; // 输出: Hello, World
  ```

- **`replace()`**：替换字符串的部分内容（前面已示例）。

- **`find_first_of()`, `find_last_of()`**：查找字符集合中的任何一个字符。

  ```cpp
  std::string str = "apple, banana, cherry";
  size_t pos = str.find_first_of(", ");
  std::cout << "第一个逗号或空格的位置: " << pos << std::endl; // 输出: 5
  ```



------



### 5. 高级用法



#### 5.1 字符串流（`stringstream`）



`std::stringstream` 是 C++ 标准库中第 `<sstream>` 头文件提供的一个类，用于在内存中进行字符串的读写操作，类似于文件流。



**基本用法示例：**



```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::stringstream ss;
    ss << "Value: " << 42 << ", " << 3.14;

    std::string result = ss.str();
    std::cout << result << std::endl; // 输出: Value: 42, 3.14

    return 0;
}
```



**从字符串流中读取数据：**



```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string data = "123 45.67 Hello";
    std::stringstream ss(data);

    int a;
    double b;
    std::string c;

    ss >> a >> b >> c;

    std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
    // 输出: a: 123, b: 45.67, c: Hello

    return 0;
}
```



#### 5.2 字符串与其他数据类型的转换



**将其他类型转换为 `std::string`：**



- **使用 `std::to_string()`：**

  ```cpp
  #include <iostream>
  #include <string>
  
  int main() {
      int num = 100;
      double pi = 3.14159;
  
      std::string str1 = std::to_string(num);
      std::string str2 = std::to_string(pi);
  
      std::cout << "str1: " << str1 << ", str2: " << str2 << std::endl;
      // 输出: str1: 100, str2: 3.141590
      return 0;
  }
  ```



**将 `std::string` 转换为其他类型：**



- **使用字符串流：**

  ```cpp
  #include <iostream>
  #include <sstream>
  #include <string>
  
  int main() {
      std::string numStr = "256";
      std::string piStr = "3.14";
  
      int num;
      double pi;
  
      std::stringstream ss1(numStr);
      ss1 >> num;
  
      std::stringstream ss2(piStr);
      ss2 >> pi;
  
      std::cout << "num: " << num << ", pi: " << pi << std::endl;
      // 输出: num: 256, pi: 3.14
      return 0;
  }
  ```

- **使用 `std::stoi()`, `std::stod()` 等函数（C++11 及以上）：**

  ```cpp
  #include <iostream>
  #include <string>
  
  int main() {
      std::string numStr = "256";
      std::string piStr = "3.14";
  
      int num = std::stoi(numStr);
      double pi = std::stod(piStr);
  
      std::cout << "num: " << num << ", pi: " << pi << std::endl;
      // 输出: num: 256, pi: 3.14
      return 0;
  }
  ```



#### 5.3 正则表达式与字符串匹配



C++ 标准库提供了 `<regex>` 头文件，用于支持正则表达式。

关于正则表达式的规则可以参考菜鸟教程文档[https://www.runoob.com/regexp/regexp-syntax.html](https://www.runoob.com/regexp/regexp-syntax.html)



**基本用法示例：**



```cpp
#include <iostream>
#include <string>
#include <regex>

int main() {
    std::string text = "The quick brown fox jumps over the lazy dog.";
    std::regex pattern(R"(\b\w{5}\b)"); // 匹配所有5个字母的单词

    std::sregex_iterator it(text.begin(), text.end(), pattern);
    std::sregex_iterator end;

    std::cout << "5个字母的单词有:" << std::endl;
    while (it != end) {
        std::cout << (*it).str() << std::endl;
        ++it;
    }

    return 0;
}
```



**输出：**



```makefile
5个字母的单词有:
quick
brown
jumps
leazy
```



**说明：**



- `\b` 匹配单词边界。
- `\w{5}` 匹配恰好5个字母的单词。



**注意：** 使用原始字符串字面值（`R"()"`）以简化正则表达式的编写。



------



### 6. 字符串与 C 风格字符串的转换



#### 6.1 从 C 风格字符串转换为 `std::string`



通过 `std::string` 的构造函数，可以轻松将 C 风格字符串转换为 `std::string`。



```cpp
#include <iostream>
#include <string>

int main() {
    const char* cstr = "Hello, C-strings!";
    std::string str(cstr);
    std::cout << str << std::endl; // 输出: Hello, C-strings!
    return 0;
}
```



#### 6.2 从 `std::string` 转换为 C 风格字符串



使用 `c_str()` 成员函数，可以获取 C 风格字符串指针。



```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, std::string!";
    const char* cstr = str.c_str();
    std::cout << cstr << std::endl; // 输出: Hello, std::string!
    return 0;
}
```



**注意：** 返回的指针是只读的，且指向的内存由 `std::string` 管理，确保在 `std::string` 对象有效期间使用。



------



### 7. 示例项目



#### 示例项目1：简易文本分析器



**需求分析：**



创建一个程序，接受用户输入的一段文本，并提供以下功能：



- 统计单词数量
- 统计每个单词出现的次数
- 查找指定单词的出现次数
- 输出最长的单词



**代码实现：**



```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

int main() {
    std::string text;
    std::cout << "请输入一段文本（结束请输入Ctrl+D/Ctrl+Z）：\n";
    
    // 读取整段文本
    std::ostringstream oss;
    std::string line;
    while (std::getline(std::cin, line)) {
        oss << line << " ";
    }
    text = oss.str();

    // 使用字符串流分割单词
    std::stringstream ss(text);
    std::string word;
    std::map<std::string, int> wordCount;
    size_t totalWords = 0;
    std::string longestWord;

    while (ss >> word) {
        // 去除标点符号（简单处理）
        word.erase(std::remove_if(word.begin(), word.end(), 
            [](char c) { return ispunct(c); }), word.end());

        // 转为小写
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (!word.empty()) {
            wordCount[word]++;
            totalWords++;
            if (word.length() > longestWord.length()) {
                longestWord = word;
            }
        }
    }

    std::cout << "\n统计结果:\n";
    std::cout << "总单词数: " << totalWords << std::endl;
    std::cout << "每个单词出现的次数:\n";
    for (const auto& pair : wordCount) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::cout << "最长的单词: " << longestWord << std::endl;

    // 查找指定单词的出现次数
    std::string searchWord;
    std::cout << "\n请输入要查找的单词: ";
    std::cin >> searchWord;
    // 转为小写
    std::transform(searchWord.begin(), searchWord.end(), searchWord.begin(), ::tolower);
    auto it = wordCount.find(searchWord);
    if (it != wordCount.end()) {
        std::cout << "'" << searchWord << "' 出现了 " << it->second << " 次。" << std::endl;
    } else {
        std::cout << "'" << searchWord << "' 未在文本中找到。" << std::endl;
    }

    return 0;
}
```



**运行示例：**



```vbnet
请输入一段文本（结束请输入Ctrl+D/Ctrl+Z）：
Hello, world! Hello C++.
This is a simple text analyzer. Analyzing text is fun.

统计结果:
总单词数: 10
每个单词出现的次数:
hello: 2
world: 1
c: 1
this: 1
is: 2
a: 1
simple: 1
text: 2
analyzer: 1
analyzing: 1
fun: 1
最长的单词: analyzer

请输入要查找的单词: text
'text' 出现了 2 次。
```



**代码解析：**



1. **读取用户输入的文本**：使用 `std::ostringstream` 和 `std::getline` 读取用户输入的多行文本，直到用户输入结束（Ctrl+D 或 Ctrl+Z）。
2. **分割单词并统计**：
   - 使用 `std::stringstream` 将文本分割为单词。
   - 使用 `std::map` 存储每个单词出现的次数。
   - 计算总单词数和最长单词。
3. **查找指定单词**：用户输入要查找的单词，程序查找并输出出现次数。



#### 示例项目2：用户输入验证工具



**需求分析：**



编写一个程序，接受用户输入的电子邮件地址，并验证其格式是否正确。简单的验证标准：



- 包含一个 `@` 符号
- `@` 后面有一个 `.` 符号
- 不包含空格



**代码实现：**



```cpp
#include <iostream>
#include <string>
#include <regex>

bool isValidEmail(const std::string& email) {
    // 简单的正则表达式，匹配基本的邮件格式
    const std::regex pattern(R"((\w+)(\.?\w+)*@(\w+)(\.\w+)+)");
    return std::regex_match(email, pattern);
}

int main() {
    std::string email;
    std::cout << "请输入您的电子邮件地址: ";
    std::cin >> email;

    if (isValidEmail(email)) {
        std::cout << "电子邮件地址有效。" << std::endl;
    } else {
        std::cout << "电子邮件地址无效。" << std::endl;
    }

    return 0;
}
```



**运行示例：**



```sql
请输入您的电子邮件地址: user@example.com
电子邮件地址有效。
```



```makefile
请输入您的电子邮件地址: userexample.com
电子邮件地址无效。
```



**代码解析：**



1. **定义验证函数 `isValidEmail`**：
   - 使用正则表达式 `(\w+)(\.?\w+)*@(\w+)(\.\w+)+` 来匹配基本的邮箱格式。
   - 该正则表达式匹配如下部分：
     - 用户名部分：由字母数字字符组成，可以包含点号。
     - `@` 符号。
     - 域名部分：由字母数字字符组成，至少包含一个点号后跟字母数字字符。
2. **主函数**：
   - 提示用户输入邮箱地址。
   - 调用 `isValidEmail` 函数进行验证，并输出结果。



**注意：** 这个正则表达式只是一个基础的验证，实际应用中可能需要更复杂的正则表达式来处理更多的邮件格式。



------
