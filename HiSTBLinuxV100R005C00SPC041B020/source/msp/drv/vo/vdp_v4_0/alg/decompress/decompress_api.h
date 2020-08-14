#ifndef __DECOMPRESS_API_H__
#define __DECOMPRESS_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_mmz.h"
#include "drv_disp_osal.h"
#include "ttv_mode.h"


typedef struct hiS_HEAD_INFO
{
	HI_U16 cmp_flag:	1;
	HI_U16 num_pack:	4;
	HI_U16 res:			2;
	HI_U16 sample:		1;
	HI_U16 pre_mode:	8;
}S_HEAD_INFO;

typedef struct hiOriDataInfo
{
    HI_U16 width;
    HI_U16 height;
    HI_U16 slot_width;
    HI_U16 slot_height;
    HI_U32 stride;
    HI_U8 structure;
    HI_U8 chroma_idc;
    HI_U8 yuv_bit_depth;
    HI_U8 bit_depth;
    HI_U8 compress;
}HI_ORI_DATA_INFO_S;

HI_S32 VO_ALG_Decompress(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_U8 *pDstBufAddr,HI_U32 u32DstBufSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
