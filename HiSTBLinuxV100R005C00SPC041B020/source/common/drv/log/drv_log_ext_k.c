/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_log_ext_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/02/29
  Description   :
  History       :
  1.Date        : 2008/02/29
    Author      : c42025
    Modification: Created file, we can adjust our log level as do as HI_PRINT, eg
    command line in shell :
    [pub@hi3511mpp]$echo "venc = 2" > /proc/msp/log
    will change log levle of module "VENC" to 2, then, all message with greate
    levle than "2" will be suppressed.
  2.Date        : 2009/12/17
    Author      : jianglei
    Modification:   add control of output mode, output log to network through read from log buffer
                    CNcomment:增加输出方式控制,通过增加log buffer,可以通过读取log输出到网络

******************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include "hi_kernel_adapt.h"

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_osal.h"
#include "hi_drv_sys.h"
#include "hi_drv_log.h"
#include "hi_drv_proc.h"
#include "drv_log.h"
#include "hi_drv_mmz.h"
#include "drv_log_ioctl.h"
#include "hi_drv_file.h"


MMZ_BUFFER_S   g_LogConfigBuf;
static HI_U32  g_LogModInit = 0;
static LOG_CONFIG_INFO_S *g_pLogConfigInfo; /*kernel-state pointer of log control info*//*CNcomment:打印控制信息的内核态指针*/

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
static LOG_BUFFER_INFO_S  g_MsgBufInfo;
#endif

HI_DECLARE_MUTEX(g_LogMutex);

#define LOG_FILE_LOCK()   down_interruptible(&g_LogMutex)
#define LOG_FILE_UNLOCK() up(&g_LogMutex)

#define MAX_FILENAME_LENTH 256

/* this variable will be used by /kmod/load script */
static int LogBufSize = 256 * DEBUG_MSG_BUF_RESERVE;

static char g_szPathBuf[MAX_FILENAME_LENTH] = {0};
static char *UdiskLogFile = g_szPathBuf;
static HI_BOOL g_bSetLogFileFlag = HI_FALSE;
static char g_szStorePathBuf[MAX_FILENAME_LENTH] = "/mnt";
char *StorePath = g_szStorePathBuf;

char *DebugLevelName[HI_LOG_LEVEL_BUTT+1] = {
    "FATAL" ,
    "ERROR" ,
    "WARN"  ,
    "INFO"  ,
    "DEBUG" ,
    "BUTT"
};

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
static HI_VOID LOGBufferReset(HI_VOID)
{
    unsigned long flags;

    local_irq_save(flags);
    g_MsgBufInfo.u32ReadAddr = g_MsgBufInfo.u32WriteAddr;
    g_MsgBufInfo.u32ResetFlag++;
    local_irq_restore((unsigned long)flags);
}

