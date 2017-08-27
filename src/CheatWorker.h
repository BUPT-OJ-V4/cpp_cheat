//
// Created by liuwei on 7/29/17.
//

#ifndef CHEAT_CHEATWORKER_H
#define CHEAT_CHEATWORKER_H
#include "WorkIterm.h"
#include "AntiCheat.h"
#include <string>

typedef std::pair<std::string,int> Ans;

class CheatWorker : public WorkIterm {
public:
    CheatWorker(const int& a, const int& b)
        : _sub1(a), _sub2(b), antiCheat(nullptr){
    };
    void run();
    void callback();
    static void calc(Ans &ans);
    static void init(Ans &ans);
    static std::string username, password, problem_id;
private:
    int _sub1, _sub2;
    double _ans;
    AntiCheat* antiCheat;
};


#endif //CHEAT_CHEATWORKER_H
