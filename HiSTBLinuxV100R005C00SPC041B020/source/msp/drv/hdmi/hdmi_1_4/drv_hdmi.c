/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
 File Name     :  drv_hdmi.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2010/6/20
Description   :
History       :
Date          : 2010/11/22
Author        : l00168554
Modification  :
 *******************************************************************************/
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>

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
#include <linux/string.h>

#include "drv_hdmi.h"

#include "hi_drv_hdmi.h"

#include "hi_unf_hdmi.h"

#include "si_audio.h"
#include "si_hdmitx.h"
#include "si_infofrm.h"
#include "si_txapidefs.h"
#include "si_defstx.h"
#include "si_hdcpdefs.h"
#include "si_eedefs.h"
#include "si_de.h"
#include "si_edid.h"
#include "si_txvidp.h"
#include "si_timer.h"
#include "si_eeprom.h"
#include "si_cec.h"
#include "si_phy.h"

#include "hi_unf_disp.h"
#include "si_mpi_hdmi.h"
#include "drv_disp_ext.h"
#include "hi_kernel_adapt.h"
#include "drv_cipher_ext.h"
#include "hi_drv_sys.h"

#include "drv_global.h"
#include "drv_reg_proc.h"
#include "drv_compatibility.h"
#include "hi_drv_module.h"


#ifdef ANDROID_SUPPORT
#include <linux/switch.h>
extern HI_BOOL g_switchOk;
extern struct switch_dev hdmi_tx_sdev;
#endif

#define HDCP_KEY_CHECK_OK 0xa
typedef enum hiHDMI_VIDEO_TIMING_E
{
    VIDEO_TIMING_UNKNOWN,
    VIDEO_TIMING_640X480P_59940,        /* 1: 640x480p  @ 59.94Hz  No Repetition */
    VIDEO_TIMING_640X480P_60000,        /* 1: 640x480p  @ 60Hz     No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X480P_59940,        /* 2: 720x480p @ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_720X480P_60000,        /* 2: 720x480p  @ 60Hz     No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1280X720P_59940,       /* 4: 1280x720p @ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_1280X720P_60000,       /* 4: 1280x720p @ 60Hz     No Repetition */
    VIDEO_TIMING_1920X1080I_60000,      /* 5: 1920x1080i@ 59.94Hz  No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080I_59940,      /* 5: 1920x1080i@ 60Hz     No Repetition */
    VIDEO_TIMING_720X480I_59940,        /* 6: 720x480i  @ 59.94Hz  pixel sent 2 times */
#endif
    VIDEO_TIMING_720X480I_60000,        /* 6: 720x480i  @ 60Hz     pixel sent 2 times */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X240P_59940,        /* 8: 720x240p  @ 59.94Hz  pixel sent 2 times */
    VIDEO_TIMING_720X240P_60000,        /* 8: 720x240p  @ 60Hz     pixel sent 2 times */
    VIDEO_TIMING_2880X480I_59940,       /* 10:2880x480i @ 59.94Hz  pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X480I_60000,       /* 10:2880x480i @ 60Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X240P_59940,       /* 12:2880x240p @ 59.94Hz  pixel sent 1 to 10 timesn */
    VIDEO_TIMING_2880X240P_60000,       /* 12:2880x240p @ 60Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_1440X480P_59940,       /* 14:1440x480p @ 59.94Hz  pixel sent 1 to 2 times */
    VIDEO_TIMING_1440X480P_60000,       /* 14:1440x480p @ 60Hz     pixel sent 1 to 2 times */
    VIDEO_TIMING_1920X1080P_59940,      /* 16:1920x1080p@ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_60000,      /* 16:1920x1080p@ 60Hz     No Repetition */
    VIDEO_TIMING_720X576P_50000,        /* 17:720x576p  @ 50Hz     No Repetition */
    VIDEO_TIMING_1280X720P_50000,       /* 19:1280x720p @ 50Hz     No Repetition */
    VIDEO_TIMING_1920X1080I_50000,      /* 20:1920x1080i@ 50Hz     No Repetition */
    VIDEO_TIMING_720X576I_50000,        /* 21:720x576i  @ 50Hz     pixel sent 2 times */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X288P_50000,        /* 23:720x288p @ 50Hz      pixel sent 2 times */
    VIDEO_TIMING_2880X576I_50000,       /* 25:2880x576i @ 50Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X288P_50000,       /* 27:2880x288p @ 50Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_1440X576P_50000,       /* 29:1440x576p @ 50Hz     pixel sent 1 to 2 times */
#endif
    VIDEO_TIMING_1920X1080P_50000,      /* 31:1920x1080p @ 50Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080P_23980,      /* 32:1920x1080p @ 23.98Hz No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_24000,      /* 32:1920x1080p @ 24Hz   No Repetition */
    VIDEO_TIMING_1920X1080P_25000,      /* 33:1920x1080p @ 25Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080P_29980,      /* 34:1920x1080p @ 29.98Hz No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_30000,      /* 34:1920x1080p @ 30Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_2880X480P_59940,       /* 35:2880x480p @ 59.94Hz  pixel sent 1, 2 or 4 times */
    VIDEO_TIMING_2880X480P_60000,       /* 35:2880x480p @ 60Hz     pixel sent 1, 2 or 4 times */
    VIDEO_TIMING_2880X576P_50000,       /* 37:2880x576p @ 50Hz     pixel sent 1, 2 or 4 times*/
#endif
    VIDEO_TIMING_MAX
}DRV_HDMI_VIDEO_TIMING_E;

enum
{
    HDMI_CALLBACK_NULL,
    HDMI_CALLBACK_USER,
    HDMI_CALLBACK_KERNEL
};

typedef struct
{
    HI_U32                      VidIdCode;
    DRV_HDMI_VIDEO_TIMING_E     Mode;
    HI_U32                      FrameRate;
    HI_U32                      Active_X;
    HI_U32                      Active_Y;
    HI_U32                      Active_W;
    HI_U32                      Active_H;
    VIDEO_SAMPLE_TYPE_E  ScanType;
    HI_UNF_HDMI_ASPECT_RATIO_E       AspectRatio;
    HI_U32                      PixelRepetition;
}hdmi_VideoIdentification_t;

