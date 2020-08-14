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

 
 
#ifndef CRYS_RND_LOCAL_H
#define CRYS_RND_LOCAL_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */



#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Wed Dec 29 13:23:59 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief A brief description of this module
   *
   *  \version CRYS_RND_local.h#1:incl:1
   *  \author adams
   */


#include "crys_aes.h"
#include "crys_rnd.h"


/************************ Defines ****************************/


/*************************************************************/
/****** Common definitions for RND_DMA and non DMA     *******/ 
/*************************************************************/

#define CRYS_RND_ENGINE_TYPE  LLF_RND_ENGINE_TYPE


#define CRYS_RND_VECT_IN_RANGE_MAX_COUNT_OF_TRIES    100

#define CRYS_RND_BASIC_BLOCK_SIZE_IN_WORDS 4
#define CRYS_RND_BASIC_BLOCK_SIZE_IN_BYTES (CRYS_RND_BASIC_BLOCK_SIZE_IN_WORDS*sizeof(DxUint32_t))
#define CRYS_RND_ENTROPY_BLOCK_SIZE_IN_WORDS 4
#define CRYS_RND_ENTROPY_BLOCK_SIZE_IN_BYTES (CRYS_RND_ENTROPY_BLOCK_SIZE_IN_WORDS*sizeof(DxUint32_t))

/* Bit-fields of Instantiation steps in the StateFlag:
    - b'0: 0 - not instantiated, 1 - instantiated normally;
    - b'1: 1 - loss samples, 0 - no loss;
    - b'2: 1 - time exceeding, 0 - no time exceeding. 
    In case of sample loss or time exceed b`0 must be 0 */
#define CRYS_RND_NonInstantiated             0UL
#define CRYS_RND_Instantiated                1UL 
#define CRYS_RND_InstantReseedAutocorrErrors 2UL 
#define CRYS_RND_InstantReseedTimeExceed     4UL
#define CRYS_RND_InstantReseedLessEntropy    8UL  
	
/* The 2-bit field in the StateFlag, defining the working or KAT modes: 
     - b`9,8: 0 - working mode, 1 - KAT DRBG mode, 2 - KAT TRNG mode, 3 - KAT
       DRBG or/and TRNG mode */
#define CRYS_RND_WorkMode                 (0UL << 8)
#define CRYS_RND_KAT_DRBG_mode 	          (1UL << 8)
#define CRYS_RND_KAT_TRNG_mode            (2UL << 8)
#define CRYS_RND_KAT_mode 	          CRYS_RND_KAT_DRBG_mode

/* The bit-field in the StateFlag, defining that the previous generated random
   block is valid for comparison with current block or not */ 
#define CRYS_RND_PreviousIsValid          (1UL << 16)

/* RND WorkBuffer = ESTIM_BUFF || ENTROPY_SOURCE_BUFF. Size of buffer = 1KB = * 
*  1024 words.  Max size (in words) of internal buffers:		      */
#define CRYS_RND_ESTIM_BUFF_SIZE_WORDS           386 /*256+128+2*/
#define CRYS_RND_ENTROPY_SOURCE_BUFF_SIZE_WORDS  520/*2+504+12+1+padding */
#define CRYS_RND_WORK_BUFF_TMP1_OFFSET  (CRYS_RND_ESTIM_BUFF_SIZE_WORDS + CRYS_RND_ENTROPY_SOURCE_BUFF_SIZE_WORDS + 4)
#define CRYS_RND_WORK_BUFF_TMP2_OFFSET  (CRYS_RND_WORK_BUFF_TMP1_OFFSET + CRYS_RND_SEED_MAX_SIZE_WORDS + 4)

/* max size of KAT entropy and nonce data in words on Fast and Slow modes*/
#define CRYS_RND_ENTROPY_TEMP_BUFFER_MAX_SIZE_WORDS  126

/* RND buffers placing: |EstimatorBuffer||RND_SrcBuffer|, where: RND_SrcBuffer =
        |Empty 2bytes|2bytes for CRYS|TRNG source|        */
#define CRYS_RND_SRC_BUFF_OFFSET_WORDS  CRYS_RND_ESTIM_BUFF_SIZE_WORDS


/* Offsets (in words) of RND estimator buffer members inside the buffer */
#define CRYS_RND_H_BUFF_OFFSET   0
#define CRYS_RND_EC_BUFF_OFFSET  256

