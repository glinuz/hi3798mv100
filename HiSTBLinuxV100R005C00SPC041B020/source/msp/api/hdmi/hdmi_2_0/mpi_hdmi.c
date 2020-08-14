/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_hdmi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/6/20
  Description   :
  History       :
  Date          : 2010/6/20
  Author        : q46153 l00168554
  Modification  : 2015/06/16 l232354
*******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <hi_common.h>
#include "drv_hdmi_ioctl.h"
#include "list.h"

#if defined (CONFIG_HDMI_STB_SDK)
#include "hi_unf_hdmi.h"
#include "hi_mpi_hdmi.h"
#include "hi_drv_struct.h"
#include "hi_drv_disp.h"
#include "hi_mpi_mem.h"
#elif defined (CONFIG_HDMI_BVT_SDK)
#include  "hi_comm_hdmi.h"
#include "mpi_hdmi_com.h"
#endif

/************************************************************************/
/* constants                                                            */
/************************************************************************/
#if defined (CONFIG_HDMI_BVT_SDK)
#define 	UMAP_DEVNAME_HDMI 			"hdmi"
#define 	HI_UNF_HDMI_ID_BUTT 		HI_HDMI_ID_BUTT
#define 	HI_MALLOC(arg1, arg2) 		malloc(arg2)
#define 	HI_FREE(arg1, arg2) 		free(arg2)
#endif

typedef struct list_head List;

#define HDMI_LOCK()                                      	    \
do{                                                             \
    (void)pthread_mutex_lock(&s_stHdmiMutex);                   \
}while(0)

#define HDMI_UNLOCK()                                    	    \
do{                                                             \
    (void)pthread_mutex_unlock(&s_stHdmiMutex);                 \
}while(0)

#define HDMI_CEC_LOCK()                                         \
do{                                                         	\
    (void)pthread_mutex_lock(&s_stCECAttr.stCecMutex);          \
}while(0)

#define HDMI_CEC_UNLOCK()                                       \
do{                                                         	\
    (void)pthread_mutex_unlock(&s_stCECAttr.stCecMutex);        \
}while(0)

#define HDMI_CHECK_NULL_PTR(ptr)  do{                       	\
    if (NULL == (ptr))                                  		\
    {                                                   		\
        HI_ERR_HDMI("poiner is NULL!!\n" );             		\
        return HI_ERR_HDMI_NUL_PTR;                   			\
    }                                                   		\
}while(0)

#define HDMI_CheckChnOpen(l_HdmiID) do{                     	\
    if (HI_TRUE != s_stHdmiChnUserParam[l_HdmiID].bOpen) 	    \
    {                                                   		\
        HI_WARN_HDMI("enHdmi:%d do NOT open\n", l_HdmiID);		\
        return HI_ERR_HDMI_DEV_NOT_OPEN;                		\
    }                                                   		\
}while(0)

#define HDMI_CHECK_ID(HdmiId)  do{                          	\
    if ((HdmiId) >= HI_UNF_HDMI_ID_BUTT)                        \
    {                                                       	\
    	HI_ERR_HDMI("HdmiId %d is invalid.\n", HdmiId);     	\
    	return HI_ERR_HDMI_INVALID_PARA;                    	\
    }                                                    		\
}while(0)

#define MAX_DELAY_TIME_MS  (10000)

/************************************************************************/
/* data structures                                                      */
/************************************************************************/
typedef struct hi_HDMI_CALLBACK_NODE_S
{
    List list;
    HI_UNF_HDMI_CALLBACK_FUNC_S stCallbackfunc;
}HDMI_CALLBACK_NODE_S;

typedef struct
{
    HI_BOOL            bOpen;
    HI_BOOL            bStart;
    HI_UNF_HDMI_ID_E   eHdmiId;
    HI_UNF_HDMI_ATTR_S stAttr;
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S   stAVIInfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S   stAUDInfoFrame;
}HDMI_CHN_USER_ATTR_S;

typedef struct
{
    HI_BOOL     bHdmiInit;
    HI_BOOL     bEnableTimer;             /* Timer thread Flag */
    pthread_t   tEventTimer;              /* Timer thread ID */
    HI_BOOL     bHdmiExit;                /* HDMI Exit Flag */
}HDMI_COMM_USER_ATTR_S;

typedef struct
{
    HI_UNF_HDMI_ID_E enHdmi;
    HI_BOOL     bCecCbReg;
    HI_BOOL     bCecEnable;
    pthread_t   CECTread;
    pthread_mutex_t   stCecMutex;
    HI_UNF_HDMI_CECCALLBACK pCECCallback;
}HDMI_CEC_ATTR_S;

/************************************************************************/
/* global data                                                          */
/************************************************************************/
static HI_S32           		s_s32HdmiDevFd = -1;
static HDMI_CALLBACK_NODE_S 	s_stHDMICallBackList[HI_UNF_HDMI_ID_BUTT];
static HDMI_CEC_ATTR_S 		    s_stCECAttr;
static const HI_CHAR    		s_u8HdmiDevName[] = "/dev/"UMAP_DEVNAME_HDMI;
static HDMI_COMM_USER_ATTR_S 	s_stHdmiCommUserParam[HI_UNF_HDMI_ID_BUTT];
static HDMI_CHN_USER_ATTR_S 	s_stHdmiChnUserParam[HI_UNF_HDMI_ID_BUTT];
static pthread_mutex_t   		s_stHdmiMutex = PTHREAD_MUTEX_INITIALIZER;

/************************************************************************/
/* private interface                                                    */
/************************************************************************/

/**
\brief HOT-PLUG Callback function, it will invoke Customer callback function.
\attention Customer callback function is register in HI_UNF_HDMI_Init
\param[in] none
\retval none
\see \n
*/
static void Hdmi_EventCallbackInvoke(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_EVENT_TYPE_E event)
{
    HDMI_CALLBACK_NODE_S *tmp = HI_NULL;
    List *pos = HI_NULL;

    if (HI_UNF_HDMI_ID_BUTT <= enHdmi)
    {
        HI_ERR_HDMI("enHdmi %d, event %d is invalid\n", enHdmi, event);
        return;
    }

    list_for_each(pos, &s_stHDMICallBackList[enHdmi].list)
    {
        tmp = list_entry(pos, HDMI_CALLBACK_NODE_S, list);
        tmp->stCallbackfunc.pfnHdmiEventCallback(
            event, tmp->stCallbackfunc.pPrivateData);
    }

	return;
}

static HI_VOID * Hdmi_EventPollThread(void* pParam)
{
    HI_U32 s32Ret;
    DRV_HDMI_EVENT_S stEvent;
    HI_UNF_HDMI_ID_E enHdmi = 0;
    HI_UNF_HDMI_EVENT_TYPE_E eEvent;

    if (HI_NULL == pParam)
    {
        HI_ERR_HDMI("pParam %p is invalid!\n", pParam);
        return (void *)0;
    }
    enHdmi = *(HI_UNF_HDMI_ID_E*)pParam;
    if (HI_UNF_HDMI_ID_BUTT <= enHdmi)
    {
        HI_ERR_HDMI("enHdmi %d is invalid!\n", enHdmi);
        return (void *)0;
    }

    while((HI_FALSE == s_stHdmiCommUserParam[enHdmi].bHdmiExit))
    {
        if (HI_FALSE == s_stHdmiChnUserParam[enHdmi].bOpen)
        {
            usleep(100*1000);
            continue;
        }
        memset(&stEvent, 0, sizeof(DRV_HDMI_EVENT_S));
        stEvent.enHdmiID = enHdmi;
        s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_POLL_EVENT, &stEvent);
        if (s32Ret == HI_SUCCESS && stEvent.enEvent != HDMI_EVENT_BUTT)
        {
            HDMI_KEVENTTYPE_2_UEVENTTYPE(stEvent.enEvent, eEvent);
            Hdmi_EventCallbackInvoke(enHdmi, eEvent);
        }
        else if (stEvent.enEvent == HDMI_EVENT_BUTT)
        {
            HI_ERR_HDMI("poll event fail,enEvent=0x%x\n",stEvent.enEvent);
        }

    }

   return (void *)0;
}

