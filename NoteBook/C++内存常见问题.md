## 什么是内存安全，如何在C++中确保内存安全？

内存安全是指程序在运行时不会访问或修改未分配给它的内存区域，也不会在释放了内存后仍然尝试访问该内存。

1. 智能指针
2. 越界检查
3. 避免悬挂指针
- 指针悬挂是指指向已经释放的内存的指针。使用空指针或智能指针来确保在释放内存后将指针设置为nullptr。
```c++
int *ptr = new int;
// ...
delete ptr;
ptr = nullptr; // 避免悬挂指针
```
4. RAII
```c++
class FileHandler {
public:
    FileHandler(const std::string& filename) : file_(std::fopen(filename.c_str(), "r")) {
        if (!file_) {
            throw std::runtime_error("Failed to open file");
        }
    }

    ~FileHandler() {
        if (file_) {
            std::fclose(file_);
        }
    }

private:
    FILE* file_;
};
```

## C++中如何进行自定义内存管理？

### 重载new和delete
通过重载new和delete运算符，你可以控制对象的动态内存分配和释放。这使得你可以使用自定义的内存分配策略。
```c++
class MyClass {
public:
    void* operator new(size_t size) {
        // 自定义内存分配逻辑
        return ::operator new(size);
    }

    void operator delete(void* ptr) {
        // 自定义内存释放逻辑
        ::operator delete(ptr);
    }
};
```

### 自定义内存分配器
可以通过实现自定义的内存分配器类来控制内存分配和释放的细节。这个类需要实现 allocate 和 deallocate 方法，以及可能的构造函数和析构函数。
```c++
class CustomAllocator {
public:
    void* allocate(size_t size) {
        // 实现自定义内存分配逻辑
        return ::operator new(size);
    }

    void deallocate(void* ptr) {
        // 实现自定义内存释放逻辑
        ::operator delete(ptr);
    }
};

class MyClass {
public:
    static void* operator new(size_t size) {
        return allocator.allocate(size);
    }

    static void operator delete(void* ptr) {
        allocator.deallocate(ptr);
    }

private:
    static CustomAllocator allocator;
};

CustomAllocator MyClass::allocator;
```

## C++11中的nullptr与C风格中的NULL或0有什么区别？

nullptr是在C++11中引入的一项改进，旨在提供更好的类型安全性和清晰的语义，避免与整数发生混淆。

在新的代码中，推荐使用nullptr而不是NULL或0来表示空指针。

## C++中如何使用placement new？

Placement new 是 C++ 中的一种内存分配技术，它允许你在已分配的内存中构造对象，而不是使用默认的内存分配机制。这对于在特定位置（已分配的内存块）上创建对象很有用，例如，用于对象池或者需要精确控制对象位置和生命周期的场景。以下是使用 placement new 的基本示例：
```c++
#include <iostream>

class MyClass {
public:
    MyClass(int value) : data(value) {
        std::cout << "Constructor called: " << data << std::endl;
    }

    ~MyClass() {
        std::cout << "Destructor called: " << data << std::endl;
    }

private:
    int data;
};

int main() {
    // 分配内存块，例如，一个字符数组
    char buffer[sizeof(MyClass) * 5];

    // 使用 placement new 在预分配的内存中构造对象
    MyClass* obj1 = new (buffer) MyClass(1);
    MyClass* obj2 = new (buffer + sizeof(MyClass)) MyClass(2);

    // 调用对象的方法
    std::cout << "obj1 data: " << obj1->data << std::endl;
    std::cout << "obj2 data: " << obj2->data << std::endl;

    // 手动调用析构函数，因为没有使用 delete
    obj1->~MyClass();
    obj2->~MyClass();

    return 0;
}
```

## 什么是内存泄漏？如何在C++中检测内存泄漏？

内存泄漏是指在程序运行时，分配的内存没有被释放，而且没有任何方式来访问或释放该内存。内存泄漏可能会导致程序消耗越来越多的内存，最终耗尽系统可用的内存资源，导致程序崩溃或变得异常缓慢。在长时间运行的程序中，内存泄漏是一种常见的问题。

