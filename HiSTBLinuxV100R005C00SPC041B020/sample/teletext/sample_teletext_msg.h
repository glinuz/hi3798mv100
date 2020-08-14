#ifndef SAMPLE_TELETEXT_MSG_H__
#define SAMPLE_TELETEXT_MSG_H__

#include "hi_type.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagMESSAGE_S
{
    HI_U16  u16MsgLength;                                    /* data length*/
    HI_U8  *pu8MsgData;                                     /*pointer to data*/

    struct tagMESSAGE_S * pstNext;
} MESSAGE_S, *MESSAGE_S_PTR;

typedef struct tagMSG_QUEUE_S
{
    HI_U8*          pu8BaseAddr;            /*Init address,pointer to start position*/
    HI_U32          u32MaxLen;                    /*the max length of this Queue*/
    pthread_mutex_t Queue_lock;
    MESSAGE_S_PTR   pstMsgRear;            /* on receiving, pointer to write data */
    MESSAGE_S_PTR   pstMsgFront;           /* on parsing, pointer to read data */
} MSG_QUEUE_S, *MSG_QUEUE_S_PTR;

HI_S32 MsgQueue_Init(HI_VOID);

HI_S32 MsgQueue_DeInit(HI_VOID);

HI_S32 MsgQueue_En(MESSAGE_S * pstMsg);

HI_S32 MsgQueue_De(MESSAGE_S * pstMsg);

#ifdef __cplusplus
}
#endif

#endif
