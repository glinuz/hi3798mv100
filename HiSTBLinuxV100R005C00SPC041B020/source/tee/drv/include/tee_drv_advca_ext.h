/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tee_drv_advca_ext.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __TEE_DRV_ADVCA_EXT_H_
#define __TEE_DRV_ADVCA_EXT_H_

#include "hi_type.h"
#include "hi_tee_cipher.h"
#include "hi_tee_advca.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef HI_S32 (*FN_CA_Crypto)(DRV_ADVCA_EXTFUNC_PARAM_S stParam);
typedef HI_S32 (*FN_CA_Suspend)(HI_VOID);
typedef HI_S32 (*FN_CA_Resume)(HI_VOID);

typedef struct
{
    FN_CA_Crypto pfnAdvcaCrypto;
	FN_CA_Suspend pfnAdvcaSuspend;
	FN_CA_Suspend pfnAdvcaResume;
}ADVCA_EXPORT_FUNC_S;

HI_S32 ADVCA_DRV_ModeInit(HI_VOID);
HI_VOID ADVCA_DRV_ModeExit(HI_VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* _TEE_DRV_ADVCA_EXT_H_ */

