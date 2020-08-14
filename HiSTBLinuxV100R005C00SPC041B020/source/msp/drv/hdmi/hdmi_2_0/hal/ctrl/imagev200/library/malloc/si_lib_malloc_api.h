/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_lib_malloc.c
*
* @brief Dynamic memory allocation from static memory pool
*
*****************************************************************************/

#ifndef __SI_LIB_MALLOC_API_H__
#define __SI_LIB_MALLOC_API_H__

/***** #include statements ***************************************************/

#include "si_datatypes.h"

/***** public functions ******************************************************/

void* SiiLibMallocCreate( uint_t size );
void SiiLibMallocDelete( void* p );
uint_t SiiLibMallocBytesAllocatedGet( void );
void SiiLibMallocLock( void );
void SiiLibMallocDeleteAll( void );

#endif /* __SI_LIB_MALLOC_API_H__ */

/***** end of file ***********************************************************/
