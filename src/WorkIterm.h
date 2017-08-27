//
// Created by liuwei on 7/29/17.
//

#ifndef CHEAT_WORKITERM_H
#define CHEAT_WORKITERM_H

class WorkIterm {
public:
    WorkIterm() {};
public:
    virtual void run() = 0;
    virtual void callback(T& ans) = 0;
    virtual static void calc(T &ans) = 0;
    virtual static void init(T &ans) = 0;
};



#endif //CHEAT_WORKITERM_H
