//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_THREAD_POOL_H
#define CHEAT_THREAD_POOL_H
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <string>
#include "WorkIterm.h"

class ThreadPool : boost::noncopyable
{
public:
    ThreadPool(int num)
        : mThreadCount(num){
    }
    ~ThreadPool() {
        Wait();
    }
    void Start();
    void Close();
    void Push(WorkItermPtr task);
    size_t Size();
    void Wait();
private:
    void Run(int num);
    int Pop(WorkItermPtr& task);
private:
    std::queue<WorkItermPtr> mTaskQue;
    boost::mutex mMutex;
    boost::condition_variable_any mCond;
    std::vector<boost::thread> mThreadGroup;
    int mThreadCount;
    volatile bool mClosed;    
};

#endif //CHEAT_THREAD_POOL_H
