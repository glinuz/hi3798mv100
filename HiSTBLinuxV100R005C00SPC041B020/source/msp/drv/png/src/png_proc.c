/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_proc.c
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/14
Description    : png proc infomation
Function List     :

History           :
Date                Author                Modification
2010/10/14        z00141204        Created file
******************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>        /* everything... */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

#include "hi_drv_proc.h"

#include "png_proc.h"
#include "png_define.h"
#include "hi_gfx_sys_k.h"

#ifndef CONFIG_PNG_PROC_DISABLE

PNG_PROC_INFO_S s_stPngProcInfo = {0};

static HI_BOOL s_bPngProcOn = HI_FALSE;


static char *s_decstate[HI_PNG_STATE_BUTT + 1] =
{
    "Nostart",
    "Decoding",
    "Finish",
    "Err",
    "Unknown"
};

HI_S32 PNG_Read_Proc(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *item = NULL;
    PNG_PROC_INFO_S *procinfo = NULL;

    if ((NULL == p) || (NULL == p->private))
    {
       return HI_FAILURE;
    }

    item = (DRV_PROC_ITEM_S *)(p->private);
    if (NULL == item)
    {
       return HI_FAILURE;
    }

    procinfo = (PNG_PROC_INFO_S *)(item->data);

    PROC_PRINT(p, "width\t\t:%u\n", procinfo->u32Width);
    PROC_PRINT(p, "height\t\t:%u\n", procinfo->u32Height);

    switch(procinfo->eColorFmt)
    {
        case HI_PNG_IMAGEFMT_AGRAY:
            PROC_PRINT(p, "fmt\t\t:AGray\n");
            break;
        case HI_PNG_IMAGEFMT_ARGB:
            PROC_PRINT(p, "fmt\t\t:ARGB\n");
            break;
        case HI_PNG_IMAGEFMT_CLUT:
            PROC_PRINT(p, "fmt\t\t:Clut\n");
            break;
        case HI_PNG_IMAGEFMT_GRAY:
            PROC_PRINT(p, "fmt\t\t:Gray\n");
            break;
        case HI_PNG_IMAGEFMT_RGB:
            PROC_PRINT(p, "fmt\t\t:RGB\n");
            break;
        default:
            PROC_PRINT(p, "fmt\t\t:Unknown\n");
            break;
    }

    PROC_PRINT(p, "bitdepth\t:%d\n", procinfo->u8BitDepth);
    PROC_PRINT(p, "transform\t:0x%x\n", procinfo->u32Transform);
    PROC_PRINT(p, "sync\t\t:%s\n", procinfo->bSync?("YES"):("NO"));
    PROC_PRINT(p, "filter buf addr\t:0x%x\n", procinfo->u32FlterPhyaddr);
    PROC_PRINT(p, "filter buf size\t:0x%x\n", procinfo->u32Size);
    PROC_PRINT(p, "stream buf addr\t:0x%x\n", procinfo->u32StreamBufPhyaddr);
    PROC_PRINT(p, "dst addr\t:0x%x\n", procinfo->u32ImagePhyaddr);
    PROC_PRINT(p, "dst stride\t:0x%x\n", procinfo->u32Stride);
    PROC_PRINT(p, "transcolor\t:0x%x%x%x\n", procinfo->u16TrnsColorRed, procinfo->u16TrnsColorGreen, procinfo->u16TrnsColorBlue);
    PROC_PRINT(p, "filler\t\t:0x%x\n", procinfo->u16Filler);
    if(procinfo->eState<(HI_PNG_STATE_BUTT + 1))
    {
        PROC_PRINT(p, "state\t\t:%s\n", s_decstate[procinfo->eState]);
    }
    return HI_SUCCESS;
}

HI_S32 PNG_Write_Proc(struct file * file,
    const char __user * pBuf, size_t count, loff_t *ppos)
{
    UNUSED(file);
    UNUSED(pBuf);
    UNUSED(count);
    UNUSED(ppos);

    return 0;
}


HI_VOID PNG_ProcInit(HI_VOID)
{
    GFX_PROC_ITEM_S pProcItem = {PNG_Read_Proc,PNG_Write_Proc,NULL};
    HI_GFX_PROC_AddModule("png", &pProcItem, &s_stPngProcInfo);
    return;
}

HI_VOID PNG_ProcCleanup(HI_VOID)
{
    HI_GFX_PROC_RemoveModule("png");
    return;
}

HI_BOOL PNG_IsProcOn(HI_VOID)
{
    return s_bPngProcOn;
}

HI_VOID PNG_GetProcStruct(PNG_PROC_INFO_S **ppstProcInfo)
{
    *ppstProcInfo = &s_stPngProcInfo;

    return;
}

#endif
