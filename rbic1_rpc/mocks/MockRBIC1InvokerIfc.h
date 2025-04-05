#ifndef MOCK_RBIC1INVOKERIFC_H
#define MOCK_RBIC1INVOKERIFC_H

#include <gmock/gmock.h>
#include "RBIC1InvokerIfc.h"

class MockRBIC1InvokerIfc : public RBIC1InvokerIfc {
public:
    MOCK_METHOD(void, init, (const std::string& reader_topic, const std::string& writer_topic), (override));
    MOCK_METHOD(void, run, (), (override));
    MOCK_METHOD(void, replyReceived, (const std::string& reply), ());
};

#endif // MOCK_RBIC1INVOKERIFC_H