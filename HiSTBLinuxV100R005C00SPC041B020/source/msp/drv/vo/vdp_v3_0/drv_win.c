/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_win.c
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
#include "drv_display.h"
#include "drv_disp_debug.h"
#include "drv_disp_ext.h"
#include "drv_disp_osal.h"
#include "drv_disp_debug.h"

#include "hi_drv_win.h"
#include "drv_win_ext.h"
#include "drv_vdec_ext.h"
#include "drv_vi_ext.h"
#include "drv_win_ioctl.h"
#include "drv_window.h"
#include "hi_osal.h"
#include "drv_win_hal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

static atomic_t    g_VoCount = ATOMIC_INIT(0);
//WIN_GLOBAL_STATE_S   g_VoGlobalState;
WIN_STATE_S          g_VoModState;
HI_BOOL     g_VoSuspend = HI_FALSE;
HI_U32  g_bWinPrivateInformation = HI_FALSE;


HI_DECLARE_MUTEX(g_VoMutex);

HI_U8 *g_pAlgLocation[ALG_LOCATION_IN_BUTT] = {
    "In V0",
    "In VP",
};

HI_U8 *g_pWinTrueString[2] = {
    "False",
    "True ",
};

HI_U8 *g_pWinYNString[2] = {
    "N",
    "Y",
};

HI_U8 *g_pWinEnableString[2] = {
    "Disable",
    "Enable ",
};
HI_U8 *g_pWinRotateString[5] = {
    "Rotation_00",
    "Rotation_90 ",
    "Rotation_180 ",
    "Rotation_270 ",
    "Rotation_butt ",
};
//WIN_PROC_INFO_S s_stProcInfo;
//static HI_U32 s_WinProcId[HI_DRV_DISPLAY_BUTT][WINDOW_MAX_NUMBER]={WINDOW_INVALID_ID};
HI_U8 *g_pWinStateString[6] = {
    "Run        ",
    "Pause      ",
    "Resume     ",
    "FreezeLast ",
    "FreezeBlack",
};

HI_U8 *g_pWinDispModeString[DISP_STEREO_BUTT] = {
    "2D",
    "FPK",
    "SBS_HALF",
    "TAB",
    "FILED_ALTE",
    "LINE_ALTE",
    "SBS_FULL",
    "L_DEPTH",
    "LDEP_GDEP",
};

HI_U8 *g_pWinQucikString[2] = {
    " ",
    "+QuickMode",
};

HI_U8 *g_pWinStepString[2] = {
    " ",
    "+StepMode",
};


HI_U8 *g_pWinFreezeString[2] = {"LAST", "BLACK"};

HI_U8 *g_pWinTpyeString[HI_DRV_WIN_BUTT] = {
    "Display",
    "Virtual",
    "Main   ",
    "Slave  ",
};

HI_U8 *g_pWinAspectCvrsString[HI_DRV_ASP_RAT_MODE_BUTT] = {
    "Full      ",
    "LetterBox ",
    "PanAndScan",
    "Combined  ",
    "FullHori  ",
    "FullVert  ",
    "Customer  "
};

HI_U8 *g_pWinFrameTypetring[HI_DRV_FT_BUTT] = {
    "2D",
    "SideBySid",
    "TopAndBottom",
    "MVC",
};

HI_U8 *g_pWinFieldModeString[HI_DRV_FIELD_BUTT] = {
    "Top",
    "Bottom",
    "All",
};

HI_U8 *g_pWinMemTypeString[] = {
    "WinSupply",
    "UserSupply",
};

HI_U8 *g_pWinFieldTypeString[HI_DRV_FIELD_BUTT+1] = {
    "Top",
    "Bottom",
    "Frame",
    "BUTT",
};

HI_U8 *g_pPixFmtString[] = {
    "NV12       ",
    "NV21       ",
    "NV12CMP    ",
    "NV21CMP    ",
    "NV12TILE   ",
    "NV21TILE   ",
    "NV12TILECMP",
    "NV21TILECMP",
    "YUYV       ",
    "YYUV       ",
    "YVYU       ",
    "UYVY       ",
    "VYUY       ",
    "           ",
};

HI_U8 g_pPixFmtUnknown[20] = {0};


HI_U8 *g_pSourceString[] = {
    "Butt",
    "AVPlay ",
    "VI     ",
};

HI_U8 *DrvWinGetPixfmtString(HI_DRV_PIX_FORMAT_E enPixfmt)
{
    switch(enPixfmt)
    {
        case HI_DRV_PIX_FMT_NV12:
            return g_pPixFmtString[0];
        case HI_DRV_PIX_FMT_NV21:
            return g_pPixFmtString[1];
        case HI_DRV_PIX_FMT_NV12_CMP:
            return g_pPixFmtString[2];
        case HI_DRV_PIX_FMT_NV21_CMP:
            return g_pPixFmtString[3];
        case HI_DRV_PIX_FMT_NV12_TILE:
            return g_pPixFmtString[4];
        case HI_DRV_PIX_FMT_NV21_TILE:
            return g_pPixFmtString[5];
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
            return g_pPixFmtString[6];
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            return g_pPixFmtString[7];
        case HI_DRV_PIX_FMT_YUYV:
            return g_pPixFmtString[8];
        case HI_DRV_PIX_FMT_YYUV:
            return g_pPixFmtString[9];
        case HI_DRV_PIX_FMT_YVYU:
            return g_pPixFmtString[10];
        case HI_DRV_PIX_FMT_UYVY:
            return g_pPixFmtString[11];
        case HI_DRV_PIX_FMT_VYUY:
            return g_pPixFmtString[12];
        default:
            HI_OSAL_Snprintf(g_pPixFmtUnknown, 10, "%d", enPixfmt);
            return g_pPixFmtUnknown;
    }

}

HI_S32 WIN_ProcParsePara(HI_CHAR *pProcPara,HI_CHAR **ppItem,HI_CHAR **ppValue)
{
    HI_CHAR *pChar = HI_NULL;
    HI_CHAR *pItem,*pValue;

    pChar = strchr(pProcPara,'=');
    if (HI_NULL == pChar)
    {
        return HI_FAILURE; /* Not Found '=' */
    }

    pItem = pProcPara;
    pValue = pChar + 1;
    *pChar = '\0';

    /* remove blank bytes from item tail */
    pChar = pItem;
    while(*pChar != ' ' && *pChar != '\0')
    {
        pChar++;
    }
    *pChar = '\0';

    /* remove blank bytes from value head */
    while(*pValue == ' ')
    {
        pValue++;
    }

    *ppItem = pItem;
    *ppValue = pValue;

    return HI_SUCCESS;
}

