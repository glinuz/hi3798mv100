/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_event.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/12/10
  Description   :
  History       :
  Date          : 2014/12/10
  Author        : t00273561
  Modification  :
*******************************************************************************/


#ifndef __DRV_HDMI_EVENT__
#define __DRV_HDMI_EVENT__

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/list.h>
#include "hi_type.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_platform.h"

#define HDMI_EVENT_POOL_SIZE	10
#define HDMI_EVENT_ID_EXIST	0xff

/* pool running status */
typedef struct
{
	HI_U32						u32HPDRdCnt;			/* [current] HDMI_EVENT_HOTPLUG event read count in the pool */
	HI_U32						u32HPDWrCnt;			/* [current] HDMI_EVENT_HOTPLUG event write count in the pool */
	HI_U32						u32UnHPDRdCnt;			/* [current] HDMI_EVENT_HOTUNPLUG event read count in the pool */
	HI_U32						u32UnHPDWrCnt;			/* [current] HDMI_EVENT_HOTUNPLUG event write count in the pool */
	HI_U32						u32EdidFailRdCnt;		/* [current] HDMI_EVENT_EDID_FAIL event read count in the pool */
	HI_U32						u32EdidFailWrCnt;		/* [current] HDMI_EVENT_EDID_FAIL event write count in the pool */	
	HI_U32						u32HdcpFailRdCnt;		/* [current] HDMI_EVENT_HDCP_FAIL event read count in the pool */
	HI_U32						u32HdcpFailWrCnt;		/* [current] HDMI_EVENT_HDCP_FAIL event write count in the pool */
	HI_U32						u32HdcpSuccRdCnt;		/* [current] HDMI_EVENT_HDCP_SUCCESS event read count in the pool */
	HI_U32						u32HdcpSuccWrCnt;		/* [current] HDMI_EVENT_HDCP_SUCCESS event write count in the pool */
	HI_U32						u32RsenConRdCnt;		/* [current] HDMI_EVENT_RSEN_CONNECT event read count in the pool */
	HI_U32						u32RsenConWrCnt;		/* [current] HDMI_EVENT_RSEN_CONNECT event write count in the pool */
	HI_U32						u32RsenDisRdCnt;		/* [current] HDMI_EVENT_RSEN_DISCONNECT event read count in the pool */
	HI_U32						u32RsenDisWrCnt;		/* [current] HDMI_EVENT_RSEN_DISCONNECT event write count in the pool */
	HI_U32						u32HdcpUserRdCnt;		/* [current] HDMI_EVENT_HDCP_USERSETTING event read count in the pool */
	HI_U32						u32HdcpUserWrCnt;		/* [current] HDMI_EVENT_HDCP_USERSETTING event write count in the pool */
	
	HI_U32					   	u32ErrRdCnt;			/* [current] read error event(s) total count in the pool */
	HI_U32					   	u32ErrWdCnt;			/* [current] write error event(s) total count in the pool */
	
}HDMI_EVENT_RUN_CNT_S;

/* pool ctrl status */
typedef struct
{
	HI_U32                     	u32PoolID;				/* Pool ID */	
	HI_BOOL		               	bWakeUpFlag;			/* wake up flag */
	HI_U32					   	u32RdAbleCnt;			/* [current] readable event(s) count in the pool */
	HI_U32                      u32ReadPtr;				/* read pointer */
    HI_U32                      u32WritePtr;			/* write pointer */
	
	HDMI_EVENT_E				enEventPool[HDMI_EVENT_POOL_SIZE]; /* event memory unit */

}HDMI_EVENT_RUN_CTRL_S;

/* event pool 
NOTE: a pool is a event buffer.When a new process(task) needs events ,
it should request a pool and we must malloc one to do service for it.All the active
pool make up a list.So,one such list  meet one HDMI DEVICE.*/
typedef struct	 hiHDMI_EVENT_POOL_S
{
	HDMI_EVENT_RUN_CNT_S		stRunCnt;

	HDMI_EVENT_RUN_CTRL_S		stCtrl;

    struct list_head           	stPoolNode; 
}HDMI_EVENT_POOL_S;

/* event manage  information description  struct */
typedef struct{
	HDMI_MUTEX				eventMutex;			/* lock to avoid competition*/
	HDMI_WAIT_QUEUE			wtQueue;			/* wait queue to wake up the sleeping process  */
	HI_BOOL					bInit;				/* is the struct init */
	HI_U32					u32Total;			/* total pools in the list */
    struct list_head        stPoolList;
}HDMI_EVENT_INFO_S;



/**
\brief	  to make sure  HDMI_EVENT_INFO_S in a init state 
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32	DRV_HDMI_EventInit(HDMI_EVENT_INFO_S *pstEvtInfo);

/**
\brief	to make sure  HDMI_EVENT_INFO_S releases all the using resource  
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\retval  HI_SUCCESS,HI_FAILURE
*/
HI_S32	DRV_HDMI_EventDeInit(HDMI_EVENT_INFO_S *pstEvtInfo);


/**
\brief	malloc a HI_HDMI_EVENT_POOL_S to service a process
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[out]	@pu32PoolID 		PoolID handle return to user
\retval  HI_SUCCESS;HI_FAILURE.
*/
HI_S32	DRV_HDMI_EventPoolMalloc(HDMI_EVENT_INFO_S *pstEvtInfo,
											  HI_U32	 *pu32PoolID);


/**
\brief	free an active HI_HDMI_EVENT_POOL_S
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[in]	@pu32PoolID 		PoolID handle 
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32	DRV_HDMI_EventPoolFree(HDMI_EVENT_INFO_S *pstEvtInfo,
										   HI_U32 u32PoolID);


/**
\brief	write all pools in the list
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[in]	@enEvent 			the writting event
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32	DRV_HDMI_EventPoolWrite(HDMI_EVENT_INFO_S *pstEvtInfo,
											HDMI_EVENT_E	 enEvent);

/**
\brief	read a pool to get the first event
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[in]	@pu32PoolID 		PoolID handle 
\param[out]	@penEvent			pointer to the user's reading memory
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32	DRV_HDMI_EventPoolRead(HDMI_EVENT_INFO_S *pstEvtInfo,
										    HI_U32 u32PoolID,
										    HDMI_EVENT_E *penEvent);

/**
\brief	get a pool status(for debug)
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[in]	@pu32PoolNum		range[1~u32Toal]
\param[out]	@pstCtrl				pointer to the user's memory
\param[out]	@pstCnt				pointer to the user's memory
\retval  HI_SUCCESS;HI_FAILURE
\NOTE: Debuger don't know  PoolID details,
\          so it must get the total number pools in the list first,
\		then use u32PoolNum to get a status.
*/
HI_S32	DRV_HDMI_EventPoolStatusGet(HDMI_EVENT_INFO_S *pstEvtInfo,
									HI_U32 u32PoolNum,
									HDMI_EVENT_RUN_CTRL_S *pstCtrl,
									HDMI_EVENT_RUN_CNT_S *pstCnt);


/**
\brief	get a list  pools total number
\param[in]	@pstEvtInfo 			pointer to a HDMI_EVENT_INFO_S
\param[out]	@pu32Total			pointer to the user's memory
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32	DRV_HDMI_EventPoolTotalGet(HDMI_EVENT_INFO_S *pstEvtInfo,
								   HI_U32	 *pu32Total);



#ifdef __cplusplus
}
#endif

#endif //__DRV_HDMI_EVENT__



