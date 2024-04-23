//
// Created by lewisfff.wang on 2024/4/23.
//

#ifndef LRUCACHE_SQUEUE_H
#define LRUCACHE_SQUEUE_H

template <class T, int MaxLength>
class Squeue {
private:
  bool inited_; // 队列被初始化标志
  T data_[MaxLength];
  int head_;
  int tail_;
  int size_;

  Squeue(const Squeue&) = delete;
  Squeue &operator=(const Squeue&) = delete;

public:
  Squeue(){Init();}

  // 循环队列的初始化操作
  // 注意：如果用于共享内存的队列，不会调用构造函数，必须调用此函数初始化 ？
  void Init(){
    if(inited_ == false) {
      head_ = 0;
      tail_ = MaxLength - 1; // 为了写代码方便 ？
      size_ = 0;
      memset(m_data, 0, sizeof(data_));
      inited_ = true;
    }
  }
  
  bool push(const T& ee){
    if(full() == true) {
      cout << "循环队列已满，入队失败" << std::endl;
      return false;
    }
    tail_ = (tail_ + 1) % MaxLength;
    data_[tail_] == ee;
    size_++;
    return true;
  }

  int size() {
    return size_;
  }

  bool empty() {
    return size_ == 0;
  }

  bool full() {
    return size_ == MaxLength;
  }

  T& front() {
    return data_[head_];
  }

  bool pop() {
    if(empty()) return false;
    head_ = (head_ + 1) % MaxLength;
    size_--;
    return true;
  }
};

#endif // LRUCACHE_SQUEUE_H