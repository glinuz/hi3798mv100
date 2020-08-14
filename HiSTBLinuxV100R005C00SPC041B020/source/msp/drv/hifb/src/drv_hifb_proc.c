/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_proc.c
Version         : Initial Draft
Author          :
Created         : 2016/08/27
Description     :
Function List   :


History         :
Date                   Author                Modification
2016/08/27            y00181162              Created file
******************************************************************************/

#ifdef CFG_HIFB_PROC_SUPPORT

/*********************************add include here******************************/
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fb.h>
#include <asm/uaccess.h>
#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>


#include "hi_drv_proc.h"
#include "drv_hifb_proc.h"
#include "drv_hifb_osr.h"
#include "drv_hifb_mem.h"
#include "hifb_drv.h"

#ifdef CFG_HIFB_FENCE_SUPPORT
#include "drv_hifb_fence.h"
#endif

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
static const HI_CHAR* s_pszFmtName[] ={
                                        "RGB565",
                                        "RGB888",
                                        "KRGB444",
                                        "KRGB555",
                                        "KRGB888",
                                        "ARGB4444",
                                        "ARGB1555",
                                        "ARGB8888",
                                        "ARGB8565",
                                        "RGBA4444",
                                        "RGBA5551",
                                        "RGBA5658",
                                        "RGBA8888",
                                        "BGR565",
                                        "BGR888",
                                        "ABGR4444",
                                        "ABGR1555",
                                        "ABGR8888",
                                        "ABGR8565",
                                        "KBGR444",
                                        "KBGR555",
                                        "KBGR888",
                                        "1BPP",
                                        "2BPP",
                                        "4BPP",
                                        "8BPP",
                                        "ACLUT44",
                                        "ACLUT88",
                                        "PUYVY",
                                        "PYUYV",
                                        "PYVYU",
                                        "YUV888",
                                        "AYUV8888",
                                        "YUVA8888",
                                        "BUTT"
                                    };


const static HI_CHAR* gs_LayerName[] ={
                                            "layer_hd_0",
                                            "layer_hd_1",
                                            "layer_hd_2",
                                            "layer_hd_3",
                                            "layer_sd_0",
                                            "layer_sd_1",
                                            "layer_sd_2",
                                            "layer_sd_3",
                                            "layer_ad_0",
                                            "layer_ad_1",
                                            "layer_ad_2",
                                            "layer_ad_3",
                                            "layer_cursor"
                                      };

#ifdef CFG_HIFB_FENCE_SUPPORT
   extern HIFB_SYNC_INFO_S gs_SyncInfo;
#endif

/******************************* API declaration *****************************/
extern HI_VOID HI_UNF_HIFB_CaptureImgFromLayer(HI_U32 u32LayerID, HI_BOOL bAlphaEnable);
extern HI_VOID HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_BOOL CaptureImgEnable);


static HI_VOID HIFB_CreateProc(HI_U32 LayerID);
static HI_VOID HIFB_DestoryProc(HI_U32 LayerID);

static HI_S32 DRV_HIFB_PrintProc(struct seq_file *p, HI_VOID *v);
static HI_S32 DRV_HIFB_WriteProc(struct file * file,const char __user * buf, size_t count, loff_t *ppos);
static HI_VOID HIFB_ParseProcEchoCmd(HI_U32 u32LayerId, HI_CHAR *pCmd);


static HI_S32 HIFB_PrintWbcProc(struct fb_info * info, struct seq_file *p, HI_VOID *v);
static HI_S32 HIFB_PrintHDProc(struct fb_info * info, struct seq_file *p, HI_VOID *v);
static inline HI_VOID HIFB_PROC_SetAlpha(HI_U32 u32LayerId, HI_CHAR *pCmd);
static inline HI_U32 HIFB_PROC_GetAlpha(HI_CHAR *pStr);
static inline HI_VOID HIFB_PROC_OpenLayer(HIFB_PAR_S *pstPar);
static inline HI_VOID HIFB_PROC_CloseLayer(HIFB_PAR_S *pstPar);
static inline HI_VOID HIFB_PrintHwcRefreshStatus(struct fb_info *pstFbInfo, struct seq_file *pstSeqFile);



/******************************* API relese **********************************/
HI_S32 DRV_HIFB_CreateProc(HI_U32 LayerID)
{
    HI_S32 Ret = HI_SUCCESS;
    HIFB_SLVLAYER_DATA_S stWbcLayerInfo;
    struct fb_info *info = NULL;
    HIFB_PAR_S *par = NULL;

    info = s_stLayer[LayerID].pstInfo;
    par  = (HIFB_PAR_S *)(info->par);

    HIFB_CreateProc(LayerID);

    HI_GFX_Memset(&stWbcLayerInfo,0,sizeof(HIFB_SLVLAYER_DATA_S));
    Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_GetSlvLayerInfo(&stWbcLayerInfo);
    if (HI_SUCCESS != Ret){
        return HI_FAILURE;
    }

    par->stProcInfo.WbcLayerID = stWbcLayerInfo.enLayerID;
    HIFB_CreateProc(stWbcLayerInfo.enLayerID);

    return HI_SUCCESS;
}


static HI_VOID HIFB_CreateProc(HI_U32 LayerID)
{
    GFX_PROC_ITEM_S item;
    HI_CHAR entry_name[256] = {'\0'};

    struct fb_info *info = NULL;
    HIFB_PAR_S *par = NULL;

    info = s_stLayer[LayerID].pstInfo;
    par  = (HIFB_PAR_S *)(info->par);

    if (HI_TRUE == par->stProcInfo.bCreateProc)
    {
       return;
    }

    snprintf(entry_name, sizeof(entry_name), "hifb%d", LayerID);
    entry_name[sizeof(entry_name) - 1] = '\0';

    item.fnRead   = DRV_HIFB_PrintProc;
    item.fnWrite  = DRV_HIFB_WriteProc;
    item.fnIoctl  = HI_NULL;

    HI_GFX_PROC_AddModule(entry_name, &item, (HI_VOID*)s_stLayer[LayerID].pstInfo);

    par->stProcInfo.bCreateProc = HI_TRUE;

    return;
}



HI_VOID DRV_HIFB_DestoryProc(HI_U32 LayerID)
{
   HI_U32 WbcLayerID    = 0;
   struct fb_info *info = NULL;
   HIFB_PAR_S *par = NULL;

   info = s_stLayer[LayerID].pstInfo;
   par  = (HIFB_PAR_S *)(info->par);

   HIFB_DestoryProc(LayerID);

   WbcLayerID = par->stProcInfo.WbcLayerID;
   HIFB_DestoryProc(WbcLayerID);

   return;
}

static HI_VOID HIFB_DestoryProc(HI_U32 LayerID)
{
    HIFB_PAR_S *par;
    struct fb_info *info;
    HI_CHAR entry_name[256] = {'\0'};

    info = s_stLayer[LayerID].pstInfo;
    par  = (HIFB_PAR_S *)(info->par);

    if (HI_FALSE == par->stProcInfo.bCreateProc){
        return;
    }

    snprintf(entry_name, sizeof(entry_name), "hifb%d", LayerID);
    HI_GFX_PROC_RemoveModule(entry_name);

    par->stProcInfo.bCreateProc = HI_FALSE;

    return;
}


static HI_S32 DRV_HIFB_WriteProc(struct file * file,const char __user * buf, size_t count, loff_t *ppos)
{
    struct fb_info *info = NULL;
    HIFB_PAR_S *pstPar   = NULL;
    HI_CHAR buffer[256]  = {'\0'};
    struct seq_file *seq  = NULL;
    DRV_PROC_ITEM_S *item = NULL;

    if ((NULL == file) || (NULL == file->private_data))
    {
        return 0;
    }

    seq  = file->private_data;

    if (NULL == seq)
    {
        return 0;
    }
    item = seq->private;

    if (NULL == item->data)
    {
        return 0;
    }
    info = (struct fb_info *)(item->data);
    pstPar = (HIFB_PAR_S *)(info->par);

    if (NULL == buf)
    {
        HIFB_ERROR("input buf is null\n");
        return 0;
    }

    if (count > sizeof(buffer))
    {
        HIFB_ERROR("The command string is out of buf space :%d bytes !\n", sizeof(buffer));
        return 0;
    }

    if (copy_from_user(buffer, buf, count))
    {
        HIFB_ERROR("failed to call copy_from_user !\n");
        return 0;
    }

    HIFB_ParseProcEchoCmd(pstPar->stBaseInfo.u32LayerID, (HI_CHAR*)buffer);

    return count;
}