static HI_VOID *Hdmi_CEC_EventThread(HI_VOID* pParam)
{
    HI_S32                  s32Ret = 0;
    HI_UNF_HDMI_ID_E        enHdmi = 0;
    HI_UNF_HDMI_CEC_CMD_S   stCECCmd;

    if (HI_NULL == pParam)
    {
        HI_ERR_HDMI("pParam %p is invalid!\n", pParam);
        return (void *)0;
    }
    enHdmi = *(HI_UNF_HDMI_ID_E*)pParam;
    if (HI_UNF_HDMI_ID_BUTT <= enHdmi)
    {
        HI_ERR_HDMI("enHdmi %d is invalid!\n", enHdmi);
        return HI_NULL;
    }

    while((HI_TRUE == s_stCECAttr.bCecEnable) && (s_stHdmiChnUserParam[enHdmi].bOpen) )
    {
        s32Ret = HI_MPI_HDMI_ComGetCECCommand(s_stCECAttr.enHdmi, &stCECCmd);
        if (s32Ret == HI_SUCCESS)
        {
            HDMI_CEC_LOCK();
            if (s_stCECAttr.pCECCallback)
            {
                s_stCECAttr.pCECCallback(s_stCECAttr.enHdmi, &stCECCmd, HI_NULL);
            }
            HDMI_CEC_UNLOCK();
        }
    }
    return HI_NULL;
}


/************************************************************************/
/* public interface                                                     */
/************************************************************************/
#if defined(HDMI_DUAL_CPU_SUPPORT)
HI_S32 HI_MPI_HDMI_ComGetFd(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMI_CHECK_ID(enHdmi);

    return s_s32HdmiDevFd;
}
#endif

