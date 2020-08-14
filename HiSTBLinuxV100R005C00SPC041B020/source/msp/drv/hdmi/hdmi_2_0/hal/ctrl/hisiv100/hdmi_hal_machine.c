/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_machine.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/

#include <linux/jiffies.h>
#include <linux/delay.h>
#include "hdmi_hal_machine.h"

#define MACHINE_MAX_ID_NUM          100
#define MACHIME_MIN_TINTERVAL       5

#define MACHINE_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_ERR("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define MACHINE_INIT_CHK(pstMachInfo) do{\
	MACHINE_NULL_CHK(pstMachInfo);\
	if(pstMachInfo->bInit != HI_TRUE)\
	{HDMI_ERR("Machine module didn't init!\n");\
	return HI_FAILURE;}\
}while(0)

#define MACHINE_ID_CHK(u32MachId) do{\
    if(u32MachId >= MACHINE_MAX_ID_NUM )\
    {\
        HDMI_WARN("can't find machine id = %u,fail\n!",u32MachId);\
        return HI_FAILURE;\
    }\
}while(0)


typedef struct
{
    HDMI_MUTEX				stMachMutex;			/* lock to avoid competition*/
	HI_BOOL					bInit;				    /* is the struct init */
	HI_U32					u32Total;			    /* total state machine in the list */
	HDMI_MACH_ELEM_S        astMachElem[MACHINE_MAX_ID_NUM];             /* machine element data */

}HDMI_MACH_INFO_S;

static HDMI_MACH_INFO_S    s_MachineInfo;


