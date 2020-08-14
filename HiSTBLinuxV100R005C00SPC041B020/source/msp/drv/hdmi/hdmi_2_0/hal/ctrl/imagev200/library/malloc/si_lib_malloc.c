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
//#define SII_DEBUG

/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "platform_api.h"
#include "si_lib_obj_api.h"

#ifdef HI_MINIBOOT_SUPPORT
#include <malloc.h>
#endif


/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_malloc);

/***** local macro definitions ***********************************************/
#if 0
#define MEMPOOL_SIZE_IN_BYTES   (0x2000)
#define MEMPOOL_SIZE_IN_WORDS   ((MEMPOOL_SIZE_IN_BYTES>>2)+2)

/***** local prototypes ******************************************************/

static void sMemoryClear( uint_t size, uint_t ptr );

/***** local data objects ****************************************************/

static uint32_t  sMemPool[MEMPOOL_SIZE_IN_WORDS]; /* Ensure memory pool location at 4-byte boundary */
static uint_t    sPtr     = 0;                    /* Pointer to next avaialble memory location      */
static bool_t    sbLock   = false;
#endif
/***** public functions ******************************************************/

void* SiiLibMallocCreate( uint_t size )
{
    void *p = NULL;
#if 0
	uint_t words = ((size>>2)+1); /* Round up to nearest number of words (1 word = 4 bytes) */

	/* Check if memory pool is locked */
	SII_PLATFORM_DEBUG_ASSERT(!sbLock);

	if( MEMPOOL_SIZE_IN_WORDS > (sPtr+words) )
	{
		uint_t ptr = sPtr;

		/* Clear memory */
		sMemoryClear(words, sPtr);

		/* Increase pointer to next available memory location */
		sPtr += words;

		return (void*)(&sMemPool[ptr]);
	}
	else
	{
		SII_PLATFORM_DEBUG_ASSERT(0);
		return NULL;
	}
#endif
#ifndef HDMI_BUILD_IN_BOOT
    p = kzalloc(size, GFP_KERNEL);
    if (!p)
    {
        SII_PRINTF("Error In %s,Line %d\n",__FUNCTION__,__LINE__);       
    }
#else
    p = (void *)malloc(size);
    if (!p)
    {
        SII_PRINTF("Error In %s,Line %d\n",__FUNCTION__,__LINE__);       
    }
#endif

    return p;
}

#if 0 // not used
uint_t SiiLibMallocBytesAllocatedGet( void )
{
	/* Return total amound of bytes allocated */
	return (uint_t)(sPtr<<2);
}
#endif

void SiiLibMallocDelete( void* p )
{
#if 0
	/* Make sure that delete pointer is in allocated memory space */
	SII_PLATFORM_DEBUG_ASSERT((uint32_t*)p >= &sMemPool[0]);
	SII_PLATFORM_DEBUG_ASSERT((uint32_t*)p <  &sMemPool[sPtr]);

	/* Make sure that delete pointer is on a even 4 byte boundary */
	SII_PLATFORM_DEBUG_ASSERT(!((((uint8_t*)p) - ((uint8_t*)&sMemPool[0]))%4));

	sPtr = (((uint8_t*)p) - ((uint8_t*)&sMemPool[0])) >> 2;
#endif

    if (p == NULL)return;
        
#ifndef HDMI_BUILD_IN_BOOT
    kfree(p);
#else
    free(p);
#endif
    p = NULL;
}

#if 0 // not used
void SiiLibMallocLock( void )
{
	sbLock = true;
}

void SiiLibMallocDeleteAll( void )
{
	sbLock = false;
	sPtr   = 0;
}
#endif

/***** local functions *******************************************************/
#if 0
static void sMemoryClear( uint_t size, uint_t ptr )
{

	uint32_t* pData = &sMemPool[ptr];

	while( size-- )
	{
		*pData = 0;
		pData++;
	}
   
}
#endif 
/***** end of file ***********************************************************/
