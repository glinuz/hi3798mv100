/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/



#ifndef _DX_PAL_INIT_H
#define _DX_PAL_INIT_H

#include "dx_pal_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * PAL layer entry point. 
 * The function initializes customer platform sub components, such as memory mapping used later by CRYS to get physical contiguous memory.
 */
int DX_PAL_Init(void);



/** 
 * @brief   PAL layer entry point. 
 *          The function releases customer platform initialized sub components,
 *           by DX_PAL_Init().
 *         
 *
 * @param[in] addr - Virtual start address of contiguous memory
 * @return None
 */ 	
void DX_PAL_Terminate(void);


   
#ifdef __cplusplus
}
#endif

#endif


