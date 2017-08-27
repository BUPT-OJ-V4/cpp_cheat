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
void CheatWorker::callback(Ans& ans)
{
    if (ans.second > 0) ans.first += ",";
    ans.first += "(" + problem_id + ", " + std::to_string(_sub1) + "," + std::to_string(_sub2);
    ans.first += ", '" + antiCheat->_userinfo[_sub1] + "', '" + antiCheat->_userinfo[_sub2] + "', " + std::to_string(_ans) + ")";
    ans.second ++;
}

void CheatWorker::calc(Ans &ans) {
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con = driver->connect("tcp://127.0.0.1:3306", username, password);
    sql::Statement *state = con->createStatement();
    state->execute("use oj");
    if (ans.second > 0) {
        bool flag = state->execute(ans.first);
    }
    delete state;
    delete con;
}
void CheatWorker::init(Ans& ans) {
    ans.first = "INSERT INTO `cheat_record` (`problem_id`, `sub1_id`, `sub2_id`, `user1`, `user2`,`probability`) VALUES";
}