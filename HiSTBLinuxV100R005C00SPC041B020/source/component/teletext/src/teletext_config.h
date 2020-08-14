/******************************************************************************

*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*

 ******************************************************************************
  File Name     : teletext_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/07/11
  Description   :
  History       :
  1.Date        : 2012/07/11
    Author      :
    Modification: Created file

*******************************************************************************/
/**
 * \file
 * \brief Describes the information about the teletext module config.
 */
#ifndef __HI_TELETEXT_CONFIG_H__
#define __HI_TELETEXT_CONFIG_H__


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif


/**(2.8M)it's about  size of 1000 pages  */
#define TTX_MAX_MALLOCSIZE (2800000)
/**size of one page is 2772Byte *//*(32+16+8+5+5)*42*/
#define TTX_MAX_PAGESTORE (2048)


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif

