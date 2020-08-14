/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "typedefs.h"
#include "timer.h"
#include "vmode.h"

//#define WIDE_PULSE 0x80

// Table is derived from "Encoded Audio Sampling Frequency" in the 9134 PR. Undefined entries are set to 0:
const HI_U32 N_Val[] = { 6272,
                            0,
                            6144,
                            4096,
                            0,
                            0,
                            0,
                            0,
                            12544,
                            0,
                            12288,
                            0,
                            25088,
                            0,
                            24576
                        };

const VModeInfoType VModeTables[NMODES]=
//  ModeIdType                                                   PixClk     TagType                                     HVPos       HVRes           Aspect  _656Type                    PixRep
{
// 60 Hz
    /* 00 */{{ 1, 0,NSM, HI_UNF_ENC_FMT_BUTT,    HI_UNF_ENC_FMT_BUTT},    2517,   {ProgrVNegHNeg,     6000,   {800,525}},     {144, 35},  {640,480},      _4,     {0,96,2,33,48,16,10,0},         0}, // 1. 640 x 480p @ 60 VGA
    /* 01 */{{ 2, 3,NSM, HI_UNF_ENC_FMT_480P_60, HI_UNF_ENC_FMT_480P_60}, 2700,   {ProgrVNegHNeg,     6000,   {858,525}},     {122, 36},  {720,480},      _4or16, {0,62,6,30,60,19,9,0},          0}, // 2,3 720 x 480p
    /* 02 */{{ 4, 0,NSM, HI_UNF_ENC_FMT_720P_60, HI_UNF_ENC_FMT_BUTT},    7417,   {ProgrVPosHPos,     6000,   {1650,750}},    {260, 25},  {1280,720},     _16,    {0,40,5,20,220,110,5,0},        0}, // 4   1280 x 720p
    /* 03 */{{ 5, 0,NSM, HI_UNF_ENC_FMT_1080i_60,HI_UNF_ENC_FMT_BUTT},    7417,   {InterlaceVPosHPos, 6000,   {2200,562}},    {192,20},   {1920,1080},    _16,    {0,44,5,15,148,88,2,1100},      0}, // 5 1920 x 1080i
    /* 04 */{{ 6, 7,NSM, HI_UNF_ENC_FMT_NTSC, HI_UNF_ENC_FMT_BUTT},       2700,   {InterlaceVNegHNeg, 6000,   {1716,264}},    {119,18},   {720,480},      _4or16, {0x03,62,3,15,114,17,5,429},   1}, // 6,7 720 x 480i, pix repl
    /* 05 */{{ 8, 9,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,262}},    {119,18},   {1440,240},     _4or16, {0,124,3,15,114,38,4,0},        1}, // 8,9(1) 1440 x 240p
    /* 06 */{{ 8, 9,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,263}},    {119,18},   {1440,240},     _4or16, {0,124,3,15,114,38,4,0},        1}, // 8,9(2) 1440 x 240p
    /* 07 */{{10,11,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {InterlaceVNegHNeg, 6000,   {3432,525}},    {238,18},   {2880,480},     _4or16, {0,248,3,15,228,76,4,1716},     1}, // 10,11 2880 x 480p
    /* 08 */{{12,13,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {3432,262}},    {238,18},   {2880,240},     _4or16, {0,248,3,15,228,76, 4, 0},      1}, // 12,13(1) 2280 x 240p
    /* 09 */{{12,13,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {3432,263}},    {238,18},   {2880,240},     _4or16, {0,248,3,15,228,76, 4, 0},      1}, // 12,13(2) 2280 x 240p
    /* 0a */{{14,15,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {1716,525}},    {244,36},   {1440,480},     _4or16, {0,124,6,30,120,32, 9, 0},      0}, // 14, 15 1140 x 480p
    /* 0b */{{16, 0,NSM, HI_UNF_ENC_FMT_1080P_60, HI_UNF_ENC_FMT_BUTT},   14850,  {ProgrVPosHPos,     6000,   {2200,1125}},   {192,41},   {1920,1080},    _16,    {0,44,5, 36,148, 88, 4, 0},     0}, // 16 1920 x 1080p

// 50 Hz
    /* 0c */{{17,18,NSM, HI_UNF_ENC_FMT_576P_50, HI_UNF_ENC_FMT_576P_50}, 2700,   {ProgrVNegHNeg,     5000,   {864, 625}},    {132,44},   {720, 576},     _4or16, {0,64,5, 39, 68, 12, 5, 0},     0}, // 17,18 720 x 576p
    /* 0d */{{19, 0,NSM, HI_UNF_ENC_FMT_720P_50, HI_UNF_ENC_FMT_BUTT},    7425,   {ProgrVPosHPos,     5000,   {1980,750}},    {260,25},   {1280,720},     _16,    {0,40,5, 20,220,440, 5, 0},     0}, // 19 1280 x 720p
    /* 0e */{{20, 0,NSM, HI_UNF_ENC_FMT_1080i_50, HI_UNF_ENC_FMT_BUTT},   7425,   {InterlaceVPosHPos, 5000,   {2640, 1125}},  {192,20},   {1920,1080},    _16,    {0,44,5,15,148,528, 2,1320},    0}, // 20 1920 x 1080i
    /* 0f */{{21,22,NSM, HI_UNF_ENC_FMT_PAL, HI_UNF_ENC_FMT_BUTT},       2700,   {InterlaceVNegHNeg, 5000,   {1728, 625}},   {132,22},   {720, 576},     _4,     {3,63,3, 19,138, 24, 2, 432},  1}, // 21,22 1440 x 576i
    /* 10 */{{23,24,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     5000,   {1728,312}},    {132,22},   {1440,288},     _4or16, {0,126,3, 19,138, 24, 2,   0},  1}, // 23,24(1) 1440 x 288p
    /* 11 */{{23,24,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     5000,   {1728,313}},    {132,22},   {1440,288},     _4or16, {0,126,3, 19,138, 24, 2,   0},  1}, // 23,24(2) 1440 x 288p
    /* 12 */{{23,24,  3, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     5000,   {1728,314}},    {132,22},   {1440,288},     _4or16, {0,126,3, 19,138, 24, 2,   0},  1}, // 23,24(3) 1440 x 288p
    /* 13 */{{25,26,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {InterlaceVNegHNeg, 5000,   {3456, 625}},   {264,22},   {2880,576},     _4or16, {0,252,3, 19,276, 48, 2,1728},  1}, // 25, 26 2880 x 576p
    /* 14 */{{27,28,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     5000,   {3456,312}},    {264,22},   {2880,288},     _4or16, {0,252,3,  19,276, 48, 2, 0},   1}, // 27,28(1) 2880 x 288p
    /* 15 */{{27,28,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     5000,   {3456,313}},    {264,22},   {2880,288},     _4or16, {0,252,3,  19,276, 48, 3, 0},   1}, // 27,28(2) 2880 x 288p
    /* 16 */{{27,28,  3, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     5000,   {3456,314}},    {264,22},   {2880,288},     _4or16, {0,252,3,  19,276, 48, 4, 0},   1}, // 27,28(3) 2880 x 288p
    /* 17 */{{29,30,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVPosHNeg,     5000,   {1728,625}},    {264,44},   {1440,576},     _4or16, {0,128,5, 39,136, 24, 5, 0},    0}, // 29,30 1440 x 576p
    /* 18 */{{31, 0,NSM, HI_UNF_ENC_FMT_1080P_50, HI_UNF_ENC_FMT_BUTT},   14850,  {ProgrVPosHPos,     5000,   {2640,1125}},   {192,41},   {1920,1080},    _16,    {0,44,5, 36,148,528, 4, 0},     0}, // 31(1) 1920 x 1080p
    /* 19 */{{32, 0,NSM, HI_UNF_ENC_FMT_1080P_24, HI_UNF_ENC_FMT_BUTT},   7417,   {ProgrVPosHPos,     2400,   {2750,1125}},   {192,41},   {1920,1080},    _16,    { 0,44,5, 36,148,638, 4, 0},    0}, // 32(2) 1920 x 1080p
    /* 1a */{{33, 0,NSM, HI_UNF_ENC_FMT_1080P_25, HI_UNF_ENC_FMT_BUTT},   7425,   {ProgrVPosHPos,     2500,   {2640,1125}},   {192,41},   {1920,1080},    _16,    { 0,44,5, 36,148,528, 4, 0},    0}, // 33(3) 1920 x 1080p
    /* 1b */{{34, 0,NSM, HI_UNF_ENC_FMT_1080P_30, HI_UNF_ENC_FMT_BUTT},   7417,   {ProgrVPosHPos,     3000,   {2200,1125}},   {192,41},   {1920,1080},    _16,    { 0,44,5, 36,148,528, 4, 0},    0}, // 34(4) 1920 x 1080p

};

