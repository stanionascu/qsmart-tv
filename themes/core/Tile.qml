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

    property bool fullscreen: false
    property bool selected: false

    smooth: true
    clip: false
    color: "blue"

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
            font.pixelSize: 20 * PPMY
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

        Behavior on opacity { NumberAnimation { duration: 400 } }

        Connections {
            target: contentLoader.item
            ignoreUnknownSignals: true
            onQuit: {
                root.fullscreen = false
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

    states: [
        State {
            name: "MINI"; when: !selected && !fullscreen
            PropertyChanges {
                target: miniContent
                opacity: 1.0
            }
            PropertyChanges {
                target: widgetLoader
                sourceComponent: widget
                opacity: 1.0
            }
            PropertyChanges {
                target: root
                opacity: 0.7
            }
            PropertyChanges {
                target: root
                scale: 0.9
            }
            PropertyChanges {
                target: root
                z: 0
            }
        },
        State {
            name: "SELECTED"
            extend: "MINI"; when: selected && !fullscreen
            PropertyChanges {
                target: root
                opacity: 1.0
            }
            PropertyChanges {
                target: root
                scale: 1.0
            }
            PropertyChanges {
                target: root
                z: 1
            }
        },
        State {
            name: "FULL"; when: fullscreen
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
            from: "SELECTED"
            to: "FULL"
            ParentAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "x"; duration: 200 }
                    NumberAnimation { target: root; properties: "y"; duration: 200 }
                    NumberAnimation { target: root; properties: "width"; duration: 200 }
                    NumberAnimation { target: root; properties: "height"; duration: 200 }
                }
            }
        },
        Transition {
            from: "FULL"
            to: "SELECTED"
            ParentAnimation {
                ParallelAnimation {
                    NumberAnimation { target: root; properties: "x"; duration: 200 }
                    NumberAnimation { target: root; properties: "y"; duration: 200 }
                    NumberAnimation { target: root; properties: "width"; duration: 200 }
                    NumberAnimation { target: root; properties: "height"; duration: 200 }
                }
            }
        }
    ]
}
