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

 
 
#ifndef CRYS_RSA_LOCAL_H
#define CRYS_RSA_LOCAL_H

/*
 * All the includes that are needed for code using this module to
 * compile correctly should be #included here.
 */
#include "crys_error.h" 
#include "crys_defs.h"
#include "crys_rsa_types.h"



#ifdef __cplusplus
extern "C"
{
#endif

  /*
   *  Object %name    : %
   *  State           :  %state%
   *  Creation date   :  Wed Dec 22 17:36:19 2004
   *  Last modified   :  %modify_time%
   */
  /** @file
   *  \brief this file contains the prototype of the service functions for
   *         the CRYS RSA module that are for internally usage.  
   *
   *  \version crys_rsa_local.h#1:incl:1
   *  \author adams
   */






/************************ Defines ******************************/

/* the RSA public key user validity TAG */
#define CRYS_RSA_PUB_KEY_VALIDATION_TAG 0x13579BDF

/* the RSA private key user validity TAG */
#define CRYS_RSA_PRIV_KEY_VALIDATION_TAG 0x2468ACE0

/* the RSA sign Context user validity TAG */
#define CRYS_RSA_SIGN_CONTEXT_VALIDATION_TAG   0x98765432
#define CRYS_RSA_VERIFY_CONTEXT_VALIDATION_TAG 0x45678901

/* For testing the CRYS_RSA_SEP_InvModWord function the 
CRYS_RSA_SEP_InvModWord_MeasuprePerform definition must be undefined.
For measuring performance of CRYS_RSA_SEP_InvModWord function 
with SEP set the CRYS_RSA_SEP_InvModWord_MeasuprePerform definition */ 

/*#define CRYS_RSA_SEP_InvModWord_MeasuprePerform*/


/***************

  ASN1 types - for BER Parser - used for PKCS#1 Ver 1.5

***************/

#define ASN1_BOOLEAN		1

#define ASN1_INTEGER		2

#define ASN1_BIT_STRING 	3

#define ASN1_OCTET_STRING	4

#define ASN1_NULL		5

#define ASN1_OBJECT_IDENTIFIER	6  

#define ASN1_SEQUENCE		16      /* 0x10 */ 

#define ASN1_SET		17          /* 0x11 */

#define ASN1_PRINTABLE_STRING	19  /* 0x13 */

#define ASN1_TELETEX_STRING	20      /* 0x14 */

#define ASN1_IA5STRING		22      /* 0x16 */

#define ASN1_UTC_TIME	        23  /* 0x17 */

/**********************
 structures definition
 **********************/
#define TEST_MSB_BIT 0x80


typedef struct BerParserObj_struct
{
 DxUint8_t  DigestAlg;
 DxUint8_t *MessageDigest_ptr;
 DxUint16_t MessageDigestSize;
}BerParserObj_t;

/************************ macros ********************************/

/* this macro is required to remove compilers warnings if the HASH or PKI is not supported */

#if defined(CRYS_NO_HASH_SUPPORT) || defined(CRYS_NO_PKI_SUPPORT)
#define RETURN_IF_RSA_UNSUPPORTED( a , b , c , d , e , f , g , h , i , j , k , l , a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1 , i1 , j1) \
  (a)=0;(b)=0;(c)=0;(d)=0;(e)=0;(f)=0;(g)=0;(h)=0;(i)=0;(j)=0;(k)=0;(l)=0; \
  (a1)=0;(b1)=0;(c1)=0;(d1)=0;(e1)=0;(f1)=0;(g1)=0;(h1)=0;(i1)=0;(j1)=0; \
  (a)=(a);(b)=(b);(c)=(c);(d)=(d);(e)=(e);(f)=(f);(g)=(g);(h)=(h);(i)=(i);(j)=(j);(k)=(k);(l)=(l); \
  (a1)=(a1);(b1)=(b1);(c1)=(c1);(d1)=(d1);(e1)=(e1);(f1)=(f1);(g1)=(g1);(h1)=(h1);(i1)=(i1);(j1)=(j1); \
  return CRYS_RSA_IS_NOT_SUPPORTED
#else  /* !CRYS_NO_HASH_SUPPORT && ! CRYS_NO_PKI_SUPPORT */
#define RETURN_IF_RSA_UNSUPPORTED( a , b , c , d , e , f , g , h , i , j , k , l , a1 , b1 , c1 , d1 , e1 , f1 , g1 , h1 , i1 , j1) 
#endif /* !CRYS_NO_HASH_SUPPORT && ! CRYS_NO_PKI_SUPPORT */
               
/************************ Typedefs  ****************************/


/************************ Structs  ******************************/
typedef struct CRYS_OAEP_Data_t{
 DxUint8_t  MaskDB[CRYS_RSA_OAEP_ENCODE_MAX_MASKDB_SIZE];
 DxUint8_t  SeedMask[CRYS_RSA_OAEP_ENCODE_MAX_SEEDMASK_SIZE];
 CRYS_HASHUserContext_t HashUsercontext;
 CRYS_HASH_Result_t         HashResultBuff;
}CRYS_OAEP_Data_t;

/************************ Public Variables **********************/


/************************ Public Functions **********************/

/*
 	Function Name: CRYS_RSA_PKCS1_v1_5_Encode
 	Date:   18-01-2005
 	Author:	Ohad Shperling
 
 
    \brief CRYS_RSA_PKCS1_v1_5_Encode implements EME-PKCS1_v1_5_Encoding algorithm 
   as defined in PKCS#1 v1.5 Sec 8.1
 
   @param[in] K - The modulus size denoted in octets.
   @param[in] M_ptr - Pointer to the Message to pad the ouput stream
   @param[in] MSize - Denotes the Message size
   @param[in] BlockType - 
					BlockType for PUB Key is 02
					BlockType for Priv Key is 01
					
   @param[out] Output_ptr - Pointer to a buffer which is at least K octets long
   
   @return CRYSError_t - CRYS_OK,
                         
*/
CRYSError_t CRYS_RSA_PKCS1_v1_5_Encode(DxUint16_t K,
					CRYS_PKCS1_Block_Type 	BlockType,
					DxUint8_t     *M_ptr,
					DxUint32_t     MSize,
					DxUint8_t     *Output_ptr);
                  
/***************************************************************************************/
/**
 	Function Name: CRYS_RSA_PKCS1_v1_5_Decode
 	Date:   18-01-2005
 	Author:	Ohad Shperling
 
 
    \brief CRYS_RSA_PKCS1_v1_5_Encode implements EME-PKCS1_v1_5_Encoding algorithm 
   as defined in PKCS#1 v1.5 Sec 8.1
 
   @param[in] EncryptedBlock - Pointer to the Encrypted block to parse - given after an exponent operation
   @param[in] K - The modulus size denoted in octets.   
   @param[out] D_ptr - Pointer to the output Data - to parse out of the EncryptedBlock buffer
   @param[out] DSize - Denotes the Data size
	Note BlockType - 
					BlockType for PUB Key is 02
					BlockType for Priv Key is 01
					
   @return CRYSError_t - CRYS_OK,
                         
*/
CRYSError_t CRYS_RSA_PKCS1_v1_5_Decode(DxUint8_t *EncryptedBlock,
					DxUint16_t 	K,
					DxUint8_t     *D_ptr,
					DxUint16_t    *DSize_ptr);
                                            
/* -------------------------------------------------------------
 *  Function Name: BerParser
 *  Date: 24/09/2001
 *  Author: Evgeny Belenky
 *
 *      Inpruts:
 *      Outputs:
 *
 *  change tested certificate by:
 *
 *  Local Data Structures:
 *
 *  Description and Design info:
 *      purpose:This function returns the pointer to the signature and
 *               the signature length.
 *
 *      Assumptions,
 *      Constrains,
 *      Limitations,
 *      Side effects
 *
 *  Algorithm:
 *
 *  List Of Units Used:
 *
 *  Trace info to SRS, IDD and Detailed SDD:
 *
 *  UCert_ptrdate History:
 *  Date:       Description:
 *  NOTE ! (by ronen ) Works on MSB in first byte order
 *  --------------------------------------
 *
 * ----------------------------------------------------------- */

CRYSError_t      CRYS_RSA_BER_BerParser(
				DxUint8_t  		*Data_ptr, 
				DxUint16_t 		DataSize,
				BerParserObj_t 	*BerParserObj_ptr);
                           
/* -------------------------------------------------------------
 *	Function Name: BerEncoder
 *	Date:   4-1-2002
 *	Author:	Ronen Greenspan
 *
 *	Inputs:
 *
 *	Outputs:
 *
 *	Local Data Structures:
 *
 *	Description and Design info:
 *		Purpose,
 *		Assumptions,        NOTE ! - function handel buffers with max 
 *                                   size of 0xFF 
 *		Constrains,
 *		Limitations,
 *		Side effects
 *
 *	Algorithm:
 *
 *
 *	List Of Units Used:
 *
 *	Trace info to SRS, IDD and Detailed SDD:
 *
 *	Update History:
 *	Date:		Description:
 *	--------------------------------------
 *
 * ----------------------------------------------------------- */

CRYSError_t CRYS_RSA_BER_BerEncoder(DxUint8_t	**Buffer_ptr,
				DxUint8_t    *BufferSize,
				DxUint16_t    BufferTotalSize,
				BerParserObj_t 	*BerParserObj_ptr);

                           

/* -------------------------------------------------------------
 *	Function Name: BerEncoder
 *	Date:   4-1-2002
 *	Author:	Ronen Greenspan
 *
 *	Inputs:
 *
 *	Outputs:
 *
 *	Local Data Structures:
 *
 *	Description and Design info:
 *		Purpose,
 *		Assumptions,        
 *		Constrains,
 *		Limitations,
 *		Side effects
 *
 *	Algorithm:  this function returns length of the following fields and move pointer to the next field
 *
 *
 *	List Of Units Used:
 *
 *	Trace info to SRS, IDD and Detailed SDD:
 *
 *	Update History:
 *	Date:		Description:
 *	--------------------------------------
 *
 * ----------------------------------------------------------- */

void CRT_API_GetLengthField(DxUint8_t** PtrToLengthField, DxUint16_t * LengthFieldValue);



/**********************************************************************************************************/

/* -------------------------------------------------------------
 *	Function Name: CRYS_RSA_OAEPMGF1
 *	Date:   09-1-2005
 *	Author:	Ohad Shperling
 *
 *	Inputs:
 *
 *	Outputs:
 *
 *	Local Data Structures:
 *
 *	Description and Design info:
 *		Purpose,
 *		Assumptions,
 *		Constrains,
 *		Limitations,
 *		Side effects
 *
 *	Algorithm:
 *
 *
 *	List Of Units Used:
 *
 *	Trace info to SRS, IDD and Detailed SDD:
 *
 *	Update History:
 *	Date:		Description:
 *	--------------------------------------
 *
 * ----------------------------------------------------------- */


CRYSError_t CRYS_RSA_OAEPMGF1( DxUint16_t hLen, /*size in Bytes*/
			    DxUint8_t * Z_ptr,
			    DxUint16_t ZSize,/*size in Bytes*/
			    DxUint32_t L,
			    DxUint8_t  *Mask_ptr,
			    CRYS_PKCS1_HashFunc_t hashFunc,
			    DxUint8_t  *T_Buf,	/*T_Buf is a buffer used for data manipulation for the function to use instead of allocating the space on stack*/
			    DxUint8_t  *T_TMP_Buf);/*T_TMP_Buf is a buffer used for data manipulation for the function to use instead of allocating the space on stack*/                                    


/**********************************************************************************************************/
/**
 * @brief 
 * This function does implements the functionality of PKCS1 Ver 2.1 Sign 
 *	operation after the Hash operation
 *
 * Before using that function a Hash must be completed on the Data.
 * The function is called after the call to Hash_Finish
 *
 * @param[in] Context_ptr - Pointer to a valid context as
 *                           given from CRYS_RSA_SIGN
 *                           
 * @param[out] Output_ptr - A buffer allocated for the output which is at least the size of the MOdulus N
 *
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                        value MODULE_* as defined in ...
 */
 
CRYSError_t CRYS_RSA_PSS_Sign21(RSAPrivContext_t *Context_ptr,
                                    DxUint8_t     *Output_ptr);



/**********************************************************************************************************/
/**
 	Function Name: CRYS_RSA_PSS_Verify21
 	Date:   06-12-2004
 	Author:	Ohad Shperling
 
 
    \brief CRYS_RSA_PSS_Verify21 implements EMSA-PSS-Verify algorithm 
   as defined in PKCS#1 v2.1 Sec 9.1.2
 
   @param[in] Context_ptr - Pointer to a valid context as
                            given from the VerifyFinish function.
                            
   The field HASH_Result inside the Context_ptr is initialized with the Hashed digested message.
   The field HASH_Result_Size inside the Context_ptr is initialized with the Hash digested message size

   @return CRYSError_t - CRYS_OK,
                         CRYS_BAD_PARAM, CRYS_OUT_OF_RANGE 
*/




CRYSError_t CRYS_RSA_PSS_Verify21(RSAPubContext_t *Context_ptr);

/**********************************************************************************************************/
/** 
 * @brief The CRYS_RSA_GenerateVectorInRangeX931 function generates a random vector in range:
 *            MinVect < RandVect < MaxVect, where:
 *            MinVect = sqwRoot(2) * 2^(RndSizeInBits-1),  MaxVect = 2^RndSizeInBits. 
 *
 *            Note: 1. MSBit of RandVect must be set to 1.
 *                  2. Words order of output vector is set from LS word to MS
 *      	       word. 
 *            
 *        This function is used in PKI RSA for random generation according to ANS X9.31 standard.
 *        If PKI_RSA is not supported, the function does nothing.
 *
 *        Functions algorithm:: 
 *
 *        1.  Calls the CRYS_RND_GenerateVector() function for generating random vector 
 *            RndVect of size RndSizeInWords, rounded up to bytes. Set index i
 *            to high word i = SizeInWords-1.
 *        2.  Check and adust candidate for msWord inside the random vector
 *            starting from msWord himselv, if msWord > high word of MinVect,
 *            goto step 3, else try next word i--; if no words to try, then goto
 *            step 1.
 *        3.  Set the found msWord to high position in array and generate new
 *            random words instead all checked and rejected words.
 *
 * @rndSizeWords[in]  - The size of random vectore that is required.
 * @rnd_ptr[out]      - The output buffer of size not less, than rndSizeWords.
 *
 * @return CRYSError_t - On success CRYS_OK is returned, on failure a
 *                       value MODULE_* as defined in ...
 */
CRYSError_t CRYS_RSA_GenerateVectorInRangeX931(DxUint32_t   rndSizeWords, 
                                               DxUint32_t  *rnd_ptr);

/**********************************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif

