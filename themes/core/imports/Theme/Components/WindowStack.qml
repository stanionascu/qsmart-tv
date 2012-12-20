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
import "WindowStack.js" as Stack

Item {
    id: root

    property int depth: 0
    property Item currentWindow: null

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    function push(window) {
        if (typeof window === "string") {
            var pathCheckRegex = /^file:|^\//i
            if (!pathCheckRegex.test(window))
                window = context_AppPath + "/" + window
            window = Qt.createComponent(window)
            if (window.status === Component.Error)
                console.log(window.errorString())
        }
        Stack.push(window)
        depth = Stack.windowStack.length
    }

    function pop() {
        Stack.pop()
        depth = Stack.windowStack.length
    }

    Component {
        id: pushAnimationComponent
        ParallelAnimation {
            id: animation

            property Item window: null

            NumberAnimation { target: window; properties: "opacity"; from: 0.0; to: 1.0; duration: 400 }

            onRunningChanged: {
                if (!running)
                    animation.destroy()
            }
        }
    }

    Component {
        id: popAnimationComponent
        ParallelAnimation {
            id: animation

            property Item window: null

            NumberAnimation { target: window; properties: "opacity"; from: 1.0; to: 0.0; duration: 400 }

            onRunningChanged: {
                if (!running)
                    animation.destroy()
            }

            Component.onDestruction: {
                if (window)
                    window.destroy()
            }
        }
    }
}
