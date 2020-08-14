#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>

#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>


#include "drv_sync.h"
#include "drv_disp_osal.h"
#include "hi_osal.h"

#define FENCE_DBG 0

#ifdef HI_VO_WIN_SYNC_SUPPORT
HI_S32 DRV_SYNC_Init(WIN_SYNC_INFO_S *pstSyncInfo,HI_U32 u32Index)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    HI_U32 u32Cnt;
    HI_U8  u8DefaultName[12];
    HI_U8  u8TimelineName[12];

    if (HI_NULL == pstSyncInfo)
    {
        WIN_ERROR("Invalid pstSyncInfo\n");
        return HI_FAILURE;
    }

    pstSyncInfo->u32RefreshCnt = 0;

    pstSyncInfo->u32LastRefreshIdx = 0xffffffff;

    pstSyncInfo->u32FenceCnt = 0;

    pstSyncInfo->u32NextFenceValue = 1;

    pstSyncInfo->u32Timeline = 0;

	pstSyncInfo->pstTimeline = HI_NULL;

    HI_OSAL_Snprintf(u8TimelineName, 12, "vo%04x", u32Index);

    pstSyncInfo->pstTimeline = sw_sync_timeline_create(u8TimelineName);
    if (pstSyncInfo->pstTimeline == HI_NULL)
    {
        WIN_ERROR("Create Sync TimeLine Failed\n");
		return HI_FAILURE;
    }

    HI_OSAL_Snprintf(u8DefaultName, 12, "win%04x", u32Index);

    memcpy(pstSyncInfo->u8FenceName,u8DefaultName,sizeof(HI_U8)*12);

    for(u32Cnt = 0; u32Cnt < DEF_FENCE_ARRAY_LENTH; u32Cnt ++)
    {
        pstSyncInfo->u32FenceArray[u32Cnt][0] = DEF_FENCE_INVALID_INDEX;
        pstSyncInfo->u32FenceArray[u32Cnt][1] = DEF_FENCE_INVALID_INDEX;
    }

    pstSyncInfo->u32ArrayHead = 0;
    pstSyncInfo->u32ArrayTail = 0;

#endif
    return HI_SUCCESS;
}

HI_S32 DRV_SYNC_DeInit(WIN_SYNC_INFO_S *pstSyncInfo)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
    if (pstSyncInfo->pstTimeline != HI_NULL)
        sync_timeline_destroy((struct sync_timeline*)pstSyncInfo->pstTimeline);

	pstSyncInfo->pstTimeline = HI_NULL;
#endif

	return HI_SUCCESS;
}


HI_VOID  DRV_SYNC_AddData(WIN_SYNC_INFO_S *pstSyncInfo, HI_U32 u32NewIndex,HI_U32 u32NewAddr)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
	struct sw_sync_timeline *timeline;

	HI_U32 u32Head;
	HI_U32 u32Tail;

	HI_U32 u32PtValue;

	timeline = pstSyncInfo->pstTimeline;

	u32PtValue = pstSyncInfo->u32NextFenceValue;

	if (timeline == NULL) {
		//return -EINVAL;
		return ;
	}

	u32Head = pstSyncInfo->u32ArrayHead;
	u32Tail = pstSyncInfo->u32ArrayTail;

	if ((u32Tail+1)%DEF_FENCE_ARRAY_LENTH == u32Head)
	{
		WIN_ERROR("FenceArray is Full\n");
		//return -EINVAL;
		return ;
	}

	//add idx and pt_value into array
	pstSyncInfo->u32FenceArray[u32Tail][0] = u32NewIndex;
	pstSyncInfo->u32FenceArray[u32Tail][1] = pstSyncInfo->u32NextFenceValue;

#if FENCE_DBG
    WIN_ERROR("Create %d idx %d addr %#x\n",pstSyncInfo->u32NextFenceValue,u32NewIndex,u32NewAddr);
#endif

	//update related cnt
	pstSyncInfo->u32NextFenceValue++;
	pstSyncInfo->u32FenceCnt++;

	pstSyncInfo->u32ArrayTail = (u32Tail+1) % DEF_FENCE_ARRAY_LENTH;
#endif
	return ;
}
WIN_FENCE_FD DRV_SYNC_CreateFence(WIN_SYNC_INFO_S *pstSyncInfo, HI_U32 u32NewIndex,HI_U32 u32NewAddr)
{
	int fd = 0;
#ifdef HI_VO_WIN_SYNC_SUPPORT

	struct sync_fence *fence;
	struct sync_pt *pt;
	struct sw_sync_timeline *timeline;

	HI_U32 u32Head;
	HI_U32 u32Tail;

	HI_U32 u32PtValue;

	timeline = pstSyncInfo->pstTimeline;

	u32PtValue = pstSyncInfo->u32NextFenceValue;

	if (timeline == NULL) {
		return -EINVAL;
	}

	u32Head = pstSyncInfo->u32ArrayHead;
	u32Tail = pstSyncInfo->u32ArrayTail;

	if ((u32Tail+1)%DEF_FENCE_ARRAY_LENTH == u32Head)
	{
		WIN_ERROR("FenceArray is Full\n");
		return -EINVAL;
	}

	//create fence
	fd = get_unused_fd();
	if (fd < 0) {
		WIN_ERROR("get_unused_fd failed!\n");
		return fd;
	}

	pt = sw_sync_pt_create(timeline, u32PtValue);
	if (pt == NULL) {
		return -ENOMEM;
	}

	fence = sync_fence_create(pstSyncInfo->u8FenceName, pt);
	if (fence == NULL) {
		sync_pt_free(pt);
		return -ENOMEM;
	}

	sync_fence_install(fence, fd);

	//add idx and pt_value into array
	pstSyncInfo->u32FenceArray[u32Tail][0] = u32NewIndex;
	pstSyncInfo->u32FenceArray[u32Tail][1] = pstSyncInfo->u32NextFenceValue;

#if FENCE_DBG
    WIN_ERROR("Create %d idx %d addr %#x\n",pstSyncInfo->u32NextFenceValue,u32NewIndex,u32NewAddr);
#endif

	//update related cnt
	pstSyncInfo->u32NextFenceValue++;
	pstSyncInfo->u32FenceCnt++;

	pstSyncInfo->u32ArrayTail = (u32Tail+1) % DEF_FENCE_ARRAY_LENTH;
#endif
	return fd;
}


