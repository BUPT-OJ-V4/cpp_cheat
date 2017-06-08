#include <iostream>
#include <fstream>
#include <cpp_redis/cpp_redis>
#include <ctime>
#include <boost/filesystem.hpp>

#include "CheatWorker.h"
#include "cheat_algorithm.h"

namespace fs = boost::filesystem;


volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
int DEFAULT_THREAD_NUM = 1;

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

void solve_for_old_oj(std::string problem_id) {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://10.105.240.51:3306", "oj", "");
    con->setAutoCommit(false);
    state = con->createStatement();
    std::cout << "commit mode: " << con->getAutoCommit() << std::endl;
    state->execute("use oj");
    std::string query = "select sid, user_id, problem_index_id, code_file from Submission_submission t1 where t1.status = 'Accepted' and t1.problem_index_id="+problem_id;
    sql::ResultSet* result = state->executeQuery(query);
    std::vector<std::pair<int,int>> subs;
    system("mkdir data");
    while (result->next()) {
        int user_id = result->getInt("user_id");
        int idx = result->getInt("sid");
        std::string code_url = result->getString("code_file");
        std::cout << code_url << std::endl;
        std::string cmd = "cd ./data && curl -O http://10.105.240.51" + code_url;
        system(cmd.c_str());
        cheat::deal_code_file(idx, "./data/" + code_url.substr(code_url.length() - 6));
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
    system("rm -rf data");
}

void solve(std::string const & msg) {
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
    std::cout << argc << std::endl;
    if (argc > 1 && std::strcmp(argv[1], "-t") == 0) {
        DEFAULT_THREAD_NUM = to_int(argv[2]);
    }
    cpp_redis::redis_subscriber client;
    cheat::init();
    //get_test_data("2960");
    solve("xxx");
    /*
    client.connect();
    client.subscribe("cheat", [](const std::string& chan, const std::string& msg) {
        std::cout << "Message (" << msg << ") from channel " << chan << std:: endl;
        clock_t start = clock();
        solve(msg);

    });
    client.commit();
    signal(SIGINT, &sigint_handler);
    while(!should_exit) {

    }
     */
    return 0;
}