/* Validation tag for random working state: should be set by:   	      * 
   _DX_RND_InstantiateOrReseed function on not continued mode or by           * 
*  LLF_RND_StartTrngHW function on continued mode       		      */
#define CRYS_RND_WORK_STATE_VALID_TAG  0X0123ABCD


/************************ Enums ********************************/


/************************ Structs  ******************************/

/* The internal state of DRBG mechanism based on AES CTR and CBC-MAC
   algorithms. It will be set as global data defined by the following 
   structure  */
typedef  struct  
{
	/* Seed buffer, consists from concatenated Key||V: max size 12 words */
	DxUint32_t  Seed[CRYS_RND_SEED_MAX_SIZE_WORDS];
	/* Previous value for continuous test */
	DxUint32_t  PreviousRandValue[CRYS_AES_BLOCK_SIZE_IN_WORDS];
	/* AdditionalInput buffer max size = seed max size words + 4w for padding*/
	DxUint32_t  AdditionalInput[CRYS_RND_ADDITINAL_INPUT_MAX_SIZE_WORDS+4];
	DxUint32_t  AddInputSizeWords; /* size of additional data set by user, words   */
	
	/* entropy source size in words */
	DxUint32_t  EntropySourceSizeWords;
	
	/* reseed counter (32 bits active) - indicates number of requests for entropy 
	since instantiation or reseeding */
	DxUint32_t  ReseedCounter;  
	
	/* key size: 4 or 8 words according to security strength 128 bits or 256 bits*/
	DxUint32_t KeySizeWords; 
	
	/* State flag (see definition of StateFlag above), containing bit-fields, defining: 
	- b'0: instantiation steps:   0 - not done, 1 - done; 
	- 2b'9,8: working or testing mode: 0 - working, 1 - KAT DRBG test, 2 - 
	  KAT TRNG test;
	b'16: flag defining is Previous random valid or not: 
	        0 - not valid, 1 - valid */
	DxUint32_t StateFlag; 
	
	/* Trng processing flag - indicates which ROSC lengths are:
	-  allowed (bits 0-3);
	-  total started (bits 8-11);
	-  processed (bits 16-19);           
	-  started, but not processed (bits24-27)              */
	DxUint32_t TrngProcesState;

	/* validation tag */
	DxUint32_t ValidTag;

	/* Rnd source entropy size in bits */
	DxUint32_t  EntropySizeBits;

} CRYS_RND_State_t;


/* The CRYS Random Generator Parameters  structure CRYS_RND_Params_t - 
   structure containing the user given Parameters */
typedef struct  CRYS_RND_Params_t
{
	/* parameters defining TRNG */   
	CRYS_RND_mode_t TrngMode;
	
	/* allowed ring oscillator lengths: bits 0,1,2,3  */
	DxUint32_t  RoscsAllowed;
	
	/* sampling interval: count of ring oscillator cycles between 
	   consecutive bits sampling */
	DxUint32_t  SubSamplingRatio; 

}CRYS_RND_Params_t;


/* If not supported algoritm returning macro */
#if defined CRYS_NO_AES_SUPPORT || defined CRYS_NO_RND_SUPPORT
#define RETURN_IF_RND_UNSUPPORTED( a , b , c , d , e , f , g , h , i , j, k , l , m , n , o ) \
	(a)=0;(b)=0;(c)=0;(d)=0;(e)=0;(f)=0;(g)=0;(h)=0;(i)=0;(j)=0; (k)=0;(l)=0;(m)=0;(n)=0;(o)=0; \
	(a)=(a);(b)=(b);(c)=(c);(d)=(d);(e)=(e);(f)=(f);(g)=(g);(h)=(h);(i)=(i);(j)=(j); \
	(k)=(k);(l)=(l);(m)=(m);(n)=(n);(o)=(o); \
	return CRYS_RND_IS_NOT_SUPPORTED
#else  /* !CRYS_NO_AES_SUPPORT */
#define RETURN_IF_RND_UNSUPPORTED( a , b , c , d , e , f , g , h , i , j , k , l , m , n , o ) 
#endif /* !CRYS_NO_AES_SUPPORT */
	

/************************ Typedefs  ****************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/


#ifdef __cplusplus
}
#endif

#endif


