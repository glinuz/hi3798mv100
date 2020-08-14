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

#ifndef  _DX_UTIL_ASSET_DEFS_H
#define  _DX_UTIL_ASSET_DEFS_H


/* asset package offsets and sizes definitions */
/* field desc(byte size):    token(4)     version(4)  user data(4)    encrypted asset size(4)    nonce(8)     encrypted data(max 512)      mac(8) */
/* bytes offset         :    0..3          4..7        8..11           12..15                     16..23         24....max 536         ..max 544*/
#define ASSET_PKG_TOKEN_OFFSET 0
#define ASSET_PKG_TOKEN_SIZE 4
#define ASSET_PKG_VERSION_OFFSET (ASSET_PKG_TOKEN_OFFSET+ASSET_PKG_TOKEN_SIZE)
#define ASSET_PKG_VERSION_SIZE 4
#define ASSET_PKG_USER_DATA_OFFSET (ASSET_PKG_VERSION_OFFSET+ASSET_PKG_VERSION_SIZE)
#define ASSET_PKG_USER_DATA_SIZE 4
#define ASSET_PKG_EN_DATA_SIZE_OFFSET (ASSET_PKG_USER_DATA_OFFSET+ASSET_PKG_USER_DATA_SIZE)
#define ASSET_PKG_EN_DATA_SIZE_SIZE 4
#define ASSET_PKG_CCM_ADDITIONAL_DATA_OFFSET 0
#define ASSET_PKG_CCM_ADDITIONAL_DATA_SIZE  (ASSET_PKG_TOKEN_SIZE+ASSET_PKG_VERSION_SIZE+ASSET_PKG_USER_DATA_SIZE+ASSET_PKG_EN_DATA_SIZE_SIZE)

#define ASSET_PKG_CCM_NONCE_OFFSET (ASSET_PKG_EN_DATA_SIZE_OFFSET+ASSET_PKG_EN_DATA_SIZE_SIZE)
#define ASSET_PKG_CCM_NONCE_SIZE 8
#define ASSET_PKG_EN_DATA_OFFSET (ASSET_PKG_CCM_NONCE_OFFSET+ASSET_PKG_CCM_NONCE_SIZE)
#define ASSET_PKG_MAC_SIZE   	            8  
#define ASSET_PKG_NONE_ASSET_DATA_SIZE    (ASSET_PKG_CCM_ADDITIONAL_DATA_SIZE+ASSET_PKG_CCM_NONCE_SIZE+ASSET_PKG_MAC_SIZE)

#define ASSET_PKG_AES_CMAC_RESULT_SIZE_IN_BYTES			16  //DX_UTIL_AES_CMAC_RESULT_SIZE_IN_BYTES
#define DX_UTIL_KOEM_SIZE_IN_BYTES				ASSET_PKG_AES_CMAC_RESULT_SIZE_IN_BYTES	    	      
#define DX_UTIL_OEM_ASSET_DATA_MAX_SIZE_IN_BYTES	    512  
#define DX_UTIL_OEM_ASSET_PACKAGE_MAX_SIZE_IN_BYTES	    (DX_UTIL_OEM_ASSET_DATA_MAX_SIZE_IN_BYTES+ASSET_PKG_NONE_ASSET_DATA_SIZE)  

#define OEM_ASSET_DATA_IN_CMAC_LENGTH (4+sizeof(int)) 

#define OEM_ASSET_MAX_HBK_BUFF_SIZE 32

#define OEM_ASSET_PROV_KEY_FILE_KPRTL_OFFSET 0
#define OEM_ASSET_KRTL_BUFF_SIZE 16   
#define OEM_ASSET_PROV_KEY_FILE_SCP_OFFSET 16
#define OEM_ASSET_SCP_BUFF_SIZE 16  // originally from OTP 8 bytes, but we need to have additional 8 0'es for Kprov derivation
#define OEM_ASSET_CM_SECRETS_BUFF_SIZE (OEM_ASSET_SCP_BUFF_SIZE+OEM_ASSET_KRTL_BUFF_SIZE)   // Scp+Krtl sizes taken from prov_key file

/* definitions for input buffer for AES_CMAC Koem derivation*/
#define KOEM_DATA_IN_PREFIX_DATA0  0x01   
#define KOEM_DATA_IN_PREFIX_DATA1  0x50 
#define KOEM_DATA_IN_PREFIX_DATA2  0x00 
#define KOEM_DATA_IN_SUFIX_DATA    0x80

#define OEM_ASSET_MUL_16_BYTES_MASK (0x10-0x1)

#define  OEM_ASSET_PACK_TOKEN   0x20052001
#define  OEM_ASSET_VERSION      0x00000001


#define CONVERT_WORD_TO_BYTE_ARR(inWord, outPtr) {\
	*outPtr     = (inWord>>24) & 0xFF;\
	*(outPtr+1) = (inWord>>16) & 0xFF;\
	*(outPtr+2) = (inWord>>8) & 0xFF;\
	*(outPtr+3) = (inWord) & 0xFF;\
}

#define CONVERT_BYTE_ARR_TO_WORD(inPtr, outWord) {\
	outWord = (*inPtr<<24) | (*(inPtr+1)<<16) | (*(inPtr+2)<<8) | (*(inPtr+3)); \
}




#endif /*_DX_UTIL_ASSET_DEFS_H*/
