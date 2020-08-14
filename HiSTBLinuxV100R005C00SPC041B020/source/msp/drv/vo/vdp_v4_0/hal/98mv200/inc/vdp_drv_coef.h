#ifndef __DRV_COEF_H__
#define __DRV_COEF_H__

// #include "vdp_hal_hdr.h"
#include "hi_type.h"
#include "vdp_define.h"
#if VDP_CBB_FPGA
  typedef struct file FILE;
#else
  #include <stdio.h>
#endif


typedef struct
{
    HI_U32 data3;
    HI_U32 data2;
    HI_U32 data1;
    HI_U32 data0;
    HI_U32 depth;
}VDP_U128_S;


typedef enum tagVDP_COEF_LUT_MODE
{
    COEF_DATA_TYPE_U8 = 0,
    COEF_DATA_TYPE_S8  ,
    COEF_DATA_TYPE_U16 ,
    COEF_DATA_TYPE_S16 ,
    COEF_DATA_TYPE_U32 ,
    COEF_DATA_TYPE_S32 ,
    COEF_DATA_TYPE_BUTT
} VDP_COEF_DATA_TYPE_E;


typedef enum tagSTI_FILE_TYPE_E
{
    STI_FILE_DATA_VID0 = 0 ,
    STI_FILE_DATA_VID1     ,
    STI_FILE_DATA_VID2     ,
    STI_FILE_DATA_VID3     ,
    STI_FILE_DATA_VID4     ,
    STI_FILE_DATA_GFX0     ,
    STI_FILE_DATA_GFX1     ,
    STI_FILE_DATA_GFX2     ,
    STI_FILE_DATA_GFX3     ,
    STI_FILE_DATA_GFX4     ,

    STI_FILE_COEF_ZME_V0   ,
    STI_FILE_COEF_ACM      ,
    STI_FILE_COEF_DCI      ,
    STI_FILE_COEF_DIM      ,
    STI_FILE_COEF_GMM      ,
    STI_FILE_COEF_OD       ,
    STI_FILE_COEF_RGBW     ,
    STI_FILE_COEF_DEMURA   ,
    STI_FILE_TYPE_NODEF    ,
    STI_FILE_TILE_64_CMP_Y ,
    STI_FILE_TILE_64_CMP_C ,
    STI_FILE_TILE_64_CMP_Y_2BIT,
    STI_FILE_TILE_64_CMP_C_2BIT,
    STI_FILE_TILE_64_CMP_Y_HEAD,
    STI_FILE_TILE_64_CMP_C_HEAD,
    STI_FILE_OSD_CMP       ,
    STI_FILE_COEF_DM       ,
    STI_FILE_COEF_GDM      ,
	STI_FILE_COEF_METADATA ,
    STI_FILE_DATA_TNR_REF ,
    STI_FILE_DATA_TNR_MAD ,
    STI_FILE_DATA_VDH_DB  ,

    STI_FILE_DATA_VMX      ,

    STI_FILE_TYPE_BUTT
} STI_FILE_TYPE_E;


typedef struct
{
    HI_U8                  *coef_addr       ; 
    STI_FILE_TYPE_E         sti_type        ;
    FILE*                   fp_coef         ;
    HI_U32                  lut_num         ; 
    HI_U32                  burst_num       ;
    HI_U32                  cycle_num       ; 
    void   * *              p_coef_array    ; 
    HI_U32 *                lut_length      ; 
    HI_U32 *                coef_bit_length ;
    VDP_COEF_DATA_TYPE_E    data_type       ;
}VDP_COEF_SEND_CFG;

typedef enum tagVDP_RM_COEF_MODE_E
{
    VDP_RM_COEF_MODE_TYP = 0x0,
    VDP_RM_COEF_MODE_RAN = 0x1,
    VDP_RM_COEF_MODE_MIN = 0x2,
    VDP_RM_COEF_MODE_MAX = 0x3,
    VDP_RM_COEF_MODE_ZRO = 0x4,
    VDP_RM_COEF_MODE_CUS = 0x5,
    VDP_RM_COEF_MODE_BUTT
} VDP_RM_COEF_MODE_E;

typedef struct
{
    void   *                p_coef          ;
    void   *                p_coef_new      ;
    HI_S32 *                ps32_coef_temp  ;
    VDP_COEF_DATA_TYPE_E    coef_data_type  ;
    HI_U32                  length          ;
    VDP_RM_COEF_MODE_E      coef_data_mode  ; 
    HI_S32                  coef_max        ;
    HI_S32                  coef_min        ;
}VDP_COEF_GEN_CFG;


HI_U32 VDP_FUNC_SendCoef(VDP_COEF_SEND_CFG* stCfg);
HI_S32 VDP_FUNC_Push128(VDP_U128_S* pstData128, HI_U32 coef_data, HI_U32 bit_len);
HI_U32 VDP_FUNC_WriteDDR(STI_FILE_TYPE_E enStiType, FILE* fp_coef ,HI_U8  *addr, VDP_U128_S * pstData128 );
HI_U32 VDP_FUNC_FindMax(HI_U32* u32Array, HI_U32 num);

#endif
