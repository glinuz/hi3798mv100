/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/6/20
  Description   :
  History       :
  Date          : 2010/6/20
  Author        : q46153 l00168554
  Modification  : 2015/06/16 l232354
*******************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "hi_unf_hdmi.h"
#include "hi_mpi_hdmi.h"
#include "hi_mpi_mem.h"

/************************************************************************/
/* constants data                                                       */
/************************************************************************/
#define     HDMI_ARRAY_SIZE(a)          (sizeof(a)/sizeof(a[0]))
#define     HDMI_VIC_PC_BASE            ((HDMI_VIC_VIRTUAL_BASE)+0x100)
#define     DOLBY_IEEE_OUI              0x00D046
/************************************************************************/
/* data structure                                                       */
/************************************************************************/
typedef struct
{
    HI_U32              u32EtablishCode;
    HI_U32              u32Format;
}HDMI_ESTAB_FORMAT_S;

typedef struct
{
    HI_U32              u32Vic;
    HI_U32              u32PixlFreq;                /* KHz */
    HI_U32              u32HorActive;
    HI_U32              u32VerActive;
    HI_U32              u32FieldRate;               /* 0.01Hz */
    HI_U32              u32Format;
}HDMI_FMT_PARAM_S;

/************************************************************************/
/* global data                                                          */
/************************************************************************/
static const HDMI_ESTAB_FORMAT_S s_g_stFmtEstabTable[] =
{
    { HDMI_EDID_ESTABTIMG_VESA_800X600_60       ,   HI_UNF_ENC_FMT_VESA_800X600_60  },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_56       ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_640X480_75       ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_640X480_72       ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_VGA_640X480_67    ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_XGA_640X480_60    ,   HI_UNF_ENC_FMT_VESA_800X600_60  },
    { HDMI_EDID_ESTABTIMG_IBM_XGA2_720X400_88   ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_XGA_720X400_70    ,   HI_UNF_ENC_FMT_BUTT             },

    { HDMI_EDID_ESTABTIMG_VESA_1280X1024_75     ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_75      ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_70      ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_60      ,   HI_UNF_ENC_FMT_VESA_1024X768_60 },
    { HDMI_EDID_ESTABTIMG_IBM_1024X768_87       ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_APPLE_MACII_832X624_75,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_75       ,   HI_UNF_ENC_FMT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_72       ,   HI_UNF_ENC_FMT_BUTT             },

    { HDMI_EDID_ESTABTIMG_VESA_1152X870_75      ,   HI_UNF_ENC_FMT_BUTT             },

};

