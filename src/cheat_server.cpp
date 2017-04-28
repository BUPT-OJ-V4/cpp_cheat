#include <iostream>
#include "CheatWorker.h"
#include <cpp_redis/redis_subscriber.hpp>
volatile std::atomic<bool> should_exit = ATOMIC_VAR_INIT(false);
void calc(int i) {
    std::cout << i << "th task, the answer is: " << i * i << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    cpp_redis::redis_subscriber client;
    client.connect("127.0.0.1", 6397, [](cpp_redis::redis_subscriber&) {
        std::cout << "sub disconnected. " << std::endl;
        should_exit = true;
    });
    client.subscribe("cheat", [](const std::string& chan, const std::string& msg) {
        std::cout << "Message (" << msg << ") from channel " << chan << std:: endl;
    });
    return 0;
}