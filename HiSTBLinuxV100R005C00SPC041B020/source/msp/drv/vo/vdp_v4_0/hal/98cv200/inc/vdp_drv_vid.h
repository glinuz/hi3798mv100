#ifndef __DRV_VID_H__
#define __DRV_VID_H__
#include "vdp_define.h"
#include "vdp_hal_vid.h"

#ifndef __DISP_PLATFORM_BOOT__
#include "drv_pq_ext.h"
#endif



typedef struct tagVDP_ADDR_S
{
	HI_U32 u32LumAddr;
	HI_U32 u32ChmAddr;

	HI_U32 u32LumStr;
	HI_U32 u32ChmStr;
	
	HI_U32  u32DcmpHeadSize;
	
	HI_BOOL bSecure;
	HI_BOOL bSmmu;
}VDP_ADDR_S;

typedef enum tagVDP_ADDR_E
{
	VDP_ADDR_DATA = 0,//2d left eye data
	VDP_ADDR_3D      ,//3d right eye data
	VDP_ADDR_TILE    ,//tile64 2bit
	VDP_ADDR_HEAD    ,//dcmp head data
	VDP_ADDR_MUTE    ,//black frame buffer 
	VDP_ADDR_BUTT
}VDP_ADDR_E;

typedef enum tagVDP_CLR_SPACE_E
{
    VDP_CLR_SPACE_YUV_601   = 0x1,
    VDP_CLR_SPACE_YUV_709   ,
    VDP_CLR_SPACE_YUV_2020  ,
    VDP_CLR_SPACE_RGB_601   ,
    VDP_CLR_SPACE_RGB_709   ,
    VDP_CLR_SPACE_RGB_2020  ,
    VDP_CLR_SPACE_YUV_601_L   ,
    VDP_CLR_SPACE_YUV_709_L   ,
    VDP_CLR_SPACE_YUV_2020_L  ,
    VDP_CLR_SPACE_RGB_601_L   ,
    VDP_CLR_SPACE_RGB_709_L   ,
    VDP_CLR_SPACE_RGB_2020_L  ,

    VDP_CLR_SPACE_BUTT        
    
}VDP_CLR_SPACE_E;


typedef struct tagVDP_SRC_INFO_S
{
    VDP_ADDR_S        stAddr[VDP_ADDR_BUTT] ; 
    HI_BOOL           bSecureEn             ; 
    HI_BOOL           bSmmuEn               ; 

    HI_BOOL           bUvOrder              ; 
    HI_BOOL           bDcmpEn               ; 
    HI_BOOL           bDcmpLost             ; 

    VDP_VID_IFMT_E    enInFmt               ; 
    VDP_DATA_WTH      enDataWidth          ; 
    VDP_CLR_SPACE_E   enSrcClrSpace              ; 

    VDP_RECT_S        stSrcRect             ; 

    HI_BOOL           bSrcIsInterlace       ; 
    VDP_DISP_MODE_E   enSrc3dMode           ; 

}VDP_SRC_INFO_S;

typedef struct
{
    HI_U32 u32ZmeFrmWIn;      /* zme input frame width */
    HI_U32 u32ZmeFrmHIn;      /* zme input frame height */
    HI_U32 u32ZmeFrmWOut;     /* zme output frame width */
    HI_U32 u32ZmeFrmHOut;     /* zme output frame height */


    HI_U8 u8ZmeYCFmtIn;       /* video format for zme input: 0-422; 1-420; 2-444 */
    HI_U8 u8ZmeYCFmtOut;      /* video format for zme Output: 0-422; 1-420; 2-444 */


    HI_BOOL bZmeFrmFmtIn;     /* Frame format for zme input: 0-field; 1-frame */
    HI_BOOL bZmeFrmFmtOut;    /* Frame format for zme Output: 0-field; 1-frame */
    HI_BOOL bZmeBFIn;         /* Input field polar when input is field format: 0-top field; 1-bottom field */
    HI_BOOL bZmeBFOut;        /* Output field polar when Output is field format: 0-top field; 1-bottom field */

	HI_BOOL   bZmeFirHL;       /* zme mode of horizontal luma     : 0-copy mode; 1-FIR filter mode */
    HI_BOOL   bZmeFirHC;       /* zme mode of horizontal chroma  : 0-copy mode; 1-FIR filter mode */
    HI_BOOL   bZmeFirVL;       /* zme mode of vertial luma          : 0-copy mode; 1-FIR filter mode */
    HI_BOOL   bZmeFirVC;       /* zme mode of vertial chroma      : 0-copy mode; 1-FIR filter mode */
	
	HI_BOOL   bZmeMedH; 	   /* zme Median filter enable of horizontal luma: 0-off; 1-on */
	HI_BOOL   bZmeMedV;

	HI_BOOL   bZmeDefault;     /* 1:rwzb; 0:no rwzb */
	
    /* 1.OriRect; 2.InFrameRate; 3.OutRate; 4.Out I/P */
    //ALG_RECT_S stOriRect;
    HI_U32  u32InRate;        /* Vpss out Rate  RealRate*1000 */
    HI_U32  u32OutRate;       /* Disp Rate      RealRate*1000 */
    HI_BOOL bDispProgressive; /* 1:Progressive  0:Interlace */
    HI_U32  u32Fidelity;      /* rwzb info >0:is rwzb */
} VDP_VZME_DRV_PARA_S;

