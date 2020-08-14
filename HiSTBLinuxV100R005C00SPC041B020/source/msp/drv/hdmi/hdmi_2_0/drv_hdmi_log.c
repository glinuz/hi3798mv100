/******************************************************************************

  Copyright (C), 2010-2026, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : drv_hdmi_log.c
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2015/6/27
 * Description   : HDMI log of debuging message printing Driver
 * History       :
 * Date          : 2015/6/27
 * Author        : l00232354
 * Modification  :
 *******************************************************************************
 */
#include "drv_hdmi_log.h"
#include "drv_hdmi_platform.h"
#include "hdmi_platform.h"

/******************************************************************************/
/* Constants                                                                  */
/******************************************************************************/
#define LOG_CHECK_CTX_NULL(pLogCtx)\
do{ \
    if (HI_NULL == pLogCtx){\
        OSAL_PRINTK("func: %s line: %d pLogCtx is NULL!!!\n", __FUNCTION__, __LINE__);\
        return;\
    } \
}while(0)

#define LOG_CHECK_CTX_VALID(pLogCtx)\
do{ \
    if (HI_NULL == pLogCtx){\
        OSAL_PRINTK("func: %s line: %d pLogCtx is NULL!!!\n", __FUNCTION__, __LINE__);\
        return HI_FAILURE;\
    } \
}while(0)

#define LOG_CHECK_RET(pfunc, ret)\
do{ \
    if (HI_SUCCESS != ret){\
        OSAL_PRINTK("call %s failed ret: %d!!!\n", pfunc, ret);\
    } \
}while(0)

/******************************************************************************/
/* Data Structures                                                            */
/******************************************************************************/

static HI_U8 g_LogCunt = 0;

/* string length is LOG_HEAD_INFO_DATA_LEN = 9 */
const HI_CHAR *g_LogFuncType[LOG_TYPE_FUNC_BUTT] = {
    "<F-ALL > ",
    "<F-API > ",
    "<F-HPD > ",
    "<F-EDID> ",
    "<F-HDCP> ",
    "<F-SCDC> ",
    "<F-CEC > ",
};

/******************************************************************************/
/* Gloabl Data                                                                */
/******************************************************************************/
LOG_CTX_S *g_pstLogCtx = HI_NULL;

/******************************************************************************/
/* Private Interface                                                          */
/******************************************************************************/
/* Get log ctx */
LOG_CTX_S *LogGetCtx(HI_VOID)
{
    if (HI_NULL != g_pstLogCtx)
    {
        if (g_pstLogCtx->bInited)
        {
            return g_pstLogCtx;
        }
    }

    return HI_NULL;
}

/* Log memory resource create */
HI_S32 LogCreate(LOG_CTX_S *pLogCtx)
{
    LOG_MEM_S *pstMem = HI_NULL;

    LOG_CHECK_CTX_VALID(pLogCtx);
    pstMem = &(pLogCtx->stMem);
    pstMem->u32MemSize = LOG_BUF_SIZE;
    pstMem->pVirAddr   = HI_NULL;

    pstMem->pVirAddr = HDMI_KMALLOC(pstMem->u32MemSize);
    if (HI_NULL == pstMem->pVirAddr)
    {
        OSAL_PRINTK("alloc memory for log failed!!\n");
        return HI_FAILURE;
    }
    OSAL_MEMSET(pstMem->pVirAddr, 0x0, pstMem->u32MemSize);

    pstMem->pPos = pstMem->pVirAddr;

    return HI_SUCCESS;
}

/* Log memory resource destroy */
HI_S32 LogDestroy(LOG_CTX_S *pLogCtx)
{
    LOG_MEM_S *pstMem = HI_NULL;

    LOG_CHECK_CTX_VALID(pLogCtx);

    /* close log file */
    if (HI_NULL != pLogCtx->pFile)
    {
        HDMI_OSAL_FileClose(pLogCtx->pFile);
        pLogCtx->pFile = HI_NULL;
    }

    /* release log memory  */
    pstMem = &(pLogCtx->stMem);
    if (HI_NULL != pstMem->pVirAddr)
    {
        HDMI_KFREE(pstMem->pVirAddr);
    }

    pstMem->u32MemSize = 0;
    pstMem->pPos       = HI_NULL;
    pstMem->pVirAddr   = HI_NULL;
    pstMem->bRollFlag  = HI_FALSE;

    return HI_SUCCESS;
}

