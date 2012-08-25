#include "applicationsettings.h"

namespace SmartTV {

class ApplicationSettingsPrivate
{
public:
    ApplicationSettingsPrivate() :
        q_ptr(nullptr)
    {
    }

    ~ApplicationSettingsPrivate()
    {
    }

private:
    Q_DECLARE_PUBLIC(ApplicationSettings)
    ApplicationSettings *q_ptr;
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

bool ApplicationSettings::event(QEvent *event)
{
    return QObject::event(event);
}

}
