#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockRBIC1ProxyIfc.h"

using ::testing::_;
using ::testing::Return;

TEST(RBIC1ProxyIfcTests, InitTest) {
    MockRBIC1ProxyIfc mockProxy;

    EXPECT_CALL(mockProxy, init("RequestTopic", "ReplyTopic"))
        .Times(1);

    mockProxy.init("RequestTopic", "ReplyTopic");
}

TEST(RBIC1ProxyIfcTests, RunTest) {
    MockRBIC1ProxyIfc mockProxy;

    EXPECT_CALL(mockProxy, run())
        .Times(1);

    mockProxy.run();
}