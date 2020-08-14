/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hdmi_test_cmd.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2010/07/08
Last Modified :
Description   : HDMI TEST COMMAND H FILE
Function List :
History       :
******************************************************************************/
#ifndef __HDMI_TEST_CMD_H__
#define __HDMI_TEST_CMD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "hi_unf_common.h"

#define __HDMI_COMPATIBILITY_TEST_DEMO__

typedef struct tagHDMI_SAMPLE_CONTEXT_S
{
    HI_U32          HDMI_Status;                 /* Init/Term/Enable/Disable */
    HI_U32          HDMI_Force_Flag;             /* TRUE:Force, False: Ignore, Mutex with DVI_Forc_Flag */
    HI_U32          DVI_Forc_Flag;               /* TRUE:Force, False: Ignore, Mutex with DVI_Forc_Flag */
    
    HI_U32          HDMI_Video_Input_Timing;     /* 720p60, 1080i50, ... */
    HI_U32          HDMI_Video_Input_mode;       /* RGB444, YPbPr422, YPbPr444 */
    HI_U32          HDMI_Video_Input_ApectRate;  /* 4:3, 16:9, ... */
    
    HI_U32          HDMI_Video_Output_Timing;    /* Should be the same as Input */
    HI_U32          HDMI_Video_Output_mode;      /* SI can change color mode: txvptbl.h */
    HI_U32          HDMI_Video_Output_ApectRate; /* ? Should be the same as Input */
    
    HI_U32          DeepColorFlag;               /* Deep Color enable. SetDeepColor() in txvidp.c */
    
    HI_U32          HDMI_Audio_Input_Type;        /* PCM(uncompress), AAC(compress), AC3(compress), ... */
    HI_U32          HDMI_Audio_Input_Freq;       /* 32K, 44.1k, 48K, ... */
    HI_U32          HDMI_Audio_Input_Path;       /* I2S or SPDIF */
    HI_U32          HDMI_Audio_Input_Channel_Num;/* Only useful for PCM audio data, 2/6.1 channel */
    
    HI_U32          HDMI_Audio_Output_Type;      /* Should be the same as Input */
    HI_U32          HDMI_Audio_Output_Freq;      /* Should be the same as Input */
    HI_U32          HDMI_Audio_Output_Channel_Num;/* Should be the same as Input */
    
    HI_U32          HDCP_Forc_Flag;               /* TRUE:Force, False: Ignore */
    HI_U32          HDCP_Staus;
    
    HI_U32          Video_Enable;                 /* TRUE:STB have transfer video data to HDMI IP */
    HI_U32          Video_Component_type;         /* MPEG1/MPEG2, H.264, VC1, MPEGP4 ... */
    HI_U32          Video_Stream_Type;            /* ES/PES/TS, ... */
    
    HI_U32          Audio_Enable;                 /* TRUE:STB have transfer audio data to HDMI IP */
    HI_U32          Audio_Component_type;         /* PCM, MPEG1/MPEG2, AAC, AC3, DDPLUS, ... */
    HI_U32          Audio_Stream_Type;            /* ES/PES/TS, ... */
    
    HI_U32          HDCP_CEC_Flag;               /* TRUE:Force, False: Ignore */
    HI_U32          CEC_Staus;
    
    //Semaphore to mutext operation ?
    //EDID Structure.
    
}HDMI_SAMPLE_CONTEXT_S, * HDMI_SAMPLE_CONTEXT_PTR;

typedef struct
{
    HI_BOOL PlugInFlag;
    HI_U32 vpid;
    HI_U32 apid;
    HI_UNF_VCODEC_TYPE_E vdectype;
    HI_U32 AdecType;
    HI_U32 AOutCompressType;
    HI_UNF_ENC_FMT_E Fmt;
    HI_BOOL HDCPOKFlag;
    HI_UNF_HDMI_DEEP_COLOR_E DeepColorFlag;
}HDMI_Compat_t;

typedef struct
{
    HI_U32 Index;
    HI_U8  Index_String[20];
}HDMI_INPUT_PARAM_S;

typedef struct
{
    HI_U32 Index;
    HI_U32 TimingIndex;
    HI_U32 ColorMode;
    HI_U32 DeepColor;
    HI_U32 AspectRatio;
    HI_U8  Index_String[48];
}HDMI_CTS_TEST_PARAM_S;

//void HDMI_Hisi_LED_Display(HI_U32 u32HDCPFlag, HI_UNF_ENC_FMT_E nFmt);
//HI_U32 hdmi_debug(HI_UNF_HDMI_SINK_CAPABILITY_S *pCapbility);
HI_U32 HDMI_Timing_Switch(HI_U8 IncreaseFlag);
HI_U32 hdmi_hdcp(HI_U32 hdcpflag);
HI_U32 hdmi_video_timing(HI_U32 u32TimingIndex);
HI_U32 hdmi_deepcolor(HI_U32 u32DeepColorFlag);
HI_U32 hdmi_a_compress(HI_U32 compressFlag);
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_U32 HDMI_Audio_Compress_Switch(HDMI_Compat_t *pHDMICompat);
HI_U32 HDMI_VideoDeepColor_Switch(HDMI_Compat_t *pHDMICompat);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

#define HDMI_BASE_REG_ADDR 0xf8ce0000

#ifdef __HDMI_COMPATIBILITY_TEST_DEMO__
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_S32 HDMI_Compatitbility_Menu_Setup(void);
HI_S32 HDMI_Compatitbility_Menu_TextOut(HDMI_Compat_t *pHMDICompat);
HI_S32 HDMI_Compatitbility_Menu_Exit(void);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HDMI_TEST_CMD_H__ */
/*------------------------------------END-------------------------------------*/
