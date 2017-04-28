//
// Created by liuwei on 17/4/28.
//

#include "CheatWorker.h"

void CheatWorker::add_task(const Task &task) {
    boost::unique_lock<boost::mutex> lock(this->_mutex);
    this->_task_que.push(task);
}