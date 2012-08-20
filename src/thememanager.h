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

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>

class QQuickView;

namespace SmartTV {

class Theme;
class ThemeManagerPrivate;

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    virtual ~ThemeManager();
    static ThemeManager *instance();
    
    void registerTypes();

    void setView(QQuickView *view);
    QQuickView *view();

    QUrl themeUrl();

signals:
    
public slots:

private:
    explicit ThemeManager(QObject *parent = 0);

private:
    Q_DECLARE_PRIVATE(ThemeManager)
    ThemeManagerPrivate *d_ptr;
    
};

}

#endif // THEMEMANAGER_H
