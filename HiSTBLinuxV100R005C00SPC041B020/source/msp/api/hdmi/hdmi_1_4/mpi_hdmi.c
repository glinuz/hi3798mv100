/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/6/20
  Description   :
  History       :
  Date          : 2010/6/20
  Author        : q46153 l00168554
  Modification  :
*******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <hi_common.h>
#include "hi_unf_hdmi.h"
#include "hi_mpi_hdmi.h"
#include "hi_drv_struct.h"
#include "hi_drv_disp.h"

#include "hi_mpi_mem.h"

#include "drv_hdmi_ioctl.h"

#include "list.h"

//10s
#define MAX_DELAY_TIME_MS 10000

typedef struct list_head List;

typedef struct hi_HDMI_CALLBACK_NODE_S
{
    List list;
    HI_UNF_HDMI_CALLBACK_FUNC_S stCallbackfunc;
}HDMI_CALLBACK_NODE_S;

HDMI_CALLBACK_NODE_S g_pstHDMICallBackList;


typedef struct
{
    HI_BOOL            bOpen;
    HI_BOOL            bStart;
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
    HI_BOOL     bCECEnable;
    pthread_t   CECTread; 
    HI_UNF_HDMI_CECCALLBACK pCECCallback;
}HDMI_CEC_ATTR_S;

static HDMI_CEC_ATTR_S g_stCECAttr;


//typedef struct tagMPI_HDMI_CALLBACK_S
//{
    //HI_U32 u32CallbackAddr;
    //HI_UNF_HDMI_CALLBACK_FUNC_S stCallbackFunc;
//}MPI_HDMI_CALLBACK_S;
//static MPI_HDMI_CALLBACK_S g_stCallback; 

//static HDMI_AUDIO_ATTR_S    g_stHDMIAudAttr; 

static HI_S32           g_HDMIDevFd = -1;
static const HI_CHAR    g_HDMIDevName[] = "/dev/"UMAP_DEVNAME_HDMI;
static HI_U32           g_HDMIProcID = HI_INVALID_HANDLE;//defalult value: HI_INVALID_HANDLE == 0xffffffff

static HDMI_COMM_USER_ATTR_S g_stHdmiCommUserParam;

static HDMI_CHN_USER_ATTR_S g_stHdmiChnUserParam[HI_UNF_HDMI_ID_BUTT];

static pthread_mutex_t   g_HDMIMutex = PTHREAD_MUTEX_INITIALIZER;

//static HI_U32 hdmi_Mutex_Count = 0;

#define HI_HDMI_LOCK()                                      \
do{                                                         \
    (void)pthread_mutex_lock(&g_HDMIMutex);                 \
}while(0)

#define HI_HDMI_UNLOCK()                                    \
do{                                                         \
    (void)pthread_mutex_unlock(&g_HDMIMutex);               \
}while(0)

#define HDMI_CHECK_NULL_PTR(ptr)  do{                       \
        if (NULL == (ptr))                                  \
        {                                                   \
            HI_ERR_HDMI("poiner is NULL!!\n" );             \
            return HI_ERR_HDMI_NUL_PTR;                     \
        }                                                   \
    }while(0)

#define HDMI_CHECK_ID(l_enHdmi)  do{                        \
    if ((l_enHdmi) >= HI_UNF_HDMI_ID_BUTT)                  \
        {                                                   \
            HI_WARN_HDMI("enHdmi %d is invalid\n", l_enHdmi);\
            return HI_ERR_HDMI_INVALID_PARA;                \
        }                                                   \
    }while(0)

#define HDMI_CheckChnOpen(l_HdmiID) do{                     \
        if (HI_TRUE != g_stHdmiChnUserParam[0].bOpen)    \
        {                                                   \
            HI_WARN_HDMI("enHdmi:%d do NOT open\n", l_HdmiID);\
            return HI_ERR_HDMI_DEV_NOT_OPEN;                \
        }                                                   \
    }while(0)

static void HI_MPI_UNF_HDMI_GenEvent(HI_UNF_HDMI_EVENT_TYPE_E event);

HI_VOID * Hdmi_Poll_Event_Thread(void* pParam)
{
    HI_U32 Ret;
    HDMI_POLL_EVENT_S stPollEvent;

    pParam = pParam;//just to move compile warring

    while(HI_FALSE == g_stHdmiCommUserParam.bHdmiExit)
    {
        if (HI_FALSE == g_stHdmiChnUserParam[0].bOpen)
        {
            usleep(100*1000);
            continue;
        }

        memset(&stPollEvent, 0, sizeof(HDMI_POLL_EVENT_S));
        
        stPollEvent.enHdmi        = HI_UNF_HDMI_ID_0;
        stPollEvent.u32ProcID = g_HDMIProcID;
        Ret = ioctl(g_HDMIDevFd, CMD_HDMI_POLL_EVENT, &stPollEvent);
        if (Ret == HI_SUCCESS)
        {
            HI_INFO_HDMI("Get new event:%d\n", stPollEvent.Event);
            HI_MPI_UNF_HDMI_GenEvent(stPollEvent.Event);
        }
        usleep(80*1000);
    }

   return (void *)0; 
}

