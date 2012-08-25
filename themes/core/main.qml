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
import SmartTV 1.0

Theme {
    id: root

    property int activeIndex: -1
    property int selectedIndex: 0
    focus: true

    Keys.onEscapePressed: {
        Qt.quit()
    }

    Keys.onReturnPressed: {
        if (applicationsRepeater.itemAt(selectedIndex).state === "MINI") {
            applicationsRepeater.itemAt(selectedIndex).state = "FULL"
        }
    }

    Keys.onRightPressed: {
        if (selectedIndex < applications.count - 1)
            selectedIndex ++
    }

    Keys.onLeftPressed: {
        if (selectedIndex > 0)
            selectedIndex --
    }

    Image { anchors.fill: parent; source: "images/videos.jpg"; fillMode: Image.PreserveAspectFit }

    Repeater {
        id: applicationsRepeater

        anchors.fill: parent

        model: applications
        Tile {
            id: tile
            appId: identifier
            text: name
            x: state === "FULL" ? 0 : (index - selectedIndex) * (root.width * 0.4 + 5) + root.width * 0.6 / 2
            y: state === "FULL" ? 0 : root.height * 0.6 / 2
            width: root.width * 0.4
            height: root.height * 0.4
            color: "blue"
            iconSource: icon.length > 0 ? icon : ""
            widget: widgetComponent
            content: contentComponent
            state: "MINI"

            opacity: index !== selectedIndex ? activeIndex === -1 ? 0.6 : 0.0 : 1.0
            angle: index === selectedIndex ? 0 : index < selectedIndex ? -45 : 45
            scale: index === selectedIndex ? 1.0 : 0.9
            z: index === selectedIndex ? 2 : 1

            Behavior on opacity { NumberAnimation { duration: 500 } }
            onStateChanged: {
                if (state === "FULL")
                    activeIndex = index
                else if (state === "MINI" && activeIndex === index)
                    activeIndex = -1
            }
        }
    }
}
