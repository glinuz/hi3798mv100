#ifndef __VDP_IP_DRV_SHARPEN_H__
#define __VDP_IP_DRV_SHARPEN_H__

#include "vdp_hal_ip_sharpen.h"
#include "vdp_drv_comm.h"
#if EDA_TEST
#include <stdio.h>
#endif

typedef enum tagVDP_VID_SHARPEN_MODE_E
{
    VDP_VID_SHARPEN_TYP      = 0x0,
    VDP_VID_SHARPEN_MAX      = 0x1,
    VDP_VID_SHARPEN_MIN      = 0x2,
    VDP_VID_SHARPEN_RAND     = 0x3,
    VDP_VID_SHARPEN_ZERO     = 0x4,
    VDP_VID_SHARPEN_ONE      = 0x5,

    VDP_VID_SHARPEN_CFG_BUTT        
    
}VDP_VID_SHARPEN_MODE_E;

//function declare

HI_VOID VDP_FUNC_SetSharpenMode(VDP_LAYER_VID_E u32EnLayer, VDP_VID_SHARPEN_MODE_E SharpenMode);

#endif



