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



#ifndef _DX_SECURE_DEFS_H
#define _DX_SECURE_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Secure Clock defintions */
/*-------------------------*/
/* Assumes Core Clock frequency should be 200 MHz */
#define CORE_CLOCK_HZ 240000000/*200000000*/
/* Assumes external slow oscillator frequency should be about 1 MHz */
#define EXTERNAL_SLOW_OSCILLATOR_HZ 50000000/*1000000*/

#ifdef __cplusplus
}
#endif

#endif



