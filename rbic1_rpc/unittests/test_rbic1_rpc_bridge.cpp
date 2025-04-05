#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "RBIC1RpcBridge.h"
#include "MockRBIC1RpcBridgeIfc.h"

using ::testing::_;
using ::testing::Return;

class RBIC1RpcBridgeTest : public ::testing::Test {
protected:
    boost::asio::io_context io_context;
    QString reader_topic = "ReplyTopic";
    QString writer_topic = "RequestTopic";
    RBIC1RpcBridge rpcBridge{io_context, reader_topic, writer_topic};
};

TEST_F(RBIC1RpcBridgeTest, StartRpcTaskTest) {
    // Ensure the startRpcTask method can be called without exceptions
    EXPECT_NO_THROW(rpcBridge.startRpcTask());
}

TEST_F(RBIC1RpcBridgeTest, RpcReplyReceivedSignalTest) {
    // Connect a lambda to the rpcReplyReceived signal
    QString expectedReply = "Test Reply";
    bool signalEmitted = false;

    QObject::connect(&rpcBridge, &RBIC1RpcBridge::rpcReplyReceived, [&](const QString& reply) {
        EXPECT_EQ(reply, expectedReply);
        signalEmitted = true;
    });

    // Emit the signal
    emit rpcBridge.rpcReplyReceived(expectedReply);

    // Verify that the signal was emitted
    EXPECT_TRUE(signalEmitted);
}