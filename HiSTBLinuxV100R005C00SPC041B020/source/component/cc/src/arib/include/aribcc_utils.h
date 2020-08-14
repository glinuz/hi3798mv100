#ifndef ARIBCC_UTILS_H__
#define ARIBCC_UTILS_H__
//#include "teletext_def.h"
#include "hi_type.h"
#include "pthread.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagARIBCC_SEGMENT_S
{
    HI_U16  u16DataLength;                                    /* data length*/
    HI_U8  *pu8SegmentData;                                     /*pointer to data*/
    struct tagARIBCC_SEGMENT_S* pstNext;
} ARIBCC_SEGMENT_S, *ARIBCC_SEGMENT_S_PTR;

typedef struct tagARIBCC_SEGMENT_QUEUE_S
{
    const HI_U8*      pu8BaseAddr;                /*Init address,pointer to start position*/
    HI_U32            u32MaxLen;                  /*the max length of this Queue*/
    pthread_mutex_t   Queue_lock;
    HI_BOOL           bFull;
    ARIBCC_SEGMENT_S_PTR pstSegmentRear;              /* on receiving, pointer to write data */
    ARIBCC_SEGMENT_S_PTR pstSegmentFront;             /* on parsing, pointer to read data */
} ARIBCC_SEGMENT_QUEUE_S, *ARIBCC_SEGMENT_QUEUE_S_PTR;

HI_S32 ARIBCC_PesQueue_Init(ARIBCC_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen);

HI_S32 ARIBCC_PesQueue_En(ARIBCC_SEGMENT_QUEUE_S* pstQueue, ARIBCC_SEGMENT_S* pSegment);

HI_S32 ARIBCC_PesQueue_De(ARIBCC_SEGMENT_QUEUE_S* pstQueue, ARIBCC_SEGMENT_S* pSegment);

HI_S32 ARIBCC_PesQueue_Reset(ARIBCC_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen);

#ifdef __cplusplus
}
#endif

#endif
