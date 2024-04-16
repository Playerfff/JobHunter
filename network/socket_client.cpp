/*
 * 程序名：demo1.cpp，此程序用于演示 socket 的客户端
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
  if (argc != 3) {
    cout << "Using:./demo1 服务端的 IP 服务端的端口\nExample:./demo1 "
            "192.168.101.139 5005\n\n ";
    return -1;
  }
  // 第 1 步：创建客户端的 socket。
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("socket");
    return -1;
  }
  // 第 2 步：向服务器发起连接请求。
  struct hostent *h; // 用于存放服务端 IP 的结构体。
  if ((h = gethostbyname(argv[1])) == 0) // 把字符串格式的 IP 转换成结构体。
  {
    cout << "gethostbyname failed.\n" << endl;
    close(sockfd);
    return -1;
  }
  struct sockaddr_in servaddr; // 用于存放服务端 IP 和端口的结构体。
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  memcpy(&servaddr.sin_addr, h->h_addr, h->h_length); // 指定服务端的 IP 地址。
  servaddr.sin_port = htons(atoi(argv[2])); // 指定服务端的通信端口。
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) !=
      0) // 向服务端发起连
    接清求。 {
      perror("connect");
      close(sockfd);
      return -1;
    }
  // 第 3
  // 步：与服务端通讯，客户发送一个请求报文后等待服务端的回复，收到回复后，再发下一个请求报文。
  char buffer[1024];
  for (int ii = 0; ii < 3; ii++) // 循环 3 次，将与服务端进行三次通讯。
  {
    int iret;
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "这是第%d 个超级女生，编号%03d。", ii + 1,
            ii + 1); // 生成请求报文内容。
    // 向服务端发送请求报文。
    if ((iret = send(sockfd, buffer, strlen(buffer), 0)) <= 0) {
      perror("send");
      break;
    }
    cout << "发送：" << buffer << endl;
    memset(buffer, 0, sizeof(buffer));
    // 接收服务端的回应报文，如果服务端没有发送回应报文，recv()函数将阻塞等待。
    if ((iret = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) {
      cout << "iret=" << iret << endl;
      break;
    }
    cout << "接收：" << buffer << endl;
    sleep(1);
  }
  // 第 4 步：关闭 socket，释放资源。
  close(sockfd);
}