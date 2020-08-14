#ifndef __VDP_ASSERT_H__
#define __VDP_ASSERT_H__
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vdp_define.h"
#include "vdp_env_def.h"
#include "vdp_hal_comm.h"
#include "alg_comm.h"
//#include "c_union_define.h"
#include "hi_reg_common.h"

#endif
#if VDP_C_COMPILE

#include <vdp_assert.h>
#include "vdp_define.h"
// #include "vdp_env_def.h"
#include "vdp_drv_func.h"

#include "vdp_hal_comm.h"
//#include "vdp_drv_comm_fpga.h"
#include "vdp_hal_vid.h"
#include "vdp_hal_gfx.h"
#include "vdp_hal_wbc.h"
#include "vdp_hal_chn.h"

// #include "alg_comm.h"
#include "vdp_reg.h"
#endif


#define CheckError(x) { if (x) { printf("\nErr @ (%s, %d)\n", __FILE__, __LINE__); \
                                                         exit(-__LINE__); } }
#define ADDR_CFG_NUM 40
#define VID_CHK_NUM  3
#define GFX_CHK_NUM  2
#define WBC_CHK_NUM  1


typedef enum 
{
    IDX_ADDR    = 0,
    IDX_SIZE    = 1,
    IDX_ID      = 2
} IDX_E;
#if 1

HI_VOID VDP_ReadLatencyCalc (HI_U32 u32Enable);

HI_S32 VDP_ConfigCheck      (HI_U32 type);
HI_S32 VDP_Sys_ConfigCheck  (HI_VOID);
HI_S32 VDP_VID_ConfigCheck  (HI_U32 u32Data);
HI_S32 VDP_GFX_ConfigCheck  (HI_U32 u32Data);
HI_S32 VDP_VP_ConfigCheck   (HI_U32 u32Data);
HI_S32 VDP_GP_ConfigCheck   (HI_U32 u32Data);
HI_S32 VDP_WBC_ConfigCheck  (HI_U32 u32Data);
HI_S32 VDP_AddrConfigCheck  (HI_U32 u32Data);
HI_S32 VDP_MMU_AddrCheck    (HI_VOID);
#if VDP_C_COMPILE
// HI_S32 s32SignConv          (HI_S32 s32Data, HI_U32 u32BitWidth);
#endif
#endif
#endif
