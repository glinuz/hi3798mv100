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

 
 
#ifndef CRYS_ERROR_H
#define CRYS_ERROR_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "dx_pal_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object name     :  crys_error.h
   *  State           :  %state%
   *  Creation date   :  Wed Nov 17 16:55:47 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief This module defines the error return code types and the numbering spaces of the error codes
   *         for each module of the layers listed below.
   *
   *  \version crys_error.h#1:incl:1
   *  \author adams
   */


/************************ Defines ******************************/
/**
 * \brief The definitions of the error number space used for the different modules
 */

/* ........... Error base numeric mapping definitions ................... */
/* ----------------------------------------------------------------------- */
 
 /* The global error base number */
#define CRYS_ERROR_BASE          0x00F00000UL

/* The error range number assigned for each layer */
#define CRYS_ERROR_LAYER_RANGE   0x00010000UL

/* The error range number assigned to each module on its specified layer */
#define CRYS_ERROR_MODULE_RANGE  0x00000100UL

/* Defines the layer index for the error mapping */
#define CRYS_LAYER_ERROR_IDX  0UL
#define LLF_LAYER_ERROR_IDX   1UL
#define LLFCD_LAYER_ERROR_IDX 2UL
#define VOS_LAYER_ERROR_IDX   3UL
#define PLAT_LAYER_ERROR_IDX  4UL
#define GENERIC_ERROR_IDX     5UL

/* Defines the module index for error mapping */
#define AES_ERROR_IDX         0x00UL
#define DES_ERROR_IDX         0x01UL
#define HASH_ERROR_IDX        0x02UL
#define HMAC_ERROR_IDX        0x03UL
#define RSA_ERROR_IDX         0x04UL
#define DH_ERROR_IDX          0x05UL
#define RC4_ERROR_IDX         0x06UL
#define INTERNAL_ERROR_IDX    0x07UL
#define ECPKI_ERROR_IDX       0x08UL
#define CMLA_ERROR_IDX        0x09UL
#define CRYS_SST_ERROR_IDX    0x0AUL
#define PKI_ERROR_IDX         0x0BUL
#define RND_ERROR_IDX         0x0CUL
#define COMMON_ERROR_IDX      0x0DUL
#define CCM_ERROR_IDX         0x0EUL
#define INIT_ERROR_IDX        0x0FUL
#define C2_ERROR_IDX          0x10UL
#define KDF_ERROR_IDX         0x11UL
#define KMNG_ERROR_IDX        0x12UL
#define OTF_ERROR_IDX         0x13UL
#define FLOW_ERROR_IDX        0x14UL
#define AESCCM_ERROR_IDX      0x15UL
#define OTF_MC_ERROR_IDX      0x16UL
#define FIPS_ERROR_IDX        0x17UL
#define BYPASS_ERROR_IDX      0x18UL
#define COMBINED_ERROR_IDX    0x19UL
#define SELF_TEST_ERROR_IDX   0x20UL

 



/* .......... defining the error spaces for each module on each layer ........... */
/* ------------------------------------------------------------------------------ */

/* AES module on the CRYS layer base address - 0x00F00000 */
#define CRYS_AES_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * AES_ERROR_IDX ) )  
                                    

/* AES module on the LLF layer base address -  0x00F10000 */
#define LLF_AES_MODULE_ERROR_BASE   (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * AES_ERROR_IDX ) )   

/* AES module on the LLFCD layer base address - 0x00F20000 */
#define LLFCD_AES_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLFCD_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * AES_ERROR_IDX ) )
                                    
/* DES module on the CRYS layer base address - 0x00F00100 */
#define CRYS_DES_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * DES_ERROR_IDX ) )   
                                    

/* DES module on the LLF layer base address -  0x00F10100 */
#define LLF_DES_MODULE_ERROR_BASE   (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * DES_ERROR_IDX ) )   

/* DES module on the LLFCD layer base address - 0x00F20100 */
#define LLFCD_DES_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLFCD_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * DES_ERROR_IDX ) ) 
                                    
/* HASH module on the CRYS layer base address - 0x00F00200 */
#define CRYS_HASH_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * HASH_ERROR_IDX ) )   
                                    

/* HASH module on the LLF layer base address -  0x00F10200 */
#define LLF_HASH_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * HASH_ERROR_IDX ) )   

/* HASH module on the LLFCD layer base address - 0x00F20200 */
#define LLFCD_HASH_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                      (CRYS_ERROR_LAYER_RANGE * LLFCD_LAYER_ERROR_IDX) + \
                                      (CRYS_ERROR_MODULE_RANGE * HASH_ERROR_IDX ) )

