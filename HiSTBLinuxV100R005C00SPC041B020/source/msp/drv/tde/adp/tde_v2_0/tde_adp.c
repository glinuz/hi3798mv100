/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_adp.c
Version             : Initial Draft
Author              :
Created             : 2015/07/11
Description         :
Function List       :             
History             :
Date                            Author                  Modification
2015/07/11                                              Created file      
******************************************************************************/
#include "tde_hal.h"
#include "tde_define.h"
#include "tde_adp.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

static HI_U32 gs_u32Capability = ROP|ALPHABLEND|COLORIZE|CLUT|COLORKEY|CLIP|DEFLICKER|RESIZE|MIRROR|CSCCOVERT|QUICKFILL|QUICKCOPY|\
PATTERFILL;
HI_S32 TdeHalGetCapability(HI_U32 *pstCapability)
{
    *pstCapability = gs_u32Capability;
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

