#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockRpcDdsIfc.h"

using ::testing::_;
using ::testing::Return;

TEST(RpcDdsIfcTests, InitTest) {
    MockRpcDdsIfc mockRpcDds;

    EXPECT_CALL(mockRpcDds, init("ReaderTopic", "WriterTopic"))
        .Times(1);

    mockRpcDds.init("ReaderTopic", "WriterTopic");
}

TEST(RpcDdsIfcTests, WriteTest) {
    MockRpcDdsIfc mockRpcDds;

    EXPECT_CALL(mockRpcDds, getWriter())
        .Times(1)
        .WillOnce(Return(nullptr)); // Example: Return a nullptr for the writer

    auto writer = mockRpcDds.getWriter();
    EXPECT_EQ(writer, nullptr);
}