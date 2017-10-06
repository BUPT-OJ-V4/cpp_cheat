//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_SIMPLE_THREAD_POOL_H
#define CHEAT_SIMPLE_THREAD_POOL_H
#include <boost/thread.hpp>

#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>
#include <vector>
#include <string>

template <typename T>
class ThreadQueue
{
public:
    ThreadQueue()
    {
    }
    ~ThreadQueue() {
    }
    void Push(const T& task);
    int Pop(T& task);
    T Pop();
    size_t Size() {
        boost::unique_lock<boost::mutex> lock(mMutex);
        return mTaskQue.size();
    }
private:
    std::queue<T> mTaskQue;
    boost::mutex mMutex;
};

template <typename T>
int ThreadQueue<T>::Pop(T& t) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    if (mTaskQue.empty()) return 0;
    t = mTaskQue.front();
    mTaskQue.pop();
    return 1;
}

template <typename T>
T ThreadQueue<T>::Pop() {
    boost::unique_lock<boost::mutex> lock(mMutex);
    T t = mTaskQue.front();
    mTaskQue.pop();
    return t;
}

template <typename T>
void ThreadQueue<T>::Push(const T& task) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mTaskQue.push(task);
}

#endif //CHEAT_SIMPLE_THREAD_POOL_H
