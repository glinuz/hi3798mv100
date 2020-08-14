/*****************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
  File Name     : drv_venc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/07
  Last Modified :
  Description   :
  Function List :
  History       :
  1.Date        :
  Author        : j00131665
  Modification  : Created file
 ******************************************************************************/

#include "hal_venc.h"
#include "drv_win_ext.h"
#include "drv_vpss_ext.h"
#include "drv_venc_ext.h"
#include "drv_disp_ext.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
#include "hi_drv_file.h"
#include "hi_drv_reg.h"
#include "drv_venc_efl.h"
#include "drv_venc_osal.h"
#include "drv_venc_buf_mng.h"
#include "drv_venc.h"
#include "hi_mpi_venc.h"
//#include "hi_drv_venc.h"  //fix me
#include "hi_drv_video.h"
#include "hi_kernel_adapt.h"
#include "hi_reg_common.h"
#include "hi_osal.h"
#include "drv_vi_ext.h"

#ifdef __VENC_SUPPORT_JPGE__
#include "drv_jpge_ext.h"
#endif

#include <linux/delay.h>
#include <linux/kernel.h>

//#include "drv_vi_ext.h"
//#include "drv_ndpt_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//Check null pointer
//#define VDEC_APPENDIX_LEN 16
#define VENC_MAX_FRAME_LEN 200000

OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];
VEDU_OSAL_EVENT g_VencWait_Stream[VENC_MAX_CHN_NUM];
extern VEDU_OSAL_EVENT g_VENC_Event_stream;

spinlock_t g_SendFrame_Lock[VENC_MAX_CHN_NUM];     /*lock the destroy and send frame*/
//HI_DECLARE_MUTEX(g_VencStreamMutex[VENC_MAX_CHN_NUM]);

volatile HI_U32 g_VENCCrgCtrl = 0;
//HI_S32 VencThreadGetIFrame(HI_VOID * arg);

HI_U32 g_vedu_chip_id = 0;
//static HI_U32 g_venc_curent_chn_num = 0;
extern VPSS_EXPORT_FUNC_S *pVpssFunc;
extern VENC_BUFFER_S g_stKernelOmxmmz[VENC_MAX_CHN_NUM][OMX_OUTPUT_PORT_BUFFER_MAX];

#ifdef __VENC_SUPPORT_JPGE__
extern JPGE_EXPORT_FUNC_S *pJpgeFunc;
#endif

