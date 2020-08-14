/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_dmx_helper.c
* Description:  Define implement of demux helper.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/
#include "boot.h"
#include "tee_demux_utils.h"
#include "sre_syscalls_chip.h"

HI_S32 TEE_DMX_RegisterChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    return __TEE_DRV_DMX_RegisterChannel(DmxId, ChanId);
}

HI_S32 TEE_DMX_UnregisterChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    return __TEE_DRV_DMX_UnregisterChannel(DmxId, ChanId);
}

HI_S32 TEE_DMX_RegisterOq(HI_U32 DmxId, HI_U32 OqId)
{
    return __TEE_DRV_DMX_RegisterOq(DmxId, OqId);
}

HI_S32 TEE_DMX_UnregisterOq(HI_U32 DmxId, HI_U32 OqId)
{
    return __TEE_DRV_DMX_UnregisterOq(DmxId, OqId);
}

HI_S32 TEE_DMX_RegisterRamPort(HI_U32 RamPortId)
{
    return __TEE_DRV_DMX_RegisterRamPort(RamPortId);
}

HI_S32 TEE_DMX_UnregisterRamPort(HI_U32 RamPortId)
{
    return __TEE_DRV_DMX_UnregisterRamPort(RamPortId);
}

HI_S32 TEE_DMX_LockChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    return __TEE_DRV_DMX_LockChannel(DmxId, ChanId);
}

HI_VOID TEE_DMX_UnlockChannel(HI_U32 DmxId, HI_U32 ChanId)
{
    __TEE_DRV_DMX_UnlockChannel(DmxId, ChanId);
}

HI_S32 TEE_DMX_RegisterVidSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize)
{
    return __TEE_DRV_DMX_RegisterVidSecBuf(Handle, BufStartAddr, BufSize);
}

HI_S32 TEE_DMX_UnregisterVidSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr)
{
    return __TEE_DRV_DMX_UnregisterVidSecBuf(Handle, BufStartAddr);
}

HI_S32 TEE_DMX_RegisterAudSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize, HI_U32 ShadowBufStartAddr, HI_U32 ShadowBufSize)
{
    return __TEE_DRV_DMX_RegisterAudSecBuf(Handle, BufStartAddr, BufSize, ShadowBufStartAddr, ShadowBufSize);
}

HI_S32 TEE_DMX_UnregisterAudSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr)
{
    return __TEE_DRV_DMX_UnregisterAudSecBuf(Handle, BufStartAddr);
}

HI_S32 TEE_DMX_FixupAudSecBuf(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen)
{
    return __TEE_DRV_DMX_FixupAudSecBuf(Handle, ParserAddr, ParserLen);
}

HI_S32 TEE_DMX_ParserPesHeader(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen, HI_U32 *PesPktLen, HI_U32 *LastPts, HI_U8 *StreamId)
{  
    return __TEE_DRV_DMX_ParserPesHeader(Handle, ParserAddr, ParserLen, PesHeaderLen, PesPktLen, LastPts, StreamId);
}

HI_S32 TEE_DMX_ParserPesDispCtrlInfo(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, Disp_Control_t *pstDispCtrlInfo)
{  
    return __TEE_DRV_DMX_ParserPesDispCtrlInfo(Handle, ParserAddr, ParserLen, pstDispCtrlInfo);
}

HI_S32 TEE_DMX_GetPesHeaderLen(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    return __TEE_DRV_DMX_GetPesHeaderLen(Handle, ParserAddr, ParserLen, PesHeaderLen);
}

HI_S32 TEE_DMX_RegisterRecSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize)
{
    return __TEE_DRV_DMX_RegisterRecSecBuf(Handle, BufStartAddr, BufSize);
}

HI_S32 TEE_DMX_UnregisterRecSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr)
{
    return __TEE_DRV_DMX_UnregisterRecSecBuf(Handle, BufStartAddr);
}

HI_S32 TEE_DMX_ParserSCData(HI_HANDLE Handle, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen)
{
    return __TEE_DRV_DMX_ParserSCData(Handle, Pid, ParserOffset, IdxBufPhyAddr, IdxBufLen);
}

HI_S32 TEE_DMX_ParserFilterSCData(HI_HANDLE Handle, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen, HI_U32 *MonoParserLen)
{
    return __TEE_DRV_DMX_ParserFilterSCData(Handle, Pid, ParserOffset, IdxBufPhyAddr, IdxBufLen, MonoParserLen);
}

