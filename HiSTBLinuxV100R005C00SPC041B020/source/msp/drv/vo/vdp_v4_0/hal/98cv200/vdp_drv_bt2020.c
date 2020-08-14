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
#include "vdp_drv_func.h"
#include "vdp_drv_coef.h"

#include "Hisihdr10l2PqLutA.h"
#include "Hisihdr10l2PqLutB.h"
#include "Hisihdr10l2PqLutX.h"
#include "HisiHdr10DeGamma.h"
#include "HIsiHdr10_depq1024Lut.h"

#include "HiHdr_Hlg_TMLut.h"
#include "HiHDR_IM_deGamma1024Lut_Hlg.h"
#include "HiHdr_IM_GammaLutA_Hlg.h"
#include "HiHdr_IM_GammaLutB_Hlg.h"
#include "HiHdr_IM_GammaLutX_Hlg.h"
#include "HiHdr_OM_deGamma1024Lut_Hlg.h"
#include "HiHdr_OM_GammaLutA_Hlg.h"
#include "HiHdr_OM_GammaLutB_Hlg.h"
#include "HiHdr_OM_GammaLutX_Hlg.h"

//o gamma


#include "HisiHdr10_l2gLutA.h"
#include "HisiHdr10_l2gLutB.h"
#include "HisiHdr10_l2gLutX.h"
#include "vdp_drv_hihdr.h"

#if VDP_CBB_FPGA
#include "hi_drv_mmz.h"
#endif

#include "vdp_drv_bt2020.h"

extern  uint32_t HisiHdr10l2pqLutX[128];
extern  uint32_t HisiHdr10l2pqLutA[128];
extern  uint32_t HisiHdr10l2pqLutB[128];
extern  uint32_t HisiHdr10GammaLutX[128];
extern  uint32_t HisiHdr10GammaLutA[128];
extern  uint32_t HisiHdr10GammaLutB[128];




extern uint32_t HisiHdr10Degamma[256] ;
extern  uint32_t HisiHdr10_1024Lut[1024]; 


extern uint32_t  HisiSdrBt2020_709LutX[128];
extern uint32_t  HisiSdrBt2020_709LutA[128];
extern uint32_t  HisiSdrBt2020_709LutB[128];

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;
//extern HI_U32 gdm_gmm_rgb2bt2020lut[64];
extern HI_U32 gdm_degmm_lut[64];

#if 1//王正提供rgb->bt2020表
HI_U32 gdm_gmm_rgb2bt2020lut[64] =
{
0 ,
369 ,
737 ,
1106 ,
1475 ,
1843 ,
2212 ,
2580 ,
2949 ,
3318 ,
3686 ,
4055 ,
4424 ,
4792 ,
5161 ,
5544 ,
5898 ,
6241 ,
7510 ,
8653 ,
9699 ,
10668,
11574,
12428,
13237,
14008,
14745,
15451,
16131,
16787,
17421,
18035,
18631,
19210,
19774,
20323,
22393,
24297,
26066,
27725,
29291,
30778,
32195,
33551,
34854,
36108,
37319,
38490,
39625,
40727,
41798,
42841,
43857,
44849,
45819,
46766,
47694,
48603,
52067,
55297,
58333,
61204,
63933,
65535,

};
#endif

HI_S32 VDP_DRV_SetHisiBt2020GfxBaseSetting(VDP_HISI_HDR_CFG_S  *stHdrCfg)
{
    #if 0
	
	VDP_GFX_SetLayerEnable             (VDP_LAYER_GFX0, stHdrCfg->bGfxEn);
		VDP_GFX_SetInDataFmt		   (VDP_LAYER_GFX0, VDP_GFX_IFMT_ARGB_8888);
		VDP_GFX_SetReadMode 		   (VDP_LAYER_GFX0, VDP_RMODE_PROGRESSIVE);
		VDP_GFX_SetPreMultEnable	   (VDP_LAYER_GFX0, HI_FALSE);
		VDP_GFX_SetLayerGalpha		   (VDP_LAYER_GFX0, 0xff);
		VDP_GFX_SetPalpha			   (VDP_LAYER_GFX0, 1, 1, 0, 0);

		VDP_GFX_SetInReso			   (VDP_LAYER_GFX0, stHdrCfg->stGfxReso);
		VDP_GFX_SetVideoPos 		   (VDP_LAYER_GFX0, stHdrCfg->stGfxReso);
		VDP_GFX_SetDispPos			   (VDP_LAYER_GFX0, stHdrCfg->stGfxReso);

		VDP_GP_SetInReso			   (VDP_LAYER_GP0, stHdrCfg->stGfxReso);
		VDP_GP_SetOutReso			   (VDP_LAYER_GP0, stHdrCfg->stBlReso);
		VDP_GP_SetVideoPos			   (VDP_LAYER_GP0, stHdrCfg->stBlReso);
		VDP_GP_SetDispPos			   (VDP_LAYER_GP0, stHdrCfg->stBlReso);

		if((stHdrCfg->stGfxReso.u32Wth != stHdrCfg->stBlReso.u32Wth)
		|| (stHdrCfg->stGfxReso.u32Hgt != stHdrCfg->stBlReso.u32Hgt))
		{
			HI_U32 u64ZmeWth = stHdrCfg->stGfxReso.u32Wth;
			VDP_GP_SetZmeHfirOrder		   (VDP_LAYER_GP0, VDP_ZME_ORDER_VH);

			VDP_GP_SetZmeEnable 		   (VDP_LAYER_GP0, VDP_ZME_MODE_HOR, HI_TRUE);
			VDP_GP_SetZmeEnable 		   (VDP_LAYER_GP0, VDP_ZME_MODE_VER, HI_TRUE);
			VDP_GP_SetZmeFirEnable		   (VDP_LAYER_GP0, VDP_ZME_MODE_HOR, HI_FALSE);
			VDP_GP_SetZmeMidEnable		   (VDP_LAYER_GP0, VDP_ZME_MODE_HOR, HI_FALSE);
			VDP_GP_SetZmeHorRatio		   (VDP_LAYER_GP0, (u64ZmeWth*GZME_HPREC)/stHdrCfg->stBlReso.u32Wth);
			VDP_GP_SetZmePhase			   (VDP_LAYER_GP0, VDP_ZME_MODE_HOR, 0*GZME_HPREC);
			VDP_GP_SetZmeFirEnable		   (VDP_LAYER_GP0, VDP_ZME_MODE_VER, HI_FALSE);
			VDP_GP_SetZmeMidEnable		   (VDP_LAYER_GP0, VDP_ZME_MODE_VER, HI_FALSE);
			VDP_GP_SetZmeVerRatio		   (VDP_LAYER_GP0, (stHdrCfg->stGfxReso.u32Hgt*GZME_VPREC)/stHdrCfg->stBlReso.u32Hgt);
			VDP_GP_SetZmePhase			   (VDP_LAYER_GP0, VDP_ZME_MODE_VER, 0*GZME_VPREC);
		}


		VDP_GP_SetLayerGalpha		   (VDP_LAYER_GP0, 0xff);
#endif
return HI_SUCCESS;
	}