/* Log ctx global init */
HI_VOID LogInit(HI_VOID)
{
    HI_S32 s32Ret;

    if (HI_NULL != LogGetCtx())
    {
        OSAL_PRINTK("g_pstLogCtx is inited!!!\n");
        return;
    }

    g_pstLogCtx = HDMI_KMALLOC(sizeof(LOG_CTX_S));
    LOG_CHECK_CTX_NULL(g_pstLogCtx);

    OSAL_MEMSET(g_pstLogCtx, 0x0, sizeof(LOG_CTX_S));
    OSAL_INIT_LOCK(g_pstLogCtx->stSpinLock);

    s32Ret = LogCreate(g_pstLogCtx);
    LOG_CHECK_RET("LogCreate", s32Ret);

    g_pstLogCtx->bInited = HI_TRUE;

    return;
}

/* Log ctx global deinit */
HI_VOID LogDeInit(HI_VOID)
{
    HI_S32 s32Ret;

    if (HI_NULL == LogGetCtx())
    {
        OSAL_PRINTK("g_pstLogCtx have not be inited!!!\n");
        return;
    }

    s32Ret = LogDestroy(g_pstLogCtx);
    LOG_CHECK_RET("LogDestroy", s32Ret);

    g_pstLogCtx->bInited = HI_FALSE;
    HDMI_KFREE(g_pstLogCtx);
    g_pstLogCtx = HI_NULL;

    return;
}

/* record into memory */
HI_S32 LogRecordToMem(LOG_LEVEL_E eLevel, LOG_TYPE_FUNC_E eFuncType, HI_U8 *pParam)
{
    HI_U32 len, left, right;
    HI_U8  temp[LOG_MEM_BUF_SIZE + LOG_INFO_HEAD_SIZE] = {0};
    LOG_MEM_S *pstMem = HI_NULL;
    LOG_CTX_S *pLogCtx = HI_NULL;

    if (HI_NULL == pParam)
    {
        OSAL_PRINTK("pParam = null is invalid!!!\n");
        return HI_FAILURE;
    }

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    if(HI_TRUE == pLogCtx->bReadFlag)
    {
        OSAL_PRINTK("memory of log buffer is output now!!!\n");
        return LOG_OUTPUT_NOW;
    }

    pstMem = &(pLogCtx->stMem);
    if ((HI_NULL == pstMem->pVirAddr) || (HI_NULL == pstMem->pPos))
    {
        OSAL_PRINTK("pVirAddr = %p, pPos = %p is invalid!!!\n", pstMem->pVirAddr, pstMem->pPos);
        return HI_FAILURE;
    }

    /* len do not include '\0' */
    len = OSAL_STRLEN(pParam);
    if (LOG_MEM_BUF_SIZE < len)
    {
        OSAL_PRINTK("len = %d of pParam is invalid!!!\n", len);
        return HI_FAILURE;
    }

    /* -1 means reduce LOG_INF_HEAD_SIZE and '\0' */
    left  = len + LOG_INFO_HEAD_SIZE - 1;
    right = 0;

    HDMI_OSAL_Snprintf(temp, LOG_MEM_BUF_SIZE + LOG_INFO_HEAD_SIZE,
                "%s%04d%d%04d%s%s", "##<", eFuncType, eLevel, len, ">##", pParam);
    if ((pstMem->pPos + left) >= (pstMem->pVirAddr + pstMem->u32MemSize))
    {
        left  = pstMem->pVirAddr + pstMem->u32MemSize - pstMem->pPos;
        right = (len + LOG_INFO_HEAD_SIZE - 1) - left;
    }

    OSAL_LOCK(pLogCtx->stSpinLock);

    OSAL_MEMCPY(pstMem->pPos, temp, left);
    if(right)
    {
        OSAL_MEMCPY(pstMem->pVirAddr, temp + left, right);
        pstMem->pPos = pstMem->pVirAddr + right;
        pstMem->bRollFlag = HI_TRUE;
    }
    else
    {
        pstMem->pPos += left;
    }
    *pstMem->pPos = '\0';

    OSAL_UNLOCK(pLogCtx->stSpinLock);

    return HI_SUCCESS;
}

