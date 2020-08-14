#ifndef __SHA_H__
#define __SHA_H__

#include "hi_type.h"

#define SHA1HashSize 20

typedef struct hiSHA1Context
{
    HI_U32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest */
    HI_U32 u32Count[2];            /* Message length in bits */
    HI_U8 Message_Block_Index;     /* Index into message block array */
    HI_U8  Message_Block[64];      /* 512-bit message blocks  */
} SHA1Context;

void SHA1Reset();
void SHA1DoBody();
void SHA1DoTail();

#endif
