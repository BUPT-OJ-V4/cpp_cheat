#include <iostream>
#include <fstream>
#include <cpp_redis/cpp_redis>
#include <ctime>
#include <boost/filesystem.hpp>

#include "CheatWorker.h"
#include "cheat_algorithm.h"

namespace fs = boost::filesystem;


volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);

void sigint_handler(int) {
    should_exit = true;
}

Res compare_code(const int & a, const int & b)
{

    double res = cheat::lcs(cheat::brackets[a], cheat::brackets[b]);
    res += cheat::cal_common_substring(cheat::cache[a], cheat::cache[b]);
    res *= 0.5;
    double third = cheat::frequency_statistic(cheat::cache[a], cheat::cache[b]);
    if (third >= 99) return Res(std::make_pair(a, b), 100.0);
    return Res(std::make_pair(a, b), res);
}

void connect_to_mysql(CheatWorker* cheatWorker, std::string problem_id) {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *state;
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://10.105.240.51:3306", "oj", "");
    con->setAutoCommit(false);
    state = con->createStatement();
    //std::cout << "commit mode: " << con->getAutoCommit() << std::endl;
    state->execute("use oj");
    std::string query = "select sid, user_id, code_file from Submission_submission t1 where t1.status = 'Accepted' and t1.problem_index_id="+problem_id;
    sql::ResultSet* result = state->executeQuery(query);
    std::vector<int> subs;
    int idx = 0;
    while (result->next()) {
        int id = result->getInt("sid");
        cheat::deal_code_file(id, result->getString("code_file"));
        subs.push_back(id);
    }
    delete state;
    delete con;
    delete result;
    std::sort(subs.begin(), subs.end());
    for(int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            Task t = boost::bind(compare_code, subs[i], subs[j]);
            cheatWorker->add_task(t);
        }
    }
}

void solve(std::string const & msg) {
    std::cout << "start solve" << std::endl;
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
    CheatWorker cheatWorker(2);
    std::cout << "==================" << vs.size() << std::endl;
    for(int i = 0; i < vs.size(); i ++) {
        for(int j = i + 1; j < vs.size(); j ++) {
            auto t = boost::bind(&compare_code, vs[i], vs[j]);
            cheatWorker.add_task(t);
        }
    }
    cheatWorker.start();
    cheatWorker.close();
    cheatWorker.wait();
    std::cout << "===========================end " << std::endl;

}

int main(int argc, char *argv[]) {
    cpp_redis::redis_subscriber client;
    cheat::init();
    solve("xxx");
    /*
    client.connect();
    client.subscribe("cheat", [](const std::string& chan, const std::string& msg) {
        std::cout << "Message (" << msg << ") from channel " << chan << std:: endl;
        clock_t start = clock();
        solve(msg);
        clock_t end = clock();
        std::cout << "=========================cost time==============" << std::endl;
        std::cout << (end - start) / CLOCKS_PER_SEC << std::endl;

    });
    client.commit();
    signal(SIGINT, &sigint_handler);
    while(!should_exit) {

    }
     */
    return 0;
}