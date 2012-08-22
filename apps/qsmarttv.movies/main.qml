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
    width: 100
    height: 62

    Text {
        color: "white"
        text: "Movies - fullscreen"
        font.pixelSize: parent.width / 10
    }

    Keys.onEscapePressed: {
        console.log("Movies: quit")
        quit()
    }

    Keys.onLeftPressed: {
        console.log("Movies: left")
    }

    Keys.onRightPressed: {
        console.log("Movies: right")
    }

    Keys.onUpPressed: {
        console.log("Movies: up")
    }

    Keys.onDownPressed: {
        console.log("Movies: down")
    }
}
