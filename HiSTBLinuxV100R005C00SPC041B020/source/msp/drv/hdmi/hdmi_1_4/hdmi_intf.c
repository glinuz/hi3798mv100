/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <mach/hardware.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

//#include "common_dev.h" 头文件改名
#include "hi_drv_dev.h"
//#include "common_proc.h"
#include "hi_drv_proc.h"


//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "drv_pdm_ext.h"
#include "drv_hdmi.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_edid.h"
#include "si_phy.h"
#include "si_isr.h"
#include "si_delay.h"
#include "si_cec.h"

//#include "hi_common_id.h"
#include "hi_module.h"
//#include "hi_common_log.h"
#include "hi_debug.h"
//#include "common_module_drv.h"
#include "hi_drv_module.h"
#include "drv_hdmi_ext.h"
#include "si_timer.h"
#include "drv_global.h"
#include "drv_hdmi_debug.h"

#include "hi_osal.h"
#include "drv_reg_proc.h"
#include "drv_compatibility.h"

/******************************************************************************/
/* HDMI Version                                                               */
/******************************************************************************/
#define HDMI_VER_MAJOR    1
#define HDMI_VER_MINOR    4
#define HDMI_VER_REVISE   0
#define HDMI_VER_DATE     20160625
#define HDMI_VER_TIMES    0

#define MAKE_VER_BIT(x) # x
#define MAKE_MACRO2STR(exp) MAKE_VER_BIT(exp)
#define MAKE_VERSION \
    MAKE_MACRO2STR(HDMI_VER_MAJOR) "."  \
    MAKE_MACRO2STR(HDMI_VER_MINOR) "."  \
    MAKE_MACRO2STR(HDMI_VER_REVISE) "." \
    MAKE_MACRO2STR(HDMI_VER_DATE) "."   \
    MAKE_MACRO2STR(HDMI_VER_TIMES)

#ifdef ANDROID_SUPPORT

#include <linux/switch.h>

struct switch_dev hdmi_tx_sdev =
{
    .name = "hdmi",
};

HI_BOOL g_switchOk = HI_FALSE;


#endif


#define HDMI_NAME                      "HI_HDMI"
#if defined (SUPPORT_FPGA)
#include "hdmi_fpga.h"
#endif
HI_S32 DRV_HDMI_ReadPhy(void)
{
    HI_U32 u32Ret;

    u32Ret = SI_TX_PHY_GetOutPutEnable();

    return u32Ret;
}






extern HI_S32 hdmi_Open(struct inode *inode, struct file *filp);
extern HI_S32 hdmi_Close(struct inode *inode, struct file *filp);
extern HI_S32 hdmi_Ioctl(struct inode *inode, struct file *file,
                           unsigned int cmd, HI_VOID *arg);
extern HI_S32 hdmi_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
extern HI_S32 hdmi_Resume(PM_BASEDEV_S *pdev);
extern HI_S32 hdmi_SoftResume(HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo);

static HDMI_EXPORT_FUNC_S s_stHdmiExportFuncs = {
    .pfnHdmiInit = HI_DRV_HDMI_Init,
    .pfnHdmiDeinit = HI_DRV_HDMI_Deinit,
    .pfnHdmiOpen  = HI_DRV_HDMI_Open,
    .pfnHdmiClose = HI_DRV_HDMI_Close,
    .pfnHdmiGetPlayStus = HI_DRV_HDMI_GetPlayStatus,
    .pfnHdmiGetAoAttr = HI_DRV_HDMI_GetAOAttr,
    .pfnHdmiGetSinkCapability = HI_DRV_HDMI_GetSinkCapability,
    .pfnHdmiGetAudioCapability = HI_DRV_HDMI_GetAudioCapability,
    .pfnHdmiSetAudioMute = HI_DRV_HDMI_SetAudioMute,
    .pfnHdmiSetAudioUnMute = HI_DRV_HDMI_SetAudioUnMute,
    .pfnHdmiAudioChange = HI_DRV_HDMI_SetAOAttr,
    .pfnHdmiPreFormat = HI_DRV_HDMI_PreFormat,
    .pfnHdmiSetFormat = HI_DRV_HDMI_SetFormat,
    .pfnHdmiDetach = HI_DRV_HDMI_Detach,
    .pfnHdmiAttach = HI_DRV_HDMI_Attach,
    .pfnHdmiResume = hdmi_Resume,
    .pfnHdmiSuspend = hdmi_Suspend,
    .pfnHdmiSoftResume = hdmi_SoftResume,
};

static long  hdmi_Drv_Ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, hdmi_Ioctl);
}

static struct file_operations hdmi_FOPS =
{
    owner   : THIS_MODULE,
    open    : hdmi_Open,
    unlocked_ioctl   : hdmi_Drv_Ioctl,
    release : hdmi_Close,
};
#if 0
static /*struct*/ PM_BASEOPS_S  hdmi_DRVOPS = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = hdmi_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = hdmi_Resume,
};
#else
static /*struct*/ PM_BASEOPS_S  hdmi_DRVOPS = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = NULL,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = NULL,
};
#endif

static UMAP_DEVICE_S   g_hdmiRegisterData;

HI_U32 u32PixelClk[HI_UNF_ENC_FMT_BUTT + 1] = {0};

#if (1 == HI_PROC_SUPPORT)

static HI_U8 *g_pDefHDMIMode[] = {"NULL","HDMI","DVI","BUTT"};

static HI_U8 *g_pAudioFmtCode[]=
{
    "Reserved", "PCM",  "AC3",     "MPEG1", "MP3",   "MPEG2", "AAC",
    "DTS",     "ATRAC", "ONE_BIT", "DDP",   "DTS_HD", "MAT",  "DST",
    "WMA_PRO"
};
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
static HI_U8 *g_pSampleRate[] =
{
    "32", "44.1", "48", "88.2","96","176.4","192","BUTT"
};
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
static HI_U8 *g_pSpeaker[] =
{
    "FL/FR", "LFE", "FC", "RL/RR","RC","FLC/FRC","RLC/RRC",
    "FLW/FRW","FLH/FRH","TC","FCH"
};


static HI_U8 *g_pAudInputType[] = { "I2S","SPDIF","HBR","BUTT"};
static HI_U8 *g_pColorSpace[] = {"RGB444","YCbCr422","YCbCr444","Future"};
static HI_U8 *g_pDeepColor[] = {"24bit","30bit","36bit","OFF","BUTT"};

static HI_U8 *g_p3DMode[] = {
    "FPK",
    "FILED_ALTER",
    "LINE_ALTE",
    "SBS_FULL",
    "L_DEPTH",
    "L_DEPTH_G_DEPTH",
    "TAB",
    "", //0x07 unknown
    "SBS_HALF",
};

static HI_U8 *g_pScanInfo[] = {"No Data","OverScan","UnderScan","Future"};
static HI_U8 *g_pPixelRep[] = {"1x(No Repeat)","2x","3x","4x","5x","6x","7x","8x","9x","10x","Reserved"};
const static HI_CHAR* g_pszVersion = "HDMI Version:   "MAKE_VERSION;

static HI_U8 g_pDispFmtString[HI_DRV_DISP_FMT_BUTT+1][DEF_FILE_NAMELENGTH];

static HI_U8 g_pUnfFmtString[HI_UNF_ENC_FMT_BUTT+1][DEF_FILE_NAMELENGTH];

void hdmi_InitFmtArray(void)
{
    HI_U32 i = 0;

    for(i = 0; i < HI_UNF_ENC_FMT_BUTT+1;i++)
    {
        memset(g_pUnfFmtString[i] , 0 ,sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
        HI_OSAL_Strncpy(g_pUnfFmtString[i] , "unknown" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    }

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_60              ], "1080P60"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_60              ], "1080P60"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_50              ], "1080P50"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_30              ], "1080P30"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_25              ], "1080P25"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_24              ], "1080P24"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080i_60              ], "1080i60"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080i_50              ], "1080i50"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_720P_60               ], "720P60"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_720P_50               ], "720P50"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_576P_50               ], "576P50"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_480P_60               ], "480P60"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_PAL                   ], "PAL"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_PAL_N                 ], "PAL_N"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_PAL_Nc                ], "PAL_Nc"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_NTSC                  ], "NTSC"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_NTSC_J                ], "NTSC_J"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_NTSC_PAL_M            ], "NTSC_PAL_M"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_SECAM_SIN             ], "SECAM_SIN"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_SECAM_COS             ], "SECAM_COS"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING], "1080P24_FP"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_720P_60_FRAME_PACKING ], "720P60_FP"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_720P_50_FRAME_PACKING ], "720P50_FP"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_861D_640X480_60       ], "640x480"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_800X600_60       ], "800x600"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1024X768_60      ], "1024x768"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1280X720_60      ], "1280x720"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1280X800_60      ], "1280x800"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1280X1024_60     ], "1280x1024"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1360X768_60      ], "1360x768"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1366X768_60      ], "1366x768"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1400X1050_60     ], "1400x1050"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1440X900_60      ], "1440x900"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1440X900_60_RB   ], "1440x900_RB"  , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1600X900_60_RB   ], "1600x900_RB"  , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1600X1200_60     ], "1600x1200"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1680X1050_60     ], "1680x1050"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1680X1050_60_RB  ], "1680x1050_RB" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1920X1080_60     ], "1920x1080"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1920X1200_60     ], "1920x1200"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_1920X1440_60     ], "1920x1440"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_2048X1152_60     ], "2048x1152 "   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_2560X1440_60_RB  ], "2560x1440 _RB", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_VESA_2560X1600_60_RB  ], "2560x1600 _RB", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_3840X2160_24          ], "3840X2160_24" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_3840X2160_25          ], "3840X2160_25" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_3840X2160_30          ], "3840X2160_30" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_4096X2160_24          ], "4096X2160_24" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pUnfFmtString[HI_UNF_ENC_FMT_BUTT                  ], "BUTT"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    for(i = 0; i < HI_DRV_DISP_FMT_BUTT;i++)
    {
        memset(g_pDispFmtString[i] , 0 ,sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
        HI_OSAL_Strncpy(g_pDispFmtString[i] , "unknown" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    }

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_60             ], "1080P60"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_50             ], "1080P50"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_30             ], "1080P30"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_25             ], "1080P25"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_24             ], "1080P24"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080i_60             ], "1080i60"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080i_50             ], "1080i50"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_720P_60              ], "720P60"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_720P_50              ], "720P50"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_576P_50              ], "576P50"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_480P_60              ], "480P60"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL                  ], "PAL"            , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_B                ], "PAL_B"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_B1               ], "PAL_B1"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_D                ], "PAL_D"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_D1               ], "PAL_D1"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_G                ], "PAL_G"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_H                ], "PAL_H"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_K                ], "PAL_K"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_I                ], "PAL_I"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_N                ], "PAL_N"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_Nc               ], "PAL_Nc"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_M                ], "PAL_M"          , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_PAL_60               ], "PAL_60"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_NTSC                 ], "NTSC"           , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_NTSC_J               ], "NTSC_J"         , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_NTSC_443             ], "NTSC_443"       , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_SIN            ], "SECAM_SIN"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_COS            ], "SECAM_COS"      , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_L              ], "SECAM_L"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_B              ], "SECAM_B"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_G              ], "SECAM_G"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_D              ], "SECAM_D"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_K              ], "SECAM_K"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_SECAM_H              ], "SECAM_H"        , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1440x576i_50         ], "1440x576i_50"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1440x480i_60         ], "1440x480i_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_1080P_24_FP          ], "1080P_24_FP"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_720P_60_FP           ], "720P_60_FP"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_720P_50_FP           ], "720P_50_FP"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_861D_640X480_60      ], "640X480_60"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_800X600_60      ], "800X600_60"     , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1024X768_60     ], "1024X768_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1280X720_60     ], "1280X720_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1280X800_60     ], "1280X800_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1280X1024_60    ], "1280X1024_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1360X768_60     ], "1360X768_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1366X768_60     ], "1366X768_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1400X1050_60    ], "1400X1050_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1440X900_60     ], "1440X900_60"    , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1440X900_60_RB  ], "1440X900_60_RB" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1600X900_60_RB  ], "1600X900_60_RB" , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1600X1200_60    ], "1600X1200_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1680X1050_60    ], "1680X1050_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1680X1050_60_RB ], "1680X1050_60_RB", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1920X1080_60    ], "1920X1080_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1920X1200_60    ], "1920X1200_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_1920X1440_60    ], "1920X1440_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_2048X1152_60    ], "2048X1152_60"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_2560X1440_60_RB ], "2560X1440_60_RB", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_VESA_2560X1600_60_RB ], "2560X1600_60_RB", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_3840X2160_24         ], "3840X2160_24"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_3840X2160_25         ], "3840X2160_25"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_3840X2160_30         ], "3840X2160_30"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_4096X2160_24         ], "4096X2160_24"   , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);

    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_CUSTOM               ], "Customer Timing", sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
    HI_OSAL_Strncpy(g_pDispFmtString[HI_DRV_DISP_FMT_BUTT                 ], "BUTT"           , sizeof(HI_U8)*DEF_FILE_NAMELENGTH);
}


