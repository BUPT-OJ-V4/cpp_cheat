#include <iostream>
#include <boost/thread.hpp>
#include "CheatWorker.h"

void calc(int i) {
    std::cout << i << "th task, the answer is: " << i * i << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    CheatWorker ct(3);
    ct.start();
    for(int i = 0; i < 10; i ++) {
        Task task = boost::bind(calc, i);
        ct.add_task(task);
    }
    ct.close();
    ct.wait();
    std::cout << "task end" << std::endl;
    ct.stop();
    std::cout << "process end" << std::endl;
    return 0;
}