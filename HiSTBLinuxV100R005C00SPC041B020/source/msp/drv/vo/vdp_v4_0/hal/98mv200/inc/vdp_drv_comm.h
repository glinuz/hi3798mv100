#ifndef __DRV_COMM_H__
#define __DRV_COMM_H__

#include "vdp_define.h"
#include "outer_depend.h"

#include "vdp_hal_comm.h"

#define  HAL_VDP_DDR_NAME "VDP_DDR_CFG"

#if ((VDP_CBB_FPGA) && defined(VDP_ZME_COPY_MODE))
#define  GetVdpZmeCoef VDP_GetVdpZmeCoef
#else
#define  GetVdpZmeCoef DRV_PQ_GetVdpZmeCoef
#endif

#if (!VDP_CBB_FPGA) 
#include <string.h>
#endif

#if  !VDP_CBB_FPGA
#include <stdlib.h>
#endif
//----------------------------------------------------------------------
// address distrubite
//----------------------------------------------------------------------
#define  VDP_ALL_COEF_SIZE        0x30000
#define  COEF_SIZE_IN_GAMMA       4096
#define  COEF_SIZE_OUT_GAMMA      4096
#define  COEF_SIZE_IN_DEGAMMA     4096
#define  COEF_SIZE_OUT_DEGAMMA    4096
#define  COEF_SIZE_CVM            4096
#define  COEF_SIZE_GDM_GAMMA      4096
#define  COEF_SIZE_GDM_DEGAMMA    4096
#define  COEF_SIZE_ZMEH           4096
#define  COEF_SIZE_ZMEV           4096
#define  COEF_SIZE_ZME2H           4096
#define  COEF_SIZE_ZME2V           4096

#define  COEF_SIZE_ZME_TMP        4096
#define  COEF_SIZE_VMX_V0         4096
#define  COEF_SIZE_VMX_V1         4096
#define  COEF_SIZE_VMX_VP0        4096
#define  COEF_SIZE_WBC_ZMEH           4096
#define  COEF_SIZE_WBC_ZMEV           4096
#define  COEF_SIZE_HDR_METADATA       4096
#define  COEF_SIZE_ZME2H           4096
#define  COEF_SIZE_ZME2V           4096


#define  COEF_SIZE_VDM_DEGAMMA           4096
#define  COEF_SIZE_VDM_GAMMA           4096
#define  COEF_SIZE_VDM_TONEMAP       4096
#define  COEF_SIZE_GDM_GAMMA2        4096
#define  COEF_SIZE_GDM_TONEMAP       4096
#define  COEF_SIZE_V1_MUTE_L      4096
#define  COEF_SIZE_V1_MUTE_C      4096

//video buf size: stride*h*2(flip)*2(3d) = (4096*10/8)*2160*2*2 ~= 0x3000000 
#define  VDP_BUF_SIZE_V0              0x3000000
#define  VDP_BUF_SIZE_V1              0x3000000
#define  VDP_BUF_SIZE_V2              0x3000000
#define  VDP_BUF_SIZE_V3              0x3000000
#define  VDP_BUF_SIZE_V4              0x3000000


//----------------------------------------------------------------------
// ALG macro define
//----------------------------------------------------------------------
#define IABS(x) (((x) < 0) ? -(x) : (x))
#define CCLIP(x,max,min) (((x) < min) ? min : (((x) > max)?max:(x)))

#define VDP_ABS(x) (((x) < 0) ? -(x) : (x))

typedef struct hiVDP_MMZ_BUFFER_S
{
    //HI_U32 u32StartVirAddr;
    
	HI_U8* pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
}VDP_MMZ_BUFFER_S;


typedef enum
{
    VDP_BUF_V0 = 0,
    VDP_BUF_V1    ,
    VDP_BUF_V2    ,
    VDP_BUF_V3    ,
    VDP_BUF_V4    ,
    VDP_BUF_G0    ,
    VDP_BUF_G1    ,
    VDP_BUF_G2    ,
    VDP_BUF_G3    ,
    VDP_BUF_G4    ,
    VDP_BUF_G5    ,
    VDP_BUF_BUTT

}VDP_LAYER_BUF_E;


