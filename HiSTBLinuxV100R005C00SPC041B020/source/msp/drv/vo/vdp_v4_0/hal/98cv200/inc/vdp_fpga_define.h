#ifndef _VDP_FPGA_DEFINE_H_
#define _VDP_FPGA_DEFINE_H_

#include "hi_type.h"
#include "vdp_define.h"
#define DISP_BASE_ADDRESS 0xf8cc0000UL
#define DISP_INT_NUMBER   122

#if  1
typedef enum tagDISP_VENC_E
{
    DISP_VENC_HDATE0 = 0, 
    DISP_VENC_SDATE0,
    DISP_VENC_VGA0,
    DISP_VENC_MAX
}DISP_VENC_E;
#endif
//-----------------------------------
//debug define 
//-----------------------------------
#define FPGA_DEBUG_TEST      0
#define TWOCHN_TEST     0
#define debug_width    1280     //1008 // 992   // 1008 //1280//1920 //
#define THRD_LBL       0

//MMU ²âÊÔºê¶¨Òå

#define FPGA_AUTORUN   0
#define FPGA_AUTORUN_CFG 0
#define BEGIN_NUBER 1
#define END_NUBER 2
#define MAX_FRAME_NUM  1


#define HDR_FRAME_NUM  1
#define VDP_HDR_EN  1
#define HDR_MDLEN_TEST  1

#define REG_LOG  0

#define MMU_TEST  0

//#define VDP_STREAM_PHY_BASE_ADDR ((HI_U32)0x5b000000uL)//FPGA
#define VDP_STREAM_PHY_BASE_ADDR ((HI_U32)0x20000000uL)//FPGA



#define  W_576I 720
#define  H_576I 576


//#define MMU_TEST  1



#define    VDP_DISP_DIGFMT_TMP  VDP_DISP_DIGFMT_1080P50
#define    VDP_DISP_DIGFMT_TMP_I  VDP_DISP_DIGFMT_PAL
#define    VDP_DISP_DIGFMT_TMP_DHD0_I  VDP_DISP_DIGFMT_1080I50


#define VDP_HI3798CV200     1
#define VDP_BUF_ADDR VDP_STREAM_PHY_BASE_ADDR
#define VDP_GFX_BUF_SIZE 0x01000000L  // 16M
#define VDP_BUF_SIZE 0xf000000
#define VDP_BUF_STRIDE  0x08000000

#define VDP_GFX_BUF_ADDR (VDP_BUF_ADDR + VDP_BUF_SIZE)
#define VDP_GFX_ACLUT_BUF_ADDR (VDP_BUF_ADDR + 3*VDP_BUF_SIZE)
#define VDP_WBC_BUF_ADDR  (VDP_GFX_BUF_ADDR + 4*VDP_BUF_STRIDE)
#define VDP_TILE_BUF_ADDR  (VDP_WBC_BUF_ADDR + 3*VDP_BUF_STRIDE)
#define VDP_FLIP_BUF_ADDR  (VDP_WBC_BUF_ADDR + 5*VDP_BUF_STRIDE)
#define VDP_TNR_BUF_ADDR  (VDP_FLIP_BUF_ADDR + 0*VDP_BUF_STRIDE)
#define VDP_TNR_DCMP_BUF_ADDR  (VDP_FLIP_BUF_ADDR + 2*VDP_BUF_STRIDE)
#define VDP_DBDETECT_BUF_ADDR  (VDP_FLIP_BUF_ADDR + 1*VDP_BUF_STRIDE)
#define VDP_WBC_STT_BUF_ADDR   (VDP_DBDETECT_BUF_ADDR + 1*VDP_BUF_STRIDE)


#define DBM_CFG_ADDR  (VDP_WBC_BUF_ADDR + 4*VDP_BUF_STRIDE)
//#if VDP_CBB_EDA
//#define printk printf   
//#endif
#define VGA_MUX  0
//#define OPTM_STREAM_MAX_SIZE 0x6000000L


//VOUÏµÍ³Ê±ÖÓ¿ØÖÆ¼Ä´æÆ÷
//System control ¼Ä´æÆ÷
#define OPTM_REGS_VDP_SYS_SET 0xf8a220d8

