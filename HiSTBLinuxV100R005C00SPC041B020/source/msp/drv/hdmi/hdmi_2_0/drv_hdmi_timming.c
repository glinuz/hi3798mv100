/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_timming.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/02/01
  Description   :
  History       :
  Date          : 2016/02/01
  Author        : t00273561
  Modification  :
*******************************************************************************/

#if defined(CONFIG_HDMI_STB_SDK)

#include "drv_hdmi_timming.h"
#include "drv_hdmi_platform.h"
#include "hi_drv_hdmi.h"
#include "hdmi_product_define.h"

#define CRG_HPLL_ADDR_0             0xf8a22028
#define CRG_HPLL_ADDR_1             0xf8a2202c
#define HDMI_CRG_VDP_0              0xf8a220d8
#define HDMI_CRG_VDP_1              0xf8a2234c
#define HIFONE_V300_VO_ADDR 	    0xf8cc0000
#define HDMI_ADDR_DHD0_VTTHD        (HIFONE_V300_VO_ADDR+0xc01cU)
#define HDMI_ADDR_DHD0_VTTHD3       (HIFONE_V300_VO_ADDR+0xc018U)
#define HDMI_ADDR_DHD0_PWR          (HIFONE_V300_VO_ADDR+0xc014U)
#define HDMI_ADDR_DHD0_VPLUS        (HIFONE_V300_VO_ADDR+0xc010U)
#define HDMI_ADDR_DHD0_HSYNC2       (HIFONE_V300_VO_ADDR+0xc00cU)
#define HDMI_ADDR_DHD0_HSYNC1       (HIFONE_V300_VO_ADDR+0xc008U)
#define HDMI_ADDR_DHD0_VSYNC        (HIFONE_V300_VO_ADDR+0xc004U)
#define HDMI_ADDR_DHD0_CTRL         (HIFONE_V300_VO_ADDR+0xc000U)
#define HDMI_ADDR_DHD0_SYNC_INV     (HIFONE_V300_VO_ADDR+0xc020U)


#define TIMMING_NULL_CHK(p) \
do{\
    if(HI_NULL == p) {\
	    HDMI_ERR("%s is null pointer!return fail.\n",#p);\
	    return ;}\
}while(0)


typedef struct
{
    HDMI_VIC_E enVic;
    HI_U32     bSynm;
    HI_U32     bIop;
    HI_U32     u32Intfb; 

    HI_U32     u32Vact ;
    HI_U32     u32Vbb;
    HI_U32     u32Vfb;

    HI_U32     u32Hact;
    HI_U32     u32Hbb;
    HI_U32     u32Hfb;

    HI_U32     u32Bvact;
    HI_U32     u32Bvbb;
    HI_U32     u32Bvfb;

    HI_U32     u32Hpw;
    HI_U32     u32Vpw;
    HI_U32     u32Hmid;

    HI_U32     bIdv;
    HI_U32     bIhs;
    HI_U32     bIvs;

    HI_U32  u32PixelClk;
}VDP_TIMING_CONFIG_S;

static VDP_TIMING_CONFIG_S s_stVdpFormatTable[] =
{
             /* Synm Iop  	Itf  Vact Vbb Vfb   Hact  Hbb Hfb      Bvact Bvbb Bvfb  Hpw Vpw Hmid bIdv bIhs bIvs u32PixelClk*/
/* 000:480I(NTSC) */ {HDMI_1440X480I60_4_3,  0,	0,   0,   240,	18,  4,   720,	119,	19,		240,	19,	4,	62,	3,  310, 	0, 1,   1, 27000000},
/* 000:480I(NTSC) */ {HDMI_1440X480I60_16_9, 0,  0,   0,   240,  18,  4,   720,  119,    19,     240,    19, 4,  62, 3,  310,    0, 1,   1, 27000000},
/* 001:576I(PAL) */	 {HDMI_1440X576I50_4_3,  0,	0,   0,   288,	22,  2,   720,	132,	12,		288,	23,	2,	63,	3,  300, 	0,  1,  1, 27000000},
/* 001:576I(PAL) */  {HDMI_1440X576I50_16_9, 0, 0,   0,   288,  22,  2,   720,  132,    12,     288,    23, 2,  63, 3,  300,    0,  1,  1, 27000000},
/* 002:480P@60Hz */	 {HDMI_720X480P60_4_3,   1,	1,   2,   480,	36,  9,   720,	122,	16,		1,	1,	1,	62, 	6,  1,  	0,  1,  1, 27000000}, 
/* 002:480P@60Hz */  {HDMI_720X480P60_16_9,  1, 1,   2,   480,  36,  9,   720,  122,    16,     1,  1,  1,  62,     6,  1,      0,  1,  1, 27000000}, 
/* 003:576P@50Hz */	 {HDMI_720X576P50_4_3,   1,	1,   2,   576,	44,  5,   720,	132,	12,		1,	1,	1,	64, 	5,  1,  	0,  1,  1, 27000000},
/* 003:576P@50Hz */  {HDMI_720X576P50_16_9,  1, 1,   2,   576,  44,  5,   720,  132,    12,     1,  1,  1,  64,     5,  1,      0,  1,  1, 27000000},
/* 004:720P@50Hz */	 {HDMI_1280X720P50_16_9, 1,	1,   2,   720,	25,  5,  1280,	260,	440,		1,	1,	1,	40, 	5,  1,  	0,  0,  0, 74250000},  
/* 005:720P@60Hz */	 {HDMI_1280X720P60_16_9, 1,	1,   2,   720,	25,  5,  1280,	260,	110,		1,	1,	1,	40,  5,  1, 	0,  0,  0, 74250000}, 
/* 006:1080P@24Hz */ {HDMI_1920X1080P24_16_9,1,	1,   2,  1080,	41,  4,  1920,	192,	638,		1,	1,	1,	44, 	5,  1, 	0,  0,  0, 74250000}, 
/* 007:1080P@25Hz */ {HDMI_1920X1080P25_16_9,1,	1,   2,  1080,	41,  4,  1920,	192,	528,		1,	1,	1,	44, 	5,  1,  	0,  0,  0, 74250000}, 
/* 008:1080P@30Hz */ {HDMI_1920X1080P30_16_9,1,	1,   2,  1080,	41,  4,  1920,	192,	88,		1,	1,	1,	44,  5,  1, 	0,  0,  0, 74250000},
/* 009:1080I@50Hz */ {HDMI_1920X1080I50_16_9,1,	0,   2,   540,	20,  2,  1920,	192,	528,		540,	21,	2,	44,  5,  1128,	0,  0,  0, 74250000}, 
/* 010:1080I@60Hz */ {HDMI_1920X1080I60_16_9,1,	0,   2,   540,	20,  2,  1920,	192,	88,		540,	21,	2,	44,  5,  908,	0,  0,  0, 74250000}, 
/* 011:1080P@50Hz */ {HDMI_1920X1080P50_16_9,1,	1,   2,  1080,	41,  4,  1920,	192,	528,		1,	1,	1,	44, 	5,  1,  	0,  0,  0, 148500000},
/* 012:1080P@60Hz */ {HDMI_1920X1080P60_16_9,1,	1,   2,  1080,	41,  4,  1920,	192,	88,		1,	1,	1,	44, 	5,  1,  	0,  0,  0, 148500000}, 
/* 013:3840x2160_24*/{HDMI_3840X2160P24_16_9,1,	1,   2,  2160,	82,  8,  3840,	384,	1276,	1,	1,	1,    88,	10, 1, 	0,  0,  0, 297000000}, 
/* 014:3840x2160_25*/{HDMI_3840X2160P25_16_9,1,	1,   2,  2160,	82,  8,  3840,	384,	1056,	1,	1,	1,    88,	10, 1, 	0,  0,  0, 297000000}, 
/* 015:3840x2160_30*/{HDMI_3840X2160P30_16_9,1,	1,   2,  2160,	82,  8,  3840,	384,	176,		1,	1,	1,    88,	10, 1, 	0,  0,  0, 297000000}, 
/* 016:3840x2160_50*/{HDMI_3840X2160P50_16_9,1,	1,   2,  2160,	82,  8,  3840,	384,	1056,	1,	1,	1,    88,	10, 1, 	0,  0,  0, 594000000}, 
/* 017:3840x2160_60*/{HDMI_3840X2160P60_16_9,1,	1,   2,  2160,	82,  8,  3840,	384,	176,		1,	1,	1,    88,	10, 1, 	0,  0,  0, 594000000}, 
/* 018:4096x2160_24*/{HDMI_4096X2160P24_256_135,1,	1,   2,  2160,  82,  8,  4096, 384, 1020,	1,	1,	1,	88,	10, 1,	0,  0,  0, 297000000},
/* 019:4096x2160_25*/{HDMI_4096X2160P25_256_135,1,	1,   2,  2160,  82,  8,  4096, 216, 968,		1,	1,	1,	88,	10, 1,	0,  0,  0, 297000000},
/* 020:4096x2160_30*/{HDMI_4096X2160P30_256_135,1,	1,   2,  2160,  82,  8,  4096, 216, 88,		1,	1,	1,    88,	10, 1,	0,  0,  0, 297000000},
/* 021:4096x2160_50*/{HDMI_4096X2160P50_256_135,1,	1,   2,  2160,  82,  8,  4096, 216, 968,		1,	1,	1,	88,	10, 1,	0,  0,  0, 594000000},
/* 022:4096x2160_60*/{HDMI_4096X2160P60_256_135,1,	1,   2,  2160,  82,  8,  4096, 216, 88,		1,	1,	1,	88,	10, 1,	0,  0,  0, 594000000},

};

static HDMI_VDP_TIMMING_S s_stVdpTimming[] = 
{
    /*vic 001~007*/
    { HDMI_640X480P60_4_3,      HI_DRV_DISP_FMT_861D_640X480_60,    1, 0x81d68106, 0x00008208, 0x0001005f, 0x00000000, 0x0000000f, 0x008f027f, 0x024221df, 0x8000c011},  // 13 640X480P_60000
    { HDMI_720X480P60_4_3,      HI_DRV_DISP_FMT_480P_60,            1, 0x81d58106, 0x00008208, 0x0005003d, 0x00000000, 0x0000000f, 0x007902cf, 0x020231df, 0x80008011},  // 10 720X480P_60000
    { HDMI_720X480P60_16_9,     HI_DRV_DISP_FMT_480P_60,            1, 0x81d58106, 0x00008208, 0x0005003d, 0x00000000, 0x0000000f, 0x007902cf, 0x020231df, 0x80008011},  // 10 720X480P_60000
    { HDMI_1280X720P60_16_9,    HI_DRV_DISP_FMT_720P_60,            1, 0x82968177, 0x000082e9, 0x00040027, 0x00000000, 0x0000006d, 0x010304ff, 0x010182cf, 0x80008011},  // 07 1280X720P_60000
    { HDMI_1920X1080I60_16_9,   HI_DRV_DISP_FMT_1080i_60,           1, 0x81ee8119, 0x0000822d, 0x0004002b, 0x0041421b, 0x01c50057, 0x00bf077f, 0x0041321b, 0x80008001},  // 05 1920X1080i_60000
    { HDMI_1440X480I60_4_3,     HI_DRV_DISP_FMT_1440x480i_60,       2, 0x80ea8083, 0x00008101, 0x0002007b, 0x00c120ef, 0x01350025, 0x00ed059f, 0x00c110ef, 0x80008001},  // 12 480I_60000
    { HDMI_1440X480I60_16_9,    HI_DRV_DISP_FMT_1440x480i_60,       2, 0x80ea8083, 0x00008101, 0x0002007b, 0x00c120ef, 0x01350025, 0x00ed059f, 0x00c110ef, 0x80008001},  // 12 480I_60000
    /*vic 0016~022*/    
    { HDMI_1920X1080P60_16_9,   HI_DRV_DISP_FMT_1080P_60,           1, 0x83dd8232, 0x00008460, 0x0004002b, 0x00000000, 0x00000057, 0x00bf077f, 0x00c28437, 0x80008011},  // 00 1080p60
    { HDMI_720X576P50_4_3,      HI_DRV_DISP_FMT_576P_50,            1, 0x82388138, 0x0000826c, 0x0004003f, 0x00000000, 0x0000000b, 0x008302cf, 0x0102b23f, 0x80008011},  // 09 720X576P_50000
    { HDMI_720X576P50_16_9,     HI_DRV_DISP_FMT_576P_50,            1, 0x82388138, 0x0000826c, 0x0004003f, 0x00000000, 0x0000000b, 0x008302cf, 0x0102b23f, 0x80008011},  // 09 720X576P_50000
    { HDMI_1280X720P50_16_9,    HI_DRV_DISP_FMT_720P_50,            1, 0x82a88177, 0x000082e9, 0x00040027, 0x00000000, 0x000001b7, 0x010304ff, 0x010182cf, 0x80008011},  // 08 1280X720P_50000
    { HDMI_1920X1080I50_16_9,   HI_DRV_DISP_FMT_1080i_50,           1, 0x81fc8119, 0x0000822d, 0x0004002b, 0x0041421b, 0x0233020f, 0x00bf077f, 0x0041321b, 0x80008001},  // 06 1920X1080i_50000
    { HDMI_1440X576I50_4_3,     HI_DRV_DISP_FMT_1440x576i_50,       2, 0x811b809c, 0x00008133, 0x0002007d, 0x0041611f, 0x012b0017, 0x0107059f, 0x0041511f, 0x80008001},  // 11 576I_50000
    { HDMI_1440X576I50_16_9,    HI_DRV_DISP_FMT_1440x576i_50,       2, 0x811b809c, 0x00008133, 0x0002007d, 0x0041611f, 0x012b0017, 0x0107059f, 0x0041511f, 0x80008001},  // 11 576I_50000    
    /*vic 031~034*/
    { HDMI_1920X1080P50_16_9,   HI_DRV_DISP_FMT_1080P_50,           1, 0x83f98232, 0x00008460, 0x0004002b, 0x00000000, 0x0000020f, 0x00bf077f, 0x00c28437, 0x80008011},  // 01 1920X1080P_50000
    { HDMI_1920X1080P24_16_9,   HI_DRV_DISP_FMT_1080P_24,           1, 0x84338232, 0x00008460, 0x0004002b, 0x00000000, 0x0000027d, 0x00bf077f, 0x00c28437, 0x80008011},  // 04 1920X1080P_24000
    { HDMI_1920X1080P25_16_9,   HI_DRV_DISP_FMT_1080P_25,           1, 0x84318232, 0x00008460, 0x0004002b, 0x00000000, 0x0000020f, 0x00bf077f, 0x00c28437, 0x80008011},  // 03 1920X1080P_25000
    { HDMI_1920X1080P30_16_9,   HI_DRV_DISP_FMT_1080P_30,           1, 0x84258232, 0x00008460, 0x0004002b, 0x00000000, 0x00000057, 0x00bf077f, 0x00c28437, 0x80008011},  // 02 1920X1080P_30000
    /*vic 093~097*/
    { HDMI_3840X2160P24_16_9,   HI_DRV_DISP_FMT_3840X2160_24,       1, 0x88658465, 0x000088c5, 0x00090057, 0x00000000, 0x000004fb, 0x017f0eff, 0x01c5186f, 0x80008011},  // 25 3840X2160_24
    { HDMI_3840X2160P25_16_9,   HI_DRV_DISP_FMT_3840X2160_25,       1, 0x88628465, 0x000088c5, 0x00090057, 0x00000000, 0x0000041f, 0x017f0eff, 0x01c5186f, 0x80008011},  // 26 3840X2160_25
    { HDMI_3840X2160P30_16_9,   HI_DRV_DISP_FMT_3840X2160_30,       1, 0x884a8465, 0x000088c5, 0x00090057, 0x00000000, 0x000000af, 0x017f0eff, 0x01c5186f, 0x80008011},  // 27 3840X2160_30
    { HDMI_3840X2160P50_16_9,   HI_DRV_DISP_FMT_3840X2160_50,       1, 0x87f18465, 0x000088c5, 0x00090057, 0x00000000, 0x0000041f, 0x017f0eff, 0x01c5186f, 0x80008011},  // 28 3840X2160_50
    { HDMI_3840X2160P60_16_9,   HI_DRV_DISP_FMT_3840X2160_60,       1, 0x87b98465, 0x000088c5, 0x00090057, 0x00000000, 0x000000af, 0x017f0eff, 0x01c5186f, 0x80008011},  // 29 3840X2160_60
    /*vic 098~102*/
    { HDMI_4096X2160P24_256_135,HI_DRV_DISP_FMT_4096X2160_24,       1, 0x88658465, 0x000088c5, 0x00090057, 0x00000000, 0x000003fb, 0x017f0fff, 0x01c5186f, 0x80008011},  // 30 4096X2160_24
    { HDMI_4096X2160P25_256_135,HI_DRV_DISP_FMT_4096X2160_25,       1, 0x88628465, 0x000088c5, 0x00090057, 0x00000000, 0x000003c7, 0x00d70fff, 0x01c5186f, 0x80008011},  // 31 4096X2160_25
    { HDMI_4096X2160P30_256_135,HI_DRV_DISP_FMT_4096X2160_30,       1, 0x884a8465, 0x000088c5, 0x00090057, 0x00000000, 0x00000057, 0x00d70fff, 0x01c5186f, 0x80008011},  // 32 4096X2160_30
    { HDMI_4096X2160P50_256_135,HI_DRV_DISP_FMT_4096X2160_50,       1, 0x87f18465, 0x000088c5, 0x00090057, 0x00000000, 0x000003c7, 0x00d70fff, 0x01c5186f, 0x80008011},  // 33 4096X2160_50
    { HDMI_4096X2160P60_256_135,HI_DRV_DISP_FMT_4096X2160_60,       1, 0x87b98465, 0x000088c5, 0x00090057, 0x00000000, 0x00000057, 0x00d70fff, 0x01c5186f, 0x80008011},  // 34 4096X2160_60
    /*vic 103~107*/
    { HDMI_3840X2160P24_64_27,  HI_DRV_DISP_FMT_3840X2160_24,       1, 0x88658465, 0x000088c5, 0x00090057, 0x00000000, 0x000004fb, 0x017f0eff, 0x01c5186f, 0x80008011},  // 25 3840X2160_24
    { HDMI_3840X2160P25_64_27,  HI_DRV_DISP_FMT_3840X2160_25,       1, 0x88628465, 0x000088c5, 0x00090057, 0x00000000, 0x0000041f, 0x017f0eff, 0x01c5186f, 0x80008011},  // 26 3840X2160_25
    { HDMI_3840X2160P30_64_27,  HI_DRV_DISP_FMT_3840X2160_30,       1, 0x884a8465, 0x000088c5, 0x00090057, 0x00000000, 0x000000af, 0x017f0eff, 0x01c5186f, 0x80008011},  // 27 3840X2160_30
    { HDMI_3840X2160P50_64_27,  HI_DRV_DISP_FMT_3840X2160_50,       1, 0x87f18465, 0x000088c5, 0x00090057, 0x00000000, 0x0000041f, 0x017f0eff, 0x01c5186f, 0x80008011},  // 28 3840X2160_50
    { HDMI_3840X2160P60_64_27,  HI_DRV_DISP_FMT_3840X2160_60,       1, 0x87b98465, 0x000088c5, 0x00090057, 0x00000000, 0x000000af, 0x017f0eff, 0x01c5186f, 0x80008011}, // 29 3840X2160_60

};

static VDP_TIMING_CONFIG_S *SOC_VdpTimmingGetData(HDMI_VIC_E enVic)
{
    HI_U32  i;
    VDP_TIMING_CONFIG_S *pVdpTimingCfg;

    pVdpTimingCfg = &s_stVdpFormatTable[0];
    for (i = 0; (pVdpTimingCfg && i< HDMI_ARRAY_SIZE(s_stVdpFormatTable)); pVdpTimingCfg++, i++)
    {
        if (enVic == pVdpTimingCfg->enVic)
        {
            return pVdpTimingCfg;
        }
    }

    HDMI_ERR("Can not find the enVic %d in array of s_stVdpFormatTable\n", enVic);

    return HI_NULL;
}

static HI_VOID SOC_HpllConfig(HI_U32 u32PllClk)
{
    switch(u32PllClk)
    {
        case 342000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2103156);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2003156);
            break;
        case 355000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2103163);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2003163);
            break;
        case 387200:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x0103183);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12334500);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x0003183);
            break;
        case 402800:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x01030c9);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12666674);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x00030c9);	
            break;
        case 426000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x10306a);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x1181B320);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x00306a);
            break;
        case 432000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2101048);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2001048);
            break;
        case 468000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2103075);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2003075);
            break;
        case 476000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x10204f);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11557300);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x00204f);
            break;
        case 483000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x103078);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11C042C0);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x003078);
            break;
        case 487000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x103079);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11C042C0);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x003079);
            break;
        case 537000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x103086);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11401640);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x003086);
            break;
        case 594000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2101063);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2001063);
            break;
        case 616000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x210309a);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x200309a);
            break;
        case 627000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x10309c);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11C042C0);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x00309c);
            break;
        case 640000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x21030a0);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x20030a0);	
            break;
        case 648000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2101036);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x10000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2001036);
            break;
        case 668000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x21030a7);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x20030a7);
            break;
        case 684000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x21030ab);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x20030ab);
            break;
        case 710000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x102076);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11557300);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x002076);
            break;
        case 774400:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x1030c1);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x1199A7F0);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x0030c1);
            break;
        case 782000:
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x102076);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x11557300);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x002076);
            break;
        default:
            HDMI_ERR("pll clk not corect, config default value 594000");
            HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2101063);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_0,0x12000000);
    		HDMIRegWriteU32(CRG_HPLL_ADDR_1,0x2001063);
            break;
    }

}

