#ifndef __HI_DRV_VPSS_H__
#define __HI_DRV_VPSS_H__

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"

/** Handle Definition.*/
/**<CNcomment: ??¡À¨²?¡§¨°? \n
      ??¡À¨²¨º¦Ì?¨º¨¦?¨º?¨°?????¨ºy¡ê?¨®?¨®¨²¡À¨º¨º?chano¨ªport¨º¡À¨ºy?¦Ì¦Ì??¡§¨°?¨®D?¨´2?¨ª?¡ê?¦Ì???¨°?-1¡Á¡Â?a?TD¡ì??¡À¨²?¡ê\n
      1. VPSS¨º¦Ì¨¤y??¡À¨²¡ê????¦Ì?a¨º¦Ì¨¤y¦Ì??¡Â¨°yo?(ID) \n
      2. ???¨²??¡À¨²¡ê????¦Ì¨°a¡À¨ª¨º?¨¢????¨²¨¨Y: ?¨´¨º?¨ª¡§¦Ì¨¤?¡Â¨°y¡ê?¨°??¡ã???¨²¡À?¨¦¨ª¦Ì??¡Â¨°y?¡ê\n
              = ¨º¦Ì¨¤y?¡Â¨°y*256 + ???¨²?¡Â¨°y */

typedef HI_S32 VPSS_HANDLE;
#define VPSS_INVALID_HANDLE (-1)

#define PORTHANDLE_TO_VPSSID(hPort)    (hPort >> 8)
#define PORTHANDLE_TO_PORTID(hPort)    (hPort & 0xff)

/*************************** Macro definition ****************************/
#define DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER 40
#define DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_EXT_NUMBER 12
#define DEF_HI_DRV_VPSS_PORT_MAX_NUMBER 3

/** Last Frame Flag,indicate it is a valid frame.*/
/**<CNcomment: ?¡ã??¡ä?¨¨?¦Ì?¡Á?o¨®¨°???¡À¨º??¡ê?????¨®DD¡ì*/
#define DEF_HI_DRV_VPSS_LAST_FRAME_FLAG 0xffee

/** Last Frame Flag,indicate it is an invalid frame.*/
/**<CNcomment: ?¡ã??¡ä?¨¨?¦Ì?¡Á?o¨®¨°???¡À¨º??¡ê??????TD¡ì*/
#define DEF_HI_DRV_VPSS_LAST_ERROR_FLAG 0xff00

/*************************** Structure Definition ****************************/

/**defines the vertical flip type.*/
/**CNcomment:?¡§¨°?¨º?¡¤?¡ä¨°?a¡ä1?¡À¡¤-¡Áa1|?¨¹*/
typedef enum
{
    HI_DRV_VPSS_HFLIP_DISABLE = 0,
    HI_DRV_VPSS_HFLIP_ENABLE,
    HI_DRV_VPSS_HFLIP_BUTT
}HI_DRV_VPSS_HFLIP_E;

/**defines the horizon flip type.*/
/**CNcomment:?¡§¨°?¨º?¡¤?¡ä¨°?a????¡¤-¡Áa1|?¨¹*/
typedef enum
{
    HI_DRV_VPSS_VFLIP_DISABLE = 0,
    HI_DRV_VPSS_VFLIP_ENABLE,
    HI_DRV_VPSS_VFLIP_BUTT
}HI_DRV_VPSS_VFLIP_E;


/**defines the ROTATION type.*/
/**CNcomment:?¡§¨°?¨º¨®?¦ÌDy¡Áa???¨¨*/
typedef enum
{
    HI_DRV_VPSS_ROTATION_DISABLE = 0,
    HI_DRV_VPSS_ROTATION_90,
    HI_DRV_VPSS_ROTATION_180,
    HI_DRV_VPSS_ROTATION_270,
    HI_DRV_VPSS_ROTATION_BUTT
}HI_DRV_VPSS_ROTATION_E;

/*Delete*/
typedef enum
{
    HI_DRV_VPSS_STEREO_DISABLE = 0,
    HI_DRV_VPSS_STEREO_SIDE_BY_SIDE,
    HI_DRV_VPSS_STEREO_TOP_AND_BOTTOM,
    HI_DRV_VPSS_STEREO_TIME_INTERLACED,
    HI_DRV_VPSS_STEREO_BUTT
}HI_DRV_VPSS_STEREO_E;


