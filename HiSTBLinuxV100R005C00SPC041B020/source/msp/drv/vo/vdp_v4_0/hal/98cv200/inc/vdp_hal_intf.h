#ifndef __VDP_HAL_INTF_H__
#define  __VDP_HAL_INTF_H__
#include "hi_common.h"

#ifndef  __DISP_PLATFORM_BOOT__
#define  VDP_PQ_STRATEGY
#endif

#ifdef VDP_PQ_STRATEGY
#include "drv_pq_ext.h"
#endif

typedef enum tagCBB_FRAME_TYPE_E
{
	CBB_FT_NOT_STEREO = 0,    /*2d fmt*/
    CBB_FT_SBS,/*side by side.*/
    CBB_FT_TAB,/*top and bottom.*/
    CBB_FT_FPK,/*mvc fmt, or called: frame packing.*/
    CBB_FT_TILE,/*tile 3d, not usual.*/
    CBB_FT_BUTT
}CBB_FRAME_TYPE_E;

typedef enum tagCBB_VID_IFMT_E
{
    CBB_VID_IFMT_SP_400      = 0x1,
    CBB_VID_IFMT_SP_420      = 0x3,
    CBB_VID_IFMT_SP_422      = 0x4,
    CBB_VID_IFMT_SP_444      = 0x5,
    CBB_VID_IFMT_SP_TILE     = 0x6,
    CBB_VID_IFMT_SP_TILE_64  = 0x7,
    CBB_VID_IFMT_PKG_UYVY    = 0x9,
    CBB_VID_IFMT_PKG_YUYV    = 0xa,
    CBB_VID_IFMT_PKG_YVYU    = 0xb,
    
    CBB_VID_IFMT_BUTT
}CBB_PIX_FORMAT_E;

typedef enum tagCBB_FROST_MODE_E
{
    CBB_FROST_CLOSE = 0x0 ,       /**<Close  */ /**<CNcomment:关闭 CNend*/
    CBB_FROST_LOW         ,       /**<Low    */ /**<CNcomment:弱   CNend*/
    CBB_FROST_MIDDLE      ,       /**<Middle */ /**<CNcomment:中   CNend*/
    CBB_FROST_HIGH        ,       /**<High   */ /**<CNcomment:强   CNend*/

    CBB_FROST_BUTT
}CBB_FROST_MODE_E;


typedef enum tagCBB_CSC_MODE_
{
    CBB_CSC_BT601_YUV_LIMITED,
    CBB_CSC_BT601_YUV_FULL,
    CBB_CSC_BT709_YUV_LIMITED,
    CBB_CSC_BT709_YUV_FULL,
    CBB_CSC_BT709_RGB_FULL,    
    CBB_CSC_BT2020_YUV_LIMITED,
    CBB_CSC_BT2020_RGB_FULL,
    CBB_CSC_BUTT
} CBB_CSC_MODE_E;

typedef enum tagCBB_PIXEL_BITWIDTH_E 
{
    CBB_PIXEL_BITWIDTH_8BIT = 0,
    CBB_PIXEL_BITWIDTH_10BIT,
    CBB_PIXEL_BITWIDTH_10BIT_CTS,
    CBB_PIXEL_BITWIDTH_12BIT,    
    CBB_PIXEL_BITWIDTH_BUTT,
} CBB_PIXEL_BITWIDTH_E;

typedef struct tagCBB_RECT_S
{
    HI_S32 s32X;
    HI_S32 s32Y;
    HI_S32 s32Width;
    HI_S32 s32Height;
} CBB_RECT_S;


typedef enum tagCBB_STEREO_E
{
    CBB_DISP_STEREO_NONE = 0,
    CBB_DISP_STEREO_FPK,
    CBB_DISP_STEREO_SBS_HALF,
    CBB_DISP_STEREO_TAB,
    CBB_DISP_STEREO_FIELD_ALTE,
    CBB_DISP_STEREO_LINE_ALTE,
    CBB_DISP_STEREO_SBS_FULL,
    CBB_DISP_STEREO_L_DEPT,
    CBB_DISP_STEREO_L_DEPT_G_DEPT,
    CBB_DISP_STEREO_BUTT
}CBB_DISP_STEREO_E;

