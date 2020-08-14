/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_lib_video.c
*
* @brief supporting library functions for TPG
*
*****************************************************************************/

/***** #include statements ***************************************************/

#include "si_lib_video_api.h"
#include "si_lib_mtx_api.h"
#include "si_lib_obj_api.h"

/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_video);

/***** local macro definitions ***********************************************/

#define SII_UINT_FRAC(num, den)         ( ((((num)<<1) / (den)) + 1) >> 1 )
#define SII_INT_FRAC(num, den)          ( (0>(((int)num)*((int)den))) ? ((((((int)num)*2)/((int)den))-1)/2) : ((((((int)num)*2)/((int)den))+1)/2) )

#define AR_SQP   SII_LIB_VIDEO_PICTAR__SQP
#define AR_4X3   SII_LIB_VIDEO_PICTAR__4X3
#define AR_16X9  SII_LIB_VIDEO_PICTAR__16X9
#define AR_NONE  SII_LIB_VIDEO_PICTAR__NONE

// Repetition Factor
#define	RP1		 0x01	// x1 (no repetition)
#define	RP2		 0x02	// x2 (doubled)
#define	RP4		 0x04	// x4
#define	RP_ANY	 0x80	// any from x1 to x10

#define	PROG	 0      // progressive scan
#define INTL	 1	    // interlaced scan

#define	POS		 0      // positive pulse
#define	NEG		 1      // negative pulse

#define NTSC	 1      // NTSC system (60Hz)
#define PAL		 2      // PAL system (50Hz)

/***** local prototypes ******************************************************/

static bool_t sIsCorrectVm( SiiLibVideoVM_t vM );
static uint32_t sHfreqGet( SiiLibVideoVM_t vM );
static uint32_t sVfreqGet( SiiLibVideoVM_t vM );

/***** local type definitions ************************************************/

typedef struct
{
	uint16_t	H; // Number of horizontal pixels
	uint16_t	V; // Number of vertical pixels
} sizesPix_t;

typedef  struct
{
	SiiLibVideoVM_t vM;
	const char      *LabelStr;
	uint32_t        pixelFrq;      // in Hz
	SiiLibVideoPictAR_t pictAr;
	uint8_t         Vic4x3;        // CEA VIC for 4:3 picture aspect rate, 0 if not available
	uint8_t         Vic16x9;       // CEA VIC for 16:9 picture aspect rate, 0 if not available
	uint8_t         HdmiVic;       // HDMI VIC for 16:9 picture aspect rate, 0 if not available
	sizesPix_t      Active;        // Number of active pixels per frame
	sizesPix_t      Total;         // Total number of pixels per frame
	sizesPix_t      SyncOffset;    // Offset of sync pulse per frame
	sizesPix_t      SyncWidth;     // Width of sync pulse per frame
	bit_fld_t       Interlaced :1; // true for interlaced video
	bit_fld_t       HPol       :1; // true on negative polarity for horizontal pulses
	bit_fld_t       VPol       :1; // true on negative polarity for vertical pulses
	bit_fld_t       NtscPal    :2; // 60/120/240Hz (1) or 50/100/200Hz (2) or unknown (0) TVs
	uint8_t         Repetition;    // Allowed video pixel repetition
	uint8_t         MaxAudioSR8Ch; // maximum allowed audio sample rate for 8 channel audio in kHz
} videoMode_t;

/***** local data objects ****************************************************/
static  float sRGBtoYC601[9] = 
{
	0.299000f,  0.587000f,  0.114000f,
	-0.168736f, -0.331264f,  0.500000f,
	0.500000f, -0.418688f, -0.081312f
};

static  float sRGBtoYC709[9] = 
{
	0.212600f,  0.715200f,  0.072200f,
		-0.114572f, -0.385428f,  0.500000f,
		0.500000f, -0.454153f, -0.045847f
};

static  float sRGBtoRGB[9] = 
{
	0.0000f,  1.0000f,  0.0000f,
		0.0000f,  0.0000f,  1.0000f,
		1.0000f,  0.0000f,  0.0000f
};