extern HI_U8 *g_pVDPColorSpaceString[HI_DRV_CS_BUTT];
HI_S32 DRV_WIN_ProcRead(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *pProcItem;
    WINBUF_STATE_S *pstBuffer;
    HI_DRV_VIDEO_FRAME_S *pstNewFrame;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    WIN_PROC_INFO_S *pstProcInfo;
    HI_HANDLE hWin;
    HI_S32 nRet;
    HI_U32 i;
    HI_U8 *pu8WinSate;

    nRet = down_interruptible(&g_VoMutex);
    pProcItem = p->private;

    hWin = (HI_HANDLE)pProcItem->data;

    pstProcInfo = (WIN_PROC_INFO_S *)DISP_MALLOC(sizeof(WIN_PROC_INFO_S));
    if (!pstProcInfo)
    {
        PROC_PRINT(p,"-------- Malloc Proc Buffer Failed!--------\n");
        goto _ERR_EXIT_;
    }

    nRet = WinGetProcInfo(hWin, pstProcInfo);
    if (nRet)
    {
        HI_ERR_WIN("WinGetProcInfo FAILED!\n");
        goto _ERR_EXIT_;
    }

    PROC_PRINT(p,"------------------------------Win%04x[Z=%d]------------------------------------\n"
                      "------------Win Info-------------------|-------------Frame Info-------------------\n",
                        (HI_U32)(pstProcInfo->u32Index & 0xffffUL),
                        pstProcInfo->u32Zorder);

    switch(pstProcInfo->u32WinState)
    {
        case 1:
            pu8WinSate = g_pWinStateString[1];
            break;
        case 2:
            pu8WinSate = g_pWinStateString[2];
            break;
        case 3:
            if (pstProcInfo->enFreezeMode == HI_DRV_WIN_SWITCH_LAST)
            {
                pu8WinSate = g_pWinStateString[3];
            }
            else
            {
                pu8WinSate = g_pWinStateString[4];
            }
            break;
        default:
            pu8WinSate = g_pWinStateString[0];
            break;
    }

#if 0
    nRet = WinGetProcIndex(pstProcInfo->hSlvWin, &u32SlvWinIndex);
    if (nRet != HI_SUCCESS )
    {
        u32SlvWinIndex = 0xFFFFFFFFul;
    }
#endif

    /*
    ------------------------------Win0100 [Z=1]-------------------------------------
    ------------Win Info-----------------|-------------Frame Info-------------------
    Enable              :TRUE            |Type/PixFmt         :NotStereo/NV12
    State               :Working         |Circurotate         :FALSE
    Type                :Main            |W/H(WvsH)           :1920/1080(16:9)
    LayerID             :0               |Disp(X/Y/W/H)       :0/0/1920/1080
    AspectRatioConvert  :Full            |FrameRate           :50
    CustAspectRatio     :HI_FALSE        |ColorSpace          :BT709_YUV_LIMITED
    Crop                :Enable          |Fieldmode(Origin)   :Frame(Bottom)
    Crop(L/T/R/B)       :0/0/0/0         |OriRect(X/Y/W/H)    :0/0/720/576
    In  (X/Y/W/H)       :0/0/0/0         |FrameIndex          :0x15eb5
    Out (X/Y/W/H)       :0/0/1280/720    |SrcPTS/PTS          :0x31a7345/0x31a7359
    DispMode/RightFirst :2D/HI_FALSE     |PlayTime            :1
    Masked              :FALSE           |FieldMode           :All
    AttachSource        :AVPLAY0         |Fidelity            :0
    CallBack            :Acquire(N)      |Y/CAddr             :0x23719000/0x23913400
                        :Release(Y)      |Y/CStride           :0x780/0x780
                        :SetAttr(Y)      |
    SlaveID             :win0000         |
    ----VirtualWindowInfo-----           |
    MemoryType          :NA              |
    BufferNum           :0               |
    PixelFormat         :NA              |
    */

    pstNewFrame = &pstProcInfo->stBufState.stCurrentFrame;
    pstPriv     = (HI_DRV_VIDEO_PRIVATE_S *)(&pstNewFrame->u32Priv[0]);
    PROC_PRINT(p, "%-19s:%-20s", "Enable", g_pWinTrueString[pstProcInfo->bEnable]);
    PROC_PRINT(p, "%-19s:%-10s/%-9s\n", "|Type/PixFmt", g_pWinFrameTypetring[pstNewFrame->eFrmType],
                                                            DrvWinGetPixfmtString(pstNewFrame->ePixFormat));
    PROC_PRINT(p, "%-19s:%-20s", "State", pu8WinSate);
    PROC_PRINT(p, "%-19s:%-20s\n", "|Rotation", g_pWinRotateString[pstNewFrame->u32Circumrotate > 0]);
    PROC_PRINT(p, "%-19s:%-20s", "Type", g_pWinTpyeString[pstProcInfo->enType]);
    PROC_PRINT(p, "%-19s:%4d/%4d(%4d:%4d)\n", "|W/H(Aspect W:H)", pstNewFrame->u32Width, pstNewFrame->u32Height,
                                                              (HI_U32)pstNewFrame->u32AspectWidth,
                                                              (HI_U32)pstNewFrame->u32AspectHeight);
    PROC_PRINT(p, "%-19s:%-20X", "*LayerID", pstProcInfo->u32LayerId);
	
    PROC_PRINT(p, "%-19s:%4d/%4d/%4d/%4d \n", "|Disp(X/Y/W/H)",
                                                    pstNewFrame->stDispRect.s32X,
                                                    pstNewFrame->stDispRect.s32Y,
                                                    pstNewFrame->stDispRect.s32Width,
                                                    pstNewFrame->stDispRect.s32Height);
    PROC_PRINT(p, "%-19s:%-20s", "AspectRatioConvert", g_pWinAspectCvrsString[pstProcInfo->stAttr.enARCvrs]);
    PROC_PRINT(p, "%-19s:%d.%d\n", "|FrameRate", pstNewFrame->u32FrameRate/100,
                                        pstNewFrame->u32FrameRate - (pstNewFrame->u32FrameRate - (pstNewFrame->u32FrameRate/100 * 100))
                                        );

    PROC_PRINT(p, "%-19s:%-4d:%-15d", "CustAspectRatio", (HI_U32)pstProcInfo->stAttr.stCustmAR.u32ARw,
                                                              (HI_U32)pstProcInfo->stAttr.stCustmAR.u32ARh);
    PROC_PRINT(p, "%-19s:%-20s\n", "|ColorSpace", g_pVDPColorSpaceString[pstPriv->eColorSpace]);

    PROC_PRINT(p, "%-19s:%-20s", "Crop", g_pWinTrueString[pstProcInfo->stAttr.bUseCropRect]);
    PROC_PRINT(p, "%-19s:%s(%s)\n", "|Fieldmode(Origin)", g_pWinFieldTypeString[pstNewFrame->enFieldMode],
                                                                    g_pWinFieldTypeString[pstPriv->eOriginField]);
    PROC_PRINT(p, "%-19s:%4x/%4x/%4x/%4x ", "Crop(L/T/R/B) ",  pstProcInfo->stAttr.stCropRect.u32LeftOffset,
                                                                    pstProcInfo->stAttr.stCropRect.u32TopOffset,
                                                                    pstProcInfo->stAttr.stCropRect.u32RightOffset,
                                                                    pstProcInfo->stAttr.stCropRect.u32BottomOffset);
    PROC_PRINT(p, "%-19s:%d/%d/%d/%d\n", "|OriRect(X/Y/W/H)",  0,
                                                               0,
                                                                    pstPriv->stVideoOriginalInfo.u32Width,
                                                                    pstPriv->stVideoOriginalInfo.u32Height);
    PROC_PRINT(p, "%-19s:%4d/%4d/%4d/%4d ", "In  (X/Y/W/H)",pstProcInfo->stAttr.stInRect.s32X,
                                                                pstProcInfo->stAttr.stInRect.s32Y,
                                                                pstProcInfo->stAttr.stInRect.s32Width,
                                                                pstProcInfo->stAttr.stInRect.s32Height);
    PROC_PRINT(p, "%-19s:0x%x\n", "|FrameIndex",  pstNewFrame->u32FrameIndex);
    PROC_PRINT(p, "%-19s:%4d/%4d/%4d/%4d ", "Out(X/Y/W/H)", pstProcInfo->stAttr.stOutRect.s32X,
                                                                pstProcInfo->stAttr.stOutRect.s32Y,
                                                                pstProcInfo->stAttr.stOutRect.s32Width,
                                                                pstProcInfo->stAttr.stOutRect.s32Height);
    PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|SrcPTS/PTS",  pstNewFrame->u32SrcPts, pstNewFrame->u32Pts);
    PROC_PRINT(p, "%-19s:%-10s/%-9s", "DispMode/RightFirst", g_pWinDispModeString[pstProcInfo->eDispMode],
                                                                 g_pWinTrueString[pstProcInfo->bRightEyeFirst]);
    PROC_PRINT(p, "%-19s:%d\n", "|PlayTime", pstPriv->u32PlayTime);
    PROC_PRINT(p, "%-19s:%-20s", "*Masked", g_pWinTrueString[pstProcInfo->bMasked]);
    PROC_PRINT(p, "%-19s:%-20s\n", "|FieldMode", g_pWinFieldModeString[pstNewFrame->enFieldMode]);
    PROC_PRINT(p, "%-19s:%-20s", "AttachSource", g_pWinTrueString[pstProcInfo->hSrc!= HI_NULL]);
    PROC_PRINT(p, "%-19s:%d\n", "|Fidelity", pstPriv->u32Fidelity);
    PROC_PRINT(p, "%-19s:%-20s", "*CallBack(Acquire)", g_pWinYNString[pstProcInfo->pfAcqFrame    !=HI_NULL]);
    PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|YAddr/YStride", pstNewFrame->stBufAddr[0].u32PhyAddr_Y,pstNewFrame->stBufAddr[0].u32Stride_Y);
    PROC_PRINT(p, "%-19s:%-20s", "*CallBack(Release)", g_pWinYNString[pstProcInfo->pfRlsFrame    !=HI_NULL]);
    PROC_PRINT(p, "%-19s:0x%x/0x%x\n", "|CAddr/CStride", pstNewFrame->stBufAddr[0].u32PhyAddr_C,pstNewFrame->stBufAddr[0].u32Stride_C);
    PROC_PRINT(p, "%-19s:%-20s|\n", "*CallBack(SetAttr)", g_pWinYNString[pstProcInfo->pfSendWinInfo !=HI_NULL]);



    if (pstProcInfo->hSlvWin)
    {
        PROC_PRINT(p, "%-19s:%04X                |\n", "SlaveWinID", (HI_U32)(pstProcInfo->hSlvWin & 0xFFFFul));
    }
    else
    {
        PROC_PRINT(p, "%-19s:%04X                |\n", "SlaveWinID", (HI_U32)0xFFFFul);
    }

    PROC_PRINT(p, "%-19s:%-4d                |\n", "bQuickoutMode", pstProcInfo->bQuickMode);
    PROC_PRINT(p, "%-19s:%-4d                |\n", "bVirtualCoordinate", pstProcInfo->bVirtualCoordinate);

//    if (g_bWinPrivateInformation)
	if (1)
    {
        PROC_PRINT(p, "%-19s:%-20X \n", "*LayerRegionNum", pstProcInfo->u32LayerRegionNo);
        PROC_PRINT(p, "%-19s:%-20X \n", "*Zorder", pstProcInfo->u32Zorder);
		
        if (pstProcInfo->stWinInfoForDeveloper.stSrDciPhysicalInfo.bDci
            && pstProcInfo->stWinInfoForDeveloper.stSrDciPhysicalInfo.bSR)
        {

            PROC_PRINT(p, "%-19s\n", "-------------------SR And DCI Location Info----------------");

            PROC_PRINT(p, "%-19s:%-20s \n", "SR  Location:",  g_pAlgLocation[pstProcInfo->stWinInfoForDeveloper.stSrDciPhysicalInfo.eSrLocation]);
            PROC_PRINT(p, "%-19s:%-20s \n", "DCI Location:",  g_pAlgLocation[pstProcInfo->stWinInfoForDeveloper.stSrDciPhysicalInfo.eDciLocation]);
            PROC_PRINT(p, "%-19s:%-20s \n", "SR Behind DCI:", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.stSrDciPhysicalInfo.bSrBehindDci]);

            PROC_PRINT(p, "%-19s\n", "-------------------SR INFO---------------------------------------");

            PROC_PRINT(p, "%-19s:%-20s \n", "Hor SR Pre  Enable", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bHorSrOpenInPreProcess]);
            PROC_PRINT(p, "%-19s:%-20s \n", "Hor SR Post Enable", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bHorSrOpenInPostProcess]);
            PROC_PRINT(p, "%-19s:%-20s \n", "Ver SR Pre  Enable", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bVerSrOpenInPreProcess]);
            PROC_PRINT(p, "%-19s:%-20s \n", "Ver SR Post Enable", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bVerSrOpenInPostProcess]);

#if 0
            if (pstProcInfo->stWinInfoForDeveloper.bHorSrOpenInPreProcess
                && pstProcInfo->stWinInfoForDeveloper.bHorSrOpenInPostProcess
                && pstProcInfo->stWinInfoForDeveloper.bVerSrOpenInPreProcess
                && pstProcInfo->stWinInfoForDeveloper.bVerSrOpenInPostProcess)
#else
            if (1)
