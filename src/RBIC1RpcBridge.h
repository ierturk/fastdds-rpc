#ifndef RBIC1RPCBRIDGE_H
#define RBIC1RPCBRIDGE_H

#include "RBIC1RpcBridgeIfc.h"
#include <QString>
#include <boost/asio.hpp>
#include "RBIC1Invoker.h"

class RBIC1RpcBridge : public RBIC1RpcBridgeIfc {
    Q_OBJECT

public:
    explicit RBIC1RpcBridge(boost::asio::io_context& io_context,
                            const QString& reader_topic,
                            const QString& writer_topic,
                            QObject* parent = nullptr);

    Q_INVOKABLE void startRpcTask() override; // Marked as Q_INVOKABLE

signals:
    void rpcReplyReceived(const QString& reply) override;

private:
    boost::asio::io_context& io_context_;
    RBIC1Invoker invoker_;
};

#endif // RBIC1RPCBRIDGE_H