static const HDMI_FMT_PARAM_S s_g_stFmtParamTable[]=
{
    /********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
    {                        1, 25175 ,  640 ,480 , 6000 ,  HI_UNF_ENC_FMT_861D_640X480_60  },      // 1. 640 x 480p @ 60 VGA
    {                        2, 27000 ,  720 ,480 , 6000 ,  HI_UNF_ENC_FMT_480P_60          },      // 2,3 720 x 480p
    {                        3, 27000 ,  720 ,480 , 6000 ,  HI_UNF_ENC_FMT_480P_60          },      // 2,3 720 x 480p
    {                        4, 74170 ,  1280,720 , 6000 ,  HI_UNF_ENC_FMT_720P_60          },      // 4   1280 x 720p
    {                        5, 74170 ,  1920,1080, 6000 ,  HI_UNF_ENC_FMT_1080i_60         },      // 5 1920 x 1080i
    {                        6, 27000 ,  720 ,480 , 6000 ,  HI_UNF_ENC_FMT_NTSC             },      // 6,7 720 x 480i, pix repl
    {                        7, 27000 ,  720 ,480 , 6000 ,  HI_UNF_ENC_FMT_NTSC             },      // 6,7 720 x 480i, pix repl
    {                        8, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 8,9(1) 1440 x 240p
    {                        8, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 8,9(2) 1440 x 240p
    {                        9, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 8,9(1) 1440 x 240p
    {                        9, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 8,9(2) 1440 x 240p
    {                       10, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 10,11 2880 x 480p
    {                       11, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 10,11 2880 x 480p
    {                       12, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 12,13(1) 2280 x 240p
    {                       12, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 12,13(2) 2280 x 240p
    {                       14, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 14, 15 1140 x 480p
    {                       16, 148500,  1920,1080, 6000 ,  HI_UNF_ENC_FMT_1080P_60         },      // 16 1920 x 1080p
    {                       17, 27000 ,  720 ,576 , 5000 ,  HI_UNF_ENC_FMT_576P_50          },      // 17,18 720 x 576p
    {                       18, 27000 ,  720 ,576 , 5000 ,  HI_UNF_ENC_FMT_576P_50          },      // 17,18 720 x 576p
    {                       19, 74250 ,  1280,720 , 5000 ,  HI_UNF_ENC_FMT_720P_50          },      // 19 1280 x 720p
    {                       20, 74250 ,  1920,1080, 5000 ,  HI_UNF_ENC_FMT_1080i_50         },      // 20 1920 x 1080i
    {                       21, 27000 ,  1440,576 , 5000 ,  HI_UNF_ENC_FMT_PAL              },      // 21,22 1440 x 576i
    {                       22, 27000 ,  1440,576 , 5000 ,  HI_UNF_ENC_FMT_PAL              },      // 21,22 1440 x 576i
    {                       23, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 23,24(1) 1440 x 288p
    {                       23, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 23,24(2) 1440 x 288p
    {                       23, 27000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 23,24(3) 1440 x 288p
    {                       25, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 25, 26 2880 x 576p
    {                       27, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 27,28(1) 2880 x 288p
    {                       27, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 27,28(2) 2880 x 288p
    {                       27, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 27,28(3) 2880 x 288p
    {                       29, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 29,30 1440 x 576p

    /********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
    {                       31, 148500,  1440,576 , 5000 ,  HI_UNF_ENC_FMT_1080P_50         },      // 31(1) 1920 x 1080p
    {                       32, 74170 ,  1920,1080, 2400 ,  HI_UNF_ENC_FMT_1080P_24         },      // 32(2) 1920 x 1080p
    {                       33, 74250 ,  1920,1080, 2500 ,  HI_UNF_ENC_FMT_1080P_25         },      // 33(3) 1920 x 1080p
    {                       34, 74170 ,  1920,1080, 3000 ,  HI_UNF_ENC_FMT_1080P_30         },      // 34(4) 1920 x 1080p
    {                       35, 108000,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 35, 36 2880 x 480p@59.94/60Hz
    {                       37, 108000,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 37, 38 2880 x 576p@50Hz
    {                       39, 72000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 39 1920 x 1080i@50Hz
    {                       40, 148500,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 40 1920 x 1080i@100Hz
    {                       41, 148500,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 41 1280 x 720p@100Hz
    {                       42, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 42, 43, 720p x 576p@100Hz
    {                       44, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 44, 45, 720p x 576i@100Hz, pix repl
    {                       46, 148500,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 46, 1920 x 1080i@119.88/120Hz
    {                       47, 148500,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 47, 1280 x 720p@119.88/120Hz
    {                       48, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 48, 49 720 x 480p@119.88/120Hz
    {                       50, 54000 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 50, 51 720 x 480i@119.88/120Hz
    {                       52, 10800 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 52, 53, 720 x 576p@200Hz
    {                       54, 10800 ,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 54, 55, 1440 x 720i @200Hz, pix repl
    {                       56, 108000,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 56, 57, 720 x 480p @239.76/240Hz
    {                       58, 108000,  0   ,0   , 0    ,  HI_UNF_ENC_FMT_BUTT             },      // 58, 59, 1440 x 480i @239.76/240Hz, pix repl

#if defined (DVI_SUPPORT)
    /********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
    { HDMI_VIC_PC_BASE+      0, 40000 , 800 ,600 ,  60 ,      HI_UNF_ENC_FMT_VESA_800X600_60    },  // 800x600@60
    { HDMI_VIC_PC_BASE+      1, 65000 , 1024,768 ,  60 ,      HI_UNF_ENC_FMT_VESA_1024X768_60   },  // 1024x768@60
    { HDMI_VIC_PC_BASE+      2, 74250 , 1280,720 ,  60 ,      HI_UNF_ENC_FMT_VESA_1280X720_60   },  // 720x400@70.08
    { HDMI_VIC_PC_BASE+      3, 83500 , 1280,800 ,  60 ,      HI_UNF_ENC_FMT_VESA_1280X800_60   },  // 720x400@85.04
    { HDMI_VIC_PC_BASE+      4, 108000, 1280,1024,  60 ,      HI_UNF_ENC_FMT_VESA_1280X1024_60  },  // 1280x1024@60
    { HDMI_VIC_PC_BASE+      5, 85500 , 1360,768 ,  60 ,      HI_UNF_ENC_FMT_VESA_1360X768_60   },  // 1360x768@60
    { HDMI_VIC_PC_BASE+      6, 85500 , 1366,768 ,  60 ,      HI_UNF_ENC_FMT_VESA_1366X768_60   },  // 1360x768@60
    { HDMI_VIC_PC_BASE+      7, 121750, 1400,1050,  60 ,      HI_UNF_ENC_FMT_VESA_1400X1050_60  },  // 1360x768@60
    { HDMI_VIC_PC_BASE+      8, 106500, 1440,900,   60 ,      HI_UNF_ENC_FMT_VESA_1440X900_60   },  // 800x600@56.25
    { HDMI_VIC_PC_BASE+      9, 162000, 1600,1200,  60 ,      HI_UNF_ENC_FMT_VESA_1600X1200_60  },  // 1600x1200@60
    { HDMI_VIC_PC_BASE+      10,146250, 1680,1050,  60 ,      HI_UNF_ENC_FMT_VESA_1680X1050_60  },  // 800x600@72.19
    { HDMI_VIC_PC_BASE+      11,148500, 1920,1080,  60 ,      HI_UNF_ENC_FMT_VESA_1920X1080_60  },  // 800x600@72.19
    { HDMI_VIC_PC_BASE+      12,154000, 1920,1200,  60 ,      HI_UNF_ENC_FMT_VESA_1920X1200_60  },  // 1920x1200@59.95
    { HDMI_VIC_PC_BASE+      13,234000, 1920,1440,  60 ,      HI_UNF_ENC_FMT_VESA_1920X1440_60  },  // 1024x768@60
    { HDMI_VIC_PC_BASE+      14,156750, 2048,1152,  60 ,      HI_UNF_ENC_FMT_VESA_2048X1152_60  },  // 1024x768@70.07
    //{ HDMI_VIC_PC_BASE+    15, 88750, 1440,900,   60 ,      HI_UNF_ENC_FMT_VESA_1440X900_60_RB    },  // 800x600@56.25
    { HDMI_VIC_PC_BASE+      15, 97750, 1600,900,   60 ,      HI_UNF_ENC_FMT_VESA_1600X900_60_RB    },  // 800x600@56.25
    { HDMI_VIC_PC_BASE+     16, 108000, 0   ,0   ,  0    ,    HI_UNF_ENC_FMT_BUTT               },  // 1152x864@75
    { HDMI_VIC_PC_BASE+     17, 108000, 0   ,0   ,  0    ,    HI_UNF_ENC_FMT_BUTT               },  // 1152x864@75
    { HDMI_VIC_PC_BASE+     18, 68250 , 0   ,0   ,  0    ,    HI_UNF_ENC_FMT_BUTT               },  // 1280x768@59.95
    { HDMI_VIC_PC_BASE+     19, 79500 , 0   ,0   ,  0    ,    HI_UNF_ENC_FMT_BUTT               },  // 1280x768@59.87


#endif

    /********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
    { 93, 297000,  3840,2160, 2400 ,    HI_UNF_ENC_FMT_3840X2160_24         },
    { 94, 297000,  3840,2160, 2500 ,    HI_UNF_ENC_FMT_3840X2160_25         },
    { 95, 297000,  3840,2160, 3000 ,    HI_UNF_ENC_FMT_3840X2160_30         },
    { 96, 594000,  3840,2160, 5000 ,    HI_UNF_ENC_FMT_3840X2160_50         },
    { 97, 594000,  3840,2160, 6000 ,    HI_UNF_ENC_FMT_3840X2160_60         },

    { 98, 297000,  4096,2160, 2400 ,    HI_UNF_ENC_FMT_4096X2160_24         },
    { 99, 297000,  4096,2160, 2500 ,    HI_UNF_ENC_FMT_4096X2160_25         },
    { 100, 297000,  4096,2160, 3000 ,   HI_UNF_ENC_FMT_4096X2160_30         },
    { 101, 594000,  4096,2160, 5000 ,   HI_UNF_ENC_FMT_4096X2160_50         },
    { 102, 594000,  4096,2160, 6000 ,   HI_UNF_ENC_FMT_4096X2160_60         },

};

static HDMI_INFOFRAME_ID_E  s_enDrvInfoFrameId[HI_INFOFRAME_TYPE_BUTT] =
{
    HDMI_INFOFRAME_TYPE_AVI,
    HDMI_INFOFRAME_TYPE_SPD,
    HDMI_INFOFRAME_TYPE_AUDIO,
    HDMI_INFOFRAME_TYPE_MPEG,
    HDMI_INFOFRAME_TYPE_VENDOR,
};

typedef struct
{
    HI_DRV_DISP_FMT_E       enFmt;
    HDMI_VIDEO_TIMING_E     enTiming;
}HDMI_UNF_FMT_TO_TIMING_S;

static HDMI_UNF_FMT_TO_TIMING_S g_enUnfFmtToTiming[] =
{
    {HI_DRV_DISP_FMT_1080P_60,                  HDMI_VIDEO_TIMING_1920X1080P_60000		},
    {HI_DRV_DISP_FMT_1080P_59_94,               HDMI_VIDEO_TIMING_1920X1080P_60000      },
    {HI_DRV_DISP_FMT_1080P_50,                  HDMI_VIDEO_TIMING_1920X1080P_50000      },
    {HI_DRV_DISP_FMT_1080P_30,                  HDMI_VIDEO_TIMING_1920X1080P_30000      },
    {HI_DRV_DISP_FMT_1080P_29_97,               HDMI_VIDEO_TIMING_1920X1080P_30000      },
    {HI_DRV_DISP_FMT_1080P_25,                  HDMI_VIDEO_TIMING_1920X1080P_25000      },
    {HI_DRV_DISP_FMT_1080P_24,                  HDMI_VIDEO_TIMING_1920X1080P_24000      },
    {HI_DRV_DISP_FMT_1080P_23_976,              HDMI_VIDEO_TIMING_1920X1080P_24000      },
    {HI_DRV_DISP_FMT_1080i_60,                  HDMI_VIDEO_TIMING_1920X1080I_60000      },
    {HI_DRV_DISP_FMT_1080i_59_94,               HDMI_VIDEO_TIMING_1920X1080I_60000      },
    {HI_DRV_DISP_FMT_1080i_50,                  HDMI_VIDEO_TIMING_1920X1080I_50000      },
    {HI_DRV_DISP_FMT_720P_60,                   HDMI_VIDEO_TIMING_1280X720P_60000       },
    {HI_DRV_DISP_FMT_720P_59_94,                HDMI_VIDEO_TIMING_1280X720P_60000       },
    {HI_DRV_DISP_FMT_720P_50,                   HDMI_VIDEO_TIMING_1280X720P_50000       },
    {HI_DRV_DISP_FMT_576P_50,                   HDMI_VIDEO_TIMING_720X576P_50000        },
    {HI_DRV_DISP_FMT_480P_60,                   HDMI_VIDEO_TIMING_720X480P_60000        },
    {HI_DRV_DISP_FMT_3840X2160_24,              HDMI_VIDEO_TIMING_3840X2160P_24000      },
    {HI_DRV_DISP_FMT_3840X2160_23_976,          HDMI_VIDEO_TIMING_3840X2160P_24000      },
    {HI_DRV_DISP_FMT_3840X2160_25,              HDMI_VIDEO_TIMING_3840X2160P_25000      },
    {HI_DRV_DISP_FMT_3840X2160_30,              HDMI_VIDEO_TIMING_3840X2160P_30000      },
    {HI_DRV_DISP_FMT_3840X2160_29_97,           HDMI_VIDEO_TIMING_3840X2160P_30000      },
    {HI_DRV_DISP_FMT_4096X2160_24,              HDMI_VIDEO_TIMING_4096X2160P_24000      },
    {HI_DRV_DISP_FMT_4096X2160_25,              HDMI_VIDEO_TIMING_4096X2160P_25000      },
    {HI_DRV_DISP_FMT_4096X2160_30,              HDMI_VIDEO_TIMING_4096X2160P_30000      },
    {HI_DRV_DISP_FMT_3840X2160_50,              HDMI_VIDEO_TIMING_3840X2160P_50000      },
    {HI_DRV_DISP_FMT_3840X2160_60,              HDMI_VIDEO_TIMING_3840X2160P_60000      },
    {HI_DRV_DISP_FMT_4096X2160_50,              HDMI_VIDEO_TIMING_4096X2160P_50000      },
    {HI_DRV_DISP_FMT_4096X2160_60,              HDMI_VIDEO_TIMING_4096X2160P_60000      },
    {HI_DRV_DISP_FMT_1440x576i_50,              HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL,                       HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_B,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_B1,                    HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_D,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_D1,                    HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_G,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_H,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_K,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_I,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_M,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_N,                     HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_Nc,                    HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_PAL_60,                    HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_SIN,                 HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_COS,                 HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_L,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_B,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_G,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_D,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_K,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_SECAM_H,                   HDMI_VIDEO_TIMING_1440X576I_50000       },
    {HI_DRV_DISP_FMT_1440x480i_60,              HDMI_VIDEO_TIMING_1440X480I_60000       },
    {HI_DRV_DISP_FMT_NTSC,                      HDMI_VIDEO_TIMING_1440X480I_60000       },
    {HI_DRV_DISP_FMT_NTSC_J,                    HDMI_VIDEO_TIMING_1440X480I_60000       },
    {HI_DRV_DISP_FMT_NTSC_443,                  HDMI_VIDEO_TIMING_1440X480I_60000       },
    {HI_DRV_DISP_FMT_861D_640X480_60,           HDMI_VIDEO_TIMING_640X480P_60000        },
    {HI_DRV_DISP_FMT_VESA_800X600_60,           HDMI_VIDEO_TIMING_VESA_800X600_60       },
    {HI_DRV_DISP_FMT_VESA_1024X768_60,          HDMI_VIDEO_TIMING_VESA_1024X768_60      },
    {HI_DRV_DISP_FMT_VESA_1280X800_60,          HDMI_VIDEO_TIMING_VESA_1280X800_60      },
    {HI_DRV_DISP_FMT_VESA_1280X1024_60,	        HDMI_VIDEO_TIMING_VESA_1280X1024_60     },
    {HI_DRV_DISP_FMT_VESA_1360X768_60,          HDMI_VIDEO_TIMING_VESA_1360X768_60      },
    {HI_DRV_DISP_FMT_VESA_1440X900_60,          HDMI_VIDEO_TIMING_VESA_1440X900_60      },
    {HI_DRV_DISP_FMT_VESA_1440X900_60_RB,       HDMI_VIDEO_TIMING_VESA_1440X900_60_RB   },
    {HI_DRV_DISP_FMT_VESA_1400X1050_60,         HDMI_VIDEO_TIMING_VESA_1440X1050_60     },
    {HI_DRV_DISP_FMT_VESA_1600X1200_60,         HDMI_VIDEO_TIMING_VESA_1600X1200_60     },
    {HI_DRV_DISP_FMT_VESA_1680X1050_60,         HDMI_VIDEO_TIMING_VESA_1680X1050_60     },
    {HI_DRV_DISP_FMT_VESA_1680X1050_60_RB,      HDMI_VIDEO_TIMING_VESA_1680X1050_60_RB  },
    {HI_DRV_DISP_FMT_VESA_1920X1200_60,         HDMI_VIDEO_TIMING_VESA_1920X1200_60     },
    {HI_DRV_DISP_FMT_VESA_1920X1440_60,         HDMI_VIDEO_TIMING_VESA_1920X1440_60     },
    {HI_DRV_DISP_FMT_VESA_2560X1600_60_RB,      HDMI_VIDEO_TIMING_VESA_2560x1600_60     },
    {HI_DRV_DISP_FMT_VESA_1280X720_60,          HDMI_VIDEO_TIMING_USER_1280X720_60      },
    {HI_DRV_DISP_FMT_VESA_1366X768_60,          HDMI_VIDEO_TIMING_USER_1366X768_60      },
    {HI_DRV_DISP_FMT_VESA_1600X900_60_RB,       HDMI_VIDEO_TIMING_USER_1600X900_60_RB   },
    {HI_DRV_DISP_FMT_VESA_1920X1080_60,         HDMI_VIDEO_TIMING_USER_1920X1080_60     },
    {HI_DRV_DISP_FMT_VESA_2048X1152_60,         HDMI_VIDEO_TIMING_USER_2048X1152_60     },
    {HI_DRV_DISP_FMT_VESA_2560X1440_60_RB,      HDMI_VIDEO_TIMING_USER_2560X1440_30     },
    {HI_DRV_DISP_FMT_CUSTOM,                    HDMI_VIDEO_TIMING_UNKNOWN               },
    {HI_DRV_DISP_FMT_BUTT,                      HDMI_VIDEO_TIMING_UNKNOWN               },
};

/************************************************************************/
/* private interface                                                    */
/************************************************************************/
#define HDMI_UNF_NULL_CHK(p) do{\
    if(HI_NULL==p){\
        HI_ERR_HDMI("pointer %s=null!",#p);\
        return HI_FAILURE;\
    }\
}while(0)

#define HDMI_UNF_NULL_CHK_NO_RET(p) do{\
    if(HI_NULL==p){\
        HI_ERR_HDMI("pointer %s=null!",#p);\
        return ;\
    }\
}while(0)

#define HDMI_KAUDIOCODING_2_UAUDIOCODING(kaudiocoding, uaudiocoding) do{\
    switch(kaudiocoding)\
    {\
        case HDMI_AUDIO_CODING_TYPE_STREAM:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_PCM:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_AC3:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_MPEG1:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_MP3:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_MPEG2:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_AAC_LC:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_DTS:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_ATRAC:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_DSD:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_EAC3:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_DTS_HD:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_MLP:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_DST:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DST;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_WMA_PRO:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_RESERVED:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT;\
            break;\
        case HDMI_AUDIO_CODING_TYPE_BUTT:\
        default:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT;\
            break;\
    }\
}while(0)

