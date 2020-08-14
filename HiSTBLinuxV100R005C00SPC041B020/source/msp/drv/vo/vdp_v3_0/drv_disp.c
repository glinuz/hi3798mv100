/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_disp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/20
  Description   :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file

*******************************************************************************/
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>

#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_kernel_adapt.h"

#include "hi_drv_module.h"

#include "hi_drv_disp.h"
#include "drv_disp.h"
#include "drv_disp_ioctl.h"
#include "drv_display.h"
#include "drv_disp_debug.h"
#include "drv_disp_ext.h"
#include "drv_disp_osal.h"
#include "hi_osal.h"
#include "drv_win_ext.h"
#include "drv_disp_priv.h"
#include "hi_drv_stat.h"

#include "drv_hdmi_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

DRV_DISP_STATE_S         g_DispModState;
DRV_DISP_GLOBAL_STATE_S  g_DispUserCountState;
DRV_DISP_GLOBAL_STATE_S  g_DispKernelCountState;
DRV_DISP_GLOBAL_STATE_S  g_DispAllCountState;
HI_BOOL g_DispSuspend = HI_FALSE;
HI_S32 g_s32DispAttachCount = 0;
HI_U32 g_bDispPrivateInformation = 0;

static atomic_t        g_DispCount = ATOMIC_INIT(0);
HI_DECLARE_MUTEX(g_DispMutex);

HI_S32 DRV_DISP_ProcessCmd(unsigned int cmd, HI_VOID *arg, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);
HI_S32 DISP_ExtOpen(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);
HI_S32 DISP_ExtClose(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);

/* ================================================ */
#define DEF_DRV_DISP_PROC_FUNCTION_START_FROM_HERE
HI_U8 *g_pDispLayerString[HI_DRV_DISP_LAYER_BUTT] = {
    "NONE",
    "VIDEO",
    "GFX",
};

//TODO
HI_U8 *g_pVDPDispFmtString[HI_DRV_DISP_FMT_BUTT] = {
    "1080P60",
    "1080P50",
    "1080P30",
    "1080P25",
    "1080P24",
    "1080i60",
    "1080i50",
    "720P60",
    "720P50",

    "576P50",
    "480P60",

    "PAL",
    "PAL_B",
    "PAL_B1",
    "PAL_D",
    "PAL_D1",
    "PAL_G",
    "PAL_H",
    "PAL_K",
    "PAL_I",
    "PAL_M",
    "PAL_N",
    "PAL_Nc",
    "PAL_60",

    "NTSC",
    "NTSC_J",
    "NTSC_443",

    "SECAM_SIN",
    "SECAM_COS",
    "SECAM_L",
    "SECAM_B",
    "SECAM_G",
    "SECAM_D",
    "SECAM_K",
    "SECAM_H",

    "1440x576i",
    "1440x480i",

    "1080P24_FP",
    "720P60_FP",
    "720P50_FP",

    "640x480",
    "800x600",
    "1024x768",
    "1280x720",
    "1280x800",
    "1280x1024",
    "1360x768",
    "1366x768",
    "1400x1050",
    "1440x900",
    "1440x900_RB",

    "1600x900_RB",
    "1600x1200",
    "1680x1050",
    "1680x1050_RB",
    "1920x1080",
    "1920x1200",
    "1920x1440",
    "2048x1152",
    "2560x1440_RB",
     "2560x1600_RB",

    "3840x2160_24",
    "3840x2160_25",
    "3840x2160_30",
    "3840x2160_50",
    "3840x2160_60",
    
    "4096x2160_24",
    "4096x2160_25",
    "4096x2160_30",
    "4096x2160_50",
    "4096x2160_60",

    "3840x2160_23.976",
    "3840x2160_29.97",
    "1280x720_59.94",
    "1920x1080_59.94",
    "1920x1080_29.97",
    "1920x1080_23.976",
    "1920x1080i_59.94",
    "CustomerTiming"
};

HI_U8 *g_pVDPDispModeString[DISP_STEREO_BUTT] = {
    "2D",
    "FPK",
    "SBS_HALF",
    "TAB",
    "FILED_ALTE",
    "LINE_ALTE",
    "SBS_FULL",
    "L_DEPTH",
    "L_DEPTH_G_DEPTH",
};


HI_U8 *g_pVDPColorSpaceString[HI_DRV_CS_BUTT] = {
    "Butt",
    "Default",
    "BT601_YUV_LIMITED",
    "BT601_YUV_FULL",
    "BT601_RGB_LIMITED",
    "BT601_RGB_FULL",
    "NTSC1953",
    "BT470_M",
    "BT470_BG",
    "BT709_YUV_LIMITED",
    "BT709_YUV_FULL",
    "BT709_RGB_LIMITED",
    "BT709_RGB_FULL",
    "REC709",
    "SMPT170M",
    "SMPT240M",
    "BT878",
    "XVYCC",
    "JPEG",
};


HI_U8 *g_pVDPCInterfaceString[HI_DRV_DISP_INTF_ID_MAX] = {
    "YPbPr0",
      "RGB0",
    "S_VIDEO0",
    "CVBS0",
    "VGA0",
    "HDMI0",
    "HDMI1",
    "HDMI2",
    "BT656_0",
    "BT656_1",
    "BT656_2",
    "BT1120_0",
    "BT1120_1",
    "BT1120_2",
    "LCD_0",
    "LCD_1",
    "LCD_2",
};

HI_U8 *g_pDispMacrovisionString[4] = {"TYPE0", "TYPE1", "TYPE2", "TYPE3"};
#define DISPLAY_INVALID_ID 0xFFFFFFFFul
static HI_U32 s_DispProcId[HI_DRV_DISPLAY_BUTT]={DISPLAY_INVALID_ID};

static HI_S32 DISP_ProcRead(struct seq_file *p, HI_VOID *v);
static HI_S32 DISP_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos);


static HI_S32 DRV_DISP_ProcInit(HI_VOID)
{
    HI_U32 u;

    for(u=0; u<(HI_U32)HI_DRV_DISPLAY_BUTT; u++)
    {
        s_DispProcId[u] = DISPLAY_INVALID_ID;
    }

    return HI_SUCCESS;
}

static HI_VOID DRV_DISP_ProcDeInit(HI_VOID)
{
    HI_CHAR ProcName[12];
    HI_U32 u;

    for(u=0; u<(HI_U32)HI_DRV_DISPLAY_BUTT; u++)
    {
        if (s_DispProcId[u] != DISPLAY_INVALID_ID)
        {
            HI_OSAL_Snprintf(ProcName, 12, "%s%d", HI_MOD_DISP, u);
            HI_DRV_PROC_RemoveModule(ProcName);
        }
    }

    return;
}

static HI_S32 DRV_DISP_ProcAdd(HI_DRV_DISPLAY_E enDisp)
{
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR           ProcName[12];

    /* register HD-display PROC*/
    HI_OSAL_Snprintf(ProcName, 12, "%s%d", HI_MOD_DISP, enDisp);
    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_DISP("add %s proc failed.\n", ProcName);
        DISP_Close(enDisp);

        return HI_ERR_DISP_CREATE_ERR;
    }

    pProcItem->read  = DISP_ProcRead;
    pProcItem->write = DISP_ProcWrite;
    pProcItem->data  = (HI_VOID *)enDisp;

    s_DispProcId[enDisp] = enDisp;

    return HI_SUCCESS;
}

static HI_S32 DRV_DISP_ProcDel(HI_DRV_DISPLAY_E enDisp)
{
    HI_CHAR ProcName[12];

    /* register HD-display PROC*/
    HI_OSAL_Snprintf(ProcName, 12, "%s%d", HI_MOD_DISP, enDisp);
    HI_DRV_PROC_RemoveModule(ProcName);

    s_DispProcId[enDisp] = DISPLAY_INVALID_ID;
    return HI_SUCCESS;
}


HI_U8 disp_attr[256];
HI_U8 *Disp_GetAttr(HI_VOID)
{
    HI_U8 *pBuffer;
    memset(disp_attr, 0x0, 256);

#ifdef  HI_DISP_TTX_SUPPORT
    pBuffer = "ttx ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_DISP_ATTACH_OSD_SUPPORT
    pBuffer = "attach_osd";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef  HI_DISP_MODE_TC
    pBuffer = "tc ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_WRAP_SUPPORT
    pBuffer = "vo_wrap";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_DISP_CGMS_SUPPOR
    pBuffer = "cgms ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_DISP_LCD_SUPPORT
    HI_OSAL_Strncat(disp_attr, "lcd ", 256);
#endif

#ifdef HI_VO_MOSAIC_SUPPORT
    pBuffer = "mosaic ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_SINGLE_VIDEO_SUPPORT
    pBuffer = "signle_vid ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_STILLFRAME_SUPPORT
    pBuffer = "vo_still ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_DUMPFRAME_SUPPORT
    pBuffer = "vo_dump ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_MOSAIC_SUPPORT
    pBuffer = "mosaic ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_SHARPNESS_SUPPORT
    pBuffer = "sharp ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif

#ifdef HI_VO_HD_VIDEO_DO_DEI
    pBuffer = "vo_not_dei ";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
#endif
    pBuffer = "\r\n";
    HI_OSAL_Strncat(disp_attr, pBuffer, strlen(pBuffer)+1);
    return disp_attr;
}

#ifndef HI_ADVCA_FUNCTION_RELEASE
HI_CHAR *g_pVDPDispName[HI_DRV_DISPLAY_BUTT+1] = {"display0", "display1","display2","invalid  display"};
HI_CHAR *g_pVDPDispState[2] = {"Close", "Open"};
HI_CHAR *g_pVDPDispState_1[2] = {"Disable", "Enable"};
HI_CHAR *g_pVDPDispARMode[2] = {"Auto", "Custmer Setting"};
HI_CHAR *g_pVDPDispCastAllocMode[2] = {"DispAllocate", "UserAllocate"};
HI_U32  g_pVDPDispCastBufferState[5] = {1, 4, 2, 3, 4};
static DISP_PROC_INFO_S  s_DispAttr;
#endif


