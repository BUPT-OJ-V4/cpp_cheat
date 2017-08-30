//
// Created by liuwei on 8/27/17.
//

#include "SQLWriter.h"


bool SQLWriter::write(const int& tid, const int& sub1, const int& sub2,
                      const std::string& user1, const std::string& user2,
                      const double& ans, const std::string& problem_id)
{
    assert(tid >= 0 && tid < mConnectors.size());
    std::string sql = "(" + problem_id + ", " + std::to_string(sub1) + "," + std::to_string(sub2)
                    + ", '" + user1 + "', '" + user2 + "', " + std::to_string(ans) + ")";
    mConnectors[tid]->write(sql);
}

bool SQLWriter::connect()
{
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    for (int i = 0; i < mConnectionSize) {
        sql::Connection * conn = driver->connect("tcp://127.0.0.1:3306/oj", _username, _password);
        if (!conn) {
            mConnectors.clear();
            return false;
        }
        mConnectors.push_back(new SQLConnector(conn));
    }
}