videoMode_t sVideoModeTable[] =
{
	//        video mode              label            Fpix      AR       VIC, HVIC {H/V active}, {H/V  total}, {HVSyncO}, {HVSyncW},I/P , HPol, VPol, syst, repetition  ,audio
	// CEA-861D video modes
	{ SII_LIB_VIDEO_VM__VGA60,        "VGA60",         25174826, AR_SQP,   1, 0, 0, {640 , 480 }, {800 , 525 }, {16 , 10}, {96 , 2}, PROG, NEG,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__480P,         "480P",          27000000, AR_4X3,   2, 3, 0, {720 , 480 }, {858 , 525 }, {16 ,  9}, {62 , 6}, PROG, NEG,  NEG,  NTSC, RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__720P60,       "720P60",        74175824, AR_16X9,  0, 4, 0, {1280, 720 }, {1650, 750 }, {110,  5}, {40 , 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080I60,      "1080I60",       74175824, AR_16X9,  0, 5, 0, {1920, 1080}, {2200, 1125}, {88 ,  5}, {44 ,10}, INTL, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__480I2,        "480I2",         27000000, AR_4X3,   6, 7, 0, {1440, 480 }, {1716, 525 }, {38 ,  9}, {124, 6}, INTL, NEG,  NEG,  NTSC,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__240P2A,       "240P2A",        27000000, AR_4X3,   8, 9, 0, {1440, 240 }, {1716, 262 }, {38 ,  4}, {124, 3}, PROG, NEG,  NEG,  NTSC,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__240P2B,       "240P2B",        27000000, AR_4X3,   8, 9, 0, {1440, 240 }, {1716, 263 }, {38 ,  5}, {124, 3}, PROG, NEG,  NEG,  NTSC,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__480I4,        "480I4",         54000000, AR_4X3,  10,11, 0, {2880, 480 }, {3432, 525 }, {76 ,  9}, {248, 6}, INTL, NEG,  NEG,  NTSC,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__240P4A,       "240P4A",        54000000, AR_4X3,  12,13, 0, {2880, 240 }, {3432, 262 }, {76 ,  4}, {248, 3}, PROG, NEG,  NEG,  NTSC,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__240P4B,       "240P4B",        54000000, AR_4X3,  12,13, 0, {2880, 240 }, {3432, 263 }, {76 ,  5}, {248, 3}, PROG, NEG,  NEG,  NTSC,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__480P2,        "480P2",         54000000, AR_4X3,  14,15, 0, {1440, 480 }, {1716, 525 }, {32 ,  9}, {124, 6}, PROG, NEG,  NEG,  NTSC, RP1|RP2     ,  96 },
	{ SII_LIB_VIDEO_VM__1080P60,      "1080P60",      148351648, AR_16X9,  0,16, 0, {1920, 1080}, {2200, 1125}, {88 ,  4}, {44 , 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__576P,         "576P",          27000000, AR_4X3,  17,18, 0, {720 , 576 }, {864 , 625 }, {12 ,  5}, {64 , 5}, PROG, NEG,  NEG,  PAL , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__720P50,       "720P50",        74250000, AR_16X9,  0,19, 0, {1280, 720 }, {1980, 750 }, {440,  5}, {40 , 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080I50,      "1080I50",       74250000, AR_16X9,  0,20, 0, {1920, 1080}, {2640, 1125}, {528,  5}, {44 ,10}, INTL, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__576I2,        "576I2",         27000000, AR_4X3,  21,22, 0, {1440, 576 }, {1728, 625 }, {24 ,  5}, {126, 6}, INTL, NEG,  NEG,  PAL ,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__288P2A,       "288P2A",        27000000, AR_4X3,  23,24, 0, {1440, 288 }, {1728, 312 }, {24 ,  2}, {126, 3}, PROG, NEG,  NEG,  PAL ,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__288P2B,       "288P2B",        27000000, AR_4X3,  23,24, 0, {1440, 288 }, {1728, 313 }, {24 ,  3}, {126, 3}, PROG, NEG,  NEG,  PAL ,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__288P2C,       "288P2C",        27000000, AR_4X3,  23,24, 0, {1440, 288 }, {1728, 314 }, {24 ,  4}, {126, 3}, PROG, NEG,  NEG,  PAL ,     RP2     ,  48 },
	{ SII_LIB_VIDEO_VM__576I4,        "576I4",         54000000, AR_4X3,  25,26, 0, {2880, 576 }, {3456, 625 }, {48 ,  5}, {252, 6}, INTL, NEG,  NEG,  PAL ,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__288P4A,       "288P4A",        54000000, AR_4X3,  27,28, 0, {2880, 288 }, {3456, 312 }, {48 ,  2}, {252, 3}, PROG, NEG,  NEG,  PAL ,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__288P4B,       "288P4B",        54000000, AR_4X3,  27,28, 0, {2880, 288 }, {3456, 313 }, {48 ,  3}, {252, 3}, PROG, NEG,  NEG,  PAL ,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__288P4C,       "288P4C",        54000000, AR_4X3,  27,28, 0, {2880, 288 }, {3456, 314 }, {48 ,  4}, {252, 3}, PROG, NEG,  NEG,  PAL ,       RP_ANY,  96 },
	{ SII_LIB_VIDEO_VM__576P2,        "576P2",         54000000, AR_4X3,  29,30, 0, {1440, 576 }, {1728, 625 }, {24 ,  5}, {128, 5}, PROG, NEG,  POS,  PAL , RP1|RP2     ,  96 }, // H-neg, V-pos
	{ SII_LIB_VIDEO_VM__1080P50,      "1080P50",      148500000, AR_16X9,  0,31, 0, {1920, 1080}, {2640, 1125}, {528,  4}, {44 , 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080P24,      "1080P24",       74175824, AR_16X9,  0,32, 0, {1920, 1080}, {2750, 1125}, {638,  4}, {44 , 5}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080P25,      "1080P25",       74250000, AR_16X9,  0,33, 0, {1920, 1080}, {2640, 1125}, {528,  4}, {44 , 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080P30,      "1080P30",       74175824, AR_16X9,  0,34, 0, {1920, 1080}, {2200, 1125}, {88 ,  4}, {44 , 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__480P4,        "480P4",        108000000, AR_4X3,  35,36, 0, {2880, 480 }, {3432, 525 }, {96 ,  9}, {248, 6}, PROG, NEG,  NEG,  NTSC, RP1|RP2|RP4 , 192 },
	{ SII_LIB_VIDEO_VM__576P4,        "576P4",        108000000, AR_4X3,  37,38, 0, {2880, 576 }, {3456, 625 }, {48 ,  5}, {256, 5}, PROG, NEG,  NEG,  PAL , RP1|RP2|RP4 , 192 },
	{ SII_LIB_VIDEO_VM__1080I50A,     "1080I50A",      72000000, AR_16X9,  0,39, 0, {1920, 1080}, {2304, 1250}, {32 , 46}, {168,10}, INTL, POS,  NEG,  PAL , RP1         , 192 }, // H-pos, V-neg, 1,2 blanks are same = 85
	{ SII_LIB_VIDEO_VM__1080I100,     "1080I100",     148500000, AR_16X9,  0,40, 0, {1920, 1080}, {2640, 1125}, {528,  5}, {44 ,10}, INTL, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__720P100,      "720P100",      148500000, AR_16X9,  0,41, 0, {1280, 720 }, {1980, 750 }, {440,  5}, {40 , 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__576P100,      "576P100",       54000000, AR_4X3,  42,43, 0, {720 , 576 }, {864 , 625 }, {12 ,  5}, {64 , 5}, PROG, NEG,  NEG,  PAL , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__576I100,      "576I100",       54000000, AR_4X3,  44,45, 0, {1440, 576 }, {1728, 625 }, {24 ,  5}, {126, 6}, INTL, NEG,  NEG,  PAL ,     RP2     ,  96 },
	{ SII_LIB_VIDEO_VM__1080I120,     "1080I120",     148351648, AR_16X9,  0,46, 0, {1920, 1080}, {2200, 1125}, {88 ,  5}, {44 ,10}, INTL, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__720P120,      "720P120",      148351648, AR_16X9,  0,47, 0, {1280, 720 }, {1650, 750 }, {110,  5}, {40 , 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__480P120,      "480P120",       54000000, AR_4X3,  48,49, 0, {720 , 480 }, {858 , 525 }, {16 ,  9}, {62 , 6}, PROG, NEG,  NEG,  NTSC, RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__480I120,      "480I120",       54000000, AR_4X3,  50,51, 0, {1440, 480 }, {1716, 525 }, {38 ,  9}, {124, 6}, INTL, NEG,  NEG,  NTSC,     RP2     ,  96 },
	{ SII_LIB_VIDEO_VM__576P200,      "576P200",      108000000, AR_4X3,  52,53, 0, {720 , 576 }, {864 , 625 }, {12 ,  5}, {64 , 5}, PROG, NEG,  NEG,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__576I200,      "576I200",      108000000, AR_4X3,  54,55, 0, {1440, 576 }, {1728, 625 }, {24 ,  5}, {126, 6}, INTL, NEG,  NEG,  PAL ,     RP2     , 192 },
	{ SII_LIB_VIDEO_VM__480P240,      "480P240",      108000000, AR_4X3,  56,57, 0, {720 , 480 }, {858 , 525 }, {16 ,  9}, {62 , 6}, PROG, NEG,  NEG,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__480I240,      "480I240",      108000000, AR_4X3,  58,59, 0, {1440, 480 }, {1716, 525 }, {38 ,  9}, {124, 6}, INTL, NEG,  NEG,  NTSC,     RP2     , 192 },
	// CEA-861E video modes
	{ SII_LIB_VIDEO_VM__720P24,       "720P24",        59400000, AR_16X9,  0,60, 0, {1280, 720 }, {3300, 750 }, {1760, 5}, { 40, 5}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__720P25,       "720P25",        74250000, AR_16X9,  0,61, 0, {1280, 720 }, {3960, 750 }, {2420, 5}, { 40, 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__720P30,       "720P30",        74250000, AR_16X9,  0,62, 0, {1280, 720 }, {3300, 750 }, {1760, 5}, { 40, 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080P120,     "1080P120",     297000000, AR_16X9,  0,63, 0, {1920, 1080}, {2200, 1125}, {  88, 4}, { 44, 5}, PROG, POS,  POS,  NTSC, RP1         , 192 },
	{ SII_LIB_VIDEO_VM__1080P100,     "1080P100",     297000000, AR_16X9,  0,64, 0, {1920, 1080}, {2640, 1125}, { 528, 4}, { 44, 5}, PROG, POS,  POS,  PAL , RP1         , 192 },
	// HDMI 1.4a video modes
	{ SII_LIB_VIDEO_VM__2160P30,      "2160P30",      296703298, AR_16X9,  0, 0, 1, {3840, 2160}, {4400, 2250}, {176 , 8}, {88 ,10}, PROG, POS,  POS,  NTSC, RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__2160P25,      "2160P25",      297000000, AR_16X9,  0, 0, 2, {3840, 2160}, {5280, 2250}, {1056, 8}, {88 ,10}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__2160P24,      "2160P24",      296703298, AR_16X9,  0, 0, 3, {3840, 2160}, {5500, 2250}, {1276, 8}, {88 ,10}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__2160P24S,     "2160P24S",     296703298, AR_16X9,  0, 0, 4, {4096, 2160}, {5500, 2250}, {1020, 8}, {88 ,10}, PROG, POS,  POS,  0   , RP1         , 192 }, // SMPTE (same pixel count as in 2160p24)
	// PC Resolutions
	{ SII_LIB_VIDEO_VM__VGA72,        "VGA72",         31500000, AR_SQP,   0, 0, 0, {640, 480  }, {832, 520},   {24 ,  9}, {40 , 3}, PROG, NEG,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__VGA75,        "VGA75",         31500000, AR_SQP,   0, 0, 0, {640, 480  }, {840, 500},   {16 ,  1}, {64 , 3}, PROG, NEG,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__VGA85_1,      "VGA85_1",       36000000, AR_SQP,   0, 0, 0, {640, 480  }, {832, 509},   {56 ,  1}, {56 , 3}, PROG, NEG,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__VGA85_2,      "VGA85_2",       35500000, AR_SQP,   0, 0, 0, {720, 400  }, {936, 446},   {36 ,  1}, {72 , 3}, PROG, NEG,  POS,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__SVGA56,       "SVGA56",        36000000, AR_SQP,   0, 0, 0, {800, 600  }, {1024, 625},  {24 ,  1}, {72 , 2}, PROG, POS,  POS,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__SVGA60,       "SVGA60",        39750330, AR_SQP,   0, 0, 0, {800, 600  }, {1056, 628},  {40 ,  1}, {128, 4}, PROG, POS,  POS,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__SVGA72,       "SVGA72",        50000000, AR_SQP,   0, 0, 0, {800, 600  }, {1040, 666},  {56 , 37}, {120, 6}, PROG, POS,  POS,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__SVGA75,       "SVGA75",        50000000, AR_SQP,   0, 0, 0, {800, 600  }, {1056, 625},  {16 ,  1}, {80 , 3}, PROG, POS,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__SVGA85,       "SVGA85",        56250000, AR_SQP,   0, 0, 0, {800, 600  }, {1048, 631},  {32 ,  1}, {64 , 3}, PROG, POS,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__XGAI87,       "XGAI87",        44900000, AR_SQP,   0, 0, 0, {1024, 768 }, {1264, 817},  {8  ,  1}, {176, 8}, INTL, POS,  POS,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__XGA60,        "XGA60",         64930910, AR_SQP,   0, 0, 0, {1024, 768 }, {1344, 806},  {24 ,  3}, {136, 6}, PROG, NEG,  NEG,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__XGA70,        "XGA70",         75000000, AR_SQP,   0, 0, 0, {1024, 768 }, {1328, 806},  {24 ,  3}, {136, 6}, PROG, NEG,  NEG,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__XGA75_1,      "XGA75_1",       78750000, AR_SQP,   0, 0, 0, {1024, 768 }, {1312, 800},  {16 ,  1}, {96 , 3}, PROG, POS,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__XGA85,        "XGA85",         94500000, AR_SQP,   0, 0, 0, {1024, 768 }, {1376, 808},  {48 ,  1}, {96 , 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__XGA75_2,      "XGA75_2",      108108000, AR_SQP,   0, 0, 0, {1152, 864 }, {1600, 900},  {64 ,  1}, {128, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WXGA60_1,     "WXGA60_1",      68250000, AR_SQP,   0, 0, 0, {1280, 768 }, {1440, 790},  {48 ,  3}, {32 , 7}, PROG, POS,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__WXGA60_2,     "WXGA60_2",      79500000, AR_SQP,   0, 0, 0, {1280, 768 }, {1664, 798},  {64 ,  3}, {128, 7}, PROG, NEG,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__WXGA75,       "WXGA75",       102300000, AR_SQP,   0, 0, 0, {1280, 768 }, {1696, 805},  {80 ,  3}, {128, 7}, PROG, NEG,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WXGA60_800,   "WXGA60_800",    83500000, AR_SQP,   0, 0, 0, {1280, 800 }, {1680, 831},  {72 ,  3}, {128, 6}, PROG, NEG,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__WXGA60_3,     "WXGA60_3",     108108000, AR_SQP,   0, 0, 0, {1280, 960 }, {1800, 1000}, {96 ,  1}, {112, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__SXGA60,       "SXGA60",       107856624, AR_SQP,   0, 0, 0, {1280, 1024}, {1688, 1066}, {48 ,  1}, {112, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WSXGA60_1,    "WSXGA60_1",     85500000, AR_SQP,   0, 0, 0, {1360, 768 }, {1792, 795},  {64 ,  3}, {112, 6}, PROG, POS,  POS,  0   , RP1         ,  96 },
	{ SII_LIB_VIDEO_VM__WSXGA60_2,    "WSXGA60_2",    101000000, AR_SQP,   0, 0, 0, {1400, 1050}, {1560, 1080}, {48 ,  3}, {32 , 4}, PROG, POS,  NEG,  0   , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__WXGA85_1,     "WXGA85_1",     117500000, AR_SQP,   0, 0, 0, {1280, 768 }, {1712, 809},  {80 ,  3}, {136, 7}, PROG, NEG,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WXGA85_2,     "WXGA85_2",     148500000, AR_SQP,   0, 0, 0, {1280, 960 }, {1728, 1011}, {64 ,  1}, {160, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__SXGA75,       "SXGA75",       135000000, AR_SQP,   0, 0, 0, {1280, 1024}, {1688, 1066}, {16 ,  1}, {144, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__SXGA85,       "SXGA85",       157500000, AR_SQP,   0, 0, 0, {1280, 1024}, {1728, 1072}, {64 ,  1}, {160, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__HD60,         "HD60",          85860000, AR_SQP,   0, 0, 0, {1366, 768 }, {1800, 795},  {72 ,  1}, {144, 3}, PROG, NEG,  POS,  0   , RP1         ,  96 }, // TODO: Verify Sync polarities
	{ SII_LIB_VIDEO_VM__WSXGA60,      "WSXGA60",      121750000, AR_SQP,   0, 0, 0, {1400, 1050}, {1864, 1089}, {88 ,  3}, {144, 4}, PROG, NEG,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WSXGA75,      "WSXGA75",      156000000, AR_SQP,   0, 0, 0, {1400, 1050}, {1896, 1099}, {104,  3}, {144, 4}, PROG, NEG,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WSXGA50,      "WSXGA50",       86750000, AR_SQP,   0, 0, 0, {1440, 900 }, {1872, 929},  {72 ,  3}, {144, 6}, PROG, NEG,  POS,  0   , RP1         ,  96 }, // TODO: Verify Sync polarities
	{ SII_LIB_VIDEO_VM__HD50_PLUS,    "HD50_PLUS",     96500000, AR_SQP,   0, 0, 0, {1600, 900 }, {2080, 929},  {80 ,  3}, {160, 5}, PROG, NEG,  POS,  0   , RP1         ,  96 }, // TODO: Verify Sync polarities
	{ SII_LIB_VIDEO_VM__UXGA60,       "UXGA60",       161838162, AR_SQP,   0, 0, 0, {1600, 1200}, {2160, 1250}, {64 ,  1}, {192, 3}, PROG, POS,  POS,  0   , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__WSXGA50_PLUS, "WSXGA50_PLUS", 119500000, AR_SQP,   0, 0, 0, {1680, 1050}, {2208, 1083}, {88 ,  3}, {176, 6}, PROG, NEG,  POS,  0   , RP1         , 192 }, // TODO: Verify Sync polarities
	{ SII_LIB_VIDEO_VM__WUXGA50,      "WUXGA50",      158250000, AR_SQP,   0, 0, 0, {1920, 1200}, {2560, 1238}, {120,  3}, {200, 6}, PROG, NEG,  POS,  0   , RP1         , 192 }, // TODO: Verify Sync polarities
	{ SII_LIB_VIDEO_VM__WUXGA60,      "WUXGA60",      154000000, AR_SQP,   0, 0, 0, {1920, 1200}, {2080, 1235}, {48 ,  3}, {32 , 6}, PROG, POS,  NEG,  0   , RP1         ,  48 },
	// Non-PC and non-HDMI resolutions
	{ SII_LIB_VIDEO_VM__480I,         "720X480i",      13500000, AR_4X3,   0, 0, 0, {720 , 480 }, {858 , 525 }, {16 ,  9}, {62 , 6}, INTL, NEG,  NEG,  NTSC, RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__576I,         "720X576i",      13500000, AR_4X3,   0, 0, 0, {720 , 576 }, {864 , 625 }, {12 ,  5}, {64 , 5}, INTL, NEG,  NEG,  PAL , RP1         ,  48 },
	{ SII_LIB_VIDEO_VM__540X60,       "960X540",       39600000, AR_4X3,   0, 0, 0, {960 , 540 }, {1100, 600 }, {16 , 20}, {32 ,10}, PROG, POS,  POS,  0   , RP1         ,  48 },
	// HDMI 2.0 video modes
	{ SII_LIB_VIDEO_VM__2160P50,      "2160P50",      594000000, AR_16X9,  0, 0, 2, {3840, 2160}, {5280, 2250}, {1056, 8}, {88 ,10}, PROG, POS,  POS,  PAL , RP1         , 192 },
	{ SII_LIB_VIDEO_VM__2160P60,      "2160P60",      593406594, AR_16X9,  0, 0, 3, {3840, 2160}, {4400, 2250}, {176,  8}, {88 ,10}, PROG, POS,  POS,  NTSC, RP1         , 192 },

	// End of Table mark
	{ SII_LIB_VIDEO_VM__NONE,         "EndOfTable",           0, AR_NONE,  0, 0, 0, {0   , 0   }, {0   , 0   }, {0  ,  0}, {0  , 0}, 0   , 0  ,  0  ,  0   , 0           ,   0 } // End of Table Mark
};

/***** public functions ******************************************************/

void SiiLibVideoFullRgbToLimitedRgb( SiiLibVideoRGB_t* pRgbO, const SiiLibVideoRGB_t* pRgbI )
{
	float val1 = (float)(219.0/255.0);
	float val2 = (float)(16.0/255.0);

	pRgbO->fR = pRgbI->fR*val1 + val2;
	pRgbO->fG = pRgbI->fG*val1 + val2;
	pRgbO->fB = pRgbI->fB*val1 + val2;
}

void SiiLibVideoRgbToYc709( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI )
{
	SiiLibMtx_t* pMtx = SiiLibMtxCreate(3, 3, "pMtx");
	SiiLibMtx_t* pVec = SiiLibMtxCreate(3, 1, "pVec");

	SiiLibMtxSetCoef(pVec, 0, 0, pRgbI->fR);
	SiiLibMtxSetCoef(pVec, 1, 0, pRgbI->fG);
	SiiLibMtxSetCoef(pVec, 2, 0, pRgbI->fB);

	SiiLibMtxLoad(pMtx, sRGBtoYC709);

	SiiLibMtxMultiply(pVec, pMtx, pVec);

	pYuvO->fY  = SiiLibMtxGetCoef(pVec, 0, 0);
	pYuvO->fCb = SiiLibMtxGetCoef(pVec, 1, 0);
	pYuvO->fCr = SiiLibMtxGetCoef(pVec, 2, 0);

	SiiLibMtxDelete(pVec);
	SiiLibMtxDelete(pMtx);
}

void SiiLibVideoQuantizeYc( SiiLibVideoYUV_t* pYuv, uint8_t bits )
{
	uint32_t msk = ((1<<bits)-1);

	/* Upper limit Clipping */
	if(  1.0 < pYuv->fY  ) pYuv->fY  =  1.0;
	if( +0.5 < pYuv->fCb ) pYuv->fCb = +0.5;
	if( +0.5 < pYuv->fCr ) pYuv->fCr = +0.5;

	/* Lower limit Clipping */
	if(  0.0 > pYuv->fY  ) pYuv->fY  =  0.0;
	if( -0.5 > pYuv->fCb ) pYuv->fCb = -0.5;
	if( -0.5 > pYuv->fCr ) pYuv->fCr = -0.5;

	/* Apply binary offset to Cb and Cr */
	pYuv->fCb += 0.5;
	pYuv->fCr += 0.5;

	/* Quantization */
	pYuv->fY  = (float)((uint32_t)(pYuv->fY  * msk + 0.5));
	pYuv->fCb = (float)((uint32_t)(pYuv->fCb * msk + 0.5));
	pYuv->fCr = (float)((uint32_t)(pYuv->fCr * msk + 0.5));
}

void SiiLibVideoRgbToYc601( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI )
{
	SiiLibMtx_t* pMtx = SiiLibMtxCreate(3, 3, "pMtx");
	SiiLibMtx_t* pVec = SiiLibMtxCreate(3, 1, "pVec");

	SiiLibMtxSetCoef(pVec, 0, 0, pRgbI->fR);
	SiiLibMtxSetCoef(pVec, 1, 0, pRgbI->fG);
	SiiLibMtxSetCoef(pVec, 2, 0, pRgbI->fB);

	SiiLibMtxLoad(pMtx, sRGBtoYC601);

	SiiLibMtxMultiply(pVec, pMtx, pVec);

	pYuvO->fY  = SiiLibMtxGetCoef(pVec, 0, 0);
	pYuvO->fCb = SiiLibMtxGetCoef(pVec, 1, 0);
	pYuvO->fCr = SiiLibMtxGetCoef(pVec, 2, 0);

	SiiLibMtxDelete(pVec);
	SiiLibMtxDelete(pMtx);
}

void SiiLibVideoRgbToRGB( SiiLibVideoYUV_t* pYuvO, const SiiLibVideoRGB_t* pRgbI )
{
	SiiLibMtx_t* pMtx = SiiLibMtxCreate(3, 3, "pMtx");
	SiiLibMtx_t* pVec = SiiLibMtxCreate(3, 1, "pVec");

	SiiLibMtxSetCoef(pVec, 0, 0, pRgbI->fR);
	SiiLibMtxSetCoef(pVec, 1, 0, pRgbI->fG);
	SiiLibMtxSetCoef(pVec, 2, 0, pRgbI->fB);

	SiiLibMtxLoad(pMtx, sRGBtoRGB);

	SiiLibMtxMultiply(pVec, pMtx, pVec);

	pYuvO->fY  = SiiLibMtxGetCoef(pVec, 0, 0);
	pYuvO->fCb = SiiLibMtxGetCoef(pVec, 1, 0);
	pYuvO->fCr = SiiLibMtxGetCoef(pVec, 2, 0);

	SiiLibMtxDelete(pVec);
	SiiLibMtxDelete(pMtx);
}

void SiiLibVideoQuantizeRgb( SiiLibVideoRGB_t* pRgb, uint8_t bits )
{
	uint32_t msk = ((1<<bits)-1);

	/* Upper limit Clipping */
	if( 1.0 < pRgb->fR ) pRgb->fR =  1.0;
	if( 1.0 < pRgb->fG ) pRgb->fG =  1.0;
	if( 1.0 < pRgb->fB ) pRgb->fB =  1.0;

	/* Lower limit Clipping */
	if( 0.0 > pRgb->fR ) pRgb->fR =  0.0;
	if( 0.0 > pRgb->fG ) pRgb->fG =  0.0;
	if( 0.0 > pRgb->fB ) pRgb->fB =  0.0;

	/* Quantization */
	pRgb->fR = (float)((uint32_t)(pRgb->fR * msk + 0.5));
	pRgb->fG = (float)((uint32_t)(pRgb->fG * msk + 0.5));
	pRgb->fB = (float)((uint32_t)(pRgb->fB * msk + 0.5));
}

//-------------------------------------------------------------------------------------------------
//! @brief      Return video format timing details.
//!
//! Note: For video formats allowing both 59.94 and 60 Hz timings
//!       SiiLibVideoTimingDetailGet() returns timing for 59.94 Hz formats.
//! Note: Some video formats allow more than one aspect ratio.
//!       For such formats the function returns a default one.
//!
//! @param[in]  vM       Video format ID
//! @param[in]  mode3D   3D format specifier
//! @param[in]  perField true- return timing per video field; false- per video frame
//! @param[out] pTiming  Pointer to a timing structure to fill the result in
//!
//-------------------------------------------------------------------------------------------------
void SiiLibVideoTimingDetailGet(SiiLibVideoVM_t vM, SiiLibVideoMod3D_t mode3D, bool_t perField, SiiLibVideoTiming_t* pTiming)
{
	/* Make sure that correct vM index is supplied */
	if( sIsCorrectVm(vM) )
	{
		// Get data from the table (to be modified according to 3D mode)
		uint32_t pixelFreq    = sVideoModeTable[vM].pixelFrq;
		uint32_t hFreq        = sHfreqGet(vM);
		uint32_t vFreq        = sVfreqGet(vM);
		bool_t   interlaced   = (sVideoModeTable[vM].Interlaced == INTL);
		bool_t   vPol         = sVideoModeTable[vM].VPol;
		bool_t   hPol         = sVideoModeTable[vM].HPol;
		uint16_t vTotal       = sVideoModeTable[vM].Total.V;
		uint16_t hTotal       = sVideoModeTable[vM].Total.H;
		uint16_t vSync        = sVideoModeTable[vM].SyncWidth.V;
		uint16_t vFrontPorch  = sVideoModeTable[vM].SyncOffset.V;
		uint16_t vActive      = sVideoModeTable[vM].Active.V;
		uint16_t hSync        = sVideoModeTable[vM].SyncWidth.H;
		uint16_t hFrontPorch  = sVideoModeTable[vM].SyncOffset.H;
		uint16_t hActive      = sVideoModeTable[vM].Active.H;
		uint16_t vBlank       = vTotal - vActive;

		// Correct data taken from the table according to 3D mode
		switch( mode3D )
		{
		case SII_LIB_VIDEO_MOD3D__FP: // Frame packing: progressive/interlaced
			vTotal    *= 2;
			pixelFreq *= 2;
			if(interlaced)
			{
				// Interlaced
				vActive = (vActive * 2) + vBlank + (vBlank + 1) / 2;

				// Convert vSync and vFrontPorch frame information into field information
				vSync       /= 2;
				vFrontPorch /= 2;

				// One frame contains even and odd images (all 4 fields)
				// and even it is interlaced from a scaler perspective,
				// it appears as progressive format for HDMI receivers/transmitters.
			}
			else
			{
				// Progressive
				vActive = (vActive * 2) + vBlank;
			}
			break;

		case SII_LIB_VIDEO_MOD3D__FA: // Field alternative: interlaced only
			// Frame: Left Odd  with V Sync, Right Odd  without V Sync,
			//        Left Even with V Sync, Right Even without V Sync.
			// The timing returning by the function corresponds to
			// the entire frame that includes the 4 fields.
			vActive     *= 2;
			vTotal      *= 2;
			pixelFreq   *= 2;
			break;

		case SII_LIB_VIDEO_MOD3D__LA: // Line alternative: progressive only
			vSync       *= 2;
			vFrontPorch *= 2;
			vActive     *= 2;
			vTotal      *= 2;
			pixelFreq   *= 2;
			break;

		case SII_LIB_VIDEO_MOD3D__SBSF: // Side-by-Side (Full): progressive/interlaced
			hSync       *= 2;
			hFrontPorch *= 2;
			hActive     *= 2;
			hTotal      *= 2;
			pixelFreq   *= 2;
			break;

		case SII_LIB_VIDEO_MOD3D__LD: // L + depth: progressive only
			vActive     = vActive * 2 + vBlank;
			vTotal      *= 2;
			pixelFreq   *= 2;
			break;

		case SII_LIB_VIDEO_MOD3D__LDG: // L + depth + graphics + graphics-depth: progressive only
			vActive     = vActive * 4 + vBlank * 3;
			vTotal      *= 4;
			pixelFreq   *= 4;
			break;

		default:
			break;
		}

		// Re-calculate vBlank as the number of lines may have changed
		vBlank = vTotal - vActive;

		pTiming->vTotal         = vTotal;
		pTiming->vFrontPorch    = vFrontPorch;
		pTiming->vSync          = vSync;
		pTiming->vBackPorch     = vBlank - vFrontPorch - vSync;
		pTiming->vActive        = vActive;
		pTiming->hTotal         = hTotal;
		pTiming->hFrontPorch    = hFrontPorch;
		pTiming->hSync          = hSync;
		pTiming->hBackPorch     = hTotal - hActive - hFrontPorch - hSync;
		pTiming->hActive        = hActive;
		pTiming->bInterlaced    = interlaced;
		pTiming->bVpol          = vPol;
		pTiming->bHpol          = hPol;
		pTiming->pixelFreq      = pixelFreq;
		pTiming->lineFreq       = hFreq;
		pTiming->vFreq          = vFreq;
		pTiming->pictAr         = sVideoModeTable[vM].pictAr; // default AR
		pTiming->mode3D			= mode3D;

		if(perField && pTiming->bInterlaced && (SII_LIB_VIDEO_MOD3D__FP != mode3D))
		{
			// Convert data from "per frame" to "per field"
			pTiming->vFreq       *= 2;
			pTiming->vTotal      /= 2;
			pTiming->vFrontPorch /= 2;
			pTiming->vSync       /= 2;
			pTiming->vActive     /= 2;
			pTiming->vBackPorch  = pTiming->vTotal - pTiming->vActive
				- pTiming->vFrontPorch - pTiming->vSync;
		}
	}
	else
	{
		//SII_PLATFORM_DEBUG_ASSERT(0);
		SII_MEMSET(&pTiming, 0, sizeof(SiiLibVideoTiming_t));
	}
}

/***** local functions *******************************************************/

static uint32_t sHfreqGet( SiiLibVideoVM_t vM )
{
	videoMode_t* pVmTbl = &sVideoModeTable[vM];

	return SII_UINT_FRAC(pVmTbl->pixelFrq, pVmTbl->Total.H);
}

static uint32_t sVfreqGet( SiiLibVideoVM_t vM )
{
	videoMode_t* pVmTbl = &sVideoModeTable[vM];

	return SII_UINT_FRAC(pVmTbl->pixelFrq, pVmTbl->Total.H * pVmTbl->Total.V);
}

//-------------------------------------------------------------------------------------------------
//! @brief      Verifies if video ID is valid or not.
//!
//! @param[in]  vM Video mode index
//!
//! @retval     true  Given video format is valid
//! @retval     false Given video format is invalid
//-------------------------------------------------------------------------------------------------
static bool_t sIsCorrectVm( SiiLibVideoVM_t vM )
{
	if( SII_LIB_VIDEO_VM__NONE <= vM )
		return false;

	if( sVideoModeTable[vM].vM != vM )
	{
		//DEBUG_PRINT(MSG_ALWAYS, "si_lib_video_stdfrm: VideoModeTable[%d].%d = %d\n", vM, VideoModeTable[vM].vM);
		return false;
	}

	return true;
}

/***** end of file ***********************************************************/