#endif
            {

                PROC_PRINT(p, "%-25s%d \n", "Current Winnum:", pstProcInfo->stWinInfoForDeveloper.u32WinNum);
                PROC_PRINT(p, "%-25s%d/%d \n", "Current FMT(w/h):", pstProcInfo->stWinInfoForDeveloper.eCurrentFmt.s32Width,
                                                                   pstProcInfo->stWinInfoForDeveloper.eCurrentFmt.s32Height);
                PROC_PRINT(p, "%-25s%s \n", "bExistScaleDown_WhenRatioRevise:", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bExistScaleDown_WhenRatioRevise]);
                PROC_PRINT(p, "%-25s%s \n", "bIn3DMode:", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bIn3DMode]);

                PROC_PRINT(p, "%-25s%d/%d \n", "Original frame Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stOringinFrameSize.s32Width,
                                                                   pstProcInfo->stWinInfoForDeveloper.stOringinFrameSize.s32Height);

                PROC_PRINT(p, "%-25s%d/%d/%d/%d \n", "Window Output Coordinate(x/y/w/h):", pstProcInfo->stWinInfoForDeveloper.stFinalWinOutputSize.s32X,
                                                                   pstProcInfo->stWinInfoForDeveloper.stFinalWinOutputSize.s32Y,
                                                                   pstProcInfo->stWinInfoForDeveloper.stFinalWinOutputSize.s32Width,
                                                                   pstProcInfo->stWinInfoForDeveloper.stFinalWinOutputSize.s32Height);

                PROC_PRINT(p, "%-25s%d/%d/%d/%d \n", "V0'S Output Coordinate(x/y/w/h):",
                                                                   pstProcInfo->stWinInfoForDeveloper.stOutputSizeOfV0.s32X,
                                                                   pstProcInfo->stWinInfoForDeveloper.stOutputSizeOfV0.s32Y,
                                                                   pstProcInfo->stWinInfoForDeveloper.stOutputSizeOfV0.s32Width,
                                                                   pstProcInfo->stWinInfoForDeveloper.stOutputSizeOfV0.s32Height);

                PROC_PRINT(p, "%-25s%d/%d \n", "Vdp Require Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stVdpRequire.s32Width,
                                                       pstProcInfo->stWinInfoForDeveloper.stVdpRequire.s32Height);

                PROC_PRINT(p, "%-25s%d/%d \n", "Vpss Give Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stVpssGive.s32Width,
                                           pstProcInfo->stWinInfoForDeveloper.stVpssGive.s32Height);

                PROC_PRINT(p, "%-25s%d/%d \n", "Sr's output Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stSrOutputSize.s32Width,
                                           pstProcInfo->stWinInfoForDeveloper.stSrOutputSize.s32Height);

                PROC_PRINT(p, "%-25s%d/%d \n", "Sr's output Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stSrOutputSize.s32Width,
                                           pstProcInfo->stWinInfoForDeveloper.stSrOutputSize.s32Height);
            }

            PROC_PRINT(p, "%-25s\n", "-------------------DCI INFO-------------------------------------------");
            PROC_PRINT(p, "%-25s%-20s \n", "Dci Enable:", g_pWinTrueString[pstProcInfo->stWinInfoForDeveloper.bDciOpen]);

            if (pstProcInfo->stWinInfoForDeveloper.bDciOpen)
            {
                PROC_PRINT(p, "%-25s%d/%d \n", "Vpss Give Frame Size(w/h):", pstProcInfo->stWinInfoForDeveloper.stDciFrameSize.s32Width,
                           pstProcInfo->stWinInfoForDeveloper.stDciFrameSize.s32Height);

                PROC_PRINT(p, "%-25s%d/%d/%d/%d \n", "Vpss Give Dci Position(x/y/x'/y'):",
                                           pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32X,
                                           pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32Y,
                                           pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32Width + pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32X,
                                           pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32Height + pstProcInfo->stWinInfoForDeveloper.stOriginDCIPositionInFrame.s32Y);

                PROC_PRINT(p, "%-25s%d/%d/%d/%d \n", "DCI  input  effective content size(x/y/w/h):",
                                           pstProcInfo->stWinInfoForDeveloper.stDciEffecttiveContentInputSize.s32X,
                                           pstProcInfo->stWinInfoForDeveloper.stDciEffecttiveContentInputSize.s32Y,
                                           pstProcInfo->stWinInfoForDeveloper.stDciEffecttiveContentInputSize.s32Width,
                                           pstProcInfo->stWinInfoForDeveloper.stDciEffecttiveContentInputSize.s32Height);

                PROC_PRINT(p, "%-25s%d/%d/%d/%d \n", "DCI  Config   Position(x/y/x'/y'):",
                                           pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32X,
                                           pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32Y,
                                           pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32Width + pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32X,
                                           pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32Height + pstProcInfo->stWinInfoForDeveloper.stWinFinalPosition.s32Y);
            }
        }

		if ((pstProcInfo->stWinWbcIsoProcInfor.bWbcInit == HI_TRUE)
			&& (pstProcInfo->enType == HI_DRV_WIN_ACTIVE_SLAVE))
		{
			PROC_PRINT(p, "\n");	
			PROC_PRINT(p, "-----------------wbc  infor--------------------------------|\n");
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc point", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.eCurrentWbcPoint);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc config ok counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigOkCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc config try counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigTryCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc genarate ok counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcGenarateSuccessCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"p2iopenflag:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcP2IOpenedOrNot);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbcpoint switch cnt:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcPointSwitchCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc retrieve frm from sd:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcRetriveFromSdNodeCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"dhd current field:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32DhdFieldModeWhenConfigWbc);
			PROC_PRINT(p, "%-19s:%-20d\n",	"dhd current line:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32DhdCurrentLineWhenConfigWbc);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc input field:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcInputFiledMode);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc output field:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcOutputFiledMode);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc config speed:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcRunningInfor.u32WbcConfigSpeed);
			
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc total frame counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcBufferStatus.u32WbcTotalNodeCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc free frm counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcBufferStatus.u32WbcFreeNodesCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc full frm counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcBufferStatus.u32WbcFullNodesCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc configure counts:", pstProcInfo->stWinWbcIsoProcInfor.stWbcProcInfor.stWbcBufferStatus.U32WbcConfiguringNodeCounts);
			
			
			PROC_PRINT(p, "-----------------partfinish	infor--------------------------------|\n");
			PROC_PRINT(p, "%-19s:%-20d\n",	"partfinish state:", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.ePartFinishThreadState);
			PROC_PRINT(p, "%-19s:%-20d\n",	"partfinish interval:", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishIntervals);
			PROC_PRINT(p, "%-19s:%-20d\n",	"partfinish push frm cnts(ok):", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishPushFrameCnts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"partfinish push frm cnts(try):", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishTrycounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"partfinish buf node cnts     :", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishNodesCnts);
            PROC_PRINT(p, "%-19s:%-20d\n",	"wbc generate frame speed     :", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcPartFinishSpeed);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc start time(ms)           :", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcStartTime);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc current time(ms)         :", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcCurrentTime);
			PROC_PRINT(p, "%-19s:%-20d\n",	"wbc be late counts           :", pstProcInfo->stWinWbcIsoProcInfor.stPartFinishThreadProcInfor.u32WbcHighLantencyTimes);
			
			PROC_PRINT(p, "-----------------sd channel	infor--------------------------------|\n");
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd totalfrm counts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdWinTotalFrameCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd to be displayed counts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdToBedisplayedCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd displaying:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdDisplayingFrameCounts);
			
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd return counts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdReturnBackFrameCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd underload cnts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdUnderloadCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd discard cnts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdDiscardsCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd field mismatch cnts:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdFiledMisMatchCounts);
			PROC_PRINT(p, "%-19s:%-20d\n",	"dsd current field:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32DsdFieldMode);
			PROC_PRINT(p, "%-19s:%-20d\n",	"dsd current line :", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32DsdCurrentLine);
			PROC_PRINT(p, "%-19s:%-20d\n",	"sd frame filed:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32SdFrameFieldMode);
			PROC_PRINT(p, "%-19s:%-20d\n",	" hd/sd frame espired time:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32FrameLantencyTimeMs);
			PROC_PRINT(p, "%-19s:%-20d\n",	" expect wbc finish   time:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32FrameExpectWbcTimeMs);
			PROC_PRINT(p, "%-19s:%-20d\n",	" linker process      time:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32FrameLinkerProcessTimeMs);
			PROC_PRINT(p, "%-19s:%-20d\n",	" sd receive frame    time:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32FrameSdReceiveTimeMs);
			PROC_PRINT(p, "%-19s:%-20d\n",	" sd display          time:", pstProcInfo->stWinWbcIsoProcInfor.stSdWinProcInfor.u32FrameSdDisplayTimeMs);
		}
    }


    if (pstProcInfo->stAttr.bVirtual)
    {
        PROC_PRINT(p, "----VirtualWindowInfo-----           |\n");
        PROC_PRINT(p, "%-19s:%-20s\n", "MemoryType", g_pWinMemTypeString[pstProcInfo->stAttr.bUserAllocBuffer==HI_TRUE]);
        PROC_PRINT(p, "%-19s:%-20d\n", "BufferNum", pstProcInfo->stAttr.u32BufNumber);
        PROC_PRINT(p, "%-19s:%-20s\n", "PixelFormat", DrvWinGetPixfmtString(pstProcInfo->stAttr.enDataFormat));
    }

	/*
	-----------------------Buffer State---------------------------------
	Queue(Try/OK)       :179584/179581
	Dequeue(Try/OK)     :179558/179558
	Underload           :13750977
	Discard             :4
	UndispFrame(Q/DQ)   :5/5
	FieldUnmatchCnt     :32
	-------------------------------------------------------------------
	BufferQueue[state, FrameID]
	(State: 1,Empty[15]; 2,Write[0]; 3,ToDisp[0]; 4,Disp[1]; 5,Disped[0])
	[1,0x5eaf] [1,0x5eb0] [1,0x5eb0] [1,0x5eb1]
	[1,0x5eb1] [1,0x5eb2] [1,0x5eb2] [1,0x5eb3]
	[1,0x5eb3] [1,0x5eb4] [1,0x5eb5] [1,0x5eb4]
	[4,0x5eb5] [1,0x5eae] [1,0x5eae] [1,0x5eaf]
	*/

    pstBuffer = &pstProcInfo->stBufState;

    PROC_PRINT(p,
        "-----------------------Buffer State---------------------------------\n"
        "Queue(Try/OK)       :%d/%d\n"
        "Release(Try/OK)     :%d/%d\n"
        "Config              :%d\n"
        "Underload           :%d\n"
        "Discard             :%d\n"
        "UndispFrame(Q/DQ)   :%d/%d\n"
        "*FieldUnmatchCnt    :%d\n",
        pstBuffer->stRecord.u32TryQueueFrame,pstBuffer->stRecord.u32QueueFrame,
        pstBuffer->stRecord.u32Release, pstBuffer->stRecord.u32Release,
        pstBuffer->stRecord.u32Config,
        pstProcInfo->u32UnderLoad,
        pstBuffer->stRecord.u32Disacard,
        pstProcInfo->u32ULSIn,pstProcInfo->u32ULSOut,
        pstProcInfo->u32TBNotMatchCount
    );

    {
        HI_S32 EmpNum, WrtNum, ToDispNum, DispNum, DispedNum;

        EmpNum    = 0;
        WrtNum    = 0;
        ToDispNum = 0;
        DispNum   = 0;
        DispedNum = 0;

        for (i=0; i<pstProcInfo->stBufState.u32Number; i++)
        {
            switch(pstProcInfo->stBufState.stNode[i].u32State)
            {
                case 0:
                case 1:
                    EmpNum++;
                    break;
                case 2:
                    WrtNum++;
                    break;
                case 3:
                    ToDispNum++;
                    break;
                case 4:
                    DispNum++;
                    break;
                case 5:
                    DispedNum++;
                    break;
            }
        }

        PROC_PRINT(p,
            "--------------------------------------------------------------------\n"
            "BufferQueue[state, FrameID]\n"
            "(State: 1,Empty[%d]; 2,Write[%d]; 3,ToDisp[%d]; 4,Disp[%d]; 5,Disped[%d])\n",
            EmpNum, WrtNum, ToDispNum, DispNum, DispedNum
        );
    }

    for (i=0; i<pstBuffer->u32Number;)
    {
			PROC_PRINT(p,"[%d,0x%x] ",
            pstBuffer->stNode[i].u32State,
            pstBuffer->stNode[i].u32FrameIndex
            );

        i++;
        if( (i%4) == 0)
        {
            PROC_PRINT(p, "\n");
        }
    }

    PROC_PRINT(p, "\n");

_ERR_EXIT_:
    if (pstProcInfo)
    {
        DISP_FREE(pstProcInfo);
    }
    up(&g_VoMutex);
    return nRet;
}

static HI_S32 WinProcParsePara(HI_CHAR *pProcPara,HI_CHAR **ppArg1,HI_CHAR **ppArg2)
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
HI_VOID WIN_ProcWriteHelp(HI_VOID)
{
    HI_DRV_PROC_EchoHelper("------------------Win Debug Option----------------\n");
    HI_DRV_PROC_EchoHelper("echo help                    > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo pause   on/off          > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo reset   black/still     > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo freeze  black/still/off > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo order   up/down         > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo quick   on/off          > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo rota    0/90/180/270    > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo flip    hori/vert       > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo depth   X               > /proc/msp/winXXXX  \n");
    HI_DRV_PROC_EchoHelper("echo capture path            > /proc/msp/winXXXX  \n");

}


#ifndef HI_ADVCA_FUNCTION_RELEASE
HI_S32 WinProcCmdProcee(HI_HANDLE hWin, HI_CHAR *pArg1,HI_CHAR *pArg2)
{
    HI_S32 nRet = HI_SUCCESS;

    if (0 == HI_OSAL_Strncmp(pArg1, "pause", strlen("pause")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "on", strlen("on")))
        {
            nRet = WIN_Pause(hWin, HI_TRUE);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = WIN_Pause(hWin, HI_FALSE);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "reset", strlen("reset")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "black", strlen("black")))
        {
            nRet = WIN_Reset(hWin, HI_DRV_WIN_SWITCH_BLACK);
        }
        if (0 == HI_OSAL_Strncmp(pArg2, "still", strlen("still")))
        {
            nRet = WIN_Reset(hWin, HI_DRV_WIN_SWITCH_LAST);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "freeze", strlen("freeze")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "black", strlen("black")))
        {
            nRet = WIN_Freeze(hWin, HI_TRUE, HI_DRV_WIN_SWITCH_BLACK);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "still", strlen("still")))
        {
            nRet = WIN_Freeze(hWin, HI_TRUE, HI_DRV_WIN_SWITCH_LAST);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = WIN_Freeze(hWin, HI_FALSE, HI_DRV_WIN_SWITCH_BLACK);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "order", strlen("order")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "up", strlen("up")))
        {
            nRet = WIN_SetZorder(hWin, HI_DRV_DISP_ZORDER_MOVEUP);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "down", strlen("down")))
        {
            nRet = WIN_SetZorder(hWin, HI_DRV_DISP_ZORDER_MOVEDOWN);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "quick", strlen("quick")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "on", strlen("on")))
        {
            nRet = WIN_SetQuick(hWin, HI_TRUE);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = WIN_SetQuick(hWin, HI_FALSE);
        }
        else
        {
            WIN_ProcWriteHelp();
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "rota", strlen("rota")))
    {

        HI_ERR_WIN("Not support set rotation now\n");
        return HI_SUCCESS;

#if 0
        if (0 == HI_OSAL_Strncmp(pArg2, "0", strlen("0")))
        {
            nRet = WIN_SetRotation(hWin, HI_DRV_ROT_ANGLE_0);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "90", strlen("90")))
        {
            nRet = WIN_SetRotation(hWin, HI_DRV_ROT_ANGLE_90);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "180", strlen("180")))
        {
            nRet = WIN_SetRotation(hWin, HI_DRV_ROT_ANGLE_180);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "270", strlen("270")))
        {
            nRet = WIN_SetRotation(hWin, HI_DRV_ROT_ANGLE_270);
        }
        else
        {
            return HI_FAILURE;
        }
#endif
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "flip", strlen("flip")))
    {
        HI_ERR_WIN("Not support set flip now\n");
        return HI_SUCCESS;

#if 0
        HI_BOOL bHoriFlip, bVertFlip;

        nRet = WIN_GetFlip(hWin, &bHoriFlip, &bVertFlip);
        if (nRet != HI_SUCCESS)
        {
            HI_ERR_WIN("WIN_GetFlip failed\n");
            return HI_SUCCESS;
        }

        if (0 == HI_OSAL_Strncmp(pArg2, "hori", strlen("hori")))
        {
            nRet = WIN_SetFlip(hWin, HI_TRUE, bVertFlip);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "vert", strlen("vert")))
        {
            nRet = WIN_SetFlip(hWin, bHoriFlip, HI_TRUE);
        }
        else
        {
            return HI_FAILURE;
        }
