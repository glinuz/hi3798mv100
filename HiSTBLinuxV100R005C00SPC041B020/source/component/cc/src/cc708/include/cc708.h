/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/
#ifndef __CC708_H__
#define __CC708_H__

#include<pthread.h>

#include "hi_type.h"
#include "cc_queue.h"
#include "hi_unf_cc.h"

#ifdef __cplusplus
extern "C" {
#endif


#define USERDATA_QUEUE_SIZE 256
#define USERDATA_BUFFER_SIZE 256
#define SERVICE_QUEUE_SIZE 128


typedef enum tagDTVCC_ModuleState_E
{
    CC_INIT,
    CC_READY,
    CC_RUN,
    CC_STOP
}DTVCC_ModuleState_E;

typedef struct tagDTVCC_Screen_S
{
    HI_UNF_CC_RECT_S PhysicalRect;          /* screen position and size; */
    HI_UNF_CC_RECT_S CaptionRect ;          /* safe-title area(caption area) position and size;*/
    HI_U16           u16MaxHorizonalCells;  /* max horizonal anchor point*/
    HI_U16           u16MaxColumnNum;       /* max cc columns for a window,32 for 4:3 and 42 for 16:9*/
} DTVCC_Screen_S;

typedef struct tagServiceFifo_S
{
    HI_U8   *pu8Buffer;
    HI_U8   u8ServiceLen;
}DTVCC_ServiceElement_S;

typedef struct tagDTVCC_Element_S
{
    HI_U8   *pu8Buffer;
    HI_U16  u16BufferSize;
    HI_U32  u32DataSize;
    HI_U32  u32UserData;
}DTVCC_Element_S;

typedef struct tagDTVCC_CMDBuffer_S
{
    HI_U8   au8CMD[8];
    HI_U8   u8TotalSize;
    HI_U8   u8CurSize;
}DTVCC_CMDBuffer_S;

typedef struct tagDTVCC_Packet_S
{
    HI_U8   u8SeqenceNo;
    HI_U8   u8TotalSize;
    HI_U8   u8CurSize;
    HI_U8   au8PacketData[128];
    HI_BOOL bStart;            /*indicate if received PACKET_START_FLAG*/
}DTVCC_Packet_S;

typedef struct tagDTVCC_ServiceBlock_S
{
    HI_U8   u8ServiceNo;
    HI_U8   u8ServiceSize;
    HI_U8   u8CurSize;
    HI_U8   u8Data[32];
}DTVCC_ServiceBlock_S;

/*******************************************************************************
* CC module object structure
*******************************************************************************/
typedef struct tagCC708_OBJECT_S
{
    HI_U8                  u8ModuleID;
    HI_U8                  u8IsStart;        /*indicate if is ready to start*/
    HI_U8                  u8IsDelay;        /*set to 1 when receive delay command*/
    DTVCC_ModuleState_E    enModuleState;
    CRITICAL_SECTION       mCriticalSection; /*mutual exclusive critical sectio,synchronization objectn*/

    DTVCC_ServiceBlock_S   stServiceBlock;
    HI_S32                 s32LastSeqenceNo;  /*verify if there some sequences missing*/
    HI_BOOL                bServiceBlockEnded;

    DTVCC_Packet_S         stPacket;
    DTVCC_CMDBuffer_S      stCMDBuffer;
    DTVCC_Screen_S         stScreen;            /*Main window defintion*/

    HI_S32                 s32RowHeight;
    HI_S32                 s32MaxCharWidth;
    HI_U8                  u8ScrollInterval;
    HI_U8                  u8ScrollTimes;

    HI_U32                 u32ServiceType;/*Service  #1  is  designated as the Primary Caption Service...*/
    HI_U32                 u32UserFontName;       //font style 0-7
    HI_U32                 u32UserFontStyle;   //font type:Normal=0,Italic=1,Underline&Normal =2, Underline&Italic = 3;
    HI_U32                 u32UserFontSize;   //CC_SMALL_PENSIZE,CC_STANDARD_PENSIZE,CC_LARGE_PENSIZE
    HI_U32                 u32UserTextFGColor;
    HI_U32                 u32UserTextFGOpacity;
    HI_U32                 u32UserTextBgColor;
    HI_U32                 u32UserTextBGOpacity;
    HI_U32                 u32UserWinColor;
    HI_U32                 u32UserWinOpac;
    HI_U32                 u32UserTextEdgetype;
    HI_U32                 u32UserTextEdgecolor;
    HI_U32                 u32CurTextEdgeOpacity;

    TRI_QUEUE              stUserdataQueue;
    TRI_QUEUE              stServiceQueue;
    HI_U8                  au8UserData[USERDATA_QUEUE_SIZE][USERDATA_BUFFER_SIZE];
    HI_U8                  au8ServiceData[SERVICE_QUEUE_SIZE][32];
} CC708_OBJECT_S;


/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

HI_S32 CC708_Init(HI_VOID);
HI_S32 CC708_DeInit(HI_VOID);

HI_S32 CC708_Create(HI_HANDLE *hcc708);
HI_S32 CC708_Destroy(HI_HANDLE hcc708);

HI_S32 CC708_Start(HI_U8 moduleID);
HI_S32 CC708_Stop(HI_U8 moduleID);
HI_S32 CC708_Reset(HI_U8 moduleID);
HI_U8  CC708_IsStart(HI_U8 moduleID);

HI_S32 CC708_Config(HI_U8 moduleid,HI_UNF_CC_708_CONFIGPARAM_S *pCC708Config);
HI_S32 CC708_GetConfig(HI_U8 moduleid,HI_UNF_CC_708_CONFIGPARAM_S *pstCC708ConfigParam);

HI_S32 CC708_ParseUserData(HI_U8 moduleID, HI_U8 *pu8userdata, HI_U32 u32dataLen, HI_BOOL bTopFieldFirst);
HI_S32 CC708_Userdata_Inject(HI_U8 moduleID, HI_U8 *pu8UserData, HI_U32 u32UsrDataLen, HI_BOOL bTopFieldFirst);
HI_S32 CC708_ProcessData(HI_VOID);
HI_S32 CC708_CharFlash(HI_VOID);

#ifdef __cplusplus
}
#endif

#endif

/*****************************************************************************
*                    End Of File
*****************************************************************************/
