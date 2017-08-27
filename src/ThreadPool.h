//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_CHEATWORKER_H
#define CHEAT_CHEATWORKER_H
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
public:
    typedef std::shared_ptr<WorkIterm> WorkItermPtr;
private:
    std::queue<WorkItermPtr> _taskQue;
    boost::mutex _mutex;
    boost::condition_variable_any _cond;
    boost::thread_group _threadGroup;
    int _thread_num;
    volatile bool _closed;
private:
    void run(int num);
    int popTask(WorkItermPtr& task);
public:
    ThreadPool(int num) {
        this->_thread_num = num;
    }
    ~ThreadPool() {
        wait();
    }
    void start();
    void close();
    void add_task(WorkItermPtr task);
    size_t size();
    void wait();
};

#endif //CHEAT_CHEATWORKER_H
