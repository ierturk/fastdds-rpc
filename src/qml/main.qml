import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "TaskHandler Example"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Start Task"
            onClicked: {
                console.log("Button clicked, starting task...");
                taskHandler.startTask("Hello, World!");
            }
        }

        Text {
            id: resultText
            text: "Waiting for task..."
        }
    }

    Connections {
        target: taskHandler
        function onTaskCompleted(result) {
            console.log("Task completed with result:", result);
            resultText.text = result;
        }
    }
}