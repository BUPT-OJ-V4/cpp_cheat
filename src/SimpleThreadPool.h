//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_SIMPLE_THREAD_POOL_H
#define CHEAT_SIMPLE_THREAD_POOL_H
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>

#include <string>
#include "WorkIterm.h"

class SimpleThreadPool
{
public:
    SimpleThreadPool(int num)
        : mThreadCount(num)
        , mClosed(false)
    {
    }
    ~SimpleThreadPool() {
        Wait();
    }
    void Start();
    void Close();
    void Push(const WorkItermPtr& task);
    size_t Size();
    void Wait();
private:
    void Run(int num);
    int Pop(WorkItermPtr& task);
private:
    std::queue<WorkItermPtr> mTaskQue;
    boost::mutex mMutex;
    boost::condition_variable_any mCond;
    boost::thread_group mThreadGroup;
    int mThreadCount;
    volatile bool mClosed;    
};

#endif //CHEAT_SIMPLE_THREAD_POOL_H
