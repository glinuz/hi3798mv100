/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_adp.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_ADP_H__
#define __DRV_HIFB_ADP_H__


/*********************************add include here******************************/
#ifndef HI_BUILD_IN_BOOT
#include "hi_type.h"
#include "hi_common.h"
#include "drv_disp_ext.h"
#include "drv_hifb_common.h"
#else
#include "hifb_debug.h"
#include "hi_drv_disp.h"
#endif

#include "drv_hifb_hal.h"


#include "hi_gfx_comm_k.h"
#include "optm_define.h"
#ifdef CONFIG_GFX_PQ
#include "drv_pq_ext.h"
#endif

/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Macro Definition ******************************/

#ifdef HI_BUILD_IN_BOOT
#define HIFB_FATAL(fmt...)              HI_FATAL_PRINT(HI_ID_FB, fmt)
#define HIFB_ERROR(fmt...)              HI_ERR_PRINT(HI_ID_FB, fmt)
#define HIFB_WARNING(fmt...)            HI_WARN_PRINT(HI_ID_FB, fmt)
#define HIFB_INFO(fmt...)               HI_INFO_PRINT(HI_ID_FB, fmt)
#endif

#define OPTM_ENABLE                      1
#define OPTM_DISABLE                     0

/**
 **master layer gp
 **高清图层GP
 **/
#define OPTM_MASTER_GPID                 OPTM_GFX_GP_0
/**
 **slaver layer gp
 **标清图层GP
 **/
#define OPTM_SLAVER_GPID                 OPTM_GFX_GP_1
/**
 **slaver layer id
 **同源回写使用的图层
 **/
#define OPTM_SLAVER_LAYERID              HIFB_LAYER_SD_0

#define OPTM_MAX_LOGIC_HIFB_LAYER        ((HI_U32)HIFB_LAYER_ID_BUTT)

#define OPTM_CMAP_SIZE                   0x400    /*unit:KB 256*4*/

/**
 ** check gp mask flag,return success when it's true, or else continue
 **/
#define OPTM_CheckGPMask_BYLayerID(u32LayerID)  \
            do{\
                if (g_stGfxGPDevice[g_stGfxDevice[u32LayerID].enGPId].bMaskFlag)\
                {\
                    return HI_SUCCESS;\
                }\
            }while(0)

#define OPTM_CheckGPMask_BYGPID(enGPId) \
            do{\
                if (g_stGfxGPDevice[enGPId].bMaskFlag)\
                {\
                    return HI_SUCCESS;\
                }\
            }while(0)


#define IS_MASTER_GP(enGpId) ((g_enOptmGfxWorkMode == HIFB_GFX_MODE_HD_WBC)&&(enGpId == OPTM_GFX_GP_0))
#define IS_SLAVER_GP(enGpId) ((g_enOptmGfxWorkMode == HIFB_GFX_MODE_HD_WBC)&&(enGpId == OPTM_GFX_GP_1))



/*************************** Structure Definition ****************************/
typedef struct tagOPTM_GFX_CSC_PARA_S
{
    HI_U32               u32Bright;
    HI_U32               u32Contrast;
    HI_U32               u32Saturation;
    HI_U32               u32Hue;
    HI_U32               u32Kr;
    HI_U32               u32Kg;
    HI_U32               u32Kb;
}OPTM_GFX_CSC_PARA_S;

typedef enum tagOPTM_GFX_GP_E
{
    OPTM_GFX_GP_0 = 0x0,/** process gfx0,gfx1,gfx2,gfx3   */
    OPTM_GFX_GP_1,        /** process gfx4,gfx5             */
    OPTM_GFX_GP_BUTT
}OPTM_GFX_GP_E;

typedef struct tagOPTM_GFX_OFFSET_S
{
    HI_U32 u32Left;    /** left offset   */
    HI_U32 u32Top;     /** top offset    */
    HI_U32 u32Right;   /** right offset  */
    HI_U32 u32Bottom;  /** bottom offset */
}OPTM_GFX_OFFSET_S;



