/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_ai.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : ai
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_DRV_AI_H__
#define __HI_DRV_AI_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_unf_ai.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define AI_MAX_TOTAL_NUM    (4)
#define AI_MAX_HANDLE_ID    ((HI_ID_AI << 16) | AI_MAX_TOTAL_NUM)
#define AI_MIN_HANDLE_ID    (HI_ID_AI << 16)
#define AI_CHNID_MASK       (0xffff)

/* Define Debug Level For HI_ID_AI */
#define HI_FATAL_AI(fmt...) \
    HI_FATAL_PRINT(HI_ID_AI, fmt)

#define HI_ERR_AI(fmt...) \
    HI_ERR_PRINT(HI_ID_AI, fmt)

#define HI_WARN_AI(fmt...) \
    HI_WARN_PRINT(HI_ID_AI, fmt)

#define HI_INFO_AI(fmt...) \
    HI_INFO_PRINT(HI_ID_AI, fmt)

#define CHECK_AI_NULL_PTR(p) \
    do { \
        if (HI_NULL == p) \
        { \
            HI_ERR_AI("NULL pointer!\n"); \
            return HI_ERR_AI_NULL_PTR; \
        } \
    } while(0)

#define CHECK_AI_ID(handle) \
    do { \
        if ((AI_MAX_HANDLE_ID <= handle) || (AI_MIN_HANDLE_ID > handle)) \
        { \
            HI_ERR_AI("Invalid Ai id 0x%x\n", handle); \
            return HI_ERR_AI_INVALID_ID; \
        } \
    } while (0)

#define CHECK_AI_CHN_STATE(hAI, state) \
    do { \
        if (HI_NULL == state) \
        { \
            HI_ERR_AI("AI chn(%d) not open!\n", hAI & AI_CHNID_MASK); \
            return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_SAMPLERATE(samplerate) \
    do \
    { \
        switch (samplerate) \
        { \
            case HI_UNF_SAMPLE_RATE_8K:   \
            case HI_UNF_SAMPLE_RATE_11K:  \
            case HI_UNF_SAMPLE_RATE_12K:  \
            case HI_UNF_SAMPLE_RATE_16K:  \
            case HI_UNF_SAMPLE_RATE_22K:  \
            case HI_UNF_SAMPLE_RATE_24K:  \
            case HI_UNF_SAMPLE_RATE_32K:  \
            case HI_UNF_SAMPLE_RATE_44K:  \
            case HI_UNF_SAMPLE_RATE_48K:  \
            case HI_UNF_SAMPLE_RATE_88K:  \
            case HI_UNF_SAMPLE_RATE_96K:  \
            case HI_UNF_SAMPLE_RATE_176K: \
            case HI_UNF_SAMPLE_RATE_192K: \
                break; \
            default: \
                HI_WARN_AI("Invalid out sample rate %d\n", samplerate); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_BCLKDIV(BclkDiv) \
    do \
    { \
        switch (BclkDiv) \
        { \
            case HI_UNF_I2S_BCLK_1_DIV: \
            case HI_UNF_I2S_BCLK_2_DIV: \
            case HI_UNF_I2S_BCLK_3_DIV: \
            case HI_UNF_I2S_BCLK_4_DIV: \
            case HI_UNF_I2S_BCLK_6_DIV: \
            case HI_UNF_I2S_BCLK_8_DIV: \
            case HI_UNF_I2S_BCLK_12_DIV: \
            case HI_UNF_I2S_BCLK_24_DIV: \
            case HI_UNF_I2S_BCLK_32_DIV: \
            case HI_UNF_I2S_BCLK_48_DIV: \
            case HI_UNF_I2S_BCLK_64_DIV: \
                break; \
            default: \
                HI_WARN_AI("Invalid BclkDiv %d\n", BclkDiv); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_MCLKDIV(MclkSel) \
    do \
    { \
        switch (MclkSel) \
        { \
            case HI_UNF_I2S_MCLK_128_FS: \
            case HI_UNF_I2S_MCLK_256_FS: \
            case HI_UNF_I2S_MCLK_384_FS: \
            case HI_UNF_I2S_MCLK_512_FS: \
            case HI_UNF_I2S_MCLK_768_FS: \
            case HI_UNF_I2S_MCLK_1024_FS: \
                break; \
            default: \
                HI_WARN_AI("Invalid mclk sel %d\n", MclkSel); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_CHN(Chn) \
    do \
    { \
        switch (Chn) \
        { \
            case HI_UNF_I2S_CHNUM_1: \
            case HI_UNF_I2S_CHNUM_2: \
            case HI_UNF_I2S_CHNUM_8: \
                break; \
            default: \
                HI_WARN_AI("Invalid chn %d\n", Chn); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_BITDEPTH(BitDepth) \
    do \
    { \
        switch (BitDepth) \
        { \
            case HI_UNF_I2S_BIT_DEPTH_16: \
            case HI_UNF_I2S_BIT_DEPTH_24: \
                break; \
            default: \
                HI_WARN_AI("Invalid BitDepth %d\n", BitDepth); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)


#define CHECK_AI_PCMDELAY(PcmDelayCycle) \
    do \
    { \
        switch (PcmDelayCycle) \
        { \
            case HI_UNF_I2S_PCM_0_DELAY: \
            case HI_UNF_I2S_PCM_1_DELAY: \
            case HI_UNF_I2S_PCM_8_DELAY: \
            case HI_UNF_I2S_PCM_16_DELAY: \
            case HI_UNF_I2S_PCM_32_DELAY: \
                break; \
            default: \
                HI_WARN_AI("Invalid PcmDelayCycle %d\n", PcmDelayCycle); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

#define CHECK_AI_HdmiDataFormat(HdmiDataFormat) \
    do \
    { \
        switch (HdmiDataFormat) \
        { \
            case HI_UNF_AI_HDMI_FORMAT_LPCM: \
            case HI_UNF_AI_HDMI_FORMAT_LBR: \
            case HI_UNF_AI_HDMI_FORMAT_HBR: \
                break; \
            default: \
                HI_ERR_AI("Invalid Hdmi DataFormat %d\n", HdmiDataFormat); \
                return HI_ERR_AI_INVALID_PARA; \
        } \
    } while (0)

typedef struct
{
    HI_PHYS_ADDR_T tPhyBaseAddr;
    HI_U32         u32Read;
    HI_U32         u32Write;
    HI_U32         u32Size;
    /* user space virtual address */
    HI_VIRT_ADDR_T tUserVirAddr;
    /* kernel space virtual address */
    HI_VIRT_ADDR_T tKernelVirAddr;
} AI_BUF_ATTR_S;

// for alsa to create AI
typedef struct
{
    HI_HANDLE         hAI;
    HI_UNF_AI_E       enAiPort;
    HI_UNF_AI_ATTR_S  stAttr;

    HI_BOOL           bAlsa;
    HI_VOID*          pAlsaPara;
} AI_DRV_Create_Param_S;

typedef struct
{
    HI_U32         u32AqcTryCnt;
    HI_U32         u32AqcCnt;
    HI_U32         u32RelTryCnt;
    HI_U32         u32RelCnt;
#ifdef __DPT__
    HI_UNF_AI_HDMI_DATA_TYPE_E enDataType;
#endif
} AI_PROC_INFO_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
