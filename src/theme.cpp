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

#include "theme.h"

#include <QQmlEngine>

#include "applicationmanager.h"
#include "applicationcategorymodel.h"

namespace SmartTV {

class ThemePrivate
{
public:
    ThemePrivate() :
        q_ptr(nullptr)
    {
    }

    ~ThemePrivate()
    {
    }

    void _q_sort()
    {
        ApplicationManager::instance()->categories()->sortAndFilter(displayCategories);
    }

private:
    Q_DECLARE_PUBLIC(Theme)
    Theme *q_ptr;

    QStringList displayCategories;
};

Theme::Theme(QQuickItem *parent) :
    QQuickItem(parent), d_ptr(new ThemePrivate)
{
    d_ptr->q_ptr = this;
}

Theme::~Theme()
{
    delete d_ptr;
}

ApplicationCategoryModel *Theme::categories()
{
    return ApplicationManager::instance()->categories();
}

const QStringList &Theme::displayCategories()
{
    Q_D(Theme);
    return d->displayCategories;
}

void Theme::setDisplayCategories(const QStringList &categoriesOrder)
{
    Q_D(Theme);
    if (d->displayCategories != categoriesOrder) {
        d->displayCategories = categoriesOrder;
        d->_q_sort();
        emit displayCategoriesChanged();
    }
}

}
