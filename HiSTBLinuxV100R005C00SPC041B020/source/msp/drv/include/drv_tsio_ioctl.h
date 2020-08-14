/*******************************************************************************
*             Copyright 2016 - 2016, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: drv_tsio_ioctl.h
* Description: tsio ioctl definition.
*
* History:
* Version   Date              Author      DefectNum      Description
* main\1    2016-08-15        l00326021     NULL            init.
*******************************************************************************/

#ifndef __DRV_TSIO_IOCTL_H__
#define __DRV_TSIO_IOCTL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TSIO_CMD_MASK              (0xF0)
#define TSIO_GBL_CMD               (0x00)
#define TSIO_TSI_PORT_CMD          (0x10)
#define TSIO_RAM_PORT_CMD          (0x20)
#define TSIO_CHAN_CMD              (0x30)
#define TSIO_SE_CMD                (0x40)
#define TSIO_CC_CMD                (0x50)

#define TSIO_IOCTL_GBL_GETCAP          _IOWR(HI_ID_TSIO, 0x0, TSIO_CAPABILITY)  

typedef struct {
    TSIO_PORT           Port;      /* [in] */
    TSIO_TSI_Port_Attrs Attrs;     /* [in] */
    HI_HANDLE           Handle;    /* [out] */
} TSIO_OpenTsiPort_Info;

typedef struct {
    HI_HANDLE           Handle;    /* [in] */
    TSIO_TSI_Port_Attrs Attrs;     /* [out] */
}TSIO_GetTsiPortAttr_Info;

typedef TSIO_GetTsiPortAttr_Info TSIO_SetTsiPortAttr_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    TSIO_TSI_Port_Status Status;    /* [out] */
}TSIO_GetTsiPortStatus_Info;

#define TSIO_IOCTL_TSI_OPENPORT        _IOWR(HI_ID_TSIO, 0x10, TSIO_OpenTsiPort_Info) 
#define TSIO_IOCTL_TSI_GETPORTATTRS    _IOWR(HI_ID_TSIO, 0x11, TSIO_GetTsiPortAttr_Info)
#define TSIO_IOCTL_TSI_SETPORTATTRS    _IOW(HI_ID_TSIO,  0x12, TSIO_SetTsiPortAttr_Info) 
#define TSIO_IOCTL_TSI_GETPORTSTATUS   _IOWR(HI_ID_TSIO, 0x13, TSIO_GetTsiPortStatus_Info)
#define TSIO_IOCTL_TSI_CLOSEPORT       _IOW(HI_ID_TSIO,  0x14, HI_HANDLE) 


typedef struct {
    TSIO_PORT           Port;      /* [in] */
    TSIO_RAM_Port_Attrs Attrs;     /* [in] */
    HI_HANDLE           Handle;    /* [out] */
    TSIO_BUFFER         Buf;       /* [out] */
} TSIO_OpenRamPort_Info;

typedef struct {
    HI_HANDLE           Handle;    /* [in] */
    TSIO_RAM_Port_Attrs Attrs;     /* [out] */
}TSIO_GetRamPortAttr_Info;

typedef TSIO_GetRamPortAttr_Info TSIO_SetRamPortAttr_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    TSIO_RAM_Port_Status Status;    /* [out] */
}TSIO_GetRamPortStatus_Info;

typedef struct {
    HI_HANDLE            Handle;         /* [in] */
    HI_U8                EvenIVKey[16];  /* [in] */
    HI_U8                OddIVKey[16];   /* [in] */
}TSIO_BeginBulkSeg_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    HI_U32               ReqLen;    /* [in] */
    HI_U32               TimeOut;   /* [in] */
    TSIO_BUFFER          Buf;       /* [out] */
}TSIO_GetRamBuffer_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    TSIO_BUFFER          Buf;       /* [in] */
}TSIO_PushRamBuffer_Info;

typedef TSIO_PushRamBuffer_Info TSIO_PutRamBuffer_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    HI_U32               PhyAddr;   /* [out] */
}TSIO_PreMmap_Info;

