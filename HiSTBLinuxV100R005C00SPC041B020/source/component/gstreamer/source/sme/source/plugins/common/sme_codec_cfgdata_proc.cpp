/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmecodeccfgdataproc.c
 * @brief   实现各种codec cfgdata转换的函数
 * @author  liurongliang(l00180035)
 * @date    2014/4/4
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/4
 * Author : liurongliang(l00180035)
 * Desc   : Created file
 *
******************************************************************************/
#include "osal_type.h"
#include "sme_macro.h"
#include "sme_log_adpt.h"
#include "sme_codec_cfgdata_proc.h"
#include "osal_str.h"
#include "osal_mem.h"

//lint -e801 //goto, 这里可以使用goto。
//lint -e1784 //extern 'C'
//lint -e1746 //参数应该声明为const
//lint -e10 //can not see the declaration of malloc

#define GST_AVC_CFG_LEN_MIN 7
#define GST_HEVC_CFG_LEN_MIN 22

static ST_SME_LOG_CB* pstThisLogCtx = NULL;


V_BOOL SME_AvcRawCfgDataToNal(IN ST_SME_CFG_DATA stCfgData,
                        INOUT PST_AVCNALCFG_LIST pstAvcCfgSet,
                        INOUT V_UINT32* pu32Profile)
{
    V_BOOL bRet = ICS_FALSE;
    const V_UINT8* pu8Tmp = (const V_UINT8 *)stCfgData.pu8CfgData;
    V_UINT32 u32CfgDataLen = 0;
    V_INT32 idx = 0;
    V_INT32 i32SpsSetCount = 0;
    V_INT32 i32PpsSetCount = 0;
    E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;
    V_UINT32 u32CfgItemLen = 0;

    SME_LOGI_ADPT("SME_AvcRawCfgDataToNal in");

    if(NULL == pstAvcCfgSet){
        SME_LOGE_ADPT("invalid args, pstAvcCfgSet null pointer");
        return ICS_FALSE;
    }

    pstAvcCfgSet->u32Count = 0;

    if((NULL == stCfgData.pu8CfgData) || (stCfgData.u32CfgDataSize < GST_AVC_CFG_LEN_MIN)){
        SME_LOGE_ADPT("invalid args, null pointer");
        goto EXIT;
    }

    eReturn = VOS_Memset_S(pstAvcCfgSet->pstItem,
        sizeof(pstAvcCfgSet->pstItem), 0,  sizeof(pstAvcCfgSet->pstItem));
    if(E_SME_MEM_OK != eReturn)
    {
        ICS_LOGW("VOS_Memset_S pstAvcCfgSet->pstItem : new Failed(%d)", eReturn);
    }

    /* 判断最小大小及version == 1*/
    if (pu8Tmp[0] != 1)
    {
        /*这里有些parser中提供的config已经解析好了，这里直接返回成功*/
        SME_LOGE_ADPT("unsupport the data");
        goto EXIT;
    }

    if(NULL != pu32Profile){
        //profile
        *pu32Profile = pu8Tmp[1];
    }

    SME_LOGI_ADPT("avc Profile=%u", pu8Tmp[1]);

    pu8Tmp = (const V_UINT8 *)stCfgData.pu8CfgData;
    u32CfgDataLen = stCfgData.u32CfgDataSize;
    i32SpsSetCount = pu8Tmp[5] & 31;
    pu8Tmp += 6;
    u32CfgDataLen -= 6;

    if(i32SpsSetCount <= 0)
    {
        SME_LOGE_ADPT("numSeqParameterSets is 0");
        goto EXIT;
    }

    /*先处理sps*/
    for (idx = 0; idx < i32SpsSetCount; ++idx)
    {
        V_UINT32 u32SpsLen = 0;
        if (u32CfgDataLen < 2)
        {
            SME_LOGE_ADPT("sps err");
            goto EXIT;
        }

        //for fortify
        u32SpsLen = (pu8Tmp[0] << 8 | pu8Tmp[1]) & 0xFFFF;

        pu8Tmp += 2;
        u32CfgDataLen -= 2;

        if (u32CfgDataLen < u32SpsLen)
        {
            SME_LOGE_ADPT("sps len err");
            goto EXIT;
        }

        if(pstAvcCfgSet->u32Count >= M_SME_AVC_CFG_ITEM_MAX){
            SME_LOGE_ADPT("cfg count too long");
            goto EXIT;
        }
        u32CfgItemLen = (V_UINT32)(sizeof(ST_AVCNALCFG_ITEM) + u32SpsLen + SME_AVC_NAL_HEAD_LEN);
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count] =
            (PST_AVCNALCFG_ITEM)malloc(u32CfgItemLen);
        if(NULL == pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]){
            SME_LOGE_ADPT("malloc sp failed");
            goto EXIT;
        }
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[0] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[1] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[2] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[3] = 1;

        eReturn = VOS_Memcpy_S(
            &pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[SME_AVC_NAL_HEAD_LEN],
            u32SpsLen, pu8Tmp,  u32SpsLen);
        if(E_SME_MEM_OK != eReturn)
        {
            ICS_LOGW("VOS_Memcpy_S pstAvcCfgSet->pstItem : new Failed(%d)", eReturn);
        }

        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->u32Len = u32SpsLen + SME_AVC_NAL_HEAD_LEN;
        pstAvcCfgSet->u32Count++;
        SME_LOGE_ADPT("SME_AvcRawCfgDataToNal get sps,len=%d,count=%d", u32SpsLen,pstAvcCfgSet->u32Count);
        pu8Tmp += u32SpsLen;
        u32CfgDataLen -= u32SpsLen;
    }

    if (u32CfgDataLen < 1)
    {
        SME_LOGE_ADPT("u32CfgDataLen left err");
        goto EXIT;
    }

    /*处理pps的情况*/
    //for coverity
    i32PpsSetCount = (*pu8Tmp) & 0xFF;
    ++pu8Tmp;
    --u32CfgDataLen;

    for (idx = 0; idx < i32PpsSetCount; ++idx)
    {
        V_UINT32 u32PpsLen = 0;
        if (u32CfgDataLen < 2)
        {
            SME_LOGE_ADPT("sps err");
            goto EXIT;
        }

        //for fortify
        u32PpsLen = (pu8Tmp[0] << 8 | pu8Tmp[1]) & 0xFFFF;

        pu8Tmp += 2;
        u32CfgDataLen -= 2;

        if (u32CfgDataLen < u32PpsLen)
        {
            SME_LOGE_ADPT("pps len err");
            goto EXIT;
        }

        if(pstAvcCfgSet->u32Count >= M_SME_AVC_CFG_ITEM_MAX){
            SME_LOGE_ADPT("cfg count too long");
            goto EXIT;
        }

        u32CfgItemLen = (V_UINT32)(sizeof(ST_AVCNALCFG_ITEM) +
            u32PpsLen + SME_AVC_NAL_HEAD_LEN);
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count] =
            (PST_AVCNALCFG_ITEM)malloc(u32CfgItemLen);
        if(NULL == pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]){
            SME_LOGE_ADPT("malloc pps failed");
            goto EXIT;
        }
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[0] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[1] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[2] = 0;
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[3] = 1;
        eReturn = VOS_Memcpy_S(
            &pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->au8SpsOrPps[SME_AVC_NAL_HEAD_LEN],
            u32PpsLen, pu8Tmp,  u32PpsLen);
        if(E_SME_MEM_OK != eReturn)
        {
            ICS_LOGW("VOS_Memcpy_S pstAvcCfgSet->pstItem : new Failed(%d)", eReturn);
        }
        pstAvcCfgSet->pstItem[pstAvcCfgSet->u32Count]->u32Len = u32PpsLen + SME_AVC_NAL_HEAD_LEN;
        pstAvcCfgSet->u32Count++;
        SME_LOGE_ADPT("SME_AvcRawCfgDataToNal get pps,len=%d,count=%d", u32PpsLen,pstAvcCfgSet->u32Count);
        pu8Tmp += u32PpsLen;
        u32CfgDataLen -= u32PpsLen;
    }

    bRet = ICS_TRUE;

