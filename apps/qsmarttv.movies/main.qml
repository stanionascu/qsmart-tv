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

Window {
    id: root
    color: "black"

    Text {
        id: title
        color: "white"
        text: "Movies"
        font.pixelSize: parent.width / 10
    }

    ListModel {
        id: listModel
        ListElement {
            text: "Item 1"
        }
    }

    ListView {
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: listModel

        delegate: Rectangle {
            Text {
                text: model.text
                color: "white"
                font.pixelSize: root.width / 20
            }
        }
    }

    Keys.onEscapePressed: {
        console.log("Movies: quit")
        quit()
    }

    Keys.onReturnPressed: {
        console.log("Movies: showWindow")
        greenWindow.show()
    }

    Window {
        id: greenWindow
        color: "green"
        opacity: 0.5
        visible: false

        Keys.onReturnPressed: {
            console.log("Movies: hideWindow")
            hide()
        }
    }

    Component.onCompleted: {
        console.log("context_AppId:", context_AppId)
    }
}