HI_VOID VDP_DRV_SetHisiGdmGfx709_2020Out(HI_VOID)
{
    HI_U32 u32Pre =1  ; 
    HI_U32 u32DcPre =1 ;
    
    HI_U32  u32Scale;
    HI_S32 m33Yuv2Rgb[3][3];
    
    VDP_BT2020_CFG_T  tc_bt2020_cfg; 


	#if 1
	

    HI_U32 g_degamma_en = 1;//stCfg -> degamm_en; 
    HI_U32 g_gamma_en   = 1;//stCfg -> gamm_en;
    HI_U32 gcvm_en      = 1;//stCfg -> gcvm_en;
    HI_U32 cvm_sel      = 0;//stCfg -> cvm_sel;
    //HI_U32 cvm_en       = 1;//stCfg -> cvm_en;
    HI_U32 gdm_en       = 1;//stCfg -> gdm_en;

    HI_U32 g_rgb2lms_en             ; 
	int16_t csc_coef[3][3]={{0},{0},{0}};
	HI_S32  csc_offset[3]={0};

	
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
#if 1//wangzheng
//****************-----gamm-----**********************
		g_gmm_pos[0]		   = 557056 ;//1792  ;//256;//mod by suyong //832;//16384;
		g_gmm_pos[1]		   = 2916352 ;//345856;//15616;//mod by suyong //21312;//16384*2;
		g_gmm_pos[2]		   = 14450688  ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
		g_gmm_pos[3]		   = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
		g_gmm_step[0]		   = 15;//4;//mod by suyong //5;//10;
		g_gmm_step[1]		   = 17;//16;//10;//mod by suyong //11;//10;
		g_gmm_step[2]		   = 19;//18;//16;//mod by suyong //16;//10;
		g_gmm_step[3]		   = 21;//20;//mod by suyong //21;//10;
		g_gmm_num[0]		   = 17;//16;//mod by suyong //26;//16;
		g_gmm_num[1]		   = 18;//15;//mod by suyong //10;//16;
		g_gmm_num[2]		   = 22;//20;//mod by suyong //15;//16;
		g_gmm_num[3]		   = 7;//13;//mod by suyong //12;//16;
//****************-----degamm end-----**********************
#endif
#if 0//苏勇最新提供20160105
//****************-----gamm-----**********************
		g_gmm_pos[0]		   = 557056 ;//1792  ;//256;//mod by suyong //832;//16384;
		g_gmm_pos[1]		   = 2916352 ;//345856;//15616;//mod by suyong //21312;//16384*2;
		g_gmm_pos[2]		   = 14450688  ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
		g_gmm_pos[3]		   = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
		g_gmm_step[0]		   = 15;//4;//mod by suyong //5;//10;
		g_gmm_step[1]		   = 17;//16;//10;//mod by suyong //11;//10;
		g_gmm_step[2]		   = 19;//18;//16;//mod by suyong //16;//10;
		g_gmm_step[3]		   = 21;//20;//mod by suyong //21;//10;
		g_gmm_num[0]		   = 17;//16;//mod by suyong //26;//16;
		g_gmm_num[1]		   = 18;//15;//mod by suyong //10;//16;
		g_gmm_num[2]		   = 22;//20;//mod by suyong //15;//16;
		g_gmm_num[3]		   = 7;//13;//mod by suyong //12;//16;
//****************-----degamm end-----**********************
#endif

#endif


    }
    
        
                                    
    VDP_HDR_SetGDmEn(gdm_en);
    VDP_HDR_SetGDmDegammaEn           ( g_degamma_en);
    VDP_HDR_SetGDmRgb2lmsEn           ( g_rgb2lms_en             ) ;


	csc_coef[0][0]	   = 10279;
	csc_coef[0][1]	   = 5396;
	csc_coef[0][2]	   = 710;
	csc_coef[1][0]	   = 1134;
	csc_coef[1][1]	   = 15064;
	csc_coef[1][2]	   = 187;
	csc_coef[2][0]	   = 268;
	csc_coef[2][1]	   = 1442;
	csc_coef[2][2]	   = 14673;

	csc_offset[0]=0;	
	csc_offset[1]=0;	
	csc_offset[2]=0;

	
    VDP_HDR_SetGDmImapRgb2lms         ( csc_coef     ) ;
    VDP_HDR_SetGDmImapRgb2lmsscale2p  ( 14    ) ;
	VDP_HDR_SetGDmImapRgb2lmsmin          ( 0 );
	//VDP_HDR_SetGDmImapRgb2lmsmax          ( ((1<<25)  -1)       );
	VDP_HDR_SetGDmImapRgb2lmsmax          ( 26214400       );
    VDP_HDR_SetGDmImapRgb2lmsOutDc    ( csc_offset ) ;


#if 0
	csc_coef[0][0]	   = 3685;
	csc_coef[0][1]	   = 9512;
	csc_coef[0][2]	   = 832;
	csc_coef[1][0]	   = -2004;
	csc_coef[1][1]	   = -5171;
	csc_coef[1][2]	   = 7175;
	csc_coef[2][0]	   = 7175;
	csc_coef[2][1]	   = -6598;
	csc_coef[2][2]	   = -577;

	csc_offset[0]=2048*2;	
	csc_offset[1]=16384*2;	
	csc_offset[2]=16384*2;

	
    VDP_HDR_SetGCvmLms2IptEn          ( 1             ) ;
    VDP_HDR_SetGCvmLms2Ipt            ( csc_coef     ) ;
    VDP_HDR_SetGCvmLms2IptScale2p     ( 14    ) ;
    VDP_HDR_SetGCvmLms2IptOutDc       ( csc_offset ) ;
	VDP_HDR_SetGCvmLms2IptMin	   (0);//	 (-((1<<15)));
	VDP_HDR_SetGCvmLms2IptMax		  (32767);
#endif

#if  1//Full rgb -> Full yuv out
	
		   VDP_HDR_SetCvmEn 			 (true);
		   VDP_HDR_SetGCvmRgb2yuvEn 	 (true);
		   
	
		   u32Pre = 1 << 10;
		   u32DcPre = 4 * 32 ;//1:8bit; 4:10bit
		  
		  tc_bt2020_cfg.v0_csc_cfg.m33Yuv2RgbScale2P	=  15;	   //sGetRandEx(5,31);		   
		  
		  u32Scale	  = 1 << (tc_bt2020_cfg.v0_csc_cfg.m33Yuv2RgbScale2P - 10 );
		  u32DcPre	  = 4 * 64;
		  
		  
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2RgbScale2P	= 15;  //sGetRandEx(5,31);		   
#if 1
#if 1
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][0] 	= (HI_S32)( 0.2249* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][1] 	= (HI_S32)( 0.5806* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][2] 	= (HI_S32)( 0.0508* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][0] 	= (HI_S32)(-0.1223* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][1] 	= (HI_S32)(-0.3156* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][2] 	= (HI_S32)( 0.4379* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][0] 	= (HI_S32)( 0.4379* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][1] 	= (HI_S32)( -0.4027* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][2] 	= (HI_S32)(-0.0352* u32Pre*u32Scale/2);
#endif
	
		  
		  
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[0] = (HI_S32)(16   * u32DcPre/2);	
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[1] = (HI_S32)(128 * u32DcPre/2);	
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[2] = (HI_S32)(128 * u32DcPre/2);	
#endif
	
		  
		  
		  m33Yuv2Rgb[0][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][0];
		  m33Yuv2Rgb[0][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][1];
		  m33Yuv2Rgb[0][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][2];
		  
		  m33Yuv2Rgb[1][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][0];
		  m33Yuv2Rgb[1][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][1];
		  m33Yuv2Rgb[1][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][2];
		  
		  m33Yuv2Rgb[2][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][0];
		  m33Yuv2Rgb[2][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][1];
		  m33Yuv2Rgb[2][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][2];
		  VDP_HDR_SetGCvmOmapRgb2yuvscale2p(tc_bt2020_cfg.v1_csc_cfg.m33Yuv2RgbScale2P | (1<<5) );
		  VDP_HDR_SetGCvmOmapRgb2yuv	   (m33Yuv2Rgb	);
		  VDP_HDR_SetGCvmOmapRgb2yuvOutDc  (tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb );
		  VDP_HDR_SetGCvmOmapRgb2yuvmin    (0);
		  VDP_HDR_SetGCvmOmapRgb2yuvmax    (0x7FFF);
#endif




    VDP_HDR_SetGDmDegmmPos            ( g_degmm_pos);
    VDP_HDR_SetGDmDegmmStep           ( g_degmm_step);
    VDP_HDR_SetGDmDegmmNum            ( g_degmm_num);
    VDP_HDR_SetGDmGammaEn             ( g_gamma_en);
    VDP_HDR_SetGDmGammaPos            ( g_gmm_pos);
    VDP_HDR_SetGDmGammaStep           ( g_gmm_step);
    VDP_HDR_SetGDmGmmNum              ( g_gmm_num);