#define HDMI_UAUDIOCODING_2_KAUDIOCODING(uaudiocoding,kaudiocoding) do{\
switch(uaudiocoding)\
{\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_STREAM;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_PCM;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_AC3;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_MPEG1;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_MP3;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2 :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_MPEG2;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_AAC_LC;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_DTS;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_ATRAC;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_DSD;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_EAC3;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_DTS_HD;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_MLP;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_DST :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_DST;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_WMA_PRO;\
        break;\
    case HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT :\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_RESERVED;\
        break;\
    default:\
        kaudiocoding=HDMI_AUDIO_CODING_TYPE_BUTT;\
        break;\
}\
}while(0)


#define HDMI_URGBQUANTIZATION_2_KQUANTIZATION(urgbquantization, krgbquantization) \
do{\
    if (HDMI_RGB_QUANTIZATION_DEFAULT_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_DEFAULT;\
    }\
    else if (HDMI_RGB_QUANTIZATION_LIMITED_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_LIMITED;\
    }\
    else if (HDMI_RGB_QUANTIZATION_FULL_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_FULL;\
    }\
    else\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_BUTT;\
    }\
}while(0)

#define HDMI_KQUANTIZATION_2_URGBQUANTIZATION(krgbquantization, urgbquantization) \
do{\
    if (HDMI_QUANTIZATION_RANGE_DEFAULT == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE ;\
    }\
    else if (HDMI_QUANTIZATION_RANGE_LIMITED == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_LIMITED_RANGE;\
    }\
    else if (HDMI_QUANTIZATION_RANGE_FULL == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_FULL_RANGE;\
    }\
    else\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;\
    }\
}while(0)

#define HDMI_UYCCQUANTIZATION_2_KYCCQUANTIZATION(uyccquantization, kyccquantization) \
do{\
    if (HDMI_YCC_QUANTIZATION_LIMITED_RANGE == uyccquantization)\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;\
    }\
    else if (HDMI_YCC_QUANTIZATION_FULL_RANGE == uyccquantization)\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;\
    }\
    else\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_BUTT;\
    }\
}while(0)

#define HDMI_KYCCQUANTIZATION_2_UYCCQUANTIZATION(kyccquantization, uyccquantization) \
do{\
    if (HDMI_YCC_QUANTIZATION_RANGE_LIMITED == kyccquantization)\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;\
    }\
    else if (HDMI_YCC_QUANTIZATION_RANGE_FULL == kyccquantization)\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_FULL_RANGE;\
    }\
    else\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;\
    }\
}while(0)

static HI_U32 Hdmi_Etablish2Format(HI_U32 u32EtablishCode)
{
    HI_U32 i = 0;
    HDMI_ESTAB_FORMAT_S *pstEstabTiming = HI_NULL;

    for (i=0; i<HDMI_ARRAY_SIZE(s_g_stFmtEstabTable); i++)
    {
        pstEstabTiming = (HDMI_ESTAB_FORMAT_S *)&s_g_stFmtEstabTable[i];
        if ((pstEstabTiming != HI_NULL) &&
            (pstEstabTiming->u32EtablishCode == u32EtablishCode))
        {
            return pstEstabTiming->u32Format;
        }
    }

    return HI_UNF_ENC_FMT_BUTT;
}

