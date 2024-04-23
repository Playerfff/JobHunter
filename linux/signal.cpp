// linux的信号
// 1. signal是软件中断/进程之间相互传递消息的一种方法,用于通知进程发生了事件, 但是和qt中的信号有区别的是: linux的信号不能传递数据
// 通过kill命令传递信号
// 2. signal的种类
// 3. signal的处理
// 进程对信号的处理方法有三种:
  // 1.系统默认处理操作是:终止进程
  // 2. 设置中断的处理函数:收到信号后,由该函数来处理 -> 通过signal()函数来处理
  // 3. 忽略某个信号,对该信号不做任何处理


// alarm函数 -> signal 14
// kill函数 int kill(pid_t pid, int sig); 将sig指定的信号给pid号进程
  // pid > 0 将信号传给进程号为pid的进程
  // pid = 0 将信号传给和当前进程相同进程组的所有进程，常用于父进程给子进程发送信号，注意：发送信号者进程也会收到自己发送出的信号
  // pid = -1 将信号广播给系统内所有进程，例如系统关机时，会向所有的登录窗口广播关机信息

#include <signal.h>
#include <iostream>

void func(int signal_num) {
  std::cout << "收到了信号:" << signal_num << std::endl;
  signal(signal_num, SIG_DFL); // 恢复信号的默认处理方法
}

int main(int argc, char** argv) {
  signal(1, func);
  signal(15, func);
  while(true) {
    std::cout << "processing..." << std::endl;
    sleep(5);
  }
  return 0;
}