/** csc state*/
typedef enum tagOPTM_CSC_STATE_E
{
    OPTM_CSC_SET_PARA_ENABLE = 0x0,
    OPTM_CSC_SET_PARA_RGB         ,
    OPTM_CSC_SET_PARA_BGR         ,
    OPTM_CSC_SET_PARA_CLUT        ,
    OPTM_CSC_SET_PARA_CbYCrY      ,
    OPTM_CSC_SET_PARA_YCbYCr      ,
    OPTM_CSC_SET_PARA_BUTT
} OPTM_CSC_STATE_E;

typedef enum optm_COLOR_SPACE_E
{
    OPTM_CS_UNKNOWN = 0,

    OPTM_CS_BT601_YUV_LIMITED, /** BT.601 **/
    OPTM_CS_BT601_YUV_FULL,
    OPTM_CS_BT601_RGB_LIMITED,
    OPTM_CS_BT601_RGB_FULL,

    OPTM_CS_BT709_YUV_LIMITED, /** BT.709 **/
    OPTM_CS_BT709_YUV_FULL,
    OPTM_CS_BT709_RGB_LIMITED,
    OPTM_CS_BT709_RGB_FULL,

    OPTM_CS_BUTT
} OPTM_COLOR_SPACE_E;



/**
 ** wbc work state
 ** 回写状态
 **/
#ifndef HI_BUILD_IN_BOOT
typedef struct tagOPTM_GFX_WORK_S
{
    HI_U32                   u32Data;
    struct work_struct       work;
}OPTM_GFX_WORK_S;
#endif

/**
 ** what that want to updata
 ** 更新哪种状态
 **/
typedef union tagOPTM_GFX_UP_FLAG_U
{
    struct
    {
        unsigned int    IsNeedRegUp     : 1 ; /*  [0] */
        unsigned int    IsEnable        : 1 ; /*  [1] */
        unsigned int    IsNeedUpInRect  : 1 ; /*  [2] */
        unsigned int    OutRect         : 1 ; /*  [3] */
        unsigned int    IsNeedUpAlpha   : 1 ; /*  [4] */
        unsigned int    IsNeedUpPreMult : 1 ; /*  [5] */
        unsigned int    WbcMode         : 1 ; /*  [6] */
        unsigned int    Reserved        : 25; /*  [31...7] */
    } bits;
    unsigned int    u32;
}OPTM_GFX_UP_FLAG_U;


/**
 ** display ID
 ** 显示通道ID，高清通道和标清通道
 **/
typedef enum tagOPTM_DISPCHANNEL_E
{
    OPTM_DISPCHANNEL_0   = 0,    /** gfx4,gfx5             **/
    OPTM_DISPCHANNEL_1      ,    /** gfx0,gfx1,gfx2,gfx3   **/
    OPTM_DISPCHANNEL_BUTT
}OPTM_DISPCHANNEL_E;

/**
 **GP Para
 **GP相关参数信息结构体
 **/
typedef struct tagOPTM_GFX_GP_S
{
    HI_BOOL bOpen;
    HI_BOOL b3DEnable;
    HI_BOOL bMaskFlag;
    HI_BOOL bBGRState;
    HI_BOOL bInterface;
    HI_BOOL bGpClose;
    HI_BOOL bResume;
    HI_BOOL bRecoveryInNextVT;
    HI_BOOL bNeedExtractLine;
    HI_BOOL bDispInitial;
    HI_RECT_S stInRect;
    HI_RECT_S stOutRect;
    HI_U32    u32ZmeDeflicker;
#ifdef CHIP_TYPE_hi3798cv200/** hdmi cts bug **/
    HI_DRV_DISP_FMT_E         enDispFmt;
#endif
    HIFB_STEREO_MODE_E        enTriDimMode;
    OPTM_GFX_CSC_PARA_S       stCscPara;
#ifndef CONFIG_GFX_PQ
    OPTM_COLOR_SPACE_E        enInputCsc;
    OPTM_COLOR_SPACE_E        enOutputCsc;
#else
    HI_PQ_GFX_COLOR_SPACE_E   enInputCsc;
    HI_PQ_GFX_COLOR_SPACE_E   enOutputCsc;
#endif
    OPTM_VDP_LAYER_GP_E       enGpHalId;
    OPTM_DISPCHANNEL_E        enDispCh;
    OPTM_GFX_UP_FLAG_U        unUpFlag;

#ifndef HI_BUILD_IN_BOOT
    struct workqueue_struct   *queue;
    OPTM_GFX_WORK_S           stOpenSlvWork;
    OPTM_GFX_WORK_S           st3DModeChgWork;
#endif
    HI_U32 u32Prior;
    HI_U32 u32Alpha;
    OPTM_VDP_DATA_RMODE_E     enReadMode;
    OPTM_VDP_BKG_S            stBkg;
    OPTM_VDP_CBM_MIX_E        enMixg;
}OPTM_GFX_GP_S;


