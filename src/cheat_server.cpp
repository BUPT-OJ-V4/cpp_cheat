#include <iostream>
#include "CheatWorker.h"
#include <signal.h>
#include <cpp_redis/cpp_redis>
volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
void calc(int i) {
    std::cout << i << "th task, the answer is: " << i * i << std::endl;
}

void sigint_handler(int) {
    should_exit = true;
}


int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    cpp_redis::redis_subscriber client;
    client.connect();
    client.subscribe("cheat", [](const std::string& chan, const std::string& msg) {
        std::cout << "Message (" << msg << ") from channel " << chan << std:: endl;
        should_exit = true;
    });
    client.commit();
    signal(SIGINT, &sigint_handler);
    while(!should_exit) {

    }
    return 0;
}