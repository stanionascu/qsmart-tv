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

#include "vlcvideoitem.h"

#include <QMutex>
#include <QPainter>
#include <QDebug>

#include <vlc/vlc.h>

namespace SmartTV {

class VLCVideoItemPrivate
{
public:
    VLCVideoItemPrivate() :
        q_ptr(nullptr), autoPlay(true), duration(0),
        vlcInstance(nullptr), vlcMedia(nullptr),
        vlcPlayer(nullptr)
    {
        pixels = QImage(640, 480, QImage::Format_RGB32);
    }

    ~VLCVideoItemPrivate()
    {
        libvlc_free(vlcInstance);
    }

    void initializeVLC()
    {
        libvlc_free(vlcInstance);

        vlcInstance = libvlc_new(0, nullptr);
        vlcPlayer = libvlc_media_player_new(vlcInstance);
        libvlc_video_set_callbacks(vlcPlayer, _lock, _unlock, _display, this);
    }

    void _q_updateVLCMedia()
    {
        if (vlcMedia) {
            libvlc_media_player_stop(vlcPlayer);
            libvlc_media_release(vlcMedia);
        }

        if (sourceUrl.isEmpty() || !sourceUrl.isValid())
            return;

        qDebug() << Q_FUNC_INFO << sourceUrl;

        if (sourceUrl.isLocalFile())
            vlcMedia = libvlc_media_new_path(vlcInstance, sourceUrl.toString().toAscii());
        else
            vlcMedia = libvlc_media_new_location(vlcInstance, sourceUrl.toString().toAscii());

        libvlc_media_player_set_media(vlcPlayer, vlcMedia);
        libvlc_video_set_format(vlcPlayer, "RV32", 640, 480, 640*4);

        if (autoPlay)
            libvlc_media_player_play(vlcPlayer);
    }

    static void *_lock(void *data, void **pixels)
    {
        ((VLCVideoItemPrivate*)data)->mutex.lock();
        *pixels = ((VLCVideoItemPrivate*)data)->pixels.bits();
        return nullptr;
    }

    static void _unlock(void *data, void *id, void *const *pixels)
    {
        ((VLCVideoItemPrivate*)data)->mutex.unlock();
        Q_UNUSED(id);
        Q_UNUSED(pixels);
    }

    static void _display(void *data, void *id)
    {
        ((VLCVideoItemPrivate*)data)->q_ptr->update();
        Q_UNUSED(id);
    }

private:
    Q_DECLARE_PUBLIC(VLCVideoItem)
    VLCVideoItem *q_ptr;

    QUrl sourceUrl;
    bool autoPlay;
    uint duration;

    QMutex mutex;
    QImage pixels;

    //VLC
    libvlc_instance_t *vlcInstance;
    libvlc_media_t *vlcMedia;
    libvlc_media_player_t *vlcPlayer;
};

VLCVideoItem::VLCVideoItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), d_ptr(new VLCVideoItemPrivate)
{
    d_ptr->q_ptr = this;

    connect(this, SIGNAL(sourceChanged()), this, SLOT(_q_updateVLCMedia()));
}

VLCVideoItem::~VLCVideoItem()
{
    delete d_ptr;
}

const QUrl &VLCVideoItem::source()
{
    Q_D(VLCVideoItem);
    return d->sourceUrl;
}

void VLCVideoItem::setSource(const QUrl &source)
{
    Q_D(VLCVideoItem);
    if (d->sourceUrl != source) {
        d->sourceUrl = source;
        emit sourceChanged();
    }
}

bool VLCVideoItem::autoPlay()
{
    Q_D(VLCVideoItem);
    return d->autoPlay;
}

void VLCVideoItem::setAutoPlay(bool autoPlay)
{
    Q_D(VLCVideoItem);
    if (d->autoPlay != autoPlay) {
        d->autoPlay = autoPlay;
        emit autoPlayChanged();
    }
}

uint VLCVideoItem::duration() const
{
    return 0;
}

uint VLCVideoItem::position() const
{
    return 0;
}

void VLCVideoItem::setPosition(uint position)
{
    Q_UNUSED(position);
}

void VLCVideoItem::paint(QPainter *painter)
{
    Q_D(VLCVideoItem);
    d->mutex.lock();
    painter->drawImage(0, 0, d->pixels);
    d->mutex.unlock();
}

void VLCVideoItem::componentComplete()
{
    Q_D(VLCVideoItem);
    QQuickPaintedItem::componentComplete();

    d->initializeVLC();
    d->_q_updateVLCMedia();
}

}

#include "moc_vlcvideoitem.cpp"