#define D_VENC_GET_CHN(u32VeChn, hVencChn) \
    do {\
        if (hVencChn == NULL)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hVEncHandle == hVencChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_CHN_BY_UHND(u32VeChn, hVencUsrChn) \
    do {\
        if (hVencUsrChn == HI_INVALID_HANDLE)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hUsrHandle == hVencUsrChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_PRIORITY_ID(s32VeChn, sPriorityID) \
    do {\
        sPriorityID = 0; \
        while (sPriorityID < VENC_MAX_CHN_NUM)\
        {   \
            if (PriorityTab[0][sPriorityID] == s32VeChn)\
            { \
                break; \
            } \
            sPriorityID++; \
        } \
    } while (0)

//extern HI_U32 gVpNethandle;
wait_queue_head_t gEncodeFrame;
volatile HI_U32 gencodeframe = 0;
extern HI_S32 gMax, gBMAX;
//HI_U8 gSendbuf[VENC_MAX_FRAME_LEN + VDEC_APPENDIX_LEN + 4];
HI_U8 ZeroDelaySuffix[] = {0x00, 0x00, 0x01, 0x1e, 0x48, 0x53, 0x50, 0x49, 0x43, 0x45, 0x4E, 0x44, 0x00, 0x00, 0x01,
                           0x1e
                          };
HI_U32 gVencVppayload = 0;
//extern wait_queue_head_t stSendTimeQue;
//volatile HI_U32 gSendStart = 0;

//extern HI_U32 g_VpStaticCtrl;
//extern HI_S32 g_VpStatic[];
//extern HI_S32 g_Pts;

HI_S8 PriorityTab[2][VENC_MAX_CHN_NUM] = {{ -1, -1, -1, -1, -1, -1, -1, -1}, {}};
///////////////////////////////////////////////////////////////
extern VENC_EXPORT_FUNC_S s_VencExportFuncs;

static HI_VOID GetPixFormat(HI_UNF_VIDEO_FORMAT_E VideoFormat, HI_DRV_PIX_FORMAT_E *pPixFormat)
{
    switch (VideoFormat)
    {
        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422:
            *pPixFormat = HI_DRV_PIX_FMT_NV61_2X1;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_420:
            *pPixFormat = HI_DRV_PIX_FMT_NV21;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV:
            *pPixFormat = HI_DRV_PIX_FMT_NV12;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_400:
            *pPixFormat = HI_DRV_PIX_FMT_NV80;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_411:
            *pPixFormat = HI_DRV_PIX_FMT_NV12_411;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2:
            *pPixFormat = HI_DRV_PIX_FMT_NV61;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_444:
            *pPixFormat = HI_DRV_PIX_FMT_NV42;
            break;

        case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
            *pPixFormat = HI_DRV_PIX_FMT_UYVY;
            break;

        case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
            *pPixFormat = HI_DRV_PIX_FMT_YUYV;
            break;

        case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
            *pPixFormat = HI_DRV_PIX_FMT_YVYU;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_400:
            *pPixFormat = HI_DRV_PIX_FMT_YUV400;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_411:
            *pPixFormat = HI_DRV_PIX_FMT_YUV411;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_420:
            *pPixFormat = HI_DRV_PIX_FMT_YUV420p;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_1X2:
            *pPixFormat = HI_DRV_PIX_FMT_YUV422_1X2;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_422_2X1:
            *pPixFormat = HI_DRV_PIX_FMT_YUV422_2X1;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_444:
            *pPixFormat = HI_DRV_PIX_FMT_YUV_444;
            break;

        case HI_UNF_FORMAT_YUV_PLANAR_410:
            *pPixFormat = HI_DRV_PIX_FMT_YUV410p;
            break;

        default:
            *pPixFormat = HI_DRV_PIX_BUTT;
            break;
    }
}

static HI_VOID GetFrameType(HI_UNF_VIDEO_FRAME_PACKING_TYPE_E FramePackingType, HI_DRV_FRAME_TYPE_E *pFrmType)
{
    switch (FramePackingType)
    {
        case HI_UNF_FRAME_PACKING_TYPE_NONE:
            *pFrmType = HI_DRV_FT_NOT_STEREO;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE:
            *pFrmType = HI_DRV_FT_SBS;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM:
            *pFrmType = HI_DRV_FT_TAB;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED:
            *pFrmType = HI_DRV_FT_NOT_STEREO;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_FRAME_PACKING:
            *pFrmType = HI_DRV_FT_FPK;
            break;

        case HI_UNF_FRAME_PACKING_TYPE_3D_TILE:
            *pFrmType = (HI_DRV_FT_FPK + 1);  //HI_DRV_FT_FS;  fix me!
            break;

        default:
            *pFrmType = HI_DRV_FT_BUTT;
            break;
    }
}

static HI_S32 Convert_FrameStructure(HI_UNF_VIDEO_FRAME_INFO_S* pstUnfImage, HI_DRV_VIDEO_FRAME_S* pstDrvImage)
{
    D_VENC_CHECK_PTR(pstUnfImage);
    D_VENC_CHECK_PTR(pstDrvImage);
    pstDrvImage->u32FrameIndex  = pstUnfImage->u32FrameIndex;
    pstDrvImage->stBufAddr[0].u32PhyAddr_Y = pstUnfImage->stVideoFrameAddr[0].u32YAddr;
    pstDrvImage->stBufAddr[0].u32Stride_Y  = pstUnfImage->stVideoFrameAddr[0].u32YStride;
    pstDrvImage->stBufAddr[0].u32PhyAddr_C = pstUnfImage->stVideoFrameAddr[0].u32CAddr;
    pstDrvImage->stBufAddr[0].u32Stride_C  = pstUnfImage->stVideoFrameAddr[0].u32CStride;
    pstDrvImage->stBufAddr[0].u32PhyAddr_Cr = pstUnfImage->stVideoFrameAddr[0].u32CrAddr;
    pstDrvImage->stBufAddr[0].u32Stride_Cr = pstUnfImage->stVideoFrameAddr[0].u32CrStride;

    pstDrvImage->stBufAddr[1].u32PhyAddr_Y = pstUnfImage->stVideoFrameAddr[1].u32YAddr;
    pstDrvImage->stBufAddr[1].u32Stride_Y  = pstUnfImage->stVideoFrameAddr[1].u32YStride;
    pstDrvImage->stBufAddr[1].u32PhyAddr_C = pstUnfImage->stVideoFrameAddr[1].u32CAddr;
    pstDrvImage->stBufAddr[1].u32Stride_C  = pstUnfImage->stVideoFrameAddr[1].u32CStride;
    pstDrvImage->stBufAddr[1].u32PhyAddr_Cr = pstUnfImage->stVideoFrameAddr[1].u32CrAddr;
    pstDrvImage->stBufAddr[1].u32Stride_Cr = pstUnfImage->stVideoFrameAddr[1].u32CrStride;
    pstDrvImage->u32Width = pstUnfImage->u32Width;
    pstDrvImage->u32Height = pstUnfImage->u32Height;
    pstDrvImage->u32SrcPts = pstUnfImage->u32SrcPts;
    pstDrvImage->u32Pts   = pstUnfImage->u32Pts;
    pstDrvImage->u32AspectWidth       = pstUnfImage->u32AspectWidth;
    pstDrvImage->u32AspectHeight      = pstUnfImage->u32AspectHeight;
    pstDrvImage->u32FrameRate = pstUnfImage->stFrameRate.u32fpsInteger;

    GetPixFormat(pstUnfImage->enVideoFormat, &pstDrvImage->ePixFormat);

    pstDrvImage->bProgressive    = pstUnfImage->bProgressive;

    switch (pstUnfImage->enFieldMode)
    {
        case HI_UNF_VIDEO_FIELD_ALL:
            pstDrvImage->enFieldMode = HI_DRV_FIELD_ALL;
            break;
        case HI_UNF_VIDEO_FIELD_TOP:
            pstDrvImage->enFieldMode = HI_DRV_FIELD_TOP;
            break;
        case HI_UNF_VIDEO_FIELD_BOTTOM:
            pstDrvImage->enFieldMode = HI_DRV_FIELD_BOTTOM;
            break;
        default:
            pstDrvImage->enFieldMode = HI_DRV_FIELD_BUTT;
            break;
    }
    pstDrvImage->bTopFieldFirst = pstUnfImage->bTopFieldFirst;
    pstDrvImage->stDispRect.s32Height = pstUnfImage->u32DisplayHeight;
    pstDrvImage->stDispRect.s32Width  = pstUnfImage->u32DisplayWidth;
    pstDrvImage->stDispRect.s32X      = pstUnfImage->u32DisplayCenterX;
    pstDrvImage->stDispRect.s32Y      = pstUnfImage->u32DisplayCenterY;

    GetFrameType(pstUnfImage->enFramePackingType, &pstDrvImage->eFrmType);

    pstDrvImage->u32Circumrotate = pstUnfImage->u32Circumrotate;
#ifndef __VENC_DPT_ONLY__
    pstDrvImage->bToFlip_H = pstUnfImage->bHorizontalMirror;
#endif
    pstDrvImage->bToFlip_V = pstUnfImage->bVerticalMirror;
    pstDrvImage->u32ErrorLevel = pstUnfImage->u32ErrorLevel;

    memcpy(pstDrvImage->u32Priv , pstUnfImage->u32Private, sizeof(HI_U32) * 64);
    return HI_SUCCESS;
}

static HI_S32 Judge_FrameInfo_forVPSS(HI_DRV_VIDEO_FRAME_S* pstDrvImage)
{
    HI_BOOL ErrCfg = HI_FALSE;
    D_VENC_CHECK_PTR(pstDrvImage);

    ErrCfg |= (pstDrvImage->bProgressive   != 1                    );
    ErrCfg |= (pstDrvImage->enFieldMode    != HI_DRV_FIELD_ALL     );
    ErrCfg |= (pstDrvImage->eFrmType       != HI_DRV_FT_NOT_STEREO );
    ErrCfg |= (pstDrvImage->u32Width       == 0                    );
    ErrCfg |= (pstDrvImage->u32Height      == 0                    );
    ErrCfg |= (pstDrvImage->u32Width        > (4096)               );
    ErrCfg |= (pstDrvImage->u32Height       > (2048)               );
    if (ErrCfg)
    {
        HI_ERR_VENC("Judge_FrameInfo_forVPSS failed!!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static HI_VOID GetVideoFormat(HI_DRV_PIX_FORMAT_E PixFormat, HI_UNF_VIDEO_FORMAT_E *pVideoFormat)
{
    switch (PixFormat)
    {
        case HI_DRV_PIX_FMT_NV61_2X1:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422 ;
            break;

        case HI_DRV_PIX_FMT_NV12:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV;
            break;

        case HI_DRV_PIX_FMT_NV21:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420 ;
            break;

        case HI_DRV_PIX_FMT_NV80:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_400;
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_411;
            break;

        case HI_DRV_PIX_FMT_NV61:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2;
            break;

        case HI_DRV_PIX_FMT_NV42:
            *pVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_444;
            break;

        case HI_DRV_PIX_FMT_UYVY:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_UYVY;
            break;

        case HI_DRV_PIX_FMT_YUYV:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
            break;

        case HI_DRV_PIX_FMT_YVYU:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YVYU;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_400;
            break;

        case HI_DRV_PIX_FMT_YUV411:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_411;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_420;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_1X2;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_422_2X1;
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_444;
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            *pVideoFormat = HI_UNF_FORMAT_YUV_PLANAR_410;
            break;

        default:
            *pVideoFormat = HI_UNF_FORMAT_YUV_BUTT;
            break;
    }
}
static HI_S32 Convert_DrvFrameStructure(HI_DRV_VIDEO_FRAME_S* pstDrvImage, HI_UNF_VIDEO_FRAME_INFO_S* pstUnfImage)
{
    D_VENC_CHECK_PTR(pstUnfImage);
    D_VENC_CHECK_PTR(pstDrvImage);
    pstUnfImage->u32FrameIndex  = pstDrvImage->u32FrameIndex;
    pstUnfImage->stVideoFrameAddr[0].u32YAddr = pstDrvImage->stBufAddr[0].u32PhyAddr_Y ;
    pstUnfImage->stVideoFrameAddr[0].u32YStride = pstDrvImage->stBufAddr[0].u32Stride_Y ;
    pstUnfImage->stVideoFrameAddr[0].u32CAddr = pstDrvImage->stBufAddr[0].u32PhyAddr_C ;
    pstUnfImage->stVideoFrameAddr[0].u32CStride = pstDrvImage->stBufAddr[0].u32Stride_C ;
    pstUnfImage->stVideoFrameAddr[0].u32CrAddr = pstDrvImage->stBufAddr[0].u32PhyAddr_Cr;
    pstUnfImage->stVideoFrameAddr[0].u32CrStride = pstDrvImage->stBufAddr[0].u32Stride_Cr ;

    pstUnfImage->stVideoFrameAddr[1].u32YAddr = pstDrvImage->stBufAddr[1].u32PhyAddr_Y ;
    pstUnfImage->stVideoFrameAddr[1].u32YStride = pstDrvImage->stBufAddr[1].u32Stride_Y ;
    pstUnfImage->stVideoFrameAddr[1].u32CAddr = pstDrvImage->stBufAddr[1].u32PhyAddr_C ;
    pstUnfImage->stVideoFrameAddr[1].u32CStride = pstDrvImage->stBufAddr[1].u32Stride_C ;
    pstUnfImage->stVideoFrameAddr[1].u32CrAddr = pstDrvImage->stBufAddr[1].u32PhyAddr_Cr;
    pstUnfImage->stVideoFrameAddr[1].u32CrStride = pstDrvImage->stBufAddr[1].u32Stride_Cr ;

    pstUnfImage->u32Width = pstDrvImage->u32Width;
    pstUnfImage->u32Height = pstDrvImage->u32Height;
    pstUnfImage->u32SrcPts = pstDrvImage->u32SrcPts;
    pstUnfImage->u32Pts = pstDrvImage->u32Pts ;
    pstUnfImage->u32AspectWidth = pstDrvImage->u32AspectWidth;
    pstUnfImage->u32AspectHeight = pstDrvImage->u32AspectHeight;
    pstUnfImage->stFrameRate.u32fpsInteger = pstDrvImage->u32FrameRate;

    GetVideoFormat(pstDrvImage->ePixFormat, &pstUnfImage->enVideoFormat);

    pstUnfImage->bProgressive = pstDrvImage->bProgressive;

    switch (pstDrvImage->enFieldMode)
    {
        case HI_DRV_FIELD_ALL:
            pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
            break;
        case HI_DRV_FIELD_TOP:
            pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_TOP;
            break;
        case HI_DRV_FIELD_BOTTOM:
            pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_BOTTOM;
            break;
        default:
            pstUnfImage->enFieldMode = HI_UNF_VIDEO_FIELD_BUTT;
            break;
    }
    pstUnfImage->bTopFieldFirst = pstDrvImage->bTopFieldFirst;
    pstUnfImage->u32DisplayHeight = pstDrvImage->stDispRect.s32Height;
    pstUnfImage->u32DisplayWidth = pstDrvImage->stDispRect.s32Width;
    pstUnfImage->u32DisplayCenterX = pstDrvImage->stDispRect.s32X;
    pstUnfImage->u32DisplayCenterY = pstDrvImage->stDispRect.s32Y;
    switch(pstDrvImage->eFrmType)
    {
         case HI_DRV_FT_NOT_STEREO:
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_NONE;
            break;
         case HI_DRV_FT_SBS:
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE;
            break;
         case HI_DRV_FT_TAB:
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM;
            break;
         case HI_DRV_FT_FPK:
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_FRAME_PACKING;
            break;
        case (HI_DRV_FRAME_TYPE_E)(HI_DRV_FT_FPK+1):
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_3D_TILE;
            break;
         default:
            pstUnfImage->enFramePackingType = HI_UNF_FRAME_PACKING_TYPE_BUTT;  //QS
            break;
    }
    pstUnfImage->u32Circumrotate = pstDrvImage->u32Circumrotate;
#ifdef __VENC_DPT_ONLY__
    pstUnfImage->bHorizontalMirror = 0;//pstDrvImage->bToFlip_H;
#else
    pstUnfImage->bHorizontalMirror = pstDrvImage->bToFlip_H;
#endif
    pstUnfImage->bVerticalMirror = pstDrvImage->bToFlip_V;
    pstUnfImage->u32ErrorLevel = pstDrvImage->u32ErrorLevel;

    memcpy( pstUnfImage->u32Private, pstDrvImage->u32Priv , sizeof(HI_U32) * 64);

    return HI_SUCCESS;
}

HI_VOID VENC_DRV_InitEvent(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        //init_waitqueue_head(&g_astVencWait[i]);
        VENC_DRV_OsalInitEvent(&g_VencWait_Stream[i], 0);
        spin_lock_init(&g_SendFrame_Lock[i]);
    }

    init_waitqueue_head(&gEncodeFrame);
    VENC_DRV_OsalInitEvent(&g_VENC_Event_stream, 0);  //change by l00228308

    gencodeframe = 0;

#if (defined(__VENC_S40V200_CONFIG__) || defined(__VENC_S5V100_CONFIG__) || defined(__VENC_98CV200_CONFIG__) )    //s5v100 & s40v200 & 98cv200 have no vedu_chip_id limit!
    g_vedu_chip_id = 2;
#elif defined(__VENC_98M_CONFIG__)
    g_vedu_chip_id = g_pstRegPeri->PERI_SOC_FUSE_0.bits.vedu_chip_id;
    switch (g_vedu_chip_id)
    {
        case 0: /*1080P*/
            g_vedu_chip_id = 2;
            break;
        case 1: /*720P*/
            g_vedu_chip_id = 1;
            break;
        default: /*D1*/
            g_vedu_chip_id = 0;
            break;
    }
#else                                    //lmq_tmp
    g_vedu_chip_id = g_pstRegPeri->PERI_SOC_FUSE.bits.vedu_chip_id;
#endif

}

HI_VOID VENC_DRV_BoardInit(HI_VOID)
{

    U_PERI_CRG35 unTmpValue;
#ifdef HI_SMMU_SUPPORT
    U_PERI_CRG203 unTmpSmmuCfg;

     /*if support SMMU should open the smmu clock*/
    unTmpSmmuCfg.bits.venc_smmu_cken      = 1;
    unTmpSmmuCfg.bits.venc_smmu_srst_req  = 0;
    g_pstRegCrg->PERI_CRG203.u32 = unTmpSmmuCfg.u32;

#endif

    memset(&unTmpValue,0,sizeof(U_PERI_CRG35));
    /* reset */
    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

    //msleep(1);

    /* open vedu clock */
#ifdef __VENC_98CV200_CONFIG__
    unTmpValue.bits.venc_core_cken = 1;
    unTmpValue.bits.venc_axi_cken  = 1;
#else
    unTmpValue.bits.venc_cken = 1;
#endif

#ifdef  __VENC_98M_CONFIG__
   #ifdef __VENC98CV200_B_EXT__
        /* 98cv200_a/b: 0:500Mkz, 01:DPLL postdiv输出，466/533MHz；10:跟mde1 axi总线时钟同频；11:保留*/
        unTmpValue.bits.venc_clk_sel  = 2;
   #else
        /* 98M:config vedu clock frequency: 0:200Mhz  2: 150M*/
        unTmpValue.bits.venc_clk_sel  = 2;
   #endif
#else
   #ifndef  __VENC_S5V200L_EXT_CONFIG__
       unTmpValue.bits.venc_clk_sel  = 0;   //选用默认频率
   #endif

   #ifdef __VENC_98CV200_CONFIG__
       unTmpValue.bits.venc_clk_sel  = 1;   //0:270Mhz; 1:200Mkz
   #endif
#endif
    /* cancel reset */
    unTmpValue.bits.venc_srst_req = 0;
    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

    //msleep(1);   //change by l00228308
}

HI_VOID VENC_DRV_BoardDeinit(HI_VOID)
{
    U_PERI_CRG35 unTmpValue;
#ifdef HI_SMMU_SUPPORT
    U_PERI_CRG203 unTmpSmmuCfg;
#endif

    /* reset */
    unTmpValue.bits.venc_srst_req = 1;
    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

    //msleep(1);  change by l00228308

    /* close vedu clock */
#ifdef __VENC_98CV200_CONFIG__
    unTmpValue.bits.venc_core_cken = 0;
    unTmpValue.bits.venc_axi_cken  = 0;
#else
    unTmpValue.bits.venc_cken = 0;
#endif

#ifdef HI_SMMU_SUPPORT
     /*if support SMMU should open the smmu clock*/
    unTmpSmmuCfg.bits.venc_smmu_cken      = 0;
    unTmpSmmuCfg.bits.venc_smmu_srst_req  = 1;
    g_pstRegCrg->PERI_CRG203.u32 = unTmpSmmuCfg.u32;
#endif

    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;
}

static HI_VOID VENC_DRV_GetSetCfgInfo(VeduEfl_EncCfg_S *pstEncCfg, HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs)
{
    switch (pVencUnfAttrs->enVencType)
    {
        case HI_UNF_VCODEC_TYPE_H264:
            pstEncCfg->Protocol = VEDU_H264;
            pstEncCfg->Profile  = (HI_U32)pVencUnfAttrs->enVencProfile;
            pstEncCfg->Gop      = pVencUnfAttrs->u32Gop;
            pstEncCfg->QLevel   = 0;
            break;

        case HI_UNF_VCODEC_TYPE_H263:
            pstEncCfg->Protocol = VEDU_H263;
            break;

        case HI_UNF_VCODEC_TYPE_MPEG4:
            pstEncCfg->Protocol = VEDU_MPEG4;
            break;

        case HI_UNF_VCODEC_TYPE_JPEG:
            pstEncCfg->Protocol = VEDU_JPGE;
            pstEncCfg->Profile  = VEDU_H264_PROFILE_BUTT;
            pstEncCfg->QLevel   = pVencUnfAttrs->u32Qlevel;
            pstEncCfg->Gop      = 0;
            break;

        default:
            pstEncCfg->Protocol = VEDU_H264;
            break;
    }
}

static HI_S32 VENC_DRV_CheckCapLevel(HI_UNF_VCODEC_CAP_LEVEL_E enCapLevel)
{
    switch (g_vedu_chip_id)
    {
        case 0:
            if (enCapLevel > HI_UNF_VCODEC_CAP_LEVEL_D1)
            {
                HI_ERR_VENC("vedu_chip_id(%d) not support create this level(%d) channel(should below D1)!\n", g_vedu_chip_id, enCapLevel);

                return HI_ERR_VENC_NOT_SUPPORT;
            }

            break;

        case 1:
            if (enCapLevel > HI_UNF_VCODEC_CAP_LEVEL_720P)
            {
                HI_ERR_VENC("vedu_chip_id(%d) not support create this level(%d) channel(should below 720p)!\n", g_vedu_chip_id, enCapLevel);

                return HI_ERR_VENC_NOT_SUPPORT;
            }

            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

static HI_VOID VENC_DRV_GetRCSkipFrmEn(VeduEfl_EncCfg_S *pstEncCfg)
{
    pstEncCfg->bRCSkipFrmEn = (pstEncCfg->DriftRCThr <= 100) ? HI_TRUE : HI_FALSE;
}

static HI_VOID VENC_DRV_GetSplitSize(VeduEfl_EncCfg_S *pstEncCfg, HI_U32 SplitSize)
{
    pstEncCfg->SplitSize = (pstEncCfg->SlcSplitEn) ? SplitSize : 0;
}

HI_S32 VENC_DRV_CreateChn(VeduEfl_EncPara_S** phVencChn, venc_chan_cfg *pstAttr,
                      VENC_CHN_INFO_S *pstVeInfo,HI_BOOL bOMXChn, struct file  *pfile)
{
    HI_S32 s32Ret = 0, i = 0;
    VeduEfl_EncCfg_S EncCfg;
    HI_HANDLE hJpgeChn = HI_INVALID_HANDLE;
    HI_UNF_VENC_CHN_ATTR_S* pVencUnfAttrs;

    VeduEfl_EncPara_S *pstEncChnPara;
    VeduEfl_RcAttr_S RcAttrCfg;
    HI_CHAR YUVFileName[64] = {""};
    HI_CHAR StreamFileName[64] = {""};

#ifdef __VENC_SUPPORT_JPGE__
    Jpge_EncCfg_S    JpgeEncCfg;
    memset(&JpgeEncCfg, 0, sizeof(Jpge_EncCfg_S));
#endif

    D_VENC_CHECK_PTR(phVencChn);
    D_VENC_CHECK_PTR(pstAttr);
    D_VENC_CHECK_PTR(pstVeInfo);

    pVencUnfAttrs = &pstAttr->VencUnfAttr;
    //w00283384 fix me here

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (g_stVencChn[i].hVEncHandle == NULL)
        {
            break;
        }
    }

    if (i == VENC_MAX_CHN_NUM)
    {
        HI_ERR_VENC("channal create err! \n");
        return HI_ERR_VENC_CREATE_ERR;
    }

#ifdef __VENC_3719MV100_CONFIG__
    if (HI_UNF_VCODEC_TYPE_JPEG == pVencUnfAttrs->enVencType)
    {
        HI_ERR_VENC("3719MV100 not support create JPEG channel!\n");
        return HI_ERR_VENC_CREATE_ERR;
    }
#endif

    VENC_DRV_GetSetCfgInfo(&EncCfg, pVencUnfAttrs);

    s32Ret = VENC_DRV_CheckCapLevel(pVencUnfAttrs->enCapLevel);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

#ifdef __VENC_98CV200_B_EXT__
    if (pVencUnfAttrs->enCapLevel > HI_UNF_VCODEC_CAP_LEVEL_720P)
    {
        HI_ERR_VENC("This Chip(98cv200_b) not support create this level(%d) channel(should below 720p)!\n", pVencUnfAttrs->enCapLevel);
        return HI_ERR_VENC_NOT_SUPPORT;
    }
#endif

    EncCfg.bAutoRequestIfrm = pstAttr->bAutoRequestIFrm;
    EncCfg.bPrependSpsPps = pstAttr->bPrependSpsPps;
    EncCfg.CapLevel      = pVencUnfAttrs->enCapLevel;
    EncCfg.FrameWidth    = pVencUnfAttrs->u32Width;
    EncCfg.FrameHeight   = pVencUnfAttrs->u32Height;
    EncCfg.RotationAngle = pVencUnfAttrs->u32RotationAngle;
    EncCfg.Priority      = pVencUnfAttrs->u8Priority;
    EncCfg.streamBufSize = pVencUnfAttrs->u32StrmBufSize;
    EncCfg.QuickEncode   = pVencUnfAttrs->bQuickEncode;
    EncCfg.SlcSplitEn    = (HI_TRUE == pVencUnfAttrs->bSlcSplitEn) ? 1 : 0;
    EncCfg.DriftRCThr    = pVencUnfAttrs->u32DriftRateThr;

    VENC_DRV_GetRCSkipFrmEn(&EncCfg);

    VENC_DRV_GetSplitSize(&EncCfg, pVencUnfAttrs->u32SplitSize);   

    EncCfg.bOMXChn  = bOMXChn;

    s32Ret = VENC_DRV_EflCreateVenc(phVencChn, &EncCfg);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

#ifdef __VENC_SUPPORT_JPGE__
    if (EncCfg.Protocol == VEDU_JPGE)
    {   
        if (pJpgeFunc == HI_NULL)
        {
            HI_ERR_VENC("pJpgeFunc is NULL!\n");
            goto create_error_1;
        }
        JpgeEncCfg.FrameWidth  = pVencUnfAttrs->u32Width;
        JpgeEncCfg.FrameHeight = pVencUnfAttrs->u32Height;
        JpgeEncCfg.SlcSplitEn  = 0;                         //EncCfg.SlcSplitEn;
        JpgeEncCfg.SplitSize   = EncCfg.SplitSize;
        JpgeEncCfg.Qlevel      = pVencUnfAttrs->u32Qlevel;
        
        s32Ret = pJpgeFunc->pfnJpgeCreateChn(&hJpgeChn, &JpgeEncCfg);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("pfnJpgeCreateChn Failed!\n");
            s32Ret = HI_ERR_VENC_CREATE_ERR;
            goto create_error_1;
        }

        s32Ret = VENC_DRV_EflAllocBuf2Jpge(*phVencChn, EncCfg.CapLevel);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("pfnJpgeCreateChn Failed!\n");
            s32Ret = HI_ERR_VENC_CREATE_ERR;
            goto create_error_2;
        }
    }
#endif

    pstEncChnPara = (VeduEfl_EncPara_S*)(*phVencChn);
    RcAttrCfg.BitRate    = pVencUnfAttrs->u32TargetBitRate;
    RcAttrCfg.InFrmRate  = pVencUnfAttrs->u32InputFrmRate;
    RcAttrCfg.OutFrmRate = pVencUnfAttrs->u32TargetFrmRate;
    RcAttrCfg.MaxQp      = pVencUnfAttrs->u32MaxQp;
    RcAttrCfg.MinQp      = pVencUnfAttrs->u32MinQp;
    s32Ret = VENC_DRV_EflRcAttrInit(*phVencChn, &RcAttrCfg);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("config venc err:%#x.\n", s32Ret);
        s32Ret = HI_ERR_VENC_INVALID_PARA;
        goto create_error_3;
    }

    /*creat one proc file*/
    s32Ret = VENC_DRV_ProcAdd(*phVencChn, i);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("venc_ProcAdd failed, ret = 0x%08x\n", s32Ret);
        s32Ret = HI_ERR_VENC_CREATE_ERR;
        goto create_error_3;
    }

    pstVeInfo->handle            = g_stVencChn[i].hUsrHandle;//*phVencChn;
    pstVeInfo->u32StrmBufPhyAddr = pstEncChnPara->StrmBufAddr;
    pstVeInfo->u32BufSize        = pstEncChnPara->StrmBufSize;
    pstVeInfo->u32StrmBufPhyAddrJpeg = pstEncChnPara->JpgMMZBuf.u32StartPhyAddr;

    pstVeInfo->u32BufSizeJpeg    = pstEncChnPara->JpgMMZBuf.u32Size;
    g_stVencChn[i].StrmBufAddr   = pstEncChnPara->StrmBufAddr;
    g_stVencChn[i].StrmBufSize   = pstEncChnPara->StrmBufSize;
    g_stVencChn[i].hVEncHandle   = *phVencChn;
    g_stVencChn[i].hSource       = HI_INVALID_HANDLE;
    g_stVencChn[i].pWhichFile    = pfile;
    g_stVencChn[i].u32SliceSize            = EncCfg.SplitSize;
    g_stVencChn[i].u32FrameNumLastInput    = 0;
    g_stVencChn[i].u32FrameNumLastEncoded  = 0;
    g_stVencChn[i].u32TotalByteLastEncoded = 0;
    g_stVencChn[i].u32LastSecInputFps      = 0;
    g_stVencChn[i].u32LastSecEncodedFps    = 0;
    g_stVencChn[i].u32LastSecKbps          = 0;
    g_stVencChn[i].u32LastSecTryNum        = 0;
    g_stVencChn[i].u32LastTryNumTotal      = 0;
    g_stVencChn[i].u32LastSecOKNum         = 0;
    g_stVencChn[i].u32LastOKNumTotal       = 0;
    g_stVencChn[i].u32LastSecPutNum        = 0;
    g_stVencChn[i].u32LastPutNumTotal      = 0;
    
    g_stVencChn[i].u32LastStrmGetTryNumTotal = 0;
    g_stVencChn[i].u32LastStrmGetOKNumTotal = 0;
    g_stVencChn[i].u32LastStrmRlsOKNumTotal = 0;
    g_stVencChn[i].u32LastStrmRlsTryNumTotal = 0;    
    g_stVencChn[i].u32LastSecStrmGetTryNum = 0;
    g_stVencChn[i].u32LastSecStrmGetOKNum  = 0;
    g_stVencChn[i].u32LastSecStrmRlsTryNum = 0;
    g_stVencChn[i].u32LastSecStrmRlsOKNum  = 0;
    
    g_stVencChn[i].bNeedVPSS               = HI_FALSE;
    g_stVencChn[i].bFrameBufMng            = HI_TRUE;
    g_stVencChn[i].bEnable                 = HI_FALSE;
    g_stVencChn[i].enSrcModId              = HI_ID_BUTT;
    g_stVencChn[i].bOMXChn                 = bOMXChn;
    g_stVencChn[i].stProcWrite.bFrameModeRun     = HI_FALSE;
    g_stVencChn[i].stProcWrite.bTimeModeRun      = HI_FALSE;
    g_stVencChn[i].stProcWrite.bSaveYUVFileRun   = HI_FALSE;
    g_stVencChn[i].stProcWrite.u32FrameModeCount = 0;
    g_stVencChn[i].stProcWrite.fpSaveFile        = HI_NULL;
    g_stVencChn[i].hJPGE                         = hJpgeChn;
    g_stVencChn[i].bWithoutVPSS                  = (HI_BOOL)pstEncChnPara->WithoutVPSS;

    g_stVencChn[i].stChnUserCfg  = *pVencUnfAttrs;
    if (EncCfg.Protocol == VEDU_JPGE)
    {
        g_stVencChn[i].stChnUserCfg.u32Gop = 0;
        g_stVencChn[i].stChnUserCfg.u32TargetBitRate = 0;
        g_stVencChn[i].stChnUserCfg.u32MaxQp = 0;
        g_stVencChn[i].stChnUserCfg.u32MinQp = 0;
        g_stVencChn[i].stChnUserCfg.enVencProfile = HI_UNF_H264_PROFILE_BUTT;
    }
    else
    {
        g_stVencChn[i].stChnUserCfg.u32Qlevel = 0;
    }

    memset(&g_stKernelOmxmmz[i],0,sizeof(VENC_BUFFER_S)*OMX_OUTPUT_PORT_BUFFER_MAX);

    s32Ret = HI_OSAL_Snprintf(YUVFileName, sizeof(YUVFileName), "venc_proc_chn%02d.yuv", i);
    if (0 == s32Ret)
    {
        HI_ERR_VENC("HI_OSAL_Snprintf failed!\n");
    }
    memcpy(g_stVencChn[i].stProcWrite.YUVFileName, YUVFileName, 64);
    s32Ret = HI_OSAL_Snprintf(StreamFileName, sizeof(StreamFileName), "venc_proc_chn%02d.h264", i);
    if (0 == s32Ret)
    {
        HI_ERR_VENC("HI_OSAL_Snprintf failed!\n");
    }
    memcpy(g_stVencChn[i].stProcWrite.StreamFileName, StreamFileName, 64);
    HI_INFO_VENC("create OK, Chn:%d/%#x.\n", i, g_stVencChn[i].hVEncHandle);

#ifdef __VENC_RC_DBG__

    g_stVencChn[i].stProcWrite.fpSaveFile = VENC_DRV_OsalFopen("/mnt/yuv_stream/venc_rc_dbg.h264",  O_RDWR | O_CREAT | O_APPEND, 0);
    if (HI_NULL == g_stVencChn[i].stProcWrite.fpSaveFile)
    {
        HI_ERR_VENC("Can not create file.\n");
        g_stVencChn[i].stProcWrite.bTimeModeRun = HI_FALSE;
        return HI_FAILURE;
    }
    g_stVencChn[i].stProcWrite.bTimeModeRun = HI_TRUE;
#endif

    return HI_SUCCESS;

create_error_3:
#ifdef __VENC_SUPPORT_JPGE__
    if (EncCfg.Protocol == VEDU_JPGE)
    {
        VENC_DRV_EflFreeBuf2Jpge(*phVencChn);
    }
#endif
#ifdef __VENC_SUPPORT_JPGE__
create_error_2:
    if (EncCfg.Protocol == VEDU_JPGE)
    {
        if (pJpgeFunc != HI_NULL)
        {
            pJpgeFunc->pfnJpgeDestroyChn(hJpgeChn);
        }
    }
create_error_1:
    VENC_DRV_EflDestroyVenc(*phVencChn);
#endif
    return s32Ret;

}

HI_S32 VENC_DRV_DestroyChn( VeduEfl_EncPara_S* hVencChn)
{
    HI_S32 s32Ret   = 0;
    HI_U32 u32VeChn = 0;
    OPTM_VENC_CHN_S* pstVenc;
    unsigned long flags;
    //VI_EXPORT_FUNC_S *pViFunc = HI_NULL;
    VeduEfl_EncPara_S*  pstEncChnPara;
    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    spin_lock_irqsave(&g_SendFrame_Lock[u32VeChn], flags);
    pstVenc = &g_stVencChn[u32VeChn];
    pstEncChnPara = g_stVencChn[u32VeChn].hVEncHandle;
    spin_unlock_irqrestore(&g_SendFrame_Lock[u32VeChn], flags);
    //VENC must be stop working
    if (pstVenc->bEnable)
    {
        HI_WARN_VENC("Error:Destroy channel when VENC is run.\n");
        VENC_DRV_EflStopVenc(hVencChn);
        VENC_DRV_OsalGiveEvent(&g_VencWait_Stream[u32VeChn]);
    }

#ifdef __VENC_RC_DBG__
    g_stVencChn[u32VeChn].stProcWrite.bTimeModeRun = HI_FALSE;
    VENC_DRV_OsalFclose(g_stVencChn[u32VeChn].stProcWrite.fpSaveFile);

#endif

    switch (pstVenc->enSrcModId)
    {
        case HI_ID_DISP:
        {
            pstEncChnPara->stSrcInfo.pfDetachFunc(pstVenc->hSource, pstVenc->hUsrHandle);
        }
        break;
        default:
            break;
    }
    /**/
    VENC_DRV_ProcDel(hVencChn, u32VeChn);

#ifdef __VENC_SUPPORT_JPGE__
    if ((HI_INVALID_HANDLE != g_stVencChn[u32VeChn].hJPGE) && (pJpgeFunc != HI_NULL))
    {
        s32Ret = pJpgeFunc->pfnJpgeDestroyChn(g_stVencChn[u32VeChn].hJPGE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_VENC("Error:Destroy JGPE ERR.\n");
        }

        VENC_DRV_EflFreeBuf2Jpge(hVencChn);
    }
#endif

    s32Ret = VENC_DRV_EflDestroyVenc(hVencChn);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

#if 0
    if (g_stVencChn[s32VeChn].hSource != HI_INVALID_HANDLE)
    {

        if (HI_ID_VI == g_stVencChn[s32VeChn].enSrcModId)
        {
            HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
            if (HI_NULL != pViFunc)
            {
                s32Ret = pViFunc->pfnViPutUsrID(pstVenc->hSource & 0xff, pstVenc->u32SrcUser);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VENC("ViuPutUsrID failed, Ret=%#x.\n", s32Ret);
                    return s32Ret;
                }
            }
        }
    }
#endif

    spin_lock_irqsave(&g_SendFrame_Lock[u32VeChn], flags);
    g_stVencChn[u32VeChn].hSource     = HI_INVALID_HANDLE;
    g_stVencChn[u32VeChn].hVEncHandle = NULL;;
    g_stVencChn[u32VeChn].bNeedVPSS   = HI_FALSE;
    g_stVencChn[u32VeChn].bFrameBufMng = HI_TRUE;
    g_stVencChn[u32VeChn].bOMXChn     = HI_FALSE;
    g_stVencChn[u32VeChn].bEnable     = HI_FALSE;

    spin_unlock_irqrestore(&g_SendFrame_Lock[u32VeChn], flags);
    HI_INFO_VENC("VENC_DestroyChn %d OK\n", u32VeChn);

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_AttachInput(VeduEfl_EncPara_S* hVencChn, HI_HANDLE hSrc, HI_MOD_ID_E enModId )
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32VeChn = 0;
    OPTM_VENC_CHN_S* pstVenc;
    VeduEfl_SrcInfo_S stSrcInfo;
    //VI_EXPORT_FUNC_S *pViFunc = HI_NULL;
    WIN_EXPORT_FUNC_S *pVoFunc = HI_NULL;
    DISP_EXPORT_FUNC_S* pDispFunc = HI_NULL; //to be continued with vdp

    if (enModId >= HI_ID_BUTT)
    {
        return HI_ERR_VENC_INVALID_PARA;
    }

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    memset(&stSrcInfo,0,sizeof(VeduEfl_SrcInfo_S));
    pstVenc = &g_stVencChn[u32VeChn];

    if (HI_TRUE == pstVenc->bEnable)
    {
        HI_ERR_VENC("VENC has already start! please attach before start!\n");
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }
    if ((enModId != HI_ID_VI) && (enModId != HI_ID_VO)&& (enModId != HI_ID_DISP))
    {
        HI_ERR_VENC("ModId not surpport now, enModId=%x!\n", enModId);
        return HI_ERR_VENC_INVALID_PARA;
    }

    if (pstVenc->hSource != HI_INVALID_HANDLE)
    {
        HI_ERR_VENC("Venc%d already attached to %#x!\n", u32VeChn, pstVenc->hSource);
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }


    if ( HI_ID_DISP == enModId )
    {
        ret = HI_DRV_MODULE_GetFunction(HI_ID_DISP, (HI_VOID**)&pDispFunc);
        if (ret)
        {
            HI_ERR_VENC("GetFunction for Module(%d),failed!\n", HI_ID_DISP);
            return HI_FAILURE;
        }
        if (HI_NULL != pDispFunc)
        {
            stSrcInfo.handle     = hSrc;
            stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
            stSrcInfo.pfPutImage = (VE_IMAGE_FUNC)(pDispFunc->pfnDispRlsCastFrm);
            stSrcInfo.pfDetachFunc = (VE_DETACH_FUNC)(pDispFunc->pfnDispExtDeAttach);
            stSrcInfo.pfChangeInfo = (VE_CHANGE_INFO_FUNC)(pDispFunc->pfnDispSetCastAttr);
        }
        D_VENC_CHECK_PTR(stSrcInfo.pfChangeInfo);
        ret = (stSrcInfo.pfChangeInfo)(hSrc,
                            pstVenc->stChnUserCfg.u32Width,
                            pstVenc->stChnUserCfg.u32Height,
                            VEDU_MAX_INPUT_FRAME_RATE);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_VENC("stSrcInfo.pfChangeInfo failed!\n");
            
            return HI_FAILURE;
        }

        ret = VENC_DRV_EflAttachInput(hVencChn, &stSrcInfo);
    }
    else if ( HI_ID_VO == enModId )
    {
        ret = HI_DRV_MODULE_GetFunction(HI_ID_VO, (HI_VOID**)&pVoFunc);
        if (ret)
        {
            HI_ERR_VENC("GetFunction for Module(%d),failed!\n", HI_ID_VO);
            return HI_FAILURE;
        }

        if (HI_NULL != pVoFunc)
        {
            stSrcInfo.handle     = hSrc;
            stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
            stSrcInfo.pfPutImage = (VE_IMAGE_FUNC)(pVoFunc->pfnWinRlsFrm);
            stSrcInfo.pfDetachFunc = NULL;
            stSrcInfo.pfChangeInfo = (VE_CHANGE_INFO_FUNC)(pVoFunc->pfnWinSetVirAttr);

        }
        D_VENC_CHECK_PTR(stSrcInfo.pfChangeInfo);
        ret = (stSrcInfo.pfChangeInfo)(hSrc,
                            pstVenc->stChnUserCfg.u32Width,
                            pstVenc->stChnUserCfg.u32Height,
                            VEDU_MAX_INPUT_FRAME_RATE);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_VENC("stSrcInfo.pfChangeInfo failed!\n");
            
            return HI_FAILURE;
        }
        ret = VENC_DRV_EflAttachInput(hVencChn, &stSrcInfo);
    }

    if (HI_SUCCESS == ret)
    {
        pstVenc->enSrcModId   = enModId;
        pstVenc->hSource      = hSrc;
        pstVenc->bFrameBufMng = HI_TRUE;
        HI_INFO_VENC("VENC%d attchInputOK, srcHdl:%#x, UserHdl:%#x.\n", u32VeChn, pstVenc->hSource, pstVenc->u32SrcUser);
    }
    return ret;
}