//VOU PLL ¼Ä´æÆ÷
#define OPTM_REGS_HPLL01_SET 0xF8A22028
#define OPTM_REGS_HPLL02_SET 0xF8A2202C

#define HI_OPTM_PRINT(fmt...) printk(fmt)

#define VDP_CHK_ADDR_DHD0 ( VDP_BASE_ADDR+ 0xc368)
#define VDP_CHK_ADDR_DHD0_1  ( VDP_BASE_ADDR+ 0xc374)

#define VDP_CHK_ADDR_DHD1  (VDP_BASE_ADDR+ 0xc768)
#define VDP_CHK_ADDR_DSD0 VDP_BASE_ADDR+ 0xd3f0

#define VDP_CHK_ADDR_WBCDHD0 VDP_BASE_ADDR + 0x90f0
#define VDP_CHK_ADDR_WBCVP0 VDP_BASE_ADDR + 0x9cf0
#define VDP_CHK_ADDR_WBCGP0 VDP_BASE_ADDR + 0x94f0

#define MAX_SUM ((3+2*3)*CHN_MAX)
#define OPTM_AA_ACC_COEF_NUM 256
#define OPTM_AA_ACC_TABLE_SIZE 45
#define OPTM_AA_GMM_COEF_NUM 257*3


#define   HI_SUCCESS          0
#define   HI_FAILURE          (-1)

//VOU¼Ä´æÆ÷ÁÐ±íÎïÀí»ùµØÖ·
#define OPTM_REGS_BASE_ADDR   0xf8cc0000


//VOUÏµÍ³Ê±ÖÓ¿ØÖÆ¼Ä´æÆ÷

//VOU hpll cfg  ¼Ä´æÆ÷
#define VDP_PLL_REG_BASE_ADDR0  0xF8A22028
#define VDP_PLL_REG_BASE_ADDR1  0xF8A2202c
#define VDP_PLL_REG_BASE_ADDR2  0xF8A220d8


//VOU VO_HD ¼Ä´æÆ÷
#define OPTM_REGS_VO_HD_SET 0xF8A22028
//VOU IO_CONFIG ¼Ä´æÆ÷
#define OPTM_REGS_IO_CONFIG_SET 0xF8A22028
//VOU perl ctrl ¼Ä´æÆ÷
#define OPTM_REGS_PERL_SET  0xF8A22028

//VOU perl ctrl  0D ¼Ä´æÆ÷
#define OPTM_REGS_PERL_0D_SET  0xF8A22028

#define LoopNumBreak  1

#if 0
#define ABS(x)           (((x) < 0) ? -(x) : (x))
#define MAX(a, b)         (( (a) < (b) ) ?  (b) : (a))
#define MIN(a, b)         (( (a) > (b) ) ?  (b) : (a))
#define MIN_3(a,b,c) MIN(MIN(a,b),MIN(b,c))
#endif


//VDP COEF LAYER add by hyx
#define LAYER_GDM_GAMMA         0
#define LAYER_GDM_DEGAMMA       1
#define LAYER_CVM       		2
#define LAYER_IN_GAMMA       	3
#define LAYER_OUT_GAMMA       	4
#define LAYER_IN_DEGAMMA        5
#define LAYER_OUT_DEGAMMA       6
#define LAYER_VID_ZME_H       	7
#define LAYER_VID_ZME_V       	8
#define LAYER_VID_ZME_TMP       9



typedef enum tagVDP_RM_LAYER_E
{
    VDP_RM_LAYER_VID0 = 0,
    VDP_RM_LAYER_VID1,
    VDP_RM_LAYER_VID2, 
    VDP_RM_LAYER_VID3, 
    VDP_RM_LAYER_VID4, 
    VDP_RM_LAYER_GFX0,   
    VDP_RM_LAYER_GFX1,   
    VDP_RM_LAYER_GFX2,   
    VDP_RM_LAYER_GFX3,   
    VDP_RM_LAYER_GFX4,   
    VDP_RM_LAYER_WBC_HD0,   
    VDP_RM_LAYER_WBC_GP0,   
    VDP_RM_LAYER_GP0,   
    VDP_RM_LAYER_GP1, 
    VDP_RM_LAYER_VP0,   
    VDP_RM_LAYER_TT,   

} VDP_RM_LAYER_E;


#endif