typedef struct
{
	HI_S32 s32ZmeOffsetHC;
	HI_S32 s32ZmeOffsetVC;
	HI_S32 s32ZmeOffsetVLBtm;
	HI_S32 s32ZmeOffsetVCBtm;
	HI_S32 bZmeOrder;

	HI_U32 u32ZmeWIn ;
	HI_U32 u32ZmeWOut;
	HI_U32 u32ZmeHIn ;
	HI_U32 u32ZmeHOut;
	HI_U32 u32ZmeRatioHL;
	HI_U32 u32ZmeRatioVL;
	HI_U32 u32ZmeRatioHC;
	HI_U32 u32ZmeRatioVC;

	HI_S32 s32ZmeOffsetHL;
	HI_S32 s32ZmeOffsetVL;
}VDP_VZME_RTL_PARA_S;

typedef struct tagVDP_DISPLAY_INFO_S
{
    HI_BOOL bInterlace;
	/* ???¡ìo??¡ìo?3?3D??¡§o?¡ìo? */
	VDP_DISP_MODE_E 	   enDispMode;
    VDP_RECT_S            stFmtResolution;	
}VDP_DISPLAY_INFO_S;


typedef struct tagVDP_LAYER_VID_INFO_S
{

    HI_U32                 u32RegionNO                    ;
	HI_U32                 u32LayerZorder                 ;
	
    HI_BOOL                bRegionEn                      ;
    HI_BOOL                bEnable                        ;
	HI_BOOL                bRegionModeEn                  ;

    VDP_SRC_INFO_S         stSrcInfo;
    //source info
    HI_BOOL                bFlipEn                        ;

    //input info
    VDP_DATA_RMODE_E       enReadMode                     ;//
    VDP_RECT_S             stCropRect/*stInRect?*/        ;
    VDP_CLR_SPACE_E        enInClrSpace                   ;

    //output info
    VDP_RECT_S             stVideoRect                    ;
    VDP_RECT_S             stDispRect                     ;
    VDP_CLR_SPACE_E        enOutClrSpace                  ;
    //process
    HI_BOOL                bMuteEn                        ;
	VDP_BKG_S              stMuteColor                    ; 
    VDP_BKG_S              stLetterBoxColor               ;
    VDP_BKG_S              stMixvColor                    ; 

    VDP_DISP_MODE_E        enDispMode                     ;
    HI_U32                 u32DepthLevel                  ;
	
	VDP_DRAW_MODE_E enDrawMode;
	
	VDP_RECT_S stZme2InRect;
	
	VDP_VZME_DRV_PARA_S pstZmeDrvPara;//add by hyx
	VDP_DISPLAY_INFO_S 		stVdpDispInfo; 
	
}VDP_LAYER_VID_INFO_S;

