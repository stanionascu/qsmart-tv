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

#include "folderlistmodel.h"

#include <QDir>
#include <QDebug>

namespace SmartTV {

class FolderListModelPrivate
{
public:
    FolderListModelPrivate() :
        q_ptr(nullptr)
    {
        absolutePath = QDir(".").absolutePath();
        showDotDot = true;
    }

    ~FolderListModelPrivate()
    {
    }

    void _q_buildList()
    {
        Q_Q(FolderListModel);
        q->beginResetModel();
        files = QDir(absolutePath).entryInfoList((showDotDot ? QDir::NoDot : QDir::NoDotAndDotDot) | QDir::AllEntries, QDir::Name);
        foreach(QFileInfo file, files) {
            if (file.isFile() && !filter.contains(file.suffix(), Qt::CaseInsensitive))
                files.removeOne(file);
        }

        q->endResetModel();
    }

private:
    Q_DECLARE_PUBLIC(FolderListModel)
    FolderListModel *q_ptr;

    QFileInfoList files;

    QString absolutePath;
    bool showDotDot;
    QStringList filter;
};

FolderListModel::FolderListModel(QObject *parent) :
    QAbstractListModel(parent), d_ptr(new FolderListModelPrivate)
{
    d_ptr->q_ptr = this;
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    roles[IsDirRole] = "isDir";
    roles[IsFileRole] = "isFile";
    setRoleNames(roles);

    connect(this, SIGNAL(pathChanged()), this, SLOT(_q_buildList()));
    connect(this, SIGNAL(showDotDotChanged()), this, SLOT(_q_buildList()));
    connect(this, SIGNAL(filterChanged()), this, SLOT(_q_buildList()));

    d_ptr->_q_buildList();
}

FolderListModel::~FolderListModel()
{
    delete d_ptr;
}

const QString &FolderListModel::path()
{
    Q_D(FolderListModel);
    return d->absolutePath;
}

void FolderListModel::setPath(const QString &path)
{
    Q_D(FolderListModel);
    QString absolutePath = QDir(path).absolutePath();
    if (absolutePath != d->absolutePath) {
        d->absolutePath = absolutePath;
        emit pathChanged();
    }
}

bool FolderListModel::showDotDot()
{
    Q_D(FolderListModel);
    return d->showDotDot;
}

void FolderListModel::setShowDotDot(bool show)
{
    Q_D(FolderListModel);
    if (d->showDotDot != show) {
        d->showDotDot = show;
        emit showDotDotChanged();
    }
}

const QStringList &FolderListModel::filter()
{
    Q_D(FolderListModel);
    return d->filter;
}

void FolderListModel::setFilter(const QStringList &filter)
{
    Q_D(FolderListModel);
    if (d->filter != filter) {
        d->filter = filter;
        emit filterChanged();
    }
}

int FolderListModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const FolderListModel);
    if (parent.isValid())
        return 0;
    return d->files.count();
}

QVariant FolderListModel::data(const QModelIndex &index, int role) const
{
    Q_D(const FolderListModel);
    if (index.isValid()) {
        if (role == NameRole)
            return d->files[index.row()].fileName();
        else if (role == PathRole)
            return d->files[index.row()].absoluteFilePath();
        else if (role == IsDirRole)
            return d->files[index.row()].isDir();
        else if (role == IsFileRole)
            return d->files[index.row()].isFile();
    }
    return QVariant();
}

}

#include "moc_folderlistmodel.cpp"
