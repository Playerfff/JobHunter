// How to use this design pattern to deal with the traffic spike?
// the real problem that bothers us is not that we really have so much traffic to deal
// but is we have to deal with two challenging problems at once:
// 1. traffic spike is unpredictable
// 2. we only have limited resource to deal with these traffic

// the core of this problem is a queue and a pool

#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
using namespace std;

template <class T> class MutexSafe
{
  public:
    MutexSafe(T *resource) : _resource(resource)
    {
    }
    ~MutexSafe()
    {
        delete _resource;
    }
    void lock()
    {
        _mtx.lock();
    }
    void unlock()
    {
        _mtx.unlock();
    }
    bool try_lock()
    {
        return _mtx.try_lock();
    }
    mutex &Mutex()
    {
        return _mtx;
    }
    T &Acquire(unique_lock<MutexSafe<T>> &lock)
    {
        MutexSafe<T> _safe = lock.mutex();
        if (&(_safe.Mutex()) != &_mtx) // not the mutex from the safe that own the resource
        {
            throw runtime_error("wrong lock object passed to Acquire function.");
        }
        return *_resource;
    }
    T &Acquire(unique_lock<mutex> &lock)
    {
        if (lock.mutex() != &_mtx)
        {
            throw runtime_error("wrong lock object passed to Acquire function.");
        }
        return *_resource;
    }

  private:
    mutex _mtx;
    T *_resource;
    T *operator->()
    {
    }
    T &operator&()
    {
    }
};

template <class MsgType> class MsgQueue
{
  public:
    explicit MsgQueue(size_t size = 3) : _size(size)
    {
    }
    void Enqueue(MsgType msg)
    {
        unique_lock<mutex> lock(_mtx);
        if (_queue.size() >= _size)
        {
            _enqCv.wait(lock, [this] { return this->_queue.size() < this->_size; });
        }
        _queue.emplace(msg);
        _deqCv.notify_one();
    }
    MsgType &Dequeue()
    {
        unique_lock<mutex> lock(_mtx);
        if (_queue.empty())
        {
            _deqCv.wait(lock, [this] { return !this->_queue.empty(); });
        }
        MsgType &msg = _queue.front();
        _queue.pop();
        _enqCv.notify_one();
        return msg;
    }
    size_t size()
    {
        unique_lock<mutex> lock(_mtx);
        return _queue.size();
    }

  private:
    queue<MsgType> _queue;
    mutex _mtx;
    condition_variable _enqCv;
    condition_variable _deqCv;
    size_t _size;
};

struct CustomerTask
{
    string _task;
    float _money;
    CustomerTask() = default;
    CustomerTask(const CustomerTask &cp) = default;
    void ExecuteTask() const
    {
        if (_money > 0)
            cout << "Task " << _task << " is executed at $" << _money << endl;
        else
            cout << "Bank closed because the price is $" << _money << endl;
    }
};

using TaskQueueType = MsgQueue<CustomerTask>;
using TaskQueueSafe = MutexSafe<TaskQueueType>;

class ThreadPool
{
  public:
    void ExecuteTask()
    {
        while (true)
        {
            auto task = _taskQueue.Dequeue();
            task.ExecuteTask();
            if (task._money < 0)
            {
                _threadPoolStop = true;
                _taskQueue.Enqueue(task);
            }
            if (_threadPoolStop)
            {
                cout << "thread finished!" << endl;
                break;
            }
        }
        // the sleep function simulates that the task takes a while to finish
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    }
    explicit ThreadPool(TaskQueueType &taskQueue, size_t size = 3) : _size(size), _taskQueue(taskQueue)
    {
        for (int i = 0; i < _size; ++i)
        {
            _workerThreads.push_back(new thread(&ThreadPool::ExecuteTask, this));
        }
    }
    ~ThreadPool()
    {
        for (auto threadObj : _workerThreads)
        {
            if (threadObj->joinable())
            {
                threadObj->join();
                delete threadObj;
            }
        }
    }

  private:
    size_t _size;
    vector<thread *> _workerThreads;
    TaskQueueType &_taskQueue;
    bool _threadPoolStop = false;
};

void TestLeaderFollower()
{
    TaskQueueType taskQueue(5);
    ThreadPool pool(taskQueue, 3);
    // the sleep function simulates the situation that all
    // worker threads are waiting for the empty message queue at the beginning
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 0; i < 10; i++)
    {
        CustomerTask task;
        task._money = i + 1;
        if (task._money > 5)
            task._task = "deposit $";
        else
            task._task = "withdraw $";
        taskQueue.Enqueue(task);
    }
    CustomerTask bankClosedTask;
    bankClosedTask._task = "Bank Closed!";
    bankClosedTask._money = -1;
    taskQueue.Enqueue(bankClosedTask);
}

int main()
{
    TestLeaderFollower();
    return 0;
}