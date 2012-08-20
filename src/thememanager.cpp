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

#include "thememanager.h"

#include <QDir>
#include <QUrl>
#include <QQuickItem>
#include <QQuickView>

#include "applicationmanager.h"
#include "settings.h"
#include "theme.h"

namespace SmartTV {

class ThemeManagerPrivate
{
public:
    ThemeManagerPrivate() :
        q_ptr(nullptr), view(nullptr)
    {
    }

    ~ThemeManagerPrivate()
    {
    }

    void updateRootObjectProperties()
    {
    }

private:
    Q_DECLARE_PUBLIC(ThemeManager)
    ThemeManager *q_ptr;
    QQuickView *view;

    static ThemeManager *instance;
};
ThemeManager *ThemeManagerPrivate::instance = nullptr;

ThemeManager::ThemeManager(QObject *parent) :
    QObject(parent), d_ptr(new ThemeManagerPrivate)
{
    d_ptr->q_ptr = this;
}

ThemeManager::~ThemeManager()
{
    delete d_ptr;
    ThemeManagerPrivate::instance = nullptr;
}

ThemeManager *ThemeManager::instance()
{
    if (ThemeManagerPrivate::instance == nullptr)
        ThemeManagerPrivate::instance = new ThemeManager;
    return ThemeManagerPrivate::instance;
}

void ThemeManager::registerTypes()
{
    qmlRegisterType<SmartTV::Theme>("SmartTV", 1, 0, "Theme");
}

void ThemeManager::setView(QQuickView *view)
{
    Q_D(ThemeManager);
    d->view = view;
    if (d->view)
        d->updateRootObjectProperties();
}

QQuickView *ThemeManager::view()
{
    Q_D(ThemeManager);
    return d->view;
}

QUrl ThemeManager::themeUrl()
{
    return QUrl(Settings::instance()->themesDir() + QDir::separator() + Settings::instance()->themeName() + QDir::separator() + "main.qml");
}

}
