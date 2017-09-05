//
// Created by liuwei on 7/29/17.
//

#include "CheatWorker.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
void CheatWorker::run(int tid)
{
    double ans = mAntiCheat->Calc(mSub1, mSub2);
    mWriter->write(tid, mSub1, mSub2,
                   mAntiCheat->GetUserInfo(mSub1),
                   mAntiCheat->GetUserInfo(mSub2),
                   ans, mAntiCheat->GetProblemId());
}
