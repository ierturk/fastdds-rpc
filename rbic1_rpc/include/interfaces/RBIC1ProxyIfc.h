#ifndef RBIC1PROXYIFC_H
#define RBIC1PROXYIFC_H

#include <string>

// Interface class for RBIC1Proxy
class RBIC1ProxyIfc {
public:
    virtual ~RBIC1ProxyIfc() = default;

    // Pure virtual methods
    virtual void init(const std::string& request_topic, const std::string& reply_topic) = 0;
    virtual void run() = 0;
};

#endif // RBIC1PROXYIFC_H