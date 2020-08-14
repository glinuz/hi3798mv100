/*
 * Copyright (c) (2015 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    vdh_normal.h
 *
 * Purpose: vdh hardware abstract layer normal header
 *
 * Author:  yangyichang 00226912
 *
 * Date:    06, 09, 2015
 *
 */

#ifndef __VDH_NORMAL_H__
#define __VDH_NORMAL_H__

#include "vdh_common.h"


#ifdef __cplusplus
extern "C" {
#endif

/******************************************
 Initialize VDH hardware abstract layer 
******************************************/
SINT32 VDH_Normal_Init(VOID);

/******************************************
 Deinitialize VDH hardware abstract layer 
******************************************/
SINT32 VDH_Exit(VOID);

/******************************************
 Create vdh distributor proc
******************************************/
VOID  VDH_Init_Proc (VOID);

/******************************************
 Destroy vdh distributor proc
******************************************/
VOID   VDH_Exit_Proc(VOID);

/******************************************
 Suspend vdh distributor
******************************************/
VOID   VDH_Suspend(VOID);

/******************************************
 Resume vdh distributor
******************************************/
VOID   VDH_Resume(VOID);

#ifdef HI_TEE_SUPPORT
/******************************************
 Get VDH Hal common Context  
******************************************/
SINT32 VDH_Get_ShareData(VDH_SHARE_DATA_S *pShareData);

/******************************************
 Check if secure need to wake up distributor thread 
******************************************/
VOID VDH_Secure_WakeUp_Distributor(VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif
