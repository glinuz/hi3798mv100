/***********************************************************
 *                      Copyright    , 2009-2014, Hisilicon Tech. Co., Ltd.
 *                                   ALL RIGHTS RESERVED
 * FileName:  upgrd_usb.c
 * Description: usb upgrade mode
 *
 * History:
 * Version   Date           Author            DefectNum      Description
 * main\1    2009-2-12    chenling102556      NULL          Create this file.
 ************************************************************/
#include "com_osal.h"
#include "com_debug.h"
#include "com_buffer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct tagCOM_MODULE_S
{
    HI_U32          u32ModuleID;                    /** Identifier for a module */
    HI_U32          u32ModuleSize;                  /** Size of a module */
    HI_U32          u32BlockCount;                  /* 整个Module中需要接收Block的个数*/
    HI_U8*          pu8BlockReceiveFlag;            /* Block 接收状态标志*/

    HI_U32          u32ReceiveStatus;               /* 接收状态 0:未开始接收;1:正在接收;2:接收完毕;*/
    HI_U32          u32ReceiveStart;                /* 本次接收数据的起始datagram number*/
    HI_U32          u32ReceiveCount;                /* 本次要接收数据的datagram 的数目*/
    HI_U32          u32ReceivedCount;               /* 本次已经接收数据的datagram 的数目*/

    HI_U32          u32CachedSize;                  /** Cached data size */

    HI_U32          u32MemSize;                     /* size of allocated memory */
    HI_U8*          pu8MemAddr;

} COM_MODULE_S;

typedef struct tagCOM_BUFFER_INFO_S
{
    HI_U32          u32MemSize;
    HI_U8*          pu8MemBuff;
    HI_U32          u32BlockSize;

    HI_U32          u32ModuleNum;
    COM_MODULE_S    astModuleBuf[COM_CFG_D_CNT_MODULE];

    HI_U32          u32ReceivedSize;
} COM_BUFFER_INFO_S;

static COM_BUFFER_INFO_S s_stBufferIns;

