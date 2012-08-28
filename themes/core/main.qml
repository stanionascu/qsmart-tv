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

    property int tileWidth: root.width * 0.4
    property int tileHeight: root.height * 0.4

    displayCategories: ["Live TV", "Movies", "TV Shows", "Music", "Pictures", "Utilities", "Settings"]

    focus: true

    Keys.onEscapePressed: {
        Qt.quit()
    }

    Image { anchors.fill: parent; source: "images/videos.jpg"; fillMode: Image.PreserveAspectFit }

    ListView {
        id: categoriesList
        currentIndex: 0
        contentX: -(root.width - tileWidth) / 2 + currentIndex * tileWidth; y: (root.height - tileHeight) / 2
        width: root.width; height: root.height
        model: categories
        orientation: ListView.Horizontal
        interactive: false

        delegate: FocusScope {
            id: categoryPanel

            property int categoryIndex: model.index
            property bool active: categoryIndex === categoriesList.currentIndex

            z: active ? 1 : 0
            width: tileWidth; height: root.height
            opacity: categoriesList.currentIndex === categoryIndex ? 1.0 : 0.4
            focus: active

            ListView {
                id: applicationsList
                currentIndex: 0
                contentY: currentIndex * tileHeight
                clip: false
                width: tileWidth
                height: root.height
                model: applications
                interactive: false
                focus: categoryPanel.active

                delegate: Tile {
                    id: tile

                    property bool active: applicationsList.currentIndex === model.index && categoryPanel.active

                    screen: root
                    focus: active
                    opacity: active ? 1.0 : 0.7
                    scale: active ? 1.0 : 0.9
                    width: tileWidth
                    height: tileHeight
                    color: "blue"
                    z: active ? 1 : 0
                    parent: applicationsList
                    state: "MINI"

                    appId: model.identifier
                    text: model.name
                    iconSource: model.icon.length > 0 ? model.icon : ""
                    widget: model.widgetComponent
                    content: model.contentComponent

                    Keys.onReturnPressed: {
                        state = "FULL"
                    }

                    Behavior on scale { NumberAnimation { duration: 400 } }
                    Behavior on opacity { NumberAnimation { duration: 400 } }
                    Behavior on angle { NumberAnimation { duration: 400 } }
                }

                Behavior on contentY { NumberAnimation { duration: 400 } }

                Keys.onDownPressed: { incrementCurrentIndex(); console.log(currentIndex) }
                Keys.onUpPressed: decrementCurrentIndex()
            }

            Behavior on opacity { NumberAnimation { duration: 400 } }
        }

        Keys.onRightPressed: incrementCurrentIndex()
        Keys.onLeftPressed: decrementCurrentIndex()

        Behavior on contentX { NumberAnimation { duration: 400 } }
    }
}
