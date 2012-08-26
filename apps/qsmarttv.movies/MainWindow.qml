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

Window {
    id: root
    color: "black"

    focus: true

    Text {
        id: title
        color: "white"
        text: "Movies"
        font.pixelSize: parent.width / 10
    }

    FolderListModel {
        id: listModel
        path: "/home"

        onPathChanged: {
            list.currentIndex = 0
        }
    }

    ListView {
        id: list
        focus: true
        currentIndex: 0
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        model: listModel
        highlight: Component {
            Rectangle {
                height: 60
                width: root.width
                color: "white"
                opacity: 0.5
            }
        }

        delegate: FocusScope {
            height: 60
            focus: true
            width: parent.width
            Text {
                focus: true
                anchors.verticalCenter: parent.verticalCenter
                text: model.name
                color: "white"
                font.pixelSize: root.width / 20
            }

            Keys.onReturnPressed: {
                if (model.isDir)
                    listModel.path = model.path
            }
        }

        Keys.onDownPressed: list.incrementCurrentIndex()
        Keys.onUpPressed: list.decrementCurrentIndex()
    }

    Keys.onTabPressed: {
        windowStack.push(secondaryWindowComponent)
    }

    Component.onCompleted: {
        console.log("context_AppId:", context_AppId)
        console.log("Focus:", focus)
    }

    Component {
        id: secondaryWindowComponent
        Window {
            id: secondaryWindow
            visible: false
            anchors.fill: parent
            color: "green"
            opacity: 0.5
            focus: visible

            Keys.onPressed: {
                console.log("secondaryWindow = keyPress")
            }

            Keys.onTabPressed: {
                windowStack.pop()
            }
        }
    }
}
