/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_os.c
* Description:
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-08-02              NULL         Create this file.
*******************************************************************************/
#include "gfx2d_config.h"
#include "hi_gfx_comm_k.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#if HWC_VERSION >= 0x500
#define GFX2D_CONFIG_NODE_SIZE (1536)
#else
#define GFX2D_CONFIG_NODE_SIZE (1024)
#endif

#ifdef CFG_HI_GFX2D_MEM_SIZE
#define GFX2D_CONFIG_DEFAULT_MEMSIZE (CFG_HI_GFX2D_MEM_SIZE)
#else
#define GFX2D_CONFIG_DEFAULT_MEMSIZE (60 * GFX2D_CONFIG_NODE_SIZE)
#endif

/*Mem size(KByte)*/
static HI_U32 gs_u32MemSize = GFX2D_CONFIG_DEFAULT_MEMSIZE;

#ifndef GFX2D_PROC_UNSUPPORT
static HI_U32 gs_u32StartTimeMs = 0;
static HI_U32 gs_u32StartTimeUs = 0;
static HI_U32 gs_u32EndTimeMs = 0;
static HI_U32 gs_u32EndTimeUs = 0;
#endif

HI_S32 GFX2D_CONFIG_SetMemSize(HI_U32 u32MemSize)
{
    if (0 == u32MemSize)
    {
        u32MemSize = GFX2D_CONFIG_DEFAULT_MEMSIZE;
    }
    else if (u32MemSize < GFX2D_CONFIG_NODE_SIZE)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Mem not enough to work!\n");
        return HI_FAILURE;
    }

    gs_u32MemSize = u32MemSize;

    return HI_SUCCESS;
}

HI_U32 GFX2D_CONFIG_GetMemSize(HI_VOID)
{
    return gs_u32MemSize;
}


HI_U32 GFX2D_CONFIG_GetNodeNum(HI_VOID)
{
    /*A node occupy 1K byte,so node num can be calculated by mem_size/1024.
    Node size is affected by logic mainly,but also the software node mangment structure.
    1K is not a accurate size of a node, that means a smaller size may also works.Why 1K?
    1.It obligate space for future change of node size,generate by logic design or software design.
    2.It is easy to explain and remember the relation between node num and mem size.*/
    return gs_u32MemSize / GFX2D_CONFIG_NODE_SIZE;
}

#ifndef GFX2D_PROC_UNSUPPORT
HI_VOID GFX2D_CONFIG_StartTime(HI_VOID)
{
    HI_GFX_GetTimeStamp(&gs_u32StartTimeMs, &gs_u32StartTimeUs);
}

HI_U32 GFX2D_CONFIG_EndTime(HI_VOID)
{
    HI_GFX_GetTimeStamp(&gs_u32EndTimeMs, &gs_u32EndTimeUs);

    if (gs_u32EndTimeMs < gs_u32StartTimeMs)
    {
        return 0;
    }

    return (gs_u32EndTimeMs - gs_u32StartTimeMs) * 1000 + (gs_u32EndTimeUs - gs_u32StartTimeUs);
}

HI_S32 GFX2D_CONFIG_WriteProc(struct file *file, const char __user *buf,
                              size_t count, loff_t *ppos)
{
    UNUSED(file);
    UNUSED(buf);
    UNUSED(count);
    UNUSED(ppos);
    return 0;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
