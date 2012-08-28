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

#include "applicationsettings.h"

#include <QDir>
#include <QQmlContext>
#include <QQmlEngine>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QDebug>

#include "applicationmanager.h"
#include "settings.h"

namespace SmartTV {

class ApplicationSettingsPrivate
{
public:
    ApplicationSettingsPrivate() :
        q_ptr(nullptr), context(nullptr)
    {
        ignoredPropertyNames << "objectName" << "name";
    }

    ~ApplicationSettingsPrivate()
    {
        save();
    }

    void load()
    {
        Q_Q(ApplicationSettings);

        if (appId.isEmpty())
            return;

        QString newConfigFilePath = ApplicationManager::instance()->configDir(appId) + QDir::separator() + (name.isEmpty() ? "config.json" : name + ".config.json");
        if (configFilePath == newConfigFilePath)
            return;
        configFilePath = newConfigFilePath;
        qDebug() << "Loading config:" << configFilePath;

        QJsonObject data;

        QFile file(configFilePath);
        if (file.open(QFile::ReadOnly))
            data = QJsonDocument::fromJson(file.readAll()).object();
        file.close();

        if (!data.isEmpty()) {
            for (int i = 0; i < q->metaObject()->propertyCount(); i++) {
                QMetaProperty property = q->metaObject()->property(i);
                if (!ignoredPropertyNames.contains(property.name()) && data.contains(property.name()))
                    property.write(q, data.value(property.name()).toVariant());
            }
        }
    }

    void save()
    {
        Q_Q(ApplicationSettings);

        if (appId.isEmpty() || configFilePath.isEmpty())
            return;

        QJsonObject data;
        for (int i = 0; i < q->metaObject()->propertyCount(); i++) {
            QMetaProperty property = q->metaObject()->property(i);
            if (!ignoredPropertyNames.contains(property.name()))
                data.insert(property.name(), QJsonValue::fromVariant(property.read(q)));
        }

        QFile file(configFilePath);
        if (file.open(QFile::WriteOnly))
            file.write(QJsonDocument(data).toJson());
        file.close();
    }

private:
    Q_DECLARE_PUBLIC(ApplicationSettings)
    ApplicationSettings *q_ptr;

    QString appId;
    QString configFilePath;
    QQmlContext *context;
    QStringList ignoredPropertyNames;
    QString name;
};

ApplicationSettings::ApplicationSettings(QObject *parent) :
    QObject(parent), d_ptr(new ApplicationSettingsPrivate)
{
    d_ptr->q_ptr = this;
}

ApplicationSettings::~ApplicationSettings()
{
    delete d_ptr;
}

void ApplicationSettings::classBegin()
{
    Q_D(ApplicationSettings);
    d->context = QQmlEngine::contextForObject(this);
    Q_ASSERT(d->context);
    if (d->context)
        d->appId = d->context->contextProperty("context_AppId").toString();
    Q_ASSERT(!d->appId.isEmpty());
}

void ApplicationSettings::componentComplete()
{
    Q_D(ApplicationSettings);
    d->load();
}

const QString &ApplicationSettings::name() const
{
    Q_D(const ApplicationSettings);
    return d->name;
}

void ApplicationSettings::setName(const QString &name)
{
    Q_D(ApplicationSettings);
    if (d->name != name) {
        d->save();
        d->name = name;
        d->load();
        emit nameChanged();
    }
}


}
