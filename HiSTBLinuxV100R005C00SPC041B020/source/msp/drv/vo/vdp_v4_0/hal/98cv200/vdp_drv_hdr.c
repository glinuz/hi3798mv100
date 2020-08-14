#include "hi_type.h"
#include "vdp_define.h"
#include "vdp_drv_pq_hdr.h"
#include "vdp_drv_hdr.h"
#include "vdp_hal_hdr.h"
#include "vdp_hal_vid.h"
#include "vdp_hal_gfx.h"
#include "vdp_hal_mmu.h"
#include "vdp_hal_chn.h"
#include "drv_pq_ext.h"
#include "vdp_drv_comm.h"
#include "vdp_drv_pq_zme.h"
#include "vdp_hal_comm.h"
#if VDP_CBB_FPGA
#include "hi_drv_mmz.h"
#endif
extern HI_U32 gdm_gmm_lut[64]; 
extern HI_U32 gdm_degmm_lut[64]; 

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;

HI_S32 VDP_DRV_SetHdrMode(VDP_HDR_CFG_S *stHdrCfg, rpu_ext_config_fixpt_main_t *pstCompCfg, DmKsFxp_t *pstDmKs)
{

	//enable
	HI_U32 hdr_en            = 1;
	HI_U32 bl_en             = 1;
	HI_U32 el_en             = 1;
	HI_U32 composer_en       = 1;
	HI_U32 edr_en            = 0;
	HI_U32 edr_md_en         = 0;
	HI_U32 edr_ds_en         = 0;
    VDP_VP_MUTE_BK_S        stVpMuteBkg={0};

	//vdm
	HI_U32 vdm_yuv2rgb_en    = 1;
	HI_U32 vdm_yuv2rgb_v0_en = 1;
	HI_U32 vdm_yuv2rgb_v1_en = 1;
	HI_U32 vdm_norm_en       = 1;
	HI_U32 vdm_degmm_en      = 1;
	HI_U32 vdm_rgb2lms_en    = 1;
	HI_U32 vdm_gmm_en        = 1;
	HI_U32 vdm_gmm_v0_en     = 1;
	HI_U32 vdm_gmm_v1_en     = 1;

	//cvm1:full spec
	HI_U32 cvm_sel           = 0;//0: vid select cvm1; 1: gfx select cvm1
	HI_U32 vcvm_en           = 1;
	HI_U32 cvm1_lms2ipt_en   = 1;
	HI_U32 cvm1_ipt2lms_en   = 1;
	HI_U32 cvm1_lms2rgb_en   = 1;
	HI_U32 cvm1_rgb2yuv_en   = 1;
	HI_U32 cvm1_denorm_en    = 1;
    HI_U32 cvm1_degmm_en     = 1;
    HI_U32 cvm1_gmm_en       = 1;

    //gdm
	HI_U32 gdm_en            = 0;
	HI_U32 gdm_rgb2lms_en    = 0;
	HI_U32 gdm_degmm_en      = 0;
	HI_U32 gdm_gmm_en        = 0;
	HI_U32 cvm2_lms2ipt_en   = 1;
	HI_S32 cvm2_lms2ipt_out_dc[3] = {0};

	HI_S32 csc_offset[3] = {0};
	HI_S16 csc_m3[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	HI_U32 coef_shift        = 0;
    HI_PQ_ZME_COEF_RATIO_E enHiPqZmeCoefRatio;

    VDP_RECT_S stVp0DispReso; 
    VDP_RECT_S stVp0VidReso; 
	VDP_RECT_S stOutReso ; 
	VDP_RECT_S stVidReso ;
	VDP_RECT_S stDispReso ;
    
//	HI_S16 g_lms2ipt[3][3];
//S	HI_S16 m33Rgb2LmsTmp[3][3];
	//-------for ec csc---------//
	HI_U32 u32Pre	=0; 
	HI_U32 u32DcPre  =0;
	HI_U32 vcvm_csc_en =1;
	HI_U32	u32Scale=0;
	HI_S16 m33Yuv2RgbScale2P=0;
	HI_S16 v_imap_lms2ipt[3][3]={{0,0,0}};
	HI_S32 s32CscDc[3]={0};

	stOutReso  = stHdrCfg->stBlReso;
	stVidReso  = stHdrCfg->stBlReso;
	stDispReso = stHdrCfg->stBlReso;

    VDP_SetCheckSumEnable(1);//enable check sum.
	VDP_VID_SetMuteEnable              (VDP_LAYER_VID0, stHdrCfg->bMuteEn);
	VDP_VID_SetMuteEnable              (VDP_LAYER_VID1, stHdrCfg->bMuteEn);
	//-------end for ec csc---------//


	//--------add by hyx for ec csc-----------------------//
	VDP_HDR_SetVDmImapRgb2lms(csc_m3);
	VDP_HDR_SetVDmImapRgb2lmsscale2p(0);
	VDP_HDR_SetVDmRgb2lmsEn(1);
	
	 u32Pre = 1 << 10;
	 u32DcPre = 4 * 32 ;//1:8bit; 4:10bit
	
	m33Yuv2RgbScale2P	  =  15;	 //sGetRandEx(5,31);		 
	
	u32Scale	= 1 << (m33Yuv2RgbScale2P - 10);
	u32DcPre	= 4 * 64;
	
	
	v_imap_lms2ipt[0][0]	  =  (HI_S32)(1  * u32Pre*u32Scale/2);
	v_imap_lms2ipt[0][1]	  =  (HI_S32)(0  * u32Pre*u32Scale/2);
	v_imap_lms2ipt[0][2]	  =  (HI_S32)(0  * u32Pre*u32Scale/2);
	v_imap_lms2ipt[1][0]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
	v_imap_lms2ipt[1][1]	  =  (HI_S32)(1 * u32Pre*u32Scale/2);
	v_imap_lms2ipt[1][2]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
	v_imap_lms2ipt[2][0]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
	v_imap_lms2ipt[2][1]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
	v_imap_lms2ipt[2][2]	  =  (HI_S32)(1 * u32Pre*u32Scale/2);
	s32CscDc[0] =  0;	
	s32CscDc[1] =  0;	
	s32CscDc[2] =  0;	
	
	VDP_HDR_SetCvmSel	(0);
	VDP_HDR_SetVCvmLms2iptEn		(vcvm_csc_en);
	
	VDP_HDR_SetVCvmImapLms2iptscale2p(m33Yuv2RgbScale2P | (vcvm_csc_en<<5) );
	
	VDP_HDR_SetVCvmImapLms2ipt		 (v_imap_lms2ipt);
	VDP_HDR_SetVCvmImapLms2iptOutDc  (s32CscDc );
	VDP_HDR_SetVDmImapLms2iptmin	 (0);
	VDP_HDR_SetVDmImapLms2iptmax	 (0x7FFF);
	
	//-------------end----------------------//

	//----------------------------------------------------------------------
	//DOVI YUV OUTPUT
	//----------------------------------------------------------------------
	if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_DOVI_YUV_OUT)
	{
		edr_en               = 0;
	    edr_md_en            = 1;
	    edr_ds_en            = 1;
		cvm_sel              = 1;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_DOVI_YUV_OUT)
	{
		edr_en               = 0;
	    edr_md_en            = 1;
	    edr_ds_en            = 1;
		cvm_sel              = 1;

		vdm_yuv2rgb_en       = 0;
		vdm_yuv2rgb_v0_en    = 0;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 0;
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;
	    cvm2_lms2ipt_en      = 0;

	    cvm1_ipt2lms_en      = 0;
	    cvm1_lms2rgb_en      = 0;
	    cvm1_rgb2yuv_en      = 0;
	    cvm1_denorm_en       = 0;
	}
	//----------------------------------------------------------------------
	//HDR10 OUTPUT
	//----------------------------------------------------------------------
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_HDR10_OUT_CERT)
	{
	    edr_ds_en            = 1;
		cvm_sel              = 0;
		edr_en               = 1;
	    edr_md_en            = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_HDR10_OUT_CERT)
	{
	    edr_ds_en            = 1;
		cvm_sel              = 0;
		edr_en               = 1;
	    edr_md_en            = 0;

		vdm_yuv2rgb_en       = 1;
		vdm_yuv2rgb_v0_en    = 1;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 1;
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;
		cvm1_lms2ipt_en      = 1;
	    cvm2_lms2ipt_en      = 1;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT_CERT)
	{
		bl_en                = 0;
		//el_en                = 0;
		composer_en          = 0;
	    edr_ds_en            = 1;
		cvm_sel              = 1;//1;认证时要打开hyx修改
		edr_en               = 1;
	    edr_md_en            = 0;
	}
	//----------------------------------------------------------------------
	//HDR10 OUTPUT
	//----------------------------------------------------------------------
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT)
	{
		bl_en                = 0;
		//el_en                = 0;
		composer_en          = 0;
		cvm_sel              = 1;
	    edr_ds_en            = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_HDR10_OUT)
	{
	    edr_ds_en  = 0;
		cvm_sel    = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_HDR10_OUT)
	{
	    edr_ds_en            = 0;
		cvm_sel              = 0;
		edr_en               = 1;
	    edr_md_en            = 0;

		vdm_yuv2rgb_en       = 1;
		vdm_yuv2rgb_v0_en    = 1;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 1;
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;
		cvm1_lms2ipt_en      = 1;
	    cvm2_lms2ipt_en      = 1;
	}
	//----------------------------------------------------------------------
	//SDR OUTPUT
	//----------------------------------------------------------------------
	else if(stHdrCfg->enHdrMode == VDP_HDR_SDR_IN_SDR_OUT)
	{
		hdr_en               = 0;
		bl_en                = 0;
		//el_en                = 0;
		composer_en          = 0;

		vdm_yuv2rgb_en       = 0;
		vdm_yuv2rgb_v0_en    = 0;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 0;
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;

		cvm_sel              = 0;
		vcvm_en              = 0;
		cvm1_lms2ipt_en      = 0;
		cvm1_ipt2lms_en      = 0;
		cvm1_lms2rgb_en      = 0;
		cvm1_rgb2yuv_en      = 0;
		cvm1_denorm_en       = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_OUT)
	{
		bl_en                = 1;//???
		//el_en                = 1;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT)
	{
		bl_en                = 1;
		//el_en                = 0;
	    edr_ds_en            = 0;
		cvm_sel              = 0;

		edr_en               = 0;
	    edr_md_en            = 0;

		vdm_yuv2rgb_en       = 1;
		vdm_yuv2rgb_v0_en    = 1;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 1;
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;
		cvm1_lms2ipt_en      = 1;
	    cvm2_lms2ipt_en      = 1;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT)
	{
		bl_en                = 0;
		//el_en                = 0;
		composer_en          = 0;
	    edr_md_en            = 0;
	    edr_ds_en            = 0;
		cvm_sel              = 0;
	    cvm2_lms2ipt_en      = 1;
	}
	//----------------------------------------------------------------------
	//SDR OUTPUT for certificate
	//----------------------------------------------------------------------
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT_CERT)
	{
		bl_en                = 0;
		//el_en                = 0;
		composer_en          = 0;
	    edr_md_en            = 0;
	    edr_ds_en            = 1;
		cvm_sel              = 0;
	    cvm2_lms2ipt_en      = 1;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT)
	{
		bl_en             = 1;
		el_en             = 0;
	    edr_ds_en         = 1;
		cvm_sel           = 0;

		edr_en            = 0;
	    edr_md_en         = 0;

		vdm_yuv2rgb_en    = 1;
		vdm_yuv2rgb_v0_en = 1;
		vdm_yuv2rgb_v1_en = 0;
		vdm_norm_en       = 0;
		vdm_degmm_en      = 0;
		vdm_rgb2lms_en    = 1;//1:1
		vdm_gmm_en        = 0;
		vdm_gmm_v0_en     = 0;
		vdm_gmm_v1_en     = 0;
		cvm1_lms2ipt_en   = 1;
	    cvm2_lms2ipt_en   = 1;
	    cvm1_ipt2lms_en   = 0;//1:1
	    cvm1_lms2rgb_en   = 1;//1:1
	    cvm1_rgb2yuv_en   = 1;
	    cvm1_denorm_en    = 0;
        cvm1_degmm_en     = 0;
        cvm1_gmm_en       = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT)
	{
	    edr_ds_en            = 1;
	    edr_md_en            = 0;
		cvm_sel              = 0;

	    cvm1_ipt2lms_en   = 0;//1:1
	    cvm1_lms2rgb_en   = 1;//1:1
	    cvm1_rgb2yuv_en   = 1;
	    cvm1_denorm_en    = 0;
        cvm1_degmm_en     = 0;
        cvm1_gmm_en       = 0;

	}
	//----------------------------------------------------------------------
	//IPT OUTPUT
	//----------------------------------------------------------------------
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_DOVI_IPT_OUT)
	{
		edr_en               = 0;
	    edr_md_en            = 1;
	    edr_ds_en            = 1;
		cvm_sel              = 1;

		vdm_yuv2rgb_en       = 1;
		vdm_yuv2rgb_v0_en    = 1;
		vdm_yuv2rgb_v1_en    = 1;
		vdm_norm_en          = 1;
		vdm_degmm_en         = 1;
		vdm_rgb2lms_en       = 1;//1:1
		vdm_gmm_en           = 1;
		vdm_gmm_v0_en        = 1;
		vdm_gmm_v1_en        = 1;
		cvm1_lms2ipt_en      = 1;
	    cvm2_lms2ipt_en      = 1;

	    cvm1_ipt2lms_en      = 0;//1:1
	    cvm1_lms2rgb_en      = 1;//1:1
	    cvm1_rgb2yuv_en      = 1;
	    cvm1_denorm_en       = 0;
        cvm1_degmm_en        = 0;
        cvm1_gmm_en          = 0;
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT)
	{
		edr_en               = 0;
	    edr_md_en            = 1;
	    edr_ds_en            = 1;
		cvm_sel              = 1;

		vdm_yuv2rgb_en       = 1;
		vdm_yuv2rgb_v0_en    = 1;
		vdm_yuv2rgb_v1_en    = 0;
		vdm_norm_en          = 0;
		vdm_degmm_en         = 0;
		vdm_rgb2lms_en       = 1;//1:1
		vdm_gmm_en           = 0;
		vdm_gmm_v0_en        = 0;
		vdm_gmm_v1_en        = 0;
		cvm1_lms2ipt_en      = 1;
	    cvm2_lms2ipt_en      = 1;

	    cvm1_ipt2lms_en      = 0;//1:1
	    cvm1_lms2rgb_en      = 1;//1:1
	    cvm1_rgb2yuv_en      = 1;
	    cvm1_denorm_en       = 0;
        cvm1_degmm_en        = 0;
        cvm1_gmm_en          = 0;
	}
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_IPT_OUT)
	{
		bl_en             = 1;
		el_en             = 0;
	    edr_ds_en         = 0;
		cvm_sel           = 0;

		edr_en            = 0;
	    edr_md_en         = 0;

		vdm_yuv2rgb_en    = 1;
		vdm_yuv2rgb_v0_en = 1;
		vdm_yuv2rgb_v1_en = 0;
		vdm_norm_en       = 0;
		vdm_degmm_en      = 0;
		vdm_rgb2lms_en    = 1;//1:1
		vdm_gmm_en        = 0;
		vdm_gmm_v0_en     = 0;
		vdm_gmm_v1_en     = 0;
		cvm1_lms2ipt_en   = 1;
	    cvm2_lms2ipt_en   = 1;

	    cvm1_ipt2lms_en   = 0;
	    cvm1_lms2rgb_en   = 1;//1:1
	    cvm1_rgb2yuv_en   = 1;
	    cvm1_denorm_en    = 0;
        cvm1_degmm_en     = 0;
        cvm1_gmm_en       = 0;

	}
	else
	{
	}


	//----------------------------------------------------------------------
	//composer setting
	//----------------------------------------------------------------------
	VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, VDP_LAYER_VID0,1);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, VDP_LAYER_VID1,0);	
	VDP_DRV_SetComposer            (pstCompCfg);
	VDP_DRV_SetVdm                 (pstDmKs);

	VDP_HDR_SetVDmEn               (1);


	VDP_DRV_SetGdmDegammaCoef(gdm_degmm_lut);
	VDP_DRV_SetGdmgammaCoef(gdm_gmm_lut);
	
	VDP_DRV_SetDmDegammaCoef(pstDmKs);
	VDP_DRV_SetDmgammaCoef(pstDmKs);
    if(cvm_sel == 0)
    {
        VDP_DRV_SetDmCvmCoef(pstDmKs->ksTMap.tmLutI, pstDmKs->ksTMap.tmLutS, pstDmKs->ksTMap.smLutI, pstDmKs->ksTMap.smLutS);
    }
    else
    {
        VDP_DRV_SetDmCvmCoef(pstDmKs->ksTMapG.tmLutI, pstDmKs->ksTMapG.tmLutS, pstDmKs->ksTMapG.smLutI, pstDmKs->ksTMapG.smLutS);
    }
    
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_DEGMM);
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_GMM);
	VDP_HDR_SetDmInputDegmmParaup  ();
	VDP_HDR_SetDmInputGammaParaup  ();
	VDP_HDR_SetDmCvmParaup		   ();
	VDP_HDR_SetDmOutputDegmmParaup ();
	VDP_HDR_SetDmOutputGammaParaup ();


	el_en = (pstCompCfg->disable_residual_flag == 1) ? 0 : 1;
	//----------------------------------------------------------------------
	//gfx setting
	//----------------------------------------------------------------------
	{
		//VDP_FUNC_SetGdmMode            (VDP_HDR_GDM_TYP);
		VDP_DRV_SetGdm();

		gdm_en            = 1;
		gdm_rgb2lms_en    = 1;
		gdm_degmm_en      = 1;
		gdm_gmm_en        = 1;

	}

	//----------------------------------------------------------------------
	//enable setting
	//----------------------------------------------------------------------
	//VDM(V2020)
    VDP_HDR_SetVDmYuv2rgbEn        ( vdm_yuv2rgb_en    );
    VDP_HDR_SetVDmYuv2rgbV0En      ( vdm_yuv2rgb_v0_en );
    VDP_HDR_SetVDmYuv2rgbV1En      ( vdm_yuv2rgb_v1_en );

    VDP_HDR_SetVDmNormEn           ( vdm_norm_en       );
    VDP_HDR_SetVDmDegammaEn        ( vdm_degmm_en      );
    VDP_HDR_SetVDmRgb2lmsEn        ( vdm_rgb2lms_en    );
    VDP_HDR_SetVDmGammaEn          ( vdm_gmm_en        );
    VDP_HDR_SetVDmGammaV0En        ( vdm_gmm_v0_en     );
    VDP_HDR_SetVDmGammaV1En        ( vdm_gmm_v1_en     );

    //VCVM1
    VDP_HDR_SetVCvmEn              ( vcvm_en           );
    VDP_HDR_SetVCvmLms2iptEn       ( cvm1_lms2ipt_en   );
    VDP_HDR_SetVCvmIpt2lmsEn       ( cvm1_ipt2lms_en   );
    VDP_HDR_SetVCvmLms2rgbEn       ( cvm1_lms2rgb_en   );
    VDP_HDR_SetVCvmRgb2yuvEn       ( cvm1_rgb2yuv_en   );
    VDP_HDR_SetVCvmDenormEn        ( cvm1_denorm_en    );

    VDP_HDR_SetVCvmDegammaEn       ( cvm1_degmm_en     );
    VDP_HDR_SetVCvmMGammaEn        ( cvm1_gmm_en       );

    //GDM
    VDP_HDR_SetGDmEn               ( gdm_en            );
    VDP_HDR_SetGDmDegammaEn        ( gdm_degmm_en      );
    VDP_HDR_SetGDmRgb2lmsEn        ( gdm_rgb2lms_en    );
    VDP_HDR_SetGDmGammaEn          ( gdm_gmm_en        );
    VDP_HDR_SetAuthEn              ( 1                 );

    //EDR
    VDP_HDR_SetDmEdrEn             ( edr_en            );
    VDP_HDR_SetDmHdrMdEn           ( edr_md_en         );
    VDP_HDR_SetDmHdrDsEn           ( edr_ds_en         );

	if(edr_md_en == 1)
	{
		VDP_DISP_SetNxgDataSwapEnable (VDP_CHN_DHD0, HI_TRUE);
	}
    else
	{
		VDP_DISP_SetNxgDataSwapEnable (VDP_CHN_DHD0, HI_FALSE);
	}

