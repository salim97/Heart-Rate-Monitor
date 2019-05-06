import QtQuick 2.0
//hard to find right name of this
Item {
    id: root
    property string value: ""
    property string type: ""
    property string value2: ""
    property string type2: ""
    Rectangle{
        property int size: parent.height > parent.width ? parent.width : parent.height
        property int space: 100
        anchors.centerIn: parent

        height: size -space
        width: size - space
        radius: size - space
        color: "transparent"
        border.width: 5
        border.color: "#E91E63"
        Rectangle{
            color: "transparent"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2
            Text {
                anchors.centerIn: parent
                font.pixelSize: 60
                font.bold: true
                text: root.value+" "+root.type
                color: "white"
            }
        }
        Rectangle{
            color: parent.border.color
            width: parent.width
            height: 8
            anchors.verticalCenter: parent.verticalCenter
//            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle{
            color: "transparent"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 2
            Text {
                anchors.centerIn: parent
                font.pixelSize: 60
                font.bold: true
                text: root.value2+" "+root.type2
                color: "white"
            }
        }

    }
}
