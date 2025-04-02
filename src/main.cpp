#include <iostream>
#include <thread>
#include "RBIC1Proxy.h"
#include "RBIC1Invoker.h"

int main() {
    std::cout << "Main: Starting..." << std::endl;

    // Create and initialize Proxy
    RBIC1Proxy proxy;
    proxy.init("RequestTopic", "ReplyTopic");

    // Create and initialize Invoker
    RBIC1Invoker invoker;
    invoker.init("ReplyTopic","RequestTopic");

    // Run Proxy and Invoker in separate threads
    std::jthread proxy_thread([&proxy]() {
        proxy.run();
    });

    std::jthread invoker_thread([&invoker]() {
        invoker.run();
    });

    std::cout << "Main: Waiting for threads to finish..." << std::endl;

    return 0;
}