static HI_VOID SOC_PixelClkConfig(HI_U32 u32PixelClk)
{
	HI_U32	u32TmpValue = 0;
    HI_U32  u8PixelClkDiv= 0;
    HI_U32  u32PllClk = 0;

    if(u32PixelClk >= 21250 && u32PixelClk <= 50000)
    {
        u8PixelClkDiv   = 16;
    }
    else if(u32PixelClk > 50000 && u32PixelClk <= 100000)
    {
        u8PixelClkDiv   = 8;
    }
    else if(u32PixelClk > 100000 && u32PixelClk <= 200000)
    {
        u8PixelClkDiv   = 4;
    }
    else if(u32PixelClk > 200000 && u32PixelClk <=340000)
    {
        u8PixelClkDiv   = 2;
    }
    else if(u32PixelClk > 340000 && u32PixelClk <=600000)
    {
        u8PixelClkDiv   = 1;
    }

    u32PllClk = u32PixelClk * u8PixelClkDiv;
    printk("hdmi timming para u32PllClk = %u.\n", u32PllClk);
    SOC_HpllConfig(u32PllClk);

    printk("hdmi timming para u8pixelclkdiv = %u.\n", u8PixelClkDiv);

    if(u8PixelClkDiv <= 2)
    {
        HDMIRegWriteU32(HDMI_CRG_VDP_0,0x8a09005d);
    }
    else
    {
        HDMIRegWriteU32(HDMI_CRG_VDP_0,0x8a01005d);
        u8PixelClkDiv /= 2;
    }
    HDMIRegWriteU32(HDMI_CRG_VDP_1,0x7FFF);

    printk("hdmi timming para u8pixelclkdiv after handle = %u.\n", u8PixelClkDiv);

	u32TmpValue  = HDMIRegReadU32(HDMI_CRG_VDP_1);
	u32TmpValue &= (~0x1f000000);
	u32TmpValue |= (((u8PixelClkDiv-1) & 0x1f)<<24)&0x1f000000;
	HDMIRegWriteU32(HDMI_CRG_VDP_1,u32TmpValue);

}