/**defines the Deinterlace mode.
    3FIELD¡ê?4FIELD¡ê?5FIELD is supported?¡ê
    Default is HI_DRV_VPSS_DIE_5FIELD*/
/**CNcomment:?¡§¨°?¨¨£¤??DD??¡¤¡§?¡ê¨º?
    ???¡ì3?¨¨y?¡é???¡é??3??¡ê¨º?
    ??¨¨??????a??3??¡ê¨º?*/
typedef enum
{
    HI_DRV_VPSS_DIE_DISABLE = 0,
    HI_DRV_VPSS_DIE_AUTO,
    HI_DRV_VPSS_DIE_2FIELD,
    HI_DRV_VPSS_DIE_3FIELD,
    HI_DRV_VPSS_DIE_4FIELD,
    HI_DRV_VPSS_DIE_5FIELD,
    HI_DRV_VPSS_DIE_6FIELD,
    HI_DRV_VPSS_DIE_7FIELD,
    HI_DRV_VPSS_DIE_BUTT
}HI_DRV_VPSS_DIE_MODE_E;

/*Delete*/
typedef enum
{
    HI_DRV_VPSS_ACC_DISABLE = 0,
    HI_DRV_VPSS_ACC_LOW,
    HI_DRV_VPSS_ACC_MIDDLE,
    HI_DRV_VPSS_ACC_HIGH,
    HI_DRV_VPSS_ACC_BUTT
}HI_DRV_VPSS_ACC_E;

/*Delete*/
typedef enum
{
    HI_DRV_VPSS_ACM_DISABLE = 0,
    HI_DRV_VPSS_ACM_BLUE,
    HI_DRV_VPSS_ACM_GREEN,
    HI_DRV_VPSS_ACM_BG,
    HI_DRV_VPSS_ACM_SKIN,
    HI_DRV_VPSS_ACM_BUTT
}HI_DRV_VPSS_ACM_E;

/*Delete*/
typedef enum
{
    HI_DRV_VPSS_CC_DISABLE = 0,
    HI_DRV_VPSS_CC_ENABLE,
    HI_DRV_VPSS_CC_AUTO,
    HI_DRV_VPSS_CC_BUTT
}HI_DRV_VPSS_CC_E;

/**defines the Sharpen type.*/
/**CNcomment:?¡§¨°?¨¨??¡¥?¡ê¨º?*/
/*D¨¨¨°a¨¨¡¤¨¨?*/
typedef enum
{
    HI_DRV_VPSS_SHARPNESS_DISABLE = 0,
    HI_DRV_VPSS_SHARPNESS_ENABLE,
    HI_DRV_VPSS_SHARPNESS_AUTO,
    HI_DRV_VPSS_SHARPNESS_BUTT
}HI_DRV_VPSS_SHARPNESS_E;

/**defines the Dnr type.*/
/**CNcomment:?¡§¨°?¨¨£¤?¨¦¨¨£¤??¨¢???¡¤¡§?¡ê¨º?*/
typedef enum
{
    HI_DRV_VPSS_DNR_DISABLE = 0,
    HI_DRV_VPSS_DNR_ENABLE,
    HI_DRV_VPSS_DNR_AUTO,
    HI_DRV_VPSS_DNR_BUTT
}HI_DRV_VPSS_DNR_E;

/*Delete ,will be HI_DRV_VPSS_DNR_E*/
typedef enum
{
    HI_DRV_VPSS_DB_DISABLE = 0,
    HI_DRV_VPSS_DB_ENABLE,
    HI_DRV_VPSS_DB_AUTO,
    HI_DRV_VPSS_DB_BUTT
}HI_DRV_VPSS_DB_E;
/*Delete ,will be HI_DRV_VPSS_DNR_E*/
typedef enum
{
    HI_DRV_VPSS_DR_DISABLE = 0,
    HI_DRV_VPSS_DR_ENABLE,
    HI_DRV_VPSS_DR_AUTO,
    HI_DRV_VPSS_DR_BUTT
}HI_DRV_VPSS_DR_E;
/*delete*/
/**defines the color space convert type.*/
/**CNcomment:?¡§¨°?¨¦?2¨º????¡Áa???¡ê¨º?*/
typedef enum
{
    HI_DRV_VPSS_CSC_DISABLE = 0,
    HI_DRV_VPSS_CSC_ENABLE,
    HI_DRV_VPSS_CSC_AUTO,
    HI_DRV_VPSS_CSC_BUTT
}HI_DRV_VPSS_CSC_E;