static hdmi_VideoIdentification_t  VideoCodes[] =
{
    /* {Video Identification Code, Timing Mode, x, y, w, h, Frame Rate, Scan Type, Aspect Ratio, Pixel Repetition} */
    {0,  VIDEO_TIMING_UNKNOWN,              0, 0, 0,    0,    0, VIDEO_SAMPLE_TYPE_UNKNOWN,      HI_UNF_HDMI_ASPECT_RATIO_NO_DATA, 0},
    {1,  VIDEO_TIMING_640X480P_59940,   59940, 0, 0,  640,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
    {1,  VIDEO_TIMING_640X480P_60000,   60000, 0, 0,  640,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#if defined (DVI_SUPPORT)
    {2,  VIDEO_TIMING_720X480P_59940,   59940, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#endif
    {2,  VIDEO_TIMING_720X480P_60000,   60000, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#if defined (DVI_SUPPORT)
    {4,  VIDEO_TIMING_1280X720P_59940,  59940, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9,  0},
#endif
    {4,  VIDEO_TIMING_1280X720P_60000,  60000, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {5,  VIDEO_TIMING_1920X1080I_60000, 60000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {5,  VIDEO_TIMING_1920X1080I_59940, 59940, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {6,  VIDEO_TIMING_720X480I_59940,   59940, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3, 1},
#endif
    {6,  VIDEO_TIMING_720X480I_60000,   60000, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3, 1},
#if defined (DVI_SUPPORT)
    {8,  VIDEO_TIMING_720X240P_59940,   59940, 0, 0,  720,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1},
    {8,  VIDEO_TIMING_720X240P_60000,   60000, 0, 0,  720,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/,  1},
    {10, VIDEO_TIMING_2880X480I_59940,  59940, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/,  1/*1 to 10 times*/},
    {10, VIDEO_TIMING_2880X480I_60000,  60000, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 10 times*/},
    {12, VIDEO_TIMING_2880X240P_59940,  59940, 0, 0, 2280,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 10 times*/},
    {12, VIDEO_TIMING_2880X240P_60000,  60000, 0, 0, 2280,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 10 times*/},
    {14, VIDEO_TIMING_1440X480P_59940,  59940, 0, 0, 1440,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 2 times*/},
    {14, VIDEO_TIMING_1440X480P_60000,  60000, 0, 0, 1440,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/,  1/*1 to 2 times*/},
    {16, VIDEO_TIMING_1920X1080P_59940, 59940, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {16, VIDEO_TIMING_1920X1080P_60000, 60000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {17, VIDEO_TIMING_720X576P_50000,   50000, 0, 0,  720,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3, 0},
    {19, VIDEO_TIMING_1280X720P_50000,  50000, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {20, VIDEO_TIMING_1920X1080I_50000, 50000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {21, VIDEO_TIMING_720X576I_50000,   50000, 0, 0,  720,  576, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3,  1},
#if defined (DVI_SUPPORT)
    {23, VIDEO_TIMING_720X288P_50000,   50000, 0, 0,  720,  288, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1},
    {25, VIDEO_TIMING_2880X576I_50000,  50000, 0, 0, 2880,  576, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 10 times*/},
    {27, VIDEO_TIMING_2880X288P_50000,  50000, 0, 0, 2880,  288, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 10 times*/},
    {29, VIDEO_TIMING_1440X576P_50000,  50000, 0, 0, 1440,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 1/*1 to 2 times*/},
#endif
    {31, VIDEO_TIMING_1920X1080P_50000, 50000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {32, VIDEO_TIMING_1920X1080P_23980, 23980, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {32, VIDEO_TIMING_1920X1080P_24000, 24000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {33, VIDEO_TIMING_1920X1080P_25000, 25000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {34, VIDEO_TIMING_1920X1080P_29980, 29980, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {34, VIDEO_TIMING_1920X1080P_30000, 30000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {35, VIDEO_TIMING_2880X480P_59940,  59940, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 0/*1, 2 or 4 times*/},
    {35, VIDEO_TIMING_2880X480P_60000,  60000, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 0/*1, 2 or 4 times*/},
    {37, VIDEO_TIMING_2880X576P_50000,  50000, 0, 0, 2280,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_NO_DATA/*?*/, 0/*1, 2 or 4 times*/},
#endif
};

#define HDMI_CHECK_NULL_PTR(ptr)  do{                         \
    if (NULL == (ptr))                                    \
    {                                                     \
        return HI_ERR_HDMI_NUL_PTR;                       \
    }                                                     \
}while(0)

#define HDMI_CHECK_ID(l_enHdmi)  do{                          \
    if ((l_enHdmi) >= HI_UNF_HDMI_ID_BUTT)                \
    {                                                     \
        COM_INFO("enHdmi %d is invalid.\n", l_enHdmi); \
        return HI_ERR_HDMI_INVALID_PARA;                  \
    }                                                     \
}while(0)

#define HDMI_CheckChnOpen(l_HdmiID) do{                       \
    if (!DRV_Get_IsChnOpened(l_HdmiID))      \
    {                                                     \
        COM_INFO("enHdmi %d is NOT open.\n", l_HdmiID);\
        return HI_ERR_HDMI_DEV_NOT_OPEN;                  \
    }                                                     \
}while(0)

static HI_U32 hdmi_Mutex_Event_Count = 0;
HI_DECLARE_MUTEX(g_HDMIEventMutex);
#define HDMI_EVENT_LOCK()                                     \
    do{                                                           \
        hdmi_Mutex_Event_Count ++;                                \
        /*HI_INFO_HDMI("hdmi_Mutex_Event_Count:%d\n", hdmi_Mutex_Count);*/  \
        if (down_interruptible(&g_HDMIEventMutex))                \
        {}                                                         \
    }while(0)                                                     \

#define HDMI_EVENT_UNLOCK()                                   \
    do{                                                           \
        hdmi_Mutex_Event_Count --;                                \
        /*HI_INFO_HDMI("hdmi_Mutex_Event_Count:%d\n", hdmi_Mutex_Count); */ \
        up(&g_HDMIEventMutex);                                    \
    }while(0)                                                     \


HI_DECLARE_MUTEX(g_HDMIAttrMutex);
#define HDMI_ATTR_LOCK()                                     \
    do{                                                           \
        if (down_interruptible(&g_HDMIAttrMutex))                \
        ;                                                         \
    }while(0)                                                     \

#define HDMI_ATTR_UNLOCK()                                   \
    do{                                                           \
        up(&g_HDMIAttrMutex);                                     \
    }while(0)                                                     \


static HI_U32 hdmi_Create_AVI_Infoframe(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *punAVIInfoFrame, HI_U8 *pu8AviInfoFrame);
static HI_U32 hdmi_Create_Audio_Infoframe(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame, HI_U8 *pu8AudioInfoFrame);
//static HI_U32 hdmi_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstHDMIAttr, HI_BOOL UpdateFlag);
static HI_U32 hdmi_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
static HI_U32 hdmi_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
static HI_U32 hdmi_AdjustAVIInfoFrame(HI_UNF_HDMI_ID_E enHdmi);
static HI_U32 hdmi_AdjustVSDBInfoFrame(HI_UNF_HDMI_ID_E enHdmi);
static HI_U32 hdmi_AdjustAUDInfoFrame(HI_UNF_HDMI_ID_E enHdmi);
static HI_VOID hdmi_SetAndroidState(HI_S32 PlugState);
HI_DRV_DISP_FMT_E DispFormatAdjust(HI_DRV_DISP_FMT_E enEncFmt);


static void hdmi_ProcEvent(HI_UNF_HDMI_EVENT_TYPE_E event,HI_U32 procID);

static HI_U32 g_HDMIWaitFlag[MAX_PROCESS_NUM];
static wait_queue_head_t g_astHDMIWait;

//One Proccess can only init/open one HDMI Instance!
static HI_U32 g_HDMIKernelInitNum = 0; //Kernel HDMI Init Num
static HI_U32 g_HDMIUserInitNum   = 0; //User HDMI Init Num
static HI_U32 g_HDMIOpenNum       = 0; //HDMI Open Num
static HI_U32 g_UserCallbackFlag  = HDMI_CALLBACK_NULL;
//static HI_U32 g_UCallbackAddr     = 0; //save callback pointer Address

HI_U32 unStableTimes = 0;

extern unsigned int suspend_flag;

HI_S32 Hdmi_KThread_Timer(void* pParm)
{
    HI_U32 Ret;

    pParm = pParm;//ignore compire warnning

    while ( 1 )
    {
        if (kthread_should_stop())
        {
            break;
        }

        if(!siiIsTClockStable() && (!DRV_Get_IsThreadStoped()) && !SI_IsHDMIResetting())
        {
            unStableTimes++;
        }

        if (!DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_0) || DRV_Get_IsThreadStoped() || SI_IsHDMIResetting() || suspend_flag)
        {
            //msleep(50);
            msleep(10);
            continue;
        }

#ifndef __HDMI_INTERRUPT__
        Ret = SI_TimerHandler();
#endif
       DRV_HDMI_SetSoftResume(HI_FALSE);
        msleep(90);
    }

    return HI_SUCCESS;
}

#if defined (CEC_SUPPORT)
HI_S32 Hdmi_KThread_CEC(void* pParm)
{
    pParm = pParm;

    while ( 1 )
    {
        if (kthread_should_stop())
            break;

        /* HDMI do not start, Just sleep */
        if (!DRV_Get_IsCECEnable(HI_UNF_HDMI_ID_0))
        {
            msleep(100);
            continue;
        }

        if (!DRV_Get_IsCECStart(HI_UNF_HDMI_ID_0))
        {
            HI_U32 u32Status = 0, index;
            HI_UNF_EDID_BASE_INFO_S *pSinkCap = DRV_Get_SinkCap(HI_UNF_HDMI_ID_0);
            HI_UNF_HDMI_CEC_STATUS_S     *pstCECStatus;
            HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();

            /* Only Check CEC 5 time */
            if(pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount < 10)
            {
                pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount ++;
            }
            else
            {
                /* NO CEC Response, Just sleep */
                msleep(100);
                continue;
            }

            /*We need to do AutoPing */
            SI_CEC_AudioPing(&u32Status);

            if(0x01 == (u32Status & 0x01))
            {
                //Build up CEC Status!
                if(!DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
                    continue;

                //Physical Address
                pstCECStatus = DRV_Get_CecStatus(HI_UNF_HDMI_ID_0);

                if(!pSinkCap->stCECAddr.u8PhyAddrA)
                {
                    continue; //Bad CEC Phaycail Address
                }

                pstCECStatus->u8PhysicalAddr[0] = pSinkCap->stCECAddr.u8PhyAddrA;
                pstCECStatus->u8PhysicalAddr[1] = pSinkCap->stCECAddr.u8PhyAddrB;
                pstCECStatus->u8PhysicalAddr[2] = pSinkCap->stCECAddr.u8PhyAddrC;
                pstCECStatus->u8PhysicalAddr[3] = pSinkCap->stCECAddr.u8PhyAddrD;

                //CEC Network
                for(index = 0; index < HI_UNF_CEC_LOGICALADD_BUTT; index ++)
                {
                    if((u32Status & (1 << index)) != 0)
                    {
                        pstCECStatus->u8Network[index] = HI_TRUE;
                        CEC_INFO("cec ping logAddr[0x%02x] Ack \n",index);
                    }
                    else
                    {
                        CEC_INFO("cec ping logAddr[0x%02x] No Ack \n",index);
                    }
                }

                //Logical Address
                if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_1] == HI_TRUE)            //bit3
                {
                    if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_2] == HI_TRUE)        //bit6
                    {
                        if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_3] == HI_TRUE)    //bit7
                        {
                            if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_4] == HI_TRUE)//bit10
                                pstCECStatus->u8LogicalAddr = 0x0f;//Brocast Address!
                            else
                                pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_4;
                        }
                        else
                        {
                            pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_3;
                        }
                    }
                    else
                    {
                        pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_2;
                    }
                }
                else
                {
                    pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_1;
                }

                pstCECStatus->bEnable =  HI_TRUE;
                CEC_INFO("CEC is build up *****\n");

                SI_CEC_Open(pstCECStatus->u8LogicalAddr);
                //Should send out Brocast messsage of <Report Physical Address> !
                {
                    HI_UNF_HDMI_CEC_CMD_S CECCmd = {0};

                    CECCmd.u8Opcode = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
                    CECCmd.enSrcAdd = pstCECStatus->u8LogicalAddr;
                    CECCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST;
                    CECCmd.unOperand.stRawData.u8Length  = 3;
                    CECCmd.unOperand.stRawData.u8Data[0] = ((pstCECStatus->u8PhysicalAddr[0] << 4) & 0xf0) | (pstCECStatus->u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
                    CECCmd.unOperand.stRawData.u8Data[1] = ((pstCECStatus->u8PhysicalAddr[2] << 4) & 0xf0) | (pstCECStatus->u8PhysicalAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
                    CECCmd.unOperand.stRawData.u8Data[2] = pstCECStatus->u8LogicalAddr; // [Physical Address(A.B.C.D):C D]
                    SI_CEC_SendCommand(&CECCmd);
                }
                //Should send out Brocast message of <Vendor Device ID>!
                DRV_Set_CECStart(HI_UNF_HDMI_ID_0,HI_TRUE);
                CEC_INFO("\n-------CEC Started-------\n");
            }

            msleep(100);
            continue;
        }
        else
        {
            SI_CEC_Event_Handler();
            msleep(50);
        }

        msleep(100);
    }
    return HI_SUCCESS;
}

#endif

static hdmi_VideoIdentification_t * hdmi_GetVideoCode(HI_DRV_DISP_FMT_E enTimingMode)
{
    HI_U32 Index, VideoTimingMode;

    switch (enTimingMode)
    {
        case HI_DRV_DISP_FMT_1080P_60:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_60000;
            break;
        case HI_DRV_DISP_FMT_1080P_50:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_50000;
            break;
        case HI_DRV_DISP_FMT_1080P_30:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_30000;
            break;
        case HI_DRV_DISP_FMT_1080P_25:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_25000;
            break;
        case HI_DRV_DISP_FMT_1080P_24:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_24000;
            break;
        case HI_DRV_DISP_FMT_1080i_60:
            VideoTimingMode = VIDEO_TIMING_1920X1080I_60000;
            break;
        case HI_DRV_DISP_FMT_1080i_50:
            VideoTimingMode = VIDEO_TIMING_1920X1080I_50000;
            break;
        case HI_DRV_DISP_FMT_720P_60:
            VideoTimingMode = VIDEO_TIMING_1280X720P_60000;
            break;
        case HI_DRV_DISP_FMT_720P_50:
            VideoTimingMode = VIDEO_TIMING_1280X720P_50000;
            break;
        case HI_DRV_DISP_FMT_576P_50:
            VideoTimingMode = VIDEO_TIMING_720X576P_50000;
            break;
        case HI_DRV_DISP_FMT_480P_60:
            VideoTimingMode = VIDEO_TIMING_720X480P_60000;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            VideoTimingMode = VIDEO_TIMING_720X576I_50000;
            break;
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
            VideoTimingMode = VIDEO_TIMING_720X480I_60000;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60:
            VideoTimingMode = VIDEO_TIMING_640X480P_60000;
            break;
        default:
            COM_INFO("Non CEA video timing:%d\n", enTimingMode);
            // 4k2k && vesa
            VideoTimingMode = VIDEO_TIMING_UNKNOWN;
            break;
    }

    for (Index=0; Index < VIDEO_TIMING_MAX; Index ++)
    {
        if (VideoCodes[Index].Mode == VideoTimingMode)
            break;
    }

    if (Index >= VIDEO_TIMING_MAX)
    {
        Index = 0;
    }

    COM_INFO("Get Video Code index:%d, Mode:0x%x, VidIdCode:0x%x\n", Index, VideoCodes[Index].Mode, VideoCodes[Index].VidIdCode);
    return(&(VideoCodes[Index]));
}

static HI_VOID hdmi_SetAndroidState(HI_S32 PlugState)
{
#ifdef ANDROID_SUPPORT
    if(g_switchOk == HI_TRUE)
    {
        switch_set_state(&hdmi_tx_sdev, PlugState);
    }
#endif
}

/*we may delete this func , because all the complex-use  is controlled by excel table in uboot-stage.*/
HI_VOID HDMI_PinConfig(HI_VOID)
{
    return ;
}


static HI_U32 hdmi_Get_FmtVIC(HI_DRV_DISP_FMT_E enEncFmt)
{
    HI_U32 u32VIC = 0;
    switch(enEncFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60:
            u32VIC = 0x10;
            break;
        case HI_DRV_DISP_FMT_1080P_50:
            u32VIC = 0x1f;
            break;
        case HI_DRV_DISP_FMT_1080P_30:
            u32VIC = 0x22;
            break;
        case HI_DRV_DISP_FMT_1080P_25:
            u32VIC = 0x21;
            break;
        case HI_DRV_DISP_FMT_1080P_24:
            u32VIC = 0x20;
            break;
        case HI_DRV_DISP_FMT_1080i_60:
            u32VIC = 0x05;
            break;
        case HI_DRV_DISP_FMT_1080i_50:
            u32VIC = 0x14;
            break;
        case HI_DRV_DISP_FMT_720P_60:
            u32VIC = 0x04;
            break;
        case HI_DRV_DISP_FMT_720P_50:
            u32VIC = 0x13;
            break;
        case HI_DRV_DISP_FMT_576P_50:
            u32VIC = 0x11;
            break;
        case HI_DRV_DISP_FMT_480P_60:
            u32VIC = 0x02;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
        case HI_DRV_DISP_FMT_1440x576i_50:
             u32VIC = 0x15;
            break;
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
        case HI_DRV_DISP_FMT_1440x480i_60:
            u32VIC = 0x06;
            break;
        default:
            u32VIC = 0x00;
            break;
    }
    COM_INFO("hdmi vic:0x%x\n",u32VIC);
    return u32VIC;
}

static HI_U32 g_Event_Count[MAX_PROCESS_NUM];
#define MIX_EVENT_COUNT  1

HI_U32 DRV_HDMI_Init(HI_U32 FromUserSpace)
{
    HI_U8                ucChipInf[3] = {0};
    HI_U8                BlankValue[3];
    HI_U32               u32Vic = 0;
    HI_BOOL              bOpenAlready = HI_FALSE;          //Judge whether HDMI is setup already
    HI_U32               u32Ret = HI_SUCCESS;
    HDMI_COMM_ATTR_S    *pstCommAttr = DRV_Get_CommAttr();
    HDMI_CHN_ATTR_S     *pstChnAttr = DRV_Get_ChnAttr();
    HI_U32               reg = 0;
    DISP_EXPORT_FUNC_S  *disp_func_ops = HI_NULL;

    reg = ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR);

    if(DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_0) == 0)
    {
        /* Need to reset this  param */
        DRV_Set_Mce2App(HI_FALSE);
        DRV_Set_OpenedInBoot(HI_FALSE);
    }

    COM_INFO("****Enter DRV_HDMI_Init*****\n");
    //HDMI_PinConfig();

    u32Ret = HI_DRV_MODULE_GetFunction(HI_ID_DISP, (HI_VOID**)&disp_func_ops);
     /*if func ops and func ptr is null, then return ;*/
    if((NULL == disp_func_ops) || (NULL == disp_func_ops->pfnDispGetFormat) || (u32Ret != HI_SUCCESS))
    {
        COM_INFO("No disp, hdmi init err\n");
        return HI_FAILURE;
    }

    BlankValue[0] = 128;
    BlankValue[1] = 16;
    BlankValue[2] = 128;
    SI_WriteBlockHDMITXP0(TX_VID_BLANK1_BLUE, 3, BlankValue);

    /*if FromUserSpace == HI_TRUE,Setup from User,or Setup from Kerne */
    if (HI_TRUE != FromUserSpace)
    {
        if (g_HDMIKernelInitNum)
        {
            g_HDMIKernelInitNum ++;
            COM_WARN("From Kernel:HDMI has been inited!\n");
            return HI_ERR_HDMI_INIT_ALREADY;
        }
    }
    else
    {
        if (g_HDMIUserInitNum)
        {
            g_HDMIUserInitNum ++;
            COM_WARN("FromUser:HDMI has been inited!\n");
            return HI_ERR_HDMI_CALLBACK_ALREADY;
        }
    }

    g_UserCallbackFlag = HDMI_CALLBACK_NULL;

      /* Judge whether it has opened already */
    bOpenAlready = SI_HDMI_Setup_INBoot(&u32Vic);
    COM_INFO("bOpenAlready:%d\n", bOpenAlready);

    if (bOpenAlready == HI_TRUE)
    {
        HI_DRV_DISP_FMT_E enEncFmt = HI_DRV_DISP_FMT_BUTT;

        if(disp_func_ops && disp_func_ops->pfnDispGetFormat)
        {
            disp_func_ops->pfnDispGetFormat(HI_UNF_DISPLAY1, &enEncFmt);
            enEncFmt = DispFormatAdjust(enEncFmt);
        }
        else
        {
            COM_WARN("Can't Get disp_func_ops \n");
        }

        if(hdmi_Get_FmtVIC(enEncFmt) == u32Vic && (enEncFmt < HI_DRV_DISP_FMT_BUTT))
        {
            bOpenAlready = HI_TRUE;
        }
    }

    if(bOpenAlready == HI_TRUE)
    {

        if ((g_HDMIUserInitNum > 0) || (g_HDMIKernelInitNum > 0))
        {
            if (HI_TRUE == FromUserSpace)
            {
                g_HDMIUserInitNum ++;
            }
            else
            {
                g_HDMIKernelInitNum ++;
            }
            COM_INFO("Open Already in MCE!\n");
            /* open already flag */
            //pstCommAttr->bOpenGreenChannel = HI_TRUE;
            DRV_Set_Mce2App(HI_TRUE);

            COM_INFO("****green channel change %d *****\n",DRV_Get_IsMce2App());
            return HI_SUCCESS;   //Open Already in MCE!
        }
        else
        {
            DRV_Set_OpenedInBoot(HI_TRUE);
            //Open Already in FastBoot!
            COM_INFO("HDMI is setup in fastboot!!\n");
        }
    }
    else /* Normal Setup */
    {
        COM_INFO("come to SI_HW_ResetHDMITX\n");
        SI_HW_ResetHDMITX();
        SI_SW_ResetHDMITX();
#if        defined(CHIP_TYPE_hi3798mv100) \
        || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
        || defined(CHIP_TYPE_hi3716mv420) \
        || defined(CHIP_TYPE_hi3716mv410)
        SI_TX_PHY_PowerDown(HI_TRUE);
    #endif
    }

    if(DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_0) == 0)
    {
        u32Ret = SI_OpenHdmiDevice();
#if defined (CEC_SUPPORT)
        /* Enable CEC_SETUP */
        SI_CEC_SetUp();
#endif
        SI_ReadChipInfo(ucChipInf);
        COM_INFO("ReadChipInfo HDMI: Id 0x%02x.0x%02x. Rev %02x\n",
            (int)ucChipInf[0],(int)ucChipInf[1],(int)ucChipInf[2]);

        memset((void *)pstChnAttr, 0, sizeof(HDMI_CHN_ATTR_S) * HI_UNF_HDMI_ID_BUTT);

        //g_HDMIWaitFlag = HI_FALSE;

        //need move to channel Attr??
        memset(g_HDMIWaitFlag,0,sizeof(HI_U32) * MAX_PROCESS_NUM);
        memset(g_Event_Count,0,sizeof(HI_U32) * MAX_PROCESS_NUM);
        DRV_Set_ThreadStop(HI_FALSE);

        COM_INFO("SetDefaultAttr \n");
        DRV_HDMI_SetDefaultAttr();

        init_waitqueue_head(&g_astHDMIWait);
        /* create hdmi task */
        if (pstCommAttr->kThreadTimer == NULL)
        {
            COM_INFO("create Timer task kThreadTimer \n");
            pstCommAttr->kThreadTimer = kthread_create(Hdmi_KThread_Timer, NULL, "HI_HDMI_kThread");
            if (IS_ERR(pstCommAttr->kThreadTimer))
            {
                COM_ERR("start hdmi kernel thread failed.\n");
            }
            else
            {
                wake_up_process(pstCommAttr->kThreadTimer);
            }
        }

#if defined (CEC_SUPPORT)
        if (pstCommAttr->kCECRouter == NULL)
        {
            /* create CEC task */
            pstCommAttr->kCECRouter = kthread_create(Hdmi_KThread_CEC, NULL, "HI_HDMI_kCEC");
            if (IS_ERR(pstCommAttr->kCECRouter))
            {
                COM_ERR("Unable to start hdmi kernel thread.\n");
            }
            else
            {
                wake_up_process(pstCommAttr->kCECRouter);
            }
        }
#endif

        /* Normal init HDMI */
        COM_INFO("WriteDefaultConfigToEEPROM\n");
        SI_WriteDefaultConfigToEEPROM();

        COM_INFO("Leave DRV_HDMI_Init\n");
#ifdef __HDMI_INTERRUPT__
        HDMI_IRQ_Setup();
#endif
    }

    if (HI_TRUE == FromUserSpace)
    {
        g_HDMIUserInitNum ++;
    }
    else
    {
        g_HDMIKernelInitNum ++;
    }
    //DRV_PrintCommAttr();
    hdmi_SetAndroidState(STATE_PLUG_UNKNOWN);
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_DeInit(HI_U32 FromUserSpace)
{
    int ret = 0;
    HI_S32 i;
    HDMI_COMM_ATTR_S *pstCommAttr = DRV_Get_CommAttr();

#ifdef __HDMI_INTERRUPT__
    HDMI_IRQ_Exit();
#endif

    COM_INFO("Enter DRV_HDMI_DeInit g_HDMIUserInitNum:0x%x\n", g_HDMIUserInitNum);
    if (DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_0) == 0)
    {
        COM_INFO("HDMI has been deInited!\n");
        return HI_SUCCESS;
    }

    if( (FromUserSpace == HI_TRUE) )
    {
        g_HDMIUserInitNum --;
    }

    if( (FromUserSpace == HI_FALSE) )
    {
        g_HDMIKernelInitNum --;
    }

    if(g_HDMIUserInitNum == 0)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_KERNEL;
    }
    //We only do HDMI Deinit when UserLevel Count is 0.
    COM_INFO("after g_HDMIUserInitNum:0x%x\n", g_HDMIUserInitNum);

    if(DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_0) != 0)
    {
        COM_INFO("\n ignore DeInit \n");
        return HI_SUCCESS;
    }

    COM_INFO("stop hdmi task\n");

    if (pstCommAttr->kThreadTimer)
    {
        ret = kthread_stop(pstCommAttr->kThreadTimer);
        COM_INFO("end HDMI Timer thread. ret = %d\n" , ret);
        pstCommAttr->kThreadTimer = NULL;
    }
#if defined (CEC_SUPPORT)
    if (pstCommAttr->kCECRouter)
    {
        ret = kthread_stop(pstCommAttr->kCECRouter);
        COM_INFO("end HDMI CEC thread. ret = %d\n" , ret);
        pstCommAttr->kCECRouter = NULL;
    }
#endif
    for (i = 0; i < HI_UNF_HDMI_ID_BUTT; i++)
    {
        ret = DRV_HDMI_Close(i);
    }
    SI_HW_ResetHDMITX();
    SI_SW_ResetHDMITX();
    //Disable HDMI IP
    SI_DisableHdmiDevice();
    SI_CloseHdmiDevice();
    //It will power down the whole HDMI IP.
    SI_PoweDownHdmiDevice();

    //force to clear hdmi count
    g_HDMIKernelInitNum = 0;
    g_HDMIUserInitNum   = 0;

    COM_INFO("Leave DRV_HDMI_DeInit\n");
    return HI_SUCCESS;
}

HI_VOID hdmi_OpenNotify(HI_U32 u32ProcID,HI_UNF_HDMI_EVENT_TYPE_E event)
{
    COM_INFO("\nhdmi_OpenNotify-----u32ProcID %d,event 0x%x\n",u32ProcID,event);
    if (DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_0))
    {
        if ((event == HI_UNF_HDMI_EVENT_HOTPLUG))
        {
#if defined (CEC_SUPPORT)
            HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();
#endif

            // Enable Interrupts: VSync, Ri check, HotPlug
            SI_EnableInterrupts();

#if defined (CEC_SUPPORT)
            /* Enable CEC_SETUP */
            SI_CEC_SetUp();
            pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount = 0;
#endif
            SI_HPD_SetHPDUserCallbackCount();

            hdmi_SetAndroidState(STATE_HOTPLUGIN);

        }
        else if (event == HI_UNF_HDMI_EVENT_NO_PLUG)
        {
            if (g_HDMIUserInitNum)
            {
#if defined (CEC_SUPPORT)
                HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();
#endif
                //0x72:0x08 powerdown is only needed in isr && cec
                /* Close HDMI Output */
                //SI_PowerDownHdmiTx();

                SI_DisableHdmiDevice();
                DRV_Set_ChnStart(HI_UNF_HDMI_ID_0,HI_FALSE);
#if defined (HDCP_SUPPORT)
                /*Set HDCP Off */
                SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif
#if defined (CEC_SUPPORT)
                //Close CEC
                SI_CEC_Close();
                DRV_Set_CECStart(HI_UNF_HDMI_ID_0,HI_FALSE);
                memset(&(pstChnAttr[HI_UNF_HDMI_ID_0].stCECStatus), 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));
#endif
            }
            hdmi_SetAndroidState(STATE_HOTPLUGOUT);
        }
        else
        {
            COM_ERR("Unknow Event:0x%x\n", event);
        }
        COM_INFO("line:%d,event:%d\n",__LINE__,event);

        hdmi_ProcEvent(event, u32ProcID);
    }
}

HI_U32 DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HDMI_OPEN_S *pOpen, HI_U32 FromUserSpace,HI_U32 u32ProcID)
{
    //HI_U32 Ret;
    HI_BOOL bOpenLastTime = DRV_Get_IsChnOpened(enHdmi);
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);

    COM_INFO("Enter DRV_HDMI_Open\n");
    HDMI_CHECK_ID(enHdmi);

    g_HDMIOpenNum ++; //record open num

    /* Need to set this  param */
    DRV_Set_DefaultOutputMode(enHdmi,pOpen->enDefaultMode);

    //set output mode
     COM_INFO("enForceMode:%d\n", DRV_Get_DefaultOutputMode(enHdmi));

    //In Mce or Boot,we setted hdmi mode.so we use Hdmi in reg.
    if(DRV_Get_IsMce2App() || DRV_Get_IsOpenedInBoot())
    {
        if(ReadByteHDMITXP1(AUDP_TXCTRL_ADDR) & BIT_TXHDMI_MODE)
        {
            pstAppAttr->bEnableHdmi = HI_TRUE;
        }
        else
        {
            pstAppAttr->bEnableHdmi = HI_FALSE;
        }
    }
    //In None Mce mode, we use User setting defalut mode defaut mode
    else
    {
        if(!DRV_Get_IsValidSinkCap(enHdmi))
        {
            if(HI_UNF_HDMI_DEFAULT_ACTION_DVI != DRV_Get_DefaultOutputMode(enHdmi))
            {
                pstAppAttr->bEnableHdmi = HI_TRUE;
            }
        }
    }

    DRV_Set_ChnOpen(enHdmi,HI_TRUE);

    if (g_HDMIUserInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_USER;
        if (g_HDMIKernelInitNum > 0)
        {
            /* It means we have initalized in Kernel and user */
            //We need to Reset a HotPlug Event to User
            if (HI_TRUE == SI_HPD_Status())
            {
        		DelayMS(200);
    		    SI_TX_PHY_PowerDown(HI_FALSE);
                hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_HOTPLUG);
            }
            else
            {
                SI_TX_PHY_PowerDown(HI_TRUE);
                hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_NO_PLUG);
            }
        }
        else
        {
            COM_INFO("SI_HPD_Status():%d\n", SI_HPD_Status());
            if (HI_TRUE == SI_HPD_Status())
            {
                HI_U8 tempOutputState = SI_GetHDMIOutputStatus();
                COM_INFO("tempOutputState:%d\n", tempOutputState);

                if((tempOutputState == CABLE_PLUGIN_HDMI_OUT) || (tempOutputState == CABLE_PLUGIN_DVI_OUT))
                {
                    //When OutputState is Out, Phy, HDMI should be open collectly!
                    COM_INFO("Warring:Force to send out a HotPlug Event!\n");
                    //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
            		DelayMS(200);
        		    SI_TX_PHY_PowerDown(HI_FALSE);
                    hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_HOTPLUG);
                }
            }
        }
    }
    else if (g_HDMIKernelInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_KERNEL;
    }

    if (!DRV_Get_IsMce2App())
    {
        if (bOpenLastTime)
        {
            return HI_SUCCESS;
        }
    }

    //// only do it in first time////
    // Enable Interrupts: VSync, Ri check, HotPlug
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);

    COM_INFO("Leave DRV_HDMI_Open\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    COM_INFO("Enter DRV_HDMI_Close g_HDMIOpenNum:%d\n", g_HDMIOpenNum);
    HDMI_CHECK_ID(enHdmi);

    //Stop HDMI IP
    if( g_HDMIOpenNum > 1)
    {
        g_HDMIOpenNum --;
        return HI_SUCCESS;
    }
    else if(g_HDMIOpenNum == 1)
    {
        g_HDMIOpenNum = 0;
    }
    else
    {
    //Just do following action.
    }

    if(DRV_Get_IsChnStart(enHdmi))
    {
        SI_SetHdmiVideo(HI_FALSE);
        SI_SetHdmiAudio(HI_FALSE);
        SI_PowerDownHdmiTx();
#if defined (HDCP_SUPPORT)
        /*Set HDCP Off */
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
        msleep(50);
#endif
        DRV_Set_ChnStart(enHdmi,HI_FALSE);
    }
    //Close HDMI
    if (DRV_Get_IsChnOpened(enHdmi))
    {
        DRV_Set_ChnOpen(enHdmi,HI_FALSE);
    }
    //No callback
    g_UserCallbackFlag = HDMI_CALLBACK_NULL;
    //Disable HDMI IP
    SI_DisableHdmiDevice();
    SI_CloseHdmiDevice();

    COM_INFO("Leave DRV_HDMI_Close\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pstSinkAttr)
{
    COM_INFO("Enter DRV_HDMI_GetSinkCapability\n");
    HDMI_CHECK_NULL_PTR(pstSinkAttr);
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (HI_SUCCESS != SI_GetHdmiSinkCaps(pstSinkAttr))
    {
        COM_WARN("GetHdmiSinkCaps error.\n");
        return HI_FAILURE;
    }

    COM_INFO("Leave DRV_HDMI_GetSinkCapability\n");
    return HI_SUCCESS;
}

static HI_U32 hdmi_VideoAttrChanged(HI_UNF_HDMI_ID_E enHdmi, HDMI_VIDEO_ATTR_S *pstAttr1, HDMI_VIDEO_ATTR_S *pstAttr2,
    HI_BOOL *pVUpdate)
{
    *pVUpdate = HI_FALSE;

    /* HDMI has not started, we set this value */
    if (!DRV_Get_IsChnStart(enHdmi))
    {
        *pVUpdate = HI_TRUE;
        return HI_SUCCESS;
    }

    if ( (pstAttr1->enVideoFmt          != pstAttr2->enVideoFmt)
        || (pstAttr1->b3DEnable           != pstAttr2->b3DEnable)
        || (pstAttr1->u83DParam           != pstAttr2->u83DParam)
       )
    {
        *pVUpdate = HI_TRUE;
    }
    else
    {
        COM_INFO("We do not need to update Video!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_U32 hdmi_AudioAttrChanged(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstAttr1, HDMI_AUDIO_ATTR_S *pstAttr2,
    HI_BOOL *pAUpdate)
{
    *pAUpdate = HI_TRUE;

    /* HDMI Has not started, we set this value */
    if (!DRV_Get_IsChnStart(enHdmi))
    {
        *pAUpdate = HI_TRUE;
        return HI_SUCCESS;
    }

    /* Same setting, return directly */
    if ( (pstAttr1->enSoundIntf                == pstAttr2->enSoundIntf)
        && (pstAttr1->bIsMultiChannel     == pstAttr2->bIsMultiChannel)
        && (pstAttr1->enSampleRate        == pstAttr2->enSampleRate)
        && (pstAttr1->u8DownSampleParm    == pstAttr2->u8DownSampleParm)
        && (pstAttr1->enBitDepth          == pstAttr2->enBitDepth)
        && (pstAttr1->u8I2SCtlVbit        == pstAttr2->u8I2SCtlVbit)
        )
    {
        *pAUpdate = HI_FALSE;
        COM_INFO("Same as before, do not need to setting!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

//when need update audio or hdcpflag,but did not need update video,then return update flag false
//when change video,we need recfg all
//return HI_FAILUR need not update anyone
//return HI_SUCCESS && pUpdate == TRUE  need recfg all
//return HI_SUCCESS && pUpdate == FALSE only update audio && hdcp
static HI_U32 hdmi_AppAttrChanged(HI_UNF_HDMI_ID_E enHdmi, HDMI_APP_ATTR_S *pstAttr1, HDMI_APP_ATTR_S *pstAttr2,
    HI_BOOL *pUpdate)
{
    *pUpdate = HI_FALSE;

    if (!DRV_Get_IsChnStart(enHdmi))
    {
        if(DRV_Get_IsMce2App() || DRV_Get_IsOpenedInBoot())
        {
            if((pstAttr1->bEnableHdmi != pstAttr2->bEnableHdmi)
                || (pstAttr1->enVidOutMode != pstAttr2->enVidOutMode))
            {
                *pUpdate = HI_TRUE;
                DRV_Set_Mce2App(HI_FALSE);
                DRV_Set_OpenedInBoot(HI_FALSE);
            }
            else
            {
                *pUpdate = HI_FALSE;
            }
        }
        else
        {
            *pUpdate = HI_TRUE;
        }
        return HI_SUCCESS;
    }

    if ((pstAttr1->bEnableHdmi            != pstAttr2->bEnableHdmi)
        || (pstAttr1->bEnableVideo        != pstAttr2->bEnableVideo)
        || (pstAttr1->enVidOutMode        != pstAttr2->enVidOutMode)
        || (pstAttr1->enDeepColorMode     != pstAttr2->enDeepColorMode)
        || (pstAttr1->bxvYCCMode          != pstAttr2->bxvYCCMode)
       )
    {
        COM_INFO("App Attr need update video!\n");
        *pUpdate = HI_TRUE;
    }
    else if((pstAttr1->bEnableAudio        != pstAttr2->bEnableAudio)
        || (pstAttr1->bEnableAviInfoFrame != pstAttr2->bEnableAviInfoFrame)
        || (pstAttr1->bEnableAudInfoFrame != pstAttr2->bEnableAudInfoFrame)
        || (pstAttr1->bEnableSpdInfoFrame != pstAttr2->bEnableSpdInfoFrame)
        || (pstAttr1->bEnableMpegInfoFrame!= pstAttr2->bEnableMpegInfoFrame)
        || (pstAttr1->bDebugFlag          != pstAttr2->bDebugFlag)
        || (pstAttr1->bHDCPEnable         != pstAttr2->bHDCPEnable))
    {
        COM_INFO("App Attr need not update video!\n");
    }
    else
    {
        COM_INFO("App Attr need not update anything!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}




HI_BOOL get_current_rgb_mode(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMI_APP_ATTR_S *pstAppAttr = DRV_Get_AppAttr(enHdmi);
    return (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAppAttr->enVidOutMode )?HI_TRUE:HI_FALSE;
}

HI_U32 DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr)
{
    HI_U32 Ret = HI_SUCCESS;
    HI_U32 ForceUpdateFlag = HI_FALSE;
    HI_U32 PartUpdateFlag = HI_FALSE;

    COM_INFO("Enter DRV_HDMI_SetAttr\n");

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAttr);

    // SetAppAttr only cfg hdcp,
    // the other attr will be config in DRV_HDMI_SetVO/AOAttr && hdmi_AdjustAVI/AUDInfoFrame
    DRV_HDMI_SetAPPAttr(enHdmi, &pstAttr->stAppAttr,HI_FALSE);

    //Force update flag can changed in set format && DRV_HDMI_SetAPPAttr
    ForceUpdateFlag = DRV_Get_IsNeedForceUpdate(enHdmi);
    if(ForceUpdateFlag == HI_TRUE)
    {
        PartUpdateFlag = HI_TRUE;
    }
    else
    {
        PartUpdateFlag = DRV_Get_IsNeedPartUpdate(enHdmi);
    }
    COM_INFO("ForceUpdateFlag %d, PartUpdateFlag %d \n",ForceUpdateFlag,PartUpdateFlag);

    DRV_Set_ForceUpdateFlag(enHdmi,HI_FALSE);
    DRV_Set_PartUpdateFlag(enHdmi,HI_FALSE);
    if(DRV_Get_IsOpenedInBoot())
    {
        COM_INFO("Green Channel First Time \n");

        //when opend in boot,then audio not be configed.
        DRV_HDMI_SetAOAttr(enHdmi, &pstAttr->stAudioAttr,PartUpdateFlag);
        hdmi_AdjustAUDInfoFrame(enHdmi);
        return HI_SUCCESS;
    }
    else if((HI_FALSE == ForceUpdateFlag)&&(HI_FALSE == PartUpdateFlag)&& DRV_Get_IsMce2App())
    {
        COM_INFO(" Mce2App  \n");
        return HI_SUCCESS;
    }

    DRV_HDMI_SetVOAttr(enHdmi, &pstAttr->stVideoAttr,ForceUpdateFlag);
    DRV_HDMI_SetAOAttr(enHdmi, &pstAttr->stAudioAttr,PartUpdateFlag);

    hdmi_AdjustAVIInfoFrame(enHdmi);
    hdmi_AdjustVSDBInfoFrame(enHdmi);
    hdmi_AdjustAUDInfoFrame(enHdmi);


    COM_INFO("Leave DRV_HDMI_SetAttr\n");
    return Ret;
}

HI_U32 DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr)
{
    HDMI_ATTR_S *pstHDMIAttr = DRV_Get_HDMIAttr(enHdmi);
    COM_INFO("Enter DRV_HDMI_GetAttr\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAttr);

    pstHDMIAttr->stAppAttr.bEnableVideo = HI_TRUE;
    memcpy(pstAttr, pstHDMIAttr, sizeof(HDMI_ATTR_S));

    COM_INFO("Leave DRV_HDMI_GetAttr\n");
    return HI_SUCCESS;
}

#if defined (CEC_SUPPORT)
HI_U32 DRV_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus)
{
    HI_UNF_HDMI_CEC_STATUS_S *pstCecStatus = DRV_Get_CecStatus(enHdmi);
    memset(pStatus, 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));
    memcpy(pStatus, pstCecStatus, sizeof(HI_UNF_HDMI_CEC_STATUS_S));

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetCECAddress(HI_U8 *pPhyAddr, HI_U8 *pLogicalAddr)
{
    HI_UNF_HDMI_CEC_STATUS_S *pstCecStatus = DRV_Get_CecStatus(HI_UNF_HDMI_ID_0);
    //Only invoke in private mode
    if (!DRV_Get_IsCECStart(HI_UNF_HDMI_ID_0))
    {
        return HI_ERR_HDMI_DEV_NOT_OPEN;
    }

    memcpy(pPhyAddr, pstCecStatus->u8PhysicalAddr, 4);
    *pLogicalAddr = pstCecStatus->u8LogicalAddr;

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, const HI_UNF_HDMI_CEC_CMD_S  *pCECCmd)
{
    HI_UNF_HDMI_CEC_STATUS_S *pstCecStatus = DRV_Get_CecStatus(enHdmi);
    HI_U32 Ret = HI_SUCCESS;

    COM_INFO("Enter DRV_HDMI_SetCECCommand\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCmd);

    if(!DRV_Get_IsCECStart(enHdmi))
    {
        CEC_ERR("CEC do not start\n");
        return HI_ERR_HDMI_DEV_NOT_OPEN;
    }

    if(pCECCmd->enSrcAdd != pstCecStatus->u8LogicalAddr)
    {
        COM_INFO("Invalid enSrcAdd:0x%x\n", pCECCmd->enSrcAdd);
        return HI_ERR_HDMI_INVALID_PARA;
    }
    Ret = SI_CEC_SendCommand((HI_UNF_HDMI_CEC_CMD_S *)pCECCmd);

    CEC_INFO("Leave DRV_HDMI_SetCECCommand\n");
    return Ret;
}

extern unsigned int  get_cec_msg(HI_UNF_HDMI_CEC_CMD_S *rx_cmd, unsigned int num, HI_U32 timeout);
HI_U32 DRV_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout)
{
    return get_cec_msg(pCECCmd, 1, timeout);
}
#endif

static HI_U32 hdmi_Create_AVI_Infoframe(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *punAVIInfoFrame, HI_U8 *pu8AviInfoFrame)
{
    HI_U8  u8AviInfoFrameByte = 0;
    hdmi_VideoIdentification_t *pstVidCode;
    HI_S32 retval = HI_SUCCESS;
    HI_U32 VidIdCode;

    HI_DRV_DISP_FMT_E encFmt;

    /* HDMI AVI Infoframe is use Version = 0x02 in HDMI1.3 */

    /* Fill Data Byte 1 */
    u8AviInfoFrameByte=0;
    /* Scan information bits 0-1:S0,S1 */
    /*
       S1 S0 Scan Information
       0   0    No Data
       0   1   overscanned
       1   0   underscanned
       1   1   Future
       */
    switch(punAVIInfoFrame->enScanInfo)
    {
        case HDMI_SCAN_INFO_NO_DATA :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_NO_DATA;
            break;
        case HDMI_SCAN_INFO_OVERSCANNED :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_OVERSCANNED;
            break;
        case HDMI_SCAN_INFO_UNDERSCANNED :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_UNDERSCANNED;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Bar Information bits 2-3:B0,B1 */
    /*
       B1 B0  Bar Info
       0   0  not valid
       0   1  Vert. Bar Info valid
       1   0  Horiz.Bar Info Valid
       1   1  Vert. and Horiz. Bar Info valid
       */
    switch (punAVIInfoFrame->enBarInfo)
    {
        case HDMI_BAR_INFO_NOT_VALID :
            u8AviInfoFrameByte |= (HI_U8) 0x00;
            break;
        case HDMI_BAR_INFO_V :
            u8AviInfoFrameByte |= (HI_U8) 0x04;
            break;
        case HDMI_BAR_INFO_H :
            u8AviInfoFrameByte |= (HI_U8) 0x08;
            break;
        case HDMI_BAR_INFO_VH :
            u8AviInfoFrameByte |= (HI_U8) 0x0C;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Active information bit 4:A0 */
    /*
       A0 Active Format Information Present
       0        No Data
       1      Active Format(R0…R3) Information valid
       */
    if (punAVIInfoFrame->bActive_Infor_Present)
    {
        u8AviInfoFrameByte |= (HI_U8)0x10;  /* Active Format Information Valid */
    }
    else
    {
        u8AviInfoFrameByte &= ~(HI_U8)0x10;  /* Active Format Information Valid */
    }
    /* Output Type bits 5-6:Y0,Y1 */
    /*
       Y1 Y0  RGB orYCbCr
       0  0   RGB (default)
       0  1   YCbCr 4:2:2
       1  0   YCbCr 4:4:4
       1  1    Future
       */
    COM_INFO("punAVIInfoFrame->enOutputType:%d\n", punAVIInfoFrame->enOutputType);
    switch (punAVIInfoFrame->enOutputType)
    {
        case HI_UNF_HDMI_VIDEO_MODE_RGB444 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR422 :
            u8AviInfoFrameByte |= (HI_U8)0x20;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR444 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        default :
            COM_INFO("Error Output format *******\n");
            retval = HI_FAILURE;
            break;
    }
    pu8AviInfoFrame[0]= (HI_U8)(u8AviInfoFrameByte&0x7F);

    /* Fill Data byte 2 */
    u8AviInfoFrameByte=0;
    /* Active Format aspect ratio bits 0-3:R0...R3 */
    /*
       R3 R2 R1 R0  Active Format Aspect Ratio
       1  0  0  0   Same as picture aspect ratio
       1  0  0  1   4:3 (Center)
       1  0  1  0   16:9 (Center)
       1  0  1  1   14:9 (Center)
       */

    COM_INFO("Active Format aspect ratio  set to 0x1000:Same as picture aspect ratio\n");
    u8AviInfoFrameByte |= (HI_U8) 0x08;

    switch (punAVIInfoFrame->enAspectRatio)
    {
        case HI_UNF_HDMI_ASPECT_RATIO_4TO3 :
            u8AviInfoFrameByte |= (HI_U8) 0x10;
            break;
        case HI_UNF_HDMI_ASPECT_RATIO_16TO9 :
            u8AviInfoFrameByte |= (HI_U8) 0x20;
            break;
        default :
            u8AviInfoFrameByte |=  (HI_U8) 0x00;
            break;
    }

    /* Colorimetry bits 6-7 of data byte2:C0,C1 */
    /*
       C1 C0    Colorim
       0   0    No Data
       0   1    SMPTE 170M[1] ITU601 [5]
       1   0    ITU709 [6] 1 0 16:9
       1   1    Extended Colorimetry Information Valid (colorimetry indicated in bits EC0, EC1,
       EC2. See Table 11)
       */
    switch (punAVIInfoFrame->enColorimetry)
    {
        case HDMI_COLORIMETRY_ITU601 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        case HDMI_COLORIMETRY_ITU709 :
            u8AviInfoFrameByte |= (HI_U8)0x80;
            break;
        case HDMI_COLORIMETRY_XVYCC_601 :
        case HDMI_COLORIMETRY_XVYCC_709 :
        case HDMI_COLORIMETRY_EXTENDED :
            u8AviInfoFrameByte |= (HI_U8)0xC0;
            break;
        default :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    pu8AviInfoFrame[1] = (HI_U8)(u8AviInfoFrameByte&0XFF);

    /* Fill data Byte 3: Picture Scaling bits 0-1:SC0,SC1 */
    u8AviInfoFrameByte=0;
    /*
       SC1  SC0   Non-Uniform Picture Scaling
       0     0    No Known non-uniform Scaling
       0     1    Picture has been scaled horizontally
       1     0    Picture has been scaled vertically
       1     1    Picture has been scaled horizontally and vertically
       */
    switch (punAVIInfoFrame->enPictureScaling)
    {
        case HDMI_PICTURE_NON_UNIFORM_SCALING :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_PICTURE_SCALING_H :
            u8AviInfoFrameByte |= (HI_U8)0x01;
            break;
        case HDMI_PICTURE_SCALING_V :
            u8AviInfoFrameByte |= (HI_U8)0x02;
            break;
        case HDMI_PICTURE_SCALING_HV :
            u8AviInfoFrameByte |= (HI_U8)0x03;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill data Byte 3: RGB quantization range bits 2-3:Q0,Q1 */
    /*
       Q1  Q0  RGB Quantization Range
       0   0   Default (depends on video format)
       0   1   Limited Range
       1   0   Full Range
       1   1   Reserved
       */
    switch (punAVIInfoFrame->enRGBQuantization)
    {
        case HDMI_RGB_QUANTIZATION_DEFAULT_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_RGB_QUANTIZATION_LIMITED_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x04;
            break;
        case HDMI_RGB_QUANTIZATION_FULL_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x08;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill data Byte 3: Extended colorimtery range bits 4-6:EC0,EC1,EC2 */
    /*
       EC2 EC1 EC0   Extended Colorimetry
       0   0   0      xvYCC601
       0   0   1      xvYCC709
       -   -   -      All other values reserved
       */
    /*
       xvYCC601 is based on the colorimetry defined in ITU-R BT.601.
       xvYCC709 is based on the colorimetry defined in ITU-R BT.709.
       */
    switch (punAVIInfoFrame->enColorimetry)
    {
        case HDMI_COLORIMETRY_XVYCC_601 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_COLORIMETRY_XVYCC_709 :
            u8AviInfoFrameByte |= (HI_U8)0x10;
            break;
        default:
            break;
    }

    /* Fill data Byte 3: IT content bit 7:ITC
       ITC  IT content
       0    No data
       1    IT content
       */
    if (punAVIInfoFrame->bIsITContent)
        u8AviInfoFrameByte |= 0x80;
    else
        u8AviInfoFrameByte &= ~0x80;

    pu8AviInfoFrame[2] = (HI_U8)(u8AviInfoFrameByte&0XFF);

    /* Fill Data byte 4: Video indentification data Code, Bit0~7:VIC0 ~ VIC6 */
    u8AviInfoFrameByte=0;
    encFmt = DRV_HDMI_Unf2DispFmt(punAVIInfoFrame->enTimingMode);
    pstVidCode = hdmi_GetVideoCode(encFmt);


    VidIdCode = pstVidCode->VidIdCode;
    if (HI_UNF_HDMI_ASPECT_RATIO_16TO9 == punAVIInfoFrame->enAspectRatio)
    {
        if (encFmt == HI_DRV_DISP_FMT_480P_60)
        {
            VidIdCode = 3;
            COM_INFO("Sepcail setting:change pstVidCode(480p_60 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if (encFmt == HI_DRV_DISP_FMT_576P_50)
        {
            VidIdCode = 18;
            COM_INFO("Sepcail setting:change pstVidCode(576p_50 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if ((encFmt == HI_DRV_DISP_FMT_PAL)||
            (encFmt == HI_DRV_DISP_FMT_PAL_B)||
            (encFmt == HI_DRV_DISP_FMT_PAL_B1)||
            (encFmt == HI_DRV_DISP_FMT_PAL_D)||
            (encFmt == HI_DRV_DISP_FMT_PAL_D1)||
            (encFmt == HI_DRV_DISP_FMT_PAL_G)||
            (encFmt == HI_DRV_DISP_FMT_PAL_H)||
            (encFmt == HI_DRV_DISP_FMT_PAL_K)||
            (encFmt == HI_DRV_DISP_FMT_PAL_I)||
            (encFmt == HI_DRV_DISP_FMT_PAL_M)||
            (encFmt == HI_DRV_DISP_FMT_PAL_N)||
            (encFmt == HI_DRV_DISP_FMT_PAL_Nc)||
            (encFmt == HI_DRV_DISP_FMT_PAL_60)||
            (encFmt == HI_DRV_DISP_FMT_1440x576i_50)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_SIN)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_COS)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_L)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_B)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_G)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_D)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_K)||
            (encFmt == HI_DRV_DISP_FMT_SECAM_H))

        {
            VidIdCode = 22;
            COM_INFO("Sepcail setting:change pstVidCode(576i_50 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if ((encFmt == HI_DRV_DISP_FMT_NTSC)||
            (encFmt == HI_DRV_DISP_FMT_NTSC_J)||
            (encFmt == HI_DRV_DISP_FMT_NTSC_443)||
            (encFmt == HI_DRV_DISP_FMT_1440x480i_60))
        {
            VidIdCode = 7;
            COM_INFO("Sepcail setting:change pstVidCode(480i_60 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else
        {
            COM_INFO("do not need to change VIC\n");
        }
    }

    pu8AviInfoFrame[3] = (HI_U8)(VidIdCode & 0x7F);
    /* Fill Data byte 5: Pixel repetition, Bit0~3:PR0~PR3 */
    /*
       PR3 PR2 PR1 PR0 Pixel Repetition Factor
       0   0   0    0   No Repetition (i.e., pixel sent once)
       0   0   0    1   pixel sent 2 times (i.e., repeated once)
       0   0   1    0   pixel sent 3 times
       0   0   1    1   pixel sent 4 times
       0   1   0    0   pixel sent 5 times
       0   1   0    1   pixel sent 6 times
       0   1   1    0   pixel sent 7 times
       0   1   1    1   pixel sent 8 times
       1   0   0    0   pixel sent 9 times
       1   0   0    1   pixel sent 10 times
       0Ah-0Fh          Reserved
       */
    u8AviInfoFrameByte = (HI_U8)(punAVIInfoFrame->u32PixelRepetition& 0x0F);

    /* Fill Data byte 5: Content Type, Bit4~5:CN0~CN1 */
    /*
       ITC  CN1 CN0 Pixel Repetition Factor
       (1)   0    0   Graphics
       (1)   0    1   Photo
       (1)   1    0   Cinema
       (1)   1    1   Game
       */
    switch (punAVIInfoFrame->enContentType)
    {
        case HDMI_CONTNET_GRAPHIC:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_CONTNET_PHOTO:
            u8AviInfoFrameByte |= (HI_U8)0x10;
            break;
        case HDMI_CONTNET_CINEMA:
            u8AviInfoFrameByte |= (HI_U8)0x20;
            break;
        case HDMI_CONTNET_GAME:
            u8AviInfoFrameByte |= (HI_U8)0x30;
            break;
        default:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    /* Fill Data byte 5: YCC Full Range, Bit6~7:YQ0~YQ1 */
    /*
       YQ1 YQ0 Pixel Repetition Factor
       0    0   Limitation Range
       0    1   Full Range
       */
    switch (punAVIInfoFrame->enYCCQuantization)
    {
        case HDMI_YCC_QUANTIZATION_LIMITED_RANGE:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_YCC_QUANTIZATION_FULL_RANGE:
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        default:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    pu8AviInfoFrame[4]= (HI_U8)(u8AviInfoFrameByte&0XFF);

    if ( (0 == punAVIInfoFrame->u32LineNEndofTopBar) && (0 == punAVIInfoFrame->u32LineNStartofBotBar)
        && (0 == punAVIInfoFrame->u32PixelNEndofLeftBar) && (0 == punAVIInfoFrame->u32PixelNStartofRightBar) )
    {
        punAVIInfoFrame->u32LineNEndofTopBar      = pstVidCode->Active_X;
        punAVIInfoFrame->u32LineNStartofBotBar    = pstVidCode->Active_H;
        punAVIInfoFrame->u32PixelNEndofLeftBar    = pstVidCode->Active_Y;
        punAVIInfoFrame->u32PixelNStartofRightBar = pstVidCode->Active_W;
    }
    if(punAVIInfoFrame->enBarInfo == HDMI_BAR_INFO_NOT_VALID)
    {
        punAVIInfoFrame->u32LineNEndofTopBar      = 0;
        punAVIInfoFrame->u32LineNStartofBotBar    = 0;
        punAVIInfoFrame->u32PixelNEndofLeftBar    = 0;
        punAVIInfoFrame->u32PixelNStartofRightBar = 0;
    }
    /* Fill Data byte 6  */
    pu8AviInfoFrame[5] = (HI_U8)(punAVIInfoFrame->u32LineNEndofTopBar & 0XFF);

    /* Fill Data byte 7  */
    pu8AviInfoFrame[6] = (HI_U8)((punAVIInfoFrame->u32LineNEndofTopBar>>8) & 0XFF);

    /* Fill Data byte 8  */
    pu8AviInfoFrame[7] = (HI_U8)(punAVIInfoFrame->u32LineNStartofBotBar & 0XFF);

    /* Fill Data byte 9  */
    pu8AviInfoFrame[8] = (HI_U8)((punAVIInfoFrame->u32LineNStartofBotBar>>8) & 0XFF);

    /* Fill Data byte 10  */
    pu8AviInfoFrame[9] = (HI_U8)(punAVIInfoFrame->u32PixelNEndofLeftBar &0XFF);

    /* Fill Data byte 11  */
    pu8AviInfoFrame[10] = (HI_U8)((punAVIInfoFrame->u32PixelNEndofLeftBar>>8) &0XFF);

    /* Fill Data byte 12  */
    pu8AviInfoFrame[11] = (HI_U8)(punAVIInfoFrame->u32PixelNStartofRightBar &0XFF);

    /* Fill Data byte 13  */
    pu8AviInfoFrame[12] = (HI_U8)((punAVIInfoFrame->u32PixelNStartofRightBar>>8) &0XFF);

    return retval;
}

static HI_U32 hdmi_Create_Audio_Infoframe(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame, HI_U8 *pu8AudioInfoFrame)
{
    HI_U8 u8AudioInfoFrameByte=0;
    HI_S32 retval = HI_SUCCESS;
    u8AudioInfoFrameByte=0;

    switch (punAUDInfoFrame->u32ChannelCount)
    {
        case 2 :
            u8AudioInfoFrameByte |= 0x01;
            break;
        case 3 :
            u8AudioInfoFrameByte |= 0x02;
            break;
        case 4 :
            u8AudioInfoFrameByte |= 0x03;
            break;
        case 5 :
            u8AudioInfoFrameByte |= 0x04;
            break;
        case 6 :
            u8AudioInfoFrameByte |= 0x05;
            break;
        case 7 :
            u8AudioInfoFrameByte |= 0x06;
            break;
        case 8 :
            u8AudioInfoFrameByte |= 0x07;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }

    switch (punAUDInfoFrame->enCodingType)
    {
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3 :
            u8AudioInfoFrameByte |= 0x20;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1 :
            u8AudioInfoFrameByte |= 0x30;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3 :
            u8AudioInfoFrameByte|= 0x40;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2 :
            u8AudioInfoFrameByte |= 0x50;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC :
            u8AudioInfoFrameByte |= 0x60;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS :
            u8AudioInfoFrameByte |= 0x70;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC :
            u8AudioInfoFrameByte |= 0x80;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT :
            u8AudioInfoFrameByte |= 0x90;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP :
            u8AudioInfoFrameByte |= 0xA0;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD :
            u8AudioInfoFrameByte |= 0xB0;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT :
            u8AudioInfoFrameByte |= 0xC0;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_DST :
            u8AudioInfoFrameByte |= 0xD0;
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO :
            u8AudioInfoFrameByte |= 0xE0;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }
    pu8AudioInfoFrame[0] = (HI_U8)(u8AudioInfoFrameByte& 0xF7);

    u8AudioInfoFrameByte=0;
    /* Fill Sample Size (Data Byte 2) bits2: 0~1*/
    /*
       SS1 SS0    Sample Size
       0   0      Refer to Stream header
       0   1      16 bit
       1   0      20 bit
       1   1      24 bit
       */
    switch (punAUDInfoFrame->u32SampleSize)
    {
        case 16 :
            u8AudioInfoFrameByte |= 0x01;
            break;
        case 20 :
            u8AudioInfoFrameByte |= 0x02;
            break;
        case 24 :
            u8AudioInfoFrameByte |= 0x03;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }

    /* Fill Sample Frequency (Data Byte 2)bits3: 2~4*/
    /*
       SF2 SF1 SF0 Sampling Frequency
       0   0   0   Refer to Stream Header
       0   0   1   32 kHz
       0   1   0   44.1 kHz (CD)
       0   1   1   48 kHz
       1   0   0   88.2 kHz
       1   0   1   96 kHz
       1   1   0   176.4 kHz
       1   1   1   192 kHz
       */
    switch (punAUDInfoFrame->u32SamplingFrequency)
    {
        case 32000 :
            u8AudioInfoFrameByte |= 0x04;
            break;
        case 44100 :
            u8AudioInfoFrameByte |= 0x08;
            break;
        case 48000 :
            u8AudioInfoFrameByte |= 0x0C;
            break;
        case 88200 :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case 96000 :
            u8AudioInfoFrameByte |= 0x14;
            break;
        case 176400 :
            u8AudioInfoFrameByte |= 0x18;
            break;
        case 192000 :
            u8AudioInfoFrameByte |= 0x1C;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }
    pu8AudioInfoFrame[1] = (HI_U8)(u8AudioInfoFrameByte& 0x1F);

    u8AudioInfoFrameByte=0;
    /* Fill the Bit rate coefficient for the compressed audio format (Data byte 3)*/
    switch (punAUDInfoFrame->enCodingType)
    {
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3 :
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS :
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1 :
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2 :
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3 :
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC :
            pu8AudioInfoFrame[2] = (HI_U8)0XFF;//? Data Byte 3 is reserved and shall be zero.
            break;
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM :
        default :
            pu8AudioInfoFrame[2] = 0X00;
            break;
    }

    /* Data Bytes 4 and 5 apply only to multi-channel (i.e., more than two channels) uncompressed audio. */
    /* Fill Channel allocation (Data Byte 4) */
    /*
       CA(binary)       CA(hex)  Channel Number
       7 6 5 4 3 2 1 0          8 7 6 5 4 3 2 1
       0 0 0 0 0 0 0 0  00      - - - - - - FR FL
       0 0 0 0 0 0 0 1  01      - - - - - LFE FR FL
       0 0 0 0 0 0 1 0  02      - - - - FC - FR FL
       0 0 0 0 0 0 1 1  03      - - - - FC LFE FR FL
       0 0 0 0 0 1 0 0  04      - - - RC - - FR FL
       0 0 0 0 0 1 0 1  05      - - - RC - LFE FR FL
       0 0 0 0 0 1 1 0  06      - - - RC FC - FR FL
       0 0 0 0 0 1 1 1  07      - - - RC FC LFE FR FL
       0 0 0 0 1 0 0 0  08      - - RR RL - - FR FL
       0 0 0 0 1 0 0 1  09      - - RR RL - LFE FR FL
       0 0 0 0 1 0 1 0  0A      - - RR RL FC - FR FL
       0 0 0 0 1 0 1 1  0B      - - RR RL FC LFE FR FL
       0 0 0 0 1 1 0 0  0C      - RC RR RL - - FR FL
       0 0 0 0 1 1 0 1  0D      - RC RR RL - LFE FR FL
       0 0 0 0 1 1 1 0  0E      - RC RR RL FC - FR FL
       0 0 0 0 1 1 1 1  0F      - RC RR RL FC LFE FR FL
       0 0 0 1 0 0 0 0  10      RRC RLC RR RL - - FR FL
       0 0 0 1 0 0 0 1  11      RRC RLC RR RL - LFE FR FL
       0 0 0 1 0 0 1 0  12      RRC RLC RR RL FC - FR FL
       0 0 0 1 0 0 1 1  13      RRC RLC RR RL FC LFE FR FL
       0 0 0 1 0 1 0 0  14      FRC FLC - - - - FR FL
       0 0 0 1 0 1 0 1  15      FRC FLC - - - LFE FR FL
       0 0 0 1 0 1 1 0  16      FRC FLC - - FC - FR FL
       0 0 0 1 0 1 1 1  17      FRC FLC - - FC LFE FR FL
       0 0 0 1 1 0 0 0  18      FRC FLC - RC - - FR FL
       0 0 0 1 1 0 0 1  19      FRC FLC - RC - LFE FR FL
       0 0 0 1 1 0 1 0  1A      FRC FLC - RC FC - FR FL
       0 0 0 1 1 0 1 1  1B      FRC FLC - RC FC LFE FR FL
       0 0 0 1 1 1 0 0  1C      FRC FLC RR RL - - FR FL
       0 0 0 1 1 1 0 1  1D      FRC FLC RR RL - LFE FR FL
       0 0 0 1 1 1 1 0  1E      FRC FLC RR RL FC - FR FL
       0 0 0 1 1 1 1 1  1F      FRC FLC RR RL FC LFE FR FL
       */
    pu8AudioInfoFrame[3] = (HI_U8)(punAUDInfoFrame->u32ChannelAlloc &0XFF);

    /* Fill Level Shift (Data Byte 5) bits4:3~7 */
    /*
       LSV3 LSV2 LSV1 LSV0 Level Shift Value
       0     0    0    0     0dB
       0     0    0    1     1dB
       0     0    1    0     2dB
       0     0    1    1     3dB
       0     1    0    0     4dB
       0     1    0    1     5dB
       0     1    1    0     6dB
       0     1    1    1     7dB
       1     0    0    0     8dB
       1     0    0    1     9dB
       1     0    1    0    10dB
       1     0    1    1    11dB
       1     1    0    0    12dB
       1     1    0    1    13dB
       1     1    1    0    14dB
       1     1    1    1    15dB
       */
    switch (punAUDInfoFrame->u32LevelShift)
    {
        case 0 :
            u8AudioInfoFrameByte |= 0x00;
            break;
        case 1 :
            u8AudioInfoFrameByte |= 0x08;
            break;
        case 2 :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case 3 :
            u8AudioInfoFrameByte |= 0x18;
            break;
        case 4 :
            u8AudioInfoFrameByte |= 0x20;
            break;
        case 5 :
            u8AudioInfoFrameByte |= 0x28;
            break;
        case 6 :
            u8AudioInfoFrameByte |= 0x30;
            break;
        case 7 :
            u8AudioInfoFrameByte |= 0x38;
            break;
        case 8 :
            u8AudioInfoFrameByte |= 0x40;
            break;
        case 9 :
            u8AudioInfoFrameByte |= 0x48;
            break;
        case 10 :
            u8AudioInfoFrameByte |= 0x50;
            break;
        case 11 :
            u8AudioInfoFrameByte |= 0x58;
            break;
        case 12 :
            u8AudioInfoFrameByte |= 0x60;
            break;
        case 13 :
            u8AudioInfoFrameByte |= 0x68;
            break;
        case 14 :
            u8AudioInfoFrameByte |= 0x70;
            break;
        case 15 :
            u8AudioInfoFrameByte |= 0x78;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill Down mix inhibit flag bit7 */
    if (punAUDInfoFrame->u32DownmixInhibit)
    {
        u8AudioInfoFrameByte |= 0x80;
    }
    else
    {
        u8AudioInfoFrameByte &= ~0x80;
    }
    pu8AudioInfoFrame[4] = (HI_U8)(u8AudioInfoFrameByte&0xF8);

    return retval;
}

static HI_U32 hdmi_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 siRet = HI_SUCCESS;

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    COM_INFO("pstInfoFrame->enInfoFrameType %d \n",pstInfoFrame->enInfoFrameType);
    switch(pstInfoFrame->enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            /*The InfoFrame provided by HDMI is limited to 30 bytes plus a checksum byte.*/
            HI_U8 pu8AviInfoFrame[32];
            HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstAviInfoFrm = DRV_Get_AviInfoFrm(enHdmi);

            memset(pu8AviInfoFrame, 0, 32);
            siRet = hdmi_Create_AVI_Infoframe((HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(pstInfoFrame->unInforUnit.stAVIInfoFrame), pu8AviInfoFrame);
            memcpy(pstAviInfoFrm, &(pstInfoFrame->unInforUnit.stAVIInfoFrame), sizeof(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S));

            if (DRV_Get_IsMce2App())
            {
                COM_INFO("DRV_Get_IsMce2App:%d \n", DRV_Get_IsMce2App);
                //return HI_SUCCESS;
            }

            /* Set relative Register in HDMI IP */
            SI_DisableInfoFrame(AVI_TYPE);
            /* default AVI Infoframe: DefaultAVIInfoFrame[0xd] in eeprom.c*/
            SI_BlockWriteEEPROM( 13, EE_TXAVIINFO_ADDR, pu8AviInfoFrame);
            SI_SendAVIInfoFrame();
            SI_EnableInfoFrame(AVI_TYPE);
            siRet = HI_SUCCESS;
            break;
        }
        case HI_INFOFRAME_TYPE_SPD:
            break;
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            HI_U8 pu8AudioInfoFrame[32];
            HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAudInfoFrm = DRV_Get_AudInfoFrm(enHdmi);

            memset(pu8AudioInfoFrame, 0, 32);
            hdmi_Create_Audio_Infoframe((HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(pstInfoFrame->unInforUnit.stAUDInfoFrame), pu8AudioInfoFrame);
            memcpy(pstAudInfoFrm, &(pstInfoFrame->unInforUnit.stAUDInfoFrame), sizeof(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S));

            //SI_SetHdmiAudio(HI_FALSE);
            SI_DisableInfoFrame(AUD_TYPE);
            SI_Set_AudioInfoFramePacket (pu8AudioInfoFrame, 0,  0);
            SI_EnableInfoFrame(AUD_TYPE);
            //SI_SetHdmiAudio(HI_TRUE);

            break;
        }
        case HI_INFOFRAME_TYPE_MPEG:
            break;
        case HI_INFOFRAME_TYPE_VENDORSPEC:
            break;
        default:
            break;
    }
    return siRet;
}

static HI_U32 hdmi_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 siRet = HI_SUCCESS;

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    memset(pstInfoFrame, 0, sizeof(HI_UNF_HDMI_INFOFRAME_S));
    switch(enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstAviInfoFrm = DRV_Get_AviInfoFrm(enHdmi);

            pstInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AVI;
            memcpy(&(pstInfoFrame->unInforUnit.stAVIInfoFrame), pstAviInfoFrm, sizeof(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S));
            break;
        }
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAudInfoFrm = DRV_Get_AudInfoFrm(enHdmi);

            pstInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
            memcpy(&(pstInfoFrame->unInforUnit.stAUDInfoFrame), pstAudInfoFrm, sizeof(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S));
            break;
        }
        default:
            siRet = HI_FAILURE;
            break;
    }

    return siRet;
}

HI_U32 DRV_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_U32 Ret;
    COM_INFO("Enter DRV_HDMI_SetInfoFrame\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);


    if(pstInfoFrame->unInforUnit.stAVIInfoFrame.enOutputType == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
    {
        COM_INFO("%s.%d : SetInfoFrame YCBCR422 return \n",__FUNCTION__,__LINE__);
        return HI_ERR_HDMI_INVALID_PARA;
    }

    Ret = hdmi_SetInfoFrame(enHdmi, pstInfoFrame);
    SI_timer_count();
    COM_INFO("Leave DRV_HDMI_SetInfoFrame\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_U32 Ret;
    COM_INFO("Enter DRV_HDMI_GetInfoFrame\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    Ret = hdmi_GetInfoFrame(enHdmi, enInfoFrameType, pstInfoFrame);

    COM_INFO("Leave DRV_HDMI_GetInfoFrame\n");
    return HI_SUCCESS;
}
#if defined (HDCP_SUPPORT)
static HI_U32 HDCP_FailCount = 0;
#endif

#ifdef DEBUG_NOTIFY_COUNT
HI_U32 hpd_changeCount = 0;
#endif
HI_U32 DRV_HDMI_ReadEvent(HI_UNF_HDMI_ID_E enHdmi,HI_U32 procID)
{
    HI_S32 s32Ret=-1;
    HI_U32 index, event = 0;

    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(enHdmi);
    HI_U32 u32EventNo = pEventList[procID].CurEventNo;

    if(procID >= MAX_PROCESS_NUM)
    {
        COM_ERR("Invalid procID in ReadEvent\n");
        return HI_UNF_HDMI_EVENT_BUTT;
    }

    //We deal with HDMI Event only after HDMI opened!
    if(DRV_Get_IsChnOpened(enHdmi))
    {
        //DEBUG_PRINTK("msecs_to_jiffies(100):%d\n", msecs_to_jiffies(100));

        //s32Ret = wait_event_interruptible_timeout(g_astHDMIWait, g_HDMIWaitFlag, (HI_U32)msecs_to_jiffies(100));
        s32Ret = wait_event_interruptible_timeout(g_astHDMIWait, g_HDMIWaitFlag[procID], (HI_U32)msecs_to_jiffies(10));
        if (s32Ret <= 0)
        {
            return 0;
        }

        if(g_Event_Count[procID] > MIX_EVENT_COUNT)
        {
            g_Event_Count[procID]--;
        }
        else
        {
            g_HDMIWaitFlag[procID] = HI_FALSE;
        }

        COM_INFO("Ha we get a event!!!!!!\n");
        SI_timer_count();

#ifdef DEBUG_EVENTLIST
        COM_WARN("read start\n");
        COM_WARN("\n procID %d CurEventNo %d \n",procID,pEventList[procID].CurEventNo);
        for(index = 0;index < PROC_EVENT_NUM;index++)
        {
            COM_WARN("____eventlist %d: 0x%x_____\n",index, pEventList[procID].Event[index]);
        }
#endif

        HDMI_EVENT_LOCK();
        event = 0;
        for(index = 0; index < PROC_EVENT_NUM; index ++)
        {

            if ((pEventList[procID].Event[u32EventNo] >= HI_UNF_HDMI_EVENT_HOTPLUG)
                && (pEventList[procID].Event[u32EventNo] <=HI_UNF_HDMI_EVENT_RSEN_DISCONNECT))
            {
                event = pEventList[procID].Event[u32EventNo];
                pEventList[procID].Event[u32EventNo] = 0;
                if(event == HI_UNF_HDMI_EVENT_HOTPLUG)
                {
                    if (HI_TRUE == SI_Is_HPDKernelCallback_DetectHPD())
                    {
                    #ifdef DEBUG_NOTIFY_COUNT
                        hpd_changeCount++;
                        HPD_ERR("Warnning:Detect Hotplug Event,But hotplug signal is low! %d times \n",hpd_changeCount);
                    #endif
                        event = 0;
                    }
                    break;
                }
#if defined (HDCP_SUPPORT)
                else if(event == HI_UNF_HDMI_EVENT_HDCP_FAIL)
                {
                    HDCP_FailCount ++;
                    if (HDCP_FailCount >= 50)
                    {
                        HDCP_FailCount = 0;
                        HDCP_ERR("HDCP Authentication Fail times:50!\n");
                    }
                    break;
                }
#endif
                else
                {
                    COM_WARN("event:%d\n",event);
                    break;
                }
            }
            u32EventNo = (u32EventNo + 1) % PROC_EVENT_NUM;
#ifdef DEBUG_EVENTLIST
            COM_WARN("u32EventNo : %d \n",u32EventNo);
#endif
        }
        HDMI_EVENT_UNLOCK();
    }
    COM_INFO("line:%d,event:%d\n",__LINE__,event);

#ifdef DEBUG_EVENTLIST
    COM_WARN("read over\n");
    COM_WARN("\n procID %d CurEventNo %d \n",procID,pEventList[procID].CurEventNo);
    for(index = 0;index < PROC_EVENT_NUM;index++)
    {
        COM_WARN("____eventlist %d: 0x%x_____\n",index, pEventList[procID].Event[index]);
    }
#endif

    return event;
}

extern void hdmi_MCE_ProcHotPlug(HI_HANDLE hHdmi);

static void hdmi_ProcEvent(HI_UNF_HDMI_EVENT_TYPE_E event,HI_U32 procID)
{
    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(HI_UNF_HDMI_ID_0);
    COM_INFO("line:%d,event:%d,g_UserCallbackFlag:%d\n",__LINE__,event,g_UserCallbackFlag);


    /*
    //for avoid Silicon image 40Pll phy oe problem
      when hotplug occured,we need stop open oe in any time.
      but in boot,we can not detect hotplug changed,so display blank in some tv
      we need close & open oe first time in these TV
    */
    if(HI_TRUE == DRV_Get_IsChnStart(HI_UNF_HDMI_ID_0))
    {   
        Check1stOE(HI_UNF_HDMI_ID_0);
    }
    SI_CheckClockStable();
    
    if(procID >= MAX_PROCESS_NUM)
    {
        COM_ERR("Invalid procID:%d in hdmi_ProcEvent\n",procID);
        return;
    }

    if(g_UserCallbackFlag == HDMI_CALLBACK_USER) //app
    {
        HI_U32 u32CurEvent = pEventList[procID].CurEventNo;
#ifdef DEBUG_EVENTLIST
        int i;
#endif
        HDMI_EVENT_LOCK();
        g_Event_Count[procID]++;


        pEventList[procID].Event[u32CurEvent] = event;
        pEventList[procID].CurEventNo = (u32CurEvent + 1) % PROC_EVENT_NUM;

#ifdef DEBUG_EVENTLIST
        HI_PRINT("\n procID %d CurEventNo %d \n",procID,pEventList[procID].CurEventNo);
        for(i = 0;i < PROC_EVENT_NUM;i++)
        {
            HI_PRINT("____eventlist %d: 0x%x_____\n",i, pEventList[procID].Event[i]);
        }
#endif

        HDMI_EVENT_UNLOCK();
        g_HDMIWaitFlag[procID] = HI_TRUE;
        wake_up(&g_astHDMIWait);
        COM_INFO("callback finish wake up event g_HDMIWaitFlag:0x%x\n", g_HDMIWaitFlag[procID]);
    }
    else if(g_UserCallbackFlag == HDMI_CALLBACK_KERNEL)//mce
    {
        switch ( event )
        {
            case HI_UNF_HDMI_EVENT_HOTPLUG:
                //when unf_init after load ko too fast, maybe two hotplug will be trigered in the same time.
                HDMI_EVENT_LOCK();
                hdmi_MCE_ProcHotPlug(HI_UNF_HDMI_ID_0);
                HDMI_EVENT_UNLOCK();
                break;
#if defined (HDCP_SUPPORT)
            case HI_UNF_HDMI_EVENT_HDCP_SUCCESS:
                HI_PRINT("\n MCE HDMI event: HDCP_SUCCESS!\n");
                break;
#endif
            default:
                break;
        }
    }

    return;
}

#ifdef DEBUG_NOTIFY_COUNT
HI_U32 NotifyCount = 0;
#endif
void DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_TYPE_E event)
{
    HI_U32 u32ProcIndex = 0;
    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(HI_UNF_HDMI_ID_0);

#ifdef DEBUG_NOTIFY_COUNT
    if(event != HI_UNF_HDMI_EVENT_HDCP_USERSETTING)
    {
        NotifyCount++;
        HI_PRINT("\n **** Notify %d times : event 0x%x **** \n",NotifyCount,event);
    }
#endif

    COM_INFO("HDMI EVENT TYPE:0x%x\n", event);
    if (DRV_Get_IsChnOpened(HI_UNF_HDMI_ID_0))
    {
        if ((event == HI_UNF_HDMI_EVENT_HOTPLUG))
        {
#if defined (CEC_SUPPORT)
            HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();
#endif

            SI_EnableInterrupts();
#if defined (CEC_SUPPORT)
            SI_CEC_SetUp();
            pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount = 0;
#endif
            SI_HPD_SetHPDUserCallbackCount();

            //hdmi_AdjustAVIInfoFrame(HI_UNF_HDMI_ID_0);
            hdmi_SetAndroidState(STATE_HOTPLUGIN);
        }
#if defined (HDCP_SUPPORT)
        else if (event == HI_UNF_HDMI_EVENT_HDCP_USERSETTING)
        {
            //special doing!!
            //DRV_HDMI_Start(HI_UNF_HDMI_ID_0);
            return;
        }
#endif
        else if (event == HI_UNF_HDMI_EVENT_NO_PLUG)
        {
            if (g_HDMIUserInitNum)
            {
#if defined (CEC_SUPPORT)
                HDMI_CHN_ATTR_S *pstChnAttr = DRV_Get_ChnAttr();
#endif
                //0x72:0x08 powerdown is only needed in isr && cec
                /* Close HDMI Output */
                //SI_PowerDownHdmiTx();
                SI_SetHdmiVideo(HI_FALSE);
                SI_SetHdmiAudio(HI_FALSE);
                SI_DisableHdmiDevice();
                DRV_Set_ChnStart(HI_UNF_HDMI_ID_0,HI_FALSE);
#if defined (HDCP_SUPPORT)
                /*Set HDCP Off */
                SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif
#if defined (CEC_SUPPORT)
                //Close CEC
                SI_CEC_Close();
                DRV_Set_CECStart(HI_UNF_HDMI_ID_0,HI_FALSE);
                pstChnAttr[HI_UNF_HDMI_ID_0].u8CECCheckCount = 0;
                memset(&(pstChnAttr[HI_UNF_HDMI_ID_0].stCECStatus), 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));
#endif
            }
            else if(g_HDMIKernelInitNum)
            {
                //if oe not matching hotplug,some err will occured in phy

                //0x72:0x08 powerdown is only needed in isr && cec
                /* Close HDMI Output */
                //SI_PowerDownHdmiTx();
                SI_SetHdmiVideo(HI_FALSE);
                SI_SetHdmiAudio(HI_FALSE);
                SI_DisableHdmiDevice();
            }

            hdmi_SetAndroidState(STATE_HOTPLUGOUT);
        }
        else if (event == HI_UNF_HDMI_EVENT_EDID_FAIL)
        {

        }
#if defined (HDCP_SUPPORT)
        else if (event == HI_UNF_HDMI_EVENT_HDCP_FAIL)
        {

        }
        else if (event == HI_UNF_HDMI_EVENT_HDCP_SUCCESS)
        {
            SI_timer_stop();
        }
#endif
        else if (event == HI_UNF_HDMI_EVENT_RSEN_CONNECT)
        {
            COM_INFO("CONNECT Event:0x%x\n", event);
        }
        else if (event == HI_UNF_HDMI_EVENT_RSEN_DISCONNECT)
        {
            COM_INFO("DISCONNECT Event:0x%x\n", event);
        }
        else
        {
            COM_INFO("Unknow Event:0x%x\n", event);
        }
        COM_INFO("line:%d,event:%d\n",__LINE__,event);
        for(u32ProcIndex = 0; u32ProcIndex < MAX_PROCESS_NUM; u32ProcIndex++)
        {
            if(HI_TRUE == pEventList[u32ProcIndex].bUsed)
            {
                COM_INFO("proc id %d bUsed %d\n",u32ProcIndex,pEventList[u32ProcIndex].bUsed);
                hdmi_ProcEvent(event, u32ProcIndex);
            }
        }

        SI_timer_count();
    }
}

HI_U32 DRV_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);

    COM_INFO("Enter DRV_HDMI_Start\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    SI_timer_count();

    if(DRV_Get_IsMce2App() || DRV_Get_IsOpenedInBoot())
    {
        DRV_Set_Mce2App(HI_FALSE);
        DRV_Set_OpenedInBoot(HI_FALSE);
        SI_EnableHdmiDevice();
        SI_SendCP_Packet(HI_FALSE);//maybe avmute on while start,so set avmute off first
        DRV_Set_ChnStart(enHdmi,HI_TRUE);
        COM_INFO("DRV_HDMI_Start IsMce2App = %d, IsOpenedInBoot = %d\n", DRV_Get_IsMce2App(), DRV_Get_IsOpenedInBoot());
    }
    else
    {
        /* Enable HDMI Ouptut */
        if (HI_TRUE == pstAppAttr->bEnableHdmi)
        {
            if(!SI_TX_IsHDMImode())
            {
                COM_INFO("-->start: SI_Start_HDMITX\n");
                SI_Start_HDMITX();
                //SI_TX_SetHDMIMode(ON);    //for hdmi
            }
        }
#if defined (DVI_SUPPORT)
        else
        {
            if(SI_TX_IsHDMImode())
            {
                HI_PRINT("-->start: SI_Init_DVITX\n");
                SI_Init_DVITX();
                //SI_TX_SetHDMIMode(OFF);    //for dvi
            }
        }
#endif

        SI_timer_count();
        COM_INFO("Before TX_SYS_CTRL1_ADDR:0x%x\n", ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR));

        /* Now we wake up HDMI Output */
        SI_WakeUpHDMITX();

        /*leo in order to sync Audio  on hotplug*/
        SI_SetHdmiAudio(pstAppAttr->bEnableAudio);
        SI_SetHdmiVideo(pstAppAttr->bEnableVideo);

        SI_EnableHdmiDevice();
        SI_SendCP_Packet(HI_FALSE);
        COM_INFO("After TX_SYS_CTRL1_ADDR:0x%x\n", ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR));

        DRV_Set_ChnStart(enHdmi,HI_TRUE);

        SI_timer_count();
#if defined (HDCP_SUPPORT)
        if (HI_TRUE == pstAppAttr->bHDCPEnable)
        {
            if (HI_TRUE == SI_Is_HPDKernelCallback_DetectHPD())
            {
                /* HPD again before we try to open Auth 1*/
                HDCP_ERR("HPD Callback detect new HPD\n");
            }
            else if(HI_TRUE == SI_Is_HPDUserCallback_DetectHPD())
            {
                /* HPD again before we try to open Auth 2*/
                HDCP_ERR("HPD Callback with Auth detect new HPD\n");
            }
            else
            {
                HDCP_INFO("try to open HDCP Auth\n");
                SI_WriteByteEEPROM(EE_TX_HDCP, 0xFF);
            }
        }
        else
        {
            SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
        }
        SI_timer_count();

        if (HI_TRUE != pstAppAttr->bHDCPEnable)
        {
            SI_timer_stop();
        }
#endif
    }
    COM_INFO("Leave DRV_HDMI_Start\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi)
{
    COM_INFO("Enter DRV_HDMI_Stop\n");
    HDMI_CHECK_ID(enHdmi);

    //only in (init num == 1) case ,we can close output hdmi
    COM_INFO("Enter DRV_HDMI_Stop\n");
    if(DRV_HDMI_GetUserInitNum(enHdmi) != 1)
    {
        COM_INFO("DRV_HDMI_GetInitNum != 1 return \n");
        return HI_SUCCESS;
    }

    if (!DRV_Get_IsChnStart(enHdmi))
    {
        return HI_SUCCESS;
    }

    SI_SendCP_Packet(ON);
    msleep(40);
    SI_SetHdmiVideo(HI_FALSE);
    SI_SetHdmiAudio(HI_FALSE);
    SI_DisableHdmiDevice();
    //disable oe not need 10ms
    //msleep(10);
    SI_PowerDownHdmiTx();

    DRV_Set_ChnStart(enHdmi,HI_FALSE);
#if defined (HDCP_SUPPORT)
    /*Set HDCP Off */
    SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif

    COM_INFO("Leave DRV_HDMI_Stop\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
    HDMI_AUDIO_ATTR_S   *pstAudAttr = DRV_Get_AudioAttr(enHdmi);
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);

    HI_U8                SiDeepColor;
    HI_U32               RetError = HI_SUCCESS;

    COM_INFO("Enter DRV_HDMI_SetDeepColor\n");
    pstAppAttr->enDeepColorMode = enDeepColor;

    if (HI_UNF_HDMI_DEEP_COLOR_24BIT == enDeepColor)
    {
        SiDeepColor = (HI_U8)SiI_DeepColor_Off;
    }
    else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == enDeepColor)
    {
        SiDeepColor = (HI_U8)SiI_DeepColor_30bit;
    }
    else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == enDeepColor)
    {
        SiDeepColor = (HI_U8)SiI_DeepColor_36bit;
    }
    else
    {
        SiDeepColor = (HI_U8)SiI_DeepColor_Off;
    }
    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_TRUE);

    SI_SetDeepColor(SiDeepColor);

    /* DeepColor Atribute */
    if ( ((HI_UNF_HDMI_DEEP_COLOR_30BIT == pstAppAttr->enDeepColorMode) || (HI_UNF_HDMI_DEEP_COLOR_36BIT == pstAppAttr->enDeepColorMode))
        && ((HI_DRV_DISP_FMT_1080P_60 == pstVidAttr->enVideoFmt) || (HI_DRV_DISP_FMT_1080P_50 == pstVidAttr->enVideoFmt))
       )
    {
        SI_TX_PHY_HighBandwidth(HI_TRUE);
    }
    else
    {
        SI_TX_PHY_HighBandwidth(HI_FALSE);
    }

    RetError = DRV_HDMI_SetVOAttr(HI_UNF_HDMI_ID_0, pstVidAttr,HI_TRUE);
    RetError |= DRV_HDMI_SetAOAttr(HI_UNF_HDMI_ID_0, pstAudAttr,HI_TRUE);

    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_FALSE);
    COM_INFO("Leave DRV_HDMI_SetDeepColor\n");

    //DeepColor is setting by HDMI PHY, but it will effect
    //HDMI CONTROLLER Video frequency, it need to do software reset, before reset.
    if ((SiDeepColor == SiI_DeepColor_Off) || (SiDeepColor == SiI_DeepColor_24bit))
    {
        SI_SW_ResetHDMITX();   //force to reset the whole CTRL+PHY!
    }

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_U32 Ret;
    HI_U8 u8Data;
    HI_U8 InfCtrl2;
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);

    InfCtrl2 = ReadByteHDMITXP1(INF_CTRL2);
    if (HI_TRUE == bEnable)
    {
        COM_INFO("enable xvYCC\n");
        /* enable Gamut Metadata InfoFrame transmission */
        InfCtrl2 |= 0xc0;
        COM_INFO("INF_CTRL2(0x%02x) is 0x%02x\n", INF_CTRL2, InfCtrl2);
        WriteByteHDMITXP1(INF_CTRL2, InfCtrl2);  //Packet Buffer Control #2 Register 0x7A   0x3F

        /* Gamut boundary descriptions (GBD) and other gamut-related metadata
           are carried using the Gamut Metadata Packet.*/
        if ( pstVidAttr->enVideoFmt <= HI_DRV_DISP_FMT_720P_50)
        {
            Ret = SI_SendGamutMeta_Packet(HI_TRUE);
        }
        else
        {
            Ret = SI_SendGamutMeta_Packet(HI_FALSE);
        }

        u8Data = ReadByteHDMITXP0(RGB2XVYCC_CT);  //hdmi/hmitx.h  TX_SLV0:0x72
        u8Data = 0x07;
        COM_INFO("RGB2XVYCC_CT (0x%02x) data is 0x%02x\n", RGB2XVYCC_CT, u8Data);
        WriteByteHDMITXP0(RGB2XVYCC_CT, u8Data);
    }
    else
    {
        COM_INFO("Disable xvYCC\n");
        /* disable Gamut Metadata InfoFrame transmission */
        InfCtrl2 &= ~0xc0;
        COM_INFO("INF_CTRL2(0x%02x) is 0x%02x\n", INF_CTRL2, InfCtrl2);
        WriteByteHDMITXP1(INF_CTRL2, InfCtrl2);  //Packet Buffer Control #2 Register 0x7A   0x3F

        u8Data = ReadByteHDMITXP0(RGB2XVYCC_CT);  //hdmi/hmitx.h  TX_SLV0:0x72
        u8Data = 0x00;
        COM_INFO("RGB2XVYCC_CT (0x%02x) data is 0x%02x\n", RGB2XVYCC_CT, u8Data);
        WriteByteHDMITXP0(RGB2XVYCC_CT, u8Data);
    }
    COM_INFO("end of HI_UNF_HDMI_SetxvYCCMode\n");

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    COM_INFO("Enter DRV_HDMI_SetAVMute, bAvMute:%d\n", bAvMute);
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (bAvMute)
    {
        HI_U32 Delay = 0;
        //Disable HDMI Output!!
        SI_SendCP_Packet(HI_TRUE);
        GetmuteDelay(enHdmi, &Delay);
        DelayMS(Delay); //HDMI compatibility requirement for Suddenly Close.
        //DelayMS(200);
    }
    else
    {
        SI_SendCP_Packet(HI_FALSE);
    }

    COM_INFO("Leave DRV_HDMI_SetAVMute\n");
    return HI_SUCCESS;
}

// The Procedures for SetFormat
// Set AV mute
// HW Reset controller
// Hw Reset Phy
// delay 50us
// Release Hw reset Phy
// cfg Phy
// Release HW reset for controller
// Delay 5ms for pll stable
HI_U32 DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo)
{
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
    HDMI_AUDIO_ATTR_S   *pstAudAttr = DRV_Get_AudioAttr(enHdmi);
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);

    HI_UNF_EDID_BASE_INFO_S    *pSinkCap = DRV_Get_SinkCap(HI_UNF_HDMI_ID_0);
    HI_UNF_HDMI_VIDEO_MODE_E          enVidOutMode;
    HI_DRV_DISP_FMT_E                 enEncodingFormat = enFmt;
    HI_U32 delayTime = 0;

#if defined (DEBUG_TIMER)
    HI_S32 start = 0, end = 0;

    SI_timer_start();
    start = SI_timer_count();
#endif
    enEncodingFormat = DispFormatAdjust(enEncodingFormat);

    COM_INFO("Enter DRV_HDMI_SetFormat enEncodingFormat:%d enStereo %d \n", enEncodingFormat,enStereo);


    SI_timer_count();

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    //if the format change when mce to app,clear some flags,fix DTS2014082207534,add by h00180450
    if (pstVidAttr->enVideoFmt != enEncodingFormat)
    {
        DRV_Set_Mce2App(HI_FALSE);
        DRV_Set_OpenedInBoot(HI_FALSE);
        DRV_Set_ForceUpdateFlag(enHdmi,HI_TRUE);
    }

    if ((enEncodingFormat == HI_DRV_DISP_FMT_PAL)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_B)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_B1)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_D)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_D1)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_G)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_H)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_K)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_I)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_M)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_N)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_Nc)||
        (enEncodingFormat == HI_DRV_DISP_FMT_PAL_60)||
        (enEncodingFormat == HI_DRV_DISP_FMT_1440x576i_50)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_SIN)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_COS)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_L)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_B)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_G)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_D)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_K)||
        (enEncodingFormat == HI_DRV_DISP_FMT_SECAM_H))
    {
        enEncodingFormat = HI_DRV_DISP_FMT_PAL;
    }
    else if ((enEncodingFormat == HI_DRV_DISP_FMT_NTSC)||
        (enEncodingFormat == HI_DRV_DISP_FMT_NTSC_J)||
        (enEncodingFormat == HI_DRV_DISP_FMT_1440x480i_60)||
        (enEncodingFormat == HI_DRV_DISP_FMT_NTSC_443))
    {
        enEncodingFormat = HI_DRV_DISP_FMT_NTSC;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_1080P_24_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_1080P_24;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_720P_60_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_720P_60;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_720P_50_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_720P_50;
    }

    pstVidAttr->b3DEnable = HI_TRUE;
    if(DISP_STEREO_FPK == enStereo)
    {
        pstVidAttr->u83DParam = HI_UNF_EDID_3D_FRAME_PACKETING;
    }
    else if (DISP_STEREO_SBS_HALF == enStereo)
    {
        pstVidAttr->u83DParam = HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF;
    }
    else if (DISP_STEREO_TAB == enStereo)
    {
        pstVidAttr->u83DParam = HI_UNF_EDID_3D_TOP_AND_BOTTOM;
    }
    else
    {
        pstVidAttr->b3DEnable = HI_FALSE;
        pstVidAttr->u83DParam = HI_UNF_EDID_3D_BUTT;
    }

    COM_INFO("FMT:%d,3DFlag:%d, 3dParm:%d\n",enEncodingFormat,pstVidAttr->b3DEnable,pstVidAttr->u83DParam);

    if(DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
    {
        if (HI_TRUE == pSinkCap->bSupportFormat[DRV_HDMI_Disp2UnfFmt(enEncodingFormat)])
        {
            COM_INFO("From EDID, sink can receive this format!!!\n");
        }
        else
        {
            COM_ERR("Warring:From EDID, Sink CAN NOT receive this format*******\n");
            //return HI_FAILURE;
        }
    }
    else
    {
        COM_INFO("Invalid capability, we can't know support fmt \n");
    }

#if 0 /*-- change mode in app --*/
    if(HI_TRUE == pSinkCap->bSupportHdmi)
    {
        bHDMIMode = HI_TRUE;
    }
    else
    {
        bHDMIMode = HI_FALSE;
    }
#endif


    enVidOutMode = pstAppAttr->enVidOutMode;

    /*this part should be removed*/
    if(enVidOutMode >= HI_UNF_HDMI_VIDEO_MODE_BUTT)
    {
        COM_ERR("no set color space!\n");
        if(DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
        {
            if (HI_TRUE == pSinkCap->stColorSpace.bYCbCr444)
            {
                enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
            }
            else
            {
                enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
            }
        }
        else
        {
            COM_INFO("Sink capability invalid, change to rgb!\n");
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }
    }

   if ((DRV_Get_IsLCDFmt(enEncodingFormat))
     || (pstAppAttr->bEnableHdmi == HI_FALSE))
    {
          COM_INFO("DVI timing \n");
          enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
    else if(enVidOutMode >= HI_UNF_HDMI_VIDEO_MODE_BUTT)
    {
        if((DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
        && (HI_TRUE == pSinkCap->stColorSpace.bYCbCr444))
        {
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }
        else
        {
            COM_INFO("EDID error! change to rgb!\n");
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }
    }

    COM_INFO("enVidOutMode:%d\n", enVidOutMode);
    pstAppAttr->enVidOutMode = enVidOutMode;
    //pstAppAttr->bEnableHdmi = bHDMIMode;        //HDMI or DVI
    pstVidAttr->enVideoFmt = enEncodingFormat;

    if(HI_TRUE != SI_HPD_Status())
    {
        //when undetect hotplug,don't need to set fmt,we process it in hotplug callbackfunc
        COM_INFO("hot plug not detected!");

        //setting format in unplug means, video did not configed.so we need config video,in next hotplug callbackfunc
        DRV_Set_ForceUpdateFlag(enHdmi,HI_TRUE);

        return HI_SUCCESS;
    }
    else  //power up,and need 1ms for clk stable
    {
        DRV_Set_ForceUpdateFlag(enHdmi,HI_FALSE);
        SI_TX_PHY_PowerDown(HI_FALSE);
        DelayMS(1);
    }
    
    DRV_HDMI_SetVOAttr(enHdmi, pstVidAttr,HI_TRUE);
    DRV_HDMI_SetAOAttr(enHdmi, pstAudAttr,HI_TRUE);

    COM_INFO("attr.bEnableHdmi:0x%x\n", pstAppAttr->bEnableHdmi);

    hdmi_AdjustAVIInfoFrame(enHdmi);
    hdmi_AdjustVSDBInfoFrame(enHdmi);
    hdmi_AdjustAUDInfoFrame(enHdmi);

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(pstVidAttr->b3DEnable)
    {
        SI_VSDB_Setting(VSDB_MODE_3D,pstVidAttr->u83DParam);
    }
    else if(DRV_Get_Is4KFmt(pstVidAttr->enVideoFmt))
    {
        SI_VSDB_Setting(VSDB_MODE_4K,pstVidAttr->enVideoFmt);
    }
    else
    {
        SI_VSDB_Setting(VSDB_MODE_NONE,0xff);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

    /*between oe disable and enable need to have at least 100ms due to some RX Sink devices
      not reacting too fast. If the system turns off TMDS output enable and turns on TMDS output
      enable too fast, some RX Sink devices may not have those changes. */
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    SetFormatDelay(enHdmi,&delayTime);
    if(delayTime > 100)
    {
        DelayMS(delayTime);
    }
    else
    {
        DelayMS(100);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    DelayMS(100);
    SI_EnableHdmiDevice();

//    hdmi_AdjustAVIInfoFrame(enHdmi);
//    hdmi_AdjustAUDInfoFrame(enHdmi);

    GetFormatDelay(enHdmi,&delayTime);
    DelayMS(delayTime);


#if defined (HDCP_SUPPORT)
    if(pstAppAttr->bHDCPEnable == HI_TRUE)
        SI_WriteByteEEPROM(EE_TX_HDCP, 0xFF);
    else
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);

    msleep(60);
#endif

    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_FALSE);

#if defined (DEBUG_TIMER)
    end = SI_timer_count();
    SI_timer_stop();

    HI_PRINT("SetFormat Cost %dms \n",end - start);
#endif

    COM_INFO("Leave DRV_HDMI_SetFormat\n");

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_PreFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enEncodingFormat)
{
#if defined (DEBUG_TIMER)
    HI_S32 start = 0, end = 0;

    SI_timer_start();
    start = SI_timer_count();
#endif

    COM_INFO("DRV_HDMI_PreFormat \n");



#if defined (HDCP_SUPPORT)
    //Disable HDCP
    if(SI_ReadByteEEPROM(EE_TX_HDCP))
    {
        /* First to close HDCP Key */
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
        SI_SetEncryption(OFF);
        //need so long?
        DelayMS(200);
    }
#endif


    DRV_HDMI_SetAVMute(enHdmi, HI_TRUE);

    SI_DisableHdmiDevice();
    DelayMS(1);

    SI_TX_PHY_PowerDown(HI_TRUE);

    COM_INFO("DRV_HDMI_PreFormat leave\n");

#if defined (DEBUG_TIMER)
    end = SI_timer_count();
    SI_timer_stop();

    HI_PRINT("PreFormat Cost %dms \n",end - start);
#endif

    return 0;
}

HI_U32 DRV_HDMI_Force_GetEDID(HDMI_EDID_S *pEDID)
{
    HI_U32 u32EdidLength;
    HI_U8 u32EdidBlock[512];
    HI_U32  ret;

    EDID_INFO("Enter DRV_HDMI_Extern_GetEDID\n");
    HDMI_CHECK_ID(pEDID->enHdmi);
    HDMI_CheckChnOpen(pEDID->enHdmi);

    ret = SI_Force_GetEDID(u32EdidBlock, &u32EdidLength);
    if(ret != HI_SUCCESS)                                //Rowe
    {
        EDID_ERR("Force Get EDID fail!:%x\n",ret);
        return ret;
    }

    if((u32EdidLength > 0) && (u32EdidLength <= 512))
    {
        pEDID->u8EdidValid   = HI_TRUE;
        pEDID->u32Edidlength = u32EdidLength;
        memcpy(pEDID->u8Edid, u32EdidBlock, pEDID->u32Edidlength);
    }
    else
    {
        pEDID->u8EdidValid   = HI_FALSE;
        pEDID->u32Edidlength = 0;
        memset(pEDID->u8Edid, 0, 512);
    }

    EDID_INFO("Leave DRV_HDMI_Extern_GetEDID\n");
    return 0;
}

HI_U32 DRV_HDMI_Read_EDID(HDMI_EDID_S *pEDID)
{
    HI_U32 u32EdidLength;
    HI_U8  u32EdidBlock[512];
    HI_U32 u32Ret = HI_FAILURE;
    
    EDID_INFO("Enter DRV_HDMI_Read_EDID\n");  
    
    u32Ret = SI_Proc_ReadEDIDBlock(u32EdidBlock, &u32EdidLength);
    if (u32Ret == HI_SUCCESS)
    {          
        pEDID->u8EdidValid   = HI_TRUE;            
        pEDID->u32Edidlength = u32EdidLength;
        memcpy(pEDID->u8Edid, u32EdidBlock, u32EdidLength);        
        EDID_INFO("DRV_HDMI_Read_EDID success!!! len = %d\n", pEDID->u32Edidlength);
    }        
    else
    {        
        EDID_ERR("DRV_HDMI_Read_EDID error:length = %d\n", u32EdidLength);
        goto EDID_FAIL;            
    }
    
    EDID_INFO("Leave DRV_HDMI_Read_EDID\n");
    
    return u32Ret;
    
EDID_FAIL:
    
    pEDID->u8EdidValid   = HI_FALSE;
    pEDID->u32Edidlength = 0;
    memset(pEDID->u8Edid, 0, 512);      
    EDID_INFO("Leave DRV_HDMI_Read_EDID\n");
    
    return HI_FAILURE;
}


HI_U32 DRV_HDMI_GetPlayStatus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus)
{
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    *pu32Stutus = DRV_Get_IsChnStart(enHdmi);
    return 0;
}

HI_U32 DRV_HDMI_LoadKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_U32 u32Ret = HI_SUCCESS;
#if defined (HDCP_SUPPORT)
    HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S stFlashEncrytedHdcpKey ;
    CIPHER_EXPORT_FUNC_S *g_stCIPHERExportFunctionLists = HI_NULL;

    u32Ret = HI_DRV_MODULE_GetFunction(HI_ID_CIPHER, (HI_VOID**)&g_stCIPHERExportFunctionLists);
    if((NULL == g_stCIPHERExportFunctionLists) || (u32Ret != HI_SUCCESS))
    {
        HDCP_ERR("Get cipher functions failed!\n");
        return HI_FAILURE;
    }

    HDMI_CHECK_ID(enHdmi);

    if((HI_NULL != g_stCIPHERExportFunctionLists)
     &&(HI_NULL != g_stCIPHERExportFunctionLists->pfnCipherLoadHdcpKey))
    {
        /*load hdcp key */
        memcpy(stFlashEncrytedHdcpKey.u8Key, pstLoadKey->pu8InputEncryptedKey, pstLoadKey->u32KeyLength);
        stFlashEncrytedHdcpKey.u32KeyLen = pstLoadKey->u32KeyLength;
        stFlashEncrytedHdcpKey.enHDCPKeyType = HI_DRV_HDCPKEY_TX0;
        stFlashEncrytedHdcpKey.enHDCPVersion = HI_DRV_HDCP_VERIOSN_1x;
        u32Ret = (g_stCIPHERExportFunctionLists->pfnCipherLoadHdcpKey)(&stFlashEncrytedHdcpKey);
        if( HI_SUCCESS != u32Ret)
        {
            HDCP_ERR("Load hdcp key error!\n");
        }
        HDCP_INFO("Load hdcp key successful!\n");
    }
#endif
    return u32Ret;
}

extern HI_U32 g_u32ProcHandle ;
HI_S32 DRV_HDMI_GetProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32ProcID)
{
    HI_S32 u32Ret = HI_FAILURE;
    HI_U32 u32ProcIndex = 0;
    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(HI_UNF_HDMI_ID_0);
    COM_INFO("DRV_HDMI_GetProcID\n");

    //待测试
    //没配置通道ID
    //HDMI_CHECK_ID(enHdmi);

    if(pu32ProcID == NULL)
    {
        COM_WARN("Null ProcID pointer! \n");
        return u32Ret;
    }

    for(u32ProcIndex = 0; u32ProcIndex < MAX_PROCESS_NUM; u32ProcIndex++)
    {
        if(HI_TRUE != pEventList[u32ProcIndex].bUsed)
        {
            *pu32ProcID = u32ProcIndex;
            pEventList[u32ProcIndex].bUsed = HI_TRUE;
            pEventList[u32ProcIndex].u32ProcHandle =  g_u32ProcHandle;
            u32Ret = HI_SUCCESS;
            break;
        }
    }

    COM_INFO("Getted ProcID %d\n",*pu32ProcID);
    return u32Ret;
}

