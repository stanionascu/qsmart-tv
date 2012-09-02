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

#include "window.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickView>
#include <QJsonValue>

#include <QDesktopWidget>

#include "applicationmanager.h"
#include "application.h"
#include "settings.h"
#include "thememanager.h"

namespace SmartTV {

class WindowPrivate {
public:
    WindowPrivate() :
        q_ptr(nullptr), view(nullptr)
    {
    }

    ~WindowPrivate()
    {
        delete view;
        delete Settings::instance();
    }

    void initializeWindow()
    {
        Q_Q(Window);

        if (view)
            return;

        view = new QQuickView;
        initializeApplicationManager();
        initializeThemeManager();
        registerTypes();

        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->setMinimumSize(QSize(640, 480));
        view->setColor(Qt::black);

        view->setSource(ThemeManager::instance()->themeUrl());

        view->setWindowTitle("QSmartTV");

        q->connect(view->engine(), SIGNAL(quit()), QApplication::instance(), SLOT(quit()));
        q->connect(view, SIGNAL(widthChanged(int)), q, SIGNAL(widthChanged()));
        q->connect(view, SIGNAL(heightChanged(int)), q, SIGNAL(heightChanged()));

        q->connect(view, SIGNAL(widthChanged(int)), q, SLOT(_q_updatePPI()));
        q->connect(view, SIGNAL(heightChanged(int)), q, SLOT(_q_updatePPI()));
    }

    void registerTypes()
    {
        view->engine()->addImportPath(Settings::instance()->appsDir() + QDir::separator() + "imports");
        view->engine()->addImportPath(Settings::instance()->themesDir() + QDir::separator() + Settings::instance()->themeName() +
                                      QDir::separator() + "imports");

        view->engine()->rootContext()->setContextProperty("context_AppId", "global");
        _q_updatePPI();

        qDebug() << view->engine()->importPathList();

        ApplicationManager::instance()->registerTypes();
        ThemeManager::instance()->registerTypes();
    }

    void initializeApplicationManager()
    {
    }

    void initializeThemeManager()
    {
        ThemeManager::instance()->setView(view);
    }

    void showWindow()
    {
        if (Settings::instance()->value(Settings::KeyFullScreen).toBool(true))
            view->showFullScreen();
        else
            view->showNormal();
    }

    void _q_updatePPI()
    {
        if (view) {
            qreal PPMX = (qreal)view->width() / (qreal)view->minimumSize().width();
            qreal PPMY = (qreal)view->height() / (qreal)view->minimumSize().height();

            view->rootContext()->setContextProperty("PPMX", PPMX);
            view->rootContext()->setContextProperty("PPMY", PPMY);
        }
    }

private:
    Q_DECLARE_PUBLIC(Window)
    Window *q_ptr;

    QQuickView *view;
};

Window::Window(QObject *parent) :
    QObject(parent), d_ptr(new WindowPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->initializeWindow();
}

Window::~Window()
{
    delete d_ptr;
}

void Window::setFullScreen(bool value)
{
    Q_D(Window);
    if (Settings::instance()->value(Settings::KeyFullScreen).toBool(true) != value) {
        Settings::instance()->setValue(Settings::KeyFullScreen, value);
        emit fullScreenChanged();
        d->showWindow();
    }
}

bool Window::fullScreen() const
{
    return Settings::instance()->value(Settings::KeyFullScreen).toBool(true);
}

int Window::width() const
{
    Q_D(const Window);
    return d->view->width();
}

int Window::height() const
{
    Q_D(const Window);
    return d->view->height();
}

void Window::show()
{
    Q_D(Window);
    d->showWindow();
}

}

#include "moc_window.cpp"
