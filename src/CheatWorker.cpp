//
// Created by liuwei on 17/4/28.
//

#include "CheatWorker.h"
#include "cheat_algorithm.h"

std::string username="oj", password="";

std::string to_str(Res t) {
    return "(" + std::to_string(t.first.first) + ", " + std::to_string(t.first.second) + ") = " + std::to_string(t.second);
}

void TaskQueue::add_task(const Task &task) {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    this->_task_que.push(task);
    this->_cond.notify_one();
}

void TaskQueue::close() {
    this->closed = true;
    this->_cond.notify_all();
}

int TaskQueue::pop_task(Task & t) {
    boost::unique_lock<boost::mutex> lock(this->_mutex1);
    if (_task_que.empty()) {
        if (this->closed) return 0;
        this->_cond.wait(lock);
    }
    if (this->_task_que.empty()) return 0;
    t = _task_que.front();
    //boost::unique_lock<boost::mutex> lock2(this->_mutex2);
    this->_task_que.pop();
    return 1;
}

int TaskQueue::size() {
    return (int)this->_task_que.size();
}

void CheatWorker::run(int num) {
    std::cout << "start a thread" << std::endl;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", username, password);
    state = con->createStatement();
    state->execute("use oj");
    std::string sql = "INSERT INTO `cheat_record` (`problem_id`, `sub1_id`, `sub2_id`, `user1`, `user2`,`probability`) VALUES";
    int cnt = 0;
    for(int idx = 0; is_run; idx ++) {
        Task task;
        int status = this->_task_queue.pop_task(task);
        if (status == 0) break;
        Res t = task();
        if (cnt > 0) sql += ",";
        sql += "(" + this->problem_id + ", " + std::to_string(t.first.first) + "," + std::to_string(t.first.second);
        sql += ", '" + cheat::userinfo[t.first.first] + "', '" + cheat::userinfo[t.first.second] + "', " + std::to_string(t.second) + ")";
        cnt ++;
    }
    std::cout << sql << std::endl;
    if (cnt > 0) {
        sql += ";";
        bool ans = state->execute(sql);
        std::cout << ans << std::endl;
        //if (!ans) {
        //    std::cout <<"insert failed" << std::endl;
        //}
    }
    delete state;
    delete con;
}

void CheatWorker::close() {
    _task_queue.close();
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

