
#ifndef __SYNTAX_DEC_HEADER__
#define __SYNTAX_DEC_HEADER__

//#include "basedef.h"
//#include "scd_drv.h"
#include "vfmw.h"
#include "syn_cmn.h"
#ifdef VFMW_MPEG2_SUPPORT
#include "mpeg2.h"
#endif
#ifdef VFMW_MPEG4_SUPPORT
#include "mpeg4.h"
#endif
#ifdef VFMW_H264_SUPPORT
#include "h264.h"
#endif
#ifdef VFMW_HEVC_SUPPORT
#include "hevc.h"
#endif
#ifdef VFMW_MVC_SUPPORT
#include "mvc.h"
#endif
#ifdef VFMW_AVS_SUPPORT
#include "avs.h"
#endif
#ifdef VFMW_VC1_SUPPORT
#include "vc1.h"
#endif
#ifdef VFMW_REAL8_SUPPORT
#include "real8.h"
#endif
#ifdef VFMW_REAL9_SUPPORT
#include "real9.h"
#endif
#ifdef VFMW_DIVX3_SUPPORT
#include "divx3.h"
#endif
#ifdef VFMW_VP6_SUPPORT
#include "vp6.h"
#endif
#ifdef VFMW_VP8_SUPPORT
#include "vp8.h"
#endif
#ifdef VFMW_H263_SUPPORT
#include "sdec_imedia.h"
#endif

#define SYN_OK        0
#define SYN_ERR      -1

#define LAST_ALREADY_OUT        1
#define LAST_OUTPUT_OK      0
#define LAST_OUTPUT_FAIL      -1

// BIT:  2,  1,  0
//       DC, DB, DR
#define __DNRENABLE__  0x03

#define HISI_STREAM_END_CODE1              0x48535049
#define HISI_STREAM_END_CODE2              0x43454E44


typedef struct
{
    SYNTAX_EXTRA_DATA_S *pstExtraData;
    IMAGE_VO_QUEUE     ImageQue;
    SINT32        	   ChanID;
} USER_CTX_S;

/* syntax context */
typedef struct hiSYNTAX_CTX_S
{
    VID_STD_E  eVidStd;
    union
    {
#ifdef VFMW_MPEG2_SUPPORT
        MP2_CTX_S   stMpeg2Ctx;
#endif
#ifdef VFMW_MPEG4_SUPPORT
        MP4_CTX_S   stMpeg4Ctx;
#endif
#ifdef VFMW_AVS_SUPPORT
        AVS_CTX_S   stAvsCtx;
#endif
#ifdef VFMW_H264_SUPPORT
        H264_CTX_S  stH264Ctx;
#endif
#ifdef VFMW_HEVC_SUPPORT
        HEVC_CTX_S  stHevcCtx;
#endif
#ifdef VFMW_MVC_SUPPORT
        MVC_CTX_S  stMvcCtx;
#endif
#ifdef VFMW_VC1_SUPPORT
        VC1_CTX_S   stVc1Ctx;
#endif
#ifdef VFMW_REAL8_SUPPORT
        RV8_CTX_S   stReal8Ctx;
#endif
#ifdef VFMW_REAL9_SUPPORT
        RV9_CTX_S   stReal9Ctx;
#endif
#ifdef VFMW_DIVX3_SUPPORT
        DIVX3_CTX_S  stDivx3Ctx;
#endif
#ifdef VFMW_VP6_SUPPORT
        VP6_CTX_S   stVp6Ctx;
#endif
#ifdef VFMW_VP8_SUPPORT
        VP8_CTX_S   stVp8Ctx;
#endif
#ifdef VFMW_H263_SUPPORT
        iMediaSDEC_CTX_S stiMediaSoftCtx;
#endif
        USER_CTX_S  stUserCtx;
    } unSyntax;                     /* 该通道对应的syntax上下文 */
} SYNTAX_CTX_S;

#define  MAX_USRDAT_BLK_NUM   48*4

typedef struct hiVDEC_USRDAT_POOL
{
    SINT32  s32AccessIdx;
    VDEC_USRDAT_S  stUsrDat[MAX_USRDAT_BLK_NUM];
} VDEC_USRDAT_POOL_S;
#define VDEC_USRDAT_POOL_SIZE  (sizeof(VDEC_USRDAT_POOL_S))

VOID DecodeStream(STREAM_SEG_PACKET_S *pSegMan, SINT32 VideoStandard, VOID *pstCtx);
SINT32 InitVoQueue(IMAGE_VO_QUEUE *pstVoQue);
VOID PrintVoQueue(IMAGE_VO_QUEUE *pstVoQue);
VOID ResetVoQueue(IMAGE_VO_QUEUE *pstVoQue);
SINT32 InsertImgToVoQueue(SINT32 ChanID, VID_STD_E eVidStd, VOID *pCtx, IMAGE_VO_QUEUE *pstVoQue, IMAGE *p_image);
VOID GetQueueImgNum(IMAGE_VO_QUEUE *pstVoQue, SINT32 *pReadImgNum, SINT32 *pNewImgNum);
SINT32 IsNewpicSeg(SINT32 *pNewPicDetector, VID_STD_E VidStd, STREAM_SEG_S *pStreamSeg);

SINT32 SYNTAX_ReceiveImage(SYNTAX_CTX_S *pSynCtx, IMAGE *p_image);
SINT32 SYNTAX_ReleaseImage(SYNTAX_CTX_S *pSynCtx, IMAGE *p_image);
SINT32 SYNTAX_ReturnImage(SYNTAX_CTX_S *pSynCtx);
SINT32 OpenUsdPool_WithMem(SINT32 ChanID, SINT8 *Viraddr);
VOID CloseUsdPool_WithMem(SINT32 ChanID);
SINT32 IsImageInVoQueue(IMAGE_VO_QUEUE *pstVoQue, IMAGE *p_image);
SINT32 IsVoQueueEmpty(IMAGE_VO_QUEUE *pstVoQue);
VOID SetAspectRatio(IMAGE *pImg, VDEC_DAR_E Code);
UINT32 GetVoLastImageID(IMAGE_VO_QUEUE *pstVoQue);
VOID ReleaseCurPic(VID_STD_E VidStd, VOID *pCtx);
#ifdef VFMW_HEVC_SUPPORT
SINT32 IsMain10Profile(VOID *pstCtx);
#endif
#endif
