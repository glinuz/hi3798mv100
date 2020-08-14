#include <linux/io.h>
#include <linux/delay.h>
#include "vpss_cbb_reg.h"
#include "vpss_cbb_base.h"
#include "vpss_cbb_reg.h"
#include "vpss_cbb_component.h"


#define VPSS_ASSERT(ops) {\
if(!(ops)) \
{\
	bErrFlag = HI_TRUE;\
	VPSS_FATAL("ASSERT failed at: [%s] \n", #ops);\
}}

HI_VOID VPSS_HAL_Assert(HI_S32 s32NodeId, volatile S_VPSS_REGS_TYPE *vpss_reg, HI_BOOL bForcePrintInfo)
{
	HI_BOOL	bErrFlag            = HI_FALSE;
	HI_U32 YC_422				= 0;

    HI_U32 vpss_tmp_width       = 0;//for size check
    HI_U32 vpss_tmp_height      = 0;
    HI_U32 vpss_tmp_format      = 0;
    HI_U32 vpss_vhd0_tmp_width  = 0;//for size check
    HI_U32 vpss_vhd0_tmp_height = 0;
    HI_U32 vpss_vhd0_tmp_format = 0;
    //**************************//
    //       GET REG CFG
    //**************************//
    //MAC && BUS
    HI_U32 bfield              = vpss_reg->VPSS_CTRL.bits.bfield;
	HI_U32 bfield_mode		   = vpss_reg->VPSS_CTRL.bits.bfield_mode;
    HI_U32 in_format           = vpss_reg->VPSS_IN_CTRL.bits.in_format;
    HI_U32 nx2_offx            = vpss_reg->VPSS_NXT2_CTRL.bits.nxt2_hor_offset;
    HI_U32 nx2_offy            = vpss_reg->VPSS_NXT2_CTRL.bits.nxt2_ver_offset;
    HI_U32 nx1_offx            = vpss_reg->VPSS_NXT1_CTRL.bits.nxt1_hor_offset;
    HI_U32 nx1_offy            = vpss_reg->VPSS_NXT1_CTRL.bits.nxt1_ver_offset;
    HI_U32 cur_offx            = vpss_reg->VPSS_CUR_CTRL.bits.cur_hor_offset;
    HI_U32 cur_offy            = vpss_reg->VPSS_CUR_CTRL.bits.cur_ver_offset;
    HI_U32 ref_offx            = vpss_reg->VPSS_REF_CTRL.bits.ref_hor_offset;
    HI_U32 ref_offy            = vpss_reg->VPSS_REF_CTRL.bits.ref_ver_offset;
    HI_U32 cur_tunl_en         = vpss_reg->VPSS_NX2_TUNL_CTRL.bits.nx2_tunl_en;
    HI_U32 rotate_en           = vpss_reg->VPSS_CTRL.bits.rotate_en;
    HI_U32 vhd0_flip           = vpss_reg->VPSS_VHD0CTRL.bits.vhd0_flip;
    HI_U32 four_pix_en         = vpss_reg->VPSS_CTRL.bits.four_pix_en;
    HI_U32 img_pro_mode        = vpss_reg->VPSS_CTRL.bits.img_pro_mode;
    HI_U32 prot				   = vpss_reg->VPSS_CTRL.bits.prot;

    HI_U32 cur_type            = vpss_reg->VPSS_CUR_CTRL.bits.cur_tile_format;
    HI_U32 ref_type            = vpss_reg->VPSS_REF_CTRL.bits.ref_tile_format;
    HI_U32 nx1_type            = vpss_reg->VPSS_NXT1_CTRL.bits.nxt1_tile_format;
    HI_U32 nx2_type            = vpss_reg->VPSS_NXT2_CTRL.bits.nxt2_tile_format;

    HI_U32 nx2_dcmp_en         = vpss_reg->VPSS_NXT2_CTRL.bits.nxt2_dcmp_en;
    HI_U32 in_pix_bitw         = vpss_reg->VPSS_IN_CTRL.bits.in_pix_bitw;

    HI_U32 nx2_width           = vpss_reg->VPSS_IMGSIZE.bits.imgwidth  + 1;
    HI_U32 nx2_height          = vpss_reg->VPSS_IMGSIZE.bits.imgheight + 1;

    HI_U32 ref_width           = vpss_reg->VPSS_REFSIZE.bits.refwidth  + 1;
    HI_U32 ref_height          = vpss_reg->VPSS_REFSIZE.bits.refheight + 1;

    //channel addr and stride
    HI_U32 nx2_y_addr          = vpss_reg->VPSS_NXT2YADDR.u32;
    HI_U32 nx2_c_addr          = vpss_reg->VPSS_NXT2CADDR.u32;
    HI_U32 nx2_y_stride        = vpss_reg->VPSS_NXT2STRIDE.bits.nxt2y_stride;
    HI_U32 nx2_c_stride        = vpss_reg->VPSS_NXT2STRIDE.bits.nxt2c_stride;
    HI_U32 nx2_y_head_addr     = vpss_reg->VPSS_NX2Y_HEAD_ADDR.u32;
    HI_U32 nx2_c_head_addr     = vpss_reg->VPSS_NX2C_HEAD_ADDR.u32;
    HI_U32 nx2_head_stride     = vpss_reg->VPSS_NX2_HEAD_STRIDE.bits.nx2_head_stride;

    HI_U32 nx1_y_addr          = vpss_reg->VPSS_NXT1YADDR.u32;
    HI_U32 nx1_c_addr          = vpss_reg->VPSS_NXT1CADDR.u32;
    HI_U32 nx1_y_stride        = vpss_reg->VPSS_NXT1STRIDE.bits.nxt1y_stride;
    HI_U32 nx1_c_stride        = vpss_reg->VPSS_NXT1STRIDE.bits.nxt1c_stride;

    HI_U32 cur_y_addr          = vpss_reg->VPSS_CURYADDR.u32;
    HI_U32 cur_c_addr          = vpss_reg->VPSS_CURCADDR.u32;
    HI_U32 cur_y_stride        = vpss_reg->VPSS_CURSTRIDE.bits.cury_stride;
    HI_U32 cur_c_stride        = vpss_reg->VPSS_CURSTRIDE.bits.curc_stride;

    HI_U32 ref_y_addr          = vpss_reg->VPSS_REFYADDR.u32;
    HI_U32 ref_c_addr          = vpss_reg->VPSS_REFCADDR.u32;
    HI_U32 ref_y_stride        = vpss_reg->VPSS_REFSTRIDE.bits.refy_stride;
    HI_U32 ref_c_stride        = vpss_reg->VPSS_REFSTRIDE.bits.refc_stride;

    HI_U32 ree_y_addr          = vpss_reg->VPSS_REEYADDR.u32;
    HI_U32 ree_c_addr          = vpss_reg->VPSS_REECADDR.u32;
    HI_U32 ree_y_stride        = vpss_reg->VPSS_REESTRIDE.bits.reey_stride;
    HI_U32 ree_c_stride        = vpss_reg->VPSS_REESTRIDE.bits.reec_stride;

    HI_U32 prjv_cur_addr       = vpss_reg->VPSS_PRJVCURADDR.u32;
    HI_U32 prjh_cur_addr       = vpss_reg->VPSS_PRJHCURADDR.u32;
    HI_U32 prjv_cur_stride     = vpss_reg->VPSS_PRJCURSTRIDE.bits.prjv_cur_stride;
    HI_U32 prjh_cur_stride     = vpss_reg->VPSS_PRJCURSTRIDE.bits.prjh_cur_stride;

    HI_U32 rgmv_cur_addr       = vpss_reg->VPSS_RGMVCURADDR.u32;
    HI_U32 rgmv_cur_stride     = vpss_reg->VPSS_RGMVSTRIDE.bits.rgmv_cur_stride;
    HI_U32 rgmv_nx1_addr       = vpss_reg->VPSS_RGMVNX1ADDR.u32;
    HI_U32 rgmv_nx1_stride     = vpss_reg->VPSS_RGMVSTRIDE.bits.rgmv_nx1_stride;

    HI_U32 blkmv_nx1_addr      = vpss_reg->VPSS_BLKMVNX1ADDR.u32;
    HI_U32 blkmv_nx1_stride    = vpss_reg->VPSS_BLKMVNX1STRIDE.bits.blkmv_nx1_stride;
    HI_U32 blkmv_cur_addr      = vpss_reg->VPSS_BLKMVCURADDR.u32;
    HI_U32 blkmv_cur_stride    = vpss_reg->VPSS_BLKMVSTRIDE.bits.blkmv_cur_stride;
    HI_U32 blkmv_ref_addr      = vpss_reg->VPSS_BLKMVREFADDR.u32;
    HI_U32 blkmv_ref_stride    = vpss_reg->VPSS_BLKMVSTRIDE.bits.blkmv_ref_stride;

    HI_U32 tnr_rmad_addr       = vpss_reg->VPSS_MAD_RADDR.u32;
    HI_U32 tnr_rmad_stride     = vpss_reg->VPSS_MADSTRIDE.bits.mad_stride;

    HI_U32 snr_rmad_addr       = vpss_reg->VPSS_SNR_MAD_RADDR.u32;
    HI_U32 snr_rmad_stride     = vpss_reg->VPSS_MADSTRIDE.bits.mad_stride;

    HI_U32 dei_rst_addr        = vpss_reg->VPSS_STRADDR.u32;
    HI_U32 dei_rst_stride      = vpss_reg->VPSS_STSTRIDE.bits.st_stride;

    HI_U32 vhd0_y_addr         = vpss_reg->VPSS_VHD0YADDR.u32;
    HI_U32 vhd0_c_addr         = vpss_reg->VPSS_VHD0CADDR.u32;
    HI_U32 vhd0_y_stride       = vpss_reg->VPSS_VHD0STRIDE.bits.vhd0y_stride;
    HI_U32 vhd0_c_stride       = vpss_reg->VPSS_VHD0STRIDE.bits.vhd0c_stride;

    HI_U32 rfr_y_addr          = vpss_reg->VPSS_RFRYADDR.u32;
    HI_U32 rfr_c_addr          = vpss_reg->VPSS_RFRCADDR.u32;
    HI_U32 rfr_y_stride        = vpss_reg->VPSS_RFRSTRIDE.bits.rfry_stride;
    HI_U32 rfr_c_stride        = vpss_reg->VPSS_RFRSTRIDE.bits.rfrc_stride;

    HI_U32 cue_y_addr          = vpss_reg->VPSS_CUEYADDR.u32;
    HI_U32 cue_c_addr          = vpss_reg->VPSS_CUECADDR.u32;
    HI_U32 cue_y_stride        = vpss_reg->VPSS_CUESTRIDE.bits.cuey_stride;
    HI_U32 cue_c_stride        = vpss_reg->VPSS_CUESTRIDE.bits.cuec_stride;

    HI_U32 prjv_nx2_addr       = vpss_reg->VPSS_PRJVNX2ADDR.u32;
    HI_U32 prjh_nx2_addr       = vpss_reg->VPSS_PRJHNX2ADDR.u32;
    HI_U32 prjv_nx2_stride     = vpss_reg->VPSS_PRJNX2STRIDE.bits.prjh_nx2_stride;
    HI_U32 prjh_nx2_stride     = vpss_reg->VPSS_PRJNX2STRIDE.bits.prjh_nx2_stride;

    HI_U32 blkmv_nx2_addr      = vpss_reg->VPSS_BLKMVNX2ADDR.u32;
    HI_U32 blkmv_nx2_stride    = vpss_reg->VPSS_BLKMVNX2STRIDE.bits.blkmv_nx2_stride;

    HI_U32 rgmv_nx2_addr       = vpss_reg->VPSS_RGMVNX2ADDR.u32;
    HI_U32 rgmv_nx2_stride     = vpss_reg->VPSS_RGMVNX2STRIDE.bits.rgmv_nx2_stride;

    HI_U32 tnr_wmad_addr       = vpss_reg->VPSS_MAD_WADDR.u32;
    HI_U32 tnr_wmad_stride     = vpss_reg->VPSS_MADSTRIDE.bits.mad_stride;

    HI_U32 dei_wst_addr        = vpss_reg->VPSS_STWADDR.u32;
    HI_U32 dei_wst_stride      = vpss_reg->VPSS_STSTRIDE.bits.st_stride;

    //VC1
    HI_U32 nx2_vc1_en          = vpss_reg->VPSS_CTRL.bits.nx2_vc1_en;
    HI_U32 nx1_vc1_en          = vpss_reg->VPSS_CTRL.bits.nx1_vc1_en;
    HI_U32 cur_vc1_en          = vpss_reg->VPSS_CTRL.bits.cur_vc1_en;
    HI_U32 ref_vc1_en          = vpss_reg->VPSS_CTRL.bits.ref_vc1_en;

    //TNR
    HI_U32 tnr_mode            = 0;//vpss_reg->VPSS_CTRL3.bits.tnr_mode;
    HI_U32 tnr_en              = vpss_reg->VPSS_CTRL.bits.tnr_en;
    HI_U32 rgme_en             = vpss_reg->VPSS_CTRL.bits.rgme_en;
    HI_U32 motionedgeen        = 0;//vpss_reg->VPSS_SNR_ENABLE.bits.motionedgeen;
    HI_U32 sharemotionen       = vpss_reg->VPSS_TNR_CONTRL.bits.sharemotionen;
    HI_U32 cbcrupdateen        = vpss_reg->VPSS_TNR_MAMC_CBCR_UPEN.bits.cbcrupdateen;
    HI_U32 ymotionmode         = vpss_reg->VPSS_TNR_FILR_MODE.bits.ymotionmode;
    HI_U32 ymotioncalcmode     = vpss_reg->VPSS_TNR_MAMD_CTRL.bits.ymotioncalcmode;
    HI_U32 cmotioncalcmode     = vpss_reg->VPSS_TNR_MAMD_CTRL.bits.cmotioncalcmode;
    HI_U32 motionestimateen    = vpss_reg->VPSS_TNR_MAME_CTRL.bits.motionestimateen;
    HI_U32 noisedetecten       = vpss_reg->VPSS_TNR_MAND_CTRL.bits.noisedetecten;
    HI_U32 tnr_tmu_addr        = vpss_reg->VPSS_TNR_ADDR.u32;
    HI_U32 rfr_en              = vpss_reg->VPSS_CTRL.bits.rfr_en;

    //DEI
    HI_U32 dei_en              = vpss_reg->VPSS_CTRL.bits.dei_en;
    HI_U32 mcdi_en             = vpss_reg->VPSS_CTRL.bits.mcdi_en;
    HI_U32 ifmd_en             = vpss_reg->VPSS_CTRL.bits.ifmd_en;
    HI_U32 igbm_en             = vpss_reg->VPSS_CTRL.bits.igbm_en;
    HI_U32 mcstartr            = vpss_reg->VPSS_MCDI_LCDINEW0.bits.mcstartr               ;//0; //8bit; [0,255]
    HI_U32 mcstartc            = vpss_reg->VPSS_MCDI_LCDINEW0.bits.mcstartc               ;//0; //8bit; [0,255]
    HI_U32 mcendr              = vpss_reg->VPSS_MCDI_LCDINEW0.bits.mcendr                 ;//0; //8bit; [0,255]
    HI_U32 mcendc              = vpss_reg->VPSS_MCDI_LCDINEW0.bits.mcendc                 ;//0; //8bit; [0,255]
    HI_U32 pre_info_en         = vpss_reg->VPSS_DIEHISMODE.bits.pre_info_en                            ;
    HI_U32 ppre_info_en        = vpss_reg->VPSS_DIEHISMODE.bits.ppre_info_en                           ;
    HI_U32 dei_lmmode          = vpss_reg->VPSS_DIECTRL.bits.die_l_mode ; //0:5  1:4  2:3
    HI_U32 dei_cmmode          = vpss_reg->VPSS_DIECTRL.bits.die_c_mode;
    HI_U32 dei_tmu_addr        = vpss_reg->VPSS_DEI_ADDR.u32;

    //SNR
    HI_U32 snr_en              = vpss_reg->VPSS_CTRL.bits.snr_en;
    HI_U32 db_en               = vpss_reg->VPSS_DB_CTRL.bits.db_en;
    HI_U32 blk_det_en          = vpss_reg->VPSS_CTRL.bits.blk_det_en;
    HI_U32 det_hy_en           = vpss_reg->VPSS_DB_CTRL.bits.det_hy_en;
    HI_U32 det_hc_en           = 0;
    HI_U32 det_vy_en           = 0;
    HI_U32 snr_tmu_addr        = vpss_reg->VPSS_SNR_ADDR.u32;

    //VPZME
    HI_U32 vhd0_pre_vfir_en    = vpss_reg->VPSS_CTRL3.bits.vhd0_pre_vfir_en;
    HI_U32 vhd0_pre_vfir_mode  = vpss_reg->VPSS_VPZME_CFG0.bits.vhd0_pre_vfir_mode;
    //CROP
    HI_U32 vhd0_crop_en        = vpss_reg->VPSS_CTRL3.bits.vhd0_crop_en;
    HI_U32 vhd0_crop_x         = vpss_reg->VPSS_VHD0CROP_POS.bits.vhd0_crop_x;
    HI_U32 vhd0_crop_y         = vpss_reg->VPSS_VHD0CROP_POS.bits.vhd0_crop_y;
    HI_U32 vhd0_crop_width     = vpss_reg->VPSS_VHD0CROP_SIZE.bits.vhd0_crop_width + 1;
    HI_U32 vhd0_crop_height    = vpss_reg->VPSS_VHD0CROP_SIZE.bits.vhd0_crop_height + 1;
    //ZME
    HI_U32 vhd0_zme_en         = vpss_reg->VPSS_CTRL3.bits.zme_vhd0_en;
    HI_U32 vhd0_zme_ow         = vpss_reg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow+1;
    HI_U32 vhd0_zme_oh         = vpss_reg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh+1;
	HI_U32 vhd0_zme_order      = vpss_reg->VPSS_VHD0_HSP.bits.hfir_order;
    HI_U32 vhd0_hlmsc_en       = vpss_reg->VPSS_VHD0_HSP.bits.hlmsc_en;
    HI_U32 vhd0_vlmsc_en       = vpss_reg->VPSS_VHD0_VSP.bits.vlmsc_en;
    HI_U32 vhd0_hchmsc_en      = vpss_reg->VPSS_VHD0_HSP.bits.hchmsc_en;
    HI_U32 vhd0_vchmsc_en      = vpss_reg->VPSS_VHD0_VSP.bits.vchmsc_en;
    HI_U32 vhd0_zme_outfmt     = vpss_reg->VPSS_VHD0_VSP.bits.zme_out_fmt;
    HI_U32 vhd0_zme_tmu_addr   = vpss_reg->VPSS_ZME_ADDR.u32;
    //LBA
    HI_U32 vhd0_lba_en         = vpss_reg->VPSS_CTRL3.bits.vhd0_lba_en;
    HI_U32 vhd0_lba_dis_width  = vpss_reg->VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_width  + 1;
    HI_U32 vhd0_lba_dis_height = vpss_reg->VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_height + 1;
    HI_U32 vhd0_lba_xfpos      = vpss_reg->VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_xfpos;
    HI_U32 vhd0_lba_yfpos      = vpss_reg->VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_yfpos;
    //VHD0
    HI_U32 vhd0_en             = vpss_reg->VPSS_CTRL.bits.vhd0_en;
    HI_U32 vhd0_pix_bitw       = vpss_reg->VPSS_VHD0CTRL.bits.vhd0_pix_bitw;
    HI_U32 vhd0_format         = vpss_reg->VPSS_VHD0CTRL.bits.vhd0_format;
    HI_U32 vhd0_out_width      = vpss_reg->VPSS_VHD0SIZE.bits.vhd0_width + 1;
    HI_U32 vhd0_out_height     = vpss_reg->VPSS_VHD0SIZE.bits.vhd0_height + 1;
    HI_U32 cts_en              = vpss_reg->VPSS_IN_CTRL.bits.cts_en;
    //HI_U32 vhd0_flip_en        = vpss_reg->VPSS_VHD0CTRL.bits.vhd0_flip;
    HI_U32 vhd0_tunl_en        = vpss_reg->VPSS_VHD0_TUNL_CTRL.bits.vhd0_tunl_en;
    

    //**************************//
    //       PRINT REG CFG
    //**************************//
    switch(in_format)
    {
        case 0x0://LINEAR_SP420   :
        case 0x3://SP422_1X2      :
        case 0x7://PLANAR_422_1X2 :
        case 0x4://PLANAR_400     :
        case 0x8://PLANAR_420     :
        case 0xa://PLANAR_410     :
        {
            YC_422 = 0;
        }
        break;

        case 0x1://SP422_2X1      :
        case 0x5://PLANAR_444     :
        case 0x6://PLANAR_422_2X1 :
        case 0x9://PLANAR_411     :
        case 0xb://PACKAGE422_YUYV:
        case 0xc://PACKAGE422_YVYU:
        case 0xd://PACKAGE422_UYVY:
        case 0xe://ARGB_8888       :
        case 0xf://ABGR_8888       :
        case 0x10://ARGB_1555       :
        case 0x11://ABGR_1555       :
        case 0x12://RGB_565         :
        case 0x13://BGR_565         :
        {
            YC_422 = 1;
        }
        break;
    }
    if (bfield == 1) //field in, offy is field's offsety. It should turn to frame's offsety.
    {
        ref_offy *= 2;
        cur_offy *= 2;
        nx1_offy *= 2;
        nx2_offy *= 2;
    }
   
    //**************************//
    //       CHECK REG CFG
    //**************************//
    //MAC && BUS
    //type/linear type
    if((in_format==0xb)||(in_format==0xc)||(in_format==0xd))//package
    {
        VPSS_ASSERT(cur_type == 0);
        VPSS_ASSERT(ref_type == 0);
        VPSS_ASSERT(nx1_type == 0);
        VPSS_ASSERT(nx2_type == 0);
    }
    if(bfield==1) {VPSS_ASSERT(nx2_type == 0);}
    //DCMP
    if(nx2_dcmp_en==1) 
    {
        VPSS_ASSERT(nx2_type == 1);
        VPSS_ASSERT(nx2_y_head_addr != 0);
        VPSS_ASSERT(nx2_c_head_addr != 0);
    }
    if(nx2_dcmp_en==1 || nx2_type==1)
    {
        VPSS_ASSERT(dei_en == 0);
        VPSS_ASSERT(tnr_en == 0);
        VPSS_ASSERT(snr_en == 0);
    }
    //offset
    VPSS_ASSERT((nx2_offx%2) == 0);
    VPSS_ASSERT((nx2_offy%2) == 0);
    VPSS_ASSERT((nx1_offx%2) == 0);
    VPSS_ASSERT((nx1_offy%2) == 0);
    VPSS_ASSERT((cur_offx%2) == 0);
    VPSS_ASSERT((cur_offy%2) == 0);
    VPSS_ASSERT((ref_offx%2) == 0);
    VPSS_ASSERT((ref_offy%2) == 0);
    if(bfield==1&&YC_422==0)
    {
        VPSS_ASSERT((nx2_offy%4) == 0);
        VPSS_ASSERT((nx1_offy%4) == 0);
        VPSS_ASSERT((cur_offy%4) == 0);
        VPSS_ASSERT((ref_offy%4) == 0);
    }
    if (in_format>0x4) {VPSS_ASSERT((nx2_offx==0)&&(nx2_offy==0));}
    //width & height
    VPSS_ASSERT(nx2_width  > 1);
    VPSS_ASSERT(nx2_height > 1);
    if ((bfield == 1)&&(YC_422==0)) //field in, offy is field's offsety. It should turn to frame's offsety.
    {
        VPSS_ASSERT((nx2_height%4) == 0);
    }
    //4p/c
    if(four_pix_en==1)
    {
        VPSS_ASSERT((nx2_offx%4) == 0);
        VPSS_ASSERT((nx2_width%4) == 0);
        VPSS_ASSERT(in_format<0x5);
        VPSS_ASSERT(nx2_vc1_en==0);
        VPSS_ASSERT(nx1_vc1_en==0);
        VPSS_ASSERT(cur_vc1_en==0);
        VPSS_ASSERT(ref_vc1_en==0);

        VPSS_ASSERT(tnr_en==0);
        VPSS_ASSERT(dei_en==0);
        VPSS_ASSERT(snr_en==0);
    }
    //10bit
    if(in_pix_bitw==1)
    {
    	if (cts_en)
    	{
        	VPSS_ASSERT(four_pix_en==1);
        }
        VPSS_ASSERT(nx2_vc1_en==0);
        VPSS_ASSERT(nx1_vc1_en==0);
        VPSS_ASSERT(cur_vc1_en==0);
        VPSS_ASSERT(ref_vc1_en==0);

        VPSS_ASSERT(tnr_en==0);
        VPSS_ASSERT(dei_en==0);
        VPSS_ASSERT(snr_en==0);

        VPSS_ASSERT(vhd0_pre_vfir_en==0);
        VPSS_ASSERT(vhd0_crop_en==0);
        VPSS_ASSERT(vhd0_zme_en==0);
        if (cts_en)
    	{
        	VPSS_ASSERT(vhd0_lba_en==0);
        }
    }
    VPSS_ASSERT(vhd0_pix_bitw==in_pix_bitw);
    //input tunnel
    if (cur_tunl_en==1){VPSS_ASSERT(rotate_en == 0);VPSS_ASSERT(bfield == 0);VPSS_ASSERT(in_format < 5);VPSS_ASSERT((nx2_offx==0)&&(nx2_offy==0));}

    //GET width height format
    vpss_tmp_width       = nx2_width;//for size check
    vpss_tmp_height      = nx2_height;
    vpss_tmp_format      = YC_422;

    //VC1
    if(nx2_vc1_en || nx1_vc1_en || cur_vc1_en || ref_vc1_en)
    {
        VPSS_ASSERT((vpss_tmp_width%16)  == 0);
        VPSS_ASSERT(vpss_tmp_format  == 0);//only 420
        if(bfield == 1)
        {    VPSS_ASSERT((vpss_tmp_height%32) == 0);}
        else
        {    VPSS_ASSERT((vpss_tmp_height%16) == 0);}
    }

    //TNR
    if(tnr_en==1) {
        if(sharemotionen==1) {VPSS_ASSERT(vpss_tmp_width%4==0);}
        if(vpss_tmp_width>720) {VPSS_ASSERT(rgme_en==0);}
        if(vpss_tmp_height>576) {VPSS_ASSERT(rgme_en==0);}
        VPSS_ASSERT(vpss_tmp_width  == ref_width);
        VPSS_ASSERT(vpss_tmp_height == ref_height);

        VPSS_ASSERT(cur_y_addr != 0);
        VPSS_ASSERT(cur_c_addr != 0);
        VPSS_ASSERT(tnr_rmad_addr != 0);
        VPSS_ASSERT(tnr_wmad_addr != 0);
        VPSS_ASSERT(tnr_tmu_addr != 0);

        if(rgme_en==1)
        {
            VPSS_ASSERT(prjv_cur_addr != 0);
            VPSS_ASSERT(prjh_cur_addr != 0);

            VPSS_ASSERT(rgmv_cur_addr != 0);
            VPSS_ASSERT(rgmv_nx1_addr != 0);

            VPSS_ASSERT(prjv_nx2_addr != 0);
            VPSS_ASSERT(prjh_nx2_addr != 0);

            VPSS_ASSERT(rgmv_nx2_addr != 0);
        }

        if(vpss_tmp_format == 0)//while 420
        {
            VPSS_ASSERT(cbcrupdateen == 0);
            VPSS_ASSERT(ymotionmode  == 1);
        }

        VPSS_ASSERT(ymotioncalcmode == 0);
        VPSS_ASSERT(cmotioncalcmode == 0);

        if(sharemotionen==1)
        {
            VPSS_ASSERT(vpss_tmp_width%4 == 0);
            //VPSS_ASSERT(vpss_tmp_width>720);//sharemotonen do no support SD size
        }
        if(motionestimateen==1)
        {
            VPSS_ASSERT(rgme_en==1);
        }
        if(noisedetecten==1)
        {
            VPSS_ASSERT(vpss_tmp_width>=64 && vpss_tmp_width<=720);
        }
    }
    if(rgme_en==1) VPSS_ASSERT(tnr_en == 1);
    if ((snr_en==1)&&(tnr_en==0)&&(bfield==0)) {VPSS_ASSERT(motionedgeen == 0);}
    if(rfr_en==1)
    {
        VPSS_ASSERT(four_pix_en == 0); 
        VPSS_ASSERT(rfr_y_addr != 0);
        VPSS_ASSERT(rfr_c_addr != 0);
        VPSS_ASSERT(rfr_y_stride != 0);
        VPSS_ASSERT(rfr_c_stride != 0);
    }

    //DEI
    if (bfield == 1)   {VPSS_ASSERT(dei_en == 1);}
    if (dei_en == 0)   {VPSS_ASSERT(ifmd_en == 0);VPSS_ASSERT(igbm_en == 0);VPSS_ASSERT(mcdi_en == 0);}
    if (dei_en == 1)   {VPSS_ASSERT(dei_lmmode == 1);}
    if (dei_en == 1)   {VPSS_ASSERT(tnr_mode == 0);}
    if (bfield == 1)   {VPSS_ASSERT((in_format!=0xb)&&(in_format!=0xc)&&(in_format!=0xd));}
    if(dei_en==1)
    {
        VPSS_ASSERT(vpss_tmp_width  == ref_width);
        VPSS_ASSERT(vpss_tmp_height == ref_height);

        if(vpss_tmp_width>720) {VPSS_ASSERT(mcdi_en==0);}
        if(vpss_tmp_height>576) {VPSS_ASSERT(mcdi_en==0);}
        VPSS_ASSERT(dei_lmmode == dei_cmmode);
        VPSS_ASSERT((ppre_info_en!=0)||(pre_info_en!=0));
        VPSS_ASSERT((mcstartr + mcendr)<=vpss_tmp_height/2-1);
        VPSS_ASSERT((mcstartc + mcendc)<=vpss_tmp_width-1);


        VPSS_ASSERT(nx1_y_addr != 0);
        VPSS_ASSERT(nx1_c_addr != 0);
        VPSS_ASSERT(cur_y_addr != 0);
        VPSS_ASSERT(cur_c_addr != 0);
        VPSS_ASSERT(ref_y_addr != 0);
        VPSS_ASSERT(ref_c_addr != 0);
        VPSS_ASSERT(dei_rst_addr != 0);
        VPSS_ASSERT(dei_wst_addr != 0);
        VPSS_ASSERT(dei_tmu_addr != 0);

        if(mcdi_en==1)
        {
            VPSS_ASSERT(ree_y_addr != 0);
            VPSS_ASSERT(ree_c_addr != 0);

            VPSS_ASSERT(prjv_cur_addr != 0);
            VPSS_ASSERT(prjh_cur_addr != 0);

            VPSS_ASSERT(rgmv_cur_addr != 0);
            VPSS_ASSERT(rgmv_nx1_addr != 0);

            VPSS_ASSERT(blkmv_nx1_addr != 0);
            VPSS_ASSERT(blkmv_cur_addr != 0);
            VPSS_ASSERT(blkmv_ref_addr != 0);

            VPSS_ASSERT(cue_y_addr != 0);
            VPSS_ASSERT(cue_c_addr != 0);

            VPSS_ASSERT(prjv_nx2_addr != 0);
            VPSS_ASSERT(prjh_nx2_addr != 0);

            VPSS_ASSERT(blkmv_nx2_addr != 0);

            VPSS_ASSERT(rgmv_nx2_addr != 0);
        }
    }

    //SNR
    if(snr_en==1)
    {
        VPSS_ASSERT(nx2_vc1_en==0);
        VPSS_ASSERT(nx1_vc1_en==0);
        VPSS_ASSERT(cur_vc1_en==0);
        VPSS_ASSERT(ref_vc1_en==0);
        VPSS_ASSERT(tnr_en==0);
        VPSS_ASSERT(dei_en==0);
        VPSS_ASSERT(vpss_tmp_width <= 720);
        VPSS_ASSERT(vpss_tmp_height <= 576);
        VPSS_ASSERT(snr_tmu_addr != 0);
        VPSS_ASSERT((in_format!=0xb)&&(in_format!=0xc)&&(in_format!=0xd));
    }

    //DBM
    if (blk_det_en == 1){VPSS_ASSERT(snr_en==1);VPSS_ASSERT(db_en==1);}
    if (blk_det_en == 0){VPSS_ASSERT(det_hy_en==0);VPSS_ASSERT(det_hc_en==0);VPSS_ASSERT(det_vy_en==0);}

    //VMUX
    vpss_vhd0_tmp_width  = vpss_tmp_width ;//for size check
    vpss_vhd0_tmp_height = vpss_tmp_height;
    vpss_vhd0_tmp_format = vpss_tmp_format;
  
    //RTT
    if(rotate_en == 1)
    {

        VPSS_ASSERT(vhd0_en     ==1);
        VPSS_ASSERT(vhd0_pix_bitw == 0); //8bit
        VPSS_ASSERT((img_pro_mode==1)||(img_pro_mode==2));

        vpss_vhd0_tmp_width  = vpss_tmp_height ;//for size check
        vpss_vhd0_tmp_height = vpss_tmp_width;
        vpss_vhd0_tmp_format = 0;//420
    }

    //VPZME
    if(vhd0_pre_vfir_en==1)
    {
        if(vhd0_pre_vfir_mode==0)
        {
            VPSS_ASSERT((vpss_vhd0_tmp_height%4)  == 0);
            vpss_vhd0_tmp_height /= 2;
        }
        else if(vhd0_pre_vfir_mode==1)
        {
            VPSS_ASSERT((vpss_vhd0_tmp_height%8)  == 0);
            vpss_vhd0_tmp_height /= 4;
        }
        else if(vhd0_pre_vfir_mode==2)
        {
            VPSS_ASSERT((vpss_vhd0_tmp_height%16) == 0);
            vpss_vhd0_tmp_height /= 8;
        }
        else  //vhd0_pre_vfir_mode == 3
        {
            VPSS_ASSERT((vpss_vhd0_tmp_height%4)  == 0);
            vpss_vhd0_tmp_height /= 2;
        }
    }

    //CROP
    if(vhd0_crop_en == 1)
    {
        VPSS_ASSERT(vhd0_crop_x + vhd0_crop_width  <= vpss_vhd0_tmp_width);
        VPSS_ASSERT(vhd0_crop_y + vhd0_crop_height <= vpss_vhd0_tmp_height);

        if(four_pix_en==1)
        {
            VPSS_ASSERT((vhd0_crop_x%4) == 0);
            VPSS_ASSERT((vhd0_crop_width%4) == 0);
        }

        vpss_vhd0_tmp_width  = vhd0_crop_width ;
        vpss_vhd0_tmp_height = vhd0_crop_height;

    }

    //ZME
    if(vhd0_zme_en == 1)
    {
        VPSS_ASSERT(vpss_vhd0_tmp_width <= 4096);
        VPSS_ASSERT(vpss_vhd0_tmp_height <= 4096);

        //printk("vpss_vhd0_tmp_width:%d, vpss_vhd0_tmp_height:%d", vpss_vhd0_tmp_width, vpss_vhd0_tmp_height);

        VPSS_ASSERT(vhd0_zme_ow <= 1920);
//        VPSS_ASSERT(vhd0_zme_oh <= 1080); //ZME只关注输出宽度<=1920, 不关注高度

        if(four_pix_en == 1)
            VPSS_ASSERT(vhd0_zme_ow%4 == 0);

		VPSS_ASSERT(vhd0_hlmsc_en == 1);
		VPSS_ASSERT(vhd0_zme_order == 0);
        VPSS_ASSERT(vhd0_vlmsc_en == vhd0_vchmsc_en);
        VPSS_ASSERT(vhd0_hlmsc_en == vhd0_hchmsc_en);

        VPSS_ASSERT(vhd0_zme_tmu_addr != 0);
        if(vpss_vhd0_tmp_format != vhd0_format)
        {
            VPSS_ASSERT(vhd0_zme_en == 1);
            VPSS_ASSERT(vhd0_vlmsc_en == 1);
            VPSS_ASSERT(vhd0_vchmsc_en == 1);
        }

        vpss_vhd0_tmp_format = (vhd0_zme_outfmt==1)?0:1;
        vpss_vhd0_tmp_width  = vhd0_zme_ow ;
        vpss_vhd0_tmp_height = vhd0_zme_oh;
    }

    //LBA
    if(vhd0_lba_en == 1)
    {
        VPSS_ASSERT((vhd0_lba_dis_width  >= vpss_vhd0_tmp_width + vhd0_lba_xfpos));
        VPSS_ASSERT((vhd0_lba_dis_height >= vpss_vhd0_tmp_height+ vhd0_lba_yfpos));

        if(four_pix_en==1)
        {
            VPSS_ASSERT((vhd0_crop_x%4) == 0);
            VPSS_ASSERT((vhd0_lba_dis_width%4) == 0);
        }

        vpss_vhd0_tmp_width  = vhd0_lba_dis_width ;
        vpss_vhd0_tmp_height = vhd0_lba_dis_height;
    }

    //VHD0
    //output tunnel
    if (vhd0_tunl_en==1){VPSS_ASSERT(vhd0_flip == 0);}
    if (rotate_en==1){VPSS_ASSERT(vhd0_format == 0);}
    VPSS_ASSERT(vhd0_format == vpss_vhd0_tmp_format);
    VPSS_ASSERT(vpss_vhd0_tmp_width  == vhd0_out_width);
    VPSS_ASSERT(vpss_vhd0_tmp_height == vhd0_out_height);
    if(cts_en==1){VPSS_ASSERT(vhd0_pix_bitw==1);}

	if ((HI_TRUE == bErrFlag) || (HI_TRUE == bForcePrintInfo))
	{
	    if (HI_TRUE == bErrFlag)
	    {
		    printk("\n###############\nVPSS Assert at Node	 = %d\n###############\n", s32NodeId);
		}
		else// (HI_TRUE == bForcePrintInfo)
		{
            printk("\n###############\nVPSS Node(%d) Info\n###############\n", s32NodeId);
		}

	    printk("bfield               = %d;\n",bfield              );
		printk("bfield_mode  		 = %d;\n",bfield_mode		  );
		printk("bSecurity	  		 = %d;\n",prot				  );
		
	    
	    printk("in_format            = %d;\n",in_format           );
	    printk("nx2_offx             = %d;\n",nx2_offx            );
	    printk("nx2_offy             = %d;\n",nx2_offy            );
	    printk("nx1_offx             = %d;\n",nx1_offx            );
	    printk("nx1_offy             = %d;\n",nx1_offy            );
	    printk("cur_offx             = %d;\n",cur_offx            );
	    printk("cur_offy             = %d;\n",cur_offy            );
	    printk("ref_offx             = %d;\n",ref_offx            );
	    printk("ref_offy             = %d;\n",ref_offy            );
	    printk("cur_tunl_en          = %d;\n",cur_tunl_en         );
	    printk("rotate_en            = %d;\n",rotate_en           );
	    printk("vhd0_tunl_en         = %d;\n",vhd0_tunl_en        );
	    printk("vhd0_flip            = %d;\n",vhd0_flip           );
	    printk("four_pix_en          = %d;\n",four_pix_en         );
	    printk("img_pro_mode         = %d;\n",img_pro_mode        );

	    printk("cur_type             = %d;\n",cur_type            );
	    printk("ref_type             = %d;\n",ref_type            );
	    printk("nx1_type             = %d;\n",nx1_type            );
	    printk("nx2_type             = %d;\n",nx2_type            );
	 
	    printk("nx2_dcmp_en          = %d;\n",nx2_dcmp_en         );
	    printk("in_pix_bitw          = %d;\n",in_pix_bitw         );

	    printk("nx2_width            = %d;\n",nx2_width           );
	    printk("nx2_height           = %d;\n",nx2_height          );

	    printk("ref_width            = %d;\n",ref_width           );
	    printk("ref_height           = %d;\n",ref_height          );


	    printk("nx2_y_addr           = %x;\n",nx2_y_addr          );
	    printk("nx2_c_addr           = %x;\n",nx2_c_addr          );
	    printk("nx2_y_stride         = %d;\n",nx2_y_stride        );
	    printk("nx2_c_stride         = %d;\n",nx2_c_stride        );
	    printk("nx2_y_head_addr      = %x;\n",nx2_y_head_addr     );
	    printk("nx2_c_head_addr      = %x;\n",nx2_c_head_addr     );
	    printk("nx2_head_stride      = %d;\n",nx2_head_stride     );

	    printk("nx1_y_addr           = %x;\n",nx1_y_addr          );
	    printk("nx1_c_addr           = %x;\n",nx1_c_addr          );
	    printk("nx1_y_stride         = %d;\n",nx1_y_stride        );
	    printk("nx1_c_stride         = %d;\n",nx1_c_stride        );

	    printk("cur_y_addr           = %x;\n",cur_y_addr          );
	    printk("cur_c_addr           = %x;\n",cur_c_addr          );
	    printk("cur_y_stride         = %d;\n",cur_y_stride        );
	    printk("cur_c_stride         = %d;\n",cur_c_stride        );

	    printk("ref_y_addr           = %x;\n",ref_y_addr          );
	    printk("ref_c_addr           = %x;\n",ref_c_addr          );
	    printk("ref_y_stride         = %d;\n",ref_y_stride        );
	    printk("ref_c_stride         = %d;\n",ref_c_stride        );

	    printk("ree_y_addr           = %x;\n",ree_y_addr          );
	    printk("ree_c_addr           = %x;\n",ree_c_addr          );
	    printk("ree_y_stride         = %d;\n",ree_y_stride        );
	    printk("ree_c_stride         = %d;\n",ree_c_stride        );

	    printk("prjv_cur_addr        = %x;\n",prjv_cur_addr       );
	    printk("prjh_cur_addr        = %x;\n",prjh_cur_addr       );
	    printk("prjv_cur_stride      = %d;\n",prjv_cur_stride     );
	    printk("prjh_cur_stride      = %d;\n",prjh_cur_stride     );

	    printk("rgmv_cur_addr        = %x;\n",rgmv_cur_addr       );
	    printk("rgmv_cur_stride      = %d;\n",rgmv_cur_stride     );
	    printk("rgmv_nx1_addr        = %x;\n",rgmv_nx1_addr       );
	    printk("rgmv_nx1_stride      = %d;\n",rgmv_nx1_stride     );

	    printk("blkmv_nx1_addr       = %x;\n",blkmv_nx1_addr      );
	    printk("blkmv_nx1_stride     = %d;\n",blkmv_nx1_stride    );
	    printk("blkmv_cur_addr       = %x;\n",blkmv_cur_addr      );
	    printk("blkmv_cur_stride     = %d;\n",blkmv_cur_stride    );
	    printk("blkmv_ref_addr       = %x;\n",blkmv_ref_addr      );
	    printk("blkmv_ref_stride     = %d;\n",blkmv_ref_stride    );

	    printk("tnr_rmad_addr        = %d;\n",tnr_rmad_addr       );
	    printk("tnr_rmad_stride      = %d;\n",tnr_rmad_stride     );

	    printk("snr_rmad_addr        = %x;\n",snr_rmad_addr       );
	    printk("snr_rmad_stride      = %d;\n",snr_rmad_stride     );

	    printk("dei_rst_addr         = %x;\n",dei_rst_addr        );
	    printk("dei_rst_stride       = %d;\n",dei_rst_stride      );

	    printk("vhd0_y_addr          = %x;\n",vhd0_y_addr         );
	    printk("vhd0_c_addr          = %x;\n",vhd0_c_addr         );
	    printk("vhd0_y_stride        = %d;\n",vhd0_y_stride       );
	    printk("vhd0_c_stride        = %d;\n",vhd0_c_stride       );

	    printk("rfr_y_addr           = %x;\n",rfr_y_addr          );
	    printk("rfr_c_addr           = %x;\n",rfr_c_addr          );
	    printk("rfr_y_stride         = %d;\n",rfr_y_stride        );
	    printk("rfr_c_stride         = %d;\n",rfr_c_stride        );

	    printk("cue_y_addr           = %x;\n",cue_y_addr          );
	    printk("cue_c_addr           = %x;\n",cue_c_addr          );
	    printk("cue_y_stride         = %d;\n",cue_y_stride        );
	    printk("cue_c_stride         = %d;\n",cue_c_stride        );

	    printk("prjv_nx2_addr        = %x;\n",prjv_nx2_addr       );
	    printk("prjh_nx2_addr        = %x;\n",prjh_nx2_addr       );
	    printk("prjv_nx2_stride      = %d;\n",prjv_nx2_stride     );
	    printk("prjh_nx2_stride      = %d;\n",prjh_nx2_stride     );

	    printk("blkmv_nx2_addr       = %x;\n",blkmv_nx2_addr      );
	    printk("blkmv_nx2_stride     = %d;\n",blkmv_nx2_stride    );

	    printk("rgmv_nx2_addr        = %x;\n",rgmv_nx2_addr       );
	    printk("rgmv_nx2_stride      = %d;\n",rgmv_nx2_stride     );

	    printk("tnr_wmad_addr        = %x;\n",tnr_wmad_addr       );
	    printk("tnr_wmad_stride      = %d;\n",tnr_wmad_stride     );

	    printk("dei_wst_addr         = %x;\n",dei_wst_addr        );
	    printk("dei_wst_stride       = %d;\n",dei_wst_stride      );


	    printk("nx2_vc1_en           = %d;\n",nx2_vc1_en          );
	    printk("nx1_vc1_en           = %d;\n",nx1_vc1_en          );
	    printk("cur_vc1_en           = %d;\n",cur_vc1_en          );
	    printk("ref_vc1_en           = %d;\n",ref_vc1_en          );


	    printk("tnr_mode             = %d;\n",tnr_mode            );
	    printk("tnr_en               = %d;\n",tnr_en              );
	    printk("rgme_en              = %d;\n",rgme_en             );
	    printk("motionedgeen         = %d;\n",motionedgeen        );
	    printk("sharemotionen        = %d;\n",sharemotionen       );
	    printk("cbcrupdateen         = %d;\n",cbcrupdateen        );
	    printk("ymotionmode          = %d;\n",ymotionmode         );
	    printk("ymotioncalcmode      = %d;\n",ymotioncalcmode     );
	    printk("cmotioncalcmode      = %d;\n",cmotioncalcmode     );
	    printk("motionestimateen     = %d;\n",motionestimateen    );
	    printk("noisedetecten        = %d;\n",noisedetecten       );
	    printk("tnr_tmu_addr         = %x;\n",tnr_tmu_addr        );


	    printk("dei_en               = %d;\n",dei_en              );
	    printk("mcdi_en              = %d;\n",mcdi_en             );
	    printk("ifmd_en              = %d;\n",ifmd_en             );
	    printk("igbm_en              = %d;\n",igbm_en             );
	    printk("mcstartr             = %d;\n",mcstartr            );
	    printk("mcstartc             = %d;\n",mcstartc            );
	    printk("mcendr               = %d;\n",mcendr              );
	    printk("mcendc               = %d;\n",mcendc              );
	    printk("pre_info_en          = %d;\n",pre_info_en         );
	    printk("ppre_info_en         = %d;\n",ppre_info_en        );
	    printk("dei_lmmode           = %d;\n",dei_lmmode          );
	    printk("dei_cmmode           = %d;\n",dei_cmmode          );
	    printk("dei_tmu_addr         = %x;\n",dei_tmu_addr        );


	    printk("snr_en               = %d;\n",snr_en              );
	    printk("db_en                = %d;\n",db_en               );
	    printk("blk_det_en           = %d;\n",blk_det_en          );
	    printk("det_hy_en            = %d;\n",det_hy_en           );
	    printk("det_hc_en            = %d;\n",det_hc_en           );
	    printk("det_vy_en            = %d;\n",det_vy_en           );
	    printk("snr_tmu_addr         = %x;\n",snr_tmu_addr        );


	    printk("vhd0_pre_vfir_en     = %d;\n",vhd0_pre_vfir_en    );
	    printk("vhd0_pre_vfir_mode   = %d;\n",vhd0_pre_vfir_mode  );

	    printk("vhd0_crop_en         = %d;\n",vhd0_crop_en        );
	    printk("vhd0_crop_x          = %d;\n",vhd0_crop_x         );
	    printk("vhd0_crop_y          = %d;\n",vhd0_crop_y         );
	    printk("vhd0_crop_width      = %d;\n",vhd0_crop_width     );
	    printk("vhd0_crop_height     = %d;\n",vhd0_crop_height    );

	    printk("vhd0_zme_en          = %d;\n",vhd0_zme_en         );
	    printk("vhd0_zme_ow          = %d;\n",vhd0_zme_ow         );
	    printk("vhd0_zme_oh          = %d;\n",vhd0_zme_oh         );
	    printk("vhd0_hlmsc_en        = %d;\n",vhd0_hlmsc_en       );
	    printk("vhd0_vlmsc_en        = %d;\n",vhd0_vlmsc_en       );
	    printk("vhd0_hchmsc_en       = %d;\n",vhd0_hchmsc_en      );
	    printk("vhd0_vchmsc_en       = %d;\n",vhd0_vchmsc_en      );
	    printk("vhd0_zme_outfmt      = %d;\n",vhd0_zme_outfmt     );
	    printk("vhd0_zme_tmu_addr    = %x;\n",vhd0_zme_tmu_addr   );

	    printk("vhd0_lba_en          = %d;\n",vhd0_lba_en         );
	    printk("vhd0_lba_dis_width   = %d;\n",vhd0_lba_dis_width  );
	    printk("vhd0_lba_dis_height  = %d;\n",vhd0_lba_dis_height );
	    printk("vhd0_lba_xfpos       = %d;\n",vhd0_lba_xfpos      );
	    printk("vhd0_lba_yfpos       = %d;\n",vhd0_lba_yfpos      );

	    printk("vhd0_en              = %d;\n",vhd0_en             );
	    printk("vhd0_pix_bitw        = %d;\n",vhd0_pix_bitw       );
	    printk("vhd0_format          = %d;\n",vhd0_format         );
	    printk("vhd0_out_width       = %d;\n",vhd0_out_width      );
	    printk("vhd0_out_height      = %d;\n",vhd0_out_height     );
	}


}

