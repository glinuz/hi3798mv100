/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_dmx_helper.c
* Description:  Define implement of demux helper.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#include "sre_syscalls_chip.h"
#include "hi_type.h"

#include "hi_tee_nexguard.h"
#include "hi_tee_verimatrix.h"
#include "hi_tee_vdp.h"

#define HI_FATAL_VDP(fmt, args...)    uart_printf_func("[%s]:%s[%d]:" fmt "", "FATAL-TEE_VDP", __func__, __LINE__, ## args)
#define HI_ERR_VDP(fmt, args...)     uart_printf_func("[%s]:%s[%d]:" fmt "", "ERROR-TEE_VDP", __func__, __LINE__, ## args)
#define HI_WARN_VDP(fmt, args...)        uart_printf_func("[%s]:%s[%d]:" fmt "", "WARN-TEE_VDP", __func__, __LINE__, ## args)
#define HI_INFO_VDP(fmt, args...)      
#define HI_DBG_VDP(fmt, args...)    


VM_HW_Err_t VM_HW_Init (void)
{	
	return VM_HW_OK;
} 

VM_HW_Err_t VM_HW_OpenSession(uint8_t bServiceIdx) 
{	
	HI_S32 s32Ret;
	HI_TEE_VDP_VMX_PARA_S stVMXPara; 
	memset(&stVMXPara,0x0,sizeof(HI_TEE_VDP_VMX_PARA_S));
	stVMXPara.bServiceIdx = bServiceIdx;
	
	HI_DBG_VDP(" VM_HW_OpenSession!\n");
	return HI_TEE_VDP_DRV_IOCTL(HI_TEE_VDP_CMD_VM_OPEN,&stVMXPara);
} 

VM_HW_Err_t VM_HW_CloseSession(uint8_t bServiceIdx)
{	
	HI_S32 s32Ret;
	HI_TEE_VDP_VMX_PARA_S stVMXPara; 
	
	memset(&stVMXPara,0x0,sizeof(HI_TEE_VDP_VMX_PARA_S));
	stVMXPara.bServiceIdx = bServiceIdx;
	
	HI_DBG_VDP(" VM_HW_CloseSession!\n");
	return HI_TEE_VDP_DRV_IOCTL(HI_TEE_VDP_CMD_VM_CLOSED,&stVMXPara);
}


VM_HW_Err_t VM_HW_Term (void) 
{	
	return VM_HW_OK;
} 

VM_HW_Err_t VM_HW_SetParameters_Rend (HI_U8 bServiceIdx, vmx_hw_soc_rend_t * hwParameters)
{	
	HI_S32 s32Ret;
	HI_TEE_VDP_VMX_PARA_S stVMXPara; 

	//stVMXPara.payload_symbols_addr = pcSymbols;
	stVMXPara.bServiceIdx = bServiceIdx;
	stVMXPara.hwParameters = &hwParameters[bServiceIdx];
	
	HI_DBG_VDP(" HI_TEE_VDP_DRV_IOCTL!\n");
	return HI_TEE_VDP_DRV_IOCTL(HI_TEE_VDP_CMD_VM_SEND,&stVMXPara);
}


int HI_TEE_VDP_DRV_IOCTL(int cmd, void *arg)
{
    return __vdp_drv_Ioctl(cmd ,arg);
}






