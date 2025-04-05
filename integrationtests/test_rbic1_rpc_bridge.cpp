#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MockRBIC1RpcBridgeIfc.h"

using ::testing::_;
using ::testing::Return;

class RBIC1RpcBridgeTest : public ::testing::Test {
protected:
    MockRBIC1RpcBridgeIfc mockBridge;
};

TEST_F(RBIC1RpcBridgeTest, StartRpcTaskTest) {
    // Expect the startRpcTask method to be called once
    EXPECT_CALL(mockBridge, startRpcTask())
        .Times(1);

    // Call the method
    mockBridge.startRpcTask();
}

TEST_F(RBIC1RpcBridgeTest, RpcReplyReceivedSignalTest) {
    // Expect the rpcReplyReceivedSignal mock method to be called with a specific value
    QString expectedReply = "Test Reply";
    EXPECT_CALL(mockBridge, rpcReplyReceivedSignal(expectedReply))
        .Times(1);

    // Emit the signal
    mockBridge.rpcReplyReceived(expectedReply);
}