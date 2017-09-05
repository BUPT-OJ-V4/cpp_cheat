//
// Created by liuwei on 17/4/28.
//

#include "ThreadPool.h"


std::string username="oj", password="";

void ThreadPool::Close() {
    mClosed = true;
    mCond.notify_all();
}

int ThreadPool::Pop(WorkItermPtr& t) {
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

size_t ThreadPool::Size() {
    boost::unique_lock<boost::mutex> lock(mMutex);
    return mTaskQue.size();
}

void ThreadPool::Run(int num) {
    for(int idx = 0; ; idx ++) {
        WorkItermPtr task;
        int status = Pop(task);
        if (status == 0) break;
        task->run(num);
    }
}

void ThreadPool::Start(){
    if (mThreadCount <= 0) return;
    for (int i = 0; i < mThreadCount; i ++) {
        //boost::shared_ptr<boost::thread> t()
        mThreadGroup.push_back(boost::thread(boost::bind(&ThreadPool::Run, this, i)));
    }
}

void ThreadPool::Push(WorkItermPtr task) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    mTaskQue.push(task);
    mCond.notify_one();
}

void ThreadPool::Wait() {
    for(auto &t: mThreadGroup) {
        t.join();
    }
}

