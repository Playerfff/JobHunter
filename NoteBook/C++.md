# C

1. malloc & calloc
   1. malloc分配内存, 不初始化;
   2. calloc分配内存, 且全部初始化为0.

# C++

1. overload和override区别？

   1. overload是函数重载;
   2. override是虚函数重写;

2. c语言是否支持函数重载? 为什么?

   1. 不支持
   2. c语言

3. low-level const & top-level const:

   1. low-level:

   ```cpp
   const int x = 1;
   const int *p = &x; // y
   *p = 2; // n, 指针所指为const
   ```

   2. top-level:

   ```cpp
   int x = 1;
   int *const p = &x; // 指针为const
   ```

4. enum class;

   1. c++11 引入了枚举类,更安全

   ```cpp
   enum class Color {
       Red = 1,
       Green,
       Blue
   };
   
   Color chosenColor = Color::Green;
   
   ```

   2. `enum class` 枚举值的整数值没有默认值。与传统的 `enum` 不同，`enum class` 不会自动分配整数值，它需要您显式指定每个枚举值的整数值。这是为了提供更严格的类型安全和避免与其他枚举值或全局命名空间的符号冲突。

5. **函数的默认参数**应该在函数的声明或定义中指定，而不是在函数的实现中。这是因为默认参数信息需要在编译阶段可见，以便正确生成函数调用代码。

6. 函数指针:

   1. 函数指针的使用:

   ```cpp
   int add(int a, int b) {
       return a + b;
   }
   
   int subtract(int a, int b) {
       return a - b;
   }
   
   // 函数指针的定义和初始化
   int (*operation)(int, int) = add; // 初始化函数指针，指向 add 函数
   
   int result = operation(5, 3); // 调用 operation 函数指针，执行 add 函数
   
   operation = subtract; // 切换函数指针，使其指向 subtract 函数
   result = operation(5, 3); // 调用 operation 函数指针，执行 subtract 函数
   
   ```

   2. 在C++11及更高版本中，`std::function`和`std::bind`通常用于替代传统的函数指针，提供了更灵活和类型安全的函数调用机制。

      `std::function` 是一个通用的函数包装器，它允许您存储和调用各种可调用对象，包括函数指针、函数对象、Lambda表达式等。使用 `std::function` 可以实现动态绑定不同的可调用对象，而不需要考虑具体的类型。

      以下是 `std::function` 的基本用法：

      ```cpp
      #include <iostream>
      #include <functional>
      
      int add(int a, int b) {
          return a + b;
      }
      
      int main() {
          std::function<int(int, int)> func = add; // 存储函数指针
      
          int result = func(3, 4); // 调用函数
      
          std::cout << "结果：" << result << std::endl;
      
          return 0;
      }
      ```

      在上述示例中，我们使用 `std::function` 声明了一个可以存储接受两个整数参数并返回整数的可调用对象。我们将函数指针 `add` 存储在 `func` 中，然后通过 `func` 调用 `add` 函数。

      `std::function` 提供了一种更灵活的方式来存储和调用可调用对象，使得函数调用更加通用和类型安全。这对于实现回调、函数替换和类似的功能非常有用。

7. **移动语义**专题:

   1. 什么是移动构造函数和移动赋值运算符？

      移动构造函数（Move Constructor）和移动赋值运算符（Move Assignment Operator）是C++11及更高版本引入的特性，用于提高对象的性能和资源管理效率，特别是在处理动态分配内存和大型数据结构时。这些特性与C++的移动语义相关。

      1. **移动构造函数（Move Constructor）：**

         移动构造函数是一个特殊的构造函数，用于从一个对象“窃取”其资源，通常是动态分配的内存或其他资源，而不是执行昂贵的复制操作。移动构造函数通常采用右值引用（Rvalue Reference）作为参数，允许将源对象的资源移动到目标对象，同时将源对象置于有效但无效状态。

         示例：

         ```cpp
         class MyString {
         public:
             MyString(MyString&& other) {
                 data = other.data;
                 size = other.size;
                 other.data = nullptr; // 将源对象置于有效但无效状态
                 other.size = 0;
             }
         };
         ```

      2. **移动赋值运算符（Move Assignment Operator）：**

         移动赋值运算符是一个特殊的赋值运算符，用于实现对象之间的资源移动。它允许将源对象的资源移动到目标对象，同时释放或清理源对象的资源，以避免资源泄漏。

         示例：

         ```c++
         MyString& operator=(MyString&& other) {
             if (this != &other) {
                 delete[] data; // 清理当前对象的资源
                 data = other.data;
                 size = other.size;
                 other.data = nullptr; // 将源对象置于有效但无效状态
                 other.size = 0;
             }
             return *this;
         }
         ```

      移动构造函数和移动赋值运算符通常在使用标准库容器和智能指针等对象时自动调用，以提高性能和资源管理效率。它们在移动大型数据结构时特别有用，因为它们可以避免不必要的复制和资源分配。在C++中，使用移动语义可以大幅提高程序的性能。

   2. 移动语义什么时候可以使用呢?

      移动语义可以在以下情况下使用：

      1. **对象将要被销毁：** 当一个对象即将被销毁，它的资源可以被移动到另一个对象，以避免不必要的复制操作。
      2. **资源管理类：** 在使用资源管理类（如智能指针、动态数组）时，移动语义可以帮助有效地传递资源的所有权，避免不必要的资源复制。
      3. **STL容器：** 使用STL容器时，移动语义可以避免在容器元素之间进行不必要的复制。
      4. **大型数据结构：** 当操作大型数据结构时，移动语义可以显著提高性能，因为资源的复制会占用大量时间和内存。

      以下是一个示例，说明在何种情况下使用移动语义可以提高性能：

      ```c++
      #include <iostream>
      #include <vector>
      #include <string>
      
      int main() {
          std::vector<std::string> source; // 创建一个包含字符串的向量
          source.push_back("Hello");
          source.push_back("World");
      
          std::vector<std::string> destination; // 创建另一个向量
      
          // 使用移动语义，将source向量的内容移动到destination向量，而不是复制
          destination = std::move(source);
      
          // 此时source向量已经为空，destination向量包含了原始数据
          std::cout << "Source vector size: " << source.size() << std::endl; // 输出 0
          std::cout << "Destination vector size: " << destination.size() << std::endl; // 输出 2
      
          return 0;
      }
      ```

      在这个示例中，通过使用 `std::move` 将source向量的内容移动到destination向量，而不是进行复制，从而提高了性能。这是因为在移动操作中，资源的所有权被传递，而不是复制。在实际应用中，移动语义通常用于避免不必要的资源复制和提高程序性能。

   3. 这个里面的std::move的作用是不是把source从左值变成右值,然后就可以调用移动赋值运算符把资源偷过来了?

      是的，您的理解是正确的。`std::move` 的作用是将一个左值（通常是一个已命名的对象，如`source`）转换为右值引用，这样就可以调用移动构造函数或移动赋值运算符，将资源从一个对象移动到另一个对象。

      在上面的示例中，`std::move(source)` 将`source` 转换为右值引用，然后使用移动赋值运算符 `=` 将`source` 的内容移动到`destination` 中，而不是进行资源的复制。这允许资源的所有权从一个对象转移到另一个对象，而不需要复制。

      通过使用 `std::move`，您可以显式指定将要使用移动语义的对象，从而提高程序性能，特别是在处理大型数据结构或资源管理时。但请注意，使用 `std::move` 时需要小心，确保不再使用源对象，因为源对象可能处于有效但无效的状态。

   4. 移动返回:

      在C++11及更高版本的标准中，您可以在函数内构建一个`std::vector`，并通过移动返回（Move Return）来返回它，以避免不必要的复制。以下是一个示例：

      ```c++
      #include <iostream>
      #include <vector>
      
      std::vector<int> createAndReturnVector() {
          std::vector<int> myVector;
          
          // 向myVector添加一些元素
          myVector.push_back(1);
          myVector.push_back(2);
          myVector.push_back(3);
      
          // 使用 std::move 返回myVector，执行移动语义
          return std::move(myVector);
      }
      
      int main() {
          std::vector<int> result = createAndReturnVector();
      
          // 此时myVector的资源已被移动到result，myVector为空
          std::cout << "Result size: " << result.size() << std::endl; // 输出 3
          std::cout << "MyVector size: " << myVector.size() << std::endl; // 输出 0
      
          return 0;
      }
      ```

      在这个示例中，`createAndReturnVector` 函数内部构建了一个`std::vector`对象 `myVector`，然后通过 `std::move` 将它移动返回。这样，资源的所有权被移动到 `result`，而不是进行复制。这提高了性能，特别是当`myVector`包含大量元素时。

      使用移动返回可以有效地避免不必要的复制操作，特别是对于大型数据结构或容器来说，这在C++11及更高版本的标准中是一个非常有用的功能。

   5. 但是就算不加这个std::move,std::vector<int> result = createAndReturnVector();这句话等号右边不是也是右值吗

      您说得对，即使不加 `std::move`，`std::vector<int> result = createAndReturnVector();` 这句话右边也是右值。在这种情况下，编译器会自动执行移动语义，因为返回的对象是临时对象（右值），编译器会选择移动构造函数来优化性能。

      所以，您完全可以省略 `std::move`，因为返回的对象已经是右值，编译器会根据需要自动应用移动语义。以下是不使用 `std::move` 的示例：

      ```c++
      std::vector<int> createAndReturnVector() {
          std::vector<int> myVector;
          
          myVector.push_back(1);
          myVector.push_back(2);
          myVector.push_back(3);
      
          return myVector; // 返回右值
      }
      
      int main() {
          std::vector<int> result = createAndReturnVector();
      
          // 此时myVector的资源已被移动到result，myVector为空
          std::cout << "Result size: " << result.size() << std::endl; // 输出 3
          std::cout << "MyVector size: " << myVector.size() << std::endl; // 输出 0
      
          return 0;
      }
      ```

      这段代码也能正常工作，因为返回的对象已经是右值，编译器会自动应用移动语义来优化性能。`std::move` 在这里是可选的，但它可以用来明确表达您的意图，即要使用移动语义。

   6. 既然编译器会自动执行,那为什么要设计移动返回的语法?

      移动返回语法和 `std::move` 的设计是为了提供更灵活的控制和明确的意图，而不仅仅依赖于编译器的自动优化。虽然编译器在某些情况下可以自动执行移动语义，但有时您可能希望显式指定移动，以确保性能和资源管理。

      以下是一些使用移动返回语法和 `std::move` 的情况和优点：

      1. **明确的语义：** 使用 `std::move` 或移动返回语法可以明确表达您的意图，即要执行移动语义。这使代码更易于理解，尤其是对其他开发人员来说，他们能够清楚地看到资源的所有权将被移动。
      2. **强制移动：** 有时，您可能希望强制执行移动语义，而不依赖于编译器的自动优化。这可以在确保最佳性能和资源管理方面提供更大的控制。
      3. **防止不必要的拷贝：** 在某些情况下，编译器可能无法自动执行移动，尤其是当对象有多个具有副作用的复制构造函数时。通过使用 `std::move` 或移动返回语法，您可以明确指定要执行移动，而不进行不必要的拷贝。
      4. **清晰的代码：** 使用移动返回语法或 `std::move` 可以提高代码的清晰度和可读性，使代码更容易维护和调试。

      虽然编译器会尽力自动执行移动语义以提高性能，但明确指定移动的语法提供了更多的控制和表达性，使程序员能够更精细地调整和优化代码的性能和资源管理。这种设计哲学是C++语言中的一部分，使得C++非常灵活，适用于广泛的应用场景。

   8. 什么是 RAII（Resource Acquisition Is Initialization）技术，如何使用它来管理资源？

      RAII（Resource Acquisition Is Initialization）是一种C++编程技术，用于管理资源，如内存、文件句柄、数据库连接等。RAII 原则的核心思想是，资源的获取应该在对象的构造阶段完成，而资源的释放应该在对象的析构阶段进行，从而确保资源的正确分配和释放。这种技术有助于避免资源泄漏，提高代码的可维护性和安全性。

      使用 RAII 技术的基本步骤如下：

      1. **定义一个管理资源的类：** 创建一个用于管理资源的类，通常称为资源管理类。这个类的构造函数负责分配资源，析构函数负责释放资源。
      2. **资源的获取：** 在资源管理类的构造函数中，获取所需的资源，如分配内存、打开文件、建立数据库连接等。
      3. **资源的使用：** 在资源管理类中定义其他成员函数，以便在资源上执行操作或提供资源的访问。这些成员函数应该在资源可用时被调用。
      4. **资源的释放：** 当资源管理类的实例超出作用域（例如，离开代码块）时，其析构函数自动调用，以释放资源。这确保了资源的正确释放，即使发生异常也不会泄漏资源。

      下面是一个示例，展示如何使用 RAII 技术来管理文件句柄：

      ```cpp
      #include <iostream>
      #include <fstream>
      
      class FileResource {
      private:
          std::ofstream file;
      
      public:
          FileResource(const std::string& filename) {
              file.open(filename);
              if (!file.is_open()) {
                  throw std::runtime_error("Failed to open file");
              }
          }
      
          void writeData(const std::string& data) {
              file << data;
          }
      
          // 析构函数负责释放资源（关闭文件）
          ~FileResource() {
              if (file.is_open()) {
                  file.close();
              }
          }
      };
      
      int main() {
          try {
              FileResource file("example.txt"); // 创建文件资源管理对象
      
              file.writeData("Hello, RAII!"); // 使用文件资源进行写操作
      
              // 文件资源会在离开作用域时自动释放，即使发生异常也不会泄漏资源
          } catch (const std::exception& e) {
              std::cerr << "Exception: " << e.what() << std::endl;
          }
      
          return 0;
      }
      ```

      在这个示例中，`FileResource` 类负责打开文件并在析构函数中关闭文件。当 `FileResource` 对象超出作用域时，文件资源会自动被释放，即使发生异常也不会导致文件句柄泄漏。这遵循了 RAII 原则，确保资源的正确管理和释放。

      RAII 技术在C++中是一种强大的资源管理工具，它可用于管理各种资源，包括内存、文件、数据库连接、锁等。它可以提高代码的可维护性，避免资源泄漏，并增加代码的安全性。

   9. 什么是模板偏特化（Template Partial Specialization）？它有什么作用？

      模板偏特化（Template Partial Specialization）是C++中模板元编程的一个重要特性，用于特化模板类或函数，以适应特定的模板参数组合。它允许你根据模板参数的一部分来定义不同的实现，以满足特定的需求。模板偏特化通常用于处理特殊情况或特定类型的参数，以提供更精确的控制。

      模板偏特化的基本语法如下：

      ```c++
      template <typename T, typename U>
      class MyTemplate {
          // 通用模板类的实现
      };
      
      template <typename T>
      class MyTemplate<T, int> {
          // 针对第二个模板参数为 int 的偏特化实现
      };
      
      template <>
      class MyTemplate<char, int> {
          // 针对两个模板参数都为 char 和 int 的偏特化实现
      };
      ```

      在上述示例中，`MyTemplate` 类的第一个模板参数 `T` 是通用的，但第二个模板参数可以是任何类型。通过定义不同的偏特化版本，你可以根据第二个模板参数的类型来提供不同的实现。偏特化版本可以根据需要特殊处理特定的情况，以满足特定的需求。

      模板偏特化的作用包括：

      1. **提供更精确的控制：** 模板偏特化允许你根据模板参数的一部分来特化实现，以提供更精确的控制。这对于处理特殊情况非常有用。
      2. **优化性能：** 通过为特定的参数组合提供优化的实现，可以提高程序的性能。例如，你可以为特定类型的参数提供更高效的算法。
      3. **适应不同的情况：** 模板偏特化可以使模板类或函数适应不同的情况，以满足多样化的需求。

      需要注意的是，模板偏特化适用于类模板和函数模板，但并不是所有情况都需要使用。通常，只有在需要处理特殊情况或优化性能时，才会使用模板偏特化。它是模板元编程的强大工具，但需要小心使用，以避免过度复杂化代码。

   10. 解释 C++ 中的缓冲区是什么，以及为什么要使用缓冲区？

       在C++中，缓冲区（Buffer）是一个用于临时存储数据的内存区域。缓冲区可以是一个数组、一个字符数组、一个字符串或任何用于暂时存储数据的数据结构。在C++中，缓冲区通常用于输入和输出操作，特别是与文件、流和I/O设备的交互操作。

       **为什么要使用缓冲区？**

       使用缓冲区有以下几个主要原因：

       1. **性能优化：** 缓冲区可以减少频繁的I/O操作，从而提高性能。相比每次读写一个字节或一个字符，将数据存储在缓冲区中，并以更大的块进行I/O操作，通常会更加高效。
       2. **减少系统调用：** 缓冲区可以减少与操作系统的交互次数。操作系统通常以块的方式处理文件或流，因此将数据存储在缓冲区中可以减少系统调用的数量，从而提高效率。
       3. **批量处理：** 缓冲区使得批量处理数据更容易。你可以将一组数据写入缓冲区，然后一次性将整个缓冲区的数据写入文件或流。
       4. **数据处理：** 缓冲区允许你在内存中进行数据处理，例如对文本数据进行分割、解析或格式化，然后再将结果写入文件或流。
       5. **缓冲数据：** 缓冲区还可以用于缓存数据，以便稍后进行访问。这在某些情况下可以提高访问速度。

       在C++中，标准库提供了用于处理缓冲区的工具，例如 `std::cout` 和 `std::cin`，它们分别用于标准输出和标准输入，并具有内置的缓冲机制。你还可以使用文件流和字符串流等工具来操作缓冲区，以满足不同的需求。使用缓冲区可以提高I/O操作的效率，并允许更灵活地处理数据。

   11. 

