#ifndef __VFMW_TEE_ADAPTER_H__
#define __VFMW_TEE_ADAPTER_H__

#include "vfmw.h"

#define DEBUG_SAVE_SUPPORT    (0)

#define MAX_RAW_NUM           (512)
#define MAX_EVENT_NUM         (512)
#define MAX_PARA_SIZE         (1024)

#define MAX_PROC_SIZE         (100*1024)

#define TRUSTED_CHAN_BEGIN    (MAX_CHAN_NUM-6)

/* stream list address */
typedef struct hiSTREAM_LIST_S
{
    UADDR  stream_list_phy_addr;
} STREAM_LIST_S;

typedef struct
{
    SINT32      Head_NS;
    SINT32      Head_S;
    SINT32      Tail_S;
    SINT32      Tail_NS;
    STREAM_DATA_S RawPacket[MAX_RAW_NUM];
} RAW_ARRAY_NS;

typedef struct
{
    SINT32      Head;
    SINT32      Tail;
    SINT32      History;
    IMAGE      StImage[MAX_QUEUE_IMAGE_NUM];
} IMAGE_LIST_S;
typedef struct
{
    SINT32     IsUsed;
    UADDR      TopLumaPhyAddr;
} OUTPUT_IMAGE_RECORD_S;
typedef struct
{
    OUTPUT_IMAGE_RECORD_S   StOutputRec[MAX_QUEUE_IMAGE_NUM];
    IMAGE_LIST_S            StOutputImg;
    IMAGE_LIST_S            StReleaseImg;
} IMAGE_QUEUE_NS;

typedef struct
{
    SINT32      IsValid;
    SINT32      ChanID;
    SINT32      Type;
    UINT8       para[MAX_PARA_SIZE];
    VDEC_ADAPTER_TYPE_E eAdapterType;          /* channel type vdec/omxvdec */
} MSG_DATA_S;

typedef struct
{
    SINT32      Head;
    SINT32      Tail;
    MSG_DATA_S  Message[MAX_EVENT_NUM];
} CALLBACK_ARRAY_NS;

typedef struct
{
    CALLBACK_ARRAY_NS  CallBackBuf;
    VDEC_CHAN_OPTION_S ChanOption;
    RAW_ARRAY_NS       StreamBuf[MAX_CHAN_NUM];
    VDEC_CHAN_STATE_S  ChanState[MAX_CHAN_NUM];
    IMAGE_QUEUE_NS     ImageQue[MAX_CHAN_NUM];
#ifndef  HI_ADVCA_FUNCTION_RELEASE
    UINT8              ProcBuf[MAX_PROC_SIZE];
#endif
#if (1 == DEBUG_SAVE_SUPPORT)
    UINT8              SaveStreamBuf[3 * 1024 * 1024];
#endif
} MEMORY_NEEDED_SECVFMW_S;

#if defined(ENV_ARMLINUX_KERNEL)

VOID   TVP_VDEC_OpenModule(VOID);
VOID   TVP_VDEC_ExitModule(VOID);
SINT32 TVP_VDEC_TrustDecoderInit(VDEC_OPERATION_S *pArgs);
SINT32 TVP_VDEC_TrustDecoderExit(VOID);
SINT32 TVP_VDEC_Control(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs);
SINT32 TVP_VDEC_Suspend(VOID);
SINT32 TVP_VDEC_Resume(VOID);
SINT32 TVP_VDEC_GetImage(SINT32 ChanID, IMAGE *pImage);
SINT32 TVP_VDEC_ReleaseImage(SINT32 ChanID, IMAGE *pImage);

#endif

#endif