typedef struct {
    HI_HANDLE            Handle;    /* [in] */
    HI_VOID              *UsrAddr;  /* [in] */
}TSIO_PstMmap_Info;

#define TSIO_IOCTL_RAM_OPENPORT        _IOWR(HI_ID_TSIO, 0x20, TSIO_OpenRamPort_Info)  
#define TSIO_IOCTL_RAM_GETPORTATTRS    _IOWR(HI_ID_TSIO, 0x21, TSIO_GetRamPortAttr_Info)
#define TSIO_IOCTL_RAM_SETPORTATTRS    _IOW(HI_ID_TSIO,  0x22, TSIO_SetRamPortAttr_Info)
#define TSIO_IOCTL_RAM_GETPORTSTATUS   _IOWR(HI_ID_TSIO, 0x23, TSIO_GetRamPortStatus_Info)
#define TSIO_IOCTL_RAM_BEGINBULKSEG    _IOW(HI_ID_TSIO,  0x24, TSIO_BeginBulkSeg_Info)
#define TSIO_IOCTL_RAM_ENDBULKSEG      _IOW(HI_ID_TSIO,  0x25, HI_HANDLE)
#define TSIO_IOCTL_RAM_GETBUFFER       _IOWR(HI_ID_TSIO, 0x26, TSIO_GetRamBuffer_Info)
#define TSIO_IOCTL_RAM_PUSHBUFFER      _IOW(HI_ID_TSIO,  0x27, TSIO_PushRamBuffer_Info) 
#define TSIO_IOCTL_RAM_PUTBUFFER       _IOW(HI_ID_TSIO,  0x28, TSIO_PutRamBuffer_Info) 
#define TSIO_IOCTL_RAM_RESETBUFFER     _IOW(HI_ID_TSIO,  0x29, HI_HANDLE)
#define TSIO_IOCTL_RAM_CLOSEPORT       _IOW(HI_ID_TSIO,  0x2a, HI_HANDLE)
#define TSIO_IOCTL_RAM_PREMMAP         _IOWR(HI_ID_TSIO, 0x2b, TSIO_PreMmap_Info)  
#define TSIO_IOCTL_RAM_PSTMMAP         _IOW(HI_ID_TSIO,  0x2c, TSIO_PstMmap_Info) 


typedef struct {
    HI_HANDLE           PtHandle;   /* [in] */
    HI_U32              Pid;        /* [in] */
    HI_HANDLE           Handle;     /* [out] */
}TSIO_CreatePidChannel_Info;

typedef struct {
    HI_HANDLE               Handle; /* [in] */
    TSIO_Pid_Channel_Status Status; /* [out] */
}TSIO_GetPidChannelStatus_Info;

#define TSIO_IOCTL_PID_CREATECHANNEL    _IOWR(HI_ID_TSIO, 0x30, TSIO_CreatePidChannel_Info)  
#define TSIO_IOCTL_PID_GETCHANNELSTATUS _IOWR(HI_ID_TSIO, 0x31, TSIO_GetPidChannelStatus_Info)
#define TSIO_IOCTL_PID_DESTROYCHANNEL   _IOW(HI_ID_TSIO,  0x32, HI_HANDLE) 

typedef struct {
    HI_HANDLE           PtHandle;   /* [in] */
    HI_HANDLE           Handle;     /* [out] */
}TSIO_CreateRawChannel_Info;

#define TSIO_IOCTL_RAW_CREATECHANNEL    _IOWR(HI_ID_TSIO, 0x33, TSIO_CreateRawChannel_Info)  
#define TSIO_IOCTL_RAW_DESTROYCHANNEL   _IOW(HI_ID_TSIO,  0x34, HI_HANDLE) 


typedef struct {
    TSIO_SID                 Sid;       /* [in] */
    TSIO_Secure_Engine_Attrs Attrs;     /* [in] */
    HI_HANDLE                Handle;    /* [out] */
}TSIO_CreateSe_Info;

