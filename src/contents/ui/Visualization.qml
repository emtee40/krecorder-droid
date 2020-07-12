import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.2 as Controls
import KRecorder 1.0

Item {
    id: visualization
    
    property int maxBarHeight
    property int animationIndex // which index rectangle is being expanded
    property var volumes: []
    property bool showLine
    
    Component.onCompleted: {
        AudioRecorder.prober.maxVolumes = width / 4;
        AudioPlayer.prober.maxVolumes = width / 4;
    }
    
    onWidthChanged: {
        AudioRecorder.prober.maxVolumes = width / 4;
        AudioPlayer.prober.maxVolumes = width / 4;
    }
    
    // central line
    Rectangle {
        visible: showLine
        id: centralLine
        width: parent.width
        height: 3
        anchors.verticalCenter: parent.verticalCenter
        color: "#e0e0e0"
    }
    
    ListView {
        model: visualization.volumes
        orientation: Qt.Horizontal
        
        interactive: false
        anchors.top: centralLine.top
        height: maxBarHeight
        width: parent.width
        
        delegate: Item {
            width: 4
            // below centre line
            Rectangle {
                color: "#616161"
                width: 2
                height: index === animationIndex ? 0 : maxBarHeight * modelData / 1000
                antialiasing: true
                
                Behavior on height {
                    SmoothedAnimation {
                        duration: 500
                    }
                }
            }
            // above centre line
            Rectangle {
                color: "#616161"
                width: 2
                height: index == animationIndex ? 0 : maxBarHeight * modelData / 1000
                antialiasing: true
                y: -height
                
                Behavior on height {
                    SmoothedAnimation {
                        duration: 500
                    }
                }
            }
        }
    }
}
