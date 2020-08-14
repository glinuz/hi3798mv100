/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_event.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/12/10
  Description   :
  History       :
  Date          : 2014/12/10
  Author        : t00273561
  Modification  :
*******************************************************************************/
#include <linux/delay.h>
#include "drv_hdmi_event.h"
#include "hdmi_platform.h"



#define EVENT_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("null pointer!\n");\
	return HI_FAILURE;}\
}while(0)

#define EVENT_INIT_CHK(pstEvtInfo) do{\
	EVENT_NULL_CHK(pstEvtInfo);\
	if(pstEvtInfo->bInit != HI_TRUE)\
	{HDMI_WARN("event didn't init!\n");\
	return HI_FAILURE;}\
}while(0)


static HI_S32 EventTypeCounter(HDMI_EVENT_POOL_S *pstPool,HDMI_EVENT_E enEvent,HI_BOOL bWrite)
{
	EVENT_NULL_CHK(pstPool);

	switch(enEvent)
	{
		case HDMI_EVENT_HOTPLUG:
			bWrite ? pstPool->stRunCnt.u32HPDWrCnt++ : pstPool->stRunCnt.u32HPDRdCnt++;
			break;
		case HDMI_EVENT_HOTUNPLUG:
			bWrite ? pstPool->stRunCnt.u32UnHPDWrCnt++ : pstPool->stRunCnt.u32UnHPDRdCnt++;
			break;

		case HDMI_EVENT_EDID_FAIL:
			bWrite ? pstPool->stRunCnt.u32EdidFailWrCnt++ : pstPool->stRunCnt.u32EdidFailRdCnt++;
			break;

		case HDMI_EVENT_HDCP_FAIL:
			bWrite ? pstPool->stRunCnt.u32HdcpFailWrCnt++ : pstPool->stRunCnt.u32HdcpFailRdCnt++;
			break;

		case HDMI_EVENT_HDCP_SUCCESS:
			bWrite ? pstPool->stRunCnt.u32HdcpSuccWrCnt++ : pstPool->stRunCnt.u32HdcpUserRdCnt++;
			break;

		case HDMI_EVENT_RSEN_CONNECT:
			bWrite ? pstPool->stRunCnt.u32RsenConWrCnt++ : pstPool->stRunCnt.u32RsenConRdCnt++;
			break;

		case HDMI_EVENT_RSEN_DISCONNECT:
			bWrite ? pstPool->stRunCnt.u32RsenDisWrCnt++ : pstPool->stRunCnt.u32RsenDisRdCnt++;
			break;

		case HDMI_EVENT_HDCP_USERSETTING:
			bWrite ? pstPool->stRunCnt.u32HdcpUserWrCnt++ : pstPool->stRunCnt.u32HdcpUserRdCnt++;
			break;

		case HDMI_EVENT_BUTT:
		default:
			break;
	}

	return HI_SUCCESS;
}

HI_S32 DRV_HDMI_EventInit(HDMI_EVENT_INFO_S *pstEvtInfo)
{
	EVENT_NULL_CHK(pstEvtInfo);

    if (pstEvtInfo->bInit != HI_TRUE)
    {
    	/* init */
        HDMI_INIT_WAIT_QUEUE(pstEvtInfo->wtQueue);
    	HDMI_INIT_MUTEX(pstEvtInfo->eventMutex);
        INIT_LIST_HEAD(&pstEvtInfo->stPoolList);
    	pstEvtInfo->u32Total 		= 0;
    	pstEvtInfo->bInit			= HI_TRUE;
    }

	return HI_SUCCESS;
}


HI_S32	DRV_HDMI_EventDeInit(HDMI_EVENT_INFO_S *pstEvtInfo)
{
	struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_INIT_CHK(pstEvtInfo);

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    /* event info may not init, so check it*/
    if (list_empty(&pstEvtInfo->stPoolList))
    {
       HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
       HDMI_WARN("event pool list is empty\n");
       return HI_SUCCESS;
    }

    /*before free event pool, wake up all process which is waiting for event */
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
        pstTmpPool->stCtrl.bWakeUpFlag = HI_TRUE;
    }

	HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

	wake_up(&pstEvtInfo->wtQueue);
	msleep(200);

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    /* if it is not empty,free the pool list */
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
        list_del(&pstTmpPool->stPoolNode);
        HDMI_KFREE(pstTmpPool);
        pstEvtInfo->u32Total--;
    }

	HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
	pstEvtInfo->bInit = HI_FALSE;

	return HI_SUCCESS;
}

