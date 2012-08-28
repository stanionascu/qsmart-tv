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

#include "applicationloader.h"

#include <QDebug>
#include <QQmlComponent>
#include <QQmlContext>

#include "applicationmanager.h"
#include "applicationmodel.h"
#include "application.h"

namespace SmartTV {

class ApplicationLoaderPrivate
{
public:
    ApplicationLoaderPrivate() :
        q_ptr(nullptr), component(nullptr), item(nullptr)
    {
    }

    ~ApplicationLoaderPrivate()
    {
    }

    void init()
    {
        Q_Q(ApplicationLoader);
        q->connect(q, SIGNAL(widthChanged()), q, SLOT(_q_updateItem()));
        q->connect(q, SIGNAL(heightChanged()), q, SLOT(_q_updateItem()));
    }

    void setComponent(QQmlComponent *newComponent)
    {
        Q_Q(ApplicationLoader);
        if (component)
            component->disconnect(q);

        component = newComponent;
        if (newComponent)
            q->connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), q, SLOT(_q_prepareItem(QQmlComponent::Status)));
        emit q->statusChanged();
    }

    void _q_createItem()
    {
        Q_Q(ApplicationLoader);
        if (item) {
            item->deleteLater();
            item = nullptr;
        }

        if (!component || appId.isEmpty())
            return;

        if (component->status() == QQmlComponent::Ready) {
            item = qobject_cast<QQuickItem*>(
                        component->create(
                            ApplicationManager::instance()->findById(appId)->context()));
            item->setParent(q);
            item->setParentItem(q);
            item->setPos(QPointF(0, 0));
            item->forceActiveFocus();
            _q_updateItem();
            emit q->loaded();
            emit q->statusChanged();
        }
    }

    void _q_prepareItem(QQmlComponent::Status status)
    {
        if (status == QQmlComponent::Ready)
            _q_createItem();
    }

    void _q_updateItem()
    {
        Q_Q(ApplicationLoader);
        if (item)
            item->setSize(QSizeF(q->width(), q->height()));
    }

private:
    Q_DECLARE_PUBLIC(ApplicationLoader)
    ApplicationLoader *q_ptr;

    QString appId;
    QQmlComponent *component;

    QQuickItem *item;
};

ApplicationLoader::ApplicationLoader(QQuickItem *parent) :
    QQuickItem(parent), d_ptr(new ApplicationLoaderPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->init();

    setFlag(QQuickItem::ItemIsFocusScope, true);
    setFlag(QQuickItem::ItemHasContents, false);
}

ApplicationLoader::~ApplicationLoader()
{
    delete d_ptr;
}

const QString &ApplicationLoader::applicationId()
{
    Q_D(ApplicationLoader);
    return d->appId;
}

void ApplicationLoader::setApplicationId(const QString &id)
{
    Q_D(ApplicationLoader);
    if (d->appId != id) {
        d->appId = id;
        d->_q_createItem();
        emit applicationIdChanged();
        emit itemChanged();
    }
}

QQmlComponent *ApplicationLoader::sourceComponent()
{
    Q_D(ApplicationLoader);
    return d->component;
}

void ApplicationLoader::setSourceComponent(QQmlComponent *component)
{
    Q_D(ApplicationLoader);
    if (d->component != component) {
        d->setComponent(component);
        d->_q_createItem();
        emit sourceComponentChanged();
        emit itemChanged();
    }
}

QQuickItem *ApplicationLoader::item()
{
    Q_D(ApplicationLoader);
    return d->item;
}

QQmlComponent::Status ApplicationLoader::status()
{
    Q_D(ApplicationLoader);
    if (!d->component)
        return QQmlComponent::Null;
    else if (d->component && !d->item)
        return QQmlComponent::Loading;
    else
        return QQmlComponent::Ready;
}

}

#include "moc_applicationloader.cpp"
