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
* @file si_lib_obj.c
*
* @brief Object creater
*
*****************************************************************************/
//#define SII_DEBUG

/***** #include statements ***************************************************/

//#include <stdio.h>
#include <stdarg.h>

#include "si_datatypes.h"
#include "si_lib_obj_api.h"
#include "si_lib_malloc_api.h"
#include "platform_api.h"
#include "sii_time.h"

/***** Register Module name **************************************************/

//SII_LIB_OBJ_MODULE_DEF(lib_obj);

/***** local macro definitions ***********************************************/

#define DATA_MEM_LOC               (0x10000000)
#define LOG_LINE_WRAP              (80)

/***** local type definitions ************************************************/

typedef enum
{
	STATE_STOPPED,
	STATE_STARTED
} State_t;

typedef struct Hdr_s
{
	SiiLibObjList_t*  pInstList;
	const char*        pInstStr;
	struct Hdr_s*      pNxtHdr;
	State_t            state;
} Hdr_t;

typedef struct Lst_s
{
	const char*        pClassStr;
	Hdr_t*             pLstHdr;
	struct Lst_s*      pNxtLst;
} Lst_t;

/***** local prototypes ******************************************************/

static void sAddList( Lst_t* pLstNew );
static void sRemoveList( Lst_t* pLstDel );
static Hdr_t* sPrevHeaderGet( Hdr_t* pHdr );
//static void sPreInsert( Hdr_t* pDesHdr, Hdr_t* pSrcHdr );
static void sPostInsert( Hdr_t* pDesHdr, Hdr_t* pSrcHdr );
static void sInsertHeader( Hdr_t* pHdr );
static void sRemoveHeader( Hdr_t* pHdr );

/***** ufd definition ********************************************************/

#ifdef SII_ENV_BUILD_UFD
#include "si_lib_ufd_api.h"

static void sUfdDumpInstances( void );

static SiiLibUfdCmdRec_t sUfdCmdTbl[] =
{
	UFD_HDR( "SiiLibObj",     "Instantiation manager"  ),

	UFD_RC1( "InstDmp",        VOI,         sUfdDumpInstances,      NULL,                  "Lists all created instantiations"   ),

	UFD_END()
};
#endif /* SII_ENV_BUILD_UFD */

/***** local data objects ****************************************************/

static Lst_t*   spLstFirst = NULL;

/***** public functions ******************************************************/

void* SiiLibObjCreate( SiiLibObjList_t* pInstList, const char* pClassStr, const char* pInstStr, SiiLibObjSize_t size )
{
	Hdr_t* pHdr;
	Lst_t* pLst;

#ifdef SII_ENV_BUILD_UFD
	if( !spLstFirst )
	{
		SiiLibUfdRegisterCmdTable(sUfdCmdTbl);
	}
#endif /* SII_ENV_BUILD_UFD */

	if( *pInstList )
	{
		pLst = (Lst_t*)*pInstList;

		/* Check for correct list */
		SII_PLATFORM_DEBUG_ASSERT(pClassStr == pLst->pClassStr);
	}
	else
	{
		/* Create new list */
		pLst = (Lst_t*)SiiLibMallocCreate(sizeof(Lst_t));
		SII_PLATFORM_DEBUG_ASSERT(pLst);
		pLst->pClassStr = pClassStr;
		pLst->pLstHdr   = NULL;
		pLst->pNxtLst   = NULL;
		*pInstList      = (SiiLibObjList_t)pLst;

		/* Create link between each list of instantiations */
		sAddList(pLst);
	}

	/* Request memory for instance header */
	pHdr = (Hdr_t*)SiiLibMallocCreate(sizeof(Hdr_t)+size);
	if( pHdr )
	{
		/* Configure header */
		pHdr->pInstList = pInstList;
		pHdr->pInstStr  = pInstStr;
		pHdr->pNxtHdr   = NULL;
		pHdr->state     = STATE_STOPPED;

		/* Insert instance to linked list */
		sInsertHeader(pHdr);
		SII_PLATFORM_DEBUG_ASSERT(pHdr->pNxtHdr);

		return (((uint8_t*)pHdr)+sizeof(Hdr_t));
	}
	return NULL;
}

void SiiLibObjDelete( void* p )
{
	Hdr_t* pHdr = (Hdr_t*)((uint8_t*)p-sizeof(Hdr_t));
	Lst_t* pLst = (Lst_t*)(*pHdr->pInstList);

	/* Remove instance from linked list */
	SII_PLATFORM_DEBUG_ASSERT(pHdr->pNxtHdr);
	sRemoveHeader(pHdr);
	SII_PLATFORM_DEBUG_ASSERT(!pHdr->pNxtHdr);

	/* Clear List reference in parent (user) */
	if( !pLst->pLstHdr )
	{
		*pHdr->pInstList = NULL;
	}

	/* Delete instance memory */
	SiiLibMallocDelete(pHdr);
	if( !pLst->pLstHdr )
	{
		/* Remove list of instances */
		sRemoveList(pLst);

		/* Delete list memory */
		SiiLibMallocDelete(pLst);
	}

#ifdef SII_ENV_BUILD_UFD
	if( !spLstFirst )
	{
		SiiLibUfdUnRegisterCmdTable(sUfdCmdTbl);
	}
#endif /* SII_ENV_BUILD_UFD */
}

#if 0 // not used
const char* SiiLibObjClassNameGet( void* p )
{
	Hdr_t* pHdr = (Hdr_t*)(p-sizeof(Hdr_t));
	Lst_t* pLst = (Lst_t*)*pHdr->pInstList;

	return pLst->pClassStr;
}
#endif

const char* SiiLibObjInstanceNameGet( void* p )
{
	Hdr_t* pHdr = (Hdr_t*)((uint8_t*)p-sizeof(Hdr_t));

	return pHdr->pInstStr;
}

#if(SII_ENV_BUILD_ASSERT)
bool_t SiiLibObjCheck( SiiLibObjList_t* pInstList, void* p )
{
	if( p )
	{
		Hdr_t* pHdr = (Hdr_t*)((uint8_t*)p - sizeof(Hdr_t));

		/* Check if object is registered to the same list */
		if( pInstList == pHdr->pInstList )
			return true;
	}
	return false;
}
#endif

void* SiiLibObjFirstGet( SiiLibObjList_t instList )
{
	if( instList )
	{
		Lst_t* pLst = (Lst_t*)instList;

		return (((uint8_t*)(pLst->pLstHdr->pNxtHdr))+sizeof(Hdr_t));
	}
	return NULL;
}

void* SiiLibObjNextGet( void* p )
{
	Hdr_t* pHdr = (Hdr_t*)((uint8_t*)p-sizeof(Hdr_t));
	Lst_t* pLst = (Lst_t*)*pHdr->pInstList;

	SII_PLATFORM_DEBUG_ASSERT(pHdr);
	SII_PLATFORM_DEBUG_ASSERT(pLst);
	if( pHdr != pLst->pLstHdr )
		return (((uint8_t*)(pHdr->pNxtHdr))+sizeof(Hdr_t));

	/* Reached end of list */
	return NULL;
}

