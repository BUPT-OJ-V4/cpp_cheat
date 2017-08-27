//
// Created by liuwei on 7/29/17.
//

#include "CheatWorker.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
void CheatWorker::run()
{
    _ans = antiCheat->calc(_sub1, _sub2);
}