HI_S32 HI_DRV_LOG_BufferRead(HI_U8 *Buf,  HI_U32 BufLen, HI_U32 *pCopyLen, HI_BOOL bKernelCopy)
{
    LOG_BUFFER_INFO_S MsgInfoNow;
    HI_U32 BufUsed;
    HI_U32 DataLen1;
    HI_U32 DataLen2;
    HI_U32 CopyLen;
    HI_U32 NewReadAddr;
    unsigned long flags;

    if (0 == g_MsgBufInfo.u32BufSize)
    {
        HI_ERR_LOG("Log Buffer size is 0, Please confige the Buffer size,for example:");
        HI_ERR_LOG("    Config buffer size 500K: insmod hi_cmpi.ko LogBufSize=0x80000");
        return HI_FAILURE;
    }

    if(g_MsgBufInfo.u32WriteAddr == g_MsgBufInfo.u32ReadAddr)
    {
        if (g_bSetLogFileFlag == HI_TRUE)
        {
            return HI_FAILURE;
        }
        else
        {
            /* the following code segment will pending when reboot or reload ko */
            wait_event_interruptible(g_MsgBufInfo.wqNoData,
                    (g_MsgBufInfo.u32WriteAddr != g_MsgBufInfo.u32ReadAddr));
        }
    }

    local_irq_save(flags);
    memcpy(&MsgInfoNow,  &g_MsgBufInfo, sizeof(g_MsgBufInfo));
    local_irq_restore((unsigned long)flags);

    if(MsgInfoNow.u32WriteAddr < MsgInfoNow.u32ReadAddr)
    {
        BufUsed = MsgInfoNow.u32BufSize - MsgInfoNow.u32ReadAddr
                    + MsgInfoNow.u32WriteAddr;
        DataLen1 =  MsgInfoNow.u32BufSize - MsgInfoNow.u32ReadAddr;
        DataLen2 = MsgInfoNow.u32WriteAddr;
    }
    else
    {
        BufUsed = MsgInfoNow.u32WriteAddr - MsgInfoNow.u32ReadAddr;
        DataLen1 = BufUsed;
        DataLen2 = 0;
    }

    if (BufLen <= (DataLen1 + DataLen2))
    {
        CopyLen = BufLen;
    }
    else
    {
        CopyLen = DataLen1 + DataLen2;
    }

    if (DataLen1 >= CopyLen)
    {
        if (bKernelCopy == HI_FALSE)
        {
            if(copy_to_user(Buf, (MsgInfoNow.u32ReadAddr+MsgInfoNow.pu8StartAddrVir), CopyLen))
            {
                HI_ERR_LOG("copy_to_user error\n");
                return HI_FAILURE;
            }
        }
        else
        {
            memcpy(Buf, (MsgInfoNow.u32ReadAddr+MsgInfoNow.pu8StartAddrVir), CopyLen);
        }

        NewReadAddr = MsgInfoNow.u32ReadAddr + CopyLen;
    }
    else
    {
        if (bKernelCopy == HI_FALSE)
        {
            if(copy_to_user(Buf, (MsgInfoNow.u32ReadAddr+MsgInfoNow.pu8StartAddrVir), DataLen1))
            {
                HI_ERR_LOG("copy_to_user error\n");
                return HI_FAILURE;
            }
        }
        else
        {
            memcpy(Buf, (MsgInfoNow.u32ReadAddr+MsgInfoNow.pu8StartAddrVir), DataLen1);
        }

        if (bKernelCopy == HI_FALSE)
        {
            if(copy_to_user((Buf + DataLen1), MsgInfoNow.pu8StartAddrVir, (CopyLen - DataLen1)))
            {
                HI_ERR_LOG("copy_to_user error\n");
                return HI_FAILURE;
            }
        }
        else
        {
            memcpy((Buf + DataLen1), MsgInfoNow.pu8StartAddrVir, (CopyLen - DataLen1));
        }

        NewReadAddr = CopyLen - DataLen1;
    }

    *pCopyLen = CopyLen;

    if (NewReadAddr >= MsgInfoNow.u32BufSize)
        NewReadAddr = 0;

    local_irq_save(flags);
    if (MsgInfoNow.u32ResetFlag == g_MsgBufInfo.u32ResetFlag)
    {
        g_MsgBufInfo.u32ReadAddr = NewReadAddr;
    }
    local_irq_restore((unsigned long)flags);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_LOG_BufferWrite(HI_U8 *Buf,  HI_U32 MsgLen, HI_U32 UserOrKer)
{
    HI_U32 CopyLen1;
    HI_U32 CopyLen2;
    HI_U32 NewWriteAddr;
    unsigned long flags;

    if (0 == g_MsgBufInfo.u32BufSize)
    {
        return HI_SUCCESS;
    }

    /*protect with semaphore while two module write at the same time*/
    /*CNcomment:两个模块同时写要做信号量保护*/
    local_irq_save(flags);

    if(g_MsgBufInfo.u32WriteAddr < g_MsgBufInfo.u32ReadAddr)
    {
        if ((g_MsgBufInfo.u32ReadAddr - g_MsgBufInfo.u32WriteAddr)
              < DEBUG_MSG_BUF_RESERVE)
        {
            LOGBufferReset();
        }
    }
    else
    {
        if ((g_MsgBufInfo.u32WriteAddr - g_MsgBufInfo.u32ReadAddr)
              > (g_MsgBufInfo.u32BufSize - DEBUG_MSG_BUF_RESERVE))
        {
            LOGBufferReset();
        }
    }

    if ((MsgLen + g_MsgBufInfo.u32WriteAddr) >= g_MsgBufInfo.u32BufSize)
    {
        CopyLen1 = g_MsgBufInfo.u32BufSize - g_MsgBufInfo.u32WriteAddr;
        CopyLen2 = MsgLen - CopyLen1;
        NewWriteAddr = CopyLen2;
    }
    else
    {
        CopyLen1 = MsgLen;
        CopyLen2 = 0;
        NewWriteAddr = MsgLen + g_MsgBufInfo.u32WriteAddr;
    }

    if(CopyLen1 > 0)
    {
        if(MSG_FROM_KERNEL == UserOrKer)
        {
            memcpy((g_MsgBufInfo.u32WriteAddr+g_MsgBufInfo.pu8StartAddrVir),
                    Buf, CopyLen1);
        }
        else
        {
            if(copy_from_user((g_MsgBufInfo.u32WriteAddr+g_MsgBufInfo.pu8StartAddrVir),
                    Buf, CopyLen1))
            {
                HI_ERR_LOG("copy_from_user error\n");
            }
        }
    }
    if(CopyLen2 > 0)
    {
        if(MSG_FROM_KERNEL == UserOrKer)
        {
            memcpy(g_MsgBufInfo.pu8StartAddrVir, (Buf + CopyLen1), CopyLen2);
        }
        else
        {
            if(copy_from_user(g_MsgBufInfo.pu8StartAddrVir,
                    (Buf + CopyLen1), CopyLen2))
            {
                HI_ERR_LOG("copy_from_user error\n");
            }
        }
    }

    //local_irq_save(flags);
    g_MsgBufInfo.u32WriteAddr = NewWriteAddr;

    if (g_bSetLogFileFlag != HI_TRUE)
    {
        wake_up_interruptible(&g_MsgBufInfo.wqNoData);
    }

    local_irq_restore((unsigned long)flags);

    return HI_SUCCESS;
}

/***********************log ops **************************/
static HI_S32 LOGPrintToBuf(const char *format, ...)
{
    char    log_str[LOG_MAX_TRACE_LEN] = {0};
    HI_U32  MsgLen = 0;
    va_list args;

    if (0 == g_LogModInit)
    {
        HI_ERR_LOG("log device not init(ker)!\n");
        return HI_FAILURE;
    }

    va_start(args, format);
    MsgLen = HI_OSAL_Vsnprintf(log_str, LOG_MAX_TRACE_LEN-1, format, args);
    va_end(args);

    if (MsgLen >= (LOG_MAX_TRACE_LEN-1))
    {
        log_str[LOG_MAX_TRACE_LEN-1] = '\0';  /* even the 'vsnprintf' commond will do it */
        log_str[LOG_MAX_TRACE_LEN-2] = '\n';
        log_str[LOG_MAX_TRACE_LEN-3] = '.';
        log_str[LOG_MAX_TRACE_LEN-4] = '.';
        log_str[LOG_MAX_TRACE_LEN-5] = '.';
    }

    return HI_DRV_LOG_BufferWrite((HI_U8 *)log_str, MsgLen, MSG_FROM_KERNEL);
}
#endif

static HI_U32 LOGGetTimeMs(HI_VOID)
{
    struct timeval tv;
    do_gettimeofday(&tv);
    return (((HI_U32)tv.tv_sec)*1000 + ((HI_U32)tv.tv_usec)/1000);
}

HI_S32 LOGAddModule(HI_PCHAR szProcName, HI_MOD_ID_E u32ItemID)
{
    if (NULL == g_pLogConfigInfo || u32ItemID >= LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S))
    {
        return HI_FAILURE;
    }

    g_pLogConfigInfo[u32ItemID].u8LogLevel = HI_LOG_LEVEL_DEFAULT;
    HI_OSAL_Snprintf(g_pLogConfigInfo[u32ItemID].ModName, sizeof(g_pLogConfigInfo[u32ItemID].ModName), szProcName);

    return HI_SUCCESS;
}

