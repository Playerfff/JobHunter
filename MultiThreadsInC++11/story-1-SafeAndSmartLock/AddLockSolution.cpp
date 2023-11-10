//
// Created by 10070 on 11/7/2023.
// Story2 - Add Lock
//
#include <iostream>
#include <mutex>
#include <thread>

void DannyWrite(std::string &blackboard)
{
    blackboard += "My";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " name";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " is";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " Danny\n";
}

void PeterWrite(std::string &blackboard)
{
    blackboard += "My";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " name";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " is";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand()%3));
    blackboard += " Peter\n";
}

void DannyWriteWithMutex(std::mutex& mutex, std::string &blackboard)
{
    mutex.lock();
    DannyWrite(blackboard);
    mutex.unlock();
}

void PeterWriteWithMutex(std::mutex& mutex, std::string &blackboard)
{
    mutex.lock();
    PeterWrite(blackboard);
    mutex.unlock();
}

int main()
{
    std::string blackboard;
    std::mutex mutex;
    // create a thread in cxx11
    std::thread DannyThread(DannyWriteWithMutex, std::ref(mutex), std::ref(blackboard)); // after created, execute immediately
    std::thread PeterThread(PeterWriteWithMutex, std::ref(mutex), std::ref(blackboard));
    DannyThread.join();
    PeterThread.join();
    std::cout << blackboard << std::endl;
}