HI_VOID Hdmi_Unf2DrvAttr(HDMI_APP_ATTR_S *pstDestAttr,HI_UNF_HDMI_ATTR_S *pstSrcAttr)
{
    pstDestAttr->bEnableHdmi          = pstSrcAttr->bEnableHdmi;
    pstDestAttr->bEnableVideo         = pstSrcAttr->bEnableVideo;
    pstDestAttr->bEnableAudio         = pstSrcAttr->bEnableAudio;
   
    pstDestAttr->enVidOutMode         = pstSrcAttr->enVidOutMode;
    pstDestAttr->enDeepColorMode      = pstSrcAttr->enDeepColorMode;
    pstDestAttr->bxvYCCMode           = pstSrcAttr->bxvYCCMode;
    pstDestAttr->bEnableAviInfoFrame  = pstSrcAttr->bEnableAviInfoFrame;
    pstDestAttr->bEnableAudInfoFrame  = pstSrcAttr->bEnableAudInfoFrame;
    pstDestAttr->bEnableSpdInfoFrame  = pstSrcAttr->bEnableSpdInfoFrame;
    pstDestAttr->bEnableMpegInfoFrame = pstSrcAttr->bEnableMpegInfoFrame;
    
    //pstDestAttr->bDebugFlag           = pstSrcAttr->bDebugFlag;
    pstDestAttr->bHDCPEnable          = pstSrcAttr->bHDCPEnable;
}

HI_VOID Hdmi_Drv2UnfAttr(HI_UNF_HDMI_ATTR_S *pstDestAttr,HDMI_APP_ATTR_S *pstSrcAttr)
{
    pstDestAttr->bEnableHdmi          = pstSrcAttr->bEnableHdmi;
    pstDestAttr->bEnableVideo         = pstSrcAttr->bEnableVideo;
    pstDestAttr->bEnableAudio         = pstSrcAttr->bEnableAudio;

    pstDestAttr->enVidOutMode         = pstSrcAttr->enVidOutMode;
    pstDestAttr->enDeepColorMode      = pstSrcAttr->enDeepColorMode;
    pstDestAttr->bxvYCCMode           = pstSrcAttr->bxvYCCMode;
    pstDestAttr->bEnableAviInfoFrame  = pstSrcAttr->bEnableAviInfoFrame;
    pstDestAttr->bEnableSpdInfoFrame  = pstSrcAttr->bEnableSpdInfoFrame;
    pstDestAttr->bEnableMpegInfoFrame = pstSrcAttr->bEnableMpegInfoFrame;
    pstDestAttr->bEnableAudInfoFrame  = pstSrcAttr->bEnableAudInfoFrame;

    //pstDestAttr->bDebugFlag           = pstSrcAttr->bDebugFlag;
    pstDestAttr->bHDCPEnable          = pstSrcAttr->bHDCPEnable;                                                                               
}


/**
\brief
\attention \n
\param
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_Init(void)
{
    HI_S32 Ret = 0;
    HDMI_INIT_S stHDMIInit;
    HDMI_GET_PROCID_S stHDMIProcID;
    HI_U32 taskreturn __attribute__((unused));
    if ( (g_HDMIDevFd != 0) && (g_HDMIDevFd != -1) )
    {
        /* HDMI have been initialized */
        return HI_SUCCESS;
    }

    INIT_LIST_HEAD(&g_pstHDMICallBackList.list);
    memset(&g_stHdmiCommUserParam, 0, sizeof(HDMI_COMM_USER_ATTR_S));
    HI_INFO_HDMI("Add HDMI Lock to deal with mutex\n");
    
    //g_stHDMIAudAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_I2S;
    
    g_HDMIDevFd = open(g_HDMIDevName, O_RDWR);
    if (g_HDMIDevFd <= 0)
    {
        HI_FATAL_HDMI("open HDMI err.\n");       
        //Ret = pthread_mutex_destroy(&g_HDMIMutex);
        return HI_ERR_HDMI_DEV_NOT_OPEN;
    }

    Ret = pthread_mutex_init(&g_HDMIMutex, NULL);
    HI_HDMI_LOCK();
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_INIT, &stHDMIInit);
    if ((Ret == HI_SUCCESS) || (Ret == HI_ERR_HDMI_CALLBACK_ALREADY))
    {
        if(Ret == HI_ERR_HDMI_CALLBACK_ALREADY)
        {
            HI_INFO_HDMI("HDMI has been inited already \n");
        }
        
        /*if hdmi alreadyinit , hdmi have already created thread*/
        if(HI_FALSE == g_stHdmiCommUserParam.bHdmiInit)
        {
            Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_PROCID, &stHDMIProcID);
            if(Ret == HI_SUCCESS)
            {
                g_HDMIProcID = stHDMIProcID.u32ProcID;
                /* create hdmi task */
                taskreturn = pthread_create(&g_stHdmiCommUserParam.tEventTimer, HI_NULL, 
                    Hdmi_Poll_Event_Thread, HI_NULL);
                g_stHdmiCommUserParam.bEnableTimer = HI_TRUE;
                HI_INFO_HDMI("timer task return:0x%x\n", taskreturn);
            }
            else
            {
                HI_ERR_HDMI("Error:HDMI Process is full,can't get process ID\n");
            }
        }
    }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    else if (Ret == HI_ERR_HDMI_CALLBACK_ALREADY)
    {
        HI_INFO_HDMI("\n Callback already \n");
        if(HI_FALSE == g_stHdmiCommUserParam.bHdmiInit)
        {                          
            Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_PROCID, &stHDMIProcID);
            if(Ret == HI_SUCCESS)
            {
                //do what??
                //HI_INFO_HDMI("Warring:HDMI has been inited already, New Callback funciton will be active\n");

                g_HDMIProcID = stHDMIProcID.u32ProcID;
                /* create hdmi task */
                taskreturn = pthread_create(&g_stHdmiCommUserParam.tEventTimer, HI_NULL, 
                    Hdmi_Poll_Event_Thread, HI_NULL);
                g_stHdmiCommUserParam.bEnableTimer = HI_TRUE;
                HI_INFO_HDMI("timer task return:0x%x\n", taskreturn);
            }
            else
            {
                HI_ERR_HDMI("Error:HDMI Process is full,can't get process ID");
            }
        }
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    else
    {
        HI_ERR_HDMI("HDMI Init Error:0x%x\n", Ret);
        HI_HDMI_UNLOCK();
        Ret = pthread_mutex_destroy(&g_HDMIMutex);
        return Ret;
    }
     
    

    g_stHdmiCommUserParam.bHdmiInit      = HI_TRUE;
    g_stHdmiCommUserParam.bHdmiExit      = HI_FALSE;
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

