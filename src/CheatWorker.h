//
// Created by liuwei on 17/4/28.
//

#ifndef CHEAT_CHEATWORKER_H
#define CHEAT_CHEATWORKER_H
#include <boost/thread/thread_pool.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <queue>


typedef boost::function<void(void)> Task;

class CheatWorker : boost::noncopyable {
private:
    std::queue<Task> _task_que;
    boost::mutex _mutex;
public:
    void add_task(const Task& task);
};


#endif //CHEAT_CHEATWORKER_H
