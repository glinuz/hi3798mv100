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

 
 #ifndef CRYS_ECPKI_TYPES_H
#define CRYS_ECPKI_TYPES_H
/*
   *  Object % CRYS_ECPKI_Types.h    : %
   *  State           :  %state%
   *  Creation date   :  05/12/2005
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief Contains all of the enums and definitions 
   *         that are used for the CRYS ECPKI APIs, as well as the APIs themselves.
   *
   *  \version CRYS_ECPKI_Types.hh#1:incl:1
   *  \author R.Levin
   *           All rights reserved.
   */

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */


#include "crys_rsa_types.h"
#include "crys_hash.h"
#include "crys_pka_defs_hw.h"
#include "dx_pal_compiler.h"

#ifdef __cplusplus
extern "C"
{
#endif

 
/************************ Defines ******************************/

/*! The valid maximum EC modulus size in 32-bit words */
#define CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS  18 /*(CRYS_ECPKI_MODUL_MAX_LENGTH_IN_BITS + 31)/(sizeof(uint32_t))*/
#define CRYS_ECPKI_ORDER_MAX_LENGTH_IN_WORDS  (CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1)

/* The type defines integer array of lengths of maximum lengths of EC modulus */ 
typedef  uint32_t  CRYS_ECPKI_ARRAY_t[CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];

/*! Predefined arithmetic reduction mode for modular multiplication:
   Choose Montgomery or Barret. Note: Barret is not currently supported. */
#define CRYS_ECPKI_DEFINED_REDUCTION_MODE   Montgomery  /* Barret */

#define CRYS_DEFS_DUMMY_AES_BLOCK_SIZE 16
/**************************************************************************************
 *	              Enumerators
 ***************************************************************************************/

/* Enumerator for the EC finite field Identificator */
typedef enum
{
	GFp = 0,
	GF2 = 1,
	
	FieldID_OffMode,
			
	CRYS_ECPKI_FieldIDLast= 0x7FFFFFFF,

} CRYS_ECPKI_FieldID_t;

/*------------------------------------------------------------------*/
/* Enumerator for the EC Domain Identificator 
   References: [13] - SEC 2: Recomended elliptic curve domain parameters. 
                      Version 1.0. Certicom 2000. 
               [8]  - WAP-261-WTLS-20010406-a, Version 06-April-2001     */

typedef enum
{
   /* For prime field */
   CRYS_ECPKI_DomainID_secp160k1,   /* EC secp160r1 from [13] (identic 7 from [8]), used in CRYS 3.5 */
   CRYS_ECPKI_DomainID_secp160r1,   /* EC secp160k1 from [13]  */
   CRYS_ECPKI_DomainID_secp160r2,   /* EC secp160r2 from [13]  */
   CRYS_ECPKI_DomainID_secp192k1,   /* EC secp192k1 from [13]  */
   CRYS_ECPKI_DomainID_secp192r1,   /* EC secp192r1 from [13]  */
   CRYS_ECPKI_DomainID_secp224k1,   /* EC secp224k1 from [13]  */
   CRYS_ECPKI_DomainID_secp224r1,   /* EC secp224r1 from [13]  */
   CRYS_ECPKI_DomainID_secp256k1,   /* EC secp256k1 from [13]  */
   CRYS_ECPKI_DomainID_secp256r1,   /* EC secp256r1 from [13]  */
   CRYS_ECPKI_DomainID_secp384r1,   /* EC secp384r1 from [13]  */
   CRYS_ECPKI_DomainID_secp521r1,   /* EC secp521r1 from [13]  */
   
   CRYS_ECPKI_DomainID_OffMode, 

   CRYS_ECPKI_DomainIDLast      = 0x7FFFFFFF,

}CRYS_ECPKI_DomainID_t;   


/*------------------------------------------------------------------*/
/* Defines the enum for the HASH operation mode.
 * The enumerator defines 6 HASH modes according to IEEE 1363.
 * In the WMDRM DSA is used SHA1 mode only.
 */
typedef enum
{   
   CRYS_ECPKI_HASH_SHA1_mode    = 0,
   CRYS_ECPKI_HASH_SHA224_mode  = 1,
   CRYS_ECPKI_HASH_SHA256_mode  = 2,
   CRYS_ECPKI_HASH_SHA384_mode  = 3,
   CRYS_ECPKI_HASH_SHA512_mode  = 4,
   
   CRYS_ECPKI_AFTER_HASH_SHA1_mode    = 5,
   CRYS_ECPKI_AFTER_HASH_SHA224_mode  = 6,
   CRYS_ECPKI_AFTER_HASH_SHA256_mode  = 7,
   CRYS_ECPKI_AFTER_HASH_SHA384_mode  = 8,
   CRYS_ECPKI_AFTER_HASH_SHA512_mode  = 9,
   

   CRYS_ECPKI_HASH_NumOfModes,

   CRYS_ECPKI_HASH_OpModeLast        = 0x7FFFFFFF,

}CRYS_ECPKI_HASH_OpMode_t;   
  

/*---------------------------------------------------*/
/* Enumerator for the EC point coordinates identificator */
typedef enum
{   
   EC_PointAffine           = 0,   /* Affine coordinates */
   EC_PointJacobian         = 1,   /* Jacobian projective coordinates */
   EC_PointProjStandard     = 2,   /* Standard projective coordinates */
   EC_PointChudnovsky       = 3,   /* Chudnovsky projective coordinates */
   
   EC_PointID_OffMode       = 4,
   
   CRYS_ECPKI_PointIDLast  = 0x7FFFFFFF,

}CRYS_ECPKI_PointID_t; 

/*---------------------------------------------------*/
/* Enumerator for the EC point compression identificator */
typedef enum
{   
   CRYS_EC_PointCompressed     = 2,   
   CRYS_EC_PointUncompressed   = 4,      
   CRYS_EC_PointContWrong      = 5, /* wrong Point Control value */ 
   CRYS_EC_PointHybrid         = 6,

   CRYS_EC_PointCompresOffMode = 8,

   CRYS_ECPKI_PointCompressionLast= 0x7FFFFFFF,

}CRYS_ECPKI_PointCompression_t; 

/*----------------------------------------------------*/
/*  Enumerator for compatibility ofthe DHC 
   with cofactor multiplication to DH ordinary */
typedef enum
{
   CRYS_ECPKI_SVDP_DHC_CompatibleDH         = 0,   /* Requested compatiblity of SVDP_DHC with cofactor to SVDP_DH */
   CRYS_ECPKI_SVDP_DHC_NonCompatibleDH	    = 1,   /* Compatiblity of SVDP_DHC with cofactor to SVDP_DH is not requested */
   
   CRYS_ECPKI_SVDP_DHC_OffMode,
    
   CRYS_ECPKI_SVDP_DHC_OpModeLast         = 0x7FFFFFFF,

}CRYS_ECPKI_SVDP_DHC_OpMode_t; 

/*----------------------------------------------------*/
/* Enumerator for choosing reduction mode on EC arithmetic operations */
typedef enum {
   Montgomery   = 0,   /*  */
   Barret       = 1,   /*  */
   
   ReductionOffMode,
    
   ReductionModeLast= 0x7FFFFFFF,

}ReductionMode_t; 

#define DEFINED_REDUCTION_MODE  Montgomery

/*----------------------------------------------------*/
/* Enumerator for indication of EC adding or subtracting operation on FullAdd algotithm */
typedef enum {
   Add            = 0,   /*  */
   Subtract       = 1,   /*  */
      
   AddSubtract_OffMode,
    
   AddSubtractModeLast= 0x7FFFFFFF,

}AddSubtractMode_t; 

/*----------------------------------------------------*/
/* Enumerator for indication what checking of EC public key must be performed */
typedef enum {
   CheckPointersAndSizesOnly = 0,   /* Only preliminary input parameters checking */
   ECpublKeyPartlyCheck      = 1,   /* In addition check that EC PubKey is point on curve */
   ECpublKeyFullCheck        = 2,   /* In addition check that EC_GeneratorOrder*PubKey = O */

   PublKeyChecingOffMode,
    
   EC_PublKeyCheckModeLast  = 0x7FFFFFFF,

}EC_PublKeyCheckMode_t; 


/***************************************************************************************
 *	             EC arithmetic operations mode                                    
 ***************************************************************************************/
typedef struct {
   CRYS_ECPKI_FieldID_t      Field;	  
   ReductionMode_t           ReductionMode;

} CRYS_ECPKI_ArithmOpMode_t;

/**************************************************************************************
 *	             EC DomainInfo structure definition
 ***************************************************************************************/
/*! This structure contains the EC domain data used on the CRYS level. */
typedef  struct {		/*! Size of field modulus  in bits and in words*/
	uint16_t      EC_ModulusSizeInBits;
	/*! Sizes of order of generator in bits and in words*/
	uint16_t      EC_OrderSizeInBits;
	/*! EC Domain identifier: number of curve*/
	CRYS_ECPKI_DomainID_t	DomainID;
	/*! EC finite field identifier*/
	CRYS_ECPKI_FieldID_t	FieldID;
	/* MS byte of the modulus */
	uint8_t    ModulusMSByte;
} CRYS_ECPKI_DomainInfo_t ;    

/**************************************************************************************
 *	             EC  Domain structure definition
 ***************************************************************************************/
					 
/*! The structure containing the EC domain parameters in little-endian form */
 
typedef  struct {	
	 
	/*! Field modulus:  GF_Modulus = P*/ 
	uint32_t	GF_Modulus [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];
	/*! EC equation parameters a, b*/
	uint32_t	EC_Param_A [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];
	uint32_t	EC_Param_B [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];
	/*! Order of generator: EC_GenerOrder*/
	uint32_t	EC_GenerOrder [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1];
	/*! Generator (EC base point) coordinates in projective form*/
	uint32_t	EC_Gener_X [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];
	uint32_t	EC_Gener_Y [CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS];
	/*! EC cofactor EC_Cofactor_K*/
	uint32_t  EC_Cofactor_K;
	/*! EC cofactor inverse EC_CofactorInv*/
	uint32_t	EC_CofactorInv [1/*CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS*/];
	/*! Size of field modulus in bits and in words*/
	uint16_t  EC_ModulusSizeInBits;
	uint16_t  EC_ModulusSizeInWords;
	/*! Size of generator order in bits and in words*/
	uint16_t  EC_OrderSizeInBits;	
	uint16_t  EC_OrderSizeInWords;
	
	/* include the specific fields that are used by the low level */
	uint32_t crysDomainIntBuff[CRYS_PKA_DOMAIN_BUFF_SIZE_IN_WORDS];
	
	/*! EC Domain identifier*/
	CRYS_ECPKI_DomainID_t	DomainID;
	/*! EC finite field identifier*/
	CRYS_ECPKI_FieldID_t	FieldID;	
 
} CRYS_ECPKI_Domain_t; 			   



/**************************************************************************************
 *	             EC  point structures definitions
 ***************************************************************************************/
					 
/*! The structure containing the EC point in affine coordinates 
   and little endian form */
typedef  struct  
{
    /* Point coordinates */	  	
    /*! X-coordinate */
    CRYS_ECPKI_ARRAY_t			CoordX;     
    /*! Y-coordinate */
    CRYS_ECPKI_ARRAY_t			CoordY;     
         
}CRYS_ECPKI_PointAffine_t;


/*! The structure containing the EC point in any type of coordinates,
   defined by PointID. */
typedef  struct CRYS_ECPKI_Point_t 
{
 	  	
    /*! X-coordinate */
    CRYS_ECPKI_ARRAY_t			CoordX;
    /*! Y-coordinate */
    CRYS_ECPKI_ARRAY_t			CoordY;
    /*! Z-coordinate */
    CRYS_ECPKI_ARRAY_t			CoordZ;     
    /*! Enum variable: defines type of point presentation*/
    CRYS_ECPKI_PointID_t		PointID;   
}CRYS_ECPKI_Point_t;



/**************************************************************************************
 *	              ECPKI public and private key  Structures
 ***************************************************************************************/

/* --------------------------------------------------------------------- */
/* .................. The public key structures definitions ............ */
/* --------------------------------------------------------------------- */

/*! The structure containing the Public Key in affine coordinates.

   Size = 2*CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1 + 
   ( sizeof(LLF_ECPKI_publ_key_db_def.h) = 0 )          */

typedef  struct  
{
    /*! Public Key coordinates */	  	

    /*! X-coordinate */
    CRYS_ECPKI_ARRAY_t			PublKeyX; 
    /*! Y-coordinate */
    CRYS_ECPKI_ARRAY_t			PublKeyY;
    /*! Enumerator variable: defines current EC domain*/
    CRYS_ECPKI_DomainID_t		DomainID;
    
    /* include the specific fields that are used by the low level */
    uint32_t crysPubKeyIntBuff[CRYS_PKA_PUB_KEY_BUFF_SIZE_IN_WORDS];     
} CRYS_ECPKI_PublKey_t;


/*! The user structure containing EC public key data base form.
 
   Size = 2*CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 2 + 
   ( sizeof(LLF_ECPKI_publ_key_db_def.h) = 0 )          */

typedef struct   CRYS_ECPKI_UserPublKey_t 
{   
   uint32_t 	valid_tag;  
   uint32_t  	PublKeyDbBuff[(sizeof(CRYS_ECPKI_PublKey_t)+3)/4]; 
   
}  CRYS_ECPKI_UserPublKey_t;


/* --------------------------------------------------------------------- */
/* .................. The private key structures definitions ........... */
/* --------------------------------------------------------------------- */

/*! The EC private key structure in little endian form.
 
   Size = CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 2 + 
   ( sizeof(LLF_ECPKI_priv_key_db_def.h) = 0 )        */
typedef  struct   
{
    /*! Private Key data */
   uint32_t	    PrivKey[CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 1]; 
   /*! Enumerator variable: defines current EC domain */
   CRYS_ECPKI_DomainID_t     DomainID;
   /* include the specific fields that are used by the low level. */
   uint32_t crysPrivKeyIntBuff[CRYS_PKA_PRIV_KEY_BUFF_SIZE_IN_WORDS];
    
}CRYS_ECPKI_PrivKey_t;


/*! The user structure containing EC private key data base in little endian form.
 
   Size = CRYS_ECPKI_MODUL_MAX_LENGTH_IN_WORDS + 3 + 
   ( sizeof(LLF_ECPKI_priv_key_db_def.h) = 0 )          */
typedef struct   CRYS_ECPKI_UserPrivKey_t 
{   
   uint32_t 	valid_tag;  
   uint32_t    PrivKeyDbBuff[(sizeof(CRYS_ECPKI_PrivKey_t)+3)/4];
    
}  CRYS_ECPKI_UserPrivKey_t;


/*! The ECPKI KG temporary data type */
typedef struct CRYS_ECPKI_KG_TempData_t
{
   /* #include specific fields that are used by the low level */
    uint32_t crysKGIntBuff[CRYS_PKA_KG_BUFF_MAX_LENGTH_IN_WORDS];
}CRYS_ECPKI_KG_TempData_t; 

/*!  The ECDH temporary data type  */
typedef struct CRYS_ECDH_TempData_t
{
   /* #include specific fields that are used by the low level /such as for ECPKI KG */
	uint32_t crysEcdhIntBuff[CRYS_PKA_ECDH_BUFF_MAX_LENGTH_IN_WORDS];
}CRYS_ECDH_TempData_t; 

/*!  The EC ELGAMAL temporary data type */
typedef struct CRYS_EC_ELGAMAL_TempData_t
{
   /* #include specific fields that are used by the low level /such as for ECPKI KG */
    uint32_t  crysElgamalIntBuff[CRYS_PKA_EL_GAMAL_BUFF_MAX_LENGTH_IN_WORDS];
}CRYS_EC_ELGAMAL_TempData_t; 

typedef struct CRYS_ECPKI_BUILD_TempData_t
{
   /* #include specific fields that are used by the low level /such as for ECPKI KG */
    uint32_t  crysBuildTmpIntBuff[CRYS_PKA_ECPKI_TMP_BUFF_MAX_LENGTH_IN_WORDS];
}CRYS_ECPKI_BUILD_TempData_t;

/**************************************************************************
 *	              CRYS ECDSA context structures
 **************************************************************************/

/* --------------------------------------------------------------------- */
/*                CRYS ECDSA Signing context structure                   */
/* --------------------------------------------------------------------- */

typedef  struct  
{
   /* A user's buffer for the Private Key Object -*/
   CRYS_ECPKI_UserPrivKey_t 	     ECDSA_SignerPrivKey; 
      
   /*HASH specific data and buffers */	   
   uint32_t CRYSPKAHashCtxBuff[CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS];
   CRYS_HASH_Result_t             	 HASH_Result;
   uint32_t                  	 	 HASH_Result_Size; /* length, in words */
   CRYS_ECPKI_HASH_OpMode_t    		 HashMode;
  
   /* #include specific fields that are used by the low level */
   uint32_t crysEcdsaSignIntBuff[CRYS_PKA_ECDSA_SIGNE_BUFF_MAX_LENGTH_IN_WORDS];
   
    /* This Buffer is added for encrypting the context ( encrypted part's size must be 0 modulo 16).
    * Note: This block must be at the end of the context.
    */
   uint8_t DummyBufAESBlockSize[CRYS_DEFS_DUMMY_AES_BLOCK_SIZE];
   
}ECDSA_SignContext_t;  


/* --------------------------------------------------------------------- */
/*                CRYS ECDSA  Signing User context database              */
/* --------------------------------------------------------------------- */


typedef struct  CRYS_ECDSA_SignUserContext_t 
{                                            
   uint32_t 	valid_tag; 
   /* For the use of the AES Encryption - Decryption of the context in CCM */              
   uint32_t 	AES_iv;        
   uint32_t  	context_buff [(sizeof(ECDSA_SignContext_t)+3)/4];  
} CRYS_ECDSA_SignUserContext_t;



/****************************************************************************/

/* --------------------------------------------------------------------- */
/*                CRYS ECDSA Verifying context structure                 */
/* --------------------------------------------------------------------- */
typedef  struct  
{
   /* A user's buffer for the Private Key Object -*/
   CRYS_ECPKI_UserPublKey_t         ECDSA_SignerPublKey;     

    /*HASH specific data and buffers */	   
   uint32_t CRYSPKAHashCtxBuff[CRYS_PKA_RSA_HASH_CTX_SIZE_IN_WORDS];
   CRYS_HASH_Result_t               HASH_Result;
   uint16_t                  	    HASH_Result_Size; /* length, in words */
   CRYS_ECPKI_HASH_OpMode_t    	    HashMode;
  
   /* #include specific fields that are used by the low level */
   uint32_t crysEcdsaVerIntBuff[CRYS_PKA_ECDSA_VERIFY_BUFF_MAX_LENGTH_IN_WORDS]; 
   uint32_t     MemControl;  
   
   /* This Buffer is added for encrypting the context ( encrypted part's size must be 0 modulo 16).
    * Note: This block must be at the end of the context.
    */
   uint8_t DummyBufAESBlockSize[CRYS_DEFS_DUMMY_AES_BLOCK_SIZE];
   
}ECDSA_VerifyContext_t;  


/* --------------------------------------------------------------------- */
/*                CRYS ECDSA Verifying User context database             */
/* --------------------------------------------------------------------- */

typedef struct  CRYS_ECDSA_VerifyUserContext_t 
{                                            
   uint32_t 		valid_tag; 
   /* For the use of the AES Encryption - Decryption of the context in CCM */              
   uint32_t 		AES_iv;        
   uint32_t  		context_buff[(sizeof(ECDSA_VerifyContext_t)+3)/4];  
}CRYS_ECDSA_VerifyUserContext_t;




/*****************************************************************************/ 

  
  
#ifdef __cplusplus
}
#endif

#endif
