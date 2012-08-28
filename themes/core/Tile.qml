/******************************************************************************
**  Quick SmartTV media center built on Qt5 and QtQuick (QML) technologies.
**  Copyright (C) 2012  Stanislav Ionascu <stanislav.ionascu@gmail.com>
**
**  Contact: Stanislav Ionascu <stanislav.ionascu@gmail.com>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation, Inc.,
**  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/

import QtQuick 2.0
import Theme.Components 1.0
import SmartTV 1.0

Rectangle {
    id: root

    property string appId

    property Item screen: null

    property Component widget
    property Component content
    property alias text: text.text
    property alias iconSource: icon.source
    property int angle: 0

    smooth: true
    clip: false
    state: "MINI"

    Item {
        id: miniContent
        anchors.fill: root
        opacity: 0.0
        ApplicationLoader {
            id: widgetLoader
            anchors.fill: parent
            applicationId: identifier
            //asynchronous: true
        }

        Text {
            id: text
            text: qsTr("Tile")
            color: "white"
            font.pixelSize: root.width / 10
            style: Text.Sunken
            anchors.fill: parent
        }

        Image {
            id: icon
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            height: parent.height
        }

        Behavior on opacity { NumberAnimation { duration: 400 } }
    }

    ApplicationLoader {
        id: contentLoader
        applicationId: appId
        anchors.fill: root
        opacity: 0.0
        transform: Rotation {
            angle: 180
            origin: Qt.vector3d(root.width / 2, root.height / 2, 0)
            axis: Qt.vector3d(0, 1, 0)
        }
        Behavior on opacity { NumberAnimation { duration: 400 } }

        Connections {
            target: contentLoader.item
            ignoreUnknownSignals: true
            onQuit: {
                console.log("Content - Quit!")
                root.state = "MINI"
            }
        }
    }

    BusyIndicator {
        visible: widgetLoader.status !== Loader.Ready && contentLoader.status !== Loader.Ready && icon.status !== Image.Ready
        anchors.centerIn: parent
    }

    PropertyAnimation {
        id: widgetFadeOutAnimation
        target: widgetLoader.item
        properties: "opacity"
        from: 1.0
        to: 0
    }

    transform: Rotation {
        angle: root.angle
        origin: Qt.vector3d(width / 2, height / 2, 0)
        axis: Qt.vector3d(0, 1, 0)
    }

    states: [
        State {
            name: "MINI"
            PropertyChanges {
                target: miniContent
                opacity: 1.0
            }
            PropertyChanges {
                target: widgetLoader
                sourceComponent: widget
                opacity: 1.0
            }
        },
        State {
            name: "FULL"
            PropertyChanges {
                target: root
                angle: 180
                explicit: true
            }
            PropertyChanges {
                target: miniContent
                opacity: 0.0
            }
            PropertyChanges {
                target: contentLoader
                sourceComponent: content
                opacity: 1.0
            }
            ParentChange {
                target: root
                parent: screen
                x: 0; y: 0
                width: screen.width; height: screen.height
            }
        }
    ]

    transitions: [
        Transition {
            from: "MINI"
            to: "FULL"
            ParentAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "x"; duration: 400 }
                    NumberAnimation { target: root; properties: "y"; duration: 400 }
                    NumberAnimation { target: root; properties: "width"; duration: 400 }
                    NumberAnimation { target: root; properties: "height"; duration: 400 }
                }
            }
        },
        Transition {
            from: "FULL"
            to: "MINI"
            ParentAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "x"; duration: 400 }
                    NumberAnimation { target: root; properties: "y"; duration: 400 }
                    NumberAnimation { target: root; properties: "width"; duration: 400 }
                    NumberAnimation { target: root; properties: "height"; duration: 400 }
                    ScriptAction {
                        script: root.focus = true
                    }
                }
            }
        }
    ]
}