HI_S32 DRV_HDMI_ReleaseProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 u32ProcID)
{
    HI_S32 u32Ret = HI_FAILURE;
    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(HI_UNF_HDMI_ID_0);

    COM_INFO("DRV_HDMI_ReleaseProcID %d\n",u32ProcID);

    //待测试
    //HDMI_CHECK_ID(enHdmi);

    if(u32ProcID >= MAX_PROCESS_NUM)
    {
        return u32Ret;
    }

    memset(&pEventList[u32ProcID], 0, sizeof(HDMI_PROC_EVENT_S));

    u32Ret = HI_SUCCESS;

    return u32Ret;
}



HI_S32 DRV_HDMI_AudioChange(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr)
{
    HI_S32                            Ret = HI_SUCCESS;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe = DRV_Get_AudInfoFrm(enHdmi);
    HDMI_AUDIO_ATTR_S                *pstAudAttr = DRV_Get_AudioAttr(enHdmi);
    //    HDMI_APP_ATTR_S                  *pstAppAttr = DRV_Get_AppAttr(enHdmi);

    COM_INFO("DRV_HDMI_AudioChange : enSoundIntf:%d,enSampleRate:%d,u32Channels:%d \n",pstHDMIAOAttr->enSoundIntf,pstHDMIAOAttr->enSampleRate,pstHDMIAOAttr->u32Channels);
    COM_INFO("enBitDepth:%d,u8DownSampleParm:%d,u8I2SCtlVbit:%d\n",pstHDMIAOAttr->enBitDepth,pstHDMIAOAttr->u8DownSampleParm,pstHDMIAOAttr->u8I2SCtlVbit);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    //we force set bit depth to 16bit
    pstHDMIAOAttr->enBitDepth = HI_UNF_BIT_DEPTH_16;

    if(pstHDMIAOAttr->enSoundIntf == pstAudAttr->enSoundIntf &&
       pstHDMIAOAttr->enSampleRate == pstAudAttr->enSampleRate &&
       pstHDMIAOAttr->u32Channels == pstAUDInfoframe->u32ChannelCount &&
       pstHDMIAOAttr->enBitDepth == pstAudAttr->enBitDepth &&
       pstHDMIAOAttr->enAudioCode == pstAUDInfoframe->enCodingType)
    {
        COM_INFO("%s.%d : The same Audio Attr \n",__FUNCTION__,__LINE__);
        //SI_SetHdmiAudio(pstAppAttr->bEnableAudio);
        return HI_SUCCESS;
    }

    if (HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf
       || HDMI_AUDIO_INTERFACE_SPDIF== pstHDMIAOAttr->enSoundIntf
       || HDMI_AUDIO_INTERFACE_HBR== pstHDMIAOAttr->enSoundIntf)
    {
        pstAudAttr->enSoundIntf = pstHDMIAOAttr->enSoundIntf;
    }
    else
    {
        COM_ERR("Error input Audio interface(%d)\n",pstHDMIAOAttr->enSoundIntf);
        //SI_SetHdmiAudio(pstAppAttr->bEnableAudio);
        return HI_FAILURE;
    }

    COM_INFO("\n\n\n audio Sample Rate : %d\n\n\n",pstHDMIAOAttr->enSampleRate);
    switch (pstHDMIAOAttr->enSampleRate)
    {
        case HI_UNF_SAMPLE_RATE_32K:
        case HI_UNF_SAMPLE_RATE_44K:
        case HI_UNF_SAMPLE_RATE_48K:
        case HI_UNF_SAMPLE_RATE_88K:
        case HI_UNF_SAMPLE_RATE_96K:
        case HI_UNF_SAMPLE_RATE_176K:
        case HI_UNF_SAMPLE_RATE_192K:
            pstAudAttr->enSampleRate = pstHDMIAOAttr->enSampleRate;
            break;
        default:
            COM_ERR("Error input Audio Frequency(%d)\n",pstHDMIAOAttr->enSampleRate);
            //SI_SetHdmiAudio(pstAppAttr->bEnableAudio);
            return HI_FAILURE;
    }

#if 0 /*--old solution--*/
    /* Set Audio infoframe */
    /* New function to set Audio Infoframe */
    /* HDMI requires the CT, SS and SF fields to be set to 0 ("Refer to Stream Header")
       as these items are carried in the audio stream.*/
    //InfoFrame.enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
    pstAUDInfoframe->u32ChannelCount      = pstHDMIAOAttr->u32Channels;

    //Refer to Stream Header
    pstAUDInfoframe->enCodingType         = HI_UNF_HDMI_DEFAULT_SETTING;
#endif

    if(pstHDMIAOAttr->enSoundIntf == HDMI_AUDIO_INTERFACE_SPDIF
        || pstHDMIAOAttr->enSoundIntf == HDMI_AUDIO_INTERFACE_HBR)
    {
        COM_INFO("Audio channel refer from stream \n");
        pstAUDInfoframe->u32ChannelCount      = 0;// refer from stream head
    }
    else
    {
        COM_INFO("Audio channel %d \n",pstHDMIAOAttr->u32Channels);
        pstAUDInfoframe->u32ChannelCount      = pstHDMIAOAttr->u32Channels; //PCM maybe 2 or 8;
    }

    if(pstHDMIAOAttr->enAudioCode == HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3
       || pstHDMIAOAttr->enAudioCode == HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS
       || pstHDMIAOAttr->enAudioCode == HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP
       || pstHDMIAOAttr->enAudioCode == HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD)
    {
        COM_INFO("Aduio Code : %d \n",pstHDMIAOAttr->enAudioCode);
        pstAUDInfoframe->enCodingType     = pstHDMIAOAttr->enAudioCode;
    }
    else
    {
        COM_INFO("Aduio Code : %d \n",pstHDMIAOAttr->enAudioCode);
        pstAUDInfoframe->enCodingType     = HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED;
    }


    //Refer to Stream Header
    pstAUDInfoframe->u32SampleSize        = HI_UNF_HDMI_DEFAULT_SETTING;
    //Refer to Stream Header
    pstAUDInfoframe->u32SamplingFrequency = HI_UNF_HDMI_DEFAULT_SETTING;

    switch(pstHDMIAOAttr->u32Channels)     //HDMI channel map
    {
        case 3:
            pstAUDInfoframe->u32ChannelAlloc = 0x01;
            break;
        case 6:
            pstAUDInfoframe->u32ChannelAlloc = 0x0b;
            break;
        case 8:
            pstAUDInfoframe->u32ChannelAlloc = 0x13;
            break;
        default:
            pstAUDInfoframe->u32ChannelAlloc = 0x00;
            break;
    }
    pstAUDInfoframe->u32LevelShift        = 0;
    pstAUDInfoframe->u32DownmixInhibit    = HI_FALSE;
    COM_INFO("***HI_UNF_HDMI_SetInfoFrame for AUDIO Infoframe\n");

    if((pstHDMIAOAttr->u32Channels > 2)
    &&(HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf))
    {
        pstAudAttr->bIsMultiChannel = HI_TRUE;
    }
    else
    {
        pstAudAttr->bIsMultiChannel = HI_FALSE;
    }


    pstAudAttr->u32Channels = pstHDMIAOAttr->u32Channels;
    pstAudAttr->enBitDepth = HI_UNF_BIT_DEPTH_16;

    Ret = DRV_HDMI_SetAOAttr(enHdmi, pstAudAttr, HI_TRUE);

    hdmi_AdjustAUDInfoFrame(enHdmi);


    if(Ret != HI_SUCCESS)
    {
        COM_ERR("Set HDMI Audio Attr failed\n");
    }

    return Ret;
}

