/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_demux_utils.h
* Description:  tee demux helper functions.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_DEMUX_UTILS_H__
#define __TEE_DEMUX_UTILS_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * keep synchronization with sdk/source/common/include/hi_module.h
 */
#define HI_ID_DEMUX (0x0A)

/*
 * the next definition keep similar with sdk/source/msp/api/include/hi_mpi_demux.h.
 */
#define DMX_DMXID(ChanHandle)  ( ((ChanHandle) & 0x0000ff00) >> 8)

#define DMX_CHANHANDLE_MAGIC      (0x1)
#define DMX_CHANID(ChanHandle)      ((ChanHandle) & 0x000000ff)
#define DMX_CHANHANDLE(DmxId, ChanId)    ((HI_ID_DEMUX << 28) | (DMX_CHANHANDLE_MAGIC << 24) | \
                    ((0 << 16) & 0xff0000) | ((DmxId << 8) & 0x0000ff00) | (ChanId & 0x000000ff))
                    
#define DMX_CHECK_CHANHANDLE(ChanHandle)                                \
    do                                                                  \
    {                                                                   \
        if (((ChanHandle >> 28) & 0x0000000f) != HI_ID_DEMUX || ((ChanHandle >> 24) & 0x0000000f) != DMX_CHANHANDLE_MAGIC) \
        {\
            HI_ERR_DEMUX("Invalid Channel handle(0x%x)\n", ChanHandle); \
            return HI_ERR_DMX_INVALID_PARA;\
        }\
    } while (0)

#define DMX_RECHANDLE_MAGIC      (0x3)
#define DMX_RECID(RecHandle)    ((RecHandle) & 0x000000ff)
#define DMX_RECHANDLE(DmxId, RecId)    ((HI_ID_DEMUX << 24) | (DMX_RECHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (RecId & 0x000000ff))
#define DMX_CHECK_RECHANDLE(RecHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((RecHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((RecHandle >> 16) & 0x000000ff) != DMX_RECHANDLE_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Rec Handle 0x%x\n", RecHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0) 

/*
 * the next definition keep similar with sdk/source/msp/api/include/hi_mpi_descrambler.h.
 */
#define DMX_KEYHANDLE_MAGIC      (0x5)
#define DMX_KEYID(KeyHandle)    ((KeyHandle) & 0x000000ff)
#define DMX_KEYHANDLE(DmxId, KeyId)    ((HI_ID_DEMUX << 24) | (DMX_KEYHANDLE_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (KeyId & 0x000000ff))
#define DMX_CHECK_KEYHANDLE(KeyHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((KeyHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((KeyHandle >> 16) & 0x000000ff) != DMX_KEYHANDLE_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Key Handle 0x%x\n", KeyHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)

#define DMX_KEYHANDLE2_MAGIC      (0x7)
#define DMX_KEYHANDLE2(DmxId, KeyId)    ((HI_ID_DEMUX << 24) | (DMX_KEYHANDLE2_MAGIC << 16) | ((DmxId << 8) & 0x0000ff00) | (KeyId & 0x000000ff))
#define DMX_CHECK_KEYHANDLE2(KeyHandle)                                  \
    do                                                                  \
    {                                                                   \
        if (((KeyHandle >> 24) & 0x000000ff) != HI_ID_DEMUX || ((KeyHandle >> 16) & 0x000000ff) != DMX_KEYHANDLE2_MAGIC) \
        {                                                               \
            HI_ERR_DEMUX("Invalid Encrypt Key Handle 0x%x\n", KeyHandle);          \
            return HI_ERR_DMX_INVALID_PARA;                             \
        }                                                               \
    } while (0)
    
/*
 * sync with source/msp/drv/demux/demux_v2/drv_demux_define.h
 */
typedef struct hi_Disp_Control_t
{
    HI_U32          u32DispTime;
    HI_U32          u32DispEnableFlag;       
    HI_U32          u32DispFrameDistance;   
    HI_U32          u32DistanceBeforeFirstFrame;
    HI_U32          u32GopNum;
} Disp_Control_t;

HI_S32 TEE_DMX_RegisterChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_S32 TEE_DMX_UnregisterChannel(HI_U32 DmxId, HI_U32 ChanId);

HI_S32 TEE_DMX_LockChannel(HI_U32 DmxId, HI_U32 ChanId);
HI_VOID TEE_DMX_UnlockChannel(HI_U32 DmxId, HI_U32 ChanId);

HI_S32 TEE_DMX_RegisterOq(HI_U32 DmxId, HI_U32 OqId);
HI_S32 TEE_DMX_UnregisterOq(HI_U32 DmxId, HI_U32 OqId);

HI_S32 TEE_DMX_RegisterVidSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize);
HI_S32 TEE_DMX_UnregisterVidSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr);
HI_S32 TEE_DMX_RegisterAudSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize, HI_U32 ShadowBufStartAddr, HI_U32 ShadowBufSize);
HI_S32 TEE_DMX_UnregisterAudSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr);
HI_S32 TEE_DMX_FixupAudSecBuf(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen);

HI_S32 TEE_DMX_ParserPesHeader(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen, HI_U32 *PesPktLen, HI_U32 *LastPts, HI_U8 *pstreamId);
HI_S32 TEE_DMX_ParserPesDispCtrlInfo(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, Disp_Control_t *pstDispCtrlInfo);
HI_S32 TEE_DMX_GetPesHeaderLen(HI_HANDLE Handle, HI_U32 ParserAddr, HI_U32 ParserLen, HI_U32 *PesHeaderLen);

HI_S32 TEE_DMX_RegisterRecSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr, HI_U32 BufSize);
HI_S32 TEE_DMX_UnregisterRecSecBuf(HI_HANDLE Handle, HI_U32 BufStartAddr);

HI_S32 TEE_DMX_ParserSCData(HI_HANDLE Handle, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen);
HI_S32 TEE_DMX_ParserFilterSCData(HI_HANDLE Handle, HI_U32 Pid, HI_U32 ParserOffset, HI_U32 IdxBufPhyAddr, HI_U32 IdxBufLen, HI_U32 *MonoParserLen);

HI_S32 TEE_DMX_RegisterRamPort(HI_U32 RamPortId);
HI_S32 TEE_DMX_UnregisterRamPort(HI_U32 RamPortId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_DEMUX_UTILS_H__ */
