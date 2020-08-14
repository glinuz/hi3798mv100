/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:
* Description: driver aiao common header
*
* History:
* Version   Date         Author         DefectNum    Description
* main\1       AudioGroup     NULL         Create this file.
***********************************************************************************/
#ifndef __DRV_AI_COMMON_H__
#define __DRV_AI_COMMON_H__

#include <sound/pcm.h>

#include "circ_buf.h"
#include "hal_aiao_common.h"
#include "drv_ai_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define AI_NAME "HI_AI"

#define AI_LATENCYMS_PERFRAME_DF  (20)
#define AI_SAMPLE_PERFRAME_DF (48000/1000*AI_LATENCYMS_PERFRAME_DF)
#define AI_BUFF_FRAME_NUM_DF  (16)

enum
{
    AI_I2S0_MSK = 0,
    AI_I2S1_MSK,
    AI_ADAC_MSK,
    AI_HDMI_MSK,
};

#define AI_OPEN_CNT_MAX       (2)

#if (1 == HI_PROC_SUPPORT)
#define AI_PATH_NAME_MAXLEN   256        //for proc
#define AI_FILE_NAME_MAXLEN   256

#define AI_STRING_SKIP_BLANK(str)       \
    while (str[0] == ' ')           \
    {                               \
        (str)++;                    \
    }

#define AI_PROC_SHOW_HELP(u32Ai) \
    do                                                         \
    {                                                          \
        HI_DRV_PROC_EchoHelper("-------------------------------------------------AI----------------------------------------------------\n"); \
        HI_DRV_PROC_EchoHelper("echo        command         para              path               explanation\n\n"); \
        HI_DRV_PROC_EchoHelper("echo        save            [start/stop]     > /proc/msp/ai%d     Record pcm data from ai\n",u32Ai); \
        HI_DRV_PROC_EchoHelper("echo        NR              [normal/mute/off]> /proc/msp/ai%d    Set AI NR Mode\n", u32Ai); \
        HI_DRV_PROC_EchoHelper("echo        setNRlevel      [-60~-100]       > /proc/msp/ai%d     Set AI NR mute threshold\n", u32Ai); \
        HI_DRV_PROC_EchoHelper("echo        dre             [0/1]            > /proc/msp/ai%d     Ai dre switch(0:off;1:on)\n",u32Ai); \
        HI_DRV_PROC_EchoHelper("echo        delay           [0~320]          > /proc/msp/ai%d     Ai set delay(step:1;unit:ms)\n",u32Ai); \
        HI_DRV_PROC_EchoHelper("-------------------------------------------------------------------------------------------------------\n"); \
    } while (0)

typedef enum
{
    AI_CMD_CTRL_STOP = 0,
    AI_CMD_CTRL_START,
    AI_CMD_CTRL_BUTT
} AI_CMD_CTRL_E;

typedef enum
{
    AI_CMD_PROC_SAVE_AI = 0,
    AI_CMD_PROC_NR_SWITCH,
    AI_CMD_PROC_SET_DELAY,
    AI_CMD_PROC_SET_NRLEVEL,
    AI_CMD_PROC_BUTT
} AI_CMD_PROC_E;
#endif

typedef struct
{
    HI_PHYS_ADDR_T  tBufPhyAddr;
    HI_VIRT_ADDR_T  tBufVirAddr;
    HI_U32  u32BufSize;
    HI_U32  u32PeriodByteSize;
    HI_U32  u32Periods;
} AI_ALSA_BUF_ATTR_S;

typedef struct
{
    AI_ALSA_BUF_ATTR_S       stBuf; //for alsa mmap dma buffer
    AIAO_IsrFunc*            IsrFunc;
    HI_VOID* pSubstream;  //for alsa ISR func params
} AI_ALSA_Param_S;

typedef enum
{
    AI_CHN_STATUS_STOP   = 0,
    AI_CHN_STATUS_START,
    AI_CHN_STATUS_BUTT,
} AI_CHN_STATUS_E;

typedef struct
{
    HI_BOOL           bUsed;
    MMZ_BUFFER_S      AIProcMmz;
    HI_CHAR           szProcMmzName[32];
    AI_PROC_INFO_S*   pstAiProcInfo;
} AI_PROC_ATTR_S;

