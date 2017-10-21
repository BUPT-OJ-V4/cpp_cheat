#include <iostream>
#include <fstream>
#include <ctime>
#include <evnsq/consumer.h>
#include <evpp/event_loop.h>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "common.h"
#include "AntiCheat.h"
#include "SQLWriter.h"

#define DEFAULT_THREAD_NUM 2
namespace fs = boost::filesystem;

std::string username, password;
int threadnum = DEFAULT_THREAD_NUM;

inline void getSubmission(const std::string& problem_id,
                          std::vector<std::pair<int, std::string>>& subs,
                          AntiCheat* antiCheat)
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306/oj", username, password);
    state = con->createStatement();
    std::string query = "select t2.id, t3.username, t1.code from "
                        "submission_submission t1, contest_contestsubmission "
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

inline void merge_sort(std::vector<std::vector<Result>>& results)
{
    int len = 0;
    for(size_t i = 0; i < threadnum; i ++) {
        len += results[i].size();
    }
    results[threadnum].reserve(len);
    for(size_t i = 0; i < threadnum; i ++) {
        for(size_t j = 0; j < results[i].size(); j ++) {
            results[threadnum].push_back(results[i][j]);
        }
    }
    std::sort(results[threadnum].begin(), results[threadnum].end());
}

void Run(const std::vector<P>& queue, AntiCheat* antiCheat, std::vector<Result>* results,std::string problem_id)
{
    size_t len = queue.size();
    for(size_t i = 0; i < len; i ++)
    {
        const P& task = queue[i];
        std::string user1, user2;
        double ans = antiCheat->Calc(task.first, task.second, user1, user2);
        results->emplace_back(ans, task.first, task.second, user1, user2);
    }
    std::cout << "result count : " << results->size() << std::endl;
}

int Solve(const evnsq::Message* msg) {
    std::cout << "start solve" << std::endl;
    std::string problem_id = msg->body.ToString();
    for(auto x: problem_id) {
        if (x < '0' || x > '9') return 0;
    }
    std::cout << problem_id << std::endl;

    AntiCheat antiCheat;
    std::vector<std::pair<int, std::string>> subs;
    std::cout << "Get submission" << std::endl;
    getSubmission(problem_id, subs, &antiCheat);
    boost::thread_group threadGroup;
    assert(threadnum > 0);
    std::vector<P> queue[threadnum];
    std::vector<std::vector<Result>> results(threadnum + 1);
    for(int i = 0, k = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            if (subs[i].second == subs[j].second) continue;
            queue[k].push_back(std::make_pair(subs[i].first,
                            subs[j].first));
            k = (k + 1) % threadnum;
        }
    }
    SQLWriterPtr sqlWriter(new SQLWriter(username, password));
    if (!sqlWriter->connect()) {
        std::cout << "failed to connect sql" << std::endl;
        return 0;
    }
    for(size_t i = 0; i < threadnum; i ++)
    {
        std::cout << "calc num: " << queue[i].size() << std::endl;
        results[i].reserve(queue[i].size() + 1);
        threadGroup.create_thread(boost::bind(Run, queue[i], &antiCheat, &results[i], problem_id));
    }
    threadGroup.join_all();
    merge_sort(results);
    std::cout << "all ans num: " << results[threadnum].size() << std::endl;
    for(auto& result: results[threadnum]) {
        std::string sql = "(" + problem_id + ", " + std::to_string(result.sub1) + "," + std::to_string(result.sub2)
                    + ", '" + result.user1 + "', '" + result.user2 + "', " + std::to_string(result.ans) + ")";
        sqlWriter->write(sql);
    }
    return 0;
}

int to_int(const char *str) {
    int ans = 0;
    size_t len = strlen(str);
    for(int i = 0; i < len; i ++) {
        ans = ans * 10 + (int)(str[i] - '0');
    }
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
