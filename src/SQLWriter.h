//
// Created by liuwei on 8/27/17.
//

#ifndef CHEAT_SQLWRITER_H
#define CHEAT_SQLWRITER_H
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <memory>

class SQLWriter
{
public:
    SQLWriter(const std::string& username,
              const std::string& password)
        : mUsername(username), mPassword(password)
    {
        mCount = 0;
        mSql.reserve(mLimit * 20 + 100);
    }
    ~SQLWriter()
    {
        if (mCount) {
            sql::Statement* state = mConn->createStatement();
            state->execute(mSql);
            delete state;
            mCount = 0;            
        }
        if (mConn) delete mConn;
    }

    void write(const std::string& sql)
    {
        if (mCount == 0) {
            mSql = "INSERT INTO `cheat_record` (`problem_id`, `sub1_id`, `sub2_id`, `user1`, `user2`,`probability`) VALUES";
            mSql.reserve(mLimit * 20 + 100);
        }
        mSql += sql;
        if (mCount > mLimit) {
            sql::Statement* state = mConn->createStatement();
            state->execute(mSql);
            delete state;
        }
    }

    bool connect()
    {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        mConn = driver->connect("tcp://127.0.0.1:3306/oj", mUsername, mPassword);
    }
private:
    std::string mUsername, mPassword;
    sql::Connection *mConn;
    size_t mCount;
    static const size_t mLimit = 100;
    std::string mSql;
};

typedef std::shared_ptr<SQLWriter> SQLWriterPtr;

#endif //CHEAT_SQLWRITER_H
