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

#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QAbstractItemModel>

namespace SmartTV {

class Application;
class ApplicationModelPrivate;

class ApplicationModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY rowCountChanged)
public:
    enum ApplicationRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        CategoryRole,
        VersionRole,
        IconRole,
        WidgetComponentRole,
        ContentComponentRole
    };

    ApplicationModel(QObject *parent = 0);
    virtual ~ApplicationModel();

    void append(Application *app);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() { return rowCount(QModelIndex()); }

signals:
    void rowCountChanged();

private:
    Q_DECLARE_PRIVATE(ApplicationModel)
    ApplicationModelPrivate *d_ptr;
};

}

#endif // APPLICATIONMODEL_H