/**
\brief	  machine init 
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachInit(HI_VOID)
{
    
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo; 

    if (!pstMachInfo->bInit)
    {
        HDMI_INIT_MUTEX(pstMachInfo->stMachMutex);
        HDMI_MEMSET(pstMachInfo->astMachElem,0,MACHINE_MAX_ID_NUM*sizeof(HDMI_MACH_ELEM_S));
        pstMachInfo->u32Total   = 0;
        pstMachInfo->bInit      = HI_TRUE; 
    }
    
    return HI_SUCCESS;
}


/**
\brief	  machine deinit 
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachDeInit(HI_VOID)
{
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;
    
    MACHINE_INIT_CHK(pstMachInfo);
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    HDMI_MEMSET(pstMachInfo->astMachElem,0,sizeof(pstMachInfo->astMachElem));
    pstMachInfo->u32Total   = 0;
    pstMachInfo->bInit      = HI_FALSE;
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);

	return HI_SUCCESS;
    
}

/**
\brief	  machine invoke 
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachInvoke(HI_VOID)
{
    HI_U32              i = 0;
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;
    
    MACHINE_INIT_CHK(pstMachInfo);

    if ( pstMachInfo->u32Total == 0)
    {
        msleep(20);
    }
    else
    {
        for (i=0,pstTmpElem=&pstMachInfo->astMachElem[0];
            i < MACHINE_MAX_ID_NUM && pstTmpElem;
            ++i,pstTmpElem=&pstMachInfo->astMachElem[i])
        
        {
            if (pstTmpElem->stMachRun.bValidId && pstTmpElem->stMachRun.bEnable)
            {
                if (jiffies - pstTmpElem->stMachRun.u64LastTime > pstTmpElem->stMachCtrl.u64Interval)
                {
                    pstTmpElem->stMachCtrl.pfnCallBack(pstTmpElem->stMachCtrl.pCbData);
                    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
                    pstTmpElem->stMachRun.u64LastTime = jiffies;
                    pstTmpElem->stMachRun.au64Timestamp[pstTmpElem->stMachRun.u32StampPtr] = jiffies;
                    pstTmpElem->stMachRun.u32StampPtr = (pstTmpElem->stMachRun.u32StampPtr + 1) % HDMI_MACH_MAX_STAMPE_NUM;
                    pstTmpElem->stMachRun.u32RunCnt++;
                    HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
                }
            }
        }
        
    }
    return HI_SUCCESS;
}

/**
\brief	  regiser a machine
\param[in]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the init inforamtion of a new machine
\param[out]	@pu32MachId 			pointer to a machine ID which is malloced
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachRegister(HDMI_MACH_CTRL_S *pstMachCtrl,HI_U32 *pu32MachId)
{
    HI_U32              i = 0;
    HI_S32              s32NameLen = 0;
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;
    
    MACHINE_NULL_CHK(pstMachCtrl);
    MACHINE_INIT_CHK(pstMachInfo);
    
	if (pstMachInfo->u32Total >= MACHINE_MAX_ID_NUM)
	{
        HDMI_WARN("FULL num=%u!You can't register machine any more! \n",MACHINE_MAX_ID_NUM);
    	s32Ret = HI_FAILURE;
	}
	else
    {     
        HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
        
        for (i=0,pstTmpElem=&pstMachInfo->astMachElem[0];
            i < MACHINE_MAX_ID_NUM && pstTmpElem;
            ++i,pstTmpElem=&pstMachInfo->astMachElem[i])
        {
            
            if (!pstTmpElem->stMachRun.bValidId)
            {
                HDMI_MEMSET(&pstTmpElem->stMachRun , 0 , sizeof(HDMI_MACH_RUN_S));
            	HDMI_MEMCPY(&pstTmpElem->stMachCtrl , pstMachCtrl , sizeof(HDMI_MACH_CTRL_S));

                s32NameLen = HDMI_STRLEN(pstTmpElem->stMachCtrl.pu8Name);
                s32NameLen = (s32NameLen<HDMI_MACH_MAX_NAME_SIZE) ? s32NameLen : (HDMI_MACH_MAX_NAME_SIZE-1);
                HDMI_STRNCPY(pstTmpElem->stMachRun.au8Name,pstTmpElem->stMachCtrl.pu8Name,s32NameLen);
                pstTmpElem->stMachCtrl.pu8Name = pstTmpElem->stMachRun.au8Name;
                
                pstMachInfo->u32Total++;
                pstTmpElem->stMachRun.bValidId = HI_TRUE;
            	*pu32MachId = i;
            	break;
            }

        }

    	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
    	
    	if (i < MACHINE_MAX_ID_NUM)
    	{
            HDMI_INFO("register new machine id=%u success!\n",*pu32MachId);
        	s32Ret = HI_SUCCESS;
    	}
    	else
    	{
            HDMI_INFO("register new machine id=%u fail!\n",*pu32MachId);
        	s32Ret = HI_FAILURE;
    	}
    	
    }
    return s32Ret;
}


/**
\brief	  un-regiser a machine
\param[in]	@u32MachId                    the machine ID which expected to unregister
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachUnRegister(HI_U32 u32MachId)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;
    
    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
    HDMI_MEMSET(&pstTmpElem->stMachRun , 0 , sizeof(HDMI_MACH_RUN_S));
	HDMI_MEMSET(&pstTmpElem->stMachCtrl , 0 , sizeof(HDMI_MACH_CTRL_S));
    pstMachInfo->u32Total--;
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
}



/**
\brief	  config a machine
\param[out]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the re-config-inforamtion of exist-machine
\param[in]	@pu32MachId 			the machine ID .
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachCfgGet( HI_U32 u32MachId,HDMI_MACH_CTRL_S *pstMachCtrl)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;

    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
	HDMI_MEMCPY(pstMachCtrl , &pstTmpElem->stMachCtrl , sizeof(HDMI_MACH_CTRL_S));
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
    
}


/**
\brief	  config a machine
\param[in]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the re-config-inforamtion of exist-machine
\param[in]	@pu32MachId 			the machine ID .
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachCfgSet( HI_U32 u32MachId,HDMI_MACH_CTRL_S *pstMachCtrl)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;

    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
	HDMI_MEMCPY(&pstTmpElem->stMachCtrl , pstMachCtrl , sizeof(HDMI_MACH_CTRL_S));
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
    
}


/**
\brief	  start a machine 
\param[in]	@u32MachId                    the machine ID which expected to start
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStart(HI_U32 u32MachId)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;

    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
    pstTmpElem->stMachRun.bEnable       = HI_TRUE;
    pstTmpElem->stMachRun.u64EnableTime = jiffies;
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
}

/**
\brief	  stop a machine 
\param[in]	@u32MachId                    the machine ID which expected to stop
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStop(HI_U32 u32MachId)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;

    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
    pstTmpElem->stMachRun.bEnable           = HI_FALSE;
    pstTmpElem->stMachRun.u64DisableTime    = jiffies;
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
}

/**
\brief	  get total machine number
\param[out]	@pstStat                     pointer to user's memory ,initial & total  status 
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStatusGet(HDMI_MACH_STAT_S *pstStat)
{
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;
    
    MACHINE_NULL_CHK(pstStat);
    MACHINE_INIT_CHK(pstMachInfo);

    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);

    pstStat->bInit      = pstMachInfo->bInit;
    pstStat->u32Total   = pstMachInfo->u32Total;

    HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);

    return HI_SUCCESS;
    
}

/**
\brief	  get a machine status
\param[in]	@u32MachId                  the machine ID
\param[out]	@pstStatus                      a machine status
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachElemStatusGet(HI_U32 u32MachId,HDMI_MACH_ELEM_STAT_S *pstStatus)
{
    HDMI_MACH_ELEM_S    *pstTmpElem = NULL;
    HDMI_MACH_INFO_S    *pstMachInfo = &s_MachineInfo;

    MACHINE_INIT_CHK(pstMachInfo);
    MACHINE_ID_CHK(u32MachId);
    
    HDMI_MUTEX_LOCK(pstMachInfo->stMachMutex);
    pstTmpElem = &pstMachInfo->astMachElem[u32MachId];
    HDMI_MEMCPY(&pstStatus->stMachCtrl, &pstTmpElem->stMachCtrl, sizeof(HDMI_MACH_CTRL_S));
    HDMI_MEMCPY(&pstStatus->stMachRun, &pstTmpElem->stMachRun, sizeof(HDMI_MACH_RUN_S));
	HDMI_MUTEX_UNLOCK(pstMachInfo->stMachMutex);
	
	return HI_SUCCESS;
}


/**
\brief	  get current time
\param     none
\retval     jiffies time
*/
HI_U64 HAL_HDMI_MachMsGet(HI_VOID)
{
    return jiffies;
}            