/**
 ** 压缩解压模块
 **/
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    typedef enum tagOPTM_CMP_STATUS_E
    {
        OPTM_CMP_STATUS_STANDBY          = 0x0 ,  /** hardware was ready to work          */
        OPTM_CMP_STATUS_PARALYSED              ,  /** hardware unable to work             */
        OPTM_CMP_STATUS_COMPRESSIONABLE        ,  /** hardware can be setted to compress  */
        OPTM_CMP_STATUS_COMPRESSING            ,  /** hardware was compressing            */
        OPTM_CMP_STATUS_COMPRESSFINISHED       ,  /** hardware has finished compression   */
        OPTM_CMP_STATUS_DECOMPRESSIONABLE      ,  /** hardware can be setted to decompress*/
        OPTM_CMP_STATUS_DECOMPRESSING          ,  /** hardware was decompressing          */
        OPTM_CMP_STATUS_BUTT                   ,
    }OPTM_CMP_STATUS_E;

    typedef struct tagOPTM_CMP_DATA_INFO_S
    {
        HI_U32 u32ARCmpAddr;
        HI_U32 u32GBCmpAddr;
        HI_U32 u32Width;
        HI_U32 u32Height;
        HI_U32 u32CmpStride;
        HI_U32 u32HeadStride;
        HI_U32 u32HeadSize;
        HI_U32 u32CMPBufSize;
        HIFB_RECT stUpdateRect;
        HIFB_RECT stCmpRect;
        GFX_MMZ_BUFFER_S stCMPBuffer;
    }OPTM_CMP_DATA_INFO_S;

    typedef struct tagOPTM_GFX_CMP_WORK_S
    {
        HI_U32                   u32Data;
        struct work_struct       work;
        struct workqueue_struct  *queue;
    }OPTM_GFX_CMP_WORK_S;


    typedef struct tagOPTM_GFX_CMP_S
    {
        HI_U32  u32CmpIndex;
        HI_BOOL bLayerRefreshed;
        HI_BOOL bLayerResChgFlag;
        HI_BOOL bCMPQueueWorkStart;
        HI_BOOL bDDRDetectOpen;
        HI_BOOL bDDRDetectReady;
        HIFB_CMP_MODE_E         enCMPMode;
        OPTM_VDP_LAYER_WBC_E    enCmpHalID;
        OPTM_VDP_INTMSK_E       enIntMsk;
        OPTM_CMP_STATUS_E       enStatus;
        OPTM_CMP_DATA_INFO_S    stDataInfo;
        OPTM_GFX_CMP_WORK_S     stCMPWork;
    }OPTM_GFX_CMP_S;

#endif

