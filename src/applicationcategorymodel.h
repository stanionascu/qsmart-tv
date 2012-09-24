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

#ifndef APPLICATIONCATEGORYMODEL_H
#define APPLICATIONCATEGORYMODEL_H

#include <QAbstractItemModel>

namespace SmartTV {

class Application;
class ApplicationModel;

class ApplicationCategoryModelPrivate;
class ApplicationCategoryModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY rowCountChanged)
public:
    enum ApplicationCategoryRoles {
        NameRole = Qt::UserRole + 1,
        ApplicationsRole
    };

    explicit ApplicationCategoryModel(QObject *parent = 0);
    virtual ~ApplicationCategoryModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    void append(const QString &name, ApplicationModel *model);

    Application *findById(const QString &appId);
    void sortAndFilter(const QStringList &categories);

    inline int count() { return rowCount(QModelIndex()); }
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void rowCountChanged();

private:
    Q_DECLARE_PRIVATE(ApplicationCategoryModel)
    ApplicationCategoryModelPrivate *d_ptr;
};


}

#endif // APPLICATIONCATEGORYMODEL_H
