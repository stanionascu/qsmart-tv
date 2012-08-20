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

#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QQuickItem>

namespace SmartTV {

class ApplicationModel;
class ThemePrivate;

class Theme : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(SmartTV::ApplicationModel *applications READ applications NOTIFY applicationsChanged)
public:
    explicit Theme(QQuickItem *parent = 0);
    virtual ~Theme();
    
    ApplicationModel *applications();

signals:
    void applicationsChanged();
    
public slots:

private:
    Q_DECLARE_PRIVATE(Theme)
    ThemePrivate *d_ptr;
    
};

}

#endif // THEME_H