#if 0
	if(stHdrCfg->bPreMult == HI_FALSE)
	{
		VDP_GFX_SetPreMultEnable(VDP_LAYER_GFX0,HI_FALSE);
		VDP_GFX_MixvBypassEn		   (1);
		VDP_GFX_MixvPremultEn		   (0);
	}
	else
	{
		VDP_GFX_SetPreMultEnable(VDP_LAYER_GFX0,HI_TRUE);
		VDP_GFX_MixvBypassEn		   (0);
		VDP_GFX_MixvPremultEn		   (0);
	}
#endif

	if(cvm_sel == 1) 
	{
		
		VDP_HDR_SetGCvmLms2IptEn		  ( cvm2_lms2ipt_en 					);
		VDP_HDR_SetGCvmLms2Ipt			  ( pstDmKs->ksIMap.m33Lms2Ipt			  );
		VDP_HDR_SetGCvmLms2IptScale2p	  ( pstDmKs->ksIMap.m33Lms2IptScale2P + 1 );
		VDP_HDR_SetGCvmLms2IptOutDc 	  ( cvm2_lms2ipt_out_dc 				);
		
		VDP_HDR_SetVCvmImapLms2ipt		  ( pstDmKs->ksIMapG.m33Lms2Ipt 		  );
		VDP_HDR_SetVCvmImapLms2iptscale2p ( pstDmKs->ksIMapG.m33Lms2IptScale2P + 1);
		VDP_HDR_SetVDmImapLms2iptmin	  ( -((1<<15) -1 ));
		VDP_HDR_SetVDmImapLms2iptmax	  ( ((1<<15 ) -1 ));

	}
	else
	{
		//if(stHdrCfg->bPreMult == HI_FALSE)
		{
			VDP_HDR_SetGCvmLms2IptEn          ( cvm2_lms2ipt_en                     );
			VDP_HDR_SetGCvmLms2Ipt            ( pstDmKs->ksIMapG.m33Lms2Ipt           );
			VDP_HDR_SetGCvmLms2IptScale2p     ( pstDmKs->ksIMapG.m33Lms2IptScale2P + 1);
			VDP_HDR_SetGCvmLms2IptOutDc       ( cvm2_lms2ipt_out_dc                 );

			VDP_HDR_SetVCvmImapLms2ipt        ( pstDmKs->ksIMap.m33Lms2Ipt            );
			VDP_HDR_SetVCvmImapLms2iptscale2p ( pstDmKs->ksIMap.m33Lms2IptScale2P + 1 );

			VDP_HDR_SetGCvmLms2IptMax         (((1<<15)-1));
			
		}

	}
    
    VDP_HDR_SetGCvmLms2IptMin      (0x8001);//   (-((1<<15)));

	//GDM rgb2lms
	VDP_HDR_SetGDmRgb2lmsEn               ( gdm_rgb2lms_en                      );
	VDP_HDR_SetGDmImapRgb2lms             ( pstDmKs->ksIMapG.m33Rgb2Lms           );
	VDP_HDR_SetGDmImapRgb2lmsscale2p      ( pstDmKs->ksIMapG.m33Rgb2LmsScale2P    );
	VDP_HDR_SetGDmImapRgb2lmsmin          ( 0 );
	VDP_HDR_SetGDmImapRgb2lmsmax          ( ((1<<25)  -1)       );
	VDP_HDR_SetGDmImapRgb2lmsOutDc        ( cvm2_lms2ipt_out_dc );

	//----------------------------------------------------------------------
	//data path setting
	//----------------------------------------------------------------------
	VDP_HDR_SetHdrEnable                  ( hdr_en );
	VDP_VID_SetComposerBlEn               ( bl_en  );
	VDP_VID_SetComposerElEn               ( el_en  );
	VDP_HDR_SetCvmSel                     ( cvm_sel);

	if(edr_ds_en == 1)
	{
		VDP_DISP_SetHdmiMode       (VDP_CHN_DHD0, 1);//1: no swap
	}
    else
    {
		VDP_DISP_SetHdmiMode       (VDP_CHN_DHD0, 0);//1: no swap
	}  

	//----------------------------------------------------------------------
	//BL setting
	//----------------------------------------------------------------------
	VDP_VID_SetLayerEnable             (VDP_LAYER_VID0, HI_TRUE);
    VDP_VID_SetInDataFmt               (VDP_LAYER_VID0, stHdrCfg->enVdpBlVidIFmt);
	VDP_VID_SetReadMode                (VDP_LAYER_VID0, VDP_RMODE_INTERFACE, VDP_RMODE_INTERFACE);
	VDP_VID_SetDataWidth               (VDP_LAYER_VID0, stHdrCfg->u32DataWidth);



    stVidReso.u32X = 0;
    stVidReso.u32Y = 0;
    stVidReso.u32Wth= stOutReso.u32Wth;
    stVidReso.u32Hgt= stOutReso.u32Hgt;

	
    stDispReso = stVidReso;
	stVp0VidReso  =stHdrCfg->stBlReso;
	stVp0DispReso =stHdrCfg->stBlReso;

    if(stHdrCfg->bFlag480I576I == HI_TRUE)
    {
        stOutReso.u32Wth  =2*stHdrCfg->stBlReso.u32Wth;
        stVidReso.u32Wth  =2*stHdrCfg->stBlReso.u32Wth;
        stDispReso.u32Wth  =2*stHdrCfg->stBlReso.u32Wth;
        
        stVidReso.u32X   =2*stHdrCfg->stBlReso.u32X;
        stDispReso.u32X  =2*stHdrCfg->stBlReso.u32X;

		stVp0VidReso.u32Wth =2*stHdrCfg->stBlReso.u32Wth;
		stVp0DispReso.u32Wth=2*stHdrCfg->stBlReso.u32Wth;
		stVp0VidReso.u32X	=2*stHdrCfg->stBlReso.u32X;
		stVp0DispReso.u32X	=2*stHdrCfg->stBlReso.u32X;
    }
	
	VDP_VID_SetInReso                  (VDP_LAYER_VID0, stHdrCfg->stVidReso);
	VDP_VID_SetOutReso                 (VDP_LAYER_VID0, stOutReso);
	VDP_VID_SetVideoPos                (VDP_LAYER_VID0, stVidReso);
	VDP_VID_SetDispPos                 (VDP_LAYER_VID0, stDispReso);

    VDP_VID_SetLayerAddr               (VDP_LAYER_VID0, 0, stHdrCfg->stBlAddr.u32LumAddr, stHdrCfg->stBlAddr.u32ChmAddr, stHdrCfg->stBlAddr.u32LumStr, stHdrCfg->stBlAddr.u32ChmStr);
    VDP_VID_SetLayerAddr               (VDP_LAYER_VID0, 1, stHdrCfg->stBlAddr_2LowBit.u32LumAddr, stHdrCfg->stBlAddr_2LowBit.u32ChmAddr, stHdrCfg->stBlAddr.u32LumStr, stHdrCfg->stBlAddr.u32ChmStr);
    VDP_VID_SetTileStride(VDP_LAYER_VID0, stHdrCfg->stBlAddr_2LowBit.u32LumStr, stHdrCfg->stBlAddr_2LowBit.u32ChmStr);
    VDP_VID_SetTileDcmpEnable          (VDP_LAYER_VID0, stHdrCfg->bDcmpEn);
    if(stHdrCfg->bDcmpEn == HI_TRUE)
    {
        VDP_VID_SetHeadAddr        (VDP_LAYER_VID0, stHdrCfg->stHeadAddr.u32LumAddr, stHdrCfg->stHeadAddr.u32ChmAddr);
        VDP_VID_SetHeadStride      (VDP_LAYER_VID0, stHdrCfg->stHeadAddr.u32LumStr);
        VDP_VID_SetHeadSize        (VDP_LAYER_VID0, stHdrCfg->stHeadAddr.u32DcmpHeadSize);
    }


