#ifndef __DRV_VDEC_EXT_H__
#define __DRV_VDEC_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "drv_vdec_ioctl.h"
#include "hi_drv_vdec.h"
#include "hi_mpi_vdec.h"
#include "hi_drv_dev.h"

//#define PRE_ALLOC_VDEC_VDH_MMZ (1)
#define PRE_ALLOC_VDEC_SCD_MMZ (0)
#define PRE_ALLOC_VDEC_ESBUF_MMZ (0)

/**Compress Info*/
typedef struct hiVDEC_COMPRESS_INFO_S
{
    HI_U32 u32CompressFlag;
    HI_S32 s32CompFrameHeight;
    HI_S32 s32CompFrameWidth;
    HI_U32 u32HeadOffset;            /**<DNRInfo head offset */
    HI_U32 u32YHeadAddr;             /**<Y head info when compress is used */
    HI_U32 u32CHeadAddr;             /**<C head info when compress is used */
    HI_U32 u32HeadStride;            /**<YC head info stride when compress is used */
}HI_VDEC_COMPRESS_INFO_S;

/**VC1 Range Info*/
typedef struct hiVDEC_VC1_RANGE_INFO_S
{
    HI_U8 u8PicStructure;     /**< 0: frame, 1: top, 2: bottom, 3: mbaff, 4: field pair */
    HI_U8 u8PicQPEnable;
    HI_U8 u8ChromaFormatIdc;  /**< 0: yuv400, 1: yuv420 */
    HI_U8 u8VC1Profile;

    HI_S32 s32QPY;
    HI_S32 s32QPU;
    HI_S32 s32QPV;
    HI_S32 s32RangedFrm;

    HI_U8 u8RangeMapYFlag;
    HI_U8 u8RangeMapY;
    HI_U8 u8RangeMapUVFlag;
    HI_U8 u8RangeMapUV;
    HI_U8 u8BtmRangeMapYFlag;
    HI_U8 u8BtmRangeMapY;
    HI_U8 u8BtmRangeMapUVFlag;
    HI_U8 u8BtmRangeMapUV;
}HI_VDEC_VC1_RANGE_INFO_S;

/**BTL Info*/
typedef struct hiVDEC_BTL_INFO_S
{
    HI_U32 u32BTLImageID;
    HI_U32 u32Is1D;         /**< 0:2D, 1:1D */
    HI_U32 u32IsCompress;

    HI_U32 u32DNROpen;      /**< 0: DNR close, 1: DNR open */
    HI_U32 u32DNRInfoAddr;  /**< DNR info from BTL */
    HI_U32 u32DNRInfoStride;/**< DNR info stride from BTL */
}HI_VDEC_BTL_INFO_S;

typedef struct hiVDEC_PRIV_FRAMEINFO_S
{
    HI_U32                      u32BeVC1;
    HI_VDEC_COMPRESS_INFO_S     stCompressInfo;
    HI_VDEC_VC1_RANGE_INFO_S    stVC1RangeInfo;
    HI_VDEC_BTL_INFO_S          stBTLInfo;
    HI_UNF_VCODEC_TYPE_E        entype;
    HI_U32                      u32SeqFrameCnt;     /**<Picture ID in a video sequence. The ID of the first frame in each sequence is numbered 0*/ /**<CNcomment: 视频序列中的图像编号，每个序列中第一帧编号为0*/
    HI_U32                      u32DispTime;        /**<PVR Display time*/
    HI_U32                      image_id;
    HI_U32                      image_id_1;
    HI_S32                      s32InterPtsDelta;   /*interleaved source, VPSS module swtich field to frame, need to adjust pts*/
    HI_U8                       u8Repeat;           /**<Times of playing a video frame*/ /**<CNcomment: 视频帧播放次数.*/
    HI_U8                       u8EndFrame;         /**<0 Not end frame; 1 Current frame is the end frame; 2 Prior frame is the end frame */
    HI_U8                       u8TestFlag;         /**<VDEC_OPTMALG_INFO_S.Rwzb*/
    HI_U8                       u8Marker;           /**<Bit0: 1 Resolution change
                                                        Bit1: 1 close deinterlace
                                                    */
    HI_U32                      u32OriFrameRate;  /* 1000*rate */
    HI_S32                      s32GopNum;
    HI_S32                      s32FrameFormat;
    HI_S32                      s32TopFieldFrameFormat;
    HI_S32                      s32BottomFieldFrameFormat;
    HI_S32                      s32FieldFlag;
    HI_U32                      u32Usertag;
}HI_VDEC_PRIV_FRAMEINFO_S;

typedef HI_S32  (*FN_VDEC_Suspend)(PM_BASEDEV_S *, pm_message_t state);
typedef HI_S32  (*FN_VDEC_Resume)(PM_BASEDEV_S *);
typedef HI_S32  (*FN_VDEC_ReportEsRels)(HI_HANDLE hDmxVidChn);


typedef HI_VOID (*FN_VDEC_SetOmxCallBacks)(OMX_EXPORT_FUNC_S* OmxCallbacks);
typedef struct tagVDEC_EXPORT_FUNC_S
{
    FN_VDEC_Suspend             pfnVDEC_Suspend;
    FN_VDEC_Resume              pfnVDEC_Resume;
	PFN_VDEC_Chan_VOAcqFrame      pfnVOAcqFrame;
    PFN_VDEC_Chan_VORlsFrame      pfnVORlsFrame;
    PFN_VDEC_Chan_VOChangeWinInfo pfnVOSendWinInfo;
    FN_VDEC_SetOmxCallBacks      pfnVDEC_SetOmxCallBacks;
    FN_VDEC_ReportEsRels         pfnVDEC_ReportEsRels;
}VDEC_EXPORT_FUNC_S;


HI_S32 VDEC_DRV_ModInit(HI_VOID);
HI_VOID VDEC_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_VDEC_EXT_H__ */

