/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_scrolltext.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_SCROLLTEXT_H__
#define __DRV_HIFB_SCROLLTEXT_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include <linux/fb.h>


/*****************************************************************************/


/*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */


/***************************** Macro Definition ******************************/

/** \addtogroup      H_3_5 */
/** @{ */  /** <!—[ECS error code]*/

#if 1
#define SCROLLTEXT_CACHE_NUM  2   /** 双buffer  **/
#define SCROLLTEXT_NUM         2  /** 最大只支持上下两个字幕  **/


/*************************** Structure Definition ****************************/

typedef enum
{
    HIFB_SCROLLTEXT_HD0_HANDLE0 = 0x00,
    HIFB_SCROLLTEXT_HD0_HANDLE1 = 0x01,
    HIFB_SCROLLTEXT_HD1_HANDLE0 = 0x10,
    HIFB_SCROLLTEXT_HD1_HANDLE1 = 0x11,
    HIFB_SCROLLTEXT_HD2_HANDLE0 = 0x20,
    HIFB_SCROLLTEXT_HD2_HANDLE1 = 0x21,
    HIFB_SCROLLTEXT_SD0_HANDLE0 = 0x40,
    HIFB_SCROLLTEXT_SD0_HANDLE1 = 0x41,
    HIFB_SCROLLTEXT_BUTT_HANDLE
}HIFB_SCROLLTEXT_HANDLE;

/** @} */  /*! <!-- Macro Definition End */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_3_2*/
/** @{*/  /** <!—[HiFB_SCROLLTEXT]*/

/**Obtains the colorkey of a graphics layer.*/



/**HIFB_SCROLLTEXT_CACHE*/
typedef struct
{
    //HI_BOOL bAvailable;           /*the cachebuffer for scrolltext is available or not*/
    HI_BOOL bInusing;
    HI_U32  u32PhyAddr;
    HI_U8   *pVirAddr;
}HIFB_SCROLLTEXT_CACHE;


/**HIFB_SCROLLTEXT_S*/
typedef struct
{
    HI_BOOL bAvailable;           /*the scrolltext is available or not*/
    HI_BOOL bPause;               /*0:resume; 1:pause;*/
    HI_BOOL bDeflicker;
    HI_BOOL bBliting;              /*0:          ; 1:tde bliting cache buffer*/
    HI_U32  u32cachebufnum;       /*the number of cache buffer for scrolltext*/
    HI_U32  u32Stride;
    HI_S32  s32TdeBlitHandle;     /* blit handle */
    HIFB_SCROLLTEXT_HANDLE enHandle;
    volatile HI_U32 u32IdleFlag;  /*whether it is an idle cache buffer*/
    wait_queue_head_t wbEvent;    /*wait for a idle cache buffer*/
    HIFB_RECT  stRect;            /*region of the scrolltext showing on the screen*/
    HIFB_COLOR_FMT_E  ePixelFmt;  /*the color fmt of the scrolltext content*/
    HIFB_SCROLLTEXT_CACHE stCachebuf[SCROLLTEXT_CACHE_NUM];
}HIFB_SCROLLTEXT_S;


/**HIFB_SCROLLTEXT_INFO_S*/
typedef struct
{
    HI_BOOL bAvailable;
    HI_U32  u32textnum;
    HI_U32  u32ScrollTextId;
    HIFB_SCROLLTEXT_S stScrollText[SCROLLTEXT_NUM];
    struct work_struct blitScrollTextWork;
}HIFB_SCROLLTEXT_INFO_S;



/** @}*/  /** <!-- ==== Structure Definition End ====*/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

/** \addtogroup      H_1_3_2*/
/** @{*/  /** <!-- -HiFB_SCROLLTEXT=*/

/**-----Standard Functions--------*/
/**
\brief alloc scrolltext handle when create scrolltext.
\attention \n

\param[in] fd ID of an FB device
\param[out]
\retval HIFB_SCROLLTEXT_HANDLE
\par example
\code
\endcode
*/
HI_U32 hifb_alloscrolltext_handle(HI_U32 u32LayerId);


/**-----Standard Functions--------*/
/**
\brief parse the handle of scrolltext.
\attention \n

\param[in] handle of scrolltext
\param[out] pU32LayerId, pScrollTextId
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_U32 hifb_parse_scrolltexthandle(HI_U32 u32Handle, HI_U32 *pU32LayerId, HI_U32 *pScrollTextId);


/**-----Standard Functions--------*/
/**
\brief check the parament of scrolltext before create scrolltext.
\attention \n

\param[in] u32LayerId, attributes of scrolltext
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_check_scrolltext_para(HI_U32 u32LayerId, HIFB_SCROLLTEXT_ATTR_S *stAttr);



/**-----Standard Functions--------*/
/**
\brief release the cache buffer of scrolltext.
\attention \n

\param[in] struct HIFB_SCROLLTEXT_S *
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_freescrolltext_cachebuf(HIFB_SCROLLTEXT_S *pstScrollText);

/**-----Standard Functions--------*/
/**
\brief alloc the cache buffer for the scrolltext.
\attention \n

\param[in] struct HIFB_SCROLLTEXT_ATTR_S *
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_allocscrolltext_buf(HI_U32 u32LayerId, HIFB_SCROLLTEXT_ATTR_S *stAttr);


/**-----Standard Functions--------*/
/**
\brief create scrolltext.
\attention \n

\param[in] struct HIFB_SCROLLTEXT_CREATE_S *
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_create_scrolltext(HI_U32 u32LayerId, HIFB_SCROLLTEXT_CREATE_S *stScrollText);


/**-----Standard Functions--------*/
/**
\brief fill the usr data to the cache buffer.
\attention \n

\param[in] struct HIFB_SCROLLTEXT_DATA_S *
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_fill_scrolltext(HIFB_SCROLLTEXT_DATA_S *stScrollTextData);


/**-----Standard Functions--------*/
/**
\brief destroy the scrolltext and release the resource.
\attention \n

\param[in]
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_destroy_scrolltext(HI_U32 u32LayerID, HI_U32 u32ScrollTextID);


/**-----Standard Functions--------*/
/**
\brief blit the cache buffer to the display buffer.
\attention \n

\param[in]
\param[out]
\retval 0:SUCCESS
\par example
\code
\endcode
*/
HI_S32 hifb_scrolltext_blit(HI_U32 u32LayerId);

/** @}*/  /** <!-- ==== API Declaration End ====*/
#endif



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
