#ifndef MOCK_RBIC1PROXYIFC_H
#define MOCK_RBIC1PROXYIFC_H

#include <gmock/gmock.h>
#include "RBIC1ProxyIfc.h"

class MockRBIC1ProxyIfc : public RBIC1ProxyIfc {
public:
    MOCK_METHOD(void, init, (const std::string& request_topic, const std::string& reply_topic), (override));
    MOCK_METHOD(void, run, (), (override));
};

#endif // MOCK_RBIC1PROXYIFC_H