//
// Created by liuwei on 17/4/28.
//

#include "CheatWorker.h"

std::string to_str(Res t) {
    return "(" + std::to_string(t.first.first) + ", " + std::to_string(t.first.second) + ") = " + std::to_string(t.second);
}

void TaskQueue::add_task(const Task &task) {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    this->_task_que.push(task);
    this->_cond.notify_one();
}

int TaskQueue::pop_task(Task & t) {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    if (_task_que.empty()) {
        if (this->closed) return -1;
        return 0;
    }
    t = _task_que.front();
    //boost::unique_lock<boost::mutex> lock2(this->_mutex2);
    this->_task_que.pop();
    return 1;
}

int TaskQueue::size() {
    return (int)this->_task_que.size();
}

void CheatWorker::run(int num) {
    /*
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
    */
    try{
        int cnt = 0;
        for(int idx = 0; is_run; idx ++) {
            Task task;
            int status = this->_task_queue.pop_task(task);
            if (status == 0) {
                boost::this_thread::sleep(boost::posix_time::seconds(1));
                continue;
            }
            if (status == -1) break;
            Res t = task();
            if (cnt % 1000 == 0) {
                std::cout << to_str(t) << std::endl;
            }
            cnt ++;
        }
        boost::this_thread::sleep(boost::posix_time::seconds(3 * num));
        std::cout << "end thread:"<< num << cnt << std::endl;
        //delete state;
        //delete con;
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
        _thread_group.add_thread(new boost::thread(boost::bind(&CheatWorker::run, this, i)));
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

