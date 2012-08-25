#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>

namespace SmartTV {

class ApplicationSettingsPrivate;

class ApplicationSettings : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationSettings(QObject *parent = 0);
    virtual ~ApplicationSettings();
    
    virtual bool event(QEvent *);

signals:
    
public slots:

private:
    Q_DECLARE_PRIVATE(ApplicationSettings)
    ApplicationSettingsPrivate *d_ptr;
};
}

#endif // APPLICATIONSETTINGS_H
