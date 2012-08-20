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

#include "theme.h"

#include "applicationmanager.h"

namespace SmartTV {

class ThemePrivate
{
public:
    ThemePrivate() :
        q_ptr(nullptr)
    {
    }

    ~ThemePrivate()
    {
    }

private:
    Q_DECLARE_PUBLIC(Theme)
    Theme *q_ptr;

};

Theme::Theme(QQuickItem *parent) :
    QQuickItem(parent), d_ptr(new ThemePrivate)
{
    d_ptr->q_ptr = this;
    connect(ApplicationManager::instance(), SIGNAL(installedApplicationsChanged()), this, SIGNAL(applicationsChanged()));
}

Theme::~Theme()
{
    delete d_ptr;
}

ApplicationModel *Theme::applications()
{
    return ApplicationManager::instance()->installedApplications();
}

}
