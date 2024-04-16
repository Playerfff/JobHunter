// demo3.cpp，本程序演示了 Linux 底层文件的操作 - 创建文件并写入数据。
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;

// linux 一切皆为文件, socket 也是文件，也就意味着send和receive可以被替换为write和read
// linux 文件描述符的分配规则：
  // /proc/进程id/fd目录中，存放了每个进程打开的fd(文件描述符)
  // linux进程默认打开了三个文件描述符
    // 0 -> 标准输入(键盘) -> cin
    // 1 -> 标准输出(显示器) -> cout / printf
    // 2 -> 标准错误(显示器) -> cerr

int main() {
  int fd; // 定义一个文件描述符/文件句柄。
  // 打开文件，注意，如果创建后的文件没有权限，可以手工授权 chmod 777 data.txt。
  fd = open("data.txt", O_CREAT | O_RDWR | O_TRUNC);
  if (fd == -1) {
    perror("open(data.txt)");
    return -1;
  }
  printf("文件描述符 fd=%d\n", fd);
  char buffer[1024];
  strcpy(buffer, "我是一只傻傻鸟。\n");
  if (write(fd, buffer, strlen(buffer)) == -1) // 把数据写入文件。
  {
    perror("write()");
    return -1;
  }
  close(fd); // 关闭文件。
}