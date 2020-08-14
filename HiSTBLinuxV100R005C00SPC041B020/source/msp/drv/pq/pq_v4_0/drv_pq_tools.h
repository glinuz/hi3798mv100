/******************************************************************************
  *
  * Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
  *
  * This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
  *  and may not be copied, reproduced, modified, disclosed to others, published or used, in
  * whole or in part, without the express prior written permission of Hisilicon.

******************************************************************************
  File Name     : drv_pq_tools.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/06/02
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_PQ_TOOLS_H__
#define __PQ_MNG_PQ_TOOLS_H__

#include "hi_drv_pq.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 PQ_TOOLS_SetRegister(HI_PQ_REGISTER_S* pstAttr);
HI_S32 PQ_TOOLS_GetRegister(HI_PQ_REGISTER_S* pstAttr);
HI_S32 PQ_TOOLS_GetBinPhyAddr(HI_U32* pu32Addr);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
