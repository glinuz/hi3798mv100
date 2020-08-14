#ifndef __AIP_CIRC_BUF_H__
#define __AIP_CIRC_BUF_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct
{
    volatile HI_U32  *pu32Write;
    volatile HI_U32  *pu32Read;
    volatile HI_U8   *pu8Data;
    HI_U32           u32Lenght;
} CIRC_BUF_S;

static inline HI_VOID CIRC_BUF_Init(CIRC_BUF_S *pstCb, 
                                    HI_VOID *pu32Write,  
                                    HI_VOID *pu32Read, 
                                    HI_VOID *pu8Data,
                                    HI_U32  u32Len)
{
    pstCb->pu32Write  = pu32Write;
    pstCb->pu32Read   = pu32Read;
    pstCb->pu8Data    = pu8Data;
    pstCb->u32Lenght  = u32Len;

    *pstCb->pu32Write = 0;
    *pstCb->pu32Read  = 0;
}

static inline HI_VOID CIRC_BUF_DeInit(CIRC_BUF_S *pstCb)
{
    return;
}

static inline HI_U32 CIRC_BUF_QueryBusy(CIRC_BUF_S *pstCb)
{
    HI_U32 u32ReadPos  = *(pstCb->pu32Read);
    HI_U32 u32WritePos = *(pstCb->pu32Write);

    return ((u32WritePos >= u32ReadPos) ? (u32WritePos - u32ReadPos) : pstCb->u32Lenght - (u32ReadPos - u32WritePos));
}

static inline HI_U32 CIRC_BUF_QueryFree(CIRC_BUF_S *pstCb)
{
    HI_U32 u32ReadPos  = *(pstCb->pu32Read);
    HI_U32 u32WritePos = *(pstCb->pu32Write);

    return ((u32WritePos >= u32ReadPos) ? (pstCb->u32Lenght - (u32WritePos - u32ReadPos)) : (u32ReadPos - u32WritePos));
}

static inline HI_U32 CIRC_BUF_Read(CIRC_BUF_S *pstCb, HI_U8 *pDest, HI_U32 u32Len)
{
    HI_U8  *pVirAddr   = HI_NULL;
    HI_U32  u32RdLen[2] = {0,0}, u32RdPos[2] = {0,0}, i;
    HI_U32  u32ReadPos, u32WritePos;

    u32ReadPos  = *(pstCb->pu32Read);
    u32WritePos = *(pstCb->pu32Write);

    u32RdPos[0] = u32ReadPos;
    if (u32WritePos >= u32ReadPos)
    {
        if (u32WritePos >= u32ReadPos + u32Len)
        {
            u32RdLen[0] = u32Len;
        }
        else
        {
            u32RdLen[0] = u32WritePos - u32ReadPos;
        }
    }
    else
    {
        if ( u32ReadPos + u32Len <= pstCb->u32Lenght)
        {
            u32RdLen[0] = u32Len;
        }
        else
        {
            u32RdLen[0] = pstCb->u32Lenght - u32ReadPos;

            u32RdPos[1] = 0;
            u32RdLen[1] = u32Len - u32RdLen[0];
            if(u32WritePos < u32RdLen[1])
            {
                u32RdLen[1] = u32WritePos;
            }
        }
    }

    for (i=0; ( i < 2 ) && (u32RdLen[i] != 0); i++)
    {
        pVirAddr = (HI_U8*)(pstCb->pu8Data + u32RdPos[i]);
        memcpy(pDest, pVirAddr, u32RdLen[i]);
        pDest += u32RdLen[i];
        u32ReadPos = u32RdPos[i] + u32RdLen[i];
    }

    *(pstCb->pu32Read) = ((u32ReadPos == pstCb->u32Lenght) ? 0 : u32ReadPos);

    return u32RdLen[0] + u32RdLen[1];
}

static inline HI_U32 CIRC_BUF_Write(CIRC_BUF_S *pstCb, HI_U8 *pDest, HI_U32 u32Len)
{
    HI_U8  *pVirAddr = HI_NULL;
    
    HI_U32  u32WtLen[2]={0,0};
    HI_U32  u32WtPos[2]={0,0};
    HI_U32  i;
    HI_U32  u32ReadPos, u32WritePos;

    u32ReadPos  = *(pstCb->pu32Read);
    u32WritePos = *(pstCb->pu32Write);
    
    u32WtPos[0] = u32WritePos;
    if (u32WritePos >= u32ReadPos)
    {
        if (pstCb->u32Lenght >= (u32WritePos + u32Len))
        {
            u32WtLen[0] = u32Len;
        }
        else
        {
            u32WtLen[0] = pstCb->u32Lenght - u32WritePos;
            u32WtLen[1] = u32Len - u32WtLen[0];
            
            u32WtPos[1] = 0;
        }
    }
    else
    {
        u32WtLen[0] = u32Len;
    }

    for (i = 0; ( i < 2 ) && (u32WtLen[i] != 0); i++)
    {
        pVirAddr = (HI_U8*)(pstCb->pu8Data + u32WtPos[i]);
        if(HI_NULL != pDest)  
        {
            memcpy(pVirAddr, pDest, u32WtLen[i]);
            pDest = pDest+ u32WtLen[i];
        }
        else
        {
            memset(pVirAddr, 0, u32WtLen[i]);
        }
        u32WritePos = u32WtPos[i] + u32WtLen[i];
    }

    *(pstCb->pu32Write) = ((u32WritePos == pstCb->u32Lenght) ? 0 : u32WritePos);

    return u32WtLen[0] + u32WtLen[1];
}

static inline HI_VOID CIRC_BUF_Flush(CIRC_BUF_S *pstCb)
{
    *pstCb->pu32Write = *pstCb->pu32Read;
}

static inline HI_U32 CIRC_BUF_UpdateRptr(CIRC_BUF_S *pstCb, HI_U32 u32Len)
{
    HI_U32 u32ReadPos  = *(pstCb->pu32Read);
    HI_U32 u32WritePos = *(pstCb->pu32Write);

    if (u32WritePos >= u32ReadPos)
    {
        u32ReadPos += u32Len;
    }
    else
    {
        if ( u32ReadPos + u32Len <= pstCb->u32Lenght)
        {
            u32ReadPos += u32Len;
        }
        else
        {
            u32ReadPos = u32Len - (pstCb->u32Lenght - u32ReadPos);
        }
    }

    *(pstCb->pu32Read) = ((u32ReadPos == pstCb->u32Lenght) ? 0 : u32ReadPos);

    return u32Len;
}

static inline HI_U32 CIRC_BUF_UpdateWptr(CIRC_BUF_S *pstCb, HI_U32 u32Len)
{
    HI_U32 u32ReadPos  = *(pstCb->pu32Read);
    HI_U32 u32WritePos = *(pstCb->pu32Write);
    
    if (u32WritePos >= u32ReadPos)
    {
        if (pstCb->u32Lenght >= (u32WritePos + u32Len))
        {
            u32WritePos += u32Len;
        }
        else
        {
            u32WritePos = u32Len - (pstCb->u32Lenght - u32WritePos);
        }
    }
    else
    {
        u32WritePos += u32Len;
    }

    *(pstCb->pu32Write) = ((u32WritePos == pstCb->u32Lenght) ? 0 : u32WritePos);

    return u32Len;
}

static inline HI_U32 CIRC_BUF_QueryRead(CIRC_BUF_S *pstCb)
{
    return *(pstCb->pu32Read);
}

static inline HI_U32 CIRC_BUF_QueryWrite(CIRC_BUF_S *pstCb)
{
    return *(pstCb->pu32Write);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __AIP_CIRC_BUF_H__*/
