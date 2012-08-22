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

    focus: true

    Keys.onEscapePressed: {
        Qt.quit()
    }

    Keys.onReturnPressed: {
        if (applicationsRepeater.itemAt(applicationsRepeater.selectedIndex).state === "MINI") {
            applicationsRepeater.itemAt(applicationsRepeater.selectedIndex).state = "FULL"
        }
    }

    Keys.onRightPressed: {
        if (applicationsRepeater.selectedIndex < applications.count - 1)
            applicationsRepeater.selectedIndex ++
    }

    Keys.onLeftPressed: {
        if (applicationsRepeater.selectedIndex > 0)
            applicationsRepeater.selectedIndex --
    }

    Image { anchors.fill: parent; source: "images/videos.jpg"; fillMode: Image.PreserveAspectFit }

    Repeater {
        id: applicationsRepeater

        property int selectedIndex: 0

        anchors.fill: root

        model: applications
        Tile {
            text: name
            x: (index - applicationsRepeater.selectedIndex) * (width + 5) + (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: root.width * 0.4
            height: root.height * 0.4
            color: "blue"
            iconSource: icon.length > 0 ? icon : ""
            widget: widgetComponent
            content: contentComponent
            state: "MINI"

            opacity: index !== applicationsRepeater.selectedIndex ? 0.6 : 1.0
            angle: index === applicationsRepeater.selectedIndex ? 0 : index < applicationsRepeater.selectedIndex ? -45 : 45
            scale: index === applicationsRepeater.selectedIndex ? 1.0 : 0.9
            z: index === applicationsRepeater.selectedIndex ? 2 : 1

            Behavior on opacity { NumberAnimation { duration: 500 } }
        }
    }
}