HI_S32 VENC_DRV_SetSrcInfo(VeduEfl_EncPara_S* hVencChn, HI_DRV_VENC_SRC_HANDLE_S* pstSrc)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32VeChn = 0;
    VeduEfl_SrcInfo_S stSrcInfo;
    OPTM_VENC_CHN_S* pstVenc;
    VI_EXPORT_FUNC_S* pViFunc = HI_NULL; //to be continued with vdp
    VeduEfl_EncPara_S*  pEncPara;
    
    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);
    D_VENC_CHECK_PTR(pstSrc);
    
    pstVenc = &g_stVencChn[u32VeChn];
    pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32VeChn].hVEncHandle;

    memset(&stSrcInfo,0,sizeof(VeduEfl_SrcInfo_S));

    if (pstVenc->hSource == HI_INVALID_HANDLE)
    {
        HI_ERR_VENC("Venc%d haven't attach any src!\n", u32VeChn);
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    switch (pstSrc->enSrcMode)
    {
        case HI_ID_VI:
        {
            s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
            if ((HI_SUCCESS != s32Ret)||(HI_NULL == pViFunc))
            {
                HI_ERR_VENC("GetFunction for Module(%d),failed!\n", HI_ID_VI);
                return HI_FAILURE;
            }
        
            stSrcInfo.handle     = pstSrc->hSrc;
            stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
            stSrcInfo.pfPutImage = (VE_IMAGE_FUNC)(pViFunc->pfnVencRlsFrame);
            stSrcInfo.pfDetachFunc = NULL;
            stSrcInfo.pfChangeInfo = (VE_CHANGE_INFO_FUNC)(pViFunc->pfnDispSetCastAttr);
            pstVenc->bFrameBufMng = HI_TRUE;

            D_VENC_CHECK_PTR(stSrcInfo.pfChangeInfo);
            
            s32Ret = (stSrcInfo.pfChangeInfo)(pstSrc->hSrc,
                                    pstVenc->stChnUserCfg.u32Width,
                                    pstVenc->stChnUserCfg.u32Height,
                                    VEDU_MAX_INPUT_FRAME_RATE);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VENC("The first time SetInfo to VI failed!ret = %x\n", s32Ret);
                pstVenc->hSource      = HI_INVALID_HANDLE;
                pstVenc->enSrcModId   = HI_ID_BUTT;
                pstVenc->bFrameBufMng = HI_TRUE;
                return HI_FAILURE;
            }
            break;
        }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    case HI_ID_VO:
        D_VENC_CHECK_PTR(pstSrcInfo->pfChangeInfo);
        s32Ret = (pstSrcInfo->pfChangeInfo)(pstSrcInfo->hSrc,
                                pstVenc->stChnUserCfg.u32Width,
                                pstVenc->stChnUserCfg.u32Height,
                                pEncPara->VoFrmRate);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("The first time SetInfo to WINDOW failed!ret = %x\n", s32Ret);
            pstVenc->hSource      = HI_INVALID_HANDLE;
            pstVenc->enSrcModId   = HI_ID_BUTT;
            pstVenc->bFrameBufMng = HI_TRUE;
            return HI_FAILURE;
        }
        if (pstSrcInfo->pfAcqFrame != NULL)
        {
            stSrcInfo.pfGetImage   = (VE_IMAGE_FUNC)pstSrcInfo->pfAcqFrame;
            pstVenc->bFrameBufMng  = HI_FALSE;
        }
        else
        {
            stSrcInfo.pfGetImage   = VENC_DRV_EflGetImage;
        }
        stSrcInfo.handle       = pstSrcInfo->hSrc;
        stSrcInfo.pfPutImage   = (VE_IMAGE_FUNC)pstSrcInfo->pfRlsFrame;
        stSrcInfo.pfChangeInfo = (VE_CHANGE_INFO_FUNC)pstSrcInfo->pfChangeInfo;
        break;
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    default:
        HI_ERR_VENC("VENC not support set Src to Mod(%d)\n", pstVenc->enSrcModId);
        return HI_ERR_VENC_INVALID_PARA;
}
    s32Ret = VENC_DRV_EflAttachInput(hVencChn, &stSrcInfo);
    return s32Ret;
}

