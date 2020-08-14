/**
 \file
 \brief aenc inner mpi head
 \copyright Shenzhen Hisilicon Co., Ltd.
 \version draft
 \author QuYaxin 46153
 \date 2010-1-29
 */

#ifndef __MPI_PRIV_AENC_H__
#define __MPI_PRIV_AENC_H__

/* add include here */
#include "hi_mpi_aenc.h"
#include "hi_unf_sound.h"
#include "hi_module.h"
#include "hi_debug.h"
#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define AENC_INSTANCE_MAXNUM 3     /* max encoder instance */

#define AENC_MIN_INPUT_BUFFER_SIZE (1024 * 256)
#define AENC_MAX_INPUT_BUFFER_SIZE (1024 * 512 * 4)
#define AENC_DEFAULT_INPUT_BUFFER_SIZE (1024 * 512)
#define AENC_DEFAULT_OUTBUF_NUM 32

#define AENC_MAX_SRC_FRAC (48000 / 8000)
#define AENC_MAX_CHANNELS 2
#define ANEC_MAX_SMAPLEPERFRAME (2048)
#define AENC_MAX_POSTPROCESS_FRAME (ANEC_MAX_SMAPLEPERFRAME * AENC_MAX_CHANNELS * AENC_MAX_SRC_FRAC)
#define AENC_WORK_BUFFER_NUM 2

typedef enum 
{
    ANEC_SOURCE_AI         = 0,
    ANEC_SOURCE_CAST,
    ANEC_SOURCE_VIRTRACK,
    ANEC_SOURCE_BUTT
} ANEC_SOURCE_TYPE_E;
typedef struct hi_AENC_INFO_ATTACH_S
{
    ANEC_SOURCE_TYPE_E eType;
    HI_HANDLE hSource;
}AENC_INFO_ATTACH_S;

typedef enum
{
    AENC_CMD_CTRL_START = 0,
    AENC_CMD_CTRL_STOP,
    AENC_CMD_CTRL_BUTT
} AENC_CMD_CTRL_E;     


typedef enum
{
    AENC_CMD_PROC_SAVE_PCM = 0,
    AENC_CMD_PROC_SAVE_ES,
    AENC_CMD_PROC_BUTT
} AENC_CMD_SAVE_E;     

typedef struct hiAENC_PROC_ITEM_S
{
    AENC_INFO_ATTACH_S stAttach;
    HI_BOOL            bAencWorkEnable;
    HI_U32             u32CodecID;
    HI_CHAR            szCodecType[32];
    HI_U32             u32SampleRate;
    HI_U32             u32BitWidth;
    HI_U32             u32Channels;

    HI_BOOL            bAutoSRC;
    HI_U32             u32EncFrame;
    HI_U32             u32ErrFrame;

    HI_U32 u32InBufSize;
    HI_VIRT_ADDR_T tInBufRead;
    HI_VIRT_ADDR_T tInBufWrite;

    HI_U32 u32OutFrameNum;
    HI_U32 u32OutFrameRIdx;
    HI_U32 u32OutFrameWIdx;
    HI_U32 u32DbgSendBufCount_Try;
    HI_U32 u32DbgSendBufCount;
    HI_U32 u32DbgReceiveStreamCount_Try;
    HI_U32 u32DbgReceiveStreamCount;
    HI_U32 u32DbgReleaseStreamCount_Try;
    HI_U32 u32DbgReleaseStreamCount;
    HI_U32 u32DbgTryEncodeCount;

    AENC_CMD_CTRL_E         enPcmCtrlState;
    AENC_CMD_CTRL_E         enEsCtrlState;
    HI_U32                  u32SavePcmCnt;
    HI_U32                  u32SaveEsCnt;
    HI_CHAR                 filePath[512];           
} AENC_PROC_ITEM_S;

/* use macro to check parameter */
#define  HI_MPI_AENC_RetUserErr(DrvErrCode, aenc_mutex) \
    do                                                  \
    {                                                   \
        HI_S32 retvalerr;                               \
        if (HI_SUCCESS != DrvErrCode)                   \
        {                                               \
            switch (DrvErrCode)                         \
            {                                           \
            case  HI_ERR_AENC_IN_BUF_FULL:           \
            case  HI_ERR_AENC_DEV_NOT_OPEN:          \
            case  HI_ERR_AENC_NULL_PTR:          	 \
            case  HI_ERR_AENC_INVALID_PARA:          \
            case  HI_ERR_AENC_OUT_BUF_FULL:          \
            case  HI_ERR_AENC_INVALID_OUTFRAME:      \
            case  HI_ERR_AENC_DATASIZE_EXCEED:       \
            case  HI_ERR_AENC_OUT_BUF_EMPTY:         \
                retvalerr = DrvErrCode;              \
                break;                               \
            default:                                 \
                retvalerr = HI_FAILURE;              \
                break;                               \
            }                                        \
            if (HI_ERR_AENC_IN_BUF_FULL == retvalerr)          \
                HI_INFO_AENC(" DriverErrorCode =0x%x\n",retvalerr); \
            else if (HI_ERR_AENC_OUT_BUF_EMPTY == retvalerr)   \
                HI_INFO_AENC(" DriverErrorCode =0x%x\n",retvalerr); \
            else                                               \
                HI_ERR_AENC(" DriverErrorCode =0x%x\n",retvalerr);  \
            AENC_UNLOCK(aenc_mutex); \
            return retvalerr;                        \
        }                                            \
    } while (0)

