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

 
 
#ifndef CRYS_COMMON_MATH_H
#define CRYS_COMMON_MATH_H

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
   *  Object %crys_common_math.h    : %
   *  State           :  %state%
   *  Creation date   :  Tue Nov 23 09:29:46 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This file contains general math operations that are used by the CRYS software.
   *
   *  \version crys_common_math.h#1:incl:2
   *  \author adams
   */

/************************ Defines ******************************/

/************************ Macros *******************************/

/* rotate right 32-bits word by n bits */
#define CRYS_COMMON_ROTR32(x, n) ( ((x) >> (n)) | ((x) << ( 32 - (n) )) )
/* rotate 32-bits word by 16 bits */
#define CRYS_COMMON_ROT32(x) ( (x) >> 16 | (x) << 16 )

/* inverse the bytes order in a word */
#define CRYS_COMMON_REVERSE32(x)  ( ((CRYS_COMMON_ROT32((x)) & 0xff00ff00UL) >> 8) | ((CRYS_COMMON_ROT32((x)) & 0x00ff00ffUL) << 8) )
					   
/* !!! this macro definition must be deleted and changed in *c files by CRYS_COMMON_REVERSE32 */ 
#define CRYS_COMMON_INVERSE_UINT32_BYTES( x ) CRYS_COMMON_REVERSE32( (x) )
   
/* inverse the bytes order in words of array */
#define CRYS_COMMON_INVERSE_UINT32_IN_ARRAY( Array, SizeWords ) \
{  \
	DxUint32_t ii2; \
	for( ii2 = 0; ii2 < (SizeWords); ii2++ ) \
	{ \
		(Array)[ii2] = CRYS_COMMON_REVERSE32( (Array)[ii2] ); \
	} \
}

#ifndef BIG__ENDIAN
/* define word endiannes*/
#define  CRYS_SET_WORD_ENDIANESS
#else
#define  CRYS_SET_WORD_ENDIANESS(val) CRYS_COMMON_REVERSE32(val)
#endif


/* get a bit val from a word array */   
#define CRYS_COMMON_GET_BIT_VAL_FROM_WORD_ARRAY( ptr , bit_pos ) \
   ( ((ptr)[(bit_pos)/32] & (1 << ((bit_pos) % 32))) >> ((bit_pos) % 32) ) 
   
/* exchange a bit on a word array */
#define CRYS_COMMON_EXCHANGE_BIT_ON_WORD_ARRAY(ptr,bit_pos) ( (ptr)[(bit_pos)/32] ^= (1UL << ((bit_pos) % 32) ) )
              
/* the minimum and maximum macros */
#undef min
#define min( a , b ) ( ( (a) < (b) ) ? (a) : (b) )

#undef max    
#define max( a , b ) ( ( (a) > (b) ) ? (a) : (b) )

/* macros for copying 4 words to non aligned output according to macine endianness.
   Note: output is given by aligned down pointer and alignment of output data in bits, 
         input must be aligned to 4 bytes */
#ifdef BIG__ENDIAN
      
    #define CRYS_COMMON_Copy4wordsToBytes( out32_ptr, outAlign, in32_ptr )  \
      if( outAlign != 0 ) \
      {  \
          (out32_ptr)[0]  = ((out32_ptr)[0] & (0xFFFFFFFF << (32-(outAlign))))      | \
                              CRYS_COMMON_REVERSE32((in32_ptr)[0]) >> (outAlign);   \
          (out32_ptr)[1]  = CRYS_COMMON_REVERSE32((in32_ptr)[0]) << (32-(outAlign)) | \
                              CRYS_COMMON_REVERSE32((in32_ptr)[1]) >> (outAlign);   \
          (out32_ptr)[2]  = CRYS_COMMON_REVERSE32((in32_ptr)[1]) << (32-(outAlign)) | \
                              CRYS_COMMON_REVERSE32((in32_ptr)[2]) >> (outAlign);   \
          (out32_ptr)[3]  = CRYS_COMMON_REVERSE32((in32_ptr)[2]) << (32-(outAlign)) | \
                              CRYS_COMMON_REVERSE32((in32_ptr)[3]) >> (outAlign);   \
          (out32_ptr)[4]  = ((out32_ptr)[4] & (0xFFFFFFFF >> (outAlign)))           | \
                              CRYS_COMMON_REVERSE32((in32_ptr)[3]) << (32-(outAlign)); \
      } \
      else  \
      { \
          (out32_ptr)[0]  = CRYS_COMMON_REVERSE32((in32_ptr)[0]);   \
          (out32_ptr)[1]  = CRYS_COMMON_REVERSE32((in32_ptr)[1]);   \
          (out32_ptr)[2]  = CRYS_COMMON_REVERSE32((in32_ptr)[2]);   \
          (out32_ptr)[3]  = CRYS_COMMON_REVERSE32((in32_ptr)[3]);   \
      }

