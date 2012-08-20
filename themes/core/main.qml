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

    Image { anchors.fill: parent; source: "images/videos.jpg"; fillMode: Image.PreserveAspectFit }

    Keys.onEscapePressed: {
        Qt.quit()
    }

    Keys.onReturnPressed: {
        if (applicationsRepeater.itemAt(0).state === "MINI")
            applicationsRepeater.itemAt(0).state = "FULL"
        else
            applicationsRepeater.itemAt(0).state = "MINI"
    }

    Repeater {
        id: applicationsRepeater
        model: applications
        Tile {
            text: name
            anchors.centerIn: root
            width: root.width * 0.4
            height: root.height * 0.4
            color: "blue"
            widget: widgetComponent
            content: contentComponent
            state: "MINI"
        }
    }
}
