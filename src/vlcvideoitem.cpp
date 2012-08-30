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
#include <QTimer>
#include <QDebug>

#include <vlc/vlc.h>

namespace SmartTV {

class VLCVideoItemPrivate
{
public:
    VLCVideoItemPrivate() :
        q_ptr(nullptr), autoPlay(true), dirty(true),
        vlcInstance(nullptr), vlcMedia(nullptr),
        vlcPlayer(nullptr)
    {
        frameTimer.setInterval(20);
    }

    ~VLCVideoItemPrivate()
    {
        libvlc_media_player_stop(vlcPlayer);
        libvlc_media_release(vlcMedia);
        libvlc_media_player_release(vlcPlayer);
        libvlc_free(vlcInstance);
    }

    void initializeVLC()
    {
        libvlc_free(vlcInstance);

        static char const *argv[] =
        {
            "--ffmpeg-hw"
        };
        vlcInstance = libvlc_new(1, argv);
        vlcPlayer = libvlc_media_player_new(vlcInstance);
        vlcPlayerEventManager = libvlc_media_player_event_manager(vlcPlayer);

        libvlc_event_attach(vlcPlayerEventManager, libvlc_MediaPlayerLengthChanged, _event, this);
        libvlc_event_attach(vlcPlayerEventManager, libvlc_MediaPlayerPositionChanged, _event, this);

        libvlc_video_set_callbacks(vlcPlayer, _lock, _unlock, _display, this);
        libvlc_video_set_format_callbacks(vlcPlayer, _setupFrame, nullptr);
    }

    static void _event(const libvlc_event_t *event, void *data)
    {
        VLCVideoItemPrivate *d = static_cast<VLCVideoItemPrivate*>(data);
        switch (event->type) {
        case libvlc_MediaPlayerLengthChanged:
            emit d->q_func()->durationChanged();
            break;
        case libvlc_MediaPlayerPositionChanged:
            emit d->q_func()->positionChanged();
            break;
        default:
            return;
        }
    }

    static unsigned _setupFrame(void **data, char *chroma,
                            unsigned *width, unsigned *height,
                            unsigned *pitches,
                            unsigned *lines)
    {
        VLCVideoItemPrivate *d = static_cast<VLCVideoItemPrivate*>(*data);
        strcpy(chroma, "RV32");
        d->frame = QImage(*width, *height, QImage::Format_RGB32);

        *pitches = *width * 4;
        *lines = *height;
        return 1;
    }

    void _q_updateSourceMedia()
    {
        Q_Q(VLCVideoItem);

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

        if (autoPlay)
            q->play();
        frameTimer.start();

        emit q->positionChanged();
    }

    static void *_lock(void *data, void **pixels)
    {
        ((VLCVideoItemPrivate*)data)->mutex.lock();
        *pixels = ((VLCVideoItemPrivate*)data)->frame.bits();
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
        ((VLCVideoItemPrivate*)data)->dirty = true;
        Q_UNUSED(data);
        Q_UNUSED(id);
    }

    void _q_renderFrame()
    {
        Q_Q(VLCVideoItem);
        if (dirty)
            q->update();
        dirty = false;
    }

private:
    Q_DECLARE_PUBLIC(VLCVideoItem)
    VLCVideoItem *q_ptr;

    QUrl sourceUrl;
    bool autoPlay;
    uint duration;

    QMutex mutex;
    QImage frame;

    bool dirty;

    QTimer frameTimer;

    //VLC
    libvlc_instance_t *vlcInstance;
    libvlc_media_t *vlcMedia;
    libvlc_media_player_t *vlcPlayer;
    libvlc_event_manager_t *vlcPlayerEventManager;
};

VLCVideoItem::VLCVideoItem(QQuickItem *parent) :
    QQuickPaintedItem(parent), d_ptr(new VLCVideoItemPrivate)
{
    d_ptr->q_ptr = this;

    connect(this, SIGNAL(sourceChanged()), this, SLOT(_q_updateSourceMedia()));
    connect(&d_ptr->frameTimer, SIGNAL(timeout()), this, SLOT(_q_renderFrame()));
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

qint64 VLCVideoItem::duration() const
{
    Q_D(const VLCVideoItem);
    if (d->vlcPlayer)
        return libvlc_media_player_get_length(d->vlcPlayer);
    return 0;
}

qint64 VLCVideoItem::position() const
{
    Q_D(const VLCVideoItem);
    if (d->vlcPlayer)
        return libvlc_media_player_get_time(d->vlcPlayer);
    return 0;
}

void VLCVideoItem::setPosition(qint64 position)
{
    Q_D(VLCVideoItem);
    libvlc_media_player_set_time(d->vlcPlayer, position);
}

void VLCVideoItem::paint(QPainter *painter)
{
    Q_D(VLCVideoItem);
    d->mutex.lock();
    painter->drawImage(boundingRect(), d->frame);
    d->mutex.unlock();
}

void VLCVideoItem::componentComplete()
{
    Q_D(VLCVideoItem);
    QQuickPaintedItem::componentComplete();

    d->initializeVLC();
    d->_q_updateSourceMedia();
}

void VLCVideoItem::play()
{
    Q_D(VLCVideoItem);
    libvlc_media_player_play(d->vlcPlayer);
}

void VLCVideoItem::pause()
{
    Q_D(VLCVideoItem);
    libvlc_media_player_pause(d->vlcPlayer);
}

void VLCVideoItem::stop()
{
    Q_D(VLCVideoItem);
    libvlc_media_player_stop(d->vlcPlayer);
}

}

#include "moc_vlcvideoitem.cpp"
