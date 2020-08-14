#include "bitstream.h"

#ifndef VFMW_BIG_ENDIAN

#define  ENDIAN32( x )                      \
        ( (x)<<24 ) |                       \
        ( ( (x) & 0x0000ff00 ) << 8 ) |     \
        ( ( (x) & 0x00ff0000 ) >> 8 ) |     \
        ( ( (x) >> 24 ) & 0x000000ff )

#else

#define  ENDIAN32( x )      (x)

#endif

static UINT8 CalcZeroNum[256] =
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

#if 1
UINT32 ZerosMS_32(UINT32 data)
{
    UINT32 i, ZeroNum = 0, tmpZeroNum = 0;
    UINT32 tmpdata = 0;

    for (i=4; i>0; i--)
    {
        tmpdata = (data & 0xff000000) >> 24;
        tmpZeroNum = (UINT32)CalcZeroNum[tmpdata];
        ZeroNum += tmpZeroNum;
        if (8 != tmpZeroNum)
        {
            break;
        }
        data = data<<8;
    }

    return ZeroNum;
}
#endif

/******************************************************/
VOID BsInit( BS *pBS, UINT8 *pInput, HI_SIZE_T length )
{
    UINT32 Data;

    pBS->pHead    = (UINT8 *)(((HI_SIZE_T)pInput)&(~0x3));
    pBS->pTail    = pBS->pHead + 8;
    pBS->BsLen    = length;

    Data = *(UINT32*)pBS->pHead;
    pBS->Bufa     = ENDIAN32( Data );
    Data = *(UINT32*)(pBS->pHead+4);
    pBS->Bufb     = ENDIAN32( Data );

    pBS->BufPos   = (((HI_SIZE_T)pInput)&0x3)<<3;
    pBS->TotalPos = 0;

    return;
}

/******************************************************/
SINT32 BsPos( BS *pBS )
{
    return( pBS->TotalPos );
}

/******************************************************/
__inline SINT32  BsGet( BS *pBS, SINT32 nBits )
{
    UINT32 Data;

    Data = BsShow( pBS, nBits );
    BsSkip( pBS, nBits );

    return(Data);
}

/******************************************************/
__inline SINT32  BsShow( BS *pBS, SINT32 nBits )
{
    SINT32 ABBufPos = nBits + pBS->BufPos;
    UINT32 Data, Data1;

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
__inline SINT32  BsSkip( BS *pBS, SINT32 nBits )
{
    SINT32 ABBufPos = nBits + pBS->BufPos;
    UINT32 Data1;

    pBS->TotalPos += nBits;

    if( ABBufPos >= 32 )
    {
        pBS->BufPos   =  ABBufPos - 32;

        pBS->Bufa = pBS->Bufb;
        Data1 = *(UINT32*)pBS->pTail;
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
SINT32  BsBack( BS *pBS, SINT32 nBits )
{
    SINT32 PredBufPos;
    UINT32 Data1;

    if( nBits > pBS->TotalPos )
        nBits = pBS->TotalPos;

    PredBufPos     = (SINT32)pBS->BufPos - nBits;
    pBS->TotalPos -= nBits;

    if( PredBufPos >= 0 )
    {
        pBS->BufPos = PredBufPos;
    }
    else
    {
        pBS->pTail  -= 4;
        pBS->Bufb   = pBS->Bufa;
        Data1 = *(UINT32*)(pBS->pTail - 8);
        pBS->Bufa = ENDIAN32( Data1 );

        pBS->BufPos = PredBufPos + 32;

    }
    return( nBits );
}


SINT32  BsBitsToNextByte( BS *pBS )
{
    SINT32  SkipLen, AlignPos;
    AlignPos = (pBS->TotalPos + 7) & 0xfffffff8;
    SkipLen  = AlignPos - pBS->TotalPos;
    return SkipLen;
}


/******************************************************/
SINT32  BsToNextByte( BS *pBS )
{
    SINT32  SkipLen, AlignPos;
    AlignPos = (pBS->TotalPos + 7) & 0xfffffff8;
    SkipLen  = AlignPos - pBS->TotalPos;

    BsSkip( pBS, SkipLen );
    return SkipLen;
}

/******************************************************/
HI_SIZE_T BsResidBits( BS *pBS )
{
    return( 8*pBS->BsLen - pBS->TotalPos );
}

/******************************************************/
SINT32  BsIsByteAligned( BS *pBS )
{
    if( pBS->TotalPos & 7 )
        return(0);
    else
        return(1);
}

/******************************************************/
SINT32  BsNextBitsByteAligned( BS *pBS, SINT32 nBits )
{   // 'nBits' should <= 24, otherwise may be wrong.
    UINT32 BitsToByte, Data;

    BitsToByte = 8 - (pBS->TotalPos & 7);

    BsSkip( pBS, BitsToByte );
    Data = BsShow( pBS, nBits );
    BsBack( pBS, BitsToByte );

    return( Data );
}

/******************************************************/
//该跳跃只保证计数器的准确性,不能保证bufa和bufb中读得有效数据(当发生在包边界时)
SINT32 BsLongSkip(BS *pBS, SINT32 nBits)
{
    UINT32 bits,words;
    if(nBits==0)
    {
        return 0;
    }
    if(nBits<=64)
    {
        if(nBits>32)
        {
            BsSkip(pBS, 32);
            nBits-=32;
        }
        BsSkip(pBS, nBits);
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
         pBS->Bufa = ENDIAN32(*(UINT32 *)pBS->pTail);
         pBS->pTail+=4;
         pBS->Bufb = ENDIAN32(*(UINT32 *)pBS->pTail);
         pBS->pTail+=4;
         pBS->TotalPos += (words*32);

         if (bits>0)
         {
             BsSkip(pBS,bits);
         }
    }
    return nBits;
}

//类似于BsSkip,只是不在totalpos中计数,便于非32位对齐的码流包进行位置调整
//而调整的bit不记入消耗.因为配置bsinit是按32位对齐来实现的,可能读入了已消耗的bit
SINT32  BsSkipWithoutCount( BS *pBS, SINT32 nBits )
{
    SINT32 ABBufPos = nBits + pBS->BufPos;
    UINT32 Data1;

    if( ABBufPos >= 32 )
    {
        pBS->BufPos   =  ABBufPos - 32;

        pBS->Bufa = pBS->Bufb;
        Data1 = *(UINT32*)pBS->pTail;
        pBS->Bufb = ENDIAN32( Data1 );

        pBS->pTail += 4;
    }
    else
    {
        pBS->BufPos   += nBits;
    }

    return( nBits );
}

UINT8* BsGetNextBytePtr( BS *pBS )
{
    UINT8* ptr;
    SINT32 BitsInBufa;

    BitsInBufa = (32-pBS->BufPos);
    ptr = ((UINT8*)(pBS->pTail)) - (4+BitsInBufa/8);

    return( ptr );
}

