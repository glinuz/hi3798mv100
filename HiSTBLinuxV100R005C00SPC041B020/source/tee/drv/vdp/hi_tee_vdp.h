/***********************************************************************************
*              Copyright 2004 - 2015, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  tee_dmx_helper.h
* Description:  tee dmx helper functions.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20151012    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_VDP_EXT_H__
#define __TEE_VDP_EXT_H__

#include "hi_type.h"

#include "hi_tee_nexguard.h"
#include "hi_tee_verimatrix.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
typedef enum hiTEE_VDP_CMD_E
{
    HI_TEE_VDP_CMD_NXG_RESET   = 0x0,                                              
    HI_TEE_VDP_CMD_NXG_ENABLE_IN,                                                            
    HI_TEE_VDP_CMD_NXG_DEBUG_ENABLE_IN,           
    HI_TEE_VDP_CMD_NXG_SET_KEY_INSEED,                                                          
    HI_TEE_VDP_CMD_NXG_OPERATORLD,             
    HI_TEE_VDP_CMD_NXG_SUBSCRIBERID,   
    HI_TEE_VDP_CMD_NXG_TIME_CODE, 
    HI_TEE_VDP_CMD_NXG_SETTING, 
    HI_TEE_VDP_CMD_NXG_SET_PARAME,
    HI_TEE_VDP_CMD_VM_OPEN,
	HI_TEE_VDP_CMD_VM_SEND,
	HI_TEE_VDP_CMD_VM_CLOSED,
    HI_TEE_VDP_CMD_BUTT
}HI_TEE_VDP_CMD_E;


typedef struct hiTEE_VDP_VMX_PARA_S 
{ 
	vmx_hw_soc_rend_t * hwParameters;
	HI_U8  bServiceIdx;
	HI_CHAR *payload_symbols_addr;
}HI_TEE_VDP_VMX_PARA_S;



int HI_TEE_VDP_DRV_IOCTL(int cmd, void *arg);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_DMX_HELPER_H__ */
