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

#ifndef SW_CRYS_RSA_SHARED_TYPES_H
#define SW_CRYS_RSA_SHARED_TYPES_H


/*
* All the includes that are needed for code using this module to
* compile correctly should be #included here.
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*
 *  Object % ccsw_crys_rsa_types.h    : %
 *  State           :  %state%
 *  Creation date   :  Wed Nov 17 16:42:30 2004
 *  Last modified   :  %modify_time%
 */
/** @file
 *  \brief This file contains all of the enums and definitions 
 *         that are used for the CRYS AES APIs, as well as the APIs themselves.
 *
 *  \version ccsw_crys_rsa_types.h#1:incl:13
 *  \author adams
 */

/************************ Defines ******************************/

/************************************************************************/
/* the following defintions are only relevant for RSA code on SW */
/************************************************************************/
/* Define the maximal allowed width of the exponentiation sliding window 
in range 2...6. This define is actual for projects on soft platform.
To minimize code size use the minimum value. To optimize performance 
choose the maximum value */

/* The valid key size in bits */
#define SW_CRYS_RSA_MIN_VALID_KEY_SIZE_VALUE_IN_BITS          512

#ifndef CRYS_NO_RSA_MAX_KEY_SIZE_4096_BIT_SUPPORT                     
#define SW_CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS         4096        
#define SW_CRYS_RSA_MAX_KEY_GENERATION_SIZE_BITS             4096        
#else                                                                 
#ifndef CRYS_NO_RSA_MAX_KEY_SIZE_3072_BIT_SUPPORT
#define SW_CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS         3072
#define SW_CRYS_RSA_MAX_KEY_GENERATION_SIZE_BITS             3072   
#else 
#define SW_CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS         2048      
#define SW_CRYS_RSA_MAX_KEY_GENERATION_SIZE_BITS             2048   
#endif  
#endif  





/* Define the size of the exponentiation temp buffer, used in LLF_PKI exponentiation and NON DEPENDED on
   width of the sliding window. The size defined in units equaled to maximal RSA modulus size */
#define PKI_EXP_CONST_TEMP_BUFF_SIZE_IN_MODULUS_UNITS  7

#define PKI_EXP_SLIDING_WINDOW_MAX_VALUE  2

    /* Size of buffers for sliding window exponents */
#if (PKI_EXP_SLIDING_WINDOW_MAX_VALUE == 6)
#define PKI_EXP_WINDOW_TEMP_BUFFER_SIZE_IN_MODULUS_UNITS  34
#else
#define PKI_EXP_WINDOW_TEMP_BUFFER_SIZE_IN_MODULUS_UNITS  (3 + (1 << (PKI_EXP_SLIDING_WINDOW_MAX_VALUE-1)))
#endif

	/* The maximum buffer size for the 'H' value */
#define SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS ((SW_CRYS_RSA_MAX_VALID_KEY_SIZE_VALUE_IN_BITS + 64UL ) / 32 )


/* definition of PKI_KEY_GEN_TEMP_BUFF_SIZE_WORDS IS DEPENDED on width of the sliding window*/
#if( PKI_EXP_SLIDING_WINDOW_MAX_VALUE > 2 )
#define PKI_KEY_GEN_TEMP_BUFF_SIZE_WORDS \
	((4 + (1<<(PKI_EXP_SLIDING_WINDOW_MAX_VALUE-2))) * SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS)
#else
#define PKI_KEY_GEN_TEMP_BUFF_SIZE_WORDS \
	(16 * SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS)
#endif


/* Define the size of the temp buffer, used in LLF_PKI exponentiation and DEPENDED on
   width of the sliding window in words */
#if (PKI_EXP_CONST_TEMP_BUFF_SIZE_IN_MODULUS_UNITS > PKI_EXP_WINDOW_TEMP_BUFFER_SIZE_IN_MODULUS_UNITS )
#define PKI_EXP_TEMP_BUFFER_SIZE_IN_WORDS  \
	(PKI_EXP_CONST_TEMP_BUFF_SIZE_IN_MODULUS_UNITS * SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS + 2 )
#else
#define PKI_EXP_TEMP_BUFFER_SIZE_IN_WORDS  \
	(PKI_EXP_WINDOW_TEMP_BUFFER_SIZE_IN_MODULUS_UNITS * SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS + 2 )
#endif


/* the RSA data type */
typedef struct SW_Shared_CRYS_RSAPrimeData_t {
	/* The aligned input and output data buffers */
	uint32_t DataIn[SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
	uint32_t DataOut[SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];

	/* #include specific fields that are used by the low level */
	struct {
		union {
			struct { /* Temporary buffers used for the exponent calculation */
				uint32_t Tempbuff1[PKI_EXP_TEMP_BUFFER_SIZE_IN_WORDS];
				uint32_t Tempbuff2[SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS * 2];
				/* Temporary buffer for self-test support */
				uint32_t TempBuffer[SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS];
			}NonCrt;

			struct { /* Temporary buffers used for the exponent calculation */
				uint32_t Tempbuff1[PKI_EXP_TEMP_BUFFER_SIZE_IN_WORDS];
				uint32_t Tempbuff2[SW_CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS * 2];
			}Crt;
		}Data;
	}LLF;

}SW_Shared_CRYS_RSAPrimeData_t; 




#ifdef __cplusplus
}
#endif
#endif 
