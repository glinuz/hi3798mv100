#ifndef __VMX_3RD_ADP_H__
#define __VMX_3RD_ADP_H__

#include "vmx_3rd_auth.h"

#define VMX_CheckPointer(p) \
    do {  \
        if (HI_NULL == p)\
        {\
            VMX_PRINT("pointer parameter is NULL.\n"); \
            return -1; \
        } \
    } while (0)

#define VMX_ASSERT(api, ret) \
    do{ \
        ret = api; \
        if (ret != 0) \
        { \
            VMX_PRINT("run %s failed, ERRNO:%#x.\n", #api, ret); \
            return -1;\
        } \
        else\
        {\
            /*printf("sample %s: run %s ok.\n", __FUNCTION__, #api);}*/   \
        }\
        ret = ret;\
    }while(0)

#define CIPHER_1M_16 (1*1024*1024 - 16)


int VMX3RD_ADP_R2RByMiscKeyladder(unsigned char bR2REncDec, unsigned char* input, int length, unsigned char* output);
int VMX3RD_ADP_VerifyRSASignature(unsigned char* signature, int sig_len, unsigned char* data, int data_len);
int VMX3RD_ADP_DecryptBySWPK(unsigned char* input, int length, unsigned char* output);

#endif /* __VMX_3RD_ADP_H__ */

