#ifndef TELETEXT_UTILS_H__
#define TELETEXT_UTILS_H__

#include "teletext_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#define TTX_DIGIT_NUM_OF_PAGE_NUM  (3)
#define TTX_DIGIT_NUM_OF_SUBCODE  (4)
#define TTX_INPUT_NUM_BUF_LEN   (5)     /* ensure that TTX_INPUT_NUM_BUF_LEN > TTX_DIGIT_NUM_OF_SUBCODE && TTX_INPUT_NUM_BUF_LEN > TTX_DIGIT_NUM_OF_PAGE_NUM */


typedef struct tagTTX_INPUT_NUM_S
{
    HI_U8 au8Buf[TTX_INPUT_NUM_BUF_LEN];
    HI_U8 u8Count;
}TTX_INPUT_NUM_S;

HI_S32 TTX_PesQueue_Init(TTX_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen);

HI_S32 TTX_PesQueue_En(TTX_SEGMENT_QUEUE_S* pstQueue, TTX_SEGMENT_S* pSegment);

HI_S32 TTX_PesQueue_De(TTX_SEGMENT_QUEUE_S* pstQueue, TTX_SEGMENT_S* pSegment);

HI_S32 TTX_PesQueue_Reset(TTX_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen);

HI_S32 TTX_PesQueue_IsMemTensity(TTX_SEGMENT_QUEUE_S* pstQueue);



HI_BOOL TTX_IsLastFigureInputted(TTX_INPUT_NUM_S * pstInputNum, HI_U8 u8InputNumConuntMax);

HI_BOOL TTX_InputNumIsOnGoing(TTX_INPUT_NUM_S * pstInputNum);

HI_VOID TTX_ResetInputNum(TTX_INPUT_NUM_S * pstInputNum);



#ifdef __cplusplus
}
#endif

#endif
