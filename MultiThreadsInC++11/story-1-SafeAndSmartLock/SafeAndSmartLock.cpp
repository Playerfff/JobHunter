//
// Created by 10070 on 11/7/2023.
// Design Pattern 1 - Safe & Smart Lock
// To avoid the programmer do not follow the rules, we lock the shared resource into the safe
// ppl who want the shared resource have no choice but to use a smart lock to unlock th safe first
//
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

template <typename T> class MutexSafe
{
  private:
    mutex _mutex;
    T *_resource;
    T *operator->()
    {
    }
    T &operator&()
    {
    }

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
        _mutex.lock();
    }
    void unlock()
    {
        _mutex.unlock();
    }
    bool try_lock()
    {
        return _mutex.try_lock();
    }
    mutex &Mutex()
    {
        return _mutex;
    }

    // this is version 3 of Acquire
    //  it requires the passed in lock parameter is the safe object itself
    // thus it avoids the issue that an arbitrary lock can access the resource.
    // Also, it now requires the lock to be the same type of Safe that Acquire is.
    T &Acquire(unique_lock<MutexSafe<T>> &lock)
    {
        MutexSafe<T> *_safe = lock.mutex();
        if (&_safe->Mutex() != &_mutex)
        {
            throw "wrong lock object passed to Acquire function.\n";
        }
        return *_resource;
    }
    // This overloaded Acquire allows unique_lock<mutex> to be passed in as a parameter
    T &Acquire(unique_lock<mutex> &lock)
    {
        if (lock.mutex() != &_mutex)
        {
            throw "wrong lock object passed to Acquire function.\n";
        }
        return *_resource;
    }
};
void DannyWrite(string &blackboard)
{
    blackboard = +"My";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " name";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " is";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " Danny\n";
}
void PeterWrite(string &blackboard)
{
    blackboard += "My";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " name";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " is";
    this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " Peter\n";
}

typedef MutexSafe<string> Safe;
void SafeDannyWrite(Safe &safe)
{
    unique_lock<Safe> lock(safe);
    string &blackboard = safe.Acquire(lock);
    DannyWrite(blackboard);
}
void SafePeterWrite(Safe &safe)
{
    unique_lock<Safe> lock(safe);
    string &blackboard = safe.Acquire(lock);
    PeterWrite(blackboard);
}
void TestSafeSmartlock()
{
    Safe safe(new string());

    thread DannyThread(SafeDannyWrite, ref(safe));
    thread PeterThread(SafePeterWrite, ref(safe));
    DannyThread.join();
    PeterThread.join();

    unique_lock<Safe> lock(safe);
    string &blackboard = safe.Acquire(lock);
    cout << blackboard << endl;
}

int main()
{
    TestSafeSmartlock();
    return 0;
}