#if 1
		if(stHdrCfg->stVidReso.u32Wth == stHdrCfg->stBlReso.u32Wth)
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}
		else if((stHdrCfg->stBlReso.u32Wth >= 3*stHdrCfg->stVidReso.u32Wth/4)&(stHdrCfg->stBlReso.u32Wth < 1*stHdrCfg->stVidReso.u32Wth/1))
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
		}
		else if((stHdrCfg->stBlReso.u32Wth >= 1*stHdrCfg->stVidReso.u32Wth/2)&(stHdrCfg->stBlReso.u32Wth < 3*stHdrCfg->stVidReso.u32Wth/4))
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
		}
		else if((stHdrCfg->stBlReso.u32Wth >= 1*stHdrCfg->stVidReso.u32Wth/3)&(stHdrCfg->stBlReso.u32Wth < 1*stHdrCfg->stVidReso.u32Wth/2))
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
		}
		else if((stHdrCfg->stBlReso.u32Wth >= 1*stHdrCfg->stVidReso.u32Wth/4)&(stHdrCfg->stBlReso.u32Wth < 1*stHdrCfg->stVidReso.u32Wth/3))
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
		}
		else if((stHdrCfg->stBlReso.u32Wth>= 1*stHdrCfg->stVidReso.u32Wth/5)&(stHdrCfg->stBlReso.u32Wth < 1*stHdrCfg->stVidReso.u32Wth/4))
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
		}
		else
		{
			enHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
		}

		DRV_PQ_GetVdpZmeCoef(enHiPqZmeCoefRatio,HI_PQ_ZME_COEF_TYPE_LH,HI_PQ_ZME_TAP_8T32P, (HI_S16 *)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
		DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_8LH,HI_PQ_ZME_COEF_TYPE_LH,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZMEH]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));

		
		DRV_PQ_GetVdpZmeCoef(enHiPqZmeCoefRatio,HI_PQ_ZME_COEF_TYPE_CH,HI_PQ_ZME_TAP_4T32P, (HI_S16 *)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
		DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CH,HI_PQ_ZME_COEF_TYPE_CH,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZMEH]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	

		
		DRV_PQ_GetVdpZmeCoef(enHiPqZmeCoefRatio,HI_PQ_ZME_COEF_TYPE_LV,HI_PQ_ZME_TAP_6T32P, (HI_S16 *)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
		DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_6LV,HI_PQ_ZME_COEF_TYPE_LV,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZMEV]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	
		DRV_PQ_GetVdpZmeCoef(enHiPqZmeCoefRatio,HI_PQ_ZME_COEF_TYPE_CV,HI_PQ_ZME_TAP_4T32P, (HI_S16 *)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
		DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CV,HI_PQ_ZME_COEF_TYPE_CV,(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZMEV]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));	
	
		VDP_VID_SetZmeCoefAddr(VDP_LAYER_VID0, VDP_VID_PARA_ZME_HOR , (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_ZMEH]));
		VDP_VID_SetZmeCoefAddr(VDP_LAYER_VID0, VDP_VID_PARA_ZME_VER , (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_ZMEV]));
		VDP_VID_SetParaUpd(VDP_LAYER_VID0,VDP_VID_PARA_ZME_HOR);
		VDP_VID_SetParaUpd(VDP_LAYER_VID0,VDP_VID_PARA_ZME_VER);
#endif

		//----------mmu setting-------------//
		VDP_VID_Set16RegionSmmuLumBypass (VDP_LAYER_VID0, 0, !stHdrCfg->bSmmuEn);
		VDP_VID_Set16RegionSmmuChromBypass (VDP_LAYER_VID0, 0, !stHdrCfg->bSmmuEn);
		VDP_VID_Set16RegionSmmuLumBypass (VDP_LAYER_VID1, 0, !stHdrCfg->bSmmuEn);
		VDP_VID_Set16RegionSmmuChromBypass (VDP_LAYER_VID1, 0, !stHdrCfg->bSmmuEn);
		//----------------end---------------//

		VDP_VID_SetNoSecFlag(VDP_LAYER_VID0, !stHdrCfg->bSecureEn);
		VDP_VID_SetNoSecFlag(VDP_LAYER_VID1, !stHdrCfg->bSecureEn);

	if((stHdrCfg->stVidReso.u32Wth != stOutReso.u32Wth) || (stHdrCfg->stVidReso.u32Hgt != stOutReso.u32Hgt))
	{

        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_HORL, HI_TRUE);
        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_VERL, HI_TRUE);
        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_HORC, HI_TRUE);
        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_VERC, HI_TRUE);
		VDP_VID_SetZmePhase            (VDP_LAYER_VID0, VDP_ZME_MODE_HOR, 0);
		VDP_VID_SetZmePhase            (VDP_LAYER_VID0, VDP_ZME_MODE_VER, 0);
		VDP_VID_SetZmeFirEnable        (VDP_LAYER_VID0, VDP_ZME_MODE_HOR, HI_TRUE);
		VDP_VID_SetZmeFirEnable        (VDP_LAYER_VID0, VDP_ZME_MODE_VER, HI_TRUE);

        
        VDP_VID_SetZmeHorRatio         (VDP_LAYER_VID0, (stHdrCfg->stVidReso.u32Wth*ZME_HPREC)/stOutReso.u32Wth);
        VDP_VID_SetZmeVerRatio         (VDP_LAYER_VID0, (stHdrCfg->stVidReso.u32Hgt*ZME_VPREC)/stOutReso.u32Hgt);

		VDP_VID_SetZmeInFmt            (VDP_LAYER_VID0, VDP_PROC_FMT_SP_420);
		VDP_VID_SetZmeOutFmt           (VDP_LAYER_VID0, VDP_PROC_FMT_SP_420);
	}
    else
    {
        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_HOR, HI_FALSE);
        VDP_VID_SetZmeEnable           (VDP_LAYER_VID0, VDP_ZME_MODE_VER, HI_FALSE);
    }
	//----------------------------------------------------------------------
	//EL setting
	//----------------------------------------------------------------------
	VDP_VID_SetLayerEnable         (VDP_LAYER_VID1, el_en);
	if(el_en == 1)
	{
        
        VDP_VID_SetInDataFmt               (VDP_LAYER_VID1, stHdrCfg->enVdpElVidIFmt);
		VDP_VID_SetReadMode                (VDP_LAYER_VID1, VDP_RMODE_INTERFACE, VDP_RMODE_INTERFACE);
		VDP_VID_SetDataWidth               (VDP_LAYER_VID1, stHdrCfg->u32DataWidth);

		VDP_VID_SetInReso                  (VDP_LAYER_VID1, stHdrCfg->stElReso);
		VDP_VID_SetOutReso                 (VDP_LAYER_VID1, stOutReso);
		VDP_VID_SetVideoPos                (VDP_LAYER_VID1, stVidReso);
		VDP_VID_SetDispPos                 (VDP_LAYER_VID1, stDispReso);
        VDP_VID_SetLayerAddr               (VDP_LAYER_VID1, 0, stHdrCfg->stElAddr.u32LumAddr, stHdrCfg->stElAddr.u32ChmAddr, stHdrCfg->stElAddr.u32LumStr, stHdrCfg->stElAddr.u32ChmStr);

		if((stHdrCfg->stElReso.u32Wth != stOutReso.u32Wth) || (stHdrCfg->stElReso.u32Hgt != stOutReso.u32Hgt))
		{
			VDP_VID_SetZmeEnable           (VDP_LAYER_VID1, VDP_ZME_MODE_HOR, HI_TRUE);
			
			if(stHdrCfg->bFlag480I576I == HI_TRUE)
			{
				VDP_VID_SetZmeEnable           (VDP_LAYER_VID1, VDP_ZME_MODE_VER, HI_FALSE);
			}
			else
			{
				VDP_VID_SetZmeEnable           (VDP_LAYER_VID1, VDP_ZME_MODE_VER, HI_TRUE);
			}

			VDP_VID_SetZmePhase            (VDP_LAYER_VID1, VDP_ZME_MODE_HOR, 0);
			VDP_VID_SetZmePhase            (VDP_LAYER_VID1, VDP_ZME_MODE_VER, 0);
			VDP_VID_SetZmeFirEnable        (VDP_LAYER_VID1, VDP_ZME_MODE_HOR, HI_TRUE);
			VDP_VID_SetZmeFirEnable        (VDP_LAYER_VID1, VDP_ZME_MODE_VER, HI_TRUE);

			VDP_VID_SetZmeInFmt            (VDP_LAYER_VID1, VDP_PROC_FMT_SP_420);
			VDP_VID_SetZmeOutFmt           (VDP_LAYER_VID1, VDP_PROC_FMT_SP_420);
			VDP_VID_SetHdrElZme            (VDP_LAYER_VID1, HI_TRUE);
		}
		else
		{
			VDP_VID_SetZmeEnable           (VDP_LAYER_VID1, VDP_ZME_MODE_HOR, HI_FALSE);
			VDP_VID_SetZmeEnable           (VDP_LAYER_VID1, VDP_ZME_MODE_VER, HI_FALSE);

			VDP_VID_SetHdrElZme            (VDP_LAYER_VID1, HI_FALSE);
		}
	}
	
	//----------------------------------------------------------------------
	//VP setting
	//----------------------------------------------------------------------
	VDP_VP_SetInReso               (VDP_LAYER_VP0, stOutReso);
	VDP_VP_SetVideoPos             (VDP_LAYER_VP0, stVp0DispReso);
	VDP_VP_SetDispPos              (VDP_LAYER_VP0, stVp0VidReso);
    VDP_VP_SetMuteEnable           (VDP_LAYER_VP0, stHdrCfg->bMuteEn);

    stVpMuteBkg.u32VpMuteBkgY=0x40;
    stVpMuteBkg.u32VpMuteBkgU=0x200;
    stVpMuteBkg.u32VpMuteBkgV=0x200;
    VDP_VP_SetMuteBkg              (VDP_LAYER_VP0,stVpMuteBkg);

	if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT_CERT)
	{
        csc_offset[0] =0;
        csc_offset[1] =0;
        csc_offset[2] =0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );
		
		csc_offset[0] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[0] * 16;
		csc_offset[1] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[1] * 16;
		csc_offset[2] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[2] * 16;

		VDP_HDR_SetVDmInBits                 ( 1 );//14bit
		VDP_VID_SetComposeElUpsampleMax      ( 16383 );
		VDP_VID_SetComposeElUpsampleMin      ( 0 );
		VDP_HDR_SetVDmImapYuv2rgbOutDc0      ( csc_offset );
		VDP_HDR_SetVDmImapRange              ( pstDmKs->ksIMap.eotfParam.range * 16 );

	}
	if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_HDR10_OUT)
	{
        csc_offset[0] =0;
        csc_offset[1] =0;
        csc_offset[2] =0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );
        
		csc_offset[0] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[0] * 16;
		csc_offset[1] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[1] * 16;
		csc_offset[2] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[2] * 16;

		VDP_HDR_SetVDmInBits                 ( 1 );//14bit
		VDP_VID_SetComposeElUpsampleMax      ( 16383 );
		VDP_VID_SetComposeElUpsampleMin      ( 0 );
		VDP_HDR_SetVDmImapYuv2rgbOutDc0      ( csc_offset );
		VDP_HDR_SetVDmImapRange              ( pstDmKs->ksIMap.eotfParam.range * 16 );

		//----------------------------------------------------------------------
		//for FPGA test
		//----------------------------------------------------------------------
		csc_offset[0] = pstDmKs->ksOMap.v3Rgb2YuvOff[0] * 4;
		csc_offset[1] = pstDmKs->ksOMap.v3Rgb2YuvOff[1] * 4;
		csc_offset[2] = pstDmKs->ksOMap.v3Rgb2YuvOff[2] * 4;

		VDP_HDR_SetVCvmOmapRgb2yuvOutDc      ( csc_offset );
		VDP_HDR_SetCvmOutBits                ( 2 );
	}
	if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_HDR10_OUT)
	{
        
        csc_offset[0] = 0;
        csc_offset[1] = 0;
        csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );
		//----------------------------------------------------------------------
		//for FPGA test
		//----------------------------------------------------------------------
		csc_offset[0] = pstDmKs->ksOMap.v3Rgb2YuvOff[0] * 4;
		csc_offset[1] = pstDmKs->ksOMap.v3Rgb2YuvOff[1] * 4;
		csc_offset[2] = pstDmKs->ksOMap.v3Rgb2YuvOff[2] * 4;

		VDP_HDR_SetVCvmOmapRgb2yuvOutDc      ( csc_offset );
		VDP_HDR_SetCvmOutBits                ( 2 );
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_HDR10_OUT)
	{

		//----------------------------------------------------------------------
		//add offset and clip for I component
		//12bit valid unsigned data in [15:4]
		//----------------------------------------------------------------------
		coef_shift = 9;
		csc_m3[0][0] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = (1<<8) ;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = csc_m3[1][1] ;

		VDP_HDR_SetVDmImap0Yuv2rgb           ( csc_m3 );

		csc_offset[0] = -pstDmKs->ksIMap.v3IptOff[0];
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );

		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
		VDP_HDR_SetVDmImapYuv2rgbOutDc0      ( csc_offset );
		VDP_HDR_SetVDmImapYuv2rgbscale2p     ( 11 ) ;//shift = scale2p - 4

		//----------------------------------------------------------------------
		//right shift 1bit in ipt_in: [15:4] -> [14:3]
		//----------------------------------------------------------------------
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;

		VDP_HDR_SetVDmImapIptoff             ( csc_offset ) ;
		VDP_HDR_SetVDmImapIptScale           ( 1<<16      ) ;//17-16=1

		//----------------------------------------------------------------------
		// bypass I component
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1<<8;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = pstDmKs->ksIMap.iptScale >> (coef_shift);

		csc_offset[0] = 0;
		csc_offset[1] = (-pstDmKs->ksIMap.v3IptOff[1]*csc_m3[1][1]) >> 8;
		csc_offset[2] = (-pstDmKs->ksIMap.v3IptOff[2]*csc_m3[2][2]) >> 8;

		VDP_HDR_SetVCvmImapLms2iptOutDc      ( csc_offset ) ;
		VDP_HDR_SetVCvmImapLms2ipt           ( csc_m3     ) ;
		VDP_HDR_SetVCvmImapLms2iptscale2p    ( 8          ) ;
		VDP_HDR_SetVDmImapLms2iptmin         ( -((1<<15 ) ) ) ;
		VDP_HDR_SetVDmImapLms2iptmax         ( ((1<<15 ) -1 ) ) ;
		//----------------------------------------------------------------------
		//444 10bit output
		//----------------------------------------------------------------------
		csc_offset[0] = pstDmKs->ksOMap.v3Rgb2YuvOff[0] * 4;
		csc_offset[1] = pstDmKs->ksOMap.v3Rgb2YuvOff[1] * 4;
		csc_offset[2] = pstDmKs->ksOMap.v3Rgb2YuvOff[2] * 4;

		VDP_HDR_SetVCvmOmapRgb2yuvOutDc      ( csc_offset );
		VDP_HDR_SetCvmOutBits                ( 2 );

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT || stHdrCfg->enHdrMode == VDP_HDR_HDR10_IN_SDR_OUT_CERT)
	{
		csc_offset[0] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[0] * 16;
		csc_offset[1] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[1] * 16;
		csc_offset[2] = pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[2] * 16;

		VDP_HDR_SetVDmInBits                 ( 1 );//14bit
		VDP_VID_SetComposeElUpsampleMax      ( 16383 );
		VDP_VID_SetComposeElUpsampleMin      ( 0 );
		VDP_HDR_SetVDmImapYuv2rgbOutDc0      ( csc_offset );
		VDP_HDR_SetVDmImapRange              ( pstDmKs->ksIMap.eotfParam.range * 16 );

        csc_offset[0] =0;
        csc_offset[1] =0;
        csc_offset[2] =0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );


	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_DOVI_IPT_OUT)
	{
		VDP_HDR_SetCvmOutColor              (1  );//0:ipt; 1:yuv; force yuv input,bypass ipt_out
		//----------------------------------------------------------------------
		//add offset and clip for I component
		//12bit valid unsigned data in [15:4]
		//----------------------------------------------------------------------
		coef_shift = 9;
		csc_m3[0][0] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = (1<<8) ;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = csc_m3[1][1] ;

		VDP_HDR_SetVDmImap0Yuv2rgb          ( csc_m3 );

		csc_offset[0] = -pstDmKs->ksIMap.v3IptOff[0];
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );

		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
		VDP_HDR_SetVDmImapYuv2rgbOutDc0     ( csc_offset );
		VDP_HDR_SetVDmImapYuv2rgbscale2p    ( 11 ) ;//shift = scale2p - 4

		//----------------------------------------------------------------------
		//right shift 1bit in ipt_in: [15:4] -> [14:3]
		//----------------------------------------------------------------------
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;

		VDP_HDR_SetVDmImapIptoff            ( csc_offset );
		VDP_HDR_SetVDmImapIptScale          ( 1<<16      );//17-16=1

		//----------------------------------------------------------------------
		// bypass I component
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1<<8;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = pstDmKs->ksIMap.iptScale >> (coef_shift);

		csc_offset[0] = 0;
		csc_offset[1] = (-pstDmKs->ksIMap.v3IptOff[1]*csc_m3[1][1]) >> 8;
		csc_offset[2] = (-pstDmKs->ksIMap.v3IptOff[2]*csc_m3[2][2]) >> 8;

		VDP_HDR_SetGCvmLms2Ipt            ( csc_m3        ) ;
		VDP_HDR_SetGCvmLms2IptScale2p     ( 8    ) ;
		VDP_HDR_SetGCvmLms2IptOutDc       ( csc_offset   ) ;
		VDP_HDR_SetGCvmLms2IptMin         (-((1<<15)));
		VDP_HDR_SetGCvmLms2IptMax         (((1<<15)-1));

		if(pstDmKs->ksOMap.clr == 3)//IPT out
		{
			//----------------------------------------------------------------------
			// 3.RGB2YUV
			//----------------------------------------------------------------------
			csc_m3[0][0] = pstDmKs->ksOMap.iptScale;
			csc_m3[0][1] = 0;
			csc_m3[0][2] = 0;
			csc_m3[1][0] = 0;
			csc_m3[1][1] = pstDmKs->ksOMap.iptScale*2;
			csc_m3[1][2] = 0;
			csc_m3[2][0] = 0;
			csc_m3[2][1] = 0;
			csc_m3[2][2] = pstDmKs->ksOMap.iptScale*2;

			csc_offset[0] = pstDmKs->ksOMap.v3IptOff[0];
			csc_offset[1] = pstDmKs->ksOMap.v3IptOff[1]-pstDmKs->ksOMap.iptScale;
			csc_offset[2] = pstDmKs->ksOMap.v3IptOff[2]-pstDmKs->ksOMap.iptScale;

			VDP_HDR_SetVCvmOmapRgb2yuv          ( csc_m3            ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvscale2p   ( 16     ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvOutDc     ( csc_offset          ) ;

		}

		//----------------------------------------------------------------------
		// 4.bypass the CSC
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;

		VDP_HDR_SetVDmImapRgb2lms           ( csc_m3 );
		VDP_HDR_SetVDmImapRgb2lmsscale2p    ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmin        ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))

		VDP_HDR_SetVCvmOmapIpt2lms          ( csc_m3 );
		VDP_HDR_SetVCvmOmapIpt2lmsscale2p   ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmin        ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmax        ( (1<<16)-1 );

		VDP_HDR_SetVCvmOmapLms2rgb          ( csc_m3 );
		VDP_HDR_SetVCvmOmapLms2rgbscale2p   ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmin        ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmax        ( 0xffffffff );

	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT_CERT || stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_IPT_OUT)
	{
		VDP_HDR_SetCvmOutColor              (1  );//0:ipt; 1:yuv; force yuv input,bypass ipt_out
		//----------------------------------------------------------------------
		//add offset and clip for I component
		//12bit valid unsigned data in [15:4]
		//----------------------------------------------------------------------
		coef_shift = 9;
		csc_m3[0][0] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = (1<<8) ;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = csc_m3[1][1] ;

		VDP_HDR_SetVDmImap0Yuv2rgb          ( csc_m3 );

		csc_offset[0] = -pstDmKs->ksIMap.v3IptOff[0];
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );

		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
		VDP_HDR_SetVDmImapYuv2rgbOutDc0     ( csc_offset );
		VDP_HDR_SetVDmImapYuv2rgbscale2p    ( 11 ) ;//shift = scale2p - 4

		//----------------------------------------------------------------------
		//right shift 1bit in ipt_in: [15:4] -> [14:3]
		//----------------------------------------------------------------------
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;

		VDP_HDR_SetVDmImapIptoff            ( csc_offset );
		VDP_HDR_SetVDmImapIptScale          ( 1<<16      );//17-16=1

		//----------------------------------------------------------------------
		// bypass I component
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1<<8;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = pstDmKs->ksIMap.iptScale >> (coef_shift);

		csc_offset[0] = 0;
		csc_offset[1] = (-pstDmKs->ksIMap.v3IptOff[1]*csc_m3[1][1]) >> 8;
		csc_offset[2] = (-pstDmKs->ksIMap.v3IptOff[2]*csc_m3[2][2]) >> 8;

		VDP_HDR_SetVCvmImapLms2iptOutDc      ( csc_offset);
		VDP_HDR_SetVCvmImapLms2ipt           ( csc_m3    );
		VDP_HDR_SetVCvmImapLms2iptscale2p    ( 8 ) ;
        VDP_HDR_SetVDmImapLms2iptmin         ( -((1<<15)));
        VDP_HDR_SetVDmImapLms2iptmax         ( ((1<<15)-1));

		if(pstDmKs->ksOMap.clr == 3)//IPT out
		{
			//----------------------------------------------------------------------
			// 3.RGB2YUV
			//----------------------------------------------------------------------
			csc_m3[0][0] = pstDmKs->ksOMap.iptScale;
			csc_m3[0][1] = 0;
			csc_m3[0][2] = 0;
			csc_m3[1][0] = 0;
			csc_m3[1][1] = pstDmKs->ksOMap.iptScale*2;
			csc_m3[1][2] = 0;
			csc_m3[2][0] = 0;
			csc_m3[2][1] = 0;
			csc_m3[2][2] = pstDmKs->ksOMap.iptScale*2;

			csc_offset[0] = pstDmKs->ksOMap.v3IptOff[0];
			csc_offset[1] = pstDmKs->ksOMap.v3IptOff[1]-pstDmKs->ksOMap.iptScale;
			csc_offset[2] = pstDmKs->ksOMap.v3IptOff[2]-pstDmKs->ksOMap.iptScale;



			VDP_HDR_SetVCvmOmapRgb2yuv          ( csc_m3            ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvscale2p   ( 16     ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvOutDc     ( csc_offset          ) ;

		}

		//----------------------------------------------------------------------
		// 4.bypass the CSC
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;

		VDP_HDR_SetVDmImapRgb2lms           ( csc_m3 );
		VDP_HDR_SetVDmImapRgb2lmsscale2p    ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmin        ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))


		VDP_HDR_SetVCvmOmapLms2rgb          ( csc_m3 );
		VDP_HDR_SetVCvmOmapLms2rgbscale2p   ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmin        ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmax        ( 0xffffffff );
	}
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_HDR10_OUT_CERT)
	{

		//----------------------------------------------------------------------
		//add offset and clip for I component
		//12bit valid unsigned data in [15:4]
		//----------------------------------------------------------------------
		coef_shift = 9;
		csc_m3[0][0] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = (1<<8) ;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = csc_m3[1][1] ;

		VDP_HDR_SetVDmImap0Yuv2rgb           ( csc_m3 );

		csc_offset[0] = -pstDmKs->ksIMap.v3IptOff[0];
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );

		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
		VDP_HDR_SetVDmImapYuv2rgbOutDc0      ( csc_offset );
		VDP_HDR_SetVDmImapYuv2rgbscale2p     ( 11 ) ;//shift = scale2p - 4

		//----------------------------------------------------------------------
		//right shift 1bit in ipt_in: [15:4] -> [14:3]
		//----------------------------------------------------------------------
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;

		VDP_HDR_SetVDmImapIptoff             ( csc_offset ) ;
		VDP_HDR_SetVDmImapIptScale           ( 1<<16      ) ;//17-16=1

		//----------------------------------------------------------------------
		// bypass I component
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1<<8;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = pstDmKs->ksIMap.iptScale >> (coef_shift);

		csc_offset[0] = 0;
		csc_offset[1] = (-pstDmKs->ksIMap.v3IptOff[1]*csc_m3[1][1]) >> 8;
		csc_offset[2] = (-pstDmKs->ksIMap.v3IptOff[2]*csc_m3[2][2]) >> 8;

		VDP_HDR_SetVCvmImapLms2iptOutDc      ( csc_offset ) ;
		VDP_HDR_SetVCvmImapLms2ipt           ( csc_m3     ) ;
		VDP_HDR_SetVCvmImapLms2iptscale2p    ( 8          ) ;
		VDP_HDR_SetVDmImapLms2iptmin         ( -((1<<15 ) ) ) ;
		VDP_HDR_SetVDmImapLms2iptmax         ( ((1<<15 ) -1 ) ) ;

	}
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_OUT)
    {
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );
        VDP_HDR_SetVCvmImapLms2iptOutDc      ( csc_offset );
    }
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_NBC_IN_SDR_OUT)
    {
		VDP_HDR_SetCvmOutColor              (1  );//0:ipt; 1:yuv; force yuv input,bypass ipt_out
		//----------------------------------------------------------------------
		//add offset and clip for I component
		//12bit valid unsigned data in [15:4]
		//----------------------------------------------------------------------
		coef_shift = 9;
		csc_m3[0][0] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = (1<<8) ;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = csc_m3[1][1] ;

		VDP_HDR_SetVDmImap0Yuv2rgb          ( csc_m3 );

		csc_offset[0] = -pstDmKs->ksIMap.v3IptOff[0];
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0      ( csc_offset );

		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
		VDP_HDR_SetVDmImapYuv2rgbOutDc0     ( csc_offset );
		VDP_HDR_SetVDmImapYuv2rgbscale2p    ( 11 ) ;//shift = scale2p - 4

		//----------------------------------------------------------------------
		//right shift 1bit in ipt_in: [15:4] -> [14:3]
		//----------------------------------------------------------------------
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;

		VDP_HDR_SetVDmImapIptoff            ( csc_offset );
		VDP_HDR_SetVDmImapIptScale          ( 1<<16      );//17-16=1

		//----------------------------------------------------------------------
		// bypass I component
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1<<8;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = pstDmKs->ksIMap.iptScale >> (coef_shift);
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = pstDmKs->ksIMap.iptScale >> (coef_shift);

		csc_offset[0] = 0;
		csc_offset[1] = (-pstDmKs->ksIMap.v3IptOff[1]*csc_m3[1][1]) >> 8;
		csc_offset[2] = (-pstDmKs->ksIMap.v3IptOff[2]*csc_m3[2][2]) >> 8;

		VDP_HDR_SetVCvmImapLms2iptOutDc      ( csc_offset);
		VDP_HDR_SetVCvmImapLms2ipt           ( csc_m3    );
		VDP_HDR_SetVCvmImapLms2iptscale2p    ( 8 ) ;
        VDP_HDR_SetVDmImapLms2iptmin         ( -((1<<15)));
        VDP_HDR_SetVDmImapLms2iptmax         ( ((1<<15)-1));
			//----------------------------------------------------------------------
		// 4.bypass the CSC
		//----------------------------------------------------------------------
#if 0
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;

		VDP_HDR_SetVDmImapRgb2lms           ( csc_m3 );
		VDP_HDR_SetVDmImapRgb2lmsscale2p    ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmin        ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))