/*
SD : 1 1 2 2 3 3 4 4

HD :   1   2   3   4

when SD config the 2 twice,if release 1 ,the HD is still using it.

to solve this problem,calculate the repeat times.

when repeat 3 times ,release last frame.
*/
HI_U32 u32VoFenceRepeatCnt = 0;

HI_S32 DRV_SYNC_Signal(WIN_SYNC_INFO_S *pstSyncInfo,HI_U32 u32RefreshIndex,HI_U32 u32RefreshAddr)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
	struct sw_sync_timeline *pstTimeline;

	HI_U32 u32SearchHead;
	HI_U32 u32SearchTail;
	HI_U32 u32SearchCnt;
	HI_U32 u32SignalLine = 0;

	pstTimeline = pstSyncInfo->pstTimeline;

	if (pstTimeline == HI_NULL)
	{
		WIN_ERROR("Invalid Time line\n");
		return -ENOMEM;
	}

	pstSyncInfo->u32RefreshCnt ++;

	u32SearchHead = pstSyncInfo->u32ArrayHead;
	u32SearchTail = pstSyncInfo->u32ArrayTail;
	u32SearchCnt = u32SearchHead;

#if FENCE_DBG
	WIN_ERROR("signal %d addr %#x \n", u32RefreshIndex, u32RefreshAddr);
#endif

    if (u32RefreshIndex != pstSyncInfo->u32LastRefreshIdx)
	{
		u32SignalLine = u32RefreshIndex - 1;

		u32VoFenceRepeatCnt = 0;
	}
	else
	{
		u32VoFenceRepeatCnt++;

		if (u32VoFenceRepeatCnt >= 3)
		{
			u32SignalLine = u32RefreshIndex;
		}
		else
		{
			u32SignalLine = u32RefreshIndex - 1;
		}
	}

	while(u32SearchHead != u32SearchTail
			&&
		pstSyncInfo->u32FenceArray[u32SearchCnt][0] < u32SignalLine
            &&
        pstSyncInfo->u32FenceArray[u32SearchCnt][0] != pstSyncInfo->u32LastRefreshIdx
        )
	{
		sw_sync_timeline_inc(pstSyncInfo->pstTimeline, 1);

		pstSyncInfo->u32Timeline ++;

#if FENCE_DBG
		WIN_ERROR("time line %d\n",pstSyncInfo->u32Timeline);
#endif

		u32SearchHead = (u32SearchHead + 1)%DEF_FENCE_ARRAY_LENTH;
        u32SearchCnt = u32SearchHead;

		pstSyncInfo->u32ArrayHead = u32SearchHead;
	}

	pstSyncInfo->u32LastRefreshIdx = u32RefreshIndex;
#endif


	return HI_SUCCESS;
}


HI_S32 DRV_SYNC_Flush(WIN_SYNC_INFO_S *pstSyncInfo)
{
#ifdef HI_VO_WIN_SYNC_SUPPORT
	struct sw_sync_timeline *pstTimeline;

	HI_U32 u32SearchHead;
	HI_U32 u32SearchTail;
	HI_U32 u32SearchCnt;

    if (HI_NULL == pstSyncInfo)
    {
        WIN_ERROR("Invalid pstSyncInfo\n");
        return HI_FAILURE;
    }

	pstTimeline = pstSyncInfo->pstTimeline;

	if (pstTimeline == HI_NULL)
	{
		WIN_ERROR("Invalid Time line\n");
		return -ENOMEM;
	}

	pstSyncInfo->u32RefreshCnt ++;

	u32SearchHead = pstSyncInfo->u32ArrayHead;
	u32SearchTail = pstSyncInfo->u32ArrayTail;
	u32SearchCnt = u32SearchHead;

#if FENCE_DBG
	WIN_ERROR("head %d tail %d\n",u32SearchHead,u32SearchTail);
#endif

	while(u32SearchHead != u32SearchTail)
	{
		sw_sync_timeline_inc(pstSyncInfo->pstTimeline, 1);

		pstSyncInfo->u32Timeline ++;

#if FENCE_DBG
WIN_ERROR("time line %d\n",pstSyncInfo->u32Timeline);
#endif

		u32SearchHead = (u32SearchHead + 1)%DEF_FENCE_ARRAY_LENTH;
        u32SearchCnt = u32SearchHead;

		pstSyncInfo->u32ArrayHead = u32SearchHead;
	}
#endif

	return HI_SUCCESS;
}
#endif