EXIT:
    if(ICS_FALSE == bRet){
        for(V_UINT32 i = 0; i < pstAvcCfgSet->u32Count; i++){
            if(pstAvcCfgSet->pstItem[i]){
                free(pstAvcCfgSet->pstItem[i]);
                pstAvcCfgSet->pstItem[i] = NULL;
            }
        }
        pstAvcCfgSet->u32Count = 0;
    }

    SME_LOGI_ADPT("SME_AvcRawCfgDataToNal out,bRet=%d, count=%d", (V_INT32)bRet, pstAvcCfgSet->u32Count);

    return bRet;
}


V_VOID SME_AddNalHeaderToAvcData(INOUT V_UINT8* pu8Data,
                        IN V_UINT32 u32Len)
{
    V_UINT32 u32Idx = 0;

    UTILS_M_RET_IF((NULL == pu8Data) || (u32Len <= SME_AVC_NAL_HEAD_LEN));

    //如果已经是NAL了不处理
    UTILS_M_RET_IF(SME_HAS_AVC_NAL(pu8Data));

    while((u32Idx + SME_AVC_NAL_HEAD_LEN ) < u32Len)
    {
        V_UINT8* pu8Temp = &pu8Data[u32Idx];
        V_UINT32 u32FrameLen = SME_GET_UINT_FOR_BGE(pu8Temp);
        UTILS_M_BRK_IF(0 == u32FrameLen);
        UTILS_MLOGW_BRK_IF((u32Idx + SME_AVC_NAL_HEAD_LEN + u32FrameLen) > u32Len,
            ("SME_AddNalHeaderToAvcData, invalid len, u32FrameLen=%d, u32Len=%d",
            u32FrameLen, u32Len));
        pu8Data[u32Idx] = 0;
        pu8Data[u32Idx + 1] = 0;
        pu8Data[u32Idx + 2] = 0;
        pu8Data[u32Idx + 3] = 1;
        u32Idx += (SME_AVC_NAL_HEAD_LEN + u32FrameLen);

    }

    return;
}


