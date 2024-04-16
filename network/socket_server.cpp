/*
 * 程序名：demo2.cpp，此程序用于演示 socket 通信的服务端
 */
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout
        << "Using:./demo2 通讯端口\nExample:./demo2 5005\n\n"; // 端口大于
                                                               // 1024，不与其它的重复。
    cout << "注意：运行服务端程序的 Linux 系统的防火墙必须要开通 5005 端口。\n";
    cout << " 如果是云服务器，还要开通云平台的访问策略。\n\n";
    return -1;
  }
  // 第 1 步：创建服务端的 socket。
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    perror("socket");
    return -1;
  }
  // 第 2 步：把服务端用于通信的 IP 和端口绑定到 socket 上。
  struct sockaddr_in servaddr; // 用于存放服务端 IP 和端口的数据结构。
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; // 指定协议。
  servaddr.sin_addr.s_addr =
      htonl(INADDR_ANY); // 服务端任意网卡的 IP 都可以用于通讯。
  servaddr.sin_port =
      htons(atoi(argv[1])); // 指定通信端口，普通用户只能用 1024 以上的端口。
  // 绑定服务端的 IP 和端口。
  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
    perror("bind");
    close(listenfd);
    return -1;
  }
  // 第 3 步：把 socket 设置为可连接（监听）的状态。
  if (listen(listenfd, 5) != 0) {
    perror("listen");
    close(listenfd);
    return -1;
  }
  // 第 4 步：受理客户端的连接请求，如果没有客户端连上来，accept()函数将阻塞等待。
  int clientfd = accept(listenfd, 0, 0);
  if (clientfd == -1) {
    perror("accept");
    close(listenfd);
    return -1;
  }
  cout << "客户端已连接。\n";
  // 第 5 步：与客户端通信，接收客户端发过来的报文后，回复 ok。
  char buffer[1024];
  while (true) {
    int iret;
    memset(buffer, 0, sizeof(buffer));
    // 接收客户端的请求报文，如果客户端没有发送请求报文，recv()函数将阻塞等待。
    // 如果客户端已断开连接，recv()函数将返回 0。
    if ((iret = recv(clientfd, buffer, sizeof(buffer), 0)) <= 0) {
      cout << "iret=" << iret << endl;
      break;
    }
    cout << "接收：" << buffer << endl;
    strcpy(buffer, "ok"); // 生成回应报文内容。
    // 向客户端发送回应报文。
    if ((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0) {
      perror("send");
      break;
    }
    cout << "发送：" << buffer << endl;
  }
  // 第 6 步：关闭 socket，释放资源。
  close(listenfd); // 关闭服务端用于监听的 socket。
  close(clientfd); // 关闭客户端连上来的 socket。
}