typedef struct tagVDP_DRV_ADDR_INFO_S
{
	//for flip calc
	HI_BOOL bFlipEn;
	HI_U32 u32Hgt;
	HI_U32 u32ChmRatio;

	VDP_VID_IFMT_E enInFmt;
	VDP_DISP_MODE_E enDispMode;

	//addr
	VDP_ADDR_S stAddr[VDP_ADDR_BUTT];

	//dcmp
	HI_BOOL bDcmpEn;

	//draw mode
	VDP_DRAW_MODE_E enDrawMode;

}VDP_DRV_ADDR_INFO_S;


typedef struct tagVDP_DRV_CROP_INFO_S
{
	VDP_RECT_S         stSrcRect   ;
	VDP_RECT_S         stCropRect  ;//Crop Out
    VDP_VID_IFMT_E     enInFmt     ;
	VDP_DATA_RMODE_E   enReadMode  ;
	VDP_DISP_MODE_E    enDispMode  ;
    HI_BOOL            bFlipEn     ;
    HI_U32             u32ChmRatio ;
	VDP_DRAW_MODE_E    enDrawMode;
	
	VDP_ADDR_S         stAddr[VDP_ADDR_BUTT];
    

}VDP_DRV_CROP_INFO_S;

typedef struct tagVDP_DRV_ZME_INFO_S
{
	VDP_RECT_S stZme2InRect;
    VDP_RECT_S stInRect         ;
    VDP_RECT_S stOutRect        ;
    VDP_VID_IFMT_E enInFmt      ;
    VDP_DATA_WTH   enDataWith     ;
    VDP_DATA_RMODE_E enReadMode ;
    VDP_DISP_MODE_E enDispMode  ;
	VDP_DRAW_MODE_E enDrawMode;

	VDP_RECT_S  stFmtResolution;
    VDP_DISPLAY_INFO_S 		stVdpDispInfo; 
	VDP_VZME_DRV_PARA_S pstZmeDrvPara;//add by hyx
}VDP_DRV_ZME_INFO_S;

//typedef struct tagVDP_DRV_CROP_INFO_S
//{
//	VDP_RECT_S stSrcRect;
//	VDP_RECT_S stCropRect;
//	HI_U32     u32ChmRatio;
//
//	VDP_DISP_MODE_E enDispMode;
//
//}VDP_DRV_CROP_INFO_S;

HI_S32 VDP_DRV_SetVidLayer(HI_U32 u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo);
HI_S32 VDP_DRV_SetVidCrop (HI_U32 u32LayerId, const VDP_DRV_CROP_INFO_S *pstCropInfo, VDP_DRV_ADDR_INFO_S *pstAddrInfo);
HI_S32 VDP_DRV_SetVidAddr(HI_U32 u32LayerId, const VDP_DRV_ADDR_INFO_S *pstAddrInfo);
HI_S32 VDP_DRV_SetVidZme(HI_U32 u32LayerId, VDP_DRV_ZME_INFO_S *pstZmeInfo);
HI_S32 VDP_DRV_SetVidCsc(HI_U32 u32LayerId, VDP_CLR_SPACE_E enInClr, VDP_CLR_SPACE_E enOutClr);
HI_VOID VdpZmeComnSet(VDP_VZME_DRV_PARA_S* pstZmeDrvPara, VDP_VZME_RTL_PARA_S* pstZmeRtlPara);
HI_VOID VDP_DRV_SetVidZme2ForPal(HI_U32 u32LayerId, HI_BOOL bEnable);
HI_VOID VDP_DRV_SetVid1ZmeForPal(HI_U32 u32LayerId);
HI_S32 VDP_DRV_SetVid0Zme1ForPal(HI_U32 u32LayerId, VDP_DRV_ZME_INFO_S *pstZmeInfo);
HI_VOID VDP_DRV_SetLayerBkgColorAndAlpha(HI_U32 u32LayerId, VDP_BKG_S *pstMuteColor, VDP_CLR_SPACE_E    enOutClrSpace);


#ifndef __DISP_PLATFORM_BOOT__
HI_S32 VDP_DRV_GetVidZmeStrategy(VDP_DRV_ZME_INFO_S  *pstZmeInfo,
	                                            HI_PQ_ZME_COEF_RATIO_E  *penHiPqZmeCoefRatio,
	                                            HI_PQ_ZME_COEF_TYPE_E enPqZmeCoefRatio,
	                                            VDP_VID_IFMT_E   enInZmeFmt);
#endif

#endif