/**defines the fidelity type.*/
/**CNcomment:?¡§¨°?¡À¡ê??¡ä|¨¤¨ª?¡ê¨º?*/
typedef enum
{
    HI_DRV_VPSS_FIDELITY_DISABLE = 0,
    HI_DRV_VPSS_FIDELITY_ENABLE,
    HI_DRV_VPSS_FIDELITY_AUTO,
    HI_DRV_VPSS_FIDELITY_BUTT
}HI_DRV_VPSS_FIDELITY_E;


typedef enum
{
    HI_DRV_VPSS_SCENE_MODE_NORMAL,
    HI_DRV_VPSS_SCENE_MODE_VP_PREVIEW,
    HI_DRV_VPSS_SCENE_MODE_VP_REMOTE,
    HI_DRV_VPSS_SCENE_MODE_BUTT,
}HI_DRV_VPSS_SCENEMODE_E;


typedef struct
{
    HI_DRV_VPSS_HFLIP_E  eHFlip;
    HI_DRV_VPSS_VFLIP_E  eVFlip;
    HI_DRV_VPSS_STEREO_E eStereo;
    HI_DRV_VPSS_ROTATION_E  eRotation;
    HI_DRV_VPSS_DIE_MODE_E eDEI;
    HI_DRV_VPSS_ACC_E eACC;
    HI_DRV_VPSS_ACM_E eACM;
    HI_DRV_VPSS_CC_E eCC;
    HI_DRV_VPSS_SHARPNESS_E eSharpness;
    HI_DRV_VPSS_DB_E eDB;
    HI_DRV_VPSS_DR_E eDR;

    HI_BOOL bIFMD;

    HI_RECT_S stInRect;
    HI_BOOL   bUseCropRect;
    HI_DRV_CROP_RECT_S stCropRect;
}HI_DRV_VPSS_PROCESS_S;

typedef struct
{
    HI_DRV_VPSS_CSC_E eCSC;
    HI_DRV_VPSS_FIDELITY_E eFidelity;
}HI_DRV_VPSS_PORT_PROCESS_S;


/**defines the vpss instance attr.*/
/**CNcomment:?¡§¨°??e??DD?¨¬2a????*/
typedef enum
{
    HI_DRV_VPSS_PRODETECT_PROGRESSIVE = 0,
    HI_DRV_VPSS_PRODETECT_INTERLACE,
    HI_DRV_VPSS_PRODETECT_AUTO,
    HI_DRV_VPSS_PRODETECT_BUTT
}HI_DRV_VPSS_PRODETECT_E;

/**defines the vpss IP attr.*/
/**CNcomment:?¡§¨°?VPSS¡ã¨®?¡§IP????*/
typedef enum
{
    HI_DRV_VPSS_IPMODE_AUTO = 0,
    HI_DRV_VPSS_IPMODE_IP0,
    HI_DRV_VPSS_IPMODE_IP1,
    HI_DRV_VPSS_IPMODE_BUTT
}HI_DRV_VPSS_IPMODE_E;



