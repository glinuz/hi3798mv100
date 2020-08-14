#include <stdio.h>
#include <stdlib.h>
#include <cutils/log.h>

#include "authdefine.h"
#include "alg.h"
#include "hi_type.h"

#define  MB (1024*1024)

int main(int argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_S32 flag = 0; 
    HI_U8 *p = NULL;
    HI_U32 index = 0;
    HI_HANDLE *handle = NULL;
    HI_U8 hash[32];
    HI_U8 hash1[32];
    HI_U8 hash256[32];
    
    p = (HI_U8 *)malloc(10 * MB);
    if (NULL == p)
    {
        SAMPLE_PRINTF("malloc failed [%s:%d]\n", __FILE__, __LINE__);
        return 0;
    }
    memset(p + 1 * MB, 0, 1 * MB);
    // Sha1 
    CA_ASSERT(CA_common_InitHASH(0, &handle), ret);
    for (index = 0; index < 10; index++)      
    {
        CA_common_UpdateHASH(handle, (HI_U8 *)p, MB);
    }         
    CA_common_FinishHASH(handle, (HI_U8 *)hash1); 

    sha1(p, 10 * MB, hash);

    SAMPLE_PRINTF("Sha1:    ");
    for (index = 0; index < 20; index++)
    {
        SAMPLE_PRINTF("0x%02x ", hash[index]);
    }

    SAMPLE_PRINTF("\nSW sha1: ");
    for (index = 0; index < 20; index++)
    {
        SAMPLE_PRINTF("0x%02x ", hash1[index]);
    }
    SAMPLE_PRINTF("\n");
    
    for (index = 0; index < 20; index++)
    {
        if (hash[index] != hash1[index])
        {
            flag = -1;
            break;
        }
    }

    if (-1 == flag)
    {
        SAMPLE_PRINTF("\nsha1 ERR!!\n");
    }
    else
    {
        SAMPLE_PRINTF("\nsha1 OK!!\n");
    }

    // Sha256
    flag = 0;
    CA_ASSERT(CA_common_InitHASH(1, &handle), ret);
    for (index = 0; index < 10; index++)      
    {
        CA_common_UpdateHASH(handle, (HI_U8 *)p + index  * MB, MB);
    }         
    CA_common_FinishHASH(handle, (HI_U8 *)hash256); 

    sha2(p, 10 * MB, hash, 0);

    SAMPLE_PRINTF("Sha256:      ");
    for (index = 0; index < 32; index++)
    {
        SAMPLE_PRINTF("0x%02x ", hash[index]);
    }

    SAMPLE_PRINTF("\nSW sha256: ");
    for (index = 0; index < 32; index++)
    {
        SAMPLE_PRINTF("0x%02x ", hash256[index]);
    }
    SAMPLE_PRINTF("\n");    
    for (index = 0; index < 32; index++)
    {
        if (hash[index] != hash256[index])
        {
            flag = -1;
            break;
        }
    }
    if (-1 == flag)
    {
        SAMPLE_PRINTF("\nsha256 ERR!!\n");
    }
    else
    {
        SAMPLE_PRINTF("\nsha256 OK!!\n");
    }
   
    return 0;
}