HI_S32 LOGRemoveModule(HI_PCHAR szProcName, HI_MOD_ID_E u32ItemID)
{

    if (NULL == g_pLogConfigInfo || u32ItemID >= LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S))
    {
        return HI_FAILURE;
    }

    g_pLogConfigInfo[u32ItemID].u8LogLevel = HI_LOG_LEVEL_DEFAULT;
    HI_OSAL_Snprintf(g_pLogConfigInfo[u32ItemID].ModName, sizeof(g_pLogConfigInfo[u32ItemID].ModName), "Invalid");

    return HI_SUCCESS;
}

/* HI_TRUE: need print ;  HI_FALSE: not print */
static HI_BOOL LOGLevelChk(HI_S32 s32Levle, HI_U32 enModId)
{
    if (enModId < LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S))
    {
        /* log module has Initialized yet */
        if (g_LogModInit && s32Levle <= g_pLogConfigInfo[enModId].u8LogLevel)
        {
            return HI_TRUE;
        }

        /* log module has not Initialized yet */
        if(!g_LogModInit && s32Levle <= HI_LOG_LEVEL_DEFAULT)
        {
            return HI_TRUE;
        }

    }

    return HI_FALSE;
}


/***********************log ops **************************/
/* 0 seria port, 1 network*/
static HI_U32 LOGPrintPosGet(HI_MOD_ID_E enModId)
{
    HI_U32 u32Pos = 0;

    if (enModId >= LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S))
    {
        return LOG_OUTPUT_SERIAL;
    }

    if (0 == g_LogModInit)
    {
        return LOG_OUTPUT_SERIAL;
    }

    //s32Ret = LOG_FILE_LOCK();

    if (g_bSetLogFileFlag == HI_TRUE)
    {
       u32Pos = LOG_OUTPUT_UDISK;
    }
    else
    {
        u32Pos = g_pLogConfigInfo[enModId].u8LogPrintPos;
    }

    //LOG_FILE_UNLOCK();

    return u32Pos;
}