/**
\brief
\attention \n
\param
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComInit(void)
{
    HI_U32 u32HdmiID;

    if (s_s32HdmiDevFd > 0)
    {
        /* HDMI have been initialized */
        return HI_SUCCESS;
    }

    for (u32HdmiID = 0; u32HdmiID < HI_UNF_HDMI_ID_BUTT; u32HdmiID++)
    {
        INIT_LIST_HEAD(&s_stHDMICallBackList[u32HdmiID].list);
    }

    memset(&s_stCECAttr, 0, sizeof(HDMI_CEC_ATTR_S));
    pthread_mutex_init(&s_stCECAttr.stCecMutex, NULL);

    memset(&s_stHdmiChnUserParam, 0, HI_UNF_HDMI_ID_BUTT*sizeof(HDMI_CHN_USER_ATTR_S));
    memset(&s_stHdmiCommUserParam, 0, HI_UNF_HDMI_ID_BUTT*sizeof(HDMI_COMM_USER_ATTR_S));
    pthread_mutex_init(&s_stHdmiMutex, NULL);

    s_s32HdmiDevFd = open(s_u8HdmiDevName, O_RDWR);
    if (s_s32HdmiDevFd <= 0)
    {
        HI_FATAL_HDMI("open HDMI err.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/**
\brief
\attention \n
\param
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComDeInit(void)
{
    if (s_s32HdmiDevFd <= 0)
    {
        /* HDMI have no initialized */
        return HI_SUCCESS;
    }

    close(s_s32HdmiDevFd);
    s_s32HdmiDevFd = -1;

    memset(&s_stHdmiCommUserParam, 0, HI_UNF_HDMI_ID_BUTT*sizeof(HDMI_COMM_USER_ATTR_S));//clean
    /*destory mutex */
    pthread_mutex_destroy(&s_stHdmiMutex);

    return HI_SUCCESS;
}

/**
\brief
\attention \n
\param[in] enHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComOpen(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_OPEN_PARA_S *pstOpenPara)
{
    HI_S32 s32Ret = 0;
    HDMI_OPEN_S stOpen;

    HDMI_CHECK_ID(enHdmi);

    if (s_s32HdmiDevFd <= 0)
    {
        HI_FATAL_HDMI("HDMI device not open\n");
        return HI_ERR_HDMI_NOT_INIT;
    }

    if (s_stHdmiChnUserParam[enHdmi].bOpen)
    {
        return HI_SUCCESS;
    }

    s_stHdmiChnUserParam[enHdmi].eHdmiId = enHdmi;
    s32Ret = pthread_create(&s_stHdmiCommUserParam[enHdmi].tEventTimer, HI_NULL, Hdmi_EventPollThread, &(s_stHdmiChnUserParam[enHdmi].eHdmiId));
    if (s32Ret != HI_SUCCESS)
    {
        HI_FATAL_HDMI("timer task return:0x%x\n", s32Ret);
        return HI_FAILURE;
    }
    s_stHdmiCommUserParam[enHdmi].bEnableTimer = HI_TRUE;


    HDMI_LOCK();

    stOpen.enHdmiID = enHdmi;
	if (HI_NULL != pstOpenPara)
	{
    	stOpen.enDefaultMode = pstOpenPara->enDefaultMode;
	}
	s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_OPEN, &stOpen);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_HDMI("hdmi open err:%d\n",s32Ret);
        HDMI_UNLOCK();
        return s32Ret;
    }

#if defined (HDMI_DEFUALT_CALLBACK_SUPPORT)
	s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_UNREGISTER_CALLBACK, &enHdmi);
#endif
    s_stHdmiChnUserParam[enHdmi].bOpen = HI_TRUE;//Enable HDMI thread
	HDMI_UNLOCK();

    return HI_SUCCESS;
}


/**
\brief
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComClose(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (HI_TRUE == s_stHdmiChnUserParam[enHdmi].bOpen)
    {
        HDMI_LOCK();
        s_stHdmiChnUserParam[enHdmi].bOpen = HI_FALSE;
        s_stHdmiCommUserParam[enHdmi].bHdmiExit = HI_TRUE;
        s32Ret |= ioctl(s_s32HdmiDevFd, CMD_HDMI_CLOSE, &enHdmi);
        if (s32Ret != HI_SUCCESS)
        {
            HDMI_UNLOCK();
            return s32Ret;
        }
        HDMI_UNLOCK();
    }

    if (s_stHdmiCommUserParam[enHdmi].bEnableTimer == HI_TRUE)
    {
        s32Ret |= pthread_join(s_stHdmiCommUserParam[enHdmi].tEventTimer, NULL);
        s_stHdmiCommUserParam[enHdmi].bEnableTimer = HI_FALSE;
    }

    return s32Ret;
}


/**
\brief Start HDMI output. It should be invoked within User callback.
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComStart(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (s_stHdmiChnUserParam[enHdmi].bStart == HI_TRUE)
    {
        HI_INFO_HDMI("HI_MPI_HDMI_ComStart Already Start before!\n");
    }

    HDMI_LOCK();
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_START, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    s_stHdmiChnUserParam[enHdmi].bStart = HI_TRUE;
    HDMI_UNLOCK();

    return HI_SUCCESS;
}

/**
\brief Stop HDMI output
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComStop(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (s_stHdmiChnUserParam[enHdmi].bStart== HI_FALSE)
    {
        return HI_SUCCESS;
    }

    HDMI_LOCK();
    if (HI_TRUE != s_stHdmiChnUserParam[enHdmi].bStart)
    {
        HDMI_UNLOCK();
        return HI_SUCCESS;
    }

    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_STOP, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    s_stHdmiChnUserParam[enHdmi].bStart = HI_FALSE;
    HDMI_UNLOCK();

    return HI_SUCCESS;
}


/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pstAttr
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_APP_ATTR_S *pstAppAttr)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAppAttr);

    HDMI_LOCK();
    pstAppAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_ATTR, pstAppAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pstAttr
\retval none
\see \n
*/

HI_S32 HI_MPI_HDMI_ComGetAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_APP_ATTR_S *pstAppAttr)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAppAttr);

    HDMI_LOCK();
    pstAppAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_ATTR, pstAppAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param