static HI_S32 DISP_ProcRead(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    DRV_PROC_ITEM_S  *pProcItem;
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 Ret, i;

    pProcItem = p->private;
    enDisp = (HI_DRV_DISPLAY_E)pProcItem->data;

    /* Disp PROC*/
    Ret = DISP_GetProcInto(enDisp, &s_DispAttr);
    if (Ret)
    {
        PROC_PRINT(p,"---------Get Hisilicon DISP %d Out Info Failed!---------\n", enDisp);
        return HI_SUCCESS;
    }
    PROC_PRINT(p,"---------Hisilicon DISP %d State---------\n", enDisp);

    PROC_PRINT(p, "%-20s:%s\n", "State",g_pVDPDispState[s_DispAttr.bEnable]);
    PROC_PRINT(p, "%-20s:%s/%s\n", "Formt/DispMode",g_pVDPDispFmtString[s_DispAttr.eFmt], g_pVDPDispModeString[s_DispAttr.eDispMode]);
    if (HI_DRV_DISP_FMT_CUSTOM == s_DispAttr.eFmt)
    {
        PROC_PRINT(p, "%-20s:\n", "------Custom Timing Para List------");
        PROC_PRINT(p, "       %-20s: %d/%d/%d\n","HACT/VACT/VFreq" ,s_DispAttr.stTiming.u32HACT,s_DispAttr.stTiming.u32VACT,s_DispAttr.stTiming.u32VertFreq);
        PROC_PRINT(p, "       %-20s: %d/%d/%d\n","HBB/HFB/HPW " ,s_DispAttr.stTiming.u32HBB,s_DispAttr.stTiming.u32HFB,s_DispAttr.stTiming.u32HPW);
        PROC_PRINT(p, "       %-20s: %d/%d/%d\n","VBB/VFB/VPW" ,s_DispAttr.stTiming.u32VBB,s_DispAttr.stTiming.u32VFB,s_DispAttr.stTiming.u32VPW);
        PROC_PRINT(p, "       %-20s: %d/%d/%d/%d\n","Inter/IDV/IHS/IVS" ,s_DispAttr.stTiming.bInterlace,s_DispAttr.stTiming.bIDV,s_DispAttr.stTiming.bIHS,s_DispAttr.stTiming.bIVS);
        PROC_PRINT(p, "       %-20s: %d/%d/%d\n","PixFreq/ARW/ARH",s_DispAttr.stTiming.u32PixFreq, s_DispAttr.stTiming.u32AspectRatioW,s_DispAttr.stTiming.u32AspectRatioH);
        PROC_PRINT(p, "       %-20s: 0x%x/0x%x\n","ClkPara0/ClkPara1/" ,s_DispAttr.stTiming.u32ClkPara0,s_DispAttr.stTiming.u32ClkPara1);
    }
    PROC_PRINT(p, "%-20s:%s\n", "RightEyeFirst",g_pVDPDispState_1[s_DispAttr.bRightEyeFirst]);
    PROC_PRINT(p, "%-20s:%d/%d\n", "VirtualScreen",s_DispAttr.stVirtaulScreen.s32Width,s_DispAttr.stVirtaulScreen.s32Height);
    PROC_PRINT(p, "%-20s:%d/%d/%d/%d\n", "Offset(L/T/R/B)",s_DispAttr.stOffsetInfo.u32Left,
                                                  s_DispAttr.stOffsetInfo.u32Top,
                                                  s_DispAttr.stOffsetInfo.u32Right,
                                                  s_DispAttr.stOffsetInfo.u32Bottom);

    PROC_PRINT(p, "%-20s:%s\n", "AspectRatioMode",g_pVDPDispARMode[(HI_U32)s_DispAttr.bCustAspectRatio]);
    PROC_PRINT(p, "%-20s:%d:%d\n", "AspectRatio",s_DispAttr.u32AR_w,s_DispAttr.u32AR_h);

    PROC_PRINT(p, "%-20s:%s->%s\n", "ColorSpace",g_pVDPColorSpaceString[(HI_U32)s_DispAttr.eDispColorSpace],
                                                g_pVDPColorSpaceString[(HI_U32)s_DispAttr.eDispColorSpace]);
    PROC_PRINT(p, "%-20s:%d\n", "Bright",s_DispAttr.u32Bright);
    PROC_PRINT(p, "%-20s:%d\n", "Contrast",s_DispAttr.u32Contrst);
    PROC_PRINT(p, "%-20s:%d\n", "Saturation",s_DispAttr.u32Satur);
    PROC_PRINT(p, "%-20s:%d\n", "Hue",s_DispAttr.u32Hue);
    PROC_PRINT(p, "%-20s:0x%x/0x%x/0x%x\n", "Background (R/G/B)",s_DispAttr.stBgColor.u8Red,
                                                     s_DispAttr.stBgColor.u8Green,
                                                     s_DispAttr.stBgColor.u8Blue);
    PROC_PRINT(p, "%-20s:%s->%s\n", "Zorder(Bot->Top)",g_pDispLayerString[s_DispAttr.enLayer[0]],
                                                        g_pDispLayerString[s_DispAttr.enLayer[1]]);

    if (s_DispAttr.bMaster == HI_TRUE)
        PROC_PRINT(p, "%-20s:%s\n", "AttachRole","source");

    if (s_DispAttr.bSlave == HI_TRUE)
        PROC_PRINT(p, "%-20s:%s\n", "AttachRole","destination");

    if ((s_DispAttr.bMaster != HI_TRUE) && (s_DispAttr.bSlave != HI_TRUE))
        PROC_PRINT(p, "%-20s:%s\n", "AttachRole","single running.");

    PROC_PRINT(p, "%-20s:%s\n", "AttachDisp",g_pVDPDispName[s_DispAttr.enAttachedDisp]);

    PROC_PRINT(p, "%-20s:", "Interface");
    for(i=0; i<s_DispAttr.u32IntfNumber;i++)
    {
        PROC_PRINT(p, "%s ", g_pVDPCInterfaceString[(HI_S32)s_DispAttr.stIntf[i].eID]);

        if (s_DispAttr.stIntf[i].eID <= HI_DRV_DISP_INTF_VGA0)
            PROC_PRINT(p, "(%3d/%3d/%3d) ",
                s_DispAttr.stIntf[i].u8VDAC_Y_G,
                s_DispAttr.stIntf[i].u8VDAC_Pb_B,
                s_DispAttr.stIntf[i].u8VDAC_Pr_R);
    }
    PROC_PRINT(p, "\n");

    PROC_PRINT(p, "%-20s:%d\n", "InitCount", atomic_read(&g_DispCount));
    PROC_PRINT(p, "%-20s:%d/%d\n", "OpenCnt[User/Kernel]", g_DispUserCountState.DispOpenNum[enDisp], g_DispKernelCountState.DispOpenNum[enDisp]);
    PROC_PRINT(p, "%-20s:%d\n", "LowbandCount", s_DispAttr.u32Underflow);

    if (s_DispAttr.pstCastInfor)
    {
        PROC_PRINT(p,"------------------CAST Info---------------------------------\n");
        PROC_PRINT(p, "%-20s:%s\n", "State", g_pVDPDispState_1[s_DispAttr.stCastInfor.bEnable]);
        PROC_PRINT(p, "%-20s:%s\n", "Crop", "False");
        PROC_PRINT(p, "%-20s:%d/%d/%d/%d\n", "CropRect(L/T/R/B)", 0,0,0,0);
        PROC_PRINT(p, "%-20s:%d/%d\n", "Resolution", s_DispAttr.stCastInfor.u32OutResolutionWidth,
                                                  s_DispAttr.stCastInfor.u32OutResolutionHeight);

        PROC_PRINT(p, "%-20s:%s\n", "PixelFormat","NV21");
        PROC_PRINT(p, "%-20s:%d\n", "FrameRate", s_DispAttr.stCastInfor.u32CastOutFrameRate/2);
        PROC_PRINT(p, "%-20s:%s\n", "LowDelay", g_pVDPDispState_1[s_DispAttr.stCastInfor.bLowDelay]);

        PROC_PRINT(p, "%-20s:%s\n", "MemoryType", g_pVDPDispCastAllocMode[s_DispAttr.stCastInfor.bUserAllocate]);
        PROC_PRINT(p, "%-20s:%d\n", "BufferNumber", s_DispAttr.stCastInfor.u32TotalBufNum);

        PROC_PRINT(p, "%-20s:%d/%d\n", "BufferWidth/Height", s_DispAttr.stCastInfor.u32OutResolutionWidth,
                                                             s_DispAttr.stCastInfor.u32OutResolutionHeight);

        PROC_PRINT(p, "%-20s:0x%x\n", "BufferSize", s_DispAttr.stCastInfor.u32BufSize);
        PROC_PRINT(p, "%-20s:%d\n", "BufferStride", s_DispAttr.stCastInfor.u32BufStride);

        PROC_PRINT(p, "%-20s:%d\n", "bAttached", s_DispAttr.stCastInfor.bAttached);
        if (g_bDispPrivateInformation)
        {
           PROC_PRINT(p, "%-20s:%d\n", "CastIntrCnt", s_DispAttr.stCastInfor.u32CastIntrCnt);
        }

        PROC_PRINT(p,"------------------Buffer---------------------------------\n");
        PROC_PRINT(p, "%-20s:%d/%d\n", "Acquire(Try/OK)", s_DispAttr.stCastInfor.u32CastAcquireTryCnt,
                                                          s_DispAttr.stCastInfor.u32CastAcquireOkCnt);

        PROC_PRINT(p, "%-20s:%d/%d\n", "Release(Try/OK)", s_DispAttr.stCastInfor.u32CastReleaseTryCnt,
                                                          s_DispAttr.stCastInfor.u32CastReleaseOkCnt);


        PROC_PRINT(p,"---------------------------------------------------------\n");
        PROC_PRINT(p,"BufferQueue: [state, FrameID]\n");
        PROC_PRINT(p,"(State: 1,Empty[%d]; 2,Write[%d]; 3,Full[%d]; 4,Use[%d]\n",
                                    s_DispAttr.stCastInfor.u32CastEmptyBufferNum,
                                    s_DispAttr.stCastInfor.u32CastWriteBufferNum,
                                    s_DispAttr.stCastInfor.u32CastFullBufferNum,
                                    s_DispAttr.stCastInfor.u32CastUsedBufferNum);

        for (i = 0; i < s_DispAttr.stCastInfor.u32TotalBufNum; i++)
        {
            if ((i != 0) && ((i%4) ==0))
                PROC_PRINT(p,"\n");

                PROC_PRINT(p,"[%d,0x%x] ", g_pVDPDispCastBufferState[s_DispAttr.stCastInfor.enState[i]],
                                           s_DispAttr.stCastInfor.u32FrameIndex[i]);
        }

        PROC_PRINT(p,"\n");
    }

#endif

    return HI_SUCCESS;
}

