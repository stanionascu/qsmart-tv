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

FocusScope {
    id: root

    property bool selected: false
    property alias applicationsModel: applicationsList.model
    property int currentIndex: -1

    signal launched(string identifier, Component contentComponent)

    Grid {
        id: applicationsGrid
        anchors {
            margins: 6
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        columns: 2
        spacing: 3
        Repeater {
            id: applicationsList
            Tile {
                appId: model.identifier
                contentComponent: model.contentComponent
                text: model.name
                width: 100 * PPMX
                height: 100 * PPMY
                iconSource: model.icon
                selected: root.selected && currentIndex === model.index
            }
        }
    }

    onFocusChanged: {
        if (focus)
            currentIndex = 0
        else
            currentIndex = -1
    }

    Keys.onRightPressed: {
        selectNext()
    }

    Keys.onLeftPressed: {
        selectPrevious()
    }

    Keys.onUpPressed: {
        selectPreviousRow()
    }

    Keys.onDownPressed: {
        selectNextRow()
    }

    Keys.onReturnPressed: {
        var item = applicationsList.itemAt(currentIndex)
        if (item)
            launched(item.appId, item.contentComponent)
    }

    Keys.onEscapePressed: {
        focus = false
    }

    function selectNextRow() {
        if (currentIndex < applicationsList.count - 2)
            currentIndex += 2
        else if (currentIndex < applicationsList.count - 1)
            selectNext()
        else
            focus = false
    }

    function selectNext() {
        if (currentIndex < applicationsList.count - 1)
            currentIndex ++
        else
            focus = false
    }

    function selectPreviousRow() {
        if (currentIndex >= 2)
            currentIndex -= 2
        else if (currentIndex > 0)
            selectPrevious()
        else
            focus = false
    }

    function selectPrevious() {
        if (currentIndex > 0)
            currentIndex --
        else
            focus = false
    }

    states: [
        State {
            name: "SELECTED"; when: selected
            PropertyChanges {
                target: root
                opacity: 1.0
            }
        },
        State {
            name: "DISABLED"; when: !selected
            PropertyChanges {
                target: root
                opacity: 0.3
            }
        }
    ]

    transitions: [
        Transition {
            from: "SELECTED"
            to: "DISABLED"
            ParallelAnimation {
                NumberAnimation { target: root; properties: "opacity"; duration: 200 }
            }
        },
        Transition {
            from: "DISABLED"
            to: "SELECTED"
            ParallelAnimation {
                NumberAnimation { target: root; properties: "opacity"; duration: 200 }
            }
        }
    ]
}
