#ifndef APPLICATIONLOADER_H
#define APPLICATIONLOADER_H

#include <QQuickItem>

class QQmlComponent;

namespace SmartTV {

class ApplicationLoaderPrivate;

class ApplicationLoader : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *item READ item NOTIFY itemChanged)
    Q_PROPERTY(const QString &applicationId READ applicationId WRITE setApplicationId NOTIFY applicationIdChanged)
    Q_PROPERTY(QQmlComponent *sourceComponent READ sourceComponent WRITE setSourceComponent NOTIFY sourceComponentChanged)

public:
    explicit ApplicationLoader(QQuickItem *parent = 0);
    virtual ~ApplicationLoader();

    const QString &applicationId();
    void setApplicationId(const QString &id);

    QQmlComponent *sourceComponent();
    void setSourceComponent(QQmlComponent *component);

    QQuickItem *item();
    
signals:
    void applicationIdChanged();
    void sourceComponentChanged();
    void itemChanged();
    void loaded();
    
private:
    Q_PRIVATE_SLOT(d_func(), void _q_createItem())
    Q_PRIVATE_SLOT(d_func(), void _q_prepareItem(QQmlComponent::Status))
    Q_PRIVATE_SLOT(d_func(), void _q_updateItem())

private:
    Q_DECLARE_PRIVATE(ApplicationLoader)
    ApplicationLoaderPrivate *d_ptr;
};

}

#endif // APPLICATIONCOMPONENT_H
