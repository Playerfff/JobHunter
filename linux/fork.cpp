#include <iostream>
#include <bits/stdc++.h>

int main() {
  int bh = 8;
  std::string msg = "birdy";

  pid pid = fork(); // 从这里父子分道扬镳
                    // 子进程获得父进程数据空间、堆、栈的副本，不是和父进程共享
                    // 父子进程共享文件描述符，文件只有一个，父子进程共享同一个文件偏移量

  if(pid > 0) {
    // 父进程执行这段代码
    std::cout << "I am the father." << std::endl;
  }else if(pid == 0) {
    // 子进程执行这段代码
    std::cout << "I am the son." << std::endl;
  } else {
    std::cout << "fork failed!" << std::endl;
  }
}