static HI_VOID HIFB_ParseProcEchoCmd(HI_U32 u32LayerId, HI_CHAR *pCmd)
{

    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    HI_S32 OpenDevCnt  = atomic_read(&(pstPar->stBaseInfo.ref_count));

    if ((0 == OpenDevCnt) && (pstPar->stBaseInfo.u32LayerID <= HIFB_LAYER_HD_3))
    {
        HIFB_INFO("err:device no open!\n");
        return;
    }

    if (HI_FALSE == pstPar->stProcInfo.bCreateProc)
    {
        HIFB_INFO("the hifb proc is not been created!\n");
        return;
    }

    if (strncmp("help", pCmd, 4) == 0){
        HI_DRV_PROC_EchoHelper("===============================================================\n");
        HI_DRV_PROC_EchoHelper("### help info:\n");
        HI_DRV_PROC_EchoHelper("### ***********************************************************\n");
        HI_DRV_PROC_EchoHelper("### cat /proc/msp/hifb*\n");
        HI_DRV_PROC_EchoHelper("### echo cmd > /proc/msp/hifb*\n");
        HI_DRV_PROC_EchoHelper("### for example,if you want to hide layer 0,you can input\n");
        HI_DRV_PROC_EchoHelper("### echo hide > /proc/msp/hifb0\n");
        HI_DRV_PROC_EchoHelper("### ***********************************************************\n");
        HI_DRV_PROC_EchoHelper("### hifb proc support cmd as follows:\n");
        HI_DRV_PROC_EchoHelper("###     UseWorkQueueToRefresh      : hwc refresh with work queue type\n");
        HI_DRV_PROC_EchoHelper("###     unUseWorkQueueToRefresh    : hwc refresh with sync type\n");
        HI_DRV_PROC_EchoHelper("###     show      : show layer\n");
        HI_DRV_PROC_EchoHelper("###     hide      : hide layer\n");
        HI_DRV_PROC_EchoHelper("###     capture   : capture image from frame buffer\n");
        HI_DRV_PROC_EchoHelper("###     vblank on : vblank on\n");
        HI_DRV_PROC_EchoHelper("###     vblank off: vblank off\n");
        HI_DRV_PROC_EchoHelper("###     open int  : open vdp intrupt\n");
        HI_DRV_PROC_EchoHelper("###     debug on  : it can print more message for debug\n");
        HI_DRV_PROC_EchoHelper("###     debug off : it only print message for customer\n");
        HI_DRV_PROC_EchoHelper("###     alpha=255 : set layer's global alpha\n");
        HI_DRV_PROC_EchoHelper("===============================================================\n");
        return;
    }

    if (strncmp("UseWorkQueueToRefresh", pCmd, 21) == 0)
    {
     #ifdef CFG_HIFB_FENCE_SUPPORT
       pstPar->bHwcRefreshWithWorkQueue = HI_TRUE;
     #endif
       return;
    }

    if (strncmp("unUseWorkQueueToRefresh", pCmd, 23) == 0)
    {
     #ifdef CFG_HIFB_FENCE_SUPPORT
       pstPar->bHwcRefreshWithWorkQueue = HI_FALSE;
     #endif
       return;
    }

    if (strncmp("show", pCmd, 4) == 0)
    {
        HIFB_PROC_OpenLayer(pstPar);
        return;
    }

    if (strncmp("hide", pCmd, 4) == 0)
    {
        HIFB_PROC_CloseLayer(pstPar);
        return;
    }

    if (strncmp("open int", pCmd, 7) == 0)
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetVdpIntMask(pstPar->stProcInfo.VdpIntMaskValue);
        HI_PRINT("vdp intrupt value is 0x%x\n",pstPar->stProcInfo.VdpIntMaskValue);
        return;
    }

    if (strncmp("debug on", pCmd, 8) == 0)
    {
        pstPar->stProcInfo.bDebugByMoreMessage = HI_TRUE;
        g_stDrvAdpCallBackFunction.HIFB_DRV_GetVdpIntMask(&(pstPar->stProcInfo.VdpIntMaskValue));
        HI_PRINT("set proc debug on.\n");
        return;
    }

    if (strncmp("debug off", pCmd, 9) == 0)
    {
        pstPar->stProcInfo.bDebugByMoreMessage = HI_FALSE;
        HI_PRINT("set proc debug off.\n");
        return;
    }

    if (strncmp("vblank on", pCmd, 9) == 0){
        pstPar->bVblank = HI_TRUE;
        return;
    }

    if (strncmp("vblank off", pCmd, 10) == 0){
        pstPar->bVblank = HI_FALSE;
        return;
    }

    if (strncmp("alpha", pCmd, 5) == 0)
    {
       HIFB_PROC_SetAlpha(u32LayerId,pCmd);
       return;
    }

    if (strncmp("capture", pCmd, 7) == 0){
        HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_TRUE);
        HI_UNF_HIFB_CaptureImgFromLayer(pstPar->stBaseInfo.u32LayerID, HI_FALSE);
        HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_FALSE);
        return;
    }

    if (strncmp("excapture", pCmd, 9) == 0){
       HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_TRUE);
       HI_UNF_HIFB_CaptureImgFromLayer(pstPar->stBaseInfo.u32LayerID, HI_TRUE);
       HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_FALSE);
       return;
    }

    if (strncmp("savedata", pCmd, 8) == 0){
       HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_TRUE);
       return;
    }

    if (strncmp("unsavedata", pCmd, 10) == 0){
       HI_UNF_HIFB_SetCaptureImgFromLayerState(HI_FALSE);
       return;
    }

    HIFB_ERROR("unsupported cmd:%s ", pCmd);
    HIFB_ERROR("you can use help cmd to show help info!\n");
    return;
}

