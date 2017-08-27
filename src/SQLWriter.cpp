//
// Created by liuwei on 8/27/17.
//

#include "SQLWriter.h"


bool SQLWriter::write(const int& sub1, const int& sub2,
                      const std::string& user1, const std::string& user2,
                      const double& ans, const std::string& problem_id)
{
    if (!_count) {
        _sql = "INSERT INTO `cheat_record` (`problem_id`, `sub1_id`, `sub2_id`, `user1`, `user2`,`probability`) VALUES";
    }
    _sql += "(" + problem_id + ", " + std::to_string(sub1) + "," + std::to_string(sub2)
                    + ", '" + user1 + "', '" + user2 + "', " + std::to_string(ans) + ")";
    _count ++;
}

bool SQLWriter::connect()
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    if (!driver) return false;
    _conn = driver->connect("tcp://127.0.0.1:3306", _username, _password);
    if (!_conn) return false;
    _state = _conn->createStatement();
    _state->execute("use oj");
}