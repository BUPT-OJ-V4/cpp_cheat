//
// Created by liuwei on 7/29/17.
//

#ifndef CHEAT_WORKITERM_H
#define CHEAT_WORKITERM_H
#include <memory>

class WorkIterm {
public:
    WorkIterm() {};
public:
    virtual void run(int thread_id = -1) = 0;
};

typedef std::shared_ptr<WorkIterm> WorkItermPtr;



#endif //CHEAT_WORKITERM_H