static inline HI_VOID HIFB_PROC_CloseLayer(HIFB_PAR_S *pstPar)
{
    if (HIFB_LAYER_CURSOR == pstPar->stBaseInfo.u32LayerID){
       HIFB_INFO("cursor layer doesn't support for this cmd!\n");
       return;
    }

    if (HI_TRUE == pstPar->stExtendInfo.bShow){
        pstPar->stRunInfo.bModifying = HI_TRUE;
        pstPar->stExtendInfo.bShow   = HI_FALSE;
        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
        pstPar->stRunInfo.bModifying = HI_FALSE;
    }

    return;
}

static inline HI_VOID HIFB_PROC_OpenLayer(HIFB_PAR_S *pstPar)
{
   if (HIFB_LAYER_CURSOR == pstPar->stBaseInfo.u32LayerID){
      HIFB_INFO("cursor layer doesn't support for this cmd!\n");
      return;
   }
   if (HI_FALSE == pstPar->stExtendInfo.bShow){
       pstPar->stRunInfo.bModifying = HI_TRUE;
       pstPar->stExtendInfo.bShow   = HI_TRUE;
       pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
       pstPar->stRunInfo.bModifying = HI_FALSE;
   }

   return;
}


static inline HI_VOID HIFB_PROC_SetAlpha(HI_U32 u32LayerId, HI_CHAR *pCmd)
{
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    HI_U32  u32Alpha = 0;
    HI_CHAR TmpCmd[HIFB_FILE_NAME_MAX_LEN]={0};
    HI_CHAR *pStr = HI_NULL;

    strncpy(TmpCmd,pCmd,(HIFB_FILE_NAME_MAX_LEN-1));
    TmpCmd[HIFB_FILE_NAME_MAX_LEN-1] = '\0';

    pStr = strstr(TmpCmd, "=");
    if (HI_NULL == pStr)
    {
        return;
    }

    u32Alpha = HIFB_PROC_GetAlpha(pStr);
    if (u32Alpha > 255)
    {
        u32Alpha = 255;
    }

    pstPar->stExtendInfo.stAlpha.bAlphaChannel = HI_TRUE;
    pstPar->stExtendInfo.stAlpha.u8GlobalAlpha = u32Alpha;

    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAlpha(pstPar->stBaseInfo.u32LayerID, &pstPar->stExtendInfo.stAlpha);
    g_stDrvAdpCallBackFunction.HIFB_DRV_UpdataLayerReg(pstPar->stBaseInfo.u32LayerID);

    return;
}

static inline HI_U32 HIFB_PROC_GetAlpha(HI_CHAR *pStr)
{
    HI_U32 u32Alpha = 0;
    HI_CHAR *pStrTmp = HI_NULL;
    HI_BOOL bIsStrValid = HI_FALSE;

    pStr++;
    while (pStr != '\0')
    {
        if (HI_FALSE == bIsStrValid)
        {
            if (isdigit(*pStr) || ('X' == *pStr) || ('x' == *pStr)
                || (*pStr >= 'a' && *pStr <= 'f') || (*pStr >= 'A' && *pStr <= 'F'))
            {
                bIsStrValid = HI_TRUE;
                pStrTmp = pStr;
            }
            else
            {
                HIFB_ERROR("cmd is invalid\n");
                return 255;
            }
        }
        else
        {
            if ((!isdigit(*pStr)) && ('X' != *pStr) && ('x' != *pStr)
                    && (!(*pStr >= 'a' && *pStr <= 'f') && !(*pStr >= 'A' && *pStr <= 'F')))
            {
                *pStr = '\0';
                break;
            }
        }
        pStr++;
    }

    if (NULL != pStrTmp)
    {
        u32Alpha = simple_strtoul(pStrTmp, (char **)NULL, 0);
    }

    return u32Alpha;
}

