
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_alg_struct.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_ALG_STRUCT_H__
#define __DRV_DISP_ALG_STRUCT_H__

#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


typedef struct tagDISP_ALG_VERSION_S
{
    HI_U32 u32VersionPartL;
    HI_U32 u32VersionPartH;
}DISP_ALG_VERSION_S;

typedef enum tagDISP_ALG_TYPE_E
{
    DISP_ALG_CSC = 0,
    DISP_ALG_V_ACC,
    DISP_ALG_V_ACM,
    DISP_ALG_V_ZME,
    DISP_ALG_V_DIE,
    DISP_ALG_V_DNR,
    DISP_ALG_V_LTICTI,
    DISP_ALG_V_CC,

    DISP_ALG_TYPE_BUTT

}DISP_ALG_TYPE_E;

typedef enum tagDISP_ALG_CS_E
{
    DISP_ALG_CS_eUnknown            = 0,
    DISP_ALG_CS_eItu_R_BT_709       = 1,
    DISP_ALG_CS_eFCC                = 4,
    DISP_ALG_CS_eItu_R_BT_470_2_BG  = 5,
    DISP_ALG_CS_eSmpte_170M         = 6,
    DISP_ALG_CS_eSmpte_240M         = 7,
    DISP_ALG_CS_eXvYCC_709          = DISP_ALG_CS_eItu_R_BT_709,
    DISP_ALG_CS_eXvYCC_601          = 8,
    DISP_ALG_CS_eRGB                = 9,
    DISP_ALG_CS_BUTT
} DISP_ALG_CS_E;

typedef struct tagDISP_ALG_YCBCR_S
{
    HI_U8 u8Y;
    HI_U8 u8Cb;
    HI_U8 u8Cr;
}DISP_ALG_YCBCR_S;

typedef struct tagDISP_ALG_RGB_S
{
    HI_U8 u8R;
    HI_U8 u8G;
    HI_U8 u8B;
}DISP_ALG_RGB_S;






#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_ALG_STRUCT_H__  */










