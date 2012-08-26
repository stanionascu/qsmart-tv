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

#include "applicationmanager.h"

#include <qqml.h>
#include <QHash>
#include <QDir>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkProxyFactory>

#include "application.h"
#include "applicationmodel.h"
#include "applicationloader.h"
#include "folderlistmodel.h"
#include "settings.h"

namespace SmartTV {

class ApplicationManagerPrivate
{
public:
    ApplicationManagerPrivate() :
        q_ptr(nullptr)
    {
        categories << "Movies" << "TV Shows" << "Utilities" << "Settings";
    }

    void applySystemProxySettings()
    {
#if defined(Q_OS_UNIX)
        QUrl httpProxyUrl(getenv("http_proxy"));
        if (httpProxyUrl.isEmpty())
            return;

        QNetworkProxy httpProxy(QNetworkProxy::HttpProxy, httpProxyUrl.host(), httpProxyUrl.port(8080),
                                httpProxyUrl.userName(), httpProxyUrl.password());
        QNetworkProxy::setApplicationProxy(httpProxy);
        qDebug() << "Detecting proxies:";
        qDebug() << "HTTP:" << getenv("http_proxy");
        qDebug() << "HTTPS:" << getenv("https_proxy");
        qDebug() << "FTP:" << getenv("ftp_proxy");
#else
        qDebug() << "Windows Proxy Support Not Implemented!";
#endif

    }

    void scanForApps()
    {
        Q_Q(ApplicationManager);
        QDir appsDir(Settings::instance()->appsDir());
        foreach(const QString &entry, appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            if (checkAppOrder(entry) != -1) {
                Application *app = new Application(entry, q);
                applications.append(app);
            } else {
                qCritical() << "Invalid application:" << entry;
            }
        }
    }

    int checkAppOrder(const QString &app)
    {
        if (app == "imports")
            return -1;

        QString appFolder = Settings::instance()->appsDir() + QDir::separator() + app;
        QString appInfoFilePath = appFolder + QDir::separator() + "appinfo.json";
        QFile appInfoFile(appInfoFilePath);
        if (!appInfoFile.open(QFile::ReadOnly))
            return -1;

        QJsonObject appInfo = QJsonDocument::fromJson(appInfoFile.readAll()).object();
        QStringList requiredAppInfoKeys;
        requiredAppInfoKeys << "Name" << "Version" << "Category" << "Icon";
        foreach (const QString &key, requiredAppInfoKeys)
            if (!appInfo.keys().contains(key, Qt::CaseInsensitive))
                return -1;

        return categories.indexOf(appInfo["Category"].toString());
    }

private:
    Q_DECLARE_PUBLIC(ApplicationManager)
    ApplicationManager *q_ptr;

    ApplicationModel applications;
    QStringList categories;

    static ApplicationManager *instance;
};
ApplicationManager* ApplicationManagerPrivate::instance = nullptr;

ApplicationManager::ApplicationManager(QObject *parent) :
    QObject(parent), d_ptr(new ApplicationManagerPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->applySystemProxySettings();
    d_ptr->scanForApps();
}

ApplicationManager::~ApplicationManager()
{
    delete d_ptr;
    ApplicationManagerPrivate::instance = nullptr;
}

ApplicationManager *ApplicationManager::instance()
{
    if (ApplicationManagerPrivate::instance == nullptr)
        ApplicationManagerPrivate::instance = new ApplicationManager;
    return ApplicationManagerPrivate::instance;
}

void ApplicationManager::registerTypes()
{
    qmlRegisterType<SmartTV::ApplicationLoader>("SmartTV", 1, 0, "ApplicationLoader");
    qmlRegisterType<SmartTV::ApplicationModel>("SmartTV", 1, 0, "ApplicationModel");
    qmlRegisterType<SmartTV::FolderListModel>("SmartTV", 1, 0, "FolderListModel");
    qmlRegisterUncreatableType<SmartTV::ApplicationManager>("SmartTV", 1, 0, "ApplicationManager", "Not allowed to create ApplicationManager instances.");
    qmlRegisterUncreatableType<SmartTV::Application>("SmartTV", 1, 0, "Application", "Not allowed to initialize Application instances.");
}

ApplicationModel *ApplicationManager::installedApplications()
{
    Q_D(ApplicationManager);
    return &d->applications;
}


}
