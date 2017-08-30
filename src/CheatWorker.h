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
                SQLWriterPtr writer,
                AntiCheat* antiCheat
    )
        : mSub1(a), mSub2(b), mWriter(mWriter), mAntiCheat(antiCheat){
    };
    void run(int tid = 0) override;
private:
    int mSub1, mSub2;
    AntiCheat* mAntiCheat;
    SQLWriterPtr mWriter;
};


void CheatWorker<Writer>::run()
{

}

#endif //CHEAT_CHEATWORKER_H
