/*
 * Intel MediaSDK QSV utility functions
 *
 * copyright (c) 2013 Luca Barbato
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_QSV_H
#define AVCODEC_QSV_H

#include <stdint.h>
#include <sys/types.h>
#include <mfx/mfxvideo.h>

#include "libavutil/avutil.h"

#define QSV_VERSION_MAJOR 1
#define QSV_VERSION_MINOR 1

#define ASYNC_DEPTH_DEFAULT 4       // internal parallelism
#define SYNC_TIME_DEFAULT 5 * 1000  // 5s
#define TIMEOUT_DEFAULT 5 * 1000    // 5s


typedef struct QSVTimeStamp {
    int64_t pts;
    int64_t dts;
} QSVTimeStamp;

typedef struct QSVSurfaceList {
    mfxFrameSurface1 surface;
    struct QSVSurfaceList *next;
} QSVSurfaceList;

typedef struct QSVContext {
    AVClass *class;
    mfxSession session;
    mfxVideoParam param;
    QSVSurfaceList *surflist;
    QSVTimeStamp *timestamps;
    int nb_timestamps;
    int put_dts_cnt;
    int decoded_cnt;
    mfxSyncPoint sync;
    mfxBitstream bs;
    int last_ret;
    int need_reinit;
    int timeout;
    AVPacketList *pending, *pending_end;
} QSVContext;

int ff_qsv_error(int mfx_err);

int ff_qsv_init(AVCodecContext *s, QSVContext *q);

int ff_qsv_decode(AVCodecContext *s, QSVContext *q,
                  AVFrame *frame, int *got_frame,
                  AVPacket *avpkt);

int ff_qsv_flush(QSVContext *q);

int ff_qsv_close(QSVContext *q);

int ff_qsv_reinit(AVCodecContext *s, QSVContext *q);

#endif /* AVCODEC_QSV_H */
