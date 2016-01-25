/*****************************************************************************

Copyright (c) 2012  Michal Necasek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*****************************************************************************/

/*
 * Video miniport static data.
 */

#include <miniport.h>
#include <ntddvdeo.h>
#include <video.h>
#include "videomp.h"

#if defined(ALLOC_PRAGMA)
#pragma data_seg("PAGE")
#endif

/*
 * Legacy resources claimed by the device.
 *
 *               RangeStart        RangeLength
 *               |                 |      RangeInIoSpace
 *               |                 |      |  RangeVisible
 *         +-----+-----+           |      |  |  RangeShareable
 *        low         high         |      |  |  |  RangePassive
 *         v           v           v      v  v  v  v
 */
VIDEO_ACCESS_RANGE  LegacyRanges[] = {
    { 0x000001CE, 0x00000000, 0x00000002, 1, 1, 1, 0 }, /* 0x1CE-0x1CF */
    { 0x000003B0, 0x00000000, 0x0000000C, 1, 1, 1, 0 }, /* 0x3B0-0x3BB */
    { 0x000003C0, 0x00000000, 0x00000020, 1, 1, 1, 0 }, /* 0x3C0-0x3DF */
    { 0x000A0000, 0x00000000, 0x00020000, 0, 0, 1, 0 }  /* 0xA0000-0xBFFFF */
};

ULONG   ulNumLegacyRanges = sizeof( LegacyRanges ) / sizeof( VIDEO_ACCESS_RANGE );

/* Define a resolution for all supported color depths. */
#define MODE_RES( x, y )   \
    { x, y, 8 }, { x, y, 15 }, { x, y, 16 }, { x, y, 24 }, { x, y, 32 }


/* Table of supported modes. Note that the resolutions we can set are
 * quite arbitrary, but there's no point in reporting a massive list.
 * All modes start out as unsupported and must be validated.
 */
VIDEOMP_MODE    VideoModes[] = {
    MODE_RES(  640,  480 ),
    MODE_RES(  800,  600 ),
    MODE_RES( 1024,  768 ),
    MODE_RES( 1152,  864 ),
    MODE_RES( 1280,  720 ),
    MODE_RES( 1280,  800 ),
    MODE_RES( 1280,  960 ),
    MODE_RES( 1360,  768 ),
    MODE_RES( 1360, 1024 ),
    MODE_RES( 1366,  768 ),
    MODE_RES( 1400, 1050 ),
    MODE_RES( 1440,  900 ),
    MODE_RES( 1600, 1200 ),
    MODE_RES( 1680, 1050 ),
    MODE_RES( 1920, 1080 ),
    MODE_RES( 1920, 1200 ),
    MODE_RES( 2048, 1152 ),
    MODE_RES( 2048, 1536 ),
    MODE_RES( 2560, 1600 ),
    MODE_RES( 3840, 2160 ),
    MODE_RES( 7680, 4320 )
};

ULONG   ulAllModes = sizeof( VideoModes ) / sizeof( VideoModes[0] );

/* Video Port interface version. Needs to be global because DriverEntry can
 * no longer pass context information to HwVidFindAdapter. */
ULONG   PortVersion;

#if defined( ALLOC_PRAGMA )
#pragma data_seg()
#endif
