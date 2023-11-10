//
// Created by 10070 on 11/7/2023.
//

// What is a Thread Safe Message Queue?
// 1. Producer and Consumer can mutual exclusively manipulate message queue at the same time -> we need a mutex to
// protect the queue
// 2. When the queue is not empty, the consumer have to wake to consume the message -> we need a condition variable to
// wait for the producer to produce a message
// 3. When the queue is full, the producer have to block to produce a message -> we need a condition variable to wait
// for the consumer to consume a message

#include <iostream>
#include <mutex>
#include <queue>

template <class MsgType> class MessageQueue
{

  public:
    MessageQueue(int limit = 3) : _limit(limit)
    {
    }

    void enqueue(MsgType &msg)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.size() >= _limit)
        {
            std::cout << "Queue is full, waiting for consumer to consume" << std::endl;
            _enqCv.wait(lock, [this] { return this->_queue.size() < this->_limit; });
        }
        _queue.push(msg);
        _deqCv.notify_one();
    }

    MsgType dequeue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_queue.empty())
        {
            std::cout << "Queue is empty, waiting for producer to produce" << std::endl;
            _deqCv.wait(lock, [this]{return !this->_queue.empty();});
        }
        MsgType msg = _queue.front();
        _queue.pop();
        _enqCv.notify_one();
        return msg;
    }

  private:
    std::queue<MsgType> _queue;
    std::mutex _mutex;              // to protect the MessageQueue -> 1
    std::condition_variable _enqCv; // to notify the producer to produce a message -> 3
    std::condition_variable _deqCv; // to notify the consumer to consume a message -> 2
    int _limit;
};