static HI_U32 Hdmi_Vic2Format(HI_U32 u32Vic)
{
    HI_U32 i = 0;
    HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

    for (i=0; i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable); i++)
    {
        pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
        if ((pstFmtParam != HI_NULL) &&
            (pstFmtParam->u32Vic == u32Vic))
        {
            return pstFmtParam->u32Format;
        }
    }

    return HI_UNF_ENC_FMT_BUTT;
}

static HI_U32 Hdmi_StdTiming2Format(HDMI_EDID_STD_TIMING_S *pstStdTiming)
{
    HI_U32 i = 0;
    HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

    if (HI_NULL == pstStdTiming)
    {
        return HI_UNF_ENC_FMT_BUTT;
    }

    for (i=0; i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable); i++)
    {

        pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
        if ((pstFmtParam != HI_NULL) &&
            (pstFmtParam->u32HorActive == pstStdTiming->u32HorActive) &&
            (pstFmtParam->u32VerActive == pstStdTiming->u32VerActive) &&
            (pstFmtParam->u32FieldRate >= pstStdTiming->u32RefreshRate))
        {
            return pstFmtParam->u32Format;
        }
    }

    return HI_UNF_ENC_FMT_BUTT;
}

static HI_U32 Hdmi_PreTiming2Format(HDMI_EDID_PRE_TIMING_S *pstPreTiming)
{
    HI_U32 i = 0;
    HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

    if (HI_NULL == pstPreTiming)
    {
        return HI_UNF_ENC_FMT_BUTT;
    }

    for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable);i++)
    {
        pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
        if ((pstFmtParam != HI_NULL) &&
            (pstFmtParam->u32HorActive == pstPreTiming->u32HACT) &&
            (pstFmtParam->u32VerActive == pstPreTiming->u32VACT) &&
            (pstFmtParam->u32PixlFreq >= pstPreTiming->u32PixelClk))
        {
            return pstFmtParam->u32Format;
        }
    }

    return HI_UNF_ENC_FMT_BUTT;
}

