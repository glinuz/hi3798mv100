#include "hi_reg_common.h"
#include "drv_disp_hal.h"
#include "drv_disp_com.h"
#include "drv_disp_hal.h"
#include "drv_disp_osal.h"
#include "drv_disp_da.h"
#ifndef __DISP_PLATFORM_BOOT__
#include "drv_win_hal.h"
#include "hi_drv_sys.h"
#include "hi_drv_disp.h"
#include "drv_pq_ext.h"
#endif

static DISP_FMT_CFG_S s_stDispFormatParam[] =
{
/* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
/* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 0 HI_UNF_ENC_FMT_1080P_60,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6AE6A, 0x0001062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080}, {0,0,1920,1080}, {16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 1 HI_UNF_ENC_FMT_1080P_60,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6AE6A, 0x0001062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 2 HI_UNF_ENC_FMT_1080P_50,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,     44, 5, 1, 0,  0,  0}, /* 1080P@50Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 3 HI_UNF_ENC_FMT_1080P_30
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,       1,   1,  1,    44,  5, 1, 0,  0,  0}, /* 1080P@30Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_30, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 3000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 4 HI_UNF_ENC_FMT_1080P_25,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 528,      1,   1,  1,    44, 5, 1,  0,  0,  0}, /* 1080P@25Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_25, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2500, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 5 HI_UNF_ENC_FMT_1080P_24 @74.25MHz,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 638,       1,   1,  1,    44, 5, 1, 0,  0,  0}, /* 1080P@24Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_24, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2400, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 6 HI_UNF_ENC_FMT_1080i_60
 { {1,   0,   2,   540,  20,  2,  1920, 192, 88,  540, 21,  2,    44,  5, 908,   0,  0,  0}, /* 1080I@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6AE6A, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080i_60, DISP_STEREO_NONE, HI_TRUE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 7 HI_UNF_ENC_FMT_1080i_50
 { {1,   0,   2,   540,  20,  2,  1920, 192,528,  540,  21,  2,   44, 5, 1128,  0,  0,  0}, /* 1080I@50Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080i_50, DISP_STEREO_NONE, HI_TRUE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
 
  // 8 HI_UNF_ENC_FMT_720P_60
 { {1,   1,   2,   720,  25,  5,  1280, 260,110,      1,   1,  1,    40,  5,  1, 0,  0,  0}, /* 720P@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6AE6A, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,1280,720}, {0,0,1280,720},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 9 HI_UNF_ENC_FMT_720P_50
 { {1,   1,   2,   720,  25,  5,  1280, 260,440,     1,   1,  1,     40, 5,  1,  0,  0,  0},  /* 720P@50Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,1280,720}, {0,0,1280,720},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

/* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 10 HI_UNF_ENC_FMT_576P_50,
 { {1,  1,   2,   576,   44,  5,   720, 132, 12,     1,   1,  1,     64, 5,  1,  0,  1,  1}, /* 576P@50Hz */
   DISP_CLOCK_SOURCE_SD0,
   {0x14000000, 0x02002063},
   {HI_DRV_DISP_FMT_576P_50, DISP_STEREO_NONE, HI_FALSE, {0,0,720,576} , {0,0,720,576} ,{4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
    /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
  /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
  // 11 HI_UNF_ENC_FMT_480P_60,
#if 1
 { {1,  1,   2,   480,   36,  9,   720, 122, 16,     1,   1,  1,     62, 6,  1,  0,  1,  1}, /* 480P@60Hz */
   DISP_CLOCK_SOURCE_SD0,
   {0x14000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_480P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,720,480} , {0,0,720,480} , {4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
#else
 { {1,  1,   2,   480,   36,  9,   720-16, 122+8, 16+8,     1,   1,  1,     62, 6,  1,  0,  0,  0}, /* 480P@60Hz */
   DISP_CLOCK_SOURCE_SD0,
   {0x14000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_480P_60, DISP_STEREO_NONE, HI_FALSE, {0,0,720-16,480} , {0,0,720-16,480} , {4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
#endif

 // 12 HI_UNF_ENC_FMT_PAL
 { {0,   0,   0,   288,  22,  2,  720, 132, 12,     288,  23,  2,  63 /* 126*/, 3, 300, 0,  1,  1},/* 576I(PAL) */
   DISP_CLOCK_SOURCE_SD0,
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_PAL,     DISP_STEREO_NONE, HI_TRUE,  {0,0,720,576} ,  {0,0,720,576} , {4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },
#if 1
  //576I: HDMI输出要求hmid=300, 而YPbPr要求hmid=0,
  //考虑一般用户不会使用HDMI输出576I，所以不支持HDMI_567I输出，选择hmid=0
  // 13 HI_UNF_ENC_FMT_NTSC
 { {0,   0,   0,   240,  18,  4,   720, 119, 19,     240,  19,  4, 62 /*124*/, 3,  310, 0, 1,  1},/* 480I(NTSC) */
   DISP_CLOCK_SOURCE_SD0,
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED},
 },
#else
 { {0,   0,   0,   240,  18,  4,   720-16, 119+8, 19+8,     240,  19,  4,    124, 3,  0, 0, 0,  0},/* 480I(NTSC) */
   DISP_CLOCK_SOURCE_SD0,
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720-16,480} , {0,0,720-16,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED},
 },
#endif
  //480I: HDMI输出要求hmid=310, 而YPbPr要求hmid=0,
  //考虑一般用户不会使用HDMI输出480I，所以不支持HDMI_480I输出，选择hmid=0

    /* ============================================= */
    // TODO:
    // 14, LCD

    {   {1,   1,   2,   480,  35,  10,  640, 144, 16,       1,   1,  1,      96, 2,  1, 0,  1,  1}, /* 640*480@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x24000000, 0x20050a8}, // 1080i50
        {HI_DRV_DISP_FMT_861D_640X480_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 640, 480} ,  {0, 0, 640, 480}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 15
    {   {1,   1,   2,   600,  27,  1,   800, 216, 40,       1,   1,  1,    128, 4, 1, 0,  0,  0}, /* 800*600@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x25000000, 0x201967a}, /* 800*600@60Hz */
        {HI_DRV_DISP_FMT_VESA_800X600_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 800, 600} ,  {0, 0, 800, 600}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 16
    {   {1,   1,   2,   768,  35,  3,  1024, 296, 24,      1,   1,  1,    136, 6, 1,  0,  0,  0}, /* 1024x768@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x15000000, 0x201954a}, /* 1024x768@60Hz */
        {HI_DRV_DISP_FMT_VESA_1024X768_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1024, 768} ,  {0, 0, 1024, 768}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 17
    {   {1,   1,   2,   720,  25,  5,  1280, 260, 110,  1,     1,   1,  40, 5,  1, 0,  0,  0}, /* 1280x720@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x13000000, 0x2008129}, /* 1280x720@60Hz */
        {HI_DRV_DISP_FMT_VESA_1280X720_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1280, 720} ,  {0, 0, 1280, 720}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 18
    {   {1,   1,   2,   800,  28,  3,  1280, 328, 72,  1,     1,   1,      128, 6, 1, 0,  0,  0}, /* 1280x800@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x13000000, 0x2019417}, /* 1280x800@60Hz */
        {HI_DRV_DISP_FMT_VESA_1280X800_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1280, 800} ,  {0, 0, 1280, 800}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 19
    {   {1,   1,   2,  1024,  41,  1,  1280, 360, 48,  1,     1,   1,     112, 3, 1,  0,  0,  0}, /* 1280x1024@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x15000000, 0x20198c9}, /* 1280x1024@60Hz */
        {HI_DRV_DISP_FMT_VESA_1280X1024_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1280, 1024},  {0, 0, 1280, 1024} , {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 20
    {   {1,   1,   2,   768,  24,  3,  1360, 368, 64,  1,     1,   1,    112, 6,  1,  0,  0,  0}, /* 1360x768@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x13000000, 0x2032859}, /* 1360x768@60Hz */
        {HI_DRV_DISP_FMT_VESA_1360X768_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1360, 768},  {0, 0, 1360, 768} , {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 21
    {   {1,   1,   2,   768,  27,  3,  1366, 356, 70,  1,     1,   1,      143, 3, 1, 0,  0,  0}, /* 1366x768@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x13000000, 0x200a1ad}, /* 1366X768@60Hz */
        {HI_DRV_DISP_FMT_VESA_1366X768_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1366, 768},  {0, 0, 1366, 768} , {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 22
    {   {1,   1,   2,  1050,  36,  3,  1400, 376, 88,  1,     1,   1,    144, 4,  1,  0,  0,  0}, /* 1400x1050@60Hz */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x20050cb}, /* 1400x1050@60Hz */
        {HI_DRV_DISP_FMT_VESA_1400X1050_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1400, 1050},  {0, 0, 1400, 1050} , {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

    // 23
    {   {1,   1,   2,   900,  31,  3,  1440, 384, 80,    1,   1,  1,    152, 6,   1,  0,  0,  0}, /* 1440x900@60Hz_RB@106.5MHz */
        DISP_CLOCK_SOURCE_HD0,
         {0x12000000, 0x201e42b},/* 1440x900@60Hz@106.5MHz */
        {HI_DRV_DISP_FMT_VESA_1440X900_60_RB,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1440, 900} ,  {0, 0, 1440, 900}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
        // 24
    {   {1,   1,   2,   900,  23,  3,  1440, 112, 48,     1,   1,  1,    32, 6,   1, 0,  0,  0}, /* 1440x900@60Hz@88.75MHz */
        DISP_CLOCK_SOURCE_HD0,
        {0x14000000, 0x2006163}, /* 1440x900@60Hz_RB@88.75MHz */
        {HI_DRV_DISP_FMT_VESA_1440X900_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1440, 900} ,  {0, 0, 1440, 900}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
        // 25
    {   {1,   1,   2,   900,  23,  3,  1600, 112, 48,    1,   1,  1,    32, 5,   1,  0,  0,  0}, /* 1600x900@60Hz_RB@97.750 MHz  */
            DISP_CLOCK_SOURCE_HD0,
            {0x14000000, 0x2006187}, /* 1600x900@60Hz_RB@97.750 MHz  */
            {HI_DRV_DISP_FMT_VESA_1600X900_60_RB,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1600, 900} ,  {0, 0, 1600, 900}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

    // 26
    {   {1,   1,   2,   1200, 49,  1, 1600, 496, 64,       1,   1,  1,     192, 3, 1, 0,  0,  0}, /* 1600*12000@60Hz@162.000 MHz  */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x2001036}, /* 1600*12000@60Hz@162.000 MHz  */
        {HI_DRV_DISP_FMT_VESA_1600X1200_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1600, 1200} ,  {0, 0, 1600, 1200}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 27
    {   {1,   1,   2,  1050,  36,  3,  1680, 456, 104,  1,     1,   1,   176, 6,   1,  0,  0,  0}, /* 1680x1050@60Hz@119.000 MHz  */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x2018493}, /* 1680x1050@60Hz@119.000 MHz  */
        {HI_DRV_DISP_FMT_VESA_1680X1050_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1680, 1050} ,  {0, 0, 1680, 1050}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
        // 28
    {   {1,   1,   2,  1050,  36,  3,  1680, 456, 104,  1,     1,   1,   176, 6,   1,  0,  0,  0}, /* 1680x1050@60Hz@119.000 MHz  */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x2018493}, /* 1680x1050@60Hz@119.000 MHz  */
        {HI_DRV_DISP_FMT_VESA_1680X1050_60_RB,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1680, 1050} ,  {0, 0, 1680, 1050}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

      /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
    /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
    // 11 HI_UNF_ENC_FMT_480P_60,
    // 29
    {   {1,   1,   2,  1080,  41,  4,  1920, 192, 88,  1,     1,   1,       44, 5, 1, 0,  0,  0}, /* 1920x1080@60Hz@148.500 MHz  */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x2002063}, /* 1920x1080@60Hz@148.500 MHz  */
        {HI_DRV_DISP_FMT_VESA_1920X1080_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1920, 1080} ,  {0, 0, 1920, 1080}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
    // 30
    {   {1,   1,   2,  1200,  32,  3,  1920, 112, 48,  1,     1,   1,      32, 6, 1,  0,  0,  0}, /* 1920x1200@60Hz@154.000 MHz */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x200309a}, /* 1920x1200@60Hz@154.000 MHz */
        {HI_DRV_DISP_FMT_VESA_1920X1200_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1920, 1200} ,  {0, 0, 1920, 1200}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },
        /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
    // 31
      {   {1,     1,         2,  1440,  59,      1,  1920,       552,      128,        1,             1,         1,        208,           3,      1,  0,     0,  0}, /*  1920x1440@60Hz 234MHz */
          DISP_CLOCK_SOURCE_HD0,
          {0x11000000, 0x2001027}, /*  1920x1440@60Hz 234MHz */
          {HI_DRV_DISP_FMT_VESA_1920X1440_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 1920, 1440} ,  {0, 0, 1920, 1440}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
      },

    // 32
    {   {1,   1,   2,  1152,  30,  3,  2048, 112, 48,  1,     1,   1,     32, 5,  1,  0,  0,  0}, /* 2048X1152@60Hz@156.750 MHz  */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x200309d}, /* 2048X1152@60Hz@156.750 MHz  */
        {HI_DRV_DISP_FMT_VESA_2048X1152_60,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 2048, 1152} ,  {0, 0, 2048, 1152}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

    // 33       /*not support*/
    {
        {1,     1,         2,  1440,  39,      2,  2560,       112,        48,          1,             1,         1,           32,           5,    1, 0,     0,  0}, /*  2560x1440@60Hz@241.5MHz RB */
        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x20020a1}, /*  2560x1440@60Hz@241.5MHz RB */
        {HI_DRV_DISP_FMT_VESA_2560X1440_60_RB,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 2560, 1440} ,  {0, 0, 2560, 1440}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

    // 34       /*not support*/
    {    {1,     1,         2,  1600,  43,      3,  2560,       112,       48,          1,             1,         1,           32,            6,   1,  0,     0,  0}, /*  2560x1600@60Hz@268.5MHz RB */

        DISP_CLOCK_SOURCE_HD0,
        {0x12000000, 0x20020b3}, /*  2560x1600@60Hz@268.5MHz RB */
        {HI_DRV_DISP_FMT_VESA_2560X1600_60_RB,    DISP_STEREO_NONE, HI_FALSE,  {0, 0, 2560, 1600} ,  {0, 0, 2560, 1600}, {16, 9},  6000, HI_DRV_CS_BT709_RGB_FULL}
    },

      //35 HI_UNF_ENC_FMT_PAL_TEST
     { {0,   0,   2,   288,  22,  2,  1440, 132, 12,    288,  23,  2,    126, 3,  0, 0,  0,  0},/* 576I(PAL) */
       DISP_CLOCK_SOURCE_HD0,
       {0x24000000, 0x02002063}, // 1080i50
       {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED}
     },

    /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
      // 36 HI_UNF_ENC_FMT_1080P_24_FP @74.25MHz,
     { {1,   1,   2,  1080,  41,  4,  1920, 192, 638,   1080,  41,  4,   44, 5,   1, 0,  0,  0}, /* 1080P@24Hz */
       DISP_CLOCK_SOURCE_HD0,
       {0x12000000, 0x02002063}, // 1080P60/50
       {HI_DRV_DISP_FMT_1080P_24_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2400, HI_DRV_CS_BT709_YUV_LIMITED}
     },

  // 37 HI_UNF_ENC_FMT_720P_60_FP
 { {1,   1,   2,   720,  25,  5,  1280, 260,110,    720,  25,  5,    40,  5,  1,  0,  0,  0}, /* 720P@60Hz */
   DISP_CLOCK_SOURCE_HD0,
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_60_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1280,720},  {0,0,1280,720},{16,9}, 6000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
  // 38 HI_UNF_ENC_FMT_720P_50_FP
 /* Synm Iop  Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs */
 { {1,   1,   2,   720,  25,  5,  1280, 260,440,    720,  25,   5,   40,  5,  1, 0,  0,  0},  /* 720P@50Hz */
   DISP_CLOCK_SOURCE_HD0,
   {0x12000000, 0x02002063}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_50_FP, DISP_STEREO_FPK, HI_FALSE, {0,0,1280,720},  {0,0,1280,720},{16,9}, 5000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 39  HI_UNF_ENC_FMT_PAL
 { {0,   0,   0,   288,  22,  2,  720, 132, 12,     288,  23,  2,  63  /*126*/, 3, 300, 0,  1,  1},/* 576I(PAL) */
   DISP_CLOCK_SOURCE_SD0,
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_PAL,     DISP_STEREO_NONE, HI_TRUE,  {0,0,720,576} ,  {0,0,720,576} , {4,3},  5000, HI_DRV_CS_BT601_YUV_LIMITED}
 },

  //576I: HDMI输出要求hmid=300, 而YPbPr要求hmid=0,
  //考虑一般用户不会使用HDMI输出576I，所以不支持HDMI_567I输出，选择hmid=0
  // 40 HI_UNF_ENC_FMT_NTSC
 { {0,   0,   0,   240,  18,  4,   720, 119, 19,     240,  19,  4, 62 /*124*/, 3,  310, 0, 1,  1},/* 480I(NTSC) */
   DISP_CLOCK_SOURCE_SD0,
   {0x24000000, 0x02002063}, // 1080i50
   {HI_DRV_DISP_FMT_NTSC,    DISP_STEREO_NONE, HI_TRUE,  {0,0,720,480} , {0,0,720,480} ,{4,3},  6000, HI_DRV_CS_BT601_YUV_LIMITED},
 },

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
 //41  4k*2k 24hz,
 { {1,   1,   2,  2160,  82,  8,  3840, 384, 1276,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k 24hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11000000, 0x02002063}, //4k*2k
   {HI_DRV_DISP_FMT_3840X2160_24, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 2400, HI_DRV_CS_BT709_YUV_LIMITED}
 },

 //42 4k*2k 25hz,
 { {1,   1,   2,  2160,  82,  8,  3840, 384, 1056,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k 25hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11000000, 0x02002063}, //4k*2k
   {HI_DRV_DISP_FMT_3840X2160_25, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 2500, HI_DRV_CS_BT709_YUV_LIMITED}
 },

 //43 4k*2k 30hz,
 { {1,   1,   2,  2160,  82,  8,  3840, 384, 176,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k 30hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11000000, 0x02002063}, //4k*2k
   {HI_DRV_DISP_FMT_3840X2160_30, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 3000, HI_DRV_CS_BT709_YUV_LIMITED}
 },

 //44 4k*2k SMPTE 30hz,
 { {1,   1,   2,  2160,  82,  8,  4096, 384, 1020,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k smpte 30hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11000000, 0x02002063}, //4k*2k
   {HI_DRV_DISP_FMT_4096X2160_24, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 3000, HI_DRV_CS_BT709_YUV_LIMITED}
 },
 #endif

  /*define decimal  fmt*/
 #if 1
  /*45, 3840X2160 23.976*/
 { {1,   1,   2,  2160,  82,  8,  3840, 384, 1276,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k 24hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11E6A7EF, 0x2062}, //4k*2k
   {HI_DRV_DISP_FMT_3840X2160_23_976, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 2397, HI_DRV_CS_BT709_YUV_LIMITED}
 },

 //46 4k*2k 29.97hz,
 { {1,   1,   2,  2160,  82,  8,  3840, 384, 176,       1,   1,  1,    88, 10, 1, 0,  0,  0}, /* 4k*2k 30hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x11E6A7EF, 0x2062}, //4k*2k
   {HI_DRV_DISP_FMT_3840X2160_29_97, DISP_STEREO_NONE, HI_FALSE, {0,0,3840,2160},{0,0,3840,2160},{16,9}, 2997, HI_DRV_CS_BT709_YUV_LIMITED}
 },

 // 47 720P  59.94
 { {1,   1,   2,   720,  25,  5,  1280, 260,110,      1,   1,  1,    40,  5,  1, 0,  0,  0}, /* 720P@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6A7EF, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_720P_59_94, DISP_STEREO_NONE, HI_FALSE, {0,0,1280,720}, {0,0,1280,720},{16,9}, 5994, HI_DRV_CS_BT709_YUV_LIMITED}
 },

   //48 1080p 59.94,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,      1,   1,  1,     44, 5, 1,  0,  0,  0}, /* 1080P@60Hz */
   //{0x14000000, 0x02002063}, // 1080P60/50
   DISP_CLOCK_SOURCE_HD0,
   {0x12E6A7EF, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_59_94, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5994, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  //49 1080P 29.97
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 88,       1,   1,  1,    44,  5, 1, 0,  0,  0}, /* 1080P@30Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6A7EF, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_29_97, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2997, HI_DRV_CS_BT709_YUV_LIMITED}
 },

  // 50 1080P 23.976,
 { {1,   1,   2,  1080,  41,  4,  1920, 192, 638,       1,   1,  1,    44, 5, 1, 0,  0,  0}, /* 1080P@24Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6A7EF, 0x2062}, // 1080P60/50
   {HI_DRV_DISP_FMT_1080P_23_976, DISP_STEREO_NONE, HI_FALSE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 2397, HI_DRV_CS_BT709_YUV_LIMITED}
 },

   //51  1080i 59.94
 { {1,   0,   2,   540,  20,  2,  1920, 192, 88,  540, 21,  2,    44,  5, 908,   0,  0,  0}, /* 1080I@60Hz */
//   {0x24000000, 0x02002063}, // 1080i50
   DISP_CLOCK_SOURCE_HD0,
   {0x14E6A7EF, 0x2062},
   {HI_DRV_DISP_FMT_1080i_59_94, DISP_STEREO_NONE, HI_TRUE, {0,0,1920,1080},{0,0,1920,1080},{16,9}, 5994, HI_DRV_CS_BT709_YUV_LIMITED}
 },
#endif
};

HI_S32 Get_FormatCfgPara(HI_U32 index,DISP_FMT_CFG_S *pstDispFormatParam)
{
    if (index >= sizeof(s_stDispFormatParam)/sizeof(DISP_FMT_CFG_S))
        return HI_FAILURE;

    *pstDispFormatParam = s_stDispFormatParam[index];
    return HI_SUCCESS;
}
#ifndef __DISP_PLATFORM_BOOT__
static HI_VOID internal_set_extlineRegister(HI_U32 u32LayerId, HI_S32 s32exl)
{
    HI_U32 extRegValue = 0;

    switch (s32exl)
    {
        case 1:
            extRegValue = 0;
            break;
        case 2:
            extRegValue = 1;
            break;
        case 4:
            extRegValue = 2;
            break;
        case 8:
        default:
            extRegValue = 3;
            break;
    }
    VDP_VID_SetDrawMode(u32LayerId, extRegValue, extRegValue);

    return;
}

HI_S32 Chip_Specific_LayerZmeFunc(HI_PQ_ZME_LAYER_E u32LayerId,
                            HI_PQ_ZME_PARA_IN_S *pstZmeI,
                            HI_U32     bFirlterEnable)
{
	 DRV_PQ_SetZme(u32LayerId, pstZmeI, bFirlterEnable);
    return HI_SUCCESS;
}

HI_S32 Chip_Specific_WbcZmeFunc(HI_PQ_ZME_LAYER_E u32LayerId,
                            HI_PQ_ZME_PARA_IN_S *stZmeI,
                            HI_U32     bFirlterEnable)
{	
    DRV_PQ_SetZme(u32LayerId, stZmeI, bFirlterEnable);
    return HI_SUCCESS;
}
#endif

HI_S32 Chip_Specific_Set3DMode(HI_DRV_DISPLAY_E eChn, HI_U32 u32DispId, DISP_FMT_CFG_S *pstCfg)
{
    //pstCfg->stInfo.eDispMode = DISP_STEREO_SBS_HALF;
    switch(pstCfg->stInfo.eDispMode)
    {
        case DISP_STEREO_NONE:
            VDP_DISP_SetDispMode(u32DispId, 0);
            break;
        case DISP_STEREO_FPK:
            VDP_DISP_SetDispMode(u32DispId, 5);
            break;
        case DISP_STEREO_SBS_HALF:
            VDP_DISP_SetDispMode(u32DispId, 1);
            break;
        case DISP_STEREO_TAB:
            VDP_DISP_SetDispMode(u32DispId, 4);
            break;
        case DISP_STEREO_LINE_ALTE:
            VDP_DISP_SetDispMode(u32DispId, 2);
            break;
        default:
            VDP_DISP_SetDispMode(u32DispId, 0);
            break;
    }

    return HI_SUCCESS;
}

HI_S32 Chip_Specific_SetDispMode(HI_U32 u32id, HI_DRV_DISP_STEREO_MODE_E eMode)
{
    /*s40v2 video layer should be set, when 3d set, but cv200 does not need.
     * all the configurations are moved to dhd0.*/
    return HI_SUCCESS;
}

HI_S32 Chip_Specific_ReviseWbcZmeInput(DISP_WBC_E eWbc,
                                       HI_DISP_DISPLAY_INFO_S *pstDispInfo,
                                       HI_U32 *u32Width,
                                       HI_U32 *u32Height)
{
    #ifndef __DISP_PLATFORM_BOOT__

    if (DISP_STEREO_SBS_HALF == pstDispInfo->eDispMode)
       *u32Width = (*u32Width / 2) & 0xfffffffe;
    else if (DISP_STEREO_TAB == pstDispInfo->eDispMode)
        *u32Height = (*u32Height / 2) & 0xfffffffc;

    /*fpk only write left eye, and don't make crop, so we did not revise it.*/
    #endif
    return HI_SUCCESS;
}


/*==============================================*/
HI_VOID Chip_Specific_GetVersion(HI_DRV_DISP_VERSION_S * pVersion)
{
    VDP_Disp_GetVdpVersion(&pVersion->u32VersionPartL, &pVersion->u32VersionPartH);
    return;
}

HI_S32 Chip_Specific_SetMixerBgColor(HI_DRV_DISPLAY_E eChn, VDP_BKG_S stBkg)
{
    if (HI_DRV_DISPLAY_1 == eChn)
    {
        /*set cmb1 bgcolor*/
        VDP_CBM_SetMixerBkg(VDP_CBM_MIX0, stBkg);
        VDP_CBM_SetMixerBkg(VDP_CBM_MIXV0, stBkg);
    }
    else
    {
        /**/
        VDP_CBM_SetMixerBkg(VDP_CBM_MIX1, stBkg);
    }

    return HI_SUCCESS;
}

HI_S32 Chip_Specific_SetWbc3DInfo(DISP_WBC_E eWbc, HI_DISP_DISPLAY_INFO_S *pstDispInfo, HI_RECT_S *in)
{
    #ifndef __DISP_PLATFORM_BOOT__
    VDP_DISP_RECT_S stRect;

    DISP_MEMSET(&stRect, 0, sizeof(VDP_DISP_RECT_S));
    stRect.u32DXS = 0; /*does not support crop now.*/
    stRect.u32DYS = 0;
    stRect.u32DYL = (HI_U32)in->s32Height;
    stRect.u32DXL = (HI_U32)in->s32Width;	

	if (pstDispInfo->eDispMode == DISP_STEREO_NONE)
	{
	    VDP_WBC_SetThreeMd(VDP_LAYER_WBC_HD0, 0);
	}
	else
	{
		VDP_WBC_SetThreeMd(VDP_LAYER_WBC_HD0, 2);
	}

    VDP_WBC_SetCropReso(VDP_LAYER_WBC_HD0, stRect);

    #endif

    return HI_SUCCESS;
}

#ifndef __DISP_PLATFORM_BOOT__

HI_S32 Chip_Specific_WinHalSetRegionMute(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
    if ((u32LayerId == VDP_LAYER_VID0)
        || (u32LayerId == VDP_LAYER_VID3))
    {
        VDP_VID_SetRegionMuteEnable(u32LayerId,pstPara->u32RegionNum,HI_FALSE);
    }
    else if ((u32LayerId == VDP_LAYER_VID1)
        || (u32LayerId == VDP_LAYER_VID4))
    {
        VDP_VID_SetRegionMuteEnable(u32LayerId,pstPara->u32RegionNum,HI_FALSE);
    }
	else
    {

    }
   
    return HI_SUCCESS;
}

ISOGENY_MODE_E Chip_Specific_GetIsogenyMode(HI_VOID)
{
	return ISOGENY_WBC_MODE;
}

HI_S32 Chip_Specific_WinDoFurtherZmeProcess(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    VDP_DISP_RECT_S DispRect;
    HI_PQ_ZME_PARA_IN_S stZmeI;

    DISP_MEMSET(&DispRect, 0, sizeof(VDP_DISP_RECT_S));
    memset((void*)&stZmeI, 0, sizeof(HI_PQ_ZME_PARA_IN_S));

    if ((pstPara->bSecondHorZmeEnable) || (pstPara->bSecondVerZmeEnable))
    {
        VDP_VID_SetSrEnable(0, HI_TRUE);
        VDP_VID_SetSrOutReso(0,pstPara->stVideo);

        /*in final product, sr and  zme should be opened both.*/
        /* do not care "sr_zme_en" and "demo_ctrl", pq care.
          VDP_DISP_SetSrDemoMode(0,SR_DEMO_EN);
          VDP_DISP_SetSrMux(0,VDP_SR_AND_ZME_EN);
        */

        /*in 98m sr is in v0, when sr enable ,the sr's zme output is 2 size of input.*/
        stZmeI.u32ZmeFrmWIn = pstPara->stVideo.s32Width/2;
        stZmeI.u32ZmeFrmHIn = pstPara->stVideo.s32Height/2;

        stZmeI.u32ZmeFrmWOut = pstPara->stVideo.s32Width;
        stZmeI.u32ZmeFrmHOut = pstPara->stVideo.s32Height;

        /*sr's pixel format is semiplanar444.*/
        stZmeI.u8ZmeYCFmtIn  = 2;
        stZmeI.u8ZmeYCFmtOut = 2;

        stZmeI.bZmeFrmFmtIn  = 1;
        stZmeI.bZmeFrmFmtOut = (pstPara->pstDispInfo->bInterlace == HI_TRUE) ? 0 : 1;

        stZmeI.bZmeBFIn  = 0;
        stZmeI.bZmeBFOut = 0;
        stZmeI.u32Fidelity = pstPara->u32Fidelity;
        stZmeI.stOriRect = pstPara->stOriRect;
        stZmeI.u32InRate = pstPara->pstFrame->u32FrameRate;
        stZmeI.u32OutRate = pstPara->pstDispInfo->u32RefreshRate*10;
        stZmeI.bDispProgressive = !pstPara->pstDispInfo->bInterlace;

    #if 0
    HI_PRINT("2222:%d, %d, %d, %d,%d!\n",
            stZmeI.u32ZmeFrmWIn,
            stZmeI.u32ZmeFrmHIn,
            stZmeI.u32ZmeFrmWOut,
            stZmeI.u32ZmeFrmHOut,
            stZmeI.u8ZmeYCFmtIn);
    #endif
        /*when SR open, do not care about rwzb, so open fir.*/
        if (HI_SUCCESS != Chip_Specific_LayerZmeFunc(HI_PQ_DISP_SR_LAYER_ZME, &stZmeI,HI_TRUE))
            return HI_FAILURE;
    }
    else
    {
        /*only layer 0 has sr,so only layer 0 cant close sr.*/
        if (u32LayerId == VDP_LAYER_VID0)
                VDP_VID_SetSrEnable(0, HI_FALSE);
        return HI_SUCCESS;
    }

#endif

    return HI_SUCCESS;
}


#define TILE_H_STRIDE  256
#define TILE_V_STRIDE  16

/*confirm  needed: fit for both compressed and uncompressed stream?*/
HI_S32 Chip_Specific_WinHalSetAddr(HI_U32 u32LayerId, WIN_HAL_PARA_S *pstPara, HI_S32 s32exl)
{
    #if 1
    HI_DRV_VID_FRAME_ADDR_S *pstAddr;
    HI_U32 SrcAddrL[2], SrcAddrC[2], OffsetL, OffsetC;
    HI_RECT_S   stRect;
	HI_RECT_S	stInRect = pstPara->stIn ;

    if (!pstPara)
        DISP_FATAL_RETURN();

    /*first step: we should calculate out the offset.*/
    pstAddr = &(pstPara->pstFrame->stBufAddr[0]);
    if (   (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
        || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
        || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP)
        || (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP)
        ) 
    {
        SrcAddrL[0]  = pstAddr[0].u32PhyAddr_Y;
        SrcAddrL[1]  = pstAddr[1].u32PhyAddr_Y;

        SrcAddrC[0]  = pstAddr[0].u32PhyAddr_C;
        SrcAddrC[1]  = pstAddr[1].u32PhyAddr_C;

        /*for non-compress mode, we calculte the offset because vertical offset.*/

		OffsetL = (stInRect.s32Y * pstAddr[0].u32Stride_Y);
        OffsetC = (stInRect.s32Y * pstAddr[0].u32Stride_Y/2);

    }
	else if (pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1)
    {
		SrcAddrL[0]  = pstAddr[0].u32PhyAddr_Y;
        SrcAddrL[1]  = pstAddr[1].u32PhyAddr_Y;

        SrcAddrC[0]  = pstAddr[0].u32PhyAddr_C;
        SrcAddrC[1]  = pstAddr[1].u32PhyAddr_C;

        /*for non-compress mode, we calculte the offset because vertical offset.*/
        OffsetL = (stInRect.s32Y * pstAddr[0].u32Stride_Y);
        OffsetC = (stInRect.s32Y * pstAddr[0].u32Stride_Y);
	}
	else if ((pstPara->pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE))
	{	
		HI_U32 xBlock,yBlock,BlockStride,Blockoffset;
		xBlock = 0;
		yBlock = 0;
		BlockStride = 0;
		Blockoffset = 0;
		
		SrcAddrL[0]  = pstAddr[0].u32PhyAddr_Y;
		SrcAddrL[1]  = pstAddr[1].u32PhyAddr_Y;

		SrcAddrC[0]  = pstAddr[0].u32PhyAddr_C;
		SrcAddrC[1]  = pstAddr[1].u32PhyAddr_C;

		BlockStride = pstAddr[0].u32Stride_Y/(TILE_H_STRIDE*TILE_V_STRIDE);

		if (stInRect.s32Y)
		{
		   yBlock = stInRect.s32Y /TILE_V_STRIDE;
		   stInRect.s32Y = stInRect.s32Y%TILE_V_STRIDE;
		}
		
		if (stInRect.s32X)
		{
			xBlock = stInRect.s32X /TILE_H_STRIDE;
			stInRect.s32X = stInRect.s32X%TILE_H_STRIDE;
		}
		
		Blockoffset = yBlock*BlockStride + xBlock;
		
		OffsetL = Blockoffset*TILE_H_STRIDE*TILE_V_STRIDE;
		OffsetC = Blockoffset*TILE_H_STRIDE*TILE_V_STRIDE/2;
		//HI_PRINT("1--x,y (%d,%d)-->bs,yb,xb,ofb (%d,%d,%d,%d)\n",stInRect.s32X,stInRect.s32Y,BlockStride,yBlock,xBlock,blockoffset);
		//HI_PRINT("(str->%d,y %d,x %d,of %d) offset(%d,%d)-->(%d,%d,%d,%d)\n",BlockStride,yBlock,xBlock,blockoffset,OffsetL,OffsetC,pstPara->stIn.s32X,pstPara->stIn.s32Y,pstPara->stIn.s32Width,pstPara->stIn.s32Width);
	}
	else{
       DISP_FATAL_RETURN();
    }

    /*second step: we should set the extract line.*/
    internal_set_extlineRegister(u32LayerId, s32exl);
    memset((void*)&stRect, 0, sizeof(HI_RECT_S));

    /*third step: we should set crop register, in cv200 only horizical crop exist.*/
    if ((VDP_LAYER_VID0 == u32LayerId)
        || (VDP_LAYER_VID3 == u32LayerId))
		{
			VDP_VID_SetInCrop(u32LayerId, stInRect.s32X,
                        stInRect.s32X + stInRect.s32Width);
		}
    else {
        if (VDP_LAYER_VID1 == u32LayerId)
            VDP_VP_GetLayerReso(0, &stRect);
        else
            stRect = pstPara->pstDispInfo->stFmtResolution;


        VDP_VID_SetInCrop(u32LayerId, 0, stRect.s32Width);
    }


    if ((pstPara->en3Dmode == DISP_STEREO_FPK)||
        (pstPara->en3Dmode == DISP_STEREO_TAB)||
        (pstPara->en3Dmode == DISP_STEREO_SBS_HALF)) {

        /*Confirm needed: although there exist crop, but only horizical crop exist,
         * but we should although calculte the offset.
         * as for stride, since there exist extline setting, so can set be one-line
         * stride
         */
        if (HI_DRV_FT_NOT_STEREO == pstPara->pstFrame->eFrmType){
            VDP_VID_SetLayerAddr(u32LayerId, 0, pstPara->u32RegionNum, SrcAddrL[0] + OffsetL,
                                                SrcAddrC[0] + OffsetC,
                                                pstAddr[0].u32Stride_Y,
                                                pstAddr[0].u32Stride_C);

            VDP_VID_SetLayerAddr(u32LayerId, 1, pstPara->u32RegionNum, SrcAddrL[0] + OffsetL,
                                                SrcAddrC[0] + OffsetC,
                                                pstAddr[0].u32Stride_Y,
                                                pstAddr[0].u32Stride_C);

        } else if (HI_DRV_FT_BUTT > pstPara->pstFrame->eFrmType) {

            if (pstPara->bRightEyeFirst) {
                VDP_VID_SetLayerAddr(u32LayerId, 1, pstPara->u32RegionNum, SrcAddrL[0] + OffsetL,
                                                    SrcAddrC[0] + OffsetC,
                                                    pstAddr[0].u32Stride_Y,
                                                    pstAddr[0].u32Stride_C);

                VDP_VID_SetLayerAddr(u32LayerId, 0, pstPara->u32RegionNum, SrcAddrL[1] + OffsetL,
                                                    SrcAddrC[1] + OffsetC,
                                                    pstAddr[1].u32Stride_Y,
                                                    pstAddr[1].u32Stride_C);
            } else {
                VDP_VID_SetLayerAddr(u32LayerId, 0, pstPara->u32RegionNum, SrcAddrL[0] + OffsetL,
                                                    SrcAddrC[0] + OffsetC,
                                                    pstAddr[0].u32Stride_Y,
                                                    pstAddr[0].u32Stride_C);

                VDP_VID_SetLayerAddr(u32LayerId, 1, pstPara->u32RegionNum, SrcAddrL[1] + OffsetL,
                                                    SrcAddrC[1] + OffsetC,
                                                    pstAddr[1].u32Stride_Y,
                                                    pstAddr[1].u32Stride_C);
            }
        } else {
            DISP_FATAL_RETURN();
        }

        VDP_VID_SetFlipEnable(u32LayerId,HI_FALSE);
    } else {
        /*in cv200es, there exist a flip operation, but in cv200,
            does not support flip func.*/
        VDP_VID_SetLayerAddr(u32LayerId, 0, pstPara->u32RegionNum, SrcAddrL[0] + OffsetL,
                                        SrcAddrC[0] + OffsetC,
                                        pstAddr[0].u32Stride_Y,
                                        pstAddr[0].u32Stride_C);
        VDP_VID_SetFlipEnable(u32LayerId,HI_FALSE);
    }

#endif
    return HI_SUCCESS;
}

HI_S32 Chip_Specific_SetLayerCapability(VIDEO_LAYER_CAPABILITY_S *pstVideoLayerCap)
{
    HI_U32 eId;

    DISP_MEMSET((void*)pstVideoLayerCap, 0, sizeof(VIDEO_LAYER_CAPABILITY_S) * DEF_VIDEO_LAYER_MAX_NUMBER);

    eId = 0;
    pstVideoLayerCap[eId].eId       = (HI_U32)VDP_RM_LAYER_VID0;
    pstVideoLayerCap[eId].bSupport  = HI_TRUE;
    pstVideoLayerCap[eId].bZme      = HI_TRUE;
	
	/*mv410 and 98cv200, v0's zme is better than vpss in quanlity, so use v0's
	  * zme first.
	  */
	pstVideoLayerCap[eId].bZmePrior = HI_TRUE;
    pstVideoLayerCap[eId].bACC      = HI_FALSE;
    pstVideoLayerCap[eId].bACM      = HI_TRUE;
    pstVideoLayerCap[eId].bHDIn     = HI_TRUE;
    pstVideoLayerCap[eId].bHDOut    = HI_TRUE;
    pstVideoLayerCap[eId].bDcmp     = HI_TRUE;

	/*to be added sr, 3798m_zkun.*/
    pstVideoLayerCap[eId].bDci     = HI_TRUE;
    pstVideoLayerCap[eId].bSR      = HI_FALSE;

    /*now in cv200, all the internal bitwidth of vdp is 10bit.*/
    pstVideoLayerCap[eId].u32BitWidth = 8;

    /*98m only support 2k*2k, for v0、zme and sr  all ,because the ddr is 32bit。*/
    pstVideoLayerCap[eId].u32LayerWidthMax   = 1920;
    pstVideoLayerCap[eId].u32LayerHeightMax  = 1088;

    /*add sr dci information.*/
    pstVideoLayerCap[eId].stLayerProcInfo.eSrLocation = ALG_LOCATION_IN_V0;
    pstVideoLayerCap[eId].stLayerProcInfo.eDciLocation = ALG_LOCATION_IN_VP0;
    pstVideoLayerCap[eId].stLayerProcInfo.bSrBehindDci = HI_FALSE;

    pstVideoLayerCap[eId].stLayerProcInfo.bDci = HI_FALSE;
    pstVideoLayerCap[eId].stLayerProcInfo.bSR  = HI_FALSE;


    // s2 set va1
    eId++;
    pstVideoLayerCap[eId].eId      = (HI_U32)VDP_RM_LAYER_VID1;
    pstVideoLayerCap[eId].bSupport = HI_TRUE;
    pstVideoLayerCap[eId].bZme     = HI_FALSE;
	pstVideoLayerCap[eId].bZmePrior = HI_FALSE;
    pstVideoLayerCap[eId].bACC     = HI_TRUE;
    pstVideoLayerCap[eId].bACM     = HI_TRUE;
    pstVideoLayerCap[eId].bHDIn    = HI_TRUE;
    pstVideoLayerCap[eId].bHDOut   = HI_TRUE;
    pstVideoLayerCap[eId].bDcmp    = HI_FALSE;

    pstVideoLayerCap[eId].bDci     = HI_FALSE;
    pstVideoLayerCap[eId].bSR      = HI_FALSE;


    pstVideoLayerCap[eId].u32BitWidth = 8;
    pstVideoLayerCap[eId].u32LayerWidthMax   = 1920;
    pstVideoLayerCap[eId].u32LayerHeightMax  = 1088;

    pstVideoLayerCap[eId].stLayerProcInfo.bDci = HI_FALSE;
    pstVideoLayerCap[eId].stLayerProcInfo.bSR  = HI_FALSE;

    // s2 set vb0
    eId++;
    eId++;
    pstVideoLayerCap[eId].eId       = (HI_U32)VDP_RM_LAYER_VID3;
    pstVideoLayerCap[eId].bSupport  = HI_TRUE;
    pstVideoLayerCap[eId].bZme      = HI_FALSE;
	pstVideoLayerCap[eId].bZmePrior = HI_FALSE;
    pstVideoLayerCap[eId].bACC      = HI_FALSE;
    pstVideoLayerCap[eId].bACM      = HI_FALSE;
    pstVideoLayerCap[eId].bHDIn     = HI_FALSE;
    pstVideoLayerCap[eId].bHDOut    = HI_FALSE;

    pstVideoLayerCap[eId].bDcmp    = HI_FALSE;
    pstVideoLayerCap[eId].u32LayerWidthMax   = 720;
    pstVideoLayerCap[eId].u32LayerHeightMax  = 576;
    pstVideoLayerCap[eId].u32BitWidth = 8;

    pstVideoLayerCap[eId].bDci     = HI_FALSE;
    pstVideoLayerCap[eId].bSR      = HI_FALSE;

    pstVideoLayerCap[eId].stLayerProcInfo.bDci = HI_FALSE;
    pstVideoLayerCap[eId].stLayerProcInfo.bSR  = HI_FALSE;
    
    eId++;
    for(; eId <DEF_VIDEO_LAYER_MAX_NUMBER; eId++)
    {
        pstVideoLayerCap[eId].eId = (HI_U32)DEF_VIDEO_LAYER_INVALID_ID;
        pstVideoLayerCap[eId].bSupport = HI_FALSE;
        pstVideoLayerCap[eId].bZme     = HI_FALSE;
		pstVideoLayerCap[eId].bZmePrior = HI_FALSE;
        pstVideoLayerCap[eId].bACC     = HI_FALSE;
        pstVideoLayerCap[eId].bACM     = HI_FALSE;
        pstVideoLayerCap[eId].bHDIn    = HI_FALSE;
        pstVideoLayerCap[eId].bHDOut   = HI_FALSE;

        pstVideoLayerCap[eId].stLayerProcInfo.bDci = HI_FALSE;
        pstVideoLayerCap[eId].stLayerProcInfo.bSR  = HI_FALSE;
    }

    return HI_SUCCESS;
}
#endif

HI_VOID DISP_HAL_ResetDispState(DISP_CAPA_S *pstDispCapability, DISP_CH_CFG_S *pstDispConfig)
{
    memset(&pstDispCapability[0], 0, sizeof(DISP_CAPA_S) * HI_DRV_DISPLAY_BUTT);

    pstDispCapability[HI_DRV_DISPLAY_0].bSupport = HI_TRUE;
    pstDispCapability[HI_DRV_DISPLAY_0].bHD      = HI_TRUE;
    pstDispCapability[HI_DRV_DISPLAY_0].bWbc    = HI_FALSE;
	pstDispCapability[HI_DRV_DISPLAY_0].bDhd0WbcP2I    = HI_FALSE;

    pstDispCapability[HI_DRV_DISPLAY_1].bSupport = HI_TRUE;
    pstDispCapability[HI_DRV_DISPLAY_1].bHD      = HI_TRUE;
    pstDispCapability[HI_DRV_DISPLAY_1].bWbc     = HI_FALSE;
	pstDispCapability[HI_DRV_DISPLAY_1].bDhd0WbcP2I  = HI_FALSE;
	
    pstDispCapability[HI_DRV_DISPLAY_2].bSupport = HI_FALSE;
	
    memset(&pstDispConfig[0], 0, sizeof(DISP_CH_CFG_S) * HI_DRV_DISPLAY_BUTT);
	
    return;
}


#define VDP_CLK_MODE_250MHZ     0
#define VDP_CLK_MODE_288MHZ     1
#define VDP_CLK_MODE_300MHZ     2

HI_S32 Chip_Specific_DispSetMSChnEnable(HI_U32 u32DispIdM, HI_U32 u32DispIdS, HI_U32 u32DelayMs, HI_BOOL bEnalbe)
{

    VDP_DISP_SetIntfEnable(u32DispIdM , bEnalbe);
//    VDP_DISP_SetIntfEnable(u32DispIdS, bEnalbe);

    VDP_DISP_SetRegUp(u32DispIdM);
 //   VDP_DISP_SetRegUp(u32DispIdS);


    /*Warning: time delay below is neccessary and can't be deleted.
     *you should not delete it when do some performance optimization.
     * when changing format, we should ensure that the dhdx is closed indeed.
     * so time delay is needed.because 1 period may not take effect, two cycle needed. delayms *2
     * DTS2013091500664.
     */
    if (bEnalbe == HI_FALSE)
        DISP_MSLEEP(u32DelayMs*2);

    return HI_SUCCESS;
}

HI_S32 Chip_Specific_DispSelectPll(HI_DRV_DISPLAY_E eChn, HI_U32 uPllIndex)
{
    if (HI_DRV_DISPLAY_0 == eChn)
    {
        U_PERI_CRG54 PERI_CRG54Tmp; /* 0xd8 */

        PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
        PERI_CRG54Tmp.bits.vo_sd_clk_sel = uPllIndex;
        g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;
    }
    else
    {
        U_PERI_CRG54 PERI_CRG54Tmp; /* 0xd8 */

        PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
        PERI_CRG54Tmp.bits.vo_hd_clk_sel = uPllIndex;
        g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;
    }

    VDP_SelectClk(VDP_CLK_MODE_300MHZ);

    return HI_SUCCESS;
}
HI_VOID  Chip_Specific_DispSetPll(DISP_PLL_SOURCE_E enPllIndex,HI_U32 u32PllxReg0,HI_U32 u32PllxReg1)
{
    if (DISP_CLOCK_SOURCE_HD0 == enPllIndex)
    {
        U_PERI_CRG_PLL10 PERI_CRG10Tmp;
        U_PERI_CRG_PLL11 PERI_CRG11Tmp;

        PERI_CRG10Tmp.u32 = g_pstRegCrg->PERI_CRG_PLL10.u32;
        PERI_CRG10Tmp.bits.hpll_ctrl0 = u32PllxReg0;
        g_pstRegCrg->PERI_CRG_PLL10.u32 = PERI_CRG10Tmp.u32;

        PERI_CRG11Tmp.u32 = g_pstRegCrg->PERI_CRG_PLL11.u32;
        PERI_CRG11Tmp.bits.hpll_ctrl1 = u32PllxReg1;
        g_pstRegCrg->PERI_CRG_PLL11.u32 = PERI_CRG11Tmp.u32;
    }
    else  if (DISP_CLOCK_SOURCE_HD1 == enPllIndex)
    {
        /*pay attention, no pll16/pll17.*/
    }
    else  if (DISP_CLOCK_SOURCE_SD0 == enPllIndex)
    {/*do nothing*/
    }

}

#define CLOCK_DIV_1  3
#define CLOCK_DIV_2  0
#define CLOCK_DIV_4  1

#define CLK_HDMI_VO_1_1  0
#define CLK_HDMI_VO_1_2  1

/* 3716cv200  add hdmi pix*/
HI_VOID Chip_Specific_DispSetChanClk(HI_DRV_DISPLAY_E eChn, DISP_FMT_CFG_S *pstCfg)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32  = g_pstRegCrg->PERI_CRG54.u32;
    if ((pstCfg->stInfo.eFmt >= HI_DRV_DISP_FMT_PAL )
        && (pstCfg->stInfo.eFmt <= HI_DRV_DISP_FMT_1440x480i_60 )
        )
    {
         /*if SD format  : select clk_vo_xd : clk_hdmi  1:2 */
         if (HI_DRV_DISPLAY_0 == eChn)
         {
            PERI_CRG54Tmp.bits.vo_sd_hdmi_clk_sel = CLK_HDMI_VO_1_2;
         }
         else
         {
            PERI_CRG54Tmp.bits.vo_hd_hdmi_clk_sel = CLK_HDMI_VO_1_2;
         }

        g_pstRegCrg->PERI_CRG54.u32= PERI_CRG54Tmp.u32;
    }
    else
    {
        /*if not SD format  : select clk_vo_xd : clk_hdmi  1:1 */
        if (HI_DRV_DISPLAY_0 == eChn)
         {
            PERI_CRG54Tmp.bits.vo_sd_hdmi_clk_sel = CLK_HDMI_VO_1_1;
         }
         else
         {
            PERI_CRG54Tmp.bits.vo_hd_hdmi_clk_sel = CLK_HDMI_VO_1_1;
         }
        g_pstRegCrg->PERI_CRG54.u32= PERI_CRG54Tmp.u32;
    }

    Chip_Specific_DispSelectPll(eChn, pstCfg->enPllIndex);
    VDP_DISP_SelectChanClkDiv(eChn, CLOCK_DIV_2);
#ifndef HI_DISP_DOUBLE_HD_SUPPORT
    if  (HI_DRV_DISPLAY_1 == eChn)
#endif
        Chip_Specific_DispSetPll(pstCfg->enPllIndex, pstCfg->u32Pll[0], pstCfg->u32Pll[1]);
}


#define VDP_CBM_ZORDER_NO    2

#define VDP_MIX_VIDEO    0
#define VDP_MIX_GFX   1

#define VDP_MIXv0_LayerCNT    2
#define VDP_MIXv1_LayerCNT    1
#define VDP_MIXg1_LayerCNT    2

static HI_DRV_DISP_LAYER_E s_CBM_Zorder[HI_DRV_DISPLAY_BUTT][VDP_CBM_ZORDER_NO];

static VDP_LAYER_VID_E s_MIXv0_Zorder[VDP_LAYER_VID_BUTT]=
{
    VDP_LAYER_VID0,
    VDP_LAYER_VID1
};

/*mixv1  and mixg1 be control by CMB*/
static VDP_CBM_LAYER_E s_MIXv1_Zorder[VDP_CBM_BUTT]=
{
    VDP_CBM_VID3,
};
static VDP_CBM_LAYER_E s_MIXg1_Zorder[VDP_CBM_BUTT]=
{
    VDP_CBM_GP1,
    VDP_CBM_GFX3
};

HI_U32 Chip_Specific_GetMixvMaxNumvber(VDP_CBM_MIX_E eM)
{
    switch(eM)
    {
        case VDP_CBM_MIXV0:
            return VDP_MIXv0_LayerCNT;
        case VDP_CBM_MIXV1:
            return VDP_MIXv1_LayerCNT;
        default:
            return 0;
    }
}

HI_VOID  Layer_Trans(VDP_LAYER_VID_E *penVideoLayer,VDP_CBM_LAYER_E *penCBMLayer,HI_BOOL bTrans)
{
    if (bTrans)
    {
        if (VDP_CBM_VID3 == *penCBMLayer)
            *penVideoLayer = VDP_LAYER_VID3;
        else if (VDP_CBM_VID4 == *penCBMLayer)
            *penVideoLayer = VDP_LAYER_VID4;
    }
    else
    {
        if (VDP_LAYER_VID3 == *penVideoLayer)
            *penCBMLayer = VDP_CBM_VID3;
        else if (VDP_LAYER_VID4 == *penVideoLayer)
            *penCBMLayer = VDP_CBM_VID4;
    }
}

HI_S32 Chip_Specific_CBM_SetDispZorder(HI_DRV_DISPLAY_E enDisp)
{
    HI_U32 i, j = 0;
    VDP_CBM_MIX_E eMixId;
    HI_U32 MixArray[VDP_CBM_BUTT];
    memset(MixArray,0 ,sizeof(HI_U32)*VDP_CBM_BUTT);

    if (HI_DRV_DISPLAY_1 == enDisp)
    {
        eMixId = VDP_CBM_MIX0;
        j = 2;
        if (s_CBM_Zorder[HI_DRV_DISPLAY_1][1] == VDP_MIX_GFX)
       {
            MixArray[1] = VDP_CBM_GP0;
            MixArray[0] = VDP_CBM_VP0;
        }
        else
        {
            MixArray[1] = VDP_CBM_VP0;
            MixArray[0] = VDP_CBM_GP0;
        }
    }
    else if (HI_DRV_DISPLAY_0 == enDisp)
    {
        eMixId = VDP_CBM_MIX1;
        j = 0;

        if (s_CBM_Zorder[HI_DRV_DISPLAY_0][1] == VDP_MIX_GFX)
       {
            for (i = 0;i < VDP_MIXv1_LayerCNT;i++)
            {
                MixArray[j] = s_MIXv1_Zorder[i];
                j++;
            }
             for (i = 0;i < VDP_MIXg1_LayerCNT;i++)
            {
                MixArray[j] = s_MIXg1_Zorder[i];
                j++;
            }
       }
        else
        {
             for (i = 0;i < VDP_MIXg1_LayerCNT;i++)
            {
                MixArray[j] = s_MIXg1_Zorder[i];
                j++;
            }
             for (i = 0;i < VDP_MIXv1_LayerCNT;i++)
            {
                MixArray[j] = s_MIXv1_Zorder[i];
                j++;
            }
        }
    }

    /*set zorder */
    for( i = 0; i< j; i++)
    {
        VDP_CBM_SetMixerPrio(eMixId, MixArray[i], i);
    }
     return HI_SUCCESS;
}


HI_VOID Chip_Specific_CBM_GetMixvPrio(VDP_CBM_MIX_E enMixer, HI_U32 u32prio, HI_U32 *pu32layer_id)
{
    *pu32layer_id = VDP_LAYER_VID_BUTT;

    if ( VDP_CBM_MIXV0 == enMixer)
    {
        if ( u32prio < VDP_MIXv0_LayerCNT)
        {
            *pu32layer_id = s_MIXv0_Zorder[u32prio];
        }
    }
    else if ( VDP_CBM_MIXV1 == enMixer)
    {
        if ( u32prio < VDP_MIXv1_LayerCNT)
        {
             Layer_Trans(pu32layer_id,&s_MIXv1_Zorder[u32prio],HI_TRUE);
        }
    }
    else
    {
        *pu32layer_id = VDP_LAYER_VID_BUTT;
        HI_PRINT("Error, Vou_SetCbmMixerPrio() Set mixer  select wrong layer ID\n");
    }
    //HI_PRINT("get mix%d : index (%d)  layer(%d)\n",enMixer,u32prio,*pu32layer_id);
}


HI_VOID Chip_Specific_CBM_SetMixvPrio(VDP_CBM_MIX_E enMixer,HI_U32 *pMixArray,HI_U32 u32prio)
{
    HI_U32 i;

    if ( VDP_CBM_MIXV0 == enMixer)
    {
        for (i =0 ; i< VDP_MIXv0_LayerCNT; i++)
        {
            s_MIXv0_Zorder[i] = pMixArray[i];
            VDP_CBM_SetMixerPrio(enMixer, s_MIXv0_Zorder[i] , i);
            //HI_PRINT("---mix%d---index,layer --(%d)-->(%d)\n",enMixer,i,s_MIXv0_Zorder[i]);
        }
    }
    else if ( VDP_CBM_MIXV1 == enMixer)
    {
        for (i =0 ; i< VDP_MIXv1_LayerCNT; i++)
        {
            //s_MIXv1_Zorder[i] = pMixArray[i];
            Layer_Trans(&(pMixArray[i]),&s_MIXv1_Zorder[i],HI_FALSE);
             //HI_PRINT("---mix%d---index,layer --(%d)-->(%d)\n",enMixer,i,s_MIXv1_Zorder[i]);
        }
        /**/
        Chip_Specific_CBM_SetDispZorder(HI_DRV_DISPLAY_0);
    }
    else
    {
        HI_PRINT("Error, Chip_Specific_CBM_SetMixvPrio() select Mixer ID\n");
    }

}

HI_S32 Chip_Specific_CBM_MovTOP(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer)
{
    if (HI_DRV_DISP_LAYER_VIDEO == enLayer)
    {
        s_CBM_Zorder[enDisp][1] = VDP_MIX_VIDEO;
        s_CBM_Zorder[enDisp][0] = VDP_MIX_GFX;
    }
    else if (HI_DRV_DISP_LAYER_GFX == enLayer)
    {
        s_CBM_Zorder[enDisp][1]  = VDP_MIX_GFX;
        s_CBM_Zorder[enDisp][0]  = VDP_MIX_VIDEO;
     }
    else
    {
        HI_PRINT("Error,  para DISP_LAYER wrong  \n");
        return HI_FAILURE;
    }
    Chip_Specific_CBM_SetDispZorder(enDisp);

    return HI_SUCCESS;
}


HI_S32 Chip_Specific_DISP_OpenClock(HI_VOID)
{
    /* open clock and reset vdp module*/
    U_PERI_CRG54 unTmpValue;

    unTmpValue.u32 = g_pstRegCrg->PERI_CRG54.u32;

    if (   unTmpValue.bits.vou_srst_req
        || (!unTmpValue.bits.vo_bus_cken)
        || (!unTmpValue.bits.vo_cken))
    {
        DISP_ResetCRG();
    }

    return HI_SUCCESS;
}

/*to be confirmed: the clock reset is right or not?
 *should all the clock bit be set to 0?
 */
HI_S32 Chip_Specific_DISP_CloseClock(HI_VOID)
{
    U_PERI_CRG54 unTmpValue;

    unTmpValue.u32 = g_pstRegCrg->PERI_CRG54.u32;

    /*Close Clock*/
    //unTmpValue.u32 = 0;
    unTmpValue.bits.vo_bus_cken  = 0;
    unTmpValue.bits.vo_cken      = 0;
    unTmpValue.bits.vou_srst_req = 1; /*set reset*/

    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    return HI_SUCCESS;
}
