#ifndef MOCK_RBIC1RPCBRIDGEIFC_H
#define MOCK_RBIC1RPCBRIDGEIFC_H

#include "RBIC1RpcBridgeIfc.h"
#include <gmock/gmock.h>

class MockRBIC1RpcBridgeIfc : public RBIC1RpcBridgeIfc {
    Q_OBJECT

public:
    explicit MockRBIC1RpcBridgeIfc(QObject* parent = nullptr)
        : RBIC1RpcBridgeIfc(parent) {}

    // Mock the pure virtual method
    MOCK_METHOD(void, startRpcTask, (), (override));

    // Mock the pure virtual signal
    void rpcReplyReceived(const QString& reply) override {
        emit rpcReplyReceivedSignal(reply);
    }

    // Define a mock signal for testing
    MOCK_METHOD(void, rpcReplyReceivedSignal, (const QString& reply));
};

#endif // MOCK_RBIC1RPCBRIDGEIFC_H