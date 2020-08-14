/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_common.h
Version             : Initial Draft
Author              :
Created             : 2016/01/12
Description         :
                       CNcomment: 开机logo公共头文件 CNend\n
Function List   :
History         :
Date                        Author                  Modification
2016/01/12                  y00181162               Created file
******************************************************************************/

#ifndef  __HI_GO_COMMON_H__
#define  __HI_GO_COMMON_H__


/*********************************add include here******************************/

#include "hi_type.h"
#include "exports.h"

/***************************** Macro Definition ******************************/

/**
 ** 最大分量数
 **/
#define  CONFIG_GFX_PIXELDATA_NUM         3


#ifndef HI_ADVCA_FUNCTION_RELEASE
    #define HI_GFX_Log()   printf("=======%s %s %d failure\n",__FILE__,__FUNCTION__,__LINE__)
    #define HI_GFX_Print   printf
#else
    #define HI_GFX_Log()   {do{}while(0);}
    #define HI_GFX_Print   {do{}while(0);}
#endif


#define HI_GFX_RESERVE_Malloc(size,module)  HI_GFX_LOGO_RESERVE_Alloc(size,module)
#define HI_GFX_MMZ_Malloc(size,module)      HI_GFX_LOGO_MMZ_Alloc(size,module)
#define HI_GFX_MMZ_Free(phy)                HI_GFX_LOGO_MMZ_Free(phy)
#define HI_GFX_Malloc(size,module)          HI_GFX_LOGO_Malloc(size,module)
#define HI_GFX_Free(phy)                    HI_GFX_LOGO_Free(phy)

/*************************** Structure Definition ****************************/

typedef enum tagHIGO_COMPOPT_E
{
    HIGO_COMPOPT_NONE = 0,
    HIGO_COMPOPT_BUTT
}HIGO_COMPOPT_E;

typedef enum hiGoLogoFmtE
{
    HIGO_PF_4444       = 3,
    HIGO_PF_0444       = 4,
    HIGO_PF_1555       = 5,
    HIGO_PF_0555       = 6,
    HIGO_PF_565        = 7,
    HIGO_PF_8565       = 8,
    HIGO_PF_8888       = 9,
    HIGO_PF_0888       = 10,
    HIGO_PF_YUV400     = 11,
    HIGO_PF_YUV420     = 12,
    HIGO_PF_YUV444     = 13,
    HIGO_PF_YUV422_12  = 14,
    HIGO_PF_YUV422_21  = 15,
    HIGO_PF_AYCbCr8888 = 16,
    HIGO_PF_BUTT
}HIGO_PF_E;


typedef enum hiGoLayerIdE
{
    HI_GO_LAYER_HD_0         = 0,
    HI_GO_LAYER_HD_1         = 1,
    HI_GO_LAYER_HD_2         = 2,
    HI_GO_LAYER_HD_3,
    HI_GO_LAYER_SD_0,
    HI_GO_LAYER_SD_1,
    HI_GO_LAYER_ID_BUTT
}HI_GO_LAYER_ID_E;

typedef struct hiGoRectS
{
    HI_U32 x;
    HI_U32 y;
    HI_U32 w;
    HI_U32 h;
}HI_GO_RECT_S;

typedef struct hiGoSurfaceS
{
    HI_U32   u32Width[CONFIG_GFX_PIXELDATA_NUM];
    HI_U32   u32Height[CONFIG_GFX_PIXELDATA_NUM];
    HI_U32   u32Stride[CONFIG_GFX_PIXELDATA_NUM];
    HI_U32   u32Size[CONFIG_GFX_PIXELDATA_NUM];
    HI_CHAR* pPhyAddr[CONFIG_GFX_PIXELDATA_NUM];
    HI_CHAR* pVirAddr[CONFIG_GFX_PIXELDATA_NUM];
    HI_GO_RECT_S stRect;
    HI_GO_LAYER_ID_E  enLayerID;
    HIGO_PF_E         enPixelFormat;
}HI_GO_SURFACE_S;


typedef struct tagHIGOBLTOPT_S
{
   HI_BOOL EnableScale;
}HIGO_BLTOPT_S;


typedef struct
{
    HI_VOID*        pPhyData;
    HI_U32          Pitch;
    HI_U32          Bpp;
} HIGO_PIXELDATA_S;


/********************** Global Variable declaration **************************/

typedef HI_U32             HI_COLOR;
typedef HI_GO_RECT_S       HI_RECT;
typedef HIGO_PIXELDATA_S   HI_PIXELDATA[CONFIG_GFX_PIXELDATA_NUM];

/******************************* API declaration *****************************/

/*****************************************************************************
* func            : HI_GFX_LOGO_RESERVE_Alloc
* description     : 分配保留内存
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_RESERVE_Alloc(HI_U32 u32Size,HI_CHAR* pModuleName);

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Alloc
* description     : 分配连续的物理地址，boot下连续地址也可以做虚拟地址，可直接读写,
                    这里使用保留内存
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_MMZ_Alloc(HI_U32 u32Size,HI_CHAR* pModuleName);

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Free
* description     : 释放地址
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_LOGO_MMZ_Free(HI_CHAR* pPhyAddr);

/*****************************************************************************
* func            : HI_GFX_LOGO_Malloc
* description     : 使用malloc分配内存，使用完释放给其它功能使用,小内存
                    使用该接口比如handle等，减少内存碎片
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_CHAR* HI_GFX_LOGO_Malloc(HI_U32 u32Size,HI_CHAR* pModuleName);

/*****************************************************************************
* func            : HI_GFX_LOGO_MMZ_Free
* description     : 释放地址
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
HI_VOID HI_GFX_LOGO_Free(HI_CHAR* pPhyAddr);

#endif /*__HI_GO_COMMON_H__ */
