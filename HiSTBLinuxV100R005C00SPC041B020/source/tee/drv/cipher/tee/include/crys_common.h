/*******************************************************************  
* (c) Copyright 2011-2012 Discretix Technologies Ltd.              *
* This software is protected by copyright, international           *
* treaties and patents.                                            *
* Use of this Software as part of or with the Discretix CryptoCell *
* or Packet Engine products is governed by the products'           *
* commercial end user license agreement ("EULA").                  *
* It is possible that copies of this Software might be distributed *
* under some type of GNU General Public License ("GPL").           *
* Notwithstanding any such distribution under the terms of GPL,    *
* GPL does not govern the use of this Software as part of or with  *
* the Discretix CryptoCell or Packet Engine products, for which a  *
* EULA is required.                                                *
* If the product's EULA allows any copy or reproduction of this    *
* Software, then such copy or reproduction must include this       *
* Copyright Notice as well as any other notices provided           *
* thereunder.                                                      *
********************************************************************/

 
 
#ifndef CRYS_COMMON_H
#define CRYS_COMMON_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */

#include "crys_common_error.h"


#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Mon Nov 22 10:23:20 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This file contains the definitions and function calls for
   *         common activities that are in use for all of the CRYS modules.
   *
   *  \version crys_common.h#1:incl:1
   *  \author adams
   */




/************************ Defines ******************************/
#define CRYS_COMMON_GLOBAL_DATA_RAND_KEY_ID			0
#define CRYS_COMMON_GLOBAL_DATA_AES_SECRET_KEY_ID	1


#define CRYS_AES_SECRET_KEY_SIZE_IN_WORDS 			4

/* the ROT13 defintion - relevant only on SW low level engines compiled in the ROT mode */
#define CRYS_COMMON_ROT_13_OFFSET                   13



/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

#ifndef CRYS_NO_FIPS_SUPPORT
extern DxUint8_t DX_GLOBAL_FIPS_MODE;
extern DxUint16_t	DX_GLOBAL_FIPS_SF_STATUS;
#endif
/************************ Public Functions *********************/


/***********************************************************************
 **
 * @brief This function initializes the CRYS COMMON module in cold init.
 *        it is called from the CRYS_INIT function.    
 *
 *
 * @return CRYSError_t - On success CRYS_OK.
 */
 
 CRYSError_t CRYS_COMMON_Init(DxUint32_t param);

/***********************************************************************
 **
 * @brief This function terminates the CRYS COMMON module .
 *        it is called from the CRYS_Terminate function.    
 *
 *
 * @return CRYSError_t - On success CRYS_OK.
 */
 
 CRYSError_t CRYS_COMMON_Terminate(void );


/***********************************************************************
 **
 * @brief This function executes a reverse bytes copying from one buffer to another buffer.
 *
 *        Overlapping of buffers is not allowed, excluding the case, when destination and source 
 *        buffers are the same. 
 *        Example of a 5 byte buffer:
 *
 *        dst_ptr[4] = src_ptr[0]     
 *        dst_ptr[3] = src_ptr[1]     
 *        dst_ptr[2] = src_ptr[2]     
 *        dst_ptr[1] = src_ptr[3]     
 *        dst_ptr[0] = src_ptr[4]     
 *
 * @param[in] dst_ptr - The pointer to destination buffer.
 * @param[in] src_ptr - The pointer to source buffer. 
 * @param[in] size    - The size in bytes.
 *
 */
 CRYSError_t CRYS_COMMON_ReverseMemcpy( DxUint8_t *dst_ptr , DxUint8_t *src_ptr , DxUint32_t size );
 




/***********************************************************************
 **
 * @brief This function converts aligned words array to bytes array/
 *
 *            1. Assumed, that input buffer is aligned to 4-bytes word and
 *               bytes order is set according to machine endianness.
 *            2. Output buffer receives data as bytes stream from LSB to MSB.
 *               For increasing performance on small buffers, the output data is given 
 *               by rounded down pointer and alignment.
 *            3. This implementation is given for both Big and Little endian machines. 
 *        
 *
 * @param[in] in32_ptr - The pointer to aligned input buffer. 
 * @param[in] out32_ptr - The 32-bits pointer to output buffer (rounded down to 4 bytes) . 
 * @param[in] outAlignBits - The actual output data alignment;
 * @param[in] sizeWords - The size in words (sizeWords >= 1).
 * 
 *  return - no return value.
 */
 void CRYS_COMMON_AlignedWordsArrayToBytes( DxUint32_t *in32_ptr , DxUint32_t *out32_ptr , 
                                            DxUint32_t outAlignBits, DxUint32_t sizeWords );

