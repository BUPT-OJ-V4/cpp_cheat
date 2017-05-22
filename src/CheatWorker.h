//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_CHEATWORKER_H
#define CHEAT_CHEATWORKER_H
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread_pool.hpp>
#include <functional>
#include <queue>
#include <deque>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <string>
#include <cppconn/statement.h>

typedef std::pair<std::pair<int, int>, double> Res;
//typedef long long Res;
typedef boost::function<Res(void)> Task;


class TaskQueue : boost::noncopyable {
private:
    std::queue<Task> _task_que;
    boost::mutex _mutex1;
    boost::condition_variable_any _cond;
public:
    TaskQueue() {
        closed = false;
    }
    void add_task(const Task& task);
    int pop_task(Task& task);
    int size();
    volatile bool closed;
};

class CheatWorker : boost::noncopyable{
private:
    TaskQueue _task_queue;
    boost::thread_group _thread_group;
    int _thread_num;
    volatile bool is_run;
    void run(int num);

public:
    CheatWorker(int num) {
        this->_thread_num = num;
        this->is_run = false;
    }
    void stop();
    ~CheatWorker() {
        stop();
    }
    void start();
    void close();
    void add_task(const Task& task);
    void wait();
};

#endif //CHEAT_CHEATWORKER_H