V_BOOL SME_HevcRawCfgDataToNal(IN ST_SME_CFG_DATA stCfgData,
                        INOUT PST_HEVCNALCFG_LIST pstHevcCfgSet)
{
    V_BOOL bRet = ICS_FALSE;
    const V_UINT8* pu8Tmp = (const V_UINT8 *)stCfgData.pu8CfgData;
    V_UINT32 u32CfgDataLen = 0;
    V_INT32 idx = 0;
    V_INT32 i32NalLenSize = 0;
    V_INT32 i32SetNum = 0;
    E_SME_MEM_SECURE_RET eReturn = E_SME_MEM_OK;
    V_UINT32 u32CfgItemLen = 0;

    SME_LOGE_ADPT("SME_AvcRawCfgDataToNal in");

    if(NULL == pstHevcCfgSet){
        SME_LOGE_ADPT("invalid args, pstAvcCfgSet null pointer");
        return ICS_FALSE;
    }

    pstHevcCfgSet->u32Count = 0;

    if((NULL == stCfgData.pu8CfgData) || (stCfgData.u32CfgDataSize < GST_HEVC_CFG_LEN_MIN)){
        SME_LOGE_ADPT("invalid args, null pointer");
        goto EXIT;
    }

    eReturn = VOS_Memset_S(pstHevcCfgSet->pstItem, sizeof(pstHevcCfgSet->pstItem),
        0, sizeof(pstHevcCfgSet->pstItem));
    if(E_SME_MEM_OK != eReturn)
    {
        ICS_LOGW("VOS_Memset_S pstHevcCfgSet->pstItem : new Failed(%d)", eReturn);
    }

    /* 判断最小大小及version == 1*/
    if (!pu8Tmp[0] && !pu8Tmp[1] && pu8Tmp[2] <= 1)
    {
        /*这里有些parser中提供的config已经解析好了，这里直接返回成功*/
        SME_LOGE_ADPT("unsupport the data");
        goto EXIT;
    }

    pu8Tmp = (const V_UINT8 *)(stCfgData.pu8CfgData + GST_HEVC_CFG_LEN_MIN - 1);
    u32CfgDataLen = (stCfgData.u32CfgDataSize - GST_HEVC_CFG_LEN_MIN) + 1;

    i32NalLenSize = (pu8Tmp[0] & 3) + 1;
    SME_LOGE_ADPT("hevc i32NalLenSize=%d", i32NalLenSize);

    if(u32CfgDataLen <= 2)
    {
        SME_LOGE_ADPT("left size is too small");
        goto EXIT;
    }
    i32SetNum = pu8Tmp[1];

    SME_LOGE_ADPT("hevc i32SetNum=%d", i32SetNum);
    pu8Tmp += 2;
    u32CfgDataLen -= 2;

    for (idx = 0; idx < i32SetNum; idx++)
    {
        if(u32CfgDataLen <= 3)
        {
            SME_LOGE_ADPT("left size is too small!");
            goto EXIT;
        }

        V_INT32 i32Type = pu8Tmp[0] & 0x3f;
        V_INT32 i32Cnt  = (V_INT32)(((pu8Tmp[1] << 8) | pu8Tmp[2]) & 0xFFFF);

        pu8Tmp += 3;
        u32CfgDataLen -= 3;

        for (V_INT32 j = 0; j < i32Cnt && j < M_SME_HEVC_CFG_ITEM_MAX; j++)
        {
            if(pstHevcCfgSet->u32Count >= M_SME_HEVC_CFG_ITEM_MAX)
            {
                SME_LOGE_ADPT("hevc cfg count too long");
                goto EXIT;
            }

            if(u32CfgDataLen <= 2)
            {
                SME_LOGE_ADPT("left size is too small!");
                goto EXIT;
            }

            V_INT32 i32Nalsize  = (V_INT32)(((pu8Tmp[0] << 8) | pu8Tmp[1]) & 0xFFFF);
            if(u32CfgDataLen < (V_UINT32)(2 + i32Nalsize))
            {
                SME_LOGE_ADPT("left is too small!");
                goto EXIT;
            }

            u32CfgItemLen = (V_UINT32)(sizeof(ST_HEVCNALCFG_ITEM) +
                    (V_SIZE)(i32Nalsize + SME_HEVC_NAL_HEAD_LEN));
            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count] =
                (PST_HEVCNALCFG_ITEM)malloc(u32CfgItemLen);
            if(NULL == pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count])
            {
                SME_LOGE_ADPT("malloc nal failed");
                goto EXIT;
            }

            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->au8Cfgs[0] = 0;
            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->au8Cfgs[1] = 0;
            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->au8Cfgs[2] = 0;
            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->au8Cfgs[3] = 1;
            eReturn = VOS_Memcpy_S(
                &pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->au8Cfgs[SME_HEVC_NAL_HEAD_LEN],
                (V_SIZE)i32Nalsize, &pu8Tmp[2],  (V_SIZE)i32Nalsize);
            if(E_SME_MEM_OK != eReturn)
            {
                ICS_LOGW("VOS_Memcpy_S pstHevcCfgSet->pstItem : new Failed(%d)", eReturn);
            }

            pstHevcCfgSet->pstItem[pstHevcCfgSet->u32Count]->u32Len = (V_UINT32)(i32Nalsize + SME_HEVC_NAL_HEAD_LEN);
            pstHevcCfgSet->u32Count++;
            SME_LOGE_ADPT("SME_HevcRawCfgDataToNal get nal type=%d, len=%d, count=%d", i32Type,
                i32Nalsize, pstHevcCfgSet->u32Count);
            pu8Tmp += 2 + i32Nalsize;
            u32CfgDataLen -= (V_UINT32)(2 + i32Nalsize);
        }
    }

    bRet = ICS_TRUE;

EXIT:
    if(ICS_FALSE == bRet){
        for(V_UINT32 i = 0; i < pstHevcCfgSet->u32Count; i++){
            if(pstHevcCfgSet->pstItem[i]){
                free(pstHevcCfgSet->pstItem[i]);
                pstHevcCfgSet->pstItem[i] = NULL;
            }
        }
        pstHevcCfgSet->u32Count = 0;
    }

    SME_LOGE_ADPT("SME_HevcRawCfgDataToNal out,bRet=%d, count=%d", (V_INT32)bRet, pstHevcCfgSet->u32Count);

    return bRet;
}



V_VOID SME_AddNalHeaderToHevcData(INOUT V_UINT8* pu8Data,
                        IN V_UINT32 u32Len)
{
    SME_AddNalHeaderToAvcData(pu8Data, u32Len);

    return;
}