#ifdef LOG_UDISK_SUPPORT
HI_S32 LogUdiskSave(const HI_S8* pFileName, HI_S8* pData, HI_U32 u32DataLen)
{
    HI_S32 s32WriteLen = 0;
    struct file* pFile = NULL;

    pFile = HI_DRV_FILE_Open(pFileName, 1);
    if(pFile == NULL)
    {
        HI_ERR_LOG("HI_DRV_FILE_Open %s failure...................\n", pFileName);
        return HI_FAILURE;
    }

    s32WriteLen = HI_DRV_FILE_Write(pFile, pData, u32DataLen);

    HI_DRV_FILE_Close(pFile);

    return HI_SUCCESS;
}


static struct task_struct *log_udisk_task = NULL;

int LogUdiskWriteThread(void* pArg)
{
    HI_U8 szBuf[700] = {0};
    HI_U32 u32ReadLen = 0;
    HI_S32 s32Ret = 0;
    HI_U8 szFileName[MAX_FILENAME_LENTH] = {0};
    HI_BOOL bSetFileFlag = HI_FALSE;

    while (1)
    {
        s32Ret = LOG_FILE_LOCK();

        bSetFileFlag = g_bSetLogFileFlag;

        HI_OSAL_Snprintf(szFileName, sizeof(szFileName)-1, "%s/stb.log", (const HI_S8*)UdiskLogFile);

        LOG_FILE_UNLOCK();

        set_current_state(TASK_INTERRUPTIBLE);

        if(kthread_should_stop())
        {
            break;
        }

        if ( bSetFileFlag == HI_FALSE)
        {
            msleep(10);
            continue;
        }

        memset(szBuf, 0, sizeof(szBuf));

        s32Ret = HI_DRV_LOG_BufferRead(szBuf, sizeof(szBuf)-1, &u32ReadLen, HI_TRUE);
        if (s32Ret == HI_SUCCESS)
        {
            LogUdiskSave((const HI_S8*)szFileName, szBuf, u32ReadLen);
        }

        msleep(100);
    }

    return 0;
}

HI_S32 LogUdiskInit(const HI_U8* pDiskFolder)
{
    int err;

    if (pDiskFolder == NULL )
    {
        return HI_FAILURE;
    }

    if (log_udisk_task == NULL)
    {
        log_udisk_task = kthread_create(LogUdiskWriteThread, (void*)pDiskFolder, "log_udisk_task");
        if(IS_ERR(log_udisk_task))
        {
            HI_ERR_LOG("create new kernel thread failed\n");

            err = PTR_ERR(log_udisk_task);
            log_udisk_task = NULL;

            return err;
        }

        wake_up_process(log_udisk_task);
    }

    return HI_SUCCESS;
}

HI_S32 LogUdiskExit(HI_VOID)
{
    if(log_udisk_task)
    {
        kthread_stop(log_udisk_task);
        log_udisk_task = NULL;
    }

    return HI_SUCCESS;
}
#endif

HI_VOID HI_LogOut(HI_U32 u32Level, HI_MOD_ID_E enModId, HI_U8 *pFuncName, HI_U32 u32LineNum, const char *format, ...)
{
    va_list args;
    HI_U32  MsgLen = 0;
    HI_BOOL bLevel = HI_FALSE;
    char    log_str[LOG_MAX_TRACE_LEN]={'a'};

    bLevel = LOGLevelChk(u32Level, enModId);
    if (bLevel)
    {
        log_str[LOG_MAX_TRACE_LEN-1] = 'b';
        log_str[LOG_MAX_TRACE_LEN-2] = 'c';

        va_start(args, format);
        MsgLen = HI_OSAL_Vsnprintf(log_str, LOG_MAX_TRACE_LEN, format, args);
        va_end(args);

        if (MsgLen >= LOG_MAX_TRACE_LEN)
        {
            log_str[LOG_MAX_TRACE_LEN-1] = '\0';  /* even the 'vsnprintf' commond will do it */
            log_str[LOG_MAX_TRACE_LEN-2] = '\n';
            log_str[LOG_MAX_TRACE_LEN-3] = '.';
            log_str[LOG_MAX_TRACE_LEN-4] = '.';
            log_str[LOG_MAX_TRACE_LEN-5] = '.';
        }

        /* log module has Initialized. */
        if (g_LogModInit)
        {
            int nPos = LOGPrintPosGet(enModId);
        #ifndef ANDROID
            HI_U32  TimeMs      __attribute__((unused)) = LOGGetTimeMs() % HI_DAY;
            HI_U32  Hour        __attribute__((unused)) = TimeMs / HI_HOUR;
            HI_U32  Minute      __attribute__((unused)) = (TimeMs % HI_HOUR) / HI_MINUTE;
            HI_U32  Second      __attribute__((unused)) = ((TimeMs % HI_HOUR) % HI_MINUTE) / HI_SECOND;
            HI_U32  Millisecond __attribute__((unused)) = TimeMs % HI_SECOND;
        #endif

            switch (nPos)
            {
                case LOG_OUTPUT_SERIAL:
                    if ( (enModId != HI_ID_VSYNC) && (enModId != HI_ID_ASYNC) )
                    {
                        HI_PRINT(PRINT_FMT);
                    }
                    break;

            #if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
                case LOG_OUTPUT_NETWORK:
                case LOG_OUTPUT_UDISK:
                    LOGPrintToBuf(PRINT_FMT);
                    break;
            #endif

                default:
                    break;
            }
        }
        else /* log module has not Initialized. */
        {
            HI_PRINT("[%s-Unknow]: %s[%d]:%s", DebugLevelName[u32Level],  pFuncName,
                        u32LineNum, log_str);
        }
    }
}

