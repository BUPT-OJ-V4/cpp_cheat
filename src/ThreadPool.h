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

typedef std::pair<std::pair<int, int>, double> Res;
//typedef long long Res;
typedef boost::function<Res(void)> Task;
extern std::string username, password;

class ThreadPool : boost::noncopyable{

private:
    std::queue<WorkItermPtr> mTaskQue;
    boost::mutex mMutex;
    boost::condition_variable_any mCond;
    vector<boost::Thread> mThreadGroup;
    int mThreadNum;
    volatile bool mClosed;
private:
    void Run(int num);
    int Pop(WorkItermPtr& task);
public:
    ThreadPool(int num)
        : mThreadNum(num){
    }
    ~ThreadPool() {
        wait();
    }
    void Start();
    void Close();
    void Push(WorkItermPtr task);
    size_t Size();
    void Wait();
};

#endif //CHEAT_THREAD_POOL_H
