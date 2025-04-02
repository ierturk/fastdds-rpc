#ifndef RBIC1INVOKER_H
#define RBIC1INVOKER_H

#include "rpcdds.h"
#include "RBIC1Rpc.h"
#include "RBIC1RpcPubSubTypes.h"
#include <string>
#include <iostream>

class RBIC1Invoker {
public:
    RBIC1Invoker();
    ~RBIC1Invoker();

    void init(const std::string& reader_topic, const std::string& writer_topic);
    void run();

private:
    RpcDds<RBIC1Rpc::GenericReply, RBIC1Rpc::GenericRequest, RBIC1Rpc::GenericReplyPubSubType, RBIC1Rpc::GenericRequestPubSubType> rpc_;
};

#endif // RBIC1INVOKER_H