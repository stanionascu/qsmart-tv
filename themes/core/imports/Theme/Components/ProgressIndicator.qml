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

Rectangle {
    id: root

    property bool inderterminate: false
    property int minimum: 0
    property int maximum: 100
    property int value: 0

    color: "white"

    Rectangle {
        id: total
        height: root.height / 10
        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.left: root.left
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        color: "gray"
        Rectangle {
            id: position
            color: "black"
            anchors.left: total.left
            anchors.top: total.top
            anchors.bottom: total.bottom
            width: inderterminate ? total.width : total.width * (value / (maximum - minimum))
        }
    }

    SequentialAnimation {
        running: inderterminate
        loops: Animation.Infinite
        NumberAnimation {
            target: position
            properties: "opacity"
            from: 0
            to: 1.0
            duration: 500
        }
        NumberAnimation {
            target: position
            properties: "opacity"
            from: 1.0
            to: 0
            duration: 500
        }
    }

}