typedef struct tagCBB_DISPLAY_INFO_S
{
    HI_BOOL bInterlace;
    //HI_BOOL bIsBottomField;
   // HI_U32 u32Vline;
	/* 显示输出3D模式 */
	CBB_DISP_STEREO_E 	eDispMode;	
    CBB_RECT_S stFmtResolution;	
    //HI_U32 u32RefreshRate;	
    //CBB_CSC_MODE_E eColorSpace;
}CBB_DISPLAY_INFO_S;
typedef struct tagCBB_DISP_INFO_S
{
	//CBB_DISPLAY_INFO_S	stDispInfo[HI_DRV_DISPLAY_BUTT]; /*每个显示通道的信息 */
	CBB_DISPLAY_INFO_S	stDispInfo[2]; /*每个显示通道的信息 */
} CBB_DISP_INFO_S;



/*For cbb's memory requirements.*/
typedef struct tagCBB_BUF_INFO_S
{    
	HI_U8  u8BufName[8];
    /*input value, CBB raise the requirements for SDK.*/
	HI_U32 u32BufSize;	
	HI_BOOL bSecure;
	HI_BOOL bSmmu;
	
    /*smmu addr or physical addr,*/
	HI_U32 u32BufAddr;
	/*cpu's virtual addr.*/
	HI_U8 *pu8BufCpuAddr;
} CBB_REQUIRE_BUF_INFO_S;

typedef struct tagCBB_ADDRESS_S
{	
	/* Y address*/
    HI_U32  u32PhyAddr_YHead; /* only for compress format */
    HI_U32  u32PhyAddr_Y;
    HI_U8   *vir_addr_y;
    HI_U32  u32Stride_Y;

    /* C OR Cb address*/
    HI_U32  u32PhyAddr_CHead;
    HI_U32  u32PhyAddr_C;
    HI_U8   *vir_addr_c;
    HI_U32  u32Stride_C;

    /* Cr address*/
    HI_U32  u32PhyAddr_CrHead;
    HI_U32  u32PhyAddr_Cr;
    HI_U32  u32Stride_Cr;

	HI_U32  u32Head_Stride;
    HI_U32  u32Head_Size;
} CBB_ADDRESS_S;

typedef struct hiPQ_VDP_ALG_WBC_INFO_S
{
    HI_U32    u32BitDepth;                   /* 通路处理位宽 */
    HI_RECT_S stOriCoordinate;               /* Crop之前坐标 */
    HI_RECT_S stCropCoordinate;              /* Crop之后坐标 */
	
	#ifdef VDP_PQ_STRATEGY
    HI_PQ_ALGIP_SEL_E   enAlgIPSel;          /* 算法绑定位置 */
	#endif
	
    HI_VOID *pstVdpWbcRegCpuAddr;        /* ifmd、Globlal Motion and DB的回写信息 */
	HI_U32   u32VdpWbcRegPhyAddr;        /* ifmd、Globlal Motion and DB的回写信息 */	
	HI_BOOL bSecure;
	HI_BOOL bSmmu ;

    /* DNR */
    HI_BOOL bCropBeforeDB;                   /* 0: 在DB之后Crop;1: 在DB 之前Crop */
    HI_BOOL u32SingleModeFlag;                 
    HI_U32  u32ImageAvgQP;                   /* 图像平均 QP */
    HI_U32  u32VdhInfoAddr;                  /* dbinfo 地址 */
    HI_U32  u32VdhInfoStride;                /* 信息stride, 算法约定为(图像宽度+7)/8 */
    HI_U32  u32ImageStruct;                  /* 帧/场编码标识 */
	
	#ifdef VDP_PQ_STRATEGY
    HI_PQ_VDEC_PROTOCOL_E  enVdecProtocol;   /* 解码协议 */
	#endif
} PQ_VDP_ALG_WBC_INFO_S;

