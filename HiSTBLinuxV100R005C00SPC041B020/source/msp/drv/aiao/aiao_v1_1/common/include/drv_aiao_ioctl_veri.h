/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_aiaov2.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_DRV_AIAOV2_H__
#define __HI_DRV_AIAOV2_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hal_aiao_common.h"

#define  AIAO_VERI_PROC_NAME  "aiao_veri"

typedef struct hiAIAO_Sys_Crg_S
{
    HI_U32 u32CrgSkip;
} hiAIAO_Sys_Crg_S;

typedef struct
{
    HI_UNF_HDMI_ID_E enHdmi;
    HDMI_AUDIOINTERFACE_E   enSoundIntf;         /**<the origin of Sound,suggestion set HI_UNF_SND_INTERFACE_I2S,the parameter need consistent with Ao input *//**<CNcomment:HDMI音频来源, 建议HI_UNF_SND_INTERFACE_I2S,此参数需要与AO输入保持一致 */
    HI_U32                  u32Channels;         //先channel和multy channel都保留，后续在内核态干掉multy channel
    HI_UNF_SAMPLE_RATE_E    enSampleRate;        /**<the samplerate of audio,this parameter consistent with AO config *//**<CNcomment:PCM音频采样率,此参数需要与AO的配置保持一致 */
    HI_UNF_BIT_DEPTH_E      enBitDepth;  //目前默认配16bit    /**<the audio bit depth,defualt 16,this parameter consistent with AO config*//**<CNcomment:音频位宽，默认为16,此参数需要与AO的配置保持一致 */
} AIAO_HDMI_AUDIO_ATRR_S;

#ifdef __aarch64__
#error "aiao veri only support 32bit driver!"
#endif

/***************************** Macro Definition ******************************/
/********************************************************
  AIAO command code definition
 *********************************************************/

/* AIAO base code of command code*/
#define CMD_AIAO_VERI_IOCTL 0x80

/* global */
#define CMD_AIAO_GET_CAPABILITY _IOWR (HI_ID_AIAO, 0x80, AIAO_Port_GetCapability_S)
#define CMD_AIAO_SET_SYSCRG _IOWR (HI_ID_AIAO, 0x81, hiAIAO_Sys_Crg_S)
/* global debug */
#define CMD_AIAO_RW_REGISTER _IOWR(HI_ID_AIAO, 0x82, AIAO_Dbg_Reg_S)

/* AIAO PORT */
#define CMD_AIAO_PORT_OPEN _IOWR (HI_ID_AIAO, 0x90, AIAO_Port_Open_S)
#define CMD_AIAO_PORT_CLOSE _IOWR  (HI_ID_AIAO, 0x91, AIAO_Port_Close_S)
#define CMD_AIAO_PORT_START _IOWR  (HI_ID_AIAO, 0x92, AIAO_Port_Start_S)
#define CMD_AIAO_PORT_STOP _IOWR  (HI_ID_AIAO, 0x93, AIAO_Port_Stop_S)
#define CMD_AIAO_PORT_MUTE _IOWR  (HI_ID_AIAO, 0x95, AIAO_Port_Mute_S)
#define CMD_AIAO_PORT_TRACKMODE _IOWR  (HI_ID_AIAO, 0x96, AIAO_Port_TrackMode_S)
#define CMD_AIAO_PORT_VOLUME _IOWR  (HI_ID_AIAO, 0x97, AIAO_Port_Volume_S)
#define CMD_AIAO_PORT_GET_USERCONFIG _IOWR  (HI_ID_AIAO, 0x98, AIAO_Port_GetUserConfig_S)
#define CMD_AIAO_PORT_GET_STATUS _IOWR  (HI_ID_AIAO, 0x99, AIAO_Port_GetSTATUS_S)
#define CMD_AIAO_PORT_SEL_SPDIFOUTSOURCE _IOWR  (HI_ID_AIAO, 0x9a, AIAO_Port_SelectSpdifSource_S)
#define CMD_AIAO_PORT_SET_SPDIFOUTPORT _IOWR  (HI_ID_AIAO, 0x9b, AIAO_Port_SetSpdifOutPort_S)
#define CMD_AIAO_PORT_SET_I2SDATASEL _IOWR  (HI_ID_AIAO, 0x9c, AIAO_Port_I2SDataSel_S)

#define CMD_AIAO_PORT_READ_DATA _IOWR(HI_ID_AIAO, 0xb0, AIAO_Port_ReadData_S)        /* get port data */
#define CMD_AIAO_PORT_WRITE_DATA _IOWR(HI_ID_AIAO, 0xb1, AIAO_Port_WriteData_S)       /* write port data */
#define CMD_AIAO_PORT_PREPARE_DATA _IOWR(HI_ID_AIAO, 0xb2, AIAO_Port_WriteData_S)       /* prepare port data */
#define CMD_AIAO_PORT_QUERY_BUF _IOWR(HI_ID_AIAO, 0xb3, AIAO_Port_BufStatus_S)       /* get port status */

#define CMD_AIAO_PORT_UPDATE_RPTR _IOWR(HI_ID_AIAO, 0xb8, AIAO_Port_ReadData_S)        /* get port data */
#define CMD_AIAO_PORT_UPDATE_WPTR _IOWR(HI_ID_AIAO, 0xb9, AIAO_Port_WriteData_S)       /* write port data */

/* HDMI TX */
#define CMD_AIAO_HDMI_SETAUDIO_ATTR _IOWR(HI_ID_AIAO, 0xc0, AIAO_HDMI_AUDIO_ATRR_S)        /* get port data */

#ifdef HI_AIAO_TIMER_SUPPORT
#define CMD_AIAO_TIMER_CREATE    _IOWR  (HI_ID_AIAO, 0xd0, AIAO_Timer_Create_S)
#define CMD_AIAO_TIMER_DESTROY   _IOWR  (HI_ID_AIAO, 0xd1, AIAO_Timer_Destroy_S)
#define CMD_AIAO_TIMER_SETATTR   _IOW   (HI_ID_AIAO, 0xd2, AIAO_TIMER_Attr_S)
#define CMD_AIAO_TIMER_SETENABLE _IOW   (HI_ID_AIAO, 0xd3, AIAO_TIMER_Enable_S)
#define CMD_AIAO_TIMER_GETSTATUS _IOWR  (HI_ID_AIAO, 0xd4, AIAO_TIMER_Status_S)
#endif
/*TianLai*/
#define CMD_AIAO_TIANLAI_INIT _IOW(HI_ID_AIAO, 0xe0, S5_TIANLAI_SAMPLERATE_E)
#define CMD_AIAO_TIANLAI_DEINIT _IOW(HI_ID_AIAO, 0xe1, HI_U32)
#define CMD_AIAO_TIANLAI_SETINSRC _IOW(HI_ID_AIAO, 0xe2, S5_TIANLAI_LINEIN_SEL_E)

#endif  // __HI_DRV_AIAOV2_H__
