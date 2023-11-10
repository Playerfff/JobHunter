//
// Created by 10070 on 11/7/2023.
// Story 1 - Resource Race
//

#include <iostream>
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

int main()
{
    std::string blackboard;
    // create a thread in cxx11
    std::thread DannyThread(DannyWrite, std::ref(blackboard)); // after created, execute immediately
    std::thread PeterThread(PeterWrite, std::ref(blackboard));
    DannyThread.join();
    PeterThread.join();
    std::cout << blackboard << std::endl;
}
