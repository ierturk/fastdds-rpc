import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "RBIC1RpcBridge Example"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Start RPC Task"
            onClicked: {
                console.log("Button clicked, starting RPC task...");
                rpcBridge.startRpcTask("ReplyTopic", "RequestTopic");
            }
        }

        Text {
            id: replyText
            text: "Waiting for reply..."
        }
    }

    Connections {
        target: rpcBridge
        function onRpcReplyReceived(reply) {
            console.log("RPC Reply Received:", reply);
            replyText.text = reply;
        }
    }
}