/***********************proc ops **************************/

static int SearchMod(char *s)
{
    int i= 0;
    int cnt = LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S);

    for (i = 0; i < cnt; i++){
        if (!HI_OSAL_Strncasecmp(g_pLogConfigInfo[i].ModName, s, sizeof(g_pLogConfigInfo[i].ModName)))
            return i;
    }
    return -1;
}

/* ate all blanks in a line */
static char *StripString(char *s, char *d)
{
    char *p = d;
    do{
        if (*s == '\n')
            *s = '\0';
        if (*s != ' ')
            *p++ = *s;
    }while(*s++ != '\0');
    return d;
}

static int SeperateString(char *s, char **left, char **right)
{
    char *p = s;
    /* find '=' */
    while(*p != '\0' && *p++ != '=');

    if (*--p != '=')
        return -1;

    /* seperate left from right vaule by '=' */
    *p = '\0';
    *left = s;
    *right = p + 1;
    return 0;
}

HI_S32 HI_DRV_LOG_ProcRead(struct seq_file *s, HI_VOID *pArg)
{
    HI_U32 i;
    HI_U8 u8Level;
    HI_U32 u32Total = LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S);

    if (0 == g_LogModInit)
    {
        PROC_PRINT(s,"    Log module not init\n");
        return 0;
    }
    PROC_PRINT(s, "---------------- Log Path ------------------------\n");
    PROC_PRINT(s, "log path:  %s\n", UdiskLogFile);

    PROC_PRINT(s, "---------------- Store Path ----------------------\n");
    PROC_PRINT(s, "store path:  %s\n", StorePath);

    PROC_PRINT(s, "---------------- Module Log Level ----------------\n");
    PROC_PRINT(s,"Log module\t  Level\n");
    PROC_PRINT(s, "--------------------------\n");
    for (i = 0; i < u32Total; i++)
    {
        if (HI_OSAL_Strncmp(g_pLogConfigInfo[i].ModName, "Invalid", 8))
        {
            u8Level = g_pLogConfigInfo[i].u8LogLevel;
            PROC_PRINT(s,"%-16s  %d(%s)\n",
                g_pLogConfigInfo[i].ModName, u8Level, DebugLevelName[u8Level]);
        }
    }

    PROC_PRINT(s, "\necho hi_avplay=2 > /proc/msp/log\n");
    PROC_PRINT(s, "echo log=/mnt > /proc/msp/log\n");
    PROC_PRINT(s, "echo storepath=/mnt > /proc/msp/log\n");

    return 0;
}