#endif
	csc_m3[0][0] = 1;
	csc_m3[0][1] = 0;
	csc_m3[0][2] = 0;
	csc_m3[1][0] = 0;
	csc_m3[1][1] = 1;
	csc_m3[1][2] = 0;
	csc_m3[2][0] = 0;
	csc_m3[2][1] = 0;
	csc_m3[2][2] = 1;
	
	//VDP_HDR_SetVDmImapRgb2lms			( csc_m3 );
	//VDP_HDR_SetVDmImapRgb2lmsscale2p	( 0 );
	//VDP_HDR_SetVDmImapRgb2lmsmin		( 0 );
	//VDP_HDR_SetVDmImapRgb2lmsmax		( 0x9c400000  );//(10000LL*(1<<18))
	
	//VDP_HDR_SetVCvmOmapIpt2lms			( csc_m3 );
	//VDP_HDR_SetVCvmOmapIpt2lmsscale2p	( 0 );
	//VDP_HDR_SetVDmOmapIpt2lmsmin		( 0 );
	//VDP_HDR_SetVDmOmapIpt2lmsmax		( (1<<16)-1 );
	
	//VDP_HDR_SetVCvmOmapLms2rgb			( csc_m3 );
	//VDP_HDR_SetVCvmOmapLms2rgbscale2p	( 0 );
	//VDP_HDR_SetVDmOmapLms2rgbmin		( 0 );
	//VDP_HDR_SetVDmOmapLms2rgbmax		( 0xffffffff );


		
    }
	else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_OUT_CERT )
	{
		VDP_HDR_SetCvmOutColor              (1  );//0:ipt; 1:yuv; force yuv input,bypass ipt_out
		if(pstDmKs->ksOMap.clr == 3)//IPT out
		{
			csc_m3[0][0] = pstDmKs->ksOMap.iptScale;
			csc_m3[0][1] = 0;
			csc_m3[0][2] = 0;
			csc_m3[1][0] = 0;
			csc_m3[1][1] = pstDmKs->ksOMap.iptScale*2;
			csc_m3[1][2] = 0;
			csc_m3[2][0] = 0;
			csc_m3[2][1] = 0;
			csc_m3[2][2] = pstDmKs->ksOMap.iptScale*2;
			csc_offset[0] = pstDmKs->ksOMap.v3IptOff[0];
			csc_offset[1] = pstDmKs->ksOMap.v3IptOff[1]-pstDmKs->ksOMap.iptScale;
			csc_offset[2] = pstDmKs->ksOMap.v3IptOff[2]-pstDmKs->ksOMap.iptScale;
			VDP_HDR_SetVCvmOmapRgb2yuv          ( csc_m3            ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvscale2p   ( 16     ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvOutDc     ( csc_offset          ) ;
		}
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;
		VDP_HDR_SetVCvmOmapIpt2lms			( csc_m3 );
		VDP_HDR_SetVCvmOmapIpt2lmsscale2p ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmin		( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmax		( (1<<16)-1 );
		VDP_HDR_SetVCvmOmapLms2rgb			( csc_m3 );
		VDP_HDR_SetVCvmOmapLms2rgbscale2p ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmin		( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmax		( 0xffffffff );
		VDP_HDR_SetGCvmLms2IptMin	   (0x8001);//	 (-((1<<15)));
		VDP_HDR_SetGCvmLms2IptMax		  (((1<<15)-1));
	}
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_SDR_IPT_OUT)
    {
		csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );

		if(pstDmKs->ksOMap.clr == 3)//IPT out
		{
			//----------------------------------------------------------------------
			// 3.RGB2YUV
			//----------------------------------------------------------------------
			csc_m3[0][0] = pstDmKs->ksOMap.iptScale;
			csc_m3[0][1] = 0;
			csc_m3[0][2] = 0;
			csc_m3[1][0] = 0;
			csc_m3[1][1] = pstDmKs->ksOMap.iptScale*2;
			csc_m3[1][2] = 0;
			csc_m3[2][0] = 0;
			csc_m3[2][1] = 0;
			csc_m3[2][2] = pstDmKs->ksOMap.iptScale*2;

			csc_offset[0] = pstDmKs->ksOMap.v3IptOff[0];
			csc_offset[1] = pstDmKs->ksOMap.v3IptOff[1]-pstDmKs->ksOMap.iptScale;
			csc_offset[2] = pstDmKs->ksOMap.v3IptOff[2]-pstDmKs->ksOMap.iptScale;



			VDP_HDR_SetVCvmOmapRgb2yuv          ( csc_m3            ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvscale2p   ( 16     ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvOutDc     ( csc_offset          ) ;

		}

		//----------------------------------------------------------------------
		// 4.bypass the CSC
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;

		VDP_HDR_SetVDmImapRgb2lms           ( csc_m3 );
		VDP_HDR_SetVDmImapRgb2lmsscale2p    ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmin        ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))

		VDP_HDR_SetVCvmOmapIpt2lms          ( csc_m3 );
		VDP_HDR_SetVCvmOmapIpt2lmsscale2p   ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmin        ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmax        ( (1<<16)-1 );

		VDP_HDR_SetVCvmOmapLms2rgb          ( csc_m3 );
		VDP_HDR_SetVCvmOmapLms2rgbscale2p   ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmin        ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmax        ( 0xffffffff );
    }
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_HDR10_OUT_CERT)
    {
        csc_offset[0] = 0;
        csc_offset[1] = 0;
        csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );
    }
    
#if  1	
    else if(stHdrCfg->enHdrMode == VDP_HDR_DOVI_BC_IN_DOVI_IPT_OUT)
    {
        csc_offset[0] = 0;
		csc_offset[1] = 0;
		csc_offset[2] = 0;
        VDP_HDR_SetVDmImapYuv2rgbInDc0       ( csc_offset );

		VDP_HDR_SetCvmOutColor              (1);//0:ipt; 1:yuv; force yuv input,bypass ipt_out	
#if  1

		if(pstDmKs->ksOMap.clr == 3)//IPT out
		{
			//----------------------------------------------------------------------
			// 3.RGB2YUV
			//----------------------------------------------------------------------
			csc_m3[0][0] = pstDmKs->ksOMap.iptScale;
			csc_m3[0][1] = 0;
			csc_m3[0][2] = 0;
			csc_m3[1][0] = 0;
			csc_m3[1][1] = pstDmKs->ksOMap.iptScale*2;
			csc_m3[1][2] = 0;
			csc_m3[2][0] = 0;
			csc_m3[2][1] = 0;
			csc_m3[2][2] = pstDmKs->ksOMap.iptScale*2;

			csc_offset[0] = pstDmKs->ksOMap.v3IptOff[0];
			csc_offset[1] = pstDmKs->ksOMap.v3IptOff[1]-pstDmKs->ksOMap.iptScale;
			csc_offset[2] = pstDmKs->ksOMap.v3IptOff[2]-pstDmKs->ksOMap.iptScale;


			VDP_HDR_SetVCvmOmapRgb2yuv          ( csc_m3            ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvscale2p   ( 16     ) ;
			VDP_HDR_SetVCvmOmapRgb2yuvOutDc     ( csc_offset          ) ;

		}
#endif


		//----------------------------------------------------------------------
		// 4.bypass the CSC
		//----------------------------------------------------------------------
		csc_m3[0][0] = 1;
		csc_m3[0][1] = 0;
		csc_m3[0][2] = 0;
		csc_m3[1][0] = 0;
		csc_m3[1][1] = 1;
		csc_m3[1][2] = 0;
		csc_m3[2][0] = 0;
		csc_m3[2][1] = 0;
		csc_m3[2][2] = 1;

#if  0
		VDP_HDR_SetVDmImapRgb2lms           ( csc_m3 );
		VDP_HDR_SetVDmImapRgb2lmsscale2p    ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmin        ( 0 );
		VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))
#endif
		VDP_HDR_SetVCvmOmapIpt2lms          ( csc_m3 );
		VDP_HDR_SetVCvmOmapIpt2lmsscale2p   ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmin        ( 0 );
		VDP_HDR_SetVDmOmapIpt2lmsmax        ( (1<<16)-1 );

		VDP_HDR_SetVCvmOmapLms2rgb          ( csc_m3 );
		VDP_HDR_SetVCvmOmapLms2rgbscale2p   ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmin        ( 0 );
		VDP_HDR_SetVDmOmapLms2rgbmax        ( 0xffffffff );
		        
        VDP_HDR_SetGCvmLms2IptMin         (-((1<<15)));
        VDP_HDR_SetGCvmLms2IptMax         (((1<<15)-1));

       // VDP_HDR_SetCvmEn                  ( 0);
		//VDP_HDR_SetVDmEn			      (0 );

		
    }
#endif

	//----------------------------------------------------------------------
	//do not support the function in HDR mode
	//----------------------------------------------------------------------
    VDP_VID_SetFlipEnable              (VDP_LAYER_VID0, HI_FALSE);
    VDP_VID_SetUvorder                 (VDP_LAYER_VID0, HI_FALSE);
    VDP_VID_SetDrawMode                (VDP_LAYER_VID0, VDP_DRAW_MODE_1, VDP_DRAW_MODE_1);
    VDP_VID_SetVtapReduce              (VDP_LAYER_VID0, HI_FALSE);

    VDP_VID_SetMultiModeEnable         (VDP_LAYER_VID1, HI_FALSE);
    VDP_VID_SetFlipEnable              (VDP_LAYER_VID1, HI_FALSE);
    VDP_VID_SetUvorder                 (VDP_LAYER_VID1, HI_FALSE);
    VDP_VID_SetDrawMode                (VDP_LAYER_VID1, VDP_DRAW_MODE_1, VDP_DRAW_MODE_1);

    VDP_DISP_SetDispMode               (VDP_CHN_DHD0, VDP_DISP_MODE_2D);
    VDP_DISP_SetDispMode               (VDP_CHN_DHD0, VDP_DISP_MODE_2D);
	//----------------------------------------------------------------------
	//disable all PQ
	//----------------------------------------------------------------------
    VDP_VID_SetSharpEnable             (VDP_LAYER_VID0, HI_FALSE);
    VDP_VID_SetZme2Enable              (VDP_LAYER_VID0, VDP_ZME_MODE_HOR, HI_FALSE);
    VDP_VID_SetZme2Enable              (VDP_LAYER_VID0, VDP_ZME_MODE_VER, HI_FALSE);

    VDP_VP_SetAcmEnable                (VDP_LAYER_VP0, HI_FALSE);
    VDP_VP_SetDciEnable                (VDP_LAYER_VP0, HI_FALSE);

	VDP_VID_SetRegUp(VDP_LAYER_VID0);
	VDP_VID_SetRegUp(VDP_LAYER_VID1);
    
	VDP_GP_SetRegUp(VDP_LAYER_GFX0);
	VDP_VP_SetRegUp(VDP_LAYER_VP0);
    VDP_HDR_SetRegUp();
    VDP_DISP_SetRegUp (VDP_CHN_DHD0);
	return HI_SUCCESS;

}

