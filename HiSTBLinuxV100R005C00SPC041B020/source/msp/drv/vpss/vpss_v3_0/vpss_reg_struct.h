#ifndef __VPSS_REG_STRUCT_H__
#define __VPSS_REG_STRUCT_H__
#include "hi_drv_video.h"
#include "vpss_common.h"
typedef enum hiVPSS_REG_PORT_E
{
    VPSS_REG_STR = 0,
    VPSS_REG_HD,
    VPSS_REG_SD,
    VPSS_REG_BUTT
}VPSS_REG_PORT_E;

typedef enum hiVPSS_REG_PREZME_E{
    PREZME_DISABLE = 0,
    PREZME_2X,
    PREZME_4X,
    PREZME_8X,
	PREZME_VFIELD,
    PREZME_BUTT
}VPSS_REG_PREZME_E;
typedef enum hiREG_FIELDPOS_E
{
    LAST_FIELD = 0,
    CUR_FIELD,
    NEXT1_FIELD,
    NEXT2_FIELD,
    NEXT3_FIELD,
    BUTT_FIELD
}REG_FIELDPOS_E;


typedef enum hiREG_FRAMEPOS_E
{
    LAST_FRAME = 0,
    CUR_FRAME,
    NEXT_FRAME,
    BUTT_FRAME
}REG_FRAMEPOS_E;

typedef enum hiREG_CHANELPOS_E
{
    CUR_CHANEL = 0,
    NEXT1_CHANEL,
    NEXT2_CHANEL,
    REF_CHANEL,
    BUTT_CHANEL
}REG_CHANELPOS_E;


typedef enum hiREG_TUNLPOS_E
{
    ROW_2_WIRTE_TUNL = 0,
    ROW_4_WIRTE_TUNL,
    ROW_8_WIRTE_TUNL,
    ROW_16_WIRTE_TUNL,
    BUTT_WIRTE_TUNL
}REG_TUNLPOS_E;
//#endif


typedef enum hiREG_ZME_MODE_E
{
    REG_ZME_MODE_HOR = 0,
    REG_ZME_MODE_VER,

    REG_ZME_MODE_HORL,  
    REG_ZME_MODE_HORC,  
    REG_ZME_MODE_VERL,
    REG_ZME_MODE_VERC,

    REG_ZME_MODE_ALL,
    REG_ZME_MODE_NONL,
    REG_ZME_MODE_BUTT
      
}REG_ZME_MODE_E;


typedef enum hiREG_PIX_BITW_E
{
    REG_PIX_8BIT = 0,
    REG_PIX_10BIT,
    REG_PIX_BUTT

}REG_PIX_BITW_E;

typedef enum hiREG_DITHER_MODE_E
{
    REG_DITHER_MODE_DITHER = 0,
    REG_DITHER_MODE_ROUND,
    REG_DITHER_MODE_BUTT

}REG_DITHER_MODE_E;

typedef enum hiREG_AXI_CHN_E
{
    REG_AXI_AR = 0,
    REG_AXI_AW,
    REG_AXI_BUTT

}REG_AXI_CHN_E;

typedef enum hiREG_AXI_RID_CHN_E
{
    REG_NX2C_RID = 0,
    REG_NX2Y_RID,
    REG_NX1C_RID,
    REG_NX1Y_RID,
    REG_REFC_RID,
    REG_REFY_RID,
    REG_CURC_RID,
    REG_CURY_RID,
    REG_CAS_RID,
    REG_RST_RID,
    REG_AIX_RID_BUTT

}REG_AXI_RID_CHN_E;

typedef enum hiREG_DIE_MODE_E
{
    REG_DIE_MODE_CHROME = 0,
    REG_DIE_MODE_LUMA,
    REG_DIE_MODE_ALL,
    REG_DIE_MODE_BUTT
      
}REG_DIE_MODE_E;


//--------------------------------------------
//ADD in HiFone B02 by z214841
typedef enum hiVPSS_REG_ADDR_E{
	REG_VPSS_VC1_STR_ADDR,
	REG_VPSS_ZME_ADDR,
	REG_VPSS_HSP_ADDR,
	REG_VPSS_DB_ADDR,
	REG_VPSS_DR_ADDR,
	REG_VPSS_TNR_ADDR,
	REG_VPSS_TNR_CLUT_ADDR,
	REG_VPSS_DBM_ADDR,
	REG_VPSS_DEI_ADDR,
	REG_VPSS_CHK_SUM_W_ADDR,
	REG_VPSS_VHD0_LCMP_ADDR,
	REG_VPSS_PNEXT_ADDR,
}VPSS_REG_ADDR_E;
#define VPSS_ZME_ADDR_GET(appviraddr)	((appviraddr) + 0x2000)



typedef enum hiVPSS_REG_VHD0_LCMP_E{
	VPSS_VHD0_LCMP_Y,
	VPSS_VHD0_LCMP_C
}VPSS_REG_VHD0_LCMP_E;

typedef enum hiVPSS_REG_ZME_FMT_E
{
	ZME_FMT_422,
	ZME_FMT_420
}VPSS_REG_ZME_FMT_E;

/*
@f00241306 add CMP structure definition
*/
typedef struct 
{
    HI_U32 u32Width;  
    HI_U32 u32Height;
    HI_U32 u32Cfg0_Max_rg_comp_bits;  
    HI_U32 u32Cfg0_Ri_bits; 
    HI_U32 u32Cfg0_Mb_bits;
    HI_U32 u32Cfg1_Rc_smth_ngain;  
    HI_U32 u32Cfg1_Sad_bits_ngain; 
    HI_U32 u32Cfg1_Max_qp;
    HI_U32 u32Cfg2_Pix_diff_thr;  
    HI_U32 u32Cfg2_Smth_pix_num_thr; 
    HI_U32 u32Cfg2_Smth_thr;
    HI_U32 u32Cfg3_Adj_sad_bit_thr; 
    HI_U32 u32Cfg3_Adj_sad_thr; 
    HI_U32 u32Cfg4_Qp_chg2_bits_thr; 
    HI_U32 u32Cfg4_Qp_chg1_bits_thr;
    HI_U32 u32Cfg5_Smth_lftbits_thr1;
    HI_U32 u32Cfg5_Smth_lftbits_thr0;
    HI_U32 u32Cfg5_Smth_qp1;
    HI_U32 u32Cfg5_Smth_qp0;
    
}REG_CMP_DATA_S;
#endif