typedef enum tagCBB_FIELD_MODE_E
{
    CBB_FIELD_TOP = 0,
    CBB_FIELD_BOTTOM,
    CBB_FIELD_ALL,
    CBB_FIELD_BUTT
}CBB_FIELD_MODE_E;


typedef struct tagCBB_VID_FRAME_INFO_S
{	
	/*0 means left eye, 1 means right eye.*/
    CBB_ADDRESS_S stAddress[2];

	/*low 2 bit, 0 means left eye, 1 means rigth eye.*/
	CBB_ADDRESS_S stAddress_lb[2];
	
	/* mem attr for config.*/
	HI_BOOL bSecure;
	HI_BOOL bSmmu;
	
	HI_BOOL bFrameUseFul;
	
	/*1: u first(means v in high bit.), 0: v first*/
	HI_BOOL bUvorder;
	/*1:compressed stream, 0: uncompressed.*/
	HI_BOOL bCmpFmt;
	/*1:lost compress, 0: loseless compress.*/
	HI_BOOL bLostCmp;
	
	/*include cmp, tile and  other pixelfmt etc.*/
	CBB_PIX_FORMAT_E     eSrcFmt       ; 
    CBB_CSC_MODE_E       enSrcColorSpace  ; 
    CBB_PIXEL_BITWIDTH_E enSrcBitWidth    ; 
	
	/*original frame resolution, mostly == croprect  */
	CBB_RECT_S				stSrcRect         	; 	
	/*the frame's progressive and field infor.*/
	HI_BOOL                 bFrameInterlace	    ;
	CBB_FIELD_MODE_E        eFieldMode     	    ;
	
	/* 3D process ,if 3d,frame_infor[0] means left eye, frame_infor[1] means right eye.
	  * if 2d, frame_infor[1] is useless.
	  */
	CBB_FRAME_TYPE_E		en3dSrcFormat				;	
	/*view depth configure,    now seems useless. */
	HI_U32                  u32DepthLevel               ;
    HI_U32                  u32ViewLevel                ;	
}CBB_VID_FRAME_INFO_S;


typedef struct tagDRV_COLOR_S{
	HI_BOOL  bYuvColorSpace;
	HI_U32   u32YorRValue;
	HI_U32   u32UorGValue;
	HI_U32   u32VorBValue;
	HI_U32   u32BitWidth;
} CBB_COLOR_S;

/*configuration for layer.*/
typedef struct tagCBB_LAYER_CFG_INFO_S
{
	/*layer number.*/
    HI_U32                  u32LayerNO                     ; 
    HI_U32                  u32RegionNO                    ;
	HI_U32                  u32LayerZorder                 ;

	/*pay attention, layer enable is useless. for v0, if region disable then  layer disable,
	  * for V1, if  region disable,  just disable itself. if all region disabled,disable the layer 
	  */	
	HI_BOOL                 bRegionModeEn                  ;
	HI_BOOL                 bRegionEn                      ;
    //HI_BOOL                      bEnable                                      ; 	
    HI_BOOL                 bMuteEn                        ;
	
    CBB_COLOR_S              stMuteColor                    ; 
    CBB_COLOR_S              stLetterBoxColor               ;
    CBB_COLOR_S              stVideoBGColor                 ;
	HI_BOOL                 bFlipEn                        ;
	
    /*coordinate system for zme/crop/letterbox etc.*/    
	CBB_RECT_S               stCropRect                  ;
	CBB_RECT_S               stVideoRect                 ;
	CBB_RECT_S               stDispRect                  ;
	
	HI_BOOL                  bHdcpDegradePQ              ;
	CBB_RECT_S               stHdcpIntermediateRect      ;
	CBB_FROST_MODE_E         enLayerFrostMode            ;	
    CBB_CSC_MODE_E           enOutColorSpace             ;

    CBB_VID_FRAME_INFO_S   stFrame;
	PQ_VDP_ALG_WBC_INFO_S   stAlgWbcInfor;
 #if 0
	HI_NONLINEAR_CFG_S      stNonLinear                 ;/*不用传0*/
	HI_DRV_SYS_INFO_S   	stSysInfo				    ; /*包含显示信息及*/
 #endif
 	CBB_DISPLAY_INFO_S	stDispInfo ;
}CBB_LAYER_CFG_INFO_S;

