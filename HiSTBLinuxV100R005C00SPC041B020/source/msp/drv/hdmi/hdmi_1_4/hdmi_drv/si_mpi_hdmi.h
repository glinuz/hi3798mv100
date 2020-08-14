#ifndef __MPI_HDMI_H__
#define __MPI_HDMI_H__

#include "hi_unf_hdmi.h"

#define _HI_UNF_HDMI_DRV 0

#define MAX_SIZE 0x3000L

#define HDMI_MALLOC malloc
//#define HDMI_DEBUG printk


HI_U32 SI_OpenHdmiDevice(void);
void SI_DisableHdmiDevice(void);
void SI_EnableHdmiDevice(void);
void SI_CloseHdmiDevice(void);
void SI_PoweDownHdmiDevice(void);

void SI_WriteDefaultConfigToEEPROM( void);
HI_U8 SI_TimerHandler(void);
void SI_SHAHandler(void);
void SI_TX_SetHDMIMode(HI_U8 Enabled);
void DelayMS( HI_U32 count);
void SI_CEC_Event_Handler( void );

void SI_SetHdmiAudio(HI_U8 Enabled);
void SI_SetHdmiVideo(HI_U8 Enabled);
HI_U8 SI_BlockWriteEEPROM(HI_U8 NBytes, HI_U16 Addr,  HI_U8 *Data);
HI_U8 SI_ReadByteEEPROM(HI_U8 RegAddr);
void SI_WriteByteEEPROM(HI_U8 RegAddr, HI_U8 RegData);


#endif

