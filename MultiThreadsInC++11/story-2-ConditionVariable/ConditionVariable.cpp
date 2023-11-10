// Wait efficiently and safely

#include <iostream>
#include <mutex>

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

    std::mutex &get_mutex()
    {
        return _mutex;
    }

    T &Acquire(std::unique_lock<std::mutex> &lock)
    {
        if (lock.mutex() != &_mutex)
        {
            throw std::logic_error("Mutexes do not match");
        }
        return *_resource;
    }

  private:
    T *_resource;
    std::mutex _mutex;
};

struct StockBlackboard
{
    float price;
    const std::string name;
    StockBlackboard(const std::string stockName, const float stockPrice = 0) : name(stockName), price(stockPrice)
    {
    }
};

using StockSafe = MutexSafe<StockBlackboard>;

void PeterUpdateStock_Notify(StockSafe &safe, std::condition_variable &conditionVariable)
{
    for (int i = 0; i < 4; ++i)
    {
        {
            std::unique_lock<std::mutex> lock(safe.get_mutex());
            StockBlackboard &stock = safe.Acquire(lock);
            if (i == 2)
                stock.price = 99;
            else
                stock.price = abs(rand() % 100);
            std::cout << "Peter updated stock to " << stock.price << std::endl;
            if (stock.price > 90)
            {
                lock.unlock();
                std::cout << "Peter notified Danny at price $" << stock.price << std::endl;
                conditionVariable.notify_one();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                lock.lock();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
    }
}

void DannyWait_ReadStock(StockSafe &safe, std::condition_variable &conditionVariable)
{
    std::unique_lock<std::mutex> lock(safe.get_mutex());
    std::cout << "Danny is waiting for the right price to sell..." << std::endl;
    conditionVariable.wait(lock);
    // this line did three things:
    // 1. unlock the mutex
    // 2. call wait on the condition variable
    // 3. lock the mutex again
    StockBlackboard &stock = safe.Acquire(lock);
    if (stock.price > 90)
        std::cout << "Danny sell at: $" << stock.price << std::endl;
    else
    {
        std::cout << "False alarm at $" << stock.price << " wait again..." << std::endl;
        conditionVariable.wait(lock);
        StockBlackboard &stock = safe.Acquire(lock);
        if (stock.price > 90)
            std::cout << "Danny sell at: $" << stock.price << std::endl;
    }
}

void TestConditionVariable()
{
    StockSafe safe (new StockBlackboard("APPL",30));
    std::condition_variable priceCondition;
    std::thread DannyThread(DannyWait_ReadStock, std::ref(safe), std::ref(priceCondition));
    std::thread PeterThread( PeterUpdateStock_Notify, std::ref(safe), std::ref(priceCondition));
    PeterThread.join();
    DannyThread.join();
}

int main()
{
    TestConditionVariable();
    return 0;
}