static HI_S32 LogMode(LOG_CTX_S *pLogCtx, HI_U8 *buf_out)
{
    HI_S32 s32Ret = HI_SUCCESS;

	LOG_CHECK_CTX_VALID(pLogCtx);
	LOG_CHECK_CTX_VALID(buf_out);

    switch (pLogCtx->eMode)
    {
        case LOG_PRINT_TO_NONE:
            break;

        case LOG_PRINT_TO_ALL:
            OSAL_PRINTK("%s", buf_out);
            if(HI_NULL != pLogCtx->pFile)
            {
                s32Ret = HDMI_OSAL_FileWrite(pLogCtx->pFile, buf_out, OSAL_STRLEN(buf_out));
                if (s32Ret <= 0)
                {
                    OSAL_PRINTK("HDMI_OSAL_FileWrite ret=%d!\n", s32Ret);
                }
            }
            break;

        case LOG_PRINT_TO_SCREEN:
            OSAL_PRINTK("%s", buf_out);
            break;

        case LOG_PRINT_TO_FILE:
            if(HI_NULL != pLogCtx->pFile)
            {
                s32Ret = HDMI_OSAL_FileWrite(pLogCtx->pFile, buf_out, OSAL_STRLEN(buf_out));
                if (s32Ret <= 0)
                {
                    OSAL_PRINTK("HDMI_OSAL_FileWrite ret=%d!\n", s32Ret);
                }
            }
            break;

        default :
            HDMI_ERR("please set the print mode first...\n");
            OSAL_LOCK(pLogCtx->stSpinLock);
            pLogCtx->bReadFlag = HI_FALSE;
            OSAL_UNLOCK(pLogCtx->stSpinLock);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID LogLevelStrncpy(HI_U8 u8DbgLevel, HI_U8 *u8Buf)
{
	LOG_CHECK_CTX_NULL(u8Buf);

    switch (u8DbgLevel)
    {
        case LOG_LEVEL_INFO:
            HDMI_OSAL_Strncpy(u8Buf, "INFO : ", 7);
            break;
        case LOG_LEVEL_WARN:
            HDMI_OSAL_Strncpy(u8Buf, "WARN : ", 7);
            break;
        case LOG_LEVEL_ERROR:
            HDMI_OSAL_Strncpy(u8Buf, "ERR  : ", 7);
            break;
        case LOG_LEVEL_FATAL:
            HDMI_OSAL_Strncpy(u8Buf, "FATAL: ", 7);
            break;
        default:
            HDMI_OSAL_Strncpy(u8Buf, "UNKNOW:", 7);
            break;
    }

    return;
}

static HI_S32 LogMemIsFull(HI_U8 **pos, LOG_MEM_S *pstMem, HI_S32 *pRight, HI_U8 **pmem)
{
	LOG_CHECK_CTX_VALID(*pos);
	LOG_CHECK_CTX_VALID(pstMem);
	LOG_CHECK_CTX_VALID(pRight);
	LOG_CHECK_CTX_VALID(*pmem);

    *pos = OSAL_STRSTR(*pmem, ">##");
    if(HI_NULL == *pos)
    {
        if(*(pstMem->pVirAddr + pstMem->u32MemSize - 1) == '>' &&
           *pstMem->pVirAddr == '#' && *(pstMem->pVirAddr + 1) == '#')
        {
            *pos = pstMem->pVirAddr + pstMem->u32MemSize - 1;
        }
        else if (*(pstMem->pVirAddr + pstMem->u32MemSize - 2) == '>' &&
                 *(pstMem->pVirAddr + pstMem->u32MemSize - 1) == '#' &&
                 *pstMem->pVirAddr == '#')
        {
            *pos = pstMem->pVirAddr + pstMem->u32MemSize - 2;
        }
        else
        {
            *pRight = -1;
            *pmem = pstMem->pVirAddr;
            return HI_FAILURE;
        }
    }
    else
    {
        *pmem = *pos;
    }

    return HI_SUCCESS;
}

static HI_VOID LogJumpOver(HI_U8 *pmem, LOG_MEM_S *pstMem, HI_S32 *pRight, HI_S32 *pLeft)
{
	LOG_CHECK_CTX_NULL(pmem);
	LOG_CHECK_CTX_NULL(pstMem);
	LOG_CHECK_CTX_NULL(pRight);
	LOG_CHECK_CTX_NULL(pLeft);

    pmem += LOG_FUN_NO + LOG_HEAD_INFO_DATA_LEN;      /* jump over "##<000000000" */
    if(pmem > pstMem->pVirAddr + pstMem->u32MemSize)
    {
        pmem    = pmem - pstMem->u32MemSize;
        *pLeft -= pmem - pstMem->pVirAddr;
        *pRight = -1;
    }
    else
    {
        if(*pRight > 0)
        {
            *pRight = pstMem->pVirAddr + pstMem->u32MemSize - pmem;
        }
        else
        {
            *pLeft = pstMem->pPos - pmem;
        }
    }

    return;
}

static HI_VOID LogFunNone(HI_U8 *pmem, HI_S32 *pRight, HI_S32 *pLeft)
{
	LOG_CHECK_CTX_NULL(pmem);
	LOG_CHECK_CTX_NULL(pRight);
	LOG_CHECK_CTX_NULL(pLeft);

    pmem +=  LOG_FUN_NO;    /* jump over ">##" */
    if(*pRight > 0)
    {
        *pRight -= LOG_FUN_NO;
    }
    else
    {
        *pLeft -= LOG_FUN_NO;
    }

    return;
}

/* get strings from memroy and print it out to file or serial  */
HI_S32 LogPrintOutOfMem(HI_VOID)
{
    HI_U8 *pos = HI_NULL, *pmem = HI_NULL;
    HI_U8  u8FuncNumBuf[LOG_HEAD_INFO_DATA_WIDTH + 2] = {0}, u8StrLenBuf[LOG_HEAD_INFO_DATA_WIDTH + 1] = {0};
    HI_U32 u32FuncNum, u32StrLen = 0;
    HI_U8  u8DbgLevel;
    HI_S32 right, left, ret = 0;
    LOG_MEM_S *pstMem = HI_NULL;
    LOG_CTX_S *pLogCtx = HI_NULL;
    HI_BOOL    bConditionIsValid = HI_FALSE;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    pstMem = &(pLogCtx->stMem);
    bConditionIsValid = ((HI_NULL == pstMem->pVirAddr) || (HI_NULL == pstMem->pPos));
    if (HI_TRUE == bConditionIsValid)
    {
        OSAL_PRINTK("pVirAddr = %p, pPos = %p is invalid!!!\n", pstMem->pVirAddr, pstMem->pPos);
        return HI_FAILURE;
    }

    OSAL_LOCK(pLogCtx->stSpinLock);
    pLogCtx->bReadFlag = HI_TRUE;
    OSAL_UNLOCK(pLogCtx->stSpinLock);

    pmem = pstMem->pVirAddr;
    left = pstMem->pPos - pstMem->pVirAddr;

    /*  bRollFlag is true means the mem is full */
    if(pstMem->bRollFlag)
    {
        right = pstMem->u32MemSize - left;
        pmem  = pstMem->pPos + 1;
    }
    else
    {
        right = 0;
    }

    while(left >= 0)
    {
        /* process the information head */
        /* mem is full */
        if(right > 0)
        {

            ret = LogMemIsFull(&pos, pstMem, &right, &pmem);
            if(HI_SUCCESS != ret)
            {
                continue;
            }

            HDMI_OSAL_Strncpy(u8FuncNumBuf, pos - LOG_HEAD_INFO_DATA_LEN, LOG_HEAD_INFO_DATA_WIDTH);
            u8DbgLevel = *(pos - LOG_HEAD_INFO_DATA_LEN + LOG_HEAD_INFO_DATA_WIDTH);
            HDMI_OSAL_Strncpy(u8StrLenBuf, pos - LOG_HEAD_INFO_DATA_LEN + LOG_HEAD_INFO_DATA_WIDTH + 1,
                                    LOG_HEAD_INFO_DATA_WIDTH);
        }
        else
        {
            pos = OSAL_STRSTR(pmem, ">##");
            if(!pos)
            {
                OSAL_PRINTK("can not Find '>##' in LogMem!!!\n");
                OSAL_LOCK(pLogCtx->stSpinLock);
                pLogCtx->bReadFlag = HI_FALSE;
                OSAL_UNLOCK(pLogCtx->stSpinLock);
                return HI_FAILURE;
            }

            bConditionIsValid = (((pos - pstMem->pVirAddr) >= 0) && (pos - pstMem->pVirAddr < LOG_HEAD_INFO_DATA_LEN));
            if(HI_TRUE == bConditionIsValid)
            {
                HI_U8 temp[LOG_HEAD_INFO_DATA_LEN + 1] = {0};

                HDMI_OSAL_Strncpy(temp, pstMem->pVirAddr + pstMem->u32MemSize -
                                    (LOG_HEAD_INFO_DATA_LEN - (pos - pstMem->pVirAddr)),
                                    (LOG_HEAD_INFO_DATA_LEN - (pos - pstMem->pVirAddr)));

                temp[LOG_HEAD_INFO_DATA_LEN - (pos - pstMem->pVirAddr)] = '\0';
                HDMI_OSAL_Strncat(temp, pstMem->pVirAddr, pos - pstMem->pVirAddr);

                HDMI_OSAL_Strncpy(u8FuncNumBuf, temp, LOG_HEAD_INFO_DATA_WIDTH);
                u8DbgLevel = *(temp + LOG_HEAD_INFO_DATA_WIDTH);
                HDMI_OSAL_Strncpy(u8StrLenBuf, temp + LOG_HEAD_INFO_DATA_WIDTH + 1, LOG_HEAD_INFO_DATA_WIDTH);
            }
            else if(pos - pstMem->pVirAddr >= 0)
            {
                HDMI_OSAL_Strncpy(u8FuncNumBuf, pos - LOG_HEAD_INFO_DATA_LEN, LOG_HEAD_INFO_DATA_WIDTH);
                u8DbgLevel = *(pos - LOG_HEAD_INFO_DATA_LEN + LOG_HEAD_INFO_DATA_WIDTH);
                HDMI_OSAL_Strncpy(u8StrLenBuf, pos - LOG_HEAD_INFO_DATA_LEN + LOG_HEAD_INFO_DATA_WIDTH + 1,
                                    LOG_HEAD_INFO_DATA_WIDTH);
            }
            else
            {
                OSAL_PRINTK("wrong pos value %p, start %p\n", pos, pstMem->pVirAddr);
                return HI_FAILURE;
            }
            pmem = pos;
        }

        u32FuncNum = OSAL_SMSTRTOUL(u8FuncNumBuf, NULL, 10);
        u8DbgLevel = u8DbgLevel - '0';
        u32StrLen  = OSAL_SMSTRTOUL(u8StrLenBuf, NULL, 10);

        bConditionIsValid = (   (u32FuncNum < LOG_TYPE_FUNC_BUTT)
                             && ((u32StrLen > 0) && (u32StrLen < LOG_MEM_BUF_SIZE))
                             && (u8DbgLevel < LOG_LEVEL_BUTT));
        if (HI_TRUE == bConditionIsValid)
        {
            bConditionIsValid = (!((1 << u32FuncNum) & pLogCtx->u32Mask) && u8DbgLevel <= pLogCtx->u8Level);
            if (HI_TRUE == bConditionIsValid)
            {
                HI_U8 buf_out[LOG_MEM_BUF_SIZE + LOG_INFO_HEAD_SIZE + 8] = {0};
                LogLevelStrncpy(u8DbgLevel, buf_out);

                HDMI_OSAL_Strncat(buf_out, g_LogFuncType[u32FuncNum], LOG_HEAD_INFO_DATA_LEN);

                if(pmem + LOG_FUN_NO >= pstMem->pVirAddr + pstMem->u32MemSize)
                {
                    HDMI_OSAL_Strncat(buf_out, pmem + LOG_FUN_NO - pstMem->u32MemSize, u32StrLen);
                }
                else if(pmem + LOG_FUN_NO + u32StrLen >= pstMem->pVirAddr + pstMem->u32MemSize)
                {
                    HDMI_OSAL_Strncat(buf_out, pmem + LOG_FUN_NO, pstMem->pVirAddr + pstMem->u32MemSize
                                                             - (pmem + LOG_FUN_NO));

                    HDMI_OSAL_Strncat(buf_out, pstMem->pVirAddr, u32StrLen + pmem + LOG_FUN_NO -
                                                (pstMem->pVirAddr + pstMem->u32MemSize));
                }
                else
                {
                    HDMI_OSAL_Strncat(buf_out, pmem + LOG_FUN_NO, u32StrLen);
                }

                ret = LogMode(pLogCtx, buf_out);
                if(HI_SUCCESS != ret)
                {
                    return HI_FAILURE;
                }

            }

            if(*(pmem += LOG_FUN_NO + u32StrLen) == '\0')        /* jump over ">##......" */
            {
                OSAL_PRINTK("Come to the End ...\n");
                break;
            }

            LogJumpOver(pmem, pstMem, &right, &left);
        }
        else
        {
            LogFunNone(pmem, &right, &left);
            HDMI_ERR(">>>>>>>>>>>>>>\n");
        }
    }

    OSAL_LOCK(pLogCtx->stSpinLock);
    pLogCtx->bReadFlag = HI_FALSE;
    OSAL_UNLOCK(pLogCtx->stSpinLock);

    return HI_SUCCESS;
}

/* set log level: info¡¢warning¡¢error */
HI_S32 LogSetLevel(LOG_LEVEL_E eLevel)
{
    LOG_CTX_S *pLogCtx = HI_NULL;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    if (LOG_LEVEL_BUTT <= eLevel)
    {
        OSAL_PRINTK("eLevel: %d is invalid!!!\n", eLevel);
        return HI_FAILURE;
    }

    pLogCtx->u8Level = eLevel;

    return HI_SUCCESS;
}

/* set enable for model functions */
HI_S32 LogSetEnableFunc(LOG_TYPE_FUNC_E eFuncType)
{
    LOG_CTX_S *pLogCtx = HI_NULL;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    if (LOG_TYPE_FUNC_BUTT <= eFuncType)
    {
        OSAL_PRINTK("eFunc: %d is invalid!!!\n", eFuncType);
        return HI_FAILURE;
    }
    if (LOG_TYPE_FUNC_ALL == eFuncType)
    {
        pLogCtx->u32Mask = 0;
    }
    else
    {
        pLogCtx->u32Mask &= ~(1 << eFuncType);
    }

    return HI_SUCCESS;
}

/* set mask for model functions */
HI_S32 LogSetMaskFunc(LOG_TYPE_FUNC_E eFuncType)
{
    LOG_CTX_S *pLogCtx = HI_NULL;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    if (LOG_TYPE_FUNC_BUTT <= eFuncType)
    {
        OSAL_PRINTK("eFunc: %d is invalid!!!\n", eFuncType);
        return HI_FAILURE;
    }
    pLogCtx->u32Mask |= (1 << eFuncType);

    return HI_SUCCESS;

}

/* clear the log memory */
HI_S32 LogClearMem(HI_VOID)
{
    LOG_MEM_S *pstMem = HI_NULL;
    LOG_CTX_S *pLogCtx = HI_NULL;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    pstMem = &(pLogCtx->stMem);
    if (HI_NULL != pstMem->pVirAddr)
    {
        OSAL_MEMSET(pstMem->pVirAddr, 0x0, pstMem->u32MemSize);
    }
    else
    {
        OSAL_PRINTK("pu8LogVirAddr is null!!!\n");
    }

    return HI_SUCCESS;
}

/* record into memory */
HI_VOID LogRecord(LOG_LEVEL_E eLevel, LOG_TYPE_FUNC_E eFuncType, const HI_CHAR *fmt, ...)
{
    HI_U8  u8Buf[LOG_MEM_BUF_SIZE] = {0};
    va_list argp = {0};
    LOG_MEM_S *pstMem = HI_NULL;
    LOG_CTX_S *pLogCtx = HI_NULL;
    LOG_LEVEL_E eTempLevel = LOG_LEVEL_INFO;
    LOG_TYPE_FUNC_E eTempFuncType = LOG_TYPE_FUNC_API;
    HI_S32 s32Ret = HI_FAILURE;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_NULL(pLogCtx);

    pstMem = &(pLogCtx->stMem);
    if ((HI_NULL == pstMem->pVirAddr) || (HI_NULL == pstMem->pPos))
    {
        OSAL_PRINTK("pVirAddr = %p, pPos = %p is invalid!!!\n", pstMem->pVirAddr, pstMem->pPos);
        return;
    }

    eTempLevel    = (LOG_LEVEL_BUTT <= eLevel)?  LOG_LEVEL_INFO: eLevel;
    eTempFuncType = (LOG_TYPE_FUNC_BUTT <= eFuncType)?  LOG_TYPE_FUNC_API: eFuncType;

    HDMI_OSAL_Snprintf(u8Buf, 14, "[%010lu] ", HDMI_OSAL_GetTimeInMs());
    va_start(argp, fmt);
    HDMI_OSAL_Vsnprintf(u8Buf + 13, LOG_MEM_BUF_SIZE - 13, fmt, argp);
    va_end(argp);

    s32Ret = LogRecordToMem(eTempLevel, eTempFuncType, u8Buf);
    if (LOG_OUTPUT_NOW != s32Ret)
    {
        LOG_CHECK_RET("LogRecordToMem", s32Ret);
    }

    return;
}

/* cat out of memory */
HI_VOID LogCat(HI_VOID)
{
    HI_S8 *pLogStart = "\n****************************** Log Start ******************************\n\n";
    HI_S8 *pLogEnd   = "\n****************************** Log End   ******************************\n\n";
    HI_S32 s32Ret    = HI_FAILURE;
    LOG_CTX_S *pLogCtx = HI_NULL;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_NULL(pLogCtx);

    if (LOG_PRINT_TO_NONE == pLogCtx->eMode || LOG_PRINT_TO_BUTT <= pLogCtx->eMode)
    {
        OSAL_PRINTK("Mode: %d\nPlease set the right output mode...\n", pLogCtx->eMode);
        return;
    }

    if ((HI_NULL != pLogCtx->pFile) &&
        ((LOG_PRINT_TO_FILE == pLogCtx->eMode) || (LOG_PRINT_TO_ALL == pLogCtx->eMode)))
    {
        HDMI_OSAL_FileWrite(pLogCtx->pFile, pLogStart, OSAL_STRLEN(pLogStart));
    }

    s32Ret = LogPrintOutOfMem();
    if (HI_SUCCESS != s32Ret)
    {
        OSAL_PRINTK("call func:LogPrintOutOfMem failed!!\n");
    }

    if ((HI_NULL != pLogCtx->pFile) &&
        ((LOG_PRINT_TO_FILE == pLogCtx->eMode) || (LOG_PRINT_TO_ALL == pLogCtx->eMode)))
    {
        HDMI_OSAL_FileWrite(pLogCtx->pFile, pLogEnd, OSAL_STRLEN(pLogEnd));
    }

    return;
}

/* set eLogType, file ,serial and file&serial */
HI_S32 LogSetMode(LOG_PRINT_MODE_E eMode, HI_CHAR *pFilePath)
{
    LOG_CTX_S *pLogCtx = HI_NULL;
    HI_CHAR LogFile[LOG_FILEE_NAME_SIZE] = {0};

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_VALID(pLogCtx);

    switch (eMode)
    {
        case LOG_PRINT_TO_SCREEN:
            break;
        case LOG_PRINT_TO_FILE:
        case LOG_PRINT_TO_ALL:
            if (HI_NULL != pFilePath)
            {
                if(!(OSAL_STRLEN(pFilePath) < LOG_FILEE_NAME_SIZE))
                {
                    HDMI_ERR("file path > 128 is too long\n");
                    return HI_FAILURE;
                }
                HDMI_OSAL_Snprintf(LogFile, LOG_FILEE_NAME_SIZE, "%s", pFilePath);
            }
            else
            {
                OSAL_PRINTK("Please set a right file path and name!!!\n");
                return HI_FAILURE;
            }

            if (HI_NULL == pLogCtx->pFile)
            {
                pLogCtx->pFile = HDMI_OSAL_FileOpen(LogFile, OSAL_RDWR);
                if (HI_NULL == pLogCtx->pFile)
                {
                    OSAL_PRINTK("can not open the log file: %s ...\n", LogFile);
                    return HI_FAILURE;
                }
            }
            break;
        default:
            OSAL_PRINTK("eMode: %d is invalid!!!, please set right mode...\n", eMode);
            return HI_FAILURE;
    }

    pLogCtx->eMode = eMode;
    g_LogCunt++;

    return HI_SUCCESS;
}


/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/
HI_VOID DRV_HDMI_LogInit(HI_VOID)
{
    LogInit();
    return;
}

HI_VOID DRV_HDMI_LogDeInit(HI_VOID)
{
    LogDeInit();
    return;
}

/* record into memory */
HI_VOID DRV_HDMI_LogRecord(LOG_LEVEL_E eLevel, LOG_TYPE_FUNC_E eFuncType, const HI_CHAR *fmt, ...)
{
    HI_U8  u8Buf[LOG_MEM_BUF_SIZE] = {0};
    va_list argp = {0};
    LOG_MEM_S *pstMem = HI_NULL;
    LOG_CTX_S *pLogCtx = HI_NULL;
    LOG_LEVEL_E eTempLevel = LOG_LEVEL_INFO;
    LOG_TYPE_FUNC_E eTempFuncType = LOG_TYPE_FUNC_API;
    HI_S32 s32Ret = HI_FAILURE;

    pLogCtx = LogGetCtx();
    LOG_CHECK_CTX_NULL(pLogCtx);

    pstMem = &(pLogCtx->stMem);
    if ((HI_NULL == pstMem->pVirAddr) || (HI_NULL == pstMem->pPos))
    {
        OSAL_PRINTK("pVirAddr = %p, pPos = %p is invalid!!!\n", pstMem->pVirAddr, pstMem->pPos);
        return;
    }

    eTempLevel    = (LOG_LEVEL_BUTT <= eLevel)?  LOG_LEVEL_INFO: eLevel;
    eTempFuncType = (LOG_TYPE_FUNC_BUTT <= eFuncType)?  LOG_TYPE_FUNC_API: eFuncType;

    HDMI_OSAL_Snprintf(u8Buf, 14, "[%010lu] ", HDMI_OSAL_GetTimeInMs());
    va_start(argp, fmt);
    HDMI_OSAL_Vsnprintf(u8Buf + 13, LOG_MEM_BUF_SIZE - 13, fmt, argp);
    va_end(argp);

    s32Ret = LogRecordToMem(eTempLevel, eTempFuncType, u8Buf);
    if (LOG_OUTPUT_NOW != s32Ret)
    {
        LOG_CHECK_RET("LogRecordToMem", s32Ret);
    }

    return;

}

/* cat out of memory */
HI_S32 DRV_HDMI_LogCat(LOG_PRINT_MODE_E eMode, HI_CHAR *pFilePath)
{
    HI_S32 s32Ret;

    s32Ret = LogSetMode(eMode, pFilePath);
    if (HI_SUCCESS != s32Ret)
    {
        OSAL_PRINTK("Call func LogSetMode failed s32Ret: %d!!!\n", s32Ret);
        return HI_FAILURE;
    }
    LogCat();

    return HI_SUCCESS;
}

/* set print level */
HI_S32 DRV_HDMI_LogSetLevel(LOG_LEVEL_E eLevel)
{
    HI_S32 s32Ret;

    s32Ret = LogSetLevel(eLevel);
    if (HI_SUCCESS != s32Ret)
    {
        OSAL_PRINTK("Call func LogSetLevel failed s32Ret: %d!!!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/* set mask level */
HI_S32 DRV_HDMI_LogSetMask(LOG_TYPE_FUNC_E eFuncType)
{
    HI_S32 s32Ret;

    s32Ret = LogSetMaskFunc(eFuncType);
    if (HI_SUCCESS != s32Ret)
    {
        OSAL_PRINTK("Call func LogSetMaskFunc failed s32Ret: %d!!!\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