static HI_S32 DispProcParsePara(HI_CHAR *pProcPara,HI_CHAR **ppArg1,HI_CHAR **ppArg2)
{
    HI_CHAR *pChar = HI_NULL;

    if (strlen(pProcPara) == 0)
    {
        /* not fined arg1 and arg2, return failed */
        *ppArg1  = HI_NULL;
        *ppArg2  = HI_NULL;
        return HI_FAILURE;
    }

    /* find arg1 */
    pChar = pProcPara;
    while( (*pChar == ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    if (*pChar != '\0')
    {
        *ppArg1 = pChar;
    }
    else
    {
        *ppArg1  = HI_NULL;

        return HI_FAILURE;
    }

    /* ignor arg1 */
    while( (*pChar != ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    /* Not find arg2, return */
    if (*pChar == '\0')
    {
        *ppArg2 = HI_NULL;

        return HI_SUCCESS;
    }

    /* add '\0' for arg1 */
    *pChar = '\0';

    /* start to find arg2 */
    pChar = pChar + 1;
    while( (*pChar == ' ') && (*pChar != '\0') )
    {
        pChar++;
    }

    if (*pChar != '\0')
    {
        *ppArg2 = pChar;
    }
    else
    {
        *ppArg2 = HI_NULL;
    }

    return HI_SUCCESS;
}



HI_DRV_DISP_FMT_E DispGetFmtbyString(HI_CHAR *pFmtString)
{
    if (0 == HI_OSAL_Strncmp(pFmtString, "1080p60", strlen("1080p60")))
    {
        return HI_DRV_DISP_FMT_1080P_60;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080p50", strlen("1080p50")))
    {
        return HI_DRV_DISP_FMT_1080P_50;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080p30", strlen("1080p30")))
    {
        return HI_DRV_DISP_FMT_1080P_30;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080p25", strlen("1080p25")))
    {
        return HI_DRV_DISP_FMT_1080P_25;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080p24", strlen("1080p24")))
    {
        return HI_DRV_DISP_FMT_1080P_24;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080i60", strlen("1080i60")))
    {
        return HI_DRV_DISP_FMT_1080i_60;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080i50", strlen("1080i50")))
    {
        return HI_DRV_DISP_FMT_1080i_50;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "720p60", strlen("720p60")))
    {
        return HI_DRV_DISP_FMT_720P_60;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "720p50", strlen("720p50")))
    {
        return HI_DRV_DISP_FMT_720P_50;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "576p50", strlen("576p50")))
    {
        return HI_DRV_DISP_FMT_576P_50;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "480p60", strlen("480p60")))
    {
        return HI_DRV_DISP_FMT_480P_60;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "pal", strlen("pal")))
    {
        return HI_DRV_DISP_FMT_PAL;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "ntsc", strlen("ntsc")))
    {
        return HI_DRV_DISP_FMT_NTSC;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1080p24fp", strlen("1080p24fp")))
    {
        return HI_DRV_DISP_FMT_1080P_24_FP;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "720p60fp", strlen("720p60fp")))
    {
        return HI_DRV_DISP_FMT_720P_60_FP;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "720p50fp", strlen("720p50fp")))
    {
        return HI_DRV_DISP_FMT_720P_50_FP;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_24", strlen("3840*2160_24")))
    {
        return HI_DRV_DISP_FMT_3840X2160_24;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_25", strlen("3840*2160_25")))
    {
        return HI_DRV_DISP_FMT_3840X2160_25;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_30", strlen("3840*2160_30")))
    {
        return HI_DRV_DISP_FMT_3840X2160_30;
    }
	else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_50", strlen("3840*2160_50")))
    {
        return HI_DRV_DISP_FMT_3840X2160_50;
    }
	else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_60", strlen("3840*2160_60")))
    {
        return HI_DRV_DISP_FMT_3840X2160_60;
    }

    else if (0 == HI_OSAL_Strncmp(pFmtString, "4096*2160_24", strlen("4096*2160_24")))
    {
        return HI_DRV_DISP_FMT_4096X2160_24;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "4096*2160_25", strlen("4096*2160_25")))
    {
        return HI_DRV_DISP_FMT_4096X2160_25;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "4096*2160_30", strlen("4096*2160_30")))
    {
        return HI_DRV_DISP_FMT_4096X2160_30;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "4096*2160_50", strlen("4096*2160_50")))
    {
        return HI_DRV_DISP_FMT_4096X2160_50;
    }
	else if (0 == HI_OSAL_Strncmp(pFmtString, "4096*2160_60", strlen("4096*2160_60")))
    {
        return HI_DRV_DISP_FMT_4096X2160_60;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_23_976", strlen("3840*2160_23_976")))
    {
        return HI_DRV_DISP_FMT_3840X2160_23_976;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "3840*2160_29_97", strlen("3840*2160_29_97")))
    {
        return HI_DRV_DISP_FMT_3840X2160_29_97;
    }

    else if (0 == HI_OSAL_Strncmp(pFmtString, "1280*720_59_94", strlen("1280*720_59_94")))
    {
        return HI_DRV_DISP_FMT_720P_59_94;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1920*1080_59_94", strlen("1920*1080_59_94")))
    {
        return HI_DRV_DISP_FMT_1080P_59_94;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1920*1080_29_97", strlen("1920*1080_29_97")))
    {
        return HI_DRV_DISP_FMT_1080P_29_97;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1920*1080_23_976", strlen("1920*1080_23_976")))
    {
        return HI_DRV_DISP_FMT_1080P_23_976;
    }
    else if (0 == HI_OSAL_Strncmp(pFmtString, "1920*1080i_59_94", strlen("1920*1080i_59_94")))
    {
        return HI_DRV_DISP_FMT_1080i_59_94;
    }
    else
    {
        return HI_DRV_DISP_FMT_BUTT;
    }

}

HI_S32 fpga_test_disp_Init(char *filenametmp);
HI_S32 fpga_test_disp_deInit(HI_VOID);
HI_S32 fpga_test_vo_createwin(HI_VOID);
HI_S32 fpga_test_vo_destroywin(HI_VOID);
HI_S32  fpga_openfile_queue_frame(HI_U32 u32Width,
                          HI_U32 u32Height,
                          HI_U32 u32Stride);

//HI_S32 DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enEncFmt);
HI_S32 DispProcCmdProcee(HI_DRV_DISPLAY_E enDisp, HI_CHAR *pArg1,HI_CHAR *pArg2)
{
    HI_S32 nRet = HI_SUCCESS;
    HI_VOID*  hCommonHandle;

    if (0 == HI_OSAL_Strncmp(pArg1, "fmt", strlen("fmt")))
    {
        HI_DRV_DISP_FMT_E fmt;

        fmt = DispGetFmtbyString(pArg2);

        if (fmt == HI_DRV_DISP_FMT_BUTT)
        {
            return HI_ERR_DISP_NOT_SUPPORT_FMT;
        }
        else if( (fmt >= HI_DRV_DISP_FMT_1080P_24_FP) && (fmt <= HI_DRV_DISP_FMT_720P_50_FP))
        {
            nRet = DISP_SetFormat(enDisp, HI_DRV_DISP_STEREO_FRAME_PACKING, fmt);
        }
        else
        {
            HI_DRV_DISP_FMT_E fmtold;
            HI_DRV_DISP_STEREO_MODE_E enStereoold;

            nRet = DISP_GetFormat(enDisp, &enStereoold, &fmtold);
            if (nRet == HI_SUCCESS)
            {
                nRet = DISP_SetFormat(enDisp, enStereoold, fmt);
            }
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "3d", strlen("3d")))
    {
        HI_DRV_DISP_FMT_E fmtold;
        HI_DRV_DISP_STEREO_MODE_E enStereoold;

        nRet = DISP_GetFormat(enDisp, &enStereoold, &fmtold);
        if (nRet != HI_SUCCESS)
        {
            return HI_FAILURE;
        }

        if (0 == HI_OSAL_Strncmp(pArg2, "2d", strlen("2d")))
        {
            nRet = DISP_SetFormat(enDisp, HI_DRV_DISP_STEREO_NONE, fmtold);
        }

        if (0 == HI_OSAL_Strncmp(pArg2, "sbs_hf", strlen("sbs_hf")))
        {
            nRet = DISP_SetFormat(enDisp, HI_DRV_DISP_STEREO_SBS_HALF, fmtold);
        }

        if (0 == HI_OSAL_Strncmp(pArg2, "tab", strlen("tab")))
        {
            nRet = DISP_SetFormat(enDisp, HI_DRV_DISP_STEREO_TAB, fmtold);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "rf", strlen("rf")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "on", strlen("on")))
        {
            nRet = DISP_SetRightEyeFirst(enDisp, HI_TRUE);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = DISP_SetRightEyeFirst(enDisp, HI_FALSE);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "bright", strlen("bright")))
    {
        return HI_ERR_DISP_NOT_SUPPORT;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "contrast", strlen("contrast")))
    {
        return HI_ERR_DISP_NOT_SUPPORT;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "hue", strlen("hue")))
    {
        return HI_ERR_DISP_NOT_SUPPORT;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "satu", strlen("satu")))
    {
        return HI_ERR_DISP_NOT_SUPPORT;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "left", strlen("left")))
    {
        HI_DRV_DISP_OFFSET_S screenoffset;

        nRet = DISP_GetScreenOffset(enDisp, &screenoffset);
        if (nRet == HI_SUCCESS)
        {
            screenoffset.u32Left= (HI_U32)simple_strtol(pArg2, NULL, 10);
            nRet = DISP_SetScreenOffset(enDisp, &screenoffset);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "right", strlen("right")))
    {
        HI_DRV_DISP_OFFSET_S screenoffset;

        nRet = DISP_GetScreenOffset(enDisp, &screenoffset);
        if (nRet == HI_SUCCESS)
        {
            screenoffset.u32Right = (HI_U32)simple_strtol(pArg2, NULL, 10);
            nRet = DISP_SetScreenOffset(enDisp, &screenoffset);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "bottom", strlen("bottom")))
    {
        HI_DRV_DISP_OFFSET_S screenoffset;

        nRet = DISP_GetScreenOffset(enDisp, &screenoffset);
        if (nRet == HI_SUCCESS)
        {
            screenoffset.u32Bottom = (HI_U32)simple_strtol(pArg2, NULL, 10);
            nRet = DISP_SetScreenOffset(enDisp, &screenoffset);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "video", strlen("video")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "up", strlen("up")))
        {
            nRet = DISP_SetLayerZorder(enDisp, HI_DRV_DISP_LAYER_VIDEO, HI_DRV_DISP_ZORDER_MOVEUP);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "down", strlen("down")))
        {
            nRet = DISP_SetLayerZorder(enDisp, HI_DRV_DISP_LAYER_VIDEO, HI_DRV_DISP_ZORDER_MOVEDOWN);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "reset", strlen("reset")))
    {

        HI_ERR_DISP("Not support set rotation now\n");
        return HI_SUCCESS;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "dac_detect", strlen("dac_detect")))
    {
        HI_U32 u32Data = (HI_U32)simple_strtol(pArg2, NULL, 10);
        if(0 == u32Data)
        {
            HI_DRV_PROC_EchoHelper("dac detect close\n");
            DISP_SetDACDetEn(HI_FALSE);
        }
        else if (1 == u32Data)
        {
            HI_DRV_PROC_EchoHelper("dac detect open\n");
            DISP_SetDACDetEn(HI_TRUE);
        }
        else  //do nothing
        {

        }

        return HI_SUCCESS;
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "cat", strlen("cat")))
    {
        HI_UCHAR *pChar = pArg2;
        HI_DRV_VIDEO_FRAME_S *pstCurFrame;
        HI_U32 pathlength = 0;


        pstCurFrame = (HI_DRV_VIDEO_FRAME_S*)DISP_MALLOC(sizeof(HI_DRV_VIDEO_FRAME_S));
        if (!pstCurFrame)
        {
            HI_ERR_DISP("alloc frame info memory failed\n");
            return nRet;
        }

        /* get currently displayed frame */
        nRet = DISP_AcquireSnapshot(enDisp, pstCurFrame, &hCommonHandle);
        if (nRet != HI_SUCCESS)
        {
            HI_ERR_DISP("catpure screen failed\n");
            DISP_FREE(pstCurFrame);
            return nRet;
        }

        /* calculate char nubmer of path string */
        while( (*pChar != ' ') && (*pChar != '\0') )
        {
            pChar++;
            pathlength++;
        }
        pathlength++;

        /* save yuv frame */
        nRet = vdp_DebugSaveYUVImg(pstCurFrame, pArg2, pathlength);

        DISP_ReleaseSnapshot(enDisp, pstCurFrame, hCommonHandle);

        DISP_FREE(pstCurFrame);
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "low_delay_stat", strlen("low_delay_stat")))
    {
        HI_HANDLE hCast;
        HI_VOID* hCast_ptr;

        if (HI_DRV_DISPLAY_1 != enDisp)
        {
            return HI_FAILURE;
        }

        (HI_VOID)DispGetCastHandle(enDisp, &hCast, &hCast_ptr);
        if (0 == HI_OSAL_Strncmp(pArg2, "start", strlen("start")))
        {
            if ((hCast) &&(hCast_ptr))
            {
                HI_DRV_LD_Start_Statistics(SCENES_VID_CAST, hCast);
            }
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "stop", strlen("stop")))
        {
            HI_DRV_LD_Stop_Statistics();
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "rd_door", strlen("rd_door")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "start", strlen("start")))
        {
            g_bDispPrivateInformation = HI_TRUE;
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "stop", strlen("stop")))
        {
            g_bDispPrivateInformation = HI_FALSE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "fpga_test_start", strlen("fpga_test_start")))
    {
#ifndef HI_ADVCA_FUNCTION_RELEASE
        fpga_test_disp_Init(pArg2);
        fpga_test_vo_createwin();
        fpga_openfile_queue_frame(720,576,768);
#endif        
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "fpga_test_stop", strlen("fpga_test_stop")))
    {
#ifndef HI_ADVCA_FUNCTION_RELEASE
        fpga_test_vo_destroywin();
        fpga_test_disp_deInit();
#endif
    }
    else
    {
        return HI_FAILURE;
    }

    return nRet;
}


HI_VOID DISP_ProcPrintHelp(HI_VOID)
{
/*
    HI_ERR_DISP("Please input these commands:\n"
           "echo enfromat = 0(1080P60)|1(1080P50)|2(1080p30)|3(1080p25)|4(1080p24)|\n"
                "5(1080i60)|6(1080i50)|7(720p60)|8(720p50)|9(576p50)|10(480p60)\n "
                "11(pal)|12(pal_n)|13(pal_nc)|14(ntsc)|15(ntsc_j)|16(ntsc_pal_m)\n "
                "17(secam_sin)|18(secam_cos) > /proc/msp/dispX\n"
           "echo bright = 0~100 > /proc/msp/dispxxx\n"
           "echo contrast = 0~100 > /proc/msp/dispxxx\n"
           "echo saturation = 0~100 > /proc/msp/dispxxx\n");
*/
    HI_DRV_PROC_EchoHelper("echo help                            > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo fmt 1080i50/720p50/pal/ntsc/... > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo 3d fp/sbs_hf/tab                > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo rf on/off                       > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo bright   X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo contrast X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo hue      X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo satu     X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo left     X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo top      X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo right    X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo bottom   X                      > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo video up/down                   > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo reset                           > /proc/msp/dispX\n");
    HI_DRV_PROC_EchoHelper("echo cat AbsolutePath                > /proc/msp/dispX\n");
}


HI_CHAR u8DispProcBuffer[256];
static HI_S32 DISP_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file   *p = file->private_data;
    DRV_PROC_ITEM_S  *pProcItem = p->private;
    HI_CHAR *pArg1, *pArg2;
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;

    pProcItem = p->private;
    enDisp = (HI_DRV_DISPLAY_E)pProcItem->data;

    if(count >= sizeof(u8DispProcBuffer))
    {
        HI_ERR_DISP("your parameter string is too long!\n");
        return -EFAULT;
    }

    memset(u8DispProcBuffer, 0, sizeof(u8DispProcBuffer));
    if (copy_from_user(u8DispProcBuffer, buf, count))
    {
        HI_ERR_DISP("MMZ: copy_from_user failed!\n");
        return -EFAULT;
    }
    u8DispProcBuffer[count] = 0;

    nRet = DispProcParsePara(u8DispProcBuffer, &pArg1, &pArg2);
    if(  (nRet != HI_SUCCESS)
        ||(0 == HI_OSAL_Strncmp(pArg1, "help", strlen("help")))
        ||(pArg2 == HI_NULL) )
    {
        DISP_ProcPrintHelp();
        return -EFAULT;
    }

    HI_PRINT("====================echo debug disp%d\n", enDisp);
    nRet = DispProcCmdProcee(enDisp, pArg1, pArg2);
    if (nRet != HI_SUCCESS)
    {
        DISP_ProcPrintHelp();
    }
    return count;
}



/***************************************************************/
#define DEF_DRV_DISP_FILE_FUNCTION_START_FROM_HERE

HI_S32 DISP_CheckPara(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState)
{
    if ((enDisp < HI_DRV_DISPLAY_BUTT) && pDispState->bDispOpen[enDisp])
    {
        return HI_SUCCESS;
    }

    return HI_ERR_DISP_INVALID_PARA;
}

HI_S32 DISP_FileOpen(struct inode *finode, struct file  *ffile)
{
    DRV_DISP_STATE_S *pDispState = HI_NULL;
    HI_DRV_DISPLAY_E u;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = HI_KMALLOC(HI_ID_DISP, sizeof(DRV_DISP_STATE_S), GFP_KERNEL);
    if (!pDispState)
    {
        HI_FATAL_DISP("malloc pDispState failed.\n");
        up(&g_DispMutex);
        return -1;
    }

    if (1 == atomic_inc_return(&g_DispCount))
    {
        /* for configuration such as start clock, pins re-use, etc  */
        Ret = DISP_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_KFREE(HI_ID_DISP, pDispState);
            HI_FATAL_DISP("call DISP_Init failed.\n");
            atomic_dec(&g_DispCount);
            up(&g_DispMutex);
            return -1;
        }
    }

    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        pDispState->bDispOpen[u] = HI_FALSE;
        pDispState->hCastHandle[u] = HI_NULL;
        pDispState->hSnapshot[u] = HI_NULL;
		pDispState->hVbi[u] = HI_FALSE;
    }

    ffile->private_data = pDispState;

    up(&g_DispMutex);
    return 0;
}


HI_S32 DISP_FileClose(struct inode *finode, struct file  *ffile)
{
    DRV_DISP_STATE_S *pDispState;
    HI_DRV_DISPLAY_E u;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = ffile->private_data;


    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        /*to close cast service, no matter disp open or not.*/
        if (pDispState->hCastHandle[u])
        {
            Ret = DISP_DestroyCast(pDispState->hCastHandle[u]);
            if (Ret != HI_SUCCESS)
                HI_ERR_DISP("destroy cast  %d  failed!\n", u);
        }

        /*to close snapshot service, for ctrl+c condition.*/
        if (pDispState->hSnapshot[u])
        {
            Ret = DISP_DestroySnapshot(pDispState->hSnapshot[u]);
            if (Ret != HI_SUCCESS)
                HI_ERR_DISP("destroy cast  %d  failed!\n", u);
		}

		if (pDispState->hVbi[u])
		{
			Ret = DRV_VBI_DeInit();
			if (Ret != HI_SUCCESS)
				HI_ERR_DISP("destroy vbi  %d  failed!\n", u);
		}

        if (pDispState->bDispOpen[u])
        {
            Ret = DISP_ExtClose(u, pDispState, HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_DISP("Display %d close failed!\n", u);
            }
        }
    }

    if (atomic_dec_and_test(&g_DispCount))
    {
        /* for close of clock */
        DISP_DeInit();

        //HI_ERR_DISP("DISP_FileClose DISP_DeInit\n");

        // add for multiple process
        g_s32DispAttachCount = 0;
    }

    HI_KFREE(HI_ID_DISP, ffile->private_data);

    up(&g_DispMutex);
    return 0;
}


HI_S32 DRV_DISP_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    DRV_DISP_STATE_S *pDispState;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = file->private_data;

    Ret = DRV_DISP_ProcessCmd(cmd, arg, pDispState, HI_TRUE);

    up(&g_DispMutex);
    return Ret;
}


/***************************************************************/
#define DEF_DRV_DISP_DRV_FUNCTION_START_FROM_HERE

HI_S32 HI_DRV_DISP_Process(HI_U32 cmd, HI_VOID *arg)
{
    DRV_DISP_STATE_S    *pDispState;
    HI_S32          Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = &g_DispModState;

    Ret = DRV_DISP_ProcessCmd(cmd, arg, pDispState, HI_FALSE);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_Process_Intr(HI_U32 cmd, HI_VOID *arg)
{
    DRV_DISP_STATE_S    *pDispState;
    HI_S32          Ret;
    pDispState = &g_DispModState;
    Ret = DRV_DISP_ProcessCmd(cmd, arg, pDispState, HI_FALSE);
    return Ret;
}
HI_U32 DISP_ResetCountStatus(void)
{
    HI_DRV_DISPLAY_E u;

    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        g_DispAllCountState.DispOpenNum[u] = 0;
        g_DispUserCountState.DispOpenNum[u] = 0;
        g_DispKernelCountState.DispOpenNum[u] = 0;
        g_DispModState.bDispOpen[u] = HI_FALSE;
        g_DispModState.hCastHandle[u] = HI_NULL;
        g_DispModState.hSnapshot[u] = HI_NULL;
    }

    g_DispSuspend = HI_FALSE;
    g_s32DispAttachCount = 0;

    return HI_SUCCESS;
}


HI_U32 DISP_Get_CountStatus(void)
{
    HI_DRV_DISPLAY_E u;
    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        if (g_DispAllCountState.DispOpenNum[u] > 0)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_S32 DISP_ExtOpen(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{
    HI_S32            Ret;

    /* create DISP for the first time */
    if (!pDispState->bDispOpen[enDisp])
    {
        /* call basic interface for the first time creating DISP globally*/
        if (!g_DispAllCountState.DispOpenNum[enDisp])
        {
            Ret = DISP_Open(enDisp);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_DISP(" Error number is: %x.\n", Ret);
                return Ret;
            }
        }

        pDispState->bDispOpen[enDisp] = HI_TRUE;

        g_DispAllCountState.DispOpenNum[enDisp]++;

        if (HI_TRUE == bUser)
        {
            g_DispUserCountState.DispOpenNum[enDisp]++;
        }
        else
        {
            g_DispKernelCountState.DispOpenNum[enDisp]++;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DISP_ExtClose(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{
    HI_S32       Ret = HI_SUCCESS;

    /* destroy DISP */
    if (pDispState->bDispOpen[enDisp])
    {
        if (HI_TRUE == bUser)
        {
            if (g_DispUserCountState.DispOpenNum[enDisp] == 0)
            {
                HI_WARN_DISP("Already Close User display%d =0\n", enDisp);
                return 0;
            }

            g_DispUserCountState.DispOpenNum[enDisp]--;  /* User count --   */
        }
        else
        {
            if (g_DispKernelCountState.DispOpenNum[enDisp] == 0)
            {
            HI_WARN_DISP("Already Close kernel display%d =0\n", enDisp);
            return 0;
            }
            g_DispKernelCountState.DispOpenNum[enDisp]--;

        }

        g_DispAllCountState.DispOpenNum[enDisp]--;  /* Global count -- */

        if (!g_DispAllCountState.DispOpenNum[enDisp])
        {
            Ret = DISP_Close(enDisp);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_DISP("call DISP_Close failed.\n");
            }

            g_s32DispAttachCount = 0;
        }

        pDispState->bDispOpen[enDisp] = HI_FALSE;
    }

    return Ret;
}


HI_S32 DISP_ExtAttach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 nRet = HI_SUCCESS;

    if ( (enMaster != HI_DRV_DISPLAY_1) || (enSlave != HI_DRV_DISPLAY_0))
    {
        HI_FATAL_DISP("Attach parameters invalid.\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if(   (0 == g_DispAllCountState.DispOpenNum[enMaster])
        &&(0 == g_DispAllCountState.DispOpenNum[enSlave]) )

    {
        nRet = DISP_Attach(enMaster, enSlave);
    }

    return nRet;
}

HI_S32 DISP_ExtDetach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 nRet = HI_SUCCESS;

    if ( (enMaster != HI_DRV_DISPLAY_1) || (enSlave != HI_DRV_DISPLAY_0))
    {
        HI_FATAL_DISP("Attach parameters invalid.\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if(   (0 == g_DispAllCountState.DispOpenNum[enMaster])
        &&(0 == g_DispAllCountState.DispOpenNum[enSlave]) )

    {
        nRet = DISP_Detach(enMaster, enSlave);
//HI_ERR_DISP(" DISP_ExtDetach  003 = 0x%x\n", nRet);
    }

    return nRet;
}


/* DRV_DISP_XXX */
HI_S32 HI_DRV_DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 Ret;
    DISP_ATTACH_S  enDispAttach;

    enDispAttach.enMaster = enMaster;
    enDispAttach.enSlave  = enSlave;
    Ret = HI_DRV_DISP_Process(CMD_DISP_ATTACH, &enDispAttach);
    return Ret;
}

HI_S32 HI_DRV_DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 Ret;
    DISP_ATTACH_S  enDispAttach;

    enDispAttach.enMaster = enMaster;
    enDispAttach.enSlave  = enSlave;
    Ret = HI_DRV_DISP_Process(CMD_DISP_DETACH, &enDispAttach);
    return Ret;
}

HI_S32 HI_DRV_DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enFormat)
{
    HI_S32 Ret;
    DISP_FORMAT_S  enDispFormat;

    enDispFormat.enDisp = enDisp;
    enDispFormat.enFormat = enFormat;
    enDispFormat.enStereo = HI_DRV_DISP_STEREO_NONE;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_FORMAT,  &enDispFormat);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penFormat)
{
    HI_S32 Ret;
    DISP_FORMAT_S  enDispFormat;

    if (!penFormat)
    {
        return HI_ERR_DISP_NULL_PTR;
    }
    memset(&enDispFormat, 0, sizeof(DISP_FORMAT_S));
    enDispFormat.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_FORMAT,  &enDispFormat);
    if (!Ret)
    {
        *penFormat = enDispFormat.enFormat;
    }
    return Ret;
}

HI_S32 HI_DRV_DISP_SetCustomTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming)
{
    HI_S32 Ret;
    DISP_TIMING_S  DispTiming;
    DispTiming.enDisp = enDisp;
    memcpy(&DispTiming.stTimingPara, pstTiming, sizeof(HI_DRV_DISP_TIMING_S));
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_TIMING, &DispTiming);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetCustomTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming)
{
    HI_S32 Ret;
    DISP_TIMING_S  DispTiming;
    memset(&DispTiming, 0, sizeof(DISP_TIMING_S));
    DispTiming.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_TIMING, &DispTiming);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    memcpy(pstTiming, &DispTiming.stTimingPara, sizeof(HI_DRV_DISP_TIMING_S));
    return Ret;
}

HI_S32 HI_DRV_DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    HI_S32          Ret;
    DISP_SET_INTF_S DispIntf;

    DispIntf.enDisp = enDisp;

    memcpy(&DispIntf.stIntf, pstIntf, sizeof(HI_DRV_DISP_INTF_S));

    Ret = HI_DRV_DISP_Process(CMD_DISP_ADD_INTF, &DispIntf);
    return Ret;
}

HI_S32 HI_DRV_DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    HI_S32          Ret;
    DISP_SET_INTF_S DispIntf;

    DispIntf.enDisp = enDisp;

    memcpy(&DispIntf.stIntf, pstIntf, sizeof(HI_DRV_DISP_INTF_S));

    Ret = HI_DRV_DISP_Process(CMD_DISP_DEL_INTF, &DispIntf);
    return Ret;
}

HI_S32 DRV_DISP_AttachVDAC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_ID_E eVdacId, HI_DRV_DISP_VDAC_SIGNAL_E eSignal)
{
    return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_DRV_DISP_Open(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;

    Ret = HI_DRV_DISP_Process(CMD_DISP_OPEN, &enDisp);
    return Ret;
}

HI_S32 HI_DRV_DISP_Close(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;

    Ret = HI_DRV_DISP_Process(CMD_DISP_CLOSE, &enDisp);
    return Ret;
}

HI_S32 HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    HI_S32 Ret;
    DISP_ENABLE_S  stDispEnable;

    stDispEnable.bEnable = bEnable;
    stDispEnable.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_ENABLE, &stDispEnable);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable)
{
    HI_S32 Ret;
    DISP_ENABLE_S  stDispEnable;

    memset(&stDispEnable, 0, sizeof(DISP_ENABLE_S));
    stDispEnable.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_ENABLE, &stDispEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    *pbEnable = stDispEnable.bEnable;
    return Ret;
}

HI_S32 HI_DRV_DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    HI_S32 Ret;
    DISP_R_EYE_FIRST_S  stREFirst;

    stREFirst.bREFirst = bEnable;
    stREFirst.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_R_E_FIRST, &stREFirst);
    return Ret;
}

HI_S32 HI_DRV_DISP_SetBgColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor)
{
    HI_S32 Ret;
    DISP_BGC_S  stDispBgc;

    stDispBgc.stBgColor = *pstBgColor;
    stDispBgc.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_BGC, &stDispBgc);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetBgColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor)
{
    HI_S32 Ret;
    DISP_BGC_S  stDispBgc;

    memset(&stDispBgc, 0, sizeof(DISP_BGC_S));
    stDispBgc.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_BGC, &stDispBgc);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    *pstBgColor = stDispBgc.stBgColor ;
    return Ret;
}

HI_S32 HI_DRV_DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v)
{
    HI_S32 Ret;
    DISP_ASPECT_RATIO_S stDispRatio;

    stDispRatio.enDisp = enDisp;
    stDispRatio.u32ARHori = u32Ratio_h;
    stDispRatio.u32ARVert = u32Ratio_v;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_DEV_RATIO, &stDispRatio);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v)
{
    HI_S32 Ret;
    DISP_ASPECT_RATIO_S stDispRatio;

    memset(&stDispRatio, 0, sizeof(DISP_ASPECT_RATIO_S));
    stDispRatio.enDisp = enDisp;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_DEV_RATIO, &stDispRatio);
    if(!Ret)
    {
        *pu32Ratio_h = stDispRatio.u32ARHori;
        *pu32Ratio_v = stDispRatio.u32ARVert;
    }
    return Ret;
}

HI_S32 HI_DRV_DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_S32 Ret;
    DISP_ZORDER_S stDispZorder;

    stDispZorder.enDisp = enDisp;
    stDispZorder.Layer  = enLayer;
    stDispZorder.ZFlag  = enZFlag;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_ZORDER, &stDispZorder);
    return Ret;
}

HI_S32 HI_DRV_DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder)
{
    HI_S32 Ret;
    DISP_ZORDER_S stDispZorder;

    memset(&stDispZorder, 0, sizeof(DISP_ZORDER_S));
    stDispZorder.enDisp = enDisp;
    stDispZorder.Layer  = enLayer;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_ZORDER, &stDispZorder);
    *pu32Zorder = stDispZorder.ZFlag;
    return Ret;
}

HI_S32 HI_DRV_DISP_CreateCast (HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast)
{
    HI_S32 Ret;
    DISP_CAST_CREATE_S stCastCreate;

    stCastCreate.enDisp = enDisp;
    stCastCreate.hCast = *phCast;
    stCastCreate.stCfg = *pstCfg;
    Ret = HI_DRV_DISP_Process(CMD_DISP_CREATE_CAST, &stCastCreate);
    return Ret;
}

HI_S32  HI_DRV_DISP_DestroyCast(HI_HANDLE hCast)
{
    HI_S32 Ret;
    DISP_CAST_DESTROY_S stCastDestroy;

    stCastDestroy.hCast = hCast;
    Ret = HI_DRV_DISP_Process(CMD_DISP_DESTROY_CAST, &stCastDestroy);
    return Ret;
}

HI_S32 HI_DRV_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    HI_S32 Ret;
    DISP_CAST_ENABLE_S stCastEnable;

    stCastEnable.hCast = hCast;
    stCastEnable.bEnable = bEnable;
    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_CAST_ENABLE, &stCastEnable);
    return Ret;
}

HI_S32  HI_DRV_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable)
{
    HI_S32 Ret;
    DISP_CAST_ENABLE_S stCastEnable;

    memset(&stCastEnable, 0, sizeof(DISP_CAST_ENABLE_S));
    stCastEnable.hCast = hCast;
    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_CAST_ENABLE, &stCastEnable);
    if(!Ret)
    {
        *pbEnable = stCastEnable.bEnable;
    }
    return Ret ;
}

HI_S32  HI_DRV_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    HI_S32 Ret;
    DISP_CAST_FRAME_S stCastFrame;

    memset(&stCastFrame, 0, sizeof(DISP_CAST_FRAME_S));
    stCastFrame.hCast = hCast;
    Ret = HI_DRV_DISP_Process(CMD_DISP_ACQUIRE_CAST_FRAME, &stCastFrame);
    if(!Ret)
    {
        *pstCastFrame = stCastFrame.stFrame;
    }
    return Ret ;
}

HI_S32 HI_DRV_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    HI_S32 Ret;
    DISP_CAST_FRAME_S stCastFrame;

    stCastFrame.hCast = hCast;
    stCastFrame.stFrame = *pstCastFrame;

    Ret = HI_DRV_DISP_Process_Intr(CMD_DISP_RELEASE_CAST_FRAME, &stCastFrame);

    return Ret ;
}

HI_S32 HI_DRV_DISP_ExternlAttach(HI_HANDLE hCast, HI_HANDLE hSink)
{
    HI_S32 Ret;
    DISP_EXT_ATTACH_S disp_attach_info;

    disp_attach_info.hCast = hCast;
    disp_attach_info.hMutual = hSink;
    disp_attach_info.enType = EXT_ATTACH_TYPE_SINK;

    Ret = HI_DRV_DISP_Process(CMD_DISP_EXT_ATTACH, &disp_attach_info);

    return Ret ;
}

HI_S32 HI_DRV_DISP_SetCastAttr(HI_HANDLE hCast, 
                                HI_U32 u32Width,
                                HI_U32 u32Height,
                                HI_U32 u32FrmRate)
{
    HI_S32 Ret;
    DISP_CAST_EXT_ATTR_S disp_cast_attr;

    disp_cast_attr.hCast = hCast;
    disp_cast_attr.castAttr.s32Width =  u32Width;
    disp_cast_attr.castAttr.s32Height = u32Height;

    Ret = HI_DRV_DISP_Process(CMD_DISP_SET_CASTATTR, &disp_cast_attr);
    return Ret ;
}

HI_S32 HI_DRV_DISP_GetCastAttr(HI_HANDLE hCast, HI_DRV_DISP_Cast_Attr_S *pstCastAttr)
{
    HI_S32 Ret;
    DISP_CAST_EXT_ATTR_S disp_cast_attr;
    memset((void*)&disp_cast_attr, 0, sizeof(DISP_CAST_EXT_ATTR_S));

    disp_cast_attr.hCast = hCast;

    Ret = HI_DRV_DISP_Process(CMD_DISP_GET_CASTATTR, &disp_cast_attr);
    if (HI_FAILURE ==  Ret)
        return  Ret;

    *pstCastAttr = disp_cast_attr.castAttr;
    return Ret ;
}

HI_S32 HI_DRV_DISP_ExternlDetach(HI_HANDLE hCast, HI_HANDLE hSink)
{
    HI_S32 Ret;
    DISP_EXT_ATTACH_S disp_attach_info;

    disp_attach_info.hCast = hCast;
    disp_attach_info.hMutual = hSink;
    disp_attach_info.enType = EXT_ATTACH_TYPE_SINK;

    Ret = HI_DRV_DISP_Process(CMD_DISP_EXT_DEATTACH, &disp_attach_info);

    return Ret ;
}

HI_S32 HI_DRV_DISP_GetInitFlag(HI_BOOL *pbInited)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetInitFlag(pbInited);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetVersion(pstVersion);

    up(&g_DispMutex);

    return Ret;
}

HI_BOOL HI_DRV_DISP_IsOpened(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_IsOpened(enDisp);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetSlave(enDisp, penSlave);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetMaster(enDisp, penMaster);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetDisplayInfo(enDisp, pstInfo);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 HI_DRV_DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCallback)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);
	pstCallback->eCallBackPrior =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    Ret = DISP_RegCallback(enDisp, eType, pstCallback);
    up(&g_DispMutex);
    return Ret;
}

HI_S32 HI_DRV_DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCallback)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);
	pstCallback->eCallBackPrior =  HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    Ret = DISP_UnRegCallback(enDisp, eType, pstCallback);
    up(&g_DispMutex);
    return Ret;
}

HI_S32 DRV_DISP_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 Ret;
	
	HDMI_EXPORT_FUNC_S* pstHDMIFunc = HI_NULL;
    
    Ret = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&pstHDMIFunc);

    if ((Ret != HI_SUCCESS) || (pstHDMIFunc == HI_NULL))
    {
        DISP_ERROR("DISP_get HDMI func failed!");
        return HI_FAILURE;
    }

    pstHDMIFunc->pfnHdmiSuspend(NULL,state);


    Ret = down_trylock(&g_DispMutex);
    if (Ret)
    {
        HI_FATAL_DISP("down g_DispMutex failed.\n");
        return -1;
    }

    /* no process opened the equipment, return directly */
    if (!atomic_read(&g_DispCount))
    {
        up(&g_DispMutex);
        return 0;
    }

    DISP_Suspend();

    msleep(50);

    g_DispSuspend = HI_TRUE;

    HI_PRINT("DISP suspend OK\n");

    up(&g_DispMutex);

    return 0;
}

HI_S32 DRV_DISP_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32  Ret;
    HI_DRV_DISP_INTF_S stIntf;
    HI_DRV_DISP_FMT_E enFormat = HI_DRV_DISP_FMT_BUTT;
    HI_DRV_DISP_STEREO_E eDispMode = DISP_STEREO_BUTT;

    HDMI_EXPORT_FUNC_S* pstHDMIFunc = HI_NULL;

    Ret = down_trylock(&g_DispMutex);
    if (Ret)
    {
        HI_FATAL_DISP("down g_DispMutex failed.\n");
        return -1;
    }

    /* no process opened the equipment, return directly */
    if (!atomic_read(&g_DispCount))
    {
        up(&g_DispMutex);
        return 0;
    }

    DISP_Resume();

    g_DispSuspend = HI_FALSE;

    HI_PRINT("DISP resume OK\n");

    up(&g_DispMutex);

    Ret = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&pstHDMIFunc);

    if ((Ret != HI_SUCCESS) || (pstHDMIFunc == HI_NULL))
    {
        DISP_ERROR("DISP_get HDMI func failed!");
        return HI_FAILURE;
    }
	
    if (Disp_GetFastbootupFlag() == DISP_FASTBOOTUP_FLAG)
    {
        stIntf.eID = HI_DRV_DISP_INTF_HDMI0;

        if (DispCheckIntfExistByType(HI_DRV_DISPLAY_0, &stIntf))
        {
            DISP_GetDisplaySetting(HI_DRV_DISPLAY_0, &enFormat,&eDispMode);
        }
        else if (DispCheckIntfExistByType(HI_DRV_DISPLAY_1, &stIntf))
        {
            DISP_GetDisplaySetting(HI_DRV_DISPLAY_1, &enFormat,&eDispMode);
        }
				
        pstHDMIFunc->pfnHdmiSoftResume(enFormat, eDispMode);		
		Disp_SetFastbootupFlag(0);
    }
    else
    {
        pstHDMIFunc->pfnHdmiResume(NULL);
    }

    
    return 0;
}

HI_S32 DRV_DISP_ProcessCmd(unsigned int cmd, HI_VOID *arg, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{
    HI_S32       Ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DISP_ATTACH:
            {
                DISP_ATTACH_S  *pDispAttach;

                pDispAttach = (DISP_ATTACH_S *)arg;

                Ret = DISP_ExtAttach(pDispAttach->enMaster, pDispAttach->enSlave);

                break;
            }

        case CMD_DISP_DETACH:
            {
                DISP_ATTACH_S  *pDispAttach;

                pDispAttach = (DISP_ATTACH_S *)arg;

                Ret = DISP_ExtDetach(pDispAttach->enMaster, pDispAttach->enSlave);

                break;
            }

        case CMD_DISP_OPEN:
            {
                Ret = DISP_ExtOpen(*((HI_DRV_DISPLAY_E *)arg), pDispState, bUser);

                break;
            }

        case CMD_DISP_CLOSE:
            {
                Ret = DISP_CheckPara(*((HI_DRV_DISPLAY_E *)arg), pDispState);
                if (HI_SUCCESS == Ret)
                {
                    Ret = DISP_ExtClose(*((HI_DRV_DISPLAY_E *)arg), pDispState, bUser);
                }
                else
                {
                    Ret = HI_SUCCESS;
                }

                break;
            }
#if 0
        case CMD_DISP_AttachOsd:
            {
                DISP_OSD_S  *pDispOsd;

                pDispOsd = (DISP_OSD_S *)arg;

                Ret = DISP_AttachOsd(pDispOsd->enDisp, pDispOsd->enLayer);

                break;
            }

        case CMD_DISP_DetachOsd:
            {
                DISP_OSD_S  *pDispOsd;

                pDispOsd = (DISP_OSD_S *)arg;

                Ret = DISP_DetachOsd(pDispOsd->enDisp, pDispOsd->enLayer);

                break;
            }
#endif
        case CMD_DISP_SET_ENABLE:
            {
                DISP_ENABLE_S  *pDispEnable;

                pDispEnable = (DISP_ENABLE_S *)arg;

                Ret = DISP_CheckPara(pDispEnable->enDisp, pDispState);
                if (HI_SUCCESS == Ret)
                {
                    Ret = DISP_ExternSetEnable(pDispEnable->enDisp, pDispEnable->bEnable);
                }

                break;
            }

        case CMD_DISP_GET_ENABLE:
            {
                DISP_ENABLE_S  *pDispEnable;

                pDispEnable = (DISP_ENABLE_S *)arg;

                Ret = DISP_GetEnable(pDispEnable->enDisp, &pDispEnable->bEnable);

                break;
            }

        case CMD_DISP_ADD_INTF:
            {
                DISP_SET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_SET_INTF_S *)arg;

                Ret = DISP_AddIntf(pDispIntf->enDisp, &pDispIntf->stIntf);

                break;
            }
        case CMD_DISP_DEL_INTF:
            {
                DISP_SET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_SET_INTF_S *)arg;
                Ret = DISP_DelIntf(pDispIntf->enDisp, &pDispIntf->stIntf);

                break;

            }