/**
\brief
\attention \n
\param
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_DeInit(void)
{
    HI_S32 Ret;
    HDMI_GET_PROCID_S stHDMIProcID;
    
    if ( ((g_HDMIDevFd == 0) || (g_HDMIDevFd == -1)) )
    {
        /* HDMI have no initialized */
        return HI_SUCCESS;
    }

    if (HI_FALSE == g_stHdmiCommUserParam.bHdmiInit)
    {
        return HI_SUCCESS;
    }

    //del procID

	//reserved
    //stHDMIProcID.enHdmi = xxx;
    stHDMIProcID.u32ProcID = g_HDMIProcID;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_RELEASE_PROCID, &stHDMIProcID);
    if(Ret != HI_SUCCESS)
    {
         HI_ERR_HDMI("release hdmi procID failed\n");
         return Ret;
    }
    g_HDMIProcID = HI_INVALID_HANDLE;

    /*exit thread*/
    g_stHdmiCommUserParam.bHdmiExit = HI_TRUE;

    if (g_stHdmiCommUserParam.bEnableTimer == HI_TRUE)
    {
        HI_INFO_HDMI("stop hdmi task\n");

        Ret = pthread_join(g_stHdmiCommUserParam.tEventTimer, NULL);
        g_stHdmiCommUserParam.bEnableTimer = HI_FALSE;
    }

    Ret = close(g_HDMIDevFd);
    g_HDMIDevFd = -1;
    memset(&g_stHdmiCommUserParam, 0, sizeof(HDMI_COMM_USER_ATTR_S));//clean
    
    /*destory mutex */
    Ret = pthread_mutex_destroy(&g_HDMIMutex);

    return Ret;
}