#endif
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "StereoDepth", strlen("StereoDepth")))
    {
#if 0
        HI_S32 s32Depth = 0;
        HI_ERR_WIN("Not support set 3d-depth now\n");


#define WIN_3D_VIDEO_MAX_DEPTH 50
#define WIN_3D_VIDEO_MIN_DEPTH -50

        s32Depth = simple_strtol(pArg2, NULL, 10);
        if ( (s32Depth > WIN_3D_VIDEO_MAX_DEPTH) || (s32Depth < WIN_3D_VIDEO_MIN_DEPTH))
        {
            HI_ERR_WIN("invalid 3d-depth value=%d, it must be in [-50, 50]\n", s32Depth);
            return HI_SUCCESS;
        }
#endif

        return HI_SUCCESS;

    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "pause", strlen("pause")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "on", strlen("on")))
        {
            nRet = WIN_Pause(hWin, HI_TRUE);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = WIN_Pause(hWin, HI_FALSE);
        }
        else
        {
            WIN_ProcWriteHelp();
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "capture", strlen("capture")))
    {
        HI_UCHAR *pChar = pArg2;
        HI_DRV_VIDEO_FRAME_S *pstCurFrame;
        HI_U32 pathlength = 0;


        pstCurFrame = (HI_DRV_VIDEO_FRAME_S*)DISP_MALLOC(sizeof(HI_DRV_VIDEO_FRAME_S));
        if (!pstCurFrame)
        {
            HI_ERR_WIN("alloc frame info memory failed\n");
            return nRet;
        }

        /* get currently displayed frame  */
        nRet = WinGetCurrentImg(hWin, pstCurFrame);
        if (nRet != HI_SUCCESS)
        {
            DISP_FREE(pstCurFrame);
            HI_ERR_WIN("call WinGetCurrentImg failed\n");
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

        DISP_FREE(pstCurFrame);
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "rd_door", strlen("rd_door")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "start", strlen("start")))
        {
            g_bWinPrivateInformation = HI_TRUE;
        }
        else
        {
            g_bWinPrivateInformation = HI_FALSE;
        }
    }
    else if (0 == HI_OSAL_Strncmp(pArg1, "sr_post_dci_test", strlen("sr_post_dci_test")))
    {
        WIN_Test(hWin);
    }
	else  if (0 == HI_OSAL_Strncmp(pArg1, "compress", strlen("compress")))
    {
        if (0 == HI_OSAL_Strncmp(pArg2, "on", strlen("on")))
        {
            nRet = Win_User_SetCompressFlag(hWin, HI_TRUE);
        }
        else if (0 == HI_OSAL_Strncmp(pArg2, "off", strlen("off")))
        {
            nRet = Win_User_SetCompressFlag(hWin, HI_FALSE);
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return nRet;
}
#endif

HI_CHAR u8WinProcBuffer[256];
HI_S32 DRV_WIN_ProcWrite(struct file * file,
                   const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file *p = file->private_data;
    DRV_PROC_ITEM_S *pProcItem = p->private;
    HI_CHAR *pArg1, *pArg2;
    HI_HANDLE hWin;
    HI_S32 nRet;

    hWin = (HI_HANDLE)(pProcItem->data);

    if(count >= sizeof(u8WinProcBuffer))
    {
        HI_ERR_WIN("your parameter string is too long!\n");
        return -EFAULT;
    }

    nRet = down_interruptible(&g_VoMutex);

    memset(u8WinProcBuffer, 0, sizeof(u8WinProcBuffer));
    if (copy_from_user(u8WinProcBuffer, buf, count))
    {
        up(&g_VoMutex);
        HI_ERR_WIN("MMZ: copy_from_user failed!\n");
        return -EFAULT;
    }
    u8WinProcBuffer[count] = 0;

    nRet = WinProcParsePara(u8WinProcBuffer, &pArg1, &pArg2);
    if(  (nRet != HI_SUCCESS)
        ||(0 == HI_OSAL_Strncmp(pArg1, "help", strlen("help")))
        ||(pArg2 == HI_NULL) )
    {
        up(&g_VoMutex);
        HI_ERR_WIN("Invalid echo commond!\n");
        WIN_ProcWriteHelp();
        return -EFAULT;
    }

#ifndef HI_ADVCA_FUNCTION_RELEASE
    nRet = WinProcCmdProcee(hWin, pArg1, pArg2);
    if (nRet != HI_SUCCESS)
    {
        WIN_ProcWriteHelp();
    }
#endif

    up(&g_VoMutex);

    return count;
}



/***************************************************************/
HI_S32 WIN_AddToProc(HI_HANDLE hWindow)
{
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR           ProcName[12];
    HI_U32 u32Index;
    HI_S32 Ret;

	/*as a result of wbc-isogeny mode, false may not means failed*/
	if (HI_FALSE == WinCheckWhetherCanToCreate(hWindow))
	{
		HI_WARN_WIN("WinGetProcInfo failed!\n");
        return HI_SUCCESS;
	}	

    Ret = WinGetProcIndex(hWindow, &u32Index);
    if (Ret)
    {
        HI_ERR_WIN("WinGetProcInfo failed!\n");
        return HI_ERR_VO_ADD_PROC_ERR;
    }
	
    HI_OSAL_Snprintf(ProcName, 12, "win%04x", (HI_U32)(u32Index & WINDOW_INDEX_MASK));

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_ERR_WIN("Window add proc failed!\n");
        HI_ERR_WIN("WinGetProcInfo failed!\n");
        return HI_ERR_VO_ADD_PROC_ERR;
    }
	
    pProcItem->data  = (HI_VOID *)hWindow;
    pProcItem->read  = DRV_WIN_ProcRead;
    pProcItem->write = DRV_WIN_ProcWrite;

    return HI_SUCCESS;
}

