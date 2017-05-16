//
// Created by liuwei on 17/4/28.
//

#include "CheatWorker.h"

void TaskQueue::add_task(const Task &task) {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    this->_task_que.push(task);
    this->_cond.notify_one();
}

Task* TaskQueue::pop_task() {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    if (_task_que.empty()) {
        if (this->closed) return nullptr;
        _cond.wait(lock);
    }
    Task* task = new Task(_task_que.front());
    //boost::unique_lock<boost::mutex> lock2(this->_mutex2);
    this->_task_que.pop();
    return task;
}

int TaskQueue::size() {
    return (int)this->_task_que.size();
}

void CheatWorker::run() {
    std::cout << "start a thread" << std::endl;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    // 初始化驱动
    driver = sql::mysql::get_mysql_driver_instance();
    // 建立链接
    con = driver->connect("tcp://10.105.240.51:3306", "oj", "");
    con->setAutoCommit(false);
    state = con->createStatement();
    //std::cout << "commit mode: " << con->getAutoCommit() << std::endl;
    state->execute("use oj");
    try{
         for(int idx = 0; is_run; idx ++) {
             if (_task_queue.closed && _task_queue.size() == 0) break;
             Task* task = this->_task_queue.pop_task();
             if (task == nullptr) continue;
             Res t = (*task)();
        }
        std::cout << "end thread " << std::endl;
        delete state;
        delete con;
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
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
    std::cout << "end all thread" << std::endl;
}