//  gcvm ------
    VDP_HDR_SetAuthEn                 ( gcvm_en);
    VDP_HDR_SetCvmSel                 ( cvm_sel);
    //VDP_HDR_SetCvmEn                  ( cvm_en);

}



HI_S32 VDP_DRV_SetHisiGfxMode(VDP_HISI_HDR_CFG_S  *stHdrCfg, HI_PQ_HDR_CFG  *pstDmKs)
{

	//enable
	HI_U32 bl_en             = 1;
	HI_U32 edr_ds_en         = 0;


	//cvm1:full spec
	HI_U32 cvm_sel           = 0;//0: vid select cvm1; 1: gfx select cvm1


	HI_S16 csc_m3[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
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
	//HDR10 OUTPUT
	//----------------------------------------------------------------------
	 if(stHdrCfg->enHdrMode == VDP_HIHDR_HDR10_IN_HDR10_OUT)
	{
		bl_en                = 0;
		cvm_sel              = 0;
	    edr_ds_en            = 0;
	}



	VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, VDP_LAYER_VID0,1);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, VDP_LAYER_VID1,0);	

	//VDP_DRV_SetHisiVdm                 (pstDmKs);
	VDP_HDR_SetVDmEn               (1);

	VDP_DRV_SetGdmDegammaCoef(gdm_degmm_lut);
	
	VDP_DRV_SetGdmgammaCoef(gdm_gmm_rgb2bt2020lut);
	

	

	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_DEGMM);
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_GMM);


	VDP_GFX_SetLayerEnable         (VDP_LAYER_GFX0, stHdrCfg->bGfxEn);
	if(stHdrCfg->bGfxEn)
	{
		VDP_DRV_SetHisiBt2020GfxBaseSetting(stHdrCfg);
	}


	//VDP_DRV_SetHisiVidBaseSetting(stHdrCfg);




	if(stHdrCfg->bPreMult == HI_FALSE)
	{
		
		VDP_GFX_SetPreMultEnable(VDP_LAYER_GFX0,HI_FALSE);
		VDP_GFX_MixvBypassEn		   (1);
		VDP_GFX_MixvPremultEn		   (0);
	}


	//----------------------------------------------------------------------
	//data path setting
	//----------------------------------------------------------------------
	VDP_HDR_SetCvmSel                     ( cvm_sel);

	if(stHdrCfg->enHdrMode == VDP_HIHDR_HDR10_IN_HDR10_OUT)
	{

		
		VDP_HDR_SetHdrEnable				  ( 0 );
		VDP_VID_SetZmeOutFmt		   (VDP_LAYER_VID0, VDP_PROC_FMT_SP_422);

		if(stHdrCfg->bGfxEn)
		{
			VDP_DRV_SetHisiGdmGfx709_2020Out();
		}

		
	}
	
	
    

	return HI_SUCCESS;

}

HI_VOID VDP_DRV_SetHisiGdm709_2020(HI_VOID)
{


	#if 1
	

    HI_U32 g_degamma_en = 1;//stCfg -> degamm_en; 
    HI_U32 g_gamma_en   = 1;//stCfg -> gamm_en;
    HI_U32 gcvm_en      = 1;//stCfg -> gcvm_en;
    HI_U32 cvm_sel      = 0;//stCfg -> cvm_sel;
    //HI_U32 cvm_en       = 1;//stCfg -> cvm_en;
    HI_U32 gdm_en       = 1;//stCfg -> gdm_en;

    HI_U32 g_rgb2lms_en             ; 

	int16_t csc_coef[3][3]={{0},{0},{0}};
	HI_S32  csc_offset[3]={0};
    HI_U32 u32Pre   =1; 
    HI_U32 u32DcPre  =1;
    
    HI_U32  u32Scale=1;
    HI_S32 m33Yuv2Rgb[3][3]={{0},{0},{0}};
    
    VDP_BT2020_CFG_T  tc_bt2020_cfg={0}; 

	
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

#if 1
//****************-----gamm-----**********************
		g_gmm_pos[0]		   = 557056 ;//1792  ;//256;//mod by suyong //832;//16384;
		g_gmm_pos[1]		   = 2916352 ;//345856;//15616;//mod by suyong //21312;//16384*2;
		g_gmm_pos[2]		   = 14450688  ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
		g_gmm_pos[3]		   = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
		g_gmm_step[0]		   = 15;//4;//mod by suyong //5;//10;
		g_gmm_step[1]		   = 17;//16;//10;//mod by suyong //11;//10;
		g_gmm_step[2]		   = 19;//18;//16;//mod by suyong //16;//10;
		g_gmm_step[3]		   = 21;//20;//mod by suyong //21;//10;
		g_gmm_num[0]		   = 17;//16;//mod by suyong //26;//16;
		g_gmm_num[1]		   = 18;//15;//mod by suyong //10;//16;
		g_gmm_num[2]		   = 22;//20;//mod by suyong //15;//16;
		g_gmm_num[3]		   = 7;//13;//mod by suyong //12;//16;
//****************-----degamm end-----**********************
#endif


    }
    
        
                                    
    VDP_HDR_SetGDmEn(gdm_en);
    VDP_HDR_SetGDmDegammaEn           ( g_degamma_en);
    VDP_HDR_SetGDmRgb2lmsEn           ( g_rgb2lms_en             ) ;


	csc_coef[0][0]	   = 10279;
	csc_coef[0][1]	   = 5396;
	csc_coef[0][2]	   = 710;
	csc_coef[1][0]	   = 1134;
	csc_coef[1][1]	   = 15064;
	csc_coef[1][2]	   = 187;
	csc_coef[2][0]	   = 268;
	csc_coef[2][1]	   = 1442;
	csc_coef[2][2]	   = 14673;

	csc_offset[0]=0;	
	csc_offset[1]=0;	
	csc_offset[2]=0;

	
    VDP_HDR_SetGDmImapRgb2lms         ( csc_coef     ) ;
    VDP_HDR_SetGDmImapRgb2lmsscale2p  ( 14    ) ;
	VDP_HDR_SetGDmImapRgb2lmsmin          ( 0 );
	//VDP_HDR_SetGDmImapRgb2lmsmax          ( ((1<<25)  -1)       );
	VDP_HDR_SetGDmImapRgb2lmsmax          ( 26214400       );
    VDP_HDR_SetGDmImapRgb2lmsOutDc    ( csc_offset ) ;


#if 0
	csc_coef[0][0]	   = 3685;
	csc_coef[0][1]	   = 9512;
	csc_coef[0][2]	   = 832;
	csc_coef[1][0]	   = -2004;
	csc_coef[1][1]	   = -5171;
	csc_coef[1][2]	   = 7175;
	csc_coef[2][0]	   = 7175;
	csc_coef[2][1]	   = -6598;
	csc_coef[2][2]	   = -577;

	csc_offset[0]=2048*2;	
	csc_offset[1]=16384*2;	
	csc_offset[2]=16384*2;

	
    VDP_HDR_SetGCvmLms2IptEn          ( 1             ) ;
    VDP_HDR_SetGCvmLms2Ipt            ( csc_coef     ) ;
    VDP_HDR_SetGCvmLms2IptScale2p     ( 14    ) ;
    VDP_HDR_SetGCvmLms2IptOutDc       ( csc_offset ) ;
	VDP_HDR_SetGCvmLms2IptMin	   (0);//	 (-((1<<15)));
	VDP_HDR_SetGCvmLms2IptMax		  (32767);
#endif

#if  1//Full rgb -> Full yuv out
	
		   VDP_HDR_SetCvmEn 			 (true);
		   VDP_HDR_SetGCvmRgb2yuvEn 	 (true);
		   
	
		   u32Pre = 1 << 10;
		   u32DcPre = 4 * 32 ;//1:8bit; 4:10bit
		  
		  tc_bt2020_cfg.v0_csc_cfg.m33Yuv2RgbScale2P	=  15;	   //sGetRandEx(5,31);		   
		  
		  u32Scale	  = 1 << (tc_bt2020_cfg.v0_csc_cfg.m33Yuv2RgbScale2P - 10 );
		  u32DcPre	  = 4 * 64;
		  
		  
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2RgbScale2P	= 15;  //sGetRandEx(5,31);		   
#if 1
#if 1
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][0] 	= (HI_S32)( 0.213* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][1] 	= (HI_S32)( 0.715* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][2] 	= (HI_S32)( 0.072* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][0] 	= (HI_S32)(-0.117* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][1] 	= (HI_S32)(-0.394* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][2] 	= (HI_S32)( 0.511* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][0] 	= (HI_S32)( 0.511* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][1] 	= (HI_S32)(-0.464* u32Pre*u32Scale/2);
		  tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][2] 	= (HI_S32)(-0.047* u32Pre*u32Scale/2);
#endif
	
		  
		  
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[0] = (HI_S32)(0   * u32DcPre/2);	
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[1] = (HI_S32)(128 * u32DcPre/2);	
		  tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb[2] = (HI_S32)(128 * u32DcPre/2);	
#endif
	
		  
		  
		  m33Yuv2Rgb[0][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][0];
		  m33Yuv2Rgb[0][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][1];
		  m33Yuv2Rgb[0][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[0][2];
		  
		  m33Yuv2Rgb[1][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][0];
		  m33Yuv2Rgb[1][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][1];
		  m33Yuv2Rgb[1][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[1][2];
		  
		  m33Yuv2Rgb[2][0] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][0];
		  m33Yuv2Rgb[2][1] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][1];
		  m33Yuv2Rgb[2][2] = tc_bt2020_cfg.v1_csc_cfg.m33Yuv2Rgb[2][2];
		  VDP_HDR_SetGCvmOmapRgb2yuvscale2p(tc_bt2020_cfg.v1_csc_cfg.m33Yuv2RgbScale2P | (1<<5) );
		  VDP_HDR_SetGCvmOmapRgb2yuv	   (m33Yuv2Rgb	);
		  VDP_HDR_SetGCvmOmapRgb2yuvOutDc  (tc_bt2020_cfg.v1_csc_cfg.v3Yuv2RgbOffInRgb );
		  VDP_HDR_SetGCvmOmapRgb2yuvmin    (0);
		  VDP_HDR_SetGCvmOmapRgb2yuvmax    (0x7FFF);
#endif




    VDP_HDR_SetGDmDegmmPos            ( g_degmm_pos);
    VDP_HDR_SetGDmDegmmStep           ( g_degmm_step);
    VDP_HDR_SetGDmDegmmNum            ( g_degmm_num);
    VDP_HDR_SetGDmGammaEn             ( g_gamma_en);
    VDP_HDR_SetGDmGammaPos            ( g_gmm_pos);
    VDP_HDR_SetGDmGammaStep           ( g_gmm_step);
    VDP_HDR_SetGDmGmmNum              ( g_gmm_num);
//  gcvm ------
    VDP_HDR_SetAuthEn                 ( gcvm_en);
    VDP_HDR_SetCvmSel                 ( cvm_sel);
    //VDP_HDR_SetCvmEn                  ( cvm_en);
#endif

}


HI_VOID VDP_DRV_SetHisiHlgDegammaCoef (HI_U32 *u32Degamma,HI_U32 u32Length)
{
#if VDP_CBB_FPGA
    HI_U8* input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA] ;//add by hyx
	HI_U8* output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA];//add by hyx
#elif EDA_TEST
    HI_U8* input_addr = pVdpReg->VDM_COEFAD.bits.vdm_coef_addr ;//need modify when copy to it
    HI_U8* ioutput_addr = pVdpReg->CVM1_DEGAMMA_COEFAD.bits.degamma_coef_addr ;//need modify when copy to it
#else
    HI_U32 input_addr = 0 ;
    HI_U32 output_addr = 0 ;
#endif

    VDP_COEF_SEND_CFG stCoefSend;

    void* p_coef_array[1] ;
    HI_U32 lut_length[1] ;
    HI_U32 coef_bit_length[1] ;

    FILE* fp_dm_input_degmm_coef = NULL;
    FILE* fp_dm_output_degmm_coef=NULL;    

    VDP_HDR_SetVDmDegmmCoefAddr (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA]);
	VDP_HDR_SetVCvmDegmmCoefAddr(gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA]);
#if 0
    p_coef_array[0] =HisiHdr10_1024Lut;//(给hdr10进sdr出的表)// HisiHdr10Degamma(给sdr进sdr出用的表);
    lut_length[0] = 1024 ;
    coef_bit_length[0] = 32 ;
#endif
#if 1//sdr bt2020->709
		p_coef_array[0] =u32Degamma;//u32Degamma;//(给hdr10进sdr出的表)// HisiHdr10Degamma(给sdr进sdr出用的表);
		lut_length[0] = u32Length;//u32Length ;
		coef_bit_length[0] = 32 ;
#endif

    stCoefSend.coef_addr = input_addr ;
    stCoefSend.sti_type = STI_FILE_COEF_DM ;
    stCoefSend.fp_coef = fp_dm_input_degmm_coef ;
    stCoefSend.lut_num = 1 ;
    stCoefSend.burst_num = 1 ;
    stCoefSend.cycle_num = 4 ;
    stCoefSend.p_coef_array = p_coef_array ;
    stCoefSend.lut_length = lut_length ;
    stCoefSend.coef_bit_length = coef_bit_length ;
    stCoefSend.data_type = COEF_DATA_TYPE_S32 ;

    VDP_FUNC_SendCoef(&stCoefSend);
#if 1
    //output degamma coef send
    p_coef_array[0]    = (HI_S32*)HisiHlgOutDegamma_1024Lut;//输出的degamma王正提供新表替换掉
    lut_length[0]      = 1024                ; 
    coef_bit_length[0] = 32                  ; 

    stCoefSend.coef_addr         = output_addr                ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM           ; 
    stCoefSend.fp_coef           = fp_dm_output_degmm_coef    ; 
    stCoefSend.lut_num           = 1                          ; 
    stCoefSend.burst_num         = 1                          ; 
    stCoefSend.cycle_num         = 4                          ; 
    stCoefSend.p_coef_array      = p_coef_array               ; 
    stCoefSend.lut_length        = lut_length                 ; 
    stCoefSend.coef_bit_length   = coef_bit_length            ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 
    VDP_FUNC_SendCoef(&stCoefSend);
#endif
	VDP_HDR_SetDmInputDegmmParaup  ();
	VDP_HDR_SetDmOutputDegmmParaup ();
	return;
}


HI_VOID VDP_DRV_SetHisiDegammaCoef (HI_U32 *u32Degamma,HI_U32 u32Length)
{
#if VDP_CBB_FPGA
    HI_U8* input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA] ;//add by hyx
	HI_U8* output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA];//add by hyx
#elif EDA_TEST
    HI_U8* input_addr = pVdpReg->VDM_COEFAD.bits.vdm_coef_addr ;//need modify when copy to it
    HI_U8* ioutput_addr = pVdpReg->CVM1_DEGAMMA_COEFAD.bits.degamma_coef_addr ;//need modify when copy to it
#else
    HI_U32 input_addr = 0 ;
    HI_U32 output_addr = 0 ;
#endif

    VDP_COEF_SEND_CFG stCoefSend;

    void* p_coef_array[1] ;
    HI_U32 lut_length[1] ;
    HI_U32 coef_bit_length[1] ;

    FILE* fp_dm_input_degmm_coef = NULL;
    FILE* fp_dm_output_degmm_coef=NULL;    

    VDP_HDR_SetVDmDegmmCoefAddr (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA]);
	VDP_HDR_SetVCvmDegmmCoefAddr(gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA]);
#if 0
    p_coef_array[0] =HisiHdr10_1024Lut;//
    lut_length[0] = 1024 ;
    coef_bit_length[0] = 32 ;