#if 0
        case CMD_DISP_GET_INTF:
            {
                DISP_GET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_GET_INTF_S *)arg;

                Ret = DISP_GetIntf(pDispIntf->enDisp, &(pDispIntf->u32IntfNum), (pDispIntf->stIntf));

                break;
            }
#endif
        case CMD_DISP_SET_FORMAT:
            {
                DISP_FORMAT_S  *pDispFormat;

                pDispFormat = (DISP_FORMAT_S *)arg;

                Ret = DISP_SetFormat(pDispFormat->enDisp, pDispFormat->enStereo, pDispFormat->enFormat);

                break;
            }

        case CMD_DISP_GET_FORMAT:
            {
                DISP_FORMAT_S  *pDispFormat;

                pDispFormat = (DISP_FORMAT_S *)arg;

                Ret = DISP_GetFormat(pDispFormat->enDisp, &pDispFormat->enStereo,&pDispFormat->enFormat);

                break;
            }

        case CMD_DISP_SET_R_E_FIRST:
            {
                DISP_R_EYE_FIRST_S  *pREFirst;

                pREFirst = (DISP_R_EYE_FIRST_S *)arg;

                Ret = DISP_SetRightEyeFirst(pREFirst->enDisp, pREFirst->bREFirst);

                break;
            }

        case CMD_DISP_SET_VIRTSCREEN:
            {
                DISP_VIRTSCREEN_S  pVirtScreen;
                pVirtScreen = *((DISP_VIRTSCREEN_S *)arg);

                Ret = DISP_SetVirtScreen(pVirtScreen.enDisp, pVirtScreen.stVirtScreen);

                break;
            }
        case CMD_DISP_GET_VIRTSCREEN:
            {
                DISP_VIRTSCREEN_S  *pVirtScreen;
                pVirtScreen = (DISP_VIRTSCREEN_S *)arg;

                Ret = DISP_GetVirtScreen(pVirtScreen->enDisp, &pVirtScreen->stVirtScreen);
                break;
            }
        case CMD_DISP_SET_SCREENOFFSET:
            {
                DISP_SCREENOFFSET_S  *pOffset;
                pOffset = (DISP_SCREENOFFSET_S *)arg;

                Ret = DISP_SetScreenOffset(pOffset->enDisp, &pOffset->stScreenOffset);
                break;
            }
        case CMD_DISP_GET_SCREENOFFSET:
            {
                DISP_SCREENOFFSET_S  *pOffset;
                pOffset = (DISP_SCREENOFFSET_S *)arg;

                Ret = DISP_GetScreenOffset(pOffset->enDisp, &pOffset->stScreenOffset);
                break;
            }

        case CMD_DISP_SET_TIMING:
            {
                DISP_TIMING_S  *pDispTiming;

                pDispTiming = (DISP_TIMING_S *)arg;
                Ret = DISP_SetCustomTiming(pDispTiming->enDisp, &pDispTiming->stTimingPara);
                break;
            }
        case CMD_DISP_GET_TIMING:
            {
                DISP_TIMING_S  *pDispTiming;
                pDispTiming = (DISP_TIMING_S *)arg;
                Ret = DISP_GetCustomTiming(pDispTiming->enDisp, &pDispTiming->stTimingPara);
                break;
            }

        case CMD_DISP_SET_ZORDER:
            {
                DISP_ZORDER_S   *pDispZorder;

                pDispZorder = (DISP_ZORDER_S *)arg;

                Ret = DISP_SetLayerZorder(pDispZorder->enDisp, pDispZorder->Layer, pDispZorder->ZFlag);

                break;
            }

        case CMD_DISP_GET_ORDER:
            {
                DISP_ORDER_S    *pDispOrder;

                pDispOrder = (DISP_ORDER_S *)arg;

                Ret = DISP_GetLayerZorder(pDispOrder->enDisp, pDispOrder->Layer, &pDispOrder->Order);

                break;
            }
        case CMD_DISP_SET_DEV_RATIO:
            {
                DISP_ASPECT_RATIO_S *pDispAspectRatio;

                pDispAspectRatio = (DISP_ASPECT_RATIO_S *)arg;

                Ret = DISP_SetAspectRatio(pDispAspectRatio->enDisp, pDispAspectRatio->u32ARHori, pDispAspectRatio->u32ARVert);

                break;
            }
        case CMD_DISP_GET_DEV_RATIO:
            {
                DISP_ASPECT_RATIO_S *pDispAspectRatio;

                pDispAspectRatio = (DISP_ASPECT_RATIO_S *)arg;

                Ret = DISP_GetAspectRatio(pDispAspectRatio->enDisp, &pDispAspectRatio->u32ARHori, &pDispAspectRatio->u32ARVert);
                break;
            }
        case CMD_DISP_SET_BGC:
            {
                DISP_BGC_S  *pDispBgc;

                pDispBgc = (DISP_BGC_S *)arg;

                Ret = DISP_SetBGColor(pDispBgc->enDisp, &pDispBgc->stBgColor);

                break;
            }

        case CMD_DISP_GET_BGC:
            {
                DISP_BGC_S  *pDispBgc;

                pDispBgc = (DISP_BGC_S *)arg;

                Ret = DISP_GetBGColor(pDispBgc->enDisp, &pDispBgc->stBgColor);

                break;
            }
#if 0
        case CMD_DISP_SEND_TTX:
            {
                DISP_TTX_S  *pDispTtx;

                pDispTtx = (DISP_TTX_S *)arg;

                Ret = DISP_SendTtxData(pDispTtx->enDisp, &pDispTtx->TtxData);

                break;
            }
#endif

        case CMD_DISP_CREATE_VBI_CHANNEL:
            {
                DISP_VBI_CREATE_CHANNEL_S  *pstDispVbiCrtChanl;

                pstDispVbiCrtChanl = (DISP_VBI_CREATE_CHANNEL_S *)arg;

                /*if vbi already open , return failed*/
                if (pDispState->hVbi[HI_DRV_DISPLAY_0] == HI_TRUE)
                    return HI_ERR_DISP_INVALID_OPT;

                Ret = DISP_CreateVBIChannel(pstDispVbiCrtChanl->enDisp,  &pstDispVbiCrtChanl->stCfg, &pstDispVbiCrtChanl->hVbi);

				if (HI_SUCCESS == Ret)
				{
					pDispState->hVbi[HI_DRV_DISPLAY_0] = HI_TRUE;	
				}

                break;
            }

        case CMD_DISP_DESTROY_VBI_CHANNEL:
            {
                HI_HANDLE  *pnVbiHandle;

                pnVbiHandle = (HI_HANDLE *)arg;

                if (pDispState->hVbi[HI_DRV_DISPLAY_0] == HI_FALSE)
                    return HI_ERR_DISP_INVALID_OPT;

                Ret = DISP_DestroyVBIChannel(*pnVbiHandle);

				if (HI_SUCCESS == Ret)
				{
					pDispState->hVbi[HI_DRV_DISPLAY_0] = HI_FALSE;	
				}

                break;
            }

        case CMD_DISP_SEND_VBI:
            {
                DISP_VBI_S  *pDispVbi;

                pDispVbi = (DISP_VBI_S *)arg;

                Ret = DISP_SendVbiData(pDispVbi->hVbi, &pDispVbi->stVbiData);

                break;
            }

        case CMD_DISP_SET_WSS:
            {
                DISP_WSS_S  *pDispWss;

                pDispWss = (DISP_WSS_S *)arg;

                Ret = DISP_SetWss(pDispWss->enDisp, &pDispWss->WssData);

                break;
            }

        case CMD_DISP_SET_MCRVSN:
            {
                DISP_MCRVSN_S  *pDispMcrvsn;

                pDispMcrvsn = (DISP_MCRVSN_S *)arg;

                Ret = DISP_SetMacrovision(pDispMcrvsn->enDisp, pDispMcrvsn->eMcrvsn);

                break;
            }

        case CMD_DISP_GET_MCRVSN:
            {
                DISP_MCRVSN_S  *pDispMcrvsn;

                pDispMcrvsn = (DISP_MCRVSN_S *)arg;

                Ret = DISP_GetMacrovision(pDispMcrvsn->enDisp, &pDispMcrvsn->eMcrvsn);

                break;
            }
#if 0
        case CMD_DISP_GET_HDMI_INTF:
            {
                DISP_HDMIINF_S  *pDispHdmiIntf;

                pDispHdmiIntf = (DISP_HDMIINF_S *)arg;

                Ret = DISP_GetHdmiIntf(pDispHdmiIntf->enDisp, &pDispHdmiIntf->HDMIInf);

                break;
            }

        case CMD_DISP_SET_HDMI_INTF:
            {
                DISP_HDMIINF_S  *pDispHdmiIntf;

                pDispHdmiIntf = (DISP_HDMIINF_S *)arg;

                Ret = DISP_SetHdmiIntf(pDispHdmiIntf->enDisp, &pDispHdmiIntf->HDMIInf);

                break;
            }
