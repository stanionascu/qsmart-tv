#include "folderlistmodel.h"

namespace SmartTV {

class FolderListModelPrivate
{
public:
    FolderListModelPrivate() :
        q_ptr(nullptr)
    {
    }

    ~FolderListModelPrivate()
    {
    }

private:
    Q_DECLARE_PUBLIC(FolderListModel)
    FolderListModel *q_ptr;
};

FolderListModel::FolderListModel(QObject *parent) :
    QAbstractItemModel(parent), d_ptr(new FolderListModelPrivate)
{
    d_ptr->q_ptr = this;
}

FolderListModel::~FolderListModel()
{
}

}