HI_S32 HI_DRV_LOG_ProcWrite( struct file * file,  const char __user * buf,
                     size_t count, loff_t *ppos)
{
    char m[MAX_FILENAME_LENTH] = {0};
    char d[MAX_FILENAME_LENTH] = {0};
    size_t len = MAX_FILENAME_LENTH;
    char *left, *right;
    int idx, level;
    int nRet = 0;

    if (*ppos >= MAX_FILENAME_LENTH)
        return -EFBIG;

    len = min(len, count);
    if(copy_from_user(m, buf, len ))
        return -EFAULT;

    if (0 == g_LogModInit)
    {
        HI_ERR_LOG("    Log module not init!\n");
        goto out;
    }

    StripString(m, d);

    /* echo help info to current terinmal */
    if (!HI_OSAL_Strncasecmp("help", m, 4))
    {
        HI_DRV_PROC_EchoHelper("To modify the level, use command line in shell: \n");
        HI_DRV_PROC_EchoHelper("    echo module_name = level_number > /proc/msp/log\n");
        HI_DRV_PROC_EchoHelper("    level_number: 0-fatal, 1-error, 2-warning, 3-info\n");
        HI_DRV_PROC_EchoHelper("    example: 'echo HI_DEMUX=3 > /proc/msp/log'\n");
        HI_DRV_PROC_EchoHelper("    will change log levle of module \"HI_DEMUX\" to 3, then, \n");
        HI_DRV_PROC_EchoHelper("all message with level higher than \"info\" will be printed.\n");
        HI_DRV_PROC_EchoHelper("Use 'echo \"all = x\" > /proc/msp/log' to change all modules.\n");

        HI_DRV_PROC_EchoHelper("\n\nTo modify the log path, use command line in shell: \n");
        HI_DRV_PROC_EchoHelper("Use 'echo \"log = x\" > /proc/msp/log' to set log path.\n");
        HI_DRV_PROC_EchoHelper("Use 'echo \"log = /dev/null\" > /proc/msp/log' to close log udisk output.\n");
        HI_DRV_PROC_EchoHelper("    example: 'echo log=/home > /proc/msp/log'\n");

        HI_DRV_PROC_EchoHelper("\n\nTo modify the debug file store path, use command line in shell: \n");
        HI_DRV_PROC_EchoHelper("Use 'echo \"storepath = x\" > /proc/msp/log' to set debug file path.\n");
        HI_DRV_PROC_EchoHelper("    example: 'echo storepath=/tmp > /proc/msp/log'\n");
    }

    if (SeperateString(d, &left, &right)){
        HI_WARN_LOG("string is unkown!\n");
        goto out;
    }

    if (!HI_OSAL_Strncasecmp("log", left, 4))
    {
        if (strlen(right) >= sizeof(g_szPathBuf))
        {
            HI_ERR_LOG("    Log path length is over than %d!\n",sizeof(g_szPathBuf));
            goto out;
        }

        nRet = LOG_FILE_LOCK();

        memset(g_szPathBuf, 0, sizeof(g_szPathBuf));
        memcpy(g_szPathBuf, right, strlen(right));

        if ( memcmp(g_szPathBuf, "/dev/null", strlen("/dev/null")) != 0 )
        {
            g_bSetLogFileFlag = HI_TRUE;
        }
        else
        {
            g_bSetLogFileFlag = HI_FALSE;
        }

        g_pLogConfigInfo->u8UdiskFlag = (HI_U8)g_bSetLogFileFlag;

        UdiskLogFile = g_szPathBuf;

        LOG_FILE_UNLOCK();

        HI_INFO_LOG("set log path is g_szPathBuf = %s\n", UdiskLogFile);

        goto out;
    }
    else if (!HI_OSAL_Strncasecmp("storepath", left, strlen("storepath")+1))
    {
        if (strlen(right) >= sizeof(g_szStorePathBuf))
        {
            HI_ERR_LOG("    Store path length is over than %d!\n",sizeof(g_szStorePathBuf));
            goto out;
        }

        nRet = LOG_FILE_LOCK();

        memset(g_szStorePathBuf, 0, sizeof(g_szStorePathBuf));
        memcpy(g_szStorePathBuf, right, strlen(right));

        StorePath = g_szStorePathBuf;

        LOG_FILE_UNLOCK();

        HI_INFO_LOG("set log path is StorePath = %s\n", g_szStorePathBuf);

        goto out;
    }
    else
    {
        level = simple_strtol(right, NULL, 10);
        if (!level && *right != '0'){
            HI_WARN_LOG("invalid value!\n");
            goto out;
        }
        if (!HI_OSAL_Strncasecmp("all", left, 4)){
            int i = 0;
            HI_U32 u32Total = LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S);
            for (i = 0; i < u32Total; i++)
                g_pLogConfigInfo[i].u8LogLevel = (HI_U8)level;
            goto out;
        }

        idx = SearchMod(left);
        if (-1 == idx){
            HI_WARN_LOG("%s not found in array!\n", left);
            return count;
        }
        g_pLogConfigInfo[idx].u8LogLevel = (HI_U8)level;
    }

    //HI_PRINT("\n\tMoudle '%s' level change to:%d(%s)\n", g_pLogConfigInfo[idx].ModName,
    //       level,  DebugLevelName[level]);

out:
    *ppos = len;
    return len;
#undef TMP_BUF_LEN
}

