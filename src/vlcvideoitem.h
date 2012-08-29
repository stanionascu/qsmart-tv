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

#ifndef VLCVIDEOITEM_H
#define VLCVIDEOITEM_H

#include <QQuickPaintedItem>

namespace SmartTV {

class VLCVideoItemPrivate;
class VLCVideoItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool autoPlay READ autoPlay WRITE setAutoPlay NOTIFY autoPlayChanged)
    Q_PROPERTY(uint duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(uint position READ position WRITE setPosition NOTIFY positionChanged)
public:
    explicit VLCVideoItem(QQuickItem *parent = 0);
    virtual ~VLCVideoItem();

    const QUrl &source();
    void setSource(const QUrl &source);

    bool autoPlay();
    void setAutoPlay(bool autoPlay);

    uint duration() const;
    uint position() const;
    void setPosition(uint position);
    
signals:
    void sourceChanged();
    void autoPlayChanged();
    void durationChanged();
    void positionChanged();
    
public slots:

protected:
    virtual void paint(QPainter *painter);
    virtual void componentComplete();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_updateVLCMedia())

private:
    Q_DECLARE_PRIVATE(VLCVideoItem)
    VLCVideoItemPrivate *d_ptr;
};

}

#endif // VLCVIDEOITEM_H
