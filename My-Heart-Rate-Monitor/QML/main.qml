import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtCharts 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1

Window {
    visible: true
    width: 600
    height: 900
    title: qsTr("Hello World")
    Rectangle{
        id: bg
        anchors.fill: parent
        color: "#263143"
    }
    Connections{
        target: myNetwork
        onUpdateBPM: {
            valueAxisX.max += 1
            valueAxisX.min += 1
            lineSeries.append(valueAxisX.max, bpm)
        }
    }

    Timer{
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            valueAxisX.max += 1
            valueAxisX.min += 1
            lineSeries.append(valueAxisX.max, 0)
        }
    }

    Item {
        id: root
        anchors.fill: parent
        property string currentData: ""
        SwipeView {
            id: swipeView
            anchors.top:parent.top
            height: parent.height * 0.55
            width:parent.width

            MyDataMonitor{
                id: currentBPM
                value: myNetwork.currentBPM
                type: "BPM"
                value2: myNetwork.currentSpO2
                type2: " % (SpO2)"
            }
        }

        ChartView {
            id: mainChart
            visible: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: myBottomMonitor.top
            anchors.bottomMargin: -20
            anchors.leftMargin: -40
            anchors.rightMargin: -40
            margins.bottom: 0
            margins.left: 0
            margins.right: 0
            //            width: parent.width
            height: parent.height * 0.15

            antialiasing: true
            backgroundColor: "transparent"
            legend.visible: false
//            layer.enabled: true




            AreaSeries {
                color: "#E91E63"
                borderWidth: 0
                borderColor: "transparent"
                axisX: ValueAxis {
                    id: valueAxisX
                    min: 0
                    max: 20
                    visible: false
                }

                axisY: ValueAxis {
                    min: 0
                    max: 120
                    visible: false
                }
                upperSeries: LineSeries{
                    id: lineSeries
                }

            }

        }

        MyBottomMonitor{
            id: myBottomMonitor
            height: parent.height * 0.30
            width: parent.width
            anchors.bottom: parent.bottom
        }

    }

}
