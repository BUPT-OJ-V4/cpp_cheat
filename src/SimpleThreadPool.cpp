//
// Created by liuwei on 17/4/28.
//

#include "SimpleThreadPool.h"


std::string username="oj", password="";

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
        //boost::shared_ptr<boost::thread> t()
        mThreadGroup.push_back(new boost::thread(boost::bind(&SimpleThreadPool::Run, this, i)));
    }
}

void SimpleThreadPool::Push(WorkItermPtr task) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mTaskQue.push(task);
    mCond.notify_one();
}

void SimpleThreadPool::Wait() {
    for(size_t i = 0; i < mThreadGroup.size(); i ++) {
        mThreadGroup[i]->join();
        delete mThreadGroup[i];
    }
}

