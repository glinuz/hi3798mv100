#ifndef __TVP_ADAPTER_H__
#define __TVP_ADAPTER_H__

#define SEC_VFMW_VERSION      "2016061300s"

#define DEBUG_SAVE_SUPPORT    (0)

#define MAX_RAW_NUM           (1024)
#define MAX_EVENT_NUM         (1024)
#define MAX_PARA_SIZE         (64)

#define MAX_PROC_SIZE         (100*1024)

/* stream list address */
typedef struct hiSTREAM_LIST_S
{
    UADDR  stream_list_phy_addr;
} STREAM_LIST_S;

typedef struct
{
    HI_S32      Head_NS;
    HI_S32      Head_S;
    HI_S32      Tail_S;
    HI_S32      Tail_NS;
    STREAM_DATA_S RawPacket[MAX_RAW_NUM];
} RAW_ARRAY_NS;

typedef struct
{
    HI_S32              IsValid;
    HI_S32              ChanID;
    HI_S32              Type;
    VDEC_ADAPTER_TYPE_E eAdapterType;          /* channel type vdec/omxvdec */
    HI_U8               para[MAX_PARA_SIZE];
} MSG_DATA_S;

typedef struct
{
    HI_S32      Head;
    HI_S32      Tail;
    MSG_DATA_S  Message[MAX_EVENT_NUM];
} CALLBACK_ARRAY_NS;


#if defined(ENV_ARMLINUX_KERNEL)


typedef struct hiVFMW_CALLBACK_S
{
    SINT32        (*event_report_vdec)(SINT32 ChanID, SINT32 type, VOID *p_args );
    SINT32        (*event_report_omxvdec)(SINT32 ChanID, SINT32 type, VOID *p_args );
}VFMW_CALLBACK_S;

HI_VOID TVP_VDEC_OpenModule(HI_VOID);
HI_VOID TVP_VDEC_ExitModule(HI_VOID);
HI_S32  TVP_VDEC_Init(HI_S32 (*VdecCallback)(HI_S32, HI_S32, HI_VOID *));
HI_S32  TVP_VDEC_InitWithOperation(VDEC_OPERATION_S *pArgs);
HI_S32  TVP_VDEC_Exit(HI_VOID);
HI_S32  TVP_VDEC_Control(HI_S32 ChanID, VDEC_CID_E eCmdID, HI_VOID *pArgs);
HI_S32  TVP_VDEC_Suspend(HI_VOID);
HI_S32  TVP_VDEC_Resume(HI_VOID);
HI_S32  TVP_VDEC_SetDbgOption (HI_U32 opt, HI_U8 *p_args);
HI_S32  TVP_VDEC_TrustDecoderInit(VDEC_OPERATION_S *pArgs);
HI_S32  TVP_VDEC_TrustDecoderExit(HI_VOID);

#endif

#endif