static HI_VOID SOC_VoDivCfg(HDMI_VIC_E enFormatId)
{
	HI_U32	u32TmpValue;

	if((enFormatId == HDMI_1440X480I60_4_3)||(enFormatId == HDMI_1440X480I60_16_9)||
       (enFormatId == HDMI_1440X576I50_4_3)||(enFormatId == HDMI_1440X576I50_16_9))
	{
		u32TmpValue = HDMIRegReadU32(HDMI_CRG_VDP_0);
		u32TmpValue |= 0x20000000;
		HDMIRegWriteU32(HDMI_CRG_VDP_0,u32TmpValue);
	}
	else
	{
		u32TmpValue = HDMIRegReadU32(HDMI_CRG_VDP_0);
		u32TmpValue &= (~0x20000000);
		HDMIRegWriteU32(HDMI_CRG_VDP_0,u32TmpValue);
	}
}

static HI_VOID SOC_VoTimingCfg(HDMI_VIC_E enFormatId)
{
	VDP_TIMING_CONFIG_S		*pstTimming = HI_NULL;
	HI_U32					u32Tmp;

	pstTimming = SOC_VdpTimmingGetData(enFormatId);
    TIMMING_NULL_CHK(pstTimming);

    HDMI_INFO("vact = %u, hact = %u.\n",pstTimming->u32Vact, pstTimming->u32Hact);
	//Interlace
	u32Tmp = HDMIRegReadU32(HDMI_ADDR_DHD0_CTRL);
    u32Tmp = pstTimming->bIop ? u32Tmp|0x10 : u32Tmp&0xef;
	HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,u32Tmp);
	//V Config
	HDMIRegWriteU32(HDMI_ADDR_DHD0_VSYNC,((pstTimming->u32Vact-1)|(pstTimming->u32Vbb-1)<<12|(pstTimming->u32Vfb-1)<<22));
	//H1 Config
	HDMIRegWriteU32(HDMI_ADDR_DHD0_HSYNC1,((pstTimming->u32Hact-1)|(pstTimming->u32Hbb-1)<<16));
	//H2 Config
	HDMIRegWriteU32(HDMI_ADDR_DHD0_HSYNC2,((pstTimming->u32Hfb-1)|(pstTimming->u32Hmid-1)<<16));
	//VB Config
	HDMIRegWriteU32(HDMI_ADDR_DHD0_VPLUS,((pstTimming->u32Bvact-1)|(pstTimming->u32Bvbb-1)<<12|(pstTimming->u32Bvfb-1)<<22));
	//PW Config
	HDMIRegWriteU32(HDMI_ADDR_DHD0_PWR,((pstTimming->u32Hpw-1)|(pstTimming->u32Vpw-1)<<16));
	//H POL
	u32Tmp  = HDMIRegReadU32(HDMI_ADDR_DHD0_SYNC_INV);
	u32Tmp &= (~0x200);
    u32Tmp |= pstTimming->bIhs<<9;
	HDMIRegWriteU32(HDMI_ADDR_DHD0_SYNC_INV,u32Tmp);
	//V POL
	u32Tmp = HDMIRegReadU32(HDMI_ADDR_DHD0_SYNC_INV);
	u32Tmp &= (~0x400);
	u32Tmp |= pstTimming->bIvs<<10;
	HDMIRegWriteU32(HDMI_ADDR_DHD0_SYNC_INV,u32Tmp);

}