#endif
#if 1//sdr bt2020->709
		p_coef_array[0] =u32Degamma;//u32Degamma;//
		lut_length[0] = u32Length;//u32Length ;
		coef_bit_length[0] = 32 ;
#endif

    stCoefSend.coef_addr = input_addr ;
    stCoefSend.sti_type = STI_FILE_COEF_DM ;
    stCoefSend.fp_coef = fp_dm_input_degmm_coef ;
    stCoefSend.lut_num = 1 ;
    stCoefSend.burst_num = 1 ;
    stCoefSend.cycle_num = 4 ;
    stCoefSend.p_coef_array = p_coef_array ;
    stCoefSend.lut_length = lut_length ;
    stCoefSend.coef_bit_length = coef_bit_length ;
    stCoefSend.data_type = COEF_DATA_TYPE_S32 ;

    VDP_FUNC_SendCoef(&stCoefSend);
#if 1
    //output degamma coef send
    p_coef_array[0]    = (HI_S32*)HisiHdr10deSLF_1024Lut;//输出的degamma王正提供新表替换掉
    lut_length[0]      = 1024                ; 
    coef_bit_length[0] = 32                  ; 

    stCoefSend.coef_addr         = output_addr                ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM           ; 
    stCoefSend.fp_coef           = fp_dm_output_degmm_coef    ; 
    stCoefSend.lut_num           = 1                          ; 
    stCoefSend.burst_num         = 1                          ; 
    stCoefSend.cycle_num         = 4                          ; 
    stCoefSend.p_coef_array      = p_coef_array               ; 
    stCoefSend.lut_length        = lut_length                 ; 
    stCoefSend.coef_bit_length   = coef_bit_length            ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 
    VDP_FUNC_SendCoef(&stCoefSend);
#endif
	VDP_HDR_SetDmInputDegmmParaup  ();
	VDP_HDR_SetDmOutputDegmmParaup ();
	return;
}


HI_VOID VDP_DRV_SetHisiHlgGammaCoef(HI_U32 *u32IngammaLutX,HI_U32 *u32IngammaLutA,HI_U32 *u32IngammaLutB,
HI_U32 *u32OutgammaLutX,HI_U32 *u32OutgammaLutA,HI_U32 *u32OutgammaLutB	)
{
#if FPGA_TEST
    HI_U8* input_addr = 0 ;
    HI_U8* output_addr = 0 ;
#elif EDA_TEST
    HI_U8* input_addr  = pVdpReg->VDM_COEFAD1.bits.vdm_coef_addr1 ;//need modify when copy to it
    HI_U8* output_addr = pVdpReg->CVM1_GAMMA_COEFAD.bits.gamma_coef_addr ;//need modify when copy to it
#endif
    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;

    //void* p_coef_array_l2g []     = {l2gLutX, l2gLutA, l2gLutB};
   // void* p_coef_array_l2pq []    = {HisiHdr10l2pqLutX, HisiHdr10l2pqLutA, HisiHdr10l2pqLutB};
	 void* p_coef_array_l2pq []	 = {u32IngammaLutX, u32IngammaLutA, u32IngammaLutB};
    void* p_coef_array_l2g []    = {u32OutgammaLutX, u32OutgammaLutA, u32OutgammaLutB};


	
    void** p_coef_array;

    HI_U32 lut_length[]     = {128, 128, 128};
    HI_U32 coef_bit_length[] = {32, 28, 16};

    FILE* fp_dm_input_gmm_coef = NULL;
    FILE* fp_dm_output_gmm_coef = NULL;
#if VDP_CBB_FPGA
    input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA] ;//add by hyx
	output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA] ;//add by hyx
#else
    input_addr = 0;
    output_addr = 0;
#endif

    VDP_HDR_SetVDmGammaCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA] );
	VDP_HDR_SetVCvmGammaCoefAddr   (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_GAMMA] );

    //send coef input gamma
    stCoefSend.coef_addr         = input_addr               ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ;
    stCoefSend.fp_coef           = fp_dm_input_gmm_coef     ;
    stCoefSend.lut_num           = 3                        ;
    stCoefSend.burst_num         = 1                        ;
    stCoefSend.cycle_num         = 1                        ;
    stCoefSend.p_coef_array      = p_coef_array_l2pq        ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;
    VDP_FUNC_SendCoef(&stCoefSend);

#if 1
    //send coef output gamma
    p_coef_array = p_coef_array_l2g;

    stCoefSend.coef_addr         = output_addr              ;
    stCoefSend.fp_coef           = fp_dm_output_gmm_coef    ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
#endif
    VDP_HDR_SetDmInputGammaParaup  ();
    VDP_HDR_SetDmOutputGammaParaup ();
	return;
}


HI_VOID VDP_DRV_SetHisigammaCoef(HI_U32 *u32L2GLutX,HI_U32 *u32L2GLutA,HI_U32 *u32L2GLutB)
{
#if FPGA_TEST
    HI_U8* input_addr = 0 ;
    HI_U8* output_addr = 0 ;
#elif EDA_TEST
    HI_U8* input_addr  = pVdpReg->VDM_COEFAD1.bits.vdm_coef_addr1 ;//need modify when copy to it
    HI_U8* output_addr = pVdpReg->CVM1_GAMMA_COEFAD.bits.gamma_coef_addr ;//need modify when copy to it
#endif
    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;

    //void* p_coef_array_l2g []     = {l2gLutX, l2gLutA, l2gLutB};
   // void* p_coef_array_l2pq []    = {HisiHdr10l2pqLutX, HisiHdr10l2pqLutA, HisiHdr10l2pqLutB};
	 void* p_coef_array_l2pq []	 = {u32L2GLutX, u32L2GLutA, u32L2GLutB};
    void* p_coef_array_l2g []    = {HisiHdr10GammaLutX, HisiHdr10GammaLutA, HisiHdr10GammaLutB};


	
    void** p_coef_array;

    HI_U32 lut_length[]     = {128, 128, 128};
    HI_U32 coef_bit_length[] = {32, 28, 16};

    FILE* fp_dm_input_gmm_coef = NULL;
    FILE* fp_dm_output_gmm_coef = NULL;
#if VDP_CBB_FPGA
    input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA] ;//add by hyx
	output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA] ;//add by hyx
#else
    input_addr = 0;
    output_addr = 0;
#endif

    VDP_HDR_SetVDmGammaCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA] );
	VDP_HDR_SetVCvmGammaCoefAddr   (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_GAMMA] );

    //send coef input gamma
    stCoefSend.coef_addr         = input_addr               ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ;
    stCoefSend.fp_coef           = fp_dm_input_gmm_coef     ;
    stCoefSend.lut_num           = 3                        ;
    stCoefSend.burst_num         = 1                        ;
    stCoefSend.cycle_num         = 1                        ;
    stCoefSend.p_coef_array      = p_coef_array_l2pq        ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;
    VDP_FUNC_SendCoef(&stCoefSend);

#if 1
    //send coef output gamma
    p_coef_array = p_coef_array_l2g;

    stCoefSend.coef_addr         = output_addr              ;
    stCoefSend.fp_coef           = fp_dm_output_gmm_coef    ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
#endif
    VDP_HDR_SetDmInputGammaParaup  ();
    VDP_HDR_SetDmOutputGammaParaup ();
}

HI_S32 VDP_DRV_SetHisiDmCvmCoef(HI_S16 * ptmLutI, HI_S16 * ptmLutS, HI_S16 * psmLutI, HI_S16 * psmLutS)
{
    void* p_coef_array[]    = {ptmLutI, psmLutI, ptmLutS, psmLutS};
    static HI_U32 lut_length[]     = {512,512,512,512};
    static HI_U32 coef_bit_length[]= {16,16,16,16};
    static VDP_COEF_SEND_CFG   stCoefSend;

    FILE* fp_dm_cvm_coef = NULL;
    //VDP_FOPEN(fp_dm_cvm_coef, "./golden/dm_cvm_coef.dat", VDP_FILE_WB);

#if EDA_TEST
    HI_U32 addr = pVdpReg->CVM1_VCVM_COEFAD.bits.vcvm_coef_addr;//
#endif
#if VDP_CBB_FPGA
	HI_U8  * addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_CVM] ;//add by hyx
