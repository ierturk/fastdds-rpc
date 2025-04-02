#ifndef RBIC1PROXY_H
#define RBIC1PROXY_H

#include "rpcdds.h"
#include "RBIC1Rpc.h"
#include "RBIC1RpcPubSubTypes.h"
#include <string>
#include <iostream>

class RBIC1Proxy {
public:
    RBIC1Proxy();
    ~RBIC1Proxy();

    void init(const std::string& request_topic, const std::string& reply_topic);
    void run();

private:
    RpcDds<RBIC1Rpc::GenericRequest, RBIC1Rpc::GenericReply, RBIC1Rpc::GenericRequestPubSubType, RBIC1Rpc::GenericReplyPubSubType> rpc_;
};

#endif // RBIC1PROXY_H