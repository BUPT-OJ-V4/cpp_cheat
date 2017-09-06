#include <iostream>
#include <fstream>
#include <evnsq/consumer.h>
#include <evpp/event_loop.h>
#include <ctime>
#include <boost/filesystem.hpp>
#include <cppconn/statement.h>

#include "SimpleThreadPool.h"
#include "AntiCheat.h"
#include "CheatWorker.h"
#include "SQLWriter.h"

#define DEFAULT_THREAD_NUM 2
namespace fs = boost::filesystem;


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

int solve(const evnsq::Message* msg) {
    std::cout << "start _" << std::endl;
    std::string problem_id = msg->body.ToString();
    for(auto x: problem_id) {
        if (x < '0' || x > '9') return 0;
    }
    AntiCheat antiCheat;
    std::vector<std::pair<int, std::string>> subs;
    getSubmission(problem_id, subs, &antiCheat);
    SimpleThreadPool threadPool(threadnum);
    SQLWriterPtr sqlWriter(new SQLWriter(username, password, threadnum));
    threadPool.Start();
    if (!sqlWriter->connect()) {
        std::cout << "failed to connect sql" << std::endl;
        std::cerr << "failed to connect sql" << std::endl;
        return -1;
    }
    for (int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            if (subs[i].second == subs[j].second) continue;
            threadPool.Push(std::make_shared<CheatWorker>(subs[i].first,
                            subs[j].first, sqlWriter, &antiCheat));
        }
    }
    threadPool.Close();
    threadPool.Wait();
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
    client.SetMessageCallback(&solve);
    client.ConnectToLookupds(lookupd_http_url);
    loop.Run();
    return 0;
}
