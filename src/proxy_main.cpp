#include "RBIC1Proxy.h"
#include <iostream>

int main() {
    std::cout << "Starting Proxy App (32-bit)" << std::endl;

    RBIC1Proxy proxy;
    proxy.init("RequestTopic", "ReplyTopic");
    proxy.run();

    return 0;
}