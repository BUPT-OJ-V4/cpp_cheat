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
class SQLConnector
{
public:
    SQLConnector(sql::Connection * conn)
    {
        mConn = conn;
        mCount = 0;
        mSql.reserve(mLimit * 20);
    }
    ~SQLConnector()
    {
        if (mCount) {
            sql::Statement* state = mConn->createStatement();
            state->execute(mSql);
            delete state;
            mCount = 0;            
        }
        delete mConn;
    }
    void write(const std::string& sql)
    {
        if (mCount == 0) {
            mSql = "INSERT INTO `cheat_record` (`problem_id`, `sub1_id`, `sub2_id`, `user1`, `user2`,`probability`) VALUES";
        }
        mSql += sql;
        if (mCount > mLimit) {
            sql::Statement* state = mConn->createStatement();
            state->execute(mSql);
            delete state;
            mCount = 0;
        }
    }
    sql::Connection *mConn;
    std::string mSql;
    size_t mCount;
    static const size_t mLimit = 150;
};

typedef std::shared_ptr<SQLConnector> SQLConnectorPtr;

class SQLWriter
{
public:
    SQLWriter(const std::string& username,
              const std::string& password,
              const int connectSize)
        : mUsername(username), mPassword(password)
        , mConnectionSize(connectSize)
    {
    }
    ~SQLWriter() {
    }
    bool write(const int& tid, const int& a, const int& b,
               const std::string& user1, const std::string &user2,
               const double& ans, const std::string& problem_id);
    bool connect();
private:
    std::string mUsername, mPassword;
    std::vector<SQLConnectorPtr> mConnectors;
    int mConnectionSize;
};

typedef std::shared_ptr<SQLWriter> SQLWriterPtr;


#endif //CHEAT_SQLWRITER_H