HI_S32 VENC_DRV_DetachInput(VeduEfl_EncPara_S* hVencChn, HI_HANDLE hSrc, HI_MOD_ID_E enModId )
{
    HI_U32 u32VeChn = 0;
    //HI_S32 s32Ret = HI_FAILURE;
    OPTM_VENC_CHN_S* pstVenc;
    VeduEfl_SrcInfo_S stSrcInfo = {0};
    //VI_EXPORT_FUNC_S *pViFunc = HI_NULL;

    if (enModId >= HI_ID_BUTT)
    {
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    pstVenc = &g_stVencChn[u32VeChn];
    if (pstVenc->hSource == HI_INVALID_HANDLE)
    {
        HI_WARN_VENC("Venc%d NOT attached.\n", u32VeChn);
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    if (pstVenc->hSource != hSrc)
    {
        HI_ERR_VENC("Venc%d NOT attached to %#x, but attached to %#x.\n", u32VeChn, hSrc, pstVenc->hSource);
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if ((enModId != HI_ID_VI) && (enModId != HI_ID_VO) && (enModId != HI_ID_DISP))
    {
        HI_ERR_VENC("Venc Detach, ModId not surpport now, enModId=%x!\n", enModId);
        return HI_ERR_VENC_INVALID_PARA;
    }

    //VENC must be stop working
    if (pstVenc->bEnable)
    {
        //HI_ERR_VENC("CanNOT detachInput when VENC is run.\n");
        //return HI_ERR_VENC_CHN_INVALID_STAT;
        VENC_DRV_StopReceivePic(hVencChn);

    }

#if 0
    switch (enModId)
    {
        case HI_ID_VI:
        {
            stSrcInfo.handle = HI_INVALID_HANDLE;
            stSrcInfo.pfGetImage = HI_NULL;
            stSrcInfo.pfPutImage = HI_NULL;


            HI_DRV_MODULE_GetFunction(HI_ID_VI, (HI_VOID**)&pViFunc);
            if (HI_NULL != pViFunc)
            {
                s32Ret = pViFunc->pfnViPutUsrID(hSrc & 0xff, pstVenc->u32SrcUser);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VENC("ViuPutUsrID failed, Ret=%#x.\n", s32Ret);
                    return s32Ret;
                }
            }
            break;
        }
        case HI_ID_VO:
        {
            stSrcInfo.handle = HI_INVALID_HANDLE;
            stSrcInfo.pfGetImage = HI_NULL;
            stSrcInfo.pfPutImage = HI_NULL;
            break;
        }
        default:
            break;
    }
#endif

    stSrcInfo.handle      = HI_INVALID_HANDLE;
    stSrcInfo.pfGetImage  = HI_NULL;
    stSrcInfo.pfPutImage  = HI_NULL;
    stSrcInfo.pfChangeInfo = HI_NULL;
    stSrcInfo.pfDetachFunc = HI_NULL;

    VENC_DRV_EflDetachInput(hVencChn, &stSrcInfo);
    HI_INFO_VENC("VENC%d dettchInputOK, srcHdl:%#x, UserHdl:%#x.\n", u32VeChn, pstVenc->hSource, pstVenc->u32SrcUser);

    g_stVencChn[u32VeChn].enSrcModId = HI_ID_BUTT;
    g_stVencChn[u32VeChn].hSource = HI_INVALID_HANDLE;
    g_stVencChn[u32VeChn].bFrameBufMng = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_GetJpgeStream(VeduEfl_NALU_S *pstVeduPacket, VENC_STREAM_S *pstStream, VeduEfl_EncPara_S *hVencChn,
                              HI_U8 *StartVirAddr, VENC_BUF_OFFSET_S *pstBufOffSet)
{
    HI_S32 s32Ret = -1;
    HI_U32 u32StreamTemp = 0;
    HI_S32 u32VeChn = 0;
    HI_U32 RecycleCnt = 0;
    HI_VOID* pVirAddr = (HI_VOID*)StartVirAddr;

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    if (pstVeduPacket->SlcLen[0] > 0)
    {
        memcpy(pVirAddr, pstVeduPacket->pVirtAddr[0], pstVeduPacket->SlcLen[0]);
    }

    if (pstVeduPacket->SlcLen[1] > 0)
    {
        memcpy(pVirAddr + pstVeduPacket->SlcLen[0], pstVeduPacket->pVirtAddr[1], pstVeduPacket->SlcLen[1]);
    }

    u32StreamTemp = pstVeduPacket->SlcLen[0] + pstVeduPacket->SlcLen[1];

    s32Ret = VENC_DRV_EflSkpBitStream(hVencChn, pstVeduPacket);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("Release stream failed, ret= %#x.\n", s32Ret);

        return HI_ERR_VENC_CHN_RELEASE_ERR;
    }

    do
    {
        memset(pstVeduPacket, 0, sizeof(VeduEfl_NALU_S));
        s32Ret = VENC_DRV_EflGetBitStream(hVencChn, pstVeduPacket);

        if (s32Ret != HI_SUCCESS) 
        { 
            msleep(5); 
            RecycleCnt++;
        }
    }
    while (((s32Ret != HI_SUCCESS) || (pstVeduPacket->bFrameEnd != HI_TRUE)) && (RecycleCnt < 15));

    if (RecycleCnt >= 15)
    {
        HI_ERR_VENC("The time of getting stream is too long!\n");
    }

    if (pstVeduPacket->SlcLen[0] > 0)
    {
        memcpy(pVirAddr + u32StreamTemp , pstVeduPacket->pVirtAddr[0], pstVeduPacket->SlcLen[0]);
        u32StreamTemp += pstVeduPacket->SlcLen[0];
    }

    if (pstVeduPacket->SlcLen[1] > 0)
    {
        memcpy(pVirAddr + u32StreamTemp, pstVeduPacket->pVirtAddr[1], pstVeduPacket->SlcLen[1]);
        u32StreamTemp += pstVeduPacket->SlcLen[1];
    }

    s32Ret = VENC_DRV_EflSkpBitStream(hVencChn, pstVeduPacket);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("Release stream failed, ret= %#x.\n", s32Ret);

        return HI_ERR_VENC_CHN_RELEASE_ERR;
    }

    pstStream->VirAddr      = (HI_VIRT_ADDR_T)pVirAddr;
    pstStream->u32SlcLen    = u32StreamTemp;
    pstStream->u32PtsMs     = pstVeduPacket->PTS0;

    pstStream->bFrameEnd    = (0 == pstVeduPacket->bFrameEnd) ? HI_FALSE : HI_TRUE;
    pstStream->enDataType.enH264EType = HI_UNF_H264E_NALU_BUTT;

    if (pstStream->u32SlcLen > 0)
    {
        pstBufOffSet->u32StrmBufOffset[0]
            = pstVeduPacket->PhyAddr[0] - g_stVencChn[u32VeChn].StrmBufAddr;
    }

    return HI_SUCCESS;
}

static HI_S32 VENC_DRV_SaveStream(VENC_STREAM_S* pstStream, VeduEfl_EncPara_S* phVencChn, HI_S32 u32VeChn)
{
    HI_S32 s32Ret = HI_FAILURE;
    VeduEfl_StatInfo_S StatInfo;
    static HI_BOOL bTagFirstTimeSave = HI_TRUE;
    static HI_U32 u32SaveFrameStartCount = 0;
    HI_U32 u32SkipFrmNum = 0;

    if ((g_stVencChn[u32VeChn].stProcWrite.bTimeModeRun == HI_TRUE) || (g_stVencChn[u32VeChn].stProcWrite.bFrameModeRun == HI_TRUE))
    {
        s32Ret = VENC_DRV_EflQueryStatInfo(g_stVencChn[u32VeChn].hVEncHandle, &StatInfo);

        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VENC("VeduEfl_QueryStatInfo failed.\n");

            return HI_FAILURE;
        }

        u32SkipFrmNum = StatInfo.QuickEncodeSkip + StatInfo.SamePTSSkip + StatInfo.ErrCfgSkip
                        + StatInfo.FrmRcCtrlSkip + StatInfo.TooFewBufferSkip;

        /* request one I frame and record u32SaveFrameStartCount to compare with g_VencSaveFrameCount when save file firstly */
        if (bTagFirstTimeSave == HI_TRUE)
        {
            VENC_DRV_EflRequestIframe(phVencChn);
            bTagFirstTimeSave = HI_FALSE;
            u32SaveFrameStartCount = StatInfo.GetFrameNumOK - u32SkipFrmNum;

            return HI_SUCCESS;
        }

        /* compare with u32FrameModeCount each time */
        if ((g_stVencChn[u32VeChn].stProcWrite.bFrameModeRun == HI_TRUE)
            && (StatInfo.GetFrameNumOK - u32SkipFrmNum - u32SaveFrameStartCount) > g_stVencChn[u32VeChn].stProcWrite.u32FrameModeCount)
        {
            /* time to stop save file */
            g_stVencChn[u32VeChn].stProcWrite.bFrameModeRun = HI_FALSE;

            return HI_SUCCESS;
        }

        if (pstStream->u32SlcLen > 0)
        {
            s32Ret = VENC_DRV_OsalFwrite((char*)pstStream->VirAddr, pstStream->u32SlcLen , g_stVencChn[u32VeChn].stProcWrite.fpSaveFile);

            if (s32Ret != pstStream->u32SlcLen)
            {
                HI_ERR_VENC("VeduOsal_Fwrite failed.\n");
                g_stVencChn[u32VeChn].stProcWrite.bTimeModeRun  = HI_FALSE;
                g_stVencChn[u32VeChn].stProcWrite.bFrameModeRun = HI_FALSE;
            }
        }
    }
    else
    {
        bTagFirstTimeSave = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_AcquireStream(VeduEfl_EncPara_S* hVencChn, VENC_STREAM_S* pstStream, HI_U32 u32TimeoutMs,
                              VENC_BUF_OFFSET_S* pstBufOffSet)
{
    HI_S32 s32Ret   = -1;
    HI_S32 u32VeChn = 0;
    unsigned long flags;
    VeduEfl_NALU_S stVeduPacket;
    VeduEfl_EncPara_S* pstEncChnPara = HI_NULL;
    //extern VENC_PROC_WRITE_S g_VencProcWrite;
    D_VENC_CHECK_PTR(pstStream);
    D_VENC_CHECK_PTR(pstBufOffSet);

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    spin_lock_irqsave(&g_SendFrame_Lock[u32VeChn], flags);

    pstEncChnPara = g_stVencChn[u32VeChn].hVEncHandle;
    if (!pstEncChnPara || (g_stVencChn[u32VeChn].hVEncHandle == NULL))
    {
        spin_unlock_irqrestore(&g_SendFrame_Lock[u32VeChn], flags);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    else
    {
        pstEncChnPara->stStat.GetStreamNumTry++;
    }
    spin_unlock_irqrestore(&g_SendFrame_Lock[u32VeChn], flags);

    if (VENC_DRV_EflGetStreamLen(hVencChn) <= 0)
    {
        if (u32TimeoutMs == 0)
        {
            return HI_ERR_VENC_BUF_EMPTY;
        }
        else
        {
            s32Ret = VENC_DRV_OsalWaitEvent(&g_VencWait_Stream[u32VeChn], u32TimeoutMs);
            if (HI_FAILURE == s32Ret)
            {
                return HI_ERR_VENC_BUF_EMPTY;
            }

            s32Ret = VENC_DRV_EflGetStreamLen(hVencChn);
            if (s32Ret <= 0)
            {
                return HI_ERR_VENC_BUF_EMPTY;
            }
        }
    }

    if (g_stVencChn[u32VeChn].hJPGE != HI_INVALID_HANDLE) /*JPEG*/
    {
        if (pstEncChnPara->stStat.GetStreamNumOK != pstEncChnPara->stStat.PutStreamNumOK)
        {
            return HI_ERR_VENC_CHN_INVALID_STAT;
        }
    }

    s32Ret = VENC_DRV_EflGetBitStream(hVencChn, &stVeduPacket);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_ERR_VENC_BUF_EMPTY;
    }

    if (g_stVencChn[u32VeChn].hJPGE != HI_INVALID_HANDLE) /*JPEG*/
    {
        s32Ret = VENC_DRV_GetJpgeStream(&stVeduPacket, pstStream, hVencChn, pstEncChnPara->JpgMMZBuf.pu8StartVirAddr, pstBufOffSet);
        if (s32Ret != HI_SUCCESS) 
        { 
            return s32Ret;
        }
    }
    else
    {
        if (stVeduPacket.SlcLen[1] > 0)
        {
            memcpy(&g_stVencChn[u32VeChn].stChnPacket, &stVeduPacket, sizeof(stVeduPacket));
        }

        pstStream->VirAddr      = (HI_VIRT_ADDR_T)stVeduPacket.pVirtAddr[0];
        pstStream->u32SlcLen    = stVeduPacket.SlcLen[0] + stVeduPacket.SlcLen[1];
        pstStream->u32PtsMs     = stVeduPacket.PTS0;

        pstStream->bFrameEnd = (0 == stVeduPacket.bFrameEnd) ? HI_FALSE : HI_TRUE;
        pstStream->enDataType.enH264EType = stVeduPacket.NaluType;
        if (pstStream->u32SlcLen > 0)
        {
            pstBufOffSet->u32StrmBufOffset[0]
                = stVeduPacket.PhyAddr[0] - g_stVencChn[u32VeChn].StrmBufAddr;
        }
    }

    pstEncChnPara->stStat.GetStreamNumOK++;

    HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_VSYNC, "PTS=%u.\n", pstStream->u32PtsMs);

    s32Ret = VENC_DRV_SaveStream(pstStream, hVencChn, u32VeChn);

    return s32Ret;
}

HI_S32 VENC_DRV_ReleaseStream(VeduEfl_EncPara_S* hVencChn, VENC_INFO_ACQUIRE_STREAM_S* pstAcqStrm)
{
    HI_U32 u32VeChn = 0;
    HI_S32 s32Ret = 0;
    VeduEfl_NALU_S stVeduPacket;
    VeduEfl_EncPara_S  *pstEncChnPara;
    VENC_STREAM_S  pstStream = pstAcqStrm->stStream;
    //D_VENC_CHECK_PTR(pstStream);

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);
    memset(&stVeduPacket, 0, sizeof(VeduEfl_NALU_S));
    
    pstEncChnPara = (VeduEfl_EncPara_S*)hVencChn;

    if (g_stVencChn[u32VeChn].hJPGE != HI_INVALID_HANDLE)
    {
        pstEncChnPara->stStat.PutStreamNumTry++;

        if ((pstAcqStrm->Jpeg_Phy_Addr != pstEncChnPara->JpgMMZBuf.u32StartPhyAddr)
            ||(pstEncChnPara->stStat.GetStreamNumOK <= pstEncChnPara->stStat.PutStreamNumOK))
        {
            HI_ERR_VENC("Release stream failed, GetStreamNumOK=%d,PutStreamNumOK=%d,pstStream->VirAddr = %x,JpegOutBufPhyAddr= %x.\n",
                        pstEncChnPara->stStat.GetStreamNumOK, pstEncChnPara->stStat.PutStreamNumOK, pstAcqStrm->Jpeg_Phy_Addr,
                        pstEncChnPara->JpgMMZBuf.u32StartPhyAddr);
            return HI_ERR_VENC_CHN_RELEASE_ERR;
        }

        pstEncChnPara->stStat.PutStreamNumOK++;
        return HI_SUCCESS;
    }

    /* for H264 release stream */
    //H264_Vir_Addr = g_stVencChn[u32VeChn].StrmBufAddr + pstAcqStrm->H264_stream_off;                /*phyaddr[0]*/
    //pstStream.pu8Addr  = (HI_VIRT_ADDR_T)phys_to_virt((phys_addr_t)H264_Vir_Addr);
    pstStream.VirAddr = (HI_VIRT_ADDR_T)(pstEncChnPara->stCycBuf.pBase + pstAcqStrm->H264_stream_off);

    if ((HI_VIRT_ADDR_T)g_stVencChn[u32VeChn].stChnPacket.pVirtAddr[0] == pstStream.VirAddr)
    {
       memcpy(&stVeduPacket, &g_stVencChn[u32VeChn].stChnPacket, sizeof(stVeduPacket));
       memset(&g_stVencChn[u32VeChn].stChnPacket, 0, sizeof(VeduEfl_NALU_S));
    }
    else
    {
        stVeduPacket.pVirtAddr[0] = (HI_VOID*)pstStream.VirAddr;
        stVeduPacket.SlcLen[0]    = pstStream.u32SlcLen;
        stVeduPacket.pVirtAddr[1] = stVeduPacket.pVirtAddr[0]+ stVeduPacket.SlcLen[0];
        stVeduPacket.SlcLen[1]    = 0;
        stVeduPacket.PTS0         = pstStream.u32PtsMs;
        stVeduPacket.PTS1         = 0;
        stVeduPacket.bFrameEnd    = (HI_U32)pstStream.bFrameEnd;
        stVeduPacket.NaluType     = pstStream.enDataType.enH264EType;
    }

    pstEncChnPara->stStat.PutStreamNumTry++;
    s32Ret = VENC_DRV_EflSkpBitStream(hVencChn, &stVeduPacket);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("Release stream failed, ret= %#x.\n", s32Ret);
        return HI_ERR_VENC_CHN_RELEASE_ERR;
    }
    pstEncChnPara->stStat.PutStreamNumOK++;
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_ReleaseStreamIntar(VeduEfl_EncPara_S* hVencChn, VENC_STREAM_S* pstStream)
{
    HI_U32 u32VeChn = 0;
    HI_S32 s32Ret = 0;
    VeduEfl_NALU_S stVeduPacket;
    VeduEfl_EncPara_S*  pstEncChnPara;
    D_VENC_CHECK_PTR(pstStream);

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);

    memset(&stVeduPacket, 0, sizeof(stVeduPacket));

    pstEncChnPara = (VeduEfl_EncPara_S*)hVencChn;

    if (g_stVencChn[u32VeChn].hJPGE != HI_INVALID_HANDLE)
    {
        pstEncChnPara->stStat.PutStreamNumTry++;

        if ((pstStream->VirAddr != (HI_VIRT_ADDR_T)pstEncChnPara->JpgMMZBuf.pu8StartVirAddr)
            || (pstEncChnPara->stStat.GetStreamNumOK <= pstEncChnPara->stStat.PutStreamNumOK))
        {
            HI_ERR_VENC("Release stream failed, GetStreamNumOK=%d,PutStreamNumOK=%d,pstStream->pu8Addr = %x,JpegOutBufPhyAddr= %x.\n",
                        pstEncChnPara->stStat.GetStreamNumOK, pstEncChnPara->stStat.PutStreamNumOK, pstStream->VirAddr,
                        pstEncChnPara->JpgMMZBuf.u32StartPhyAddr);
            return HI_ERR_VENC_CHN_RELEASE_ERR;
        }
        pstEncChnPara->stStat.PutStreamNumOK++;
        return HI_SUCCESS;
    }

    /* for H264 release stream */

    if ((HI_VIRT_ADDR_T)g_stVencChn[u32VeChn].stChnPacket.pVirtAddr[0] == pstStream->VirAddr)
    {
        memcpy(&stVeduPacket, &g_stVencChn[u32VeChn].stChnPacket, sizeof(stVeduPacket));
        memset(&g_stVencChn[u32VeChn].stChnPacket, 0, sizeof(VeduEfl_NALU_S));
    }
    else
    {
        stVeduPacket.pVirtAddr[0] = (HI_VOID*)pstStream->VirAddr;
        stVeduPacket.SlcLen[0]    = pstStream->u32SlcLen;
        stVeduPacket.pVirtAddr[1] = stVeduPacket.pVirtAddr[0] + stVeduPacket.SlcLen[0];
        stVeduPacket.SlcLen[1]    = 0;
        stVeduPacket.PTS0         = pstStream->u32PtsMs;
        stVeduPacket.PTS1         = 0;
        stVeduPacket.bFrameEnd    = (HI_U32)pstStream->bFrameEnd;
        stVeduPacket.NaluType     = pstStream->enDataType.enH264EType;
    }

    pstEncChnPara->stStat.PutStreamNumTry++;
    s32Ret = VENC_DRV_EflSkpBitStream(hVencChn, &stVeduPacket);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("Release stream failed, ret= %#x.\n", s32Ret);
        return HI_ERR_VENC_CHN_RELEASE_ERR;
    }
    pstEncChnPara->stStat.PutStreamNumOK++;
    return HI_SUCCESS;
}


HI_S32 VENC_DRV_StartReceivePic(VeduEfl_EncPara_S* EncHandle)
{
    HI_S32 s32Ret   = HI_FAILURE;
    HI_U32 u32VeChn = 0;
    VeduEfl_EncPara_S*  pstEncChnPara;
    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);

    pstEncChnPara = (VeduEfl_EncPara_S*)EncHandle;
#if 0
    if (g_stVencChn[u32VeChn].hSource == HI_INVALID_HANDLE)
    {
        HI_ERR_VENC("source venc%d is NOT Attached.\n", u32VeChn);
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }
#endif
    if (HI_TRUE == g_stVencChn[u32VeChn].bEnable)
    {
        return HI_SUCCESS;
    }

    s32Ret = VENC_DRV_EflStartVenc(EncHandle);
    if (HI_SUCCESS == s32Ret)
    {
        g_stVencChn[u32VeChn].bEnable = HI_TRUE;

        if ( g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)  /*non attach mode*/
        {
            if (g_stVencChn[u32VeChn].bWithoutVPSS)
            {
                pstEncChnPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
                pstEncChnPara->stSrcInfo.pfGetImage_OMX = VENC_DRV_EflGetImage_OMX;

                pstEncChnPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;
                g_stVencChn[u32VeChn].stSrcInfo = pstEncChnPara->stSrcInfo;
                g_stVencChn[u32VeChn].bNeedVPSS = HI_FALSE;
            }
            else
            {
                pstEncChnPara->stSrcInfo.pfGetImage = pVpssFunc->pfnVpssGetPortFrame;//VENC_DRV_EflGetImage;
                pstEncChnPara->stSrcInfo.pfGetImage_OMX = HI_NULL;

                pstEncChnPara->stSrcInfo.pfPutImage = pVpssFunc->pfnVpssRelPortFrame;//VENC_DRV_EflPutImage;
                g_stVencChn[u32VeChn].stSrcInfo = pstEncChnPara->stSrcInfo;
                g_stVencChn[u32VeChn].bNeedVPSS = HI_TRUE;
            }
        }
        do_gettimeofday(&(g_stVencChn[u32VeChn].stTimeStart));
        HI_INFO_VENC("start Chn %d/%#x. OK\n", u32VeChn, EncHandle);
        VENC_DRV_EflRequestIframe(EncHandle);
    }
    else
    {
        s32Ret = HI_ERR_VENC_INVALID_CHNID;
    }
    return s32Ret;
}