#define  HI_MPI_AENC_RetUserErr2(DrvErrCode, aenc_mutex) \
    do                                                   \
    {                                                    \
   	    HI_ERR_AENC(" DriverErrorCode =0x%x\n",DrvErrCode); \
        AENC_UNLOCK(aenc_mutex); \
        return DrvErrCode; \
    } while (0)			

#define CHECK_AENC_CH_CREATE(hAenc) \
    do                                                  \
    {                                                   \
        if (!g_s32AencInitCnt)  \
        {  \
            HI_ERR_AENC("AENC  device state err: please int aenc init first\n");  \
            return HI_FAILURE;  \
        }  \
        if (hAenc >= AENC_INSTANCE_MAXNUM)             \
        {                                               \
            HI_ERR_AENC(" AENC  device not open handleAenc=%d !\n",  hAenc);          \
            return HI_ERR_AENC_DEV_NOT_OPEN;            \
        }                                                \
        if (HI_FALSE == g_pstAencChan[hAenc]->beAssigned)  \
        {                                               \
            HI_ERR_AENC("AENC  device not open!\n");          \
            return HI_ERR_AENC_DEV_NOT_OPEN;              \
        }                                               \
    } while (0)

#define CHECK_AENC_NULL_PTR(ptr) \
    do                                                  \
    {                                                   \
        if (NULL == ptr)                             \
        {                                               \
            HI_ERR_AENC("invalid NULL poiner!\n");          \
            return HI_ERR_AENC_NULL_PTR;                  \
        }                                               \
    } while (0)