typedef struct {
    HI_HANDLE                Handle;    /* [in] */
    TSIO_Secure_Engine_Attrs Attrs;     /* [out] */
}TSIO_GetSeAttr_Info;

typedef TSIO_GetSeAttr_Info TSIO_SetSeAttr_Info;

typedef struct {
    HI_HANDLE                 Handle;    /* [in] */
    TSIO_Secure_Engine_Status Status;    /* [out] */
}TSIO_GetSeStatus_Info;;

typedef struct {
    HI_HANDLE                 Handle;    /* [in] */
    HI_HANDLE                 ChHandle;  /* [in] */
}TSIO_AddSeChannel_Info;

typedef TSIO_AddSeChannel_Info TSIO_DelSeChannel_Info;

typedef struct {
    HI_HANDLE                 Handle;    /* [in] */
    HI_U32                    ReqLen;    /* [in] */
    HI_U32                    TimeOut;   /* [in] */
    TSIO_BUFFER               Buf;       /* [out] */
}TSIO_AcqSeBuffer_Info;

typedef TSIO_AcqSeBuffer_Info TSIO_RelSeBuffer_Info;

#define TSIO_IOCTL_SE_CREATE            _IOWR(HI_ID_TSIO, 0x40, TSIO_CreateSe_Info)
#define TSIO_IOCTL_SE_OPEN              _IOW(HI_ID_TSIO,  0x41, HI_HANDLE)
#define TSIO_IOCTL_SE_GETATTRS          _IOWR(HI_ID_TSIO, 0x42, TSIO_GetSeAttr_Info)
#define TSIO_IOCTL_SE_SETATTRS          _IOW(HI_ID_TSIO,  0x43, TSIO_SetSeAttr_Info)
#define TSIO_IOCTL_SE_GETSTATUS         _IOWR(HI_ID_TSIO, 0x44, TSIO_GetSeStatus_Info)
#define TSIO_IOCTL_SE_ADDCHANNEL        _IOW(HI_ID_TSIO,  0x45, TSIO_AddSeChannel_Info)
#define TSIO_IOCTL_SE_DELCHANNEL        _IOW(HI_ID_TSIO,  0x46, TSIO_DelSeChannel_Info)
#define TSIO_IOCTL_SE_DELALLCHANNELS    _IOW(HI_ID_TSIO,  0x47, HI_HANDLE)
#define TSIO_IOCTL_SE_ACQBUFFER         _IOWR(HI_ID_TSIO, 0x48, TSIO_AcqSeBuffer_Info)
#define TSIO_IOCTL_SE_RELBUFFER         _IOW(HI_ID_TSIO,  0x49, TSIO_RelSeBuffer_Info)
#define TSIO_IOCTL_SE_CLOSE             _IOW(HI_ID_TSIO,  0x4a, HI_HANDLE)
#define TSIO_IOCTL_SE_DESTROY           _IOW(HI_ID_TSIO,  0x4b, HI_HANDLE)
#define TSIO_IOCTL_SE_PREMMAP           _IOWR(HI_ID_TSIO, 0x4c, TSIO_PreMmap_Info)  
#define TSIO_IOCTL_SE_PSTMMAP           _IOW(HI_ID_TSIO,  0x4d, TSIO_PstMmap_Info) 


typedef struct {
    HI_CHAR                   Cmd[MAX_CC_CMD_LEN * CC_HEX2CHR_UNIT + 4];  /* [in] 8Bytes aligned. */
    HI_U32                    CmdLen;                   /* [in] */
    HI_CHAR                   Resp[MAX_CC_CMD_LEN * CC_HEX2CHR_UNIT + 4]; /* [out] 8Bytes aligned */
    HI_U32                    RespLen;                  /* [in/out] */
    HI_U32                    TimeOut;                  /* [in] */
}TSIO_SendCCmd_Info;

#define TSIO_IOCTL_CC_SEND              _IOWR(HI_ID_TSIO, 0x50, TSIO_SendCCmd_Info)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __DRV_TSIO_IOCTL_H__