HI_S32 VENC_DRV_StopReceivePic(VeduEfl_EncPara_S* EncHandle)
{
    HI_S32 s32Ret   = HI_FAILURE;
    HI_U32 u32VeChn = 0;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);

    if (HI_FALSE == g_stVencChn[u32VeChn].bEnable)
    {
        return HI_SUCCESS;
    }

    s32Ret = VENC_DRV_EflStopVenc(EncHandle);
    if (HI_SUCCESS == s32Ret)
    {
        g_stVencChn[u32VeChn].bEnable = HI_FALSE;
        //g_stVencChn[u32VeChn].bNeedVPSS = HI_FALSE;
        HI_INFO_VENC("stop Chn %d/%#x. OK\n", u32VeChn, EncHandle);

        if (!g_stVencChn[u32VeChn].bWithoutVPSS)
        {
            pVpssFunc->pfnVpssSendCommand(g_stVencChn[u32VeChn].hVPSS, HI_DRV_VPSS_USER_COMMAND_RESET, HI_NULL);
        }
    }

    return s32Ret;
}

static HI_VOID VENC_DRV_SetAttr_Error(HI_UNF_VENC_CHN_ATTR_S *pstChnUserCfg, HI_UNF_VENC_CHN_ATTR_S* pstAttr, HI_BOOL Enable)
{
    if (pstChnUserCfg->enCapLevel != pstAttr->enCapLevel)
    {
        HI_ERR_VENC("VENC not support this active change enCapLevel:(%d -> %d)!\n",
                    pstChnUserCfg->enCapLevel, pstAttr->enCapLevel);
    }

    if (pstChnUserCfg->bSlcSplitEn != pstAttr->bSlcSplitEn)
    {
        HI_ERR_VENC("VENC not support this active change bSlcSplitEn:(%d -> %d)!\n",
                    pstChnUserCfg->bSlcSplitEn, pstAttr->bSlcSplitEn);
    }

    if (pstChnUserCfg->u32RotationAngle != pstAttr->u32RotationAngle)
    {
        HI_ERR_VENC("VENC not support this active change u32RotationAngle:(%d -> %d)!\n",
                    pstChnUserCfg->u32RotationAngle, pstAttr->u32RotationAngle);
    }

    if (pstChnUserCfg->u32StrmBufSize != pstAttr->u32StrmBufSize)
    {
        HI_ERR_VENC("VENC not support this active change u32StrmBufSize:(%d -> %d)!\n",
                    pstChnUserCfg->u32StrmBufSize, pstAttr->u32StrmBufSize);
    }

    if (pstChnUserCfg->enVencType != pstAttr->enVencType)
    {
        HI_ERR_VENC("98cv200 VENC not support this active change encode type:(%d -> %d)!\n",
                    pstChnUserCfg->enVencType, pstAttr->enVencType);
    }

    if (Enable)
    {
        if ((pstChnUserCfg->enVencProfile != pstAttr->enVencProfile)
            && (pstChnUserCfg->enVencType == HI_UNF_VCODEC_TYPE_H264))
        {
            HI_ERR_VENC("VENC not support this active change enVencProfile:(%d -> %d)! enVencType(%d,%d)\n",
                        pstChnUserCfg->enVencProfile, pstAttr->enVencProfile, pstChnUserCfg->enVencType, HI_UNF_VCODEC_TYPE_H264);
        }
    }

    return;
}

