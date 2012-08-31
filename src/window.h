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

#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>

namespace SmartTV {

class WindowPrivate;

class Window : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(bool fullScreen WRITE setFullScreen READ fullScreen NOTIFY fullScreenChanged)
public:
    explicit Window(QObject *parent = 0);
    ~Window();

    void setFullScreen(bool value);
    bool fullScreen() const;

    int width() const;
    int height() const;
    
signals:
    void quit();
    void fullScreenChanged();
    void widthChanged();
    void heightChanged();
    
public slots:
    void show();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_updatePPI())

private:
    Q_DECLARE_PRIVATE(Window)
    WindowPrivate *d_ptr;
};

}

#endif // WINDOW_H
