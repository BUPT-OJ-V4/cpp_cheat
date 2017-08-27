//
// Created by liuwei on 8/27/17.
//

#ifndef CHEAT_SQLWRITER_H
#define CHEAT_SQLWRITER_H
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>

class SQLWriter {
public:
    SQLWriter(const std::string& username,
              const std::string& password
    ) : _username(username), _password(password), _count(0) {
    }
    ~SQLWriter() {
        if (_count) {
            write();
        }
        if (_state) {
            delete _state;
        }
        if (_conn) {
            delete _conn;
        }

    }
    bool write(const int& a, const int& b,
               const std::string& user1, const std::string &user2,
               const double& ans, const std::string& problem_id);
    bool connect();
private:
    void write();
    std::string _username, _password;
    sql::Connection *_conn;
    sql::Statement *_state;
    std::string _sql;
    int _count;

};


#endif //CHEAT_SQLWRITER_H
