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

var windowStack = []

function depth() {
    return windowStack.length;
}

function push(window) {
    if (typeof window === "object") {
        var item = null
        if (window.createObject !== undefined) {
            item = init(window)
        } else
            item = window
        windowStack.push(item)

        item.windowStack = root
        item.visible = true
        item.focus = true
        root.currentWindow = item
        var pushAnimation = pushAnimationComponent.createObject(root)
        pushAnimation.window = item
        pushAnimation.start()
    }
}

function pop() {
    if (depth() === 0)
        return;


    var popAnimation = popAnimationComponent.createObject(root)
    var item = windowStack.pop()
    popAnimation.window = item;
    popAnimation.start()
    if (depth() !== 0) {
        item = windowStack[depth() - 1];
        focusOn(item)
        root.currentWindow = item
    } else {
        focusOn(root.parent)
    }
}

function init(component) {
    return component.createObject(root)
}

function focusOn(item) {
    item.focus = true
}