/**
\brief
\attention \n
\param[in] enHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_OPEN_PARA_S *pstOpenPara)
{
    HI_S32 Ret = 0;
    HDMI_OPEN_S stHDMIOpen;
    
    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstOpenPara);

    if (g_stHdmiChnUserParam[enHdmi].bOpen)
    {
        return HI_SUCCESS;
    }
    
    HI_HDMI_LOCK();
    memset(&stHDMIOpen, 0, sizeof(HDMI_OPEN_S));
    stHDMIOpen.enDefaultMode   = pstOpenPara->enDefaultMode;
    stHDMIOpen.enHdmi = enHdmi;
    stHDMIOpen.u32ProcID = g_HDMIProcID;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_OPEN, &stHDMIOpen);
    if (HI_SUCCESS != Ret)
    {
        HI_INFO_HDMI("hdmi open err:%d\n",Ret);
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    g_stHdmiChnUserParam[enHdmi].bOpen = HI_TRUE;//Enable HDMI thread
    return HI_SUCCESS;
}


/**
\brief
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 Ret = 0;
    HDMI_STOP_S  stHDMIStop;
    HDMI_CLOSE_S stHDMIClose;

    HDMI_CHECK_ID(enHdmi);
    if (g_stHdmiChnUserParam[enHdmi].bOpen == HI_FALSE)
    {
        return HI_SUCCESS;
    }
    
    if (HI_TRUE == g_stHdmiChnUserParam[enHdmi].bStart)
    {
        HI_HDMI_LOCK();
        memset(&stHDMIStop, 0, sizeof(HDMI_STOP_S));
        stHDMIStop.enHdmi          = enHdmi;
        Ret = ioctl(g_HDMIDevFd, CMD_HDMI_STOP, &stHDMIStop);
        g_stHdmiChnUserParam[enHdmi].bStart = HI_FALSE;
        HI_HDMI_UNLOCK();
    }

    if (HI_TRUE == g_stHdmiChnUserParam[enHdmi].bOpen)
    {
        HI_HDMI_LOCK();
        memset(&stHDMIClose, 0, sizeof(HDMI_CLOSE_S));
        stHDMIClose.enHdmi = enHdmi;
        Ret = ioctl(g_HDMIDevFd, CMD_HDMI_CLOSE, &stHDMIClose);
        if (Ret != HI_SUCCESS)
        {
            HI_HDMI_UNLOCK();
            return Ret;
        }
        g_stHdmiChnUserParam[enHdmi].bOpen = HI_FALSE;
        HI_HDMI_UNLOCK();
    }

    return HI_SUCCESS;
}

/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pCapability
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pstSinkCap)
{
    HI_S32 Ret = 0;
    HDMI_SINK_CAPABILITY_S stSinkCap;

    HDMI_CHECK_NULL_PTR(pstSinkCap);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stSinkCap, 0, sizeof(HDMI_SINK_CAPABILITY_S));
    stSinkCap.enHdmi = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SINK_CAPABILITY, &stSinkCap);
    if (Ret != HI_SUCCESS)
    {
        memset(pstSinkCap, 0, sizeof(HI_UNF_EDID_BASE_INFO_S));
        HI_HDMI_UNLOCK();
        return Ret;
    }
    memcpy(pstSinkCap, &(stSinkCap.SinkCap), sizeof(HI_UNF_EDID_BASE_INFO_S));
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
/**
\brief HDMI Audio sample_rate or interface change.
\attention call by mpi_hiao. why change?
\ 1) user set pass-through : spdif pass-through , hdmi pass-through
\param[in] hHdmi
\param[in] enSoundIntf
\param[in] enSampleRate
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_AudioChange(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr)
{
    HI_U32                 Ret = HI_SUCCESS;
    HDMI_AUDIO_ATTR_S      stHDMIAUDAttr;
    HI_UNF_HDMI_INFOFRAME_S     InfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe;
    HI_UNF_HDMI_ATTR_S stAttr;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if(HI_TRUE!=g_stHdmiChnUserParam[enHdmi].bOpen)
    {

        HI_WARN_HDMI("enHdmi:%d do NOT Start\n", enHdmi);
        return HI_FAILURE;
    }

    Ret = HI_MPI_HDMI_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AUDIO,&InfoFrame);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI infoframe failed\n");
        return HI_FAILURE;
    }
    pstAUDInfoframe = (HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(InfoFrame.unInforUnit.stAUDInfoFrame);

    Ret = HI_MPI_AO_HDMI_GetAttr(enHdmi, &stHDMIAUDAttr);
    Ret |= HI_MPI_HDMI_GetAttr(enHdmi, &stAttr);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI attribute failed\n");
        return HI_FAILURE;
    }
    
    if(pstHDMIAOAttr->enSoundIntf == HDMI_AUDIO_INTERFACE_I2S)
    {
        if(pstHDMIAOAttr->u32Channels > 2)
        {
            if(stAttr.bIsMultiChannel == HI_FALSE)
            {
                stAttr.bIsMultiChannel = HI_TRUE;
                Ret = HI_MPI_HDMI_SetAttr(enHdmi, &stAttr);
                if(Ret != HI_SUCCESS)
                {
                    HI_ERR_HDMI("set HDMI attribute failed\n");
                    return HI_FAILURE;                    
                }
            }
        }
        else
        {
            stAttr.bIsMultiChannel = HI_FALSE;
            Ret = HI_MPI_HDMI_SetAttr(enHdmi, &stAttr);
            if(Ret != HI_SUCCESS)
            {
                HI_ERR_HDMI("set HDMI attribute failed\n");
                return HI_FAILURE;                    
            }
        }
        
        if(pstAUDInfoframe->u32ChannelCount != stHDMIAUDAttr.u32Channels)
        {
            switch(pstHDMIAOAttr->u32Channels)
            {
                case 3:
                    pstAUDInfoframe->u32ChannelCount = 3;
                    pstAUDInfoframe->u32ChannelAlloc = 0x01;
                    break;
                case 6:
                    pstAUDInfoframe->u32ChannelCount = 6;
                    pstAUDInfoframe->u32ChannelAlloc = 0x0b;
                    break;                
                case 8:
                    pstAUDInfoframe->u32ChannelCount = 8;
                    pstAUDInfoframe->u32ChannelAlloc = 0x13;
                    break;
               default:
                    pstAUDInfoframe->u32ChannelCount = 2;
                    pstAUDInfoframe->u32ChannelAlloc = 0x00;
                    break;
            }
            Ret = HI_MPI_HDMI_SetInfoFrame(enHdmi, &InfoFrame);  
        }         
    }
    
    if(pstHDMIAOAttr->enSoundIntf == stHDMIAUDAttr.enSoundIntf && 
       pstHDMIAOAttr->enSampleRate == stHDMIAUDAttr.enSampleRate)
    {
        return HI_SUCCESS;
    }
        
    if (HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf 
       || HDMI_AUDIO_INTERFACE_SPDIF== pstHDMIAOAttr->enSoundIntf
       ||HDMI_AUDIO_INTERFACE_HBR== pstHDMIAOAttr->enSoundIntf)
    {
        stHDMIAUDAttr.enSoundIntf = pstHDMIAOAttr->enSoundIntf;
    }
    else
    {
        HI_ERR_HDMI("Error input Audio interface(%d)\n",pstHDMIAOAttr->enSoundIntf);
        return HI_FAILURE;
    }
    switch (pstHDMIAOAttr->enSampleRate)
    {
        case HI_UNF_SAMPLE_RATE_32K:
        case HI_UNF_SAMPLE_RATE_44K:
        case HI_UNF_SAMPLE_RATE_48K:
        case HI_UNF_SAMPLE_RATE_88K:
        case HI_UNF_SAMPLE_RATE_96K:
        case HI_UNF_SAMPLE_RATE_176K:
        case HI_UNF_SAMPLE_RATE_192K:
            stHDMIAUDAttr.enSampleRate = pstHDMIAOAttr->enSampleRate;
            break;
        default:
            HI_ERR_HDMI("Error input Audio Frequency(%d)\n",pstHDMIAOAttr->enSampleRate);
            return HI_FAILURE;
    }

    HI_INFO_HDMI("set HDMI Audio input chage interface(%d) samplerate(%d)\n", stHDMIAUDAttr.enSoundIntf,stHDMIAUDAttr.enSampleRate);
    /* Set Audio infoframe */
    /* New function to set Audio Infoframe */
    /* HDMI requires the CT, SS and SF fields to be set to 0 ("Refer to Stream Header") 
       as these items are carried in the audio stream.*/
    memset(&InfoFrame, 0, sizeof(InfoFrame));
    InfoFrame.enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
    pstAUDInfoframe->u32ChannelCount      = pstHDMIAOAttr->u32Channels;
    pstAUDInfoframe->enCodingType         = HDMI_AUDIO_CODING_REFER_STREAM_HEAD;
    pstAUDInfoframe->u32SampleSize        = HI_UNF_HDMI_DEFAULT_SETTING;
    pstAUDInfoframe->u32SamplingFrequency = HI_UNF_HDMI_DEFAULT_SETTING;
    switch(pstHDMIAOAttr->u32Channels)     //HDMI channel map
    {
        case 3:
            pstAUDInfoframe->u32ChannelAlloc = 0x01;
            break;
        case 6:
            pstAUDInfoframe->u32ChannelAlloc = 0x0b;
            break;                
        case 8:
            pstAUDInfoframe->u32ChannelAlloc = 0x13;
            break;
        default:
            pstAUDInfoframe->u32ChannelAlloc = 0x00;
            break;
    }
    pstAUDInfoframe->u32LevelShift        = 0;
    pstAUDInfoframe->u32DownmixInhibit    = HI_FALSE;
    HI_INFO_HDMI("***HI_UNF_HDMI_SetInfoFrame for AUDIO Infoframe\n");
    Ret = HI_MPI_HDMI_SetInfoFrame(enHdmi, &InfoFrame);  
   
    Ret = HI_MPI_AO_HDMI_GetAttr(enHdmi, &stHDMIAUDAttr);
    stHDMIAUDAttr.enSoundIntf = pstHDMIAOAttr->enSoundIntf;
    stHDMIAUDAttr.enSampleRate = pstHDMIAOAttr->enSampleRate;
    
    if((pstHDMIAOAttr->u32Channels > 2) 
    &&(HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf))
    {
        stAttr.bIsMultiChannel = HI_TRUE;
    }
    Ret = HI_MPI_HDMI_SetAttr(enHdmi, &stAttr);
    Ret |= HI_MPI_AO_HDMI_SetAttr(enHdmi, &stHDMIAUDAttr);
    
    return Ret;
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pstAttr
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = 0;
    HDMI_PORT_ATTR_S stPortAttr;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAttr);
    
    HI_HDMI_LOCK();
    memset(&stPortAttr, 0, sizeof(HDMI_PORT_ATTR_S));
    stPortAttr.enHdmi = enHdmi;
    //s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_ATTR, &stPortAttr);
    //stPortAttr.stHDMIAttr.stAttr= *pstAttr;
    Hdmi_Unf2DrvAttr(&stPortAttr.stHdmiAppAttr,pstAttr);
    ////set auido mode in kernel. not in mpi or unf
    //stPortAttr.stHDMIAttr.enSoundIntf = HDMI_AUDIO_INTERFACE_BUTT;
    //g_stHDMIAudAttr.enSampleRate = pstAttr->enSampleRate;

    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_ATTR, &stPortAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return s32Ret;
    }
    HI_HDMI_UNLOCK();
   
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    /* Warring: Below is just for test!!!! */
    if(pstAttr->bDebugFlag == 0x3fc)
    {
        //create HPD Event!!
        HI_MPI_UNF_HDMI_GenEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
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
HI_S32 HI_MPI_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = 0;
    HDMI_PORT_ATTR_S stPortAttr;

    HDMI_CHECK_NULL_PTR(pstAttr);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stPortAttr, 0, sizeof(HDMI_PORT_ATTR_S));
    memset(pstAttr,0,sizeof(HI_UNF_HDMI_ATTR_S));
    stPortAttr.enHdmi = enHdmi;
    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_ATTR, &stPortAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return s32Ret;
    }
    Hdmi_Drv2UnfAttr(pstAttr,&stPortAttr.stHdmiAppAttr);
    //*pstAttr = stPortAttr.stHDMIAttr.stAttr;
    
    HI_HDMI_UNLOCK();
    return s32Ret;
}
/**
\brief
\attention \n
\param[in] hHdmi
\param[in] pCECCmd
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd)
{
    HI_S32 Ret = 0;
    HDMI_CEC_S stCEC;

    HDMI_CHECK_NULL_PTR(pCECCmd);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stCEC, 0, sizeof(HDMI_CEC_S));
    stCEC.enHdmi = enHdmi;
    memcpy(&(stCEC.CECCmd), pCECCmd, sizeof(HI_UNF_HDMI_CEC_CMD_S));
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_CEC, &stCEC);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout)
{
    HI_S32 Ret = 0;
    HDMI_CEC_S stCEC;

    HDMI_CHECK_NULL_PTR(pCECCmd);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stCEC, 0, sizeof(HDMI_CEC_S));
    stCEC.enHdmi = enHdmi;
    stCEC.timeout = timeout;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_CEC, &stCEC);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    memcpy(pCECCmd, &(stCEC.CECCmd), sizeof(HI_UNF_HDMI_CEC_CMD_S));
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

HI_VOID *Hdmi_CEC_Event_Thread(HI_VOID* pParam)
{
    HI_S32 Ret;
    HI_UNF_HDMI_CEC_CMD_S  stCECCmd;
    
  
    while(HI_TRUE == g_stCECAttr.bCECEnable)
    {
        Ret = HI_MPI_HDMI_GetCECCommand(g_stCECAttr.enHdmi, &stCECCmd, 0);
        if (Ret == HI_SUCCESS)
        {
            g_stCECAttr.pCECCallback(g_stCECAttr.enHdmi, &stCECCmd, HI_NULL);
        }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        else
        {
            HI_INFO_HDMI("Get CEC cmd failed\n");
        }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
        usleep(80*1000);
    }

    return HI_NULL;
}

HI_S32 HI_UNF_HDMI_RegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HI_S32 Ret = 0;
    
    HDMI_CheckChnOpen(enHdmi);
    
    memset(&g_stCECAttr, 0, sizeof(HDMI_CEC_ATTR_S));
    g_stCECAttr.enHdmi = enHdmi;
    g_stCECAttr.pCECCallback = pCECCallback;
    
    Ret = pthread_create(&g_stCECAttr.CECTread, HI_NULL,
                         Hdmi_CEC_Event_Thread, HI_NULL);

    if (Ret == HI_SUCCESS)
    {
        g_stCECAttr.bCECEnable = HI_TRUE;
    }
    
    return Ret;
}

HI_S32 HI_UNF_HDMI_UnRegCECCallBackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CECCALLBACK pCECCallback)
{
    HDMI_CHECK_ID(enHdmi);
    
    if(pCECCallback != g_stCECAttr.pCECCallback)
    {
        HI_ERR_HDMI("hand err :0x%x != 0x%x\n",pCECCallback,g_stCECAttr.pCECCallback);
        return HI_FAILURE;
    }

    g_stCECAttr.bCECEnable = HI_FALSE;
    pthread_join(g_stCECAttr.CECTread, NULL);
    memset(&g_stCECAttr, 0, sizeof(HDMI_CEC_ATTR_S));
    return HI_SUCCESS;
}


HI_S32 HI_MPI_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus)
{
    HI_S32 Ret = 0;
    HDMI_CEC_STATUS CECStatus;

    HDMI_CHECK_NULL_PTR(pStatus);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&CECStatus, 0, sizeof(HDMI_CEC_STATUS));
    CECStatus.enHdmi = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_CECSTATUS, &CECStatus);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    memcpy(pStatus, &(CECStatus.stStatus), sizeof(HI_UNF_HDMI_CEC_STATUS_S));
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_CEC_Enable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 Ret = 0;
    HI_U32 u32CECFlag;
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    u32CECFlag = 1;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_CEC_ENABLE, &u32CECFlag);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_MPI_HDMI_CEC_Disable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 Ret = 0;
    HI_U32 u32CECFlag;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    u32CECFlag = 0;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_CEC_DISABLE, &u32CECFlag);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
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
HI_S32 HI_MPI_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 Ret = 0;
    HDMI_INFORFRAME_S stInfoFrame;

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stInfoFrame, 0, sizeof(HDMI_INFORFRAME_S));
    stInfoFrame.enHdmi          = enHdmi;
    stInfoFrame.enInfoFrameType = pstInfoFrame->enInfoFrameType;
    memcpy(&(stInfoFrame.InfoFrame), pstInfoFrame, sizeof(HI_UNF_HDMI_INFOFRAME_S));
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_INFORFRAME, &stInfoFrame);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
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
HI_S32 HI_MPI_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 Ret = 0;
    HDMI_INFORFRAME_S stInfoFrame;

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stInfoFrame, 0, sizeof(HDMI_INFORFRAME_S));
    stInfoFrame.enHdmi          = enHdmi;
    stInfoFrame.enInfoFrameType = enInfoFrameType;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_INFORFRAME, &stInfoFrame);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    memcpy(pstInfoFrame, &(stInfoFrame.InfoFrame), sizeof(HI_UNF_HDMI_INFOFRAME_S));
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

/**
\brief HOT-PLUG Callback function, it will invoke Customer callback function.
\attention Customer callback function is register in HI_UNF_HDMI_Init
\param[in] none
\retval none
\see \n
*/
static void HI_MPI_UNF_HDMI_GenEvent(HI_UNF_HDMI_EVENT_TYPE_E event)
{
    HDMI_CALLBACK_NODE_S *tmp;
    List *pos;

    HI_INFO_HDMI("HDMI EVENT TYPE:0x%x\n", event);

    list_for_each(pos, &g_pstHDMICallBackList.list)
    {           
        tmp = list_entry(pos, HDMI_CALLBACK_NODE_S, list);
        tmp->stCallbackfunc.pfnHdmiEventCallback(
            event, tmp->stCallbackfunc.pPrivateData);
    }
}


/**
\brief Start HDMI output. It should be invoked within User callback.
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 Ret = 0;
    HDMI_START_S stHDMIStart;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (g_stHdmiChnUserParam[enHdmi].bStart == HI_TRUE)
    {
        HI_INFO_HDMI("HI_MPI_HDMI_Start Already Start before!\n");
    }

    HI_HDMI_LOCK();
    memset(&stHDMIStart, 0, sizeof(HDMI_START_S));
    stHDMIStart.enHdmi          = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_START, &stHDMIStart);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    g_stHdmiChnUserParam[enHdmi].bStart = HI_TRUE;
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

/**
\brief Stop HDMI output
\attention \n
\param[in] hHdmi
\retval none
\see \n
*/
HI_S32 HI_MPI_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 Ret = 0;
    HDMI_STOP_S stHDMIStop;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (g_stHdmiChnUserParam[enHdmi].bStart== HI_FALSE)
    {
        return HI_SUCCESS;
    }

    HI_HDMI_LOCK();
    if (HI_TRUE != g_stHdmiChnUserParam[enHdmi].bStart)
    {
        HI_HDMI_UNLOCK();
        return HI_SUCCESS;
    }
    memset(&stHDMIStop, 0, sizeof(HDMI_STOP_S));
    stHDMIStop.enHdmi = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_STOP, &stHDMIStop);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    g_stHdmiChnUserParam[enHdmi].bStart = HI_FALSE;
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