/**defines the vpss instance attr.*/
/**CNcomment:?¡§¨°?¨º¦Ì¨¤y????*/
typedef struct
{
    HI_S32  s32Priority;  /**defines the instance priority.
                                default 16
                                0 is valid ,1 ~ 31 is more and more prior
                                */
                          /**CNcomment:0?TD¡ì¡ê?1 ~ 31?a?y3¡ê¨®??¨¨??¡ê?¨ºy?¦Ì??¡ä¨®¨®??¨¨??????*/

    HI_BOOL bAlwaysFlushSrc;    /**whether process the newest frame.
                                        */
                                /**CNcomment:¨º?¡¤???¡ä|¨¤¨ª¡Á?D?¨°???¡ê??¨²¦Ì¨ª?¨®3¨´?¡ê¨º??????aTRUE
                                            ?a?¨´??¡ä????¨¢?¨¢??Src buf¡ê???¨¢?¡Á?D?¨°???¡ä|¨¤¨ª */

    HI_DRV_VPSS_PRODETECT_E enProgInfo;

    HI_BOOL bProgRevise;
    HI_DRV_VPSS_SCENEMODE_E enSceneMode;
    HI_DRV_COLOR_SPACE_E enSrcCS;
    
    HI_DRV_VPSS_PROCESS_S stProcCtrl;

	HI_BOOL bSecure;

}HI_DRV_VPSS_CFG_S;


/**defines the out buffer mode.*/
/**CNcomment:?¡§¨°?¨º?3???¡ä?1¨¹¨¤¨ª?¡ê¨º?*/
typedef enum hiDRV_VPSS_BUFFER_TYPE_E{
    HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE = 0,
    HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE,
    HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE,
    HI_DRV_VPSS_BUF_TYPE_BUTT
}HI_DRV_VPSS_BUFFER_TYPE_E;

typedef struct hiDRV_VPSS_BUFFER_CFG_S
{
    HI_DRV_VPSS_BUFFER_TYPE_E eBufType;

    HI_U32 u32BufNumber;     /**bBufferNumber must be <= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER */
    HI_U32 u32BufSize;       /** every buffer size in Byte */
    HI_U32 u32BufStride;     /**only for HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE */
    //HI_U32 u32BufPhyAddr[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER]; /*128bit aligned */
    HI_U32 u32BufPhyAddr[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_EXT_NUMBER]; /*128bit aligned */
}HI_DRV_VPSS_BUFLIST_CFG_S;

/**defines the Port attr.*/
/**CNcomment:?¡§¨°?¨º?3????¨²????*/
typedef struct
{
    HI_DRV_COLOR_SPACE_E eDstCS;        /**Output color space*/

    HI_BOOL bOnlyKeyFrame;      /* ??o?¨®2?tFRC1¡è¡Á¡Â¡ê?¦Ì?¨®¡ã?¡ê¨º??¡äD¨¨¨°a¨º?3?¡¤????¡ä??¡ê?TV?¨¤¨®D */
    HI_BOOL bLBDCropEn;         /* ?¡¥¨¬?CROP¨°??¨¬2a¦Ì?¦Ì?o¨²¡À?¡ê?TV?¨¤¨®D */
    HI_RECT_S stVideoRect;      /* TV LBXD¨¨?¨® */

    HI_RECT_S stInRect;
    HI_BOOL   bUseCropRect;
    HI_DRV_CROP_RECT_S stCropRect;

    HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth;

    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    /*display Info*/
    HI_DRV_ASPECT_RATIO_S stDispPixAR;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;

    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;

    HI_BOOL   bInterlaced;                /**display timing*/
    HI_RECT_S stScreen;                   /**display screen resolution*/

    HI_DRV_PIX_FORMAT_E eFormat;            /**Output pix format*/
    HI_DRV_VPSS_BUFLIST_CFG_S stBufListCfg;
    HI_U32 u32MaxFrameRate;                 /* in 1/100 HZ  */

    HI_BOOL  bTunnelEnable;
    HI_S32  s32SafeThr;                 /**if Tunnel is  enabled, it is used to keep logic W/R order*/
                                        /**CNcomment:¡ã2¨¨?????¡ê?0~100¡ê??a¨º?3???¨°?¨ª¨º3¨¦¦Ì?¡ã¨´¡¤?¡À¨¨.
                                                    0¡À¨ª¨º???¨º¡À?¨¦??o¨®??¡ê?100¡À¨ª¨º?¨ª¨º¨¨?¨ª¨º3¨¦2??¨¹??o¨®?? */
    HI_BOOL   b3Dsupport;
    HI_BOOL   bPassThrough;

    HI_DRV_VPSS_ROTATION_E enRotation;
    HI_BOOL bVertFlip;
    HI_BOOL bHoriFlip;
}HI_DRV_VPSS_PORT_CFG_S;