#else  /* LITTLE_ENDIAN */
   #define CRYS_COMMON_Copy4wordsToBytes( out32_ptr, outAlign, in32_ptr )  \
      if( outAlign != 0 ) \
      {  \
          (out32_ptr)[0]  = (out32_ptr)[0] & (0xFFFFFFFF  >> (32-(outAlign))) | (in32_ptr)[0] << (outAlign); \
          (out32_ptr)[1]  = (in32_ptr)[0] >> (32-(outAlign)) | (in32_ptr)[1] << (outAlign);                   \
          (out32_ptr)[2]  = (in32_ptr)[1] >> (32-(outAlign)) | (in32_ptr)[2] << (outAlign);                   \
          (out32_ptr)[3]  = (in32_ptr)[2] >> (32-(outAlign)) | (in32_ptr)[3] << (outAlign);                   \
          (out32_ptr)[4]  = (out32_ptr)[4] & (0xFFFFFFFF << (outAlign)) | (in32_ptr)[3] >> (32-(outAlign));   \
      } \
      else \
      { \
          (out32_ptr)[0]  = (in32_ptr)[0];   \
          (out32_ptr)[1]  = (in32_ptr)[1];   \
          (out32_ptr)[2]  = (in32_ptr)[2];   \
          (out32_ptr)[3]  = (in32_ptr)[3];   \
      }
#endif


/* macros for copying 16 bytes from non aligned input into aligned output according to machine endianness.
   Note: input is given by aligned down pointer and alignment of input data in bits, 
         output must be aligned to 4 bytes */

#ifdef BIG__ENDIAN
      
    #define CRYS_COMMON_Copy16BytesToWords( in32_ptr, inAlign, out32_ptr )  \
      if( inAlign != 0 ) \
      {  \
          (out32_ptr)[0]  = CRYS_COMMON_REVERSE32( (in32_ptr)[0] << (inAlign) | (in32_ptr)[1] >> (32-(inAlign)) );  \
		  (out32_ptr)[1]  = CRYS_COMMON_REVERSE32( (in32_ptr)[1] << (inAlign) | (in32_ptr)[2] >> (32-(inAlign)) );  \
		  (out32_ptr)[2]  = CRYS_COMMON_REVERSE32( (in32_ptr)[2] << (inAlign) | (in32_ptr)[3] >> (32-(inAlign)) );  \
		  (out32_ptr)[3]  = CRYS_COMMON_REVERSE32( (in32_ptr)[3] << (inAlign) | (in32_ptr)[4] >> (32-(inAlign)) );  \
      } \
      else  \
      {  \
  		  (out32_ptr)[0]  = CRYS_COMMON_REVERSE32((in32_ptr)[0]); \
  		  (out32_ptr)[1]  = CRYS_COMMON_REVERSE32((in32_ptr)[1]); \
		  (out32_ptr)[2]  = CRYS_COMMON_REVERSE32((in32_ptr)[2]); \
		  (out32_ptr)[3]  = CRYS_COMMON_REVERSE32((in32_ptr)[3]); \
      }                                   

#else  /* LITTLE_ENDIAN */

   #define  CRYS_COMMON_Copy16BytesToWords( in32_ptr, inAlign, out32_ptr )  \
      if( inAlign != 0 ) \
      {  \
  		  (out32_ptr)[0]  = (in32_ptr)[0] >> (inAlign) | (in32_ptr)[1] << (32-(inAlign));  \
		  (out32_ptr)[1]  = (in32_ptr)[1] >> (inAlign) | (in32_ptr)[2] << (32-(inAlign));  \
		  (out32_ptr)[2]  = (in32_ptr)[2] >> (inAlign) | (in32_ptr)[3] << (32-(inAlign));  \
		  (out32_ptr)[3]  = (in32_ptr)[3] >> (inAlign) | (in32_ptr)[4] << (32-(inAlign));  \
      }  \
      else  \
      {  \
		  (out32_ptr)[0]  = (in32_ptr)[0];  \
		  (out32_ptr)[1]  = (in32_ptr)[1];  \
		  (out32_ptr)[2]  = (in32_ptr)[2];  \
		  (out32_ptr)[3]  = (in32_ptr)[3];  \
      }