HI_S32 WIN_RemFromProc(HI_HANDLE hWindow)
{
    HI_CHAR           ProcName[12];
    HI_U32 u32Index;
    HI_S32 Ret;

	/*as a result of wbc-isogeny mode, false may not means failed*/
	if (HI_FALSE == WinCheckWhetherCanToDestroy(hWindow))
	{
		HI_WARN_WIN("WinGetProcInfo failed!\n");
        return HI_SUCCESS;
	}	
	
    Ret = WinGetProcIndex(hWindow, &u32Index);	
    if (!Ret)
    {
        HI_OSAL_Snprintf(ProcName, 12 ,"win%04x", (HI_U32)(u32Index & WINDOW_INDEX_MASK));
        HI_DRV_PROC_RemoveModule(ProcName);
    }
    else
    {
        HI_FATAL_WIN("WinGetProcInfo failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_AddToState(WIN_STATE_S *pst2WinState, HI_BOOL bVirtual, HI_DRV_DISPLAY_E enDisp, HI_HANDLE hWin)
{
    HI_S32 i;

    if (bVirtual == HI_TRUE)
    {
        for(i=0; i<DEF_MAX_WIN_NUM_ON_VIRTUAL_DISP; i++)
        {
            if (pst2WinState->hVirtualWin[i] == HI_NULL)
            {
                pst2WinState->hVirtualWin[i] = hWin;
                return HI_SUCCESS;
            }
        }
    }
    else
    {
        if (enDisp >= HI_DRV_DISPLAY_BUTT)
        {
            return HI_SUCCESS;
        }

        for(i=0; i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
        {
            if (pst2WinState->hWin[enDisp][i] == HI_NULL)
            {
                pst2WinState->hWin[enDisp][i] = hWin;
                return HI_SUCCESS;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 WIN_RemFromState(WIN_STATE_S *pst2WinState,HI_BOOL bVirtual, HI_HANDLE hWin)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 i;

    if (bVirtual)
    {
        for(i=0; i<DEF_MAX_WIN_NUM_ON_VIRTUAL_DISP; i++)
        {
            if (pst2WinState->hVirtualWin[i] == hWin)
            {
                pst2WinState->hVirtualWin[i] = HI_NULL;
                return HI_SUCCESS;
            }
        }
    }
    else
    {
        for(enDisp=0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
        {
            for(i=0; i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
            {
                if (pst2WinState->hWin[enDisp][i] == hWin)
                {
                    pst2WinState->hWin[enDisp][i] = HI_NULL;
                    return HI_SUCCESS;
                }
            }
        }
    }

    return HI_SUCCESS;
}

typedef enum {
    VDP_MCE_STAGE = 0, /*mce stage*/
    VDP_NORMAL_STAGE,  /*kernel has boot already.*/
}VDP_MCE_STAGE_E;

VDP_MCE_STAGE_E sftware_stage = 0;

HI_S32 DRV_WIN_SetSftwareStage(HI_VOID)
{
    sftware_stage = VDP_NORMAL_STAGE;
    return HI_SUCCESS;
}

HI_S32 DRV_WIN_GetSftwareStage(HI_VOID)
{
    return sftware_stage;
}

HI_S32 WIN_CreateExt(WIN_CREATE_S *pVoWinCreate, WIN_STATE_S *pst2WinState)
{
    HI_DRV_WIN_INFO_S stWinInfo;
    HI_S32 Ret;

    Ret = WIN_Create(&pVoWinCreate->WinAttr, &pVoWinCreate->hWindow, pVoWinCreate->bVirtScreen);
    if (Ret)
    {
        goto __ERR_EXIT__;
    }

    if (DRV_WIN_GetSftwareStage())
    {
        // add proc
        Ret = WIN_AddToProc(pVoWinCreate->hWindow);
        if (Ret)
            goto __ERR_EXIT_DESTROY__;

        if (pVoWinCreate->WinAttr.bVirtual != HI_TRUE)
        {
            if (HI_SUCCESS == WIN_GetInfo(pVoWinCreate->hWindow, &stWinInfo))
            {
                if (stWinInfo.hSec)
                {
                    Ret = WIN_AddToProc(stWinInfo.hSec);
                    if (Ret)
                        goto __ERR_EXIT_REM_PORC__;
                }
            }
        }
    }

    WIN_AddToState(pst2WinState, pVoWinCreate->WinAttr.bVirtual, pVoWinCreate->WinAttr.enDisp, pVoWinCreate->hWindow);
    return HI_SUCCESS;

__ERR_EXIT_REM_PORC__:
    Ret = WIN_RemFromProc(pVoWinCreate->hWindow);

__ERR_EXIT_DESTROY__:
    Ret = WIN_Destroy(pVoWinCreate->hWindow);

__ERR_EXIT__:
    return Ret;
}

HI_S32 WIN_DestroyExt(HI_HANDLE hWindow, WIN_STATE_S *pstWinState)
{
    HI_DRV_WIN_INFO_S stWinInfo;
    HI_S32 Ret;

    if (HI_SUCCESS != WIN_GetInfo(hWindow, &stWinInfo))
    {
        HI_ERR_WIN("Window dose not exist!\n");
        return HI_ERR_VO_WIN_NOT_EXIST;
    }

    if (stWinInfo.eType == HI_DRV_WIN_VITUAL_SINGLE)
    {
        // if virtual window, remove proc and destory.
        Ret = WIN_RemFromProc(hWindow);
        if (Ret)
        {
            HI_ERR_WIN("WIN_RemFromProc failed!\n");
        }

        Ret = WIN_Destroy(hWindow);
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_WIN("call WIN_Destroy failed.\n");
        }

        WIN_RemFromState(pstWinState, HI_TRUE, hWindow);
    }
    else
    {
        if (stWinInfo.hSec)
        {
            // if have slave window, remove slave window proc firstly
            Ret = WIN_RemFromProc(stWinInfo.hSec);
            if (Ret)
            {
                HI_ERR_WIN("WIN_RemFromProc slave failed!\n");
            }
        }

        // remove proc and destory window
        Ret = WIN_RemFromProc(hWindow);
        if (Ret)
        {
            HI_ERR_WIN("WIN_RemFromProc failed!\n");
        }

        Ret = WIN_Destroy(hWindow);
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_WIN("call WIN_Destroy failed.\n");
        }

        WIN_RemFromState(pstWinState, HI_FALSE, hWindow);
    }

    return Ret;
}

HI_S32 WIN_CheckHanlde(HI_HANDLE hWindow, WIN_STATE_S *pstWinState)
{
    return HI_SUCCESS;
}

HI_S32 WIN_GetAttachFunc(HI_DRV_WIN_SRC_INFO_S *pstWinSrc,HI_MOD_ID_E enModID)
{
    VDEC_EXPORT_FUNC_S *pstVdecFunc = HI_NULL;
    VI_EXPORT_FUNC_S *pstViFunc = HI_NULL;
    HI_S32 s32Ret;
    HI_HANDLE hSrc;

    hSrc = pstWinSrc->hSrc;
    
    if (hSrc == HI_INVALID_HANDLE)
    {
        pstWinSrc->pfAcqFrame = HI_NULL;
        pstWinSrc->pfRlsFrame = HI_NULL;
        pstWinSrc->pfSendWinInfo = HI_NULL;
        
        return HI_SUCCESS;
    }
    
    if ( HI_ID_VDEC == enModID)
    {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VDEC,(HI_VOID**)&(pstVdecFunc));
        if (HI_SUCCESS != s32Ret || pstVdecFunc == HI_NULL)
        {
            WIN_ERROR("get vdec func error.\n");
            return HI_FAILURE;
        }

        pstWinSrc->pfAcqFrame = HI_NULL;
        pstWinSrc->pfRlsFrame = pstVdecFunc->pfnVORlsFrame;
        pstWinSrc->pfSendWinInfo = pstVdecFunc->pfnVOSendWinInfo;
        
        s32Ret = HI_SUCCESS;
    }
    else if ( HI_ID_VI == enModID )
    {
        s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VI,(HI_VOID**)&(pstViFunc));
        if (HI_SUCCESS != s32Ret || pstViFunc == HI_NULL)
        {
            WIN_ERROR("get vi func error,s32Ret:%x,pstViFunc:%p\n",s32Ret,pstViFunc);
            return HI_FAILURE;
        }

        pstWinSrc->pfAcqFrame = pstViFunc->pfnVOAcqFrame;
        pstWinSrc->pfRlsFrame = pstViFunc->pfnVORlsFrame;
        pstWinSrc->pfSendWinInfo = pstViFunc->pfnVOSendWinInfo;
        
        s32Ret = HI_SUCCESS;
    }
    else
    {
       WIN_ERROR("enModID %#x is invalid.\n",enModID);
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
    
}



HI_S32 WIN_ProcessCmd(unsigned int cmd, HI_VOID *arg, WIN_STATE_S *pstWinState)
{
    HI_S32 Ret = HI_SUCCESS;

    switch (cmd)
    {
        case CMD_WIN_CREATE:
        {
            WIN_CREATE_S  *pVoWinCreate;

            pVoWinCreate = (WIN_CREATE_S *)arg;

            Ret = WIN_CreateExt(pVoWinCreate, pstWinState);

            break;
        }

        case CMD_WIN_DESTROY:
        {
            Ret = WIN_CheckHanlde(*((HI_HANDLE *)arg), pstWinState);

            if (HI_SUCCESS == Ret)
            {
                HI_BOOL bSinkAttached = HI_TRUE;
                HI_BOOL bSrcAttached = HI_TRUE;
                HI_HANDLE hWindow = HI_INVALID_HANDLE;
                hWindow = *((HI_HANDLE *)arg);
                Ret = WIN_CheckAttachState(hWindow,&bSrcAttached,&bSinkAttached);
                if (HI_SUCCESS != Ret)
                {
                    WIN_ERROR("Can't Get window's attachState\n");
                    return HI_ERR_VO_INVALID_OPT;
                }
                if (bSrcAttached == HI_TRUE || bSinkAttached == HI_TRUE)
                {
                    WIN_ERROR("Window is still attached,Can't be destoryed."
                              "bSrcAttached %d,bSinkAttached %d\n",bSrcAttached,bSinkAttached);
                    return HI_ERR_VO_INVALID_OPT;
                }
                Ret = WIN_DestroyExt(*((HI_HANDLE *)arg), pstWinState);
            }

            break;
        }

        case CMD_WIN_SET_ENABLE:
        {
            WIN_ENABLE_S   *pWinEnable;

            pWinEnable = (WIN_ENABLE_S *)arg;

            Ret = WIN_CheckHanlde(pWinEnable->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetEnable(pWinEnable->hWindow, pWinEnable->bEnable);
            }

            break;
        }

        case CMD_WIN_GET_ENABLE:
        {
            WIN_ENABLE_S   *pVoWinEnable;

            pVoWinEnable = (WIN_ENABLE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinEnable->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetEnable(pVoWinEnable->hWindow, &pVoWinEnable->bEnable);
            }

            break;
        }

        case CMD_WIN_VIR_ACQUIRE:
        {
            WIN_FRAME_S   *pVoWinFrame;
            pVoWinFrame = (WIN_FRAME_S*)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_AcquireFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_VIR_RELEASE:
        {
            WIN_FRAME_S   *pVoWinFrame;
            pVoWinFrame = (WIN_FRAME_S*)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_ReleaseFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_SET_ATTR:
        {
            WIN_CREATE_S *pVoWinAttr;

            pVoWinAttr = (WIN_CREATE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetAttr(pVoWinAttr->hWindow, &pVoWinAttr->WinAttr);
            }

            break;
        }

        case CMD_WIN_GET_ATTR:
        {
            WIN_CREATE_S   *pVoWinAttr;

            pVoWinAttr = (WIN_CREATE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetAttr(pVoWinAttr->hWindow, &pVoWinAttr->WinAttr);
            }

            break;
        }

        case CMD_WIN_SET_ZORDER:
        {
            WIN_ZORDER_S *pVoWinZorder;

            pVoWinZorder = (WIN_ZORDER_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinZorder->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetZorder(pVoWinZorder->hWindow, pVoWinZorder->eZFlag);
            }

            break;
        }

        case CMD_WIN_GET_ORDER:
        {
            WIN_ORDER_S *pVoWinOrder;

            pVoWinOrder = (WIN_ORDER_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinOrder->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetZorder(pVoWinOrder->hWindow, &pVoWinOrder->Order);
            }

            break;
        }

        case CMD_WIN_SET_SOURCE:
        {
            WIN_SOURCE_S *pVoWinAttach;
            HI_DRV_WIN_SRC_INFO_S stWinSrc = {0};
            HI_MOD_ID_E enSrcModID;
            pVoWinAttach = (WIN_SOURCE_S *)arg;
            
            stWinSrc.hSrc = pVoWinAttach->stSrc.hSrc;
            stWinSrc.hSecondSrc = pVoWinAttach->stSrc.hSecondSrc;
            enSrcModID = pVoWinAttach->stSrc.enSrcMode;
            
            Ret = WIN_GetAttachFunc(&stWinSrc,enSrcModID);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetSource(pVoWinAttach->hWindow,
                                    &stWinSrc);
            }

            break;
        }
        case CMD_WIN_GET_SOURCE:
        {
            WIN_SOURCE_S *pVoWinAttach;
            HI_DRV_WIN_SRC_INFO_S stSrcInfo;
            pVoWinAttach = (WIN_SOURCE_S *)arg;

            Ret = WIN_GetSource(pVoWinAttach->hWindow, &stSrcInfo);
            if (HI_SUCCESS == Ret)
            {
                pVoWinAttach->stSrc.hSrc = stSrcInfo.hSrc;
                pVoWinAttach->stSrc.hSecondSrc = stSrcInfo.hSecondSrc;
            }
            
            break;
        }

        case CMD_WIN_FREEZE:
        {
            WIN_FREEZE_S  *pVoWinFreeze;

            pVoWinFreeze = (WIN_FREEZE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFreeze->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Freeze(pVoWinFreeze->hWindow, pVoWinFreeze->bEnable, pVoWinFreeze->eMode);
            }

            break;
        }

        case CMD_WIN_GET_FREEZE_STATUS:
        {
            WIN_FREEZE_S  *pVoWinFreeze;

            pVoWinFreeze = (WIN_FREEZE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFreeze->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetFreezeStatus(pVoWinFreeze->hWindow, &pVoWinFreeze->bEnable, &pVoWinFreeze->eMode);
            }

            break;
        }
        case CMD_WIN_SEND_FRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SendFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_QU_FRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;
            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_QueueFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }
            break;
        }
        case CMD_WIN_QUSYNC_FRAME:
        {
            WIN_SYNCFRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_SYNCFRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_QueueSyncFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame,&pVoWinFrame->u32FenceFd); 
            }
            break;
        }

        case CMD_WIN_QU_ULSFRAME:
        {
            WIN_FRAME_S   *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_QueueUselessFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }
            break;
        }

        case CMD_WIN_DQ_FRAME:
        {
            WIN_FRAME_S     *pVoWinFrame;

            pVoWinFrame = (WIN_FRAME_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinFrame->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_DequeueFrame(pVoWinFrame->hWindow, &pVoWinFrame->stFrame);
            }

            break;
        }

        case CMD_WIN_RESET:
        {
            WIN_RESET_S  *pVoWinReset;

            pVoWinReset = (WIN_RESET_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinReset->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Reset(pVoWinReset->hWindow, pVoWinReset->eMode);
            }

            break;
        }

        case CMD_WIN_PAUSE:
        {
            WIN_PAUSE_S  *pVoWinPause;

            pVoWinPause = (WIN_PAUSE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinPause->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_Pause(pVoWinPause->hWindow, pVoWinPause->bEnable);
            }

            break;
        }

        case CMD_WIN_GET_PLAY_INFO:
        {
            WIN_PLAY_INFO_S  *pVoWinDelay;

            pVoWinDelay = (WIN_PLAY_INFO_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinDelay->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetPlayInfo(pVoWinDelay->hWindow, &pVoWinDelay->stPlayInfo);
            }

            break;
        }

        case CMD_WIN_GET_INFO:
        {
            WIN_PRIV_INFO_S  *pVoWinDelay;

            pVoWinDelay = (WIN_PRIV_INFO_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinDelay->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetInfo(pVoWinDelay->hWindow, &pVoWinDelay->stPrivInfo);
            }

            break;
        }


        case CMD_WIN_STEP_MODE:
        {
            WIN_STEP_MODE_S  *pVoWinStepMode;

            pVoWinStepMode = (WIN_STEP_MODE_S *)arg;

            Ret = WIN_CheckHanlde(pVoWinStepMode->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetStepMode(pVoWinStepMode->hWindow, pVoWinStepMode->bStep);
            }

            break;
        }

        case CMD_WIN_STEP_PLAY:
        {
            Ret = WIN_CheckHanlde(*((HI_HANDLE *)arg), pstWinState);

            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetStepPlay(*((HI_HANDLE *)arg));
            }

        break;
        }

        case CMD_WIN_VIR_EXTERNBUF:
        {
            WIN_BUF_POOL_S*      winBufAttr = (WIN_BUF_POOL_S*)arg;

            Ret = WIN_CheckHanlde(winBufAttr->hwin, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetExtBuffer(winBufAttr->hwin, &winBufAttr->stBufPool);
            }
            break;
        }

        case CMD_WIN_SET_QUICK:
        {
            WIN_SET_QUICK_S * stQuickOutputAttr = (WIN_SET_QUICK_S*)arg;
            Ret = WIN_CheckHanlde(stQuickOutputAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetQuick(stQuickOutputAttr->hWindow, stQuickOutputAttr->bQuickEnable);
            }

            break;
        }
        case CMD_WIN_GET_QUICK:
        {
            WIN_SET_QUICK_S * pstQuickOutputAttr = (WIN_SET_QUICK_S*)arg;
            Ret = WIN_CheckHanlde(pstQuickOutputAttr->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetQuick(pstQuickOutputAttr->hWindow, &pstQuickOutputAttr->bQuickEnable);
            }

            break;
        }

        case CMD_WIN_SUSPEND:
        {
            Ret = WIN_Suspend();
            break;
        }

        case CMD_WIN_RESUM:
        {
            Ret = WIN_Resume();
            break;
        }

        case CMD_WIN_GET_HANDLE:
        {
            WIN_HANDLE_ARRAY_S stWinArray;
            WIN_GET_HANDLE_S *pWin = (WIN_GET_HANDLE_S*)arg;

            Ret = Win_DebugGetHandle(pWin->enDisp, &stWinArray);
            if (Ret == HI_SUCCESS)
            {
                HI_S32 i;

                pWin->u32WinNumber = stWinArray.u32WinNumber;

                for (i=0; (i<pWin->u32WinNumber) && (i<DEF_MAX_WIN_NUM_ON_SINGLE_DISP); i++)
                {
                    pWin->ahWinHandle[i] = stWinArray.ahWinHandle[i];
                }
            }
            break;
        }

        case CMD_WIN_ATTACH:
        {
            WIN_ATTACH_S *pstAttach;
            pstAttach = (WIN_ATTACH_S *)arg;

            if (pstAttach->enType == ATTACH_TYPE_SINK)
            {
                Ret = WIN_AttachSink(pstAttach->hWindow, pstAttach->hMutual);
            }
            else
            {
                Ret = HI_FAILURE;
            }
            break;
        }

        case CMD_WIN_DETACH:
        {
            WIN_ATTACH_S *pstAttach;
            pstAttach = (WIN_ATTACH_S *)arg;

            if (pstAttach->enType == ATTACH_TYPE_SINK)
            {
                Ret = WIN_DetachSink(pstAttach->hWindow, pstAttach->hMutual);
            }
            else
            {
                Ret = HI_FAILURE;
            }
            break;
        }

        case CMD_WIN_GET_LATESTFRAME_INFO:
        {
            WIN_FRAME_S *frame_info;
            frame_info = (WIN_FRAME_S *)arg;
            Ret = WinGetCurrentImg(frame_info->hWindow, &frame_info->stFrame);
            break;
        }
        case CMD_VO_WIN_CAPTURE_START:
        {
            WIN_CAPTURE_S *capture_info = HI_NULL;
            HI_DRV_DISPLAY_E enDisp;
            HI_U32 u32WinIndex;

            capture_info = (WIN_CAPTURE_S *)arg;

            (HI_VOID)WinGetIndex(capture_info->hWindow, &enDisp, &u32WinIndex);
            pstWinState->hCapture[enDisp][u32WinIndex] = capture_info->hWindow;


            Ret = WinCapturePause(capture_info->hWindow, 1);
            if (HI_SUCCESS != Ret) {
                pstWinState->hCapture[enDisp][u32WinIndex] = 0;
                break;
            }

            Ret = WinCaptureFrame(capture_info->hWindow,
                                    &capture_info->CapPicture,
                                    &capture_info->driver_supply_addr.startPhyAddr,
                                    &capture_info->driver_supply_addr.length);

            if (HI_SUCCESS != Ret) {
                pstWinState->hCapture[enDisp][u32WinIndex] = 0;
                WinCapturePause(capture_info->hWindow, 0);
            }

            break;
        }
        case CMD_VO_WIN_CAPTURE_RELEASE:
        {
            WIN_CAPTURE_S *capture_info = HI_NULL;
            HI_DRV_DISPLAY_E enDisp;
            HI_U32 u32WinIndex;

            capture_info = (WIN_CAPTURE_S *)arg;
            (HI_VOID)WinGetIndex(capture_info->hWindow, &enDisp, &u32WinIndex);

            Ret = WinReleaseCaptureFrame(capture_info->hWindow, &capture_info->CapPicture);
            if (HI_SUCCESS != Ret)
                pstWinState->hCapture[enDisp][u32WinIndex] = 0;

            Ret = WinCapturePause(capture_info->hWindow, 0);
            if (HI_SUCCESS != Ret)
                pstWinState->hCapture[enDisp][u32WinIndex] = 0;

            break;
        }
        case CMD_VO_WIN_CAPTURE_FREE:
        {
            WIN_CAPTURE_S *capture_info = HI_NULL;
            HI_DRV_DISPLAY_E enDisp;
            HI_U32 u32WinIndex;

            capture_info = (WIN_CAPTURE_S *)arg;

            (HI_VOID)WinGetIndex(capture_info->hWindow, &enDisp, &u32WinIndex);
            pstWinState->hCapture[enDisp][u32WinIndex] = 0;

            Ret = WinFreeCaptureMMZBuf(capture_info->hWindow, &capture_info->CapPicture);
            break;
        }
        case CMD_WIN_SET_ROTATION:
        {
            WIN_ROTATION_S *pVoWinRotation;
            pVoWinRotation = (WIN_ROTATION_S *)arg;
            Ret = WIN_CheckHanlde(pVoWinRotation->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetRotation(pVoWinRotation->hWindow, pVoWinRotation->enRotation);
            }
            break;
        }
        case CMD_WIN_GET_ROTATION:
        {
            WIN_ROTATION_S *pVoWinRotation;
            pVoWinRotation = (WIN_ROTATION_S *)arg;
            Ret = WIN_CheckHanlde(pVoWinRotation->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetRotation(pVoWinRotation->hWindow, &(pVoWinRotation->enRotation));
            }
            break;
        }
        case CMD_WIN_SET_FLIP:
        {
            WIN_FLIP_S *pVoWinFlip;

            pVoWinFlip = (WIN_FLIP_S *)arg;
            Ret = WIN_CheckHanlde(pVoWinFlip->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_SetFlip(pVoWinFlip->hWindow, pVoWinFlip->bHoriFlip,pVoWinFlip->bVertFlip);
            }
            break;
        }
        case CMD_WIN_GET_FLIP:
        {
            WIN_FLIP_S *pVoWinFlip;
            pVoWinFlip = (WIN_FLIP_S *)arg;
            Ret = WIN_CheckHanlde(pVoWinFlip->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetFlip(pVoWinFlip->hWindow,
                                    &(pVoWinFlip->bHoriFlip),
                                    &(pVoWinFlip->bVertFlip));
            }
            break;
        }
        case CMD_WIN_GET_UNLOAD:
        {
            WIN_UNLOAD_S *pWinUnload;
            pWinUnload = (WIN_UNLOAD_S *)arg;
            Ret = WIN_CheckHanlde(pWinUnload->hWindow, pstWinState);
            if (HI_SUCCESS == Ret)
            {
                Ret = WIN_GetUnload(pWinUnload->hWindow,
                                    &(pWinUnload->u32Times));
            }
            break;
        }
        default:

        up(&g_VoMutex);
        return -ENOIOCTLCMD;
    }

    return Ret;
}

extern HI_U32 DISP_Get_CountStatus(HI_VOID);

HI_S32 DRV_WIN_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32  Ret;

    Ret = down_trylock(&g_VoMutex);
    if (Ret)
    {
        HI_FATAL_WIN("down g_VoMutex failed.\n");
        return -1;
    }

    /* just return if no task opened the vo device.*/
#if 0
    if( !atomic_read(&g_VoCount) )
    {
        up(&g_VoMutex);
        return 0;
    }
#endif

    g_VoSuspend = HI_TRUE;
    WIN_Suspend();

    msleep(50);

    HI_PRINT("VO suspend OK\n");

    up(&g_VoMutex);
    return 0;
}