HI_S32 VDP_DRV_SetHdrHdmiMetadata(HI_U8 * pu8HeadData, HI_U32 u32MdLen, HI_U8 * pu8MdBuf)
{

	HI_U32 no_md     = pu8HeadData[0] & 0x1; 
	HI_U32 md_ver    = (pu8HeadData[0] >> 1) & 0x7; 
	HI_U32 md_type   = (pu8HeadData[0] >> 4) & 0x3; 
	HI_U32 md_id     = pu8HeadData[1]; 
	HI_U32 md_eos    = pu8HeadData[2] & 0x1; 

	VDP_HDR_SetDmHdrNoMd           ( no_md   );
	VDP_HDR_SetDmHdrMdVer          ( md_ver  );
	VDP_HDR_SetDmHdrMdType         ( md_type );
	VDP_HDR_SetDmHdrMdId           ( md_id   );
	VDP_HDR_SetDmHdrMdEos          ( md_eos  );


    memcpy((HI_VOID *)gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_HDR_METADATA],pu8MdBuf,u32MdLen);
    
    VDP_HDR_SetDmHdrMdLen          ( u32MdLen ); 
    VDP_HDR_SetDmMetadataCoefAddr  ( gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_HDR_METADATA]);
    VDP_HDR_SetDmMdParaup          (); 

	return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetComposer(rpu_ext_config_fixpt_main_t* pstCompCfg)
{
    HI_U32 ii=0,jj=0,kk=0;

    HI_S32 poly_coef[3][8][3];
    HI_S32 mmr_coef[2][22];
    HI_S64 mmr_coef_tmp;
    HI_S32 nlq_coef[3][3];
    HI_U32 pivot_value[3][9];
    HI_U32 poly_order[3][8];

	//----------------------------------------------------------------------
	// calc the config
	//----------------------------------------------------------------------
    for(kk=0;kk<3;kk++)
    {
        for(ii=0;ii<8;ii++)
        {
            for(jj=0;jj<3;jj++)
            {
                poly_coef[kk][ii][jj] = pstCompCfg->poly_coef_int[kk][ii][jj] << 23 | pstCompCfg->poly_coef[kk][ii][jj] << (23-pstCompCfg ->coefficient_log2_denom);
            }
        }
    }

    for(ii=0;ii<2;ii++)
    {
        for(jj=0;jj<22;jj++)
        {
            if(pstCompCfg ->coefficient_log2_denom<=14)
                mmr_coef[ii][jj] = pstCompCfg->MMR_coef_int[ii][jj] << 14 | pstCompCfg->MMR_coef[ii][jj] << (14-pstCompCfg ->coefficient_log2_denom);
            else
            {
                mmr_coef_tmp = ((((HI_S64)pstCompCfg->MMR_coef_int[ii][jj]) << pstCompCfg ->coefficient_log2_denom) | pstCompCfg->MMR_coef[ii][jj]) ;
                mmr_coef_tmp = mmr_coef_tmp >> (pstCompCfg ->coefficient_log2_denom - 14) ;
                mmr_coef[ii][jj] = (HI_S32)mmr_coef_tmp ;
            }
        }
    }

    for(ii=0;ii<3;ii++)
    {
        for(jj=0;jj<3;jj++)
        {
            nlq_coef[ii][jj] = pstCompCfg->NLQ_coeff_int[ii][jj] << 23 | pstCompCfg->NLQ_coeff[ii][jj] << (23-pstCompCfg ->coefficient_log2_denom) ;

        }
    }

    for(ii=0;ii<3;ii++)
    {
        for(jj=0;jj<9;jj++)
        {
            if(pstCompCfg ->rpu_BL_bit_depth == 8)
                pivot_value[ii][jj] = pstCompCfg->pivot_value[ii][jj] << 2; 
            else
                pivot_value[ii][jj] = pstCompCfg->pivot_value[ii][jj];
        }
    }

    for(ii=0;ii<3;ii++)
    {
        for(jj=0;jj<8;jj++)
        {
            poly_order[ii][jj] = pstCompCfg->poly_order[ii][jj] - 1;
        }
    }
#if 0
		//info print
		printk("pstCompCfg ->rpu_VDR_bit_depth		== %d\n",pstCompCfg->rpu_VDR_bit_depth	   );
		printk("pstCompCfg ->rpu_BL_bit_depth 		== %d\n",pstCompCfg->rpu_BL_bit_depth 	   );
		printk("pstCompCfg ->rpu_EL_bit_depth 		== %d\n",pstCompCfg->rpu_EL_bit_depth 	   );
		printk("pstCompCfg ->coefficient_log2_denom	 == %d\n",pstCompCfg->coefficient_log2_denom	);
		printk("pstCompCfg ->num_pivots[0]			== %d\n",pstCompCfg->num_pivots[0]		   );
		printk("pstCompCfg ->num_pivots[1]			== %d\n",pstCompCfg->num_pivots[1]		   );
		printk("pstCompCfg ->num_pivots[2]			== %d\n",pstCompCfg->num_pivots[2]		   );
		printk("pstCompCfg ->mapping_idc[0]			== %d\n",pstCompCfg->mapping_idc[0]		   );
		printk("pstCompCfg ->mapping_idc[1]			== %d\n",pstCompCfg->mapping_idc[1]		   );
		printk("pstCompCfg ->mapping_idc[2]			== %d\n",pstCompCfg->mapping_idc[2]		   );
		printk("pstCompCfg ->MMR_order[0] 			== %d\n",pstCompCfg->MMR_order[0] 		   );
		printk("pstCompCfg ->MMR_order[1] 			== %d\n",pstCompCfg->MMR_order[1] 		   );
		printk("pstCompCfg ->NLQ_method_idc			== %d\n",pstCompCfg->NLQ_method_idc		   );
		printk("pstCompCfg ->disable_residual_flag	 == %d\n",pstCompCfg->disable_residual_flag	);
		printk("pstCompCfg ->NLQ_offset[0]			== %d\n",pstCompCfg->NLQ_offset[0]		   );
		printk("pstCompCfg ->NLQ_offset[1]			== %d\n",pstCompCfg->NLQ_offset[1]		   );
		printk("pstCompCfg ->NLQ_offset[2]			== %d\n",pstCompCfg->NLQ_offset[2]		   );
		printk("pstCompCfg->pivot_value[0][0] 		== %d\n",pstCompCfg->pivot_value[0][0]		   );
		printk("pstCompCfg->pivot_value[0][1] 		== %d\n",pstCompCfg->pivot_value[0][1]		   );
		printk("pstCompCfg->pivot_value[0][2] 		== %d\n",pstCompCfg->pivot_value[0][2]		   );
		printk("pstCompCfg->pivot_value[0][3] 		== %d\n",pstCompCfg->pivot_value[0][3]		   );
		printk("pstCompCfg->pivot_value[0][4] 		== %d\n",pstCompCfg->pivot_value[0][4]		   );
		printk("pstCompCfg->pivot_value[0][5] 		== %d\n",pstCompCfg->pivot_value[0][5]		   );
		printk("pstCompCfg->pivot_value[0][6] 		== %d\n",pstCompCfg->pivot_value[0][6]		   );
		printk("pstCompCfg->pivot_value[0][7] 		== %d\n",pstCompCfg->pivot_value[0][7]		   );
		printk("pstCompCfg->pivot_value[0][8] 		== %d\n",pstCompCfg->pivot_value[0][8]		   );
		printk("pstCompCfg->pivot_value[1][0] 		== %d\n",pstCompCfg->pivot_value[1][0]		   );
		printk("pstCompCfg->pivot_value[1][1] 		== %d\n",pstCompCfg->pivot_value[1][1]		   );
		printk("pstCompCfg->pivot_value[1][2] 		== %d\n",pstCompCfg->pivot_value[1][2]		   );
		printk("pstCompCfg->pivot_value[1][3] 		== %d\n",pstCompCfg->pivot_value[1][3]		   );
		printk("pstCompCfg->pivot_value[1][4] 		== %d\n",pstCompCfg->pivot_value[1][4]		   );
		printk("pstCompCfg->pivot_value[2][0] 		== %d\n",pstCompCfg->pivot_value[2][0]		   );
		printk("pstCompCfg->pivot_value[2][1] 		== %d\n",pstCompCfg->pivot_value[2][1]		   );
		printk("pstCompCfg->pivot_value[2][2] 		== %d\n",pstCompCfg->pivot_value[2][2]		   );
		printk("pstCompCfg->pivot_value[2][3] 		== %d\n",pstCompCfg->pivot_value[2][3]		   );
		printk("pstCompCfg->pivot_value[2][4] 		== %d\n",pstCompCfg->pivot_value[2][4]		   );
	
	
		for(ii=0;ii<3;ii++)
		{
			for(jj=0;jj<8;jj++)
			{
				printk("pstCompCfg ->poly_order[%d][%d] = %d\n",ii,jj,pstCompCfg->poly_order[ii][jj]);
			}
		}
	
		for(kk=0;kk<3;kk++)
		{
			for(ii=0;ii<8;ii++)
			{
				for(jj=0;jj<3;jj++)
				{
				
				printk("pstCompCfg ->poly_coef_int[%d][%d][%d] = %08x\n",kk,ii,jj,pstCompCfg->poly_coef_int[kk][ii][jj]);
				printk("pstCompCfg ->poly_coef[%d][%d][%d] = %08x\n",kk,ii,jj,pstCompCfg->poly_coef[kk][ii][jj]);
				printk("poly_coef[%d][%d][%d] = %08x\n",kk,ii,jj,poly_coef[kk][ii][jj]);
	
				}
			}
		}
	
		for(ii=0;ii<2;ii++)
		{
			for(jj=0;jj<22;jj++)
			{
				printk("pstCompCfg ->MMR_coef_int[%d][%d] = %08x\n",ii,jj,pstCompCfg->MMR_coef_int[ii][jj]);
				printk("pstCompCfg ->MMR_coef[%d][%d] = %08x\n",ii,jj,pstCompCfg->MMR_coef[ii][jj]);
				printk("mmr_coef[%d][%d] = %08x\n",ii,jj,mmr_coef[ii][jj]);
			}
		}
	
		for(ii=0;ii<3;ii++)
		{
			for(jj=0;jj<3;jj++)
			{
				printk("pstCompCfg->NLQ_coeff[%d][%d] = %08x\n",ii,jj,pstCompCfg->NLQ_coeff[ii][jj]);
				printk("nlq_coef[%d][%d] = %08x\n",ii,jj,nlq_coef[ii][jj]);
			}
		}
	
		
#endif

    VDP_VID_SetComposerElUpsampleEn      ( pstCompCfg->el_spatial_resampling_filter_flag ) ;
    VDP_VID_SetComposerBlBitDepth        ( (pstCompCfg->rpu_BL_bit_depth-8)/2 ) ;
    VDP_VID_SetComposerElBitDepth        ( (pstCompCfg->rpu_EL_bit_depth-8)/2 ) ;
    VDP_VID_SetComposerVdrBitDepth       ( (pstCompCfg->rpu_VDR_bit_depth-12)/2 ) ;
    VDP_VID_SetComposerBlNumPivotsY      ( pstCompCfg->num_pivots[0]-2 ) ;
    VDP_VID_SetComposerBlNumPivotsU      ( pstCompCfg->num_pivots[1]-2 ) ;
    VDP_VID_SetComposerBlNumPivotsV      ( pstCompCfg->num_pivots[2]-2 ) ;
    VDP_VID_SetComposerBlPolyPivotValueY ( pivot_value[0]) ;
    VDP_VID_SetComposerBlPolyPivotValueU ( pivot_value[1]) ;
    VDP_VID_SetComposerBlPolyPivotValueV ( pivot_value[2]) ;
    VDP_VID_SetComposerBlPolyOrderY      ( poly_order[0] ) ;
    VDP_VID_SetComposerBlPolyOrderU      ( poly_order[1] ) ;
    VDP_VID_SetComposerBlPolyOrderV      ( poly_order[2] ) ;
    VDP_VID_SetComposerBlPolyCoefY       ( poly_coef[0] ) ;
    VDP_VID_SetComposerBlPolyCoefU       ( poly_coef[1] ) ;
    VDP_VID_SetComposerBlPolyCoefV       ( poly_coef[2] ) ;
    VDP_VID_SetComposerMappingIdcU       ( pstCompCfg->mapping_idc[1] ) ;
    VDP_VID_SetComposerMappingIdcV       ( pstCompCfg->mapping_idc[2] ) ;
    VDP_VID_SetComposerBlMmrOrderU       ( pstCompCfg->MMR_order[0]-1 ) ;
    VDP_VID_SetComposerBlMmrOrderV       ( pstCompCfg->MMR_order[1]-1 ) ;
    VDP_VID_SetComposerBlMmrCoefU        ( mmr_coef[0] ) ;
    VDP_VID_SetComposerBlMmrCoefV        ( mmr_coef[1] ) ;
    VDP_VID_SetComposerElEn              ( !pstCompCfg->disable_residual_flag ) ;
    VDP_VID_SetComposerElNldqOffsetY     ( pstCompCfg->NLQ_offset[0] ) ;
    VDP_VID_SetComposerElNldqOffsetU     ( pstCompCfg->NLQ_offset[1] ) ;
    VDP_VID_SetComposerElNldqOffsetV     ( pstCompCfg->NLQ_offset[2] ) ;
    VDP_VID_SetComposerElNldqCoefY       ( nlq_coef[0] ) ;
    VDP_VID_SetComposerElNldqCoefU       ( nlq_coef[1] ) ;
    VDP_VID_SetComposerElNldqCoefV       ( nlq_coef[2] ) ;

    VDP_VID_SetComposerBlEn              ( 1 );
		
    if(pstCompCfg->el_spatial_resampling_filter_flag == 1)
        VDP_VID_SetHdrElZme(VDP_LAYER_VID1, HI_TRUE);
    else
        VDP_VID_SetHdrElZme(VDP_LAYER_VID1, HI_FALSE);        

	return HI_SUCCESS;
}