HI_S32 HI_DRV_LOG_SetPath(HI_CHAR *pszPath, HI_U32 u32PathLen)
{
    int nRet = 0;
    if (pszPath == NULL || u32PathLen > sizeof(g_szPathBuf))
    {
        return HI_FAILURE;
    }

    nRet = LOG_FILE_LOCK();

    memset(g_szPathBuf, 0, sizeof(g_szPathBuf));
    nRet = copy_from_user(g_szPathBuf, pszPath, u32PathLen);

    if (nRet != 0)
    {
        LOG_FILE_UNLOCK();
        return HI_FAILURE;
    }

    if ( memcmp(pszPath, "/dev/null", strlen("/dev/null")) == 0 )
    {
        g_bSetLogFileFlag = HI_FALSE;
    }
    else
    {
        g_bSetLogFileFlag = HI_TRUE;
    }

    g_pLogConfigInfo->u8UdiskFlag = (HI_U8)g_bSetLogFileFlag;

    UdiskLogFile = g_szPathBuf;

    LOG_FILE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_DRV_LOG_GetPath(HI_S8* ps8Buf, HI_U32 u32Len)
{
    HI_S32 nPathLen = 0;

    if ( UdiskLogFile == NULL)
    {
        return HI_FAILURE;
    }

    nPathLen = strlen(UdiskLogFile) + 1;

    if (nPathLen > u32Len || nPathLen <= 1)
    {
        return HI_FAILURE;
    }

    memcpy(ps8Buf, UdiskLogFile, nPathLen);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_LOG_SetStorePath(HI_CHAR *pszPath, HI_U32 u32PathLen)
{
    int nRet = 0;
    if (pszPath == NULL || u32PathLen > sizeof(g_szStorePathBuf))
    {
        return HI_FAILURE;
    }

    nRet = LOG_FILE_LOCK();
    memset(g_szStorePathBuf, 0, sizeof(g_szStorePathBuf));
    nRet = copy_from_user(g_szStorePathBuf, pszPath, u32PathLen);

    if (nRet != 0)
    {
        LOG_FILE_UNLOCK();
        return HI_FAILURE;
    }

    StorePath = g_szStorePathBuf;
    LOG_FILE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_DRV_LOG_GetStorePath(HI_S8* ps8Buf, HI_U32 u32Len)
{
    HI_S32 s32PathLen = 0;

    if (StorePath == NULL)
    {
        return HI_FAILURE;
    }

    s32PathLen = strlen(StorePath) + 1;

    if (s32PathLen > u32Len || s32PathLen <= 1)
    {
        return HI_FAILURE;
    }

    memcpy(ps8Buf, StorePath, s32PathLen);
    return HI_SUCCESS;
}


/***********************module ops **************************/

static HI_S32 LOGConfigInfoInit(HI_VOID)
{
    HI_S32  ret;
    HI_U32  i;

    ret = HI_DRV_MMZ_AllocAndMap("CMN_LogInfo", HI_NULL, LOG_CONFIG_BUF_SIZE, 0, &g_LogConfigBuf);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_LOG("HI_DRV_MMZ_AllocAndMap failed\n");
        return HI_FAILURE;
    }

    memset((HI_U8 *)g_LogConfigBuf.pu8StartVirAddr, 0, LOG_CONFIG_BUF_SIZE);
    g_pLogConfigInfo = (LOG_CONFIG_INFO_S*)g_LogConfigBuf.pu8StartVirAddr;

    g_pLogConfigInfo->u8UdiskFlag = 0;

    /* max debug module number: 8192/28 = 341/292 */
    for (i = 0; i < LOG_CONFIG_BUF_SIZE / sizeof(LOG_CONFIG_INFO_S); i++)
    {
        g_pLogConfigInfo[i].u8LogLevel = HI_LOG_LEVEL_DEFAULT;
        g_pLogConfigInfo[i].u8LogPrintPos = LOG_OUTPUT_SERIAL;
        HI_OSAL_Snprintf(g_pLogConfigInfo[i].ModName, sizeof(g_pLogConfigInfo[i].ModName), "Invalid");
    }

    return HI_SUCCESS;
}

static HI_S32 LOGConfigInfoExit(HI_VOID)
{
    if (0 != g_LogConfigBuf.pu8StartVirAddr)
    {
        g_pLogConfigInfo = NULL;
        HI_DRV_MMZ_UnmapAndRelease(&g_LogConfigBuf);
    }

    return HI_SUCCESS;
}

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
static HI_S32 LOGBufferInit(HI_U32 BufSize)
{
    MMZ_BUFFER_S    MmzBuf;

    memset(&g_MsgBufInfo, 0, sizeof(g_MsgBufInfo));

    g_MsgBufInfo.u32BufSize = BufSize;

    init_waitqueue_head(&(g_MsgBufInfo.wqNoData));

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap("CMN_LogTrace", HI_NULL, BufSize, 0, &MmzBuf))
    {
        HI_ERR_LOG("init failed there is not enough mem for debug message buffer.\n");
        return -1;
    }

    g_MsgBufInfo.u32StartAddrPhy = MmzBuf.u32StartPhyAddr;
    g_MsgBufInfo.pu8StartAddrVir = MmzBuf.pu8StartVirAddr;

    return 0;
}

static HI_S32 LOGBufferExit(HI_VOID)
{
    if (NULL != g_MsgBufInfo.pu8StartAddrVir)
    {
        MMZ_BUFFER_S mmzBuf;

        mmzBuf.u32StartPhyAddr = g_MsgBufInfo.u32StartAddrPhy;
        mmzBuf.pu8StartVirAddr = g_MsgBufInfo.pu8StartAddrVir;

        HI_DRV_MMZ_UnmapAndRelease(&mmzBuf);

        g_MsgBufInfo.pu8StartAddrVir = NULL;
        g_MsgBufInfo.u32StartAddrPhy = 0;
    }

    return HI_SUCCESS;
}
#endif

HI_VOID HI_DRV_LOG_ConfgBufAddr(HI_U32 *addr)
{
    if (g_LogModInit)
    {
        *addr = g_LogConfigBuf.u32StartPhyAddr;
    }
    else
    {
        *addr = 0;
    }

    return;
}

HI_S32 LOG_GetLevel(HI_U32 enModId, HI_U8* u8Buf, HI_U32 u32Length)
{
    HI_U32 u32Level = 0;

    if (enModId >= LOG_CONFIG_BUF_SIZE/sizeof(LOG_CONFIG_INFO_S))
    {
        return 0;
    }

    if (0 == g_LogModInit)
    {
        return 0;
    }

    u32Level = g_pLogConfigInfo[enModId].u8LogLevel;

    memset(u8Buf, 0, u32Length);
    if (u32Length <= strlen(DebugLevelName[u32Level]) )
    {
        u32Length = strlen(DebugLevelName[u32Level]) - 1;
    }

    memcpy(u8Buf, DebugLevelName[u32Level], u32Length);

    return u32Level;
}

HI_S32 HI_DRV_LOG_KInit(HI_VOID)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    HI_S32              ret;
    HI_SYS_MEM_CONFIG_S MemConfig;

    ret = HI_DRV_SYS_GetMemConfig(&MemConfig);
    if (HI_SUCCESS != ret)
    {
        MemConfig.u32TotalSize = 1024;
    }

    if (512 == MemConfig.u32TotalSize)
    {
        LogBufSize = 128 * DEBUG_MSG_BUF_RESERVE;
    }
#endif

    if (HI_SUCCESS != LOGConfigInfoInit())
    {
        return -1;
    }

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
    if (LogBufSize < DEBUG_MSG_BUF_RESERVE)
    {
        LogBufSize = DEBUG_MSG_BUF_RESERVE;
    }

    if (HI_SUCCESS != LOGBufferInit(LogBufSize))
    {
        LOGConfigInfoExit();
        return -1;
    }
#endif

#ifdef LOG_UDISK_SUPPORT
    LogUdiskInit(UdiskLogFile);
#endif

    LOGAddModule("ASYNC", HI_ID_ASYNC);
    LOGAddModule("VSYNC", HI_ID_VSYNC);

    g_LogModInit = 1;

    return 0;
}