/**defines VPSS bypass ATTR */
typedef struct
{ 
    HI_U32 u32InputWidth;
    HI_U32 u32InputHeight;
    HI_DRV_PIX_FORMAT_E enInputPixFormat;
    HI_U32 u32InputFrameRate;
    HI_BOOL bVpssBypass;                  
}HI_DRV_VPSS_BYPASSATTR_S;

typedef struct
{
    VPSS_HANDLE hPort;
    HI_BOOL bAvailable;
}HI_DRV_VPSS_PORT_AVAILABLE_S;

/**defines the user control command.*/
typedef enum
{
    HI_DRV_VPSS_USER_COMMAND_IMAGEREADY = 0,
    HI_DRV_VPSS_USER_COMMAND_RESET,
    HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE,
    HI_DRV_VPSS_USER_COMMAND_CHECKAVAILABLE,
    HI_DRV_VPSS_USER_COMMAND_START,
    HI_DRV_VPSS_USER_COMMAND_STOP,
    HI_DRV_VPSS_USER_COMMAND_CHANGEIP,
    HI_DRV_VPSS_USER_COMMAND_STOREPRIV,
    HI_DRV_VPSS_USER_COMMAND_CHECKBYPASS,   
    HI_DRV_VPSS_USER_COMMAND_BUTT
}HI_DRV_VPSS_USER_COMMAND_E;

/**defines the out buffer state structure.*/
/**CNcomment:?¡§¨°?¨º?3???¡ä??¨®¨¢D¡Á¡ä¨¬?*/
typedef struct
{
    HI_U32 u32TotalBufNumber;
    HI_U32 u32FulBufNumber;
}HI_DRV_VPSS_PORT_BUFLIST_STATE_S;


typedef enum
{
    HI_DRV_VPSS_BUFFUL_PAUSE = 0,
    HI_DRV_VPSS_BUFFUL_KEEPWORKING,
    HI_DRV_VPSS_BUFFUL_BUTT
}HI_DRV_VPSS_BUFFUL_STRATAGY_E;


/**defines the Pre-Module Mutual Mode.*/
/**CNcomment:?¡§¨°?¨®??¡ã???¡ê?¨¦???£¤?¡ê¨º?*/
typedef enum
{
    VPSS_SOURCE_MODE_USERACTIVE = 0,
    VPSS_SOURCE_MODE_VPSSACTIVE,
    VPSS_SOURCE_MODE_BUTT
}HI_DRV_VPSS_SOURCE_MODE_E;

typedef HI_S32 (*PFN_VPSS_SRC_FUNC)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);

typedef struct
{
    PFN_VPSS_SRC_FUNC VPSS_GET_SRCIMAGE;
    PFN_VPSS_SRC_FUNC VPSS_REL_SRCIMAGE;
}HI_DRV_VPSS_SOURCE_FUNC_S;

/**defines the user buffer structure.*/
/**CNcomment:?¡§¨°?VPSS?¨°¨ªa2?¨¦¨º??¦Ì?BUFER?¨¢11*/
typedef struct
{
    VPSS_HANDLE hPort;
    HI_U8* pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
    HI_U32 u32Stride;
    HI_U32 u32FrmH;
    HI_U32 u32FrmW;
	HI_U32 u32PrivDataPhyAddr;
    HI_U8* pu8PrivDataVirAddr;
    HI_U32 u32PrivDataSize;
}HI_DRV_VPSS_FRMBUF_S;

typedef struct
{
    VPSS_HANDLE hPort;
    HI_DRV_VIDEO_FRAME_S stFrame;
}HI_DRV_VPSS_FRMINFO_S;

typedef struct
{
    VPSS_HANDLE hPort;
    HI_BOOL bAvailable;
}HI_DRV_VPSS_BUFINFO_S;

/**defines the vpss process event.*/
/**CNcomment:?¡§¨°?VPSS¡ä|¨¤¨ª¨º??t*/
typedef enum
{
    VPSS_EVENT_BUFLIST_FULL,
    VPSS_EVENT_GET_FRMBUFFER,
    VPSS_EVENT_REL_FRMBUFFER,
    VPSS_EVENT_NEW_FRAME,
    VPSS_EVENT_CHECK_FRAMEBUFFER,
    VPSS_EVENT_TASK_COMPLETE,
    VPSS_EVENT_BUTT,
}HI_DRV_VPSS_EVENT_E;