#else
	HI_U8  * addr ;//add by hyx

#endif

	VDP_HDR_SetVCvmVcvmCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_CVM]);
	
    stCoefSend.coef_addr         = addr                     ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ; 
    stCoefSend.fp_coef           = fp_dm_cvm_coef           ; 
    stCoefSend.lut_num           = 4                        ; 
    stCoefSend.burst_num         = 1                        ; 
    stCoefSend.cycle_num         = 2                        ; 
    stCoefSend.p_coef_array      = p_coef_array             ; 
    stCoefSend.lut_length        = lut_length               ; 
    stCoefSend.coef_bit_length   = coef_bit_length          ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S16       ; 

    VDP_FUNC_SendCoef(&stCoefSend);
	VDP_HDR_SetDmCvmParaup		   ();

    //VDP_FCLOSE(fp_dm_cvm_coef);

    return HI_SUCCESS;
}

 HI_VOID VDP_DRV_SetGfxRgbtYuvMode(VDP_CSC_MODE_E enVdpCscMode)
 {
	 HI_U32 u32Pre ; 
	 HI_U32 u32DcPre ;
	 
	 HI_U32 u32Scale;
	 HI_S32 csc_coef[3][3]={{0},{0},{0}};
	 HI_S32 cscDc[3]={0};
	 
	 
	 HI_S16 m33Yuv2RgbScale2P=15;
	 
	 VDP_HDR_SetCvmEn (true);
	 VDP_HDR_SetGCvmRgb2yuvEn (true);
	 
	 
	 
	 u32Pre = 1 << 10;
	 m33Yuv2RgbScale2P = 15; //sGetRandEx(5,31); 
	 u32Scale = 1 << (m33Yuv2RgbScale2P - 10 );
	 u32DcPre = 4 * 64;
	 
	 
	 
	 
	 if(enVdpCscMode == VDP_CSC_RGB2YUV_2020_2020_L)
	 {
		 m33Yuv2RgbScale2P = 15; 
		 csc_coef[0][0] = (HI_S32)( 0.2249* u32Pre*u32Scale/2);
		 csc_coef[0][1] = (HI_S32)( 0.5806* u32Pre*u32Scale/2);
		 csc_coef[0][2] = (HI_S32)( 0.0508* u32Pre*u32Scale/2);
		 csc_coef[1][0] = (HI_S32)(-0.1223* u32Pre*u32Scale/2);
		 csc_coef[1][1] = (HI_S32)(-0.3156* u32Pre*u32Scale/2);
		 csc_coef[1][2] = (HI_S32)( 0.4379* u32Pre*u32Scale/2);
		 csc_coef[2][0] = (HI_S32)( 0.4379* u32Pre*u32Scale/2);
		 csc_coef[2][1] = (HI_S32)( -0.4027* u32Pre*u32Scale/2);
		 csc_coef[2][2] = (HI_S32)(-0.0352* u32Pre*u32Scale/2);
		 cscDc[0] = (HI_S32)(16 * u32DcPre/2); 
		 cscDc[1] = (HI_S32)(128 * u32DcPre/2); 
		 cscDc[2] = (HI_S32)(128 * u32DcPre/2); 
	 }
	 else if(enVdpCscMode == VDP_CSC_RGB2YUV_709)
	 {
		 m33Yuv2RgbScale2P = 15; //sGetRandEx(5,31); 
		 csc_coef[0][0] = (HI_S32)( 0.213* u32Pre*u32Scale/2);
		 csc_coef[0][1] = (HI_S32)( 0.715* u32Pre*u32Scale/2);
		 csc_coef[0][2] = (HI_S32)( 0.072* u32Pre*u32Scale/2);
		 csc_coef[1][0] = (HI_S32)(-0.117* u32Pre*u32Scale/2);
		 csc_coef[1][1] = (HI_S32)(-0.394* u32Pre*u32Scale/2);
		 csc_coef[1][2] = (HI_S32)( 0.511* u32Pre*u32Scale/2);
		 csc_coef[2][0] = (HI_S32)( 0.511* u32Pre*u32Scale/2);
		 csc_coef[2][1] = (HI_S32)(-0.464* u32Pre*u32Scale/2);
		 csc_coef[2][2] = (HI_S32)(-0.047* u32Pre*u32Scale/2);
		 cscDc[0] = (HI_S32)(16 * u32DcPre/2); 
		 cscDc[1] = (HI_S32)(128 * u32DcPre/2); 
		 cscDc[2] = (HI_S32)(128 * u32DcPre/2); 
	 }
	 
	 VDP_HDR_SetGCvmOmapRgb2yuvscale2p(m33Yuv2RgbScale2P | (1<<5) );
	 VDP_HDR_SetGCvmOmapRgb2yuv (csc_coef );
	 VDP_HDR_SetGCvmOmapRgb2yuvOutDc (cscDc);
	 VDP_HDR_SetGCvmOmapRgb2yuvmin (0);
	 VDP_HDR_SetGCvmOmapRgb2yuvmax (0x7FFF);
	 
 }
 
 HI_VOID VDP_DRV_SetGfxBt2020Mode(HI_PQ_CSC_MODE_E enPqCscMode)
 {
	 HI_U32 g_degamma_en = 1;//stCfg -> degamm_en; 
	 HI_U32 g_gamma_en = 1;//stCfg -> gamm_en;
	 HI_U32 gcvm_en = 1;//stCfg -> gcvm_en;
	 HI_U32 cvm_sel = 0;//stCfg -> cvm_sel;
	 //HI_U32 cvm_en = 1;//stCfg -> cvm_en;
	 HI_U32 gdm_en = 1;//stCfg -> gdm_en;
	 
	 HI_U32 g_rgb2lms_en ; 
	 
	 int16_t csc_coef[3][3]={{0},{0},{0}};
	 HI_S32 csc_offset[3]={0};
	 
	 
	 //HI_U32 gamma_rd_en ; 
	 //HI_U32 degamma_rd_en ; 
	 //**************-----degamm and gamma---------------
	 HI_U32 g_degmm_step[4] ; 
	 HI_U32 g_degmm_pos[4] ; 
	 HI_U32 g_degmm_num[4] ; 
	 
	 HI_U32 g_gmm_step[4] ; 
	 HI_U32 g_gmm_pos[4] ; 
	 HI_U32 g_gmm_num[4] ; 
	 
	 HI_U32 g_rgb2yuv_en ;
	 
	 g_rgb2lms_en = 1;//stCfg -> g_rgb2lms_en;
	 g_rgb2yuv_en = 1;//stCfg -> g_rgb2yuv_en;
	 
	 
	 //****************-----degamm-----**********************
	 g_degmm_pos[0] = 64;
	 g_degmm_pos[1] = 64*2;
	 g_degmm_pos[2] = 64*3;
	 g_degmm_pos[3] = 252;//255;
	 g_degmm_step[0] = 2;
	 g_degmm_step[1] = 2;
	 g_degmm_step[2] = 2;
	 g_degmm_step[3] = 2;
	 g_degmm_num[0] = 16;
	 g_degmm_num[1] = 16;
	 g_degmm_num[2] = 16;
	 g_degmm_num[3] = 15;
	 //****************-----degamm end-----**********************
	 //****************-----gamm-----**********************
	 g_gmm_pos[0] = 557056 ;//1792 ;//256;//mod by suyong //832;//16384;
	 g_gmm_pos[1] = 2916352 ;//345856;//15616;//mod by suyong //21312;//16384*2;
	 g_gmm_pos[2] = 14450688 ;//4540160;//1326336;//mod by suyong //1004352;//16384*3;
	 g_gmm_pos[3] = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
	 g_gmm_step[0] = 15;//4;//mod by suyong //5;//10;
	 g_gmm_step[1] = 17;//16;//10;//mod by suyong //11;//10;
	 g_gmm_step[2] = 19;//18;//16;//mod by suyong //16;//10;
	 g_gmm_step[3] = 21;//20;//mod by suyong //21;//10;
	 g_gmm_num[0] = 17;//16;//mod by suyong //26;//16;
	 g_gmm_num[1] = 18;//15;//mod by suyong //10;//16;
	 g_gmm_num[2] = 22;//20;//mod by suyong //15;//16;
	 g_gmm_num[3] = 7;//13;//mod by suyong //12;//16;
	 //****************-----degamm end-----**********************

	 VDP_DRV_SetGdmDegammaCoef(gdm_degmm_lut);
	 VDP_DRV_SetGdmgammaCoef(gdm_gmm_rgb2bt2020lut);
	 
	 VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_DEGMM);
	 VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_GMM);


	 VDP_HDR_SetGDmEn(gdm_en);
	 VDP_HDR_SetGDmDegammaEn ( g_degamma_en);
	 VDP_HDR_SetGDmRgb2lmsEn ( g_rgb2lms_en ) ;
	 
	 
	 csc_coef[0][0] = 10279;
	 csc_coef[0][1] = 5396;
	 csc_coef[0][2] = 710;
	 csc_coef[1][0] = 1134;
	 csc_coef[1][1] = 15064;
	 csc_coef[1][2] = 187;
	 csc_coef[2][0] = 268;
	 csc_coef[2][1] = 1442;
	 csc_coef[2][2] = 14673;
	 
	 csc_offset[0]=0; 
	 csc_offset[1]=0; 
	 csc_offset[2]=0;
	 
	 
	 VDP_HDR_SetGDmImapRgb2lms ( csc_coef ) ;
	 VDP_HDR_SetGDmImapRgb2lmsscale2p ( 14 ) ;
	 VDP_HDR_SetGDmImapRgb2lmsmin ( 0 );
	 //VDP_HDR_SetGDmImapRgb2lmsmax ( ((1<<25) -1) );
	 VDP_HDR_SetGDmImapRgb2lmsmax ( 26214400 );
	 VDP_HDR_SetGDmImapRgb2lmsOutDc ( csc_offset ) ;
	 
	 
	 
	 
	 VDP_HDR_SetGDmDegmmPos ( g_degmm_pos);
	 VDP_HDR_SetGDmDegmmStep ( g_degmm_step);
	 VDP_HDR_SetGDmDegmmNum ( g_degmm_num);
	 VDP_HDR_SetGDmGammaEn ( g_gamma_en);
	 VDP_HDR_SetGDmGammaPos ( g_gmm_pos);
	 VDP_HDR_SetGDmGammaStep ( g_gmm_step);
	 VDP_HDR_SetGDmGmmNum ( g_gmm_num);
	 // gcvm ------
	 VDP_HDR_SetAuthEn ( gcvm_en);
	 VDP_HDR_SetCvmSel ( cvm_sel);
	 //VDP_HDR_SetCvmEn ( cvm_en);
 }