HI_S32 DRV_HDMI_GetAOAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr)
{
    HI_S32  Ret = HI_SUCCESS;
    //HDMI_ATTR_S stHDMIAttr;
    HDMI_AUDIO_ATTR_S  *pstAudAttr = DRV_Get_AudioAttr(enHdmi);
    //HDMI_AUDIO_ATTR_S stAudAttr;
    //HI_UNF_HDMI_INFOFRAME_S     InfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe = DRV_Get_AudInfoFrm(enHdmi);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    memcpy(pstHDMIAOAttr,pstAudAttr,sizeof(HDMI_AUDIO_ATTR_S));
    pstHDMIAOAttr->u32Channels = pstAUDInfoframe->u32ChannelCount;

    return Ret;
}

//需要根据edid补充完善
static HI_U32 hdmi_AdjustAVIInfoFrame(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_U32 Ret = HI_SUCCESS;
    //HDMI_ATTR_S stHDMIAttr;
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
    HI_UNF_HDMI_COLORSPACE_E enColorimetry;
    HI_UNF_HDMI_ASPECT_RATIO_E enAspectRate;
    HI_U32 u32PixelRepetition;
    HI_U32 enRGBQuantization;
    HI_DRV_DISP_FMT_E enEncodingFormat;
    HI_UNF_HDMI_INFOFRAME_S           stInfoFrame;
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S  *pstVIDInfoframe;
    //HI_UNF_EDID_BASE_INFO_S *pSinkCap = DRV_Get_SinkCap(HI_UNF_HDMI_ID_0);
    HDMI_APP_ATTR_S *pstAppAttr = DRV_Get_AppAttr(enHdmi);


    COM_INFO("adjust AVI InfoFrame start \n");

    if(SI_IsHDMIResetting() || HI_FALSE == pstAppAttr->bEnableHdmi)
    {
        COM_INFO("no need cfg AVIInfoFrame : return\n");
        return HI_SUCCESS;
    }

    /* New function to set AVI Infoframe */
    hdmi_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AVI, &stInfoFrame);

    pstVIDInfoframe = (HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(stInfoFrame.unInforUnit.stAVIInfoFrame);

    enEncodingFormat = pstVidAttr->enVideoFmt;
    COM_INFO("change DISP Timing to enEncodingFormat:%d\n", enEncodingFormat);

    enColorimetry      = HDMI_COLORIMETRY_ITU709;
    enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
    u32PixelRepetition = HI_FALSE;
    enRGBQuantization  = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;


   if(HI_DRV_DISP_FMT_1080P_60 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080P_60);
    }
    else if(HI_DRV_DISP_FMT_1080P_50 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080P_50);
    }
    else if(HI_DRV_DISP_FMT_1080P_30 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080P_30000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080P_30);
    }
    else if(HI_DRV_DISP_FMT_1080P_25 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080P_25000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080P_25);
    }
    else if(HI_DRV_DISP_FMT_1080P_24 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080P_24000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080P_24);
    }
    else if(HI_DRV_DISP_FMT_1080i_60 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080i_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080i_60);
    }
    else if(HI_DRV_DISP_FMT_1080i_50 == enEncodingFormat)
    {
        COM_INFO("Set 1920X1080i_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_1080i_50);
    }
    else if(HI_DRV_DISP_FMT_720P_60 == enEncodingFormat)
    {
        COM_INFO("Set 1280X720P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_720P_60);
    }
    else if(HI_DRV_DISP_FMT_720P_50 == enEncodingFormat)
    {
        COM_INFO("Set 1280X720P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_720P_50);
    }
    else if(HI_DRV_DISP_FMT_576P_50 == enEncodingFormat)
    {
        COM_INFO("Set 720X576P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_576P_50);
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if(HI_DRV_DISP_FMT_480P_60 == enEncodingFormat)
    {
        COM_INFO("Set 720X480P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_480P_60);
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if((HI_DRV_DISP_FMT_PAL == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_H == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_I == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_M == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_N == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_Nc == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_60 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_SIN == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_COS == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_L == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_H == enEncodingFormat)||
        (HI_DRV_DISP_FMT_1440x576i_50  == enEncodingFormat))
    {
        COM_INFO("Set PAL 576I_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_PAL);
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if((HI_DRV_DISP_FMT_NTSC == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_J == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_443 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_1440x480i_60  == enEncodingFormat))
    {
        COM_INFO("Set NTS 480I_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_NTSC);
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if(HI_DRV_DISP_FMT_861D_640X480_60 == enEncodingFormat)
    {
        COM_INFO("Set 640X480P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_861D_640X480_60);
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;
        //enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
#if defined (DVI_SUPPORT)
    else if (DRV_Get_IsLCDFmt(enEncodingFormat))
    {
        COM_INFO("DVI timing mode enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        //DEBUG_PRINTK("Force to DVI Mode\n");
        //bHDMIMode = HI_FALSE;

        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(HI_DRV_DISP_FMT_861D_640X480_60);
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;

        //enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
#endif
    else if(DRV_Get_Is4KFmt(enEncodingFormat))
    {
        enColorimetry      = HDMI_COLORIMETRY_ITU709;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
        u32PixelRepetition = HI_FALSE;
        enRGBQuantization  = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;
        pstVIDInfoframe->enTimingMode = DRV_HDMI_Disp2UnfFmt(enEncodingFormat);
    }

    //xvYcc??
    //pstAVIInfoFrame->enTimingMode = stHDMIAttr.stAttr.enVideoFmt;
    pstVIDInfoframe->enOutputType = pstAppAttr->enVidOutMode;
    pstVIDInfoframe->bActive_Infor_Present = HI_TRUE;
    pstVIDInfoframe->enBarInfo = HDMI_BAR_INFO_NOT_VALID;
    pstVIDInfoframe->enColorimetry = enColorimetry;
    pstVIDInfoframe->enAspectRatio = enAspectRate;
    pstVIDInfoframe->enActiveAspectRatio = enAspectRate;
    pstVIDInfoframe->enPictureScaling = HDMI_PICTURE_NON_UNIFORM_SCALING;
    pstVIDInfoframe->enRGBQuantization = enRGBQuantization;
    pstVIDInfoframe->bIsITContent = HI_FALSE;
    pstVIDInfoframe->u32PixelRepetition = u32PixelRepetition;
    pstVIDInfoframe->enYCCQuantization = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(DRV_Get_IsValidSinkCap(HI_UNF_HDMI_ID_0))
    {
        HDMI_PRIVATE_EDID_S *pPriSinkCap = DRV_Get_PriSinkCap(HI_UNF_HDMI_ID_0);
        if(pPriSinkCap->bUnderScanDev)
        {
            pstVIDInfoframe->enScanInfo =  HDMI_SCAN_INFO_UNDERSCANNED;
        }
        else
        {
            pstVIDInfoframe->enScanInfo =  HDMI_SCAN_INFO_OVERSCANNED;
        }
    }
    else
    {
        pstVIDInfoframe->enScanInfo = HDMI_SCAN_INFO_NO_DATA;
    }
#else
    pstVIDInfoframe->enScanInfo = HDMI_SCAN_INFO_NO_DATA;
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/


    pstVIDInfoframe->u32LineNEndofTopBar = 0;
    pstVIDInfoframe->u32LineNStartofBotBar = 0;
    pstVIDInfoframe->u32PixelNEndofLeftBar = 0;
    pstVIDInfoframe->u32PixelNStartofRightBar = 0;

    Ret |= DRV_HDMI_SetInfoFrame(enHdmi, &stInfoFrame);

    COM_INFO("adjust AVI InfoFrame over \n");

    return HI_SUCCESS;
}

static HI_U32 hdmi_AdjustVSDBInfoFrame(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
    if(pstVidAttr->b3DEnable)
    {
        SI_VSDB_Setting(VSDB_MODE_3D,pstVidAttr->u83DParam);
    }
    else if(DRV_Get_Is4KFmt(pstVidAttr->enVideoFmt))
    {
        SI_VSDB_Setting(VSDB_MODE_4K,pstVidAttr->enVideoFmt);
    }
    else
    {
        SI_VSDB_Setting(VSDB_MODE_NONE,0xff);
    }

    return HI_SUCCESS;
}

static HI_U32 hdmi_AdjustAUDInfoFrame(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_UNF_HDMI_INFOFRAME_S           stInfoFrame;
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);
    //HI_UNF_EDID_BASE_INFO_S *pSinkCap = DRV_Get_SinkCap(HI_UNF_HDMI_ID_0);


    if(SI_IsHDMIResetting() || HI_FALSE == pstAppAttr->bEnableHdmi || HI_FALSE == pstAppAttr->bEnableAudio)
    {
        COM_INFO("no need cfg AudInfoFrame : return\n");
        return HI_SUCCESS;
    }

    hdmi_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AUDIO, &stInfoFrame);

    DRV_HDMI_SetInfoFrame(enHdmi, &stInfoFrame);

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_E enHdmi)
{
    return (g_HDMIKernelInitNum + g_HDMIUserInitNum);
}

HI_S32 DRV_HDMI_GetUserInitNum(HI_UNF_HDMI_ID_E enHdmi)
{
    return (g_HDMIUserInitNum);
}

HI_S32 DRV_HDMI_GetProcNum(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 u32ProcCount = 0;
    HI_S32 index;
    HDMI_PROC_EVENT_S *pEventList = DRV_Get_EventList(HI_UNF_HDMI_ID_0);

    for(index = 0; index < MAX_PROCESS_NUM; index++)
    {
        if(HI_TRUE == pEventList[index].bUsed)
        {
            u32ProcCount++;
        }
    }

    return u32ProcCount;
}

HI_S32 DRV_HDMI_SetAPPAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_APP_ATTR_S *pstHDMIAppAttr,HI_BOOL UpdateFlag)
{
    HI_BOOL AppForceUpdate = HI_FALSE;
    HDMI_APP_ATTR_S *pstAppAttr = DRV_Get_AppAttr(enHdmi);
    COM_INFO("Enter DRV_HDMI_SetAPPAttr\n");

    if(pstHDMIAppAttr->enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
    {
        COM_ERR("Not support HI_UNF_HDMI_VIDEO_MODE_YCBCR422 return \n");
        return HI_ERR_HDMI_INVALID_PARA;    \
    }

    if (UpdateFlag == HI_TRUE)
    {
        AppForceUpdate = HI_TRUE;
    }
    else if (HI_SUCCESS != hdmi_AppAttrChanged(enHdmi,pstAppAttr,pstHDMIAppAttr,&AppForceUpdate))
    {
        COM_INFO("Set APPAttr With No video update \n");
        return HI_SUCCESS;
    }
    else
    {
        COM_INFO("app attr has change \n");
    }

    COM_INFO("HDMI: %d , Aud: %d , Vid: %d \n",pstHDMIAppAttr->bEnableHdmi,pstHDMIAppAttr->bEnableAudio,pstHDMIAppAttr->bEnableVideo);

    if(pstHDMIAppAttr->bEnableHdmi == HI_FALSE)
    {
        pstHDMIAppAttr->bEnableAudio = HI_FALSE;
        pstHDMIAppAttr->bEnableAudInfoFrame = HI_FALSE;
        pstHDMIAppAttr->bEnableAviInfoFrame = HI_FALSE;
        pstHDMIAppAttr->enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }

    memcpy(pstAppAttr, pstHDMIAppAttr, sizeof(HDMI_APP_ATTR_S));


#ifdef HDCP_SUPPORT
    //msleep(10);
    /* Set HDMI HDCP Enable flag */
    HDCP_INFO("bHDCPEnable:0x%x, bDebugFlag:0x%x\n", pstAppAttr->bHDCPEnable, pstAppAttr->bDebugFlag);
    if(pstAppAttr->bHDCPEnable == HI_TRUE)
    {
        SI_WriteByteEEPROM(EE_TX_HDCP, 0xFF);
    }
    else
    {
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
    }

    msleep(60);
#endif

    /* Set HDMI Video Enable flag */
    COM_INFO("SET hdmi video status Flag:%d\n", pstAppAttr->bEnableVideo);

    COM_INFO("AppForceUpdate %d \n",AppForceUpdate);
    if(AppForceUpdate == HI_TRUE)
    {
        DRV_Set_ForceUpdateFlag(enHdmi,HI_TRUE);
    }
    else
    {
        DRV_Set_PartUpdateFlag(enHdmi,HI_TRUE);
    }
    return HI_SUCCESS;
}

#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
HI_VOID DRV_HDMI_SetPhyAudioClk(HDMI_AUDIOINTERFACE_E  enSoundIntf,\
                                                HI_DRV_DISP_FMT_E eDispFmt,\
                                                HI_UNF_HDMI_DEEP_COLOR_E eDeepColor)
{
    HI_U32 u32Freq = 0;
    HI_HDMI_PHY_AUDIO_CLK_E     eAudioClk = 0;

    if (HDMI_AUDIO_INTERFACE_SPDIF == enSoundIntf)
    {
        SI_HW_ResetCtrl(2);
    }
    else
    {
        SI_HW_ResetCtrl(0);
        return ;
    }

    switch(eDispFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60:         /**<1080p 60 Hz*/
            u32Freq = 148500;
            break;

        case HI_DRV_DISP_FMT_1080P_50:          /**<1080p 50 Hz*/
            u32Freq = 148500;
            break;

        case HI_DRV_DISP_FMT_1080P_30:          /**<1080p 30 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_1080P_25:          /**<1080p 25 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_1080P_24:          /**<1080p 24 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_1080i_60:          /**<1080i 60 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_1080i_50:          /**<1080i 60 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_720P_60:           /**<720p 60 Hz*/
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_720P_50:           /**<720p 50 Hz */
            u32Freq = 74250;
            break;

        case HI_DRV_DISP_FMT_576P_50:           /**<576p 50 Hz*/
            u32Freq = 27000;
            break;

        case HI_DRV_DISP_FMT_480P_60:           /**<480p 60 Hz*/
            u32Freq = 27000;
            break;

        case HI_DRV_DISP_FMT_PAL:               /* B D G H I PAL */
        case HI_DRV_DISP_FMT_PAL_B:             /* B PAL， Australia */
        case HI_DRV_DISP_FMT_PAL_B1:            /* B1 PAL, Hungary */
        case HI_DRV_DISP_FMT_PAL_D:             /* D PAL, China */
        case HI_DRV_DISP_FMT_PAL_D1:            /* D1 PAL， Poland */
        case HI_DRV_DISP_FMT_PAL_G:             /* G PAL， Europe */
        case HI_DRV_DISP_FMT_PAL_H:             /* H PAL， Europe */
        case HI_DRV_DISP_FMT_PAL_K:             /* K PAL， Europe */
        case HI_DRV_DISP_FMT_PAL_I:             /* I PAL，U.K. */
        case HI_DRV_DISP_FMT_PAL_N:             /* N PAL, Jamaica/Uruguay */
        case HI_DRV_DISP_FMT_PAL_Nc:            /* Nc PAL, Argentina:21 */
        case HI_DRV_DISP_FMT_PAL_M:             /* M PAL, 525 lines */
        case HI_DRV_DISP_FMT_PAL_60:            /* 60 PAL */
        case HI_DRV_DISP_FMT_NTSC:              /* (M)NTSC       */
        case HI_DRV_DISP_FMT_NTSC_J:           /* NTSC-J        */
        case HI_DRV_DISP_FMT_NTSC_443:           /* (M)PAL        */
        case HI_DRV_DISP_FMT_SECAM_SIN:       /**< SECAM_SIN*/
        case HI_DRV_DISP_FMT_SECAM_COS:       /**< SECAM_COS*/
        case HI_DRV_DISP_FMT_SECAM_L:         /**< France*/
        case HI_DRV_DISP_FMT_SECAM_B:         /**< Middle East*/
        case HI_DRV_DISP_FMT_SECAM_G:         /**< Middle East*/
        case HI_DRV_DISP_FMT_SECAM_D:         /**< Eastern Europe*/
        case HI_DRV_DISP_FMT_SECAM_K:         /**< Eastern Europe*/
        case HI_DRV_DISP_FMT_SECAM_H:         /**< Line SECAM:34*/
            u32Freq = 27000;
            break;

        case HI_DRV_DISP_FMT_1440x576i_50:
            u32Freq = 27000;
            break;

        case HI_DRV_DISP_FMT_1440x480i_60:  /*sequnce:36*/
            u32Freq = 27000;
            break;

        case HI_DRV_DISP_FMT_1080P_24_FP:
            u32Freq = 74250*2;
            break;

        case HI_DRV_DISP_FMT_720P_60_FP:
            u32Freq = 74250*2;
            break;

        case HI_DRV_DISP_FMT_720P_50_FP:
            u32Freq = 74250*2;
            break;

        case HI_DRV_DISP_FMT_861D_640X480_60:
            u32Freq = 25175;
            break;

        case HI_DRV_DISP_FMT_VESA_800X600_60:
            u32Freq = 40000;
            break;

        case HI_DRV_DISP_FMT_VESA_1024X768_60:
            u32Freq = 65000;
            break;

        case HI_DRV_DISP_FMT_VESA_1280X720_60:
            break;

        //case HI_DRV_DISP_FMT_VESA_1280X800_60:
        case HI_DRV_DISP_FMT_VESA_1280X1024_60:
            u32Freq = 108000;
            break;

        //case HI_DRV_DISP_FMT_VESA_1360X768_60:          //Rowe
        case HI_DRV_DISP_FMT_VESA_1366X768_60:
            u32Freq = 85750;
            break;
        /*
        case HI_DRV_DISP_FMT_VESA_1400X1050_60:         //Rowe
        case HI_DRV_DISP_FMT_VESA_1440X900_60:
        case HI_DRV_DISP_FMT_VESA_1440X900_60_RB:
        case HI_DRV_DISP_FMT_VESA_1600X900_60_RB:
        */
        case HI_DRV_DISP_FMT_VESA_1600X1200_60:
            u32Freq = 162000;
            break;
        /*
        case HI_DRV_DISP_FMT_VESA_1680X1050_60:        //Rowe
        case HI_DRV_DISP_FMT_VESA_1680X1050_60_RB:        //Rowe
        case HI_DRV_DISP_FMT_VESA_1920X1080_60:
        case HI_DRV_DISP_FMT_VESA_1920X1200_60:
        case HI_DRV_DISP_FMT_VESA_1920X1440_60:
        case HI_DRV_DISP_FMT_VESA_2048X1152_60:
        case HI_DRV_DISP_FMT_VESA_2560X1440_60_RB:
        case HI_DRV_DISP_FMT_VESA_2560X1600_60_RB:  //sequence:60
        */
        case HI_DRV_DISP_FMT_3840X2160_24: /*reserve for extention*/
            u32Freq = 297000;
            break;

        case HI_DRV_DISP_FMT_3840X2160_25:
            u32Freq = 297000;
            break;

        case HI_DRV_DISP_FMT_3840X2160_30:
            u32Freq = 297000;
            break;

        case HI_DRV_DISP_FMT_4096X2160_24:
            u32Freq = 297000;
            break;

        //ADD
      //  case HI_DRV_DISP_FMT_CUSTOM:
            //break;
        default:
            u32Freq = 50000;
            COM_ERR("%s[line%d]:un-know format %d!\n",__func__,__LINE__,eDispFmt);
            break;
    }

    switch(eDeepColor)
    {
        case  HI_UNF_HDMI_DEEP_COLOR_24BIT :
        case  HI_UNF_HDMI_DEEP_COLOR_OFF:
            u32Freq *= 1000;
            break;
        case  HI_UNF_HDMI_DEEP_COLOR_30BIT :
            u32Freq *= 1250;
            break;

        case  HI_UNF_HDMI_DEEP_COLOR_36BIT :
            u32Freq *= 1500;
            break;

        default:
            COM_ERR("%s[line%d]:un-know deepcolor %d!\n",__func__,__LINE__,eDeepColor);
            return ;
            //break;
    }

    if (u32Freq > CLK_100MHZ)
    {
        eAudioClk = HI_HDMI_PHY_AUDIO_CLK_1;
    }
    else if (((u32Freq*5)/3) > CLK_100MHZ)
    {
        eAudioClk = HI_HDMI_PHY_AUDIO_CLK_5P3;
    }
    else if (((u32Freq*5)/2) > CLK_100MHZ)
    {
        eAudioClk = HI_HDMI_PHY_AUDIO_CLK_5P2;
    }
    else
    {
        eAudioClk = HI_HDMI_PHY_AUDIO_CLK_5;
    }

    SI_TX_PHY_WriteRegister(PHY_AUD_ADDR, eAudioClk);
}
#endif

HI_S32 DRV_HDMI_SetAOAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_AUDIO_ATTR_S *pstHDMIAOAttr,HI_BOOL UpdateFlag)
{
    HI_U32 Ret = HI_SUCCESS;

    HDMI_AUDIO_ATTR_S *pstAudAttr = DRV_Get_AudioAttr(enHdmi);
    HDMI_APP_ATTR_S *pstAppAttr = DRV_Get_AppAttr(enHdmi);
	
#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
    HDMI_VIDEO_ATTR_S   *pstHDMIVOAttr = DRV_Get_VideoAttr(enHdmi);
#endif

    HI_BOOL AUpdate = HI_TRUE;

    COM_INFO("Enter DRV_HDMI_SetAOAttr\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstHDMIAOAttr);

    if (UpdateFlag != HI_TRUE)
    {
        if (HI_SUCCESS != hdmi_AudioAttrChanged(enHdmi, pstHDMIAOAttr, pstAudAttr, &AUpdate))
        {
            COM_INFO("vo attr no change return \n");
            //SI_SetHdmiAudio(pstAppAttr->bEnableAudio);
            return HI_SUCCESS;
        }
    }

    COM_INFO("AUpdate %d  SetAOAttr\n",AUpdate);

    /* Set HDMI InfoFrame Enable flag */
    SI_WriteByteEEPROM(EE_AUDINFO_ENABLE, pstAppAttr->bEnableAudInfoFrame);

    memcpy(pstAudAttr, pstHDMIAOAttr, sizeof(HDMI_AUDIO_ATTR_S));

    /* Adjust Audio Path Param */
    if ((AUpdate == HI_TRUE) && (pstAppAttr->bEnableAudio == HI_TRUE))
    {
        HI_U8 u8AudioPath[4];
        HI_U32 audioSampleRate = 0;

        //SI_SetHdmiAudio(HI_FALSE);

        audioSampleRate = pstHDMIAOAttr->enSampleRate;

        if (pstHDMIAOAttr->u8DownSampleParm != 0)
        {
            /* downsample audio freqency */
            if( 1 == pstHDMIAOAttr->u8DownSampleParm)
            {
                WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x01);
            }
            else if( 2 == pstHDMIAOAttr->u8DownSampleParm)
            {
                WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x03);
            }
        }
        else
        {
            /* Remove Downsample flag */
            WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x00);
        }

        /* Use default setting before write Audio Path */
        SI_GetAudioPath(u8AudioPath);   /* hdmi/audio.c, default setting DefaultTXDVDAudio[4] in txvptbl.c */
        COM_INFO("default audio path: 0x%02x,0x%02x,0x%02x,0x%02x\n", u8AudioPath[0], u8AudioPath[1], u8AudioPath[2], u8AudioPath[3]);
        memset(u8AudioPath, 0, 4);

        /* abAudioPath[0] set Audio format & Bit 7 */
        if (HDMI_AUDIO_INTERFACE_SPDIF == pstHDMIAOAttr->enSoundIntf)
        {
            u8AudioPath[0] = 0x0;
        }
        else if (HDMI_AUDIO_INTERFACE_HBR == pstHDMIAOAttr->enSoundIntf)
        {
            COM_INFO("\nAudio input is HBR\n");
            u8AudioPath[0] = SiI_HBAudio;
        }
        else
        {
            u8AudioPath[0] = 0x1; //I2S format
        }
        /* Bit[7] of abAudioPath[0] */
        if (HI_TRUE == pstHDMIAOAttr->bIsMultiChannel)
        {
            u8AudioPath[0] |= 0x80;
        }
        else
        {
            u8AudioPath[0] &= ~0x80;
        }
        /* abAudioPath[1] set Sampling Freq Fs */
        /*
           CH_ST4          Fs Sampling Frequency
           3   2   1   0  <--- bit
           0   0   0   0   44.1 kHz
           1   0   0   0   88.2 kHz
           1   1   0   0   176.4 kHz
           0   0   1   0   48 kHz
           1   0   1   0   96 kHz
           1   1   1   0   192 kHz
           0   0   1   1   32 kHz
           0   0   0   1   not indicated
           */
        if(HI_UNF_SAMPLE_RATE_44K == audioSampleRate)
        {
            u8AudioPath[1] = 0x00;
        }
        else if (HI_UNF_SAMPLE_RATE_88K == audioSampleRate)
        {
            u8AudioPath[1] = 0x08;
        }
        else if (176400 == audioSampleRate)
        {
            u8AudioPath[1] = 0x0c;
        }
        else if (HI_UNF_SAMPLE_RATE_48K == audioSampleRate)
        {
            u8AudioPath[1] = 0x02;
        }
        else if (HI_UNF_SAMPLE_RATE_96K == audioSampleRate)
        {
            u8AudioPath[1] = 0x0a;
        }
        else if (HI_UNF_SAMPLE_RATE_192K == audioSampleRate)
        {
            u8AudioPath[1] = 0x0e;
        }
        else if (HI_UNF_SAMPLE_RATE_32K == audioSampleRate)
        {
            u8AudioPath[1] = 0x03;
        }
        else
        {
            u8AudioPath[1] = 0x00;
        }
        /* abAudioPath[2] set Sample length */
        /*
           IN_LENGTH 3:0 <--- sample length
           (0xff)0b1111 - 0b1110 = N/A
           (0x0d)0b1101 = 21 bit
           (0x0c)0b1100 = 17 bit
           (0x0b)0b1011 = 24 bit
           (0x0a)0b1010 = 20 bit
           (0x09)0b1001 = 23 bit
           (0x08)0b1000 = 19 bit
           (0x07)0b0111 - 0b0110 = N/A
           (0x05)0b0101 = 22 bit
           (0x04)0b0100 = 18 bit
           0b0011 = N/A
           (0x02)0b0010 = 16 bit
           0b0001 - 0b0000 = N/A
           */
        if (HI_UNF_BIT_DEPTH_16 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x02;
        }
        else if (HI_UNF_BIT_DEPTH_18 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x04;
        }
        else if (22 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x05;
        }
        else if (19 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x08;
        }
        else if (23 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x09;
        }
        else if (20 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0a;
        }
        else if (HI_UNF_BIT_DEPTH_24 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0b;
        }
        else if (17 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0c;
        }
        else if (21 == pstHDMIAOAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0d;
        }
        else
        {
            u8AudioPath[2] = 0x0f;
        }
        /* abAudioPath[3] I2S control bits (for 0x7A:0x1D) */
        if(HI_TRUE == pstHDMIAOAttr->u8I2SCtlVbit)
        {
            u8AudioPath[3] |= 0x10; /* 1 = Compressed */
        }
        else
        {
            u8AudioPath[3] &= (HI_U8)(~0x10); /* 0 = Uncompressed */
        }
        u8AudioPath[3] |= 0x40; /* SCK sample edge Must use "Sample clock is rising" */
        COM_INFO("set audio path: 0x%02x,0x%02x,0x%02x,0x%02x\n", u8AudioPath[0], u8AudioPath[1], u8AudioPath[2], u8AudioPath[3]);
        SI_SetAudioPath(u8AudioPath);  /* hdmi/audio.c */

#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
        DRV_HDMI_SetPhyAudioClk(pstAudAttr->enSoundIntf,
                                    pstHDMIVOAttr->enVideoFmt,
                                    pstAppAttr->enDeepColorMode);
#endif
    }

    /* Set HDMI Audio Enable flag */
    //SI_SetHdmiAudio(pstAppAttr->bEnableAudio);

    COM_INFO("Leave hdmi_SetAoAttr\n");

    return Ret;
}

HI_S32 DRV_HDMI_SetVOAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_VIDEO_ATTR_S *pstHDMIVOAttr,HI_BOOL UpdateFlag)
{
    HI_U32 Ret = HI_SUCCESS;
    HI_U8 ucData       = 0;
    HI_U8 bRxVideoMode = 0;
    HI_U8 bTxVideoMode = 0;
    HI_U8 bVideoMode;               /* Hdmi Video mode index define in vmtables.c */
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(enHdmi);
    HDMI_APP_ATTR_S     *pstAppAttr = DRV_Get_AppAttr(enHdmi);
    HI_UNF_ENC_FMT_E    Fmt = DRV_HDMI_Disp2UnfFmt(pstHDMIVOAttr->enVideoFmt);

    HI_BOOL VUpdate = HI_TRUE;

    COM_INFO("Enter DRV_HDMI_SetVOAttr \n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstHDMIVOAttr);

    if (pstAppAttr->bEnableVideo != HI_TRUE)
    {
        COM_ERR("bEnableVideo Must be set to HI_TRUE!\n");
        pstAppAttr->bEnableVideo = HI_TRUE;
    }

    if (UpdateFlag != HI_TRUE)
    {
        if (HI_SUCCESS != hdmi_VideoAttrChanged(enHdmi, pstHDMIVOAttr, pstVidAttr, &VUpdate))
        {
            COM_INFO("vo attr no change return \n");
            return HI_SUCCESS;
        }
    }

    COM_INFO("VUpdate %d \n",VUpdate);
    /* Set HDMI InfoFrame Enable flag */
    SI_WriteByteEEPROM(EE_AVIINFO_ENABLE, pstAppAttr->bEnableAviInfoFrame);
    SI_WriteByteEEPROM(EE_SPDINFO_ENABLE, pstAppAttr->bEnableSpdInfoFrame);

    memcpy(pstVidAttr, pstHDMIVOAttr, sizeof(HDMI_VIDEO_ATTR_S));

    ucData = 0x00;    // DE(Data Enable generator) disable

    SI_WriteByteEEPROM(EE_TX_DE_ENABLED_ADDR, ucData);

    /* Adjust Video Path Param */
    if ((VUpdate == HI_TRUE) && (pstAppAttr->bEnableVideo == HI_TRUE) && (!DRV_Get_IsMce2App()))
    {
        HI_U8 u8VideoPath[4];

        //  SI_SetHdmiVideo(HI_FALSE);

        /* Write VIDEO MODE INDEX */
        /* Transfer_VideoTimingFromat_to_VModeIndex() is get g_s32VmodeOfUNFFormat[] */
        bVideoMode = Transfer_VideoTimingFromat_to_VModeTablesIndex(DRV_HDMI_Disp2UnfFmt(pstHDMIVOAttr->enVideoFmt));
        if (0 == bVideoMode)
        {
            COM_INFO("The HDMI FMT (%d) is invalid.\n", pstHDMIVOAttr->enVideoFmt);
            SI_WriteByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR, 2); /* 720P */
        }
        else
        {
            COM_INFO("The HDMI_VMode of HI_UNF_ENC_FMT(%d) is %d.\n", pstHDMIVOAttr->enVideoFmt, bVideoMode);
            SI_WriteByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR, bVideoMode);
        }

        SI_GetVideoPath(u8VideoPath); /* hdmi/txvidp.c, default setting DefaultTXVPath[4] in txvptbl.c  */
        /* inMode[] is defined in hdmi/txvptbl.c */
        COM_INFO("default u8VideoPath :0x%02x,0x%02x,0x%02x,0x%02x\n", u8VideoPath[0], u8VideoPath[1], u8VideoPath[2], u8VideoPath[3]);

        if (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_861D_640X480_60)
        {
            DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_RGB444);
        }
#if defined (DVI_SUPPORT)
        else if (DRV_Get_IsLCDFmt(pstHDMIVOAttr->enVideoFmt))
        {
            COM_INFO("LCD Format, force to RGB444 into hdmi ip\n");
            DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_RGB444);
        }
#endif
        else if ((pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B1)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D1)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_G)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_H)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_K)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_I)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_M)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_N)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_Nc)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_60)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1440x576i_50)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_SIN)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_COS)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_L)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_B)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_G)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_D)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_K)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_H))
        {
            COM_INFO("SD TV Format, force to YCBCR444 into hdmi ip\n");
            DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_YCBCR444);
        }
        else if ((pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_J)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1440x480i_60)||
            (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_443))
        {
            COM_INFO("SD TV Format, force to YCBCR444 into hdmi ip\n");
            DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_YCBCR444);
        }
        else
        {
            COM_INFO("DTV Format, force to YCbCr444 into hdmi ip\n");
            DRV_Set_VIDMode(HI_UNF_HDMI_VIDEO_MODE_YCBCR444);
        }

        if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == DRV_Get_VIDMode())
        {
            bRxVideoMode = 0;  /* inRGB24[] */
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAppAttr->enVidOutMode)
            {
                COM_INFO("HDMI Input RGB444, Output RGB444\n");
                bTxVideoMode = 0;
            }
            else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == pstAppAttr->enVidOutMode)
            {
                /*--Rgb444 can convert to yuv444--*/
                COM_INFO("HDMI Input RGB444, Output YUV444\n");
                bTxVideoMode = 1;
            }
            else
            {
                HI_PRINT("Error output mode when input RGB444\n");
                pstAppAttr->enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
                bTxVideoMode = 0;
                Ret = HI_FAILURE;
            }
        }
        else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == DRV_Get_VIDMode())
        {
            HI_U32 reg;

            bRxVideoMode = 2; /* inYCbCr24[] */

            reg = ReadByteHDMITXP0(VID_ACEN_ADDR);//down sampler
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAppAttr->enVidOutMode)
            {
                COM_INFO("HDMI Input YCBCR444, Output RGB444\n");
                bTxVideoMode = 0;
                reg &= ~BIT_VID_ACEN_DWN_SAMPLE;
            }
            else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == pstAppAttr->enVidOutMode)
            {
                COM_INFO("HDMI Input YCBCR444, Output YCBCR444\n");
                bTxVideoMode = 1;
                reg &= ~BIT_VID_ACEN_DWN_SAMPLE;
            }
            else
            {
                HI_PRINT("Input YCBCR444, Output YCBCR422\n");
                bTxVideoMode = 1;
                reg |= BIT_VID_ACEN_DWN_SAMPLE;
                Ret = HI_FAILURE;
            }
            WriteByteHDMITXP0(VID_ACEN_ADDR, (HI_U8)reg);
        }
        else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR422 == DRV_Get_VIDMode())
        {
            bRxVideoMode = 3; /* inYC24[] */
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAppAttr->enVidOutMode)
            {
                COM_INFO("HDMI Input YCBCR422, Output RGB444\n");
                bTxVideoMode = 0;
            }
            else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == pstAppAttr->enVidOutMode)
            {
                COM_INFO("HDMI Input YCBCR422, Output YCBCR444\n");
                bTxVideoMode = 1;
            }
            else
            {
                COM_INFO("HDMI Input YCBCR422, Output YCBCR422\n");
                bTxVideoMode = 2;
            }
        }
        u8VideoPath[0] = bRxVideoMode;
        u8VideoPath[1] = bTxVideoMode;
        //u8VideoPath[2]; can use default value form getfunction.
        if (HI_UNF_HDMI_DEEP_COLOR_24BIT == pstAppAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 0;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == pstAppAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 1;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == pstAppAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 2;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_OFF >= pstAppAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 0xFF;
        }

        SI_BlockWriteEEPROM( 4, EE_TX_VIDEOPATH_ADDR , u8VideoPath);
        if(!DRV_Get_IsOpenedInBoot())
        {
            SI_UpdateTX_656(bVideoMode);
            SI_SetIClk( SI_ReadByteEEPROM(EE_TX_ICLK_ADDR) );


            COM_INFO("setting video path u8VideoPath :0x%02x,0x%02x,0x%02x,0x%02x\n", u8VideoPath[0], u8VideoPath[1], u8VideoPath[2], u8VideoPath[3]);
            bVideoMode = SI_ReadByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR);
            COM_INFO("bVideoMode:0x%x\n", bVideoMode);
            SI_SetVideoPath(bVideoMode, u8VideoPath);


            COM_INFO("out of siiSetVideoPath\n");

            /* Hsync/Vsync polarity:Video DE Control Register:VS_POL#, HS_POL# */
            if ((pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_576P_50)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_480P_60)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B1)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D1)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_G)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_H)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_K)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_I)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_M)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_N)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_Nc)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_60)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_SIN)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_COS)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_L)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_B)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_G)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_D)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_K)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_H)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_J)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_443)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1440x576i_50)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1440x480i_60)||
                (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_861D_640X480_60))
            {
                SI_TX_InvertSyncPol(HI_TRUE);

                // sd with bt 601 colorspace
                SI_TX_CSC709Select(HI_FALSE);
            }
            else
            {
                if(DRV_Get_Is4KFmt(pstHDMIVOAttr->enVideoFmt))
                {
                    SI_TX_PHY_4KRisingTime(HI_TRUE);
                }
                else
                {
                    SI_TX_PHY_4KRisingTime(HI_FALSE);
                }

                SI_TX_InvertSyncPol(HI_FALSE);

                // TV hd & LCD format with bt 709 colorspace
                SI_TX_CSC709Select(HI_TRUE);
            }
            
            SI_Adjust_SwingCtrl(Fmt, pstAppAttr->enDeepColorMode);

            // when TMDS Clk > 165Mhz.we need open source term
            if( (pstHDMIVOAttr->u83DParam == HI_UNF_EDID_3D_FRAME_PACKETING) && (pstHDMIVOAttr->b3DEnable == HI_TRUE)
                 && ( (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1080P_24) ||
                    (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_720P_60)  ||
                    (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_720P_50) )
                 && ((HI_UNF_HDMI_DEEP_COLOR_30BIT == pstAppAttr->enDeepColorMode)
                     || (HI_UNF_HDMI_DEEP_COLOR_36BIT == pstAppAttr->enDeepColorMode))
               )
            {
                // 3d deep color Attribute
                SI_TX_PHY_HighBandwidth(HI_TRUE);
            }
            else if ( ((HI_UNF_HDMI_DEEP_COLOR_30BIT == pstAppAttr->enDeepColorMode) || (HI_UNF_HDMI_DEEP_COLOR_36BIT == pstAppAttr->enDeepColorMode))
                && ((HI_DRV_DISP_FMT_1080P_60 == pstHDMIVOAttr->enVideoFmt)
                     || (HI_DRV_DISP_FMT_1080P_59_94 == pstHDMIVOAttr->enVideoFmt)
                     || (HI_DRV_DISP_FMT_1080P_50 == pstHDMIVOAttr->enVideoFmt)) )
            {
                // 1080P DeepColor Attribute
                SI_TX_PHY_HighBandwidth(HI_TRUE);
            }
            else if((pstHDMIVOAttr->enVideoFmt >= HI_DRV_DISP_FMT_VESA_1920X1200_60) &&
                (pstHDMIVOAttr->enVideoFmt <= HI_DRV_DISP_FMT_VESA_2560X1600_60_RB))
            {
                // vesa timing Atribute
                SI_TX_PHY_HighBandwidth(HI_TRUE);
            }
            else if(DRV_Get_Is4KFmt(pstHDMIVOAttr->enVideoFmt))
            {
                // 4k2k Atribute
                SI_TX_PHY_HighBandwidth(HI_TRUE);
            }
            else if ((pstHDMIVOAttr->enVideoFmt <= HI_DRV_DISP_FMT_720P_50)
                   || (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_720P_59_94)
                    || (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1080P_59_94)
                    || (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1080P_29_97)
                    || (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1080P_23_976)
                    || (pstHDMIVOAttr->enVideoFmt == HI_DRV_DISP_FMT_1080i_59_94))

            {
                // HD TV Fmt Atribute
                SI_TX_PHY_HighBandwidth(HI_TRUE);
            }
            else
            {
                SI_TX_PHY_HighBandwidth(HI_FALSE);
            }



        }
    }

    return Ret;
}