typedef struct tagCBB_VIDEO_LAYER_CAPABILITY_S
{
    HI_U32  eId;
    HI_BOOL bZme;
	HI_BOOL bZmePrior;
	HI_BOOL bTnrSnrDbDMDrDs;
    HI_BOOL bDcmp;
	CBB_RECT_S stVideoMaxReadSize;
	
    /*the bitwidth of the layer. used for bg color setting.*/
    HI_U32 u32BitWidth;
}CBB_VIDEO_LAYER_CAPABILITY_S;




#if 1
/* *************************** WBC **************************************************** */
typedef enum tagDRV_VDP_WBC_E
{
    DRV_VDP_WBC_DHD= 0,
	DRV_VDP_WBC_WIN ,
    DRV_VDP_WBC_BUTT
} DRV_VDP_WBC_E;

/*回写点位置*/
typedef enum tagDRV_VDP_WBC_LOCATE_E
{
    DRV_VDP_WBC_LOCATE_CBM= 0,
    DRV_VDP_WBC_LOCATE_IPU,
	DRV_VDP_WBC_LOCATE_BFZME,
    DRV_VDP_WBC_LOCATE_BUTT
} DRV_VDP_WBC_LOCATE_E;

typedef struct tagCBB_WBC_CFG_INFO_S
{
	/***************1 these are for CBB'S config********************/
	/*wbc channel*/
    DRV_VDP_WBC_E           enWBC;                    ; 	
	/*wbc enable or not.*/ 
	HI_BOOL    bWbcEnable                              ;

	/*only OutFmt needed,  the input fmt depends on the wbc point.
	 * bit width  is useless,  the wbc's input width is invisible for software.
	 */
    CBB_PIX_FORMAT_E        eOutPutPixelFmt           ;	
	/*when write back field mode .  tell the cbb: top or bottom.*/
	CBB_FIELD_MODE_E        eFieldMode;	
	/*should give input and output  colorspace.*/
    CBB_CSC_MODE_E          enSrcColorSpace           ; 
	CBB_CSC_MODE_E          enDestColorSpace          ;
	CBB_ADDRESS_S       stAddr;	
	HI_BOOL bSecure;
	HI_BOOL bSmmu;

    /****************belows are for PQ's POLICY*******************/
	HI_RECT_S stHdWinInputSize;
	HI_RECT_S stHdWinOutputSize;
	HI_RECT_S stSdWinOutputSize;
	CBB_CSC_MODE_E enHdWinSrcColorSpace;
	CBB_CSC_MODE_E enHdWinDestColorSpace;
	
	/*when lowdelay mode,  field mode is needed, give a mandatory setting.*/
	HI_BOOL   bForceWbcFieldMode;	
	/*pip/mosaic and watermark scene, wbc point should be forced to set  to VP.*/
	HI_BOOL   bForceWbcPoint2Vp;
}CBB_WBC_CFG_INFO_S;

typedef struct CBB_WBC_POLICY_S {	
	/*tell the sdk, to write back frame or field.*/
	HI_BOOL  bWbcOutputFieldMode;
	
	/*final config size,  considered the effect of wbc point.*/
	CBB_RECT_S stSdFinaleDispRect;
	CBB_CSC_MODE_E enDestColorSpace;
	CBB_CSC_MODE_E enSrcColorSpace;
}CBB_WBC_POLICY_S;

#endif


/***************************EXT*************************************************************
扩展设置：
 包含因为芯片差异，策略性的设置。  
 例如:	
	TNR(位置，前后) ,
	滤波模式，
	低功耗。（DISP接口做）
	水印   后续补充
	HDR    后续补充
***************************EXT*************************************************************/

