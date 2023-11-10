//
// Created by 10070 on 11/7/2023.
//
#include <iostream>
#include <mutex>
#include <thread>

void DannyWrite(std::string &blackboard)
{
    blackboard += "My";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " name";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " is";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " Danny\n";
}

void PeterWrite(std::string &blackboard)
{
    blackboard += "My";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " name";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " is";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3));
    blackboard += " Peter\n";
}

void DannyWriteWithMutex(std::mutex &mutex, std::string &blackboard)
{
    DannyWrite(blackboard);
}

void PeterWriteWithMutex(std::mutex &mutex, std::string &blackboard)
{
    // std::unique_lock has destructor that guarantee the mutex will be unlocked
    // no matter what happens in the function,
    // specifically an exception or a mistake made by the programmer
    std::unique_lock<std::mutex> lock(mutex);
    PeterWrite(blackboard);
}

int main()
{
    std::string blackboard;
    std::mutex mutex;
    // create a thread in cxx11
    std::thread DannyThread(DannyWriteWithMutex, std::ref(mutex),
                            std::ref(blackboard)); // after created, execute immediately
    std::thread PeterThread(PeterWriteWithMutex, std::ref(mutex), std::ref(blackboard));
    DannyThread.join();
    PeterThread.join();
    std::cout << blackboard << std::endl;
}