#endif

void hdmi_Init_PixelClkArray(void)
{
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_60   ] =  148500;
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_50   ] =  148500;     /**<1080p 50 Hz*/                
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_30   ] =  74250;     /**<1080p 30 Hz*/
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_25   ] =  74250;     /**<1080p 25 Hz*/
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_24   ] =  74250;     /**<1080p 24 Hz*/

    u32PixelClk[ HI_UNF_ENC_FMT_1080i_60   ] =  74250;     /**<1080i 60 Hz*/
    u32PixelClk[ HI_UNF_ENC_FMT_1080i_50   ] =  74250;     /**<1080i 50 Hz*/

    u32PixelClk[ HI_UNF_ENC_FMT_720P_60    ] =  74250;     /**<720p 60 Hz*/
    u32PixelClk[ HI_UNF_ENC_FMT_720P_50    ] =  74250;     /**<720p 50 Hz */

    u32PixelClk[ HI_UNF_ENC_FMT_576P_50    ] =  27000;     /**<576p 50 Hz*/
    u32PixelClk[ HI_UNF_ENC_FMT_480P_60    ] =  27000;     /**<480p 60 Hz*/

    u32PixelClk[ HI_UNF_ENC_FMT_PAL        ] =  27000;     /* B D G H I PAL */
    u32PixelClk[ HI_UNF_ENC_FMT_PAL_N      ] =  27000;     /* (N)PAL        */
    u32PixelClk[ HI_UNF_ENC_FMT_PAL_Nc     ] =  27000;     /* (Nc)PAL       */

    u32PixelClk[ HI_UNF_ENC_FMT_NTSC       ] =  27000;     /* (M)NTSC       */
    u32PixelClk[ HI_UNF_ENC_FMT_NTSC_J     ] =  27000;     /* NTSC-J        */
    u32PixelClk[ HI_UNF_ENC_FMT_NTSC_PAL_M ] =  27000;     /* (M)PAL        */

    u32PixelClk[ HI_UNF_ENC_FMT_SECAM_SIN  ] =  27000;     /**< SECAM_SIN*/
    u32PixelClk[ HI_UNF_ENC_FMT_SECAM_COS  ] =  27000;     /**< SECAM_COS*/

    u32PixelClk[ HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING ] =  148500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_720P_60_FRAME_PACKING  ] =  148500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_720P_50_FRAME_PACKING  ] =  148500 ;

    u32PixelClk[ HI_UNF_ENC_FMT_861D_640X480_60      ] = 25175 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_800X600_60      ] = 40000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1024X768_60     ] = 65000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1280X720_60     ] = 74250 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1280X800_60     ] = 83500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1280X1024_60    ] = 108000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1360X768_60     ] = 85500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1366X768_60     ] = 85500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1400X1050_60    ] = 121750 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1440X900_60     ] = 106500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1440X900_60_RB  ] = 88750 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1600X900_60_RB  ] = 97750 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1600X1200_60    ] = 162000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1680X1050_60    ] = 146250 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1680X1050_60_RB ] = 119000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1920X1080_60    ] = 148500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1920X1200_60    ] = 193250 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_1920X1440_60    ] = 234000 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_2048X1152_60    ] = 156750;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_2560X1440_60_RB ] = 241500 ;
    u32PixelClk[ HI_UNF_ENC_FMT_VESA_2560X1600_60_RB ] = 268500 ;

    u32PixelClk[ HI_UNF_ENC_FMT_3840X2160_24 ] =  297000 ;          /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_3840X2160_25 ] =  297000 ;          /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_3840X2160_30 ] =  297000 ;          /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_4096X2160_24 ] =  297000 ;          /* fmt_PixelClk * (999 / 1000) */

    u32PixelClk[ HI_UNF_ENC_FMT_3840X2160_23_976   ] =  296703 ;    /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_3840X2160_29_97    ] =  296703 ;    /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_720P_59_94         ] =  74176 ;     /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_59_94        ] =  148352 ;    /* fmt_PixelClk * (999 / 1000) */    
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_29_97        ] =  74176 ;     /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_1080P_23_976       ] =  74176 ;     /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_1080i_59_94        ] =  74176 ;     /* fmt_PixelClk * (999 / 1000) */
    u32PixelClk[ HI_UNF_ENC_FMT_BUTT               ] =  0 ;
}

HI_CHAR *hdmi_GetProcArg(HI_CHAR *chCmd, HI_CHAR *chArg, HI_U32 u32ArgBufSize)
{
    HI_CHAR *chSrc = chCmd;
    HI_U32 i = 0;


    if(NULL == chCmd || NULL == chArg)
    {
        return NULL;
    }

     /* clear ' ' and '\n' */
    while(' ' == *chSrc || '\n' == *chSrc)
    {
        chSrc++;
    }

    /* copy char to dest */
    while('\0' != *chSrc && ' ' != *chSrc && '\n' != *chSrc)
    {
        *chArg++ = *chSrc++;
        if(i++ >= u32ArgBufSize)
        {
            COM_ERR("Error : The arg size is larger than the buffer size.\n");
            return NULL;
        }
    }
    *chArg= '\0';

    return chSrc;
}


//#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
/*****************************************************************************
 Prototype    : hdmi_Proc
 Description  : HDMI status in /proc/msp/hdmi0
 Input        : None
 Output       : None
 Return Value :
 Calls        :
*****************************************************************************/
static HI_S32 HDMI0_Proc(struct seq_file *p, HI_VOID *v)
{
    HI_U32 u32Reg, index, offset,u32DefHDMIMode;
    HDMI_ATTR_S                   stHDMIAttr;
    HDMI_VIDEO_ATTR_S            *pstVideoAttr;
    HDMI_AUDIO_ATTR_S            *pstAudioAttr;
    HDMI_APP_ATTR_S              *pstAppAttr;
    HI_UNF_HDMI_STATUS_S          stHdmiStatus;
    HI_UNF_HDMI_CEC_STATUS_S      CECStatus;
    HI_UNF_ENC_FMT_E              fmt;
    HI_U32                        u32PlayStatus = 0;
    HDMI_INT_Err_Conut            HDMI_INTErrCount = {0};
    HI_U32                        u32Nvalue, u32CTSvalue;
    HI_CHAR                       u8Buf[21] = {0};
    HI_U32 w,h;
    HI_S32 s32Temp, Ret = HI_SUCCESS;

    p += PROC_PRINT(p, "%s\n", g_pszVersion);
    p += PROC_PRINT(p, "--------------------------------- Hisi HDMI Dev Stat --------------------------------\n");
    Ret = DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_0, &stHDMIAttr);
    if(Ret != HI_SUCCESS)
    {
        p += PROC_PRINT(p, "HDMI driver do not Open\n" );
        p += PROC_PRINT(p, "----------------------------------------- END -----------------------------------------\n");
        return HI_SUCCESS;
    }
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV0, 0x08);// 0x72:0x08
    if ((u32Reg & 0x01) != 0x01)
    {
        p += PROC_PRINT(p, "HDMI do not Start!\n");
        p += PROC_PRINT(p, "----------------------------------------- END -----------------------------------------\n");
        return HI_SUCCESS;
    }
    pstVideoAttr = &stHDMIAttr.stVideoAttr;
    pstAudioAttr = &stHDMIAttr.stAudioAttr;
    pstAppAttr = &stHDMIAttr.stAppAttr;

    DRV_HDMI_GetStatus(HI_UNF_HDMI_ID_0,&stHdmiStatus);

    p += PROC_PRINT(p, "%-20s: ","Hotplug");
    if(stHdmiStatus.bConnected)
    {
        p += PROC_PRINT(p, "%-20s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-20s: ","Thread");
    s32Temp = (DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_0) && !DRV_Get_IsThreadStoped() && !SI_IsHDMIResetting()) ;
    if(s32Temp)
    {
        p += PROC_PRINT(p, "%s\n", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Disable");
    }

    p += PROC_PRINT(p, "%-20s: ","Sink");
    if(stHdmiStatus.bSinkPowerOn)
    {
        p += PROC_PRINT(p, "%-20s| ", "Active");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Deactive");
    }
    p += PROC_PRINT(p, "%-20s: ","HDCP Enable");
    if(pstAppAttr->bHDCPEnable)
    {
        p += PROC_PRINT(p, "%s\n", "ON");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "OFF");
    }

    p += PROC_PRINT(p, "%-20s: ","PHY Output");
    s32Temp = DRV_HDMI_ReadPhy();
    if(s32Temp)
    {
        p += PROC_PRINT(p, "%-20s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-20s: ","HDCP Encryption");
    s32Temp = DRV_ReadByte_8BA(0, TX_SLV0, 0x0F);
    if(s32Temp&0x01)
    {
        p += PROC_PRINT(p, "%s\n", "ON");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "OFF");
    }

    p += PROC_PRINT(p, "%-20s: ","Play Status");
    DRV_HDMI_GetPlayStatus(0,&u32PlayStatus);
    if(u32PlayStatus)
    {
        p += PROC_PRINT(p, "%-20s| ", "Start");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Stop");
    }
    p += PROC_PRINT(p, "%-20s: ","HDCP KEY CRC");
    s32Temp = DRV_ReadByte_8BA(0, TX_SLV0, 0xF9);
    
    if(pstAppAttr->bHDCPEnable)
    {
        p += PROC_PRINT(p, "%s",((s32Temp&0x03) == 0x01) ? "OK" : "Error");
    }
    else
    {
        p += PROC_PRINT(p, "%s", " - ");
    }
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "%-20s: ","EDID Status");
    if(DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
    {
        p += PROC_PRINT(p, "%-20s| ", "Valid");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Unvalid");   
    }
        
    p += PROC_PRINT(p, "%-20s: ","CEC Status");
    memset(&CECStatus, 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));