HI_S32 DRV_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus)
{
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pHdmiStatus);

    pHdmiStatus->bConnected = SI_HPD_Status();
    pHdmiStatus->bSinkPowerOn = SI_RSEN_Status();

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pHdmiDelay)
{
    HDMI_CHECK_ID(enHdmi);
    //HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pHdmiDelay);

    pHdmiDelay->bForceFmtDelay = IsForceFmtDelay();
    pHdmiDelay->bForceMuteDelay = IsForceMuteDelay();
    pHdmiDelay->u32FmtDelay = GetGlobalFmtDelay();
    pHdmiDelay->u32MuteDelay = GetGlobalsMuteDelay();
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pHdmiDelay)
{
    HDMI_CHECK_ID(enHdmi);
    //HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pHdmiDelay);

    SetForceDelayMode(pHdmiDelay->bForceFmtDelay,pHdmiDelay->bForceMuteDelay);
    SetGlobalFmtDelay(pHdmiDelay->u32FmtDelay);
    SetGlobalMuteDelay(pHdmiDelay->u32MuteDelay);

    return HI_SUCCESS;
}
HI_U8 g_avi_info[13] = {0};
HI_U8 g_audio_info[13] = {0};

HI_VOID DRV_O5_HDMI_PutBinInfoFrame(HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType,HI_VOID* infor_ptr)
{
    if(enInfoFrameType == HI_INFOFRAME_TYPE_AVI)
        memcpy(g_avi_info, infor_ptr,13);
    else if(enInfoFrameType == HI_INFOFRAME_TYPE_AUDIO)
        memcpy(g_audio_info, infor_ptr,5);
    return ;
}