static HI_S32 DRV_HIFB_PrintProc(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *item = NULL;

    struct fb_info* info = NULL;
    HIFB_PAR_S *par =  NULL;

    if ((NULL == p) || (NULL == p->private))
    {
        return HI_FAILURE;
    }
    item = (DRV_PROC_ITEM_S *)(p->private);
    info    = (struct fb_info *)(item->data);

    if (NULL == info)
    {
        return HI_FAILURE;
    }
    par = (HIFB_PAR_S *)info->par;

    if (par->stBaseInfo.u32LayerID >= sizeof(gs_LayerName)/sizeof(*gs_LayerName))
    {
        HIFB_ERROR("unknow layer: layer_%d", par->stBaseInfo.u32LayerID);
        return HI_FAILURE;
    }

    if (par->stBaseInfo.u32LayerID > HIFB_LAYER_HD_3)
    {
        return HIFB_PrintWbcProc(info, p, v);
    }
    else
    {
        return HIFB_PrintHDProc(info, p, v);
    }
}


static HI_S32 HIFB_PrintWbcProc(struct fb_info * info, struct seq_file *p, HI_VOID *v)
{
    HIFB_PAR_S *par = NULL;
    const HI_CHAR* pLayerName = NULL;
    HIFB_SLVLAYER_DATA_S stWbcLayerData;

    par = (HIFB_PAR_S *)info->par;

    if (par->stBaseInfo.u32LayerID >= HIFB_LAYER_ID_BUTT){
        HIFB_ERROR("wrong layer id.\n");
        return HI_FAILURE;
    }

    pLayerName = gs_LayerName[par->stBaseInfo.u32LayerID];

    if (g_stDrvAdpCallBackFunction.HIFB_DRV_GetSlvLayerInfo(&stWbcLayerData)){
        HIFB_ERROR("fail to get layer%d info!\n", par->stBaseInfo.u32LayerID);
        return HI_FAILURE;
    }

    PROC_PRINT(p,  "LayerId                    \t : %s\n", pLayerName);
    PROC_PRINT(p,  "ShowState                  \t : %s\n", (stWbcLayerData.bShow)? "ON" : "OFF");
    PROC_PRINT(p,  "Input  Resolution          \t : (%d, %d)\n", stWbcLayerData.stSrcBufRect.w, stWbcLayerData.stSrcBufRect.h);
    PROC_PRINT(p,  "Output ScreenRegion        \t : (%d, %d, %d, %d)\n", stWbcLayerData.stScreenRect.x, stWbcLayerData.stScreenRect.y,
                                                                         (stWbcLayerData.stScreenRect.x + stWbcLayerData.stScreenRect.w),
                                                                         (stWbcLayerData.stScreenRect.y + stWbcLayerData.stScreenRect.h));
    PROC_PRINT(p,  "Output buffer Resolution   \t : (%d, %d)\n", stWbcLayerData.stCurWBCBufRect.w, stWbcLayerData.stCurWBCBufRect.h);
    PROC_PRINT(p,  "ColorFormat                \t : %s\n", s_pszFmtName[stWbcLayerData.eFmt]);
    PROC_PRINT(p,  "Stride                     \t : %d\n", stWbcLayerData.u32Stride);
    PROC_PRINT(p,  "WbcBufNum                  \t : %d\n",stWbcLayerData.u32WbcBufNum);
    PROC_PRINT(p,  "Mem size                   \t : %d KB\n\n",stWbcLayerData.u32WbcBufSize/1024);
    return HI_SUCCESS;
}