HI_VOID VDP_DRV_SetVidRgb2YuvMode(VDP_CSC_MODE_E enCscMode)
{
	HI_S16 m33Lms2Ipt[3][3] = {{0},{0},{0}};
	HI_S32 s32CscDc[3]={0};
	HI_U32 u32Pre	=1 ; 
	HI_U32 u32DcPre =1 ;
	HI_U32  u32Scale=1;

	
#if 1
	
	u32Pre = 1 << 10;
	u32DcPre = 4 * 32 ;//1:8bit; 4:10bit

	u32Scale	= 1 << (15 - 10);
	u32DcPre	= 4 * 64;
	if(enCscMode == VDP_CSC_RGB2YUV_709)
	{


		m33Lms2Ipt[0][0]	  =  (HI_S32)(0.1821  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][1]	  =  (HI_S32)(0.6124  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][2]	  =  (HI_S32)(0.0618  * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][0]	  =  (HI_S32)(-0.1003 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][1]	  =  (HI_S32)(-0.3376 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][2]	  =  (HI_S32)( 0.4379 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][0]	  =  (HI_S32)( 0.4379 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][1]	  =  (HI_S32)(-0.3978 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][2]	  =  (HI_S32)(-0.0402 * u32Pre*u32Scale/2);
		
		s32CscDc[0] =  (HI_S32)(16  * u32DcPre/2);	
		s32CscDc[1] =  (HI_S32)(128 * u32DcPre/2);	
		s32CscDc[2] =  (HI_S32)(128 * u32DcPre/2);	

	}
	else if(enCscMode == VDP_CSC_RGB2RGB)
	{
		m33Lms2Ipt[0][0]	  =  (HI_S32)(1  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][1]	  =  (HI_S32)(0  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][2]	  =  (HI_S32)(0  * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][0]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][1]	  =  (HI_S32)(1 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][2]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][0]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][1]	  =  (HI_S32)(0 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][2]	  =  (HI_S32)(1 * u32Pre*u32Scale/2);
		
		s32CscDc[0] = 0;// (HI_S32)(16  * u32DcPre/2);	
		s32CscDc[1] = 0;//  (HI_S32)(128 * u32DcPre/2);	
		s32CscDc[2] = 0;//  (HI_S32)(128 * u32DcPre/2);	
	}
	else
	{
		/*ERR  PROCESS*/
	}


#if 1
	
	VDP_HDR_SetVCvmImapLms2iptscale2p(15 | (1<<5) );
	VDP_HDR_SetVCvmImapLms2ipt		 (m33Lms2Ipt		 );
	VDP_HDR_SetVCvmImapLms2iptOutDc  (s32CscDc );
	VDP_HDR_SetVDmImapLms2iptmin	 (0);
	VDP_HDR_SetVDmImapLms2iptmax	 (0x7FFF);
	VDP_HDR_SetVCvmLms2iptEn		 ( 1 );
#endif
#if 0
	VDP_HDR_SetVCvmLms2iptEn(HI_TRUE);
	VDP_HDR_SetVCvmImapLms2ipt ( m33Lms2Ipt);
	////VDP_HDR_SetVCvmImapLms2iptscale2p ( 14 );
	VDP_HDR_SetVCvmImapLms2iptscale2p ( 12 );
	VDP_HDR_SetVCvmImapLms2iptOutDc(s32CscDc);
	VDP_HDR_SetGCvmLms2IptMin (0);;
	VDP_HDR_SetGCvmLms2IptMax (32767);
#endif
#endif
	//VDP_HDR_SetVDmNormEn(0);
}


