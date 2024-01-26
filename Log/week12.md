# week12 周报

## 进度

- C++技术栈重点思考.xmind
    - stl
    - rpc
    - linux 常用命令
    - 性能优化 - perl生成火焰图
    - 内存调查 - asan
    - 编译工具
    - gdb

- cppcon - [How to Adopt Modern C++17 into Your C++ Code : Build 2018](https://www.youtube.com/watch?v=UsrHQAzSXkA)

- docker常用用法


## 下周规划

- C++技术栈重点思考.xmind
    - 侯捷内存管理课程
- 2024/1/21 港大面试

## 下下周规划
- C++技术栈重点思考.xmind
    - levelDB

## 问题

- GDB基础命令学了, 但感觉日常用处不大, 日后有使用需求再来复习

## 疑惑
1. 想向华仔确认一点: docker可以代替虚拟机吗? 我打算用docker制作一个centos7的image做日常开发使用, 但是容器每次重启都会丢失数据, 我应该怎么做才能保存数据呢? 还是说docker本身就不应该代替虚拟机, 还是用vmware?

A: 不能代替虚拟机的，你还是用虚拟机来开发吧，虚拟机的话有完整的一些功能的，就是比如说你可以用虚拟机的一些打快照的一些功能，然后比如说你比如说你把操作系统搞坏了，你可以直接通过坏照来回滚。