HI_S32	DRV_HDMI_EventPoolMalloc(HDMI_EVENT_INFO_S *pstEvtInfo, HI_U32* pu32PoolID)
{
	HI_U32 				i = 0;
	HDMI_EVENT_POOL_S	*pstNewPool = HI_NULL;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;
	struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
	HI_U32				u32CurGid = 0;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_INIT_CHK(pstEvtInfo);

	u32CurGid = (HI_U32)get_current()->tgid;

	/*check ID exist*/
	if (!list_empty(&pstEvtInfo->stPoolList))
	{
		list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
		{
			pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
			if (pstTmpPool->stCtrl.u32PoolID == u32CurGid)
			{
				HDMI_WARN("Proc ID=%u  exist!\n",u32CurGid);
				return HDMI_EVENT_ID_EXIST;
			}
		}
	}

	/* malloc a new Pool element & init */
	pstNewPool = (HDMI_EVENT_POOL_S *)HDMI_KMALLOC(sizeof(HDMI_EVENT_POOL_S));
	if (HI_NULL == pstNewPool)
	{
		HDMI_WARN("malloc event pool fail!\n");
		return HI_FAILURE;
	}

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

	HDMI_MEMSET(&pstNewPool->stCtrl , 0 , sizeof(HDMI_EVENT_RUN_CTRL_S));
	HDMI_MEMSET(&pstNewPool->stRunCnt, 0 , sizeof(HDMI_EVENT_RUN_CNT_S));

	pstNewPool->stCtrl.u32PoolID = u32CurGid;
	if (pu32PoolID != HI_NULL)
    {
    	*pu32PoolID = pstNewPool->stCtrl.u32PoolID;
    }

	pstNewPool->stCtrl.bWakeUpFlag = HI_FALSE;

	for (i = 0; i < HDMI_EVENT_POOL_SIZE; i++)
	{
		pstNewPool->stCtrl.enEventPool[i] = HDMI_EVENT_BUTT;
	}

	pstEvtInfo->u32Total++;

	/* new pool element insert the list tail */
    list_add_tail(&pstNewPool->stPoolNode, &pstEvtInfo->stPoolList);

	HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

	return HI_SUCCESS;
}


HI_S32	DRV_HDMI_EventPoolFree(HDMI_EVENT_INFO_S *pstEvtInfo,HI_U32 u32PoolID)
{
    struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
    HI_BOOL 			bFindOut = HI_FALSE;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_INIT_CHK(pstEvtInfo);

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    if (list_empty(&pstEvtInfo->stPoolList))
    {
       HDMI_WARN("event pool list is empty\n");
       HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
       return HI_SUCCESS;
    }

    /*find a match ID and free it*/
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
        if (pstTmpPool->stCtrl.u32PoolID == u32PoolID)
        {
            list_del(&pstTmpPool->stPoolNode);
            pstEvtInfo->u32Total--;
            pstTmpPool->stCtrl.bWakeUpFlag = HI_TRUE;
            wake_up(&pstEvtInfo->wtQueue);
            msleep(200);
    	    HDMI_KFREE(pstTmpPool);
            bFindOut = HI_TRUE;
            HDMI_INFO("Delete proc(%d) node from event pool\n",u32PoolID);
            break;
        }
    }

    /* can't find a match ID*/
    if (!bFindOut)
    {
        HDMI_ERR("Event pool free fail,Not find a match proc id(%d) \n", u32PoolID);
        HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
        return HI_FAILURE;
    }

    HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

    return HI_SUCCESS;
}

HI_S32	DRV_HDMI_EventPoolWrite(HDMI_EVENT_INFO_S *pstEvtInfo,
											HDMI_EVENT_E enEvent)
{
    struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_INIT_CHK(pstEvtInfo);

	if ((enEvent < HDMI_EVENT_HOTPLUG) ||
		(enEvent > HDMI_EVENT_HDCP_USERSETTING))
	{
		HDMI_WARN("The event(0x%x) invalid ,we don't write!\n", enEvent);
		return HI_FAILURE;
	}

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    if (list_empty(&pstEvtInfo->stPoolList))
    {
       HDMI_WARN("event pool list is empty\n");
       HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
       return HI_SUCCESS;
    }

    /* write event into all event pool in the list*/
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);

        /* The event pool is overflow*/
        if (  (pstTmpPool->stCtrl.enEventPool[pstTmpPool->stCtrl.u32WritePtr] >= HDMI_EVENT_HOTPLUG)
			&&(pstTmpPool->stCtrl.enEventPool[pstTmpPool->stCtrl.u32WritePtr] <= HDMI_EVENT_HDCP_USERSETTING)  )
        {
            pstTmpPool->stRunCnt.u32ErrWdCnt++;
			pstTmpPool->stCtrl.u32RdAbleCnt--;
			pstTmpPool->stCtrl.u32ReadPtr = ( pstTmpPool->stCtrl.u32WritePtr + 1 ) % HDMI_EVENT_POOL_SIZE;
            HDMI_WARN("The event pool of proc(%d) is overflow\n",pstTmpPool->stCtrl.u32PoolID);
        }

        pstTmpPool->stCtrl.enEventPool[pstTmpPool->stCtrl.u32WritePtr++] = enEvent;
        pstTmpPool->stCtrl.u32WritePtr %= HDMI_EVENT_POOL_SIZE;
		EventTypeCounter(pstTmpPool,enEvent,HI_TRUE);
		pstTmpPool->stCtrl.u32RdAbleCnt++;
        wake_up(&pstEvtInfo->wtQueue);
        HDMI_INFO("The event(0x%x) is writed into event pool of proc(%d) success\n", enEvent, pstTmpPool->stCtrl.u32PoolID);
    }

    HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

	return HI_SUCCESS;
}



HI_S32	DRV_HDMI_EventPoolRead(HDMI_EVENT_INFO_S *pstEvtInfo,
											HI_U32 u32PoolID,
											HDMI_EVENT_E *penEvent)
{
    HI_S32 				s32Ret = HI_FAILURE;
	HI_BOOL 			bFindOut = HI_FALSE;
    struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_NULL_CHK(penEvent);
	EVENT_INIT_CHK(pstEvtInfo);

    *penEvent = HDMI_EVENT_BUTT;

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    if (list_empty(&pstEvtInfo->stPoolList))
    {
       HDMI_WARN("event pool list is empty\n");
       HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
       return HI_SUCCESS;
    }

    /*find a match proc*/
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
        if (pstTmpPool->stCtrl.u32PoolID == u32PoolID)
        {
            bFindOut = HI_TRUE;
            break;
        }
    }

    HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);


    if (bFindOut)
    {
        EVENT_NULL_CHK(pstTmpPool);
		s32Ret = wait_event_interruptible(pstEvtInfo->wtQueue, (pstTmpPool->stCtrl.bWakeUpFlag || pstTmpPool->stCtrl.u32RdAbleCnt > 0));

		if(0 == pstTmpPool->stCtrl.u32RdAbleCnt && s32Ret <= 0)
        {
        	/* when not free pool */
            if (!pstTmpPool->stCtrl.bWakeUpFlag)
            {
               HDMI_INFO("The event pool of  proc(%d) is null\n", u32PoolID);
            }
            return HI_FAILURE;
        }
    }
    else
    {
        HDMI_ERR("The proc(%d) is not find in event pool list\n",u32PoolID);
        msleep(500);
        return HI_FAILURE;
    }

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

    if (pstTmpPool->stCtrl.u32RdAbleCnt > 0)
    {
		*penEvent = pstTmpPool->stCtrl.enEventPool[pstTmpPool->stCtrl.u32ReadPtr];

		pstTmpPool->stCtrl.enEventPool[pstTmpPool->stCtrl.u32ReadPtr] = HDMI_EVENT_BUTT;
		pstTmpPool->stCtrl.u32ReadPtr = ( pstTmpPool->stCtrl.u32ReadPtr + 1 ) % HDMI_EVENT_POOL_SIZE;
    	pstTmpPool->stCtrl.bWakeUpFlag = HI_FALSE;

		if (   (*penEvent >= HDMI_EVENT_HOTPLUG)
			&& (*penEvent <= HDMI_EVENT_HDCP_USERSETTING) 	)
		{
	    	pstTmpPool->stCtrl.u32RdAbleCnt--;
	    	EventTypeCounter(pstTmpPool,*penEvent,HI_FALSE);
			s32Ret = HI_SUCCESS;
			HDMI_INFO("The proc(%d) poll event(0x%x) success\n",pstTmpPool->stCtrl.u32PoolID, *penEvent);
		}
		else
		{
			pstTmpPool->stRunCnt.u32ErrRdCnt++;
			s32Ret = HI_FAILURE;
		}

    }
	else
	{
		s32Ret = HI_FAILURE;
	}

	HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

	return s32Ret;
}