void SiiLibObjMove( void* pDes, void* pSrc )
{
	Hdr_t* pSrcHdr = (Hdr_t*)((uint8_t*)pSrc-sizeof(Hdr_t));
	Lst_t* pLst    = (Lst_t*)*pSrcHdr->pInstList;

	/* No mvove is needed if pSrc and pDes pointing to the same object */
	if( pDes == pSrc )
		return;

	/* Make sure that source is a valid pointer */
	SII_PLATFORM_DEBUG_ASSERT(pSrc);

	/* Make sure that source object is currently linked in */
	SII_PLATFORM_DEBUG_ASSERT(pSrcHdr->pNxtHdr);

	/* Remove source object from linked list */
	sRemoveHeader(pSrcHdr);

	/* With a valid destination object provided the source object will be replaced to directly behind the destination object. */
	/* However in case no destination is provided the source object will be replace to the first position.                    */
	if( pDes )
	{
		Hdr_t* pDesHdr = (Hdr_t*)((uint8_t*)pDes-sizeof(Hdr_t));

		/* Make sure that both instantiations belong to the same list (type) */
		SII_PLATFORM_DEBUG_ASSERT(pDesHdr->pInstList == pSrcHdr->pInstList);

		/* Make sure that destination object is a linked in object */
		SII_PLATFORM_DEBUG_ASSERT(pDesHdr->pNxtHdr);

		/* Insert in linked list */
		sPostInsert(pDesHdr, pSrcHdr);

		/* Update last poiter if inserted at end of list */
		if( pDesHdr == pLst->pLstHdr )
		{
			pLst->pLstHdr = pSrcHdr;
		}
	}
	else
	{
		if( pLst->pLstHdr )
		{
			/* Insert as first object in list */
			sPostInsert(pLst->pLstHdr, pSrcHdr);
		}
		else
		{
			pSrcHdr->pNxtHdr = pSrcHdr; /* Ensure circular list */
			pLst->pLstHdr    = pSrcHdr;
		}
	}
}

#if 0 // not used
void* SiiLibObjPointerGetByName( SiiLibObjList_t instList, const char* pClassStr )
{
	if( instList )
	{
		Lst_t* pLst = (Lst_t*)instList;

		if( pLst->pLstHdr )
		{
			Hdr_t* pFirstHdr = pLst->pLstHdr->pNxtHdr;
			Hdr_t* pHdr      = pFirstHdr;

			do
			{
				if( !SII_STRCMP(pHdr->pInstStr, pClassStr, 32) )
				{
					return ((void*)pHdr)+sizeof(Hdr_t);
				}
				pHdr = pHdr->pNxtHdr;
			} while( pFirstHdr != pHdr );
		}
	}
	return NULL;
}
#endif

/***** local functions *******************************************************/

static void sAddList( Lst_t* pLstNew )
{
	if( spLstFirst )
	{
		Lst_t* pLst = spLstFirst;

		while( pLst->pNxtLst )
		{
			pLst = pLst->pNxtLst;
		}
		pLst->pNxtLst = pLstNew;
	}
	else
	{
		spLstFirst = pLstNew;
	}
}

static void sRemoveList( Lst_t* pLstDel )
{
	SII_PLATFORM_DEBUG_ASSERT(spLstFirst);
	if( spLstFirst == pLstDel )
	{
		spLstFirst = spLstFirst->pNxtLst;
	}
	else
	{
		Lst_t* pLst = spLstFirst;

		while( pLst->pNxtLst )
		{
			if( pLst->pNxtLst == pLstDel )
				break;

			pLst = pLst->pNxtLst;
		}
		SII_PLATFORM_DEBUG_ASSERT(pLst->pNxtLst);
		pLst->pNxtLst = pLstDel->pNxtLst;
	}
}

static Hdr_t* sPrevHeaderGet( Hdr_t* pHdr )
{
	Hdr_t* pTempHdr = pHdr;

	while( pHdr != pTempHdr->pNxtHdr )
		pTempHdr = pTempHdr->pNxtHdr;

	return pTempHdr;
}


#if 0 // Currently not used
static void sPreInsert( Hdr_t* pDesHdr, Hdr_t* pSrcHdr )
{
	Hdr_t* pPrvHdr = sPrevHeaderGet(pDesHdr);

	pSrcHdr->pNxtHdr = pDesHdr;
	pPrvHdr->pNxtHdr = pSrcHdr;
}
#endif

static void sPostInsert( Hdr_t* pDesHdr, Hdr_t* pSrcHdr )
{
	pSrcHdr->pNxtHdr = pDesHdr->pNxtHdr;
	pDesHdr->pNxtHdr = pSrcHdr;
}


static void sInsertHeader( Hdr_t* pHdr )
{
	Lst_t* pLst = (Lst_t*)*pHdr->pInstList;

	SII_PLATFORM_DEBUG_ASSERT(!pHdr->pNxtHdr);
	if( pLst->pLstHdr )
	{
		sPostInsert(pLst->pLstHdr, pHdr);
	}
	else
	{
		pHdr->pNxtHdr = pHdr; /* Ensure circular list */
	}
	pLst->pLstHdr = pHdr;
}

static void sRemoveHeader( Hdr_t* pHdr )
{
	Lst_t* pLst = (Lst_t*)*pHdr->pInstList;

	SII_PLATFORM_DEBUG_ASSERT(pHdr->pNxtHdr);
	/* check if last instance removal */
	if( pHdr == pHdr->pNxtHdr )
	{
		/* Clear reference to linked list */
		pLst->pLstHdr = NULL;
	}
	else
	{
		Hdr_t* pPrvHdr = sPrevHeaderGet(pHdr);

		/* remove instance out of linked list */
		pPrvHdr->pNxtHdr = pHdr->pNxtHdr;

		/* Change last instance reference in case last instance is removed */
		if( pLst->pLstHdr == pHdr )
			pLst->pLstHdr = pPrvHdr;
	}

	/* Clear next reference to indicate that instance has removed from list */
	pHdr->pNxtHdr = NULL;
}


#ifdef SII_ENV_BUILD_UFD

static Hdr_t* sUfdHdrPtrGet( uint16_t instID )
{
	Hdr_t* pHdr = (Hdr_t*)(DATA_MEM_LOC+instID-sizeof(Hdr_t));

	if( (((uint32_t)pHdr)%4) || (pHdr->pInstList != pHdr->pNxtHdr->pInstList) )
	{
		SiiLibUfdPrintf("Wrong instance ID!\n");
		return NULL;
	}

	return pHdr;
}

#if 0
static Hdr_t* sUfdHdrPtrByStrGet( const char* pClassStr, const char* pInstStr )
{
	if( spLstFirst )
	{
		Lst_t* pLst = spLstFirst;

		while( pLst->pNxtLst )
		{
			Hdr_t* pHdr = pLst->pLstHdr;

			if( !SII_STRCMP(pLst->pClassStr, pClassStr, 32) )
			{
				do
				{
					if( !SII_STRCMP(pHdr->pInstStr, pInstStr, 32) )
						return pHdr;

					pHdr = pHdr->pNxtHdr;
				}
				while( pHdr != pLst->pLstHdr );
			}
			pLst = pLst->pNxtLst;
		}
	}

	return NULL;
}
#endif

static void sUfdDumpInstances( void )
{
	if( spLstFirst )
	{
		Lst_t* pLst = spLstFirst;

		SiiLibUfdPrintf("    Inst ID   Instance Name    logging   state\n");
		while( pLst->pNxtLst )
		{
			Hdr_t* pHdr = pLst->pLstHdr;

			SiiLibUfdPrintf("%s :\n", pLst->pClassStr);
			do
			{
				SiiLibUfdPrintf("   0x%04X", ((uint32_t)pHdr)-DATA_MEM_LOC+sizeof(Hdr_t));
				SiiLibUfdPrintf("  %-30s", (pHdr->pInstStr && SII_STRLEN(pHdr->pInstStr)) ? (pHdr->pInstStr) : ("Unknown"));
				SiiLibUfdPrintf(" 0x%08X", pHdr->logFlags);
				SiiLibUfdPrintf("  %s", (STATE_STOPPED == pHdr->state) ? "-" : "Started");
				SiiLibUfdPrintf("\n");

				pHdr = pHdr->pNxtHdr;
			}
			while( pHdr != pLst->pLstHdr );

			pLst = pLst->pNxtLst;
		}
	}
}

#endif /* SII_ENV_BUILD_UFD */

/***** end of file ***********************************************************/