static HI_S32 bufferReallocate(COM_BUFFER_INFO_S* pstBufferIns)
{
    HI_U8  i = 0;
    HI_U32 u32ExpectMem = 0;
    HI_U32 u32RemainMem = 0;
    HI_U32 u32MemOffset = 0;
    HI_U32 u32RemainCount = 0;
    COM_MODULE_S* pstModuleIns = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    for (i = 0; i < pstBufferIns->u32ModuleNum; i++)
    {
        pstModuleIns = &pstBufferIns->astModuleBuf[i];

        u32RemainMem = pstBufferIns->u32MemSize - u32MemOffset;

        if (u32RemainMem < pstBufferIns->u32BlockSize)
        {
            break;
        }

        if (2 != pstModuleIns->u32ReceiveStatus)
        {
            pstModuleIns->u32ReceiveStart += pstModuleIns->u32ReceivedCount;
            u32RemainCount = pstModuleIns->u32BlockCount - pstModuleIns->u32ReceiveStart;
            u32ExpectMem   = pstModuleIns->u32ModuleSize - pstModuleIns->u32ReceiveStart * pstBufferIns->u32BlockSize;

            if (u32RemainMem >= u32ExpectMem)
            {
                pstModuleIns->u32ReceiveCount = u32RemainCount;
                pstModuleIns->u32MemSize = u32ExpectMem;
            }
            else
            {
                pstModuleIns->u32ReceiveCount = u32RemainMem / pstBufferIns->u32BlockSize;
                pstModuleIns->u32MemSize = pstModuleIns->u32ReceiveCount * pstBufferIns->u32BlockSize;
            }

            pstModuleIns->pu8MemAddr = pstBufferIns->pu8MemBuff + u32MemOffset;
            pstModuleIns->u32ReceiveStatus = 1;
            pstModuleIns->u32CachedSize    = 0;
            pstModuleIns->u32ReceivedCount = 0;
            u32MemOffset += pstModuleIns->u32MemSize;
        }

    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 bufferModuleInit(COM_MODULE_S* pstModuleIns, COM_BUFFER_MODULEINFO_S* pstModuleInfo, HI_U32 u32BlockSize)
{
    HI_U32  u32BlockCount = 0;
    HI_U8*  pu8BlockReceiveFlag = HI_NULL_PTR;

    COM_DBG_FuncEnter();


    u32BlockCount = pstModuleInfo->u32ModuleSize / u32BlockSize;

    if (0 != pstModuleInfo->u32ModuleSize % u32BlockSize)
    {
        u32BlockCount++;
    }

    pu8BlockReceiveFlag = COM_MemMalloc(u32BlockCount);
    if (HI_NULL_PTR == pu8BlockReceiveFlag)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8BlockReceiveFlag, 0x00, u32BlockCount);
    pstModuleIns->u32ModuleID   = pstModuleInfo->u32ModuleID;
    pstModuleIns->u32ModuleSize = pstModuleInfo->u32ModuleSize;
    pstModuleIns->u32BlockCount = u32BlockCount;
    pstModuleIns->pu8BlockReceiveFlag = pu8BlockReceiveFlag;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 bufferModuleDeInit(COM_MODULE_S* pstModuleIns)
{
    COM_DBG_FuncEnter();

    COM_MemFreeSetNull(pstModuleIns->pu8BlockReceiveFlag);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 bufferModulesDeInit(COM_MODULE_S* astModuleBuf, HI_U32 u32ModuleNum)
{
    HI_U8 i = 0;

    COM_DBG_FuncEnter();

    for (i = 0; i < u32ModuleNum; i++)
    {
        COM_CHECK(bufferModuleDeInit(&astModuleBuf[i]));
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 COM_BUFFER_Init(COM_BUFFER_INITPARAM_S* pstInitParam)
{
    HI_U8 i = 0;
    HI_U32 u32BlockSize = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstInitParam);
    COM_CHECK_PARAM(HI_NULL_PTR == pstInitParam->pu8MemBuff);

    COM_CHECK_PARAM(0 == pstInitParam->u32MemSize);
    COM_CHECK_PARAM(0 == pstInitParam->u32BlockSize);
    COM_CHECK_PARAM(0 == pstInitParam->u32ModuleNum);

    if (COM_CFG_D_CNT_MODULE < pstInitParam->u32ModuleNum)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    u32BlockSize = pstInitParam->u32BlockSize;

    for (i = 0; i < pstInitParam->u32ModuleNum; i++)
    {
        if (u32BlockSize > pstInitParam->astModuleBuf[i].u32ModuleSize)
        {
            u32BlockSize = pstInitParam->astModuleBuf[i].u32ModuleSize;
        }
    }

    COM_CHECK_PARAM(0 == u32BlockSize);

    COM_MemSet(&s_stBufferIns, 0x00, sizeof(s_stBufferIns));
    s_stBufferIns.u32BlockSize = u32BlockSize;
    s_stBufferIns.u32ModuleNum = pstInitParam->u32ModuleNum;
    s_stBufferIns.pu8MemBuff   = pstInitParam->pu8MemBuff;
    s_stBufferIns.u32MemSize   = pstInitParam->u32MemSize;

    for (i = 0; i < pstInitParam->u32ModuleNum; i++)
    {
        s32Ret = bufferModuleInit(&s_stBufferIns.astModuleBuf[i], &pstInitParam->astModuleBuf[i], u32BlockSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(bufferModuleInit, s32Ret);
            COM_CHECK(bufferModulesDeInit(s_stBufferIns.astModuleBuf, i));
            return s32Ret;
        }
    }

    s32Ret = bufferReallocate(&s_stBufferIns);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(bufferReallocate, s32Ret);
        COM_CHECK(bufferModulesDeInit(s_stBufferIns.astModuleBuf, s_stBufferIns.u32ModuleNum));
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 COM_BUFFER_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK(bufferModulesDeInit(s_stBufferIns.astModuleBuf, s_stBufferIns.u32ModuleNum));

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 COM_BUFFER_PutData
(
    HI_U32          u32ModuleIndex,
    HI_U32          u32BlockIndex,
    const HI_U8*    pu8BlockData,
    HI_U32          u32BlockDataLen
)
{
    HI_U32 u32Offset = 0;
    HI_U32 u32MemOffset = 0;
    COM_MODULE_S* pstModuleIns = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8BlockData);
    COM_CHECK_PARAM(0 == u32BlockDataLen);

    if (u32ModuleIndex >= s_stBufferIns.u32ModuleNum)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    pstModuleIns = &(s_stBufferIns.astModuleBuf[u32ModuleIndex]);

    if (u32BlockIndex >= pstModuleIns->u32BlockCount)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    if (1 != pstModuleIns->u32ReceiveStatus)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (1 <= pstModuleIns->pu8BlockReceiveFlag[u32BlockIndex])
    {
        pstModuleIns->pu8BlockReceiveFlag[u32BlockIndex]++;

        if (COM_CFG_D_UPGRADE_TIMES < pstModuleIns->pu8BlockReceiveFlag[u32BlockIndex])
        {
            COM_PrintErrCode(COM_ERR_DOWNLOAD_TIMEOUT);
            return COM_ERR_DOWNLOAD_TIMEOUT;
        }

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if ((u32BlockIndex < pstModuleIns->u32ReceiveStart)
        || (u32BlockIndex >= pstModuleIns->u32ReceiveStart + pstModuleIns->u32ReceiveCount))
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    u32Offset = u32BlockIndex * s_stBufferIns.u32BlockSize;

    if (u32Offset + u32BlockDataLen > pstModuleIns->u32ModuleSize)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    u32MemOffset = (u32BlockIndex - pstModuleIns->u32ReceiveStart) * s_stBufferIns.u32BlockSize;
    COM_MemCpy(&(pstModuleIns->pu8MemAddr[u32MemOffset]), pu8BlockData, u32BlockDataLen);

    pstModuleIns->pu8BlockReceiveFlag[u32BlockIndex] = 1;
    pstModuleIns->u32ReceivedCount++;

    if (pstModuleIns->u32ReceivedCount + pstModuleIns->u32ReceiveStart == pstModuleIns->u32BlockCount)
    {
        pstModuleIns->u32ReceiveStatus = 2;
    }

    pstModuleIns->u32CachedSize += u32BlockDataLen;
    s_stBufferIns.u32ReceivedSize += u32BlockDataLen;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 COM_BUFFER_GetData(COM_BUFFER_DATABLOCK_S* pstData, HI_U32 u32DataLen, HI_U32* pu32DataLen)
{
    HI_U8 i = 0;
    HI_U32 u32Number = 0;
    COM_BUFFER_STATUS_E enStatus = COM_BUFFER_STATUS_BUTT;
    COM_MODULE_S* pstModuleIns = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstData);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataLen);

    s32Ret = COM_BUFFER_GetStatus(&enStatus);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_BUFFER_GetStatus, s32Ret);
        return s32Ret;
    }

    if ((COM_BUFFER_E_STATUS_NOMEM != enStatus)
        && (COM_BUFFER_E_STATUS_COMPLETE != enStatus))
    {
        COM_PrintErrCode(COM_ERR_STATUS);
        return COM_ERR_STATUS;
    }

    for (i = 0; i < s_stBufferIns.u32ModuleNum; i++)
    {
        pstModuleIns = &s_stBufferIns.astModuleBuf[i];

        if (0 == pstModuleIns->u32CachedSize)
        {
            continue;
        }

        if (0 != pstModuleIns->u32ReceiveStatus)
        {
            pstData[u32Number].u32ModuleID = pstModuleIns->u32ModuleID;
            pstData[u32Number].pu8DataBuff = pstModuleIns->pu8MemAddr;
            pstData[u32Number].u32DataSize = pstModuleIns->u32CachedSize;

            if (1 == pstModuleIns->u32ReceiveStatus)
            {
                pstData[u32Number].bLastData = HI_FALSE;
            }
            else
            {
                pstData[u32Number].bLastData = HI_TRUE;
            }

            pstModuleIns->u32CachedSize = 0;

            u32Number++;
        }
    }

    if (0 < u32Number)
    {
        if (COM_BUFFER_E_STATUS_NOMEM == enStatus)
        {
            s32Ret = bufferReallocate(&s_stBufferIns);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(bufferReallocate, s32Ret);
                return s32Ret;
            }
        }
    }

    *pu32DataLen = u32Number;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_U32 COM_BUFFER_GetSize(HI_VOID)
{
    return s_stBufferIns.u32ReceivedSize;
}

HI_U32 COM_BUFFER_GetBlockSize(HI_VOID)
{
    return s_stBufferIns.u32BlockSize;
}

HI_S32 COM_BUFFER_GetStatus(COM_BUFFER_STATUS_E* penStatus)
{
    HI_U8 i = 0;
    COM_BUFFER_STATUS_E enStatus = COM_BUFFER_E_STATUS_COMPLETE;
    COM_MODULE_S* pstModuleIns = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == penStatus);

    for (i = 0; i < s_stBufferIns.u32ModuleNum; i++)
    {
        pstModuleIns = &s_stBufferIns.astModuleBuf[i];

        if (1 == pstModuleIns->u32ReceiveStatus)
        {
            if (pstModuleIns->u32ReceivedCount < pstModuleIns->u32ReceiveCount)
            {
                enStatus = COM_BUFFER_E_STATUS_NORMAL;
                break;
            }

            enStatus = COM_BUFFER_E_STATUS_NOMEM;
        }
        else if (0 == pstModuleIns->u32ReceiveStatus)
        {
            enStatus = COM_BUFFER_E_STATUS_NOMEM;
        }
    }

    *penStatus = enStatus;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

