#include <iostream>
#include <fstream>
#include <evnsq/consumer.h>
#include <evpp/event_loop.h>
#include <ctime>
#include <boost/filesystem.hpp>

#include "CheatWorker.h"
#include "cheat_algorithm.h"

namespace fs = boost::filesystem;


volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
int DEFAULT_THREAD_NUM = 1;
std::string host="127.0.0.1";
std::string username="oj", password="";

void sigint_handler(int) {
    should_exit = true;
}

Res compare_code(const int & a, const int & b)
{

    double res = cheat::lcs(cheat::brackets[a], cheat::brackets[b]) +
        cheat::cal_common_substring(cheat::cache[a], cheat::cache[b]);
    res *= 0.5;
    double third = cheat::frequency_statistic(cheat::allcode[a], cheat::allcode[b]);
    std::cout << "=============================prob===================" << std::endl;
    std::cout << res << std::endl;

    if (third > 99.0) return Res(std::make_pair(a, b), third);
    return Res(std::make_pair(a, b), res);
}

int solve(const evnsq::Message* msg) {
    std::string problem_id = msg->body.ToString();
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://"+host+":3306", username, password);
    con->setAutoCommit(false);
    state = con->createStatement();
    std::cout << "commit mode: " << con->getAutoCommit() << std::endl;
    state->execute("use oj");
    std::string query = "select t2.id, t1.user_id, t1.code from submission_submission as t1, contest_contestsubmission as t2"
                                "where t2.problem_id=" + problem_id +
            ", t1.id=t2.submission_id and t1.status = 'AC'";
    sql::ResultSet* result = state->executeQuery(query);
    std::vector<std::pair<int,int>> subs;
    while (result->next()) {
        int user_id = result->getInt("user_id");
        int idx = result->getInt("id");
        std::string code = result->getString("code");
        cheat::normalization(idx, code);
        subs.emplace_back(idx, user_id);
    }
    CheatWorker cheatWorker(DEFAULT_THREAD_NUM, problem_id);
    for (int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            if (subs[i].second != subs[j].second) {
                cheatWorker.add_task(boost::bind(compare_code, subs[i].first, subs[j].first));
            }
        }
    }
}

void solve1() {
    std::cout << "start solve" << std::endl;
    time_t start = time(0);
    fs::path p("./data");
    if (! fs::exists(p)) {
        std::cout << "not exist" << std::endl;
        return;
    }
    fs::directory_iterator end_ite;
    std::vector<int> vs;
    for (fs::directory_iterator ite(p); ite != end_ite; ++ ite) {
        if (fs::is_regular_file(ite->status())) {
            std::string s = ite->path().string();
            int ans = 0, l = s.length();
            for(int i = 6; i > 0; i --) {
                ans = ans * 10 + (int)(s[l - i] - '0');
            }
            cheat::deal_code_file(ans, s);
            vs.push_back(ans);
        }
    }
    /*
    for(int i = 0; i < vs.size(); i ++) {
        for(int j = i + 1; j < vs.size(); j ++) {
            auto t = boost::bind(&compare_code, vs[i], vs[j], 2960);
            cheatWorker.add_task(t);
        }
    }
     */
    //cheatWorker.start();
    //cheatWorker.close();
    //cheatWorker.wait();
    time_t end = time(0);
    std::cout << "=========================cost time==============" << std::endl;
    std::cout << (int)(end - start) << std::endl;
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
            DEFAULT_THREAD_NUM = to_int(argv[i + 1]);
        }
        else if (std::strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            username = std::string(argv[i + 1]);
        }
        else if (std::strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            password = std::string(argv[i + 1]);
        }
    }
    if (argc > 1 && std::strcmp(argv[1], "-t") == 0) {
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