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
#include <QSortFilterProxyModel>

#include "application.h"
#include "applicationcategorymodel.h"
#include "applicationmodel.h"
#include "applicationloader.h"
#include "applicationsettings.h"
#include "folderlistmodel.h"
#include "settings.h"

namespace SmartTV {

class ApplicationManagerPrivate
{
public:
    ApplicationManagerPrivate() :
        q_ptr(nullptr)
    {
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
        QHash<QString, ApplicationModel*> appHashModel;
        foreach(const QString &entry, appsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)) {
            if (checkAppOrder(entry)) {
                Application *app = new Application(entry, q);
                if (!appHashModel.contains(app->category()))
                    appHashModel.insert(app->category(), new ApplicationModel(&categories));
                appHashModel[app->category()]->append(app);

                QString appDataDir = Settings::instance()->appsDataDir() + QDir::separator() + entry;
                QString appCacheDir = appDataDir + QDir::separator() + "cache";
                QString appConfigDir = appDataDir + QDir::separator() + "config";
                appsDataFolders.insert(entry, appDataDir);
                appsCacheFolders.insert(entry, appCacheDir);
                appsConfigFolders.insert(entry, appConfigDir);
            } else {
                qCritical() << "Invalid application:" << entry;
            }
        }

        foreach (QString key, appHashModel.keys()) {
            categories.append(key, appHashModel[key]);
        }

        createDataFolders();
    }

    bool checkAppOrder(const QString &app)
    {
        if (app == "imports")
            return false;

        QString appFolder = Settings::instance()->appsDir() + QDir::separator() + app;
        QString appInfoFilePath = appFolder + QDir::separator() + "appinfo.json";
        QFile appInfoFile(appInfoFilePath);
        if (!appInfoFile.open(QFile::ReadOnly))
            return false;

        QJsonObject appInfo = QJsonDocument::fromJson(appInfoFile.readAll()).object();
        QStringList requiredAppInfoKeys;
        requiredAppInfoKeys << "Name" << "Version" << "Category" << "Icon";
        foreach (const QString &key, requiredAppInfoKeys)
            if (!appInfo.keys().contains(key, Qt::CaseInsensitive))
                return false;

        return true;
    }

    void createDataFolders()
    {
        createFolders(appsDataFolders.values());
        createFolders(appsCacheFolders.values());
        createFolders(appsConfigFolders.values());
    }

    void createFolders(const QStringList &folders)
    {
        foreach(QString path, folders) {
            QDir dir(path);
            if (!dir.exists()) {
                dir.mkpath(path);
                qDebug() << "Creating folder:" << path;
            }
        }
    }

private:
    Q_DECLARE_PUBLIC(ApplicationManager)
    ApplicationManager *q_ptr;

    ApplicationCategoryModel categories;
    QHash<QString, QString> appsDataFolders;
    QHash<QString, QString> appsConfigFolders;
    QHash<QString, QString> appsCacheFolders;

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
    qmlRegisterType<SmartTV::ApplicationSettings>("SmartTV", 1, 0, "Settings");
    qmlRegisterType<SmartTV::ApplicationLoader>("SmartTV", 1, 0, "ApplicationLoader");
    qmlRegisterType<SmartTV::FolderListModel>("SmartTV", 1, 0, "FolderListModel");

    qmlRegisterUncreatableType<SmartTV::ApplicationModel>("SmartTV", 1, 0, "ApplicationModel", "Not allowed to create ApplicationModel instances.");
    qmlRegisterUncreatableType<SmartTV::ApplicationCategoryModel>("SmartTV", 1, 0, "ApplicationCategoryModel", "Not allowed to create ApplicationCategoryModel instances.");
    qmlRegisterUncreatableType<SmartTV::Application>("SmartTV", 1, 0, "Application", "Not allowed to initialize Application instances.");
}

ApplicationCategoryModel *ApplicationManager::categories()
{
    Q_D(ApplicationManager);
    return &d->categories;
}

Application *ApplicationManager::findById(const QString &appId)
{
    Q_D(ApplicationManager);
    return d->categories.findById(appId);
}

QString ApplicationManager::configDir(const QString &appId)
{
    Q_D(ApplicationManager);
    return d->appsConfigFolders.value(appId, "");
}

QString ApplicationManager::cacheDir(const QString &appId)
{
    Q_D(ApplicationManager);
    return d->appsCacheFolders.value(appId, "");
}


}
