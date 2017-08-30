//
// Created by liuwei on 17/4/28.
//

#include "ThreadPool.h"
#include "cheat_algorithm.h"

std::string username="oj", password="";

std::string to_str(Res t) {
    return "(" + std::to_string(t.first.first) + ", " + std::to_string(t.first.second) + ") = " + std::to_string(t.second);
}

void ThreadPool::close() {
    _closed = true;
    _cond.notify_all();
}

int ThreadPool::Pop(WorkItermPtr& t) {
    boost::unique_lock<boost::mutex> lock(_mutex);
    if (_taskQue.empty()) {
        if (_closed) return 0;
        this->_cond.wait(lock);
    }
    if (this->_taskQue.empty()) return 0;
    t = _taskQue.front();
    _taskQue.pop();
    return 1;
}

size_t ThreadPool::Size() {
    boost::unique_lock<boost::mutex> lock(_mutex);
    return _taskQue.size();
}

void ThreadPool::Run(int num) {
    type ans;
    for(int idx = 0; ; idx ++) {
        WorkItermPtr task;
        int status = Pop(task);
        if (status == 0) break;
        task->run(num);
    }
}

void ThreadPool::Start(){
    if (_thread_num <= 0) return;
    for (int i = 0; i < _thread_num; i ++) {
        //boost::shared_ptr<boost::thread> t()
        mThreadGroup.push_back(new boost::thread(boost::bind(&ThreadPool::run, this, i)));
    }
}

void ThreadPool::Push(WorkItermPtr task) {
    boost::unique_lock<boost::mutex> lock(_mutex);
    _taskQue.push(task);
    _cond.notify_one();
}

void ThreadPool::Wait() {
    _threadGroup.join_all();
}

