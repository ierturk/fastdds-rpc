#ifndef RBIC1RPCBRIDGEIFC_H
#define RBIC1RPCBRIDGEIFC_H

#include <QObject>
#include <QString>

class RBIC1RpcBridgeIfc : public QObject {
    Q_OBJECT

public:
    explicit RBIC1RpcBridgeIfc(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~RBIC1RpcBridgeIfc() = default;

    // Pure virtual method
    virtual void startRpcTask() = 0;

signals:
    // Pure virtual signal
    virtual void rpcReplyReceived(const QString& reply) = 0;
};

#endif // RBIC1RPCBRIDGEIFC_H