static HI_S32 HIFB_PrintHDProc(struct fb_info * info, struct seq_file *p, HI_VOID *v)
{
    HI_BOOL bIsGfxWorkAtWbc = HI_FALSE;
    HIFB_PAR_S *par = NULL;
    const HI_CHAR* pLayerName = NULL;

    HIFB_RECT  stOutputRect = {0};
    HIFB_RECT  stDispRect = {0};

    const HI_CHAR* pszBufMode[]    = {"triple", "double ", "single", "triple( no dicard frame)", "standard", "unknow"};
    const HI_CHAR* pszStereoMode[] =  {"Mono", "Side by Side" , "Top and Bottom", "Frame packing", "unknow mode"};

    const HI_CHAR* pszAntiMode[] = {"NONE", "TDE", "VOU", "BUTT"};
    const HI_CHAR* pszAntiflicerLevel[] = {"NONE", "LOW" , "MIDDLE", "HIGH", "AUTO" ,"ERROR"};

    par = (HIFB_PAR_S *)info->par;

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetLayerOutRect(par->stBaseInfo.u32LayerID, &stOutputRect);
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetDispSize(par->stBaseInfo.u32LayerID, &stDispRect);
    bIsGfxWorkAtWbc = g_stDrvAdpCallBackFunction.DRV_HIFB_GetIsGfxWorkAtWbc();

    pLayerName = gs_LayerName[par->stBaseInfo.u32LayerID];

    if (par->stBaseInfo.enAntiflickerMode > HIFB_ANTIFLICKER_BUTT)
    {
        par->stBaseInfo.enAntiflickerMode = HIFB_ANTIFLICKER_BUTT;
    }

    if (par->stBaseInfo.enAntiflickerLevel > HIFB_LAYER_ANTIFLICKER_BUTT)
    {
        par->stBaseInfo.enAntiflickerLevel = HIFB_LAYER_ANTIFLICKER_BUTT;
    }

    PROC_PRINT(p,  "LayerId                                    \t : %s\n", pLayerName);
    PROC_PRINT(p,  "ShowState                                  \t : %s\n", (par->stExtendInfo.bShow)? "ON" : "OFF");
    PROC_PRINT(p,  "Fps                                        \t : %d\n", par->stFrameInfo.u32Fps);
    PROC_PRINT(p,  "SyncType                                   \t : %s\n", (par->bHwcRefresh) ? "fence" : "vblank");
#ifdef CFG_HIFB_FENCE_SUPPORT
    if (par->bHwcRefresh){
        PROC_PRINT(p,  "FenceValue                                 \t : %d\n", gs_SyncInfo.u32FenceValue);
        PROC_PRINT(p,  "TimeLine                                   \t : %d\n", gs_SyncInfo.u32Timeline);
    }
#endif
    PROC_PRINT(p,  "Input(xres,yres)(xres_virtual,yres_virtual)\t : (%d, %d)(%d, %d)\n",
                                                                    info->var.xres, info->var.yres,
                                                                    info->var.xres_virtual, info->var.yres_virtual);
    PROC_PRINT(p,  "Flush BufferMode(higo)                     \t : %s\n",pszBufMode[par->stExtendInfo.enBufMode]);
    PROC_PRINT(p,  "Layer Input ColorFormat                    \t : %s\n", s_pszFmtName[par->stExtendInfo.enColFmt]);
    PROC_PRINT(p,  "Layer Buffer MemSize                       \t : %d KB\n",info->fix.smem_len / 1024);
    PROC_PRINT(p,  "Input Buffer DDR (xoffset,yoffset)         \t : (%d, %d)\n", info->var.xoffset, info->var.yoffset);

    PROC_PRINT(p,  "DisplayResolution                          \t : (%d, %d)\n",par->stExtendInfo.u32DisplayWidth, par->stExtendInfo.u32DisplayHeight);
    PROC_PRINT(p,  "Output Screen origination                  \t : (%d, %d)\n", par->stExtendInfo.stPos.s32XPos, par->stExtendInfo.stPos.s32YPos);
    PROC_PRINT(p,  "PixelAlpha                                 \t : enable(%s), alpha0(0x%x), alpha1(0x%x)\n",
                                                                    par->stExtendInfo.stAlpha.bAlphaEnable ? "true" : "false",
                                                                    par->stExtendInfo.stAlpha.u8Alpha0,
                                                                    par->stExtendInfo.stAlpha.u8Alpha1);
    PROC_PRINT(p,  "GlobalAlpha                                \t : 0x%x\n", par->stExtendInfo.stAlpha.u8GlobalAlpha);
    PROC_PRINT(p,  "Colorkey                                   \t : enable(%s), value(0x%x)\n",
                                                                    par->stExtendInfo.stCkey.bKeyEnable ? "true" : "false",
                                                                    par->stExtendInfo.stCkey.u32Key);
    PROC_PRINT(p,  "Deflicker                                  \t : enable(%s), mode(%s), level(%s)\n",
                                                                    par->stBaseInfo.bNeedAntiflicker ? "true" : "false",
                                                                    pszAntiMode[par->stBaseInfo.enAntiflickerMode],
                                                                    pszAntiflicerLevel[par->stBaseInfo.enAntiflickerLevel]);
    PROC_PRINT(p,  "3DMode                                     \t : StereoMode(%s)\n",pszStereoMode[par->st3DInfo.StereoMode]);
    PROC_PRINT(p,  "Input CanvaseRegion                        \t : (%d, %d, %d, %d)\n",stOutputRect.x,stOutputRect.y,stOutputRect.w,  stOutputRect.h);
    PROC_PRINT(p,  "CanavasUpdateRect                          \t : (%d,%d,%d,%d) \n",
                                                                    par->stDispInfo.stUserBuffer.UpdateRect.x,
                                                                    par->stDispInfo.stUserBuffer.UpdateRect.y,
                                                                    par->stDispInfo.stUserBuffer.UpdateRect.w,
                                                                    par->stDispInfo.stUserBuffer.UpdateRect.h);
    PROC_PRINT(p,  "CanvasResolution                           \t : (%d,%d)\n",
                                                                    par->stDispInfo.stCanvasSur.u32Width,
                                                                    par->stDispInfo.stCanvasSur.u32Height);
    PROC_PRINT(p,  "CanvasPitch                                \t : %d\n",par->stDispInfo.stCanvasSur.u32Pitch);
    PROC_PRINT(p,  "CanvasFormat                               \t : %s\n",s_pszFmtName[par->stDispInfo.stCanvasSur.enFmt]);
    PROC_PRINT(p,  "CanavasAddr                                \t : 0x%x\n",par->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr);

    if (HI_TRUE == par->stProcInfo.bDebugByMoreMessage)
    {
        PROC_PRINT(p,  "*****************************************************************************\n");
        PROC_PRINT(p,  "Open hifb%d device count                   \t : %d\n", par->stBaseInfo.u32LayerID,atomic_read(&par->stBaseInfo.ref_count));
        PROC_PRINT(p,  "Output Screen origination                  \t : (%d, %d,  %d, %d\n",stDispRect.x,stDispRect.y,stDispRect.w,stDispRect.h);
        PROC_PRINT(p,  "DisplayingAddr(Register)                   \t : 0x%x\n",par->stRunInfo.u32ScreenAddr);
        PROC_PRINT(p,  "DisplayBuf[0] addr                         \t : 0x%x\n",par->stDispInfo.u32DisplayAddr[0]);
        PROC_PRINT(p,  "DisplayBuf[1] addr                         \t : 0x%x\n",par->stDispInfo.u32DisplayAddr[1]);
        PROC_PRINT(p,  "IsNeedFlip                                 \t : %s\n",  par->stRunInfo.bNeedFlip? "YES" : "NO");
        PROC_PRINT(p,  "CurBufferIndex                             \t : %d\n",  1-par->stRunInfo.u32IndexForInt);
        PROC_PRINT(p,  "UpDateRect                                 \t : (%d, %d, %d, %d)\n",par->stDispInfo.stUpdateRect.x, par->stDispInfo.stUpdateRect.y, par->stDispInfo.stUpdateRect.w, par->stDispInfo.stUpdateRect.h);
        PROC_PRINT(p,  "*****************************************************************************\n");
        PROC_PRINT(p,  "*****************************************************************************\n");
        PROC_PRINT(p,  "wbc information as follows\n");
        PROC_PRINT(p,  "*****************************************************************************\n");
        PROC_PRINT(p,  "whether is wbc work mode                   \t : %s\n",(HI_TRUE == bIsGfxWorkAtWbc) ? "TRUE" : "FALSE");
    }


    HIFB_PrintHwcRefreshStatus(info,p);

    return 0;
}

