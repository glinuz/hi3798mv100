#ifndef __DRV_VENC_BUF_MNG_H__
#define __DRV_VENC_BUF_MNG_H__

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

/*Read loop-buffer's out data type*/
typedef struct
{
    HI_VOID*   pSrc[2];
    HI_U32     u32Len[2];
} VALG_CB_RDINFO_S;

/*Write loop-buffer's out data type*/
typedef struct
{
    HI_VOID*   pDst[2];
    HI_U32     u32Len[2];
} VALG_CB_WRINFO_S;

/*Loop-buffer*/
typedef struct
{
    HI_VOID*  pBase;          /*Loop-buffer's start addr*/
    HI_U32    u32BufLen;      /*Loop-buffer total len, unit is byte*/
    HI_U32    u32RsvByte;     /*Judge loop-buffer is full or not, unit is byte*/
    HI_U32    u32RdHead;      /*Loop-buffer read head offset, unit is byte*/
    HI_U32    u32RdTail;      /*Loop-buffer read tail offset, unit is byte*/
    HI_U32    u32WrHead;      /*Loop-buffer write head offset, unit is byte*/
    HI_U32    u32WrTail;      /*Loop-buffer read tail offset, unit is byte*/
} VALG_CRCL_BUF_S;

typedef struct
{
    HI_U32  tU32a; // only used in ShowBits32 and FlushBits31/32
    HI_U32  tU32b;
    HI_U32* pBuff;
    HI_U32  tBits;
    HI_U32  totalBits;
    HI_U32  bBigEndian;
} tBitStream;

typedef struct
{
    HI_U32* pBase;  /* base addr    */
    HI_U32  Len;    /* max length   */
    HI_U32  Head;   /* head pointer */
    HI_U32  Sum;
    HI_U32  Mean;
} VALG_FIFO_S;

/*Loop-buffer for QueueFrame*/
typedef struct
{
    HI_VOID*  pBase;          /*Loop-buffer's start addr*/
    HI_U32    u32Vir2PhyOffset;
    HI_U32    u32BufLen;      /*Loop-buffer total len, unit is byte*/
    HI_U32    u32RdHead;      /*Loop-buffer read head offset, unit is byte*/
    HI_U32    u32RdTail;      /*Loop-buffer read tail offset, unit is byte*/
    HI_U32    u32WrHead;      /*Loop-buffer write head offset, unit is byte*/
    HI_U32    u32WrTail;      /*Loop-buffer read tail offset, unit is byte*/
} VALG_CRCL_QUE_BUF_S;
/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufInit(VALG_CRCL_BUF_S* pstCB, HI_VOID* pVirtBase, HI_U32 u32Len, HI_U32 u32RsvByte)
  Description:    Initiate circular buffer
  Input:          *pstCB
 *pVirtBase  Buffre start addr
                  u32Len     Buffer len
                  u32RsvByte reserve space
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         none
 ***********************************************************************************/
