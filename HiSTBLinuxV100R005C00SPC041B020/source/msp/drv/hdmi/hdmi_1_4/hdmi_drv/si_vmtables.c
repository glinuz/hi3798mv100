/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_timer.h"
#include "si_vmode.h"

#define WIDE_PULSE 0x80

// Table is derived from "Encoded Audio Sampling Frequency" in the 9134 PR. Undefined entries are set to 0:
const HI_U32 N_Val[] = { 6272,            //0:44.1 kHz
                            0,
                            6144,         //2:48 kHz
                            4096,         //3:32 kHz
                            0,
                            0,
                            0,
                            0,
                            12544,        //8:88 kHz
                            0,
                            12288,        //a:96 kHz
                            0,
                            25088,        //c:176.4kHz
                            0,
                            24576         //e:192 kHz
                        };

// Table is derived from "Encoded Audio Sampling Frequency" in the 9134 PR. Undefined entries are set to 0:
const HI_U32 N_Val_4K[] = { 4704,         //0:44.1 kHz
                            0,
                            5120,         //2:48 kHz
                            3072,         //3:32 kHz
                            0,
                            0,
                            0,
                            0,
                            9408,         //8:88 kHz
                            0,
                            10240,        //a:96 kHz
                            0,
                            18816,        //c:176.4kHz
                            0,
                            20480         //e:192 kHz
                        };


