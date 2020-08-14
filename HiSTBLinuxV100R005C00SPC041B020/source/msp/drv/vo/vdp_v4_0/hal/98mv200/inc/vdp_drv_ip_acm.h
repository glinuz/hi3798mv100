#ifndef __VDP_IP_DRV_ACM_H__
#define __VDP_IP_DRV_ACM_H__

#include "vdp_hal_ip_acm.h"
#include "vdp_drv_comm.h"
//#include <stdio.h>

typedef enum
{
	VDP_VID_ACM_TYP =0,
	VDP_VID_ACM_TYP1 ,
	VDP_VID_ACM_RAND ,
	VDP_VID_ACM_MAX ,
	VDP_VID_ACM_MIN ,
	VDP_VID_ACM_ZERO ,
	VDP_VID_ACM_LEDGE ,
	VDP_VID_ACM_REDGE ,
	VDP_VID_ACM_BUTT
}VDP_VID_ACM_MODE_E;

typedef struct
{
	HI_U32 acm0_en ;
	HI_U32 acm1_en ;
	HI_U32 acm2_en ;
	HI_U32 acm3_en ;
	HI_U32 acm_dbg_mode ;
	HI_U32 acm_test_en ;
	HI_U32 bw_pro_mode ;
	HI_U32 acm0_sec_blk;
	HI_U32 acm0_fir_blk;
	HI_U32 acm1_sec_blk;
	HI_U32 acm1_fir_blk;
	HI_U32 acm2_sec_blk;
	HI_U32 acm2_fir_blk;
	HI_U32 acm3_sec_blk;
	HI_U32 acm3_fir_blk;

}VDP_ACM_CFG_S;

//function declare

HI_VOID VDP_FUNC_SetAcmMode(HI_U32 enLayer, VDP_VID_ACM_MODE_E AcmMode, VDP_ACM_CFG_S * pstCfg);

#endif



