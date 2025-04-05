#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockRBIC1InvokerIfc.h"

using ::testing::_;
using ::testing::Return;

TEST(RBIC1InvokerIfcTests, InitTest) {
    MockRBIC1InvokerIfc mockInvoker;

    EXPECT_CALL(mockInvoker, init("ReplyTopic", "RequestTopic"))
        .Times(1);

    mockInvoker.init("ReplyTopic", "RequestTopic");
}

TEST(RBIC1InvokerIfcTests, RunTest) {
    MockRBIC1InvokerIfc mockInvoker;

    EXPECT_CALL(mockInvoker, run())
        .Times(1);

    mockInvoker.run();
}