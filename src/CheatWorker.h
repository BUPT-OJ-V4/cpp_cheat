//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_CHEATWORKER_H
#define CHEAT_CHEATWORKER_H
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <queue>
#include <stdexcept>


typedef boost::function<void(void)> Task;

class TaskQueue : boost::noncopyable {
private:
    std::queue<Task> _task_que;
    boost::mutex _mutex;
    boost::condition_variable_any _cond;
public:
    TaskQueue() {
        closed = false;
    }
    void add_task(const Task& task);
    Task pop_task();
    int size();
    volatile bool closed;
};

class CheatWorker : boost::noncopyable{
private:
    TaskQueue _task_queue;
    boost::thread_group _thread_group;
    int _thread_num;
    volatile bool is_run;
    void run();

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
