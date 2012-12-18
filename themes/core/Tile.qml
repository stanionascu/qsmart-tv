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
    property Component contentComponent

    property alias text: titleText.text
    property alias iconSource: icon.source

    property bool fullscreen: false
    property bool selected: false

    color: "green"

    Item {
        id: miniContent
        anchors.fill: root
        anchors.margins: 6

        Label {
            id: titleText
            font.pixelSize: 18 * PPMY
            style: Text.Sunken
            anchors {
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }
        }

        Image {
            id: icon
            fillMode: Image.PreserveAspectFit
            anchors {
                bottom: titleText.top
                left: parent.left
                right: parent.right
                top: parent.top
            }
        }
    }

    states: [
        State {
            name: "SELECTED"; when: selected
            PropertyChanges {
                target: root
                scale: 1.3
            }
        },
        State {
            name: "DISABLED"; when: !selected
            PropertyChanges {
                target: root
                scale: 1.0
            }
        }
    ]
}