### string

1. 如何在一个string中查找子串?

```cpp
#include <iostream>
#include <string>

int main() {
    std::string mainString = "This is a sample string.";
    std::string subString = "sample";

    size_t found = mainString.find(subString);

    if (found != std::string::npos) {
        std::cout << "子串 '" << subString << "' 在主字符串中的位置：" << found << std::endl;
    } else {
        std::cout << "未找到子串 '" << subString << "'" << std::endl;
    }

    return 0;
}

```

2. 如何删除字符串中的空格？

```cpp
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string str = "  This is a sample   string with   spaces.  ";
    
    // 使用标准库函数erase和remove_if来删除空格
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    
    std::cout << "删除空格后的字符串：" << str << std::endl;

    return 0;
}

```

3. 如何将一个字符串转换为整数？

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "12345";
    
    // 使用 std::stoi 将字符串转换为整数
    int number = std::stoi(str);

    std::cout << "转换后的整数：" << number << std::endl;

    return 0;
}

```

## C++标准库

1. IO库:

   1. 如何在 C++ 中读取文件？如何将读取的内容写入到文件中？

      ```cpp
      // 读
      #include <fstream>
      
      int main()
      {
          // 打开文件并创建文件流对象
          std::fstream inputFile("input.txt");
          
          // 从文件中读取内容
          std::string line;
          while(std::getline(inputFile, line))
          {
              std::cout << line << std::endl;
          }
          
          // 关闭文件
          inputFile.close();
      }
      ```

      ```cpp
      // 写
      #include <fstream>
      
      int main()
      {
          std::ofstream outputFile("output.txt");
          
          outputFile << "Hello, world!" << std::endl;
          outputFile << "This is a sample text." << std::endl;
          
          outputFile.close();
      }
      ```

   2. 如何将 C++ 标准输出重定向到文件中？

      1. 将标准输出重定向到文件流：

      ```cpp
      std::streambuf* oldStdout = std::cout.rdbuf(outputFile.rdbuf());
      ```

      这一行代码将标准输出 (`std::cout`) 的缓冲区重定向到文件流 (`outputFile`) 的缓冲区。这意味着所有通过 `std::cout` 输出的内容将被写入到 "output.txt" 文件中。

      2. 恢复标准输出的缓冲区：

      ```c++
      std::cout.rdbuf(oldStdout);
      ```

      这一行代码将标准输出的缓冲区恢复到原来的状态，从而停止将输出重定向到文件。

2. 顺序容器:

   1. emplace_back和push_back区别?

      主要的区别在于 `emplace_back` 更适合在容器内构造新元素，而 `push_back` 适合将已有对象添加到容器。 `emplace_back` 可以减少不必要的复制操作，提高了效率，特别是对于包含复杂对象的容器。然而，你需要确保元素的构造函数是正确定义的，以接受相应的参数。

   2. 如何在 vector 中实现元素去重？

      ```cpp
      #include <algorithm>
      #include <iostream>
      #include <vector>
      
      int main()
      {
          std::vector<int> myVector = {3, 1, 2, 2, 1, 3, 4, 4, 5};
      
          // 排序
          std::sort(myVector.begin(), myVector.end());
      
          // 去重
          myVector.erase(std::unique(myVector.begin(), myVector.end()),
                         myVector.end());
      
          // 输出去重后的元素
          for (const auto &element : myVector)
          {
              std::cout << element << " ";
          }
      
          return 0;
      }
      ```

      **使用`std::unique`去重：** 使用`std::unique`函数将重复的元素移到`vector`的末尾，然后返回一个迭代器，指向去重后的尾部（不包括重复的元素）。

3. 泛型算法:

   1. std::sort:

   ```cpp
   #include <algorithm>
   #include <vector>
   
   int main() {
       std::vector<int> myVector = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
   
       // 使用 std::sort 和 std::greater 对容器降序排序, 默认是std::less
       std::sort(myVector.begin(), myVector.end(), std::greater<int>());
   
       // 输出降序排序后的元素
       for (const auto& element : myVector) {
           // 9 6 5 5 5 4 3 3 2 1 1
       }
   
       return 0;
   }
   
   ```

   

   2. 查找最大最小值:

   ```cpp
   #include <iostream>
   #include <vector>
   #include <algorithm>
   
   int main() {
       std::vector<int> myVector = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
   
       auto maxElement = std::max_element(myVector.begin(), myVector.end());
   
       if (maxElement != myVector.end()) {
           std::cout << "最大值是: " << *maxElement << std::endl;
       } else {
           std::cout << "容器为空" << std::endl;
       }
   
       return 0;
   }
   ```

   ```cpp
   #include <iostream>
   #include <vector>
   #include <algorithm>
   
   int main() {
       std::vector<int> myVector = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
   
       auto minElement = std::min_element(myVector.begin(), myVector.end());
   
       if (minElement != myVector.end()) {
           std::cout << "最小值是: " << *minElement << std::endl;
       } else {
           std::cout << "容器为空" << std::endl;
       }
   
       return 0;
   }
   ```

   

4. 关联容器:

   1. 什么情况下应该使用map，什么情况下应该使用unordered_map？

      在C++中，`std::map` 和 `std::unordered_map` 都是用于关联容器的类型，用于存储键-值对。它们在不同的场景下有不同的优势和适用性。

      **使用 `std::map`：**

      1. **有序数据：** 如果你需要维护键值对的有序顺序，应该使用 `std::map`。`std::map` 使用红黑树实现，确保了键值对按照键的升序顺序存储，因此可以支持有序的遍历和范围查询。
      2. **唯一键：** `std::map` 中的键是唯一的，不允许重复键。如果需要确保每个键在容器中只出现一次，应该使用 `std::map`。
      3. **对插入和删除操作没有高性能要求：** `std::map` 的插入和删除操作的时间复杂度为 O(log n)，其中 n 是容器中元素的数量。如果你不需要高性能的插入和删除操作，`std::map` 是一个不错的选择。

      **使用 `std::unordered_map`：**

      1. **哈希表：** 如果你需要高效的查找、插入和删除操作，并且不需要维护元素的顺序，应该使用 `std::unordered_map`。`std::unordered_map` 使用哈希表实现，这意味着这些操作的平均时间复杂度是 O(1)。
      2. **允许重复键：** `std::unordered_map` 中的键是唯一的，允许多个键对应相同的值。如果需要存储具有相同键的多个值，可以使用 `std::unordered_multimap`。
      3. **没有严格的有序要求：** 如果不需要按键的顺序访问元素，`std::unordered_map` 可能更适合，因为它不维护元素的有序性。
      4. **高性能要求：** 对于大型数据集或需要快速查找、插入和删除的场景，`std::unordered_map` 可能是更好的选择。

      综上所述，你应该根据具体的需求来选择使用 `std::map` 还是 `std::unordered_map`。如果需要维护元素的顺序、唯一键或者没有高性能要求，`std::map` 可能更适合。如果需要快速的查找、插入和删除操作，并且可以放宽有序性的要求，`std::unordered_map` 可能更合适。

   2. 如何在C++关联容器中使用自定义比较函数进行排序？

      ```cpp
      #include <iostream>
      #include <set>
      
      struct Person {
          std::string name;
          int age;
      };
      
      bool compareByAge(const Person& a, const Person& b) {
          return a.age < b.age;
      }
      
      int main() {
          std::set<Person, decltype(compareByAge)> people(compareByAge);
      
          people.insert({"Alice", 30});
          people.insert({"Bob", 25});
          people.insert({"Charlie", 35});
      
          for (const Person& p : people) {
              std::cout << p.name << " (" << p.age << " years old)" << std::endl;
          }
      
          return 0;
      }
      ```

      

5. 动态内存:

   1. 什么是虚拟析构函数？为什么需要它？

      **虚拟析构函数（Virtual Destructor）：** 虚拟析构函数是C++中的一种特殊函数，通常用于在继承层次结构中的基类中，以确保在派生类对象被删除时正确调用析构函数。当基类指针指向派生类对象，如果基类的析构函数不是虚拟的，那么当使用基类指针删除对象时，只会调用基类的析构函数，而不会调用派生类的析构函数，可能导致资源泄漏或不正确的清理。通过将基类的析构函数声明为虚拟析构函数，可以确保在删除对象时正确调用派生类的析构函数。

      以下是示例：

      ```cpp
      class Base {
      public:
          virtual ~Base() {
              // 虚拟析构函数
          }
      };
      
      class Derived : public Base {
      public:
          ~Derived() {
              // 派生类的析构函数
          }
      };
      
      int main() {
          Base* ptr = new Derived;
          delete ptr; // 通过基类指针删除对象，会正确调用派生类的析构函数
          return 0;
      }
      ```