#endif



/************************ Enums ********************************/

/* the counter comperation result enum */
typedef enum { CRYS_COMMON_CmpCounter1AndCounter2AreIdentical = 0,
               CRYS_COMMON_CmpCounter1GraterThenCounter2      = 1,
               CRYS_COMMON_CmpCounter2GraterThenCounter1      = 2,
               
               CRYS_COMMON_CmpCounterLast                    = 0x7FFFFFFF,

             } CRYS_COMMON_CmpCounter_t;  
             
/* shift direction  enum */
typedef enum { CRYS_COMMON_ShiftRight = 0,
               CRYS_COMMON_ShiftLeft  = 1,
               
               CRYS_COMMON_ShiftOffMode,
               
               CRYS_COMMON_ShiftDirectionLast= 0x7FFFFFFF,

             } CRYS_COMMON_ShiftDirection_t;  
            
                  

/************************ Typedefs  *****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

/*****************************************************************
 * @brief This function adds a value to a large counter presented in a buffer.
 *        The MSB of the counter is stored in the first cell in the array.
 *        
 *        for example:
 *
 *        a counter of 64 bit : the value is : 
 *
 *        byte[0] << 56 | byte[1] << 48 ............ byte[6] << 8 | byte[7]  
 *
 * @param[in] CounterBuff_ptr - The buffer containing the counter.
 * @param[in] Val             - this value to add.
 * @param[in] CounterSize      - the counter size in 32bit words. 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */

 void CRYS_COMMON_IncMsbUnsignedCounter( DxUint32_t *CounterBuff_ptr ,
                                         DxUint32_t  Val,
                                         DxUint32_t  CounterSize);
                                         
                                       
/********************************************************************************
 * @brief This function adds a value to a large counter presented in a buffer.
 *        The LSB of the counter is stored in the first cell in the array.
 *        
 *        for example:
 *
 *        a counter of 64 bit : the value is : 
 *
 *        byte[7] << 56 | byte[6] << 48 ............ byte[1] << 8 | byte[0]  
 *
 * @param[in] CounterBuff_ptr - The buffer containing the counter.
 * @param[in] Val             - this value to add.
 * @param[in] CounterSize      - the counter size in 32bit words. 
 *
 * @return carry bit from MS word if carry occur
 *                        
 */

 DxUint32_t CRYS_COMMON_IncLsbUnsignedCounter( 
                                         DxUint32_t     *CounterBuff_ptr ,
                                         DxUint32_t      Val,
                                         DxUint32_t      CounterSize);


/********************************************************************************
 * @brief This function adds a value to a large counter presented in a buffer.
 *        Exactly like CRYS_COMMON_IncLsbUnsignedCounter, but when the processor is big endian
 *        
 *
 * @param[in] CounterBuff_ptr - The buffer containing the counter.
 * @param[in] Val             - this value to add.
 * @param[in] CounterSize      - the counter size in 32bit words. 
 *
 * @return carry bit from MS word if carry occur
 *                        
 */

 void CRYS_COMMON_IncLsbUnsignedCounterBigEndian(
				 DxUint32_t   *CounterBuff_ptr,
				 DxUint32_t    Val,
				 DxUint32_t    CounterSize);


/********************************************************************************
 * @brief This function subtracts a value from a large counter presented in a buffer.
 *        The LSB of the counter is stored in the first cell in the array.
 *        
 *        for example:
 *
 *        a counter of 64 bit : the value is : 
 *
 *        byte[7] << 56 | byte[6] << 48 ............ byte[1] << 8 | byte[0]  
 *
 * @param[in] CounterBuff_ptr - the buffer containing the counter.
 * @param[in] Val             - the value to subtract.
 * @param[in]  CounterSize      - the counter size in 32bit words. 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */

  void CRYS_COMMON_DecrLsbUnsignedCounter( DxUint32_t     *CounterBuff_ptr,
                                           DxUint32_t      Val,
                                           DxUint32_t      CounterSizeInWords);
                                          

