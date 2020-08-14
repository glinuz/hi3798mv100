/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    task.h
 *
 * Purpose: omxvdec task interface
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#ifndef __OMXVDEC_MEM_H__
#define __OMXVDEC_MEM_H__

#include "hi_drv_mmz.h"

typedef struct
{
    HI_U8* pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
} OMXVDEC_BUFFER_S;


HI_S32 HI_DRV_OMXVDEC_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf);
HI_VOID HI_DRV_OMXVDEC_UnmapAndRelease(MMZ_BUFFER_S *psMBuf);
HI_S32 HI_DRV_OMXVDEC_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf, HI_BOOL is_sec);
HI_VOID HI_DRV_OMXVDEC_Release(MMZ_BUFFER_S *psMBuf, HI_BOOL is_sec);
HI_S32 HI_DRV_OMXVDEC_Map(MMZ_BUFFER_S *psMBuf);
HI_VOID HI_DRV_OMXVDEC_Unmap(MMZ_BUFFER_S *psMBuf);
HI_S32 HI_DRV_OMXVDEC_MapCache(MMZ_BUFFER_S *psMBuf);

#endif

