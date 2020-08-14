/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_common.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_ADVCA_COMMON_H__
#define __DRV_ADVCA_COMMON_H__

typedef enum
{
    CA_VERSION_100,
    CA_VERSION_200,
    CA_VERSION_BUTT
}CA_VERSION_E;

HI_VOID DRV_ADVCA_SystemSetClock(HI_VOID);
HI_VOID DRV_ADVCA_SystemSetReset(HI_VOID);
HI_VOID DRV_ADVCA_RegisterMap(void);
HI_VOID DRV_ADVCA_RegisterUnMap(void);
HI_VOID DRV_ADVCA_WriteReg(HI_U32 addr,  HI_U32 val);
HI_U32  DRV_ADVCA_ReadReg(HI_U32 addr);
HI_VOID DRV_ADVCA_Wait(HI_U32 us);

#endif  /* __DRV_ADVCA_COMMON_H__ */

