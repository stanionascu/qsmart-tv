#ifndef FOLDERLISTMODEL_H
#define FOLDERLISTMODEL_H

#include <QAbstractItemModel>

namespace SmartTV {

class FolderListModelPrivate;

class FolderListModel : public QAbstractItemModel
{
public:
    FolderListModel(QObject *parent = 0);
    virtual ~FolderListModel();

private:
    Q_DECLARE_PRIVATE(FolderListModel)
    FolderListModelPrivate *d_ptr;
};

}

#endif // FOLDERLISTMODEL_H
