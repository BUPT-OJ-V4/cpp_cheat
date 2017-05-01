#include <iostream>
#include <fstream>
#include <cpp_redis/cpp_redis>
#include <ctime>

#include "CheatWorker.h"
#include "cheat_algorithm.h"


volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);

void sigint_handler(int) {
    should_exit = true;
}

Res compare_code(const int & a, const int & b)
{
   return Res(std::make_pair(a, b), 0.5);
}

long long compare_code2(int a, int b)
{
    return 1LL;
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
        cheat::deal_code_file(result->getString("code_file"));
        subs.push_back(id);
        idx ++;
    }
    delete state;
    delete con;
    delete result;
    /*
    std::sort(subs.begin(), subs.end());
    for(int i = 0; i < subs.size(); i ++) {
        for(int j = i + 1; j < subs.size(); j ++) {
            Task t = boost::bind(compare_code, subs[i], subs[j]);
            cheatWorker->add_task(t);
        }
    }
    */
}

int main(int argc, char *argv[]) {
    //cpp_redis::redis_subscriber client;
    clock_t start = clock();
    cheat::init();
    CheatWorker cheatWorker(1);
    cheatWorker.start();
    for (int i = 2960; i < 2964; i ++) {
        connect_to_mysql(&cheatWorker, std::to_string(i));
    }
    cheatWorker.close();
    cheatWorker.wait();
    clock_t end = clock();
    std::cout << "=========================cost time==============" << std::endl;
    std::cout << (end - start) / CLOCKS_PER_SEC << std::endl;
    /*client.connect();
    client.subscribe("cheat", [](const std::string& chan, const std::string& msg) {
        std::cout << "Message (" << msg << ") from channel " << chan << std:: endl;
        should_exit = true;
    });
    client.commit();
    signal(SIGINT, &sigint_handler);
    while(!should_exit) {

    }
     */
    return 0;
}