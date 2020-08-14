#ifndef __HI_PVR_CIPHER_CFG_H__
#define __HI_PVR_CIPHER_CFG_H__

#include "hi_unf_cipher.h"

//#define PVR_FIFO_DIO

/* must be 2^n */
#define PVR_CIPHER_PKG_LEN              16        /* cipher group length */
#define PVR_CIPHER_MAX_LEN              (1024*1024 - 16)        /* cipher group length */

#define PVR_CIPHER_AES_BIT_WIDTH        HI_UNF_CIPHER_BIT_WIDTH_128BIT
#define PVR_CIPHER_DES_BIT_WIDTH        HI_UNF_CIPHER_BIT_WIDTH_64BIT
#define PVR_CIPHER_3DES_BIT_WIDTH       HI_UNF_CIPHER_BIT_WIDTH_64BIT

#define PVR_CIPHER_AES_KEY_LENGTH       HI_UNF_CIPHER_KEY_AES_128BIT
#define PVR_CIPHER_DES_KEY_LENGTH       HI_UNF_CIPHER_KEY_DES_2KEY
#define PVR_CIPHER_3DES_KEY_LENGTH      HI_UNF_CIPHER_KEY_DES_3KEY

#define PVR_CIPHER_AES_WORK_MODD        HI_UNF_CIPHER_WORK_MODE_ECB
#define PVR_CIPHER_DES_WORK_MODD        HI_UNF_CIPHER_WORK_MODE_ECB
#define PVR_CIPHER_3DES_WORK_MODD       HI_UNF_CIPHER_WORK_MODE_ECB

#define PVR_CIPHER_AES_KEY_LENGTH_BIT   128


#endif  // __HI_PVR_CIPHER_CFG_H__

