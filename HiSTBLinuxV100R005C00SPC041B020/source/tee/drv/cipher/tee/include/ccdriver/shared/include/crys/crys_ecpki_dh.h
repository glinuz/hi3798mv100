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

 
 #ifndef CRYS_ECPKI_DH_H
#define CRYS_ECPKI_DH_H

/*
 *  Object name     :  CRYS_ECPKI_DH.h
 *  State           :  %state%
 *  Creation date   :  02.02.2006
 *  Last modified   :  %modify_time%
 */
/** @file CRYS_ECPKI_DH.h
 * \brief Defines the API that supports EC Diffie-Hellman
 *        shared secret value derivation primitives
 *
 * \version CRYS_ECPKI_DH.h #1:incl:1
 * \author R.Levin
 */


#include "crys_ecpki_types.h"
#include "crys_ecpki_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 *		         CRYS_ECDH_SVDP_DH function                            *				     
 ***********************************************************************/
/**
 @brief 	Creates the shared secret value accordingly to the IEEE 1363-2000 
 			standard. 

	       This function performs the following:
 			
		-# Checks input parameters: pointers and EC Domain ID.
		-# Derives the partner public key and calls the LLF_ECPKI_SVDP_DH function,
		    which performs EC SVDP operations. On errors, outputs error messages.
		-# Exits.
	\note  The term "user" indicates any party who performs a calculation of a shared 
	secret value using this primitive. The term "partner" indicates any other party of 
	shared secret value calculation.
 
 @param[in]  PartnerPublKey_ptr  A pointer to a partner public key W 
 @param[in]  UserPrivKey_ptr	        A pointer to a user private key  
 @param[out] SharedSecretValue_ptr      A pointer to an output buffer that will contain
                                        the shared secret value 
 @param[in,out] SharedSecrValSize_ptr   A pointer to the size of user passed buffer (in) and 
                                        actual output size  of calculated shared secret value.
 @param[in]  TempBuff_ptr               A pointer to a temporary buffers of size specified in 
                                        the CRYS_ECDH_TempData_t structure.
 @return <b>CRYSError_t</b>: <br> 
			 CRYS_OK<br> 
                         CRYS_ECDH_SVDP_DH_INVALID_USER_PRIV_KEY_PTR_ERROR<br> 
                         CRYS_ECDH_SVDP_DH_USER_PRIV_KEY_VALID_TAG_ERROR<br> 
 			 CRYS_ECDH_SVDP_DH_INVALID_PARTNER_PUBL_KEY_PTR_ERROR<br> 
			 CRYS_ECDH_SVDP_DH_PARTNER_PUBL_KEY_VALID_TAG_ERROR<br> 
                         CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_PTR_ERROR<br> 
			 CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_PTR_ERROR<br> 
			 CRYS_ECDH_SVDP_DH_INVALID_SHARED_SECRET_VALUE_SIZE_ERROR<br> 
			 CRYS_ECDH_SVDP_DH_INVALID_TEMP_DATA_PTR_ERROR<br> 
			 
   NOTE:    1. The partner public key and user private key must relate to the same DomainID.
            2. The public key must be full validated befor using in this primitive.
            3. Buffer size for SharedSecretValue must be >= ModulusSizeInWords*4 bytes,  
               output size of shared value is equal to ModulusSizeInWords*4. 
			 
*/
CIMPORT_C CRYSError_t CRYS_ECDH_SVDP_DH( 
						CRYS_ECPKI_UserPublKey_t *PartnerPublKey_ptr, /*in*/
						CRYS_ECPKI_UserPrivKey_t *UserPrivKey_ptr,           /*in*/							                
						uint8_t		         *SharedSecretValue_ptr,     /*out*/
						uint32_t               *SharedSecrValSize_ptr,     /*in/out*/
						CRYS_ECDH_TempData_t     *TempBuff_ptr               /*in*/ );



#ifdef __cplusplus
}
#endif			

#endif
