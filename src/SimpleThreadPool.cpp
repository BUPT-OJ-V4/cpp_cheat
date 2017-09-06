//
// Created by liuwei on 17/4/28.
//

#include "SimpleThreadPool.h"



void SimpleThreadPool::Close() {
    mClosed = true;
    mCond.notify_all();
}

int SimpleThreadPool::Pop(WorkItermPtr& t) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    if (mTaskQue.empty()) {
        if (mClosed) return 0;
        this->mCond.wait(lock);
    }
    if (this->mTaskQue.empty()) return 0;
    t = mTaskQue.front();
    mTaskQue.pop();
    return 1;
}

size_t SimpleThreadPool::Size() {
    boost::unique_lock<boost::mutex> lock(mMutex);
    return mTaskQue.size();
}

void SimpleThreadPool::Run(int num) {
    for(int idx = 0; ; idx ++) {
        WorkItermPtr task;
        int status = Pop(task);
        if (status == 0) break;
        task->run(num);
    }
}

void SimpleThreadPool::Start(){
    if (mThreadCount <= 0) return;
    for (int i = 0; i < mThreadCount; i ++) {
        mThreadGroup.create_thread(boost::bind(&SimpleThreadPool::Run, this, i));
    }
}

void SimpleThreadPool::Push(const WorkItermPtr& task) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mTaskQue.push(task);
    mCond.notify_one();
}

void SimpleThreadPool::Wait() {
    mThreadGroup.join_all();
}

