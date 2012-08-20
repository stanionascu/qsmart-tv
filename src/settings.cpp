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

#include "settings.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

namespace SmartTV {

class SettingsPrivate
{
public:
    SettingsPrivate()
    {
        prepareStandardLocations();
        load();
    }
    ~SettingsPrivate()
    {
        save();
    }

    void save()
    {
        QFile file(configFilePath);
        if (!file.open(QFile::WriteOnly))
            qCritical() << "Could not open config file for writing.";
        else
            file.write(QJsonDocument(settings).toJson());
        file.close();
    }

    void load()
    {
        QFile file(configFilePath);
        if (!file.open(QFile::ReadOnly))
            qCritical() << "Could not open config file for reading.";
        else
            settings = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
        cacheSettings();
    }

    void cacheSettings()
    {
        Q_Q(Settings);
        themeName = q->value(Settings::KeyThemeName).toString("core");
    }

    void prepareStandardLocations()
    {
        configFolder = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
                QDir::separator() + qApp->applicationName();

        if (!QDir(configFolder).exists())
            QDir().mkpath(configFolder);

        configFilePath = configFolder + QDir::separator() + "config.json";

        if (!QFile(configFilePath).exists())
            save();

#if defined(APPS_PREFIX)
        appsFolder = QDir(QString(APPS_PREFIX)).absolutePath() + QDir::separator() + "apps";
#else
        appsFolder = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "apps";
#endif

#if defined(THEMES_PREFIX)
        themesFolder = QDir(QString(THEMES_PREFIX)).absolutePath() + QDir::separator() + "themes";
#else
        themesFolder = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "themes";
#endif

        qDebug() << appsFolder;
        qDebug() << themesFolder;
        qDebug() << configFilePath;
    }

    QString keyToString(const Settings::Key &key)
    {
        switch(key) {
        case Settings::KeyFullScreen:
            return "fullScreen";
        case Settings::KeyThemeName:
            return "theme";
        default:
            return "";
        }
    }

private:
    Q_DECLARE_PUBLIC(Settings)
    Settings *q_ptr;
    QJsonObject settings;

    QString configFolder;
    QString configFilePath;

    QString appsFolder;
    QString themesFolder;

    QString themeName;

    static Settings *instance;
};
Settings* SettingsPrivate::instance = nullptr;

Settings *Settings::instance()
{
    if (SettingsPrivate::instance == nullptr)
        SettingsPrivate::instance = new Settings;
    return SettingsPrivate::instance;
}

Settings::Settings(QObject *parent) :
    QObject(parent), d_ptr(new SettingsPrivate)
{
    d_ptr->q_ptr = this;
}

Settings::~Settings()
{
    delete d_ptr;
    SettingsPrivate::instance = 0;
}

void Settings::setValue(const Key &key, const QJsonValue &value)
{
    Q_D(Settings);
    setValue(d->keyToString(key), value);
}

void Settings::setValue(const QString &key, const QJsonValue &value)
{
    Q_D(Settings);
    d->settings.insert(key, value);
}

QJsonValue Settings::value(const Key &key)
{
    Q_D(Settings);
    return value(d->keyToString(key));
}

QJsonValue Settings::value(const QString &key)
{
    Q_D(Settings);
    // FIXME: Bug in QJsonObject
    if (d->settings.contains(key))
        return d->settings.value(key);
    else
        return QJsonValue();
}

const QString &Settings::appsDir() const
{
    Q_D(const Settings);
    return d->appsFolder;
}

const QString &Settings::themesDir() const
{
    Q_D(const Settings);
    return d->themesFolder;
}

const QString &Settings::themeName() const
{
    Q_D(const Settings);
    return d->themeName;
}

}