typedef enum tagDRV_VDP_ZME_FIR_MODE_E
{
    DRV_VDP_ZME_FIR_MODE_COPY= 0,
	DRV_VDP_ZME_FIR_MODE_FIR= 1,
    DRV_VDP_ZME_FIR_MODE_BUTT
} DRV_VDP_ZME_FIR_MODE_E;

typedef struct tagCBB_LAYER_TNR_DBM_S
{
	HI_BOOL                 	bTNRInVDP 	        ; 
}CBB_LAYER_TNR_DBM_S;

typedef struct tagWIN_LAYER_CFG_EXT_INFO_S
{
	HI_BOOL                 	bP2IEnable   		; 
	CBB_LAYER_TNR_DBM_S         stTNR_DBM   		;
	DRV_VDP_ZME_FIR_MODE_E      enFirMode 			; 		
}CBB_CFG_EXT_S;






typedef struct tagCBB_SYS_INFO_S
{
	HI_U32 u32LayerZorder[2];	/* 视频层的Z序 */
	HI_BOOL bWaterMarkMode;
	HI_U32  u32WinNum;
	HI_U32  bFrostMode;	
}CBB_SYS_INFO_S;




typedef struct tagCBB_DEBUG_INFO_S
{
}CBB_DEBUG_INFO_S;

typedef struct tagCBB_TEST_PATTERN_S
{
}CBB_TEST_PATTERN_S;


typedef HI_S32 (*MemAllocateFunType)(CBB_REQUIRE_BUF_INFO_S *);
typedef HI_S32 (*MemFreeFunType)(CBB_REQUIRE_BUF_INFO_S *);



/* **************************intf************************************************************ */
/*used to init the whole CBB's layer's mem.*/
HI_S32 CBB_Video_Init(MemAllocateFunType mem_allocate_func_ptr, MemFreeFunType mem_free_func_ptr);

/*when  sdk's video playing stopped,  sdk should call 
 *this to destroy all the cbb's video resources.
 */
HI_VOID CBB_Video_DeInit(HI_VOID);

/*used to ge the layer's capability*/
HI_S32  CBB_Video_GetLayersCapability(CBB_VIDEO_LAYER_CAPABILITY_S *pstLayerCapability);

/*used to set the whole layer's dynamic attrs: such as zorder , every display channel's infor. */
HI_S32  CBB_Video_Set_SysInfo(CBB_SYS_INFO_S *pstSysInfo);
HI_S32  CBB_Disp_Set_SysInfo(CBB_DISP_INFO_S *pstSysInfo);

/*every  period, call this func to */
HI_S32  CBB_Video_SetLayerParam(CBB_LAYER_CFG_INFO_S *pLayerCfgBasePara, 
											CBB_CFG_EXT_S *pVDPCfgExtPara);
/*init the single layer. */
HI_VOID CBB_Video_SetLayerInit(HI_U32 u32LayerNum);

/*check whether the layer is the  initiated*/
HI_BOOL CBB_Video_GetLayerStatus(HI_U32 u32LayerNum);

/*wbc should dispears , and is invisible to SDK???*/
HI_S32 CBB_Video_SetWbcParam(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,
							              CBB_CFG_EXT_S *pVDPCfgExtPara);

HI_S32 CBB_Video_GetWbcPolicy(CBB_WBC_CFG_INFO_S *pLayerCfgBasePara,
                						  CBB_CFG_EXT_S *pVDPCfgExtPara,
            							  CBB_WBC_POLICY_S *pWbcPolicy);


/*check whether the layer is wrightly configured*/
HI_BOOL  VDP_CBB_Video_HalCheck(HI_U32 u32LayerNum);
HI_VOID  CBB_Video_CheckRegConfig(HI_CHAR *pcheckItem);
HI_VOID  CBB_Video_GetDebugConfig(CBB_DEBUG_INFO_S *pstDebug);
HI_VOID  CBB_Video_HalTestPatternInputPiture(CBB_TEST_PATTERN_S *pstTestPattern);

HI_VOID  CBB_Video_SetLayerEnable(HI_U32  u32LayerNO, 
                                       HI_U32   u32RegionNO,
                                       HI_U32   bEnable);
#endif


