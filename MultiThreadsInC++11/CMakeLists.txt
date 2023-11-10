cmake_minimum_required(VERSION 3.26)
project(MultiThreadsInCXX11)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(story-1-SafeAndSmartLock)
add_subdirectory(story-2-ConditionVariable)
add_subdirectory(story-3-ThreadSafeMessageQueue)
add_subdirectory(story-4-LeaderAndFollower)
