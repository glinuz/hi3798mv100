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
* @file si_lib_obj_api.h
*
* @brief object base class
* 
*****************************************************************************/

#ifndef __SI_LIB_OBJ_API_H__
#define __SI_LIB_OBJ_API_H__
 
/***** #include statements ***************************************************/

#include "si_datatypes.h"
#include "platform_api.h"
/***** public macro definitions **********************************************/

#define SII_INST_NULL          ((SiiInst_t)0)

/* Construct to stringify defines */
#define SII_STRINGIFY(x)       #x
#define SII_DEF2STR(x)         SII_STRINGIFY(x)

/* local look-up definition */
#define SII_LIB_OBJ_LIST       (*spSiiLibObjList)

//-------------------------------------------------------------------------------------------------
//! @brief      'SII_LIB_OBJ_MODULE_DEF()' must be defined whenever 'SII_LIB_OBJ_CREATE()' is used.
//!
//!             When a module object is shared between multiple source (.c) files then the source file
//!             that containins the create/delete methods should be using 'SII_LIB_OBJ_MODULE_DEF(str)'
//!             while all other source files that refer to the same module object should use
//!             'SII_LIB_OBJ_MODULE_EXT(str)'.
//!             Both macros should only be used outside of functions.
//!
//! @param[in]  str  Pointer to string containing name of module.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_MODULE_DEF(str)        static const char* sSiiLibObjClassStr = SII_DEF2STR(str); SiiLibObjList_t SiiLibObjList_##str = NULL; static SiiLibObjList_t* spSiiLibObjList = &SiiLibObjList_##str
#define SII_LIB_OBJ_MODULE_EXT(str)        extern SiiLibObjList_t SiiLibObjList_##str; static SiiLibObjList_t* spSiiLibObjList = &SiiLibObjList_##str

//-------------------------------------------------------------------------------------------------
//! @brief      Allocates memory needed for the instantiation defined in module.
//!
//!             All creations by the same module are linked together through a linked list.
//!
//! @param[in]  str   Pointer to string containing name of instantiation.
//! @param[in]  size  Size of instantiation as defined in module.
//!
//! @return     pointer to allocated instantiation object
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_CREATE(str, size)      SiiLibObjCreate(&SII_LIB_OBJ_LIST, sSiiLibObjClassStr, str, size)

//-------------------------------------------------------------------------------------------------
//! @brief      Frees instantiation from dynamic memory pool.
//!
//! @param[in]  p  Pointer to instantiation object.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_DELETE(p)              SiiLibObjDelete(p); p=0

//-------------------------------------------------------------------------------------------------
//! @brief      Returns pointer to module (class) name associated to instantiation.
//!
//! @param[in]  p  Pointer to instantiation object.
//!
//! @return     Pointer to string containing name of module.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_NAME_MODULE(p)         SiiLibObjClassNameGet(p)

//-------------------------------------------------------------------------------------------------
//! @brief      Returns pointer to instance name associated to instantiation.
//!
//! @param[in]  p  Pointer to instantiation object.
//!
//! @return     Pointer to string containing name of instantiation.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_NAME_INSTANCE(p)       SiiLibObjInstanceNameGet(p)

//-------------------------------------------------------------------------------------------------
//! @brief      Converts pointer of instantiation to external instance.
//!
//! @param[in]  p  Pointer to instantiation object.
//!
//! @return     inst  External reference to instantiation.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_INST(p)                ((SiiInst_t)p)

//-------------------------------------------------------------------------------------------------
//! @brief      Converts external instantiation reference to pointer of instantiation object as
//!             defined locally in module. Creates an assertion if user would provide a reference
//!             to a different class of instantiation.
//!
//! @param[in]  inst  External reference to instantiation.
//!
//! @return     Pointer to instantiation object.
//-------------------------------------------------------------------------------------------------
#if(SII_ENV_BUILD_ASSERT)
#define SII_LIB_OBJ_PNTR(inst)           (SiiLibObjCheck(&SII_LIB_OBJ_LIST, (void *)inst)) ? ((void *)inst) : ((void *)0)
#else
#define SII_LIB_OBJ_PNTR(inst)           ((void *)inst)
#endif

//-------------------------------------------------------------------------------------------------
//! @brief      Provides a pointer to the first listed instantiation in the linked list.
//!             A NULL value is returned if no instantiations exist.
//!
//! @return     Pointer to first instantiation object.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_FIRST()                SiiLibObjFirstGet(SII_LIB_OBJ_LIST)

//-------------------------------------------------------------------------------------------------
//! @brief      Provides a pointer to the next listed instantiation in the linked list based on
//!             a provided pointer to a different instantiation object of the same class.
//!             A NULL value is returned if referred to last instantiation linked list.
//!
//! @param[in]  p  Pointer to instantiation object.
//!
//! @return     Pointer to next instantiation object.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_NEXT(p)                SiiLibObjNextGet(p)

//-------------------------------------------------------------------------------------------------
//! @brief      Returns pointer of object by name as used when object was created.
//!
//! @return     Pointer to object with matched name or NULL if no objects found.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_PNTR_BY_NAME(str)      SiiLibObjPointerGetByName(SII_LIB_OBJ_LIST, str)

//-------------------------------------------------------------------------------------------------
//! @brief      Relocates an instantiation ('psrc') to a different position in linked list.
//!             The insertion point for 'psrc' is directly after the 'pdes' instance.
//!
//! @param[in]  pdes  Pointer to instantiation object used as insertion point in linked list.
//!                   pdes may also be assigned with 'NULL'. In this case 'psrc' will be inserted
//!                   directly in front of the first listed instance and becomes now the new
//!                   first instance in linked-list.
//!
//! @param[in]  psrc  Pointer to instantiation object that needs to be relocated.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_MOVE(pdes, psrc)       SiiLibObjMove(pdes, psrc)

//-------------------------------------------------------------------------------------------------
//! @brief      Check if module is currently not been instantiated.
//!
//! @retval     true   If zero instantiations exist.
//! @retval     false  If 1 or more instantiations exist.
//-------------------------------------------------------------------------------------------------
#define SII_LIB_OBJ_ZEROINST()            ( (SII_LIB_OBJ_LIST) ? (false) : (true) )

/***** public type definitions ***********************************************/

typedef void*    SiiInst_t;    
typedef void*    SiiLibObjList_t;
typedef uint32_t SiiLibObjSize_t;

/***** public functions ******************************************************/

void* SiiLibObjCreate( SiiLibObjList_t* pInstList, const char* pClassStr, const char* pInstStr, SiiLibObjSize_t size );
void SiiLibObjDelete( void* p );
const char* SiiLibObjClassNameGet( void* p );
const char* SiiLibObjInstanceNameGet( void* p );
bool_t SiiLibObjCheck( SiiLibObjList_t* pInstList, void* p );
void* SiiLibObjFirstGet( SiiLibObjList_t instList );
void* SiiLibObjNextGet( void* p );
void SiiLibObjMove( void* pDes, void* pSrc );
void* SiiLibObjPointerGetByName( SiiLibObjList_t instList, const char* pClassStr );

#endif /* __SI_LIB_OBJ_API_H__ */

/***** end of file ***********************************************************/