/**
\brief Set DeepColor mode
\attention \n
\param[in] enHdmi
\param[in] enDeepColor, please refer to:HI_UNF_HDMI_DEEP_COLOR_E
\retval HI_SUCCESS
\see \n
*/
HI_S32 HI_MPI_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_S32 Ret = 0;
    HDMI_DEEPCOLORC_S stHDMIDeepcolor;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stHDMIDeepcolor, 0, sizeof(HDMI_DEEPCOLORC_S));
    stHDMIDeepcolor.enHdmi          = enHdmi;
    stHDMIDeepcolor.enDeepColor     = enDeepColor;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_DEEPCOLOR, &stHDMIDeepcolor);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
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
HI_S32 HI_MPI_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    HI_S32 Ret = 0;
    HDMI_DEEPCOLORC_S stHDMIDeepcolor;

    HDMI_CHECK_NULL_PTR(penDeepColor);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stHDMIDeepcolor, 0, sizeof(HDMI_DEEPCOLORC_S));
    stHDMIDeepcolor.enHdmi = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_DEEPCOLOR, &stHDMIDeepcolor);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    *penDeepColor = stHDMIDeepcolor.enDeepColor;

    HI_HDMI_UNLOCK();
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
HI_S32 HI_MPI_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_S32 Ret = 0;
    HDMI_SET_XVYCC_S stxvYCC;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stxvYCC, 0, sizeof(HDMI_SET_XVYCC_S));
    stxvYCC.enHdmi          = enHdmi;
    stxvYCC.xvYCCEnable     = bEnable;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_XVYCC, &stxvYCC);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
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
HI_S32 HI_MPI_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    HI_S32 Ret = 0;
    HDMI_AVMUTE_S stAVMute;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stAVMute, 0, sizeof(HDMI_AVMUTE_S));
    stAVMute.enHdmi          = enHdmi;
    stAVMute.AVMuteEnable    = bAvMute;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_AVMUTE, &stAVMute);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_S32 HI_MPI_HDMI_Pre_SetFormat(HI_DRV_DISP_FMT_E enEncodingFormat)
{
    HI_S32 Ret = 0;
    HDMI_PREVIDEOTIMING_S stPreVideoTiming;
    HI_UNF_HDMI_ID_E  enHdmi = HI_UNF_HDMI_ID_0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stPreVideoTiming, 0, sizeof(HDMI_PREVIDEOTIMING_S));
    stPreVideoTiming.enHdmi          = HI_UNF_HDMI_ID_0;
    stPreVideoTiming.VideoTiming     = enEncodingFormat;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_PREVTIMING, &stPreVideoTiming);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

