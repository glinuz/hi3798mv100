/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  sample_teletext_port.c
* Description:  Define functions used to porting of teletext
*
* History:
* Version      Date             Author     DefectNum    Description
* main\1    2012-03-012                      NULL      Create this file.
***********************************************************************************/
#include "string.h"
#include "stdlib.h"

#include "hi_unf_demux.h"

#include "sample_ttx_port.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif  /* __cplusplus */

#define MAX_CHANNEL_BUF_SIZE (64 * 1024)

HI_S32 TTX_Port_DmxInstall(TTX_PORT_S* pstTtxParam)
{
    HI_UNF_DMX_CHAN_ATTR_S tChAttr = {0};
    HI_UNF_DMX_FILTER_ATTR_S tFilterAttr = {0};
    HI_S32 s32Ret = 0;

    s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&tChAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    tChAttr.u32BufSize = MAX_CHANNEL_BUF_SIZE;
    tChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
    tChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
    tChAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

    s32Ret = HI_UNF_DMX_CreateChannel(pstTtxParam->u16DmxID, &tChAttr, &pstTtxParam->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    /* set channel PID for recving data */
    s32Ret = HI_UNF_DMX_SetChannelPID(pstTtxParam->hChannelID, pstTtxParam->u16PID);
    if (s32Ret != HI_SUCCESS)
    {
        s32Ret = HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);

        if (s32Ret != HI_SUCCESS)
        {}

        return HI_FAILURE;
    }

    memset(tFilterAttr.au8Match, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Mask, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Negate, 0, DMX_FILTER_MAX_DEPTH);
    tFilterAttr.u32FilterDepth = 1;
    tFilterAttr.au8Match[0] = 0xbd; /* Teletext stream id is 0xbd */
    tFilterAttr.au8Mask[0] = 0xff;

    s32Ret = HI_UNF_DMX_CreateFilter(pstTtxParam->u16DmxID, &tFilterAttr, &pstTtxParam->hFilterID);
    if (s32Ret != HI_SUCCESS)
    {
        s32Ret = HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);

        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_SetFilterAttr(pstTtxParam->hFilterID, &tFilterAttr);
    if (s32Ret != HI_SUCCESS)
    {
        s32Ret = HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);

        s32Ret = HI_UNF_DMX_DestroyFilter(pstTtxParam->hFilterID);

        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_AttachFilter(pstTtxParam->hFilterID, pstTtxParam->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        s32Ret = HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);

        s32Ret = HI_UNF_DMX_DestroyFilter(pstTtxParam->hFilterID);

        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(pstTtxParam->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        s32Ret = HI_UNF_DMX_DetachFilter(pstTtxParam->hFilterID, pstTtxParam->hChannelID);

        s32Ret = HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);

        s32Ret = HI_UNF_DMX_DestroyFilter(pstTtxParam->hFilterID);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 TTX_Port_DmxUninstall(TTX_PORT_S* pstTtxParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (NULL != pstTtxParam)
    {
        s32Ret &= HI_UNF_DMX_CloseChannel(pstTtxParam->hChannelID);
        s32Ret &= HI_UNF_DMX_DetachFilter(pstTtxParam->hFilterID, pstTtxParam->hChannelID);
        s32Ret &= HI_UNF_DMX_DestroyFilter(pstTtxParam->hFilterID);
        s32Ret &= HI_UNF_DMX_DestroyChannel(pstTtxParam->hChannelID);
    }

    return s32Ret;
}

HI_S32 TTX_Port_DmxChangePid(HI_HANDLE hChannelID, HI_U16 u16Pid)
{
    HI_S32 s32Ret = 0;

    if (u16Pid == 0xFFFF)
    {
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_CloseChannel(hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = HI_UNF_DMX_SetChannelPID(hChannelID, u16Pid);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 TTX_Port_CloseChannel(HI_HANDLE hChannelID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (hChannelID == 0)
    {
        return HI_FAILURE;
    }

    s32Ret &= HI_UNF_DMX_CloseChannel(hChannelID);

    return s32Ret;
}

HI_S32 TTX_Port_AcquireBuf(HI_HANDLE hChannelID, HI_U8 *pu8Data, HI_U32 *pu32DataLength)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_DMX_DATA_S stSection[5] = { {0} };
    HI_U32 u32SectionNum = 0;

    if (HI_SUCCESS != HI_UNF_DMX_AcquireBuf(hChannelID, 1, &u32SectionNum, stSection, 3000))
    {
        return HI_FAILURE;
    }

    if (u32SectionNum == 0)
    {
        return HI_FAILURE;
    }

    memcpy(pu8Data, stSection[0].pu8Data, stSection[0].u32Size);

    *pu32DataLength = stSection[0].u32Size;

    s32Ret &= HI_UNF_DMX_ReleaseBuf(hChannelID, u32SectionNum, stSection);
    return s32Ret;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */
