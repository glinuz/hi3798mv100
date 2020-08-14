/***********************************************************************
*
* Copyright (C), 2006 Hisilicon Tech. Co., Ltd.
*
* File: $bitstream.c$
* Date: $2009/4/8$
* Revision: $v1.1$
* Purpose: a general bit stream manipulation tool,
*          can be adopted by multi video standards.
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2009/4/8         z56361            Original
*
* Dependencies:
*
************************************************************************/

#include "drv_demux_bitstream.h"


#ifndef FIDX_BIG_ENDIAN

#define  ENDIAN32( x )                      \
        ( (x)<<24 ) |                       \
        ( ( (x) & 0x0000ff00 ) << 8 ) |     \
        ( ( (x) & 0x00ff0000 ) >> 8 ) |     \
        ( ( (x) >> 24 ) & 0x000000ff )

#else
#define  ENDIAN32( x )      (x)

#endif

static HI_U8 CalcZeroNum[256] =
{
    8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

HI_U32 PvrZerosMS_32(HI_U32 data)
{
    HI_U32 i, ZeroNum = 0, tmpZeroNum = 0;
    HI_U32 tmpdata = 0;

    for (i=4; i>0; i--)
    {
        tmpdata = (data & 0xff000000) >> 24;
        tmpZeroNum = (HI_U32)CalcZeroNum[tmpdata];
        ZeroNum += tmpZeroNum;
        if (8 != tmpZeroNum)
        {
            break;
        }
        data = data<<8;
    }

    return ZeroNum;
}


/******************************************************/
HI_VOID PvrBsInit( PVR_BS *pBS, HI_U8 *pInput, HI_S32 length )
{
    HI_U32 Data;
    unsigned long AlignWord = 0x3;

    pBS->pHead    = ( HI_U8 *)(((unsigned long)pInput) & (~AlignWord));
    pBS->pTail    = pBS->pHead + 8;
    pBS->BsLen    = length;

    Data = *(HI_U32*)pBS->pHead;
    pBS->Bufa     = ENDIAN32( Data );
    Data = *(HI_U32*)(pBS->pHead+4);
    pBS->Bufb     = ENDIAN32( Data );

    pBS->BufPos   = (((unsigned long)pInput) & 0x3)<<3;
    pBS->TotalPos = 0;

    return;
}

/******************************************************/
HI_S32  PvrBsPos( PVR_BS *pBS )
{
    return( pBS->TotalPos );
}

/******************************************************/
HI_S32  PvrBsGet( PVR_BS *pBS, HI_S32 nBits )
{
    HI_U32 Data;

    Data = PvrBsShow( pBS, nBits );
    PvrBsSkip( pBS, nBits );

    return(Data);
}

/******************************************************/
HI_S32  PvrBsShow( PVR_BS *pBS, HI_S32 nBits )
{
    HI_S32 ABBufPos = nBits + pBS->BufPos;
    HI_U32 Data, Data1;

    if( ABBufPos > 32 )
    {
        Data  = pBS->Bufa << pBS->BufPos;   //( ABBufPos - 32 );
        Data1 = pBS->Bufb >> (32-pBS->BufPos);
        Data |= Data1;
        Data >>= (32-nBits);
    }
    else
    {
        Data  = (pBS->Bufa << pBS->BufPos) >> (32 - nBits);
    }

    return(Data);
}

/******************************************************/
HI_S32  PvrBsSkip( PVR_BS *pBS, HI_S32 nBits )
{
    HI_S32 ABBufPos = nBits + pBS->BufPos;
    HI_U32 Data1;

    pBS->TotalPos += nBits;

    if( ABBufPos >= 32 )
    {
        pBS->BufPos   =  ABBufPos - 32;

        pBS->Bufa = pBS->Bufb;
        Data1 = *(HI_U32*)pBS->pTail;
        pBS->Bufb = ENDIAN32( Data1 );

        pBS->pTail += 4;
    }
    else
    {
        pBS->BufPos   += nBits;
    }

    return( nBits );
}

/******************************************************/
HI_S32  PvrBsBack( PVR_BS *pBS, HI_S32 nBits )
{
    HI_S32 PredBufPos;
    HI_U32 Data1;

    if( nBits > pBS->TotalPos )
        nBits = pBS->TotalPos;

    PredBufPos     = (HI_S32)pBS->BufPos - nBits;
    pBS->TotalPos -= nBits;

    if( PredBufPos >= 0 )
    {
        pBS->BufPos = PredBufPos;
    }
    else
    {
        pBS->pTail  -= 4;
        pBS->Bufb   = pBS->Bufa;
        Data1 = *(HI_U32*)(pBS->pTail - 8);
        pBS->Bufa = ENDIAN32( Data1 );

        pBS->BufPos = PredBufPos + 32;

    }
    return( nBits );
}


HI_S32  PvrBsBitsToNextByte( PVR_BS *pBS )
{
    int  SkipLen, AlignPos;
    AlignPos = (pBS->TotalPos + 7) & 0xfffffff8;
    SkipLen  = AlignPos - pBS->TotalPos;
    return SkipLen;
}


/******************************************************/
HI_S32  PvrBsToNextByte( PVR_BS *pBS )
{
    int  SkipLen, AlignPos;
    AlignPos = (pBS->TotalPos + 7) & 0xfffffff8;
    SkipLen  = AlignPos - pBS->TotalPos;

    PvrBsSkip( pBS, SkipLen );
    return SkipLen;
}

/******************************************************/
HI_S32  PvrBsResidBits( PVR_BS *pBS )
{
    return( 8*pBS->BsLen - pBS->TotalPos );
}

/******************************************************/
HI_S32  PvrBsIsByteAligned( PVR_BS *pBS )
{
    if( pBS->TotalPos & 7 )
        return(0);
    else
        return(1);
}

/******************************************************/
HI_S32  PvrBsNextBitsByteAligned( PVR_BS *pBS, HI_S32 nBits )
{   // 'nBits' should <= 24, otherwise may be wrong.
    HI_U32 BitsToByte, Data;

    BitsToByte = 8 - (pBS->TotalPos & 7);

    /* FIX codecc WARNNING. BitsToByte must be between 1 and 8, cannot be 0 */
#if 0
    if( BitsToByte == 0 )
        if( PvrBsShow( pBS, 8 ) == 0x7f )
            BitsToByte = 8;
#endif

    PvrBsSkip( pBS, BitsToByte );
    Data = PvrBsShow( pBS, nBits );
    PvrBsBack( pBS, BitsToByte );

    return( Data );
}

/******************************************************/
//! This skip simply process the bit counter, do NOT guarantee the data in the bufa or bufb
HI_S32  PvrBsLongSkip(PVR_BS *pBS, HI_S32 nBits)
{
    HI_U32 bits,words;
    if(nBits==0)
    {
        return 0;
    }
    if(nBits<=64)
    {
        if(nBits>32)
        {
            PvrBsSkip(pBS, 32);
            nBits-=32;
        }
        PvrBsSkip(pBS, nBits);
    }
    else
    {
         bits = (32-pBS->BufPos);
         pBS->TotalPos+=bits;
         pBS->BufPos = 0;
         nBits -= bits;
         words = nBits/32;
         bits = nBits&0x1f;
         pBS->pTail += 4*(words-1);
         pBS->Bufa = ENDIAN32(*(HI_U32 *)pBS->pTail);
         pBS->pTail+=4;
         pBS->Bufb = ENDIAN32(*(HI_U32 *)pBS->pTail);
         pBS->pTail+=4;
         pBS->TotalPos += (words*32);

         if (bits>0)
         {
             PvrBsSkip(pBS,bits);
         }
    }
    return nBits;
}

HI_S32  PvrBsSkipWithoutCount( PVR_BS *pBS, HI_S32 nBits )
{
    HI_S32 ABBufPos = nBits + pBS->BufPos;
    HI_U32 Data1;

    if( ABBufPos >= 32 )
    {
        pBS->BufPos   =  ABBufPos - 32;

        pBS->Bufa = pBS->Bufb;
        Data1 = *(HI_U32*)pBS->pTail;
        pBS->Bufb = ENDIAN32( Data1 );

        pBS->pTail += 4;
    }
    else
    {
        pBS->BufPos   += nBits;
    }

    return( nBits );
}

HI_U8* PvrBsGetNextBytePtr( PVR_BS *pBS )
{
    HI_U8* ptr;
    HI_S32 BitsInBufa;

    BitsInBufa = (32-pBS->BufPos);
    ptr = ((HI_U8*)(pBS->pTail)) - (4+BitsInBufa/8);

    return( ptr );
}