\param
\retval
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetAOAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_AO_ATTR_S *pstDrvAOAttr)
{
	HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDrvAOAttr);

    HDMI_LOCK();
    pstDrvAOAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_AO_ATTR, pstDrvAOAttr);
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param
\param
\retval
\see \n
*/
HI_S32 HI_MPI_HDMI_ComGetAOAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_AO_ATTR_S *pstDrvAOAttr)
{
	HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDrvAOAttr);

    HDMI_LOCK();
    pstDrvAOAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_AO_ATTR, pstDrvAOAttr);
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param
\param
\retval
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetVOAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_VO_ATTR_S *pstDrvVOAttr)
{
	HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDrvVOAttr);

    HDMI_LOCK();
    pstDrvVOAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_VO_ATTR, pstDrvVOAttr);
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param
\param
\retval
\see \n
*/

HI_S32 HI_MPI_HDMI_ComGetVOAttr(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_VO_ATTR_S *pstDrvVOAttr)
{
	HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDrvVOAttr);

	memset(pstDrvVOAttr, 0, sizeof(DRV_HDMI_VO_ATTR_S));

    HDMI_LOCK();
    pstDrvVOAttr->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_VO_ATTR, pstDrvVOAttr);
    HDMI_UNLOCK();

    return s32Ret;
}

/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pstInfoFrame
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);

	HDMI_LOCK();
    pstInfoFrame->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_INFOFRAME, pstInfoFrame);
	HDMI_UNLOCK();
	if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}


/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pstInfoFrame
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComGetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);

	HDMI_LOCK();
    pstInfoFrame->enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_INFOFRAME, pstInfoFrame);
	HDMI_UNLOCK();
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return s32Ret;
}


/**
\brief Set DeepColor mode
\attention \n
\param[in] enHdmi
\param[in] enDeepColor, please refer to:HI_UNF_HDMI_DEEP_COLOR_E
\retval HI_SUCCESS
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_DEEP_COLOR_S stDeepcolor;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HDMI_LOCK();
    stDeepcolor.enHdmiID = enHdmi;

    HDMI_UDEEPCOLOR_2_KDEEPCOLOR(enDeepColor, stDeepcolor.enDeepColorMode);
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_DEEPCOLOR, &stDeepcolor);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return HI_SUCCESS;
}

/**
\brief Get DeepColor mode
\attention \n
\param[in] enHdmi
\param[in] enDeepColor, please refer to:HI_UNF_HDMI_DEEP_COLOR_E
\retval HI_SUCCESS
\see \n
*/
HI_S32 HI_MPI_HDMI_ComGetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_DEEP_COLOR_S stDeepcolor;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(penDeepColor);

    HDMI_LOCK();
    stDeepcolor.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_DEEPCOLOR, &stDeepcolor);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_KDEEPCOLOR_2_UDEEPCOLOR(stDeepcolor.enDeepColorMode, *penDeepColor);
    HDMI_UNLOCK();

    return HI_SUCCESS;
}

