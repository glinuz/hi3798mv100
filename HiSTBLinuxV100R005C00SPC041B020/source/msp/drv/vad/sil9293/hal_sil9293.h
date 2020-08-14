/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hal_sil9293.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/03/08
  Description   :
  History       :
  1.Date        : 2013/03/08
    Author      : t00202585,j00169368
    Modification: Created file

******************************************************************************/

#ifndef __HAL_HDMIRX_H__
#define __HAL_HDMIRX_H__

#include "drv_hdmirx_ioctl.h"

#define HDMIRX_SEC_DEV 0x68
#define HDMIRX_RXC_DEV 0xD0

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define AUDIO_CHST4__FS_44 0x00 // Fs = 44.1 kHz
#define AUDIO_CHST4__FS_UNKNOWN 0x01 //
#define AUDIO_CHST4__FS_48 0x02 // Fs = 48 kHz
#define AUDIO_CHST4__FS_32 0x03 // Fs = 32 kHz
#define AUDIO_CHST4__FS_22 0x04 // Fs = 22.05 kHz
#define AUDIO_CHST4__FS_24 0x06 // Fs = 24 kHz
#define AUDIO_CHST4__FS_88 0x08 // Fs = 88.2 kHz
#define AUDIO_CHST4__FS_768 0x09 // Fs = 768 kHz (HBR Audio 4x192kHz)
#define AUDIO_CHST4__FS_96 0x0A // Fs = 96 kHz
#define AUDIO_CHST4__FS_176 0x0C // Fs = 176.4 kHz
#define AUDIO_CHST4__FS_192 0x0E // Fs = 192 kHz

typedef enum hiHDCP_STATE_E
{
    HI_HDCP_STATE_FAILED = 0,
    HI_HDCP_STATE_TRYING,
    HI_HDCP_STATE_DECRYPT,
    HI_HDCP_STATE_SUCCESS,
    HI_HDCP_STATE_BUTT,
} HI_HDCP_STATE_E;

HI_S32	SIL9293_HAL_Init(HI_VOID);
HI_VOID SIL9293_HAL_DeInit(HI_VOID);
HI_VOID SIL9293_HAL_ClearInt(HI_VOID);
HI_VOID SIL9293_HAL_Reset(HI_VOID);
HI_VOID SIL9293_HAL_ResetDcFIFO(HI_VOID);
HI_VOID SIL9293_HAL_ResetHdcp(HI_VOID);
HI_S32	SIL9293_Thread(HI_VOID *pData);

/* VIDEO */
HI_BOOL SIL9293_HAL_HasAvi(HI_VOID);
HI_VOID SIL9293_HAL_UpOffset(HI_BOOL bRGB);
HI_VOID SIL9293_HAL_UpTmdsDcCtl(HI_VOID);

HI_VOID SIL9293_HAL_SetPowerOn(HI_BOOL bPowerOn);
HI_VOID SIL9293_HAL_SetVideoAuto(HI_VOID);
HI_VOID SIL9293_HAL_SetDDCEn(HI_BOOL bDDCEn);
HI_VOID SIL9293_HAL_SetHPG(HI_BOOL bHigh);
HI_VOID SIL9293_HAL_SetTMDSEn(HI_BOOL bTMDSEn);

HI_VOID SIL9293_HAL_GetPixClk10KHZ(HI_U32 *pu32PixClk);
HI_VOID SIL9293_HAL_GetStable(HI_BOOL *pbStable);
HI_VOID SIL9293_HAL_GetClock(HI_BOOL *pbHasClock);
HI_VOID SIL9293_HAL_GetWH(HI_U32 *pu32W, HI_U32 *pu32H);
HI_VOID SIL9293_HAL_GetHVTotal(HI_U32 *pu32HTotal, HI_U32 *pu32VTotal);
HI_VOID SIL9293_HAL_GetFrameRate(HI_U32 *pu32FrameRate);
HI_VOID SIL9293_HAL_GetInterlace(HI_BOOL *pbInterlace);
HI_VOID SIL9293_HAL_GetOverSample(HI_OVERSAMPLE_MODE_E *penOverSampleMode);
HI_VOID SIL9293_HAL_GetDviMode(HI_BOOL *pbDviMode);
HI_VOID SIL9293_HAL_GetPixFmt(HI_PIXEL_FORMAT_E *penPixelFmt);
HI_VOID SIL9293_HAL_GetColorSpace(HI_COLOR_SPACE_E *penColorSpace);
HI_VOID SIL9293_HAL_GetColorDepth(HI_PIXEL_BITWIDTH_E *penBitWidth);
HI_VOID SIL9293_HAL_GetICLK(HI_OVERSAMPLE_MODE_E *penICLK);
HI_VOID SIL9293_Read(HI_U32 u32Dev, HI_U32 u32Addr, HI_U8 *pu8Value);
HI_VOID SIL9293_Write(HI_U32 u32Dev, HI_U32 u32Addr, HI_U8 u8Value);
HI_VOID SIL9293_HAL_SetBt656Enable(HI_BOOL bBt656Enable);


/* AUDIO */
HI_VOID SIL9293_HAL_SetAudioAuto(HI_VOID);
HI_VOID SIL9293_HAL_GetCTS(HI_U32 *pu32CTS);
HI_VOID SIL9293_HAL_GetN(HI_U32 *pu32N);
HI_VOID SIL9293_HAL_SetFSSel(HI_BOOL bSW);
HI_VOID SIL9293_HAL_CalcSWFS(HI_U32 *pu32SWFS);
HI_VOID SIL9293_HAL_UpdateFS(HI_U32 u32SWFS);
HI_VOID SIL9293_HAL_ResetAudioFIFO(HI_VOID);
HI_VOID SIL9293_HAL_ResetACR(HI_VOID);
HI_VOID SIL9293_HAL_ResetAAC(HI_VOID);

HI_VOID SIL9293_HAL_GetHdcpStatus(HI_HDCP_STATE_E *penHdcpState);

#endif // __HAL_HDMIRX_H__