/* HMAC module on the CRYS layer base address - 0x00F00300 */
#define CRYS_HMAC_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * HMAC_ERROR_IDX ) )   

/* PKI RSA module on the CRYS layer base address - 0x00F00400 */
#define CRYS_RSA_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                   (CRYS_ERROR_MODULE_RANGE * RSA_ERROR_IDX ) )   

/* DH module on the CRYS layer base address - 0x00F00500 */
#define CRYS_DH_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                   (CRYS_ERROR_MODULE_RANGE * DH_ERROR_IDX ) ) 

/* RC4 module on the CRYS layer base address - 0x00F00600 */
#define CRYS_RC4_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                   (CRYS_ERROR_MODULE_RANGE * RC4_ERROR_IDX ) ) 
                                   
/* RC4 module on the LLF layer base address -  0x00F10600 */
#define LLF_RC4_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                   (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                   (CRYS_ERROR_MODULE_RANGE * RC4_ERROR_IDX ) )   
                                     

/* INTERNAL debug error module on the CRYS layer base address - 0x00F00700 */
#define CRYS_INTERNAL_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                        (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                        (CRYS_ERROR_MODULE_RANGE * INTERNAL_ERROR_IDX ) )  
                                        
/* ECPKI module on the CRYS layer base address - 0x00F00800 */
#define CRYS_ECPKI_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * ECPKI_ERROR_IDX ) ) 
                                     
/* ECPKI module on the LLF layer base address -  0x00F10800 */
#define LLF_ECPKI_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * ECPKI_ERROR_IDX ) )
                                     
/* CMLA module on the CRYS layer base address - 0x00F00900 */
#define CRYS_CMLA_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * CMLA_ERROR_IDX ) )   
                                                                              
/* CRY_SST module on the CRYS layer base address - 0x00F00A00 */
#define CRYS_SST_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * CRYS_SST_ERROR_IDX ) )   

/* PKI RSA module on the LLF layer base address -  0x00F10B00 */
#define LLF_PKI_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                   (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                   (CRYS_ERROR_MODULE_RANGE * PKI_ERROR_IDX ) )   

/* RND module on the CRYS layer base address - 0x00F00C00 */
#define CRYS_RND_MODULE_ERROR_BASE   (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * RND_ERROR_IDX ) )   
                                    
/* RND module on the LLF layer base address -  0x00F10C00 */
#define LLF_RND_MODULE_ERROR_BASE    (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * RND_ERROR_IDX ) )   

/* RND module on the LLFCD layer base address - 0x00F20C00 */
#define LLFCD_RND_MODULE_ERROR_BASE   (CRYS_ERROR_BASE + \
                                      (CRYS_ERROR_LAYER_RANGE * LLFCD_LAYER_ERROR_IDX) + \
                                      (CRYS_ERROR_MODULE_RANGE * RND_ERROR_IDX ) )

/* COMMMON module on the CRYS layer base address - 0x00F00D00 */
#define CRYS_COMMON_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * COMMON_ERROR_IDX ) )                                          
                                      
/* COMMMON module on the LLF layer base address - 0x00F10D00 */
#define LLF_COMMON_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * COMMON_ERROR_IDX ) )                                          
                                      
/* CCM module on the CRYS layer base address - 0x00F00E00 */
#define CRYS_CCM_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * CCM_ERROR_IDX ) )                                          

/* INIT module on the CRYS layer base address - 0x00F00F00 */
#define CRYS_INIT_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * INIT_ERROR_IDX ) ) 
                                     
/* C2 module on the CRYS layer base address - 0x00F01000 */
#define CRYS_C2_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                  (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                  (CRYS_ERROR_MODULE_RANGE * C2_ERROR_IDX ) ) 
                                  
/* C2 module on the LLF layer base address - 0x00F11000 */
#define LLF_C2_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                  (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                  (CRYS_ERROR_MODULE_RANGE * C2_ERROR_IDX ) ) 
                                  
/* KDF module on the CRYS layer base address - 0x00F01100 */
#define CRYS_KDF_MODULE_ERROR_BASE (CRYS_ERROR_BASE + \
                                  (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                  (CRYS_ERROR_MODULE_RANGE * KDF_ERROR_IDX ) ) 

/* KMNG module on the CRYS layer base address -  0x00F01200 */
#define CRYS_KMNG_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * KMNG_ERROR_IDX ) ) 
                                     
