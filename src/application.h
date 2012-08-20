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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>

class QQmlComponent;

namespace SmartTV {

class ApplicationPrivate;

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(const QString &identifier, QObject *parent = 0);
    virtual ~Application();

    const QString &id();
    const QString &category();

    QQmlComponent *contentComponent();
    QQmlComponent *widgetComponent();

    const QVariantMap &toVariantMap();

private:
    Q_DECLARE_PRIVATE(Application)
    ApplicationPrivate *d_ptr;
    
};


}

#endif // APPLICATION_H