static inline HI_VOID HIFB_PrintHwcRefreshStatus(struct fb_info *pstFbInfo, struct seq_file *pstSeqFile)
{

    HI_U32  DispBufStride = 0;
    HI_U32  DispBufSize   = 0;
    HI_U32  CmpStride     = 0;
    HI_U32  HeadStride    = 0;
    HI_U32  HeadSize      = 0;
    HI_U32  CmpSize       = 0;
    HI_U32  PixDepth      = 0;
    HIFB_PAR_S *pstPar = NULL;

    if (NULL == pstFbInfo)
    {
        return;
    }

    pstPar = (HIFB_PAR_S *)pstFbInfo->par;
    if ( (NULL == pstPar) || (HI_FALSE == pstPar->stProcInfo.bDebugByMoreMessage))
    {
        return;
    }

    PixDepth = pstFbInfo->var.bits_per_pixel >> 3;
    DispBufStride = CONIFG_HIFB_GetMaxStride(pstPar->stExtendInfo.u32DisplayWidth,PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
    DispBufSize = HI_HIFB_GetMemSize(DispBufStride,pstPar->stExtendInfo.u32DisplayHeight);

#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE)
    DRV_HIFB_MEM_GetCmpHeadInfo(pstPar->stExtendInfo.u32DisplayHeight,&HeadSize,&HeadStride);
    CmpSize = CmpStride * pstPar->stExtendInfo.u32DisplayHeight;
#endif

    PROC_PRINT(pstSeqFile,  "*****************************************************************************\n");
    PROC_PRINT(pstSeqFile,  "hwc refresh information\n");
    PROC_PRINT(pstSeqFile,  "*****************************************************************************\n");
    PROC_PRINT(pstSeqFile,  "Input Whether is Decompress                \t : %d\n",   pstPar->stProcInfo.bHwcRefreshInDeCmpStatus);
    PROC_PRINT(pstSeqFile,  "Input display pixel format                 \t : %d\n",   pstPar->stProcInfo.HwcRefreshInDispFmt);
    PROC_PRINT(pstSeqFile,  "Input display data stride                  \t : %d\n",   pstPar->stProcInfo.HwcRefreshInDispStride);
    PROC_PRINT(pstSeqFile,  "Input display data address                 \t : 0x%lx\n",(unsigned long)pstPar->stProcInfo.HwcRefreshInDispAdress);
    PROC_PRINT(pstSeqFile,  "Whether is Stereo mode                     \t : %d\n",   pstPar->stProcInfo.b3DStatus);
    PROC_PRINT(pstSeqFile,  "Hwc Refresh No SyncTimes                   \t : %d\n",   pstPar->stProcInfo.HwcRefreshUnSyncCnt);
    PROC_PRINT(pstSeqFile,  "Decompress AR Data Err CountTimes          \t : %d\n",   pstPar->stProcInfo.DeCmpARDataErrCnt);
    PROC_PRINT(pstSeqFile,  "Decompress GB Data Err CountTimes          \t : %d\n",   pstPar->stProcInfo.DeCmpGBDataErrCnt);
    PROC_PRINT(pstSeqFile,  "Decompress AR Data Err Debug0 Status       \t : 0x%x\n", pstPar->stProcInfo.DeCmpARDataStatus[0]);
    PROC_PRINT(pstSeqFile,  "Decompress AR Data Err Debug1 Status       \t : 0x%x\n", pstPar->stProcInfo.DeCmpARDataStatus[1]);
    PROC_PRINT(pstSeqFile,  "Decompress GB Data Err Debug0 Status       \t : 0x%x\n", pstPar->stProcInfo.DeCmpGBDataStatus[0]);
    PROC_PRINT(pstSeqFile,  "Decompress GB Data Err Debug1 Status       \t : 0x%x\n", pstPar->stProcInfo.DeCmpGBDataStatus[1]);
    PROC_PRINT(pstSeqFile,  "decompress head data stride                \t : %d\n",HeadStride);
    PROC_PRINT(pstSeqFile,  "decompress AR or GB HeadSize               \t : %d\n",HeadSize);
    PROC_PRINT(pstSeqFile,  "decompress Total HeadSize                  \t : %d\n",HeadSize * 2);
    PROC_PRINT(pstSeqFile,  "decompress stride read frome register      \t : %d\n",CmpStride);
    PROC_PRINT(pstSeqFile,  "decompress AR or GB DataSize               \t : %d\n",CmpSize);
    PROC_PRINT(pstSeqFile,  "decompress Total DataSize                  \t : %d\n",CmpSize * 2);
    PROC_PRINT(pstSeqFile,  "Alloc DispBuffer or UnDecompress Stride    \t : %d\n",DispBufStride);
    PROC_PRINT(pstSeqFile,  "Alloc DispBuffer Size                      \t : %d\n",DispBufSize);
    PROC_PRINT(pstSeqFile,  "Alloc FistDispAddress with smmu and mmz    \t : 0x%lx  0x%lx\n",(unsigned long)pstFbInfo->fix.smem_start,(unsigned long)pstPar->u32SmemStartPhy);
    PROC_PRINT(pstSeqFile,  "Alloc SecondDispAddress with smmu and mmz  \t : 0x%lx  0x%lx\n",(unsigned long)(pstFbInfo->fix.smem_start + DispBufSize),(unsigned long)(pstPar->u32SmemStartPhy + DispBufSize));
    PROC_PRINT(pstSeqFile,  "Alloc ThreeDispAddress with smmu and mmz   \t : 0x%lx  0x%lx\n",(unsigned long)(pstFbInfo->fix.smem_start + DispBufSize* 2),(unsigned long)(pstPar->u32SmemStartPhy + DispBufSize * 2));

    return;
}
#endif