/* OTF module on the CRYS layer base address -  0x00F01300 */
#define CRYS_OTF_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * OTF_ERROR_IDX ) ) 
                                     
/* OTF module on the LLF layer base address -  0x00F11300 */
#define LLF_OTF_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * OTF_ERROR_IDX ) ) 
                                     
/* FLOW module on the CRYS layer base address -  0x00F01400 */
#define CRYS_FLOW_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                     (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                     (CRYS_ERROR_MODULE_RANGE * FLOW_ERROR_IDX ) ) 

/* AESCCM module on the CRYS layer base address - 0x00F01500 */
#define CRYS_AESCCM_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
	                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
	                                   (CRYS_ERROR_MODULE_RANGE * AESCCM_ERROR_IDX ) )  

/* OTF module on the LLF layer base address -  0x00F11500 */
#define LLF_AESCCM_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                      (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                      (CRYS_ERROR_MODULE_RANGE * AESCCM_ERROR_IDX ) ) 
/* OTF MultiContext module on the CRYS layer base address - 0x00F01600 */
#define CRYS_OTF_MC_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
	                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
	                                   (CRYS_ERROR_MODULE_RANGE * OTF_MC_ERROR_IDX ) )                                    

/* OTF module on the LLF layer base address -  0x00F11600 */
#define LLF_OTF_MC_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                      (CRYS_ERROR_LAYER_RANGE * LLF_LAYER_ERROR_IDX) + \
                                      (CRYS_ERROR_MODULE_RANGE * OTF_MC_ERROR_IDX ) ) 

/* FIPS module on the CRYS layer base address - 0x00F01700 */
#define CRYS_FIPS_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
	                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
	                                   (CRYS_ERROR_MODULE_RANGE * FIPS_ERROR_IDX ) )	                                       
             
/* BYPASS module on the CRYS layer base address - 0x00F01800 */
#define CRYS_BYPASS_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
	                                   (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
	                                   (CRYS_ERROR_MODULE_RANGE * BYPASS_ERROR_IDX ) )

/* Combined mode module on the CRYS layer base address - 0x00F01900 */
#define CRYS_COMBINED_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                           (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                           (CRYS_ERROR_MODULE_RANGE * COMBINED_ERROR_IDX ) )

/* SELF TEST module on the CRYS layer base address -  */
#define CRYS_SELF_TEST_MODULE_ERROR_BASE  (CRYS_ERROR_BASE + \
                                           (CRYS_ERROR_LAYER_RANGE * CRYS_LAYER_ERROR_IDX) + \
                                           (CRYS_ERROR_MODULE_RANGE * SELF_TEST_ERROR_IDX ) )

	

/* VOS platform layer base address - 0x00F30000 */
#define VOS_ERROR_BASE ( CRYS_ERROR_BASE + \
                        (CRYS_ERROR_LAYER_RANGE * VOS_LAYER_ERROR_IDX) )  
                                                         
/* User platform layer base address - 0x00F40000 */
#define PLAT_ERROR_BASE ( CRYS_ERROR_BASE + \
                         (CRYS_ERROR_LAYER_RANGE * PLAT_LAYER_ERROR_IDX) )
                    
/* User generic layer base address - 0x00F50000 */
#define GENERIC_ERROR_BASE ( CRYS_ERROR_BASE + (CRYS_ERROR_LAYER_RANGE * GENERIC_ERROR_IDX) )
#define CRYS_FATAL_ERROR			(GENERIC_ERROR_BASE + 0x00UL)
#define CRYS_OUT_OF_RESOURCE_ERROR		(GENERIC_ERROR_BASE + 0x01UL)
#define CRYS_ILLEGAL_RESOURCE_VAL_ERROR		(GENERIC_ERROR_BASE + 0x02UL)

                    
                    
/* ............ The OK (success) definition ....................... */
#define CRYS_OK 0                         
                                                                                                            

#define DX_CRYS_RETURN_ERROR(retCode, retcodeInfo, funcHandler) \
	((retCode) == 0 ? CRYS_OK : funcHandler(retCode, retcodeInfo))

/************************ Enums ********************************/


/************************ Typedefs  ****************************/

/**
 * \brief The typedef definition of all of the error codes that are returned from the CRYS functions 
 */
 
typedef uint32_t CRYSError_t;

/************************ Structs  ******************************/


/************************ Public Variables **********************/


/************************ Public Functions **********************/

#ifdef __cplusplus
}
#endif

#endif




