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

#ifndef APPLICATIONLOADER_H
#define APPLICATIONLOADER_H

#include <QQuickItem>

class QQmlComponent;

namespace SmartTV {

class ApplicationLoaderPrivate;

class ApplicationLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *item READ item NOTIFY itemChanged)
    Q_PROPERTY(const QString &applicationId READ applicationId WRITE setApplicationId NOTIFY applicationIdChanged)
    Q_PROPERTY(QQmlComponent *sourceComponent READ sourceComponent WRITE setSourceComponent NOTIFY sourceComponentChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)

public:
    explicit ApplicationLoader(QQuickItem *parent = 0);
    virtual ~ApplicationLoader();

    const QString &applicationId();
    void setApplicationId(const QString &id);

    QQmlComponent *sourceComponent();
    void setSourceComponent(QQmlComponent *component);

    QQuickItem *item();
    QQmlComponent::Status status();
    
signals:
    void applicationIdChanged();
    void sourceComponentChanged();
    void itemChanged();
    void loaded();
    void statusChanged();
    
private:
    Q_PRIVATE_SLOT(d_func(), void _q_createItem())
    Q_PRIVATE_SLOT(d_func(), void _q_prepareItem(QQmlComponent::Status))
    Q_PRIVATE_SLOT(d_func(), void _q_updateItem())

private:
    Q_DECLARE_PRIVATE(ApplicationLoader)
    ApplicationLoaderPrivate *d_ptr;
};

}

#endif // APPLICATIONCOMPONENT_H