/********************************************************************************
 * @brief This function subtracts a value from a large counter presented in a first 
 *        buffer and sets result in a second buffer. The first and the second 
 *        buffers may be the same.
 *        The LSB of the counter is stored in the last cell in the array,        
 *
 * @param[in]  CounterBuff_ptr - the buffer containing the counter.
 * @param[in]  Val             - the value to subtract.
 * @param[out] ResCounterBuff_ptr - the buffer containing the result counter.
 * @param[in]  CounterSize      - the counter size in 32bit words. 
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */

 void CRYS_COMMON_DecrMsbUnsignedCounter( DxUint32_t     *CounterBuff_ptr,
                                          DxUint32_t      Val,
                                          DxUint32_t      CounterSizeInWords);


 /**************************************************************
 * @brief This function compares a value of 2 large counter presented in a byte buffer.
 *        The MSB of the counter is stored in the first cell in the array.
 *        
 *        for example:
 *
 *        a counter of 64 bit : the value is : 
 *
 *        byte[0] << 56 | byte[1] << 48 ............ byte[6] << 8 | byte[7]  
 *
 *
 * @param[in] CounterBuff1_ptr - The first counter buffer.
 * @param[in] Counter1Size     - the first counter size in bytes. 
 * @param[in] CounterBuff2_ptr - The second counter buffer.
 * @param[in] Counter2Size     - the second counter size in bytes.
 * @param[in] SizeUnit         - the size units. 0 - bits , 1 - bytes 
 *
 * @return result - an enum with the compare result: 
 *                                0 - both counters are identical 
 *                                1 - counter 1 is larger.
 *                                2 - counter 2 is larger.
 * @note This code executes in constant time, regardless of the arguments.
 */

 CRYS_COMMON_CmpCounter_t CRYS_COMMON_CmpMsbUnsignedCounters( DxUint8_t  *CounterBuff1_ptr,
                                                              DxUint16_t  Counter1Size, 
                                                              DxUint8_t  *CounterBuff2_ptr,
                                                              DxUint16_t Counter2Size );
                                                              
                                                              

/**************************************************************
 * @brief This function compares a value of 2 large counter presented in a byte buffer.
 *        The LSB of the counter is stored in the first cell in the array.
 *        
 *        for example:
 *
 *        a counter of 64 bit : the value is : 
 *
 *        byte[7] << 56 | byte[6] << 48 ............ byte[1] << 8 | byte[0]  
 *
 * @param[in] CounterBuff1_ptr - The first counter buffer.
 * @param[in] Counter1Size     - the first counter size in bytes. 
 * @param[in] CounterBuff2_ptr - The second counter buffer.
 * @param[in] Counter2Size     - the second counter size in bytes.
 *
 * @return result - an enum with the compare result: 
 *                                0 - both counters are identical 
 *                                1 - counter 1 is larger.
 *                                2 - counter 2 is larger.
 */

 CRYS_COMMON_CmpCounter_t CRYS_COMMON_CmpLsbUnsignedCounters( DxUint8_t  *CounterBuff1_ptr,
                                                              DxUint16_t  Counter1Size, 
                                                              DxUint8_t  *CounterBuff2_ptr,
                                                              DxUint16_t Counter2Size );

/**************************************************************************
 *           CRYS_COMMON_CmpLsWordsUnsignedCounters function    	  * 
 **************************************************************************/
/**
 * @brief This function compares a value of 2 large counter presented in a word buffer.
 *        The LSWord of the counters is stored in the first cell in the array.
 *        
 *
 * @param[in] CounterBuff1_ptr  - The first counter buffer.
 * @param[in] Counter1SizeWords - the first counter size in Words. 
 * @param[in] CounterBuff2_ptr  - The second counter buffer.
 * @param[in] Counter2SizeWords - the second counter size in Words.
 *
 * @return result - an enum with the compare result: 
 *                                0 - both counters are identical 
 *                                1 - counter 1 is larger.
 *                                2 - counter 2 is larger.
 */
 CRYS_COMMON_CmpCounter_t CRYS_COMMON_CmpLsWordsUnsignedCounters(DxUint32_t  *CounterBuff1_ptr,
                                                              DxUint16_t   Counter1SizeWords, 
                                                              DxUint32_t  *CounterBuff2_ptr,
                                                              DxUint16_t   Counter2SizeWords);