使用valgrind, asan等工具来检测内存泄漏.

## 野指针是什么，如何避免？

野指针是指指向无效或未知内存地址的指针。使用野指针可能导致程序的不稳定性和不可预测的行为，因为它们可能会访问未分配给程序的内存区域，导致内存损坏或崩溃。以下是一些避免野指针的方法：

1. 初始化指针
2. 避免悬挂指针
3. 尽量使用引用而不是指针, 尽可能使用局部变量而不是动态分配内存，以减少野指针的可能性。
4. 检查空指针

## 堆和栈的主要区别是什么？

1. 分配方式：

- 栈： 栈是一种有限且固定大小的内存区域，用于存储函数的局部变量、函数参数以及函数调用的返回地址。栈的分配是由编译器自动管理的，变量的生命周期与其所在的函数调用关系密切。栈上的内存分配和释放是自动进行的。
- 堆： 堆是一种动态分配的内存区域，用于存储程序运行时动态创建的数据。堆上的内存分配和释放由程序员手动管理，需要显式调用 new 或 malloc 进行分配，以及 delete 或 free 进行释放。
2. 内存管理：

-栈： 栈的内存管理是自动的，变量在超出其作用域时会被自动销毁，无需程序员手动释放。栈上的内存分配和释放是高效的，但大小有限。
-堆： 堆上的内存需要程序员手动管理，如果不释放堆上的内存，可能会导致内存泄漏。堆上的内存分配和释放相对较慢，并且可能会导致碎片化。
3. 大小：

- 栈： 栈通常较小，其大小受限于操作系统和编译器的设定。函数调用、局部变量和参数等短暂存在于栈上。
- 堆： 堆的大小相对较大，取决于可用的系统内存。动态分配的数据可以长时间存在于堆上。
4. 生命周期：

- 栈： 栈上的变量生命周期短暂，它们在函数调用结束时被销毁。栈上的数据是临时性的，不适合存储长期存在的数据。
- 堆： 堆上的数据生命周期可以很长，直到显式释放为止。动态分配的内存通常用于存储需要长时间存在的数据，例如动态创建的对象。
5. 访问速度：

- 栈： 栈上的内存访问速度较快，因为它是由硬件直接支持的。变量的访问和释放在栈上是高效的。
- 堆： 堆上的内存访问速度较慢，因为需要通过指针进行间接访问。动态分配的数据可能导致缓存未命中，从而降低访问速度。

## new和delete与malloc和free的主要区别是什么？
- new 和 delete 是 C++ 的运算符，可以为对象分配和释放内存，并调用构造函数和析构函数。这使得它们更加类型安全，适用于处理类和对象。
- malloc 和 free 是 C 语言的函数，它们对内存的分配和释放没有类型信息，因此需要在使用时显式进行类型转换，不会调用构造函数和析构函数。

## 内存对齐

### 内存对齐是什么?
内存对齐是指数据在内存中存储时相对于地址的对齐方式。在计算机体系结构中，特定类型的数据通常需要按照一定的字节边界对齐，以便于处理器能够高效地访问这些数据。

### 为什么要内存对齐?
1. 移植原因: 不是所有的硬件平台都能访问任意内存地址上的任意数据; 某些硬件平台只能在某些地址处取某些特定类型的数据, 否则抛出硬件异常.
2. 效率, 减小不必要的CPU开销.

## 什么是内存碎片，如何避免？

内存碎片是指内存空间中已分配和未分配的部分交错排列，导致无法充分利用可用的内存。内存碎片分为两种主要类型：外部碎片和内部碎片。

1. 外部碎片：

- 外部碎片是指已分配的内存块之间存在的未分配的小块内存。即使总体上有足够的可用内存，但是这些小块的分布可能不连续，导致大块的内存无法被分配。
2. 内部碎片：

- 内部碎片是指已分配的内存块中未被使用的部分。这主要发生在动态分配的内存中，当分配的内存大于实际需要的大小时，剩余的部分被视为内部碎片。