#define CHECK_AENC_OPEN_FORMAT(rate, ch, width, bInterleaved) \
    do                                                  \
    {                                                   \
        if (rate <  HI_UNF_SAMPLE_RATE_8K || rate > HI_UNF_SAMPLE_RATE_48K)    \
        {                                           \
            HI_ERR_AENC("invalid  Pcm Format: HA Encoder only support 8K~48K samplerate \n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }         \
        if (HI_FALSE == bInterleaved)    \
        {                                           \
            HI_ERR_AENC("invalid  Pcm Format: HA Encoder only support 16bit-Interleaved format \n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
        if (16 != width)    \
        {                                           \
            HI_ERR_AENC("invalid  Pcm Format: HA Encoder only support 16bit-Interleaved format \n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
        if (2 != ch && 1 != ch)                  \
        {                                           \
            HI_ERR_AENC("invalid Pcm Format: HA Encoder only support 1 and 2 channel\n");   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
    } while (0)

#define CHECK_AENC_PCM_SAMPLESIZE(PcmSamplesPerFrame) \
    do                                                  \
    {                                                   \
        if (PcmSamplesPerFrame > ANEC_MAX_SMAPLEPERFRAME)    \
        {                                           \
            HI_ERR_AENC("invalid  AO Pcm Format: Pcm SamplesPerFrame  =%d \n",PcmSamplesPerFrame);   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
    } while (0)

#if 0
#define CHECK_AENC_PCM_FORMAT(ch, bInterleaved) \
    do                                                  \
    {                                                   \
        if (HI_FALSE == bInterleaved)    \
        {                                           \
            if (ch > 2)                  \
            {                                           \
                HI_ERR_AENC("invalid  Pcm Format: if none-Interleaved, must sure channel <=2 ! \n");   \
                return HI_ERR_AENC_INVALID_PARA;           \
            }                                           \
        }                                           \
        if (ch > 2)                  \
        {                                           \
            if ((ch != 6) && (ch != 8))                  \
            {                                           \
                HI_ERR_AENC("invalid Pcm Format: HA Encoder  only support 5.1 or 7.1 format channel=%d\n", ch);   \
                return HI_ERR_AENC_INVALID_PARA;           \
            }                                           \
        }                                           \
    } while (0)
#endif 

#define CHECK_AENC_PCM_CHANNEL(ch) \
    do                                                  \
    {                                                   \
        if (ch > 2)                  \
        {                                           \
            HI_ERR_AENC("invalid  pcm channel[%d]: must sure channel <=2 ! \n", ch);   \
            return HI_ERR_AENC_INVALID_PARA;           \
        }                                           \
    } while (0)

#define CHECK_AENC_PCM_SAMPLERATE(rate) \
     do                                                  \
    {                                                   \
        switch (rate)                                \
        {                                               \
        case  HI_UNF_SAMPLE_RATE_8K:                    \
        case  HI_UNF_SAMPLE_RATE_11K:                   \
        case  HI_UNF_SAMPLE_RATE_12K:                   \
        case  HI_UNF_SAMPLE_RATE_16K:                   \
        case  HI_UNF_SAMPLE_RATE_22K:                   \
        case  HI_UNF_SAMPLE_RATE_24K:                   \
        case  HI_UNF_SAMPLE_RATE_32K:                   \
        case  HI_UNF_SAMPLE_RATE_44K:                   \
        case  HI_UNF_SAMPLE_RATE_48K:                   \
        case  HI_UNF_SAMPLE_RATE_88K:                   \
        case  HI_UNF_SAMPLE_RATE_96K:                   \
        case  HI_UNF_SAMPLE_RATE_176K:                  \
        case  HI_UNF_SAMPLE_RATE_192K:                  \
            break;                                      \
        default:                                        \
            HI_WARN_AO("invalid samplerate[%d]\n", rate);    \
            return HI_ERR_AO_INVALID_PARA;                        \
            }                                                       \
        } while (0)   

#define CHECK_AENC_PCM_BITWIDTH(bitwidth) \
            do                                                  \
            {                                                   \
                if (16 != bitwidth && 24 != bitwidth)                  \
                {                                           \
                    HI_ERR_AENC("invalid  pcm Bitwidth[%d], must sure 16bit or 24bit\n", bitwidth);   \
                    return HI_ERR_AENC_INVALID_PARA;           \
                }                                           \
            } while (0)

        
/*Define Debug Level For HI_ID_AO                     */
#define HI_FATAL_AENC(fmt...) \
    HI_FATAL_PRINT(HI_ID_AENC, fmt)

#define HI_ERR_AENC(fmt...) \
    HI_ERR_PRINT(HI_ID_AENC, fmt)

#define HI_WARN_AENC(fmt...) \
    HI_WARN_PRINT(HI_ID_AENC, fmt)

#define HI_INFO_AENC(fmt...) \
    HI_INFO_PRINT(HI_ID_AENC, fmt)

/********************** Global Variable declaration **************************/
#define DRV_AENC_DEVICE_NAME "hi_aenc"

/* 'IOC_TYPE_ADEC' means ADEC magic macro */
#define     DRV_AENC_PROC_INIT _IOW(HI_ID_AENC, 0, HI_U32)
#define     DRV_AENC_PROC_EXIT _IO(HI_ID_AENC, 1)

/******************************* API declaration *****************************/
HI_S32 AENC_Close (HI_U32 hAenc);
HI_S32 AENC_DeInit(HI_VOID);
HI_S32 AENC_Init(const HI_CHAR* pszodecNameTable[]);
HI_S32 AENC_Open(HI_HANDLE *phAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);
HI_S32 AENC_ReceiveStream (HI_HANDLE hAenc, AENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs);
HI_S32 AENC_ReleaseStream(HI_HANDLE hAenc, const AENC_STREAM_S *pstStream);
HI_S32 AENC_SendBuffer (HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
HI_S32 AENC_SetAutoSRC (HI_HANDLE hAenc, HI_BOOL bEnable);
HI_S32 AENC_SetConfigEncoder(HI_HANDLE hAenc, HI_VOID *pstConfigStructure);
HI_S32 AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName);
HI_S32 AENC_ShowRegisterEncoder(HI_VOID);
HI_S32 AENC_SetEnable(HI_HANDLE hAenc, HI_BOOL bEnable);
HI_S32 AENC_AttachInput(HI_HANDLE hAenc, HI_HANDLE hSource);
HI_S32 AENC_GetAttachSrc(HI_HANDLE hAenc, HI_HANDLE* hSrc);
HI_S32 AENC_DetachInput(HI_HANDLE hAenc);
HI_S32 AENC_SetAttr(HI_HANDLE hAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);
HI_S32 AENC_GetAttr(HI_HANDLE hAenc, HI_UNF_AENC_ATTR_S *pstAencAttr);

#ifdef __cplusplus
}
#endif
#endif /* __MPI_PRIV_AENC_H__ */
