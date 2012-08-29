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

    property bool showControls: true
    property alias source: video.source

    VLCVideo {
        id: video
        anchors.fill: parent
        autoPlay: true
    }


    ProgressIndicator {
        id: slider
        value: video.position
        maximum: video.duration
        height: root.height / 20
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        opacity: showControls ? 1.0 : 0.0

        Behavior on opacity { NumberAnimation { duration: 400 } }
    }
}
