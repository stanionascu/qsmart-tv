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
import Theme.Components 1.0

Theme {
    id: root

    property int applicationsPanelWidth: root.width * 0.8
    property int applicationsPanelHeight: root.height * 0.65
    property int currentCategoryIndex: 0

    displayCategories: ["Live TV", "Movies", "TV Shows", "Music", "Pictures", "Utilities", "Settings"]

    Item {
        id: mainMenu
        anchors.fill: parent

        Image { anchors.fill: parent; source: "images/backgrounds/SKINDEFAULT.jpg"; fillMode: Image.PreserveAspectCrop }

        Label {
            id: timeLabel
            font.pixelSize: 24 * PPMY
            anchors {
                right: parent.right
                bottom: parent.bottom
                margins: 10
            }
        }

        Label {
            id: dateLabel
            font.pixelSize: 12 * PPMY
            anchors {
                bottom: timeLabel.top
                right: parent.right
                rightMargin: 10
            }
        }

        Timer {
            id: timeUpdateTimer
            interval: 1000
            repeat: true
            running: true
            triggeredOnStart: true
            onTriggered: {
                var date = new Date()
                interval = (60 - date.getSeconds()) * 1000
                timeLabel.text = Qt.formatTime(date, "HH:mm")
                dateLabel.text = Qt.formatDate(date, Qt.TextDate)
            }
        }

        FocusScope {
            id: categoriesFocusScope
            focus: true
            anchors.fill: parent
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                y: 70 * PPMY
                spacing: 30 * PPMX
                Repeater {
                    model: categories
                    Label {
                        text: model.name
                        horizontalAlignment: Text.Center
                        opacity: currentCategoryIndex === model.index ? 1.0 : 0.5
                        font.weight: Font.Bold
                    }
                }
            }

            Keys.onRightPressed: {
                if (currentCategoryIndex < categories.count - 1)
                    currentCategoryIndex ++
            }

            Keys.onLeftPressed: {
                if (currentCategoryIndex > 0)
                    currentCategoryIndex --
            }

            Keys.onDownPressed: {
                focusCurrentCategoryPanel()
            }

            Keys.onEscapePressed: {
                Qt.quit()
            }

            function focusCurrentCategoryPanel() {
                var panel = categoriesPanels.itemAt(currentCategoryIndex)
                if (panel)
                    panel.focus = true
            }
        }

        Row {
            spacing: 30 * PPMX
            x: (root.width - applicationsPanelWidth) / 2 - currentCategoryIndex * (applicationsPanelWidth + spacing)
            y: 110 * PPMY
            Repeater {
                id: categoriesPanels
                model: categories
                ApplicationsPanel {
                    applicationsModel: applications
                    selected: currentCategoryIndex === model.index
                    width: applicationsPanelWidth
                    height: applicationsPanelHeight

                    onFocusChanged: {
                        if (!focus)
                            categoriesFocusScope.focus = true
                    }

                    onLaunched: {
                        applicationLoader.applicationId = identifier
                        applicationLoader.sourceComponent = contentComponent
                        applicationFocusScope.focus = true
                    }
                }
            }
            Behavior on x { NumberAnimation { duration: 200 } }
        }
    }

    FocusScope {
        id: applicationFocusScope
        focus: false
        anchors.fill: parent
        ApplicationLoader {
            id: applicationLoader
            anchors.fill: parent

            onOpacityChanged: {
                if (opacity === 0.0 && !applicationFocusScope.focus)
                    sourceComponent = null
            }
        }
        Connections {
            target: applicationLoader.item
            onQuit: {
                applicationFocusScope.focus = false
                categoriesFocusScope.focusCurrentCategoryPanel()
            }
        }
    }


    states: [
        State {
            name: "MAINMENU"; when: !applicationFocusScope.focus
            PropertyChanges {
                target: mainMenu
                scale: 1.0
            }
            PropertyChanges {
                target: mainMenu
                opacity: 1.0
            }
            PropertyChanges {
                target: applicationLoader
                scale: 1.3
            }
            PropertyChanges {
                target: applicationLoader
                opacity: 0.0
            }
        },
        State {
            name: "APPLICATION"; when: applicationFocusScope.focus
            PropertyChanges {
                target: mainMenu
                scale: 0.7
            }
            PropertyChanges {
                target: mainMenu
                opacity: 0.0
            }
            PropertyChanges {
                target: applicationLoader
                scale: 1.0
            }
            PropertyChanges {
                target: applicationLoader
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            from: "MAINMENU"
            to: "APPLICATION"
            reversible: true
            ParallelAnimation {
                NumberAnimation { target: mainMenu; properties: "opacity,scale"; duration: 100 }
                NumberAnimation { target: applicationLoader; properties: "opacity,scale"; duration: 100 }
            }
        }
    ]
}
