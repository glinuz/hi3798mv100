#ifndef __TEE_DRV_HDMI_TX_REG_H__
#define __TEE_DRV_HDMI_TX_REG_H__

#include "tee_drv_hdmi_common.h"


HI_U32  HiHdmiRegRead(HI_U32 address);
HI_VOID HiHdmiRegWrite(HI_U32 address, HI_U32 data_out);
HI_VOID HiHdmiRegSetBit32(HI_U32 addr, HI_U32 mask);
HI_VOID HiHdmiRegClrBit32(HI_U32 addr, HI_U32 mask);
HI_S32 HiHdmiRegBlockWr(HI_U32 addr, HI_U32 *val, HI_U16 length);
HI_S32 HiHdmiRegBlockRd(HI_U32 addr, HI_U32 *val, HI_U16 length);

#endif

