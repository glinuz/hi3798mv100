/*******************************************************************************
*             Copyright 2016 - 2016, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_drv_tsio.h
* Description: tsio kernel export interface definition.
*
* History:
* Version   Date              Author      DefectNum      Description
* main\1    2016-08-15        l00326021     NULL            init.
*******************************************************************************/

#ifndef __HI_DRV_TSIO_H__
#define __HI_DRV_TSIO_H__

#include "hi_mpi_tsio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 HI_DRV_TSIO_GetCapability(TSIO_CAPABILITY *Cap);

struct TSIO_Session;
HI_S32 HI_DRV_TSIO_CreateSession(struct TSIO_Session **NewSession);
HI_S32 HI_DRV_TSIO_DestroySession(struct TSIO_Session *Session);

struct TSIO_Session;
HI_S32 HI_DRV_TSIO_TSI_OpenPort(TSIO_PORT Port, const TSIO_TSI_Port_Attrs *Attrs, HI_HANDLE *Handle, struct TSIO_Session *Session);
HI_S32 HI_DRV_TSIO_TSI_GetPortAttrs(HI_HANDLE Handle, TSIO_TSI_Port_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_TSI_SetPortAttrs(HI_HANDLE Handle, const TSIO_TSI_Port_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_TSI_GetPortStatus(HI_HANDLE Handle, TSIO_TSI_Port_Status *Status);
HI_S32 HI_DRV_TSIO_TSI_ClosePort(HI_HANDLE Handle);

HI_S32 HI_DRV_TSIO_RAM_OpenPort(TSIO_PORT Port, const TSIO_RAM_Port_Attrs *Attrs, HI_HANDLE *Handle, struct TSIO_Session *Session);
HI_S32 HI_DRV_TSIO_RAM_GetPortAttrs(HI_HANDLE Handle, TSIO_RAM_Port_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_RAM_SetPortAttrs(HI_HANDLE Handle, const TSIO_RAM_Port_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_RAM_GetPortStatus(HI_HANDLE Handle, TSIO_RAM_Port_Status *Status);
HI_S32 HI_DRV_TSIO_RAM_BeginBulkSeg(HI_HANDLE Handle, const HI_U8 *EvenIVKey, const HI_U8 *OddIVKey);
HI_S32 HI_DRV_TSIO_RAM_EndBulkSeg(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_RAM_GetBuffer(HI_HANDLE Handle, HI_U32 ReqLen, TSIO_BUFFER *Buf, HI_U32 TimeOut/* ms */);
HI_S32 HI_DRV_TSIO_RAM_PushBuffer(HI_HANDLE Handle, TSIO_BUFFER *Buf);
HI_S32 HI_DRV_TSIO_RAM_PutBuffer(HI_HANDLE Handle, TSIO_BUFFER *Buf);
HI_S32 HI_DRV_TSIO_RAM_ResetBuffer(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_RAM_ClosePort(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_RAM_PreMmap(HI_HANDLE Handle , HI_U32 *BufPhyAddr);
HI_S32 HI_DRV_TSIO_RAM_PstMmap(HI_HANDLE Handle , HI_VOID *BufUsrAddr);

HI_S32 HI_DRV_TSIO_PID_CreateChannel(HI_HANDLE PtHandle, HI_U32 Pid, HI_HANDLE *Handle, struct TSIO_Session *Session);
HI_S32 HI_DRV_TSIO_PID_GetChannelStatus(HI_HANDLE Handle, TSIO_Pid_Channel_Status *Status);
HI_S32 HI_DRV_TSIO_PID_DestroyChannel(HI_HANDLE Handle);

HI_S32 HI_DRV_TSIO_RAW_CreateChannel(HI_HANDLE PtHandle, HI_HANDLE *Handle, struct TSIO_Session *Session);
HI_S32 HI_DRV_TSIO_RAW_DestroyChannel(HI_HANDLE Handle);

HI_S32 HI_DRV_TSIO_SE_Create(TSIO_SID Sid, TSIO_Secure_Engine_Attrs *Attrs, HI_HANDLE *Handle, struct TSIO_Session *Session);
HI_S32 HI_DRV_TSIO_SE_Open(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_SE_GetAttrs(HI_HANDLE Handle, TSIO_Secure_Engine_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_SE_SetAttrs(HI_HANDLE Handle, const TSIO_Secure_Engine_Attrs *Attrs);
HI_S32 HI_DRV_TSIO_SE_GetStatus(HI_HANDLE Handle, TSIO_Secure_Engine_Status *Status);
HI_S32 HI_DRV_TSIO_SE_AddChannel(HI_HANDLE Handle, HI_HANDLE ChHandle);
HI_S32 HI_DRV_TSIO_SE_DelChannel(HI_HANDLE Handle, HI_HANDLE ChHandle);
HI_S32 HI_DRV_TSIO_SE_DelAllChannels(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_SE_AcquireBuffer(HI_HANDLE Handle, HI_U32 ReqLen, TSIO_BUFFER *Buf, HI_U32 TimeOut/* ms */);
HI_S32 HI_DRV_TSIO_SE_ReleaseBuffer(HI_HANDLE Handle, TSIO_BUFFER *Buf);
HI_S32 HI_DRV_TSIO_SE_Close(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_SE_Destroy(HI_HANDLE Handle);
HI_S32 HI_DRV_TSIO_SE_PreMmap(HI_HANDLE Handle , HI_U32 *BufPhyAddr);
HI_S32 HI_DRV_TSIO_SE_PstMmap(HI_HANDLE Handle , HI_VOID *BufUsrAddr);

HI_S32 HI_DRV_TSIO_CC_Send(const HI_CHAR *Cmd, HI_U32 CmdLen, HI_CHAR *Resp, HI_U32 *RespLen, HI_U32 TimeOut);

HI_S32 HI_DRV_TSIO_Suspend(HI_VOID);
HI_S32 HI_DRV_TSIO_Resume(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __HI_DRV_TSIO_H__