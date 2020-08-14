#ifndef __OMX_VENC_BUF_MNG_H__
#define __OMX_VENC_BUF_MNG_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static const HI_U8 g_msZerosTable[] =
{
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define MsZeros(zeros, c) \
    { \
        HI_U32 __code__ = (c); \
        zeros = g_msZerosTable[__code__ >> 24]; \
        if (8 == zeros) \
        { zeros += g_msZerosTable[0xFF & (__code__ >> 16)]; \
            if (16 == zeros) \
            { zeros += g_msZerosTable[0xFF & (__code__ >> 8)]; \
                if (24 == zeros) \
                { zeros += g_msZerosTable[0xFF & __code__ ]; } } } \
    }

#define ue_vlc(bits, code, val) \
    {                            \
        HI_U32 zeros;             \
        code = (val) + 1;          \
        MsZeros(zeros, code);       \
        bits = 63 - (zeros << 1);    \
    }

#define se_vlc(bits, code, val) \
    {                            \
        HI_U32 zeros;             \
        code = (((val) < 0 ? -(val) : (val)) << 1) | ((val) <= 0); \
        MsZeros(zeros, code);       \
        bits = 63 - (zeros << 1);    \
    }

#define REV32(x) ((((HI_U32)(x)) >> 24) + ((((HI_U32)(x)) >> 8) & (HI_U32)0xFF00) + \
                  (((HI_U32)(x)) << 24) + ((((HI_U32)(x)) << 8) & (HI_U32)0xFF0000))

/************************************************
Processor-related definitions

 Notes:
 1)All length units are addressing step of processor, and ARM is the "bytes";
 2)All W/R operations carried out in accordance with word, so need:
   buffer length of the packet should be in word-alignment;
   elements of the queue should be in word-alignment;
 *************************************************/

#define WORD_ALIGN 0x04       /*Bytes included in one word*/

typedef HI_UCHAR ADDR_UNIT;  /*Addressing step of processor, unit of the address*/
typedef HI_U32   CPU_WORD;   /*Word lenght of processor, unit of W/R operations*/
typedef struct
{
    HI_U32  tU32a; // only used in ShowBits32 and FlushBits31/32
    HI_U32  tU32b;
    HI_U32* pBuff;
    HI_U32  tBits;
    HI_U32  totalBits;
    HI_U32  bBigEndian;
} tBitStream;

HI_VOID OMX_VENC_BsOpenBitStream(tBitStream* pBS, HI_U32* pBuffer);
HI_VOID OMX_VENC_BsPutBits31(tBitStream* pBS, HI_U32 code, HI_U32 bits);
HI_VOID OMX_VENC_BsPutBits32(tBitStream* pBS, HI_U32 code, HI_U32 bits);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