typedef struct tagOPTM_GFX_LAYER_S{
    HI_BOOL              bOpened;
    HI_BOOL              bMaskFlag;
    HI_BOOL              bSharpEnable;
    HI_BOOL              bExtractLine;
    HI_BOOL              bEnable;
    HI_BOOL              b3DEnable;
    HI_S32               s32Depth;
    HI_BOOL              bPreMute;
    HI_U32               u32ZOrder;
    HI_BOOL              bCmpOpened;
    HI_BOOL              bDmpOpened;
    HI_BOOL              bBufferChged;
    HI_U32               s32BufferChgCount;
    HI_U32               NoCmpBufAddr;
    HI_U32               ARHeadDdr;
    HI_U32               ARDataDdr;
    HI_U32               GBHeadDdr;
    HI_U32               GBDataDdr;
    HI_U32               u32TriDimAddr;
    HI_U16               Stride;           /** no compression mode stride  */
    HI_U16               CmpStride;        /** compression mode stride     */
    HIFB_COLOR_FMT_E     enDataFmt;
    HIFB_RECT            stInRect;         /** Inres of gfx                */
    HIFB_ALPHA_S         stAlpha;
    HIFB_COLORKEYEX_S    stColorkey;
    HIFB_STEREO_MODE_E   enTriDimMode;
    OPTM_VDP_BKG_S       stBkg;
    OPTM_VDP_GFX_BITEXTEND_E enBitExtend;  /** the g_ctrl of bitext        **/
    OPTM_VDP_DATA_RMODE_E    enReadMode;
    OPTM_VDP_DATA_RMODE_E    enUpDateMode;
    OPTM_VDP_LAYER_GFX_E     enGfxHalId;   /** the gfx's hal id            */
    OPTM_GFX_GP_E            enGPId;       /** which gp the gfx belong to  */
    OPTM_CSC_STATE_E         CscState;
    volatile HI_U32          vblflag;
    wait_queue_head_t        vblEvent;
    GFX_MMZ_BUFFER_S         stCluptTable;
    #ifdef CFG_HIFB_COMPRESSION_SUPPORT
    OPTM_GFX_CMP_S       stCmpInfo;
    #endif
}OPTM_GFX_LAYER_S;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            initial operation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
typedef struct
{
    /*gfx init*/
    /*CNcomment:图形初始化*/
    HI_S32 (*OPTM_GfxInit)(HI_VOID);

    /*gfx deinit*/
    /*CNcomment:图形去初始化*/
    HI_S32 (*OPTM_GfxDeInit)(HI_VOID);

    /*open layer*/
    /*CNcomment:图层打开*/
    HI_S32 (*OPTM_GfxOpenLayer)(HIFB_LAYER_ID_E enLayerId);

    /*close layer*/
    /*CNcomment:图层关闭*/
    HI_S32 (*OPTM_GfxCloseLayer)(HIFB_LAYER_ID_E enLayerId);

    /*enable/disable layer*/
    /*CNcomment:图层使能或者非使能*/
    HI_S32 (*OPTM_GfxSetEnable)(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable);

    /*mask layer*/
    /*CNcomment:是否屏蔽图层*/
    HI_S32 (*OPTM_GfxMaskLayer)(HIFB_LAYER_ID_E enLayerId, HI_BOOL bMask);

    /*set layer address*/
    /*CNcomment:设置图层显示地址*/
    HI_S32 (*OPTM_GfxSetLayerAddr)(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Addr);

    /*get layer address*/
    /*CNcomment:获取图层显示地址*/
    HI_S32 (*OPTM_GfxGetLayerAddr)(HIFB_LAYER_ID_E enLayerId, HI_U32 *pu32Addr);

    /*set layer stride*/
    /*CNcomment:设置图层行距*/
    HI_S32 (*OPTM_GfxSetLayerStride)(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Stride);

    /*set layer data format*/
    /*CNcomment:设置图层像素格式*/
    HI_S32 (*OPTM_GfxSetLayerDataFmt)(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt);

    HI_S32 (*OPTM_GfxSetColorReg)(HIFB_LAYER_ID_E u32LayerId, HI_U32 u32OffSet, HI_U32 u32Color, HI_S32 UpFlag);

    /*wait for vertical blank*/
    /*CNcomment:等待垂直消隐*/
    HI_S32 (*OPTM_GfxWaitVBlank)(HIFB_LAYER_ID_E u32LayerId);

    /*set layer deflick level*/
    /*CNcomment:设置抗闪级别*/
    HI_S32 (*OPTM_GfxSetLayerDeFlicker)(HIFB_LAYER_ID_E enLayerId, HIFB_DEFLICKER_S *pstDeFlicker);

    /*set layer alpha*/
    /*CNcomment:设置图层alpha 值*/
    HI_S32 (*OPTM_GfxSetLayerAlpha)(HIFB_LAYER_ID_E enLayerId, HIFB_ALPHA_S *pstAlpha);

    /*get gp global alpha zero sum*/
    /*CNcomment:获取GP全局alpha值为0的像素点个数 */
    HI_S32 (*OPTM_GfxGetLayerGalphaSum)(HIFB_LAYER_ID_E enLayerId, HI_U32 *pu32GalphaSum);

    /*set layer rect*/
    /*CNcomment:设置图层输入矩形*/
    HI_S32 (*OPTM_GfxSetLayerRect)(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstRect);

    /*set graphics process device Inrect and Outrect*/
    /*CNcomment:设置图层管理器输入输出矩形*/
    HI_S32 (*OPTM_GfxSetGpRect)(OPTM_GFX_GP_E enGpId, const HIFB_RECT *pstInputRect);

    /*get graphics process device Inrect and Outrect*/
    /*CNcomment:获取图层管理器输入输出矩形*/
    HI_S32 (*OPTM_GfxGetGpRect)(OPTM_GFX_GP_E enGpId, HIFB_RECT *pstInputRect);

    HI_S32 (*OPTM_GfxSetGpInPutSize)(OPTM_GFX_GP_E enGpId, HI_U32 u32Width, HI_U32 u32Height);

    /*set layer key mask*/
    /*CNcomment:设置图层colorkey mask*/
    HI_S32 (*OPTM_GfxSetLayKeyMask)(HIFB_LAYER_ID_E enLayerId, const HIFB_COLORKEYEX_S *pstColorkey);

    /*set layer pre mult*/
    /*CNcomment:设置图层预乘*/
    HI_S32 (*OPTM_GfxSetLayerPreMult)(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable);

    /*set clut address*/
    /*CNcomment:设置CLUT 地址*/
    HI_S32 (*OPTM_GfxSetClutAddr)(HIFB_LAYER_ID_E enLayerId, HI_U32 u32PhyAddr);

    HI_VOID (*OPTM_GfxGetOSDData)  (HIFB_LAYER_ID_E enLayerId, HIFB_OSD_DATA_S  *pstLayerData);
#ifndef HI_BUILD_IN_BOOT
    HI_VOID (*HIFB_ADP_GetLogoData)(HIFB_LAYER_ID_E enLayerId, HIFB_LOGO_DATA_S *pstLogoData);
#endif
    /*set call back*/
    /*CNcomment:注册回调函数*/
#ifndef HI_BUILD_IN_BOOT
    HI_S32 (*OPTM_GfxSetCallback)(HIFB_LAYER_ID_E enLayerId, IntCallBack pCallBack, HIFB_CALLBACK_TPYE_E eIntType);
#endif
    /*update layer register*/
    /*CNcomment:更新寄存器*/
    HI_S32 (*OPTM_GfxUpLayerReg)(HIFB_LAYER_ID_E enLayerId);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    /*enable/disable stereo*/
    /*CNcomment:设置3D 使能或者非使能*/
    HI_S32 (*OPTM_GfxSetTriDimEnable)(HIFB_LAYER_ID_E enLayerId, HI_U32 bEnable);

    /*set stereo mode*/
    /*CNcomment:设置3D 模式SBS/TB/MVC*/
    HI_S32 (*OPTM_GfxSetTriDimMode)(HIFB_LAYER_ID_E enLayerId, HIFB_STEREO_MODE_E enMode, HIFB_STEREO_MODE_E enWbcSteroMode);

    /*set stereo address*/
    /*CNcomment:设置3D 模式下右眼地址*/
    HI_S32 (*OPTM_GfxSetTriDimAddr)(HIFB_LAYER_ID_E enLayerId, HI_U32 u32TriDimAddr);
#endif

    /*set the priority of layer in gp*/
    /*CNcomment:设置图层在GP 中的优先级*/
    HI_S32 (*OPTM_GfxSetLayerPriority)(HIFB_LAYER_ID_E u32LayerId, HIFB_ZORDER_E enZOrder);

    /*get the priority of layer in gp*/
    /*CNcomment:获取图层在GP 中的优先级*/
    HI_S32 (*OPTM_GfxGetLayerPriority)(HIFB_LAYER_ID_E u32LayerId, HI_U32 *pU32Priority);

    /*获取图层的能力集*/
    HI_VOID (*OPTM_GFX_GetDevCap)(const HIFB_CAPABILITY_S **pstCap);
    /*获取图层的输出区域大小*/
    HI_S32 (*OPTM_GfxGetOutRect)(HIFB_LAYER_ID_E enLayerId, HIFB_RECT * pstOutputRect);
    /*获取图层的输入区域大小*/
    HI_S32 (*OPTM_GfxGetLayerRect)(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstRect);
    /*set gp mask flag*/
    HI_S32 (*OPTM_GFX_SetGPMask)(OPTM_GFX_GP_E enGpId, HI_BOOL bFlag);
    /*get gfx mask flag*/
    HI_S32 (*OPTM_GFX_GetGfxMask)(OPTM_GFX_GP_E enGpId);
    HI_S32 (*OPTM_GfxGetDispFMTSize)(OPTM_GFX_GP_E enGpId, HIFB_RECT *pstOutRect);

#ifndef HI_BUILD_IN_BOOT
    HI_S32  (*OPTM_GFX_ClearLogoOsd)         (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_SetStereoDepth)       (HIFB_LAYER_ID_E enLayerId, HI_S32 s32Depth);
    HI_VOID (*HIFB_ADP_SetDeCmpDdrInfo)      (HIFB_LAYER_ID_E enLayerId, HI_U32 ARHeadDdr,HI_U32 ARDataDdr,HI_U32 GBHeadDdr,HI_U32 GBDataDdr, HI_U32 DeCmpStride);
    HI_VOID (*HIFB_ADP_GetDeCmpStatus)       (HIFB_LAYER_ID_E enLayerId, DRV_HIFB_DECMPSTATUS_S *pstDeCmpStatus, HI_BOOL bDebug);
    HI_VOID (*HIFB_ADP_SetVdpIntMask)        (HI_U32 IntMask);
    HI_VOID (*HIFB_ADP_GetVdpIntMask)        (HI_U32 *pIntMask);
    HI_VOID (*HIFB_ADP_GetResumeStatus)      (HIFB_LAYER_ID_E enLayerId, HI_BOOL *pbResume);
    HI_VOID (*OPTM_GFX_DECMP_Open)           (HIFB_LAYER_ID_E enLayerId);
    HI_VOID (*OPTM_GFX_DECMP_Close)          (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_CMP_Open)             (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_CMP_Close)            (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_CMP_GetSwitch)        (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_SetCmpRect)           (HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstRect);
    HI_S32  (*OPTM_GFX_SetCmpMode)           (HIFB_LAYER_ID_E enLayerId, HIFB_CMP_MODE_E enCMPMode);
    HIFB_CMP_MODE_E (*OPTM_GFX_GetCmpMode)   (HIFB_LAYER_ID_E enLayerId);
    HI_S32  (*OPTM_GFX_SetCmpDDROpen)        (HIFB_LAYER_ID_E enLayerId, HI_BOOL bOpen);
    HI_S32  (*OPTM_GFX_GetSlvLayerInfo)      (HIFB_SLVLAYER_DATA_S *pstLayerInfo);
#endif

    HI_S32  (*OPTM_GFX_SetTCFlag)         (HI_BOOL bFlag);
    HI_S32  (*OPTM_GfxSetGpDeflicker)     (OPTM_GFX_GP_E enGpId, HI_BOOL bDeflicker);
    HI_S32  (*OPTM_GFX_GetHaltDispStatus) (HIFB_LAYER_ID_E enLayerId, HI_BOOL *pbDispInit);
    HI_BOOL (*HIFB_ADP_GetIsGfxWorkAtWbc) (HI_VOID);
}OPTM_GFX_OPS_S;



