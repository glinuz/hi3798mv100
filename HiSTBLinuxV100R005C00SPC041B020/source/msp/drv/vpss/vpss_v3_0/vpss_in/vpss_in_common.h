#ifndef __VPSS_IN_COMMON_H__
#define __VPSS_IN_COMMON_H__
#include "vpss_common.h"
#include "drv_vdec_ext.h"

#define DEF_TOPFIRST_BUTT 0xffffffff
#define DEF_TOPFIRST_PROG 0xfffffffe

typedef HI_S32 (*PFN_IN_RlsCallback)(HI_HANDLE hSrc,HI_DRV_VIDEO_FRAME_S *pstImage);

typedef enum 
{
    VPSS_IN_MODE_VPSSACTIVE = 0,
    VPSS_IN_MODE_USRACTIVE,
    VPSS_IN_MODE_BUTT
}VPSS_IN_MODE_E;

typedef struct hiVPSS_IN_STREAM_INFO_S{
    HI_U32  u32IsNewImage;
    
    HI_U32 u32InRate;
    HI_U32 u32RealTopFirst;/*defines the stream top first info.
                                  *init:DEF_TOPFIRST_BUTT
                                  *Progressive:DEF_TOPFIRST_PROG
                                  *Interlace:DEF_TOPFIRST_BOTTOM or DEF_TOPFIRST_BOTTOM*/
    HI_U32 u32Rwzb;  
    HI_U32 u32StreamInRate;
    HI_BOOL u32StreamTopFirst;
    HI_BOOL u32StreamProg;
    HI_U32 u32StreamH;
    HI_U32 u32StreamW;
    HI_DRV_FRAME_TYPE_E  eStreamFrmType;
	HI_DRV_SOURCE_E      enSource;  
    HI_U32      u32TimingWidth; 
	HI_DRV_PIX_FORMAT_E ePixFormat;   
	HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;
    HI_DRV_FIELD_MODE_E   enCurFieldMode;
    HI_U32                   u32FrameIndex; 
    HI_U32                   u32Pts;
	HI_BOOL					bStreamByPass;
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	HI_BOOL                 bH265Interlace; // h265 interlacing type, TOP and BOTTOM are in different frame.  
#endif
}VPSS_IN_STREAM_INFO_S;


typedef struct 
{
    VPSS_VERSION_E enVersion;
	VPSS_CAPABILITY_U enCapability;
	HI_BOOL bSecure;
}VPSS_IN_ENV_S;

typedef struct 
{
    HI_DRV_VPSS_PRODETECT_E enProgInfo;
    HI_BOOL bProgRevise;
    HI_BOOL bAlwaysFlushSrc;
    HI_DRV_COLOR_SPACE_E enSrcCS;
    HI_BOOL bEnableTNR;
	HI_DRV_FIELD_MODE_E enProcessField;
}VPSS_IN_ATTR_S;

typedef struct
{
    VPSS_IN_MODE_E enMode;
    HI_HANDLE hSource;
    PFN_IN_RlsCallback pfnRlsCallback; 
    PFN_IN_RlsCallback pfnAcqCallback; 
}VPSS_IN_SOURCE_S;

typedef struct hiVPSS_IN_IMAGELIST_STATE_S{
    HI_U32 u32TotalNumb;
    HI_U32 u32FulListNumb;
    HI_U32 u32EmptyListNumb;
	
    HI_U32 u32GetUsrTotal;
    HI_U32 u32GetUsrSuccess;
    HI_U32 u32RelUsrTotal;
    HI_U32 u32RelUsrSuccess;

	HI_U32 u32PutSrcCount;
    HI_U32 u32CompleteSrcCount;  
    HI_U32 u32ReleaseSrcCount;  

}VPSS_IN_IMAGELIST_STATE_S;

#endif
