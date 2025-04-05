#ifndef RBIC1INVOKER_H
#define RBIC1INVOKER_H

#include "RBIC1InvokerIfc.h"
#include "rpcdds.h"
#include "RBIC1Rpc.h"
#include "RBIC1RpcPubSubTypes.h"
#include <string>
#include <boost/signals2.hpp>

class RBIC1Invoker : public RBIC1InvokerIfc {
public:
    RBIC1Invoker();
    ~RBIC1Invoker() override;

    void init(const std::string& reader_topic, const std::string& writer_topic) override;
    void run() override;

    // Signal to notify when a reply is received
    boost::signals2::signal<void(const std::string&)> replyReceived;

private:
    RpcDds<RBIC1Rpc::GenericReply, RBIC1Rpc::GenericRequest, RBIC1Rpc::GenericReplyPubSubType, RBIC1Rpc::GenericRequestPubSubType> rpc_;
};

#endif // RBIC1INVOKER_H