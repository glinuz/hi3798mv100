#ifndef __BITSTREAM_HEAD__
#define	__BITSTREAM_HEAD__

#ifdef __cplusplus
extern "C" {
#endif

//#include "basedef.h"
#include "vfmw.h"


#define  REVERSE_ENDIAN32( x )						\
    ( (x)<<24 ) |						\
    ( ( (x) & 0x0000ff00 ) << 8 ) |		\
    ( ( (x) & 0x00ff0000 ) >> 8 ) |		\
    ( ( (x) >> 24 ) & 0x000000ff )

typedef struct _tag_BITSTREAM
{
    UINT8  *pHead;
    UINT8  *pTail;
    UINT32  Bufa;
    UINT32  Bufb;
    SINT32  BsLen;
    SINT32  BufPos;
    UINT32  TotalPos;
    SINT32  HistoryPos;
} BS, *LPBS;


VOID   BsInit( BS *pBS, UINT8 *pInput, SINT32 length );
SINT32  BsGet( BS *pBS, SINT32 nBits );
SINT32  BsShow( BS *pBS, SINT32 nBits );
SINT32  BsPos( BS *pBS );
SINT32  BsSkip( BS *pBS, SINT32 nBits );
SINT32  BsBack( BS *pBS, SINT32 nBits );
SINT32  BsToNextByte( BS *pBS );
SINT32  BsBitsToNextByte( BS *pBS );
SINT32  BsResidBits( BS *pBS );
SINT32  BsIsByteAligned( BS *pBS );
SINT32  BsNextBitsByteAligned( BS *pBS, SINT32 nBits );
SINT32  BsLongSkip(BS *pBS, SINT32 nBits);
SINT32  BsSkipWithoutCount( BS *pBS, SINT32 nBits );
UINT8  *BsGetNextBytePtr( BS *pBS );
UINT32 ZerosMS_32(UINT32 data);


#ifdef __cplusplus
}
#endif

#endif

