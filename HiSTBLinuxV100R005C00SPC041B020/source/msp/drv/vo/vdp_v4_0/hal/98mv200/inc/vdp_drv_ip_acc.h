#ifndef __VDP_IP_DRV_ACC_H__
#define __VDP_IP_DRV_ACC_H__

#include "vdp_hal_ip_acc.h"
#include "vdp_drv_comm.h"

typedef enum
{
	VDP_VID_ACC_TYP =0,
	VDP_VID_ACC_TYP1 ,
	VDP_VID_ACC_RAND ,
	VDP_VID_ACC_MAX ,
	VDP_VID_ACC_MIN ,
	VDP_VID_ACC_ZERO ,
	VDP_VID_ACC_BUTT
}VDP_VID_ACC_MODE_E;

typedef struct
{
	HI_U32 acc_en ;
	HI_U32 acc_mode ;
	HI_U32 acc_dbg_mode ;
	HI_U32 acc_dbg_en ;
	HI_U32 acc_rst ;
}VDP_ACC_CFG_S;

//function declare

HI_VOID VDP_FUNC_SetAccMode(HI_U32 enLayer, VDP_VID_ACC_MODE_E AccMode, VDP_ACC_CFG_S * pstCfg);

#endif



