// *****************************************************************************
//
// Copyright(c) 2015,Hisilicon Technologies Co., Ltd.
// All Rights Reserved
//
// Project name : VDP  
// File Name    : coef.cpp
// Author       : z00294247
// Date         : 2015/7/17
//
// Module Name  : COEF.CPP
// -----------------------------------------------------------------------------
// Abstract     :
//code_template.v*
// 
// *****************************************************************************
// Called By    : TOP
// Modification History
// -----------------------------------------------------------------------------
// $Log :     $
//
//
// --=========================================================================--


#include "vdp_drv_vdm_coef.h"
//#include "vdm_process.h"
#include "vdp_drv_coef.h"
#include "vdp_define.h"
#include "vdp_fpga_define.h"
#include "vdp_hal_comm.h"
#include "vdp_hal_ip_vdm.h"
#include "VdpHiHDR_cfg.h"
#include "vdp_drv_comm.h"
#include "vdp_drv_ip_gdm.h"
#define VDP_COEF_COMPILE


#ifdef VDP_COEF_COMPILE

extern HI_U32 GdmtcLut[64];
extern HI_U32 GdmtcLut_New[64];
extern HI_U32 GdmtcLut_1[32];
extern HI_U32 GdmtcLut_New_1[32];
extern HI_U32 GdmtcLut_2[64];
extern HI_U32 GdmtcLut_New_2[64];

#endif 
HI_U32 u32GdmTMLutSDR2BBC[32] = {				   //U8.8 [0,65535]
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256,
	256
};

HI_U32 u32GdmHLGLut[64] = {            //U12.0 [0,4095]
	0	,
	28	,
	39	,
	48	,
	55	,
	78	,
	96	,
	111	,
	124	,
	136	,
	147	,
	157	,
	192	,
	222	,
	271	,
	313	,
	350	,
	384	,
	443	,
	496	,
	543	,
	586	,
	627	,
	768	,
	887	,
	991	,
	1086,
	1173,
	1254,
	1330,
	1402,
	1470,
	1536,
	1598,
	1659,
	1717,
	1773,
	1982,
	2165,
	2313,
	2436,
	2541,
	2633,
	2714,
	2788,
	2854,
	2916,
	2972,
	3024,
	3073,
	3119,
	3162,
	3203,
	3346,
	3466,
	3569,
	3660,
	3740,
	3812,
	3878,
	3939,
	3995,
	4047,
	4095
};

HI_U32 u32GdmTMLutSDR2HDR10[32] = {				   //U8.8 [0,65535]
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41,
	41
};

HI_U32 u32GdmTMLut[32] = {//U8.8 [0,65535]
4160,
4160,
4160,
4160,
4160,
4160,
3771,
3418,
3134,
2899,
2700,
2531,
2383,
2254,
2140,
2038,
1946,
1788,
1638,
1489,
1365,
1260,
1170,
1092,
1024,
819	,
683	,
512	,
410	,
341	,
293	,
256
};

HI_U32 u32GdmPQLut[64] = {            //U12.0 [0,4095]
	205	,
	264	,
	320	,
	370	,
	420	,
	657	,
	749	,
	820	,
	878	,
	927	,
	970	,
	1008,
	1129,
	1220,
	1354,
	1454,
	1533,
	1600,
	1708,
	1794,
	1865,
	1926,
	1980,
	2146,
	2267,
	2361,
	2440,
	2506,
	2564,
	2616,
	2662,
	2704,
	2742,
	2777,
	2810,
	2841,
	2869,
	2969,
	3050,
	3119,
	3178,
	3231,
	3278,
	3321,
	3360,
	3395,
	3428,
	3459,
	3488,
	3515,
	3540,
	3564,
	3587,
	3668,
	3736,
	3794,
	3846,
	3892,
	3933,
	3971,
	4006,
	4038,
	4067,
	4095
};

HI_U32 u32GdmPQLut2500[64] = {            //U12.0 [0,4095]
	0	,
	167	,
	227	,
	270	,
	304	,
	400	,
	466	,
	517	,
	559	,
	596	,
	628	,
	657	,
	749	,
	820	,
	927	,
	1008,
	1074,
	1129,
	1220,
	1293,
	1354,
	1407,
	1454,
	1600,
	1708,
	1794,
	1865,
	1926,
	1980,
	2028,
	2071,
	2110,
	2146,
	2179,
	2210,
	2239,
	2267,
	2361,
	2440,
	2506,
	2564,
	2616,
	2662,
	2704,
	2742,
	2777,
	2810,
	2841,
	2869,
	2896,
	2922,
	2946,
	2969,
	3050,
	3119,
	3178,
	3231,
	3278,
	3321,
	3360,
	3395,
	3428,
	3459,
	3488,
};

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;


