//
// Created by liuwei on 7/29/17.
//

#define CHEAT_CHEATWORKER_H

#include "WorkIterm.h"
#include "AntiCheat.h"
#include <string>

#include "SQLWriter.h"

class CheatWorker : public WorkIterm {
public:
    CheatWorker(const int& a,
                const int& b,
                SQLWriter* writer,
                AntiCheat* antiCheat
    )
        : _sub1(a), _sub2(b), _writer(writer), _antiCheat(antiCheat){
    };
    void run() override;
private:
    int _sub1, _sub2;
    AntiCheat* _antiCheat;
    SQLWriter* _writer;
};

template <typename Writer>
void CheatWorker<Writer>::run()
{
    double ans = _antiCheat->calc(_sub1, _sub2);
    _writer->write(_sub1, _sub2,
                   _antiCheat->_userinfo[_sub1],
                   _antiCheat->_userinfo[_sub2],
                   ans, _antiCheat->_problem_id);
}

#endif //CHEAT_CHEATWORKER_H
