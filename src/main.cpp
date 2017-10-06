#include <iostream>
#include <fstream>
#include <evnsq/consumer.h>
#include <evpp/event_loop.h>
#include <ctime>
#include <boost/filesystem.hpp>
#include <cppconn/statement.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "ThreadQueue.h"
#include "AntiCheat.h"
#include "SQLWriter.h"

#define DEFAULT_THREAD_NUM 2
namespace fs = boost::filesystem;

typedef std::pair<int, int> P;
typedef std::shared_ptr<ThreadQueue<P>> ThreadQueuePtr;

volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);

std::string username, password;
int threadnum = DEFAULT_THREAD_NUM;

void sigint_handler(int) {
    should_exit = true;
}

inline void getSubmission(const std::string& problem_id,
                          std::vector<std::pair<int, std::string>>& subs,
                          AntiCheat* antiCheat)
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", username, password);
    state = con->createStatement();
    state->execute("use oj");
    std::string query = "select t2.id, t3.username, t1.code from"
                        "submission_submission t1, contest_contestsubmission"
                        "t2, auth_user t3  where t2.problem_id=" + problem_id +
    " and t1.status='AC' and t1.id=t2.submission_id and t3.id=t1.user_id";
    sql::ResultSet* result = state->executeQuery(query);

    while (result->next()) {
        std::string user_name = result->getString("username");
        int idx = result->getInt("id");
        std::string code = result->getString("code");
        antiCheat->AddSubmission(idx, code, user_name);
        subs.emplace_back(idx, user_name);
    }
    delete state;
    delete con;
}

void Run(SQLWriterPtr sqlWriter, ThreadQueuePtr threadQueue, AntiCheat* antiCheat, std::string problem_id)
{
    while(threadQueue->Size())
    {
        P task = threadQueue->Pop();
        std::string user1, user2;
        double ans = antiCheat->Calc(task.first, task.second, user1, user2);
        std::string sql = "(" + problem_id + ", " + std::to_string(task.first) + "," + std::to_string(task.second)
                    + ", '" + user1 + "', '" + user2 + "', " + std::to_string(ans) + ")";
        sqlWriter->write(sql);
    }
}

int Solve(const evnsq::Message* msg) {
    std::cout << "start _" << std::endl;
    std::string problem_id = msg->body.ToString();
    for(auto x: problem_id) {
        if (x < '0' || x > '9') return 0;
    }

    AntiCheat antiCheat;
    std::vector<std::pair<int, std::string>> subs;
    getSubmission(problem_id, subs, &antiCheat);
    boost::thread_group threadGroup;
    ThreadQueuePtr threadQueue(new ThreadQueue<P>);

    for(int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            if (subs[i].second == subs[j].second) continue;
            threadQueue->Push(std::make_pair(subs[i].first,
                            subs[j].first));
        }
    }

    for(size_t i = 0; i < threadnum; i ++)
    {
        SQLWriterPtr sqlWriter(new SQLWriter(username, password));
        if (!sqlWriter->connect()) {
            std::cout << "failed to connect sql" << std::endl;
            continue;
        }
        threadGroup.create_thread(boost::bind(Run, sqlWriter, threadQueue, &antiCheat, problem_id));
    }
    threadGroup.join_all();
    return 0;
}

int to_int(const char *str) {
    int ans = 0;
    int len = strlen(str);
    if (str) {
        std::cout << len << std::endl;
        std::cout << "not none" << *str << std::endl;
    }
    for(int i = 0; i < len; i ++) {
        ans = ans * 10 + (int)(str[i] - '0');
    }
    std::cout << ans << std::endl;
    return ans;
}

int main(int argc, char *argv[]) {
    for (std::size_t i = 0; i < argc; i ++) {
        if (std::strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            threadnum = to_int(argv[i + 1]);
        }
        else if (std::strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            username = std::string(argv[i + 1]);
        }
        else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            password = std::string(argv[i + 1]);
        }
    }
    std::string nsqd_tcp_addr;
    std::string lookupd_http_url;
    lookupd_http_url = "http://127.0.0.1:4161/lookup?topic=cheat";
    evpp::EventLoop loop;
    evnsq::Consumer client(&loop, "cheat", "ch1", evnsq::Option());
    client.SetMessageCallback(&Solve);
    client.ConnectToLookupds(lookupd_http_url);
    loop.Run();
    return 0;
}
