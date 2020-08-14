/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : hi_vdp_smmu.h
Version		    : version 1.0
Author		    : 
Created		    : 2015/08/25
Description	    : Describes adp file. 
Function List 	:
History       	:
Date				Author        		Modification
2015/08/25		    w00217574  		    
******************************************************************************/


#ifndef  _HI_VDP_SMMU_H_
#define  _HI_VDP_SMMU_H_


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */


#include "hi_type.h"


/***************************** Macro Definition ******************************/

/******************************* API declaration *****************************/

HI_S32 VDP_Hal_SetSmmuScbTtbr(HI_S32 u32ScbTtbr);
HI_S32 VDP_Hal_SetSmmuCbTtbr(HI_S32 u32CbTtbr);
HI_S32 VDP_Hal_SetSmmuErrRdAddr(HI_S32 u32ErrRdAddr);
HI_S32 VDP_Hal_SetSmmuErrWrAddr(HI_S32 u32ErrWrAddr);


HI_S32 VDP_Hal_SetSmmuEnable(HI_BOOL bEnable);
HI_VOID VDP_Hal_ClearSMMUInt(HI_S32 u32State);
HI_VOID VDP_Hal_GetSMMUSta(HI_S32 *pu32State);
HI_VOID VDP_Hal_SetSMMUIntEnable(HI_BOOL bEnable);
HI_VOID ISR_VDP_SmmuDebug(HI_VOID);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif /**_HI_VDP_SMMU_H_         *\*/