HI_VOID VDP_DRV_SetVidBt2020Mode(HI_PQ_CSC_MODE_E enPqCscMode)
{
	VDP_BT2020_CFG_T bt2020_cfg;
	VDP_BT2020_CFG_T* pCfg = &bt2020_cfg;
    HI_S32 m33Dc[3]={0};
#if 0
	HI_S16 m33Lms2Ipt[3][3];
	HI_S32 s32CscDc[3]={0};
	HI_U32 u32Pre	=1 ; 
	HI_U32 u32DcPre =1 ;
	HI_U32  u32Scale=1;
#endif
	pCfg->v0_csc_cfg.m33Yuv2RgbScale2P	  = 13 ;  //wangzheng provide
	pCfg->gamma_cfg.m33Rgb2LmsScale2P	  = 14;    //wangzheng provide



if(enPqCscMode == HI_PQ_CSC_YUV2RGB_2020_709_L)
{
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][0]	  = 9567;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][1]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][2]	  = 13792;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][0]	  = 9567;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][1]	  = -1539;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][2]	  = -5344;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][0]	  = 9567;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][1]	  = 17597;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][2]	  = 0;
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[0] = 122423345;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[1] = -46449400;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[2] = 153501460;//TODo



	pCfg->gamma_cfg.m33Rgb2Lms[0][0]	  = 27209 ;
	pCfg->gamma_cfg.m33Rgb2Lms[0][1]	  = -9632 ;
	pCfg->gamma_cfg.m33Rgb2Lms[0][2]	  = -1194 ;
	pCfg->gamma_cfg.m33Rgb2Lms[1][0]	  = -2045 ;
	pCfg->gamma_cfg.m33Rgb2Lms[1][1]	  = 18565 ;
	pCfg->gamma_cfg.m33Rgb2Lms[1][2]	  = -138 ;	
	pCfg->gamma_cfg.m33Rgb2Lms[2][0]	  = -297 ;	
	pCfg->gamma_cfg.m33Rgb2Lms[2][1]	  = -1648 ;
	pCfg->gamma_cfg.m33Rgb2Lms[2][2]	  = 18330 ;

}
else if(enPqCscMode == HI_PQ_CSC_YUV2YUV_709_2020_L_L)
{
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][0]	  = 10279;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][1]	  = 5396;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][2]	  = 710;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][0]	  = 1134;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][1]	  = 15064;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][2]	  = 187;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][0]	  = 268;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][1]	  = 1442;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][2]	  = 14673;
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[0] = 0;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[1] = 0;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[2] = 0;//TODo

	pCfg->gamma_cfg.m33Rgb2Lms[0][0]	  = 10279;
	pCfg->gamma_cfg.m33Rgb2Lms[0][1]	  = 5396;
	pCfg->gamma_cfg.m33Rgb2Lms[0][2]	  = 710;
	pCfg->gamma_cfg.m33Rgb2Lms[1][0]	  = 1134;
	pCfg->gamma_cfg.m33Rgb2Lms[1][1]	  = 15064;
	pCfg->gamma_cfg.m33Rgb2Lms[1][2]	  = 187;
	pCfg->gamma_cfg.m33Rgb2Lms[2][0]	  = 268;
	pCfg->gamma_cfg.m33Rgb2Lms[2][1]	  = 1442;
	pCfg->gamma_cfg.m33Rgb2Lms[2][2]	  = 14673;


}
else if(enPqCscMode == HI_PQ_CSC_YUV2YUV_2020_2020_L_L)
{
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][0]	  = 1024;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][1]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[0][2]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][0]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][1]	  = 1024;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[1][2]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][0]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][1]	  = 0;
	pCfg->v0_csc_cfg.m33Yuv2Rgb[2][2]	  = 1024;
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[0] = 0;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[1] = 0;//TODo
	pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb[2] = 0;//TODo

	pCfg->gamma_cfg.m33Rgb2Lms[0][0]	  = 1024;
	pCfg->gamma_cfg.m33Rgb2Lms[0][1]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[0][2]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[1][0]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[1][1]	  = 1024;
	pCfg->gamma_cfg.m33Rgb2Lms[1][2]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[2][0]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[2][1]	  = 0;
	pCfg->gamma_cfg.m33Rgb2Lms[2][2]	  = 1024;

}

	pCfg->gamma_cfg.csc_min 			  = 0;	   //Todo
	pCfg->gamma_cfg.csc_max 			  = 2621440000u; //Todo
	//drv cfg

	VDP_DRV_SetHisiDegammaCoef(HisiHdr10Degamma,256);
	VDP_DRV_SetHisigammaCoef(HisiSdrBt2020_709LutX,HisiSdrBt2020_709LutA,HisiSdrBt2020_709LutB);

#if 1
	VDP_HDR_SetVDmEn				  (1  ) ;
	VDP_HDR_SetVDmInBits(1); //all see as 14bits
	//VDP_HDR_SetVCvmDenormEn(1);
	VDP_HDR_SetVDmInEotf(0);
	VDP_HDR_SetVDmYuv2rgbV0En		  (1)  ;
	VDP_HDR_SetVDmYuv2rgbV1En		  (1)  ;
    VDP_HDR_SetVDmGammaV0En           (1);
	VDP_HDR_SetVDmDegammaEn(1);
	VDP_HDR_SetVDmGammaEn(1);

	//yuv2rgb
	VDP_HDR_SetVDmYuv2rgbEn 			( 1 			) ;
	VDP_HDR_SetVDmImapYuv2rgbscale2p ( pCfg->v0_csc_cfg.m33Yuv2RgbScale2P );
	VDP_HDR_SetVDmImap0Yuv2rgb		 ( pCfg->v0_csc_cfg.m33Yuv2Rgb		  );
    VDP_HDR_SetVDmImapYuv2rgbInDc0   (m33Dc);
	VDP_HDR_SetVDmImapYuv2rgbOutDc0  ( pCfg->v0_csc_cfg.v3Yuv2RgbOffInRgb );
	VDP_HDR_SetVDmImapRange 		 ( 65535 );


	//VDM_RGB2LMS
	VDP_HDR_SetVDmRgb2lmsEn 		  ( 1			  )  ;
	VDP_HDR_SetVDmImapRangeMin		 ( pCfg->gamma_cfg.rangeMin 		 );
	VDP_HDR_SetVDmImapRange 		 ( pCfg->gamma_cfg.range			 );
	VDP_HDR_SetVDmImapRangeInv		 ( pCfg->gamma_cfg.rangeInv 		 );

	VDP_HDR_SetVDmImapRgb2lms		 ( pCfg->gamma_cfg.m33Rgb2Lms		 );
	VDP_HDR_SetVDmImapRgb2lmsscale2p ( pCfg->gamma_cfg.m33Rgb2LmsScale2P );
	VDP_HDR_SetVDmImapRgb2lmsmin	 ( pCfg->gamma_cfg.csc_min			 );
	VDP_HDR_SetVDmImapRgb2lmsmax	 ( pCfg->gamma_cfg.csc_max			 );
	
#if 0
	
#if 1



	u32Pre = 1 << 10;
	u32DcPre = 4 * 32 ;//1:8bit; 4:10bit

	u32Scale	= 1 << (15 - 10);
	u32DcPre	= 4 * 64;
	
#if 1//rgb->yuv



		m33Lms2Ipt[0][0]	  =  (HI_S32)(0.1821  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][1]	  =  (HI_S32)(0.6124  * u32Pre*u32Scale/2);
		m33Lms2Ipt[0][2]	  =  (HI_S32)(0.0618  * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][0]	  =  (HI_S32)(-0.1003 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][1]	  =  (HI_S32)(-0.3376 * u32Pre*u32Scale/2);
		m33Lms2Ipt[1][2]	  =  (HI_S32)( 0.4379 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][0]	  =  (HI_S32)( 0.4379 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][1]	  =  (HI_S32)(-0.3978 * u32Pre*u32Scale/2);
		m33Lms2Ipt[2][2]	  =  (HI_S32)(-0.0402 * u32Pre*u32Scale/2);
		
		s32CscDc[0] =  (HI_S32)(16  * u32DcPre/2);	
		s32CscDc[1] =  (HI_S32)(128 * u32DcPre/2);	
		s32CscDc[2] =  (HI_S32)(128 * u32DcPre/2);	
#endif


#endif

#if 1
	
	VDP_HDR_SetVCvmImapLms2iptscale2p(15 | (1<<5) );
	VDP_HDR_SetVCvmImapLms2ipt		 (m33Lms2Ipt		 );
	VDP_HDR_SetVCvmImapLms2iptOutDc  (s32CscDc );
	VDP_HDR_SetVDmImapLms2iptmin	 (0);
	VDP_HDR_SetVDmImapLms2iptmax	 (0x7FFF);
	VDP_HDR_SetVCvmLms2iptEn		 ( 1 );
#endif
#if 0
	VDP_HDR_SetVCvmLms2iptEn(HI_TRUE);
	VDP_HDR_SetVCvmImapLms2ipt ( m33Lms2Ipt);
	////VDP_HDR_SetVCvmImapLms2iptscale2p ( 14 );//普通通路
	VDP_HDR_SetVCvmImapLms2iptscale2p ( 12 );
	VDP_HDR_SetVCvmImapLms2iptOutDc(s32CscDc);
	VDP_HDR_SetGCvmLms2IptMin (0);;
	VDP_HDR_SetGCvmLms2IptMax (32767);
#endif
#endif
	//VDP_HDR_SetVDmNormEn(0);
#endif
}




