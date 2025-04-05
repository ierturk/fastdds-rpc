#ifndef MOCK_RPCDDSIFC_H
#define MOCK_RPCDDSIFC_H

#include <gmock/gmock.h>
#include "RpcDdsIfc.h"
#include "RBIC1Rpc.h"

class MockRpcDdsIfc : public RpcDdsIfc<RBIC1Rpc::GenericReply, RBIC1Rpc::GenericRequest> {
public:
    MOCK_METHOD(void, init, (const std::string& reader_topic, const std::string& writer_topic), (override));
    MOCK_METHOD(void, cleanup, (), (override));
    MOCK_METHOD(eprosima::fastdds::dds::DataWriter*, getWriter, (), (const, override));
    MOCK_METHOD(eprosima::fastdds::dds::DataReader*, getReader, (), (const, override));
};

#endif // MOCK_RPCDDSIFC_H