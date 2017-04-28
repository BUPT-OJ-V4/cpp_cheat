//
// Created by liuwei on 17/4/28.
//

#include "CheatWorker.h"

void TaskQueue::add_task(const Task &task) {
    boost::unique_lock<boost::mutex> lock(this->_mutex);
    this->_task_que.push(task);
    this->_cond.notify_one();
}

Task TaskQueue::pop_task() {
    boost::unique_lock<boost::mutex> lock(this->_mutex);
    if (_task_que.empty()) {
        _cond.wait(lock);
    }
    Task task(_task_que.front());
    this->_task_que.pop();
    return task;
}

int TaskQueue::size() {
    return this->_task_que.size();
}

void CheatWorker::run() {
    while(is_run) {
        if (_task_queue.closed && _task_queue.size() == 0) break;
        Task task = this->_task_queue.pop_task();
        task();
    }
}

void CheatWorker::close() {
    _task_queue.closed = true;
}

void CheatWorker::start(){
    if (_thread_num <= 0) return;
    is_run = true;
    for (int i = 0; i < _thread_num; i ++) {
        //boost::shared_ptr<boost::thread> t()
        //boost::thread *t = new boost::thread(boost::bind(&CheatWorker::run, this));
        //t->interrupt();
        _thread_group.add_thread(new boost::thread(boost::bind(&CheatWorker::run, this)));
    }
}

void CheatWorker::stop() {
    is_run = false;
}

void CheatWorker::add_task(const Task& task) {
    if (_task_queue.closed) {
        throw std::logic_error("can not add task after close");
    }
    _task_queue.add_task(task);
}

void CheatWorker::wait() {
    _thread_group.join_all();
}

