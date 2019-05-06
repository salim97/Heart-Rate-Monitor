import QtQuick 2.0

Item {
    Rectangle{
        anchors.fill: parent
        color: "#E91E63"
    }

    Row{
        anchors.fill: parent
        anchors.margins: 20
        MyLabel{
            height: parent.height
            width: parent.width / 3
            value: myNetwork.avgBPM
            subtext: "AVG"
            type: "BPM"

        }
        MyLabel{
            height: parent.height
            width: parent.width / 3
            value: myNetwork.minBPM
            subtext: "MIN"
            type: "BPM"

        }
        MyLabel{
            height: parent.height
            width: parent.width / 3
            value: myNetwork.maxBPM
            subtext: "MAX"
            type: "BPM"
        }

    }
}