static HI_VOID SOC_VoClrbarCfg(HDMI_COLORSPACE_E enColcor,HDMI_VIC_E enFormatId)
{
	if((enFormatId == HDMI_1440X480I60_4_3)||(enFormatId == HDMI_1440X480I60_16_9)||
       (enFormatId == HDMI_1440X576I50_4_3)||(enFormatId == HDMI_1440X576I50_16_9)||
	   (enFormatId == HDMI_1920X1080I50_16_9)||(enFormatId == HDMI_1920X1080I60_16_9))
	{
		switch(enColcor)
		{
			case HDMI_COLORSPACE_RGB:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xc0008001);
				break;
			case HDMI_COLORSPACE_YCbCr444:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xe0008001);
				break;
			case HDMI_COLORSPACE_YCbCr420:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xe0008601);
				break;
			default:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0x8000e001);
				break;
		}
	}
	else
	{
		switch(enColcor)
		{
			case HDMI_COLORSPACE_RGB:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xc000c011);
				break;
			case HDMI_COLORSPACE_YCbCr444:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xe0008011);
				break;				
			case HDMI_COLORSPACE_YCbCr420:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0xe0008611);
				break;
			default:
				HDMIRegWriteU32(HDMI_ADDR_DHD0_CTRL,0x8000e011);
				break;
		}
	}
}

