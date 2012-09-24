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

#include "applicationcategorymodel.h"

#include <QStringList>
#include <QDebug>

#include "applicationmodel.h"

namespace SmartTV {

class ApplicationCategoryModelPrivate
{
public:
    ApplicationCategoryModelPrivate() :
        q_ptr(nullptr)
    {
        roles[ApplicationCategoryModel::NameRole] = "name";
        roles[ApplicationCategoryModel::ApplicationsRole] = "applications";
    }

    ~ApplicationCategoryModelPrivate()
    {
    }

private:
    Q_DECLARE_PUBLIC(ApplicationCategoryModel)
    ApplicationCategoryModel *q_ptr;

    QStringList categories;
    QList<ApplicationModel*> applications;
    QHash<int, QByteArray> roles;
};

ApplicationCategoryModel::ApplicationCategoryModel(QObject *parent) :
    QAbstractListModel(parent), d_ptr(new ApplicationCategoryModelPrivate)
{
    d_ptr->q_ptr = this;

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
}

ApplicationCategoryModel::~ApplicationCategoryModel()
{
    delete d_ptr;
}

int ApplicationCategoryModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ApplicationCategoryModel);
    if (parent.isValid())
        return 0;
    return d->categories.count();
}

QVariant ApplicationCategoryModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ApplicationCategoryModel);
    if (index.isValid()) {
        if (role == NameRole)
            return d->categories[index.row()];
        else if (role == ApplicationsRole)
            return QVariant::fromValue(d->applications[index.row()]);
    }
    return QVariant();
}

void ApplicationCategoryModel::append(const QString &name, ApplicationModel *model)
{
    Q_D(ApplicationCategoryModel);
    beginInsertRows(QModelIndex(), d->categories.count(), d->categories.count());
    d->categories.append(name);
    d->applications.append(model);
    endInsertRows();
}

Application *ApplicationCategoryModel::findById(const QString &appId)
{
    Q_D(ApplicationCategoryModel);

    foreach (ApplicationModel *model, d->applications) {
        Application *result = model->findById(appId);
        if (result)
            return result;
    }
    return nullptr;
}

void ApplicationCategoryModel::sortAndFilter(const QStringList &categories)
{
    Q_D(ApplicationCategoryModel);
    beginResetModel();

    QStringList unSortedCategories = d->categories;
    QList<ApplicationModel*> unSortedApplications = d->applications;
    d->categories.clear();
    d->applications.clear();

    foreach (QString category, categories) {
        if (unSortedCategories.contains(category)) {
            int index = unSortedCategories.indexOf(category);
            d->categories.append(category);
            d->applications.append(unSortedApplications[index]);
            unSortedApplications.removeAt(index);
            unSortedCategories.removeAt(index);
        }
    }

    // Append categories whose order is not known
    foreach (QString category, unSortedCategories) {
        d->categories.append(category);
        d->applications.append(unSortedApplications[unSortedCategories.indexOf(category)]);
    }

    endResetModel();
}

QHash<int, QByteArray> ApplicationCategoryModel::roleNames() const
{
    Q_D(const ApplicationCategoryModel);
    return d->roles;
}

}
