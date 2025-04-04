#ifndef RBIC1RPCBRIDGE_H
#define RBIC1RPCBRIDGE_H

#include <QObject>
#include <QString>
#include <boost/asio.hpp>
#include "RBIC1Invoker.h"

class RBIC1RpcBridge : public QObject {
    Q_OBJECT
public:
    explicit RBIC1RpcBridge(boost::asio::io_context& io_context, QObject* parent = nullptr);

    Q_INVOKABLE void startRpcTask(const QString& reader_topic, const QString& writer_topic);

signals:
    void rpcReplyReceived(const QString& reply);

private:
    boost::asio::io_context& io_context_;
    RBIC1Invoker invoker_;
};

#endif // RBIC1RPCBRIDGE_H