HI_S32  DRV_HDMI_TimmingSet(HDMI_DEVICE_ID_E enHdmiId,HDMI_VIC_E enVic)

{
    HI_U32              i = 0;
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_VDP_TIMMING_S  *pstTmpTim = HI_NULL;
    HDMI_VIDEO_DEF_S    *pstFormat = HI_NULL;
    HDMI_VIDEO_ATTR_S   stVideoAttr;

    for (i=0,pstTmpTim = &s_stVdpTimming[0];
         i < HDMI_ARRAY_SIZE(s_stVdpTimming);
         i++,pstTmpTim++)
    {
        if (pstTmpTim->enVic == enVic)
        {
            break;
        }
    }
    if (i>= HDMI_ARRAY_SIZE(s_stVdpTimming))
    {
        HDMI_ERR("timming not support vic=%u\n",enVic);
        return HI_FAILURE;
    }

    HDMI_MEMSET(&stVideoAttr,0,sizeof(stVideoAttr));
    pstFormat = DRV_HDMI_CommFormatParamGet(enVic);
    if (!pstFormat)
    {
        HDMI_ERR("timming param not support vic=%u\n",enVic);
        return HI_FAILURE;
    }

    SOC_PixelClkConfig(pstFormat->enPixclk);
    SOC_VoDivCfg(enVic);
	SOC_VoTimingCfg(enVic);
	SOC_VoClrbarCfg(HDMI_COLORSPACE_YCbCr444,enVic);

    stVideoAttr.bDEPol              = HI_FALSE;
    stVideoAttr.bHSyncPol           = HI_FALSE;
    stVideoAttr.bVSyncPol           = HI_FALSE;
    stVideoAttr.enInBitDepth        = HDMI_VIDEO_BITDEPTH_10;
    stVideoAttr.enStereoMode        = 0;
    stVideoAttr.enVideoFmt          = pstTmpTim->enDisFmt;
    stVideoAttr.u32ClkFs            = pstFormat->enPixclk;
    stVideoAttr.u32DispRatioHeight  = 0;
    stVideoAttr.u32DispRatioWidth   = 0;
    stVideoAttr.u32PixelRepeat      = pstTmpTim->u32PixRepeat;
    stVideoAttr.u32ColorSpace       = HI_DRV_CS_BT709_YUV_LIMITED ;
   
    s32Ret = HI_DRV_HDMI_SetFormat(enHdmiId , &stVideoAttr);
    
    return s32Ret;
}