/***********************************************************************/
 /**
  * @brief This function converts in place words byffer to bytes buffer with 
  *        reversed endianity of output array.
  *  
  *        The function can convert:
  *           - big endian bytes array to words array with little endian order
  *             of words and backward.
  *
  *      Note:
  *      1. Endianness of each word in words buffer should be set allways
  *      according to processor used.
  *      2. Implementation is given for both big and little endianness of
  *      processor.
  *        
  * @param[in]  buf_ptr - The 32-bits pointer to input/output buffer. 
  * @param[in]  sizeWords - The size in words (sizeWords > 0).
  *  
  * @return - no return value. 
  */
void CRYS_COMMON_InPlaceConvertBytesWordsAndArrayEndianness( 
					DxUint32_t *buf_ptr, 
					DxUint32_t  sizeWords);


/***********************************************************************/
  /**
  * @brief This function converts big endianness bytes array to aligned words
  *        array with words order according to little endian /
  *
  *            1. Assumed, that input bytes order is set according
  *     	  to big endianness: MS Byte is most left, i.e. order is from
  *     	  Msb to Lsb.
  *            2. Output words array should set according to
  *     	  little endianness words order: LSWord is most left, i.e. order
  *     	  is from Lsw to Msw. Order bytes in each word - according to
  *     	  processor endianness.
  *            3. Owerlapping of buffers is not allowed, besides in
  *     	  place operation and size aligned to full words.
  *            4. Implementation is given for both big and little
  *     	  endianness of processor.
  *        
  * @param[out] out32_ptr - The 32-bits pointer to output buffer. 
  * @param[in] sizeOutBuffBytes - The size in bytes of output buffer, must be 
  *            aligned to 4 bytes and not less than sizeInBytes.
  * @param[in] in8_ptr - The pointer to input buffer. 
  * @param[in] sizeInBytes - The size in bytes of input data(sizeBytes >= 1).
  * 
  * @return CRYSError_t - On success CRYS_OK is returned, on failure a
  *                        value MODULE_* as defined in .
  */
CRYSError_t CRYS_COMMON_ConvertMsbLsbBytesToLswMswWords( 
					DxUint32_t *out32_ptr,
					DxUint32_t  sizeOutBuffBytes, 
					DxUint8_t  *in8_ptr, 
					DxUint32_t  sizeInBytes);


/***********************************************************************/
  /**
  * @brief This function converts big endianness bytes array to aligned words
  *        array with words order according to little endian /
  *
  *            1. Assumed, that input bytes order is set according
  *     	  to big endianness: MS Byte is most left, i.e. order is from
  *     	  Msb to Lsb.
  *            2. Output words array should set according to
  *     	  little endianness words order: LSWord is most left, i.e. order
  *     	  is from Lsw to Msw. Order bytes in each word - according to
  *     	  processor endianness.
  *            3. Owerlapping of buffers is not allowed, besides in
  *     	  place operation and size aligned to full words.
  *            4. Implementation is given for both big and little
  *     	  endianness of processor.
  *        
  * @param[in] out32_ptr - The 32-bits pointer to output buffer. 
  * @param[in] sizeOutBuffBytes - The size in bytes of output buffer, must be 
  *       not less than sizeInBytes.
  * @param[out] in8_ptr - The pointer to input buffer. 
  * @param[in] sizeInBytes - The size in bytes. The size must be not 0 and 
  *       aligned to 4 bytes word.
  * 
  * @return CRYSError_t - On success CRYS_OK is returned, on failure a
  *                        value MODULE_* as defined in .
  */
CRYSError_t CRYS_COMMON_ConvertLswMswWordsToMsbLsbBytes( 
					DxUint8_t  *out8_ptr,
					DxUint32_t  sizeOutBuffBytes, 
					DxUint32_t *in32_ptr, 
					DxUint32_t  sizeInBytes);


/***********************************************************************/
/**
 * @brief VOS_GetGlobalData get the global random key hidden inside the function
 *	the global data implemented for now are random key buffer and AES secret key buffer
 *
 * When no_rtos is declared then we allow a global data. The random key/AES secret key are hidden as static inside the function
 *
 *
 * @param[in] Globalid	   select the buffer
 * @param[in] GlobalDataSizeWords	   - the global data buffer size needed in words - this value must be a predetermined value
 * @param[out] GlobalData_ptr - Pointer to the global buffer returned. The buffer must be at least GlobalDataSizeWords size
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure an Error as defined in VOS_error
 */
