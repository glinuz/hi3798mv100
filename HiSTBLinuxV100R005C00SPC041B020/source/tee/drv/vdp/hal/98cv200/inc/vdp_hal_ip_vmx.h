#ifndef __HAL_IP_VMX_H__
#define __HAL_IP_VMX_H__

#include "vdp_define.h"
#include "sre_sem.h"
#include "drv_tee_mem.h"

#define VMX_MIN(a,b) ((a < b) ? a : b)
#define VMX_MIN3(a,b,c) VMX_MIN(VMX_MIN(a,b),VMX_MIN(b,c))
#define VMX_MAX(a,b) ((a > b) ? a : b)

#define DIRECTION_LEFT	0
#define DIRECTION_RIGHT	1
#define DIRECTION_TOP	2

#define SYMBOL_ORG_WIDTH	20
#define SYMBOL_ORG_HEIGHT	24
#define SYMBOLS_NUM			16

#define COLUMN_STORE_SIZE 4096

typedef struct VMX_CORE_PARAMETERS {
	// VideoMark Core Parameters – internal processing
	//rendering part
 	HI_U32 payload_symbols_addr;
	HI_U32 symbols_rows;
	HI_U32 symbols_cols;
	HI_U32 symbols_xpos;
	HI_U32 symbols_ypos;
	HI_U32 symbol_size;
	HI_U32 spacing_vert;
	HI_U32 spacing_horz;
	HI_U32 symbol_scale_control;

	//embedding part
	HI_U32 watermark_on;
	HI_U32 frequency_distance[3][3];
	HI_U32 background_embedding_on;
	HI_U32 embedding_strength_threshold[48]; // 48 - to accommodate all 3 cases (8, 10, 12 bits scenarios) 
	HI_U32 embedding_strength_threshold_bg[48];
	HI_U32 direction_max;
	HI_U32 strength_multiply;
}vmx_hw_soc_t;

typedef enum
{
    VDP_LAYER_VMX_VID0      = 0x0,
    VDP_LAYER_VMX_VID1      = 0x1,
    VDP_LAYER_VMX_VP0       = 0x2,
    VDP_LAYER_VMX_BUTT
} VDP_LAYER_VMX_E;


typedef struct hiVMX_PARA_S {
	HI_U32 u32VMX_Open;
	TEE_MMZ_BUFFER_S stSybleMmz;
	vmx_hw_soc_t stVmxPara;
}VMX_PARA_S;


typedef enum tagVDP_VMX_EYE_MODE_E
{
    VDP_VMX_EYE_MODE_TWOEYE   = 0,
    VDP_VMX_EYE_MODE_LEFTEYE  = 1,
    VDP_VMX_EYE_MODE_RIGHTEYE = 2,

    VDP_VMX_EYE_MODE_BUTT
} VDP_VMX_EYE_MODE_E;


HI_VOID VDP_VMX_SetVmxDwmEn           ( HI_U32 u32Data, HI_U32 dwm_en           ) ;
HI_VOID VDP_VMX_SetVmxBgEmbdEn        ( HI_U32 u32Data, HI_U32 bg_embd_en       ) ;
HI_VOID VDP_VMX_SetVmxScale           ( HI_U32 u32Data, HI_U32 scale            ) ;
//vmx_cfg
HI_VOID VDP_VMX_SetVmxWidth           ( HI_U32 u32Data, HI_U32 width            ) ;
HI_VOID VDP_VMX_SetVmxHeight          ( HI_U32 u32Data, HI_U32 height           ) ;
HI_VOID VDP_VMX_SetVmxSybYfpos        ( HI_U32 u32Data, HI_U32 syb_yfpos        ) ;
HI_VOID VDP_VMX_SetVmxSybXfpos        ( HI_U32 u32Data, HI_U32 syb_xfpos        ) ;
HI_VOID VDP_VMX_SetVmxSybYlpos        ( HI_U32 u32Data, HI_U32 syb_ylpos        ) ;
HI_VOID VDP_VMX_SetVmxSybXlpos        ( HI_U32 u32Data, HI_U32 syb_xlpos        ) ;
HI_VOID VDP_VMX_SetVmxSybVerNum       ( HI_U32 u32Data, HI_U32 syb_ver_num      ) ;
HI_VOID VDP_VMX_SetVmxSybHorNum       ( HI_U32 u32Data, HI_U32 syb_hor_num      ) ;
HI_VOID VDP_VMX_SetVmxSybVerSpac      ( HI_U32 u32Data, HI_U32 syb_ver_spac     ) ;
HI_VOID VDP_VMX_SetVmxSybHorSpac      ( HI_U32 u32Data, HI_U32 syb_hor_spac     ) ;
HI_VOID VDP_VMX_SetVmxLfreDirction    ( HI_U32 u32Data, HI_U32 dirction[3]      ) ;
HI_VOID VDP_VMX_SetVmxRfreDirction    ( HI_U32 u32Data, HI_U32 dirction[3]      ) ;
HI_VOID VDP_VMX_SetVmxTfreDirction    ( HI_U32 u32Data, HI_U32 dirction[3]      ) ;
HI_VOID VDP_VMX_SetVmxFrequencyLine   ( HI_U32 u32Data, HI_U32 frequency_line   ) ;
HI_VOID VDP_VMX_SetVmxFrequencyPix    ( HI_U32 u32Data, HI_U32 frequency_pix    ) ;
HI_VOID VDP_VMX_SetVmxFrequencySpac   ( HI_U32 u32Data, HI_U32 frequency_spac   ) ;
HI_VOID VDP_VMX_SetVmxSybStride       ( HI_U32 u32Data, HI_U32 sub_stride       ) ;
HI_VOID VDP_VMX_SetVmxStrenghMultiply ( HI_U32 u32Data, HI_U32 strengh_multiply ) ;
HI_VOID VDP_VMX_SetVmxDirectionMax    ( HI_U32 u32Data, HI_U32 direction_max    ) ;

HI_VOID VDP_VMX_SetVmxStrenghThd      ( HI_U32 u32Data, HI_U32 * strengh_thd    ) ;
HI_VOID VDP_VMX_SetVmxStrenghThdBg    ( HI_U32 u32Data, HI_U32 * strengh_bg     ) ;
HI_VOID VDP_VID_SetVmxCoefAddr        ( HI_U32 u32Data, HI_U32 u32CoefAddr      ) ;
HI_VOID VDP_VP_SetVmxCoefAddr         ( HI_U32 u32Data, HI_U32 u32CoefAddr      ) ;
// HI_VOID VDP_VMX_SetVmxStrenghThd   ( HI_U32 u32Data, HI_U32  strengh_thd[48] ) ;
// HI_VOID VDP_VMX_SetVmxStrenghThdBg ( HI_U32 u32Data, HI_U32  strengh_bg[48]  ) ;


HI_S32 VDP_DRV_SetVmxMode             ( VDP_LAYER_VMX_E enLayer, VDP_VMX_EYE_MODE_E enVmxEyeMode, VDP_RECT_S stRect, vmx_hw_soc_t * pstVmxCfg) ; 

#endif