__inline static HI_S32 VENC_DRV_BufInit(VALG_CRCL_BUF_S* pstCB, HI_VOID* pVirtBase, HI_U32 u32BufLen, HI_U32 u32RsvByte)
{
    if (((u32BufLen & (WORD_ALIGN - 1)) != 0) || (u32BufLen < u32RsvByte))
    {
        return HI_FAILURE;
    }
    else
    {
        pstCB->u32BufLen = u32BufLen;
    }

    if ((0 == u32RsvByte) || (u32RsvByte & (WORD_ALIGN - 1)) != 0)
    {
        return HI_FAILURE;
    }
    else
    {
        pstCB->u32RsvByte = u32RsvByte;
    }

    pstCB->pBase = pVirtBase;
    pstCB->u32RdHead = 0;
    pstCB->u32RdTail = 0;
    pstCB->u32WrHead = 0;
    pstCB->u32WrTail = 0;
    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufWrite(VALG_CRCL_BUF_S* pstCB, HI_VOID* pVirtSrc, HI_U32 u32WrLen)
  Description:    Write one data segment into circular buffer.
  Input:          *pstCB
 *pVirtSrc     Start address of data segment will be write into circular buffer.
                  u32WrLen      Length of data segment put into circular buffer.
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         Write data into loop-buffer, source must be a contiguous space
                  write lenght must be word-alignment, not update read tail
 ***********************************************************************************/
__inline static HI_U32 VENC_DRV_BufGetFreeLen(VALG_CRCL_BUF_S* pstCB);

__inline static HI_S32 VENC_DRV_BufWrite(VALG_CRCL_BUF_S* pstCB, HI_VOID* pVirtSrc, HI_U32 u32WrLen)
{
    HI_U32 freeLen;
    HI_U32 i;
    VALG_CB_WRINFO_S stWrInfo;
    CPU_WORD*  pDst;
    CPU_WORD*  pSrc;
    HI_U32 u32WrHeadNew = 0;

    /* added by juchaodong */
    if (pVirtSrc == NULL) /* When VEDU finish encoding, update write head*/
    {
        pstCB->u32WrHead = u32WrLen; /* u32WrLen is reuse as VEDU's write pointer */
        return HI_SUCCESS;
    }

    /* add end */

    if ((u32WrLen & (WORD_ALIGN - 1)) != 0)
    {
        return HI_FAILURE;
    }

    /*Call get buffer data len function, get the actual len*/
    freeLen = VENC_DRV_BufGetFreeLen(pstCB);

    /*No enough space to write*/
    if (freeLen < u32WrLen)
    {
        return HI_FAILURE;
    }

    /*Get enough space to write two data packets*/
    /*VENC_DRV_BufWrHeadRun(pstCB, u32WrLen, &stWrInfo);*/
    /* y45339 copy from VENC_DRV_BufWrHeadRun*/
    /*If write head cross or even reach the bottom of buffer, turn back*/
    if ((pstCB->u32WrHead + u32WrLen) >= pstCB->u32BufLen)
    {
        /*Calculate two packets's length and addr*/
        stWrInfo.pDst[0]   = (ADDR_UNIT*)pstCB->pBase + pstCB->u32WrHead;
        stWrInfo.u32Len[0] = pstCB->u32BufLen - pstCB->u32WrHead;
        stWrInfo.pDst[1]   = pstCB->pBase;
        stWrInfo.u32Len[1] = u32WrLen - stWrInfo.u32Len[0];

        /*  pstCB->u32WrHead = stWrInfo.u32Len[1];
            write head point to start of the next packet */
        u32WrHeadNew = stWrInfo.u32Len[1];
    }
    else
    {
        stWrInfo.pDst[0]   = (ADDR_UNIT*)pstCB->pBase + pstCB->u32WrHead;
        stWrInfo.u32Len[0] = u32WrLen;
        stWrInfo.pDst[1]   = (ADDR_UNIT*)stWrInfo.pDst[0] + u32WrLen;
        stWrInfo.u32Len[1] = 0;       /*The second packet length is 0*/

        /*   pstCB->u32WrHead += u32WrLen;
             write head point to start of the next packet */
        u32WrHeadNew = pstCB->u32WrHead + u32WrLen;
    }

    /* y45339 copy end*/

    /*The first input packet copy to buffer*/
    pSrc = (CPU_WORD*)pVirtSrc;
    pDst = (CPU_WORD*)stWrInfo.pDst[0];
    i = stWrInfo.u32Len[0] / sizeof(CPU_WORD);
    while (i--)
    {
        *pDst++ = *pSrc++;
    }

    /*The second input packet copy to buffer*/
    pDst = (CPU_WORD*)stWrInfo.pDst[1];
    i = stWrInfo.u32Len[1] / sizeof(CPU_WORD);
    while (i--)
    {
        *pDst++ = *pSrc++;
    }

    pstCB->u32WrHead = u32WrHeadNew;
    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufRead(VALG_CRCL_BUF_S* pstCB, HI_U32 u32RdLen, VALG_CB_RDINFO_S *pstRdInfo)
  Description:    Read packets in buffer
  Input:          *pstCB
                  u32DataLen   Length of data segment read out from CB.
  Output:         *pstRdInfo   Buffer's packet information
  Return:        HI_SUCCESS/HI_FAILURE.
  Others:         1. Use when need to read packets from buffer, only return addr and length of read
                  2. If need to copy to other position, copy process is needed
                  3. Judge input length is correct, distinguish turn back or not
 ***********************************************************************************/
__inline static HI_S32 VENC_DRV_BufRead(VALG_CRCL_BUF_S* pstCB, HI_U32 u32RdLen, VALG_CB_RDINFO_S* pstRdInfo)
{
    HI_U32 u32Rh;

    /* Judge input length is correct
     * Notes: read head cannot cross write tail after update
     * Notes:
     * 1. The write tail pointer does not turn back
     * 2. The write tail pointer turn back
     *    1)read head does not turn back, read head will not cross write tail
     *    2)read head turn back;
     */
    u32Rh = pstCB->u32RdHead + u32RdLen;
    if (pstCB->u32RdHead <= pstCB->u32WrTail)
    {
        if (u32Rh > pstCB->u32WrTail)
        {
            return HI_FAILURE;
        }
    }
    else if (u32Rh >= pstCB->u32BufLen)
    {
        u32Rh -= pstCB->u32BufLen;
        if (u32Rh > pstCB->u32WrTail)
        {
            return HI_FAILURE;
        }
    }

    /*According to input out-packet length, to determine whether the loop buffer turn back*/
    /*Reach the bottom is also turn back */
    if ((pstCB->u32RdHead + u32RdLen) >= pstCB->u32BufLen)
    {
        /*Calculate two packets's length and addr*/
        pstRdInfo->pSrc[0]   = pstCB->pBase + pstCB->u32RdHead;
        pstRdInfo->u32Len[0] = pstCB->u32BufLen - pstCB->u32RdHead;

        /*The second input packet addr equal to buffer start addr*/
        pstRdInfo->pSrc[1]   = pstCB->pBase;
        pstRdInfo->u32Len[1] = u32RdLen - pstRdInfo->u32Len[0];

        /*Read head point to the next packet start position*/
        pstCB->u32RdHead = pstRdInfo->u32Len[1];
    }
    else
    {
        pstRdInfo->pSrc[0]   = (ADDR_UNIT*)pstCB->pBase + pstCB->u32RdHead;
        pstRdInfo->u32Len[0] = u32RdLen;
        pstRdInfo->pSrc[1] = (ADDR_UNIT*)pstRdInfo->pSrc[0] + u32RdLen;

        /*The second input packet length is 0*/
        pstRdInfo->u32Len[1] = 0;

        /*Read head point to the next packet start position*/
        pstCB->u32RdHead += u32RdLen;
    }

    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       VENC_DRV_BufIsEmpty(VALG_CRCL_BUF_S* pstCB)
  Description:    Check whether the circular buffer is empty or not.
  Input:          pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE
  Others:         None
 ***********************************************************************************/
__inline static HI_BOOL VENC_DRV_BufIsEmpty(VALG_CRCL_BUF_S* pstCB)
{
    if (pstCB->u32RdHead == pstCB->u32WrTail)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

/**********************************************************************************
  Function:       VENC_DRV_BufIsFull(VALG_CRCL_BUF_S* pstCB)
  Description:    Check whether the circular buffer is full or not.
  Input:          pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         None
 ***********************************************************************************/
__inline static HI_BOOL VENC_DRV_BufIsFull(VALG_CRCL_BUF_S* pstCB)
{
    HI_U32 u32Wh;

    u32Wh = pstCB->u32WrHead + pstCB->u32RsvByte;
    if (u32Wh >= pstCB->u32BufLen)
    {
        u32Wh -= pstCB->u32BufLen;
    }

    if (u32Wh == pstCB->u32RdTail)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

/**********************************************************************************
  Function:       VENC_DRV_BufIsVaild(VALG_CRCL_BUF_S* pstCB)
  Description:    Check whether the circular buffer is full or not.
  Input:          pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         None
 ***********************************************************************************/
__inline static HI_S32 VENC_DRV_BufIsVaild(VALG_CRCL_BUF_S* pstCB)
{
    /*  HI_U32 u32Rh;*/

    /* Judge input length is correct
     * Notes: read head cannot cross write tail after update
     * Notes:
     * 1. The write tail pointer does not turn back
     * 2. The write tail pointer turn back
     *    1)read head does not turn back, read head will not cross write tail
     *    2)read head turn back;
     */
    if (((pstCB->u32RdTail <= pstCB->u32RdHead)
         && (pstCB->u32RdHead <= pstCB->u32WrTail)
         && (pstCB->u32WrTail <= pstCB->u32WrHead))
        || ((pstCB->u32WrHead <= pstCB->u32RdTail)
            && (pstCB->u32RdTail <= pstCB->u32RdHead)
            && (pstCB->u32RdHead <= pstCB->u32WrTail))
        || ((pstCB->u32WrTail <= pstCB->u32WrHead)
            && (pstCB->u32WrHead <= pstCB->u32RdTail)
            && (pstCB->u32RdTail <= pstCB->u32RdHead))
        || ((pstCB->u32RdHead <= pstCB->u32WrTail)
            && (pstCB->u32WrTail <= pstCB->u32WrHead)
            && (pstCB->u32WrHead <= pstCB->u32RdTail)))
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufGetDataLen(VALG_CRCL_BUF_S* pstCB, HI_U32 *pu32DataLen)
  Description:    Get unprocessed data length of buffer
  Input:          *pstCB
  Output:         *pu32DataLen   Length of data segment read out from CB.
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         unprocessed data length between read head and write tail
                  1) Get but not yet release(read region)
                  2) Forbidden access data(write region)
                  to guarantee that when the data length is 0, buffer is empty

 ***********************************************************************************/
__inline static HI_U32 VENC_DRV_BufGetDataLen(VALG_CRCL_BUF_S* pstCB)
{
    HI_U32 u32DataLen = 0;

    if (!pstCB)
    {
        return u32DataLen;
    }
    if (pstCB->u32WrTail >= pstCB->u32RdHead)/* Write tail before read head, that did not turn back buffer*/
    {
        /*Calculate data length in buffer when turn back*/
        u32DataLen = pstCB->u32WrTail - pstCB->u32RdHead;
    }
    else
    {
        /*Calculate data length in buffer when turn back*/
        u32DataLen = pstCB->u32BufLen - (pstCB->u32RdHead - pstCB->u32WrTail);
    }

    return u32DataLen;
}

/**********************************************************************************
  Function:       VENC_DRV_BufGetFreeLen(VALG_CRCL_BUF_S* pstCB)
  Description:    Get the length of free space in the buffer.
  Input:          *pstCB
  Output:         None
  Return:         FreeLen.
  Others:         The space between WrHead and RdTail is free space.
                  Free space is calculated: between the write head and the tail length of time,
                  minus the reserved space.
                  note: The reader can not be counted as idle interval range.
                  so cannot get free length by (BufferLen - VENC_DRV_BufGetDataLen())
                  and there is a premise: through provision of space,
                  so write head never catch up with read tail.
 ***********************************************************************************/
__inline static HI_U32 VENC_DRV_BufGetFreeLen(VALG_CRCL_BUF_S* pstCB)
{
    HI_U32 u32FreeLen;

    if (pstCB->u32WrHead >= pstCB->u32RdTail)/* Write head before read tail, that did not turn back buffer*/
    {
        /*Calculate data length in buffer when not turn back yet*/
        u32FreeLen = pstCB->u32BufLen - (pstCB->u32WrHead - pstCB->u32RdTail) - pstCB->u32RsvByte;
    }
    else
    {
        /*Calculate data length in buffer when not turn back yet*/
        u32FreeLen = pstCB->u32RdTail - pstCB->u32WrHead - pstCB->u32RsvByte;
    }

    return u32FreeLen;
}

/**********************************************************************************
  Function:       VENC_DRV_BufRdRegionLen
  Description:    get length of read region
  Input:          *pstCB
  Output:         none
  Return:         u32RdRegionLen
  Others:         get length of read region(space between read head and tail)
 ***********************************************************************************/
__inline static HI_U32 VENC_DRV_BufRdRegionLen (VALG_CRCL_BUF_S* pstCB)
{
    HI_U32 u32RdRegionLen;

    if (pstCB->u32RdHead >= pstCB->u32RdTail)
    {
        u32RdRegionLen = pstCB->u32RdHead - pstCB->u32RdTail;
    }
    else
    {
        u32RdRegionLen = pstCB->u32BufLen - (pstCB->u32RdTail - pstCB->u32RdHead);
    }

    return u32RdRegionLen;
}

/**********************************************************************************
  Function:       VENC_DRV_BufWrRegionLen
  Description:    get length of write region
  Input:          *pstCB
  Output:         none
  Return:         u32WrRegionLen
  Others:         get length of write region(space between write head and tail)
 ***********************************************************************************/
__inline static HI_U32 VENC_DRV_BufWrRegionLen (VALG_CRCL_BUF_S* pstCB)
{
    HI_U32 u32WrRegionLen;

    if (pstCB->u32WrHead >= pstCB->u32WrTail)
    {
        u32WrRegionLen = pstCB->u32WrHead - pstCB->u32WrTail;
    }
    else
    {
        u32WrRegionLen = pstCB->u32BufLen - (pstCB->u32WrTail - pstCB->u32WrHead);
    }

    return u32WrRegionLen;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufGetRdHead(VALG_CRCL_BUF_S* pstCB)
  Description:    Get stream packet read head in buffer, but do not update read pointer
  Input:          *pstCB
  Output:         None
  Return:         Current read pointer position
  Others:         Before use to determine whether the buffer is not empty
 ***********************************************************************************/
__inline static HI_VOID* VENC_DRV_BufGetRdHead(VALG_CRCL_BUF_S* pstCB)
{
    HI_VOID* pRh;

    pRh = (ADDR_UNIT*)(pstCB->pBase) + pstCB->u32RdHead;
    return pRh;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufGetRdTail(VALG_CRCL_BUF_S* pstCB)
  Description:    Get stream packet read tail in buffer, but do not update read pointer
  Input:          *pstCB
  Output:         None
  Return:         Current read tail position
  Others:         Before use to determine whether the buffer is not empty
 ***********************************************************************************/
__inline static HI_VOID* VENC_DRV_BufGetRdTail(VALG_CRCL_BUF_S* pstCB)
{
    HI_VOID* pRt;

    pRt = (ADDR_UNIT*)(pstCB->pBase) + pstCB->u32RdTail;
    return pRt;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufUpdateRp(VALG_CRCL_BUF_S* pstCB, HI_U32 u32DataLen)
  Description:    update read pointer
  Input:          *pstCB
                  u32DataLen  update data length
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         After stream packets are copied out, if reserve space allow to write data, call the function.
                  1. only update read tail
                  2. Judge input length is correct, note turn back or not
 ***********************************************************************************/
__inline static HI_S32 VENC_DRV_BufUpdateRp(VALG_CRCL_BUF_S* pstCB, HI_U32 u32RdLen)
{
    HI_U32 u32Rt;

    if ((u32RdLen & (WORD_ALIGN - 1)) != 0)
    {
        return HI_FAILURE;
    }

    /*The length of the correctness of judgments*/
    u32Rt = pstCB->u32RdTail + u32RdLen;

    /* Judge input length is correct
     * Notes: read head cannot cross write tail after update
     * Notes:
     * 1. The write tail pointer does not turn back
     * 2. The write tail pointer turn back
     *    1)read head does not turn back, read head will not cross write tail
     *    2)read head turn back;
     */
    if (pstCB->u32RdHead >= pstCB->u32RdTail)
    {
        if (u32Rt > pstCB->u32RdHead)
        {
            return HI_FAILURE;
        }
    }
    else if (u32Rt >= pstCB->u32BufLen)
    {
        u32Rt -= pstCB->u32BufLen;
        if (u32Rt > pstCB->u32RdHead)
        {
            return HI_FAILURE;
        }
    }

    pstCB->u32RdTail = u32Rt;
    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufUpdateRp(VALG_CRCL_BUF_S* pstCB, HI_U32 u32DataLen)
  Description:    update read pointer
  Input:          *pstCB
                  u32DataLen  update data length
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         After stream packets are copied out, if reserve space allow to write data, call the function.
                  1. only update read tail
                  2. Judge input length is correct, note turn back or not
 ***********************************************************************************/
__inline static HI_S32 VENC_DRV_BufGetNextRp(VALG_CRCL_BUF_S* pstCB, HI_U32 u32RdLen)
{
    HI_U32 u32Rt;

    if ((u32RdLen & (WORD_ALIGN - 1)) != 0)
    {
        return HI_FAILURE;
    }

    /*The length of the correctness of judgments*/
    u32Rt = pstCB->u32RdHead + u32RdLen;

    /* Judge input length is correct
     * Notes: read head cannot cross write tail after update
     * Notes:
     * 1. The write tail pointer does not turn back
     * 2. The write tail pointer turn back
     *    1)read head does not turn back, read head will not cross write tail
     *    2)read head turn back;
     */
    if (pstCB->u32WrTail >= u32Rt)
    {
        if (pstCB->u32WrTail == u32Rt)
        {
            return HI_FAILURE;
        }
    }
    else if (u32Rt >= pstCB->u32BufLen)
    {
        u32Rt -= pstCB->u32BufLen;
        if (u32Rt > pstCB->u32WrTail)
        {
            return HI_FAILURE;
        }
    }

    pstCB->u32RdHead = u32Rt;
    return HI_SUCCESS;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufUpdateWp(VALG_CRCL_BUF_S* pstCB)
  Description:    update write pointer
  Input:          *pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         After stream packets are copied into buffer, if data can be read, call the function.
 ***********************************************************************************/
__inline static HI_VOID VENC_DRV_BufUpdateWp(VALG_CRCL_BUF_S* pstCB)
{
    pstCB->u32WrTail = pstCB->u32WrHead;
    return;
}

/**********************************************************************************
  Function:       HI_S32 VENC_DRV_BufBackWp(VALG_CRCL_BUF_S* pstCB)
  Description:    Back write pointer
  Input:          *pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         After stream buffer is full, back stream
 ***********************************************************************************/
__inline static HI_VOID VENC_DRV_BufBackWp(VALG_CRCL_BUF_S* pstCB)
{
    pstCB->u32WrHead = pstCB->u32WrTail;
    return;
}

/**********************************************************************************
  Function:       VENC_DRV_BufBackRp
  Description:    Back read head
  Input:          *pstCB
  Output:         None
  Return:         HI_SUCCESS/HI_FAILURE.
  Others:         Used to search buffer content
 ***********************************************************************************/
__inline static HI_VOID VENC_DRV_BufBackRp(VALG_CRCL_BUF_S* pstCB)
{
    pstCB->u32RdHead = pstCB->u32RdTail;
    return;
}

/**********************************************************************************
  Function:       VENC_DRV_BufReset(VALG_CRCL_QUE_S* pstCQ)
  Description:    .
                  .
  Input:          pstCQ
  Output:         None
  Return:
  Others:
 ***********************************************************************************/
__inline static HI_VOID VENC_DRV_BufReset(VALG_CRCL_BUF_S* pstCB)
{
    pstCB->u32RdHead = 0;
    pstCB->u32RdTail = 0;
    pstCB->u32WrHead = 0;
    pstCB->u32WrTail = 0;

    return;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
#define FrmRateCntBufNum  65

__inline static HI_VOID VENC_DRV_FifoInit(VALG_FIFO_S* pFifo, HI_VOID* pBase, HI_U32 Len, HI_U32 Mean)
{
    int i;

    pFifo->pBase = (HI_U32*)pBase;
	if (Len >= FrmRateCntBufNum) {Len = FrmRateCntBufNum;}

    pFifo->Len  = Len;
    pFifo->Head = 0;
    pFifo->Sum  = 0;
    pFifo->Mean = Mean;

    for (i = 0; i < Len; i++)
    {
        pFifo->pBase[i] = 0;
    }
}

__inline static HI_VOID VENC_DRV_FifoWriteInit(VALG_FIFO_S* pFifo, HI_U32 Val)
{
    if (pFifo->pBase == NULL)
    {
        return;
    }

    pFifo->Sum -= pFifo->pBase[pFifo->Head];
    pFifo->Sum += Val;

    pFifo->pBase[pFifo->Head] = Val;

    pFifo->Head++;

    if (pFifo->Head >= pFifo->Len)
    {
        pFifo->Head -= pFifo->Len;
    }
}

__inline static HI_VOID VENC_DRV_FifoWrite(VALG_FIFO_S* pFifo, HI_U32 Val)
{
    if (pFifo->pBase == NULL)
    {
        return;
    }

    if (pFifo->Sum > pFifo->Mean)
    {
        //pFifo->Sum -= pFifo->Mean;
        pFifo->Sum -= pFifo->pBase[pFifo->Head];
    }
    else
    {
        pFifo->Sum = 0;
    }

    pFifo->Sum += Val;

    pFifo->pBase[pFifo->Head] = Val;

    pFifo->Head++;

    if (pFifo->Head >= pFifo->Len)
    {
        pFifo->Head -= pFifo->Len;
    }
}

HI_VOID VENC_DRV_BsOpenBitStream(tBitStream* pBS, HI_U32* pBuffer);
HI_VOID VENC_DRV_BsPutBits31(tBitStream* pBS, HI_U32 code, HI_U32 bits);
HI_VOID VENC_DRV_BsPutBits32(tBitStream* pBS, HI_U32 code, HI_U32 bits);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