HI_S32 HI_MPI_HDMI_AVMute(void)
{
    HI_S32 Ret = 0;

    Ret = HI_MPI_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_TRUE);

    return Ret;
}

HI_S32 HI_MPI_HDMI_AVUnMute(void)
{
    HI_S32 Ret = 0;

    Ret = HI_MPI_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_FALSE);

    return Ret;
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_S32 HI_MPI_HDMI_SetFormat(HI_DRV_DISP_FMT_E enEncodingFormat)
{
    HI_S32 Ret = 0;
    HDMI_VIDEOTIMING_S stVideoTiming;
    HI_UNF_HDMI_ID_E  enHdmi = HI_UNF_HDMI_ID_0;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    HI_HDMI_LOCK();
    memset(&stVideoTiming, 0, sizeof(HDMI_VIDEOTIMING_S));
    stVideoTiming.enHdmi          = HI_UNF_HDMI_ID_0;
    stVideoTiming.VideoTiming     = enEncodingFormat;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_VIDEO_TIMING, &stVideoTiming);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();
    return HI_SUCCESS;
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_S32 HI_MPI_HDMI_PlayStus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus)
{
    HI_S32 Ret = 0;
    HDMI_PLAYSTAUS_S PlayStaus;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if(g_stHdmiChnUserParam[enHdmi].bOpen == HI_TRUE)
    {
        memset(&PlayStaus, 0, sizeof(HDMI_PLAYSTAUS_S));
        PlayStaus.enHdmi = enHdmi;
        Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_HDMI_PLAYSTAUS, &PlayStaus);
        if (Ret != HI_SUCCESS)
        {
            *pu32Stutus = HI_FALSE;
             return Ret;
        }
        *pu32Stutus = PlayStaus.u32PlayStaus;
    }
    else
    {
        *pu32Stutus = 0;
    }

    return HI_SUCCESS;
}
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

HI_S32 HI_MPI_HDMI_Force_GetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 Ret = 0;
    HDMI_EDID_S EDIDData;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(u8Edid);
    HDMI_CHECK_NULL_PTR(u32EdidLength);

    *u32EdidLength = 0;

    HI_HDMI_LOCK();
    memset(&EDIDData, 0, sizeof(HDMI_EDID_S));
    EDIDData.enHdmi = enHdmi;
    Ret = ioctl(g_HDMIDevFd, CMD_HDMI_FORCE_GET_EDID, &EDIDData);
    if (Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return Ret;
    }
    HI_HDMI_UNLOCK();

    if(EDIDData.u8EdidValid == HI_TRUE)
    {
        *u32EdidLength = EDIDData.u32Edidlength;
        memcpy(u8Edid, EDIDData.u8Edid, EDIDData.u32Edidlength);
    }

    return Ret;
}

HI_S32 HI_MPI_HDMI_ReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 ret = 0;
    HDMI_EDID_S EdidData;
    
    HDMI_CHECK_NULL_PTR(u8Edid);
    HDMI_CHECK_NULL_PTR(u32EdidLength);
    
    HI_HDMI_LOCK();
    EdidData.enHdmi=HI_UNF_HDMI_ID_0;
    memset(&EdidData, 0, sizeof(HDMI_EDID_S));        
    *u32EdidLength = 0;

    ret = ioctl(g_HDMIDevFd, CMD_HDMI_READ_EDID, &EdidData);
    if (ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        HI_ERR_HDMI("forec get edid fail!\n");
        return ret;
    }
    HI_HDMI_UNLOCK();

    if(EdidData.u8EdidValid == HI_TRUE)
    {
        *u32EdidLength = EdidData.u32Edidlength;
        memcpy(u8Edid, EdidData.u8Edid, EdidData.u32Edidlength);
    }  
    return ret;
}