static HI_VOID Hdmi_EtablishTimingParse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

    HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
    HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i = 0; i < pstDrvCap->u32EstabNum; i++)
    {
        enTmpEncFmt = Hdmi_Etablish2Format(pstDrvCap->au32EstabTiming[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportFormat[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}

static HI_VOID Hdmi_StdTimingParse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

	HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
	HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i = 0; i < HDMI_EDID_MAX_STDTIMNG_COUNT; i++)
    {
        enTmpEncFmt = Hdmi_StdTiming2Format(&pstDrvCap->stStdTiming[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportFormat[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}

static HI_VOID Hdmi_PerferTimingParse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

    HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
    HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i = 0; i < pstDrvCap->u32PerferTimingNum; i++)
    {
        enTmpEncFmt = Hdmi_PreTiming2Format(&pstDrvCap->stPerferTiming[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportFormat[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}

static HI_VOID Hdmi_SupportVicParse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

    HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
    HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i=0; i<pstDrvCap->u32SupportVICNum; i++)
    {
        enTmpEncFmt = Hdmi_Vic2Format(pstDrvCap->au32SupportFormat[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportFormat[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}

static HI_VOID Hdmi_OnlySupportY420Parse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

    HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
    HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i = 0; i < pstDrvCap->u32OnlySupportY420VICNum; i++)
    {
        enTmpEncFmt = Hdmi_Vic2Format(pstDrvCap->au32OnlySupportY420Format[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportY420Format[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}

static HI_VOID Hdmi_AlsoSupportY420Parse(HDMI_SINK_CAPABILITY_S *pstDrvCap, HI_UNF_EDID_BASE_INFO_S *pstUserCap)
{
    HI_UNF_ENC_FMT_E enTmpEncFmt = HI_UNF_ENC_FMT_BUTT;
    HI_U32 i = 0;

    HDMI_UNF_NULL_CHK_NO_RET(pstDrvCap);
    HDMI_UNF_NULL_CHK_NO_RET(pstUserCap);

    for (i=0; i<pstDrvCap->u32SupportY420VICNum; i++)
    {
        enTmpEncFmt = Hdmi_Vic2Format(pstDrvCap->au32SupportY420Format[i]);
        if (HI_UNF_ENC_FMT_BUTT != enTmpEncFmt)
        {
            pstUserCap->bSupportY420Format[enTmpEncFmt] = HI_TRUE;
        }
    }

    return;
}


static HI_S32 Hdmi_CapabilityDrv2User(HI_UNF_EDID_BASE_INFO_S *pstUserCap,
                                      HDMI_SINK_CAPABILITY_S *pstDrvCap)
{
    HDMI_UNF_NULL_CHK(pstUserCap);
    HDMI_UNF_NULL_CHK(pstDrvCap);

    pstUserCap->bSupportHdmi    = pstDrvCap->bSupportHdmi;
    pstUserCap->enNativeFormat  = Hdmi_Vic2Format(pstDrvCap->u32NativeFormat);

    memset(pstUserCap->bSupportFormat, HI_FALSE, sizeof(pstUserCap->bSupportFormat));

    Hdmi_EtablishTimingParse(pstDrvCap, pstUserCap);
    Hdmi_StdTimingParse(pstDrvCap, pstUserCap);
    Hdmi_PerferTimingParse(pstDrvCap, pstUserCap);
    Hdmi_SupportVicParse(pstDrvCap, pstUserCap);

    memcpy(&pstUserCap->st3DInfo, &pstDrvCap->st3DInfo, sizeof(pstUserCap->st3DInfo));
    memcpy(&pstUserCap->stDeepColor, &pstDrvCap->stDeepColor, sizeof(pstUserCap->stDeepColor));
    memcpy(&pstUserCap->stColorMetry, &pstDrvCap->stColorMetry, sizeof(pstUserCap->stColorMetry));
    memcpy(&pstUserCap->stColorSpace, &pstDrvCap->stColorSpace, sizeof(pstUserCap->stColorSpace));
    memcpy(&pstUserCap->stAudioInfo, &pstDrvCap->stAudioInfo, sizeof(pstUserCap->stAudioInfo));

    pstUserCap->u32AudioInfoNum = pstDrvCap->u32AudioInfoNum;

    memcpy(&pstUserCap->bSupportAudioSpeaker, &pstDrvCap->bSupportAudioSpeaker, sizeof(pstUserCap->bSupportAudioSpeaker));

    pstUserCap->u8ExtBlockNum = pstDrvCap->u8ExtBlockNum;
    pstUserCap->u8Version     = pstDrvCap->u8Version;
    pstUserCap->u8Revision    = pstDrvCap->u8Revision;

    //memcpy(&pstUserCap->stMfrsInfo, &pstDrvCap->stMfrsInfo, sizeof(pstUserCap->stMfrsInfo));
    pstUserCap->stMfrsInfo.u8MfrsName[0]     = pstDrvCap->stMfrsInfo.u8MfrsName[0];
    pstUserCap->stMfrsInfo.u8MfrsName[1]     = pstDrvCap->stMfrsInfo.u8MfrsName[1];
    pstUserCap->stMfrsInfo.u8MfrsName[2]     = pstDrvCap->stMfrsInfo.u8MfrsName[2];
    pstUserCap->stMfrsInfo.u8MfrsName[3]     = pstDrvCap->stMfrsInfo.u8MfrsName[3];
    pstUserCap->stMfrsInfo.u32ProductCode    = pstDrvCap->stMfrsInfo.u32ProductCode ;
    pstUserCap->stMfrsInfo.u32SerialNumber   = pstDrvCap->stMfrsInfo.u32SerialNumber ;
    pstUserCap->stMfrsInfo.u32Week           = pstDrvCap->stMfrsInfo.u32Week ;
    pstUserCap->stMfrsInfo.u32Year           = pstDrvCap->stMfrsInfo.u32Year ;
    memcpy(pstUserCap->stMfrsInfo.u8pSinkName, pstDrvCap->stMfrsInfo.u8pSinkName, sizeof(pstUserCap->stMfrsInfo.u8pSinkName));
    memcpy(&pstUserCap->stCECAddr, &pstDrvCap->stCECAddr, sizeof(pstUserCap->stCECAddr));

    pstUserCap->bSupportDVIDual = pstDrvCap->bSupportDVIDual;
    pstUserCap->bSupportsAI     = pstDrvCap->bSupportsAI;

    pstUserCap->u32MaxTMDSClock                     = pstDrvCap->u32MaxTMDSClock;
    pstUserCap->stY420DeepColor.bY420DeepColor30Bit = pstDrvCap->stDeepColorY420.bDeepColor30Bit;
    pstUserCap->stY420DeepColor.bY420DeepColor36Bit = pstDrvCap->stDeepColorY420.bDeepColor36Bit;
    pstUserCap->stY420DeepColor.bY420DeepColor48Bit = pstDrvCap->stDeepColorY420.bDeepColor48Bit;

    Hdmi_OnlySupportY420Parse(pstDrvCap, pstUserCap);
    Hdmi_AlsoSupportY420Parse(pstDrvCap, pstUserCap);

    memcpy(&pstUserCap->stPerferTiming, &pstDrvCap->stPerferTiming[0], sizeof(pstUserCap->stPerferTiming));

    pstUserCap->stHDCPSupport.bHdcp14Support = pstDrvCap->stHdcpSupport.bHdcp14Support;
    pstUserCap->stHDCPSupport.bHdcp22Support = pstDrvCap->stHdcpSupport.bHdcp22Support;

    pstUserCap->stBaseDispPara.u8MaxImageWidth  = pstDrvCap->stBasicDispPara.u8MaxImageWidth;
    pstUserCap->stBaseDispPara.u8MaxImageHeight = pstDrvCap->stBasicDispPara.u8MaxImageHeight;

#ifdef HDMI_HDR_SUPPORT

        if( pstDrvCap->stHdrCap.stEotf.bEotfSdr == HI_TRUE ||
            pstDrvCap->stHdrCap.stEotf.bEotfHdr == HI_TRUE ||
            pstDrvCap->stHdrCap.stEotf.bEotfSmpteSt2084 == HI_TRUE)
        {
            pstUserCap->bHdrSupport = HI_TRUE;
        }
        else
        {
            pstUserCap->bHdrSupport = HI_FALSE;
        }

        pstUserCap->bDolbySupport = (pstDrvCap->stDolbyCap.u32DolbyOui == DOLBY_IEEE_OUI) ? HI_TRUE : HI_FALSE;

        memcpy(&(pstUserCap->stHdr), &(pstDrvCap->stHdrCap), sizeof(HI_UNF_EDID_HDR_CAP_S));
        memcpy(&(pstUserCap->stDolby), &(pstDrvCap->stDolbyCap), sizeof(HI_UNF_EDID_DOLBY_CAP_S));

#endif

    return HI_SUCCESS;
}

static HI_VOID Hdmi_AttrUser2Drv(HDMI_APP_ATTR_S *pstDrvAttr, HI_UNF_HDMI_ATTR_S *pstUserAttr)
{
    if ((HI_NULL == pstDrvAttr) || (HI_NULL == pstUserAttr))
    {
        HI_ERR_HDMI("pstDrvAttr %p, pstUserAttr %p is invalid\n", pstDrvAttr, pstUserAttr);
        return;
    }

    pstDrvAttr->bEnableHdmi          = pstUserAttr->bEnableHdmi;
    pstDrvAttr->bEnableVideo         = pstUserAttr->bEnableVideo;
    pstDrvAttr->bEnableAudio         = pstUserAttr->bEnableAudio;
    pstDrvAttr->enOutColorSpace      = pstUserAttr->enVidOutMode;
    HDMI_UDEEPCOLOR_2_KDEEPCOLOR(pstUserAttr->enDeepColorMode, pstDrvAttr->enDeepColorMode);
    pstDrvAttr->bxvYCCMode           = pstUserAttr->bxvYCCMode;
    pstDrvAttr->bEnableAviInfoFrame  = pstUserAttr->bEnableAviInfoFrame;
    pstDrvAttr->bEnableAudInfoFrame  = pstUserAttr->bEnableAudInfoFrame;
    pstDrvAttr->bEnableSpdInfoFrame  = pstUserAttr->bEnableSpdInfoFrame;
    pstDrvAttr->bEnableMpegInfoFrame = pstUserAttr->bEnableMpegInfoFrame;
    pstDrvAttr->bHDCPEnable          = pstUserAttr->bHDCPEnable;
    HDMI_UHDCPMODE_2_KHDCPMODE(pstUserAttr->enHDCPMode, pstDrvAttr->enHDCPMode);
    pstDrvAttr->bEnableClrSpaceAdapt = pstUserAttr->bEnableVidModeAdapt;
    pstDrvAttr->bEnableDeepClrAdapt  = pstUserAttr->bEnableDeepClrAdapt;
    pstDrvAttr->bAuthMode            = pstUserAttr->bAuthMode;

    return ;
}

static HI_VOID Hdmi_AttrDrv2User(HI_UNF_HDMI_ATTR_S *pstUserAttr, HDMI_APP_ATTR_S *pstDrvAttr)
{
    if ((HI_NULL == pstDrvAttr) || (HI_NULL == pstUserAttr))
    {
        HI_ERR_HDMI("pstDrvAttr %p, pstUserAttr %p is invalid\n", pstDrvAttr, pstUserAttr);
        return;
    }

    pstUserAttr->bEnableHdmi          = pstDrvAttr->bEnableHdmi;
    pstUserAttr->bEnableVideo         = pstDrvAttr->bEnableVideo;
    pstUserAttr->bEnableAudio         = pstDrvAttr->bEnableAudio;

    pstUserAttr->enVidOutMode         = pstDrvAttr->enOutColorSpace;
    HDMI_KDEEPCOLOR_2_UDEEPCOLOR(pstDrvAttr->enDeepColorMode, pstUserAttr->enDeepColorMode);
    pstUserAttr->bxvYCCMode           = pstDrvAttr->bxvYCCMode;
    pstUserAttr->bEnableAviInfoFrame  = pstDrvAttr->bEnableAviInfoFrame;
    pstUserAttr->bEnableSpdInfoFrame  = pstDrvAttr->bEnableSpdInfoFrame;
    pstUserAttr->bEnableMpegInfoFrame = pstDrvAttr->bEnableMpegInfoFrame;
    pstUserAttr->bEnableAudInfoFrame  = pstDrvAttr->bEnableAudInfoFrame;
    pstUserAttr->bHDCPEnable          = pstDrvAttr->bHDCPEnable;
    HDMI_KHDCPMODE_2_UHDCPMODE(pstDrvAttr->enHDCPMode, pstUserAttr->enHDCPMode);
    pstUserAttr->bEnableDeepClrAdapt  = pstDrvAttr->bEnableDeepClrAdapt;
    pstUserAttr->bEnableVidModeAdapt  = pstDrvAttr->bEnableClrSpaceAdapt;
    pstUserAttr->bAuthMode            = pstDrvAttr->bAuthMode;

    return ;
}

static HI_VOID Hdmi_ScanModeDrv2UserConv(HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);

    switch(pstDrvAVIInfoFrame->scan_mode)
    {
        case HDMI_SCAN_MODE_NONE :
            pstUnfAVIInfoFrame->enScanInfo = HDMI_SCAN_INFO_NO_DATA;
            break;
        case HDMI_SCAN_MODE_OVERSCAN :
            pstUnfAVIInfoFrame->enScanInfo = HDMI_SCAN_INFO_OVERSCANNED;
            break;
        case HDMI_SCAN_MODE_UNDERSCAN :
            pstUnfAVIInfoFrame->enScanInfo = HDMI_SCAN_INFO_UNDERSCANNED;
            break;
        case HDMI_SCAN_MODE_FUTURE :
            pstUnfAVIInfoFrame->enScanInfo = HDMI_SCAN_INFO_FUTURE;
            break;
        default:
            break;
    }

    return;
}

static HI_VOID Hdmi_AspectDrv2UserConv(HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);

    switch(pstDrvAVIInfoFrame->picture_aspect)
    {
        case HDMI_PICTURE_ASPECT_NONE :
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_NO_DATA;
            break;

        case HDMI_PICTURE_ASPECT_4_3 :
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
            break;

        case HDMI_PICTURE_ASPECT_16_9 :
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
            break;
        case HDMI_PICTURE_ASPECT_64_27 :
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_64TO27;
            break;
        case HDMI_PICTURE_ASPECT_256_135:
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_256TO135;
            break;
        case HDMI_PICTURE_ASPECT_FUTURE :
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_FUTURE;
            break;

        case HDMI_PICTURE_ASPECT_BUTT:
        default:
            pstUnfAVIInfoFrame->enAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_BUTT ;
            break;
    }

    return;
}

static HI_VOID Hdmi_ActiveAspectDrv2UserConv(HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);

    switch(pstDrvAVIInfoFrame->active_aspect)
    {
        case HDMI_ACTIVE_ASPECT_PICTURE:
            pstUnfAVIInfoFrame->enActiveAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_NO_DATA;
            break;

        case HDMI_ACTIVE_ASPECT_4_3:
        case HDMI_ACTIVE_ASPECT_4_3_SP_14_9:
            pstUnfAVIInfoFrame->enActiveAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
            break;

        case HDMI_ACTIVE_ASPECT_16_9_TOP:
        case HDMI_ACTIVE_ASPECT_16_9_CENTER:
        case HDMI_ACTIVE_ASPECT_16_9:
        case HDMI_ACTIVE_ASPECT_16_9_SP_14_9:
        case HDMI_ACTIVE_ASPECT_16_9_SP_4_3:
            pstUnfAVIInfoFrame->enActiveAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
            break;

        case HDMI_ACTIVE_ASPECT_14_9_TOP:
        case HDMI_ACTIVE_ASPECT_14_9:
        default:
            pstUnfAVIInfoFrame->enActiveAspectRatio = HI_UNF_HDMI_ASPECT_RATIO_FUTURE;
            break;
    }

    return;
}

static HI_VOID Hdmi_ColorimetryDrv2UserConv(HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);

    if (pstDrvAVIInfoFrame->colorimetry >= HDMI_COLORIMETRY__EXTENDED)
    {
        pstUnfAVIInfoFrame->enColorimetry = pstDrvAVIInfoFrame->extended_colorimetry + HDMI_COLORIMETRY_EXTENDED + 1;
    }
    else
    {
        switch(pstDrvAVIInfoFrame->colorimetry)
        {
            case HDMI_COLORIMETRY__NONE:
                pstUnfAVIInfoFrame->enColorimetry = HDMI_COLORIMETRY_NO_DATA;
                break;
            case HDMI_COLORIMETRY__ITU_601:
                pstUnfAVIInfoFrame->enColorimetry = HDMI_COLORIMETRY_ITU601;
                break;
            case HDMI_COLORIMETRY__ITU_709:
                pstUnfAVIInfoFrame->enColorimetry = HDMI_COLORIMETRY_ITU709;
                break;
            default:
                break;
        }
    }

    return;
}

static HI_VOID Hdmi_AudioSampleSizeDrv2UserConv(HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame, HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAUDInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAUDInfoFrame);

    switch (pstDrvAUDInfoFrame->sample_size)
    {
        case HDMI_AUDIO_SAMPLE_SIZE_16:
            pstUnfAUDInfoFrame->u32SampleSize = 16;
            break;
        case HDMI_AUDIO_SAMPLE_SIZE_20:
            pstUnfAUDInfoFrame->u32SampleSize = 20;
            break;
        case HDMI_AUDIO_SAMPLE_SIZE_24:
            pstUnfAUDInfoFrame->u32SampleSize = 24;
            break;
        default:
            break;
    }

    return;
}

static HI_VOID Hdmi_AudioSampleFreqDrv2UserConv(HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame, HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAUDInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAUDInfoFrame);

    switch (pstDrvAUDInfoFrame->sample_frequency)
    {
        case HDMI_AUDIO_SAMPLE_FREQUENCY_32000:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 32000;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_44100:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 44100;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_48000:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 48000;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_88200:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 88200;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_96000:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 96000;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_176400:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 176400;
            break;
        case HDMI_AUDIO_SAMPLE_FREQUENCY_192000:
            pstUnfAUDInfoFrame->u32SamplingFrequency = 192000;
            break;
        default:
            break;
    }

    return;
}

static HI_S32 Hdmi_InfoFrameDrv2User(HI_UNF_HDMI_INFOFRAME_S *pstDestInfoFrame, DRV_HDMI_INFOFRAME_S *pstSrcInfoFrame)
{
    HDMI_UNF_NULL_CHK(pstDestInfoFrame);
    HDMI_UNF_NULL_CHK(pstSrcInfoFrame);

    if ((HI_NULL == pstDestInfoFrame) || (HI_NULL == pstSrcInfoFrame))
    {
        HI_ERR_HDMI("pstDestInfoFrame %p, pstSrcInfoFrame %p is invalid\n", pstDestInfoFrame, pstSrcInfoFrame);
        return HI_FAILURE;
    }

    switch (pstSrcInfoFrame->enInfoFrameId)
    {
        case HDMI_INFOFRAME_TYPE_AVI:
        {
            HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame = &pstDestInfoFrame->unInforUnit.stAVIInfoFrame;
            HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame = &pstSrcInfoFrame->uInfoFrame.stAVIInfoFrame;

            pstDestInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AVI;
            pstUnfAVIInfoFrame->enOutputType = pstDrvAVIInfoFrame->colorspace;
            pstUnfAVIInfoFrame->bActive_Infor_Present = pstDrvAVIInfoFrame->active_info_valid;

            if (pstDrvAVIInfoFrame->horizontal_bar_valid &&
                pstDrvAVIInfoFrame->vertical_bar_valid)
            {
                pstUnfAVIInfoFrame->enBarInfo = HDMI_BAR_INFO_VH;
            }
            else if(pstDrvAVIInfoFrame->horizontal_bar_valid)
            {
                pstUnfAVIInfoFrame->enBarInfo = HDMI_BAR_INFO_H;
            }
            else if (pstDrvAVIInfoFrame->vertical_bar_valid)
            {
                pstUnfAVIInfoFrame->enBarInfo = HDMI_BAR_INFO_V;
            }

            Hdmi_ScanModeDrv2UserConv(pstDrvAVIInfoFrame, pstUnfAVIInfoFrame);
            Hdmi_AspectDrv2UserConv(pstDrvAVIInfoFrame, pstUnfAVIInfoFrame);

            /* !!UNF enActiveAspectRatio type defined is  not proper,so the conver is not accurate. */
            Hdmi_ActiveAspectDrv2UserConv(pstDrvAVIInfoFrame, pstUnfAVIInfoFrame);

            pstUnfAVIInfoFrame->enPictureScaling = pstDrvAVIInfoFrame->nups;
            pstUnfAVIInfoFrame->bIsITContent = pstDrvAVIInfoFrame->itc;
            pstUnfAVIInfoFrame->u32PixelRepetition = pstDrvAVIInfoFrame->pixel_repeat;
            pstUnfAVIInfoFrame->enContentType = pstDrvAVIInfoFrame->content_type;
            pstUnfAVIInfoFrame->u32LineNEndofTopBar = pstDrvAVIInfoFrame->top_bar;
            pstUnfAVIInfoFrame->u32LineNStartofBotBar = pstDrvAVIInfoFrame->bottom_bar;
            pstUnfAVIInfoFrame->u32PixelNEndofLeftBar = pstDrvAVIInfoFrame->left_bar;
            pstUnfAVIInfoFrame->u32PixelNStartofRightBar = pstDrvAVIInfoFrame->right_bar;
            HDMI_KQUANTIZATION_2_URGBQUANTIZATION(pstDrvAVIInfoFrame->quantization_range, pstUnfAVIInfoFrame->enRGBQuantization);
            HDMI_KYCCQUANTIZATION_2_UYCCQUANTIZATION(pstDrvAVIInfoFrame->ycc_quantization_range, pstUnfAVIInfoFrame->enYCCQuantization);

            Hdmi_ColorimetryDrv2UserConv(pstDrvAVIInfoFrame, pstUnfAVIInfoFrame);

            pstUnfAVIInfoFrame->enTimingMode = pstDrvAVIInfoFrame->disp_fmt;
            break;
        }
        case HDMI_INFOFRAME_TYPE_AUDIO:
        {
            HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame = &pstDestInfoFrame->unInforUnit.stAUDInfoFrame;
            HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame = &pstSrcInfoFrame->uInfoFrame.stAudioInfoFrame;
            pstDestInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
            pstUnfAUDInfoFrame->u32ChannelCount = pstDrvAUDInfoFrame->channels;
            HDMI_KAUDIOCODING_2_UAUDIOCODING(pstDrvAUDInfoFrame->coding_type, pstUnfAUDInfoFrame->enCodingType);
            pstUnfAUDInfoFrame->u32ChannelAlloc = pstDrvAUDInfoFrame->channel_allocation;
            pstUnfAUDInfoFrame->u32LevelShift   = pstDrvAUDInfoFrame->level_shift_value;
            pstUnfAUDInfoFrame->u32DownmixInhibit = pstDrvAUDInfoFrame->downmix_inhibit;

            Hdmi_AudioSampleSizeDrv2UserConv(pstDrvAUDInfoFrame, pstUnfAUDInfoFrame);
            Hdmi_AudioSampleFreqDrv2UserConv(pstDrvAUDInfoFrame, pstUnfAUDInfoFrame);

            break;
        }
        case HDMI_INFOFRAME_TYPE_SPD:
            pstDestInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_SPD;
            break;
        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HDMI_VIDEO_TIMING_E Hdmi_Fmt2HdmiTiming(HI_U32 u32Fmt)
{
    HI_U32 i = 0;
    HDMI_VIDEO_TIMING_E VideoTimingMode = HDMI_VIDEO_TIMING_UNKNOWN;

	for(i = 0; i < (sizeof(g_enUnfFmtToTiming) / sizeof(g_enUnfFmtToTiming[0])); i++)
    {
        if(u32Fmt == g_enUnfFmtToTiming[i].enFmt)
        {
            VideoTimingMode = g_enUnfFmtToTiming[i].enTiming;
        }
    }

    return VideoTimingMode;
}

#define HDMI_USCANMODE_2_KSCANMODE(uscanmode, kscanmode) \
do{\
    if (HDMI_SCAN_INFO_NO_DATA == uscanmode)\
    {\
        kscanmode = HDMI_SCAN_MODE_NONE;\
    }\
    else if (HDMI_SCAN_INFO_OVERSCANNED == uscanmode)\
    {\
        kscanmode = HDMI_SCAN_MODE_OVERSCAN;\
    }\
    else if (HDMI_SCAN_INFO_UNDERSCANNED == uscanmode)\
    {\
        kscanmode = HDMI_SCAN_MODE_UNDERSCAN;\
    }\
    else\
    {\
        kscanmode = HDMI_SCAN_MODE_FUTURE;\
    }\
}while(0)

#define HDMI_KSCANMODE_2_USCANMODE(kscanmode, uscanmode) \
do{\
    if (HDMI_SCAN_MODE_NONE == kscanmode)\
    {\
        uscanmode = HDMI_SCAN_INFO_NO_DATA;\
    }\
    else if (HDMI_SCAN_MODE_OVERSCAN == kscanmode)\
    {\
        uscanmode = HDMI_SCAN_INFO_OVERSCANNED;\
    }\
    else if (HDMI_SCAN_MODE_UNDERSCAN == kscanmode)\
    {\
        uscanmode = HDMI_SCAN_INFO_UNDERSCANNED;\
    }\
    else\
    {\
        uscanmode = HDMI_SCAN_INFO_FUTURE;\
    }\
}while(0)

#define HDMI_UASPECT_2_KASPECT(uaspect, kaspect) \
do{\
    if (HI_UNF_HDMI_ASPECT_RATIO_NO_DATA == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_NONE;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_4TO3 == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_4_3;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_16TO9 == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_16_9;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_64TO27 == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_64_27;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_256TO135 == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_256_135;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_FUTURE == uaspect)\
    {\
        kaspect = HDMI_PICTURE_ASPECT_FUTURE;\
    }\
    else\
    {\
        kaspect = HDMI_PICTURE_ASPECT_BUTT;\
    }\
}while(0)

#define HDMI_UACTIVEASPECT_2_KACTIVEASPECT(uactaspect, kactaspect) \
do{\
    if (HI_UNF_HDMI_ASPECT_RATIO_NO_DATA == uactaspect)\
    {\
        kactaspect = HDMI_ACTIVE_ASPECT_PICTURE;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_4TO3 == uactaspect)\
    {\
        kactaspect = HDMI_ACTIVE_ASPECT_4_3;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_16TO9 == uactaspect)\
    {\
        kactaspect = HDMI_ACTIVE_ASPECT_16_9;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_64TO27 == uactaspect)\
    {\
        kactaspect = HDMI_PICTURE_ASPECT_64_27;\
    }\
    else if (HI_UNF_HDMI_ASPECT_RATIO_256TO135 == uactaspect)\
    {\
        kactaspect = HDMI_PICTURE_ASPECT_256_135;\
    }\
    else\
    {\
        kactaspect = HDMI_ACTIVE_ASPECT_14_9;\
    }\
}while(0)

static HI_VOID Hdmi_BarInfoUser2DrvConv(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame, HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);

    switch (pstUnfAVIInfoFrame->enBarInfo)
    {
        case HDMI_BAR_INFO_VH:
            pstDrvAVIInfoFrame->horizontal_bar_valid = HI_TRUE;
            pstDrvAVIInfoFrame->vertical_bar_valid = HI_TRUE;
            break;
        case HDMI_BAR_INFO_V:
            pstDrvAVIInfoFrame->vertical_bar_valid = HI_TRUE;
            break;
        case HDMI_BAR_INFO_H:
            pstDrvAVIInfoFrame->horizontal_bar_valid = HI_TRUE;
            break;
        default:
            break;
    }

    return;
}

static HI_VOID Hdmi_ColorimetryUser2DrvConv(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame, HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAVIInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAVIInfoFrame);

    if (pstUnfAVIInfoFrame->enColorimetry >= HDMI_COLORIMETRY_EXTENDED)
    {
        pstDrvAVIInfoFrame->colorimetry = HDMI_COLORIMETRY__EXTENDED;
        pstDrvAVIInfoFrame->extended_colorimetry = pstUnfAVIInfoFrame->enColorimetry - HDMI_COLORIMETRY_EXTENDED - 1;
    }
    else
    {
        switch(pstUnfAVIInfoFrame->enColorimetry)
        {
            case HDMI_COLORIMETRY_NO_DATA:
                pstDrvAVIInfoFrame->colorimetry = HDMI_COLORIMETRY__NONE;
                break;
            case HDMI_COLORIMETRY_ITU601:
                pstDrvAVIInfoFrame->colorimetry = HDMI_COLORIMETRY__ITU_601;
                break;
            case HDMI_COLORIMETRY_ITU709:
                pstDrvAVIInfoFrame->colorimetry = HDMI_COLORIMETRY__ITU_709;
                break;
            default:
                break;
        }
    }

    return;
}

static HI_VOID Hdmi_AudioSampSizeUser2DrvConv(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame, HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAUDInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAUDInfoFrame);

    switch (pstUnfAUDInfoFrame->u32SampleSize)
    {
        case 16:
            pstDrvAUDInfoFrame->sample_size = HDMI_AUDIO_SAMPLE_SIZE_16;
            break;
        case 20:
            pstDrvAUDInfoFrame->sample_size = HDMI_AUDIO_SAMPLE_SIZE_20;
            break;
        case 24:
            pstDrvAUDInfoFrame->sample_size = HDMI_AUDIO_SAMPLE_SIZE_24;
            break;
        default:
            break;
    }

    return;
}

static HI_VOID Hdmi_AudioSampFreqUser2DrvConv(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame, HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame)
{
    HDMI_UNF_NULL_CHK_NO_RET(pstUnfAUDInfoFrame);
    HDMI_UNF_NULL_CHK_NO_RET(pstDrvAUDInfoFrame);

    switch (pstUnfAUDInfoFrame->u32SamplingFrequency)
    {
        case 32000:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_32000;
            break;
        case 44100:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_44100;
            break;
        case 48000:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_48000;
            break;
        case 88200:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_88200;
            break;
        case 96000:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_96000;
            break;
        case 176400:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_176400;
            break;
        case 192000:
            pstDrvAUDInfoFrame->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_192000;
            break;
        default:
            break;
    }

    return;
}


static HI_S32 Hdmi_InfoFrameUser2Drv(DRV_HDMI_INFOFRAME_S *pstDestInfoFrame, HI_UNF_HDMI_INFOFRAME_S *pstSrcInfoFrame)
{
    if ((HI_NULL == pstDestInfoFrame) || (HI_NULL == pstSrcInfoFrame))
    {
        HI_ERR_HDMI("pstDestInfoFrame %p, pstSrcInfoFrame %p is invalid.\n", pstDestInfoFrame, pstSrcInfoFrame);
        return HI_FAILURE;
    }

    switch (pstSrcInfoFrame->enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstUnfAVIInfoFrame = &pstSrcInfoFrame->unInforUnit.stAVIInfoFrame;
            HDMI_AVI_INFOFRAME_S *pstDrvAVIInfoFrame = &pstDestInfoFrame->uInfoFrame.stAVIInfoFrame;
            pstDestInfoFrame->enInfoFrameId = HDMI_INFOFRAME_TYPE_AVI;
            pstDrvAVIInfoFrame->colorspace = pstUnfAVIInfoFrame->enOutputType;
            pstDrvAVIInfoFrame->active_info_valid = pstUnfAVIInfoFrame->bActive_Infor_Present;
            HDMI_USCANMODE_2_KSCANMODE(pstUnfAVIInfoFrame->enScanInfo, pstDrvAVIInfoFrame->scan_mode);
            HDMI_UASPECT_2_KASPECT(pstUnfAVIInfoFrame->enAspectRatio, pstDrvAVIInfoFrame->picture_aspect);
            /* !!UNF enActiveAspectRatio type defined is  not proper,so the conver is not accurate. */
            HDMI_UACTIVEASPECT_2_KACTIVEASPECT(pstUnfAVIInfoFrame->enActiveAspectRatio, pstUnfAVIInfoFrame->enActiveAspectRatio);

            pstDrvAVIInfoFrame->active_aspect = HDMI_ACTIVE_ASPECT_PICTURE;
            pstDrvAVIInfoFrame->nups = pstUnfAVIInfoFrame->enPictureScaling ;
            pstDrvAVIInfoFrame->itc = pstUnfAVIInfoFrame->bIsITContent;
            pstDrvAVIInfoFrame->pixel_repeat = pstUnfAVIInfoFrame->u32PixelRepetition;
            pstDrvAVIInfoFrame->content_type = pstUnfAVIInfoFrame->enContentType;
            pstDrvAVIInfoFrame->top_bar = pstUnfAVIInfoFrame->u32LineNEndofTopBar;
            pstDrvAVIInfoFrame->bottom_bar = pstUnfAVIInfoFrame->u32LineNStartofBotBar;
            pstDrvAVIInfoFrame->left_bar = pstUnfAVIInfoFrame->u32PixelNEndofLeftBar;
            pstDrvAVIInfoFrame->right_bar = pstUnfAVIInfoFrame->u32PixelNStartofRightBar;
            HDMI_URGBQUANTIZATION_2_KQUANTIZATION(pstUnfAVIInfoFrame->enRGBQuantization, pstDrvAVIInfoFrame->quantization_range);
            HDMI_UYCCQUANTIZATION_2_KYCCQUANTIZATION(pstUnfAVIInfoFrame->enYCCQuantization, pstDrvAVIInfoFrame->ycc_quantization_range);
            Hdmi_BarInfoUser2DrvConv(pstUnfAVIInfoFrame, pstDrvAVIInfoFrame);
            Hdmi_ColorimetryUser2DrvConv(pstUnfAVIInfoFrame, pstDrvAVIInfoFrame);

            pstDrvAVIInfoFrame->disp_fmt =  pstUnfAVIInfoFrame->enTimingMode;
            pstDrvAVIInfoFrame->video_timing = Hdmi_Fmt2HdmiTiming(pstUnfAVIInfoFrame->enTimingMode);

            break;
        }
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstUnfAUDInfoFrame = &pstSrcInfoFrame->unInforUnit.stAUDInfoFrame;
            HDMI_AUDIO_INFOFRAME_S *pstDrvAUDInfoFrame = &pstDestInfoFrame->uInfoFrame.stAudioInfoFrame;
            pstDestInfoFrame->enInfoFrameId = HDMI_INFOFRAME_TYPE_AUDIO;
            pstDrvAUDInfoFrame->channels = pstUnfAUDInfoFrame->u32ChannelCount;
            HDMI_UAUDIOCODING_2_KAUDIOCODING(pstUnfAUDInfoFrame->enCodingType, pstDrvAUDInfoFrame->coding_type);
            pstDrvAUDInfoFrame->channel_allocation = pstUnfAUDInfoFrame->u32ChannelAlloc;
            pstDrvAUDInfoFrame->level_shift_value = pstUnfAUDInfoFrame->u32LevelShift;
            pstDrvAUDInfoFrame->downmix_inhibit = pstUnfAUDInfoFrame->u32DownmixInhibit;

            Hdmi_AudioSampSizeUser2DrvConv(pstUnfAUDInfoFrame, pstDrvAUDInfoFrame);
            Hdmi_AudioSampFreqUser2DrvConv(pstUnfAUDInfoFrame, pstDrvAUDInfoFrame);

            break;
        }
        case HI_INFOFRAME_TYPE_SPD:
            pstDestInfoFrame->enInfoFrameId = HDMI_INFOFRAME_TYPE_SPD;
            break;
        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/************************************************************************/
/* public interface                                                     */
/************************************************************************/
HI_S32 HI_UNF_HDMI_Init(void)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComInit();

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_DeInit(void)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComDeInit();

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_OPEN_PARA_S *pstOpenPara)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComOpen(enHdmi, pstOpenPara);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComClose(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComGetStatus(enHdmi, pHdmiStatus);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pstSinkAttr)
{
    HI_S32 s32Ret;
    DRV_HDMI_SINK_CAPABILITY_S *pstSinkCap = HI_NULL;

    HDMI_UNF_NULL_CHK(pstSinkAttr);
    pstSinkCap = (DRV_HDMI_SINK_CAPABILITY_S*)HI_MALLOC(HI_ID_HDMI,sizeof(DRV_HDMI_SINK_CAPABILITY_S));
    HDMI_UNF_NULL_CHK(pstSinkCap);

    memset(pstSinkAttr, 0, sizeof(HI_UNF_EDID_BASE_INFO_S));
    s32Ret = HI_MPI_HDMI_ComGetSinkCapability(enHdmi, pstSinkCap);
    if (HI_SUCCESS == s32Ret)
    {
        Hdmi_CapabilityDrv2User(pstSinkAttr, &pstSinkCap->stCap);
    }
    HI_FREE(HI_ID_HDMI,pstSinkCap);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;
    DRV_HDMI_APP_ATTR_S stDrvAppAttr;

    HDMI_UNF_NULL_CHK(pstAttr);

    memset(&stDrvAppAttr, 0, sizeof(DRV_HDMI_APP_ATTR_S));
    Hdmi_AttrUser2Drv(&stDrvAppAttr.stAPPAttr, pstAttr);
    s32Ret = HI_MPI_HDMI_ComSetAttr(enHdmi, &stDrvAppAttr);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;
    DRV_HDMI_APP_ATTR_S stDrvAppAttr;

    HDMI_UNF_NULL_CHK(pstAttr);

    memset(&stDrvAppAttr, 0, sizeof(DRV_HDMI_APP_ATTR_S));
    s32Ret = HI_MPI_HDMI_ComGetAttr(enHdmi, &stDrvAppAttr);
    Hdmi_AttrDrv2User(pstAttr, &stDrvAppAttr.stAPPAttr);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_RegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComRegCECCallBackFunc(enHdmi, pCECCallback);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_UnRegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComUnRegCECCallBackFunc(enHdmi, pCECCallback);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pCECCmd)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComSetCECCommand(enHdmi, pCECCmd);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pCECCmd, HI_U32 timeout)
{
    HI_S32 s32Ret = HI_FAILURE;

    //s32Ret = HI_MPI_HDMI_ComGetCECCommand(enHdmi, pCECCmd);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S *pStatus)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComCECStatus(enHdmi, pStatus);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComCECEnable(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_CEC_Disable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComCECDisable(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret;
    DRV_HDMI_INFOFRAME_S stInfoFrame;

    HDMI_UNF_NULL_CHK(pstInfoFrame);
    memset(&stInfoFrame, 0, sizeof(DRV_HDMI_INFOFRAME_S));
    s32Ret = Hdmi_InfoFrameUser2Drv(&stInfoFrame, pstInfoFrame);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    stInfoFrame.enHdmiID = enHdmi;

    s32Ret = HI_MPI_HDMI_ComSetInfoFrame(enHdmi, &stInfoFrame);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType,
                                HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret;
    DRV_HDMI_INFOFRAME_S stInfoFrame;

    if ((HI_INFOFRAME_TYPE_BUTT <= enInfoFrameType) ||
        (HI_NULL == pstInfoFrame))
    {
        HI_ERR_HDMI("enInfoFrameType %d, pstInfoFrame %p is invalid\n", enInfoFrameType, pstInfoFrame);
        return HI_FAILURE;
    }

    memset(&stInfoFrame, 0, sizeof(DRV_HDMI_INFOFRAME_S));
    stInfoFrame.enHdmiID = enHdmi;
    stInfoFrame.enInfoFrameId = s_enDrvInfoFrameId[enInfoFrameType];
    s32Ret = HI_MPI_HDMI_ComGetInfoFrame(enHdmi, &stInfoFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMI("call HI_MPI_HDMI_ComGetInfoFrame failed! s32Ret %d\n", s32Ret);
        return HI_FAILURE;
    }
    s32Ret = Hdmi_InfoFrameDrv2User(pstInfoFrame, &stInfoFrame);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComStart(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComStop(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComSetDeepColor(enHdmi, enDeepColor);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComGetDeepColor(enHdmi, penDeepColor);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComSetxvYCCMode(enHdmi, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComSetAVMute(enHdmi, bAvMute);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Force_GetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComForceGetEDID(enHdmi, u8Edid, u32EdidLength);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_ReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComReadEDID(u8Edid, u32EdidLength);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_RegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComRegCallbackFunc(enHdmi, pstCallbackFunc);

    return s32Ret;
}
HI_S32 HI_UNF_HDMI_UnRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComUnRegCallbackFunc(enHdmi, pstCallbackFunc);

    return s32Ret;
}

#ifdef HDMI_HDCP_SUPPORT
HI_S32 HI_UNF_HDMI_LoadHDCPKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComLoadHDCPKey(enHdmi, pstLoadKey);

    return s32Ret;
}
#endif

HI_S32 HI_UNF_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComSetDelay(enHdmi, pstDelay);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ComGetDelay(enHdmi, pstDelay);

    return s32Ret;
}

/*------------------------------END--------------------------------*/