/**defines the vpss process event callback.*/
/**CNcomment:?¡§¨°?VPSS¡ä|¨¤¨ª¨º??t??¦Ì¡Â
    VPSS_EVENT_BUFLIST_FULL:¨º?3???¡ä??¨²¡ê?¨¦?¡À¡§HI_DRV_VPSS_BUFFUL_STRATAGY_E?¨¢11¨¬?¡ê?¨®??¡ì¡¤¦Ì??¡ä|¨¤¨ª2???
    VPSS_EVENT_GET_FRMBUFFER:??¨¨?¨º?3???¡ä?¡ê?¨¦?¡À¡§HI_DRV_VPSS_FRMBUF_S?¨¢11¨¬?¡ê?¨®??¡ì?3?¦Ì
    VPSS_EVENT_REL_FRMBUFFER:¨º¨ª¡¤?¨®??¡ì??¡ä?¡ê?¨¦?¡À¡§HI_DRV_VPSS_FRMBUF_S?¨¢11¨¬?¡ê?¨®??¡ì¡ä|¨¤¨ª
    VPSS_EVENT_NEW_FRAME:D???¡ä|¨¤¨ª¨ª¨º3¨¦¡ê?¨¦?¡À¡§HI_NULL¡ê?¨®??¡ì¡ä|¨¤¨ª*/
typedef HI_S32 (*PFN_VPSS_CALLBACK)(HI_HANDLE hDst, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID *pstArgs);


/******************************* API declaration *****************************/

HI_S32 HI_DRV_VPSS_GlobalInit(HI_VOID);
HI_S32 HI_DRV_VPSS_GlobalDeInit(HI_VOID);

HI_S32  HI_DRV_VPSS_GetDefaultCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg);

HI_S32  HI_DRV_VPSS_CreateVpss(HI_DRV_VPSS_CFG_S *pstVpssCfg,VPSS_HANDLE *phVPSS);
HI_S32  HI_DRV_VPSS_DestroyVpss(VPSS_HANDLE hVPSS);

HI_S32  HI_DRV_VPSS_SetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32  HI_DRV_VPSS_GetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);

HI_S32  HI_DRV_VPSS_GetDefaultPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

HI_S32  HI_DRV_VPSS_CreatePort(VPSS_HANDLE hVPSS, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg, VPSS_HANDLE *phPort);
HI_S32  HI_DRV_VPSS_DestroyPort(VPSS_HANDLE hPort);

HI_S32  HI_DRV_VPSS_GetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
HI_S32  HI_DRV_VPSS_SetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

HI_S32  HI_DRV_VPSS_EnablePort(VPSS_HANDLE hPort, HI_BOOL bEnable);

HI_S32  HI_DRV_VPSS_SendCommand(VPSS_HANDLE hVPSS, HI_DRV_VPSS_USER_COMMAND_E eCommand, HI_VOID *pArgs);

HI_S32  HI_DRV_VPSS_GetPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);
HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);

HI_S32  HI_DRV_VPSS_GetPortBufListState(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstVpssBufListState);
HI_BOOL  HI_DRV_VPSS_CheckPortBufListFul(VPSS_HANDLE hPort);

HI_S32 HI_DRV_VPSS_SetSourceMode(VPSS_HANDLE hVPSS,
                          HI_DRV_VPSS_SOURCE_MODE_E eSrcMode,
                          HI_DRV_VPSS_SOURCE_FUNC_S* pstRegistSrcFunc);

HI_S32  HI_DRV_VPSS_PutImage(VPSS_HANDLE hVPSS, HI_DRV_VIDEO_FRAME_S *pstImage);
HI_S32  HI_DRV_VPSS_GetImage(VPSS_HANDLE hVPSS, HI_DRV_VIDEO_FRAME_S *pstImage);

HI_S32  HI_DRV_VPSS_RegistHook(VPSS_HANDLE hVPSS, HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);

HI_S32  HI_DRV_VPSS_InvokeDisplayInfo(HI_DISP_DISPLAY_INFO_S *pstDisplayInfo);
#endif