#endif
        case CMD_DISP_SET_CGMS:
            {
                DISP_CGMS_S  *pDispCgms;

                pDispCgms = (DISP_CGMS_S *)arg;

                Ret = DISP_SetCGMS(pDispCgms->enDisp, &pDispCgms->stCgmsCfg);

                break;
            }
         case CMD_DISP_CREATE_CAST:
            {
                DISP_CAST_CREATE_S *pstC = (DISP_CAST_CREATE_S *)arg;

                /*if cast already open , return failed*/
                if (pDispState->hCastHandle[pstC->enDisp] != HI_NULL)
                    return HI_ERR_DISP_INVALID_OPT;

                //HI_ERR_DISP(">>>>>>>>>>>>>>>> CMD_DISP_CREATE_CAST >>>>>>>>>>>>. \n");
                Ret = DISP_CreateCast(pstC->enDisp, &pstC->stCfg, &pstC->hCast);
                if (!Ret)
                {
                    pDispState->hCastHandle[pstC->enDisp] = pstC->hCast;
                }
                break;
            }
        case CMD_DISP_DESTROY_CAST:
            {
                DISP_CAST_DESTROY_S *pstC = (DISP_CAST_DESTROY_S *)arg;
                HI_S32 i;

                for (i=0; i<HI_DRV_DISPLAY_BUTT; i++)
                {
                    if (pDispState->hCastHandle[i] == pstC->hCast)
                    {
                        Ret = DISP_DestroyCast(pstC->hCast);
                        pDispState->hCastHandle[i] = HI_NULL;
                    }
                }

                break;
            }
        case CMD_DISP_SET_CAST_ENABLE:
            {
                DISP_CAST_ENABLE_S *pstC = (DISP_CAST_ENABLE_S *)arg;

                /*To do*/
                Ret = DISP_SetCastEnable(pstC->hCast, pstC->bEnable);
                break;
            }

        case CMD_DISP_GET_CAST_ENABLE:
            {
                DISP_CAST_ENABLE_S *pstC = (DISP_CAST_ENABLE_S *)arg;

                /*To do*/
                Ret = DISP_GetCastEnable(pstC->hCast, &pstC->bEnable);
                break;
            }

        case CMD_DISP_ACQUIRE_CAST_FRAME:
            {
                DISP_CAST_FRAME_S *pstC = (DISP_CAST_FRAME_S *)arg;

                /*To do*/
                Ret = DISP_AcquireCastFrame(pstC->hCast, &pstC->stFrame);
                break;
            }

        case CMD_DISP_RELEASE_CAST_FRAME:
            {
                DISP_CAST_FRAME_S *pstC = (DISP_CAST_FRAME_S *)arg;

                /*To do*/
                Ret = DISP_ReleaseCastFrame(pstC->hCast, &pstC->stFrame);
                break;
            }

        case CMD_DISP_ACQUIRE_SNAPSHOT:
            {
                DISP_SNAPSHOT_FRAME_S *pstFrame = (DISP_SNAPSHOT_FRAME_S*)arg;
                HI_VOID* snapshotHandleOut = 0;

                /*does not support continuous snapshot.*/
                if (pDispState->hSnapshot[pstFrame->enDispLayer] != 0)
                    break;

                                Ret =  DISP_AcquireSnapshot(pstFrame->enDispLayer,
                                            &pstFrame->stFrame,
                                            &snapshotHandleOut);
                if (!Ret)
                    pDispState->hSnapshot[pstFrame->enDispLayer] = snapshotHandleOut;
                else
                    pDispState->hSnapshot[pstFrame->enDispLayer] = 0;

                break;
            }
        case CMD_DISP_RELEASE_SNAPSHOT:
        {
                DISP_SNAPSHOT_FRAME_S *pstFrame = (DISP_SNAPSHOT_FRAME_S*)arg;

                /*for released snapshot, just break.*/
                if (pDispState->hSnapshot[pstFrame->enDispLayer] == 0)
                    break;

                Ret = DISP_ReleaseSnapshot(pstFrame->enDispLayer, &pstFrame->stFrame, pDispState->hSnapshot[pstFrame->enDispLayer]);
                pDispState->hSnapshot[pstFrame->enDispLayer] = 0;
                break;
        }

        case CMD_DISP_EXT_ATTACH:
            {
                DISP_EXT_ATTACH_S *pstC = (DISP_EXT_ATTACH_S *)arg;

                /*To do*/
                Ret = DISP_External_Attach(pstC->hCast, pstC->hMutual);
                break;
            }
        case CMD_DISP_EXT_DEATTACH:
            {
                DISP_EXT_ATTACH_S *pstC = (DISP_EXT_ATTACH_S *)arg;

                /*To do*/
                Ret = DISP_External_DeAttach(pstC->hCast, pstC->hMutual);
                break;
            }
        case CMD_DISP_SET_CASTATTR:
            {
                DISP_CAST_EXT_ATTR_S *pstC = (DISP_CAST_EXT_ATTR_S *)arg;

                /*To do*/
                Ret = DRV_DISP_SetCastAttr(pstC->hCast, &pstC->castAttr);
                break;
            }
        case CMD_DISP_GET_CASTATTR:
            {
                DISP_CAST_EXT_ATTR_S *pstC = (DISP_CAST_EXT_ATTR_S *)arg;

                /*To do*/
                Ret = DRV_DISP_GetCastAttr(pstC->hCast, &pstC->castAttr);
                break;
            }
        case CMD_DISP_SUSPEND:
            {
                /*To do*/
                Ret = DISP_Suspend();
                break;
            }
        case CMD_DISP_RESUME:
            {
                /*To do*/
                Ret = DISP_Resume();
                break;
            }
        case CMD_DISP_FORCESET_DAC_ENABLE:
            {
                HI_BOOL bDacEnable = *(HI_BOOL*)arg;
                if (bDacEnable == HI_TRUE)
                {
                    Ret  = DISP_SetAllDacEn(HI_TRUE);
                    Ret |= DISP_SetDACDetEn(HI_FALSE);
                }
                else
                {
                    Ret = DISP_SetDACDetEn(HI_TRUE);
                }
                break;
            }
        default:
            // ????
            //up(&g_DispMutex);
            return -ENOIOCTLCMD;
    }

    return Ret;
}


HI_S32 DRV_DISP_Init2(HI_VOID)
{
    atomic_set(&g_DispCount, 1);

    DRV_DISP_ProcInit();

    DRV_DISP_ProcAdd(HI_DRV_DISPLAY_0);
    DRV_DISP_ProcAdd(HI_DRV_DISPLAY_1);

    (HI_VOID)DISP_Init();

    HI_DRV_DISP_Open(HI_DRV_DISPLAY_0);
    HI_DRV_DISP_Open(HI_DRV_DISPLAY_1);


    return HI_SUCCESS;
}

HI_S32 DRV_DISP_DeInit2(HI_VOID)
{
    DRV_DISP_ProcDel(HI_DRV_DISPLAY_1);
    DRV_DISP_ProcDel(HI_DRV_DISPLAY_0);

    DRV_DISP_ProcDeInit();

    HI_DRV_DISP_Close(HI_DRV_DISPLAY_0);
    HI_DRV_DISP_Close(HI_DRV_DISPLAY_1);

    /* closing clock */
    DISP_DeInit();

    atomic_set(&g_DispCount, 0);

    return HI_SUCCESS;
}

//may be delete
HI_S32 HI_DRV_DISP_Init(HI_VOID)
{
    HI_S32          Ret;

    Ret = down_interruptible(&g_DispMutex);

    if (1 == atomic_inc_return(&g_DispCount))
    {
        /* for configuration such as start clock, re-use pins, etc */
        Ret = DISP_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_DISP("call DISP_Init failed.\n");
            atomic_dec(&g_DispCount);
            up(&g_DispMutex);
            return -1;
        }
    }

    up(&g_DispMutex);
        Ret = DRV_DISP_Register();
    return Ret;
}

//HI_S32 HI_DRV_DISP_ModDeinit(HI_VOID)
HI_S32 HI_DRV_DISP_DeInit(HI_VOID)
{
    HI_S32 Ret;
    HI_DRV_DISPLAY_E u;

    Ret = down_interruptible(&g_DispMutex);

    //HI_INFO_DISP("come to close HD/SD g_DispModState.bDisp1Open:%d, g_DispModState.bDisp0Open:%d\n",
    //        g_DispModState.bDisp1Open, g_DispModState.bDisp0Open);

    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        if (g_DispModState.bDispOpen[u])
        {
            HI_INFO_DISP("DISP_MOD_ExtClose HD0\n");
            Ret = DISP_ExtClose(u, &g_DispModState, HI_FALSE);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_DISP("DISP_MOD_ExtClose Display %d failed!\n", u);
            }
        }
    }

    HI_INFO_DISP("HI_DRV_DISP_Deinit:atomic g_DispCount:%d\n", atomic_read(&g_DispCount));

    if (atomic_dec_and_test(&g_DispCount))
    {
        HI_INFO_DISP("close clock\n");

        /* closing clock */
        DISP_DeInit();
    }

    up(&g_DispMutex);
    return 0;
}

static DISP_EXPORT_FUNC_S s_stDispExportFuncs = {
    .pfnDispInit             = HI_DRV_DISP_Init            ,
    .pfnDispDeInit           = HI_DRV_DISP_DeInit          ,
    .pfnDispAttach           = HI_DRV_DISP_Attach          ,
    .pfnDispDetach           = HI_DRV_DISP_Detach          ,
    .pfnDispSetFormat        = HI_DRV_DISP_SetFormat       ,
    .pfnDispGetFormat        = HI_DRV_DISP_GetFormat       ,
    .pfnDispSetCustomTiming  = HI_DRV_DISP_SetCustomTiming ,
    .pfnDispGetCustomTiming  = HI_DRV_DISP_GetCustomTiming ,
    .pfnDispAddIntf          = HI_DRV_DISP_AddIntf         ,
    .pfnDispDeIntf           = HI_DRV_DISP_DelIntf         ,

    .pfnDispOpen             = HI_DRV_DISP_Open            ,
    .pfnDispClose            = HI_DRV_DISP_Close           ,
    .pfnDispSetEnable        = HI_DRV_DISP_SetEnable       ,
    .pfnDispGetEnable        = HI_DRV_DISP_GetEnable       ,
    .pfnDispSetRightEyeFirst = HI_DRV_DISP_SetRightEyeFirst,
    .pfnDispSetBgColor       = HI_DRV_DISP_SetBgColor      ,
    .pfnDispGetBgColor       = HI_DRV_DISP_GetBgColor      ,
    .pfnDispSetAspectRatio   = HI_DRV_DISP_SetAspectRatio  ,
    .pfnDispGetAspectRatio   = HI_DRV_DISP_GetAspectRatio  ,
    .pfnDispSetLayerZorder   = HI_DRV_DISP_SetLayerZorder  ,
    .pfnDispGetLayerZorder   = HI_DRV_DISP_GetLayerZorder  ,

    .pfnDispCreatCast        = HI_DRV_DISP_CreateCast      ,
    .pfnDispDestoryCast      = HI_DRV_DISP_DestroyCast     ,
    .pfnDispSetCastEnable    = HI_DRV_DISP_SetCastEnable   ,
    .pfnDispGetCastEnable    = HI_DRV_DISP_GetCastEnable   ,
    .pfnDispAcquireCastFrm   = HI_DRV_DISP_AcquireCastFrame,
    .pfnDispRlsCastFrm       = HI_DRV_DISP_ReleaseCastFrame,

    .pfnDispExtAttach        = HI_DRV_DISP_ExternlAttach,
    .pfnDispExtDeAttach      = HI_DRV_DISP_ExternlDetach,
    .pfnDispSetCastAttr      = HI_DRV_DISP_SetCastAttr,
    .pfnDispGetCastAttr      = HI_DRV_DISP_GetCastAttr,

    .pfnDispGetInitFlag      = HI_DRV_DISP_GetInitFlag     ,
    .pfnDispGetVersion       = HI_DRV_DISP_GetVersion      ,
    .pfnDispIsOpen           = HI_DRV_DISP_IsOpened        ,
    .pfnDispGetSlave         = HI_DRV_DISP_GetSlave        ,
    .pfnDispGetMaster        = HI_DRV_DISP_GetMaster       ,
    .pfnDispGetDispInfo      = HI_DRV_DISP_GetDisplayInfo  ,

    .pfnDispIoctl            = HI_DRV_DISP_Process,
    .pfnDispRegCallback      = HI_DRV_DISP_RegCallback,
    .pfnDispUnRegCallback    = HI_DRV_DISP_UnRegCallback,
    .pfnDispSuspend = DRV_DISP_Suspend,
    .pfnDispResume = DRV_DISP_Resume,
};

HI_S32 DRV_DISP_Register(HI_VOID)
{
    HI_S32 Ret;

    // add for multiple process
    DISP_ResetCountStatus();

    //DRV_DISP_ProcInit();

    Ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_DISP, "HI_DISP", (HI_VOID *)(&s_stDispExportFuncs));
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_DISP("HI_DRV_MODULE_Register DISP failed\n");
        return Ret;
    }

    return  0;
}

HI_VOID DRV_DISP_UnRegister(HI_VOID)
{

    HI_DRV_MODULE_UnRegister((HI_U32)HI_ID_DISP);

    //DRV_DISP_ProcDeInit();

    return;
}


/* ======================================================================= */
#if 0
HI_S32 DRV_DISP_SetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Brightness)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Brightness;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_BRIGHT,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Contrast)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Contrast;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_CONTRAST,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Saturation)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Saturation;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_SATURATION,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 u32HuePlus)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32HuePlus;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_HUE,  &enDispCsc);
    return Ret;
}


HI_S32 DRV_DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode, HI_VOID *pData)
{
    HI_S32 Ret;
    DISP_MCRVSN_S  enDispMcrvsn;

    enDispMcrvsn.enDisp = enDisp;
    enDispMcrvsn.eMcrvsn = enMode;
    Ret = DRV_DISP_Process(CMD_DISP_SET_MCRVSN,  &enDispMcrvsn);
    return Ret;
}
#endif


EXPORT_SYMBOL(HI_DRV_DISP_SetFormat);

#if 0
//#ifndef MODULE

/* for intf */
EXPORT_SYMBOL(DRV_DISP_Open);
EXPORT_SYMBOL(DRV_DISP_Close);
EXPORT_SYMBOL(DRV_DISP_Attach);
EXPORT_SYMBOL(DRV_DISP_Detach);
EXPORT_SYMBOL(DRV_DISP_AddIntf);
EXPORT_SYMBOL(DRV_DISP_SetFormat);
EXPORT_SYMBOL(DRV_DISP_SetTiming);
//EXPORT_SYMBOL(DRV_DISP_SetHuePlus);
//EXPORT_SYMBOL(DRV_DISP_SetSaturation);
//EXPORT_SYMBOL(DRV_DISP_SetContrast);
//EXPORT_SYMBOL(DRV_DISP_SetBrightness);

