#ifndef __CRYS_CRYPTO_COMMON_H_
#define __CRYS_CRYPTO_COMMON_H_
#include "hi_type.h"
#include "dx_pal_types.h"
#include "crys_error.h"
#include "crys_rsa_types.h"
#include <sre_mem.h>

#define malloc(x) SRE_MemAlloc(OS_MID_SYS, OS_MEM_DEFAULT_FSC_PT, (x))
#define free(x)   SRE_MemFree(OS_MID_SYS, (x))
#define RSA_MAX_RSA_KEY_LEN  512

HI_U32 CRYS_GetBitNum(HI_U8 *pu8BigNum, HI_U32 u32NumLen);
HI_U32 CRYS_Bin2Bn(DxUint32_t *pu32bn, const HI_U8 *pbin, HI_S32 u32Len);
HI_U32 CRYS_Bn2Bin(const HI_U32 *pu32bn, HI_U8 *pbin, HI_S32 u32Len);
HI_S32 CRYS_GetRandomNumber(HI_U8 *pu8Rand, HI_U32 u32Size);
CEXPORT_C CRYSError_t CRYS_RSA_Private(HI_U8 *UserContext_ptr, CRYSRSAPrivKey_t *PrivKey_ptr,				  				  			      
                   HI_U8 *DataIn_ptr, HI_U8 *Output_ptr, HI_U32 DataSize);
CEXPORT_C CRYSError_t CRYS_RSA_Public(HI_U8 *UserContext_ptr, CRYSRSAPubKey_t *PubKey_ptr,				  				  			      
                   HI_U8 *DataIn_ptr, HI_U8 *Output_ptr, HI_U32 DataSize);
#endif