CRYSError_t CRYS_COMMON_GetGlobalData(DxUint16_t Globalid, DxUint32_t *GlobalData_ptr, DxUint16_t GlobalDataSizeWords);


/***********************************************************************/
/** 
* @brief CRYS_COMMON_StoreGlobalData store the global random key into the global buffer hidden inside the function
*	the global data implemented for now are random key buffer and AES secret key buffer
*
*
* @param[in] Globalid	   - random key / AES secret key
* @param[in] GlobalDataSizeWords	   - the global data buffer size needed in words - this value must be a predetermined value
* @param[in] GlobalData_ptr - Pointer to the global buffer to be saved. The buffer must be at least GlobalDataSizeWords size
*
*   Return Value:          
*/
CRYSError_t CRYS_COMMON_StoreGlobalData(DxUint16_t Globalid, DxUint32_t *GlobalData_ptr, DxUint16_t GlobalDataSizeWords);


/***********************************************************************/
/** 
 * @brief The CRYS_COMMON_CutAndSaveEndOfLliData() function saves the data from end of source  
 *        memory, pointed by LLI table, to destination memory, and decreases the LLI table accordingly.
 *
 *        The function executes the following major steps:
 *
 *        1. Starts copy bytes from last byte of last chunk of source LLI table into
 *           last byte of destination memory. 
 *        2. Continues copy bytes in reverse order while not completes copying of all amount of data. 
 *        3. If last chunk of source or destination data is not enough, the function crosses 
 *           to next chunk of LLI table.
 *        4. Decreases the Data size of last updated LLI entry and sets the LAST bit.
 *        5. Exits with the OK code.  
 *      
 *
 * @param[in] SrcLliTab_ptr - The pointer to the LLI table, containing pointers and sizes of
 *                            chunks of source data. The table need to be aligned and placed 
 *                            in SEP SRAM.
 * @param[in] SrcLliTabSize_ptr -   The pointer to buffer, containing th size of the LLI table in words.
 * @param[in] Dest_ptr  -  The destination address for copying the data. 
 * @param[in] DataSize  -  The count of bytes to copy. 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, 
 *                     - CRYS_COMMON_ERROR_IN_SAVING_LLI_DATA_ERROR 
 * 
 * NOTE: 1. Because the function is intended for internal using, it is presumed that all input parameters 
 *          are valid.
 *       2. Assumed, that copied source not may to take more than two last chunks of source memory.
 */
 CRYSError_t  CRYS_COMMON_CutAndSaveEndOfLliData( 
                                         DxUint32_t   *SrcLliTab_ptr,                        
                                         DxUint32_t   *SrcLliTabSize_ptr,            
                                         DxUint8_t    *Dst_ptr,             
                                         DxUint32_t    DataSize);            

/***********************************************************************/
/**
 * @brief The CRYS_COMMON_CutAndSaveBeginOfLliData() function saves the data from beginning of source  
 *        memory, pointed by LLI table, to destination memory, and decreases the LLI table accordingly.
 *
 *        The function executes the following major steps:
 *
 *        1. Starts copy bytes from first byte of first chunk of source LLI table into
 *           destination memory. 
 *        2. If first chunk of source is not enough, the function crosses 
 *           to next chunk of LLI table.
 *        3. Updates LLI table pointer and size according to copied amount of data.
 *        5. Exits with the OK code.  
 *
 * @param[in/out] SrcLliTab_ptr_ptr - The pointer to pointer to the LLI table, containing pointers and 
 *                            sizes of the chunks of source data. The table need to be aligned and  
 *                            placed in SRAM.
 * @param[in/out] SrcLliTabSize_ptr -   The pointer to buffer, containing th size of the LLI table in words.
 * @param[in] Dest_ptr  -  The destination address for copying the data. 
 * @param[in] DataSize  -  The count of bytes to copy. 
 *
 * @return - no return value. 
 * 
 * NOTE: 1. Because the function is intended for internal using, it is presumed that all input parameters 
 *          are valid.
 *       2. Assumed, that copied source not may to take more than two first chunks of source memory.
 */
 void  CRYS_COMMON_CutAndSaveBeginOfLliData( 
                                         DxUint32_t  **SrcLliTab_ptr_ptr,                        
                                         DxUint32_t   *SrcLliTabSize_ptr,            
                                         DxUint8_t    *Dst_ptr,             
                                         DxUint32_t    DataSize);            



#ifdef __cplusplus
}
#endif

#endif


