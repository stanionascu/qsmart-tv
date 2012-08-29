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
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
public:
    enum State {
        Playing,
        Stopped,
        Paused,
        NoMedia,
        InvalidMedia
    };

    explicit VLCVideoItem(QQuickItem *parent = 0);
    virtual ~VLCVideoItem();

    const QUrl &source();
    void setSource(const QUrl &source);

    bool autoPlay();
    void setAutoPlay(bool autoPlay);

    qint64 duration() const;
    qint64 position() const;
    void setPosition(qint64 position);
    
signals:
    void sourceChanged();
    void autoPlayChanged();
    void durationChanged();
    void positionChanged();
    void stateChanged();
    
public slots:
    void play();
    void pause();
    void stop();

protected:
    virtual void paint(QPainter *painter);
    virtual void componentComplete();
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_updateSourceMedia())
    Q_PRIVATE_SLOT(d_func(), void _q_renderFrame())
    Q_PRIVATE_SLOT(d_func(), void _q_updatePosition())

private:
    Q_DECLARE_PRIVATE(VLCVideoItem)
    VLCVideoItemPrivate *d_ptr;
};

}

#endif // VLCVIDEOITEM_H
