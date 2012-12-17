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

    property int tileWidth: 300 * PPMX
    property int tileHeight: 200 * PPMY
    property bool completed: true

    displayCategories: ["Live TV", "Movies", "TV Shows", "Music", "Pictures", "Utilities", "Settings"]

    focus: true

    Keys.onEscapePressed: {
        Qt.quit()
    }

    Image { anchors.fill: parent; source: "images/backgrounds/SKINDEFAULT.jpg"; fillMode: Image.PreserveAspectCrop }

    Text {
        id: timeText;
        anchors.right: parent.right; anchors.top: parent.top
        text: "16:49"
        color: "white"
        font.pixelSize: 16 * PPMX
    }

    Repeater {
        id: categoriesList

        property int currentIndex: 0

        model: categories
        width: root.width; height: root.height
        Item {
            id: applicationsScope
            property int currentIndex: 0
            property int index: model.index
            property alias count: applicationsList.count

            x: model.index * tileWidth + (root.width - tileWidth) / 2 - tileWidth * categoriesList.currentIndex
            y: (root.height - tileHeight) / 2 - currentIndex * tileHeight
            width: tileWidth
            Repeater {
                id: applicationsList
                model: applications
                Tile {
                    selected: applicationsScope.index === categoriesList.currentIndex && applicationsScope.currentIndex === model.index
                    x: 0; y: model.index * tileHeight
                    width: tileWidth; height: tileHeight
                    screen: root
                    appId: model.identifier
                    text: model.name
                    widget: model.widgetComponent
                    content: model.contentComponent
                    iconSource: model.icon.length > 0 ? model.icon : ""
                }
            }

            function itemAt(index) {
                return applicationsList.itemAt(index)
            }

            Behavior on x { SpringAnimation { spring: 2; damping: 0.2; duration: 300 } }
            Behavior on y { SpringAnimation { spring: 2; damping: 0.2; duration: 300 } }
        }
    }

    Keys.onRightPressed: {
        if (categoriesList.currentIndex < categories.count - 1)
            categoriesList.currentIndex ++
    }

    Keys.onLeftPressed: {
        if (categoriesList.currentIndex > 0)
            categoriesList.currentIndex --
    }

    Keys.onDownPressed: {
        var applicationsList = categoriesList.itemAt(categoriesList.currentIndex)
        if (applicationsList.currentIndex < applicationsList.count - 1)
            applicationsList.currentIndex ++
    }

    Keys.onUpPressed: {
        var applicationsList = categoriesList.itemAt(categoriesList.currentIndex)
        if (applicationsList.currentIndex > 0)
            applicationsList.currentIndex --
    }

    Keys.onReturnPressed: {
        var applicationsList = categoriesList.itemAt(categoriesList.currentIndex)
        var item = applicationsList.itemAt(applicationsList.currentIndex)
        if (item)
            item.fullscreen = true
    }
}