/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/



/***************************************************************************************
* func          : OPTM_GFX_GetOps
* description   : get hifb options, then can call the functions \n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GFX_GetOps(OPTM_GFX_OPS_S *ops);

/***************************************************************************************
* func          : OPTM_UnmapAndRelease
* description   : unmap the ddr and release it\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_UnmapAndRelease(GFX_MMZ_BUFFER_S *psMBuf);

/***************************************************************************************
* func          : OPTM_AllocAndMap
* description   : CNcomment: alloc and map mem CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, GFX_MMZ_BUFFER_S *psMBuf);


/***************************************************************************************
* func          : OPTM_GfxInitLayer
* description   : CNcomment: 初始化图层信息 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GfxInitLayer(HIFB_LAYER_ID_E enLayerId);

/***************************************************************************************
* func          : OPTM_GPOpen
* description   : CNcomment: 打开GP设备并设置ZME系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GPOpen(OPTM_GFX_GP_E enGPId);

/***************************************************************************************
* func          : OPTM_GFX_ReleaseClutBuf
* description   : CNcomment: 释放调色板内存 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_ReleaseClutBuf(HIFB_LAYER_ID_E enLayerId);

/***************************************************************************************
* func          : OPTM_GfxSetLayerReadMode
* description   : CNcomment: 设置读取数据模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerReadMode(HIFB_LAYER_ID_E enLayerId, OPTM_VDP_DATA_RMODE_E enReadMode);

/***************************************************************************************
* func          : OPTM_PixerFmtTransferToHalFmt
* description   : CNcomment: 根据像素数据转换成像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
OPTM_VDP_GFX_IFMT_E OPTM_PixerFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt);


HI_VOID DRV_HIFB_ADP_UpDataCallBack(HI_VOID* u32Param0, HI_VOID* u32Param1);

/***************************************************************************************
* func            : OPTM_GfxSetLayerAddr
* description    : CNcomment: 设置显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerAddr(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Addr);

/***************************************************************************************
* func          : OPTM_GfxSetLayerStride
* description   : CNcomment: 设置图层stride CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerStride(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Stride);

/***************************************************************************************
* func          : OPTM_Adapt_AllocAndMap
* description   : CNcomment: 分配内存并映射地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_Adapt_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, GFX_MMZ_BUFFER_S *psMBuf);

/***************************************************************************************
* func          : OPTM_SetCallbackToDisp
* description   : CNcomment: 向Display注册中断 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef HI_BUILD_IN_BOOT
HI_S32 OPTM_SetCallbackToDisp(OPTM_GFX_GP_E enGPId, IntCallBack pCallBack, HI_DRV_DISP_CALLBACK_TYPE_E eType, HI_BOOL bFlag);
#endif

/***************************************************************************************
* func          : OPTM_GfxSetEnable
* description   : CNcomment: 设置图层使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetEnable(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable);

/***************************************************************************************
* func          : OPTM_GfxSetLayerRect
* description   : CNcomment: 设置图层分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerRect(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstRect);

/***************************************************************************************
* func          : OPTM_GfxSetGpRect
* description   : CNcomment: 设置图层对应的GP分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetGpRect(OPTM_GFX_GP_E enGpId, const HIFB_RECT * pstInputRect);

/***************************************************************************************
* func          : OPTM_GfxCloseLayer
* description   : CNcomment: 关闭图层 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxCloseLayer(HIFB_LAYER_ID_E enLayerId);


#ifdef HI_BUILD_IN_BOOT
HI_S32  OPTM_GfxInit(HI_VOID);
HI_S32  OPTM_GfxOpenLayer(HIFB_LAYER_ID_E enLayerId);
HI_S32  OPTM_GfxSetLayerAlpha(HIFB_LAYER_ID_E enLayerId, HIFB_ALPHA_S *pstAlpha);
HI_S32  OPTM_GfxSetLayerDataFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt);
HI_S32  OPTM_GfxSetLayKeyMask(HIFB_LAYER_ID_E enLayerId, const HIFB_COLORKEYEX_S *pstColorkey);
HI_S32  OPTM_GfxUpLayerReg(HIFB_LAYER_ID_E enLayerId);
HI_S32  OPTM_GpInitFromDisp(OPTM_GFX_GP_E   enGPId, HI_BOOL Support4KLogo);
HI_S32  OPTM_GfxSetDispFMTSize(OPTM_GFX_GP_E   enGpId, const HI_RECT_S *pstOutRect);
HI_S32  OPTM_GFX_SetTCFlag(HI_BOOL bFlag);
HI_BOOL HIFB_ADP_GetIsGfxWorkAtWbc(HI_VOID);
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