EXPORT_SYMBOL(DRV_DISP_SetBgColor);
//EXPORT_SYMBOL(DRV_DISP_SetMacrovision);

EXPORT_SYMBOL(DRV_DISP_Suspend);
EXPORT_SYMBOL(DRV_DISP_Resume);

EXPORT_SYMBOL(DRV_DISP_Register);
EXPORT_SYMBOL(DRV_DISP_UnRegister);

EXPORT_SYMBOL(DRV_DISP_Ioctl);

EXPORT_SYMBOL(DRV_DISP_Init);
EXPORT_SYMBOL(DRV_DISP_DeInit);

EXPORT_SYMBOL(DRV_DISP_ProcRegister);
EXPORT_SYMBOL(DRV_DISP_ProcUnRegister);

#endif


#define FPGA_TEST_VDP
#ifndef HI_ADVCA_FUNCTION_RELEASE
#ifdef FPGA_TEST_VDP

char file_name[100];
struct file *pfYUV;
HI_U8   *framebuf;
HI_U32  framebuf_phy;
DISP_MMZ_BUF_S stMBuf;
HI_HANDLE hWin;
HI_DRV_VIDEO_FRAME_S stFrame[5];

HI_S32 fpga_test_disp_Init(char *filenametmp)
{
    HI_S32    Ret = HI_SUCCESS;
    HI_DRV_DISP_INTF_S stIntf[2];

    memcpy(file_name,filenametmp, strlen(filenametmp) + 1);

    HI_DRV_DISP_Init();

    HI_DRV_DISP_Attach(HI_DRV_DISPLAY_1, HI_DRV_DISPLAY_0);

    Ret = HI_DRV_DISP_SetFormat(HI_DRV_DISPLAY_1,HI_DRV_DISP_FMT_720P_50);
    Ret |= HI_DRV_DISP_SetFormat(HI_DRV_DISPLAY_0,HI_DRV_DISP_FMT_PAL);
    if (Ret)
    {
        HI_ERR_DISP("set format interface failed!\n");
        return Ret;
    }


    Ret = HI_DRV_DISP_Open(HI_DRV_DISPLAY_1);
    Ret |= HI_DRV_DISP_Open(HI_DRV_DISPLAY_0);


    stIntf[0].eID = HI_DRV_DISP_INTF_YPBPR0;
    stIntf[0].u8VDAC_Y_G =  1;
    stIntf[0].u8VDAC_Pb_B = 2;
    stIntf[0].u8VDAC_Pr_R = 3;

    stIntf[1].eID = HI_DRV_DISP_INTF_CVBS0;
    stIntf[1].u8VDAC_Y_G =  0;
    stIntf[1].u8VDAC_Pb_B =  HI_DISP_VDAC_INVALID_ID;
    stIntf[1].u8VDAC_Pr_R =  HI_DISP_VDAC_INVALID_ID;


    Ret = HI_DRV_DISP_AddIntf(HI_DRV_DISPLAY_1, &stIntf[0]);
    Ret |= HI_DRV_DISP_AddIntf(HI_DRV_DISPLAY_0, &stIntf[1]);
    if (Ret)
    {
        HI_ERR_DISP("add interface failed!\n");
        return Ret;
    }

    Ret = HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_1, 1);
    Ret |= HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_0, 1);
    if (Ret)
    {
        HI_ERR_DISP("set format enable failed!\n");
        return Ret;
    }


    Ret = DISP_OS_MMZ_AllocAndMap((const char *)"fpga_test_vdp", HI_NULL, 0x2000000, 16, &stMBuf);
    if (Ret)
    {
        HI_ERR_DISP("allocate mem failed!\n");
        return Ret;
    }

    framebuf  = (HI_U8 *)stMBuf.pu8StartVirAddr;
    framebuf_phy = stMBuf.u32StartPhyAddr;

    return Ret;
}

HI_S32 fpga_test_disp_deInit(HI_VOID)
{
    HI_DRV_DISP_INTF_S stIntf[2];

    HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_1, 0);
    HI_DRV_DISP_SetEnable(HI_DRV_DISPLAY_0, 0);

    stIntf[0].eID = HI_DRV_DISP_INTF_YPBPR0;
    stIntf[0].u8VDAC_Y_G =  0;
    stIntf[0].u8VDAC_Pb_B = 1;
    stIntf[0].u8VDAC_Pr_R = 3;

    stIntf[1].eID = HI_DRV_DISP_INTF_CVBS0;
    stIntf[1].u8VDAC_Y_G =  2;
    stIntf[1].u8VDAC_Pb_B =  HI_DISP_VDAC_INVALID_ID;
    stIntf[1].u8VDAC_Pr_R =  HI_DISP_VDAC_INVALID_ID;

    HI_DRV_DISP_DelIntf(HI_DRV_DISPLAY_1, &stIntf[0]);
    HI_DRV_DISP_DelIntf(HI_DRV_DISPLAY_0, &stIntf[1]);


    /*close the */
    HI_DRV_DISP_Close(HI_DRV_DISPLAY_0);
    HI_DRV_DISP_Close(HI_DRV_DISPLAY_1);

    HI_DRV_DISP_DeInit();
    DISP_OS_MMZ_UnmapAndRelease(&stMBuf);

    return HI_SUCCESS;
}

HI_S32 fpga_test_vo_createwin(HI_VOID)
{
    HI_S32                  Ret;
    HI_DRV_WIN_ATTR_S    WinAttr;
    HI_DRV_WIN_INFO_S stInfo;
    HI_DRV_WIN_SRC_HANDLE_S stSrc;

    /*if not compiled into kernel,so open is needed here.*/
    HI_DRV_WIN_Init();

    memset(&WinAttr, 0x0, sizeof(HI_DRV_WIN_ATTR_S));
    WinAttr.enDisp = HI_DRV_DISPLAY_1;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.stCustmAR.u32ARh = 0;
    WinAttr.stCustmAR.u32ARw = 0;
    WinAttr.enARCvrs = HI_DRV_ASP_RAT_MODE_FULL;
    WinAttr.stInRect.s32X = 0;
    WinAttr.stInRect.s32Y = 0;
    WinAttr.stInRect.s32Width = 0;
    WinAttr.stInRect.s32Height = 0;

    Ret = HI_DRV_WIN_Create(&WinAttr, &hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_Create\n");
        return Ret;
    }

    Ret = HI_DRV_WIN_GetInfo(hWin, &stInfo);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_GetInfo\n");
        return Ret;
    }

    Ret = HI_DRV_WIN_SetEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_SetEnable\n");
        return Ret;
    }

    Ret = HI_DRV_WIN_SetEnable(stInfo.hSec, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_SetEnable\n");
        return Ret;
    }
    
    stSrc.hSrc = HI_INVALID_HANDLE;
    stSrc.enSrcMode = HI_ID_BUTT;
    stSrc.hSecondSrc = HI_INVALID_HANDLE;
    Ret = HI_DRV_WIN_SetSource(hWin, &stSrc);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_SetSource\n");
        return Ret;
    }

    return Ret;
}

HI_S32 fpga_test_vo_destroywin(HI_VOID)
{
    HI_S32                  Ret;

    vdp_k_fclose(pfYUV);

    Ret = HI_DRV_WIN_SetEnable(hWin, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_SetEnable\n");
        return Ret;
    }

    HI_DRV_WIN_Destroy(hWin);
    HI_DRV_WIN_DeInit();

    return HI_SUCCESS;
}

HI_S32 fpga_test_queue_frame(HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32 s32Ret = 0;
    HI_DRV_WIN_INFO_S stInfo;

    s32Ret = HI_DRV_WIN_GetInfo(hWin, &stInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_WIN("ERR: HI_DRV_WIN_GetInfo\n");
        return s32Ret;
    }

    s32Ret = HI_DRV_WIN_QFrame(hWin, pFrame);
    s32Ret |= HI_DRV_WIN_QFrame(stInfo.hSec, pFrame);
    if (s32Ret)
    {
        HI_ERR_WIN("we queue frame failed, line:%d!\n!", __LINE__);
        return  HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32  fpga_openfile_queue_frame(HI_U32 u32Width,
                          HI_U32 u32Height,
                          HI_U32 u32Stride)
{


    HI_U32  u32Length = 0;
    HI_S32 ret = 0;
    HI_U32  i = 0;
    HI_DRV_VIDEO_PRIVATE_S *pstPrivInfo = HI_NULL;


    pfYUV = vdp_k_fopen("/tmp/stream/720_576_sm420.yuv", O_RDWR, 0);
    if (!pfYUV)
    {
        HI_ERR_WIN("open file '%s' fail!\n", file_name);
        return HI_FAILURE;
    }

    u32Length = (u32Stride * u32Height * 3) /2;

    ret = vdp_k_fread(framebuf,u32Length * 5, pfYUV);
    if (ret < u32Length)
    {
        HI_ERR_WIN("read file '%s' fail:%d, %x!\n", file_name, ret, u32Length);
        return HI_FAILURE;
    }

    for (i = 0; i < ret/u32Length; i ++)
    {
        stFrame[i].u32FrameIndex = i;
        stFrame[i].u32TunnelPhyAddr = 0;
        stFrame[i].hTunnelSrc = 0;
        stFrame[i].u32Width = u32Width;
        stFrame[i].u32Height = u32Height;
        stFrame[i].u32SrcPts = 0;
        stFrame[i].u32Pts = 0;
        stFrame[i].s64OmxPts = 0;
        stFrame[i].u32AspectWidth = 16;
        stFrame[i].u32AspectHeight = 9;
        stFrame[i].u32FrameRate = 25;
        stFrame[i].ePixFormat = HI_DRV_PIX_FMT_NV21;
        stFrame[i].bProgressive = 1;
        stFrame[i].enFieldMode = HI_DRV_FIELD_ALL;
        stFrame[i].bTopFieldFirst = 1;
        stFrame[i].bCompressd = 0;
        stFrame[i].enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;

        stFrame[i].stDispRect.s32X = 0;
        stFrame[i].stDispRect.s32Y = 0;
        stFrame[i].stDispRect.s32Width = u32Width;
        stFrame[i].stDispRect.s32Height = u32Height;

        stFrame[i].eFrmType = HI_DRV_FT_NOT_STEREO;
        stFrame[i].u32Circumrotate = 0;
        stFrame[i].bToFlip_H = 0;
        stFrame[i].bToFlip_V = 0;

        stFrame[i].bIsFirstIFrame = 0;
        stFrame[i].bStillFrame = 0;
        stFrame[i].enTBAdjust = HI_DRV_VIDEO_TB_PLAY;
        stFrame[i].stLbxInfo.s32X = 0;
        stFrame[i].stLbxInfo.s32Y = 0;
        stFrame[i].stLbxInfo.s32Width = u32Width;
        stFrame[i].stLbxInfo.s32Height = u32Height;

        stFrame[i].stBufAddr[0].u32PhyAddr_Y = framebuf_phy +  i *(u32Stride * u32Height*3)/2;
        stFrame[i].stBufAddr[0].u32Stride_Y  = u32Stride;

        stFrame[i].stBufAddr[0].u32PhyAddr_C = framebuf_phy +  i *(u32Stride * u32Height*3)/2 + (u32Stride * u32Height);
        stFrame[i].stBufAddr[0].u32Stride_C = u32Stride;

        stFrame[i].stBufAddr[0].u32PhyAddr_Cr = framebuf_phy +  i *(u32Stride * u32Height*3)/2 + (u32Stride * u32Height);
        stFrame[i].stBufAddr[0].u32Stride_Cr  = u32Stride;

        pstPrivInfo = (HI_DRV_VIDEO_PRIVATE_S *)&stFrame[i].u32Priv[0];
        pstPrivInfo->bValid = HI_TRUE;
        pstPrivInfo->eColorSpace = HI_DRV_CS_BT709_YUV_FULL;
        pstPrivInfo->u32PlayTime = 1;
        pstPrivInfo->u32Fidelity = 0;
        pstPrivInfo->eOriginField = HI_DRV_FIELD_ALL;
        pstPrivInfo->ePictureMode = HI_DRV_PICTURE_FRAME;
        pstPrivInfo->stVideoOriginalInfo.u32Width = u32Width;
        pstPrivInfo->stVideoOriginalInfo.u32Height = u32Height;
        fpga_test_queue_frame(&stFrame[i]);

    }

    return HI_SUCCESS;
}

#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