HI_S32  DRV_HDMI_DispAttrSet(HDMI_DEVICE_ID_E enHdmiId,HDMI_VIDEO_ATTR_S *pstDispAttr)

{
    HI_U32              i = 0;
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_VDP_TIMMING_S  *pstTmpTim = HI_NULL;
    HDMI_VIDEO_DEF_S    *pstFormat = HI_NULL;
    HDMI_VIDEO_ATTR_S   stVideoAttr;

    for (i=0,pstTmpTim = &s_stVdpTimming[0];
         i < HDMI_ARRAY_SIZE(s_stVdpTimming);
         i++,pstTmpTim++)
    {
        if (pstTmpTim->enDisFmt == pstDispAttr->enVideoFmt)
        {
            break;
        }
    }
    if (i>= HDMI_ARRAY_SIZE(s_stVdpTimming))
    {
        HDMI_ERR("timming not support enVideoFmt=%u\n",pstTmpTim->enDisFmt);
        return HI_FAILURE;
    }

    HDMI_MEMSET(&stVideoAttr,0,sizeof(stVideoAttr));
    pstFormat = DRV_HDMI_CommFormatParamGet(pstTmpTim->enVic);
    if (!pstFormat)
    {
        HDMI_ERR("timming param not support pstTmpTim->enDisFmt=%u\n",pstTmpTim->enDisFmt);
        return HI_FAILURE;
    }

    printk("pixlclk = %u, vic = %d.\n",pstFormat->enPixclk,pstTmpTim->enVic);
    SOC_PixelClkConfig(pstFormat->enPixclk);
    SOC_VoDivCfg(pstTmpTim->enVic);
	SOC_VoTimingCfg(pstTmpTim->enVic);  
	SOC_VoClrbarCfg(HDMI_COLORSPACE_YCbCr444,pstTmpTim->enVic);

    stVideoAttr.bDEPol              = pstDispAttr->bDEPol;
    stVideoAttr.bHSyncPol           = pstDispAttr->bHSyncPol;
    stVideoAttr.bVSyncPol           = pstDispAttr->bVSyncPol;
    stVideoAttr.enInBitDepth        = pstDispAttr->enInBitDepth;
    stVideoAttr.enStereoMode        = pstDispAttr->enStereoMode;
    stVideoAttr.enVideoFmt          = pstTmpTim->enDisFmt;//here
    stVideoAttr.u32ClkFs            = pstFormat->enPixclk;//here
    //HDMI_ERR("pstFormat->enPixclk=%u,pstTmpTim->enDisFmt=%u\n",pstFormat->enPixclk,pstTmpTim->enDisFmt);
    stVideoAttr.u32DispRatioHeight  = pstDispAttr->u32DispRatioHeight;
    stVideoAttr.u32DispRatioWidth   = pstDispAttr->u32DispRatioWidth;
    stVideoAttr.u32PixelRepeat      = pstTmpTim->u32PixRepeat;//here
    stVideoAttr.u32ColorSpace       = HI_DRV_CS_BT709_YUV_LIMITED ; //here
   
    s32Ret = HI_DRV_HDMI_SetFormat(enHdmiId , &stVideoAttr);
    
    return s32Ret;
}


#endif /* CONFIG_HDMI_STB_SDK */