typedef struct
{
    HI_HANDLE         hAI;
    HI_UNF_AI_ATTR_S  stSndPortAttr;
    HI_U32            u32Channels;
    HI_U32            u32BitPerSample;
    HI_UNF_AI_E       enAiPort;
    AIAO_PORT_ID_E    enPort;
    AI_CHN_STATUS_E   enCurnStatus;

    MMZ_BUFFER_S            stRbfMmz;    //port mmz buf
    MMZ_BUFFER_S            stFrameMmz;  //ai mmz buf
    AI_BUF_ATTR_S           stAiBuf;     //the same as stAiRbfMmz In physics
    struct file*            pstFile;

    HI_BOOL                 bAttach;
    HI_HANDLE               hTrack;
    HI_BOOL                 bAlsa;
    HI_VOID*                pAlsaPara;
    HI_UNF_AI_DELAY_S       stDelayComps;
#ifdef __DPT__
    HI_UNF_AI_NR_ATTR_S     stNR;
#endif

    AI_PROC_ATTR_S          stAiProcAttr;

#if (1 == HI_PROC_SUPPORT)
    /*save pcm*/
    AI_CMD_CTRL_E           enSaveState;
    HI_U32                  u32SaveCnt;
    struct file*            fileHandle;
    HI_VOID*                pSaveBuf;
    HI_U32                  u32SaveReadPos;
    HI_BOOL                 bSaveThreadRun;
#endif
} AI_CHN_STATE_S;

typedef struct
{
    HI_UNF_AI_E     enAIPortID;
    MMZ_BUFFER_S    stRbfMmz;
} AI_RESOURCE_S;

#if (1 == HI_PROC_SUPPORT)
typedef struct
{
    DRV_PROC_READ_FN  pfnReadProc;
    DRV_PROC_WRITE_FN pfnWriteProc;
} AI_REGISTER_PARAM_S;
#endif

//AI GLOABL RESOURCE
typedef struct
{
    HI_U32                u32AiUseFlag;   /* resource usage such as  (1 << I2S | 1  << HDMI RX | 1 <<  ...) */
    AI_CHN_STATE_S*       apAiState[AI_MAX_TOTAL_NUM];

#if (1 == HI_PROC_SUPPORT)
    AI_REGISTER_PARAM_S*  pstProcParam;    /* AI Proc functions */
#endif

    AI_EXPORT_FUNC_S      stExtFunc;       /* AI provide extenal functions */

    atomic_t              atmOpenCnt;      /* Open times */
} AI_GLOBAL_PARAM_S;

HI_S32 AI_DRV_Open(struct inode* inode, struct file* file);
long AI_DRV_Ioctl(struct file* file, HI_U32 cmd, unsigned long arg);
HI_S32 AI_DRV_Release(struct inode* inode, struct file* file);
HI_S32 AI_DRV_Init(HI_VOID);
HI_VOID AI_DRV_Exit(HI_VOID);

#if (1 == HI_PROC_SUPPORT)
HI_S32 AI_DRV_ReadProc(struct seq_file* file, HI_VOID* args);
HI_S32 AI_DRV_WriteProc(struct file* file, const char __user* buf, size_t count, loff_t* ppos);
HI_S32 AI_DRV_RegisterProc(AI_REGISTER_PARAM_S* pstParam);
HI_VOID AI_DRV_UnregisterProc(HI_VOID);
#endif

HI_S32 AI_DRV_Suspend(PM_BASEDEV_S* dev, pm_message_t state);
HI_S32 AI_DRV_Resume(PM_BASEDEV_S* dev);
HI_S32 AI_GetPortBuf(HI_HANDLE hAI, AIAO_RBUF_ATTR_S* pstAiaoBuf);
HI_S32 AI_GetPortAttr(HI_HANDLE hAI, AIAO_PORT_ATTR_S* pstPortAttr);

#ifdef __DPT__
HI_S32 AI_GetNRAttr(HI_HANDLE hAi, HI_UNF_AI_NR_ATTR_S* pstNRParam);
#endif

HI_S32 AI_GetEnable(HI_HANDLE hAi, HI_BOOL* pbEnable);
HI_S32 AI_SetEnable(HI_HANDLE hAi, HI_BOOL bEnable, HI_BOOL bTrackResume);
HI_S32 AI_GetDelayMs(HI_HANDLE hAi, HI_U32* pu32DelayMs);
HI_BOOL AI_CheckIsHdmiPort(HI_HANDLE hAi);

HI_S32 AI_AttachTrack(HI_HANDLE hAI, HI_HANDLE hTrack);
HI_S32 AI_DetachTrack(HI_HANDLE hAI, HI_HANDLE hTrack);

#ifdef HI_ALSA_AI_SUPPORT
HI_S32 AI_GetIsrFunc(AIAO_IsrFunc** pFunc);
HI_S32 AI_GetAiaoPortId(HI_HANDLE hAI, AIAO_PORT_ID_E* enPort);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