/**
\brief set video xvYCC mode
\attention \n
\param[in] enHdmi
\param[in] bEnable   enable xvYCC mode flag.
\retval HI_SUCCESS
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_XVYCC_ENABLE_S stXVYCC;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HDMI_LOCK();
    stXVYCC.enHdmiID = enHdmi;
    stXVYCC.bEnable  = bEnable;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_XVYCC, &stXVYCC);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return HI_SUCCESS;
}


/**
\brief set HDMI AV mute
\attention \n
\param[in] enHdmi
\param[in] bAvMute   avmute flag
\retval HI_SUCCESS
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_AVMUTE_S stAvMute;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HDMI_LOCK();
    stAvMute.enHdmiID = enHdmi;
    stAvMute.bAvMute  = bAvMute;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_AVMUTE, &stAvMute);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_ComGetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_STATUS_S stDrvHdmiStatus;

    HDMI_CHECK_ID(enHdmi);
	HDMI_CHECK_NULL_PTR(pHdmiStatus);

    HDMI_LOCK();
    memset(&stDrvHdmiStatus, 0, sizeof(DRV_HDMI_STATUS_S));
    stDrvHdmiStatus.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_STATUS, &stDrvHdmiStatus);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    memcpy(pHdmiStatus, &stDrvHdmiStatus.stHdmiStatus, sizeof(HI_UNF_HDMI_STATUS_S));
    HDMI_UNLOCK();

    return s32Ret;

}


/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pCapability
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComGetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, DRV_HDMI_SINK_CAPABILITY_S *pstDrvCap)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDrvCap);

    memset(pstDrvCap, 0, sizeof(DRV_HDMI_SINK_CAPABILITY_S));
    pstDrvCap->enHdmiID = enHdmi;
	HDMI_LOCK();
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_SINK_CAPABILITY, pstDrvCap);
	HDMI_UNLOCK();

    return s32Ret;
}

HI_S32 HI_MPI_HDMI_ComForceGetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_EDID_RAWDATA_S EDIDData;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(u8Edid);
    HDMI_CHECK_NULL_PTR(u32EdidLength);

    *u32EdidLength = 0;

    HDMI_LOCK();
    memset(&EDIDData, 0, sizeof(DRV_HDMI_EDID_RAWDATA_S));
    EDIDData.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_UPDATE_EDID, &EDIDData);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    if(EDIDData.stEdidRaw.u8EdidValid == HI_TRUE)
    {
        *u32EdidLength = EDIDData.stEdidRaw.u32EdidLen;
        memcpy(u8Edid, EDIDData.stEdidRaw.u8Edid, EDIDData.stEdidRaw.u32EdidLen);
    }

    return s32Ret;
}

HI_S32 HI_MPI_HDMI_ComReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 ret = 0;

    DRV_HDMI_EDID_RAWDATA_S EdidData;

    HDMI_CHECK_NULL_PTR(u8Edid);
    HDMI_CHECK_NULL_PTR(u32EdidLength);

    memset(&EdidData, 0, sizeof(DRV_HDMI_EDID_RAWDATA_S));
    EdidData.enHdmiID = HDMI_DEVICE_ID0;
    ret = HI_MPI_HDMI_ComForceGetEDID(EdidData.enHdmiID, &(EdidData.stEdidRaw.u8Edid[0]), &(EdidData.stEdidRaw.u32EdidLen));
    if(ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("forec get edid fail!\n");
        return ret;
    }
    *u32EdidLength = EdidData.stEdidRaw.u32EdidLen;
    memcpy(u8Edid, &(EdidData.stEdidRaw.u8Edid[0]), EdidData.stEdidRaw.u32EdidLen);

    return ret;
}

HI_S32 HI_MPI_HDMI_ComRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HDMI_CALLBACK_NODE_S *tmp = NULL;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstCallbackFunc);

#if defined (HDMI_DEFUALT_CALLBACK_SUPPORT)
    HDMI_CheckChnOpen(enHdmi);
#endif

    HDMI_LOCK();
    if(NULL != pstCallbackFunc->pfnHdmiEventCallback)
    {
        tmp = (HDMI_CALLBACK_NODE_S *)HI_MALLOC(HI_ID_HDMI,sizeof(HDMI_CALLBACK_NODE_S));
        if(NULL == tmp)
        {
            HI_ERR_HDMI("Callback malloc failed\n");
            HDMI_UNLOCK();
            return HI_FAILURE;
        }

        //tmp->enHdmi = enHdmi;
        tmp->stCallbackfunc = *pstCallbackFunc;
    }
    else
    {
        HI_ERR_HDMI("Callbackaddr is NULL!\n");
        HDMI_UNLOCK();
        return HI_FAILURE;
    }

    list_add_tail(&(tmp->list), &(s_stHDMICallBackList[enHdmi].list));

#if defined (HDMI_DEFUALT_CALLBACK_SUPPORT)
	s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_REGISTER_CALLBACK, &enHdmi);
#endif
    HDMI_UNLOCK();

    return s32Ret;
}


HI_S32 HI_MPI_HDMI_ComUnRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HDMI_CALLBACK_NODE_S *tmp;
    List *pos, *q;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstCallbackFunc);

    list_for_each_safe(pos, q,&s_stHDMICallBackList[enHdmi].list)
    {
        tmp= list_entry(pos, HDMI_CALLBACK_NODE_S, list);
        if( tmp &&
            tmp->stCallbackfunc.pfnHdmiEventCallback == pstCallbackFunc->pfnHdmiEventCallback)
        {
            list_del(pos);

            HI_FREE(HI_ID_HDMI, tmp);

#if defined (HDMI_DEFUALT_CALLBACK_SUPPORT)
			HDMI_LOCK();
		    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_UNREGISTER_CALLBACK, &enHdmi);
		    HDMI_UNLOCK();
#endif
            return s32Ret;
        }
    }
    HI_ERR_HDMI("CallbackFunc:No this Callbackfunc\n");

    return HI_FAILURE;
}

HI_S32 HI_MPI_HDMI_ComRegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCallback);

    HDMI_CEC_LOCK();
    s_stCECAttr.enHdmi = enHdmi;
    s_stCECAttr.pCECCallback = pCECCallback;
    s_stCECAttr.bCecCbReg = HI_TRUE;
    HDMI_CEC_UNLOCK();

    return s32Ret;
}

HI_S32 HI_MPI_HDMI_ComUnRegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCallback);

    HDMI_CEC_LOCK();
    if(pCECCallback != s_stCECAttr.pCECCallback)
    {
        HI_ERR_HDMI("hand err :%p != %p\n",pCECCallback,s_stCECAttr.pCECCallback);
        HDMI_CEC_UNLOCK();
        return HI_FAILURE;
    }
    s_stCECAttr.bCecCbReg = HI_FALSE;
    s_stCECAttr.pCECCallback = HI_NULL;
    s_stCECAttr.enHdmi       = 0;
    HDMI_CEC_UNLOCK();

    return HI_SUCCESS;
}

/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pCECCmd
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_ComSetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pCECCmd)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_CEC_CMD_S stDrvCecCmd;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCmd);

    if (!s_stCECAttr.bCecEnable)
    {
        HI_ERR_HDMI("Cec is not enable\n");
        return HI_FAILURE;
    }

    memset(&stDrvCecCmd, 0, sizeof(DRV_HDMI_CEC_CMD_S));
    memcpy(&stDrvCecCmd.CECCmd, pCECCmd, sizeof(HI_UNF_HDMI_CEC_CMD_S));
    stDrvCecCmd.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_CEC, &stDrvCecCmd);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("Set cec cmd fail\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_ComGetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S *pCECCmd)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_CEC_CMD_S stDrvCecCmd;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCmd);

    if (!s_stCECAttr.bCecEnable)
    {
        HI_ERR_HDMI("Cec is not enable\n");
        return HI_FAILURE;
    }

    memset(&stDrvCecCmd, 0, sizeof(DRV_HDMI_CEC_CMD_S));

    stDrvCecCmd.enHdmiID   = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_CEC, &stDrvCecCmd);
    if (s32Ret != HI_SUCCESS)
    {
        if(HI_TRUE == s_stCECAttr.bCecEnable)
        {
            HI_ERR_HDMI("Get cec cmd fail\n");
        }

        return s32Ret;
    }
    memcpy(pCECCmd, &(stDrvCecCmd.CECCmd), sizeof(HI_UNF_HDMI_CEC_CMD_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_ComCECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S *pStatus)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_CEC_INFO_S CECStatus;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pStatus);

    HDMI_LOCK();
    memset(&CECStatus, 0, sizeof(DRV_HDMI_CEC_INFO_S));
    CECStatus.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_CECSTATUS, &CECStatus);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        HI_ERR_HDMI("Get cec status fail\n");
        return s32Ret;
    }
    pStatus->bEnable        = CECStatus.stCecInfo.bNetValid & s_stCECAttr.bCecEnable;
    pStatus->u8LogicalAddr  = CECStatus.stCecInfo.u8LogicalAddr;
    memcpy(pStatus->u8PhysicalAddr, CECStatus.stCecInfo.au8PhysicalAddr,sizeof(pStatus->u8PhysicalAddr));
    memcpy(pStatus->u8Network, CECStatus.stCecInfo.au8Network, sizeof(CECStatus.stCecInfo.au8PhysicalAddr));
    HDMI_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_ComCECEnable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HDMI_LOCK();

    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_CEC_ENABLE, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        HI_ERR_HDMI("Enable cec fail\n");

        return s32Ret;
    }
	HDMI_UNLOCK();

	HDMI_CEC_LOCK();
    s_stCECAttr.enHdmi      = enHdmi;
    s_stCECAttr.bCecEnable  = HI_TRUE;

    s32Ret = pthread_create(&s_stCECAttr.CECTread, HI_NULL, Hdmi_CEC_EventThread, &(s_stHdmiChnUserParam[enHdmi].eHdmiId));
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_CEC_UNLOCK();
        HI_ERR_HDMI("Create cec event thread fail\n");
        return s32Ret;
    }
    HDMI_CEC_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_ComCECDisable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret = 0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HDMI_CEC_LOCK();

    if (!s_stCECAttr.bCecEnable)
    {
        HDMI_CEC_UNLOCK();
        HI_ERR_HDMI("Cec is not enable\n");
        return HI_FAILURE;
    }

    s_stCECAttr.bCecEnable = HI_FALSE;
    HDMI_CEC_UNLOCK();

    HDMI_LOCK();
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_CEC_DISABLE, &enHdmi);
    HDMI_UNLOCK();

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("Disable cec fail\n");
        return s32Ret;
    }

    HDMI_CEC_LOCK();
    pthread_join(s_stCECAttr.CECTread, NULL);
    HDMI_CEC_UNLOCK();

    return HI_SUCCESS;
}


#ifdef HDMI_HDCP_SUPPORT
HI_S32 HI_MPI_HDMI_ComLoadHDCPKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_LOAD_KEY_S stLoadKey;

	HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstLoadKey);

    HDMI_LOCK();
    stLoadKey.stKey.u32KeyLength = pstLoadKey->u32KeyLength;
    stLoadKey.stKey.pu8InputEncryptedKeyAddr = pstLoadKey->pu8InputEncryptedKey;
    stLoadKey.enHdmiID = enHdmi;
    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_LOAD_KEY, &stLoadKey);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI(" Ioctrl Load Key failed!\n");
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return s32Ret;
}

#endif

HI_S32 HI_MPI_HDMI_ComGetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_DELAY_S stHdmiDelay;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDelay);

    HDMI_LOCK();
    memset(&stHdmiDelay,0,sizeof(DRV_HDMI_DELAY_S));
    stHdmiDelay.enHdmiID = enHdmi;

    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_GET_DELAY , &stHdmiDelay);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    memcpy(pstDelay, &stHdmiDelay.stDelay, sizeof(HDMI_DELAY_S));
    HDMI_UNLOCK();

    return s32Ret;

}

HI_S32 HI_MPI_HDMI_ComSetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret = 0;
    DRV_HDMI_DELAY_S stHdmiDelay;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDelay);

    if((pstDelay->u32FmtDelay > MAX_DELAY_TIME_MS) ||
       (pstDelay->u32MuteDelay > MAX_DELAY_TIME_MS))
    {
        HI_ERR_HDMI("Delay Time fmt:%d Mute:%d Over Range:%d \n", pstDelay->u32FmtDelay,
            pstDelay->u32MuteDelay,MAX_DELAY_TIME_MS);
        return HI_ERR_HDMI_INVALID_PARA;
    }

    HDMI_LOCK();
    stHdmiDelay.enHdmiID = enHdmi;
    memcpy(&stHdmiDelay.stDelay, pstDelay, sizeof(HI_UNF_HDMI_DELAY_S));

    s32Ret = ioctl(s_s32HdmiDevFd, CMD_HDMI_SET_DELAY , &stHdmiDelay);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_UNLOCK();
        return s32Ret;
    }
    HDMI_UNLOCK();

    return s32Ret;
}






/*------------------------------END--------------------------------*/
