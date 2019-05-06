import QtQuick 2.0

Item {
    id: root
    //data property
    property string value: ""
    property string type: ""
    property string subtext: ""
    // design property
    property string textColor: "white"
    property string textColorALT: "white"//"#a0808f"
    property int mainTextSize: width * 0.50


    Text {
        id: mainText
        anchors.centerIn: parent
        height: root.mainTextSize
        width: root.mainTextSize
        minimumPointSize: 10
        font.pointSize: 600
        fontSizeMode: Text.Fit
        text: root.value
        color: root.textColor
    }

    Text {
        anchors.verticalCenter: mainText.verticalCenter
        anchors.left: mainText.right
        text: root.type
        font.pixelSize: 30
        color: root.textColorALT
    }

    Text{
        anchors.top: mainText.bottom
        anchors.topMargin: -20
        anchors.horizontalCenter: mainText.horizontalCenter
        width: mainText.width - mainText.width * 0.15
        height: mainText.height * 0.70
        text: root.subtext
        color: root.textColorALT

        minimumPointSize: 10
        font.pointSize: 600
        fontSizeMode: Text.HorizontalFit
    }

}