HI_VOID Vdp_Drv_SetGdmCoef_Tmmp(GDM_SCENE_MODE_E   enHiHdrGfxMode)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
	HI_U32 addr = 0;

#else
	HI_U8  *addr	= gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GDM_TONEMAP] ;//add by hyx
#endif

    VDP_COEF_SEND_CFG   stCoefSend;
    

	HI_S32 tcLut_s32_1[32];

    void*  p_coef_array[]    = {tcLut_s32_1};
    HI_U32 lut_length[]      = {32};
    HI_U32 coef_bit_length[] = {16};

    FILE*   fp_hihdr_gdm_tmmp_coef=NULL;    

    //bOpenStiFile(STI_FILE_COEF_VDM);
	VDP_GP_SetGdmGdmParaTmapAddr (VDP_LAYER_GP0,	   gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GDM_TONEMAP]);
	
	
    if(enHiHdrGfxMode == GDM_SDR_IN_HDR10_OUT)
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == GDM_SDR_IN_HDR10_OUT\n");
        p_coef_array[0]         = (HI_S32*)u32GdmTMLutSDR2HDR10;
    }
    else if(enHiHdrGfxMode == GDM_SDR_IN_BBC_OUT)
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == GDM_SDR_IN_BBC_OUT\n");
        p_coef_array[0]         = (HI_S32*)u32GdmTMLutSDR2BBC;
    }
    else
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == Default !\n");
        p_coef_array[0]         = (HI_S32*)u32GdmTMLut;//tcLut_1;
    }

	stCoefSend.coef_addr         = addr                     ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM        ; 
    stCoefSend.fp_coef           = fp_hihdr_gdm_tmmp_coef              ; 
    stCoefSend.lut_num           = 1              ; 
    stCoefSend.burst_num         = 1            ; 
    stCoefSend.cycle_num         = 8            ; 
    stCoefSend.p_coef_array      = p_coef_array        		; 
    stCoefSend.lut_length        = lut_length               ; 
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 
	
    VDP_FUNC_SendCoef(&stCoefSend);
	VDP_GP_SetGdmGdmParaTmapUpd 	(VDP_LAYER_GP0, 1);

#endif

}


HI_VOID Vdp_Drv_SetGdmCoef_Gmma(GDM_SCENE_MODE_E   enHiHdrGfxMode)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
	HI_U32 addr = 0;

#else
	HI_U8  *addr	= gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GDM_GAMMA2] ;//add by hyx
#endif

    VDP_COEF_SEND_CFG   stCoefSend;
    

	HI_S32 tcLut_s32_1[64];
    void*  p_coef_array[]    = {tcLut_s32_1};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {12};

    FILE*   fp_hihdr_gdm_gamma_coef = NULL;    

    //bOpenStiFile(STI_FILE_COEF_VDM);
	VDP_GP_SetGdmGdmParaGmmAddr  (VDP_LAYER_GP0,		gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GDM_GAMMA2]);
	
	
    if(enHiHdrGfxMode == GDM_SDR_IN_HDR10_OUT)
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == GDM_SDR_IN_HDR10_OUT\n");
        p_coef_array[0]         = (HI_S32*)u32GdmPQLut2500;
    }
    else if(enHiHdrGfxMode == GDM_SDR_IN_BBC_OUT)
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == GDM_SDR_IN_BBC_OUT\n");
        p_coef_array[0]         = (HI_S32*)u32GdmHLGLut;
    }
    else
    {
        VDP_PRINT("ToneMapping Coef : GdmSceneMode == Default !\n");
        p_coef_array[0]         = (HI_S32*)u32GdmPQLut;//tcLut_1;
    }

	stCoefSend.coef_addr         = addr                     ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM        ; 
    stCoefSend.fp_coef           = fp_hihdr_gdm_gamma_coef              ; 
    stCoefSend.lut_num           = 1              ; 
    stCoefSend.burst_num         = 1            ; 
    stCoefSend.cycle_num         = 10            ; 
    stCoefSend.p_coef_array      = p_coef_array        		; 
    stCoefSend.lut_length        = lut_length               ; 
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 
	
    VDP_FUNC_SendCoef(&stCoefSend);
	VDP_GP_SetGdmGdmParaGmmUpd		(VDP_LAYER_GP0, 1);

#endif

}