/********************************************************************************
 *
 * @brief This function returns the effective number of bits in the byte stream counter 
 *        ( searching the highest '1' in the counter )
 *
 *        The function has one implementations: for little and big endian machines.
 *
 *        Assumed, that LSB of the counter is stored in the first cell in the array.
 *         For example, the value of the 8-Bytes counter B is : 
 *             B[7]<<56 | B[6]<<48 ............ B[1]<<8 | B[0] . 
 *        
 *
 * @param[in] CounterBuff_ptr -  The counter buffer.
 * @param[in] CounterSize     -  the counter size in bytes. 
 *
 * @return result - The effective counters size in bits.
 */

 DxUint32_t CRYS_COMMON_GetBytesCounterEffectiveSizeInBits( DxUint8_t  *CounterBuff_ptr,
                                                     DxUint16_t  CounterSize );
                                                     
/*******************************************************************************
 *             CRYS_COMMON_GetWordsCounterEffectiveSizeInBits                  *
 *******************************************************************************
 *
 * @brief This function returns the effective number of bits in the words array  
 *        ( searching the highest '1' in the counter )
 *
 *        The function may works on little and big endian machines.
 *
 *        Assumed, that the words in array are ordered from LS word to MS word.
 *        For LITTLE Endian machines assumed, that LSB of the each word is stored in the first 
 *        cell in the word. For example, the value of the 8-Bytes (B) counter is : 
 *             B[7]<<56 | B[6]<<48 ............ B[1]<<8 | B[0]  
 *        
 *        For BIG Endian machines assumed, that MS byte of each word is stored in the first 
 *        cell, LS byte is stored in the last place of the word. 
 *        For example, the value of the 64 bit counter is : 
 *         B[3] << 56 | B[2] << 48 B[1] << 8 | B[0],  B[7]<<56 | B[6]<<48 | B[5]<<8 | B[4]  
 *
 *     NOTE !!: 1. For BIG Endian the counter buffer and its size must be aligned to 4-bytes word.
 *
 * @param[in] CounterBuff_ptr   -  The counter buffer.
 * @param[in] CounterSizeWords  -  The counter size in words. 
 *
 * @return result - The effective counters size in bits.
 * 
 */
 DxUint32_t CRYS_COMMON_GetWordsCounterEffectiveSizeInBits( DxUint32_t  *CounterBuff_ptr,
                                                            DxUint16_t   CounterSizeWords);

                                                            
/********************************************************************************
 *
 * @brief These macros are used for compatibility with different versions od functions,
 *        used for calculation of effective size of arrays on big end little endian machines.
 *
 *        The macro calls appropriate function according to machine endianness.
 */ 
 #define  CRYS_COMMON_GetCounterEffectiveSizeInBits( CounterBuff_ptr, CounterSize ) \
          CRYS_COMMON_GetBytesCounterEffectiveSizeInBits( (CounterBuff_ptr), (CounterSize) )
                                                     
                                                     
/********************************************************************************
 * @brief This function divides a vector by 2 - in a secured way
 *
 *        The LSB of the vector is stored in the first cell in the array. 
 *        
 *        for example:
 *
 *        a vector of 128 bit : the value is : 
 *
 *        word[3] << 96 | word[2] << 64 ............ word[1] << 32 | word[0]  
 *
 * @param[in] VecBuff_ptr     -  The vector buffer.
 * @param[in] SizeInWords     -  the counter size in words. 
 *
 * @return result - no return value.
 */
void CRYS_COMMON_DivideVectorBy2(DxUint32_t *VecBuff_ptr,DxUint32_t SizeInWords);


/********************************************************************************
 * @brief This function shifts left a big endian vector by Shift - bits (Shift < 8).
 *
 *        The MSB of the vector is stored in the first cell in the array, 
 *        
 *        For example, a vector of 128 bit is : 
 *
 *        byte[n-1] | byte[n-2] ... byte[1] | byte[0]  
 *
 * @param[in] VecBuff_ptr     -  The vector buffer.
 * @param[in] SizeInBytes     -  The counter size in bytes. 
 * @param[in] Shift           -  The number of shift left bits, must be < 8.
 * @return no return value.
 */

void CRYS_COMMON_ShiftLeftBigEndVector(DxUint8_t *VecBuff_ptr,DxUint32_t SizeInBytes, DxInt8_t Shift);

 
/*******************************************************************************
*                      CRYS_COMMON_ShiftRightVector                            *
*******************************************************************************
* @brief This function shifts right a vector by Shift - bits (Shift < 8).
*
*        The LSB of the vector is stored in the first cell in the array. 
*        For example, a vector of 128 bit is : 
*
*        byte[n-1] | byte[n-2] ... byte[1] | byte[0]  
*
* @param[in] VecBuff_ptr     -  The vector buffer.
* @param[in] SizeInBytes     -  The counter size in bytes. 
* @param[in] Shift           -  The number of shift left bits, must be < 8.
* @return no return value.
*/
void CRYS_COMMON_ShiftRightVector(DxUint8_t *VecBuff_ptr, DxUint32_t SizeInBytes, DxInt8_t Shift);


