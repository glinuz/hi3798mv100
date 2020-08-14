/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_ext.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __DRV_ADVCA_EXT_H_
#define __DRV_ADVCA_EXT_H_

#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_unf_advca.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum hiDRV_ADVCA_CA_TARGET_E
{
	DRV_ADVCA_CA_TARGET_DEMUX         = 0,
	DRV_ADVCA_CA_TARGET_MULTICIPHER,
}DRV_ADVCA_CA_TARGET_E;

typedef struct hiDRV_ADVCA_EXTFUNC_PARAM_S
{
    HI_UNF_CIPHER_CA_TYPE_E enCAType;
    HI_UNF_ADVCA_CA_TARGET_E enTarget;
    HI_U32 AddrID;
    HI_U32 EvenOrOdd;
    HI_U8 *pu8Data;
    HI_BOOL bIsDeCrypt;
}DRV_ADVCA_EXTFUNC_PARAM_S;

typedef HI_S32 (*FN_CA_Crypto)(DRV_ADVCA_EXTFUNC_PARAM_S stParam);

typedef struct
{
    FN_CA_Crypto pfnAdvcaCrypto;
}ADVCA_EXPORT_FUNC_S;

HI_S32  ADVCA_DRV_ModeInit(HI_VOID);
HI_VOID ADVCA_DRV_ModeExit(HI_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* _DRV_ADVCA_EXT_H_ */

