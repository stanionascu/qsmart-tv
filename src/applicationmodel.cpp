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

#include "applicationmodel.h"

#include <QModelIndex>
#include <QQmlComponent>
#include <QQmlContext>
#include <QStringList>

#include "application.h"

namespace SmartTV {

class ApplicationModelPrivate
{
public:
    ApplicationModelPrivate() :
        q_ptr(nullptr)
    {
        roles[ApplicationModel::IdRole] = "identifier";
        roles[ApplicationModel::CategoryRole] = "category";
        roles[ApplicationModel::NameRole] = "name";
        roles[ApplicationModel::VersionRole] = "version";
        roles[ApplicationModel::IconRole] = "icon";
        roles[ApplicationModel::WidgetComponentRole] = "widgetComponent";
        roles[ApplicationModel::ContentComponentRole] = "contentComponent";
        roles[ApplicationModel::ContextRole] = "context";
    }

    ~ApplicationModelPrivate()
    {
    }

private:
    Q_DECLARE_PUBLIC(ApplicationModel)
    ApplicationModel *q_ptr;

    QStringList applicationIds;
    QList<Application*> applicationList;
    QHash<int, QByteArray> roles;
};

ApplicationModel::ApplicationModel(QObject *parent) :
    QAbstractListModel(parent), d_ptr(new ApplicationModelPrivate)
{
    d_ptr->q_ptr = this;

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
}

ApplicationModel::~ApplicationModel()
{
    delete d_ptr;
}

void ApplicationModel::append(Application *app)
{
    Q_D(ApplicationModel);
    beginInsertRows(QModelIndex(), d->applicationList.count(), d->applicationList.count());
    d->applicationList.append(app);
    d->applicationIds.append(app->id());
    endInsertRows();
}

int ApplicationModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ApplicationModel);
    if (parent.isValid())
        return 0;
    else
        return d->applicationList.count();
}

QVariant ApplicationModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ApplicationModel);
    if (index.isValid() && !index.parent().isValid()) {
        if (role == IdRole)
            return d->applicationList[index.row()]->id();
        else if (role == CategoryRole)
            return d->applicationList[index.row()]->category();
        else if (role == NameRole)
            return d->applicationList[index.row()]->toVariantMap()["Name"];
        else if (role == VersionRole)
            return d->applicationList[index.row()]->toVariantMap()["Version"];
        else if (role == IconRole)
            return d->applicationList[index.row()]->icon();
        else if (role == WidgetComponentRole)
            return QVariant::fromValue(d->applicationList[index.row()]->widgetComponent());
        else if (role == ContentComponentRole)
            return QVariant::fromValue(d->applicationList[index.row()]->contentComponent());
        else if (role == ContextRole)
            return QVariant::fromValue(d->applicationList[index.row()]->context());
    }
    return QVariant();
}

Application *ApplicationModel::findById(const QString &id)
{
    Q_D(ApplicationModel);
    if (!d->applicationIds.contains(id))
        return nullptr;
    return d->applicationList[d->applicationIds.indexOf(id)];
}

bool ApplicationModel::contains(const QString &id)
{
    Q_D(ApplicationModel);
    return d->applicationIds.contains(id);
}

QHash<int, QByteArray> ApplicationModel::roleNames() const
{
    Q_D(const ApplicationModel);
    return d->roles;
}

}
