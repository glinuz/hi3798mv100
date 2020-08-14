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



#ifndef _DX_PAL_FILE_INT_H
#define _DX_PAL_FILE_INT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

/** 
* @brief File Description:
*        This file conatins wrapper functions for file related operations. 
*/

/**** ----- Files General Definitions ----- ****/

/* Definitions for file modes */
#define DX_PAL_MAX_SIZE_MODE            4
#define DX_PAL_NUM_OF_SUPPORT_MODES     12

typedef char DX_PAL_FileModeStr_t[DX_PAL_MAX_SIZE_MODE];

typedef DX_PAL_FileModeStr_t DX_PAL_FileModesTable_t[DX_PAL_NUM_OF_SUPPORT_MODES];
   
extern const DX_PAL_FileModeStr_t DX_PAL_FileModeTable[];
/**** ------------------------------------- ****/

//#define   _DxFile_t   FILE

/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/
/** 
* @brief A wrapper for fopen functionality (to create a new file, the file is opened for read and 
*        write). 
*          
*/
#define _DX_PAL_FileCreate(aFileName)   DX_PAL_FOpen(aFileName, DX_PAL_WriteAndRead)

/** 
* @brief A wrapper for fopen functionality. DX_PAL_FileModeTable contains all possible modes 
*        for fopen
*          
*/
#define _DX_PAL_FOpen(aFileName, aFileMode)  ((DxFile_t)fopen(aFileName, DX_PAL_FileModeTable[aFileMode]))

/** 
 * @brief A wrapper for fclose functionality. 
 *          
 */
#define _DX_PAL_FClose(aFileHandle)    fclose((FILE*)aFileHandle) 

/** 
 * @brief A wrapper for fseek functionality
 *          
 */
#define _DX_PAL_FSeek(aFileHandle ,aOffset, aSeekOrigin)     fseek((FILE*)aFileHandle ,aOffset, aSeekOrigin)

/** 
 * @brief A wrapper for ftell functionality
 *         
 */
#define _DX_PAL_FTell(aFileHandle)  ftell((FILE*)aFileHandle)

/** 
* @brief A wrapper for fread functionality
*         
*/
#define _DX_PAL_FRead(aFileHandle, aBuffer, aSize)  fread(aBuffer, 1, aSize, (FILE*)aFileHandle)

/** 
* @brief A wrapper for fwrite functionality
*         
*/
#define _DX_PAL_FWrite(aFileHandle, aBuffer, aSize)  fwrite(aBuffer, 1, aSize, (FILE*)aFileHandle)
/** 
 * @brief A wrapper for fflush functionality
 *         
 */
#define _DX_PAL_FFlush(aFileHandle)    fflush((FILE*)aFileHandle)

    
#ifdef __cplusplus
}
#endif

#endif