HI_VOID HI_DRV_LOG_KExit(HI_VOID)
{
    g_LogModInit = 0;

#ifdef LOG_UDISK_SUPPORT
    LogUdiskExit();
#endif

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
    LOGBufferExit();
#endif

    LOGConfigInfoExit();
}

#ifndef MODULE
/* Legacy boot options - nonmodular */
static int __init get_logbufsize(char *str)
{
    LogBufSize = simple_strtol(str, NULL, 0);
    HI_INFO_LOG("LogBufSize = 0x%x\n", LogBufSize);
    return 1;
}
__setup("LogBufSize=", get_logbufsize);

#else

EXPORT_SYMBOL(HI_DRV_LOG_ProcRead);
EXPORT_SYMBOL(HI_DRV_LOG_ProcWrite);

#if defined(LOG_NETWORK_SUPPORT) || defined(LOG_UDISK_SUPPORT)
EXPORT_SYMBOL(HI_DRV_LOG_BufferRead);
EXPORT_SYMBOL(HI_DRV_LOG_BufferWrite);
#endif

module_param(LogBufSize, int, S_IRUGO);

EXPORT_SYMBOL(HI_DRV_LOG_ConfgBufAddr);

module_param(UdiskLogFile, charp, S_IRUGO);
module_param(StorePath, charp, S_IRUGO);
EXPORT_SYMBOL(StorePath);
EXPORT_SYMBOL(HI_DRV_LOG_GetStorePath);

#endif

EXPORT_SYMBOL(HI_LogOut);


