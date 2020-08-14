/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_sys.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/1/31
  Description   : hi3511 system private Head File
  History       :
  1.Date        : 2007/1/31
    Author      : c42025
    Modification: Created file

******************************************************************************/
#ifndef  __HI_DRV_SYS_H__
#define  __HI_DRV_SYS_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_common.h"

/* Define Debug Level For SYS */
#define HI_FATAL_SYS(fmt...)    HI_FATAL_PRINT(HI_ID_SYS, fmt)
#define HI_ERR_SYS(  fmt...)    HI_ERR_PRINT(HI_ID_SYS, fmt)
#define HI_WARN_SYS( fmt...)    HI_WARN_PRINT(HI_ID_SYS, fmt)
#define HI_INFO_SYS( fmt...)    HI_INFO_PRINT(HI_ID_SYS, fmt)

HI_S32  HI_DRV_REG_KInit(HI_VOID);
HI_VOID HI_DRV_REG_KExit(HI_VOID);

HI_S32  HI_DRV_SYS_Init(HI_VOID);
HI_VOID HI_DRV_SYS_Exit(HI_VOID);

HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);
HI_S32  HI_DRV_SYS_GetChipPackageType(HI_CHIP_PACKAGE_TYPE_E *penPackageType);

HI_S32  HI_DRV_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs);
HI_S32  HI_DRV_SYS_GetDolbySupport(HI_U32 *pu32Support);
HI_S32  HI_DRV_SYS_GetDtsSupport(HI_U32 *pu32Support);
HI_S32  HI_DRV_SYS_GetAdvcaSupport(HI_U32 * pu32Support);
HI_S32  HI_DRV_SYS_GetRoviSupport(HI_U32 *pu32Support);
HI_S32  HI_DRV_SYS_GetHdrSupport(HI_U32 *pu32Support);
HI_S32  HI_DRV_SYS_GetDolbyvisionSupport(HI_U32 *pu32Support);
HI_S32  HI_DRV_SYS_GetMemConfig(HI_SYS_MEM_CONFIG_S *pstConfig);
HI_S32  HI_DRV_SYS_Get64DieID(HI_U64 *pu64dieid);
HI_S32  HI_DRV_SYS_Get32DieID(HI_U32 *pu32dieid);
HI_S32  HI_DRV_SYS_SetIrqAffinity(HI_MOD_ID_E eModuleId, HI_U32 u32irqNum);

#endif /* __HI_DRV_SYS_H__ */

