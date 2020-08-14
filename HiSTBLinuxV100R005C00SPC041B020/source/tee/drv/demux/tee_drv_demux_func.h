/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_drv_demux_func.h
* Description:  Define function implement.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_DRV_DEMUX_FUNC_H__
#define __TEE_DRV_DEMUX_FUNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32  DMX_OsiDescramblerCreate(HI_U32 DmxId, HI_U32 *KeyId, const HI_TEE_DMX_DESCRAMBLER_ATTR_S *DescAttr);
HI_S32  DMX_OsiDescramblerDestroy(HI_U32 DmxId, HI_U32 KeyId);
HI_S32  DMX_OsiDescramblerGetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr);
HI_S32  DMX_OsiDescramblerSetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_TEE_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr);
HI_S32  DMX_OsiDescramblerSetKey(HI_U32 DmxId, HI_U32 KeyId, HI_U32 KeyType, HI_U8 *Key);
HI_S32  DMX_OsiDescramblerSetIVKey(HI_U32 DmxId, HI_U32 KeyId, HI_U32 KeyType, HI_U8 *Key);
HI_S32  DMX_OsiDescramblerAttach(HI_U32 DmxId, HI_U32 KeyId, HI_U32 ChanId);
HI_S32  DMX_OsiDescramblerDetach(HI_U32 DmxId, HI_U32 KeyId, HI_U32 ChanId);
HI_S32  DMX_OsiDescramblerGetKeyId(HI_U32 DmxId, HI_U32 ChanId, HI_U32 *KeyId);

HI_S32  DMX_OsiScramblerCreate(HI_U32 DmxId, HI_U32 *KeyId, const HI_TEE_DMX_SCRAMBLER_ATTR_S *DescAttr);
HI_S32  DMX_OsiScramblerDestroy(HI_U32 DmxId, HI_U32 KeyId);
HI_S32  DMX_OsiScramblerGetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_TEE_DMX_SCRAMBLER_ATTR_S *pstScramblerAttr);
HI_S32  DMX_OsiScramblerSetAttr(HI_U32 DmxId, HI_U32 KeyId, HI_TEE_DMX_SCRAMBLER_ATTR_S *pstScramblerAttr);

HI_S32  DMX_OsiGetChannelId(HI_U32 DmxId, HI_U32 Pid, HI_U32 *ChanId);

HI_S32  DMX_Utils_RegisterChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32  DMX_Utils_UnregisterChannel(HI_U32 DmxId, HI_U32 ChanId);

HI_S32  DMX_Utils_RegisterOq(HI_U32 DmxId, HI_U32 OqId);
HI_S32  DMX_Utils_UnregisterOq(HI_U32 DmxId, HI_U32 OqId);

HI_S32  DMX_Utils_RegisterRamPort(HI_U32 RamPortId);
HI_S32  DMX_Utils_UnregisterRamPort(HI_U32 RamPortId);

HI_S32  DMX_Utils_LockChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_VOID DMX_Utils_UnlockChannel(HI_U32 DmxId, HI_U32 ChanId);

#ifdef DMX_SECURE_CHANNEL_SUPPORT
HI_S32  DMX_Utils_RegisterVidSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr, HI_U32 BufSize);
HI_S32  DMX_Utils_UnregisterVidSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr);
HI_S32  DMX_Utils_RegisterAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr, HI_U32 BufSize, HI_U32 ShadowBufStartAddr, HI_U32 ShadowBufSize);
HI_S32  DMX_Utils_UnregisterAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr);
HI_S32  DMX_Utils_FixupAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen);
HI_S32  DMX_Utils_ParserPesHeader(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen, HI_U32 *PesPktLen, HI_U32 *LastPts, HI_U8 *StreamId);
HI_S32  DMX_Utils_ParserPesDispCtrlInfo(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, Disp_Control_t *pstDispCtrlInfo);
HI_S32  DMX_Utils_GetPesHeaderLen(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen);
HI_S32  DMX_Utils_RegisterRecSecBuf(HI_U32 DmxId, HI_U32 RecId, HI_U32 BufStartAddr, HI_U32 BufSize);
HI_S32  DMX_Utils_UnregisterRecSecBuf(HI_U32 DmxId, HI_U32 RecId, HI_U32 BufStartAddr);
HI_S32  DMX_Utils_ParserSCData(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen);
HI_S32  DMX_Utils_ParserFilterSCData(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen, HI_U32 *MonoParserLen);
#else
static inline HI_S32  DMX_Utils_RegisterVidSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr, HI_U32 BufSize)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_UnregisterVidSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_RegisterAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr, HI_U32 BufSize, HI_U32 ShadowBufStartAddr, HI_U32 ShadowBufSize)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_UnregisterAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 BufStartAddr)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_FixupAudSecBuf(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_ParserPesHeader(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen, HI_U32 *PesPktLen, HI_U32 *LastPts, HI_U8 *StreamId)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_ParserPesDispCtrlInfo(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, Disp_Control_t *pstDispCtrlInfo)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_GetPesHeaderLen(HI_U32 DmxId, HI_U32 ChanId, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_RegisterRecSecBuf(HI_U32 DmxId, HI_U32 RecId, HI_U32 BufStartAddr, HI_U32 BufSize)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_UnregisterRecSecBuf(HI_U32 DmxId, HI_U32 RecId, HI_U32 BufStartAddr)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_ParserSCData(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen)
{
    return HI_FAILURE;
}

static inline HI_S32 DMX_Utils_ParserFilterSCData(HI_U32 DmxId, HI_U32 RecId, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen, HI_U32 *MonoParserLen)
{
    return HI_FAILURE;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_DRV_DEMUX_FUNC_H__ */