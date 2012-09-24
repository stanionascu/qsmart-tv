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

#ifndef FOLDERLISTMODEL_H
#define FOLDERLISTMODEL_H

#include <QAbstractItemModel>
#include <QFileSystemModel>

namespace SmartTV {

class FolderListModelPrivate;

class FolderListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool showDotDot READ showDotDot WRITE setShowDotDot NOTIFY showDotDotChanged)
    Q_PROPERTY(QStringList filter READ filter WRITE setFilter NOTIFY filterChanged)

public:
    enum FolderFileRoles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        IsDirRole,
        IsFileRole
    };

    FolderListModel(QObject *parent = 0);
    virtual ~FolderListModel();

    const QString &path();
    void setPath(const QString &path);

    bool showDotDot();
    void setShowDotDot(bool show);

    const QStringList &filter();
    void setFilter(const QStringList &filter);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void pathChanged();
    void showDotDotChanged();
    void filterChanged();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_buildList())

private:
    Q_DECLARE_PRIVATE(FolderListModel)
    FolderListModelPrivate *d_ptr;
};

}

#endif // FOLDERLISTMODEL_H
