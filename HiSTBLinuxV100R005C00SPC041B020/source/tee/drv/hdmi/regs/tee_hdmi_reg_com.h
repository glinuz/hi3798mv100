#ifndef __TEE_REG_HDMI_COM_H__
#define __TEE_REG_HDMI_COM_H__

#include "hi_type.h"

HI_U32 REG_HDMI_ComReadU32(volatile HI_U32 *pu32Addr);

HI_VOID REG_HDMI_ComWriteU32(volatile HI_U32 *pu32Addr, HI_U32 u32Data);

#endif



