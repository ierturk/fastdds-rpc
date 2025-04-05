#include "RBIC1RpcBridge.h"
#include <QDebug>

RBIC1RpcBridge::RBIC1RpcBridge(boost::asio::io_context& io_context,
                               const QString& reader_topic,
                               const QString& writer_topic,
                               QObject* parent)
    : RBIC1RpcBridgeIfc(parent), io_context_(io_context), invoker_() {
    // Initialize the RBIC1Invoker
    invoker_.init(reader_topic.toStdString(), writer_topic.toStdString());

    // Connect the RBIC1Invoker signal to a Qt signal
    invoker_.replyReceived.connect([this](const std::string& reply) {
        QString qReply = QString::fromStdString(reply);
        QMetaObject::invokeMethod(this, [this, qReply]() {
            emit rpcReplyReceived(qReply);
        }, Qt::QueuedConnection);
    });
}

void RBIC1RpcBridge::startRpcTask() {
    qDebug() << "RBIC1RpcBridge::startRpcTask called";

    // Run the RBIC1Invoker in a separate thread
    boost::asio::post(io_context_, [this]() {
        invoker_.run();
    });
}