HI_VOID DRV_O5_HDMI_GetBinInfoFrame(HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_VOID *infor_ptr)
{
    if (enInfoFrameType == HI_INFOFRAME_TYPE_AVI)
        memcpy(infor_ptr, g_avi_info,13);
    else if (enInfoFrameType == HI_INFOFRAME_TYPE_AUDIO)
        memcpy(infor_ptr, g_audio_info,5);
}

HI_DRV_DISP_FMT_E DispFormatAdjust(HI_DRV_DISP_FMT_E enEncFmt)
{
    HI_DRV_DISP_FMT_E retEncFmt = HI_DRV_DISP_FMT_BUTT;

    switch (enEncFmt)
    {
        case HI_DRV_DISP_FMT_3840X2160_23_976:
            retEncFmt = HI_DRV_DISP_FMT_3840X2160_24;
            break;

        case HI_DRV_DISP_FMT_3840X2160_29_97:
            retEncFmt = HI_DRV_DISP_FMT_3840X2160_30;
            break;

        case HI_DRV_DISP_FMT_720P_59_94:
            retEncFmt = HI_DRV_DISP_FMT_720P_60;
            break;

        case HI_DRV_DISP_FMT_1080P_59_94:
            retEncFmt = HI_DRV_DISP_FMT_1080P_60;
            break;

        case HI_DRV_DISP_FMT_1080P_29_97:
            retEncFmt = HI_DRV_DISP_FMT_1080P_30;
            break;

        case HI_DRV_DISP_FMT_1080P_23_976:
            retEncFmt = HI_DRV_DISP_FMT_1080P_24;
            break;

        case HI_DRV_DISP_FMT_1080i_59_94:
            retEncFmt = HI_DRV_DISP_FMT_1080i_60;
            break;
        default:
            retEncFmt = enEncFmt;
    }

    return retEncFmt;
}