#ifdef __VENC_SUPPORT_JPGE__
static HI_S32 VENC_DRV_SetAttr_Protocol(VENC_CHN_INFO_S* pstVeInfo, HI_UNF_VENC_CHN_ATTR_S* pstAttr, Jpge_EncCfg_S *pstJpgeEncCfg, 
                               VeduEfl_EncPara_S* EncHandle, OPTM_VENC_CHN_S* pstVenc)
{
    HI_S32 s32Ret = HI_FAILURE;

    D_VENC_CHECK_PTR(pstVeInfo);

    if (pstAttr->enVencType == HI_UNF_VCODEC_TYPE_H264)   /*jpeg --> h264*/
    {
        if (pstVenc->hJPGE == HI_INVALID_HANDLE)
        {
            return HI_ERR_VENC_CHN_INVALID_STAT;
        }

        if (pJpgeFunc != HI_NULL)
        {
            s32Ret = pJpgeFunc->pfnJpgeDestroyChn(pstVenc->hJPGE);

            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VENC("pfnJpgeDestroyChn failed!\n");

                return s32Ret;
            }
        }

        VENC_DRV_EflFreeBuf2Jpge(EncHandle);
        pstVeInfo->u32StrmBufPhyAddrJpeg = HI_NULL;
        pstVeInfo->u32BufSizeJpeg        = 0;
        pstVenc->hJPGE = HI_INVALID_HANDLE;
        EncHandle->Protocol = VEDU_H264;
    }
    else                                                  /*h264 --> jpeg*/
    {
        if (pstVenc->hJPGE != HI_INVALID_HANDLE)
        {
            return HI_ERR_VENC_CHN_INVALID_STAT;
        }

        if (pJpgeFunc == HI_NULL)
        {
            HI_ERR_VENC("pJpgeFunc == NULL\n");

            return HI_FAILURE;
        }

        pstJpgeEncCfg->FrameWidth  = pstAttr->u32Width;
        pstJpgeEncCfg->FrameHeight = pstAttr->u32Height;
        pstJpgeEncCfg->SlcSplitEn  = pstAttr->bSlcSplitEn;
        pstJpgeEncCfg->SplitSize   = pstVenc->u32SliceSize;
        pstJpgeEncCfg->Qlevel      = pstAttr->u32Qlevel;

        s32Ret = pJpgeFunc->pfnJpgeCreateChn(&(pstVenc->hJPGE), pstJpgeEncCfg);

        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VENC("pfnJpgeCreateChn failed!\n");

            return s32Ret;
        }

        s32Ret = VENC_DRV_EflAllocBuf2Jpge(EncHandle, (HI_U32)pstAttr->enCapLevel);

        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VENC("VENC_DRV_EflAllocBuf2Jpge Failed!\n");
            pJpgeFunc->pfnJpgeDestroyChn(pstVenc->hJPGE);

            return s32Ret;
        }

        EncHandle->Protocol          = VEDU_JPGE;
        pstVeInfo->u32StrmBufPhyAddrJpeg = EncHandle->JpgMMZBuf.u32StartPhyAddr;
        pstVeInfo->u32BufSizeJpeg        = EncHandle->JpgMMZBuf.u32Size;
    }

    EncHandle->RcStart = 1;

    return HI_SUCCESS;
}
#endif
static HI_S32 VENC_DRV_SetAttr_RCAndFrmRatePara(HI_UNF_VENC_CHN_ATTR_S* pstAttr, VeduEfl_RcAttr_S *pstRcAttrCfg, 
                                       VeduEfl_EncPara_S* EncHandle, OPTM_VENC_CHN_S* pstVenc)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL RcFlag  = HI_FALSE;

    if (pstAttr->enVencType == HI_UNF_VCODEC_TYPE_H264)
    {
        RcFlag |= (pstVenc->stChnUserCfg.u32TargetBitRate != pstAttr->u32TargetBitRate);
        RcFlag |= (pstVenc->stChnUserCfg.u32InputFrmRate  != pstAttr->u32InputFrmRate);
        RcFlag |= (pstVenc->stChnUserCfg.u32TargetFrmRate != pstAttr->u32TargetFrmRate);
        RcFlag |= (pstVenc->stChnUserCfg.u32Gop           != pstAttr->u32Gop);

        if (RcFlag == HI_TRUE)
        {
            pstRcAttrCfg->BitRate    = pstAttr->u32TargetBitRate;
            pstRcAttrCfg->InFrmRate  = pstAttr->u32InputFrmRate;
            pstRcAttrCfg->OutFrmRate = pstAttr->u32TargetFrmRate;
            pstRcAttrCfg->Gop        = pstAttr->u32Gop;

            s32Ret = VENC_DRV_EflRcSetAttr(pstVenc->hVEncHandle, pstRcAttrCfg);

            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_VENC("config venc Rate Control Attribute err:%#x.\n", s32Ret);

                return HI_FAILURE;
            }
        }
    }
    else  /* jpeg*/
    {
        if ((pstAttr->u32InputFrmRate > 60) || (pstAttr->u32InputFrmRate < pstAttr->u32TargetFrmRate))
        {
            return HI_ERR_VENC_INVALID_PARA;
        }

        /*initialize  parameter*/
        EncHandle->VoFrmRate = pstAttr->u32TargetFrmRate;
        EncHandle->ViFrmRate = pstAttr->u32InputFrmRate;
        //pstEncChnPara->TrCount   = pstEncChnPara->ViFrmRate;
        EncHandle->VBRflag   = 0;
        EncHandle->PicLevel  = 0;
    }
    return HI_SUCCESS;
}
static HI_S32 VENC_DRV_SetAttr_Resolution(VeduEfl_EncPara_S* EncHandle, OPTM_VENC_CHN_S* pstVenc, HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    HI_DRV_VPSS_PORT_CFG_S stPort0Cfg;

    if ((pstVenc->stChnUserCfg.u32Height  != pstAttr->u32Height)                //should be change
        || (pstVenc->stChnUserCfg.u32Width != pstAttr->u32Width))
    {
        if (pstVenc->enSrcModId != HI_ID_BUTT)     //attach mode
        {
            if ((EncHandle->stSrcInfo.pfChangeInfo)(EncHandle->stSrcInfo.handle,
                                                    pstAttr->u32Width,
                                                    pstAttr->u32Height,
                                                    VEDU_MAX_INPUT_FRAME_RATE) != HI_SUCCESS)
            {
                HI_ERR_VENC("Src Handle(%x) change Info failed!.\n", EncHandle->stSrcInfo.handle);

                return HI_FAILURE;
            }

            if (pstVenc->bEnable == HI_FALSE)   /*if not start encode release all frame*/
            {
                VENC_DRV_EflRlsAllFrame( EncHandle );
            }
        }

        if (!EncHandle->WithoutVPSS)
        {
            (pVpssFunc->pfnVpssGetPortCfg)(pstVenc->hPort[0], &stPort0Cfg);
            stPort0Cfg.s32OutputWidth    = pstAttr->u32Width;
            stPort0Cfg.s32OutputHeight   = pstAttr->u32Height;

            if ((pVpssFunc->pfnVpssSetPortCfg)(pstVenc->hPort[0], &stPort0Cfg) != HI_SUCCESS)
            {
                HI_ERR_VENC("VPSS Handle(%x) SetPortCfg failed!.\n", pstVenc->hPort[0]);

                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}
HI_S32 VENC_DRV_SetAttr(VeduEfl_EncPara_S* EncHandle, HI_UNF_VENC_CHN_ATTR_S* pstAttr, VENC_CHN_INFO_S* pstVeInfo)
{
    HI_S32 s32Ret   = -1;
    HI_U32 u32VeChn = 0;
    HI_BOOL ErrCfg  = HI_FALSE;

#ifdef __VENC_SUPPORT_JPGE__
    Jpge_EncCfg_S JpgeEncCfg ;
#endif
    OPTM_VENC_CHN_S* pstVenc;
    VeduEfl_RcAttr_S RcAttrCfg;
    VeduEfl_EncPara_S* pstEncChnPara = (VeduEfl_EncPara_S*)EncHandle;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);
    D_VENC_CHECK_PTR(pstAttr);
    pstVenc = &g_stVencChn[u32VeChn];

    /* check the new attr whether legal */
    ErrCfg |= (pstVenc->stChnUserCfg.enCapLevel != pstAttr->enCapLevel);
    ErrCfg |= (pstVenc->stChnUserCfg.bSlcSplitEn != pstAttr->bSlcSplitEn);
    ErrCfg |= (pstVenc->stChnUserCfg.u32RotationAngle != pstAttr->u32RotationAngle);
    ErrCfg |= (pstVenc->stChnUserCfg.u32StrmBufSize != pstAttr->u32StrmBufSize);
    ErrCfg |= (pstVenc->stChnUserCfg.enVencType != pstAttr->enVencType);
    if (pstVenc->bEnable)
    {
        ErrCfg |= (pstVenc->stChnUserCfg.enVencType != pstAttr->enVencType);

        if ( pstVenc->stChnUserCfg.enVencType == HI_UNF_VCODEC_TYPE_H264 )
        {
            ErrCfg |= (pstVenc->stChnUserCfg.enVencProfile != pstAttr->enVencProfile);
        }
    }
#ifndef __VENC_SUPPORT_JPGE__
    else
    {
        ErrCfg |= (pstVenc->stChnUserCfg.enVencType != pstAttr->enVencType);   //not support change with JPEG<->h264
    }
#endif
    if (HI_TRUE == ErrCfg)
    {
        VENC_DRV_SetAttr_Error(&pstVenc->stChnUserCfg, pstAttr, pstVenc->bEnable);

        return HI_ERR_VENC_NOT_SUPPORT;
    }

#ifdef __VENC_3719MV100_CONFIG__
    if (HI_UNF_VCODEC_TYPE_JPEG == pstAttr->enVencType)
    {
        HI_ERR_VENC("3719MV100 not support creat JPEG channel!\n");
        return HI_ERR_VENC_NOT_SUPPORT;
    }
#endif

    /* this case to change the resolution */
    s32Ret = VENC_DRV_SetAttr_Resolution(EncHandle, pstVenc, pstAttr);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

#ifdef __VENC_SUPPORT_JPGE__
    /* this case to change the protocol of Encoder */
    if (pstVenc->stChnUserCfg.enVencType != pstAttr->enVencType)
    {
        s32Ret = VENC_DRV_SetAttr_Protocol(pstVeInfo, pstAttr, &JpgeEncCfg, EncHandle, pstVenc);

        if (s32Ret != HI_SUCCESS)
        {
            return s32Ret;
        }
    }
#endif

    /* this case to change the profile of H264 Encoder  */
    if ((pstAttr->enVencType == HI_UNF_VCODEC_TYPE_H264) && (pstVenc->stChnUserCfg.enVencProfile != pstAttr->enVencProfile))
    {

        pstEncChnPara->H264HpEn    = (HI_U32)pstAttr->enVencProfile;
        pstEncChnPara->H264CabacEn = (pstEncChnPara->H264HpEn == VEDU_H264_BASELINE_PROFILE) ? 0 : 1;

        /* make sps & pps & VOL stream */
        VENC_DRV_EflFlushStrmHeader(pstEncChnPara);
        pstEncChnPara->RcStart = 1;
    }

    /* this case to change the RC & FrmRate Control parameter */

    s32Ret = VENC_DRV_SetAttr_RCAndFrmRatePara(pstAttr, &RcAttrCfg, EncHandle, pstVenc);
    if (s32Ret != HI_SUCCESS)
    {
        /*initialize  parameter*/


    /*this case to change the RotationAngle*/
        return s32Ret;
    }

    /* this case to change the Priority */
    if (pstVenc->stChnUserCfg.u8Priority != pstAttr->u8Priority)
    {
        HI_U32 PriorityID;
        D_VENC_GET_PRIORITY_ID(u32VeChn, PriorityID);
        D_VENC_CHECK_CHN(PriorityID);
        PriorityTab[1][PriorityID] = pstAttr->u8Priority;
        VENC_DRV_EflSortPriority();
    }

    /* update the new user attr */
    if (pstAttr->enVencType == HI_UNF_VCODEC_TYPE_H264)
    {
        pstVenc->stChnUserCfg.u32Gop           = pstAttr->u32Gop;
        pstVenc->stChnUserCfg.u8Priority       = pstAttr->u8Priority;
        pstVenc->stChnUserCfg.u32TargetBitRate = pstAttr->u32TargetBitRate;
        pstVenc->stChnUserCfg.u32InputFrmRate  = pstAttr->u32InputFrmRate;
        pstVenc->stChnUserCfg.u32TargetFrmRate = pstAttr->u32TargetFrmRate;
        pstVenc->stChnUserCfg.u32MaxQp         = pstAttr->u32MaxQp;
        pstVenc->stChnUserCfg.u32MinQp         = pstAttr->u32MinQp;
        pstVenc->stChnUserCfg.bQuickEncode     = pstAttr->bQuickEncode;
        pstVenc->stChnUserCfg.u32DriftRateThr  = pstAttr->u32DriftRateThr;
        pstVenc->stChnUserCfg.enVencProfile    = pstAttr->enVencProfile;
        pstVenc->stChnUserCfg.enVencType       = pstAttr->enVencType;
        pstVenc->stChnUserCfg.u32RotationAngle = pstAttr->u32RotationAngle;
        pstVenc->stChnUserCfg.u32Height        = pstAttr->u32Height;
        pstVenc->stChnUserCfg.u32Width         = pstAttr->u32Width;

        pstEncChnPara->MinQp                   = pstAttr->u32MinQp;
        pstEncChnPara->MaxQp                   = pstAttr->u32MaxQp;
        pstEncChnPara->AutoSkipFrameEn         = (pstAttr->u32DriftRateThr<=100)?1:0;
        pstEncChnPara->DriftRcSkipThr          = pstAttr->u32DriftRateThr;
    }
    else   /*JPEG*/
    {
        pstVenc->stChnUserCfg.u8Priority       = pstAttr->u8Priority;
        pstVenc->stChnUserCfg.u32InputFrmRate  = pstAttr->u32InputFrmRate;
        pstVenc->stChnUserCfg.u32TargetFrmRate = pstAttr->u32TargetFrmRate;
        pstVenc->stChnUserCfg.bQuickEncode     = pstAttr->bQuickEncode;
        pstVenc->stChnUserCfg.enVencType       = pstAttr->enVencType;
        pstVenc->stChnUserCfg.u32RotationAngle = pstAttr->u32RotationAngle;
        pstVenc->stChnUserCfg.u32Height        = pstAttr->u32Height;
        pstVenc->stChnUserCfg.u32Width         = pstAttr->u32Width;
        pstVenc->stChnUserCfg.u32Qlevel        = pstAttr->u32Qlevel;

        pstEncChnPara->QLevel                  = pstAttr->u32Qlevel;
    }
    pstEncChnPara->QuickEncode                 = pstAttr->bQuickEncode;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_GetAttr(VeduEfl_EncPara_S* EncHandle, HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    HI_U32 u32VeChn = 0;
    OPTM_VENC_CHN_S* pstVenc;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);
    D_VENC_CHECK_PTR(pstAttr);
    pstVenc = &g_stVencChn[u32VeChn];

    *pstAttr = pstVenc->stChnUserCfg;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_RequestIFrame(VeduEfl_EncPara_S* EncHandle)
{
    HI_S32 u32VeChn = 0;
    OPTM_VENC_CHN_S* pstVenc;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);
    pstVenc = &g_stVencChn[u32VeChn];

    VENC_DRV_EflRequestIframe(pstVenc->hVEncHandle);

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_QueueFrame(VeduEfl_EncPara_S* hVencChn, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo )
{
    HI_S32 s32Ret;
    HI_U32 u32VeChn = 0;
    HI_DRV_VIDEO_FRAME_S stFrame;
    VeduEfl_EncPara_S* pstEncChnPara;

    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);
    D_VENC_CHECK_PTR(pstFrameInfo);

    memset(&stFrame, 0, sizeof(HI_DRV_VIDEO_FRAME_S));
    pstEncChnPara = hVencChn;
    if (g_stVencChn[u32VeChn].bEnable != HI_TRUE)
    {
        HI_ERR_VENC("Please start the VENC before QueueFrame!\n");
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if (HI_TRUE == pstEncChnPara->bNeverEnc)
    {
        if (HI_ID_BUTT == g_stVencChn[u32VeChn].enSrcModId)   //如果未绑定则设置
        {
            pstEncChnPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
            pstEncChnPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;
        }
        else
        {
            HI_ERR_VENC("the venc had already attach another sourse!QueueFrame is  invalid!! \n");
            return HI_ERR_VENC_CHN_INVALID_STAT;
        }
    }
    s32Ret = Convert_FrameStructure(pstFrameInfo, &stFrame);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("Convert_FrameStructure error! \n");
		
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if (HI_TRUE == pstEncChnPara->bNeverEnc)
    {
        if (!pstEncChnPara->WithoutVPSS)
        {
            VENC_DRV_EflJudgeVPSS(g_stVencChn[u32VeChn].hVEncHandle , &stFrame);
        }
        else
        {
            pstEncChnPara->bNeverEnc = HI_FALSE;
            g_stVencChn[u32VeChn].bNeedVPSS = HI_FALSE;
        }
    }

    if (HI_TRUE == g_stVencChn[u32VeChn].bNeedVPSS)
    {
        if (g_stVencChn[u32VeChn].stProcWrite.bSaveYUVFileRun)     //save the yuv before send to vpss
        {
            VENC_DRV_DbgWriteYUV(&stFrame, g_stVencChn[u32VeChn].stProcWrite.YUVFileName);
        }

        stFrame.u32TunnelPhyAddr = 0;
        if (HI_SUCCESS != Judge_FrameInfo_forVPSS(&stFrame))
        {
            return HI_FAILURE;
        }

        s32Ret = (pVpssFunc->pfnVpssPutImage)(g_stVencChn[u32VeChn].hVPSS, &stFrame);
        if (!s32Ret)
        {
            pstEncChnPara->stStat.VpssQueueBufNum++;
            pVpssFunc->pfnVpssSendCommand(g_stVencChn[u32VeChn].hVPSS, HI_DRV_VPSS_USER_COMMAND_IMAGEREADY, HI_NULL);
        }
    }
    else
    {
        if (g_stVencChn[u32VeChn].stProcWrite.bSaveYUVFileRun)     //save the yuv before send to vpss
        {
            VENC_DRV_DbgWriteYUV(&stFrame, g_stVencChn[u32VeChn].stProcWrite.YUVFileName);
        }

        stFrame.u32TunnelPhyAddr = 0;
        s32Ret = VENC_DRV_EflQueueFrame(hVencChn, &stFrame);
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_DequeueFrame(VeduEfl_EncPara_S* hVencChn, HI_UNF_VIDEO_FRAME_INFO_S* pstFrameInfo )
{
    HI_S32 s32Ret;
    HI_S32 u32VeChn = 0;
    HI_DRV_VIDEO_FRAME_S stFrame;
    VeduEfl_EncPara_S* pstEncChnPara;
    D_VENC_GET_CHN(u32VeChn, hVencChn);
    D_VENC_CHECK_CHN(u32VeChn);
    D_VENC_CHECK_PTR(pstFrameInfo);

    memset(&stFrame, 0 ,sizeof(HI_DRV_VIDEO_FRAME_S));
    pstEncChnPara = hVencChn;
    if (g_stVencChn[u32VeChn].enSrcModId < HI_ID_BUTT)
    {
        HI_ERR_VENC("the venc had already attach another sourse!DequeueFrame is  invalid!! \n");
        return HI_ERR_VENC_CHN_INVALID_STAT;
    }

    if (HI_TRUE == g_stVencChn[u32VeChn].bNeedVPSS)
    {
        s32Ret = (pVpssFunc->pfnVpssGetImage)(g_stVencChn[u32VeChn].hVPSS, &stFrame);
        if (!s32Ret)
        {
            pstEncChnPara->stStat.VpssQueueBufNum--;
        }
    }
    else
    {
        s32Ret = VENC_DRV_EflDequeueFrame(hVencChn, &stFrame);
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    s32Ret = Convert_DrvFrameStructure(&stFrame, pstFrameInfo);
    return s32Ret;
}

////////////////////////////////////////////////////////////////
/*new function interface  for openTV 5*/
////////////////////////////////////////////////////////////////
HI_DECLARE_MUTEX(g_VencDrvMutex);
extern atomic_t g_VencCount;
extern struct timer_list vencTimer;
extern HI_U32 g_u32VencOpenFlag;
extern HI_VOID VENC_TimerFunc(HI_LENGTH_T value);
extern HI_S32 VENC_VpssEventHandle(HI_HANDLE hVenc, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID* pstArgs);





HI_S32 VENC_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = HI_DRV_MODULE_Register(HI_ID_VENC, "HI_VENC", (HI_VOID*)&s_VencExportFuncs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VENC("HI_DRV_MODULE_Register failed, mode ID = 0x%08X\n", HI_ID_VENC);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID VENC_DRV_Exit(HI_VOID)
{
    HI_DRV_MODULE_UnRegister(HI_ID_VENC);
    return;
}

HI_S32 HI_DRV_VENC_GetUserHndByKerHnd(VeduEfl_EncPara_S* hVencHandle, HI_HANDLE* hVencUsrHandle)
{
    HI_S32 i = 0;

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (g_stVencChn[i].hVEncHandle == hVencHandle)
        { break; }
    }

    if (i >= VENC_MAX_CHN_NUM)
    {
        HI_ERR_VENC("unknown venchandle!\n");
        return HI_FAILURE;
    }

    *hVencUsrHandle = GET_VENC_CHHANDLE(i);
    return HI_SUCCESS;
}


HI_S32 HI_DRV_VENC_Init(HI_VOID)
{
    HI_S32 s32Ret = 0;

#if 0
    /*Init the VENC device*/
    s32Ret = VENC_DRV_Init();
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("Init VENC drv fail!\n");
        return HI_FAILURE;
    }
#endif

    /*just init the wait_event*/
    //VENC_DRV_BoardInit();  ==> after create the channel

    VENC_DRV_InitEvent();

    /*creat thread to manage channel*/
    s32Ret = VENC_DRV_EflOpenVedu();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_VENC("VeduEfl_OpenVedu failed, ret=%d\n", s32Ret);
        return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 HI_DRV_VENC_DeInit(HI_VOID)
{

    HI_S32 s32Ret = 0;
    HI_U32 i = 0;

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (g_stVencChn[i].hVEncHandle == NULL)
        {
            break;
        }
    }
    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if (/*(g_stVencChn[i].pWhichFile == ffile)&& */(g_stVencChn[i].hVEncHandle != NULL))
        {
            HI_INFO_VENC("Try VENC_DestroyChn %d/%#p.\n", i, g_stVencChn[i].hVEncHandle);
            s32Ret = VENC_DRV_DestroyChn(g_stVencChn[i].hVEncHandle);
            if (HI_SUCCESS != s32Ret)
            {
                HI_WARN_VENC("force DestroyChn %d failed, Ret=%#x.\n", i, s32Ret);
            }
            g_stVencChn[i].pWhichFile = HI_NULL;
        }
    }

    VENC_DRV_EflCloseVedu();

    /*close the venc lock*/
    VENC_DRV_BoardDeinit();

#if 0
    VENC_DRV_Exit();
#endif

    return HI_SUCCESS;
}

HI_S32 HI_DRV_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S* pstAttr)
{
    if (NULL == pstAttr)
    {
        return HI_ERR_VENC_NULL_PTR;
    }

    pstAttr->enVencType       = HI_UNF_VCODEC_TYPE_H264;
    pstAttr->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_720P;
    pstAttr->enVencProfile    = HI_UNF_H264_PROFILE_HIGH;
    pstAttr->u32RotationAngle = 0;
    pstAttr->bSlcSplitEn      = HI_FALSE;
    pstAttr->u32StrmBufSize   = 1280 * 720 * 2;
    pstAttr->u32Width         = 1280;
    pstAttr->u32Height        = 720;

    pstAttr->u32TargetBitRate = 4 * 1024 * 1024;
    pstAttr->u32InputFrmRate  = 25;
    pstAttr->u32TargetFrmRate = 25;
    pstAttr->u32Gop           = 100;
    pstAttr->u32MaxQp = 48;
    pstAttr->u32MinQp = 16;
    pstAttr->bQuickEncode     = HI_FALSE;
    pstAttr->u32DriftRateThr  = (-1);
    pstAttr->u8Priority       = 0;
    pstAttr->u32Qlevel        = 0;
    return HI_SUCCESS;
}

HI_S32 HI_DRV_VENC_Create(HI_HANDLE* phVencChn, HI_UNF_VENC_CHN_ATTR_S* pstAttr, HI_BOOL bOMXChn, struct file*  pfile)
{
    VENC_CHN_INFO_S stVeInfo;   //no use output in the mode
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32Index = 0;
    VeduEfl_EncPara_S* hKernVencChn = NULL;
    venc_chan_cfg stVencCfg ;

    stVencCfg.bAutoRequestIFrm = HI_FALSE;
    stVencCfg.bPrependSpsPps   = HI_TRUE;
    stVencCfg.bOmxChn          = HI_FALSE;
    stVencCfg.ControlRateType  = VENC_DRV_ControlRateConstant;
    stVencCfg.h264Level        = 0;   //no use
    memcpy(&stVencCfg.VencUnfAttr, pstAttr, sizeof(HI_UNF_VENC_CHN_ATTR_S));

    Ret = down_interruptible(&g_VencDrvMutex);
    if (Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    Ret = VENC_DRV_CreateChn(&hKernVencChn, &stVencCfg, &stVeInfo, bOMXChn, pfile);
    D_VENC_GET_CHN(u32Index, hKernVencChn);
    if (u32Index >= VENC_MAX_CHN_NUM)
    {
        up(&g_VencDrvMutex);
        return HI_ERR_VENC_CREATE_ERR;
    }

    g_stVencChn[u32Index].hUsrHandle = GET_VENC_CHHANDLE(u32Index);
    //*phVencChn = g_stVencChn[u32Index].hUsrHandle;
    *phVencChn = g_stVencChn[u32Index].hUsrHandle;

    if (!g_stVencChn[u32Index].bWithoutVPSS)
    {
        /*vpss*/
        VPSS_HANDLE hVPSS;
        VPSS_HANDLE hPort0;
        HI_DRV_VPSS_CFG_S stVPSSCfg;
        HI_DRV_VPSS_PORT_CFG_S stPort0Cfg;

        /*create VPSS instance*/
        Ret |= (pVpssFunc->pfnVpssGetDefaultCfg)(&stVPSSCfg);
        Ret |= (pVpssFunc->pfnVpssCreateVpss)(&stVPSSCfg, &hVPSS);
        Ret |= (pVpssFunc->pfnVpssSendCommand)(hVPSS, HI_DRV_VPSS_USER_COMMAND_START, NULL);

        /*create VPSS Port*/
        Ret |= (pVpssFunc->pfnVpssGetDefaultPortCfg)(&stPort0Cfg);
        stPort0Cfg.s32OutputWidth            = pstAttr->u32Width;
        stPort0Cfg.s32OutputHeight           = pstAttr->u32Height;
        stPort0Cfg.u32MaxFrameRate           = pstAttr->u32TargetFrmRate;//pstCreateInfo->stAttr.u32InputFrmRate;
        stPort0Cfg.eDstCS                    = HI_DRV_CS_BT709_YUV_LIMITED;
        stPort0Cfg.stBufListCfg.eBufType     = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
        stPort0Cfg.stBufListCfg.u32BufNumber = 6;
        stPort0Cfg.stBufListCfg.u32BufSize   = 2 * (pstAttr->u32Width * pstAttr->u32Height);
        stPort0Cfg.stBufListCfg.u32BufStride = D_VENC_ALIGN_UP(pstAttr->u32Width, 16);
        //stPort0Cfg.eAspMode                = HI_DRV_ASP_RAT_MODE_FULL;                      /*不同比例缩放策略*/
        stPort0Cfg.eFormat                   = HI_DRV_PIX_FMT_NV21;                           /* 21  Y/CrCb 4:2:0  */
        Ret |= (pVpssFunc->pfnVpssCreatePort)(hVPSS, &stPort0Cfg, &hPort0);
        if (HI_SUCCESS == Ret)
        {
            g_stVencChn[u32Index].hVPSS       = hVPSS;
            g_stVencChn[u32Index].hPort[0]    = hPort0;
        }
        /*注册vpss回调函数*/
        Ret |= (pVpssFunc->pfnVpssRegistHook)(hVPSS, g_stVencChn[u32Index].hUsrHandle, VENC_VpssEventHandle);
    }

    up(&g_VencDrvMutex);

#ifdef __VENC_RC_DBG__

    g_stVencChn[u32Index].stProcWrite.fpSaveFile = VENC_DRV_OsalFopen("/mnt/venc_rc_dbg.h264",  O_RDWR | O_CREAT | O_APPEND, 0);
    if (HI_NULL == g_stVencChn[u32Index].stProcWrite.fpSaveFile)
    {
        HI_ERR_VENC("Can not create file.\n");
        g_stVencChn[u32Index].stProcWrite.bTimeModeRun = HI_FALSE;
        return HI_FAILURE;
    }
    g_stVencChn[u32Index].stProcWrite.bTimeModeRun = HI_TRUE;
#endif
    return Ret;
}

HI_S32 HI_DRV_VENC_Destroy(HI_HANDLE hVenc)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 u32Index = 0;

    // VENC
    Ret = down_interruptible(&g_VencDrvMutex);
    if (Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }
    
    D_VENC_GET_CHN_BY_UHND(u32Index, hVenc);
    if (u32Index >= VENC_MAX_CHN_NUM)
    {
        up(&g_VencDrvMutex);
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }


#ifdef __VENC_RC_DBG__
    g_stVencChn[u32Index].stProcWrite.bTimeModeRun = HI_FALSE;
    VENC_DRV_OsalFclose(g_stVencChn[u32Index].stProcWrite.fpSaveFile);

#endif

    if (g_stVencChn[u32Index].bWithoutVPSS)
    {
        //VPSS
        Ret |= (pVpssFunc->pfnVpssDestroyPort)(g_stVencChn[u32Index].hPort[0]);
        Ret |= (pVpssFunc->pfnVpssSendCommand)(g_stVencChn[u32Index].hVPSS, HI_DRV_VPSS_USER_COMMAND_STOP, NULL);
        Ret |= (pVpssFunc->pfnVpssDestroyVpss)(g_stVencChn[u32Index].hVPSS);
    }

    Ret  |= VENC_DRV_DestroyChn(g_stVencChn[u32Index].hVEncHandle);

    up(&g_VencDrvMutex);

    return Ret;
}

HI_S32 HI_DRV_VENC_AttachInput(HI_HANDLE hVenc, HI_HANDLE hSrc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    s32Ret = VENC_DRV_AttachInput(hVencChn, hSrc, ((hSrc & 0xff0000) >> 16));
    up(&g_VencDrvMutex);
    return s32Ret;

}

HI_S32 HI_DRV_VENC_DetachInput(HI_HANDLE hVenc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hSrc = 0;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;

    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    hSrc = hVencChn->stSrcInfo.handle;
    s32Ret =  VENC_DRV_DetachInput(hVencChn,hSrc, ((hSrc & 0xff0000) >> 16));
    up(&g_VencDrvMutex);

    return s32Ret;
}

HI_S32 HI_DRV_VENC_Start(HI_HANDLE hVenc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_StartReceivePic(hVencChn);
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_Stop(HI_HANDLE hVenc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_StopReceivePic(hVencChn);
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_AcquireStream(HI_HANDLE hVenc,VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs)
{
   VENC_BUF_OFFSET_S stBufOffSet;    //no use output in the mode
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    return VENC_DRV_AcquireStream(hVencChn, pstStream, u32TimeoutMs, &stBufOffSet);
}

HI_S32 HI_DRV_VENC_ReleaseStream(HI_HANDLE hVenc, VENC_STREAM_S *pstStream)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_ReleaseStreamIntar(hVencChn, pstStream);
    up(&g_VencDrvMutex);
    return s32Ret;
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_S32 HI_DRV_VENC_SetSrcInfo(HI_HANDLE hVenc, HI_DRV_VENC_SRC_INFO_S *pstSrcInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    //s32Ret = VENC_DRV_SetSrcInfo(hVencChn, pstSrcInfo);
    up(&g_VencDrvMutex);
    return s32Ret;
 }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

HI_S32 HI_DRV_VENC_SetAttr(HI_HANDLE hVenc,HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_SetAttr(hVencChn, pstAttr, HI_NULL);
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_GetAttr(HI_HANDLE hVenc, HI_UNF_VENC_CHN_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_GetAttr(hVencChn, pstAttr);
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_RequestIFrame(HI_HANDLE hVenc)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_RequestIFrame(hVencChn);
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_QueueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_QueueFrame(hVencChn, pstFrameinfo );
    up(&g_VencDrvMutex);
    return s32Ret;
}

HI_S32 HI_DRV_VENC_DequeueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ChnID;
    VeduEfl_EncPara_S* hVencChn;
    s32Ret = down_interruptible(&g_VencDrvMutex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    D_VENC_GET_CHN_BY_UHND(u32ChnID, hVenc);
    D_VENC_CHECK_CHN(u32ChnID);
    hVencChn = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_DequeueFrame(hVencChn, pstFrameinfo);
    up(&g_VencDrvMutex);
    return s32Ret;
}


HI_S32 HI_DRV_VENC_Open_By_OPENTV(HI_VOID)
{
    HI_S32 Ret, i;
    //HI_CHAR ProcName[12];
    //DRV_PROC_ITEM_S  *pProcItem;

    Ret = down_interruptible(&g_VencDrvMutex);
    if (Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    if (1 == atomic_inc_return(&g_VencCount))
    {
        //VENC_DRV_BoardInit();  ==> after create the channel

        VENC_DRV_InitEvent();

        Ret = VENC_DRV_EflOpenVedu();
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_VENC("VeduEfl_OpenVedu failed, ret=%d\n", Ret);
            atomic_dec(&g_VencCount);
            up(&g_VencDrvMutex);
            return HI_FAILURE;
        }


        for (i = 0; i < VENC_MAX_CHN_NUM; i++)
        {
            memset(&(g_stVencChn[i]), 0, sizeof(OPTM_VENC_CHN_S));
            g_stVencChn[i].hVEncHandle = NULL;
        }

        init_timer(&vencTimer);
        vencTimer.expires  = jiffies + (HZ);
        vencTimer.function = VENC_TimerFunc;
        add_timer(&vencTimer);
    }

    g_u32VencOpenFlag = 1;
    up(&g_VencDrvMutex);

    Ret = HI_DRV_MODULE_GetFunction(HI_ID_VPSS, (HI_VOID**)&pVpssFunc);
    if ((HI_NULL == pVpssFunc) || (HI_NULL == pVpssFunc->pfnVpssGlobalInit) || Ret)
    {
        HI_ERR_VENC("GetFunction from VPSS  failed.\n");
        return HI_FAILURE;
    }
    Ret = pVpssFunc->pfnVpssGlobalInit();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_VENC("VPSS_GlobalInit failed, ret=%d\n", Ret);
        return HI_FAILURE;
    }

#ifdef __VENC_SUPPORT_JPGE__
    Ret = HI_DRV_MODULE_GetFunction(HI_ID_JPGENC, (HI_VOID**)&pJpgeFunc);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("Get jpge function failed!\n");
        pJpgeFunc = HI_NULL;
    }
#endif


    return HI_SUCCESS;
}


HI_S32 HI_DRV_VENC_Close_By_OPENTV(struct file*  ffile)
{
    HI_U32 i = 0;
    HI_S32 Ret = 0;

    // HI_CHAR ProcName[12];

    Ret = down_interruptible(&g_VencDrvMutex);
    if (Ret != HI_SUCCESS)
    {
        HI_WARN_VENC("call down_interruptible err!\n");
    }

    del_timer_sync(&vencTimer);

    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        if ((g_stVencChn[i].pWhichFile == ffile)
            && (g_stVencChn[i].hVEncHandle != NULL))
        {
            if (g_stVencChn[i].bWithoutVPSS)
            {
                Ret |= (pVpssFunc->pfnVpssDestroyPort)(g_stVencChn[i].hPort[0]);
                Ret |= (pVpssFunc->pfnVpssSendCommand)(g_stVencChn[i].hVPSS, HI_DRV_VPSS_USER_COMMAND_STOP, NULL);
                Ret |= (pVpssFunc->pfnVpssDestroyVpss)(g_stVencChn[i].hVPSS);
            }

            HI_INFO_VENC("Try VENC_DestroyChn %d/%#x.\n", i, g_stVencChn[i].hVEncHandle);
            Ret |= VENC_DRV_DestroyChn(g_stVencChn[i].hVEncHandle);
            if (HI_SUCCESS != Ret)
            {
                HI_WARN_VENC("force DestroyChn %d failed, Ret=%#x.\n", i, Ret);
            }

            g_stVencChn[i].pWhichFile = HI_NULL;
        }
    }

    Ret = pVpssFunc->pfnVpssGlobalDeInit();
    if (HI_SUCCESS != Ret)
    { HI_ERR_VENC("VPSS_GlobalInit failed, ret=%d\n", Ret); }

    if (atomic_dec_and_test(&g_VencCount))
    {
        VENC_DRV_EflCloseVedu();
        
        VENC_DRV_BoardDeinit();
    }

    g_u32VencOpenFlag = 0;
    up(&g_VencDrvMutex);


    return HI_SUCCESS;
}

EXPORT_SYMBOL(HI_DRV_VENC_Init);
EXPORT_SYMBOL(HI_DRV_VENC_DeInit);
EXPORT_SYMBOL(HI_DRV_VENC_GetDefaultAttr);
EXPORT_SYMBOL(HI_DRV_VENC_Create);
EXPORT_SYMBOL(HI_DRV_VENC_Destroy);
EXPORT_SYMBOL(HI_DRV_VENC_AttachInput);
EXPORT_SYMBOL(HI_DRV_VENC_DetachInput);
EXPORT_SYMBOL(HI_DRV_VENC_Start);
EXPORT_SYMBOL(HI_DRV_VENC_Stop);
EXPORT_SYMBOL(HI_DRV_VENC_AcquireStream);
EXPORT_SYMBOL(HI_DRV_VENC_ReleaseStream);
EXPORT_SYMBOL(HI_DRV_VENC_SetAttr);
EXPORT_SYMBOL(HI_DRV_VENC_GetAttr);
EXPORT_SYMBOL(HI_DRV_VENC_RequestIFrame);
EXPORT_SYMBOL(HI_DRV_VENC_QueueFrame);
EXPORT_SYMBOL(HI_DRV_VENC_DequeueFrame);
//EXPORT_SYMBOL(HI_DRV_VENC_SetSrcInfo);
EXPORT_SYMBOL(HI_DRV_VENC_GetUserHndByKerHnd);

EXPORT_SYMBOL(HI_DRV_VENC_Open_By_OPENTV);
EXPORT_SYMBOL(HI_DRV_VENC_Close_By_OPENTV);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

