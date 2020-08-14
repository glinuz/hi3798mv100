
/***********************************************************************
*
* Copyright (C), 2006 Hisilicon Tech. Co., Ltd.
*
* File: $bitstream.h$
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

#ifndef _PVR_BITSTREAM_HEAD_
#define	_PVR_BITSTREAM_HEAD_

#ifdef __cplusplus
extern "C" {
#endif

#include "hi_type.h"

typedef struct _PVR_BITSTREAM{
	HI_U8	*pHead;
	HI_U8	*pTail;
	HI_U32  Bufa;
	HI_U32  Bufb;
	HI_S32	BsLen;
	HI_S32  BufPos;
	HI_S32  TotalPos;
}PVR_BS, *LP_PVR_BS;


HI_VOID PvrBsInit( PVR_BS *pBS, HI_U8 *pInput, HI_S32 length );
HI_S32  PvrBsGet( PVR_BS *pBS, HI_S32 nBits );
HI_S32  PvrBsShow( PVR_BS *pBS, HI_S32 nBits );
HI_S32  PvrBsPos( PVR_BS *pBS );
HI_S32  PvrBsSkip( PVR_BS *pBS, HI_S32 nBits );
HI_S32  PvrBsBack( PVR_BS *pBS, HI_S32 nBits );
HI_S32  PvrBsToNextByte( PVR_BS *pBS );
HI_S32  PvrBsBitsToNextByte( PVR_BS *pBS );
HI_S32  PvrBsResidBits( PVR_BS *pBS );
HI_S32  PvrBsIsByteAligned( PVR_BS *pBS );
HI_S32  PvrBsNextBitsByteAligned( PVR_BS *pBS, HI_S32 nBits );
HI_S32  PvrBsLongSkip(PVR_BS *pBS, HI_S32 nBits);
HI_S32  PvrBsSkipWithoutCount( PVR_BS *pBS, HI_S32 nBits );
HI_U8*  PvrBsGetNextBytePtr( PVR_BS *pBS );
HI_U32  PvrZerosMS_32(HI_U32 data);


#ifdef __cplusplus
}
#endif

#endif