/******************************************************************************
*                      CRYS_COMMON_ShiftLeftVector                            *
******************************************************************************
* @brief This function shifts left a vector by Shift - bits (Shift < 8).
*
*        The LSB of the vector is stored in the first cell in the array. 
*        For example, a vector of 128 bit is : 
*
*        byte[n-1] | byte[n-2] ... byte[1] | byte[0]  
*
* @param[in] VecBuff_ptr     -  The vector buffer.
* @param[in] SizeInBytes     -  The counter size in bytes. 
* @param[in] Shift           -  The number of shift left bits, must be < 8.
* @return no return value.
*/
void CRYS_COMMON_ShiftLeftVector(DxUint8_t *VecBuff_ptr,DxUint32_t SizeInBytes, DxInt8_t Shift);


/**************************************************************
 * @brief This function adds 2 vectors ( A+B).
 *
 * @param[in] A_ptr       -  input vector A.
 * @param[in] B_ptr       -  input vector B.
 * @param[in] SizeInWords - The size in words 
 * @param[in] Res_ptr     - The result pointer
 *
 * @return result  - Carry from high words addition.
 */
 
DxUint32_t  CRYS_COMMON_Add2vectors ( 
							   DxUint32_t *A_ptr, 
                               DxUint32_t *B_ptr,
                               DxUint32_t SizeInWords, 
                               DxUint32_t *Res_ptr );


/*******************************************************************************
 *                      CRYS_COMMON_SubtractWordArrays                         *
 *******************************************************************************
 
 * @brief This function subtracts two little endian words arrays of length 
          SizeInWords:  Res = (A - B) and returns Borrow from subtracting of high 
          words. 
 *
 * @param[in] A_ptr       -  input vector A.
 * @param[in] B_ptr       -  input vector B.
 * @param[in] SizeInWords -  size in words 
 * @param[in] Res_ptr     -  result pointer
 *
 * @return  Borrow from high words subtracting.
 */
 
DxUint32_t CRYS_COMMON_SubtractUintArrays(DxUint32_t *A_ptr, 
                                          DxUint32_t *B_ptr, 
                                          DxUint32_t  SizeInWords, 
                                          DxUint32_t *Res_ptr );
                                          
/*******************************************************************************
 *                      CRYS_COMMON_AddTwoLsbUint8Vectors                      *
 *******************************************************************************
 *
 * @brief This function adds two little endian vectors Res = (A + B) and returns carry. 
 * 
 *
 * @param[in] A_ptr       -  input vector A.
 * @param[in] B_ptr       -  input vector B.
 * @param[in] SizeInWords -  size in words 
 * @param[in] Res_ptr     -  result pointer
 *
 * @return - carry from adding of two high bytes.
 */
 
DxUint32_t CRYS_COMMON_AddTwoLsbUint8Vectors(
                                          DxUint8_t  *A_ptr, 
                                          DxUint8_t  *B_ptr, 
                                          DxUint32_t  VectSizeInBytes, 
                                          DxUint8_t  *Res_ptr ); 
                                          
                                                                                   
/*******************************************************************************
 *                      CRYS_COMMON_SubtractMSBUint8Arrays                     *
 *******************************************************************************
 
 * @brief This function subtracts two big endian byte arrays. 
 *
 *   Assuming:  SizeA >= SizeB. 
 *              Size of result buffer is not less, than sizeA.
 *
 * @param[in] A_ptr       -  Pointer to input vector A.
 * @param[in] sizeA       -  Size in bytes of each of vector A.
 * @param[in] B_ptr       -  Pointer to input vector B.
 * @param[in] sizeB       -  Size in bytes of each of vector B.
 * @param[in] Res_ptr     -  result pointer
 *
 * @return  Borrow from high byte of vector A.
 */
DxUint8_t CRYS_COMMON_SubtractMSBUint8Arrays(
                                          DxUint8_t  *A_ptr, 
                                          DxUint32_t  sizeA, 
                                          DxUint8_t  *B_ptr, 
                                          DxUint32_t  sizeB, 
                                          DxUint8_t  *Res_ptr );



#ifdef __cplusplus
}
#endif

#endif