HI_S32 DRV_WIN_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32  Ret;

    Ret = down_trylock(&g_VoMutex);
    if (Ret)
    {
        HI_FATAL_WIN("down g_VoMutex failed.\n");
        return -1;
    }

#if 0
    if(!atomic_read(&g_VoCount))
    {
        up(&g_VoMutex);
        return 0;
    }
#endif

    WIN_Resume();
    g_VoSuspend = HI_FALSE;

    HI_PRINT("VO resume OK\n");

    up(&g_VoMutex);
    return 0;
}

HI_S32 DRV_WIN_Process(HI_U32 cmd, HI_VOID *arg)
{
    WIN_STATE_S   *pstWinState;
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    pstWinState = &g_VoModState;

    Ret = WIN_ProcessCmd(cmd, arg, pstWinState);

    up(&g_VoMutex);
    return Ret;
}




/*add for mce interface*/
HI_S32  HI_DRV_WIN_Create(HI_DRV_WIN_ATTR_S *pWinAttr, HI_HANDLE *phWindow)
{
    HI_S32 Ret;
    WIN_CREATE_S voWinCreate;

    voWinCreate.hWindow = *phWindow;
    voWinCreate.WinAttr = *pWinAttr;
    Ret = DRV_WIN_Process(CMD_WIN_CREATE, &voWinCreate);
    *phWindow = voWinCreate.hWindow;
    return Ret;
}

HI_S32 HI_DRV_WIN_Destroy(HI_HANDLE hWindow)
{
    HI_S32 Ret;

    Ret = DRV_WIN_Process(CMD_WIN_DESTROY, &hWindow);
    return Ret;
}

HI_S32 HI_DRV_WIN_SetEnable(HI_HANDLE hWindow, HI_BOOL bEnable)
{
    HI_S32 Ret;
    WIN_ENABLE_S   enVoWinEnable;

    enVoWinEnable.bEnable = bEnable;
    enVoWinEnable.hWindow = hWindow;

    Ret = DRV_WIN_Process(CMD_WIN_SET_ENABLE, &enVoWinEnable);
    return Ret;
}

HI_S32 HI_DRV_WIN_GetEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable)
{
    HI_S32 Ret;
    WIN_ENABLE_S   enVoWinEnable;

    memset(&enVoWinEnable, 0, sizeof(WIN_ENABLE_S));
   // enVoWinEnable.bEnable = bEnable;
    enVoWinEnable.hWindow = hWindow;

    Ret = DRV_WIN_Process(CMD_WIN_GET_ENABLE, &enVoWinEnable);
    if(!Ret)
    {
        *pbEnable = enVoWinEnable.bEnable;
    }
    return Ret;
}


HI_S32 HI_DRV_WIN_SetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc)
{
    HI_S32 Ret;
    WIN_SOURCE_S VoWinAttach;

    VoWinAttach.hWindow = hWindow;
    VoWinAttach.stSrc.hSrc   = pstSrc->hSrc;
    VoWinAttach.stSrc.hSecondSrc = pstSrc->hSecondSrc;
    VoWinAttach.stSrc.enSrcMode = pstSrc->enSrcMode;

    Ret = DRV_WIN_Process(CMD_WIN_SET_SOURCE, &VoWinAttach);
    return Ret;
}

HI_S32 HI_DRV_WIN_GetSource(HI_HANDLE hWindow, HI_DRV_WIN_SRC_HANDLE_S *pstSrc)
{
    HI_S32 Ret = HI_SUCCESS;

#if 0
    WIN_SOURCE_S   stWinSrc;

    memset(&stWinSrc, 0, sizeof(WIN_SOURCE_S));
    stWinSrc.hWindow = hWin;
    Ret = DRV_WIN_Process(CMD_WIN_GET_SOURCE, &stWinSrc);
    if(!Ret)
    {
        pstSrc->hSrc = stWinSrc.stSrc.hSrc;
        pstSrc->hSecondSrc = stWinSrc.stSrc.hSecondSrc;
        Ret = WIN_GetAttachFunc(pstSrc);
    }
#endif
    return Ret;
}


HI_S32 HI_DRV_WIN_Reset(HI_HANDLE hWindow, HI_DRV_WIN_SWITCH_E enSwitch)
{
    HI_S32 Ret;
    WIN_RESET_S   VoWinReset;

    VoWinReset.hWindow = hWindow;
    VoWinReset.eMode   = enSwitch;
    Ret = DRV_WIN_Process(CMD_WIN_RESET, &VoWinReset);
    return Ret;
}

HI_S32 HI_DRV_WIN_Pause(HI_HANDLE hWin, HI_BOOL bEnable)
{
    HI_S32 Ret;
    WIN_PAUSE_S   stWinPause;

    stWinPause.hWindow = hWin;
    stWinPause.bEnable = bEnable;
    Ret = DRV_WIN_Process(CMD_WIN_PAUSE, &stWinPause);
    return Ret;
}

HI_S32 HI_DRV_WIN_SetStepMode(HI_HANDLE hWin, HI_BOOL bStepMode)
{
    HI_S32 Ret;
    WIN_STEP_MODE_S   stMode;

    stMode.hWindow = hWin;
    stMode.bStep = bStepMode;
    Ret = DRV_WIN_Process(CMD_WIN_STEP_MODE, &bStepMode);
    return Ret;
}

HI_S32 HI_DRV_WIN_SetStepPlay(HI_HANDLE hWin)
{

    return DRV_WIN_Process(CMD_WIN_STEP_PLAY, &hWin);

}

HI_S32 HI_DRV_WIN_SetExtBuffer(HI_HANDLE hWin, HI_DRV_VIDEO_BUFFER_POOL_S* pstBuf)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_VoMutex);

    Ret  = WIN_SetExtBuffer(hWin, pstBuf);

    up(&g_VoMutex);
    return Ret;
}

HI_S32 HI_DRV_WIN_AcquireFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_VoMutex);

    Ret  = WIN_AcquireFrame(hWin, pFrameinfo);

    up(&g_VoMutex);
    return Ret;
}

HI_S32 HI_DRV_WIN_ReleaseFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_VoMutex);

    Ret  = WIN_ReleaseFrame(hWin, pFrameinfo);

    up(&g_VoMutex);
    return Ret;
}

HI_S32 HI_DRV_WIN_SetQuick(HI_HANDLE hWin, HI_BOOL bEnable)
{
    HI_S32 Ret;
    WIN_SET_QUICK_S   stQuik;

    stQuik.hWindow = hWin;
    stQuik.bQuickEnable = bEnable;
    Ret = DRV_WIN_Process(CMD_WIN_SET_QUICK, &stQuik);
    return Ret;
}

HI_S32 Make_CaptuerFrame(HI_DRV_VIDEO_FRAME_S *pFrameinfo,HI_DRV_VIDEO_FRAME_S *pCapFrameinfo)
{
    HI_U32 datalen = 0, y_stride = 0, height = 0 ;
    HI_U32 u32StillFrameStartPhyAddr,u32FrameStartPhyAddr;
    HI_U8  *pu8StillFrameStartVirAddr = HI_NULL,*pu8FrameStartVirAddr = HI_NULL;
    HI_S32 nRet = 0;
    DISP_MMZ_BUF_S  stMMZ_Frame;

    if (!pCapFrameinfo)
    {
        HI_ERR_WIN("para null!\n");
        return HI_FAILURE;
    }
    if (!pFrameinfo)
    {
        HI_ERR_WIN("para null!\n");
        return HI_FAILURE;
    }

    /*1:calculate data long*/
    y_stride = pFrameinfo->stBufAddr[0].u32Stride_Y;
    height   = pFrameinfo->u32Height;

    if ( HI_DRV_PIX_FMT_NV21 == pFrameinfo->ePixFormat)
	{
        datalen = height * y_stride * 3 / 2 + height * 4;
	}
    else if (HI_DRV_PIX_FMT_NV21_CMP == pFrameinfo->ePixFormat)
	{
        datalen = height * y_stride * 3 / 2 + 16 * height * 3 / 2;
	}
	else if (HI_DRV_PIX_FMT_NV61_2X1 == pFrameinfo->ePixFormat)
	{
		datalen = height * y_stride * 2 + height * 4;
	}
	else
	{
		HI_ERR_WIN("Invalid Format %d!\n",pFrameinfo->ePixFormat);
		return HI_FAILURE;
	}

    /*2: creat still frame*/
    u32StillFrameStartPhyAddr = pCapFrameinfo->stBufAddr[0].u32PhyAddr_Y;
    u32FrameStartPhyAddr = pFrameinfo->stBufAddr[0].u32PhyAddr_Y;

    memset(&stMMZ_Frame,0,sizeof(DISP_MMZ_BUF_S));

    stMMZ_Frame.u32StartPhyAddr = pCapFrameinfo->stBufAddr[0].u32PhyAddr_Y;

    nRet = DISP_OS_MMZ_Map(&stMMZ_Frame);
    if (HI_SUCCESS != nRet)
    {
        return HI_ERR_VO_MALLOC_FAILED;
    }
	
	pu8StillFrameStartVirAddr = stMMZ_Frame.pu8StartVirAddr;
		
#ifdef	CFG_VDP_MMU_SUPPORT
	memset(&stMMZ_Frame,0,sizeof(DISP_MMZ_BUF_S));
    stMMZ_Frame.u32StartPhyAddr = u32FrameStartPhyAddr;

    nRet = DISP_OS_MMZ_Map(&stMMZ_Frame);
    if (HI_SUCCESS != nRet)
    {
        return HI_ERR_VO_MALLOC_FAILED;
    }
	pu8FrameStartVirAddr = stMMZ_Frame.pu8StartVirAddr;
#else
    pu8FrameStartVirAddr =(HI_U8*) phys_to_virt (u32FrameStartPhyAddr);
#endif
    memcpy(pCapFrameinfo,pFrameinfo,sizeof(HI_DRV_VIDEO_FRAME_S));
    memcpy((void *)pu8StillFrameStartVirAddr, (void *)pu8FrameStartVirAddr,datalen);


    /*3: calculate  frame addr*/
    pCapFrameinfo->stBufAddr[0].u32PhyAddr_YHead = u32StillFrameStartPhyAddr +(pFrameinfo->stBufAddr[0].u32PhyAddr_YHead -u32FrameStartPhyAddr);
    pCapFrameinfo->stBufAddr[0].u32Stride_Y =  pFrameinfo->stBufAddr[0].u32Stride_Y;

    pCapFrameinfo->stBufAddr[0].u32PhyAddr_Y = u32StillFrameStartPhyAddr +(pFrameinfo->stBufAddr[0].u32PhyAddr_Y - u32FrameStartPhyAddr);


    pCapFrameinfo->stBufAddr[0].u32PhyAddr_C = u32StillFrameStartPhyAddr + (pCapFrameinfo->u32Height*pCapFrameinfo->stBufAddr[0].u32Stride_Y);

    pCapFrameinfo->stBufAddr[0].u32PhyAddr_CrHead = u32StillFrameStartPhyAddr +( pFrameinfo->stBufAddr[0].u32PhyAddr_CrHead -u32FrameStartPhyAddr);
    pCapFrameinfo->stBufAddr[0].u32PhyAddr_Cr = u32StillFrameStartPhyAddr +( pFrameinfo->stBufAddr[0].u32PhyAddr_Cr -u32FrameStartPhyAddr );
    pCapFrameinfo->stBufAddr[0].u32Stride_Cr = pFrameinfo->stBufAddr[0].u32Stride_Cr;
	
#ifdef	CFG_VDP_MMU_SUPPORT
	memset(&stMMZ_Frame,0,sizeof(DISP_MMZ_BUF_S));

	stMMZ_Frame.pu8StartVirAddr = pu8FrameStartVirAddr;
	DISP_OS_MMZ_UnMap(&stMMZ_Frame);
	
	stMMZ_Frame.pu8StartVirAddr = pu8StillFrameStartVirAddr;
	DISP_OS_MMZ_UnMap(&stMMZ_Frame);	
#else
    DISP_OS_MMZ_UnMap(&stMMZ_Frame);
#endif
    return HI_SUCCESS;
}

HI_S32 HI_DRV_WIN_CapturePicture(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic)
{
    HI_S32 Ret;
    WIN_CAPTURE_S   stCapture;

    if (!pstPic)
    {
        HI_ERR_WIN("para null!\n");
        return HI_FAILURE;
    }

    stCapture.hWindow = hWin;

    Ret = DRV_WIN_Process(CMD_VO_WIN_CAPTURE_START, &stCapture);
    if (HI_SUCCESS != Ret)
    {
        WIN_ERROR("capture err!\n");
        return Ret;
    }

    stCapture.hWindow = hWin;
    pstPic->stBufAddr[0].u32PhyAddr_Y = stCapture.driver_supply_addr.startPhyAddr;

    (HI_VOID)Make_CaptuerFrame(&(stCapture.CapPicture),pstPic);

    stCapture.hWindow = hWin;
    Ret = DRV_WIN_Process(CMD_VO_WIN_CAPTURE_RELEASE, &stCapture);
    if (HI_SUCCESS != Ret)
    {
        WIN_ERROR("capture RELEASE err!\n");
    }
    return Ret;
}

HI_S32 HI_DRV_WIN_CapturePictureRelease(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pstPic)
{
    HI_S32 Ret;
    WIN_CAPTURE_S  stCapture;

    if (!pstPic)
    {
        HI_ERR_WIN("para null!\n");
        return HI_FAILURE;
    }

    stCapture.hWindow = hWin;
    stCapture.CapPicture = *pstPic;

    Ret = DRV_WIN_Process(CMD_VO_WIN_CAPTURE_FREE, &stCapture);

    return Ret;
}

HI_S32 HI_DRV_WIN_SetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E enRotation)
{
    return HI_SUCCESS;
}

HI_S32 HI_DRV_WIN_GetRotation(HI_HANDLE hWin, HI_DRV_ROT_ANGLE_E *penRotation)
{
    return HI_SUCCESS;
}

HI_S32 HI_DRV_WIN_SetFlip(HI_HANDLE hWin, HI_BOOL bHoriFlip, HI_BOOL bVertFlip)
{
    return HI_SUCCESS;
}

HI_S32 HI_DRV_WIN_GetFlip(HI_HANDLE hWin, HI_BOOL *pbHoriFlip, HI_BOOL *pbVertFlip)
{
    return HI_SUCCESS;
}

HI_S32 HI_DRV_WIN_SendFrame(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *pFrameinfo)
{
    HI_S32 Ret;
    Ret = WIN_SendFrame(hWin,pFrameinfo);
    return Ret;
}


HI_S32 HI_DRV_WIN_SetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32 Ret;
    WIN_CREATE_S   stWinAttr;

    stWinAttr.hWindow = hWin;
    stWinAttr.WinAttr = *pWinAttr;
    Ret = DRV_WIN_Process(CMD_WIN_SET_ATTR, &stWinAttr);
    return Ret;
}

HI_S32 HI_DRV_WIN_GetAttr(HI_HANDLE hWin, HI_DRV_WIN_ATTR_S *pWinAttr)
{
    HI_S32 Ret;
    WIN_CREATE_S   stWinAttr;

    memset(&stWinAttr, 0, sizeof(WIN_CREATE_S));
    stWinAttr.hWindow = hWin;
   // stWinAttr.WinAttr = *pWinAttr;
    Ret = DRV_WIN_Process(CMD_WIN_GET_ATTR, &stWinAttr);
    if(!Ret)
    {
        *pWinAttr = stWinAttr.WinAttr;
    }
    return Ret;
}

HI_S32 HI_DRV_WIN_GetDelayMs(HI_HANDLE hWindow, HI_U32 *pu32DelayTime)
{
	HI_S32 Ret;
	HI_DRV_WIN_PLAY_INFO_S		WinPlayInfo;

	Ret = HI_DRV_WIN_GetPlayInfo(hWindow, &WinPlayInfo);
	if (HI_SUCCESS == Ret)
	{
		*pu32DelayTime =  WinPlayInfo.u32DelayTime;
	}

	return Ret;
}

HI_S32 HI_DRV_WIN_GetPlayInfo(HI_HANDLE hWindow, HI_DRV_WIN_PLAY_INFO_S *pInfo)
{
    HI_S32 Ret;
    WIN_PLAY_INFO_S WinPlayInfo;

    memset(&WinPlayInfo, 0, sizeof(WIN_PLAY_INFO_S));
    WinPlayInfo.hWindow = hWindow;

    Ret = DRV_WIN_Process(CMD_WIN_GET_PLAY_INFO, &WinPlayInfo);
    *pInfo = WinPlayInfo.stPlayInfo;
    return Ret;
}

HI_S32 HI_DRV_WIN_GetInfo(HI_HANDLE hWindow, HI_DRV_WIN_INFO_S *pInfo)
{
    HI_S32 Ret;
    WIN_PRIV_INFO_S WinPrivInfo;

    memset(&WinPrivInfo, 0, sizeof(WIN_PRIV_INFO_S));
    WinPrivInfo.hWindow = hWindow;

    Ret = DRV_WIN_Process(CMD_WIN_GET_INFO, &WinPrivInfo);
    *pInfo = WinPrivInfo.stPrivInfo;

    return Ret;
}

HI_S32 DRV_WIN_SendFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;
    WIN_FRAME_S *pstWinFrame = HI_NULL;

	if (!pFrame)
    {
        HI_ERR_WIN("para null!\n");
        return HI_FAILURE;
    }
	
	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}

	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));
    pstWinFrame->hWindow = hWindow;
    pstWinFrame->stFrame = *pFrame;
	
    Ret = DRV_WIN_Process(CMD_WIN_SEND_FRAME, pstWinFrame);

	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}


HI_S32 HI_DRV_WIN_QFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;
    WIN_FRAME_S *pstWinFrame = HI_NULL;

	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}

	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));
    pstWinFrame->hWindow = hWindow;
    pstWinFrame->stFrame = *pFrame;

    Ret = DRV_WIN_Process(CMD_WIN_QU_FRAME, pstWinFrame);
	
	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}

HI_S32 HI_DRV_WIN_QULSFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;	
    WIN_FRAME_S *pstWinFrame = HI_NULL;

	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}

	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));
    pstWinFrame->hWindow = hWindow;
    pstWinFrame->stFrame = *pFrame;

    Ret = DRV_WIN_Process(CMD_WIN_QU_ULSFRAME, pstWinFrame);

	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}

HI_S32 HI_DRV_WIN_DQFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;
    WIN_FRAME_S *pstWinFrame = HI_NULL;

	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}

	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));
    pstWinFrame->hWindow = hWindow;
	
    Ret = DRV_WIN_Process(CMD_WIN_DQ_FRAME, pstWinFrame);
    if (HI_SUCCESS == Ret)
    {
        *pFrame = pstWinFrame->stFrame;
    }

	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}

HI_S32 DRV_WIN_AcquireFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;
    WIN_FRAME_S *pstWinFrame = HI_NULL;

	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}
	
	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));	
    pstWinFrame->hWindow = hWindow;
	
    Ret = DRV_WIN_Process(CMD_WIN_VIR_ACQUIRE, pstWinFrame);
	if (HI_SUCCESS == Ret)
	{
		 *pFrame = pstWinFrame->stFrame;
	}

	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}

HI_S32 DRV_WIN_ReleaseFrame(HI_HANDLE hWindow, HI_DRV_VIDEO_FRAME_S *pFrame)
{
    HI_S32     Ret = 0;
	WIN_FRAME_S *pstWinFrame = HI_NULL;

	pstWinFrame = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pstWinFrame)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}

	memset(pstWinFrame, 0, sizeof(WIN_FRAME_S));
	
    pstWinFrame->hWindow = hWindow;
    pstWinFrame->stFrame = *pFrame;
	
    Ret = DRV_WIN_Process(CMD_WIN_VIR_RELEASE, pstWinFrame);

	HI_KFREE(HI_ID_VO,pstWinFrame);
	pstWinFrame = HI_NULL;
	
    return Ret;
}


HI_S32 HI_DRV_WIN_SetZorder(HI_HANDLE hWin, HI_DRV_DISP_ZORDER_E ZFlag)
{
    HI_S32     Ret;
    WIN_ZORDER_S stWinZorder;

    stWinZorder.hWindow = hWin;
    stWinZorder.eZFlag = ZFlag;
    Ret = DRV_WIN_Process(CMD_WIN_SET_ZORDER, &stWinZorder);
    return Ret;
}

HI_S32 HI_DRV_WIN_GetZorder(HI_HANDLE hWin, HI_U32 *pu32Zorder)
{
    HI_S32     Ret;
    WIN_ORDER_S stWinOrder;

    memset(&stWinOrder, 0, sizeof(WIN_ORDER_S));
    stWinOrder.hWindow = hWin;

    Ret = DRV_WIN_Process(CMD_WIN_SET_ZORDER, &stWinOrder);
    if(!Ret)
    {
        *pu32Zorder = stWinOrder.Order ;
    }
    return Ret;
}

HI_S32 HI_DRV_WIN_Freeze(HI_HANDLE hWin, HI_BOOL bEnable,  HI_DRV_WIN_SWITCH_E eRst)
{
    HI_S32     Ret;
    WIN_FREEZE_S stWinFreeze;

    stWinFreeze.hWindow = hWin;
    stWinFreeze.bEnable = bEnable;
    stWinFreeze.eMode = eRst;

    Ret = DRV_WIN_Process(CMD_WIN_FREEZE, &stWinFreeze);

    return Ret;
}

HI_S32 HI_DRV_WIN_GetLatestFrameInfo(HI_HANDLE hWin, HI_DRV_VIDEO_FRAME_S *frame_info)
{
    HI_S32     Ret = 0;
    WIN_FRAME_S  *pframe_info_ioctl = HI_NULL;


	pframe_info_ioctl = HI_KMALLOC(HI_ID_VO, sizeof(WIN_FRAME_S), GFP_KERNEL);
	if (HI_NULL == pframe_info_ioctl)
	{
		HI_ERR_WIN("kmalloc mem failed!\n");
		return HI_ERR_VO_MALLOC_FAILED;
	}
	
    memset(pframe_info_ioctl, 0, sizeof(WIN_FRAME_S));
	
    pframe_info_ioctl->hWindow = hWin;
	
    Ret = DRV_WIN_Process(CMD_WIN_GET_LATESTFRAME_INFO, pframe_info_ioctl);
    if (HI_SUCCESS == Ret)
    {
		*frame_info = pframe_info_ioctl->stFrame;
    }

	HI_KFREE(HI_ID_VO,pframe_info_ioctl);
	pframe_info_ioctl = HI_NULL;
	
    return Ret;
}


HI_S32 WIN_DRV_DestroyAll(WIN_STATE_S *pst2WinState)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 i;

    for(enDisp=0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
    {
        for(i=0; i < DEF_MAX_WIN_NUM_ON_SINGLE_DISP; i++)
        {
            if (pst2WinState->hWin[enDisp][i] != HI_NULL)
            {
                WIN_DestroyExt(pst2WinState->hWin[enDisp][i], pst2WinState);
            }
        }
    }

    for(i=0; i<DEF_MAX_WIN_NUM_ON_VIRTUAL_DISP; i++)
    {
        if (pst2WinState->hVirtualWin[i] != HI_NULL)
        {
            WIN_DestroyExt(pst2WinState->hVirtualWin[i], pst2WinState);
        }
    }

    return HI_SUCCESS;
}



HI_S32 HI_DRV_WIN_Init(HI_VOID)
{
    HI_S32 i, j, Ret;

    Ret = down_interruptible(&g_VoMutex);

    for (i=HI_DRV_DISPLAY_0; i<HI_DRV_DISPLAY_BUTT; i++)
    {
        for (j=0; j<DEF_MAX_WIN_NUM_ON_SINGLE_DISP; j++)
        {
            g_VoModState.hWin[i][j] = HI_NULL;
        }
    }

    for (j=0; j<DEF_MAX_WIN_NUM_ON_VIRTUAL_DISP; j++)
    {
        g_VoModState.hVirtualWin[j] = HI_NULL;
    }

    if (1 == atomic_inc_return(&g_VoCount))
    {
        Ret = WIN_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_WIN("call VO_Init failed.\n");
            atomic_dec(&g_VoCount);
            up(&g_VoMutex);
            return -1;
        }
    }

    up(&g_VoMutex);

    Ret = DRV_WIN_Register();
    if (Ret != HI_SUCCESS)
    {
        HI_FATAL_WIN("call DRV_WIN_Register failed.\n");
        return -1;
    }

    return 0;
}



HI_S32 HI_DRV_WIN_DeInit(HI_VOID)
{
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    WIN_DRV_DestroyAll(&g_VoModState);

    if (atomic_dec_and_test(&g_VoCount))
    {
        WIN_DeInit();
    }

    up(&g_VoMutex);
    return 0;
}



static WIN_EXPORT_FUNC_S s_stWinExportFuncs = {
    .pfnWinInit      = HI_DRV_WIN_Init,
    .pfnWinDeInit    = HI_DRV_WIN_DeInit,
    .pfnWinCreate    = HI_DRV_WIN_Create,
    .pfnWinDestory   = HI_DRV_WIN_Destroy,
    .pfnWinSetAttr   = HI_DRV_WIN_SetAttr,
    .pfnWinGetAttr   = HI_DRV_WIN_GetAttr,
    .pfnWinGetInfo   = HI_DRV_WIN_GetInfo,
    .pfnWinSetSrc    = HI_DRV_WIN_SetSource,
    .pfnWinGetSrc    = HI_DRV_WIN_GetSource,
    .pfnWinSetEnable = HI_DRV_WIN_SetEnable,
    .pfnWinGetEnable = HI_DRV_WIN_GetEnable,
    .pfnWinQueueFrm  = HI_DRV_WIN_QFrame,
    .pWinQueueUselessFrm = HI_DRV_WIN_QULSFrame,
    .pfnWinDequeueFrm  = HI_DRV_WIN_DQFrame,
    .pfnWinGetPlayInfo = HI_DRV_WIN_GetPlayInfo,
    .pfnWinSetZorder = HI_DRV_WIN_SetZorder,
    .pfnWinGetZorder = HI_DRV_WIN_GetZorder,
    .pfnWinFreeze    = HI_DRV_WIN_Freeze,
    .pfnWinGetLatestFrameInfo = HI_DRV_WIN_GetLatestFrameInfo,
    .pfnWinReset     = HI_DRV_WIN_Reset,
    .pfnWinPause     = HI_DRV_WIN_Pause,
    .pfnWinSetStepMode = HI_DRV_WIN_SetStepMode,
    .pfnWinSetStepPlay = HI_DRV_WIN_SetStepPlay,
    .pfnWinSetExtBuffer= HI_DRV_WIN_SetExtBuffer,
    .pfnWinAcquireFrm  = HI_DRV_WIN_AcquireFrame,
    .pfnWinRlsFrm      = HI_DRV_WIN_ReleaseFrame,
//    .pfnWin3DMode      = HI_DRV_WIN_Set3DMode,
    .pfnWinSetQuik   = HI_DRV_WIN_SetQuick,
    .pfnWinCapturePic  = HI_DRV_WIN_CapturePicture,
    .pfnWinCapturePicRls = HI_DRV_WIN_CapturePictureRelease,
    .pfnWinSetRotation = HI_DRV_WIN_SetRotation,
    .pfnWinGetRotation = HI_DRV_WIN_GetRotation,
    .pfnWinSetFlip   = HI_DRV_WIN_SetFlip,
    .pfnWinGetFlip   = HI_DRV_WIN_GetFlip,
    .pfnWinSendFrm   = HI_DRV_WIN_SendFrame,
    .pfnWinResume = DRV_WIN_Resume,
    .pfnWinSuspend = DRV_WIN_Suspend,
};

HI_S32 DRV_WIN_Register(HI_VOID)
{
    HI_S32  Ret;

    Ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_VO, "HI_VO", (HI_VOID *)(&s_stWinExportFuncs));
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_WIN("HI_DRV_MODULE_Register VO failed\n");
        return Ret;
    }

    g_VoSuspend = HI_FALSE;

    return  0;
}

HI_VOID DRV_WIN_UnRegister(HI_VOID)
{

    HI_DRV_WIN_DeInit();

    HI_DRV_MODULE_UnRegister(HI_ID_VO);

    return;
}



HI_S32 WIN_DRV_Open(struct inode *finode, struct file  *ffile)
{
    WIN_STATE_S *pWinState = HI_NULL;
    HI_S32     Ret;

    Ret = down_interruptible(&g_VoMutex);

    pWinState = HI_KMALLOC(HI_ID_VO, sizeof(WIN_STATE_S), GFP_KERNEL);
    if (!pWinState)
    {
        WIN_FATAL("malloc pWinState failed.\n");
        up(&g_VoMutex);
        return -1;
    }

    memset(pWinState, 0, sizeof(WIN_STATE_S));

    if (1 == atomic_inc_return(&g_VoCount))
    {
        Ret = WIN_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_KFREE(HI_ID_VO, pWinState);
            WIN_FATAL("call VO_Init failed.\n");
            atomic_dec(&g_VoCount);
            up(&g_VoMutex);
            return -1;
        }
    }

    ffile->private_data = pWinState;

    up(&g_VoMutex);
    return 0;
}

HI_S32 WIN_DRV_Close(struct inode *finode, struct file  *ffile)
{
    WIN_STATE_S    *pVoState;
    HI_S32        Ret;
    HI_U32        i = 0, j = 0;

    Ret = down_interruptible(&g_VoMutex);
    pVoState = ffile->private_data;

    /*first close all the services belongs to the window.*/
    if (!pVoState)
    {
        WIN_FATAL(" pVoState is null.\n");
        return HI_FAILURE;
    }

    for (i = 0; i < HI_DRV_DISPLAY_BUTT; i++)
    {
        for (j = 0; j < DEF_MAX_WIN_NUM_ON_SINGLE_DISP; j++)
        {
            if (pVoState->hCapture[i][j] != 0)
                WinForceClearCapture(pVoState->hCapture[i][j]);
        }
    }
    WIN_DRV_DestroyAll(pVoState);

    if (atomic_dec_and_test(&g_VoCount))
        WIN_DeInit();

    HI_KFREE(HI_ID_VO, pVoState);

    up(&g_VoMutex);
    return 0;
}

HI_S32 DRV_WIN_Ioctl(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg)
{
    WIN_STATE_S   *pstWinState;
    HI_S32        Ret;

    Ret = down_interruptible(&g_VoMutex);

    pstWinState = file->private_data;

    //HI_PRINT("zzzzzzzzzzzzzzz WIN cmd = 0x%x\n", cmd);
    Ret = WIN_ProcessCmd(cmd, arg, pstWinState);

    up(&g_VoMutex);
    return Ret;
}

//#ifndef MODULE
EXPORT_SYMBOL(g_VoMutex);

EXPORT_SYMBOL(DRV_WIN_Ioctl);
EXPORT_SYMBOL(WIN_DRV_Open);
EXPORT_SYMBOL(WIN_DRV_Close);
EXPORT_SYMBOL(DRV_WIN_Suspend);
EXPORT_SYMBOL(DRV_WIN_Resume);
//EXPORT_SYMBOL(DRV_WIN_ProcRegister);
//EXPORT_SYMBOL(DRV_WIN_ProcfUnRegister);
EXPORT_SYMBOL(WIN_AcquireFrame);
EXPORT_SYMBOL(WIN_ReleaseFrame);

EXPORT_SYMBOL(HI_DRV_WIN_SetEnable);
EXPORT_SYMBOL(DRV_WIN_Register);
EXPORT_SYMBOL(DRV_WIN_UnRegister);

EXPORT_SYMBOL(HI_DRV_WIN_Init);
EXPORT_SYMBOL(HI_DRV_WIN_DeInit);
EXPORT_SYMBOL(HI_DRV_WIN_QFrame);
EXPORT_SYMBOL(HI_DRV_WIN_CapturePicture);
EXPORT_SYMBOL(HI_DRV_WIN_QULSFrame);
EXPORT_SYMBOL(DRV_WIN_Process);
EXPORT_SYMBOL(HI_DRV_WIN_Create);
EXPORT_SYMBOL(HI_DRV_WIN_Destroy);
EXPORT_SYMBOL(HI_DRV_WIN_Freeze);
EXPORT_SYMBOL(HI_DRV_WIN_Pause);

EXPORT_SYMBOL(HI_DRV_WIN_SetSource);
EXPORT_SYMBOL(HI_DRV_WIN_Reset);
EXPORT_SYMBOL(HI_DRV_WIN_GetPlayInfo);
EXPORT_SYMBOL(DRV_WIN_SendFrame);
EXPORT_SYMBOL(HI_DRV_WIN_SetZorder);
EXPORT_SYMBOL(DRV_WIN_SetSftwareStage);
//#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


