#include <time.h>
#include <bits/stdc++.h>

int main()
{
  time_t now1 = time(0);
  std::cout << "current time is: " << now1 << std::endl;

  // time_t是一个uint64_t的数字
  // tm是一个人类可读的结构体
  // localtime_r和mktime可以把两者互相转化
  tm now1_tm;
  localtime_r(&now1, &now1_tm);

  // 需求1: 对当前时间加30分钟
  // 思路:
  // 1. 解析当前字符串格式的时间,转换成tm结构体
  // 2. 用mktime()函数把tm结构体转换成time_t时间
  // 3. 把time_t时间加30*60秒
  // 4. 原路返回



  // sleep(秒)
  // usleep(微秒)
}