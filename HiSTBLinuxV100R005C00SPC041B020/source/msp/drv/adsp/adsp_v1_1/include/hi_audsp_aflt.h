/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv2bO4a86Ra1cP5w/R608AW9TGITFV3+8N3VYYiVf+Q2SydsXizP+UzuonkR7
tfVi+82Tob9NhY9nzNjNeP1EpMfoegdQswelW1ZFm91MRgyddYAmW+xMJncFIqQK7QVXh50M
hyCrTAVhYSpagC+OxTp7nkVL2t2zRrqCzw/PL3dPb/7KGKy10EIJtCKReaaEWQ==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_audsp_aflt.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/02/28
  Description   :
  History       :
  1.Date        : 2013/02/28
    Author      : zgjie
    Modification: Created file

 *******************************************************************************/

#ifndef __HISI_AUDIO_DSP_FILTER_H__
#define __HISI_AUDIO_DSP_FILTER_H__

#include "hi_audsp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define AFLT_COM_REG_BASE (DSP0_SHARESRAM_BASEADDR + DSP0_SHARESRAM_AFLT_OFFSET)
#define AFLT_CHN_REG_BASE (AFLT_COM_REG_BASE + AFLT_CHN_REG_OFFSET)
#define AFLT_COM_REG_OFFSET   0x0000
#define AFLT_CHN_REG_OFFSET   sizeof(S_AFLT_COM_ATTR)
#define AFLT_COM_REG_BANDSIZE sizeof(S_AFLT_COM_ATTR)
#define AFLT_CHN_REG_BANDSIZE sizeof(S_AFLT_REGS_TYPE)

#define AFLT_REG_LENGTH   0x400       //0x400 (1.0k) reg for aflt

#define AFLT_MAX_CHAN_NUM 6

#define AFLT_CUSTOMER_AUTHKEY_SIZE 4 //4*sizeof(HI_U32)  Customer authorise key

typedef enum
{
    AFLT_ErrorNone = 0,

    AFLT_ErrorInsufficientResources = (HI_S32) 0x80001000, /**<The device fails to be created due to insufficient resources.*/ /**<CNcomment:资源不够，创建设备失败 */

    AFLT_ErrorInvalidParameter = (HI_S32) 0x80001001, /**<The input parameter is invalid.*/ /**<CNcomment:输入参数非法 */

    AFLT_ErrorStreamCorrupt = (HI_S32) 0x80001002, /**<The decoding fails due to incorrect input streams.*/ /**<CNcomment:输入码流错误，解码失败 */

    AFLT_ErrorNotEnoughData = (HI_S32) 0x80001003, /**<The decoding ends due to insufficient streams.*/ /**<CNcomment:输入码流不够，退出解码 */

    AFLT_ErrorMax = 0x9FFFFFFF
} AFLT_ERRORTYPE_E;

/**Defines the cmd of an AFLT.*/
typedef enum
{
    AFLT_CMD_CREATE = 0,
    AFLT_CMD_DESTROY,
    AFLT_CMD_START,
    AFLT_CMD_STOP,
    AFLT_CMD_SETPARAM,
    AFLT_CMD_GETPARAM,
    AFLT_CMD_SETCONFIG,
    AFLT_CMD_GETCONFIG,
    AFLT_CMD_RESTORESETTING,  //for standby
    AFLT_CMD_STORESETTING,    //for standby
    AFLT_CMD_INITINBUF,
    AFLT_CMD_DEINITINBUF,
    AFLT_CMD_INITOUTBUF, //TODO
    AFLT_CMD_DEINITOUTBUF, //TODO

    AFLT_CMD_BUTT
} AFLT_CMD_E;

typedef enum
{
    AFLT_CMD_DONE = 0,
    AFLT_CMD_ERR_START_FAIL,
    AFLT_CMD_ERR_STOP_FAIL,
    AFLT_CMD_ERR_CREATE_FAIL,
    AFLT_CMD_ERR_UNKNOWCMD,
    AFLT_CMD_ERR_TIMEOUT,
    AFLT_ERR_CMD_SETPARAMS,
    AFLT_ERR_CMD_GETPARAMS,
    AFLT_ERR_CMD_SETCONFIG,
    AFLT_ERR_CMD_GETCONFIG,
    AFLT_ERR_CMD_RESTORESETTING,
    AFLT_ERR_CMD_STORESETTING,
    AFLT_ERR_CMD_INITINBUF,
    AFLT_ERR_CMD_DEINITINBUF,
    AFLT_ERR_CMD_INITOUTBUF,
    AFLT_ERR_CMD_DEINITOUTBUF,

    AFLT_CMD_ERR_BUTT
} AFLT_CMD_RET_E;

typedef enum
{
    AFLT_STATUS_START = 0, /**<Stop*//**<CNcomment: 停止 */
    AFLT_STATUS_STOP, /**<Start*//**<CNcomment: 运行 */

    AFLT_STATUS_BUTT
} AFLT_STATUS_E;

typedef enum
{
    ADSP_AFLT_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    ADSP_AFLT_CMD_STOP,     /**<Start*//**<CNcomment: 运行 */

    ADSP_AFLT_CMD_BUTT
} ADSP_AFLTSYS_CMD_E;

typedef enum
{
    ADSP_AFLT_CMD_DONE = 0,
    ADSP_AFLT_CMD_ERR_START_FAIL = 1,
    ADSP_AFLT_CMD_ERR_STOP_FAIL = 2,
    ADSP_AFLT_CMD_ERR_CREATE_FAIL = 3,
    ADSP_AFLT_CMD_ERR_UNKNOWCMD =  4,
    ADSP_AFLT_CMD_ERR_TIMEOUT = 5,
    ADSP_AFLT_CMD_ERR_BUTT
} ADSP_AFLTSYS_CMD_RET_E;

typedef enum
{
    AFLT_EFFECT_BASE = 0,

    /* dsp advance effect */
    AFLT_EFFECT_SRS,

    AFLT_EFFECT_DOLBYDV258,

    AFLT_DEC_DOLBOY_DDP,

    AFLT_DEC_DOLBOY_TRUEHD,

    AFLT_DEC_DTSHD,

    AFLT_DEC_PCM,

    AFLT_DEC_MP3,

    AFLT_DEC_MP3_XA,

    AFLT_DEC_DTSHD_XA,

    AFLT_EFFECT_DBX,

    AFLT_BUTT
} AFLT_COMPONENT_ID_E;


//=========================== AFLT REG Define begin ===========================
/* Define the union U_TIEMOUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    timeout_cnt      : 8  ;   /* [7..0]  */
        unsigned int    Reserved_9       : 24 ;  /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AFLT_STATUS0;

typedef struct
{
    volatile U_AFLT_STATUS0       AFLT_STATUS0;
    volatile unsigned int         RESERVED[7];  //com attr reversed 0x20
} S_AFLT_COM_ATTR;

/* Define the union U_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmd                   : 4   ; /* [3..0]  */
        unsigned int    cmd_done              : 1   ; /* [4]  */
        unsigned int    cmd_return_value      : 4   ; /* [8..5]  */
        unsigned int    param_index           : 7   ; /* [15..9]  */
        unsigned int    param_wordsize        : 13  ; /* [28..16]  */
        unsigned int    Reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CTRL;

/* Define the union U_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    msgpool_size          : 16  ; /* [15..0]  */
        unsigned int    filter_id             : 4   ; /* [19..16]  */
        unsigned int    inbuf_cached          : 1   ; /* [20]  */
        unsigned int    outbuf_cached         : 1   ; /* [21]  */
        unsigned int    master_flag           : 1   ; /* [22]  */
        unsigned int    eos_flag              : 1   ; /* [23]  */
        unsigned int    endofframe            : 1   ; /* [24]  */
        unsigned int    use_flag              : 1   ; /* [25]  */
        unsigned int    status                : 1   ; /* [26]  */
        unsigned int    aef_cert              : 1   ; /* [27]  */
        unsigned int    Reserved_4            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ATTR;

/* Define the union U_IP_BUF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inbuff_size           : 28  ; /* [27..0]  */
        unsigned int    inbuff_flag           : 1   ; /* [28]  */
        unsigned int    inbuff_eos_flag       : 1   ; /* [29]  */
        unsigned int    inbuff_high_flag      : 1   ; /* [30]  */
        unsigned int    Reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_IP_BUF_SIZE;

/* Define the union U_OP_BUF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_size          : 24  ; /* [23..0]  */
        unsigned int    periond_num           : 4   ; /* [27..24]  */
        unsigned int    Reserved_4            : 4  ;  /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_OP_BUF_SIZE;

/* Define the union U_OP_BUF_WIDX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_write_idx     : 4   ; /* [3..0]  */
        unsigned int    periond_write_wrap    : 1   ; /* [4]  */
        unsigned int    Reserved_4            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_OP_BUF_WIDX;

/* Define the union U_OP_BUF_RIDX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_read_idx      : 4   ; /* [3..0]  */
        unsigned int    periond_read_wrap     : 1   ; /* [4]  */
        unsigned int    Reserved_5            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_OP_BUF_RIDX;

/* Define the union U_STATUS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    frame_cnt             : 16  ; /* [15..0]  */
        unsigned int    frame_err_cnt         : 8  ;  /* [23..16]  */
        unsigned int    Reserved_6            : 8  ;  /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_STATUS0;

/* Define the union U_EXEFRAME_STATUS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    Frame_ConsumeBytes    : 16  ; /* [15..0]  */
        unsigned int    Reserved_7            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EXEFRAME_STATUS0;

/* Define the union U_TIEMOUT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sche_timeout_cnt      : 8  ;   /* [7..0]  */
        unsigned int    exe_timeout_cnt       : 8  ;   /* [15..8]  */
        unsigned int    Reserved_8            : 16  ;  /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_TIMEOUT;

//=========================== AFLT REG Define end ===========================
/* Define the global struct */
typedef struct
{
    volatile U_CTRL               CTRL;
    S_AOE_ADDR_TYPE               MSGPOOL_ADDR;
    volatile unsigned int         IPBUF_BOUNDARY;
    volatile U_ATTR               ATTR;
    S_AOE_ADDR_TYPE               IP_BUF_ADDR;
    volatile U_IP_BUF_SIZE        IP_BUF_SIZE;
    volatile unsigned int         IP_BUF_WPTR;
    volatile unsigned int         IP_BUF_RPTR;
    volatile unsigned int         IP_FIFO_ADDR;
    volatile unsigned int         IP_FIFO_SIZE;
    volatile unsigned int         IP_FIFO_RPTR;
    volatile unsigned int         IP_FIFO_BYTES;
    S_AOE_ADDR_TYPE               OP_BUF_ADDR;
    volatile U_OP_BUF_SIZE        OP_BUF_SIZE;
    volatile U_OP_BUF_WIDX        OP_BUF_WIDX;
    volatile U_OP_BUF_RIDX        OP_BUF_RIDX;
    volatile U_STATUS0            STATUS0;
    volatile U_EXEFRAME_STATUS0   EXEFRAME_STATUS0;
    volatile unsigned int         TRY_EXE_CNT;
    volatile U_TIMEOUT            TIMEOUT;
    volatile unsigned int         READPOS;
    volatile unsigned int         RESERVED0[3];
    volatile unsigned int         AUTH_KEY[AFLT_CUSTOMER_AUTHKEY_SIZE];
    volatile unsigned int         RESERVED1[(0x80 / sizeof(HI_U32)) - 27 - AFLT_CUSTOMER_AUTHKEY_SIZE];//AOEIMP:TOCHECK Only one
} S_AFLT_REGS_TYPE;


//==============================================================================

typedef struct
{
    HI_U32      u32FrameIndex;      /*frame index*/
    HI_U32      u32PtsReadPos;      /* wrap at u32BufBoundary */
    HI_U32      pFrameBuf;          /*frame buf addr*/
    HI_U32      u32FrameBufSize;    /*frame buf total size*/
    HI_U32      u32FrameDataBytes;  /*frame buf data size*/
    HI_U32      u32FramePrivBytes;  /*frame buf private info size*/
} AFLT_ADSP_FRAME_INFO_S;

typedef struct
{
    HI_U32  u32InBytesConsumed;
    HI_U32  u32OutDataBytesProduced;
} AFLT_EXCUTE_FRAME_S;

typedef struct
{
    HI_VOID* pOutPrivInfo;
    HI_U32  u32OutPrivBytesProduced;
} AFLT_FRAME_PRIVINFO_S;

typedef struct
{

    AFLT_ERRORTYPE_E (*Init)(HI_VOID* *phComp);

    AFLT_ERRORTYPE_E (*DeInit)(HI_VOID* hComp);

    AFLT_ERRORTYPE_E (*SetConfig)(HI_VOID* hComp, HI_U32 u32ConfigIndex, HI_VOID *pConfigStructure);

    AFLT_ERRORTYPE_E (*GetConfig)(HI_VOID* hComp, HI_U32 u32ConfigIndex, HI_VOID *pConfigStructure, HI_U32 *pu32ConfigStructureSize);

    AFLT_ERRORTYPE_E (*GetParameter)(HI_VOID* hComp, HI_U32 u32ParamIndex, HI_VOID *pParameterStructure, HI_U32 *pu32ParameterStructureSize);

    AFLT_ERRORTYPE_E (*SetParameter)(HI_VOID* hComp, HI_U32 u32ParamIndex, HI_VOID * pParameterStructure);

    AFLT_ERRORTYPE_E (*StoreSetting)(HI_VOID* hComp, HI_VOID * pSettingStructure, HI_U32 *pu32SettingStructureSize);

    AFLT_ERRORTYPE_E (*RestoreSetting)(HI_VOID* hComp, HI_VOID * pSettingStructure);

    AFLT_ERRORTYPE_E (*SetInputBuffer)(HI_VOID* hComp, HI_VOID *pInBuf, HI_U32 u32InBufSize);

    AFLT_ERRORTYPE_E (*SetOutputBuffer)(HI_VOID* hComp, HI_VOID *pOutBuf, HI_U32 u32OutBufSize);

    AFLT_ERRORTYPE_E (*GetInputBytesleft)(HI_VOID* hComp, HI_U32 *pu32Bytesleft);

    AFLT_ERRORTYPE_E (*GetMinInputBufSize)(HI_VOID* hComp, HI_U32 *pu32InBufsize);

    AFLT_ERRORTYPE_E (*ExcuteFrame)(HI_VOID* hComp, AFLT_EXCUTE_FRAME_S *pstExcuteFrame);

    AFLT_ERRORTYPE_E (*GetCurnFramePrivInfo)(HI_VOID* hComp, AFLT_FRAME_PRIVINFO_S *pstFramePrivInfo);

    AFLT_ERRORTYPE_E (*SetEosFlag)(HI_VOID* hComp, HI_BOOL bEosFlag);
} AFLT_COMPONENT_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* #ifndef __HISI_AUDIO_DSP_FILTER_H__ */
