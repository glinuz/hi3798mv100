/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_proc.h
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/14
Description    : PNG proc info head file
Function List    :

History           :
Date                Author                Modification
2010/10/14        z00141204        Created file
******************************************************************************/

#ifndef __PNG_PROC_H__
#define __PNG_PROC_H__

#include <linux/seq_file.h>
#include "hi_type.h"
#include "hi_png_type.h"
#include "hi_png_config.h"

#ifndef CONFIG_PNG_PROC_DISABLE

/* proc info struct */
typedef struct tag_PNG_PROC_INFO_S
{
    HI_U32 u32Width;    /* image width */
    HI_U32 u32Height;   /* image height */
    HI_PNG_IMAGEFMT_E eColorFmt;    /* image format */
    HI_U8 u8BitDepth;   /* bit depth */
    HI_BOOL bSync;      /* decode if synchronization*/
    HI_PNG_STATE_E eState;  /* decode state */
    HI_U32 u32FlterPhyaddr; /* filter buffer physical address */
    HI_U32 u32Size;         /* filter buffer size */
    HI_U32 u32StreamBufPhyaddr; /* code stream buffer physical address */
    HI_U32 u32ImagePhyaddr; /* target physical address */
    HI_U32 u32Stride;       /* target stride */
    HI_U32 u32Transform;    /* transform information */
    HI_U16 u16TrnsColorRed;
    HI_U16 u16TrnsColorGreen;
    HI_U16 u16TrnsColorBlue;
    HI_U16 u16Filler;
}PNG_PROC_INFO_S;

HI_VOID PNG_ProcInit(HI_VOID);
HI_VOID PNG_ProcCleanup(HI_VOID);
HI_BOOL PNG_IsProcOn(HI_VOID);
HI_VOID PNG_GetProcStruct(PNG_PROC_INFO_S **ppstProcInfo);
#endif

#endif /* __PNG_PROC_H__ */