HI_S32	DRV_HDMI_EventPoolStatusGet(HDMI_EVENT_INFO_S *pstEvtInfo,
												  HI_U32 u32PoolNum,
												  HDMI_EVENT_RUN_CTRL_S *pstCtrl,
												  HDMI_EVENT_RUN_CNT_S *pstCnt)
{
	HI_U32				i = 0;
    struct list_head	*n = HI_NULL;
    struct list_head	*pos = HI_NULL;
	HDMI_EVENT_POOL_S	*pstTmpPool = HI_NULL;

	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_NULL_CHK(pstCtrl);
	EVENT_NULL_CHK(pstCnt);
	EVENT_INIT_CHK(pstEvtInfo);

	HDMI_MEMSET(pstCtrl,0,sizeof(HDMI_EVENT_RUN_CTRL_S));
	HDMI_MEMSET(pstCnt,0,sizeof(HDMI_EVENT_RUN_CNT_S));

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

	if (   (u32PoolNum > pstEvtInfo->u32Total)
		|| (u32PoolNum == 0) )
	{
		HDMI_WARN("The input pool num(%d) is wrong,event pool range is [1~%d]\n", u32PoolNum, pstEvtInfo->u32Total);
		HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
		return HI_FAILURE;
	}

    if (list_empty(&pstEvtInfo->stPoolList))
    {
       HDMI_WARN("event pool list is empty\n");
       HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
       return HI_SUCCESS;
    }

    /*find a match ID and copy status*/
    list_for_each_safe(pos, n, &pstEvtInfo->stPoolList)
    {
        pstTmpPool = list_entry(pos, HDMI_EVENT_POOL_S, stPoolNode);
        i++;
        if (i == u32PoolNum)
        {
            HDMI_MEMCPY(pstCtrl, &pstTmpPool->stCtrl, sizeof(HDMI_EVENT_RUN_CTRL_S));
			HDMI_MEMCPY(pstCnt, &pstTmpPool->stRunCnt, sizeof(HDMI_EVENT_RUN_CNT_S));
        }
    }

    HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);
	return HI_SUCCESS;
}



HI_S32	DRV_HDMI_EventPoolTotalGet(HDMI_EVENT_INFO_S *pstEvtInfo,
												HI_U32 *pu32Total)
{
	EVENT_NULL_CHK(pstEvtInfo);
	EVENT_NULL_CHK(pu32Total);
	EVENT_INIT_CHK(pstEvtInfo);

    HDMI_MUTEX_LOCK(pstEvtInfo->eventMutex);

	*pu32Total = pstEvtInfo->u32Total;

	HDMI_MUTEX_UNLOCK(pstEvtInfo->eventMutex);

	return HI_SUCCESS;
}