const VModeInfoType VModeTables[NMODES]=
{
// 60 Hz
    /* 00 */{{ 1, 0,NSM, HI_UNF_ENC_FMT_861D_640X480_60,HI_UNF_ENC_FMT_BUTT},    2517,   {ProgrVNegHNeg,     6000,   {800,525}},     {144, 35},  {640,480},      _4,     {0,96,2,33,48,16,10,0},         0}, // 1. 640 x 480p @ 60 VGA
    /* 01 */{{ 2, 0,NSM, HI_UNF_ENC_FMT_480P_60, HI_UNF_ENC_FMT_480P_60/*3:HI_UNF_ENC_FMT_480P_60 16:9*/}, 2700,   {ProgrVNegHNeg,     6000,   {858,525}},     {122, 36},  {720,480},      _4, {0,62,6,30,60,19,9,0},          0}, // 2,3 720 x 480p
    /* 01 */{{ 3, 0,NSM, HI_UNF_ENC_FMT_480P_60, HI_UNF_ENC_FMT_480P_60/*3:HI_UNF_ENC_FMT_480P_60 16:9*/}, 2700,   {ProgrVNegHNeg,     6000,   {858,525}},     {122, 36},  {720,480},      _16, {0,62,6,30,60,19,9,0},         0}, // 2,3 720 x 480p
    /* 02 */{{ 4, 0,NSM, HI_UNF_ENC_FMT_720P_60, HI_UNF_ENC_FMT_BUTT},    7417,   {ProgrVPosHPos,     6000,   {1650,750}},    {260, 25},  {1280,720},     _16,    {0,40,5,20,220,110,5,0},        0}, // 4   1280 x 720p
    /* 03 */{{ 5, 0,NSM, HI_UNF_ENC_FMT_1080i_60,HI_UNF_ENC_FMT_BUTT},    7417,   {InterlaceVPosHPos, 6000,   {2200,562}},    {192,20},   {1920,1080},    _16,    {0,44,5,15,148,88,2,1100},      0}, // 5 1920 x 1080i
    /* 04 */{{ 6, 0,NSM, HI_UNF_ENC_FMT_NTSC, HI_UNF_ENC_FMT_NTSC/*7:HI_UNF_ENC_FMT_NTSC:480i@60,16:9*/},       2700,   {InterlaceVNegHNeg, 6000,   {1716,264}},    {119,18},   {720,480},      _4, {0x03,62,3,15,114,17,5,429},   1}, // 6,7 720 x 480i, pix repl
    /* 04 */{{ 7, 0,NSM, HI_UNF_ENC_FMT_NTSC, HI_UNF_ENC_FMT_NTSC/*7:HI_UNF_ENC_FMT_NTSC:480i@60,16:9*/},       2700,   {InterlaceVNegHNeg, 6000,   {1716,264}},    {119,18},   {720,480},      _16, {0x03,62,3,15,114,17,5,429},   1}, // 6,7 720 x 480i, pix repl
    /* 05 */{{ 8, 0,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,262}},    {119,18},   {1440,240},     _4, {0,124,3,15,114,38,4,0},        1}, // 8,9(1) 1440 x 240p
    /* 06 */{{ 8, 0,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,263}},    {119,18},   {1440,240},     _16, {0,124,3,15,114,38,4,0},        1}, // 8,9(2) 1440 x 240p
    /* 05 */{{ 9, 0,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,262}},    {119,18},   {1440,240},     _4, {0,124,3,15,114,38,4,0},        1}, // 8,9(1) 1440 x 240p
    /* 06 */{{ 9, 0,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       2700,   {ProgrVNegHNeg,     6000,   {1716,263}},    {119,18},   {1440,240},     _16, {0,124,3,15,114,38,4,0},        1}, // 8,9(2) 1440 x 240p
    /* 07 */{{10,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {InterlaceVNegHNeg, 6000,   {3432,525}},    {238,18},   {2880,480},     _4, {0,248,3,15,228,76,4,1716},     1}, // 10,11 2880 x 480p
    /* 07 */{{11,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {InterlaceVNegHNeg, 6000,   {3432,525}},    {238,18},   {2880,480},     _16, {0,248,3,15,228,76,4,1716},     1}, // 10,11 2880 x 480p
    /* 08 */{{12,13,  1, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {3432,262}},    {238,18},   {2880,240},     _4or16, {0,248,3,15,228,76, 4, 0},      1}, // 12,13(1) 2280 x 240p
    /* 09 */{{12,13,  2, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {3432,263}},    {238,18},   {2880,240},     _4or16, {0,248,3,15,228,76, 4, 0},      1}, // 12,13(2) 2280 x 240p
    /* 0a */{{14,15,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       5400,   {ProgrVNegHNeg,     6000,   {1716,525}},    {244,36},   {1440,480},     _4or16, {0,124,6,30,120,32, 9, 0},      0}, // 14, 15 1140 x 480p
    /* 0b */{{16, 0,NSM, HI_UNF_ENC_FMT_1080P_60, HI_UNF_ENC_FMT_BUTT},   14850,  {ProgrVPosHPos,     6000,   {2200,1125}},   {192,41},   {1920,1080},    _16,    {0,44,5, 36,148, 88, 4, 0},     0}, // 16 1920 x 1080p

// 50 Hz
    /* 0c */{{17,0,NSM, HI_UNF_ENC_FMT_576P_50, HI_UNF_ENC_FMT_576P_50/*18:HI_UNF_ENC_FMT_576P_50 16:9*/}, 2700,   {ProgrVNegHNeg,     5000,   {864, 625}},    {132,44},   {720, 576},     _4, {0,64,5, 39, 68, 12, 5, 0},     0}, // 17,18 720 x 576p
    /* 0c */{{18,0,NSM, HI_UNF_ENC_FMT_576P_50, HI_UNF_ENC_FMT_576P_50/*18:HI_UNF_ENC_FMT_576P_50 16:9*/}, 2700,   {ProgrVNegHNeg,     5000,   {864, 625}},    {132,44},   {720, 576},     _16, {0,64,5, 39, 68, 12, 5, 0},     0}, // 17,18 720 x 576p
    /* 0d */{{19, 0,NSM, HI_UNF_ENC_FMT_720P_50, HI_UNF_ENC_FMT_BUTT},    7425,   {ProgrVPosHPos,     5000,   {1980,750}},    {260,25},   {1280,720},     _16,    {0,40,5, 20,220,440, 5, 0},     0}, // 19 1280 x 720p
    /* 0e */{{20, 0,NSM, HI_UNF_ENC_FMT_1080i_50, HI_UNF_ENC_FMT_BUTT},   7425,   {InterlaceVPosHPos, 5000,   {2640, 1125}},  {192,20},   {1920,1080},    _16,    {0,44,5,15,148,528, 2,1320},    0}, // 20 1920 x 1080i
    /* 0f */{{21,0,NSM, HI_UNF_ENC_FMT_PAL, HI_UNF_ENC_FMT_PAL/*22:HI_UNF_ENC_FMT_PAL:576i@60,16:9*/},       2700,   {InterlaceVNegHNeg, 5000,   {1728, 625}},   {132,22},   {720, 576},     _4,     {3,63,3, 19,138, 24, 2, 432},  1}, // 21,22 1440 x 576i
    /* 0f */{{22,0,NSM, HI_UNF_ENC_FMT_PAL, HI_UNF_ENC_FMT_PAL/*22:HI_UNF_ENC_FMT_PAL:576i@60,16:9*/},       2700,   {InterlaceVNegHNeg, 5000,   {1728, 625}},   {132,22},   {720, 576},     _16,     {3,63,3, 19,138, 24, 2, 432},  1}, // 21,22 1440 x 576i
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
    /* 1c */{{35, 36,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {ProgrVNegHNeg,      5994,   {3432, 525}},   {488,36},  {2880,480},    _4or16,  {0, 248 ,6 ,30, 240, 64, 10, 0}, 0},    // 35, 36 2880 x 480p@59.94/60Hz
    {{37, 38,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {ProgrVNegHNeg,      5000,   {3456, 625}},   {272,39},  {2880,576},    _4or16,  {0, 256 ,5 ,40, 272, 48, 5, 0}, 0},     // 37, 38 2880 x 576p@50Hz
    {{39, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       7200,   {InterlaceVNegHNeg,  5000,   {2304, 1250}},  {352,62},  {1920,1080},   _16,     {0, 168 ,5 ,87, 184,32, 24, 0}, 0},     // 39 1920 x 1080i@50Hz
    {{40, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       14850,  {InterlaceVPosHPos, 10000,   {2640, 1125}},  {192,20},  {1920,1080},   _16,     {0, 44 ,5 ,15, 148, 528, 2, 1320}, 0},  // 40 1920 x 1080i@100Hz
    {{41, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       14850,  {InterlaceVPosHPos, 10000,   {1980, 750}},   {260,25},  {1280,720},    _16,     {0, 40 ,5 ,20, 220, 400, 5, 0}, 0},     // 41 1280 x 720p@100Hz
    {{42, 43,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      5400,   {ProgrVNegHNeg,     10000,   {864,  144}},   {132, 44}, {720, 576},    _4or16,  {0, 64 ,5 ,39, 68, 12, 5, 0}, 0},       // 42, 43, 720p x 576p@100Hz
    {{44, 45,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      5400,   {InterlaceVNegHNeg, 10000,   {864,  625}},   {132, 22}, {720, 576},    _4or16,  {0, 63 ,3 ,19, 69, 12, 2, 432},1},      // 44, 45, 720p x 576i@100Hz, pix repl
    {{46, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       14835,  {InterlaceVPosHPos, 11988,   {2200, 1125}},  {192, 20}, {1920, 1080},  _16,     {0, 44 ,5 ,15, 149, 88, 2, 1100},0},    // 46, 1920 x 1080i@119.88/120Hz
    {{47, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},       14835,  {ProgrVPosHPos,     11988,   {1650, 750}},   {260, 25}, {1280, 720},   _16,     {0, 40 ,5 ,20, 220, 110, 5, 1100}, 0},  // 47, 1280 x 720p@119.88/120Hz
    {{48, 49,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      5400,   {ProgrVNegHNeg,     11988,   {858, 525}},    {122, 36}, {720, 480},   _4or16,   {0, 62 ,6 ,30, 60, 16, 10, 0}, 0},      // 48, 49 720 x 480p@119.88/120Hz
    {{50, 51,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      5400,   {InterlaceVNegHNeg, 11988,   {858, 525}},    {119, 18}, {720, 480},   _4or16,   {0, 62 ,3 ,15, 57, 19, 4, 429}, 1},     // 50, 51 720 x 480i@119.88/120Hz
    {{52, 53,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {ProgrVNegHNeg,     20000,   {864, 625}},    {132, 44}, {720, 576},   _4or16,   {0, 64 ,5 ,39, 68, 12, 5, 0}, 0},       // 52, 53, 720 x 576p@200Hz
    {{54, 55,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {InterlaceVNegHNeg, 20000,   {864, 625}},    {132, 22}, {720, 576},   _4or16,   {0, 63 ,3 ,19, 69, 12, 2, 432}, 1},     // 54, 55, 1440 x 720i @200Hz, pix repl
    {{56, 57,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {ProgrVNegHNeg,     24000,   {858, 525}},    {122, 42}, {720, 480},   _4or16,   {0, 62 ,6 ,30, 60, 16, 9, 0}, 0},       // 56, 57, 720 x 480p @239.76/240Hz
    {{58, 59,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},      10800,  {InterlaceVNegHNeg, 24000,   {858, 525}},    {119, 18}, {720, 480},   _4or16,   {0, 62 ,3 ,15, 57, 19, 4, 429}, 1},     // 58, 59, 1440 x 480i @239.76/240Hz, pix repl
// NOTE: DO NOT ATTEMPT INPUT RESOLUTIONS THAT REQUIRE PIXEL CLOCK FREQUENCIES HIGHER THAN THOSE SUPPOTED BY THE TRANSMITTER CHIP
//                            1         2                 3       4    5         6 7          8   9      10    11, 13, 15
#if defined (DVI_SUPPORT)
    {{PC_BASE, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},    3150,   {ProgrVNegHPos,     8508,   {832, 445}},    {160,63},   {640,350},   _16,  {0,64,3,60,96,32,32, 0}, 0}, // 640x350@85.08
    {{PC_BASE+1, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  3150,   {ProgrVPosHNeg,     8508,   {832, 445}},    {160,44},   {640,400},   _16,  {0,64,3,41,96,32,1, 0},  0}, // 640x400@85.08
    {{PC_BASE+2, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  2700,   {ProgrVPosHNeg,     7008,   {900, 449}},    {0,0},      {720,400},   _16,  {0,0,0,0,0,0,0, 0},      0}, // 720x400@70.08
    {{PC_BASE+3, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  3500,   {ProgrVPosHNeg,     8504,   {936, 446}},    {20,45},    {720,400},   _16,  {0,72,3,42,108,36,1, 0}, 0}, // 720x400@85.04
    {{PC_BASE+4, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  2517,   {ProgrVNegHNeg,     5994,   {800, 525}},    {144,35},   {640,480},   _4,  {0,96,2,33,48,16,10,0},  0}, // 640x480@59.94
    {{PC_BASE+5, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  3150,   {ProgrVNegHNeg,     7281,   {832, 520}},    {144,31},   {640,480},   _4,  {0,40,3,28,128,128,9,0}, 0}, // 640x480@72.80
    {{PC_BASE+6, 0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  3150,   {ProgrVNegHNeg,     7500,   {840, 500}},    {21,19},    {640,480},   _4,  {0,64,3,28,128,24,9, 0}, 0}, // 640x480@75.00
    {{PC_BASE+7,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},   3600,   {ProgrVNegHNeg,     8500,   {832, 509}},    {168,28},   {640,480},   _4,  {0,56,3,25,128,24,9, 0}, 0}, // 640x480@85.00
    {{PC_BASE+8,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},   3600,   {ProgrVPosHPos,     5625,   {1024, 625}},   {200,24},   {800,600},   _4,  {0,72,2,22,128,24,1, 0}, 0}, // 800x600@56.25
    {{PC_BASE+9,0,NSM, HI_UNF_ENC_FMT_VESA_800X600_60, HI_UNF_ENC_FMT_BUTT},   4000,   {ProgrVPosHPos,     6032,   {1056, 628}},   {216,27},   {800,600},   _4,  {0,128,4,23,88,40,1, 0}, 0}, // 800x600@60.317
    {{PC_BASE+10,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  5000,   {ProgrVPosHPos,     7219,   {1040, 666}},   {184,29},   {800,600},   _4,  {0,120,6,23,64,56,37,0}, 0}, // 800x600@72.19
    {{PC_BASE+11,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  4950,   {ProgrVPosHPos,     7500,   {1056, 625}},   {240,24},   {800,600},   _4,  {0,80,3,21,160,16,1,0},  0}, // 800x600@75
    {{PC_BASE+12,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  5625,   {ProgrVPosHPos,     8506,   {1048, 631}},   {216,30},   {800,600},   _4,  {0,64,3,27,152,32,1,0},  0}, // 800x600@85.06
    {{PC_BASE+13,0,NSM, HI_UNF_ENC_FMT_VESA_1024X768_60, HI_UNF_ENC_FMT_BUTT},  6500,   {ProgrVNegHNeg,     6000,   {1344, 806}},   {296,35},   {1024,768},  _4,  {0,136,6,29,160,24,3,0}, 0}, // 1024x768@60
    {{PC_BASE+14,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  7500,   {ProgrVNegHNeg,     7007,   {1328, 806}},   {280,35},   {1024,768},  _4,  {0,136,6,19,144,24,3,0}, 0}, // 1024x768@70.07
    {{PC_BASE+15,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  7875,   {ProgrVPosHPos,     7503,   {1312, 800}},   {272,31},   {1024,768},  _4,  {0,96,3,28,176,16,1,0},  0}, // 1024x768@75.03
    {{PC_BASE+16,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  9450,   {ProgrVPosHPos,     8500,   {1376, 808}},   {304,39},   {1024,768},  _4,  {0,96,3,36,208,48,1,0},  0}, // 1024x768@85
    {{PC_BASE+17,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 10800,   {ProgrVPosHPos,     7500,   {1600, 900}},   {384,35},   {1152,864},  _4,  {0,128,3,32,256,64,1,0}, 0}, // 1152x864@75
    {{PC_BASE+18,0,NSM, HI_UNF_ENC_FMT_VESA_1600X1200_60, HI_UNF_ENC_FMT_BUTT}, 16200,   {ProgrVPosHPos,     6000,   {2160, 1250}},  {496,49},   {1600,1200}, _4,  {0,304,3,46,304,64,1,0}, 0}, // 1600x1200@60
    {{PC_BASE+19,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  6825,   {ProgrVNegHPos,     6000,   {1440, 790}},   {112,19},   {1280,768},  _16,  {0,32,7,12,80,48,3,0},   0}, // 1280x768@59.95
    {{PC_BASE+20,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT},  7950,   {ProgrVPosHNeg,     5987,   {1664, 798}},   {320,27},   {1280,768},  _16,  {0,128,7,20,192,64,3,0}, 0}, // 1280x768@59.87
    {{PC_BASE+21,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 10220,   {ProgrVPosHNeg,     6029,   {1696, 805}},   {320,27},   {1280,768},  _16,  {0,128,7,27,208,80,3,0}, 0}, // 1280x768@74.89
    {{PC_BASE+22,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 11750,   {ProgrVPosHNeg,     8484,   {1712, 809}},   {352,38},   {1280,768},  _16,  {0,136,7,31,216,80,3,0}, 0}, // 1280x768@85
    {{PC_BASE+23,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 10800,   {ProgrVPosHPos,     6000,   {1800, 1000}},  {424,39},   {1280,960},  _4,  {0,112,3,36,312,96,1,0}, 0}, // 1280x960@60
    {{PC_BASE+24,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 14850,   {ProgrVPosHPos,     8500,   {1728, 1011}},  {384,50},   {1280,960},  _4,  {0,160,3,47,224,64,1,0}, 0}, // 1280x960@85
    {{PC_BASE+25,0,NSM, HI_UNF_ENC_FMT_VESA_1280X1024_60, HI_UNF_ENC_FMT_BUTT}, 10800,   {ProgrVPosHPos,     6002,   {1688, 1066}},  {360,41},   {1280,1024}, _4,  {0,112,3,38,248,48,1,0}, 0}, // 1280x1024@60
    {{PC_BASE+26,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 13500,   {ProgrVPosHPos,     7502,   {1688, 1066}},  {392,41},   {1280,1024}, _4,  {0,144,3,38,248,16,1, 0},0}, // 1280x1024@75
    {{PC_BASE+27,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 15750,   {ProgrVPosHPos,     8502,   {1728, 1072}},  {384,47},   {1280,1024}, _4,  {0,160,3,4,224,64,1, 0}, 0}, // 1280x1024@85
    {{PC_BASE+28,0,NSM, HI_UNF_ENC_FMT_VESA_1366X768_60, HI_UNF_ENC_FMT_BUTT},  8550,   {ProgrVPosHPos,     6002,   {1792, 795}},   {368,24},   {1360,768},  _16,  {0,112,6,18,256,64,3,0}, 0}, // 1360x768@60
    {{PC_BASE+29,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 10100,   {ProgrVNegHPos,     5995,   {1560, 1080}},  {112,27},   {1400,1050}, _4,  {0,32,4,23,80,48,3,0},   0}, // 1400x105@59.95
    {{PC_BASE+30,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 12175,   {ProgrVPosHNeg,     5998,   {1864, 1089}},  {376,36},   {1400,1050}, _4,  {0,144,4,32,232,88,3,0}, 0}, // 1400x105@59.98
    {{PC_BASE+31,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 15600,   {ProgrVPosHNeg,     7487,   {1896, 1099}},  {392,46},   {1400,1050}, _4,  {0,144,4,22,248,104,3,0},0}, // 1400x105@74.87
    {{PC_BASE+32,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 17950,   {ProgrVPosHNeg,     8496,   {1912, 1105}},  {408,52},   {1400,1050}, _4,  {0,152,4,48,256,104,3,0},0}, // 1400x105@84.96
    {{PC_BASE+33,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 17550,   {ProgrVPosHPos,     6500,   {2160, 1250}},  {496,49},   {1600,1200}, _4,  {0,192,3,46,304,64,1,0}, 0}, // 1600x1200@65
	{{PC_BASE+34,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 18900,   {ProgrVPosHPos,     7000,   {2160, 1250}},  {496,49},   {1600,1200}, _4,  {0,192,3,46,304,64,1,0}, 0}, // 1600x1200@70
    {{PC_BASE+35,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 20250,   {ProgrVPosHPos,     7500,   {2160, 1250}},  {496,49},   {1600,1200}, _4,  {0,192,3,46,304,64,1,0}, 0}, // 1600x1200@75
    {{PC_BASE+36,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 22950,   {ProgrVPosHPos,     8500,   {2160, 1250}},  {496,49},   {1600,1200}, _4,  {0,192,3,46,304,64,1,0}, 0}, // 1600x1200@85
    {{PC_BASE+37,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 20475,   {ProgrVPosHNeg,     6000,   {2448, 1394}},  {528,49},   {1792,1344}, _4,  {0,200,3,46,328,128,1,0},0}, // 1792x1344@60
    {{PC_BASE+38,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 26100,   {ProgrVPosHNeg,     7500,   {2456, 1417}},  {568,72},   {1792,1344}, _4,  {0,216,3,69,352,96,1,0}, 0}, // 1792x1344@74.997
    {{PC_BASE+39,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 21825,   {ProgrVPosHNeg,     6000,   {2528, 1439}},  {576,46},   {1856,1392}, _4,  {0,224,3,43,352,96,1,0}, 0}, // 1856x1392@60
    {{PC_BASE+40,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 28800,   {ProgrVPosHNeg,     7500,   {2560, 1500}},  {576,107},  {1856,1392}, _4,  {0,224,3,104,352,128,1,0},0}, // 1856x1392@75
    {{PC_BASE+41,0,NSM, HI_UNF_ENC_FMT_VESA_1920X1200_60, HI_UNF_ENC_FMT_BUTT}, 15400,   {ProgrVNegHPos,     5995,   {2080, 1235}},  {112,32},   {1920,1200}, _16,  {0,32,6,26,80,48,3,0},    0}, // 1920x1200@59.95
    {{PC_BASE+42,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 19325,   {ProgrVPosHNeg,     5988,   {2592, 1245}},  {536,42},   {1920,1200}, _16,  {0,200,6,36,336,136,3,0}, 0}, // 1920x1200@59.88
    {{PC_BASE+43,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 24525,   {ProgrVPosHNeg,     7493,   {2608, 1255}},  {552,52},   {1920,1200}, _16,  {0,208,6,46,344,136,3,0}, 0}, // 1920x1200@74.93
    {{PC_BASE+44,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 28125,   {ProgrVPosHNeg,     8493,   {2624, 1262}},  {560,59},   {1920,1200}, _16,  {0,208,6,53,352,144,3,0}, 0}, // 1920x1200@84.93
    {{PC_BASE+45,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 23400,   {ProgrVPosHNeg,     6000,   {2600, 1500}},  {552,59},   {1920,1440}, _4,  {0,208,3,56,344,128,1,0}, 0}, // 1920x1440@60
    {{PC_BASE+46,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 29700,   {ProgrVPosHNeg,     7500,   {2640, 1500}},  {576,59},   {1920,1440}, _4,  {0,224,3,56,352,144,1,0}, 0}, // 1920x1440@75
    {{PC_BASE+47,0,NSM, HI_UNF_ENC_FMT_BUTT, HI_UNF_ENC_FMT_BUTT}, 29700,   {ProgrVPosHNeg,     7500,   {2640, 1500}},  {576,59},   {1920,1440}, _4,  {0,224,3,56,352,144,1,0}, 0}, // 1920x1440@75
#endif
};

const HDMIVICInfoType VICModeTables[VICMODES] = 
{
    {HI_UNF_ENC_FMT_BUTT }, // 0
    {HI_UNF_ENC_FMT_3840X2160_30 }, // 1 2160P@30
    {HI_UNF_ENC_FMT_3840X2160_25 }, // 2 2160P@25
    {HI_UNF_ENC_FMT_3840X2160_24 }, // 3 2160P@24
    {HI_UNF_ENC_FMT_4096X2160_24 }, // 4 4k*2K SMTP 4096*2160@24
};

