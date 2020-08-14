/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_machine.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_MACHINE_H__
#define __HDMI_HAL_MACHINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#endif

#define HDMI_MACH_MAX_STAMPE_NUM   6
#define HDMI_MACH_MAX_NAME_SIZE   15


typedef HI_VOID (*PFN_MACH_CALLBACK)(HI_VOID *pData);

typedef struct
{
    HI_U8                   *pu8Name;           /* machine name .*/
    HI_U64                  u64Interval;        /* machine run interval time,in ms .*/
    HI_VOID                 *pCbData;           /* machine call back data(an object pointer ) */
    PFN_MACH_CALLBACK       pfnCallBack;        /* machine call back function .*/
}HDMI_MACH_CTRL_S;

typedef struct
{
    HI_BOOL                 bValidId;
    HI_BOOL                 bEnable;            /* flag of machine is enable */
    HI_U64                  u64EnableTime;      /* machine's enable time */
    HI_U64                  u64DisableTime;     /* machine's disable time */    
    HI_U64                  u64LastTime;        /* machine's last time */
    HI_U32                  u32RunCnt;          /* machine's pfCallBack run count */ 
    HI_U32                  u32StampPtr;
    HI_U64                  au64Timestamp[HDMI_MACH_MAX_STAMPE_NUM]; /* run timestamp */
    HI_U8                   au8Name[HDMI_MACH_MAX_NAME_SIZE];
}HDMI_MACH_RUN_S;

typedef struct
{
    HDMI_MACH_RUN_S         stMachRun;          /* machine's running status. */       
    HDMI_MACH_CTRL_S        stMachCtrl;         /* machine's Ctrl information, it must be initial when malloc the machine. */       
}HDMI_MACH_ELEM_S;

typedef struct
{
    HDMI_MACH_RUN_S         stMachRun;          /* machine's running status. */       
    HDMI_MACH_CTRL_S        stMachCtrl;         /* machine's Ctrl information, it must be initial when malloc the machine. */       
}HDMI_MACH_ELEM_STAT_S;

typedef struct
{
	HI_BOOL					bInit;				/* is the struct init */
	HI_U32					u32Total;			/* total state machine in the list */

}HDMI_MACH_STAT_S;



/**
\brief	  machine init 
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachInit(HI_VOID);

/**
\brief	  machine deinit 
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachDeInit(HI_VOID);


/**
\brief	  machine invoke
\param      none
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachInvoke(HI_VOID);

/**
\brief	  register a machine
\param[in]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the init inforamtion of a new machine
\param[out]	@pu32MachId 			pointer to a machine ID which is malloced
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachRegister(HDMI_MACH_CTRL_S *pstMachCtrl,
                                        HI_U32 *pu32MachId);


/**
\brief	  un-register a machine
\param[in]	@u32MachId                    the machine ID which expected to un-register
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachUnRegister(HI_U32 u32MachId);


/**
\brief	  config a machine
\param[in]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the re-config-inforamtion of exist-machine
\param[in]	@pu32MachId 			the machine ID which expected to re-config.
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachCfgSet(HI_U32 u32MachId,HDMI_MACH_CTRL_S *pstMachCtrl);


/**
\brief	  config a machine
\param[out]	@pstMachCtrl 			pointer to a HDMI_MACH_CTRL_S,it is the re-config-inforamtion of exist-machine
\param[in]	@pu32MachId 			the machine ID .
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachCfgGet(HI_U32 u32MachId,HDMI_MACH_CTRL_S *pstMachCtrl);

/**
\brief	  start a machine 
\param[in]	@u32MachId                    the machine ID which expected to start
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStart(HI_U32 u32MachId);

/**
\brief	  stop a machine 
\param[in]	@u32MachId                    the machine ID which expected to stop
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStop(HI_U32 u32MachId);

/**
\brief	  get total machine number
\param[in]	@pstMachInfo 			pointer to a HDMI_MACH_INFO_S
\param[out]	@pstListStat                     pointer to user's memory ,initial & total element status of the list
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachStatusGet(HDMI_MACH_STAT_S *pstStat);

/**
\brief	  get a machine status
\param[in]	@u32MachId                  the machine ID
\param[out]	@pstStatus                      a machine status
\retval  HI_SUCCESS;HI_FAILURE
*/
HI_S32 HAL_HDMI_MachElemStatusGet(HI_U32 u32MachId,HDMI_MACH_ELEM_STAT_S *pstStatus);

/**
\brief	  get current time
\param     none
\retval     jiffies time
*/
HI_U64 HAL_HDMI_MachMsGet(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_MACHINE_H__ */   

