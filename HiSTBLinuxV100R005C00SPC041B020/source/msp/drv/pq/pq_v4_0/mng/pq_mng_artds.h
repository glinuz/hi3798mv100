/******************************************************************************
*
* Copyright (C) 2015-2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_artds.h
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2015/05/11
  Description   :

******************************************************************************/


#ifndef __PQ_MNG_ARTDS_H__
#define __PQ_MNG_ARTDS_H__

#include "hi_type.h"
#include "drv_pq_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 PQ_MNG_RegisterArtDS(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterArtDS(HI_VOID);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