#ifdef CEC_SUPPORT
    DRV_HDMI_CECStatus(HI_UNF_HDMI_ID_0, &CECStatus);
#endif
    if(CECStatus.bEnable)
    {
        p += PROC_PRINT(p, "%s\n", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Disable");
    }

    p += PROC_PRINT(p, "%-20s: ","Output Mode");
    u32Reg = ReadByteHDMITXP1(0x2F);
    if(u32Reg & 0x01)
    {
        p += PROC_PRINT(p, "%-20s| ", "HDMI");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "DVI");
    }

    p += PROC_PRINT(p, "%-20s: ","CEC Phy Addr");

    p += PROC_PRINT(p, "%02d.%02d.%02d.%02d\n", CECStatus.u8PhysicalAddr[0],
        CECStatus.u8PhysicalAddr[1],CECStatus.u8PhysicalAddr[2],CECStatus.u8PhysicalAddr[3]);


    p += PROC_PRINT(p, "%-20s: ","Default Mode");
    u32DefHDMIMode = DRV_Get_DefaultOutputMode(HI_UNF_HDMI_ID_0);
    p += PROC_PRINT(p, "%-20s| ", g_pDefHDMIMode[u32DefHDMIMode]);
    p += PROC_PRINT(p, "%-20s: ","CEC Logical Addr");
    p += PROC_PRINT(p, "%d\n", CECStatus.u8LogicalAddr);


    p += PROC_PRINT(p, "%-20s: ","Force SetFmt Delay");
    if(IsForceFmtDelay())
    {
        p += PROC_PRINT(p, "%-20s| ", "Force");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Default");
    }

    p += PROC_PRINT(p, "%-20s: ","AVMUTE");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0xDF);  // 0x7A:0xDF
    if ( 0x10 == (u32Reg & 0x10))
    {
        p += PROC_PRINT(p, "%s ", "Disable");
    }
    else if ( 0x01 == (u32Reg & 0x01))
    {
        p += PROC_PRINT(p, "%s ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s ", "Unknown");
    }
    p += PROC_PRINT(p, "\n");


    p += PROC_PRINT(p, "%-20s: ","Rsen Detect Mode");
    if (DRV_Get_ForcePowerState())
    {
        p += PROC_PRINT(p, "%-20s| ", "Force do not detect Rsen");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Default");
    }
    
    
    p += PROC_PRINT(p, "%-20s: ","Force Mute Delay");
    if (IsForceMuteDelay())
    {
        p += PROC_PRINT(p, "%s ", "Force");
    }
    else
    {
        p += PROC_PRINT(p, "%s ", "Default");
    }
    p += PROC_PRINT(p, "\n");
    
    
    w = DRV_ReadByte_8BA(0, TX_SLV0, HRES_L);  
    w += (DRV_ReadByte_8BA(0, TX_SLV0, HRES_H)&0x1F)<<8; 
    h = DRV_ReadByte_8BA(0, TX_SLV0, VRES_L);  
    h += (DRV_ReadByte_8BA(0, TX_SLV0, VRES_H)&0x0F)<<8; 
    
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV0, INTERLACE_POL_DETECT);  
    
    p += PROC_PRINT(p, "%-20s: ","Out Timing");
    
    p += PROC_PRINT(p, "%dx%d",w,h);
    if (w|h)
    {
        p += PROC_PRINT(p, " %c H:%c V:%c ", (u32Reg&0x04)?'i':'p',(u32Reg&0x02)?'-':'+',(u32Reg&0x01)?'-':'+');
    }
    
    p += PROC_PRINT(p, "\n");


    p += PROC_PRINT(p, "---------------- Video -------------------|---------------- Audio -------------------\n");

    p += PROC_PRINT(p, "%-20s: ","Video Output ");
    if(pstAppAttr->bEnableVideo)
    {
        p += PROC_PRINT(p, "%-20s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-20s: ","AUD Output");
    if(pstAppAttr->bEnableAudio)
    {
        p += PROC_PRINT(p, "%s\n", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Disable");
    }

    p += PROC_PRINT(p, "%-20s: ","Current Fmt");
    if(pstVideoAttr->enVideoFmt < HI_DRV_DISP_FMT_BUTT)
    {
        p += PROC_PRINT(p, "%-20s| ", g_pDispFmtString[pstVideoAttr->enVideoFmt]);
    }
    else
    {
        p += PROC_PRINT(p, "err-%-16d| ", pstVideoAttr->enVideoFmt);
    }


    p += PROC_PRINT(p, "%-20s: ","Input Type");
    p += PROC_PRINT(p, "%s\n", g_pAudInputType[pstAudioAttr->enSoundIntf]);

    p += PROC_PRINT(p, "%-20s: ","Color Space");
    p += PROC_PRINT(p, "%-20s| ", g_pColorSpace[pstAppAttr->enVidOutMode]);
    p += PROC_PRINT(p, "%-20s: ","Sample Rate");
    p += PROC_PRINT(p, "%dHz\n", pstAudioAttr->enSampleRate);

    p += PROC_PRINT(p, "%-20s: ","DeepColor");
    p += PROC_PRINT(p, "%-20s| ", g_pDeepColor[pstAppAttr->enDeepColorMode]);
    p += PROC_PRINT(p, "%-20s: ","Bit Depth");
    p += PROC_PRINT(p, "%dbit\n", pstAudioAttr->enBitDepth);


    p += PROC_PRINT(p, "%-20s: ","xvYCC");
    if(pstAppAttr->bxvYCCMode)
    {
        p += PROC_PRINT(p, "%-20s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-20s: ","Trace Mode");
    if(pstAudioAttr->bIsMultiChannel)
    {
        p += PROC_PRINT(p, "%s\n", "Multichannel(8)");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Stereo");
    }

    p += PROC_PRINT(p, "%-20s: ","3D Mode");
    if(0 == pstVideoAttr->u83DParam)
    {
        p += PROC_PRINT(p, "%-20s| ", "FPK");
    }
    else if (8 == pstVideoAttr->u83DParam)
    {
        p += PROC_PRINT(p, "%-20s| ", "SBS HALF");
    }
    else if (6 == pstVideoAttr->u83DParam)
    {
        p += PROC_PRINT(p, "%-20s| ", "TAB");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "2D");
    }

    p += PROC_PRINT(p, "%-20s: ","N Value");
    u32Reg = ReadByteHDMITXP1(0x05);  // 0x7A:0x05
    u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x04);  // 0x7A:0x04
    u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x03);  // 0x7A:0x03
    p += PROC_PRINT(p, "0x%x(%d)\n",u32Reg,u32Reg);
    u32Nvalue = u32Reg;

    p += PROC_PRINT(p, "%-20s: ","Global SetFmt Delay");
    p += PROC_PRINT(p, "%-20d| ", GetGlobalFmtDelay());


    p += PROC_PRINT(p, "%-20s: ","CTS");
    u32Reg = ReadByteHDMITXP1(0x0b);  // 0x7A:0x0b
    u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x0a);  // 0x7A:0x0a
    u32Reg = (u32Reg<<8) | ReadByteHDMITXP1(0x09);  // 0x7A:0x09
    p += PROC_PRINT(p, "0x%x(%d)\n",u32Reg,u32Reg);
    u32CTSvalue = u32Reg;

    p += PROC_PRINT(p, "%-20s: ","Global Mute Delay");
    p += PROC_PRINT(p, "%-20d| ",GetGlobalsMuteDelay());    

    SI_GetErrCount(&HDMI_INTErrCount);
    p += PROC_PRINT(p, "%-20s: ","Audio FIFO Overflow");
    p += PROC_PRINT(p, "%-20d  ",HDMI_INTErrCount.overrun);
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "%-20s: ","TMDS Clock");
    //HI_OSAL_Snprintf(u8Buf, 20, "%d Hz", (128 * (pstAudioAttr->enSampleRate) / u32Nvalue) * u32CTSvalue);
    fmt = DRV_HDMI_Disp2UnfFmt(pstVideoAttr->enVideoFmt);
    HI_OSAL_Snprintf(u8Buf, 20, "%d Hz",  DRV_Calculate_TMDSCLK(fmt, pstAppAttr->enDeepColorMode) * 1000);
    p += PROC_PRINT(p, "%-20s| ", u8Buf);
    
    p += PROC_PRINT(p, "%-20s: ","Audio FIFO Underflow");
    p += PROC_PRINT(p, "%-20d  ",HDMI_INTErrCount.under_run);
    p += PROC_PRINT(p, "\n");

    u32Reg = ReadByteHDMITXP0(TX_STAT_ADDR);
    p += PROC_PRINT(p, "%-20s: ","Pixel Clk Stable");
    if((u32Reg & BIT_HDMI_PSTABLE)!=0)
    {
        p += PROC_PRINT(p, "%-20s| ","stable");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ","unstable");
    }
    
    p += PROC_PRINT(p, "%-20s: ","S/PDIF Parity Err");
    p += PROC_PRINT(p, "%-20d  ",HDMI_INTErrCount.spdif_par);
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "---------------------------------- Info Frame status --------------------------------\n");
    p += PROC_PRINT(p, "%-25s: ","AVI InfoFrame");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3E);  // 0x7A:0x3E
    if ( 0x03 == (u32Reg & 0x03))
    {
        p += PROC_PRINT(p, "%-15s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-15s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-23s: ","Gamut Metadata Packet");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3F);  // 0x7A:0x3F
    if(0xC0 == (u32Reg & 0xC0))
    {
        p += PROC_PRINT(p, "%s\n", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Disable");
    }

    p += PROC_PRINT(p, "%-25s: ","AUD InfoFrame");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3E);  // 0x7A:0x3E
    if ( 0x30 == (u32Reg & 0x30))
    {
        p += PROC_PRINT(p, "%-15s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-15s| ", "Disable");
    }
    p += PROC_PRINT(p, "%-23s: ","Generic Packet");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3F);  // 0x7A:0x3F
    if(0x03 == (u32Reg & 0x03))
    {
        p += PROC_PRINT(p, "%s\n", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Disable");
    }

    p += PROC_PRINT(p, "%-25s: ","MPg/VendorSpec InfoFrame");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x3E);  // 0x7A:0x3E
    if ( 0xC0 == (u32Reg & 0xC0))
    {
        p += PROC_PRINT(p, "%-15s| ", "Enable");
    }
    else
    {
        p += PROC_PRINT(p, "%-15s| ", "Disable");
    }
    p += PROC_PRINT(p, "\n");
    p += PROC_PRINT(p, "-------------------------------------- Raw Data -------------------------------------\n");
    p += PROC_PRINT(p, "AVI InfoFrame :\n");
    for(index = 0; index < 17; index ++)
    {
        offset = 0x40 + index;//0x7A:0x40
        u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
        p += PROC_PRINT(p, "0x%02x,", u32Reg);
    }
    p += PROC_PRINT(p, "\n");
    p += PROC_PRINT(p, "AUD InfoFrame :\n");
    for(index = 0; index < 9; index ++)
    {
        offset = 0x80 + index;//0x7A:0x80
        u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
        p += PROC_PRINT(p, "0x%02x,", u32Reg);
    }
    p += PROC_PRINT(p, "\n");
    p += PROC_PRINT(p, "MPg/VendorSpec Inforframe :\n");
    for(index = 0; index < 12; index ++)
    {
        offset = 0xa0 + index;//0x7A:0xA0
        u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, offset);
        p += PROC_PRINT(p, "0x%02x,", u32Reg);
    }
    p += PROC_PRINT(p, "\n");
    p += PROC_PRINT(p, "------------------------------------ Parsed InfoFrame -------------------------------\n");
    p += PROC_PRINT(p, "%-20s: ","Video ID Code(VIC)");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x47);
    p += PROC_PRINT(p, "0x%-18x| ", u32Reg);
    p += PROC_PRINT(p, "%-20s: ","Colorimetry");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x45);
    u32Reg = u32Reg>>6;
    if(!u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "No Data");
    }
    else if (0x1 == u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "ITU601");
    }
    else if (0x2 == u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "ITU709");
    }
    else
    {
        u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x46);
        u32Reg &= 0x70;
        if(u32Reg == 0)
        {
            p += PROC_PRINT(p, "%s\n", "xvYCC601");
        }
        else if(u32Reg == 0x10)
        {
            p += PROC_PRINT(p, "%s\n", "xvYCC709");
        }
        else
        {
            p += PROC_PRINT(p, "%s\n", "reserve");
        }
    }
    p += PROC_PRINT(p, "%-20s: ","Pixel Repetition");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x48);
    u32Reg &= 0x0F;
    if(u32Reg > 10)
    {
        //array overflow, set to reserved
        u32Reg = 10;
    }
    p += PROC_PRINT(p, "%-20s| ", g_pPixelRep[u32Reg]);
    p += PROC_PRINT(p, "%-20s: ","ScanInfo");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x44);
    u32Reg &= 0x03;
    p += PROC_PRINT(p, "%s\n", g_pScanInfo[u32Reg]);

    p += PROC_PRINT(p, "%-20s: ","Output Color Space");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x44);
    u32Reg &= 0x60;
    u32Reg >>=5;
    p += PROC_PRINT(p, "%-20s| ", g_pColorSpace[u32Reg]);
    p += PROC_PRINT(p, "%-20s: ","AspectRatio");
    u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x45);
    u32Reg &= 0x30;
    if(0x00 == u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "No Data");
    }
    else if (0x10 == u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "4:3");
    }
    else if (0x20 == u32Reg)
    {
        p += PROC_PRINT(p, "%s\n", "16:9");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "Future");
    }

    p += PROC_PRINT(p, "--------------------------------- Debug Command -------------------------------------\n");
    p += PROC_PRINT(p, "help informatin : HDMI Base Addr : 0x%08X \n", (HI_U32)HDMI_TX_BASE_ADDR);
    p += PROC_PRINT(p, "type 'echo help > /proc/msp/hdmi0' to get more help informatin \n");
    p += PROC_PRINT(p, "---------------------------------------- END ----------------------------------------\n");

    return HI_SUCCESS;
}