HI_S32 VDP_DRV_SetVdm(DmKsFxp_t* pstDmKs)
{
	HI_U32 u32InBdp;
	HI_U32 u32OutBdp;
	HI_U32 u32InClr;
	HI_U32 u32OutClr;

#if 0
	//#if HDR_DEBUG
	printk("(HI_S32)pstDmKs->rowPitchNum					== %d\n",(HI_S32)pstDmKs->rowPitchNum				   );
	printk("(HI_U32)pstDmKs->rowPitchNum					== %d\n",(HI_U32)pstDmKs->rowPitchNum				   );
	printk("(HI_U32)pstDmKs->rowPitchNum					== %d\n",(HI_U32)pstDmKs->rowPitchNum				   );
	printk("(HI_U32)pstDmKs->rowPitchNum					== %d\n",(HI_U32)pstDmKs->rowPitchNum				   );
	printk("(HI_S32)pstDmKs->ksUds.chrmIn				== %d\n",(HI_S32)pstDmKs->ksUds.chrmIn			   );
	printk("(HI_S32)pstDmKs->ksUds.chrmOut				== %d\n",(HI_S32)pstDmKs->ksUds.chrmOut 		   );
	printk("(HI_S32)pstDmKs->ksUds.minUs					== %d\n",(HI_S32)pstDmKs->ksUds.minUs				   );
	printk("(HI_S32)pstDmKs->ksUds.maxUs					== %d\n",(HI_S32)pstDmKs->ksUds.maxUs				   );
	printk("(HI_S32)pstDmKs->ksUds.minDs					== %d\n",(HI_S32)pstDmKs->ksUds.minDs				   );
	printk("(HI_S32)pstDmKs->ksUds.maxDs					== %d\n",(HI_S32)pstDmKs->ksUds.maxDs				   );
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUsHalfSize  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUsHalfSize );
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUsScale2P   == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUsScale2P	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[0]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[0]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[1]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[1]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[2]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[2]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[3]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[3]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[4]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[4]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[5]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[5]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[6]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[6]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[7]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs0_m[7]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[0]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[0]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[1]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[1]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[2]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[2]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[3]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[3]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[4]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[4]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[5]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[5]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[6]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[6]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[7]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvRowUs1_m[7]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUsHalfSize  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUsHalfSize );
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUsScale2P   == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUsScale2P	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[0]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[0]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[1]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[1]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[2]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[2]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[3]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[3]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[4]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[4]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[5]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[5]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[6]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[6]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColUs_m[7]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColUs_m[7]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDsRadius	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDsRadius	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDsScale2P   == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDsScale2P	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[0]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[0]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[1]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[1]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[2]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[2]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[3]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[3]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[4]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[4]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[5]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[5]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[6]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[6]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[7]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[7]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[8]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[8]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[9]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[9]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[10]	  == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[10]	);
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[11]	 == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[11]  );
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[12]	 == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[12]  );
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[13]	 == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[13]  );
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[14]	 == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[14]  );
	printk("(HI_S32)pstDmKs->ksUds.filterUvColDs_m[15]	 == %d\n",(HI_S32)pstDmKs->ksUds.filterUvColDs_m[15]  );
	printk("(HI_S32)pstDmKs->ksDmCtrl.mainIn			   == %d\n",(HI_S32)pstDmKs->ksDmCtrl.mainIn			  );
	printk("(HI_S32)pstDmKs->ksDmCtrl.prf			   == %d\n",(HI_S32)pstDmKs->ksDmCtrl.prf			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.rowNum		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.rowNum		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.colNum		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.colNum		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.dtp			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.dtp			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.weav 		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.weav 		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.loc			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.loc			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.bdp			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.bdp			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.chrm 		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.chrm 		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.clr			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.clr			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.rowPitch 	   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.rowPitch 	  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.colPitch 	   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.colPitch 	  );
	printk("(HI_S32)pstDmKs->ksFrmFmtI.rowPitchC		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtI.rowPitchC		  );
	printk("(HI_S32)pstDmKs->ksIMap.clr 			   == %d\n",(HI_S32)pstDmKs->ksIMap.clr 			  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2RgbScale2P	 == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2RgbScale2P   );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[0][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[1][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Yuv2Rgb[2][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[0] == %d\n",(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[0]);
	printk("(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[1] == %d\n",(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[1]);
	printk("(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[2] == %d\n",(HI_S32)pstDmKs->ksIMap.v3Yuv2RgbOffInRgb[2]);
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.rangeMin	 == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.rangeMin  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.range    == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.range   );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.rangeInv	 == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.rangeInv  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.bdp	   == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.bdp	  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.eotf	   == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.eotf	  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.gamma    == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.gamma   );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.a 	   == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.a 	  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.b 	   == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.b 	  );
	printk("(HI_U32)pstDmKs->ksIMap.eotfParam.g 	   == %d\n",(HI_U32)pstDmKs->ksIMap.eotfParam.g 	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2LmsScale2P	 == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2LmsScale2P   );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[0][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[1][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Rgb2Lms[2][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2IptScale2P	 == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2IptScale2P   );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[0][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[1][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][0]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][0]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][1]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][1]	  );
	printk("(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][2]	   == %d\n",(HI_S32)pstDmKs->ksIMap.m33Lms2Ipt[2][2]	  );
	printk("(HI_S32)pstDmKs->ksIMap.iptScale			   == %d\n",(HI_S32)pstDmKs->ksIMap.iptScale			  );
	printk("(HI_S32)pstDmKs->ksIMap.v3IptOff[0] 	   == %d\n",(HI_S32)pstDmKs->ksIMap.v3IptOff[0] 	  );
	printk("(HI_S32)pstDmKs->ksIMap.v3IptOff[1] 	   == %d\n",(HI_S32)pstDmKs->ksIMap.v3IptOff[1] 	  );
	printk("(HI_S32)pstDmKs->ksIMap.v3IptOff[2] 	   == %d\n",(HI_S32)pstDmKs->ksIMap.v3IptOff[2] 	  );
	printk("(HI_S32)pstDmKs->ksTMap.tmLutISizeM1		   == %d\n",(HI_S32)pstDmKs->ksTMap.tmLutISizeM1		  );
	printk("(HI_S32)pstDmKs->ksTMap.tmLutSSizeM1		   == %d\n",(HI_S32)pstDmKs->ksTMap.tmLutSSizeM1		  );
	printk("(HI_S32)pstDmKs->ksTMap.smLutISizeM1		   == %d\n",(HI_S32)pstDmKs->ksTMap.smLutISizeM1		  );
	printk("(HI_S32)pstDmKs->ksTMap.smLutSSizeM1		   == %d\n",(HI_S32)pstDmKs->ksTMap.smLutSSizeM1		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.rowNum			  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.rowNum			 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.colNum			  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.colNum			 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.dtp				  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.dtp				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.weav 			  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.weav				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.loc				  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.loc				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.bdp				  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.bdp				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.chrm 			  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.chrm				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.clr				  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.clr				 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.rowPitch 		  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.rowPitch			 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.colPitch 		  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.colPitch			 );
	printk("(HI_S32)pstDmKs->ksFrmFmtG.rowPitchC			  == %d\n",(HI_S32)pstDmKs->ksFrmFmtG.rowPitchC 		 );
	printk("(HI_S32)pstDmKs->ksIMapG.clr				  == %d\n",(HI_S32)pstDmKs->ksIMapG.clr 				 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2RgbScale2P	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2RgbScale2P	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[0][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[1][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Yuv2Rgb[2][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[0]  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[0] );
	printk("(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[1]  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[1] );
	printk("(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[2]  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3Yuv2RgbOffInRgb[2] );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.rangeMin   == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.rangeMin	 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.range	  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.range 	 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.rangeInv   == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.rangeInv	 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.bdp		  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.bdp		 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.eotf 	  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.eotf		 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.gamma	  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.gamma 	 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.a		  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.a 		 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.b		  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.b 		 );
	printk("(HI_U32)pstDmKs->ksIMapG.eotfParam.g		  == %d\n",(HI_U32)pstDmKs->ksIMapG.eotfParam.g 		 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2LmsScale2P	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2LmsScale2P	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[0][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[1][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Rgb2Lms[2][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2IptScale2P	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2IptScale2P	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[0][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[1][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][0]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][0]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][1]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][1]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][2]	  == %d\n",(HI_S32)pstDmKs->ksIMapG.m33Lms2Ipt[2][2]	 );
	printk("(HI_S32)pstDmKs->ksIMapG.iptScale			  == %d\n",(HI_S32)pstDmKs->ksIMapG.iptScale			 );
	printk("(HI_S32)pstDmKs->ksIMapG.v3IptOff[0]		  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3IptOff[0] 		 );
	printk("(HI_S32)pstDmKs->ksIMapG.v3IptOff[1]		  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3IptOff[1] 		 );
	printk("(HI_S32)pstDmKs->ksIMapG.v3IptOff[2]		  == %d\n",(HI_S32)pstDmKs->ksIMapG.v3IptOff[2] 		 );
	printk("(HI_S32)pstDmKs->ksTMapG.tmLutISizeM1		  == %d\n",(HI_S32)pstDmKs->ksTMapG.tmLutISizeM1		 );
	printk("(HI_S32)pstDmKs->ksTMapG.tmLutSSizeM1		  == %d\n",(HI_S32)pstDmKs->ksTMapG.tmLutSSizeM1		 );
	printk("(HI_S32)pstDmKs->ksTMapG.smLutISizeM1		  == %d\n",(HI_S32)pstDmKs->ksTMapG.smLutISizeM1		 );
	printk("(HI_S32)pstDmKs->ksTMapG.smLutSSizeM1		  == %d\n",(HI_S32)pstDmKs->ksTMapG.smLutSSizeM1		 );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2LmsScale2P	 == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2LmsScale2P   );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[0][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[1][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Ipt2Lms[2][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2RgbScale2P	 == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2RgbScale2P   );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[0][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[1][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Lms2Rgb[2][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.oetfParam.oetf	   == %d\n",(HI_S32)pstDmKs->ksOMap.oetfParam.oetf	  );
	printk("(HI_S32)pstDmKs->ksOMap.oetfParam.min	   == %d\n",(HI_S32)pstDmKs->ksOMap.oetfParam.min	  );
	printk("(HI_S32)pstDmKs->ksOMap.oetfParam.max	   == %d\n",(HI_S32)pstDmKs->ksOMap.oetfParam.max	  );
	printk("(HI_S32)pstDmKs->ksOMap.oetfParam.bdp	   == %d\n",(HI_S32)pstDmKs->ksOMap.oetfParam.bdp	  );
	printk("(HI_S32)pstDmKs->ksOMap.clr 			   == %d\n",(HI_S32)pstDmKs->ksOMap.clr 			  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2YuvScale2P	 == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2YuvScale2P   );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[0][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[1][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][0]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][0]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][1]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][1]	  );
	printk("(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][2]	   == %d\n",(HI_S32)pstDmKs->ksOMap.m33Rgb2Yuv[2][2]	  );
	printk("(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[0]    == %d\n",(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[0]   );
	printk("(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[1]    == %d\n",(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[1]   );
	printk("(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[2]    == %d\n",(HI_S32)pstDmKs->ksOMap.v3Rgb2YuvOff[2]   );
	printk("(HI_S32)pstDmKs->ksOMap.iptScale			   == %d\n",(HI_S32)pstDmKs->ksOMap.iptScale			  );
	printk("(HI_S32)pstDmKs->ksOMap.v3IptOff[0] 	   == %d\n",(HI_S32)pstDmKs->ksOMap.v3IptOff[0] 	  );
	printk("(HI_S32)pstDmKs->ksOMap.v3IptOff[1] 	   == %d\n",(HI_S32)pstDmKs->ksOMap.v3IptOff[1] 	  );
	printk("(HI_S32)pstDmKs->ksOMap.v3IptOff[2] 	   == %d\n",(HI_S32)pstDmKs->ksOMap.v3IptOff[2] 	  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.rowNum		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.rowNum		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.colNum		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.colNum		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.dtp			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.dtp			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.weav 		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.weav 		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.loc			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.loc			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.bdp			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.bdp			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.chrm 		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.chrm 		  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.clr			   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.clr			  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.rowPitch 	   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.rowPitch 	  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.colPitch 	   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.colPitch 	  );
	printk("(HI_S32)pstDmKs->ksFrmFmtO.rowPitchC		   == %d\n",(HI_S32)pstDmKs->ksFrmFmtO.rowPitchC		  );
	printk("(HI_S32)pstDmKs->bypassShift				   == %d\n",(HI_S32)pstDmKs->bypassShift				  );
#endif

    //composer up sample
    VDP_VID_SetComposeElUpsampleMax    (pstDmKs->ksUds.maxUs);
    VDP_VID_SetComposeElUpsampleMin    (pstDmKs->ksUds.minUs);


    //vdm_yuv2rgb 
    VDP_HDR_SetVDmYuv2rgbEn            ( 1 );
    VDP_HDR_SetVDmYuv2rgbV0En          ( 1 ); 
    VDP_HDR_SetVDmYuv2rgbV1En          ( 0 ); 
    VDP_HDR_SetVDmImap0Yuv2rgb         (pstDmKs->ksIMap.m33Yuv2Rgb);
    VDP_HDR_SetVDmImapYuv2rgbscale2p   (pstDmKs->ksIMap.m33Yuv2RgbScale2P);
    VDP_HDR_SetVDmImapYuv2rgbOutDc0    (pstDmKs->ksIMap.v3Yuv2RgbOffInRgb);

    u32InBdp = (pstDmKs->ksIMap.eotfParam.bdp == 12) ? 0:1;
    VDP_HDR_SetVDmInBits               ( u32InBdp );

    //vdm_rgb2lms
    VDP_HDR_SetVDmRgb2lmsEn            ( 1 ); 
    VDP_HDR_SetVDmImapRgb2lms          ( pstDmKs->ksIMap.m33Rgb2Lms       );
    VDP_HDR_SetVDmImapRgb2lmsscale2p   ( pstDmKs->ksIMap.m33Rgb2LmsScale2P);
    VDP_HDR_SetVDmImapRgb2lmsmin       ( 0 );
    //VDP_HDR_SetVDmImapRgb2lmsmax       ( (HI_U32)((HI_U64)10000*(1<<18))  );
    VDP_HDR_SetVDmImapRgb2lmsmax        ( 0x9c400000  );//(10000LL*(1<<18))

    //cvm1_lms2ipt
    VDP_HDR_SetVCvmLms2iptEn           ( 1 ); 
    VDP_HDR_SetVDmImapLms2iptmin       ( 0-((1<<15)-1) );
    VDP_HDR_SetVDmImapLms2iptmax       ( ((1<<15)-1) );
    VDP_HDR_SetVCvmImapLms2ipt         ( pstDmKs->ksIMap.m33Lms2Ipt           );
    VDP_HDR_SetVCvmImapLms2iptscale2p  ( pstDmKs->ksIMap.m33Lms2IptScale2P +1 );

    //cvm1_ipt2lms
    VDP_HDR_SetVCvmIpt2lmsEn           ( 1 ); 
    VDP_HDR_SetVCvmOmapIpt2lms         ( pstDmKs->ksOMap.m33Ipt2Lms       );
    VDP_HDR_SetVCvmOmapIpt2lmsscale2p  ( pstDmKs->ksOMap.m33Ipt2LmsScale2P);
    VDP_HDR_SetVDmOmapIpt2lmsmin       ( 0 );
    VDP_HDR_SetVDmOmapIpt2lmsmax       ( (1<<16)-1 );

    //cvm1_lms2rgb
    VDP_HDR_SetVCvmLms2rgbEn           ( 1 ); 
    VDP_HDR_SetVCvmOmapLms2rgb         ( pstDmKs->ksOMap.m33Lms2Rgb       );
    VDP_HDR_SetVCvmOmapLms2rgbscale2p  ( pstDmKs->ksOMap.m33Lms2RgbScale2P);
    VDP_HDR_SetVDmOmapLms2rgbmin       ( pstDmKs->ksOMap.oetfParam.min    );
    VDP_HDR_SetVDmOmapLms2rgbmax       ( pstDmKs->ksOMap.oetfParam.max    );

    //cvm1_rgb2yuv
    VDP_HDR_SetVCvmRgb2yuvEn           ( 1 ); 
    VDP_HDR_SetVCvmOmapRgb2yuv         ( pstDmKs->ksOMap.m33Rgb2Yuv       );
    VDP_HDR_SetVCvmOmapRgb2yuvscale2p  ( pstDmKs->ksOMap.m33Rgb2YuvScale2P);
    VDP_HDR_SetVCvmOmapRgb2yuvOutDc    ( pstDmKs->ksOMap.v3Rgb2YuvOff     );

    //VDM_PQ2L
    VDP_HDR_SetVDmInEotf               ( pstDmKs->ksIMap.eotfParam.eotf   ) ;
    VDP_HDR_SetVDmDegammaEn            ( 1 );
    VDP_HDR_SetVCvmEn                  ( 1 );
    VDP_HDR_SetVCvmDegammaEn           ( 1 );
    VDP_HDR_SetVCvmMGammaEn            ( 1 );
    VDP_HDR_SetVDmGammaEn              ( 1 );
    VDP_HDR_SetVDmNormEn               ( 1 );
    VDP_HDR_SetVDmImapRangeMin         ( pstDmKs->ksIMap.eotfParam.rangeMin     );
    VDP_HDR_SetVDmImapRange            ( pstDmKs->ksIMap.eotfParam.range        );
    VDP_HDR_SetVDmImapRangeInv         ( pstDmKs->ksIMap.eotfParam.rangeInv     );
    VDP_HDR_SetVCvmOmapRangeMin        ( pstDmKs->ksOMap.oetfParam.rangeMin     );
    VDP_HDR_SetVCvmOmapRangeOver       ( pstDmKs->ksOMap.oetfParam.rangeOverOne );

    u32OutBdp = (pstDmKs->ksOMap.oetfParam.bdp == 12) ? 2:((pstDmKs->ksOMap.oetfParam.bdp == 10) ? 1 : 0 );
    VDP_HDR_SetCvmOutBits              ( u32OutBdp );
    VDP_HDR_SetVCvmDenormEn            ( 1 );
    VDP_HDR_SetVDmImapIptoff           ( pstDmKs->ksIMap.v3IptOff               );
    VDP_HDR_SetVDmImapIptScale         ( pstDmKs->ksIMap.iptScale               );

    u32InClr = (pstDmKs->ksIMap.clr == 3) ? 0 : 1;
    VDP_HDR_SetVDmInColor              ( u32InClr );

    u32OutClr = (pstDmKs->ksOMap.clr == 3) ? 0 : 1;
    VDP_HDR_SetCvmOutColor             ( u32OutClr );

    VDP_HDR_SetVCvmOmapIptoff          ( pstDmKs->ksOMap.v3IptOff );
    VDP_HDR_SetVCvmOmapIptScale        ( pstDmKs->ksOMap.iptScale );

	//downsample
    VDP_HDR_SetDmHdrDsMax              ( pstDmKs->ksUds.maxDs );
    VDP_HDR_SetDmHdrDsMin              ( pstDmKs->ksUds.minDs );

	return HI_SUCCESS;
}

HI_VOID VDP_DRV_SetGdm(HI_VOID)
{
	

    HI_U32 g_degamma_en = 1;//stCfg -> degamm_en; 
    HI_U32 g_gamma_en   = 1;//stCfg -> gamm_en;
    HI_U32 gcvm_en      = 1;//stCfg -> gcvm_en;
    //HI_U32 cvm_sel      = 1;//stCfg -> cvm_sel;
    //HI_U32 cvm_en       = 1;//stCfg -> cvm_en;
    HI_U32 gdm_en       = 1;//stCfg -> gdm_en;

    HI_U32 g_rgb2lms_en             ; 
    HI_S16 g_imap_rgb2lms[3][3]     ; 
    HI_U32 g_imap_rgb2lmsscale2p    ; 
    HI_U32 g_imap_rgb2lmsmin        ; 
    HI_U32 g_imap_rgb2lmsmax        ; 
    HI_S32 g_imap_rgb2lms_out_dc[3] ; 
    //HI_U32 gamma_rd_en              ; 
    //HI_U32 degamma_rd_en            ;  
//**************-----degamm and gamma---------------
    HI_U32 g_degmm_step[4]          ; 
    HI_U32 g_degmm_pos[4]           ; 
    HI_U32 g_degmm_num[4]           ; 

    HI_U32 g_gmm_step[4]            ; 
    HI_U32 g_gmm_pos[4]             ; 
    HI_U32 g_gmm_num[4]             ; 

    HI_U32 g_rgb2yuv_en             ;
    HI_S16 g_omap_rgb2yuv[3][3]     ;
    HI_U32 g_omap_rgb2yuvscale2p    ;
    HI_S32 g_omap_rgb2yuv_out_dc[3] ;

//**************-----gcvm---------------
    HI_U32 g_gcvm_x2_pos            ;
    HI_U32 g_gcvm_x1_pos            ;
    HI_U32 g_gcvm_x1_b              ;
    HI_U32 g_gcvm_x1_a              ;
    HI_U32 g_gcvm_x2_b              ;
    HI_U32 g_gcvm_x2_a              ;
    HI_U32 g_gcvm_x3_b              ;
    HI_U32 g_gcvm_x3_a              ;

                                    
    g_rgb2lms_en = 1;//stCfg -> g_rgb2lms_en;
    g_rgb2yuv_en = 1;//stCfg -> g_rgb2yuv_en;


    {      
//****************-----degamm-----**********************
        g_degmm_pos[0]           = 64;
        g_degmm_pos[1]           = 64*2;
        g_degmm_pos[2]           = 64*3;
        g_degmm_pos[3]           = 252;//255;
        g_degmm_step[0]          = 2;
        g_degmm_step[1]          = 2;
        g_degmm_step[2]          = 2;
        g_degmm_step[3]          = 2;
        g_degmm_num[0]           = 16;
        g_degmm_num[1]           = 16;
        g_degmm_num[2]           = 16;
        g_degmm_num[3]           = 15;
//****************-----degamm end-----**********************
#if 0
        //****************-----gamm-----**********************
        g_gmm_pos[0]           = 1536 ;//1792  ;//256;//mod by suyong //832;//16384;
        g_gmm_pos[1]           = 230912 ;//345856;//15616;//mod by suyong //21312;//16384*2;
        g_gmm_pos[2]           = 4425216  ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
        g_gmm_pos[3]           = 27493888;//27493888;//mod by suyong //26170716;//16384*3;
        g_gmm_step[0]          = 8;//4;//mod by suyong //5;//10;
        g_gmm_step[1]          = 14;//16;//10;//mod by suyong //11;//10;
        g_gmm_step[2]          = 17;//18;//16;//mod by suyong //16;//10;
        g_gmm_step[3]          = 21;//20;//mod by suyong //21;//10;
        g_gmm_num[0]           = 6;//16;//mod by suyong //26;//16;
        g_gmm_num[1]           = 14;//15;//mod by suyong //10;//16;
        g_gmm_num[2]           = 32;//20;//mod by suyong //15;//16;
        g_gmm_num[3]           = 12;//13;//mod by suyong //12;//16;
#else
        //****************-----gamm-----**********************
        g_gmm_pos[0]           = 557056 ;//1792 ;//256;//mod by suyong //832;//16384;
        g_gmm_pos[1]           = 2916352 ;//345856;//15616;//mod by suyong //21312;//16384*2;
        g_gmm_pos[2]           = 14450688 ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
        g_gmm_pos[3]           = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
        g_gmm_step[0]          = 15;//4;//mod by suyong //5;//10;
        g_gmm_step[1]          = 17;//16;//10;//mod by suyong //11;//10;
        g_gmm_step[2]          = 19;//18;//16;//mod by suyong //16;//10;
        g_gmm_step[3]          = 21;//20;//mod by suyong //21;//10;
        g_gmm_num[0]           = 17;//16;//mod by suyong //26;//16;
        g_gmm_num[1]           = 18;//15;//mod by suyong //10;//16;
        g_gmm_num[2]           = 22;//20;//mod by suyong //15;//16;
        g_gmm_num[3]           = 7;//13;//mod by suyong //12;//16;
        //****************-----degamm end-----**********************
#endif

//****************-----degamm end-----**********************
        g_imap_rgb2lms[0][0]     = 8; 
        g_imap_rgb2lms[0][1]     = 4; 
        g_imap_rgb2lms[0][2]     = 4; 
        g_imap_rgb2lms[1][0]     = 8; 
        g_imap_rgb2lms[1][1]     = 4; 
        g_imap_rgb2lms[1][2]     = 4; 
        g_imap_rgb2lms[2][0]     = 8; 
        g_imap_rgb2lms[2][1]     = 4; 
        g_imap_rgb2lms[2][2]     = 4; 
        g_imap_rgb2lmsscale2p    = 16;
        g_imap_rgb2lmsmin        = 0;
        g_imap_rgb2lmsmax        = 26219552;
        g_imap_rgb2lms_out_dc[0] = 10;
        g_imap_rgb2lms_out_dc[1] = 10;
        g_imap_rgb2lms_out_dc[2] = 10;
        g_omap_rgb2yuv[0][0]     = 8;
        g_omap_rgb2yuv[0][1]     = 4;
        g_omap_rgb2yuv[0][2]     = 4;
        g_omap_rgb2yuv[1][0]     = 8;
        g_omap_rgb2yuv[1][1]     = 4;
        g_omap_rgb2yuv[1][2]     = 4;
        g_omap_rgb2yuv[2][0]     = 8;
        g_omap_rgb2yuv[2][1]     = 4;
        g_omap_rgb2yuv[2][2]     = 4;
        g_omap_rgb2yuvscale2p    = 12;
        g_omap_rgb2yuv_out_dc[0] = 10; 
        g_omap_rgb2yuv_out_dc[1] = 10; 
        g_omap_rgb2yuv_out_dc[2] = 10; 
//****************-----gcvm-----**********************
        g_gcvm_x1_pos            = 16384; 
        g_gcvm_x2_pos            = 16384*2; 
        g_gcvm_x1_a              = 512;
        g_gcvm_x1_b              = 16384;
        g_gcvm_x2_a              = 512;
        g_gcvm_x2_b              = 16384*2;
        g_gcvm_x3_a              = 512;
        g_gcvm_x3_b              = 16384*2 + 500;
//****************-----gcvm end-----**********************
    }
    
        
                                    
    VDP_HDR_SetGDmEn(gdm_en);
    VDP_HDR_SetGDmDegammaEn           ( g_degamma_en);
    VDP_HDR_SetGDmRgb2lmsEn           ( g_rgb2lms_en             ) ;
    VDP_HDR_SetGDmImapRgb2lms         ( g_imap_rgb2lms     ) ;
    VDP_HDR_SetGDmImapRgb2lmsscale2p  ( g_imap_rgb2lmsscale2p    ) ;
    VDP_HDR_SetGDmImapRgb2lmsmin      ( g_imap_rgb2lmsmin        ) ;
    VDP_HDR_SetGDmImapRgb2lmsmax      ( g_imap_rgb2lmsmax        ) ;
    VDP_HDR_SetGDmImapRgb2lmsOutDc    ( g_imap_rgb2lms_out_dc ) ;
    VDP_HDR_SetGCvmLms2IptEn          ( g_rgb2yuv_en             ) ;
    VDP_HDR_SetGCvmLms2Ipt            ( g_omap_rgb2yuv     ) ;
    VDP_HDR_SetGCvmLms2IptScale2p     ( g_omap_rgb2yuvscale2p    ) ;
    VDP_HDR_SetGCvmLms2IptOutDc       ( g_omap_rgb2yuv_out_dc ) ;

    VDP_HDR_SetGDmDegmmPos            ( g_degmm_pos);
    VDP_HDR_SetGDmDegmmStep           ( g_degmm_step);
    VDP_HDR_SetGDmDegmmNum            ( g_degmm_num);
    VDP_HDR_SetGDmGammaEn             ( g_gamma_en);
    VDP_HDR_SetGDmGammaPos            ( g_gmm_pos);
    VDP_HDR_SetGDmGammaStep           ( g_gmm_step);
    VDP_HDR_SetGDmGmmNum              ( g_gmm_num);
//  gcvm ------
    VDP_HDR_SetAuthEn                 ( gcvm_en);
    //VDP_HDR_SetCvmSel                 ( cvm_sel);
    //VDP_HDR_SetCvmEn                  ( cvm_en);


}


static HI_U32   u32GFX_AlphabBack = 0xff;

static HI_U32   u32GFX_MuteBack = 0;


HI_VOID VDP_DRV_CleanHdr(HI_BOOL  bclear)
{
	if (!bclear)
	{
	    VDP_VID_SetLayerEnable (VDP_LAYER_VID1, HI_FALSE);
	    
	    VDP_HDR_SetHdrEnable		    (HI_FALSE);
	    VDP_HDR_SetVCvmDenormEn 	    (HI_FALSE);
	    VDP_HDR_SetVDmNormEn		    (HI_FALSE);
	    VDP_HDR_SetVDmInColor		    (1);
	    VDP_HDR_SetVDmEn			    (1);
	    VDP_HDR_SetVDmGammaEn           (0);
	    VDP_HDR_SetVDmDegammaEn         (0);
	    VDP_HDR_SetVDmGammaV0En         (0);
	    VDP_HDR_SetVDmGammaV1En         (0);
	    VDP_HDR_SetDmHdrDsEn            (0);
	    VDP_HDR_SetDmHdrMdEn            ( 0 );
	    VDP_HDR_SetGDmEn                ( 0 );
	    VDP_HDR_SetGDmDegammaEn         ( 0 );
	    VDP_HDR_SetGDmRgb2lmsEn         ( 0 );
	    VDP_HDR_SetGDmGammaEn           ( 0 );

	    VDP_HDR_SetCvmSel			    (0);
	    VDP_HDR_SetVCvmLms2iptEn		(1)  ;
	    VDP_HDR_SetVCvmEn			    (0 ) ;
	    VDP_HDR_SetVDmInBits			(1) ;
	    VDP_DISP_SetHdmiMode(VDP_CHN_DHD0,0);

	    //VDP_VP_SetMuteEnable              (VDP_LAYER_VP0, HI_FALSE);

		/*closed GFX */

		VDP_GP_GetLayerGalpha(0,&u32GFX_AlphabBack);
		VDP_GFX_GetMuteEnable(0,&u32GFX_MuteBack);

		VDP_GP_SetLayerGalpha(0,0);
		VDP_GFX_SetMuteEnable(0,1);

	    VDP_GP_SetRegUp(VDP_LAYER_GFX0);
		VDP_HDR_SetRegUp();
	    VDP_VID_SetRegUp(VDP_LAYER_VID1);
		//VDP_VP_SetRegUp(VDP_LAYER_VP0);
	
	    VDP_DISP_SetRegUp(VDP_CHN_DHD0);

	}
	else
	{
		/*resum GFX  */
		VDP_GP_SetLayerGalpha(0,u32GFX_AlphabBack);
		VDP_GFX_SetMuteEnable(0,u32GFX_MuteBack);
		
	    VDP_GFX_MixvBypassEn	(0);
		VDP_GP_SetRegUp(VDP_LAYER_GFX0);
		VDP_HDR_SetRegUp();
	    VDP_VID_SetRegUp(VDP_LAYER_VID1);
		
	    VDP_DISP_SetRegUp(VDP_CHN_DHD0);
	}
	
}


HI_VOID VDP_DRV_MaskAndBackupGfx(HI_U32 u32GfxLayerId,HI_U32 * pu32Alpha,HI_U32 * pu32MuteState)
{
    VDP_GP_GetLayerGalpha(u32GfxLayerId,pu32Alpha);
    VDP_GFX_GetMuteEnable(u32GfxLayerId,pu32MuteState);
    VDP_GP_SetLayerGalpha(u32GfxLayerId,0);
	VDP_GFX_SetMuteEnable(u32GfxLayerId,1);

    VDP_GP_SetRegUp(u32GfxLayerId);
    return;
}

HI_VOID VDP_DRV_UnmaskGfx(HI_U32 u32GfxLayerId,HI_U32 u32Alpha,HI_U32 u32MuteState)
{
    VDP_GP_SetLayerGalpha(u32GfxLayerId,u32Alpha);
	VDP_GFX_SetMuteEnable(u32GfxLayerId,u32MuteState);

    VDP_GP_SetRegUp(u32GfxLayerId);
    return;

}

HI_VOID VDP_DRV_SetGfxMixvBypassState(HI_U32 u32GfxLayerId,HI_BOOL bEnable)
{
    if(HI_TRUE == bEnable)
    {
        VDP_GFX_SetPreMultEnable(u32GfxLayerId,HI_FALSE);
		VDP_GFX_MixvBypassEn		   (1);
		VDP_GFX_MixvPremultEn		   (0);
    }
    else
    {
        VDP_GFX_MixvBypassEn	(0);
    }
    
    VDP_GP_SetRegUp(u32GfxLayerId);
    return;
}
