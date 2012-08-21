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

#include "application.h"

#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickView>

#include "settings.h"
#include "thememanager.h"

namespace SmartTV {

class ApplicationPrivate
{
public:
    ApplicationPrivate() :
        q_ptr(nullptr), context(nullptr),
        widgetComponent(nullptr), contentComponent(nullptr)
    {

    }

    void prepareWidgetComponent()
    {
        if (widgetComponent || widgetFileName.isEmpty())
            return;

        widgetComponent = new QQmlComponent(ThemeManager::instance()->view()->engine(),
                                            appFolder + QDir::separator() + widgetFileName, QQmlComponent::Asynchronous);
    }

    void prepareContentComponent()
    {
        if (contentComponent)
            return;

        contentComponent = new QQmlComponent(ThemeManager::instance()->view()->engine(),
                                             appFolder + QDir::separator() + "main.qml", QQmlComponent::Asynchronous);
    }

    void load(const QString &identifier)
    {
        appFolder = Settings::instance()->appsDir() + QDir::separator() + identifier;
        QString appInfoFilePath = appFolder + QDir::separator() + "appinfo.json";
        QFile appInfoFile(appInfoFilePath);
        if (appInfoFile.open(QFile::ReadOnly)) {
            appId = identifier;
            appInfoMap = QJsonDocument::fromJson(appInfoFile.readAll()).object().toVariantMap();
            category = appInfoMap["Category"].toString();
            widgetFileName = appInfoMap["Widget"].toString();
            if (!appInfoMap["Icon"].toString().isEmpty())
                appIconPath = appFolder + QDir::separator() + appInfoMap["Icon"].toString();
        } else
            qFatal(("Could not load Application Info for:" + identifier).toLatin1());

        qDebug() << identifier << appInfoMap;
    }

private:
    Q_DECLARE_PUBLIC(Application)
    Application *q_ptr;

    QQmlContext *context;
    QQmlComponent *widgetComponent;
    QQmlComponent *contentComponent;

    QString appId;
    QString appIconPath;
    QString category;
    QString widgetFileName;
    QString appFolder;
    QVariantMap appInfoMap;
};

Application::Application(const QString &identifier, QObject *parent) :
    QObject(parent), d_ptr(new ApplicationPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->load(identifier);
}

Application::~Application()
{
    delete d_ptr;
}

const QString &Application::id()
{
    Q_D(Application);
    return d->appId;
}

const QString &Application::category()
{
    Q_D(Application);
    return d->category;
}

const QVariantMap &Application::toVariantMap()
{
    Q_D(Application);
    return d->appInfoMap;
}

QQmlComponent *Application::contentComponent()
{
    Q_D(Application);
    if (!d->contentComponent)
        d->prepareContentComponent();
    return d->contentComponent;
}

QQmlComponent *Application::widgetComponent()
{
    Q_D(Application);
    if (!d->widgetComponent)
        d->prepareWidgetComponent();
    return d->widgetComponent;
}

const QString &Application::icon()
{
    Q_D(Application);
    return d->appIconPath;
}

}