static HI_S32 HDMI0_Sink_Proc(struct seq_file *p, HI_VOID *v)
{
    HI_UNF_EDID_BASE_INFO_S *pSinkCap = DRV_Get_SinkCap(HI_UNF_HDMI_ID_0);
    HI_DRV_HDMI_AUDIO_CAPABILITY_S *pOldAudioCap = DRV_Get_OldAudioCap();
    HI_U32 i,j;

    p += PROC_PRINT(p, "--------------------------------- Hisi HDMI Sink Capability -------------------------\n");

    p += PROC_PRINT(p, "%-20s: ","EDID Status");
    if(DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
    {
        p += PROC_PRINT(p, "%-20s| ", "OK");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "Failed");
    }
    p += PROC_PRINT(p, "%-20s: ","TV Manufacture Name");
    p += PROC_PRINT(p, "%s\n", pSinkCap->stMfrsInfo.u8MfrsName);

    p += PROC_PRINT(p, "%-20s: ","Source of EDID");

    if(DRV_Get_IsUserEdid(HI_UNF_HDMI_ID_0))
    {
        p += PROC_PRINT(p, "%-20s| ","User Setting");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ","From Sink");
    }

    p += PROC_PRINT(p, "%-20s: ","ProductCode");
    p += PROC_PRINT(p, "%d\n", pSinkCap->stMfrsInfo.u32ProductCode);

    p += PROC_PRINT(p, "%-20s: ","Hdmi Support");
    if(pSinkCap->bSupportHdmi)
    {
        p += PROC_PRINT(p, "%-20s| ", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "FALSE");
    }
    p += PROC_PRINT(p, "%-20s: ","SerialNumber");
    p += PROC_PRINT(p, "%d\n", pSinkCap->stMfrsInfo.u32SerialNumber);

    p += PROC_PRINT(p, "%-20s: ","EDID Version");
    p += PROC_PRINT(p, "%d.%-18d| ", pSinkCap->u8Version,pSinkCap->u8Revision);
    p += PROC_PRINT(p, "%-20s: ","Week Of Manufacture");
    p += PROC_PRINT(p, "%d\n", pSinkCap->stMfrsInfo.u32Week);

    p += PROC_PRINT(p, "%-20s: ","Extend Block Num");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->u8ExtBlockNum);
    p += PROC_PRINT(p, "%-20s: ","Year Of Manufacture");
    p += PROC_PRINT(p, "%d\n", pSinkCap->stMfrsInfo.u32Year);

    p += PROC_PRINT(p, "%-20s: ","DVI Dual");
    if(pSinkCap->bSupportDVIDual)
    {
        p += PROC_PRINT(p, "%-20s| ", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "FALSE");
    }
    p += PROC_PRINT(p, "%-20s: ","CEC PhyAddr Valid");
    if(pSinkCap->stCECAddr.bPhyAddrValid)
    {
        p += PROC_PRINT(p, "%s\n", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "FALSE");
    }

    p += PROC_PRINT(p, "%-20s: ","Supports AI");
    if(pSinkCap->bSupportsAI)
    {
        p += PROC_PRINT(p, "%-20s| ", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "FALSE");
    }
    p += PROC_PRINT(p, "%-20s: ","CEC Phy Add");
    p += PROC_PRINT(p, "%02x.%02x.%02x.%02x\n", pSinkCap->stCECAddr.u8PhyAddrA,
        pSinkCap->stCECAddr.u8PhyAddrB, pSinkCap->stCECAddr.u8PhyAddrC, pSinkCap->stCECAddr.u8PhyAddrD);
    
    p += PROC_PRINT(p, "%-20s: %s\n","SinkName", pSinkCap->stMfrsInfo.u8pSinkName);
    
    p += PROC_PRINT(p, "-------------------------------------- Video ----------------------------------------\n");
    p += PROC_PRINT(p, "%-20s: ","Video Timing");
    for(i = 0,j = 0; i < HI_UNF_ENC_FMT_BUTT; i++)
    {
        if(pSinkCap->bSupportFormat[i])
        {
            p += PROC_PRINT(p, "%s / ", g_pUnfFmtString[i]);
            j++;
            if(0 == j%6)
            {
                p += PROC_PRINT(p, "\n%-22s","");
            }
        }
    }
    p += PROC_PRINT(p, "\n");
    p += PROC_PRINT(p, "%-20s: ","Native Format");
    p += PROC_PRINT(p, "%s\n", g_pUnfFmtString[pSinkCap->enNativeFormat]);
    p += PROC_PRINT(p, "%-20s: ","Colorimetry");
    if(pSinkCap->stColorMetry.bxvYCC601)
    {
        p += PROC_PRINT(p, "%s", "xvYCC601 / ");
    }
    if(pSinkCap->stColorMetry.bxvYCC709)
    {
        p += PROC_PRINT(p, "%s", "xvYCC709 / ");
    }
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "%-20s: ","Color Space");
    p += PROC_PRINT(p, "%s", "RGB444");
    if(pSinkCap->stColorSpace.bYCbCr444)
    {
        p += PROC_PRINT(p, " / %s", "YCbCr444");
    }

    if(pSinkCap->stColorSpace.bYCbCr422)
    {
        p += PROC_PRINT(p, " / %s", "YCbCr422");
    }

    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "%-20s: ","Deep Color");
    p += PROC_PRINT(p, "%s", "24");
    if(pSinkCap->stDeepColor.bDeepColor30Bit)
    {
        p += PROC_PRINT(p, " / %s", "30");
    }
    if(pSinkCap->stDeepColor.bDeepColor36Bit)
    {
        p += PROC_PRINT(p, " / %s", "36");
    }
    if(pSinkCap->stDeepColor.bDeepColor48Bit)
    {
        p += PROC_PRINT(p, " / %s", "48");
    }
    p += PROC_PRINT(p, "bit\n");

    p += PROC_PRINT(p, "%-20s: ","3D Support");
    if(pSinkCap->st3DInfo.bSupport3D)
    {
        p += PROC_PRINT(p, "%s\n","Support");
        p += PROC_PRINT(p, "%-20s: ","3D Type");
        for(i = 0;i < HI_UNF_EDID_3D_BUTT;i++)
        {
            if(pSinkCap->st3DInfo.bSupport3DType[i])
            {
                p += PROC_PRINT(p, "%s / ",g_p3DMode[i]);
            }
        }
    }
    else
    {
        p += PROC_PRINT(p, "%s\n","Not Support");
        p += PROC_PRINT(p, "%-20s: None","3D Type");
    }

    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "-------------------------------------- Audio ----------------------------------------\n");

    p += PROC_PRINT(p, "%-16s| ","Audio Fmt");
    p += PROC_PRINT(p, "%-15s| ","Chn");
    p += PROC_PRINT(p, "%-25s","samplerate");
    //p += PROC_PRINT(p, "%-10s","Extend");
    p += PROC_PRINT(p, "\n");

    for(i = 0; i < HI_UNF_EDID_MAX_AUDIO_CAP_COUNT; i++)
    {
        if(pSinkCap->stAudioInfo[i].enAudFmtCode)
        {
            p += PROC_PRINT(p, "%-3d %-12s| ",pSinkCap->stAudioInfo[i].enAudFmtCode, g_pAudioFmtCode[pSinkCap->stAudioInfo[i].enAudFmtCode]);
            p += PROC_PRINT(p, "%-15d| ",pSinkCap->stAudioInfo[i].u8AudChannel);

            for (j = 0; j < MAX_SAMPE_RATE_NUM; j++)
            {
                if(pSinkCap->stAudioInfo[i].enSupportSampleRate[j] != 0)
                {
                    p += PROC_PRINT(p, "%d ", (pSinkCap->stAudioInfo[i].enSupportSampleRate[j]));
                }
            }
            p += PROC_PRINT(p, "Hz");

            p += PROC_PRINT(p, "\n");
        }
    }

    p += PROC_PRINT(p, "\n%-10s : %d \n","Audio Info Num",pSinkCap->u32AudioInfoNum);
    p += PROC_PRINT(p, "\n%-10s : ","Speaker");


    for(i = 0; i < HI_UNF_EDID_AUDIO_SPEAKER_BUTT; i++)
    {
        if(pSinkCap->bSupportAudioSpeaker[i])
        {
            p += PROC_PRINT(p, "%s ",g_pSpeaker[i]);
        }
    }

    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "------------------------------------ Old Audio --------------------------------------\n");
    p += PROC_PRINT(p, "Audio Fmt support : ");
    for (i = 0; i < HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT; i++)
    {
        if (pOldAudioCap->bAudioFmtSupported[i] == HI_TRUE)
        {
             switch (i)
            {
            case 1:
                p += PROC_PRINT(p, "LiniarPCM ");
                break;
            case 2:
                p += PROC_PRINT(p, "AC3 ");
                break;
            case 3:
                p += PROC_PRINT(p, "MPEG1 ");
                break;
            case 4:
                p += PROC_PRINT(p, "MP3 ");
                break;
            case 5:
                p += PROC_PRINT(p, "MPEG2 ");
                break;
            case 6:
                p += PROC_PRINT(p, "ACC ");
                break;
            case 7:
                p += PROC_PRINT(p, "DTS ");
                break;
            case 8:
                p += PROC_PRINT(p, "ATRAC ");
                break;
            case 9:
                p += PROC_PRINT(p, "OneBitAudio ");
                break;
            case 10:
                p += PROC_PRINT(p, "DD+ ");
                break;
            case 11:
                p += PROC_PRINT(p, "DTS_HD ");
                break;
            case 12:
                p += PROC_PRINT(p, "MAT ");
                break;
            case 13:
                p += PROC_PRINT(p, "DST ");
                break;
            case 14:
                p += PROC_PRINT(p, "WMA ");
                break;
            default:
                p += PROC_PRINT(p, "reserved ");
                break;
            }
        }
    }
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "Max Audio PCM channels: %d\n", pOldAudioCap->u32MaxPcmChannels);
    p += PROC_PRINT(p, "Support Audio Sample Rates:");
    for (i = 0; i < MAX_SAMPE_RATE_NUM; i++)
    {
        if(pOldAudioCap->u32AudioSampleRateSupported[i] != 0)
        {
            p += PROC_PRINT(p, " %d ", pOldAudioCap->u32AudioSampleRateSupported[i]);
        }
    }
    p += PROC_PRINT(p, "\n");

    p += PROC_PRINT(p, "------------------------------ Custom Perfer Timing ---------------------------------\n");

    p += PROC_PRINT(p, "%-20s: ","VFB");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->stPerferTiming.u32VFB);

    p += PROC_PRINT(p, "%-20s: ","HFB");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32HFB);

    p += PROC_PRINT(p, "%-20s: ","VBB");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->stPerferTiming.u32VBB);

    p += PROC_PRINT(p, "%-20s: ","HBB");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32HBB);

    p += PROC_PRINT(p, "%-20s: ","VACT");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->stPerferTiming.u32VACT);

    p += PROC_PRINT(p, "%-20s: ","HACT");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32HACT);

    p += PROC_PRINT(p, "%-20s: ","VPW");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->stPerferTiming.u32VPW);

    p += PROC_PRINT(p, "%-20s: ","HPW");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32HPW);


    p += PROC_PRINT(p, "%-20s: ","IDV");
    if(pSinkCap->stPerferTiming.bIDV)
    {
        p += PROC_PRINT(p, "%-20s| ", "TRUE");    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "FALSE");
    }

    p += PROC_PRINT(p, "%-20s: ","IHS");
    if(pSinkCap->stPerferTiming.bIHS)
    {
        p += PROC_PRINT(p, "%s\n", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%s\n", "FALSE");
    }


    p += PROC_PRINT(p, "%-20s: ","IVS");
    if(pSinkCap->stPerferTiming.bIVS)
    {
        p += PROC_PRINT(p, "%-20s| \n", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| \n", "FALSE");
    }


    p += PROC_PRINT(p, "%-20s: ","Image Width");
    p += PROC_PRINT(p, "%-20d| ", pSinkCap->stPerferTiming.u32ImageWidth);

    p += PROC_PRINT(p, "%-20s: ","Image Height");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32ImageHeight);


    p += PROC_PRINT(p, "%-20s: ","Interlace");

    if(pSinkCap->stPerferTiming.bInterlace)
    {
        p += PROC_PRINT(p, "%-20s| ", "TRUE");
    }
    else
    {
        p += PROC_PRINT(p, "%-20s| ", "FALSE");
    }

    p += PROC_PRINT(p, "%-20s: ","Pixel Clock");
    p += PROC_PRINT(p, "%d\n",  pSinkCap->stPerferTiming.u32PixelClk);

    p += PROC_PRINT(p, "------------------------------------ EDID Raw Data ---------------------------------- \n");

    if(!DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
    {
        p += PROC_PRINT(p, "!! Data unbelievably !! \n");
    }
    //no else
    {
        HI_U32 index,u32EdidLegth = 0;
        HI_U8  Data[512];

        memset(Data, 0, 512);

        SI_Proc_ReadEDIDBlock(Data, &u32EdidLegth);
        for (index = 0; index < u32EdidLegth; index ++)
        {
            p += PROC_PRINT(p, "  %02x", Data[index]);
            if (0 == ((index + 1) % 16))
            {
                p += PROC_PRINT(p, "\n");
            }
        }
    }

    p += PROC_PRINT(p, "---------------------------------------- END ----------------------------------------\n");

    return HI_SUCCESS;
}
#endif


#if (1 == HI_PROC_SUPPORT)
extern HI_U32 unStableTimes;
HI_S32 hdmi_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{


    //struct seq_file   *p = file->private_data;
    //DRV_PROC_ITEM_S  *pProcItem = s->private;
    HI_CHAR  chCmd[60] = {0};
    HI_CHAR  chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg2[DEF_FILE_NAMELENGTH] = {0};

#ifdef HDMI_DEBUG
    HI_CHAR  chArg3[DEF_FILE_NAMELENGTH] = {0};  //路径
#endif

    HI_CHAR  *chPtr = NULL;

    if(count > 40)
    {
        HI_DRV_PROC_EchoHelper("Error:Echo too long.\n");
        return HI_FAILURE;
    }

    if(copy_from_user(chCmd,buf,count))
    {
        HI_DRV_PROC_EchoHelper("copy from user failed\n");
        return HI_FAILURE;
    }


    chPtr = hdmi_GetProcArg(chCmd, chArg1, DEF_FILE_NAMELENGTH - 1);
    chPtr = hdmi_GetProcArg(chPtr, chArg2, DEF_FILE_NAMELENGTH - 1);


#ifdef HDMI_DEBUG
    chPtr = hdmi_GetProcArg(chPtr, chArg3, DEF_FILE_NAMELENGTH - 1);
#endif
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(chPtr[0] != ' ' && chPtr != NULL)
    {
        //for avoid TQE:unused val 'chPtr';
        HI_DRV_PROC_EchoHelper("Exist unused param \n");
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(chPtr == NULL)
    {
        //for avoid TQE:unused val 'chPtr';
        HI_DRV_PROC_EchoHelper("param len over Max namelen \n");
    }


    //sw reset
    if(!HI_OSAL_Strncmp(chArg1,"swrst",DEF_FILE_NAMELENGTH))
    {
        HI_DRV_PROC_EchoHelper("hdmi resetting... ... ... \n");
        SI_SW_ResetHDMITX();
    }
    //invert tmds clock
    else if(!HI_OSAL_Strncmp(chArg1,"tclk",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("hdmi TmdsClk invert...  \n");
            WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("hdmi TmdsClk not invert... \n");
            WriteByteHDMITXP1(0x3d,0x17);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"mute",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("mute...  \n");
            DRV_HDMI_SetAVMute(0,HI_TRUE);
            //WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("unmute... \n");
            DRV_HDMI_SetAVMute(0,HI_FALSE);
            //WriteByteHDMITXP1(0x3d,0x17);
        }
    }
    //else if(chArg1[0] == '3'&& chArg1[1] == 'd')
    else if(!HI_OSAL_Strncmp(chArg1,"3d",DEF_FILE_NAMELENGTH))
    {
        HDMI_ATTR_S stAttr;
        DRV_HDMI_GetAttr(0,&stAttr);
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("3d mode disable...  \n");
            //HI_DRV_HDMI_Set3DMode(0,HI_FALSE,HI_UNF_3D_MAX_BUTT);
            stAttr.stVideoAttr.b3DEnable = HI_FALSE;
            stAttr.stVideoAttr.u83DParam = HI_UNF_EDID_3D_BUTT;
            //WriteByteHDMITXP1(0x3d,0x1f);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"fp",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("Frame Packing... \n");
            //HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_FRAME_PACKETING);
            stAttr.stVideoAttr.b3DEnable = HI_TRUE;
            stAttr.stVideoAttr.u83DParam = HI_UNF_EDID_3D_FRAME_PACKETING;
            //WriteByteHDMITXP1(0x3d,0x17);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"sbs",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("Side by side(half)... \n");
            //HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_SIDE_BY_SIDE_HALF);
            stAttr.stVideoAttr.b3DEnable = HI_TRUE;
            stAttr.stVideoAttr.u83DParam = HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF;
            //WriteByteHDMITXP1(0x3d,0x17);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"tab",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("Top and bottom... \n");
            //HI_DRV_HDMI_Set3DMode(0,HI_TRUE,HI_UNF_3D_TOP_AND_BOTTOM);
            stAttr.stVideoAttr.b3DEnable = HI_TRUE;
            stAttr.stVideoAttr.u83DParam = HI_UNF_EDID_3D_TOP_AND_BOTTOM;
            //WriteByteHDMITXP1(0x3d,0x17);
        }
        DRV_HDMI_SetAttr(0,&stAttr);
    }
    else if(!HI_OSAL_Strncmp(chArg1,"cbar",DEF_FILE_NAMELENGTH))
    {
        HI_U32 u32Reg = 0;
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("colorbar disable...  \n");
            DRV_HDMI_ReadRegister(VDP_DHD_0_CTRL,&u32Reg);
            u32Reg = u32Reg & (~0x70000000);
            DRV_HDMI_WriteRegister(VDP_DHD_0_CTRL,(u32Reg | 0x1));
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("colorbar enable.. \n");
            DRV_HDMI_ReadRegister(VDP_DHD_0_CTRL,&u32Reg);
            u32Reg = u32Reg | 0x70000000;
            DRV_HDMI_WriteRegister(VDP_DHD_0_CTRL,(u32Reg | 0x1));
        }
    }
    // 0x00 0xff 0xff yellow
    // 0x00 0xff 0x00 green
    // 0xff 0x00 0x00 blue
    // 0x00 0x00 0xff red
    // 0x80 0x10 0x80 black
    // white
    else if(!HI_OSAL_Strncmp(chArg1,"vblank",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("vblank disable...  \n");
            WriteByteHDMITXP0(0x0d,0x00);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"black",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("vblank black.. \n");
            WriteByteHDMITXP0(0x0d,0x04);
            WriteByteHDMITXP0(0x4b,0x80);
            WriteByteHDMITXP0(0x4c,0x10);
            WriteByteHDMITXP0(0x4d,0x80);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"red",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("vblank red.. \n");
            WriteByteHDMITXP0(0x0d,0x04);
            WriteByteHDMITXP0(0x4b,0x00);
            WriteByteHDMITXP0(0x4c,0x00);
            WriteByteHDMITXP0(0x4d,0xff);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"green",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("vblank green.. \n");
            WriteByteHDMITXP0(0x0d,0x04);
            WriteByteHDMITXP0(0x4b,0x00);
            WriteByteHDMITXP0(0x4c,0xff);
            WriteByteHDMITXP0(0x4d,0x00);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"blue",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("vblank blue.. \n");
            WriteByteHDMITXP0(0x0d,0x04);
            WriteByteHDMITXP0(0x4b,0xff);
            WriteByteHDMITXP0(0x4c,0x00);
            WriteByteHDMITXP0(0x4d,0x00);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"enc",DEF_FILE_NAMELENGTH))
    {
        if(!HI_OSAL_Strncmp(chArg2,"phy",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("encode by phy \n");
            WriteByteHDMITXP1(0x3c,0x08);
            SI_TX_PHY_WriteRegister(0x0d,0x00);
        }
        else if(!HI_OSAL_Strncmp(chArg2,"ctrl",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("encode by ctrl \n");
            WriteByteHDMITXP1(0x3c,0x00);
            SI_TX_PHY_WriteRegister(0x0d,0x01);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"pclk",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("pclk nobypass \n");
            WriteByteHDMITXP1(0x3c,0x08);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("pclk bypass \n");
            WriteByteHDMITXP1(0x3c,0x28);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"audio",DEF_FILE_NAMELENGTH))
    {
        HDMI_AUDIO_ATTR_S stHDMIAOAttr;
        memset((void*)&stHDMIAOAttr, 0, sizeof(HDMI_AUDIO_ATTR_S));
        DRV_HDMI_GetAOAttr(0,&stHDMIAOAttr);

        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("audio I2S \n");
            stHDMIAOAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_I2S;
            DRV_HDMI_AudioChange(0,&stHDMIAOAttr);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("audio SPDIF \n");
            stHDMIAOAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_SPDIF;
            DRV_HDMI_AudioChange(0,&stHDMIAOAttr);
        }
        else if(chArg2[0] == '2')
        {
            HI_DRV_PROC_EchoHelper("audio HBR \n");
            stHDMIAOAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_HBR;
            DRV_HDMI_AudioChange(0,&stHDMIAOAttr);
        }
        else if(chArg2[0] == '3')
        {
            HI_DRV_PROC_EchoHelper("audio pcm 8ch 192Khz \n");
            stHDMIAOAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_I2S;
            stHDMIAOAttr.bIsMultiChannel = HI_TRUE;
            stHDMIAOAttr.u32Channels = 8;
            stHDMIAOAttr.enSampleRate = HI_UNF_SAMPLE_RATE_192K;
            DRV_HDMI_AudioChange(0,&stHDMIAOAttr);
        }
        else
        {
            HI_DRV_PROC_EchoHelper("not supported \n");
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"thread",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("thread stop \n");
            DRV_Set_ThreadStop(HI_TRUE);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("thread start \n");
            DRV_Set_ThreadStop(HI_FALSE);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"cec",DEF_FILE_NAMELENGTH))
    {
#if defined (CEC_SUPPORT)
        if(chArg2[0] == '0')
        {
            HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();
            //cec_enable_flag = 0;
            SI_CEC_Close();
            DRV_Set_CECEnable(HI_UNF_HDMI_ID_0,HI_FALSE);
            DRV_Set_CECStart(HI_UNF_HDMI_ID_0,HI_FALSE);
            pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount = 0;
            memset(&(pstChnAttr[HI_UNF_HDMI_ID_0].stCECStatus), 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));
        }
        else if(chArg2[0] == '1')
        {
            SI_CEC_SetUp();
            DRV_Set_CECEnable(HI_UNF_HDMI_ID_0,HI_TRUE);
        }
#else
        HI_DRV_PROC_EchoHelper("do not support cec \n");
#endif
    }
    else if (!HI_OSAL_Strncmp(chArg1,"setAttr",DEF_FILE_NAMELENGTH))
    {
        HDMI_ATTR_S *pstHDMIAttr = DRV_Get_HDMIAttr(HI_UNF_HDMI_ID_0);
        DRV_Set_ForceUpdateFlag(HI_UNF_HDMI_ID_0,HI_TRUE);
        DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_0, pstHDMIAttr);
    }
    else if (!HI_OSAL_Strncmp(chArg1,"check",DEF_FILE_NAMELENGTH))
    {
        HI_U32 u32Reg = 0;
        if(!HI_OSAL_Strncmp(chArg2,"timing",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("Check timing... \n");
            u32Reg = ReadByteHDMITXP0(TX_STAT_ADDR);

            if((u32Reg & BIT_HDMI_PSTABLE)!=0)
            {
                HI_DRV_PROC_EchoHelper("Pixel Clk      : stable \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Pixel Clk      : !!Warnning!! Clock Unstable\n");
            }

            HI_DRV_PROC_EchoHelper("Unstable Times : %d \n",unStableTimes);

            u32Reg = ReadByteHDMITXP0(0x3b);
            u32Reg = (u32Reg << 8) | ReadByteHDMITXP0(0x3a);
            HI_DRV_PROC_EchoHelper("H Total        : %d ( 0x%x )\n",u32Reg,u32Reg);

            u32Reg = ReadByteHDMITXP0(0x3d);
            u32Reg = (u32Reg << 8) | ReadByteHDMITXP0(0x3c);

            HI_DRV_PROC_EchoHelper("V Total        : %d ( 0x%x )\n",u32Reg,u32Reg);

            u32Reg = ReadByteHDMITXP0(INTERLACE_POL_DETECT);
            if((u32Reg & BIT_I_DETECTR)!=0)
            {
                HI_DRV_PROC_EchoHelper("InterlaceDetect: interlace\n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("InterlaceDetect: progress\n");
            }

            u32Reg = ReadByteHDMITXP1(DIAG_PD_ADDR);

            HI_DRV_PROC_EchoHelper("Power State    : 0x%02x\n",u32Reg);


        }
        else if(!HI_OSAL_Strncmp(chArg2,"ddc",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("check ddc... \n");

            u32Reg = ReadByteHDMITXP0(DDC_DELAY_CNT);
            HI_DRV_PROC_EchoHelper("DDC Delay Count : 0x%02x(%d)\n",u32Reg,u32Reg);

            if(u32Reg > 0)
            {
                //60Mhz osc clk
                HI_DRV_PROC_EchoHelper("DDC Speed in calc : %dHz\n",(OSC_CLK_SELECT / (u32Reg * 35)));
            }
            else
            {
                //60Mhz osc clk
                HI_DRV_PROC_EchoHelper("DDC Speed in calc : 0Hz\n");
            }

            u32Reg = ReadByteHDMITXP0(DDC_STATUS);

            if(u32Reg & BIT_MDDC_ST_I2C_LOW)
            {
                HI_DRV_PROC_EchoHelper("I2C transaction did not start because I2C bus is pulled LOW by an external device \n");
            }

            if(u32Reg & BIT_MDDC_ST_NO_ACK)
            {
                HI_DRV_PROC_EchoHelper("HDMI Transmitter did not receive an ACK from slave device during address or data write \n");
            }
            if(u32Reg & BIT_MDDC_ST_IN_PROGR)
            {
                HI_DRV_PROC_EchoHelper("DDC operation in progress \n");
            }

            if(u32Reg & BIT_MDDC_ST_FIFO_FULL)
            {
                HI_DRV_PROC_EchoHelper("DDC FIFO Full \n");
            }

            if(u32Reg & BIT_MDDC_ST_FIFO_EMPL)
            {
                HI_DRV_PROC_EchoHelper("DDC FIFO Empty \n");
            }

            if(u32Reg & BIT_MDDC_ST_FRD_USE)
            {
                HI_DRV_PROC_EchoHelper("DDC FIFO Read In Use \n");
            }

            if(u32Reg & BIT_MDDC_ST_FWT_USE)
            {
                HI_DRV_PROC_EchoHelper("DDC FIFO Write In Use \n");
            }
        }
        else if(!HI_OSAL_Strncmp(chArg2,"color",DEF_FILE_NAMELENGTH))
        {
            //HI_DRV_PROC_EchoHelper("not supported... \n");
            HDMI_APP_ATTR_S  *pstAppAttr = DRV_Get_AppAttr(HI_UNF_HDMI_ID_0);

            if(pstAppAttr->bEnableHdmi)
            {
                HI_DRV_PROC_EchoHelper("ATTR HDMI Mode : HDMI \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("ATTR HDMI Mode : DVI \n");
            }

            if(pstAppAttr->bEnableAviInfoFrame)
            {
                HI_DRV_PROC_EchoHelper("ATTR AVI InfoFrame : Enable \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("ATTR AVI InfoFrame : Disable \n");
            }

            if(pstAppAttr->enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_RGB444)
            {
                HI_DRV_PROC_EchoHelper("VIDEO Color Mode : RGB444 \n");
            }
            else if(pstAppAttr->enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR444)
            {
                HI_DRV_PROC_EchoHelper("VIDEO Color Mode : YCbCr444 \n");
            }
            else if(pstAppAttr->enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
            {
                HI_DRV_PROC_EchoHelper("VIDEO Color Mode : YCBCR422(unsupport) \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("VIDEO Color Mode : Unknown Mode(ERR) \n");
            }

            HI_DRV_PROC_EchoHelper("InfoFrame Color Space:");
            u32Reg = DRV_ReadByte_8BA(0, TX_SLV1, 0x44);
            u32Reg &= 0x60;
            u32Reg >>=5;
            HI_DRV_PROC_EchoHelper("%s \n", g_pColorSpace[u32Reg]);

            u32Reg = ReadByteHDMITXP0(TX_VID_CTRL_ADDR);

            if(u32Reg & BIT_VID_CTRL_CSCSEL)
            {
                HI_DRV_PROC_EchoHelper("csc Standard select : BT709 \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("csc Standard select : BT601 \n");
            }


            u32Reg = ReadByteHDMITXP0(VID_ACEN_ADDR);

            if(u32Reg & BIT_VID_ACEN_RGB2YCBCR)
            {
                HI_DRV_PROC_EchoHelper("csc Rgb=>Yuv : Open \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("csc Rgb=>Yuv : close \n");
            }

            u32Reg = ReadByteHDMITXP0(TX_VID_MODE_ADDR);

            if(u32Reg & BIT_TX_CSC)
            {
                HI_DRV_PROC_EchoHelper("csc Yuv=>Rgb : Open \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("csc Yuv=>Rgb : close \n");
            }

            if(u32Reg & BIT_TX_DITHER)
            {
                HI_DRV_PROC_EchoHelper("dither : Open \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("dither : close \n");
            }

            if(u32Reg & BIT_TX_DEMUX_YC )
            {
                HI_DRV_PROC_EchoHelper("One- to Two Demux : Open \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("One- to Two Demux : close \n");
            }

        }
        else if(!HI_OSAL_Strncmp(chArg2,"ro",DEF_FILE_NAMELENGTH))
        {
            HI_DRV_PROC_EchoHelper("not supported... \n");
        }
        else if(!HI_OSAL_Strncmp(chArg2,"phy",DEF_FILE_NAMELENGTH))
        {
            // unknown DM_TX_CTRL2
            SI_TX_PHY_ReadRegister(0x06,&u32Reg);
            if(u32Reg != 0x89)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x06 : 0x%02x(0x89) \n",u32Reg);
            }

            // unknown DM_TX_CTRL3
            SI_TX_PHY_ReadRegister(0x07,&u32Reg);
            if(u32Reg != 0x81)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x07 : 0x%02x(0x81) \n",u32Reg);
            }

            // unknown DM_TX_CTRL5
            SI_TX_PHY_ReadRegister(0x09,&u32Reg);
            HI_DRV_PROC_EchoHelper("Phy Reg 0x09 : 0x%02x \n",u32Reg);

            // unknown BIAS_GEN_CTRL1
            SI_TX_PHY_ReadRegister(0x0a,&u32Reg);
            if(u32Reg != 0x07)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x0a : 0x%02x(0x07) \n",u32Reg);
            }

            // unknown BIAS_GEN_CTRL2
            SI_TX_PHY_ReadRegister(0x0b,&u32Reg);
            if(u32Reg != 0x51)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x0b : 0x%02x(0x51) \n",u32Reg);
            }

            //unknown DM_TX_CTRL4
            SI_TX_PHY_ReadRegister(0x08,&u32Reg);
            if(u32Reg != 0x40)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x08 : 0x%02x(0x40) \n",u32Reg);
            }

            // enc_bypass == nobypass
            SI_TX_PHY_ReadRegister(0x0d,&u32Reg);
            if(u32Reg != 0x00)
            {
                HI_DRV_PROC_EchoHelper("Phy Reg 0x0d : 0x%02x(0x00) \n",u32Reg);
            }


            // term_en && cap_ctl  // term_en 先关掉
            SI_TX_PHY_ReadRegister(0x0e,&u32Reg);
            if(u32Reg & 0x01)
            {
                HI_DRV_PROC_EchoHelper("term_en : Enable  \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("term_en : Disable  \n");
            }

            if(u32Reg & 0x02)
            {
                HI_DRV_PROC_EchoHelper("cap_ctl : Enable (recommond to disable)  \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("cap_ctl : Disable  \n");
            }

            // pll ctrl -deep color
            SI_TX_PHY_ReadRegister(0x02,&u32Reg);
            if((u32Reg & 0x03) == 0x00)
            {
                HI_DRV_PROC_EchoHelper("dpcolor_ctl  : 8bit  \n");
            }
            else if((u32Reg & 0x03) == 0x01)
            {
                HI_DRV_PROC_EchoHelper("dpcolor_ctl  : 10bit  \n");
            }
            else if((u32Reg & 0x03) == 0x02)
            {
                HI_DRV_PROC_EchoHelper("dpcolor_ctl  : 12bit  \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("dpcolor_ctl  : invalid  \n");
            }

            // oe && pwr_down
            SI_TX_PHY_ReadRegister(0x05,&u32Reg);
            if(u32Reg & 0x10)
            {
                HI_DRV_PROC_EchoHelper("Phy No power Down \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Phy will be power down \n");
            }

            if(u32Reg & 0x20)
            {
                HI_DRV_PROC_EchoHelper("Phy Outputs enable\n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Phy Outputs Disable\n");
            }

            SI_TX_PHY_ReadRegister(0x0c,&u32Reg);
            if(u32Reg & 0x01)
            {
                HI_DRV_PROC_EchoHelper("receiver is connected \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("receiver is dis-connected \n");
            }

            if(u32Reg & 0x02)
            {
                HI_DRV_PROC_EchoHelper("Clock detected > 2.5Mhz \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("No clock detected \n");
            }

        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"fmtforce",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("Default Fmt Delay\n");
            SetForceDelayMode(HI_FALSE,IsForceMuteDelay());
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("Force Fmt Delay\n");
            SetForceDelayMode(HI_TRUE,IsForceMuteDelay());
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"muteforce",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("Default mute Delay\n");
            SetForceDelayMode(IsForceFmtDelay(),HI_FALSE);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("Force Delay\n");
            SetForceDelayMode(IsForceFmtDelay(),HI_TRUE);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"RsenMode",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        { 
            HI_DRV_PROC_EchoHelper("Default detect Rsen\n");
            DRV_Set_ForcePowerState(0);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("Force do not detect Rsen\n");
            DRV_Set_ForcePowerState(1);
            
        }
    }
    
    else if(!HI_OSAL_Strncmp(chArg1,"fmtdelay",DEF_FILE_NAMELENGTH))
    {
        HI_U32 delay;

        delay = simple_strtol(chArg2, NULL, 10);

        if(delay <= 10000)
        {
            HI_DRV_PROC_EchoHelper("Set Foramt Delay %d ms\n",delay);
            SetGlobalFmtDelay(delay);
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Out of range[0-10000] %d ms\n",delay);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"mutedelay",DEF_FILE_NAMELENGTH))
    {
        HI_U32 delay;

        delay = simple_strtol(chArg2, NULL, 10);

        if(delay <= 10000)
        {
            HI_DRV_PROC_EchoHelper("Avmut Delay %d ms\n",delay);
            SetGlobalMuteDelay(delay);
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Out of range[0-10000] %d ms\n",delay);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"oe",DEF_FILE_NAMELENGTH))
    {
        //if(SI_TX_PHY_GetOutPutEnable())
        if(!HI_OSAL_Strncmp(chArg2,"0",DEF_FILE_NAMELENGTH))
        {
            //DRV_HDMI_SetAVMute(0,HI_TRUE);
            SI_TX_PHY_DisableHdmiOutput();
            HI_DRV_PROC_EchoHelper("OE Close...\n");
        }
        else if(!HI_OSAL_Strncmp(chArg2,"1",DEF_FILE_NAMELENGTH))
        {
            SI_TX_PHY_EnableHdmiOutput();
            //DRV_HDMI_SetAVMute(0,HI_FALSE);
            HI_DRV_PROC_EchoHelper("OE Open...\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("\necho oe [1]/[0]     >   /proc/msp/hdmi0\n\n");
            HI_DRV_PROC_EchoHelper("Please Input OE Status Param\n\n");
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"ddc",DEF_FILE_NAMELENGTH))
    {
        HI_U32 delay;

        delay = simple_strtol(chArg2, NULL, 10);

        if(delay <= 127)
        {
            if(delay != 0)
            {
                HI_DRV_PROC_EchoHelper("DDC Speed nearly %d hz\n",(OSC_CLK_SELECT / (delay * 35)));
            }
            else
            {
                HI_DRV_PROC_EchoHelper("DDC Speed 0 hz\n");
            }

            DRV_Set_DDCSpeed(delay);
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Delay Count Out of range[0-127] %d\n",delay);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1,"edid",DEF_FILE_NAMELENGTH))
    {
        HI_U32 edidNum = DRV_Get_DebugEdidNum();
        HI_U32 edidIndex = 0;

        edidIndex = simple_strtol(chArg2, NULL, 10);

        if((edidIndex > 0) && (edidIndex <= edidNum))
        {
            HDMI_EDID_S stEDID;
            HDMI_Test_EDID_S *pEdidTmp = DRV_Get_DebugEdid(edidIndex);



            DRV_Set_UserEdidMode(HI_UNF_HDMI_ID_0,HI_TRUE);

            memset(&stEDID,0,sizeof(HDMI_EDID_S));
            memcpy(stEDID.u8Edid,pEdidTmp->u8Edid,pEdidTmp->bEdidLen);
            stEDID.u32Edidlength = pEdidTmp->bEdidLen;

            DRV_Set_UserEdid(HI_UNF_HDMI_ID_0, &stEDID);

            HI_DRV_PROC_EchoHelper("load UserSetting EDID:%d success,please swrst \n",edidIndex);
        }
        else
        {
            if(edidIndex == 0)
            {
                DRV_Set_UserEdidMode(HI_UNF_HDMI_ID_0,HI_FALSE);
                HI_DRV_PROC_EchoHelper("Stop UserSettting EDID Mode \n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Index:[%d] Out of Edid Range [%d] \n",edidIndex,edidNum);
            }

            HI_DRV_PROC_EchoHelper("----------------------- edidIndex %02d Out of range[1-%02d] ---------------------------\n",edidIndex,edidNum);
            HI_DRV_PROC_EchoHelper("01. normal test\n");
            HI_DRV_PROC_EchoHelper("02. for edid test block 4\n");
            HI_DRV_PROC_EchoHelper("03. alter from edid test block 4,ext num err,and vsdb in block 3\n");
            HI_DRV_PROC_EchoHelper("04. extend version 2.4\n");
            HI_DRV_PROC_EchoHelper("05. unknown,maybe test parse edid some function\n");
            HI_DRV_PROC_EchoHelper("06. audio Amplifier 1 \n");
            HI_DRV_PROC_EchoHelper("07. ony dvi device, extend 2.1 detailed timing \n");
            HI_DRV_PROC_EchoHelper("08. extend num && header err\n");
            HI_DRV_PROC_EchoHelper("09. unknown \n");
            HI_DRV_PROC_EchoHelper("10. audio Amplifier 2 \n");
            HI_DRV_PROC_EchoHelper("11. hdcp test quantumdata HDMI \n");
            HI_DRV_PROC_EchoHelper("12. hdcp test quantumdata DVI \n");
            HI_DRV_PROC_EchoHelper("13. hitachi no vsdb(in some tv,some byte has broken) && block 2 crc err && support audio \n");
            HI_DRV_PROC_EchoHelper("14. all 0xff and 0x7f,err edid, can read 2 block \n");
            HI_DRV_PROC_EchoHelper("15. all 0xff and 0x7f,err edid, can read 4 block \n");
            HI_DRV_PROC_EchoHelper("16. 2block 1block ok,2block all 0xff(0x7f) \n");
            HI_DRV_PROC_EchoHelper("17. 4block 1block ok,2/3/4block all 0xff(0x7f) \n");
            HI_DRV_PROC_EchoHelper("18. 2block and 1st block header all 0xff,crc ok \n");
            HI_DRV_PROC_EchoHelper("19. dvi 1 block 0 extend ; crc ok \n");
            HI_DRV_PROC_EchoHelper("20. dvi 1 block extend != 0 ; crc error \n");
            HI_DRV_PROC_EchoHelper("21. dvi 2 block crc ok whole detailed in block 2 \n");
            HI_DRV_PROC_EchoHelper("22. dvi 2 block crc err \n");
            HI_DRV_PROC_EchoHelper("23. customer header err, base&extend crc err, extend version err \n");
            HI_DRV_PROC_EchoHelper("24. 4k tv hisense \n");
            HI_DRV_PROC_EchoHelper("25. Sony KLV-32J400A 2008  OE problem \n");
            HI_DRV_PROC_EchoHelper("26. Sony 2009 chen OE problem \n");
            HI_DRV_PROC_EchoHelper("27. multi VSDB EDID  \n");
            HI_DRV_PROC_EchoHelper("28. multi audio block  \n");
            HI_DRV_PROC_EchoHelper("29. Dvi deviece support audio \n");
            HI_DRV_PROC_EchoHelper("-------------------------------------------------------------------------------------\n");
        }

        HI_DRV_PROC_EchoHelper("swrst for trigle hotplug... ... ... \n");
        SI_SW_ResetHDMITX();
    }
    else if(!HI_OSAL_Strncmp(chArg1,"output",DEF_FILE_NAMELENGTH))
    {
        if(chArg2[0] == '0')
        {
            HI_DRV_PROC_EchoHelper("output normal \n");
            DRV_Set_ForceOutputMode(HI_FALSE);
        }
        else if(chArg2[0] == '1')
        {
            HI_DRV_PROC_EchoHelper("output force mode \n");
            DRV_Set_ForceOutputMode(HI_TRUE);
        }
    }
#ifdef HDMI_DEBUG
    else if(!HI_OSAL_Strncmp(chArg1, "logtype", DEF_FILE_NAMELENGTH))
    {
        if(strlen(chArg3) < 2)
        {   //illegal chArg3 -- file path
            HDMI_ProcessCmd(HDMI_DEBUG_LOGTYPE, chArg2, NULL);
        }
        else
        {
            HDMI_ProcessCmd(HDMI_DEBUG_LOGTYPE, chArg2, chArg3);
        }
    }
    else if(!HI_OSAL_Strncmp(chArg1, "logmask", DEF_FILE_NAMELENGTH))
    {
        HDMI_ProcessCmd(HDMI_DEBUG_LOGMASK, chArg2, NULL);
    }
    else if(!HI_OSAL_Strncmp(chArg1, "logcatch", DEF_FILE_NAMELENGTH))
    {
        HDMI_ProcessCmd(HDMI_DEBUG_LOGCATCH, chArg2, NULL);
    }
#endif
    else
    {
        HI_DRV_PROC_EchoHelper("--------------------------------- HDMI debug options --------------------------------\n");
        HI_DRV_PROC_EchoHelper("you can perform HDMI debug with such commond:\n");
        HI_DRV_PROC_EchoHelper("echo [arg1] [arg2] [arg3] > /proc/msp/hdmi \n\n");
        HI_DRV_PROC_EchoHelper("debug action                arg1        arg2                        arg3\n");
        HI_DRV_PROC_EchoHelper("--------------------------  --------    --------------------        ---------------\n");
        HI_DRV_PROC_EchoHelper("colorbar                    cbar        0 disable / 1 enable \n");
        HI_DRV_PROC_EchoHelper("vblank(yuv data from hdmi)  vblank      0 /red / green/ blue/ black \n");
        HI_DRV_PROC_EchoHelper("DVI encoder                 enc         phy(default)/ctrl  \n");
        HI_DRV_PROC_EchoHelper("pixel clk bypass            pclk        0 nobypass(default) 1 bypass  \n");
        HI_DRV_PROC_EchoHelper("software reset              swrst       no param \n");
        HI_DRV_PROC_EchoHelper("invert Tmds clk             tclk        0 not invert(default) / 1 invert \n");
        HI_DRV_PROC_EchoHelper("Avmute                      mute        0 unmute/ 1 mute \n");
        HI_DRV_PROC_EchoHelper("Set 3D InfoFrame            3d          0 disable3D /fp/sbs/tab  \n");
        HI_DRV_PROC_EchoHelper("Debug audio Change          audio       0 I2S / 1 SPdif / 2 HBR   \n");
        HI_DRV_PROC_EchoHelper("Thread stop/start           thread      0 stop / 1 start  \n");
        HI_DRV_PROC_EchoHelper("cec enable                  cec         0 disable / 1 enable \n");
        HI_DRV_PROC_EchoHelper("Force set attr              setAttr     no param \n");
        HI_DRV_PROC_EchoHelper("check                       check       timing / ddc / color /ro (read only regs) / phy\n");
        HI_DRV_PROC_EchoHelper("Force SetForamt Delay       fmtforce    0 Default / 1 Force \n");
        HI_DRV_PROC_EchoHelper("Force Avmute Delay          muteforce   0 Default / 1 Force \n");
        HI_DRV_PROC_EchoHelper("Set Foramt Delay            fmtdelay    0 ~ 10000 (ms)\n");
        HI_DRV_PROC_EchoHelper("Avmute Delay                mutedelay   0 ~ 10000 (ms)\n");
        HI_DRV_PROC_EchoHelper("Phy OE open/close           oe          no param\n");
        HI_DRV_PROC_EchoHelper("Change DDC SPEED            ddc         0 ~ 127 (n=0:0khz  n=1-127: %d/(n*35) khz)\n",OSC_CLK_SELECT);
        HI_DRV_PROC_EchoHelper("Debug edid,need swrst       edid        1 ~ %d / 0(list && stop) \n",DRV_Get_DebugEdidNum());
        HI_DRV_PROC_EchoHelper("output mode                 output      0 normal / 1 force  \n");
        HI_DRV_PROC_EchoHelper("RSEN detect mode            RsenMode    0 detect / 1 do not detect \n");
#ifdef HDMI_DEBUG
        HI_DRV_PROC_EchoHelper("logtype(file / serial)      logtype     [serial/file/all]               [path]\n");
        HI_DRV_PROC_EchoHelper("logmask(add / reduce)       logmask     [+/-/*][ cec/hdcp/hpd/edid/all]\n");
        HI_DRV_PROC_EchoHelper("logcatch(print out of mem)  logcatch    [info/warning/error/fatal]  \n");
#endif
        HI_DRV_PROC_EchoHelper("-------------------------------------------------------------------------------------\n");
    }

    return count;
}
#endif

HI_S32 HDMI_ModeInit(HI_VOID)
{

#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S  *pProcItem;

    DRV_PROC_EX_S stFnOpt =
    {
         .fnRead = HDMI0_Proc,
    };

    DRV_PROC_EX_S stFnSinkOpt =
    {
         .fnRead = HDMI0_Sink_Proc,
    };

    //Init Array
    hdmi_InitFmtArray();
    hdmi_Init_PixelClkArray();
#endif

    /* Register hdmi device */
    HI_OSAL_Snprintf(g_hdmiRegisterData.devfs_name, 64, UMAP_DEVNAME_HDMI);
    g_hdmiRegisterData.fops   = &hdmi_FOPS;
    g_hdmiRegisterData.drvops = &hdmi_DRVOPS;
    g_hdmiRegisterData.minor  = UMAP_MIN_MINOR_HDMI;
    g_hdmiRegisterData.owner  = THIS_MODULE;
    if (HI_DRV_DEV_Register(&g_hdmiRegisterData) < 0)
    {
        COM_FATAL("register hdmi failed.\n");
        return HI_FAILURE;
    }
#if (1 == HI_PROC_SUPPORT)
    /* Register Proc hdmi Status */
    pProcItem = HI_DRV_PROC_AddModule("hdmi0", &stFnOpt, NULL);
    if (pProcItem != HI_NULL)
    {
        pProcItem->write = hdmi_ProcWrite;
    }

    //pProcItem = HI_DRV_PROC_AddModule("hdmi0_sink", &stFnSinkOpt, NULL);
    HI_DRV_PROC_AddModule("hdmi0_sink", &stFnSinkOpt, NULL);
#endif

    return HI_SUCCESS;
}

extern HI_S32  HDMI_DRV_Init(HI_VOID);


HI_VOID HDMI_ModeExit(HI_VOID)
{
    /* Unregister hdmi device */
#if (1 == HI_PROC_SUPPORT)
    HI_DRV_PROC_RemoveModule("hdmi0");
    HI_DRV_PROC_RemoveModule("hdmi0_sink");
#endif
    HI_DRV_DEV_UnRegister(&g_hdmiRegisterData);
    return;
}

HI_S32 DRV_HDMI_Register(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_HDMI,HDMI_NAME,(HI_VOID *)&s_stHdmiExportFuncs);
    if (HI_SUCCESS != ret)
    {
        COM_FATAL("HI_DRV_MODULE_Register failed\n");
        return ret;
    }
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_UnRegister(HI_VOID)
{
    HI_S32 ret;
    ret = HI_DRV_MODULE_UnRegister((HI_U32)HI_ID_HDMI);
    if (HI_SUCCESS != ret)
    {
        COM_FATAL("HI_DRV_MODULE_UnRegister failed\n");
        return ret;
    }
    return HI_SUCCESS;
}

int HDMI_DRV_ModInit(void)
{
#ifndef HI_MCE_SUPPORT
    HI_S32 ret;
#endif
#if 0//defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    HI_U32 i = 0;
    HI_DISP_PARAM_S   stDispParam = {0};
    PDM_EXPORT_FUNC_S *pstPdmFuncs  = HI_NULL;
#endif
//#ifdef HDMI_DEBUG
//    ret = HDMI_DbgInit();
//    if(HI_FAILURE == ret)
//    {
//        HI_PRINT("HDMI debug Init failed!\n");
//    }
//#endif

#if defined (SUPPORT_FPGA)
    SocHdmiInit();
#endif
    HDMI_ModeInit();

#ifndef HI_MCE_SUPPORT
    ret = HDMI_DRV_Init();
    if (HI_SUCCESS != ret)
    {
        COM_FATAL("HDMI_DRV_Init failed\n");
        return ret;
    }
#endif

#ifdef ANDROID_SUPPORT
    //android only
    if (switch_dev_register(&hdmi_tx_sdev))
    {
        COM_WARN("\n Warning:! registering HDMI switch device Failed \n");
        g_switchOk = HI_FALSE;
        //return -EINVAL;
    }
    else
    {
        g_switchOk = HI_TRUE;
    }
#endif

#if 0//defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    ret = HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&pstPdmFuncs);
    if (ret == HI_FAILURE || NULL == pstPdmFuncs)
    {
        COM_WARN("get pdm module function failed\n");

    }
    else if (pstPdmFuncs->pfnPDM_GetDispParam)
    {
        ret = pstPdmFuncs->pfnPDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam);
        if (ret == HI_FAILURE)
        {
            COM_WARN("PDM_GetDispParam failed\n");
        }
        else
        {
            for (i = 0; i < HI_UNF_DISP_INTF_TYPE_BUTT; i++)
            {
                if ((stDispParam.stIntf[i].enIntfType == HI_UNF_DISP_INTF_TYPE_HDMI)
                        && (stDispParam.stIntf[i].unIntf.enHdmi != HI_UNF_HDMI_ID_BUTT))
                {
                    HI_DRV_HDMI_Open(stDispParam.stIntf[i].unIntf.enHdmi);
                    COM_INFO("HI_DRV_HDMI_Open success\n");
                }
            }
        }
    }
    else
    {
        COM_WARN("get pdm module function failed\n");
    }
#endif

#ifdef MODULE
    HI_PRINT("Load hi_hdmi.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

extern HI_VOID  HDMI_DRV_EXIT(HI_VOID);
void HDMI_DRV_ModExit(void)
{
    HI_U32 hdmiStatus;
    hdmiStatus = DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_0);
    if(hdmiStatus > 0)
    {
        HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_0);
    }

    HDMI_ModeExit();

    HDMI_DRV_EXIT();
    //HI_DRV_HDMI_PlayStus(HI_UNF_HDMI_ID_0,&temp);
    //if(temp == HI_TRUE)
    //{
        //HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_0);
    //}
#ifdef ANDROID_SUPPORT
    //android 特有
    if(g_switchOk == HI_TRUE)
    {
        switch_dev_unregister(&hdmi_tx_sdev);
    }
#endif

#ifdef MODULE
    HI_PRINT("remove hi_hdmi.ko success.\t(%s)\n", VERSION_STRING);
#endif

//#ifdef HDMI_DEBUG
//    HDMI_DbgDeInit();
//#endif

    return;
}

#ifdef MODULE
module_init(HDMI_DRV_ModInit);
module_exit(HDMI_DRV_ModExit);
#endif
MODULE_LICENSE("GPL");




