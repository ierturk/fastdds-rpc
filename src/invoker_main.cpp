#include "RBIC1Invoker.h"
#include <iostream>

int main() {
    std::cout << "Starting Invoker App (64-bit)" << std::endl;

    RBIC1Invoker invoker;
    invoker.init("ReplyTopic", "RequestTopic");
    invoker.run();

    return 0;
}