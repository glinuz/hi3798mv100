#ifndef __DRV_VI_BUF_H__
#define __DRV_VI_BUF_H__

#include <linux/list.h>
#include "hi_drv_mmz.h"
#include "hi_mpi_vi.h"
#include "hi_unf_vi.h"
#include "hi_drv_sys.h"

/* buffer descriptor */
typedef struct hiVI_FB_S
{
    struct list_head list;
    HI_U32  u32Index;
    HI_BOOL bUsed;
    HI_U32  u32PhysAddr;
    HI_U32  u32Pts;
} VI_FB_S;

typedef struct hiVI_FB_ROOT_S
{
    MMZ_BUFFER_S MMZBuf;
    HI_U32       u32BufCnt;
    VI_FB_S      struFb[MAX_VI_FB_NUM];
    struct list_head free_list;
    struct list_head busy_list;
    struct list_head done_list;
} VI_FB_ROOT_S;

typedef enum hiVI_FB_BUF_MODE
{
    VI_FB_MODE_ALLOC,
    VI_FB_MODE_MMAP,
    VI_FB_MODE_VIRTUAL,
} VI_FB_BUF_MODE;

typedef enum hiVI_FB_BUF_STATE
{
    VI_FB_STATE_FREE = 0,
    VI_FB_STATE_BUSY,
    VI_FB_STATE_DONE,
} VI_FB_BUF_STATE;

typedef struct hiVI_FB_BUF_PROC
{
    VI_FB_BUF_STATE stState[MAX_VI_FB_NUM];
    HI_U32          u32UsedNum;
} VI_FB_BUF_PROC;

typedef struct hiVI_FB_ADDR_S
{
    VI_FB_BUF_MODE enBufMode;
    HI_U32         u32BufSize;
    HI_U32         u32PhyAddr[MAX_VI_EXT_BUF_NUM];
    HI_U32         u32BufNum;
} VI_FB_ATTR_S;

HI_S32      VI_DRV_BufInit(VI_FB_ROOT_S *pRoot, VI_FB_ATTR_S *pstBufAttr);
HI_VOID     VI_DRV_BufDeInit(VI_FB_ROOT_S *pRoot, VI_FB_ATTR_S *pstBufAttr);
HI_VOID     VI_DRV_BufReset(VI_FB_ROOT_S *pRoot, VI_FB_ATTR_S *pstBufAttr);
HI_BOOL     VI_DRV_FreeList_IsEmpty(VI_FB_ROOT_S *pRoot);
HI_S32      VI_DRV_BufGet(VI_FB_ROOT_S *pRoot, VI_FB_S *pFb, HI_BOOL bVirtual);
HI_S32      VI_DRV_BufPut(VI_FB_ROOT_S *pRoot, HI_U32 u32PhyAddr);
HI_S32      VI_DRV_BufAdd(VI_FB_ROOT_S *pRoot, VI_FB_S *pFb);
HI_S32      VI_DRV_BufSub(VI_FB_ROOT_S *pRoot, HI_U32 u32PhyAddr);
HI_VOID     VI_DRV_BufProc(VI_FB_ROOT_S *pRoot, VI_FB_BUF_PROC *pstBufProc);

#endif //__DRV_VI_BUF_H__
