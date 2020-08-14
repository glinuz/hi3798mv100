#ifndef __DRV_SYNC_H__
#define __DRV_SYNC_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_video.h"

#ifdef HI_VO_WIN_SYNC_SUPPORT

#include "sw_sync.h"

#define WIN_FENCE_FD HI_U32

#define DEF_FENCE_ARRAY_LENTH 20

#define DEF_FENCE_INVALID_INDEX 0xffffffff;

typedef struct
{
	HI_U32	 u32RefreshCnt;// =Config times

    HI_U32   u32FenceCnt;// =QueueNewFrame times

    HI_U32   u32Timeline;// =Current Timeline

	HI_U32   u32NextFenceValue;// = u32FenceCnt + 1

	HI_U32   u32LastRefreshIdx;

    struct	sw_sync_timeline *pstTimeline;

	HI_U8	u8FenceName[12];

	HI_U32 u32ArrayHead;
	HI_U32 u32ArrayTail;

	HI_U32  u32FenceArray[DEF_FENCE_ARRAY_LENTH][2];
}WIN_SYNC_INFO_S;

HI_S32 DRV_SYNC_Init(WIN_SYNC_INFO_S *pstSyncInfo,HI_U32 u32Index);

HI_S32 DRV_SYNC_DeInit(WIN_SYNC_INFO_S *pstSyncInfo);

WIN_FENCE_FD DRV_SYNC_CreateFence(WIN_SYNC_INFO_S *pstSyncInfo, HI_U32 u32NewIndex,HI_U32 u32NewAddr);

HI_S32 DRV_SYNC_Signal(WIN_SYNC_INFO_S *pstSyncInfo,HI_U32 u32RefreshIndex,HI_U32 u32RefreshAddr);


HI_S32 DRV_SYNC_Flush(WIN_SYNC_INFO_S *pstSyncInfo);

HI_VOID  DRV_SYNC_AddData(WIN_SYNC_INFO_S *pstSyncInfo, HI_U32 u32NewIndex,HI_U32 u32NewAddr);
#endif

#endif









