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


#ifndef _CRYS_PKA_DEFS_HW_H_
#define _CRYS_PKA_DEFS_HW_H_



/*The size CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS should ALWAYS be >= CRYS_HASH_USER_CTX_SIZE_IN_WORDS (with the value of the SEP side)*/
#if defined(DX_CC_SEP) || defined(DX_SEP_HOST_RPC_CONFIG)
#define CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS 33
#elif defined(DX_CC_TEE)
/* adjust the context size to the HAH context size in TEE */
#define CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS 133
#else
#define CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS 256
#endif

/* size of buffers for Barrett modulus tag NP, used in PKI algorithms */
#define CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS 5
#define CRYS_PKA_ECPKI_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS  CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS   

#define CRYS_PKA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS CRYS_RSA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS

#define CRYS_PKA_INT_PUB_KEY_DB_BUFF_SIZE_IN_WORDS CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS	

#define CRYS_PKA_PUB_KEY_BUFF_SIZE_IN_WORDS (2*CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS)

#define CRYS_PKA_PRIV_KEY_BUFF_SIZE_IN_WORDS (2*CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS)

#define CRYS_PKA_KGDATA_BUFF_SIZE_IN_WORDS   (3*CRYS_PKA_MAXIMUM_MOD_BUFFER_SIZE_IN_WORDS)

#define CRYS_PKA_RSA_PRIME_INT_BUFF_SIZE_IN_WORDS 1

#define CRYS_PKA_CONV_KEY_CRT_BUFF_SIZE_IN_WORDS 1

#define CRYS_PKA_ECPKI_TMP_BUFF_MAX_LENGTH_IN_WORDS 1

#define CRYS_PKA_RND_ENTR_ESTIM_BUFF_MAX_LENGTH_IN_WORDS 514


/*! The valid maximum EC modulus size in 32-bit words */
#define CRYS_PKA_ECPKI_MODUL_MAX_LENGTH_IN_WORDS  18 /*(CRYS_ECPKI_MODUL_MAX_LENGTH_IN_BITS + 31)/(sizeof(uint32_t))*/

#define CRYS_PKA_DOMAIN_BUFF_SIZE_IN_WORDS (2*CRYS_PKA_BARRETT_MOD_TAG_BUFF_SIZE_IN_WORDS)

#define CRYS_PKA_ECDH_BUFF_MAX_LENGTH_IN_WORDS (CRYS_PKA_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1)

#define CRYS_PKA_KG_BUFF_MAX_LENGTH_IN_WORDS (2*CRYS_ECPKI_ORDER_MAX_LENGTH_IN_WORDS)

#define CRYS_PKA_EL_GAMAL_BUFF_MAX_LENGTH_IN_WORDS (4*CRYS_PKA_ECPKI_MODUL_MAX_LENGTH_IN_WORDS+4)

#define CRYS_PKA_ECDSA_SIGNE_BUFF_MAX_LENGTH_IN_WORDS (5*CRYS_PKA_ECPKI_MODUL_MAX_LENGTH_IN_WORDS)

#define CRYS_PKA_ECDSA_VERIFY_BUFF_MAX_LENGTH_IN_WORDS (3*CRYS_PKA_ECPKI_MODUL_MAX_LENGTH_IN_WORDS)





#endif /*_CRYS_PKA_DEFS_HW_H_*/

