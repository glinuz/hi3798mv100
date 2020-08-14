/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :

******************************************************************************/
#ifndef __DRV_ADVCA_H__
#define __DRV_ADVCA_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#include <linux/fs.h>
#include <linux/module.h>
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_drv_advca.h"
#include "hi_drv_otp.h"
#include "hi_drv_cipher.h"
#include "hi_drv_sys.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "drv_advca_ext.h"

HI_VOID DRV_ADVCA_SysWriteReg(HI_U8 *addr, HI_U32 val);
HI_U32 	DRV_ADVCA_SysReadReg(HI_U8 *addr);
HI_S32 	DRV_ADVCA_GetRevision(HI_U8 u8Revision[25]);

HI_S32 	ca_down_interruptible(HI_U32 *pu32Value);
HI_VOID ca_up(HI_U32 *pu32Value);
HI_VOID ca_initMutex(HI_U32 *pu32Value);
HI_U32* ca_create_mutex(HI_VOID);
HI_S32 	ca_atomic_read(HI_U32 *pu32Value);
HI_S32 	ca_atomic_dec_return(HI_U32 *pu32Value);
HI_S32 	ca_atomic_inc_return(HI_U32 *pu32Value);

HI_VOID *ca_ioremap_nocache(HI_U32 u32Addr, HI_U32 u32Len);
HI_VOID ca_iounmap(HI_VOID *pAddr);
HI_VOID ca_msleep(HI_U32 u32Time);

HI_S32 	ca_snprintf(char * buf, HI_U32 size, const char * fmt, ...);

HI_S32 DRV_ADVCA_GetVendorId(HI_U32 *pu32VendorId);
HI_S32 DRV_ADVCA_V300_Ioctl(HI_U32 cmd, HI_VOID *arg);
HI_S32 DRV_ADVCA_V300_Open(HI_VOID);
HI_S32 DRV_ADVCA_V300_Release(HI_VOID);
HI_S32 DRV_ADVCA_V300_ModeInit_0(HI_VOID);
HI_S32 DRV_ADVCA_V300_ModeExit_0(HI_VOID);

HI_S32 HAL_ADVCA_V300_GetCWLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel);
HI_S32 HAL_ADVCA_V300_GetCSA3LadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel);
HI_S32 HAL_ADVCA_V300_GetR2RLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel);
HI_S32 HAL_ADVCA_V300_GetSPLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel);
HI_S32 HAL_ADVCA_V300_SetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel);
HI_S32 HAL_ADVCA_V300_GetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel);
HI_S32 HAL_ADVCA_V300_CryptR2R(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bIsDeCrypt);
HI_S32 HAL_ADVCA_V300_CryptGDRM(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bIsDeCrypt,HI_UNF_ADVCA_CA_TARGET_E enKlTarget);
HI_S32 HAL_ADVCA_V300_DecryptCw(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd);
HI_S32 HAL_ADVCA_V300_DecryptCsa3(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd);
HI_S32 HAL_ADVCA_V300_DecryptSP(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd,DRV_ADVCA_CA_TARGET_E enCwTarget);
HI_S32 HAL_ADVCA_V300_DecryptMisc(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd, DRV_ADVCA_CA_TARGET_E enCwTarget);
HI_S32 HAL_ADVCA_V300_GetSecretKeyChecksumFlag(HI_U32 *pu32Checksum);
HI_S32 HAL_ADVCA_V300_GetCAStates(HI_U32 *pu32State);
HI_S32 HAL_ADVCA_V300_DecryptSWPK(HI_U32 *pu32DataIn,HI_U32 AddrID);
HI_S32 HAL_ADVCA_V300_CryptbyStbRootkey(HI_U32 AddrID);
HI_S32 HAL_ADVCA_V300_GetGDRMFlag(HI_U32 * pGDRMFlag);

HI_VOID CA_readReg(HI_U8 *addr, HI_U32 *pu32Result);
HI_VOID CA_writeReg(HI_U8 *addr, HI_U32 u32Result);

HI_VOID CA_OTP_READ_REG(HI_U32 addr, HI_U32 *pu32Result);
HI_VOID CA_OTP_WRITE_REG(HI_U32 addr, HI_U32 u32Result);
HI_S32 DRV_ADVCA_DecryptbyStbRootkey(HI_U32 AddrID);

HI_S32 DRV_ADVCA_DecryptIrdetoHCA(HI_U32 AddrID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __DRV_ADVCA_H__ */

