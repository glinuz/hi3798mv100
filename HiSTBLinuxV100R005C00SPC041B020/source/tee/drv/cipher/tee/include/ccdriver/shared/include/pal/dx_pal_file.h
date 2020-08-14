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



#ifndef _DX_PAL_FILE_H
#define _DX_PAL_FILE_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "dx_pal_types.h"
#include "dx_pal_file_plat.h"
/** 
* @brief File Description:
*        This file conatins functions for file related operations. The functions implementations
*        are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior 
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/

/**** ----- Files General Definitions ----- ****/
typedef enum
{
  DX_PAL_Read   =               0,   /* "r", read only */
  DX_PAL_ReadAndWrite     =     1,   /* "r+", read and write */
  DX_PAL_Write  =               2,   /* "w", write only */
  DX_PAL_WriteAndRead =         3,   /* "w+", write and read */ 
  DX_PAL_Append =               4,   /* "a", append to the end of file */
  DX_PAL_AppendAndRead  =       5,   /* "a+", append (to the end of file) and read */
  DX_PAL_ReadBinary     =       6,   /* "rb", read binary  */
  DX_PAL_ReadAndWriteBinary =   7,   /* "r+b" read and write binary */
  DX_PAL_WriteBinary  =         8,   /* "wb" write binary */
  DX_PAL_WriteAndReadBinary =   9,   /* "w+b" write and read binary */ 
  DX_PAL_AppendBinary =         10,   /* "ab" append binary */
  DX_PAL_AppendAndReadBinary  = 11,    /* "a+b" append and read binary */
  
  DX_PAL_DummyMode  = 0x7FFFFFFF          
  
}DX_PAL_FileMode_t;

/* Definitions for SEEK positions */

#define DX_PAL_SEEK_START 0     /* Seek from start of file */
#define DX_PAL_SEEK_CUR   1     /* Seek from current position */
#define DX_PAL_SEEK_END   2     /* Seek from end of file */

/* Definition for DxFile */
typedef struct _DxFile_t*  DxFile_t;
/**** ------------------------------------- ****/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/** 
 * @brief This function purpose is to create a new file. The function will delete a file
 *        If it is already exist.
 *         
 *
 * @param[in] aFileName - The file name to create
 *
 * @return The function returns a FILE handle to the opened file, in case of failure 
 *         the function will return NULL
 */
DxFile_t DX_PAL_FileCreate(	char *aFileName  ); 

/* Definition for DX_PAL_FileCreate */
#define DX_PAL_FileCreate(aFileName)   _DX_PAL_FileCreate(aFileName)
/** 
 * @brief This function purpose is to create a new file. The function will delete a file
 *        If it is already exist.
 *         
 *
 * @param[in] aFileName - The file name to open
 * @param[in] aFileMode - The mode to open the file
 *
 * @return The function returns a FILE handle to the opened file, in case of failure 
 *         the function will return NULL
 */
DxFile_t DX_PAL_FOpen(	char *aFileName, DX_PAL_FileMode_t aFileMode  );

/* Definition for fopen */
#define DX_PAL_FOpen(aFileName, aFileMode)      _DX_PAL_FOpen(aFileName, aFileMode)
/** 
 * @brief This function purpose is to close a file (pointed by aFileHandle), The function
 *        will dissociate the file from the handle.
 *         
 *
 * @param[in] aFileHandle - The file name to create
 *
 * @return The return values will be according to operating system return values.
 */
DxError_t DX_PAL_FClose(	DxFile_t aFileHandle  );

/* Definition for fclose */
#define DX_PAL_FClose(aFileHandle)   _DX_PAL_FClose(aFileHandle)

/** 
 * @brief This function purpose is to change the file pointer position according to aOffset
 *         
 *
 * @param[in] aFileHandle - The file handle
 * @param[in] aOffset - offset to move the file pointer inside the file
 * @param[in] aSeekOrigin - seek origin (current, end or start) to move aOffset from
 *
 * @return The return values will be according to operating system return values.
 */
DxError_t DX_PAL_FSeek(	DxFile_t aFileHandle , int32_t aOffset, uint8_t aSeekOrigin );

/* Definition for fseek */
#define DX_PAL_FSeek(aFileHandle ,aOffset, aSeekOrigin)    _DX_PAL_FSeek(aFileHandle ,aOffset, aSeekOrigin)

/** 
 * @brief This function purpose is to return the file pointer position
 *         
 *
 * @param[in] aFileHandle - The file handle
 *
 * @return The file pointer position
 */
uint32_t DX_PAL_FTell(	DxFile_t aFileHandle );

/* definition for DX_PAL_FTell */
#define DX_PAL_FTell(aFileHandle)  _DX_PAL_FTell(aFileHandle)

/** 
 * @brief This function purpose is to read aSize of bytes from the file and write it
 *        to aBuffer. In case EOF reached before aSize is read the returned size will be smaller
 *        than aSize.
 *         
 *
 * @param[in] aFileHandle - The file handle
 * @param[in] aBuffer - Pointer to buffer to read the data into
 * @param[in] aSize - Number of bytes to read from file
 *
 * @return The number of bytes read from the file
 */
uint32_t DX_PAL_FRead(DxFile_t aFileHandle, void *aBuffer, uint32_t aSize );

/* Definition for fread */
#define DX_PAL_FRead(aFileHandle, aBuffer, aSize)   _DX_PAL_FRead(aFileHandle, aBuffer, aSize)
/** 
 * @brief This function purpose is to write aSize bytes from aBuffer to the file pointed 
 *        by aFileHandle.
 *         
 *
 * @param[in] aFileHandle - The file handle
 * @param[in] aBuffer - Pointer to buffer to read the data into
 * @param[in] aSize - Number of bytes to read from file
 *
 * @return The number of bytes written to the file
 */
uint32_t DX_PAL_FWrite(	DxFile_t aFileHandle, const void *aBuffer, uint32_t aSize );

#define DX_PAL_FWrite(aFileHandle, aBuffer, aSize)  _DX_PAL_FWrite(aFileHandle, aBuffer, aSize)
/** 
 * @brief This function purpose is to save all buffered data to disk
 *         
 *
 * @param[in] aFileHandle - The file handle
 *
 * @return The return values will be according to operating system return values.
 */
DxError_t DX_PAL_FFlush(	DxFile_t aFileHandle  );

/* Definition for fflush */
#define DX_PAL_FFlush(aFileHandle)   _DX_PAL_FFlush(aFileHandle)

/** 
 * @brief This function purpose is to return the file size
 *         
 *
 * @param[in] aFileHandle - The file handle
 * @param[out] aFileSize - The returned file size
 *
 * @return The function will return DX_SUCCESS in case of success, else errors from 
 *         DX_PAL_Error.h will be returned.
 */
DxError_t DX_PAL_FGetFileSize(	DxFile_t aFileHandle, uint32_t *aFileSize  );

    
#ifdef __cplusplus
}
#endif

#endif