HI_S32 HI_MPI_HDMI_RegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    //return HI_SUCCESS;
    HI_S32 s32Ret = HI_SUCCESS;
    //HDMI_REGCALLBACKFUNC_S stRegCallbackFunc;
    HDMI_CALLBACK_NODE_S *tmp = NULL;
    //List *pos, *q;
    //unsigned int i;
    
    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstCallbackFunc);
        
    HI_INFO_HDMI("enHdmi %d,procID %d----\n",enHdmi,g_HDMIProcID);
    HI_HDMI_LOCK();
    if(NULL != pstCallbackFunc->pfnHdmiEventCallback)
    {
        tmp = (HDMI_CALLBACK_NODE_S *)HI_MALLOC(HI_ID_HDMI,sizeof(HDMI_CALLBACK_NODE_S));
        //tmp = (HDMI_CALLBACK_NODE_S *)malloc(sizeof(HDMI_CALLBACK_NODE_S));
        if(NULL == tmp)
        {
            HI_ERR_HDMI("Callback malloc failed\n");
            HI_HDMI_UNLOCK();
            return HI_FAILURE;
        }
        
        //tmp->enHdmi = enHdmi;
        tmp->stCallbackfunc = *pstCallbackFunc;
    }    
    else
    {
        HI_ERR_HDMI("Callbackaddr is NULL!\n");
        HI_HDMI_UNLOCK();
        return HI_FAILURE;
    }
    
    list_add_tail(&(tmp->list), &(g_pstHDMICallBackList.list));
    HI_HDMI_UNLOCK();

    return s32Ret;
}

HI_S32 HI_MPI_HDMI_UnRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    HDMI_CALLBACK_NODE_S *tmp;
    List *pos, *q;

    HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstCallbackFunc);
    usleep(10*1000);    
    list_for_each_safe(pos, q,&g_pstHDMICallBackList.list)
    {
        tmp= list_entry(pos, HDMI_CALLBACK_NODE_S, list);
        if(tmp->stCallbackfunc.pfnHdmiEventCallback 
            == pstCallbackFunc->pfnHdmiEventCallback)
        {
            list_del(pos);
            HI_FREE(HI_ID_HDMI,tmp);
            tmp = NULL;
            return HI_SUCCESS;
        }
    }

    HI_ERR_HDMI("CallbackFunc:No this Callbackfunc\n");
    return HI_FAILURE;
}


HI_S32 HI_MPI_HDMI_LoadHDCPKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_S32 s32Ret = 0;
    HDMI_LOADKEY_S stLoadKey;

	HDMI_CHECK_ID(enHdmi);
    HDMI_CHECK_NULL_PTR(pstLoadKey);
    
    HI_HDMI_LOCK();
    stLoadKey.enHdmi=enHdmi;
    stLoadKey.stLoadKey = *pstLoadKey;
    
    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_LOADKEY, &stLoadKey);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI(" Ioctrl Load Key failed!\n");
        HI_HDMI_UNLOCK();
        return s32Ret;
    }
    HI_HDMI_UNLOCK();
    return s32Ret;
}

HI_S32 HI_MPI_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus)
{
    HI_S32 s32Ret = 0;
    HDMI_STATUS_S stHdmiStatus;

    HDMI_CHECK_ID(enHdmi);
    //HDMI_CheckChnOpen(enHdmi);
	HDMI_CHECK_NULL_PTR(pHdmiStatus);

    HI_HDMI_LOCK();
    memset(&stHdmiStatus, 0, sizeof(HDMI_STATUS_S));
    stHdmiStatus.enHdmi = enHdmi;
    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_STATUS, &stHdmiStatus);
    if (s32Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return s32Ret;
    }
    
    memcpy(pHdmiStatus, &(stHdmiStatus.stStatus), sizeof(HI_UNF_HDMI_STATUS_S));
    HI_HDMI_UNLOCK();
    return s32Ret;

}


HI_S32 HI_MPI_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret = 0;
    HDMI_DELAY_S stHdmiDelay;
            
    HDMI_CHECK_ID(enHdmi);
    //HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDelay);

    HI_HDMI_LOCK();
    memset(&stHdmiDelay,0,sizeof(HDMI_DELAY_S));
    stHdmiDelay.enHdmi = enHdmi;
    
    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_GET_DELAY , &stHdmiDelay);
    if (s32Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return s32Ret;
    }

    memcpy(pstDelay, &stHdmiDelay.stDelay, sizeof(HI_UNF_HDMI_DELAY_S));

    HI_HDMI_UNLOCK();
    return s32Ret;

}


HI_S32 HI_MPI_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret = 0;
    HDMI_DELAY_S stHdmiDelay;
            
    HDMI_CHECK_ID(enHdmi);
    //HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstDelay);

    if((pstDelay->u32FmtDelay > MAX_DELAY_TIME_MS) || (pstDelay->u32MuteDelay > MAX_DELAY_TIME_MS))
    {
        HI_ERR_HDMI("Delay Time fmt:%d Mute:%d Over Range:%d \n",pstDelay->u32FmtDelay,pstDelay->u32MuteDelay,MAX_DELAY_TIME_MS);
        return HI_ERR_HDMI_INVALID_PARA;
    }

    HI_HDMI_LOCK();
    stHdmiDelay.enHdmi = enHdmi;
    memcpy(&stHdmiDelay.stDelay, pstDelay, sizeof(HI_UNF_HDMI_DELAY_S));

    s32Ret = ioctl(g_HDMIDevFd, CMD_HDMI_SET_DELAY , &stHdmiDelay);
    if (s32Ret != HI_SUCCESS)
    {
        HI_HDMI_UNLOCK();
        return s32Ret;
    }

    HI_HDMI_UNLOCK();
    return s32Ret;
}



/*------------------------------END--------------------------------*/
