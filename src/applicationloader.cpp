#include "applicationloader.h"

#include <QDebug>
#include <QQmlComponent>
#include <QQmlContext>

#include "applicationmanager.h"
#include "application.h"

namespace SmartTV {

class ApplicationLoaderPrivate
{
public:
    ApplicationLoaderPrivate() :
        q_ptr(nullptr), component(nullptr), item(nullptr)
    {
    }

    ~ApplicationLoaderPrivate()
    {
    }

    void init()
    {
        Q_Q(ApplicationLoader);
        q->connect(q, SIGNAL(widthChanged()), q, SLOT(_q_updateItem()));
        q->connect(q, SIGNAL(heightChanged()), q, SLOT(_q_updateItem()));
    }

    void setComponent(QQmlComponent *newComponent)
    {
        Q_Q(ApplicationLoader);
        if (component)
            component->disconnect(q);

        component = newComponent;
        if (newComponent)
            q->connect(component, SIGNAL(statusChanged(QQmlComponent::Status)), q, SLOT(_q_prepareItem(QQmlComponent::Status)));
    }

    void _q_createItem()
    {
        Q_Q(ApplicationLoader);
        if (item) {
            item->deleteLater();
            item = nullptr;
        }

        if (!component || appId.isEmpty() || !ApplicationManager::instance()->installedApplications()->contains(appId))
            return;

        if (component->status() == QQmlComponent::Ready) {
            item = qobject_cast<QQuickItem*>(
                        component->create(
                            ApplicationManager::instance()->installedApplications()->byId(appId)->context()));
            item->setParent(q);
            item->setParentItem(q);
            item->setPos(QPointF(0, 0));
            item->forceActiveFocus();
            _q_updateItem();
            emit q->loaded();
        }
    }

    void _q_prepareItem(QQmlComponent::Status status)
    {
        if (status == QQmlComponent::Ready)
            _q_createItem();
    }

    void _q_updateItem()
    {
        Q_Q(ApplicationLoader);
        if (item)
            item->setSize(QSizeF(q->width(), q->height()));
    }

private:
    Q_DECLARE_PUBLIC(ApplicationLoader)
    ApplicationLoader *q_ptr;

    QString appId;
    QQmlComponent *component;

    QQuickItem *item;
};

ApplicationLoader::ApplicationLoader(QQuickItem *parent) :
    QQuickItem(parent), d_ptr(new ApplicationLoaderPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->init();

    setFlag(QQuickItem::ItemIsFocusScope, true);
    setFlag(QQuickItem::ItemHasContents, false);
}

ApplicationLoader::~ApplicationLoader()
{
    delete d_ptr;
}

const QString &ApplicationLoader::applicationId()
{
    Q_D(ApplicationLoader);
    return d->appId;
}

void ApplicationLoader::setApplicationId(const QString &id)
{
    Q_D(ApplicationLoader);
    if (d->appId != id) {
        d->appId = id;
        d->_q_createItem();
        emit applicationIdChanged();
        emit itemChanged();
    }
}

QQmlComponent *ApplicationLoader::sourceComponent()
{
    Q_D(ApplicationLoader);
    return d->component;
}

void ApplicationLoader::setSourceComponent(QQmlComponent *component)
{
    Q_D(ApplicationLoader);
    if (d->component != component) {
        d->setComponent(component);
        d->_q_createItem();
        emit sourceComponentChanged();
        emit itemChanged();
    }
}

QQuickItem *ApplicationLoader::item()
{
    Q_D(ApplicationLoader);
    return d->item;
}

}

#include "moc_applicationloader.cpp"
