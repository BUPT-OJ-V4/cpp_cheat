#include <iostream>
#include <fstream>
#include <evnsq/consumer.h>
#include <evpp/event_loop.h>
#include <ctime>
#include <boost/filesystem.hpp>
#include <cppconn/statement.h>

#include "ThreadPool.h"
#include "AntiCheat.h"
#include "CheatWorker.h"

namespace fs = boost::filesystem;


volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
int DEFAULT_THREAD_NUM = 1;
std::string host="127.0.0.1";

void sigint_handler(int) {
    should_exit = true;
}

int solve(const evnsq::Message* msg) {
    std::cout << "start _" << std::endl;
    std::string problem_id = msg->body.ToString();
    for(auto x: problem_id) {
        if (x < '0' || x > '9') return 0;
    }
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", username, password);
    state = con->createStatement();
    state->execute("use oj");
    std::string query = "select t2.id, t3.username, t1.code from submission_submission t1, contest_contestsubmission t2, auth_user t3"
                                " where t2.problem_id=" + problem_id +
    " and t1.status='AC' and t1.id=t2.submission_id and t3.id=t1.user_id";
    sql::ResultSet* result = state->executeQuery(query);
    std::vector<std::pair<int, std::string>> subs;
    AntiCheat* antiCheat = new AntiCheat;
    while (result->next()) {
        std::string user_name = result->getString("username");
        int idx = result->getInt("id");
        std::string code = result->getString("code");
        antiCheat->normalization(idx, code, user_name);
        subs.emplace_back(idx, user_name);
    }
    ThreadPool<std::pair<std::string, int>> threadPool(DEFAULT_THREAD_NUM);
    threadPool.start();
    for (int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            if (subs[i].second != subs[j].second) {
                std::shared_ptr<WorkIterm<Ans>> workItermPtr(new CheatWorker(subs[i].first, subs[j].first));
                threadPool.add_task(workItermPtr);
            }
        }
    }
    threadPool.close();
    threadPool.wait();
    delete antiCheat;
    return 0;
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
	cheat::init();
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
