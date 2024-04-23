// 多线程共享进程的地址空间，如果多个线程需要访问同一块内存，用全局变量即可
// 多进程中，每个进程的地址空间是独立的，不共享的，如果多个进程需要访问同一块内存，不能用全局变量，只能一同共享内存。

// shared_memory允许多个进程（不要求进程之间有血缘关系）访问同一块内存空间，是多个进程之间共享和传递数据最高效的方式。
// 进程可以将共享内存连接到他们自己的地址空间中
// 共享内存没有提供锁机制

// linux命令：ipcs -m查看共享内存

// linux提供了一组函数用于操作共享内存
// 1. 该函数用于创建/获取共享内存
  // int shmget(key_t key, size_t size, int shmflg);
  //                   |            |           |
  //           共享内存的key   共享内存的大小     共享内存的访问权限（同文件的访问权限）
  // return -1 -> fail
  //        >0 -> 共享内存的id

// 2. shmat函数 用于把共享内存链接到当前进程的地址空间
  // void *shmat(int shmid, const void *shmaddr, int shmflg)
  //                  |                    |
  // 由shmget()函数返回的共享内存标志   指定共享内存链接到当前进程中的地址位置，通常填0，表示让os来选择共享内存的地址

// 3. shmdt函数 用于将共享内存从当前进程中分离
  // int shmdt(const void *shmaddr);
  //                          |
  //                    shmat()函数返回的地址


struct stgirl {
  int no;
  char name[51];
}; // 共享内存只能用栈区的，也就意味着不能用stl容器（在堆区分配）

int main(int grac, int **argv) {
  // 1. create shared memory
  int shmid = shmget(0x5005, sizeof(stgirl), 0640|IPC_CREAT);
  if (shmid == -1) {
    std::cout << "shmget failed" << std::endl;
  }
  std::cout << "shmid = " << shmid << std::endl;

  // 2. link shared memory
  stgirl* ptr = (stgirl*) shmat(shmid, 0, 0);
  if (ptr == (void*)-1)
  {
    std::cout << "shmat failed" << std::endl;
  }

  // 3. use shared memory
  std::cout << "原值： no = " << ptr->no << ", name = " << ptr->name << std::endl;
  ptr->no = atoi(argv[1]);
  strcpy(ptr->name, argv[2]);
  std::cout << "新值： no = " << ptr->no << ", name = " << ptr->name << std::endl;

  // 4. 把共享内存从当前进程中分离
  shmdt(ptr);


  // 5. 删除共享内存
  if(shmctl(shmid, IPC_RMID, 0) == -1){
    std::cout << "shmctl fail" << std::endl;
  }
}