typedef enum
{
    VDP_COEF_BUF_IN_GAMMA      = 0  ,
    VDP_COEF_BUF_OUT_GAMMA     ,
    VDP_COEF_BUF_IN_DEGAMMA    ,
    VDP_COEF_BUF_OUT_DEGAMMA   ,
    VDP_COEF_BUF_CVM           ,
    
    VDP_COEF_BUF_GDM_GAMMA     ,
    VDP_COEF_BUF_GDM_DEGAMMA   ,
    VDP_COEF_BUF_ZMEH          ,
    VDP_COEF_BUF_ZMEV          ,
    VDP_COEF_BUF_ZME2H          ,
    VDP_COEF_BUF_ZME2V          ,
    VDP_COEF_BUF_ZME_V1_H          ,
    VDP_COEF_BUF_ZME_V1_V          ,
    
    VDP_COEF_BUF_ZME_TMP       ,
    VDP_COEF_BUF_VMX_V0       ,
    VDP_COEF_BUF_VMX_V1       ,
	VDP_COEF_BUF_VMX_VP0 	  ,
    VDP_COEF_BUF_WBC_ZMEH          ,
    VDP_COEF_BUF_WBC_ZMEV          ,

    VDP_COEF_BUF_VDM_DEGAMMA          ,
    VDP_COEF_BUF_VDM_GAMMA          ,
    VDP_COEF_BUF_VDM_TONEMAP          ,
    
	VDP_COEF_BUF_GDM_GAMMA2			,
	VDP_COEF_BUF_GDM_TONEMAP		  ,
	VDP_COEF_BUF_GCVM			,

    
    VDP_COEF_BUF_HDR_METADATA          ,
    //VDP_COEF_BUF_ZMEH_V1          ,
    //VDP_COEF_BUF_ZMEV_V1          ,
    VDP_COEF_BUF_MUTE_V1_L          ,
    VDP_COEF_BUF_MUTE_V1_C          ,
    VDP_COEF_BUF_BUTT

}VDP_COEF_BUF_E;

typedef struct hiVDP_COEF_ADDR_S
{
	VDP_MMZ_BUFFER_S 		stBufBaseAddr   ;
    HI_U32                  u32size         ;

    HI_U8                  *pu8CoefVirAddr[VDP_COEF_BUF_BUTT]     ;
    HI_U32                 u32CoefPhyAddr[VDP_COEF_BUF_BUTT]     ;
}VDP_COEF_ADDR_S;

typedef struct hiVDP_LAYER_BUF_ADDR_S
{
	VDP_MMZ_BUFFER_S 		stBufBaseAddr   ;
    HI_U32                  u32size         ;

    HI_U32                  u32BufAddr[VDP_BUF_BUTT]   ;
}VDP_LAYER_BUF_ADDR_S;
//-------------------------------------------------------------------
// function
//-------------------------------------------------------------------
HI_S32 VDP_DRV_AllocateMem(HI_U32 u32Size,VDP_MMZ_BUFFER_S *stVdpMmzBuffer);
HI_S32 VDP_DRV_DeleteMem(VDP_MMZ_BUFFER_S *stVdpMmzBuffer);
HI_S32 VDP_DRV_CoefBufAddrDistribute(VDP_COEF_ADDR_S *stVdpCoefBufAddr);
HI_S32 VDP_DRV_LayerBufAddrDistribute(VDP_LAYER_BUF_ADDR_S  *stVdpLayerBufAddr);

HI_S32 VDP_DRV_IoRemap(HI_U32 u32Phy, HI_U32 u32Size,HI_U32 **pu32VirtualAddr);
HI_S32 VDP_DRV_IoUnmap(HI_U32 **pu32VirtualAddr);

HI_U32 uGetRand(HI_U32 max, HI_U32 min);
HI_S32 sGetRand(HI_S32 max, HI_S32 min);

HI_U32 uGetRandEx(HI_U32 max, HI_U32 min);
HI_S32 sGetRandEx(HI_S32 max, HI_S32 min);

#endif//__DRV_COMM_H__
