#include "vdp_drv_hihdr.h"
//#include <stdio.h>
//#include "vdp_define.h"
#include "vdp_fpga_define.h"

#include "vdp_drv_vdm_coef.h"
#include "vdp_drv_gdm_coef.h"

#include "VdpHiHDR_cfg.h"
#include "vdp_drv_ip_vdm.h"
#include "vdp_drv_ip_gdm.h"

#include "vdp_drv_vid.h"
#include "vdp_hal_mmu.h"




HI_S32 VDP_DRV_GetHiHdrGfxCfg(GdmCfg_t  *pstDmCfg)
{

    if(pstDmCfg->GdmSceneMode == GDM_SDR_IN_HDR10_OUT)
    {
        VDP_PRINT("GdmSceneMode == GDM_SDR_IN_HDR10_OUT\n");
        //typ mode

        pstDmCfg->vdm_en                 = 1  ;
        pstDmCfg->vdm_degmm_en           = 1  ;
        pstDmCfg->vdm_csc_en             = 1  ;
        pstDmCfg->vdm_tmap_en            = 1  ;
        pstDmCfg->vdm_gmm_en             = 1  ;
        pstDmCfg->vdm_dither_en          = 1  ;
        pstDmCfg->vdm_rgb2yuv_en         = 1  ;
        pstDmCfg->gdm_r2y_premult_en     = 0  ;

        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->u16M3LumaCal[0]        = 1076  ;
        pstDmCfg->u16M3LumaCal[1]        = 2777  ;
        pstDmCfg->u16M3LumaCal[2]        = 243   ;
        pstDmCfg->u16ScaleLumaCal        = 12    ;
        pstDmCfg->u16ScaleCoefTM         = 8     ;

        pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
        pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
        pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
        pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
        pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
        pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
        pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
        pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
        pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
        pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
        pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
        pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

        pstDmCfg->TMPara.gmm_x1_step     = 9     ;
        pstDmCfg->TMPara.gmm_x2_step     = 10    ;
        pstDmCfg->TMPara.gmm_x3_step     = 12    ;
        pstDmCfg->TMPara.gmm_x4_step     = 13    ;
        pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
        pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
        pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
        pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
        pstDmCfg->TMPara.gmm_x1_num      = 16    ;
        pstDmCfg->TMPara.gmm_x2_num      = 8     ;
        pstDmCfg->TMPara.gmm_x3_num      = 2     ;
        pstDmCfg->TMPara.gmm_x4_num      = 5     ;

        pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
        pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;


        pstDmCfg->s16M33Src2Disp[0][0]   = 10280 ; //BT709 to BT2020
        pstDmCfg->s16M33Src2Disp[0][1]   = 5396  ;
        pstDmCfg->s16M33Src2Disp[0][2]   = 709   ;
        pstDmCfg->s16M33Src2Disp[1][0]   = 1133  ;
        pstDmCfg->s16M33Src2Disp[1][1]   = 15066 ;
        pstDmCfg->s16M33Src2Disp[1][2]   = 185   ;
        pstDmCfg->s16M33Src2Disp[2][0]   = 269   ;
        pstDmCfg->s16M33Src2Disp[2][1]   = 1442  ;
        pstDmCfg->s16M33Src2Disp[2][2]   = 14672 ;
        pstDmCfg->u16ScaleSrc2Disp       = 14    ;

        //dither cfg
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

        pstDmCfg->s16M33RGB2YUV[0][0]    = 3685 ; //BT2020
        pstDmCfg->s16M33RGB2YUV[0][1]    = 9512 ;
        pstDmCfg->s16M33RGB2YUV[0][2]    = 832  ;
        pstDmCfg->s16M33RGB2YUV[1][0]    = -2004;
        pstDmCfg->s16M33RGB2YUV[1][1]    = -5171;
        pstDmCfg->s16M33RGB2YUV[1][2]    = 7175 ;
        pstDmCfg->s16M33RGB2YUV[2][0]    = 7175 ;
        pstDmCfg->s16M33RGB2YUV[2][1]    = -6598;
        pstDmCfg->s16M33RGB2YUV[2][2]    = -577 ;
        pstDmCfg->u16ScaleRGB2YUV        = 14   ;
        pstDmCfg->s16InOffsetRGB2YUV[0]  = 0    ;
        pstDmCfg->s16InOffsetRGB2YUV[1]  = 0    ;
        pstDmCfg->s16InOffsetRGB2YUV[2]  = 0    ;
        pstDmCfg->s16OutOffsetRGB2YUV[0] = 64   ;
        pstDmCfg->s16OutOffsetRGB2YUV[1] = 512  ;
        pstDmCfg->s16OutOffsetRGB2YUV[2] = 512  ;
    }
        else if(pstDmCfg->GdmSceneMode == GDM_SDR_IN_BBC_OUT)
        {
            VDP_PRINT("GdmSceneMode == GDM_SDR_IN_BBC_OUT\n");
            //typ mode

            pstDmCfg->vdm_en                 = 1  ;
            pstDmCfg->vdm_degmm_en           = 1  ;
            pstDmCfg->vdm_csc_en             = 0  ;
            pstDmCfg->vdm_tmap_en            = 1  ;
            pstDmCfg->vdm_gmm_en             = 1  ;
            pstDmCfg->vdm_dither_en          = 1  ;
            pstDmCfg->vdm_rgb2yuv_en         = 1  ;
            pstDmCfg->gdm_r2y_premult_en     = 0  ;

            pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
            pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
            pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

            pstDmCfg->u16M3LumaCal[0]        = 1076  ;
            pstDmCfg->u16M3LumaCal[1]        = 2777  ;
            pstDmCfg->u16M3LumaCal[2]        = 243   ;
            pstDmCfg->u16ScaleLumaCal        = 12    ;
            pstDmCfg->u16ScaleCoefTM         = 8     ;

            pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
            pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
            pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
            pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
            pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
            pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
            pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

            pstDmCfg->TMPara.gmm_x1_step     = 9     ;
            pstDmCfg->TMPara.gmm_x2_step     = 10    ;
            pstDmCfg->TMPara.gmm_x3_step     = 12    ;
            pstDmCfg->TMPara.gmm_x4_step     = 13    ;
            pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
            pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
            pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
            pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
            pstDmCfg->TMPara.gmm_x1_num      = 16    ;
            pstDmCfg->TMPara.gmm_x2_num      = 8     ;
            pstDmCfg->TMPara.gmm_x3_num      = 2     ;
            pstDmCfg->TMPara.gmm_x4_num      = 5     ;

            pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
            pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
            pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
            pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
            pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
            pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
            pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
            pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
            pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
            pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
            pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
            pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
            pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
            pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
            pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
            pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
            pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
            pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
            pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
            pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
            pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
            pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

            pstDmCfg->s16M33Src2Disp[0][0]   = 27209 ;//709 to 709 bypass
            pstDmCfg->s16M33Src2Disp[0][1]   = -9632 ;
            pstDmCfg->s16M33Src2Disp[0][2]   = -1194 ;
            pstDmCfg->s16M33Src2Disp[1][0]   = -2045 ;
            pstDmCfg->s16M33Src2Disp[1][1]   = 18565 ;
            pstDmCfg->s16M33Src2Disp[1][2]   = -138  ;
            pstDmCfg->s16M33Src2Disp[2][0]   = -297  ;
            pstDmCfg->s16M33Src2Disp[2][1]   = -1648 ;
            pstDmCfg->s16M33Src2Disp[2][2]   = 18330 ;
            pstDmCfg->u16ScaleSrc2Disp       = 14    ;
            //dither cfg
            pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
            pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
            pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

            pstDmCfg->s16M33RGB2YUV[0][0]    = 2984; //BT2020
            pstDmCfg->s16M33RGB2YUV[0][1]    = 10034;
            pstDmCfg->s16M33RGB2YUV[0][2]    = 1013;
            pstDmCfg->s16M33RGB2YUV[1][0]    = -1643;
            pstDmCfg->s16M33RGB2YUV[1][1]    = -5531;
            pstDmCfg->s16M33RGB2YUV[1][2]    = 7175;
            pstDmCfg->s16M33RGB2YUV[2][0]    = 7175;
            pstDmCfg->s16M33RGB2YUV[2][1]    = -6518;
            pstDmCfg->s16M33RGB2YUV[2][2]    = -659;
            pstDmCfg->u16ScaleRGB2YUV        = 14   ;
            pstDmCfg->s16InOffsetRGB2YUV[0]  = 0    ;
            pstDmCfg->s16InOffsetRGB2YUV[1]  = 0    ;
            pstDmCfg->s16InOffsetRGB2YUV[2]  = 0    ;
            pstDmCfg->s16OutOffsetRGB2YUV[0] = 64   ;
            pstDmCfg->s16OutOffsetRGB2YUV[1] = 512  ;
            pstDmCfg->s16OutOffsetRGB2YUV[2] = 512  ;
        }
        else if(pstDmCfg->GdmSceneMode == GDM_SDR_PREMULT)
        {
            VDP_PRINT("GdmSceneMode == GDM_SDR_PREMULT\n");
            //typ mode

            pstDmCfg->vdm_en                 = 0  ;
            pstDmCfg->vdm_degmm_en           = 0  ;
            pstDmCfg->vdm_csc_en             = 0  ;
            pstDmCfg->vdm_tmap_en            = 0  ;
            pstDmCfg->vdm_gmm_en             = 0  ;
            pstDmCfg->vdm_dither_en          = 0  ;
            pstDmCfg->vdm_rgb2yuv_en         = 1  ;
            pstDmCfg->gdm_r2y_premult_en     = 1  ;

            pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
            pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
            pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

            pstDmCfg->u16M3LumaCal[0]        = 1076  ;
            pstDmCfg->u16M3LumaCal[1]        = 2777  ;
            pstDmCfg->u16M3LumaCal[2]        = 243   ;
            pstDmCfg->u16ScaleLumaCal        = 12    ;
            pstDmCfg->u16ScaleCoefTM         = 8     ;

            pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
            pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
            pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
            pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
            pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
            pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
            pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

            pstDmCfg->TMPara.gmm_x1_step     = 9     ;
            pstDmCfg->TMPara.gmm_x2_step     = 10    ;
            pstDmCfg->TMPara.gmm_x3_step     = 12    ;
            pstDmCfg->TMPara.gmm_x4_step     = 13    ;
            pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
            pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
            pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
            pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
            pstDmCfg->TMPara.gmm_x1_num      = 16    ;
            pstDmCfg->TMPara.gmm_x2_num      = 8     ;
            pstDmCfg->TMPara.gmm_x3_num      = 2     ;
            pstDmCfg->TMPara.gmm_x4_num      = 5     ;

            pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
            pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
            pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
            pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
            pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
            pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
            pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
            pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
            pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
            pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
            pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
            pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
            pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
            pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
            pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
            pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
            pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
            pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
            pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
            pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
            pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
            pstDmCfg->GammaPara.gmm_x8_num           = 63    ;


            pstDmCfg->s16M33Src2Disp[0][0]   = 10280 ; //BT709 to BT2020
            pstDmCfg->s16M33Src2Disp[0][1]   = 5396  ;
            pstDmCfg->s16M33Src2Disp[0][2]   = 709   ;
            pstDmCfg->s16M33Src2Disp[1][0]   = 1133  ;
            pstDmCfg->s16M33Src2Disp[1][1]   = 15066 ;
            pstDmCfg->s16M33Src2Disp[1][2]   = 185   ;
            pstDmCfg->s16M33Src2Disp[2][0]   = 269   ;
            pstDmCfg->s16M33Src2Disp[2][1]   = 1442  ;
            pstDmCfg->s16M33Src2Disp[2][2]   = 14672 ;
            pstDmCfg->u16ScaleSrc2Disp       = 14    ;

            //dither cfg
            pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
            pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
            pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

            pstDmCfg->s16M33RGB2YUV[0][0]    = 2984 ; //BT709
            pstDmCfg->s16M33RGB2YUV[0][1]    = 10034;
            pstDmCfg->s16M33RGB2YUV[0][2]    = 1013 ;
            pstDmCfg->s16M33RGB2YUV[1][0]    = -1643;
            pstDmCfg->s16M33RGB2YUV[1][1]    = -5531;
            pstDmCfg->s16M33RGB2YUV[1][2]    = 7175 ;
            pstDmCfg->s16M33RGB2YUV[2][0]    = 7175 ;
            pstDmCfg->s16M33RGB2YUV[2][1]    = -6518;
            pstDmCfg->s16M33RGB2YUV[2][2]    = -659 ;
            pstDmCfg->u16ScaleRGB2YUV        = 14   ;
            pstDmCfg->s16InOffsetRGB2YUV[0]  = 0    ;
            pstDmCfg->s16InOffsetRGB2YUV[1]  = 0    ;
            pstDmCfg->s16InOffsetRGB2YUV[2]  = 0    ;
            pstDmCfg->s16OutOffsetRGB2YUV[0] = 64   ;
            pstDmCfg->s16OutOffsetRGB2YUV[1] = 512  ;
            pstDmCfg->s16OutOffsetRGB2YUV[2] = 512  ;
        }
        else if(pstDmCfg->GdmSceneMode == GDM_709_IN_BT2020_OUT)
        {
            VDP_PRINT("GdmSceneMode == GDM_709_IN_BT2020_OUT\n");
            //typ mode

            pstDmCfg->vdm_en                 = 1  ;
            pstDmCfg->vdm_degmm_en           = 1  ;
            pstDmCfg->vdm_csc_en             = 1  ;
            pstDmCfg->vdm_tmap_en            = 0  ;
            pstDmCfg->vdm_gmm_en             = 1  ;
            pstDmCfg->vdm_dither_en          = 1  ;
            pstDmCfg->vdm_rgb2yuv_en         = 1  ;
            pstDmCfg->gdm_r2y_premult_en     = 0  ;

            pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
            pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
            pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

            pstDmCfg->u16M3LumaCal[0]        = 1076  ;
            pstDmCfg->u16M3LumaCal[1]        = 2777  ;
            pstDmCfg->u16M3LumaCal[2]        = 243   ;
            pstDmCfg->u16ScaleLumaCal        = 12    ;
            pstDmCfg->u16ScaleCoefTM         = 8     ;

            pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
            pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
            pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
            pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
            pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
            pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
            pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

            pstDmCfg->TMPara.gmm_x1_step     = 9     ;
            pstDmCfg->TMPara.gmm_x2_step     = 10    ;
            pstDmCfg->TMPara.gmm_x3_step     = 12    ;
            pstDmCfg->TMPara.gmm_x4_step     = 13    ;
            pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
            pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
            pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
            pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
            pstDmCfg->TMPara.gmm_x1_num      = 16    ;
            pstDmCfg->TMPara.gmm_x2_num      = 8     ;
            pstDmCfg->TMPara.gmm_x3_num      = 2     ;
            pstDmCfg->TMPara.gmm_x4_num      = 5     ;

            pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
            pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
            pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
            pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
            pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
            pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
            pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
            pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
            pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
            pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
            pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
            pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
            pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
            pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
            pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
            pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
            pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
            pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
            pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
            pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
            pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
            pstDmCfg->GammaPara.gmm_x8_num           = 63    ;


            pstDmCfg->s16M33Src2Disp[0][0]   = 27209 ;
            pstDmCfg->s16M33Src2Disp[0][1]   = -9632 ;
            pstDmCfg->s16M33Src2Disp[0][2]   = -1194 ;
            pstDmCfg->s16M33Src2Disp[1][0]   = -2045 ;
            pstDmCfg->s16M33Src2Disp[1][1]   = 18565 ;
            pstDmCfg->s16M33Src2Disp[1][2]   = -138  ;
            pstDmCfg->s16M33Src2Disp[2][0]   = -297  ;
            pstDmCfg->s16M33Src2Disp[2][1]   = -1648 ;
            pstDmCfg->s16M33Src2Disp[2][2]   = 18330 ;
            pstDmCfg->u16ScaleSrc2Disp       = 14    ;

            //dither cfg
            pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
            pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
            pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

            pstDmCfg->s16M33RGB2YUV[0][0]    = 2984  ;
            pstDmCfg->s16M33RGB2YUV[0][1]    = 10034 ;
            pstDmCfg->s16M33RGB2YUV[0][2]    = 1013  ;
            pstDmCfg->s16M33RGB2YUV[1][0]    = -1643 ;
            pstDmCfg->s16M33RGB2YUV[1][1]    = -5531 ;
            pstDmCfg->s16M33RGB2YUV[1][2]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][0]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][1]    = -6518 ;
            pstDmCfg->s16M33RGB2YUV[2][2]    = -659  ;
            pstDmCfg->u16ScaleRGB2YUV        = 14    ;
            pstDmCfg->s16InOffsetRGB2YUV[0]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[1]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[2]  = 0     ;
            pstDmCfg->s16OutOffsetRGB2YUV[0] = 64    ;
            pstDmCfg->s16OutOffsetRGB2YUV[1] = 512   ;
            pstDmCfg->s16OutOffsetRGB2YUV[2] = 512   ;
        }
        else if(pstDmCfg->GdmSceneMode == GDM_BYPASS_MODE)
        {
            VDP_PRINT("GdmSceneMode == GDM_BYPASS_MODE\n");
            //typ mode

            pstDmCfg->vdm_en                 = 0  ;
            pstDmCfg->vdm_degmm_en           = 0  ;
            pstDmCfg->vdm_csc_en             = 0  ;
            pstDmCfg->vdm_tmap_en            = 0  ;
            pstDmCfg->vdm_gmm_en             = 0  ;
            pstDmCfg->vdm_dither_en          = 0  ;
            pstDmCfg->vdm_rgb2yuv_en         = 1  ;
            pstDmCfg->gdm_r2y_premult_en     = 0  ;

            pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
            pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
            pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

            pstDmCfg->u16M3LumaCal[0]        = 1076  ;
            pstDmCfg->u16M3LumaCal[1]        = 2777  ;
            pstDmCfg->u16M3LumaCal[2]        = 243   ;
            pstDmCfg->u16ScaleLumaCal        = 12    ;
            pstDmCfg->u16ScaleCoefTM         = 8     ;

            pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
            pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
            pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
            pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
            pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
            pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
            pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

            pstDmCfg->TMPara.gmm_x1_step     = 9     ;
            pstDmCfg->TMPara.gmm_x2_step     = 10    ;
            pstDmCfg->TMPara.gmm_x3_step     = 12    ;
            pstDmCfg->TMPara.gmm_x4_step     = 13    ;
            pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
            pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
            pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
            pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
            pstDmCfg->TMPara.gmm_x1_num      = 16    ;
            pstDmCfg->TMPara.gmm_x2_num      = 8     ;
            pstDmCfg->TMPara.gmm_x3_num      = 2     ;
            pstDmCfg->TMPara.gmm_x4_num      = 5     ;

            pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
            pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
            pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
            pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
            pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
            pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
            pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
            pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
            pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
            pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
            pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
            pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
            pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
            pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
            pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
            pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
            pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
            pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
            pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
            pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
            pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
            pstDmCfg->GammaPara.gmm_x8_num           = 63    ;


            pstDmCfg->s16M33Src2Disp[0][0]   = 27209 ;
            pstDmCfg->s16M33Src2Disp[0][1]   = -9632 ;
            pstDmCfg->s16M33Src2Disp[0][2]   = -1194 ;
            pstDmCfg->s16M33Src2Disp[1][0]   = -2045 ;
            pstDmCfg->s16M33Src2Disp[1][1]   = 18565 ;
            pstDmCfg->s16M33Src2Disp[1][2]   = -138  ;
            pstDmCfg->s16M33Src2Disp[2][0]   = -297  ;
            pstDmCfg->s16M33Src2Disp[2][1]   = -1648 ;
            pstDmCfg->s16M33Src2Disp[2][2]   = 18330 ;
            pstDmCfg->u16ScaleSrc2Disp       = 14    ;

            //dither cfg
            pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
            pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
            pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

            pstDmCfg->s16M33RGB2YUV[0][0]    = 2984  ;
            pstDmCfg->s16M33RGB2YUV[0][1]    = 10034 ;
            pstDmCfg->s16M33RGB2YUV[0][2]    = 1013  ;
            pstDmCfg->s16M33RGB2YUV[1][0]    = -1643 ;
            pstDmCfg->s16M33RGB2YUV[1][1]    = -5531 ;
            pstDmCfg->s16M33RGB2YUV[1][2]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][0]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][1]    = -6518 ;
            pstDmCfg->s16M33RGB2YUV[2][2]    = -659  ;
            pstDmCfg->u16ScaleRGB2YUV        = 14    ;
            pstDmCfg->s16InOffsetRGB2YUV[0]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[1]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[2]  = 0     ;
            pstDmCfg->s16OutOffsetRGB2YUV[0] = 64    ;
            pstDmCfg->s16OutOffsetRGB2YUV[1] = 512   ;
            pstDmCfg->s16OutOffsetRGB2YUV[2] = 512   ;
        }
        else
        {
            VDP_PRINT("VDP_FUNC_SetGdmMode enter into typ mode\n");
            //typ mode

            pstDmCfg->u16M3LumaCal[0]        = 1076  ;
            pstDmCfg->u16M3LumaCal[1]        = 2777  ;
            pstDmCfg->u16M3LumaCal[2]        = 243   ;
            pstDmCfg->u16ScaleLumaCal        = 12    ;
            pstDmCfg->u16ScaleCoefTM         = 8     ;

            pstDmCfg->DePQPara.gmm_x1_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x2_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x3_step   = 4     ;
            pstDmCfg->DePQPara.gmm_x4_step   = 5     ;
            pstDmCfg->DePQPara.gmm_x1_pos    = 672   ;
            pstDmCfg->DePQPara.gmm_x2_pos    = 832   ;
            pstDmCfg->DePQPara.gmm_x3_pos    = 992   ;
            pstDmCfg->DePQPara.gmm_x4_pos    = 1023  ;
            pstDmCfg->DePQPara.gmm_x1_num    = 42    ;
            pstDmCfg->DePQPara.gmm_x2_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x3_num    = 10    ;
            pstDmCfg->DePQPara.gmm_x4_num    = 1     ;

            pstDmCfg->TMPara.gmm_x1_step     = 9     ;
            pstDmCfg->TMPara.gmm_x2_step     = 10    ;
            pstDmCfg->TMPara.gmm_x3_step     = 12    ;
            pstDmCfg->TMPara.gmm_x4_step     = 13    ;
            pstDmCfg->TMPara.gmm_x1_pos      = 8192  ;
            pstDmCfg->TMPara.gmm_x2_pos      = 16384 ;
            pstDmCfg->TMPara.gmm_x3_pos      = 24576 ;
            pstDmCfg->TMPara.gmm_x4_pos      = 65535 ;
            pstDmCfg->TMPara.gmm_x1_num      = 16    ;
            pstDmCfg->TMPara.gmm_x2_num      = 8     ;
            pstDmCfg->TMPara.gmm_x3_num      = 2     ;
            pstDmCfg->TMPara.gmm_x4_num      = 5     ;

            pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
            pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
            pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
            pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
            pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
            pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
            pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
            pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
            pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
            pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
            pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
            pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
            pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
            pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
            pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
            pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
            pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
            pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
            pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
            pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
            pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
            pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
            pstDmCfg->GammaPara.gmm_x8_num           = 63    ;


            pstDmCfg->s16M33Src2Disp[0][0]   = 27209 ;
            pstDmCfg->s16M33Src2Disp[0][1]   = -9632 ;
            pstDmCfg->s16M33Src2Disp[0][2]   = -1194 ;
            pstDmCfg->s16M33Src2Disp[1][0]   = -2045 ;
            pstDmCfg->s16M33Src2Disp[1][1]   = 18565 ;
            pstDmCfg->s16M33Src2Disp[1][2]   = -138  ;
            pstDmCfg->s16M33Src2Disp[2][0]   = -297  ;
            pstDmCfg->s16M33Src2Disp[2][1]   = -1648 ;
            pstDmCfg->s16M33Src2Disp[2][2]   = 18330 ;
            pstDmCfg->u16ScaleSrc2Disp       = 14    ;

            //dither cfg
            pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
            pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
            pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
            pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

            pstDmCfg->s16M33RGB2YUV[0][0]    = 2984  ;
            pstDmCfg->s16M33RGB2YUV[0][1]    = 10034 ;
            pstDmCfg->s16M33RGB2YUV[0][2]    = 1013  ;
            pstDmCfg->s16M33RGB2YUV[1][0]    = -1643 ;
            pstDmCfg->s16M33RGB2YUV[1][1]    = -5531 ;
            pstDmCfg->s16M33RGB2YUV[1][2]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][0]    = 7175  ;
            pstDmCfg->s16M33RGB2YUV[2][1]    = -6518 ;
            pstDmCfg->s16M33RGB2YUV[2][2]    = -659  ;
            pstDmCfg->u16ScaleRGB2YUV        = 14    ;
            pstDmCfg->s16InOffsetRGB2YUV[0]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[1]  = 0     ;
            pstDmCfg->s16InOffsetRGB2YUV[2]  = 0     ;
            pstDmCfg->s16OutOffsetRGB2YUV[0] = 64    ;
            pstDmCfg->s16OutOffsetRGB2YUV[1] = 512   ;
            pstDmCfg->s16OutOffsetRGB2YUV[2] = 512   ;
        }

        return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetHiHdrGfxCfg(GdmCfg_t  *pstDmCfg)
{

    //filed declare
    HI_U32 gdm_en    =  0;
    HI_U32 gdm_degmm_en    =  0;
    HI_U32 gdm_csc_en    =  0;
    HI_U32 gdm_tmap_en    =  0;
    HI_U32 gdm_gmm_en    =  0;
    HI_U32 gdm_dither_en    =  0;
    HI_U32 gdm_rgb2yuv_en    =  0;

    HI_U32 gdm_tmap_in_bdp    =  0;
    HI_U32 gdm_dither_mode    =  0;

    HI_U16 gdm_luma_coef0_tmap    =  0;
    HI_U16 gdm_luma_coef1_tmap    =  0;
    HI_U16 gdm_luma_coef2_tmap    =  0;
    HI_U16 gdm_tmap_luma_scale    =  0;
    HI_U16 gdm_tmap_coef_scale    =  0;
    HI_U16 gdm_tmap_out_clip_min    =  0;
    HI_U16 gdm_tmap_out_clip_max    =  0;
//  HI_U32 gdm_para_gmm_addr    =  0;
//  HI_U32 gdm_para_tmap_addr    =  0;
//  HI_U32 gdm_para_tmap_upd    =  1;
//  HI_U32 gdm_para_gmm_upd    =  1;
    HI_U32 g_degmm_x4_step    =  0;
    HI_U32 g_degmm_x3_step    =  0;
    HI_U32 g_degmm_x2_step    =  0;
    HI_U32 g_degmm_x1_step    =  0;
    HI_U32 g_degmm_x2_pos    =  0;
    HI_U32 g_degmm_x1_pos    =  0;
    HI_U32 g_degmm_x4_pos    =  0;
    HI_U32 g_degmm_x3_pos    =  0;
    HI_U32 g_degmm_x4_num    =  0;
    HI_U32 g_degmm_x3_num    =  0;
    HI_U32 g_degmm_x2_num    =  0;
    HI_U32 g_degmm_x1_num    =  0;
    HI_U32 gdm_tmap_rd_en    =  0;
    HI_U32 gdm_tmap_para_data    =  0;
    HI_U32 g_tmap_x4_step    =  0;
    HI_U32 g_tmap_x3_step    =  0;
    HI_U32 g_tmap_x2_step    =  0;
    HI_U32 g_tmap_x1_step    =  0;
    HI_U32 g_tmap_x1_pos    =  0;
    HI_U32 g_tmap_x2_pos    =  0;
    HI_U32 g_tmap_x4_num    =  0;
    HI_U32 g_tmap_x3_num    =  0;
    HI_U32 g_tmap_x2_num    =  0;
    HI_U32 g_tmap_x1_num    =  0;
    HI_U32 g_tmap_x3_pos    =  0;
    HI_U32 g_tmap_x4_pos    =  0;
    HI_U32 gdm_gamma_rd_en    =  0;
    HI_U32 gdm_gamma_para_data    =  0;

    //add_regs
    HI_U32 g_gmm_x8_step = 0;
    HI_U32 g_gmm_x7_step = 0;
    HI_U32 g_gmm_x6_step = 0;
    HI_U32 g_gmm_x5_step = 0;
    HI_U32 g_gmm_x6_pos = 0;
    HI_U32 g_gmm_x5_pos = 0;
    HI_U32 g_gmm_x8_pos = 0;
    HI_U32 g_gmm_x7_pos = 0;
    HI_U32 g_gmm_x8_num = 0;
    HI_U32 g_gmm_x7_num = 0;
    HI_U32 g_gmm_x6_num = 0;
    HI_U32 g_gmm_x5_num = 0;

    HI_U32 g_gmm_x4_step    =  0;
    HI_U32 g_gmm_x3_step    =  0;
    HI_U32 g_gmm_x2_step    =  0;
    HI_U32 g_gmm_x1_step    =  0;
    HI_U32 g_gmm_x2_pos    =  0;
    HI_U32 g_gmm_x1_pos    =  0;
    HI_U32 g_gmm_x4_pos    =  0;
    HI_U32 g_gmm_x3_pos    =  0;
    HI_U32 g_gmm_x4_num    =  0;
    HI_U32 g_gmm_x3_num    =  0;
    HI_U32 g_gmm_x2_num    =  0;
    HI_U32 g_gmm_x1_num    =  0;
    HI_S16 gdm_coef00_csc    =  0;
    HI_S16 gdm_coef01_csc    =  0;
    HI_S16 gdm_coef02_csc    =  0;
    HI_S16 gdm_coef10_csc    =  0;
    HI_S16 gdm_coef11_csc    =  0;
    HI_S16 gdm_coef12_csc    =  0;
    HI_S16 gdm_coef20_csc    =  0;
    HI_S16 gdm_coef21_csc    =  0;
    HI_S16 gdm_coef22_csc    =  0;
    HI_U16 gdm_csc_scale    =  0;
    HI_U32 gdm_csc_clip_min    =  0;
    HI_U32 gdm_csc_clip_max    =  0;
    HI_S16 gdm_00_rgb2yuv    =  0;
    HI_S16 gdm_01_rgb2yuv    =  0;
    HI_S16 gdm_02_rgb2yuv    =  0;
    HI_S16 gdm_10_rgb2yuv    =  0;
    HI_S16 gdm_11_rgb2yuv    =  0;
    HI_S16 gdm_12_rgb2yuv    =  0;
    HI_S16 gdm_20_rgb2yuv    =  0;
    HI_S16 gdm_21_rgb2yuv    =  0;
    HI_S16 gdm_22_rgb2yuv    =  0;
    HI_U16 gdm_rgb2yuvscale2p    =  0;
    HI_S16 gdm_rgb2yuv_0_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_1_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_2_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_0_out_dc    =  0;
    HI_S16 gdm_rgb2yuv_1_out_dc    =  0;
    HI_S16 gdm_rgb2yuv_2_out_dc    =  0;
    HI_U16 gdm_rgb2yuv_clip_min    =  0;
    HI_U16 gdm_rgb2yuv_clip_max    =  0;
    HI_U32 gdm_dither_round_unlim    =  0;
    HI_U32 gdm_dither_round    =  0;
    HI_U32 gdm_dither_domain_mode    =  0;
    HI_U32 gdm_dither_tap_mode    =  0;
    HI_U32 gdm_dither_sed_y0    =  0;
    HI_U32 gdm_dither_sed_u0    =  0;
    HI_U32 gdm_dither_sed_v0    =  0;
    HI_U32 gdm_dither_sed_w0    =  0;
    HI_U32 gdm_dither_sed_y1    =  0;
    HI_U32 gdm_dither_sed_u1    =  0;
    HI_U32 gdm_dither_sed_v1    =  0;
    HI_U32 gdm_dither_sed_w1    =  0;
    HI_U32 gdm_dither_sed_y2    =  0;
    HI_U32 gdm_dither_sed_u2    =  0;
    HI_U32 gdm_dither_sed_v2    =  0;
    HI_U32 gdm_dither_sed_w2    =  0;
    HI_U32 gdm_dither_sed_y3    =  0;
    HI_U32 gdm_dither_sed_u3    =  0;
    HI_U32 gdm_dither_sed_v3    =  0;
    HI_U32 gdm_dither_sed_w3    =  0;
    HI_U32 gdm_dither_thr_max    =  0;
    HI_U32 gdm_dither_thr_min    =  0;
    gdm_tmap_in_bdp   = pstDmCfg->vdm_tmap_in_bdp ;
    gdm_gmm_en        = pstDmCfg->vdm_gmm_en      ;
    gdm_tmap_en       = pstDmCfg->vdm_tmap_en     ;
    gdm_degmm_en      = pstDmCfg->vdm_degmm_en    ;
    gdm_en            = pstDmCfg->vdm_en          ;
    gdm_rgb2yuv_en    = pstDmCfg->vdm_rgb2yuv_en  ;
    gdm_csc_en        = pstDmCfg->vdm_csc_en      ;
    gdm_dither_en     = pstDmCfg->vdm_dither_en   ;
    gdm_dither_mode   = pstDmCfg->vdm_dither_mode ;

    gdm_luma_coef0_tmap       = pstDmCfg->u16M3LumaCal[0] ;// ;
    gdm_luma_coef1_tmap       = pstDmCfg->u16M3LumaCal[1] ;// ;
    gdm_luma_coef2_tmap       = pstDmCfg->u16M3LumaCal[2] ;// ;
    gdm_tmap_luma_scale       = pstDmCfg->u16ScaleLumaCal ;// ;
    gdm_tmap_coef_scale       = pstDmCfg->u16ScaleCoefTM  ;// ;
    gdm_tmap_out_clip_min     = pstDmCfg->u16ClipMinTM    ;// ;
    gdm_tmap_out_clip_max     = pstDmCfg->u16ClipMaxTM    ;// ;

    g_degmm_x1_step   = pstDmCfg->DePQPara.gmm_x1_step   ;// ;
    g_degmm_x2_step   = pstDmCfg->DePQPara.gmm_x2_step   ;// ;
    g_degmm_x3_step   = pstDmCfg->DePQPara.gmm_x3_step   ;// ;
    g_degmm_x4_step   = pstDmCfg->DePQPara.gmm_x4_step   ;// ;
    g_degmm_x1_pos    = pstDmCfg->DePQPara.gmm_x1_pos    ;// ;
    g_degmm_x2_pos    = pstDmCfg->DePQPara.gmm_x2_pos    ;// ;
    g_degmm_x3_pos    = pstDmCfg->DePQPara.gmm_x3_pos    ;// ;
    g_degmm_x4_pos    = pstDmCfg->DePQPara.gmm_x4_pos    ;// ;
    g_degmm_x1_num    = pstDmCfg->DePQPara.gmm_x1_num    ;// ;
    g_degmm_x2_num    = pstDmCfg->DePQPara.gmm_x2_num    ;// ;
    g_degmm_x3_num    = pstDmCfg->DePQPara.gmm_x3_num    ;// ;
    g_degmm_x4_num    = pstDmCfg->DePQPara.gmm_x4_num    ;// ;


    g_tmap_x1_step     = pstDmCfg->TMPara.gmm_x1_step ;// ;
    g_tmap_x2_step     = pstDmCfg->TMPara.gmm_x2_step ;// ;
    g_tmap_x3_step     = pstDmCfg->TMPara.gmm_x3_step ;// ;
    g_tmap_x4_step     = pstDmCfg->TMPara.gmm_x4_step ;// ;
    g_tmap_x1_pos      = pstDmCfg->TMPara.gmm_x1_pos  ;// ;
    g_tmap_x2_pos      = pstDmCfg->TMPara.gmm_x2_pos  ;// ;
    g_tmap_x3_pos      = pstDmCfg->TMPara.gmm_x3_pos  ;// ;
    g_tmap_x4_pos      = pstDmCfg->TMPara.gmm_x4_pos  ;// ;
    g_tmap_x1_num      = pstDmCfg->TMPara.gmm_x1_num  ;// ;
    g_tmap_x2_num      = pstDmCfg->TMPara.gmm_x2_num  ;// ;
    g_tmap_x3_num      = pstDmCfg->TMPara.gmm_x3_num  ;// ;
    g_tmap_x4_num      = pstDmCfg->TMPara.gmm_x4_num  ;// ;

    g_gmm_x1_step     = pstDmCfg->GammaPara.gmm_x1_step ;// ;
    g_gmm_x2_step     = pstDmCfg->GammaPara.gmm_x2_step ;// ;
    g_gmm_x3_step     = pstDmCfg->GammaPara.gmm_x3_step ;// ;
    g_gmm_x4_step     = pstDmCfg->GammaPara.gmm_x4_step ;// ;
    g_gmm_x5_step     = pstDmCfg->GammaPara.gmm_x5_step ;// ;
    g_gmm_x6_step     = pstDmCfg->GammaPara.gmm_x6_step ;// ;
    g_gmm_x7_step     = pstDmCfg->GammaPara.gmm_x7_step ;// ;
    g_gmm_x8_step     = pstDmCfg->GammaPara.gmm_x8_step ;// ;
    g_gmm_x1_pos      = pstDmCfg->GammaPara.gmm_x1_pos  ;// ;
    g_gmm_x2_pos      = pstDmCfg->GammaPara.gmm_x2_pos  ;// ;
    g_gmm_x3_pos      = pstDmCfg->GammaPara.gmm_x3_pos  ;// ;
    g_gmm_x4_pos      = pstDmCfg->GammaPara.gmm_x4_pos  ;// ;
    g_gmm_x5_pos      = pstDmCfg->GammaPara.gmm_x5_pos  ;// ;
    g_gmm_x6_pos      = pstDmCfg->GammaPara.gmm_x6_pos  ;// ;
    g_gmm_x7_pos      = pstDmCfg->GammaPara.gmm_x7_pos  ;// ;
    g_gmm_x8_pos      = pstDmCfg->GammaPara.gmm_x8_pos  ;// ;
    g_gmm_x1_num      = pstDmCfg->GammaPara.gmm_x1_num  ;// ;
    g_gmm_x2_num      = pstDmCfg->GammaPara.gmm_x2_num  ;// ;
    g_gmm_x3_num      = pstDmCfg->GammaPara.gmm_x3_num  ;// ;
    g_gmm_x4_num      = pstDmCfg->GammaPara.gmm_x4_num  ;// ;
    g_gmm_x5_num      = pstDmCfg->GammaPara.gmm_x5_num  ;// ;
    g_gmm_x6_num      = pstDmCfg->GammaPara.gmm_x6_num  ;// ;
    g_gmm_x7_num      = pstDmCfg->GammaPara.gmm_x7_num  ;// ;
    g_gmm_x8_num      = pstDmCfg->GammaPara.gmm_x8_num  ;// ;

    gdm_coef00_csc       = pstDmCfg->s16M33Src2Disp[0][0] ;// ;
    gdm_coef01_csc       = pstDmCfg->s16M33Src2Disp[0][1] ;// ;
    gdm_coef02_csc       = pstDmCfg->s16M33Src2Disp[0][2] ;// ;
    gdm_coef10_csc       = pstDmCfg->s16M33Src2Disp[1][0] ;// ;
    gdm_coef11_csc       = pstDmCfg->s16M33Src2Disp[1][1] ;// ;
    gdm_coef12_csc       = pstDmCfg->s16M33Src2Disp[1][2] ;// ;
    gdm_coef20_csc       = pstDmCfg->s16M33Src2Disp[2][0] ;// ;
    gdm_coef21_csc       = pstDmCfg->s16M33Src2Disp[2][1] ;// ;
    gdm_coef22_csc       = pstDmCfg->s16M33Src2Disp[2][2] ;// ;
    gdm_csc_scale        = pstDmCfg->u16ScaleSrc2Disp     ;// ;
    gdm_csc_clip_min     = pstDmCfg->u32ClipMinCSC        ;// ;
    gdm_csc_clip_max     = pstDmCfg->u32ClipMaxCSC        ;// ;

    gdm_rgb2yuvscale2p     = pstDmCfg->u16ScaleRGB2YUV         ; // ;
    gdm_rgb2yuv_0_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[0]   ; // ;
    gdm_rgb2yuv_1_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[1]   ; // ;
    gdm_rgb2yuv_2_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[2]   ; // ;
    gdm_rgb2yuv_0_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[0]  ; // ;
    gdm_rgb2yuv_1_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[1]  ; // ;
    gdm_rgb2yuv_2_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[2]  ; // ;
    gdm_rgb2yuv_clip_min   = pstDmCfg->u16ClipMinRGB2YUV       ; // ;
    gdm_rgb2yuv_clip_max   = pstDmCfg->u16ClipMaxRGB2YUV       ; // ;

    gdm_00_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][0];
    gdm_01_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][1];
    gdm_02_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][2];
    gdm_10_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][0];
    gdm_11_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][1];
    gdm_12_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][2];
    gdm_20_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][0];
    gdm_21_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][1];
    gdm_22_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][2];

    gdm_dither_round       = pstDmCfg->stDitherCfg.u32_dither_round       ;
    gdm_dither_domain_mode = pstDmCfg->stDitherCfg.u32_dither_domain_mode ;
    gdm_dither_tap_mode    = pstDmCfg->stDitherCfg.u32_dither_tap_mode    ;
    gdm_dither_round_unlim = pstDmCfg->stDitherCfg.u32_dither_round_unlim ;
    gdm_dither_sed_y0      = pstDmCfg->stDitherCfg.u32_dither_sed_y0      ;
    gdm_dither_sed_u0      = pstDmCfg->stDitherCfg.u32_dither_sed_u0      ;
    gdm_dither_sed_v0      = pstDmCfg->stDitherCfg.u32_dither_sed_v0      ;
    gdm_dither_sed_w0      = pstDmCfg->stDitherCfg.u32_dither_sed_w0      ;
    gdm_dither_sed_y1      = pstDmCfg->stDitherCfg.u32_dither_sed_y1      ;
    gdm_dither_sed_u1      = pstDmCfg->stDitherCfg.u32_dither_sed_u1      ;
    gdm_dither_sed_v1      = pstDmCfg->stDitherCfg.u32_dither_sed_v1      ;
    gdm_dither_sed_w1      = pstDmCfg->stDitherCfg.u32_dither_sed_w1      ;
    gdm_dither_thr_max     = pstDmCfg->stDitherCfg.u32_dither_thr_max     ;
    gdm_dither_thr_min     = pstDmCfg->stDitherCfg.u32_dither_thr_min     ;



#if 0
    //reg value print
    VDP_PRINT("gdm_tmap_in_bdp = %d \n",gdm_tmap_in_bdp);
    VDP_PRINT("gdm_gmm_en = %d \n",gdm_gmm_en);
    VDP_PRINT("gdm_tmap_en = %d \n",gdm_tmap_en);
    VDP_PRINT("gdm_degmm_en = %d \n",gdm_degmm_en);
    VDP_PRINT("gdm_en = %d \n",gdm_en);
    VDP_PRINT("gdm_luma_coef0_tmap = %d \n",gdm_luma_coef0_tmap);
    VDP_PRINT("gdm_luma_coef1_tmap = %d \n",gdm_luma_coef1_tmap);
    VDP_PRINT("gdm_luma_coef2_tmap = %d \n",gdm_luma_coef2_tmap);
    VDP_PRINT("gdm_tmap_luma_scale = %d \n",gdm_tmap_luma_scale);
    VDP_PRINT("gdm_tmap_coef_scale = %d \n",gdm_tmap_coef_scale);
    VDP_PRINT("gdm_tmap_out_clip_min = %d \n",gdm_tmap_out_clip_min);
    VDP_PRINT("gdm_tmap_out_clip_max = %d \n",gdm_tmap_out_clip_max);
    VDP_PRINT("gdm_para_gmm_addr = %d \n",gdm_para_gmm_addr);
    VDP_PRINT("gdm_para_tmap_addr = %d \n",gdm_para_tmap_addr);
    VDP_PRINT("gdm_para_tmap_upd = %d \n",gdm_para_tmap_upd);
    VDP_PRINT("gdm_para_gmm_upd = %d \n",gdm_para_gmm_upd);
    VDP_PRINT("g_degmm_x4_step = %d \n",g_degmm_x4_step);
    VDP_PRINT("g_degmm_x3_step = %d \n",g_degmm_x3_step);
    VDP_PRINT("g_degmm_x2_step = %d \n",g_degmm_x2_step);
    VDP_PRINT("g_degmm_x1_step = %d \n",g_degmm_x1_step);
    VDP_PRINT("g_degmm_x2_pos = %d \n",g_degmm_x2_pos);
    VDP_PRINT("g_degmm_x1_pos = %d \n",g_degmm_x1_pos);
    VDP_PRINT("g_degmm_x4_pos = %d \n",g_degmm_x4_pos);
    VDP_PRINT("g_degmm_x3_pos = %d \n",g_degmm_x3_pos);
    VDP_PRINT("g_degmm_x4_num = %d \n",g_degmm_x4_num);
    VDP_PRINT("g_degmm_x3_num = %d \n",g_degmm_x3_num);
    VDP_PRINT("g_degmm_x2_num = %d \n",g_degmm_x2_num);
    VDP_PRINT("g_degmm_x1_num = %d \n",g_degmm_x1_num);
    VDP_PRINT("gdm_tmap_rd_en = %d \n",gdm_tmap_rd_en);
    VDP_PRINT("gdm_tmap_para_data = %d \n",gdm_tmap_para_data);
    VDP_PRINT("g_tmap_x4_step = %d \n",g_tmap_x4_step);
    VDP_PRINT("g_tmap_x3_step = %d \n",g_tmap_x3_step);
    VDP_PRINT("g_tmap_x2_step = %d \n",g_tmap_x2_step);
    VDP_PRINT("g_tmap_x1_step = %d \n",g_tmap_x1_step);
    VDP_PRINT("g_tmap_x1_pos = %d \n",g_tmap_x1_pos);
    VDP_PRINT("g_tmap_x2_pos = %d \n",g_tmap_x2_pos);
    VDP_PRINT("g_tmap_x4_num = %d \n",g_tmap_x4_num);
    VDP_PRINT("g_tmap_x3_num = %d \n",g_tmap_x3_num);
    VDP_PRINT("g_tmap_x2_num = %d \n",g_tmap_x2_num);
    VDP_PRINT("g_tmap_x1_num = %d \n",g_tmap_x1_num);
    VDP_PRINT("g_tmap_x3_pos = %d \n",g_tmap_x3_pos);
    VDP_PRINT("g_tmap_x4_pos = %d \n",g_tmap_x4_pos);
    VDP_PRINT("gdm_gamma_rd_en = %d \n",gdm_gamma_rd_en);
    VDP_PRINT("gdm_gamma_para_data = %d \n",gdm_gamma_para_data);
    VDP_PRINT("g_gmm_x4_step = %d \n",g_gmm_x4_step);
    VDP_PRINT("g_gmm_x3_step = %d \n",g_gmm_x3_step);
    VDP_PRINT("g_gmm_x2_step = %d \n",g_gmm_x2_step);
    VDP_PRINT("g_gmm_x1_step = %d \n",g_gmm_x1_step);
    VDP_PRINT("g_gmm_x2_pos = %d \n",g_gmm_x2_pos);
    VDP_PRINT("g_gmm_x1_pos = %d \n",g_gmm_x1_pos);
    VDP_PRINT("g_gmm_x4_pos = %d \n",g_gmm_x4_pos);
    VDP_PRINT("g_gmm_x3_pos = %d \n",g_gmm_x3_pos);
    VDP_PRINT("g_gmm_x4_num = %d \n",g_gmm_x4_num);
    VDP_PRINT("g_gmm_x3_num = %d \n",g_gmm_x3_num);
    VDP_PRINT("g_gmm_x2_num = %d \n",g_gmm_x2_num);
    VDP_PRINT("g_gmm_x1_num = %d \n",g_gmm_x1_num);
    VDP_PRINT("gdm_csc_en = %d \n",gdm_csc_en);
    VDP_PRINT("gdm_coef00_csc = %d \n",gdm_coef00_csc);
    VDP_PRINT("gdm_coef01_csc = %d \n",gdm_coef01_csc);
    VDP_PRINT("gdm_coef02_csc = %d \n",gdm_coef02_csc);
    VDP_PRINT("gdm_coef10_csc = %d \n",gdm_coef10_csc);
    VDP_PRINT("gdm_coef11_csc = %d \n",gdm_coef11_csc);
    VDP_PRINT("gdm_coef12_csc = %d \n",gdm_coef12_csc);
    VDP_PRINT("gdm_coef20_csc = %d \n",gdm_coef20_csc);
    VDP_PRINT("gdm_coef21_csc = %d \n",gdm_coef21_csc);
    VDP_PRINT("gdm_coef22_csc = %d \n",gdm_coef22_csc);
    VDP_PRINT("gdm_csc_scale = %d \n",gdm_csc_scale);
    VDP_PRINT("gdm_csc_clip_min = %d \n",gdm_csc_clip_min);
    VDP_PRINT("gdm_csc_clip_max = %d \n",gdm_csc_clip_max);
    VDP_PRINT("gdm_rgb2yuv_en = %d \n",gdm_rgb2yuv_en);
    VDP_PRINT("gdm_00_rgb2yuv = %d \n",gdm_00_rgb2yuv);
    VDP_PRINT("gdm_01_rgb2yuv = %d \n",gdm_01_rgb2yuv);
    VDP_PRINT("gdm_02_rgb2yuv = %d \n",gdm_02_rgb2yuv);
    VDP_PRINT("gdm_10_rgb2yuv = %d \n",gdm_10_rgb2yuv);
    VDP_PRINT("gdm_11_rgb2yuv = %d \n",gdm_11_rgb2yuv);
    VDP_PRINT("gdm_12_rgb2yuv = %d \n",gdm_12_rgb2yuv);
    VDP_PRINT("gdm_20_rgb2yuv = %d \n",gdm_20_rgb2yuv);
    VDP_PRINT("gdm_21_rgb2yuv = %d \n",gdm_21_rgb2yuv);
    VDP_PRINT("gdm_22_rgb2yuv = %d \n",gdm_22_rgb2yuv);
    VDP_PRINT("gdm_rgb2yuvscale2p = %d \n",gdm_rgb2yuvscale2p);
    VDP_PRINT("gdm_rgb2yuv_0_in_dc = %d \n",gdm_rgb2yuv_0_in_dc);
    VDP_PRINT("gdm_rgb2yuv_1_in_dc = %d \n",gdm_rgb2yuv_1_in_dc);
    VDP_PRINT("gdm_rgb2yuv_2_in_dc = %d \n",gdm_rgb2yuv_2_in_dc);
    VDP_PRINT("gdm_rgb2yuv_0_out_dc = %d \n",gdm_rgb2yuv_0_out_dc);
    VDP_PRINT("gdm_rgb2yuv_1_out_dc = %d \n",gdm_rgb2yuv_1_out_dc);
    VDP_PRINT("gdm_rgb2yuv_2_out_dc = %d \n",gdm_rgb2yuv_2_out_dc);
    VDP_PRINT("gdm_rgb2yuv_clip_min = %d \n",gdm_rgb2yuv_clip_min);
    VDP_PRINT("gdm_rgb2yuv_clip_max = %d \n",gdm_rgb2yuv_clip_max);
    VDP_PRINT("gdm_dither_round_unlim = %d \n",gdm_dither_round_unlim);
    VDP_PRINT("gdm_dither_en = %d \n",gdm_dither_en);
    VDP_PRINT("gdm_dither_mode = %d \n",gdm_dither_mode);
    VDP_PRINT("gdm_dither_round = %d \n",gdm_dither_round);
    VDP_PRINT("gdm_dither_domain_mode = %d \n",gdm_dither_domain_mode);
    VDP_PRINT("gdm_dither_tap_mode = %d \n",gdm_dither_tap_mode);
    VDP_PRINT("gdm_dither_sed_y0 = %d \n",gdm_dither_sed_y0);
    VDP_PRINT("gdm_dither_sed_u0 = %d \n",gdm_dither_sed_u0);
    VDP_PRINT("gdm_dither_sed_v0 = %d \n",gdm_dither_sed_v0);
    VDP_PRINT("gdm_dither_sed_w0 = %d \n",gdm_dither_sed_w0);
    VDP_PRINT("gdm_dither_sed_y1 = %d \n",gdm_dither_sed_y1);
    VDP_PRINT("gdm_dither_sed_u1 = %d \n",gdm_dither_sed_u1);
    VDP_PRINT("gdm_dither_sed_v1 = %d \n",gdm_dither_sed_v1);
    VDP_PRINT("gdm_dither_sed_w1 = %d \n",gdm_dither_sed_w1);
    VDP_PRINT("gdm_dither_sed_y2 = %d \n",gdm_dither_sed_y2);
    VDP_PRINT("gdm_dither_sed_u2 = %d \n",gdm_dither_sed_u2);
    VDP_PRINT("gdm_dither_sed_v2 = %d \n",gdm_dither_sed_v2);
    VDP_PRINT("gdm_dither_sed_w2 = %d \n",gdm_dither_sed_w2);
    VDP_PRINT("gdm_dither_sed_y3 = %d \n",gdm_dither_sed_y3);
    VDP_PRINT("gdm_dither_sed_u3 = %d \n",gdm_dither_sed_u3);
    VDP_PRINT("gdm_dither_sed_v3 = %d \n",gdm_dither_sed_v3);
    VDP_PRINT("gdm_dither_sed_w3 = %d \n",gdm_dither_sed_w3);
    VDP_PRINT("gdm_dither_thr_max = %d \n",gdm_dither_thr_max);
    VDP_PRINT("gdm_dither_thr_min = %d \n",gdm_dither_thr_min);
#endif
    //drv transfer
    VDP_GP_SetGdmGdmTmapInBdp(VDP_LAYER_GP0, gdm_tmap_in_bdp);
    VDP_GP_SetGdmGdmGmmEn(VDP_LAYER_GP0, gdm_gmm_en);
    VDP_GP_SetGdmGdmTmapEn(VDP_LAYER_GP0, gdm_tmap_en);
    VDP_GP_SetGdmGdmDegmmEn(VDP_LAYER_GP0, gdm_degmm_en);
    VDP_GP_SetGdmGdmEn(VDP_LAYER_GP0, gdm_en);
    VDP_GP_SetGdmGdmLumaCoef0Tmap(VDP_LAYER_GP0, gdm_luma_coef0_tmap);
    VDP_GP_SetGdmGdmLumaCoef1Tmap(VDP_LAYER_GP0, gdm_luma_coef1_tmap);
    VDP_GP_SetGdmGdmLumaCoef2Tmap(VDP_LAYER_GP0, gdm_luma_coef2_tmap);
    VDP_GP_SetGdmGdmTmapLumaScale(VDP_LAYER_GP0, gdm_tmap_luma_scale);
    VDP_GP_SetGdmGdmTmapCoefScale(VDP_LAYER_GP0, gdm_tmap_coef_scale);
    VDP_GP_SetGdmGdmTmapOutClipMin(VDP_LAYER_GP0, gdm_tmap_out_clip_min);
    VDP_GP_SetGdmGdmTmapOutClipMax(VDP_LAYER_GP0, gdm_tmap_out_clip_max);
    //VDP_GP_SetGdmGdmParaGmmAddr(VDP_LAYER_GP0, gdm_para_gmm_addr);
    //VDP_GP_SetGdmGdmParaTmapAddr(VDP_LAYER_GP0, gdm_para_tmap_addr);
    //VDP_GP_SetGdmGdmParaTmapUpd(VDP_LAYER_GP0, gdm_para_tmap_upd);
    //VDP_GP_SetGdmGdmParaGmmUpd(VDP_LAYER_GP0, gdm_para_gmm_upd);
    //add_regs
    VDP_GP_SetGdmGGmmX5Num(VDP_LAYER_GP0,g_gmm_x5_num);
    VDP_GP_SetGdmGGmmX6Num(VDP_LAYER_GP0,g_gmm_x6_num);
    VDP_GP_SetGdmGGmmX7Num(VDP_LAYER_GP0,g_gmm_x7_num);
    VDP_GP_SetGdmGGmmX8Num(VDP_LAYER_GP0,g_gmm_x8_num);
    VDP_GP_SetGdmGGmmX7Pos(VDP_LAYER_GP0,g_gmm_x7_pos);
    VDP_GP_SetGdmGGmmX8Pos(VDP_LAYER_GP0,g_gmm_x8_pos);
    VDP_GP_SetGdmGGmmX5Pos(VDP_LAYER_GP0,g_gmm_x5_pos);
    VDP_GP_SetGdmGGmmX6Pos(VDP_LAYER_GP0,g_gmm_x6_pos);
    VDP_GP_SetGdmGGmmX5Step(VDP_LAYER_GP0,g_gmm_x5_step);
    VDP_GP_SetGdmGGmmX6Step(VDP_LAYER_GP0,g_gmm_x6_step);
    VDP_GP_SetGdmGGmmX7Step(VDP_LAYER_GP0,g_gmm_x7_step);
    VDP_GP_SetGdmGGmmX8Step(VDP_LAYER_GP0,g_gmm_x8_step);

    VDP_GP_SetGdmGDegmmX4Step(VDP_LAYER_GP0, g_degmm_x4_step);
    VDP_GP_SetGdmGDegmmX3Step(VDP_LAYER_GP0, g_degmm_x3_step);
    VDP_GP_SetGdmGDegmmX2Step(VDP_LAYER_GP0, g_degmm_x2_step);
    VDP_GP_SetGdmGDegmmX1Step(VDP_LAYER_GP0, g_degmm_x1_step);
    VDP_GP_SetGdmGDegmmX2Pos(VDP_LAYER_GP0, g_degmm_x2_pos);
    VDP_GP_SetGdmGDegmmX1Pos(VDP_LAYER_GP0, g_degmm_x1_pos);
    VDP_GP_SetGdmGDegmmX4Pos(VDP_LAYER_GP0, g_degmm_x4_pos);
    VDP_GP_SetGdmGDegmmX3Pos(VDP_LAYER_GP0, g_degmm_x3_pos);
    VDP_GP_SetGdmGDegmmX4Num(VDP_LAYER_GP0, g_degmm_x4_num);
    VDP_GP_SetGdmGDegmmX3Num(VDP_LAYER_GP0, g_degmm_x3_num);
    VDP_GP_SetGdmGDegmmX2Num(VDP_LAYER_GP0, g_degmm_x2_num);
    VDP_GP_SetGdmGDegmmX1Num(VDP_LAYER_GP0, g_degmm_x1_num);
    VDP_GP_SetGdmGdmTmapRdEn(VDP_LAYER_GP0, gdm_tmap_rd_en);
    VDP_GP_SetGdmGdmTmapParaData(VDP_LAYER_GP0, gdm_tmap_para_data);
    VDP_GP_SetGdmGTmapX4Step(VDP_LAYER_GP0, g_tmap_x4_step);
    VDP_GP_SetGdmGTmapX3Step(VDP_LAYER_GP0, g_tmap_x3_step);
    VDP_GP_SetGdmGTmapX2Step(VDP_LAYER_GP0, g_tmap_x2_step);
    VDP_GP_SetGdmGTmapX1Step(VDP_LAYER_GP0, g_tmap_x1_step);
    VDP_GP_SetGdmGTmapX1Pos(VDP_LAYER_GP0, g_tmap_x1_pos);
    VDP_GP_SetGdmGTmapX2Pos(VDP_LAYER_GP0, g_tmap_x2_pos);
    VDP_GP_SetGdmGTmapX4Num(VDP_LAYER_GP0, g_tmap_x4_num);
    VDP_GP_SetGdmGTmapX3Num(VDP_LAYER_GP0, g_tmap_x3_num);
    VDP_GP_SetGdmGTmapX2Num(VDP_LAYER_GP0, g_tmap_x2_num);
    VDP_GP_SetGdmGTmapX1Num(VDP_LAYER_GP0, g_tmap_x1_num);
    VDP_GP_SetGdmGTmapX3Pos(VDP_LAYER_GP0, g_tmap_x3_pos);
    VDP_GP_SetGdmGTmapX4Pos(VDP_LAYER_GP0, g_tmap_x4_pos);
    VDP_GP_SetGdmGdmGammaRdEn(VDP_LAYER_GP0, gdm_gamma_rd_en);
    VDP_GP_SetGdmGdmGammaParaData(VDP_LAYER_GP0, gdm_gamma_para_data);
    VDP_GP_SetGdmGGmmX4Step(VDP_LAYER_GP0, g_gmm_x4_step);
    VDP_GP_SetGdmGGmmX3Step(VDP_LAYER_GP0, g_gmm_x3_step);
    VDP_GP_SetGdmGGmmX2Step(VDP_LAYER_GP0, g_gmm_x2_step);
    VDP_GP_SetGdmGGmmX1Step(VDP_LAYER_GP0, g_gmm_x1_step);
    VDP_GP_SetGdmGGmmX2Pos(VDP_LAYER_GP0, g_gmm_x2_pos);
    VDP_GP_SetGdmGGmmX1Pos(VDP_LAYER_GP0, g_gmm_x1_pos);
    VDP_GP_SetGdmGGmmX4Pos(VDP_LAYER_GP0, g_gmm_x4_pos);
    VDP_GP_SetGdmGGmmX3Pos(VDP_LAYER_GP0, g_gmm_x3_pos);
    VDP_GP_SetGdmGGmmX4Num(VDP_LAYER_GP0, g_gmm_x4_num);
    VDP_GP_SetGdmGGmmX3Num(VDP_LAYER_GP0, g_gmm_x3_num);
    VDP_GP_SetGdmGGmmX2Num(VDP_LAYER_GP0, g_gmm_x2_num);
    VDP_GP_SetGdmGGmmX1Num(VDP_LAYER_GP0, g_gmm_x1_num);
    VDP_GP_SetGdmGdmCscEn(VDP_LAYER_GP0, gdm_csc_en);
    VDP_GP_SetGdmGdmCoef00Csc(VDP_LAYER_GP0, gdm_coef00_csc);
    VDP_GP_SetGdmGdmCoef01Csc(VDP_LAYER_GP0, gdm_coef01_csc);
    VDP_GP_SetGdmGdmCoef02Csc(VDP_LAYER_GP0, gdm_coef02_csc);
    VDP_GP_SetGdmGdmCoef10Csc(VDP_LAYER_GP0, gdm_coef10_csc);
    VDP_GP_SetGdmGdmCoef11Csc(VDP_LAYER_GP0, gdm_coef11_csc);
    VDP_GP_SetGdmGdmCoef12Csc(VDP_LAYER_GP0, gdm_coef12_csc);
    VDP_GP_SetGdmGdmCoef20Csc(VDP_LAYER_GP0, gdm_coef20_csc);
    VDP_GP_SetGdmGdmCoef21Csc(VDP_LAYER_GP0, gdm_coef21_csc);
    VDP_GP_SetGdmGdmCoef22Csc(VDP_LAYER_GP0, gdm_coef22_csc);
    VDP_GP_SetGdmGdmCscScale(VDP_LAYER_GP0, gdm_csc_scale);
    VDP_GP_SetGdmGdmCscClipMin(VDP_LAYER_GP0, gdm_csc_clip_min);
    VDP_GP_SetGdmGdmCscClipMax(VDP_LAYER_GP0, gdm_csc_clip_max);
    VDP_GP_SetGdmGdmRgb2yuvEn(VDP_LAYER_GP0, gdm_rgb2yuv_en);
    VDP_GP_SetGdmGdm00Rgb2yuv(VDP_LAYER_GP0, gdm_00_rgb2yuv);
    VDP_GP_SetGdmGdm01Rgb2yuv(VDP_LAYER_GP0, gdm_01_rgb2yuv);
    VDP_GP_SetGdmGdm02Rgb2yuv(VDP_LAYER_GP0, gdm_02_rgb2yuv);
    VDP_GP_SetGdmGdm10Rgb2yuv(VDP_LAYER_GP0, gdm_10_rgb2yuv);
    VDP_GP_SetGdmGdm11Rgb2yuv(VDP_LAYER_GP0, gdm_11_rgb2yuv);
    VDP_GP_SetGdmGdm12Rgb2yuv(VDP_LAYER_GP0, gdm_12_rgb2yuv);
    VDP_GP_SetGdmGdm20Rgb2yuv(VDP_LAYER_GP0, gdm_20_rgb2yuv);
    VDP_GP_SetGdmGdm21Rgb2yuv(VDP_LAYER_GP0, gdm_21_rgb2yuv);
    VDP_GP_SetGdmGdm22Rgb2yuv(VDP_LAYER_GP0, gdm_22_rgb2yuv);
    VDP_GP_SetGdmGdmRgb2yuvscale2p(VDP_LAYER_GP0, gdm_rgb2yuvscale2p);
    VDP_GP_SetGdmGdmRgb2yuv0InDc(VDP_LAYER_GP0, gdm_rgb2yuv_0_in_dc);
    VDP_GP_SetGdmGdmRgb2yuv1InDc(VDP_LAYER_GP0, gdm_rgb2yuv_1_in_dc);
    VDP_GP_SetGdmGdmRgb2yuv2InDc(VDP_LAYER_GP0, gdm_rgb2yuv_2_in_dc);
    VDP_GP_SetGdmGdmRgb2yuv0OutDc(VDP_LAYER_GP0, gdm_rgb2yuv_0_out_dc);
    VDP_GP_SetGdmGdmRgb2yuv1OutDc(VDP_LAYER_GP0, gdm_rgb2yuv_1_out_dc);
    VDP_GP_SetGdmGdmRgb2yuv2OutDc(VDP_LAYER_GP0, gdm_rgb2yuv_2_out_dc);
    VDP_GP_SetGdmGdmRgb2yuvClipMin(VDP_LAYER_GP0, gdm_rgb2yuv_clip_min);
    VDP_GP_SetGdmGdmRgb2yuvClipMax(VDP_LAYER_GP0, gdm_rgb2yuv_clip_max);
    VDP_GP_SetGdmGdmDitherRoundUnlim(VDP_LAYER_GP0, gdm_dither_round_unlim);
    VDP_GP_SetGdmGdmDitherEn(VDP_LAYER_GP0, gdm_dither_en);
    VDP_GP_SetGdmGdmDitherMode(VDP_LAYER_GP0, gdm_dither_mode);
    VDP_GP_SetGdmGdmDitherRound(VDP_LAYER_GP0, gdm_dither_round);
    VDP_GP_SetGdmGdmDitherDomainMode(VDP_LAYER_GP0, gdm_dither_domain_mode);
    VDP_GP_SetGdmGdmDitherTapMode(VDP_LAYER_GP0, gdm_dither_tap_mode);
    VDP_GP_SetGdmGdmDitherSedY0(VDP_LAYER_GP0, gdm_dither_sed_y0);
    VDP_GP_SetGdmGdmDitherSedU0(VDP_LAYER_GP0, gdm_dither_sed_u0);
    VDP_GP_SetGdmGdmDitherSedV0(VDP_LAYER_GP0, gdm_dither_sed_v0);
    VDP_GP_SetGdmGdmDitherSedW0(VDP_LAYER_GP0, gdm_dither_sed_w0);
    VDP_GP_SetGdmGdmDitherSedY1(VDP_LAYER_GP0, gdm_dither_sed_y1);
    VDP_GP_SetGdmGdmDitherSedU1(VDP_LAYER_GP0, gdm_dither_sed_u1);
    VDP_GP_SetGdmGdmDitherSedV1(VDP_LAYER_GP0, gdm_dither_sed_v1);
    VDP_GP_SetGdmGdmDitherSedW1(VDP_LAYER_GP0, gdm_dither_sed_w1);
    VDP_GP_SetGdmGdmDitherSedY2(VDP_LAYER_GP0, gdm_dither_sed_y2);
    VDP_GP_SetGdmGdmDitherSedU2(VDP_LAYER_GP0, gdm_dither_sed_u2);
    VDP_GP_SetGdmGdmDitherSedV2(VDP_LAYER_GP0, gdm_dither_sed_v2);
    VDP_GP_SetGdmGdmDitherSedW2(VDP_LAYER_GP0, gdm_dither_sed_w2);
    VDP_GP_SetGdmGdmDitherSedY3(VDP_LAYER_GP0, gdm_dither_sed_y3);
    VDP_GP_SetGdmGdmDitherSedU3(VDP_LAYER_GP0, gdm_dither_sed_u3);
    VDP_GP_SetGdmGdmDitherSedV3(VDP_LAYER_GP0, gdm_dither_sed_v3);
    VDP_GP_SetGdmGdmDitherSedW3(VDP_LAYER_GP0, gdm_dither_sed_w3);
    VDP_GP_SetGdmGdmDitherThrMax(VDP_LAYER_GP0, gdm_dither_thr_max);
    VDP_GP_SetGdmGdmDitherThrMin(VDP_LAYER_GP0, gdm_dither_thr_min);

return HI_SUCCESS;

}


HI_VOID VDP_DRV_SetGdmMode(GDM_SCENE_MODE_E  enGdmSceneMode)
{
    GdmCfg_t  pstDmCfg;
    memset((HI_VOID *)&pstDmCfg,0,sizeof(pstDmCfg));

    pstDmCfg.vdm_tmap_in_bdp        = 0 ;//1:20bit for vdm , 0:16bit for gdm
    pstDmCfg.u16ClipMinTM           = 0     ;
    pstDmCfg.u16ClipMaxTM           = 65535 ;
    pstDmCfg.u32ClipMinCSC          = 0     ;
    pstDmCfg.u32ClipMaxCSC          = 65535 ; //16bits
    pstDmCfg.u16ClipMinRGB2YUV      = 0     ;
    pstDmCfg.u16ClipMaxRGB2YUV      = 1023  ;


    pstDmCfg.GdmSceneMode = enGdmSceneMode ;
    VDP_DRV_GetHiHdrGfxCfg(&pstDmCfg);
    VDP_DRV_SetHiHdrGfxCfg(&pstDmCfg);

    //------------------------
    // adapter
    //------------------------



  //  free(pstDmCfg);

}

HI_S32 VDP_DRV_SetHiHdrCfg(DmCfg_t  *pstDmCfg)
{
        //filed declare
    HI_U32 vdm_tmap_in_bdp    =  0;
    HI_U32 vdm_csc_bind    =  0;
    HI_U32 vdm_gmm_en    =  0;
    HI_U32 vdm_tmap_en    =  0;
    HI_U32 vdm_degmm_en    =  0;
    HI_U32 vdm_en    =  0;
    HI_U16 vdm_luma_coef0_tmap    =  0;
    HI_U16 vdm_luma_coef1_tmap    =  0;
    HI_U16 vdm_luma_coef2_tmap    =  0;
    HI_U16 vdm_tmap_luma_scale    =  0;
    HI_U16 vdm_tmap_coef_scale    =  0;
    HI_U32 vdm_tmap_out_clip_min    =  0;
    HI_U32 vdm_tmap_out_clip_max    =  0;
    HI_S32 vdm_tmap_out_dc0    =  0;
    HI_S32 vdm_tmap_out_dc1    =  0;
    HI_S32 vdm_tmap_out_dc2    =  0;
    HI_U32 vdm_degamma_rd_en    =  0;
    HI_U32 vdm_degamma_para_data    =  0;
    HI_U32 v_degmm_x4_step    =  0;
    HI_U32 v_degmm_x3_step    =  0;
    HI_U32 v_degmm_x2_step    =  0;
    HI_U32 v_degmm_x1_step    =  0;
    HI_U32 v_degmm_x2_pos    =  0;
    HI_U32 v_degmm_x1_pos    =  0;
    HI_U32 v_degmm_x4_pos    =  0;
    HI_U32 v_degmm_x3_pos    =  0;
    HI_U32 v_degmm_x4_num    =  0;
    HI_U32 v_degmm_x3_num    =  0;
    HI_U32 v_degmm_x2_num    =  0;
    HI_U32 v_degmm_x1_num    =  0;
    HI_U32 vdm_tmap_rd_en    =  0;
    HI_U32 vdm_tmap_para_data    =  0;
    HI_U32 v_tmap_x4_step    =  0;
    HI_U32 v_tmap_x5_step    =  0;
    HI_U32 v_tmap_x6_step    =  0;
    HI_U32 v_tmap_x7_step    =  0;
    HI_U32 v_tmap_x8_step    =  0;
    HI_U32 v_tmap_x3_step    =  0;
    HI_U32 v_tmap_x2_step    =  0;
    HI_U32 v_tmap_x1_step    =  0;
    HI_U32 v_tmap_x1_pos    =  0;
    HI_U32 v_tmap_x2_pos    =  0;
    HI_U32 v_tmap_x4_num    =  0;
    HI_U32 v_tmap_x5_num    =  0;
    HI_U32 v_tmap_x6_num    =  0;
    HI_U32 v_tmap_x7_num    =  0;
    HI_U32 v_tmap_x8_num    =  0;
    HI_U32 v_tmap_x3_num    =  0;
    HI_U32 v_tmap_x2_num    =  0;
    HI_U32 v_tmap_x1_num    =  0;
    HI_U32 v_tmap_x3_pos    =  0;
    HI_U32 v_tmap_x4_pos    =  0;
    HI_U32 v_tmap_x5_pos    =  0;
    HI_U32 v_tmap_x6_pos    =  0;
    HI_U32 v_tmap_x7_pos    =  0;
    HI_U32 v_tmap_x8_pos    =  0;
    HI_U32 vdm_gamma_rd_en    =  0;
    HI_U32 vdm_gamma_para_data    =  0;
    HI_U32 v_gmm_x5_step    =  0;
    HI_U32 v_gmm_x6_step    =  0;
    HI_U32 v_gmm_x7_step    =  0;
    HI_U32 v_gmm_x8_step    =  0;
    HI_U32 v_gmm_x4_step    =  0;
    HI_U32 v_gmm_x3_step    =  0;
    HI_U32 v_gmm_x2_step    =  0;
    HI_U32 v_gmm_x1_step    =  0;
    HI_U32 v_gmm_x1_pos    =  0;
    HI_U32 v_gmm_x2_pos    =  0;
    HI_U32 v_gmm_x3_pos    =  0;
    HI_U32 v_gmm_x4_pos    =  0;
    HI_U32 v_gmm_x5_pos    =  0;
    HI_U32 v_gmm_x6_pos    =  0;
    HI_U32 v_gmm_x7_pos    =  0;
    HI_U32 v_gmm_x8_pos    =  0;
    HI_U32 v_gmm_x5_num    =  0;
    HI_U32 v_gmm_x6_num    =  0;
    HI_U32 v_gmm_x7_num    =  0;
    HI_U32 v_gmm_x8_num    =  0;
    HI_U32 v_gmm_x4_num    =  0;
    HI_U32 v_gmm_x3_num    =  0;
    HI_U32 v_gmm_x2_num    =  0;
    HI_U32 v_gmm_x1_num    =  0;
    HI_U32 vdm_csc_en    =  0;
    HI_S16 vdm_coef00_csc    =  0;
    HI_S16 vdm_coef01_csc    =  0;
    HI_S16 vdm_coef02_csc    =  0;
    HI_S16 vdm_coef10_csc    =  0;
    HI_S16 vdm_coef11_csc    =  0;
    HI_S16 vdm_coef12_csc    =  0;
    HI_S16 vdm_coef20_csc    =  0;
    HI_S16 vdm_coef21_csc    =  0;
    HI_S16 vdm_coef22_csc    =  0;
    HI_U16 vdm_csc_scale    =  0;
    HI_U32 vdm_csc_clip_min    =  0;
    HI_U32 vdm_csc_clip_max    =  0;
    HI_U32 vdm_v1_gamma_en    =  0;
    HI_U32 vdm_v0_gamma_en    =  0;
    HI_U32 vdm_v1_csc_en    =  0;
    HI_U32 vdm_v0_csc_en    =  0;
    HI_U32 vdm_yuv2rgb_en    =  0;
    HI_S16 vdm_00_yuv2rgb    =  0;
    HI_S16 vdm_01_yuv2rgb    =  0;
    HI_S16 vdm_02_yuv2rgb    =  0;
    HI_S16 vdm_03_yuv2rgb    =  0;
    HI_S16 vdm_04_yuv2rgb    =  0;
    HI_S16 vdm_05_yuv2rgb    =  0;
    HI_S16 vdm_06_yuv2rgb    =  0;
    HI_S16 vdm_07_yuv2rgb    =  0;
    HI_S16 vdm_08_yuv2rgb    =  0;
    HI_S16 vdm_10_yuv2rgb    =  0;
    HI_S16 vdm_11_yuv2rgb    =  0;
    HI_S16 vdm_12_yuv2rgb    =  0;
    HI_S16 vdm_13_yuv2rgb    =  0;
    HI_S16 vdm_14_yuv2rgb    =  0;
    HI_S16 vdm_15_yuv2rgb    =  0;
    HI_S16 vdm_16_yuv2rgb    =  0;
    HI_S16 vdm_17_yuv2rgb    =  0;
    HI_S16 vdm_18_yuv2rgb    =  0;
    HI_U16 vdm_yuv2rgbscale2p    =  0;
    HI_S32 vdm_yuv2rgb_00_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_01_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_02_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_00_out_dc    =  0;
    HI_S32 vdm_yuv2rgb_01_out_dc    =  0;
    HI_S32 vdm_yuv2rgb_02_out_dc    =  0;
    HI_S32 vdm_yuv2rgb_10_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_11_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_12_in_dc    =  0;
    HI_S32 vdm_yuv2rgb_10_out_dc    =  0;
    HI_S32 vdm_yuv2rgb_11_out_dc    =  0;
    HI_S32 vdm_yuv2rgb_12_out_dc    =  0;
    HI_U16 vdm_yuv2rgb_clip_min    =  0;
    HI_U16 vdm_yuv2rgb_clip_max    =  0;
    HI_U32 vdm_rgb2yuv_en    =  0;
    HI_S16 vdm_00_rgb2yuv    =  0;
    HI_S16 vdm_01_rgb2yuv    =  0;
    HI_S16 vdm_02_rgb2yuv    =  0;
    HI_S16 vdm_10_rgb2yuv    =  0;
    HI_S16 vdm_11_rgb2yuv    =  0;
    HI_S16 vdm_12_rgb2yuv    =  0;
    HI_S16 vdm_20_rgb2yuv    =  0;
    HI_S16 vdm_21_rgb2yuv    =  0;
    HI_S16 vdm_22_rgb2yuv    =  0;
    HI_U16 vdm_rgb2yuvscale2p    =  0;
    HI_S16 vdm_rgb2yuv_0_in_dc    =  0;
    HI_S16 vdm_rgb2yuv_1_in_dc    =  0;
    HI_S16 vdm_rgb2yuv_2_in_dc    =  0;
    HI_S16 vdm_rgb2yuv_0_out_dc    =  0;
    HI_S16 vdm_rgb2yuv_1_out_dc    =  0;
    HI_S16 vdm_rgb2yuv_2_out_dc    =  0;
    HI_U16 vdm_rgb2yuv_clip_min    =  0;
    HI_U16 vdm_rgb2yuv_clip_max    =  0;
    HI_U32 vdm_dither_round_unlim    =  0;
    HI_U32 vdm_dither_en    =  0;
    HI_U32 vdm_dither_round    =  0;
    HI_U32 vdm_dither_domain_mode    =  0;
    HI_U32 vdm_dither_tap_mode    =  0;
    HI_U32 vdm_dither_sed_y0    =  0;
    HI_U32 vdm_dither_sed_u0    =  0;
    HI_U32 vdm_dither_sed_v0    =  0;
    HI_U32 vdm_dither_sed_w0    =  0;
    HI_U32 vdm_dither_sed_y1    =  0;
    HI_U32 vdm_dither_sed_u1    =  0;
    HI_U32 vdm_dither_sed_v1    =  0;
    HI_U32 vdm_dither_sed_w1    =  0;
    HI_U32 vdm_dither_sed_y2    =  0;
    HI_U32 vdm_dither_sed_u2    =  0;
    HI_U32 vdm_dither_sed_v2    =  0;
    HI_U32 vdm_dither_sed_w2    =  0;
    HI_U32 vdm_dither_sed_y3    =  0;
    HI_U32 vdm_dither_sed_u3    =  0;
    HI_U32 vdm_dither_sed_v3    =  0;
    HI_U32 vdm_dither_sed_w3    =  0;
    HI_U32 vdm_dither_thr_max    =  0;
    HI_U32 vdm_dither_thr_min    =  0;


    vdm_tmap_in_bdp   = pstDmCfg->vdm_tmap_in_bdp ;
    vdm_csc_bind      = pstDmCfg->vdm_csc_bind    ;
    vdm_gmm_en        = pstDmCfg->vdm_gmm_en      ;
    vdm_tmap_en       = pstDmCfg->vdm_tmap_en     ;
    vdm_degmm_en      = pstDmCfg->vdm_degmm_en    ;
    vdm_en            = pstDmCfg->vdm_en          ;
    vdm_v1_gamma_en   = pstDmCfg->vdm_v1_gamma_en ;
    vdm_v0_gamma_en   = pstDmCfg->vdm_v0_gamma_en ;
    vdm_v1_csc_en     = pstDmCfg->vdm_v1_csc_en   ;
    vdm_v0_csc_en     = pstDmCfg->vdm_v0_csc_en   ;
    vdm_yuv2rgb_en    = pstDmCfg->vdm_yuv2rgb_en  ;
    vdm_rgb2yuv_en    = pstDmCfg->vdm_rgb2yuv_en  ;
    vdm_csc_en        = pstDmCfg->vdm_csc_en      ;
    vdm_dither_en     = pstDmCfg->vdm_dither_en   ;

    vdm_luma_coef0_tmap       = pstDmCfg->u16M3LumaCal[0] ;// ;
    vdm_luma_coef1_tmap       = pstDmCfg->u16M3LumaCal[1] ;// ;
    vdm_luma_coef2_tmap       = pstDmCfg->u16M3LumaCal[2] ;// ;
    vdm_tmap_luma_scale       = pstDmCfg->u16ScaleLumaCal ;// ;
    vdm_tmap_coef_scale       = pstDmCfg->u16ScaleCoefTM  ;// ;
    vdm_tmap_out_clip_min     = pstDmCfg->u32ClipMinTM    ;// ;
    vdm_tmap_out_clip_max     = pstDmCfg->u32ClipMaxTM    ;// ;
    vdm_tmap_out_dc0          = pstDmCfg->s32TMOff0       ;// ;
    vdm_tmap_out_dc1          = pstDmCfg->s32TMOff1       ;// ;
    vdm_tmap_out_dc2          = pstDmCfg->s32TMOff2       ;// ;

    v_degmm_x1_step   = pstDmCfg->DePQPara.gmm_x1_step   ;// ;
    v_degmm_x2_step   = pstDmCfg->DePQPara.gmm_x2_step   ;// ;
    v_degmm_x3_step   = pstDmCfg->DePQPara.gmm_x3_step   ;// ;
    v_degmm_x4_step   = pstDmCfg->DePQPara.gmm_x4_step   ;// ;
    v_degmm_x1_pos    = pstDmCfg->DePQPara.gmm_x1_pos    ;// ;
    v_degmm_x2_pos    = pstDmCfg->DePQPara.gmm_x2_pos    ;// ;
    v_degmm_x3_pos    = pstDmCfg->DePQPara.gmm_x3_pos    ;// ;
    v_degmm_x4_pos    = pstDmCfg->DePQPara.gmm_x4_pos    ;// ;
    v_degmm_x1_num    = pstDmCfg->DePQPara.gmm_x1_num    ;// ;
    v_degmm_x2_num    = pstDmCfg->DePQPara.gmm_x2_num    ;// ;
    v_degmm_x3_num    = pstDmCfg->DePQPara.gmm_x3_num    ;// ;
    v_degmm_x4_num    = pstDmCfg->DePQPara.gmm_x4_num    ;// ;

    v_tmap_x1_step     = pstDmCfg->TMPara.gmm_x1_step ;// ;
    v_tmap_x2_step     = pstDmCfg->TMPara.gmm_x2_step ;// ;
    v_tmap_x3_step     = pstDmCfg->TMPara.gmm_x3_step ;// ;
    v_tmap_x4_step     = pstDmCfg->TMPara.gmm_x4_step ;// ;
    v_tmap_x5_step     = pstDmCfg->TMPara.gmm_x5_step ;// ;
    v_tmap_x6_step     = pstDmCfg->TMPara.gmm_x6_step ;// ;
    v_tmap_x7_step     = pstDmCfg->TMPara.gmm_x7_step ;// ;
    v_tmap_x8_step     = pstDmCfg->TMPara.gmm_x8_step ;// ;
    v_tmap_x1_pos      = pstDmCfg->TMPara.gmm_x1_pos  ;// ;
    v_tmap_x2_pos      = pstDmCfg->TMPara.gmm_x2_pos  ;// ;
    v_tmap_x3_pos      = pstDmCfg->TMPara.gmm_x3_pos  ;// ;
    v_tmap_x4_pos      = pstDmCfg->TMPara.gmm_x4_pos  ;// ;
    v_tmap_x5_pos      = pstDmCfg->TMPara.gmm_x5_pos  ;// ;
    v_tmap_x6_pos      = pstDmCfg->TMPara.gmm_x6_pos  ;// ;
    v_tmap_x7_pos      = pstDmCfg->TMPara.gmm_x7_pos  ;// ;
    v_tmap_x8_pos      = pstDmCfg->TMPara.gmm_x8_pos  ;// ;
    v_tmap_x1_num      = pstDmCfg->TMPara.gmm_x1_num  ;// ;
    v_tmap_x2_num      = pstDmCfg->TMPara.gmm_x2_num  ;// ;
    v_tmap_x3_num      = pstDmCfg->TMPara.gmm_x3_num  ;// ;
    v_tmap_x4_num      = pstDmCfg->TMPara.gmm_x4_num  ;// ;
    v_tmap_x5_num      = pstDmCfg->TMPara.gmm_x5_num  ;// ;
    v_tmap_x6_num      = pstDmCfg->TMPara.gmm_x6_num  ;// ;
    v_tmap_x7_num      = pstDmCfg->TMPara.gmm_x7_num  ;// ;
    v_tmap_x8_num      = pstDmCfg->TMPara.gmm_x8_num  ;// ;

    v_gmm_x1_step     = pstDmCfg->GammaPara.gmm_x1_step ;// ;
    v_gmm_x2_step     = pstDmCfg->GammaPara.gmm_x2_step ;// ;
    v_gmm_x3_step     = pstDmCfg->GammaPara.gmm_x3_step ;// ;
    v_gmm_x4_step     = pstDmCfg->GammaPara.gmm_x4_step ;// ;
    v_gmm_x5_step     = pstDmCfg->GammaPara.gmm_x5_step ;// ;
    v_gmm_x6_step     = pstDmCfg->GammaPara.gmm_x6_step ;// ;
    v_gmm_x7_step     = pstDmCfg->GammaPara.gmm_x7_step ;// ;
    v_gmm_x8_step     = pstDmCfg->GammaPara.gmm_x8_step ;// ;
    v_gmm_x1_pos      = pstDmCfg->GammaPara.gmm_x1_pos  ;// ;
    v_gmm_x2_pos      = pstDmCfg->GammaPara.gmm_x2_pos  ;// ;
    v_gmm_x3_pos      = pstDmCfg->GammaPara.gmm_x3_pos  ;// ;
    v_gmm_x4_pos      = pstDmCfg->GammaPara.gmm_x4_pos  ;// ;
    v_gmm_x5_pos      = pstDmCfg->GammaPara.gmm_x5_pos  ;// ;
    v_gmm_x6_pos      = pstDmCfg->GammaPara.gmm_x6_pos  ;// ;
    v_gmm_x7_pos      = pstDmCfg->GammaPara.gmm_x7_pos  ;// ;
    v_gmm_x8_pos      = pstDmCfg->GammaPara.gmm_x8_pos  ;// ;
    v_gmm_x1_num      = pstDmCfg->GammaPara.gmm_x1_num  ;// ;
    v_gmm_x2_num      = pstDmCfg->GammaPara.gmm_x2_num  ;// ;
    v_gmm_x3_num      = pstDmCfg->GammaPara.gmm_x3_num  ;// ;
    v_gmm_x4_num      = pstDmCfg->GammaPara.gmm_x4_num  ;// ;
    v_gmm_x5_num      = pstDmCfg->GammaPara.gmm_x5_num  ;// ;
    v_gmm_x6_num      = pstDmCfg->GammaPara.gmm_x6_num  ;// ;
    v_gmm_x7_num      = pstDmCfg->GammaPara.gmm_x7_num  ;// ;
    v_gmm_x8_num      = pstDmCfg->GammaPara.gmm_x8_num  ;// ;

    vdm_coef00_csc       = pstDmCfg->s16M33Src2Disp[0][0] ;// ;
    vdm_coef01_csc       = pstDmCfg->s16M33Src2Disp[0][1] ;// ;
    vdm_coef02_csc       = pstDmCfg->s16M33Src2Disp[0][2] ;// ;
    vdm_coef10_csc       = pstDmCfg->s16M33Src2Disp[1][0] ;// ;
    vdm_coef11_csc       = pstDmCfg->s16M33Src2Disp[1][1] ;// ;
    vdm_coef12_csc       = pstDmCfg->s16M33Src2Disp[1][2] ;// ;
    vdm_coef20_csc       = pstDmCfg->s16M33Src2Disp[2][0] ;// ;
    vdm_coef21_csc       = pstDmCfg->s16M33Src2Disp[2][1] ;// ;
    vdm_coef22_csc       = pstDmCfg->s16M33Src2Disp[2][2] ;// ;
    vdm_csc_scale        = pstDmCfg->u16ScaleSrc2Disp     ;// ;
    vdm_csc_clip_min     = pstDmCfg->u32ClipMinCSC        ;// ;
    vdm_csc_clip_max     = pstDmCfg->u32ClipMaxCSC        ;// ;

    vdm_00_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[0][0] ;// ;
    vdm_01_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[0][1] ;// ;
    vdm_02_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[0][2] ;// ;
    vdm_03_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[1][0] ;// ;
    vdm_04_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[1][1] ;// ;
    vdm_05_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[1][2] ;// ;
    vdm_06_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[2][0] ;// ;
    vdm_07_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[2][1] ;// ;
    vdm_08_yuv2rgb     = pstDmCfg->s16M33YUV2RGB0[2][2] ;// ;
    vdm_10_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[0][0] ;// ;
    vdm_11_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[0][1] ;// ;
    vdm_12_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[0][2] ;// ;
    vdm_13_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[1][0] ;// ;
    vdm_14_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[1][1] ;// ;
    vdm_15_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[1][2] ;// ;
    vdm_16_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[2][0] ;// ;
    vdm_17_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[2][1] ;// ;
    vdm_18_yuv2rgb     = pstDmCfg->s16M33YUV2RGB1[2][2] ;// ;
    vdm_yuv2rgbscale2p     = pstDmCfg->u16ScaleYUV2RGB      ;// ;
    vdm_yuv2rgb_00_in_dc   = pstDmCfg->s32InOffsetYUV2RGB0[0]  ; // ;
    vdm_yuv2rgb_01_in_dc   = pstDmCfg->s32InOffsetYUV2RGB0[1]  ; // ;
    vdm_yuv2rgb_02_in_dc   = pstDmCfg->s32InOffsetYUV2RGB0[2]  ; // ;
    vdm_yuv2rgb_00_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB0[0] ; // ;
    vdm_yuv2rgb_01_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB0[1] ; // ;
    vdm_yuv2rgb_02_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB0[2] ; // ;
    vdm_yuv2rgb_10_in_dc   = pstDmCfg->s32InOffsetYUV2RGB1[0]  ; // ;
    vdm_yuv2rgb_11_in_dc   = pstDmCfg->s32InOffsetYUV2RGB1[1]  ; // ;
    vdm_yuv2rgb_12_in_dc   = pstDmCfg->s32InOffsetYUV2RGB1[2]  ; // ;
    vdm_yuv2rgb_10_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB1[0] ; // ;
    vdm_yuv2rgb_11_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB1[1] ; // ;
    vdm_yuv2rgb_12_out_dc  = pstDmCfg->s32OutOffsetYUV2RGB1[2] ; // ;
    vdm_yuv2rgb_clip_min   = pstDmCfg->u16ClipMinYUV2RGB       ; // ;
    vdm_yuv2rgb_clip_max   = pstDmCfg->u16ClipMaxYUV2RGB       ; // ;
    vdm_00_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][0]     ; // ;
    vdm_01_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][1]     ; // ;
    vdm_02_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[0][2]     ; // ;
    vdm_10_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][0]     ; // ;
    vdm_11_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][1]     ; // ;
    vdm_12_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[1][2]     ; // ;
    vdm_20_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][0]     ; // ;
    vdm_21_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][1]     ; // ;
    vdm_22_rgb2yuv         = pstDmCfg->s16M33RGB2YUV[2][2]     ; // ;
    vdm_rgb2yuvscale2p     = pstDmCfg->u16ScaleRGB2YUV         ; // ;
    vdm_rgb2yuv_0_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[0]   ; // ;
    vdm_rgb2yuv_1_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[1]   ; // ;
    vdm_rgb2yuv_2_in_dc    = pstDmCfg->s16InOffsetRGB2YUV[2]   ; // ;
    vdm_rgb2yuv_0_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[0]  ; // ;
    vdm_rgb2yuv_1_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[1]  ; // ;
    vdm_rgb2yuv_2_out_dc   = pstDmCfg->s16OutOffsetRGB2YUV[2]  ; // ;
    vdm_rgb2yuv_clip_min   = pstDmCfg->u16ClipMinRGB2YUV       ; // ;
    vdm_rgb2yuv_clip_max   = pstDmCfg->u16ClipMaxRGB2YUV       ; // ;

    vdm_dither_round        = pstDmCfg->stDitherCfg.u32_dither_round       ;
    vdm_dither_domain_mode  = pstDmCfg->stDitherCfg.u32_dither_domain_mode ;
    vdm_dither_tap_mode     = pstDmCfg->stDitherCfg.u32_dither_tap_mode    ;
    vdm_dither_round_unlim  = pstDmCfg->stDitherCfg.u32_dither_round_unlim ;
    vdm_dither_sed_y0       = pstDmCfg->stDitherCfg.u32_dither_sed_y0      ;
    vdm_dither_sed_u0       = pstDmCfg->stDitherCfg.u32_dither_sed_u0      ;
    vdm_dither_sed_v0       = pstDmCfg->stDitherCfg.u32_dither_sed_v0      ;
    vdm_dither_sed_w0       = pstDmCfg->stDitherCfg.u32_dither_sed_w0      ;
    vdm_dither_sed_y1       = pstDmCfg->stDitherCfg.u32_dither_sed_y1      ;
    vdm_dither_sed_u1       = pstDmCfg->stDitherCfg.u32_dither_sed_u1      ;
    vdm_dither_sed_v1       = pstDmCfg->stDitherCfg.u32_dither_sed_v1      ;
    vdm_dither_sed_w1       = pstDmCfg->stDitherCfg.u32_dither_sed_w1      ;
    vdm_dither_thr_max      = pstDmCfg->stDitherCfg.u32_dither_thr_max     ;
    vdm_dither_thr_min      = pstDmCfg->stDitherCfg.u32_dither_thr_min     ;


    //drv transfer
    VDP_VID_SetVdmVdmTmapInBdp(VDP_LAYER_VID0, vdm_tmap_in_bdp);
    VDP_VID_SetVdmVdmCscBind(VDP_LAYER_VID0, vdm_csc_bind);
    VDP_VID_SetVdmVdmGmmEn(VDP_LAYER_VID0, vdm_gmm_en);
    VDP_VID_SetVdmVdmTmapEn(VDP_LAYER_VID0, vdm_tmap_en);
    VDP_VID_SetVdmVdmDegmmEn(VDP_LAYER_VID0, vdm_degmm_en);
    VDP_VID_SetVdmVdmEn(VDP_LAYER_VID0, vdm_en);
    VDP_VID_SetVdmVdmLumaCoef0Tmap(VDP_LAYER_VID0, vdm_luma_coef0_tmap);
    VDP_VID_SetVdmVdmLumaCoef1Tmap(VDP_LAYER_VID0, vdm_luma_coef1_tmap);
    VDP_VID_SetVdmVdmLumaCoef2Tmap(VDP_LAYER_VID0, vdm_luma_coef2_tmap);
    VDP_VID_SetVdmVdmTmapLumaScale(VDP_LAYER_VID0, vdm_tmap_luma_scale);
    VDP_VID_SetVdmVdmTmapCoefScale(VDP_LAYER_VID0, vdm_tmap_coef_scale);
    VDP_VID_SetVdmVdmTmapOutClipMin(VDP_LAYER_VID0, vdm_tmap_out_clip_min);
    VDP_VID_SetVdmVdmTmapOutClipMax(VDP_LAYER_VID0, vdm_tmap_out_clip_max);
    //VDP_VID_SetVdmVdmParaDegmmAddr(VDP_LAYER_VID0, vdm_para_degmm_addr);
    //VDP_VID_SetVdmVdmParaGmmAddr(VDP_LAYER_VID0, vdm_para_gmm_addr);
    //VDP_VID_SetVdmVdmParaTmapAddr(VDP_LAYER_VID0, vdm_para_tmap_addr);
    //VDP_VID_SetVdmVdmParaTmapUpd(VDP_LAYER_VID0, vdm_para_tmap_upd);
    //VDP_VID_SetVdmVdmParaGmmUpd(VDP_LAYER_VID0, vdm_para_gmm_upd);
    //VDP_VID_SetVdmVdmParaDegmmUpd(VDP_LAYER_VID0, vdm_para_degmm_upd);
    VDP_VID_SetVdmTmapOutDc0(vdm_tmap_out_dc0);
    VDP_VID_SetVdmTmapOutDc1(vdm_tmap_out_dc1);
    VDP_VID_SetVdmTmapOutDc2(vdm_tmap_out_dc2);
    VDP_VID_SetVdmVdmDegammaRdEn(VDP_LAYER_VID0, vdm_degamma_rd_en);
    VDP_VID_SetVdmVdmDegammaParaData(VDP_LAYER_VID0, vdm_degamma_para_data);
    VDP_VID_SetVdmVDegmmX4Step(VDP_LAYER_VID0, v_degmm_x4_step);
    VDP_VID_SetVdmVDegmmX3Step(VDP_LAYER_VID0, v_degmm_x3_step);
    VDP_VID_SetVdmVDegmmX2Step(VDP_LAYER_VID0, v_degmm_x2_step);
    VDP_VID_SetVdmVDegmmX1Step(VDP_LAYER_VID0, v_degmm_x1_step);
    VDP_VID_SetVdmVDegmmX2Pos(VDP_LAYER_VID0, v_degmm_x2_pos);
    VDP_VID_SetVdmVDegmmX1Pos(VDP_LAYER_VID0, v_degmm_x1_pos);
    VDP_VID_SetVdmVDegmmX4Pos(VDP_LAYER_VID0, v_degmm_x4_pos);
    VDP_VID_SetVdmVDegmmX3Pos(VDP_LAYER_VID0, v_degmm_x3_pos);
    VDP_VID_SetVdmVDegmmX4Num(VDP_LAYER_VID0, v_degmm_x4_num);
    VDP_VID_SetVdmVDegmmX3Num(VDP_LAYER_VID0, v_degmm_x3_num);
    VDP_VID_SetVdmVDegmmX2Num(VDP_LAYER_VID0, v_degmm_x2_num);
    VDP_VID_SetVdmVDegmmX1Num(VDP_LAYER_VID0, v_degmm_x1_num);
    VDP_VID_SetVdmVdmTmapRdEn(VDP_LAYER_VID0, vdm_tmap_rd_en);
    VDP_VID_SetVdmVdmTmapParaData(VDP_LAYER_VID0, vdm_tmap_para_data);
    VDP_VID_SetVdmVTmapX4Step(VDP_LAYER_VID0, v_tmap_x4_step);
    VDP_VID_SetVTmapX5Step(v_tmap_x5_step);
    VDP_VID_SetVTmapX6Step(v_tmap_x6_step);
    VDP_VID_SetVTmapX7Step(v_tmap_x7_step);
    VDP_VID_SetVTmapX8Step(v_tmap_x8_step);
    VDP_VID_SetVdmVTmapX3Step(VDP_LAYER_VID0, v_tmap_x3_step);
    VDP_VID_SetVdmVTmapX2Step(VDP_LAYER_VID0, v_tmap_x2_step);
    VDP_VID_SetVdmVTmapX1Step(VDP_LAYER_VID0, v_tmap_x1_step);
    VDP_VID_SetVdmVTmapX1Pos(VDP_LAYER_VID0, v_tmap_x1_pos);
    VDP_VID_SetVdmVTmapX2Pos(VDP_LAYER_VID0, v_tmap_x2_pos);
    VDP_VID_SetVdmVTmapX3Num(VDP_LAYER_VID0, v_tmap_x3_num);
    VDP_VID_SetVdmVTmapX4Num(VDP_LAYER_VID0, v_tmap_x4_num);
    VDP_VID_SetVTmapX5Num(v_tmap_x5_num);
    VDP_VID_SetVTmapX6Num(v_tmap_x6_num);
    VDP_VID_SetVTmapX7Num(v_tmap_x7_num);
    VDP_VID_SetVTmapX8Num(v_tmap_x8_num);
    VDP_VID_SetVdmVTmapX2Num(VDP_LAYER_VID0, v_tmap_x2_num);
    VDP_VID_SetVdmVTmapX1Num(VDP_LAYER_VID0, v_tmap_x1_num);
    VDP_VID_SetVdmVTmapX3Pos(VDP_LAYER_VID0, v_tmap_x3_pos);
    VDP_VID_SetVdmVTmapX4Pos(VDP_LAYER_VID0, v_tmap_x4_pos);
    VDP_VID_SetVTmapX5Pos(v_tmap_x5_pos);
    VDP_VID_SetVTmapX6Pos(v_tmap_x6_pos);
    VDP_VID_SetVTmapX7Pos(v_tmap_x7_pos);
    VDP_VID_SetVTmapX8Pos(v_tmap_x8_pos);
    VDP_VID_SetVdmVdmGammaRdEn(VDP_LAYER_VID0, vdm_gamma_rd_en);
    VDP_VID_SetVdmVdmGammaParaData(VDP_LAYER_VID0, vdm_gamma_para_data);
    VDP_VID_SetVdmVGmmX4Step(VDP_LAYER_VID0, v_gmm_x4_step);
    VDP_VID_SetVdmVGmmX3Step(VDP_LAYER_VID0, v_gmm_x3_step);
    VDP_VID_SetVdmVGmmX2Step(VDP_LAYER_VID0, v_gmm_x2_step);
    VDP_VID_SetVdmVGmmX1Step(VDP_LAYER_VID0, v_gmm_x1_step);
    VDP_VID_SetVdmVGmmX2Pos(VDP_LAYER_VID0, v_gmm_x2_pos);
    VDP_VID_SetVdmVGmmX1Pos(VDP_LAYER_VID0, v_gmm_x1_pos);
    VDP_VID_SetVdmVGmmX4Pos(VDP_LAYER_VID0, v_gmm_x4_pos);
    VDP_VID_SetVdmVGmmX3Pos(VDP_LAYER_VID0, v_gmm_x3_pos);
    VDP_VID_SetVdmVGmmX4Num(VDP_LAYER_VID0, v_gmm_x4_num);
    VDP_VID_SetVdmVGmmX3Num(VDP_LAYER_VID0, v_gmm_x3_num);
    VDP_VID_SetVdmVGmmX2Num(VDP_LAYER_VID0, v_gmm_x2_num);
    VDP_VID_SetVdmVGmmX1Num(VDP_LAYER_VID0, v_gmm_x1_num);
    VDP_VID_SetVdmVGmmX6Pos(VDP_LAYER_VID0, v_gmm_x6_pos);
    VDP_VID_SetVdmVGmmX5Pos(VDP_LAYER_VID0, v_gmm_x5_pos);
    VDP_VID_SetVdmVGmmX8Pos(VDP_LAYER_VID0, v_gmm_x8_pos);
    VDP_VID_SetVdmVGmmX7Pos(VDP_LAYER_VID0, v_gmm_x7_pos);
    VDP_VID_SetVdmVGmmX8Step(VDP_LAYER_VID0, v_gmm_x8_step);
    VDP_VID_SetVdmVGmmX7Step(VDP_LAYER_VID0, v_gmm_x7_step);
    VDP_VID_SetVdmVGmmX6Step(VDP_LAYER_VID0, v_gmm_x6_step);
    VDP_VID_SetVdmVGmmX5Step(VDP_LAYER_VID0, v_gmm_x5_step);
    VDP_VID_SetVdmVGmmX8Num(VDP_LAYER_VID0, v_gmm_x8_num);
    VDP_VID_SetVdmVGmmX7Num(VDP_LAYER_VID0, v_gmm_x7_num);
    VDP_VID_SetVdmVGmmX6Num(VDP_LAYER_VID0, v_gmm_x6_num);
    VDP_VID_SetVdmVGmmX5Num(VDP_LAYER_VID0, v_gmm_x5_num);
    VDP_VID_SetVdmVdmCscEn(VDP_LAYER_VID0, vdm_csc_en);
    VDP_VID_SetVdmVdmCoef00Csc(VDP_LAYER_VID0, vdm_coef00_csc);
    VDP_VID_SetVdmVdmCoef01Csc(VDP_LAYER_VID0, vdm_coef01_csc);
    VDP_VID_SetVdmVdmCoef02Csc(VDP_LAYER_VID0, vdm_coef02_csc);
    VDP_VID_SetVdmVdmCoef10Csc(VDP_LAYER_VID0, vdm_coef10_csc);
    VDP_VID_SetVdmVdmCoef11Csc(VDP_LAYER_VID0, vdm_coef11_csc);
    VDP_VID_SetVdmVdmCoef12Csc(VDP_LAYER_VID0, vdm_coef12_csc);
    VDP_VID_SetVdmVdmCoef20Csc(VDP_LAYER_VID0, vdm_coef20_csc);
    VDP_VID_SetVdmVdmCoef21Csc(VDP_LAYER_VID0, vdm_coef21_csc);
    VDP_VID_SetVdmVdmCoef22Csc(VDP_LAYER_VID0, vdm_coef22_csc);
    VDP_VID_SetVdmVdmCscScale(VDP_LAYER_VID0, vdm_csc_scale);
    VDP_VID_SetVdmVdmCscClipMin(VDP_LAYER_VID0, vdm_csc_clip_min);
    VDP_VID_SetVdmVdmCscClipMax(VDP_LAYER_VID0, vdm_csc_clip_max);
    VDP_VID_SetVdmVdmV1GammaEn(VDP_LAYER_VID0, vdm_v1_gamma_en);
    VDP_VID_SetVdmVdmV0GammaEn(VDP_LAYER_VID0, vdm_v0_gamma_en);
    VDP_VID_SetVdmVdmV1CscEn(VDP_LAYER_VID0, vdm_v1_csc_en);
    VDP_VID_SetVdmVdmV0CscEn(VDP_LAYER_VID0, vdm_v0_csc_en);
    VDP_VID_SetVdmVdmYuv2rgbEn(VDP_LAYER_VID0, vdm_yuv2rgb_en);
    VDP_VID_SetVdmVdm00Yuv2rgb(VDP_LAYER_VID0, vdm_00_yuv2rgb);
    VDP_VID_SetVdmVdm01Yuv2rgb(VDP_LAYER_VID0, vdm_01_yuv2rgb);
    VDP_VID_SetVdmVdm02Yuv2rgb(VDP_LAYER_VID0, vdm_02_yuv2rgb);
    VDP_VID_SetVdmVdm03Yuv2rgb(VDP_LAYER_VID0, vdm_03_yuv2rgb);
    VDP_VID_SetVdmVdm04Yuv2rgb(VDP_LAYER_VID0, vdm_04_yuv2rgb);
    VDP_VID_SetVdmVdm05Yuv2rgb(VDP_LAYER_VID0, vdm_05_yuv2rgb);
    VDP_VID_SetVdmVdm06Yuv2rgb(VDP_LAYER_VID0, vdm_06_yuv2rgb);
    VDP_VID_SetVdmVdm07Yuv2rgb(VDP_LAYER_VID0, vdm_07_yuv2rgb);
    VDP_VID_SetVdmVdm08Yuv2rgb(VDP_LAYER_VID0, vdm_08_yuv2rgb);
    VDP_VID_SetVdmVdm10Yuv2rgb(VDP_LAYER_VID0, vdm_10_yuv2rgb);
    VDP_VID_SetVdmVdm11Yuv2rgb(VDP_LAYER_VID0, vdm_11_yuv2rgb);
    VDP_VID_SetVdmVdm12Yuv2rgb(VDP_LAYER_VID0, vdm_12_yuv2rgb);
    VDP_VID_SetVdmVdm13Yuv2rgb(VDP_LAYER_VID0, vdm_13_yuv2rgb);
    VDP_VID_SetVdmVdm14Yuv2rgb(VDP_LAYER_VID0, vdm_14_yuv2rgb);
    VDP_VID_SetVdmVdm15Yuv2rgb(VDP_LAYER_VID0, vdm_15_yuv2rgb);
    VDP_VID_SetVdmVdm16Yuv2rgb(VDP_LAYER_VID0, vdm_16_yuv2rgb);
    VDP_VID_SetVdmVdm17Yuv2rgb(VDP_LAYER_VID0, vdm_17_yuv2rgb);
    VDP_VID_SetVdmVdm18Yuv2rgb(VDP_LAYER_VID0, vdm_18_yuv2rgb);
    VDP_VID_SetVdmVdmYuv2rgbscale2p(VDP_LAYER_VID0, vdm_yuv2rgbscale2p);
    VDP_VID_SetVdmVdmYuv2rgb00InDc(VDP_LAYER_VID0, vdm_yuv2rgb_00_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb01InDc(VDP_LAYER_VID0, vdm_yuv2rgb_01_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb02InDc(VDP_LAYER_VID0, vdm_yuv2rgb_02_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb00OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_00_out_dc);
    VDP_VID_SetVdmVdmYuv2rgb01OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_01_out_dc);
    VDP_VID_SetVdmVdmYuv2rgb02OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_02_out_dc);
    VDP_VID_SetVdmVdmYuv2rgb10InDc(VDP_LAYER_VID0, vdm_yuv2rgb_10_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb11InDc(VDP_LAYER_VID0, vdm_yuv2rgb_11_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb12InDc(VDP_LAYER_VID0, vdm_yuv2rgb_12_in_dc);
    VDP_VID_SetVdmVdmYuv2rgb10OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_10_out_dc);
    VDP_VID_SetVdmVdmYuv2rgb11OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_11_out_dc);
    VDP_VID_SetVdmVdmYuv2rgb12OutDc(VDP_LAYER_VID0, vdm_yuv2rgb_12_out_dc);
    VDP_VID_SetVdmVdmYuv2rgbClipMin(VDP_LAYER_VID0, vdm_yuv2rgb_clip_min);
    VDP_VID_SetVdmVdmYuv2rgbClipMax(VDP_LAYER_VID0, vdm_yuv2rgb_clip_max);
    VDP_VID_SetVdmVdmRgb2yuvEn(VDP_LAYER_VID0, vdm_rgb2yuv_en);
    VDP_VID_SetVdmVdm00Rgb2yuv(VDP_LAYER_VID0, vdm_00_rgb2yuv);
    VDP_VID_SetVdmVdm01Rgb2yuv(VDP_LAYER_VID0, vdm_01_rgb2yuv);
    VDP_VID_SetVdmVdm02Rgb2yuv(VDP_LAYER_VID0, vdm_02_rgb2yuv);
    VDP_VID_SetVdmVdm10Rgb2yuv(VDP_LAYER_VID0, vdm_10_rgb2yuv);
    VDP_VID_SetVdmVdm11Rgb2yuv(VDP_LAYER_VID0, vdm_11_rgb2yuv);
    VDP_VID_SetVdmVdm12Rgb2yuv(VDP_LAYER_VID0, vdm_12_rgb2yuv);
    VDP_VID_SetVdmVdm20Rgb2yuv(VDP_LAYER_VID0, vdm_20_rgb2yuv);
    VDP_VID_SetVdmVdm21Rgb2yuv(VDP_LAYER_VID0, vdm_21_rgb2yuv);
    VDP_VID_SetVdmVdm22Rgb2yuv(VDP_LAYER_VID0, vdm_22_rgb2yuv);
    VDP_VID_SetVdmVdmRgb2yuvscale2p(VDP_LAYER_VID0, vdm_rgb2yuvscale2p);
    VDP_VID_SetVdmVdmRgb2yuv0InDc(VDP_LAYER_VID0, vdm_rgb2yuv_0_in_dc);
    VDP_VID_SetVdmVdmRgb2yuv1InDc(VDP_LAYER_VID0, vdm_rgb2yuv_1_in_dc);
    VDP_VID_SetVdmVdmRgb2yuv2InDc(VDP_LAYER_VID0, vdm_rgb2yuv_2_in_dc);
    VDP_VID_SetVdmVdmRgb2yuv0OutDc(VDP_LAYER_VID0, vdm_rgb2yuv_0_out_dc);
    VDP_VID_SetVdmVdmRgb2yuv1OutDc(VDP_LAYER_VID0, vdm_rgb2yuv_1_out_dc);
    VDP_VID_SetVdmVdmRgb2yuv2OutDc(VDP_LAYER_VID0, vdm_rgb2yuv_2_out_dc);
    VDP_VID_SetVdmVdmRgb2yuvClipMin(VDP_LAYER_VID0, vdm_rgb2yuv_clip_min);
    VDP_VID_SetVdmVdmRgb2yuvClipMax(VDP_LAYER_VID0, vdm_rgb2yuv_clip_max);
    VDP_VID_SetVdmVdmDitherRoundUnlim(VDP_LAYER_VID0, vdm_dither_round_unlim);
    VDP_VID_SetVdmVdmDitherEn(VDP_LAYER_VID0, vdm_dither_en);
    VDP_VID_SetVdmVdmDitherRound(VDP_LAYER_VID0, vdm_dither_round);
    VDP_VID_SetVdmVdmDitherDomainMode(VDP_LAYER_VID0, vdm_dither_domain_mode);
    VDP_VID_SetVdmVdmDitherTapMode(VDP_LAYER_VID0, vdm_dither_tap_mode);
    VDP_VID_SetVdmVdmDitherSedY0(VDP_LAYER_VID0, vdm_dither_sed_y0);
    VDP_VID_SetVdmVdmDitherSedU0(VDP_LAYER_VID0, vdm_dither_sed_u0);
    VDP_VID_SetVdmVdmDitherSedV0(VDP_LAYER_VID0, vdm_dither_sed_v0);
    VDP_VID_SetVdmVdmDitherSedW0(VDP_LAYER_VID0, vdm_dither_sed_w0);
    VDP_VID_SetVdmVdmDitherSedY1(VDP_LAYER_VID0, vdm_dither_sed_y1);
    VDP_VID_SetVdmVdmDitherSedU1(VDP_LAYER_VID0, vdm_dither_sed_u1);
    VDP_VID_SetVdmVdmDitherSedV1(VDP_LAYER_VID0, vdm_dither_sed_v1);
    VDP_VID_SetVdmVdmDitherSedW1(VDP_LAYER_VID0, vdm_dither_sed_w1);
    VDP_VID_SetVdmVdmDitherSedY2(VDP_LAYER_VID0, vdm_dither_sed_y2);
    VDP_VID_SetVdmVdmDitherSedU2(VDP_LAYER_VID0, vdm_dither_sed_u2);
    VDP_VID_SetVdmVdmDitherSedV2(VDP_LAYER_VID0, vdm_dither_sed_v2);
    VDP_VID_SetVdmVdmDitherSedW2(VDP_LAYER_VID0, vdm_dither_sed_w2);
    VDP_VID_SetVdmVdmDitherSedY3(VDP_LAYER_VID0, vdm_dither_sed_y3);
    VDP_VID_SetVdmVdmDitherSedU3(VDP_LAYER_VID0, vdm_dither_sed_u3);
    VDP_VID_SetVdmVdmDitherSedV3(VDP_LAYER_VID0, vdm_dither_sed_v3);
    VDP_VID_SetVdmVdmDitherSedW3(VDP_LAYER_VID0, vdm_dither_sed_w3);
    VDP_VID_SetVdmVdmDitherThrMax(VDP_LAYER_VID0, vdm_dither_thr_max);
    VDP_VID_SetVdmVdmDitherThrMin(VDP_LAYER_VID0, vdm_dither_thr_min);

return HI_SUCCESS;

}


HI_S32 VDP_DRV_GetHiHdrCfg(VDP_HISI_HDR_CFG_S *pstHdrCfg,DmCfg_t  *pstDmCfg)
{
    if (pstHdrCfg->eHiHdrMode == HIHDR_HDR10_IN_SDR_OUT) //98mv200t test ok!
    {
        //typ mode
        VDP_PRINT("VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");

        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;

#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_en                         = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ; //use BT2020
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 7       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 6       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 2       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 0       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 512     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 928     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 1020    ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 8       ;
        pstDmCfg->DePQPara.gmm_x2_num            = 26      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 23      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 4       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ; //BT2020 to BT709
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;//0xFFFF0  1048575 ;//65535   ; //

        //YUV2RGB
        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;

        //RGB2YUV
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ; //BT709
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1315   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -4425   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -5214   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -527    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif
        //DITHER
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_HDR10_IN_HDR10_OUT) //98mv200t test ok!
    {
        //typ mode
        VDP_PRINT("VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 0 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 0 ;
        pstDmCfg->vdm_degmm_en                   = 0 ;
        pstDmCfg->vdm_csc_en                     = 0 ;
        pstDmCfg->vdm_tmap_en                    = 0 ;
        pstDmCfg->vdm_gmm_en                     = 0 ;
        pstDmCfg->vdm_dither_en                  = 0 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 0 ;
        pstDmCfg->vdm_v0_csc_en                  = 0 ; //use BT2020
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 0;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 7       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ; //BT2020 to BT709
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;//0xFFFF0  1048575 ;//65535   ; //

        //YUV2RGB
        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;

        //RGB2YUV
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ; //BT709
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1315   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -4425   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -5214   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -527    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif
        //DITHER
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_HDR10_IN_HLG_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 0 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ; //use BT2020
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 10      ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 349     ;
        pstDmCfg->s32TMOff1                      = 349     ;
        pstDmCfg->s32TMOff2                      = 349     ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 16384   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = 0       ;
        pstDmCfg->s16M33Src2Disp[0][2]           = 0       ;
        pstDmCfg->s16M33Src2Disp[1][0]           = 0       ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 16384   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][0]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][1]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 16384   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 3685    ; //BT2020
        pstDmCfg->s16M33RGB2YUV[0][1]            = 9512    ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 832     ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -2004   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5171   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6598   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -577    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;

#endif
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_SDR_IN_HDR10_OUT) //not use
    {
        VDP_PRINT("VdmSceneMode == VDM_SDR_IN_HDR10_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 0 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ;
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 1076    ;
        pstDmCfg->u16M3LumaCal[1]                = 2777    ;
        pstDmCfg->u16M3LumaCal[2]                = 243     ;
        pstDmCfg->u16ScaleLumaCal                = 12      ;
        pstDmCfg->u16ScaleCoefTM                 = 8       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 9       ;
        pstDmCfg->TMPara.gmm_x2_step             = 10      ;
        pstDmCfg->TMPara.gmm_x3_step             = 12      ;
        pstDmCfg->TMPara.gmm_x4_step             = 13      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x2_pos              = 16384   ;
        pstDmCfg->TMPara.gmm_x3_pos              = 24576   ;
        pstDmCfg->TMPara.gmm_x4_pos              = 65535   ;
        pstDmCfg->TMPara.gmm_x1_num              = 16      ;
        pstDmCfg->TMPara.gmm_x2_num              = 8       ;
        pstDmCfg->TMPara.gmm_x3_num              = 2       ;
        pstDmCfg->TMPara.gmm_x4_num              = 5       ;

        pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
        pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ;
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1643   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5531   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6518   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -659    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif

        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_SDR_IN_HLG_OUT) //not use
    {
        VDP_PRINT("VdmSceneMode == VDM_SDR_IN_BBC_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 0 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ;
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 1076    ;
        pstDmCfg->u16M3LumaCal[1]                = 2777    ;
        pstDmCfg->u16M3LumaCal[2]                = 243     ;
        pstDmCfg->u16ScaleLumaCal                = 12      ;
        pstDmCfg->u16ScaleCoefTM                 = 8       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 9       ;
        pstDmCfg->TMPara.gmm_x2_step             = 10      ;
        pstDmCfg->TMPara.gmm_x3_step             = 12      ;
        pstDmCfg->TMPara.gmm_x4_step             = 13      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x2_pos              = 16384   ;
        pstDmCfg->TMPara.gmm_x3_pos              = 24576   ;
        pstDmCfg->TMPara.gmm_x4_pos              = 65535   ;
        pstDmCfg->TMPara.gmm_x1_num              = 16      ;
        pstDmCfg->TMPara.gmm_x2_num              = 8       ;
        pstDmCfg->TMPara.gmm_x3_num              = 2       ;
        pstDmCfg->TMPara.gmm_x4_num              = 5       ;

        pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
        pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ;
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1643   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5531   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6518   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -659    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif

        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_AVS_IN_SDR_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ; // 0 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ;////use Y2R BT2020 0 ; //use Y2R BT709
        pstDmCfg->vdm_v1_csc_en                  = 0 ;//                            1 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;//                            0 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;//                            1 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217    ;
        pstDmCfg->u16M3LumaCal[2]                = 1943     ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 7       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ; //BT709
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1315   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -4425   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 5740    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -5214   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -527    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif

        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_AVS_IN_HDR10_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 0 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ; //use BT2020
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 8       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 16384   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = 0       ;
        pstDmCfg->s16M33Src2Disp[0][2]           = 0       ;
        pstDmCfg->s16M33Src2Disp[1][0]           = 0       ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 16384   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][0]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][1]           = 0       ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 16384   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 3685    ; //BT2020
        pstDmCfg->s16M33RGB2YUV[0][1]            = 9512    ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 832     ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -2004   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5171   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6598   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -577    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;

#endif
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_HLG_IN_SDR_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ; // 0 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ;////use Y2R BT2020 0 ; //use Y2R BT709
        pstDmCfg->vdm_v1_csc_en                  = 0 ;//                            1 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;//                            0 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;//                            1 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217    ;
        pstDmCfg->u16M3LumaCal[2]                = 1943     ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 15      ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 349     ;
        pstDmCfg->s32TMOff1                      = 349     ;
        pstDmCfg->s32TMOff2                      = 349     ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ; //BT709
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1643   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5531   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6518   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -659    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif

        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_HLG_IN_HDR10_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 0 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 0;// 1 ;
        pstDmCfg->vdm_tmap_en                    = 1 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1;//use BT2020     0 ; //use BT709
        pstDmCfg->vdm_v1_csc_en                  = 0;//                      1 ;
        pstDmCfg->vdm_v0_gamma_en                = 1;//                      0 ;
        pstDmCfg->vdm_v1_gamma_en                = 0;//                      1 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 15      ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4       ;
        pstDmCfg->TMPara.gmm_x2_step             = 6       ;
        pstDmCfg->TMPara.gmm_x3_step             = 8       ;
        pstDmCfg->TMPara.gmm_x4_step             = 9       ;
        pstDmCfg->TMPara.gmm_x5_step             = 10      ;
        pstDmCfg->TMPara.gmm_x6_step             = 12      ;
        pstDmCfg->TMPara.gmm_x7_step             = 14      ;
        pstDmCfg->TMPara.gmm_x8_step             = 16      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64      ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512     ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024    ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072    ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536   ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680  ;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575 ;
        pstDmCfg->TMPara.gmm_x1_num              = 4       ;
        pstDmCfg->TMPara.gmm_x2_num              = 11      ;
        pstDmCfg->TMPara.gmm_x3_num              = 13      ;
        pstDmCfg->TMPara.gmm_x4_num              = 17      ;
        pstDmCfg->TMPara.gmm_x5_num              = 22      ;
        pstDmCfg->TMPara.gmm_x6_num              = 36      ;
        pstDmCfg->TMPara.gmm_x7_num              = 52      ;
        pstDmCfg->TMPara.gmm_x8_num              = 63      ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 16384 ; //BT2020 to BT2020
        pstDmCfg->s16M33Src2Disp[0][1]           = 0     ;
        pstDmCfg->s16M33Src2Disp[0][2]           = 0     ;
        pstDmCfg->s16M33Src2Disp[1][0]           = 0     ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 16384 ;
        pstDmCfg->s16M33Src2Disp[1][2]           = 0     ;
        pstDmCfg->s16M33Src2Disp[2][0]           = 0     ;
        pstDmCfg->s16M33Src2Disp[2][1]           = 0     ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 16384 ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        //YUV2RGB
        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;

        //RGB2YUV
        pstDmCfg->s16M33RGB2YUV[0][0]            = 3685   ; //BT2020
        pstDmCfg->s16M33RGB2YUV[0][1]            = 9512   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 832    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -2004  ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5171  ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175   ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175   ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6598  ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -577   ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif
        //DITHER
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_BT2020_IN_709_OUT) //98mv200t test ok!
    {
        VDP_PRINT("VdmSceneMode == VDM_BT2020_IN_709_OUT\n");
        //typ mode


        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 1 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;


#if CBB_CFG_NOT_FROM_PQ
        // enable signal
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ;
        pstDmCfg->vdm_tmap_en                    = 0 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ; //use BT2020
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 8608    ;
        pstDmCfg->u16M3LumaCal[1]                = 22217   ;
        pstDmCfg->u16M3LumaCal[2]                = 1943    ;
        pstDmCfg->u16ScaleLumaCal                = 15      ;
        pstDmCfg->u16ScaleCoefTM                 = 8       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 4     ;
        pstDmCfg->TMPara.gmm_x2_step             = 6     ;
        pstDmCfg->TMPara.gmm_x3_step             = 8     ;
        pstDmCfg->TMPara.gmm_x4_step             = 9     ;
        pstDmCfg->TMPara.gmm_x5_step             = 10    ;
        pstDmCfg->TMPara.gmm_x6_step             = 12    ;
        pstDmCfg->TMPara.gmm_x7_step             = 14    ;
        pstDmCfg->TMPara.gmm_x8_step             = 16    ;
        pstDmCfg->TMPara.gmm_x1_pos              = 64    ;
        pstDmCfg->TMPara.gmm_x2_pos              = 512   ;
        pstDmCfg->TMPara.gmm_x3_pos              = 1024  ;
        pstDmCfg->TMPara.gmm_x4_pos              = 3072  ;
        pstDmCfg->TMPara.gmm_x5_pos              = 8192  ;
        pstDmCfg->TMPara.gmm_x6_pos              = 65536 ;
        pstDmCfg->TMPara.gmm_x7_pos              = 327680;
        pstDmCfg->TMPara.gmm_x8_pos              = 1048575;
        pstDmCfg->TMPara.gmm_x1_num              = 4     ;
        pstDmCfg->TMPara.gmm_x2_num              = 11    ;
        pstDmCfg->TMPara.gmm_x3_num              = 13    ;
        pstDmCfg->TMPara.gmm_x4_num              = 17    ;
        pstDmCfg->TMPara.gmm_x5_num              = 22    ;
        pstDmCfg->TMPara.gmm_x6_num              = 36    ;
        pstDmCfg->TMPara.gmm_x7_num              = 52    ;
        pstDmCfg->TMPara.gmm_x8_num              = 63    ;

        pstDmCfg->GammaPara.gmm_x1_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 9     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x6_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x7_step          = 14    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 16    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 1024  ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 3072  ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 8192  ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 65536 ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 327680;
        pstDmCfg->GammaPara.gmm_x8_pos           = 1048575;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ; //BT2020 to BT709
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;//0xFFFF0  1048575 ;

        //YUV2RGB
        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ; //BT2020
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ; //BT709
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;

        //RGB2YUV
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ; //BT709
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1643   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5531   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6518   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -659    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif
        //DITHER
        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }
    else if (pstHdrCfg->eHiHdrMode == HIHDR_709_IN_BT2020_OUT) //not use
    {
        VDP_PRINT("VdmSceneMode == VDM_709_IN_BT2020_OUT\n");
        //typ mode

        // enable signal

        // ctrl signal
        pstDmCfg->vdm_csc_bind                        = 0 ;// 0:csc->tmap, 1:tmap->csc
        pstDmCfg->stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
        pstDmCfg->stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
        pstDmCfg->stDitherCfg.u32_dither_tap_mode     = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_round_unlim  = 0 ;

        pstDmCfg->vdm_en                         = 1 ;
#if CBB_CFG_NOT_FROM_PQ
        pstDmCfg->vdm_yuv2rgb_en                 = 1 ;
        pstDmCfg->vdm_degmm_en                   = 1 ;
        pstDmCfg->vdm_csc_en                     = 1 ;
        pstDmCfg->vdm_tmap_en                    = 0 ;
        pstDmCfg->vdm_gmm_en                     = 1 ;
        pstDmCfg->vdm_dither_en                  = 1 ;
        pstDmCfg->vdm_rgb2yuv_en                 = 1 ;
        pstDmCfg->vdm_v0_csc_en                  = 1 ;
        pstDmCfg->vdm_v1_csc_en                  = 0 ;
        pstDmCfg->vdm_v0_gamma_en                = 1 ;
        pstDmCfg->vdm_v1_gamma_en                = 0 ;

        // cfg
        pstDmCfg->u16M3LumaCal[0]                = 1076    ;
        pstDmCfg->u16M3LumaCal[1]                = 2777    ;
        pstDmCfg->u16M3LumaCal[2]                = 243     ;
        pstDmCfg->u16ScaleLumaCal                = 12      ;
        pstDmCfg->u16ScaleCoefTM                 = 8       ;
        pstDmCfg->u32ClipMinTM                   = 0       ;
        pstDmCfg->u32ClipMaxTM                   = (1 << 20) - 1   ;
        pstDmCfg->s32TMOff0                      = 0       ;
        pstDmCfg->s32TMOff1                      = 0       ;
        pstDmCfg->s32TMOff2                      = 0       ;

        pstDmCfg->DePQPara.gmm_x1_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x2_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x3_step           = 4       ;
        pstDmCfg->DePQPara.gmm_x4_step           = 5       ;
        pstDmCfg->DePQPara.gmm_x1_pos            = 672     ;
        pstDmCfg->DePQPara.gmm_x2_pos            = 832     ;
        pstDmCfg->DePQPara.gmm_x3_pos            = 992     ;
        pstDmCfg->DePQPara.gmm_x4_pos            = 1023    ;
        pstDmCfg->DePQPara.gmm_x1_num            = 42      ;
        pstDmCfg->DePQPara.gmm_x2_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x3_num            = 10      ;
        pstDmCfg->DePQPara.gmm_x4_num            = 1       ;

        pstDmCfg->TMPara.gmm_x1_step             = 9       ;
        pstDmCfg->TMPara.gmm_x2_step             = 10      ;
        pstDmCfg->TMPara.gmm_x3_step             = 12      ;
        pstDmCfg->TMPara.gmm_x4_step             = 13      ;
        pstDmCfg->TMPara.gmm_x1_pos              = 8192    ;
        pstDmCfg->TMPara.gmm_x2_pos              = 16384   ;
        pstDmCfg->TMPara.gmm_x3_pos              = 24576   ;
        pstDmCfg->TMPara.gmm_x4_pos              = 65535   ;
        pstDmCfg->TMPara.gmm_x1_num              = 16      ;
        pstDmCfg->TMPara.gmm_x2_num              = 8       ;
        pstDmCfg->TMPara.gmm_x3_num              = 2       ;
        pstDmCfg->TMPara.gmm_x4_num              = 5       ;

        pstDmCfg->GammaPara.gmm_x1_step          = 0     ;
        pstDmCfg->GammaPara.gmm_x2_step          = 2     ;
        pstDmCfg->GammaPara.gmm_x3_step          = 4     ;
        pstDmCfg->GammaPara.gmm_x4_step          = 5     ;
        pstDmCfg->GammaPara.gmm_x5_step          = 6     ;
        pstDmCfg->GammaPara.gmm_x6_step          = 8     ;
        pstDmCfg->GammaPara.gmm_x7_step          = 10    ;
        pstDmCfg->GammaPara.gmm_x8_step          = 12    ;
        pstDmCfg->GammaPara.gmm_x1_pos           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_pos           = 32    ;
        pstDmCfg->GammaPara.gmm_x3_pos           = 64    ;
        pstDmCfg->GammaPara.gmm_x4_pos           = 192   ;
        pstDmCfg->GammaPara.gmm_x5_pos           = 512   ;
        pstDmCfg->GammaPara.gmm_x6_pos           = 4096  ;
        pstDmCfg->GammaPara.gmm_x7_pos           = 20480 ;
        pstDmCfg->GammaPara.gmm_x8_pos           = 65535 ;
        pstDmCfg->GammaPara.gmm_x1_num           = 4     ;
        pstDmCfg->GammaPara.gmm_x2_num           = 11    ;
        pstDmCfg->GammaPara.gmm_x3_num           = 13    ;
        pstDmCfg->GammaPara.gmm_x4_num           = 17    ;
        pstDmCfg->GammaPara.gmm_x5_num           = 22    ;
        pstDmCfg->GammaPara.gmm_x6_num           = 36    ;
        pstDmCfg->GammaPara.gmm_x7_num           = 52    ;
        pstDmCfg->GammaPara.gmm_x8_num           = 63    ;

        pstDmCfg->s16M33Src2Disp[0][0]           = 27209   ;
        pstDmCfg->s16M33Src2Disp[0][1]           = -9632   ;
        pstDmCfg->s16M33Src2Disp[0][2]           = -1194   ;
        pstDmCfg->s16M33Src2Disp[1][0]           = -2045   ;
        pstDmCfg->s16M33Src2Disp[1][1]           = 18565   ;
        pstDmCfg->s16M33Src2Disp[1][2]           = -138    ;
        pstDmCfg->s16M33Src2Disp[2][0]           = -297    ;
        pstDmCfg->s16M33Src2Disp[2][1]           = -1648   ;
        pstDmCfg->s16M33Src2Disp[2][2]           = 18330   ;
        pstDmCfg->u16ScaleSrc2Disp               = 14      ;
        pstDmCfg->u32ClipMinCSC                  = 0       ;
        pstDmCfg->u32ClipMaxCSC                  = 1048575 ;

        pstDmCfg->s16M33YUV2RGB0[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB0[0][2]           = 13792   ;
        pstDmCfg->s16M33YUV2RGB0[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[1][1]           = -1539   ;
        pstDmCfg->s16M33YUV2RGB0[1][2]           = -5344   ;
        pstDmCfg->s16M33YUV2RGB0[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB0[2][1]           = 17597   ;
        pstDmCfg->s16M33YUV2RGB0[2][2]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[0][1]           = 0       ;
        pstDmCfg->s16M33YUV2RGB1[0][2]           = 14729   ;
        pstDmCfg->s16M33YUV2RGB1[1][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[1][1]           = -1752   ;
        pstDmCfg->s16M33YUV2RGB1[1][2]           = -4379   ;
        pstDmCfg->s16M33YUV2RGB1[2][0]           = 9567    ;
        pstDmCfg->s16M33YUV2RGB1[2][1]           = 17356   ;
        pstDmCfg->s16M33YUV2RGB1[2][2]           = 0       ;
        pstDmCfg->u16ScaleYUV2RGB                = 13      ;
        pstDmCfg->s32InOffsetYUV2RGB0[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB0[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB0[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB0[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB0[2]        = 0       ;
        pstDmCfg->s32InOffsetYUV2RGB1[0]         = -64     ;
        pstDmCfg->s32InOffsetYUV2RGB1[1]         = -512    ;
        pstDmCfg->s32InOffsetYUV2RGB1[2]         = -512    ;
        pstDmCfg->s32OutOffsetYUV2RGB1[0]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[1]        = 0       ;
        pstDmCfg->s32OutOffsetYUV2RGB1[2]        = 0       ;
        pstDmCfg->u16ClipMinYUV2RGB              = 0       ;
        pstDmCfg->u16ClipMaxYUV2RGB              = 1023    ;
        pstDmCfg->s16M33RGB2YUV[0][0]            = 2984    ;
        pstDmCfg->s16M33RGB2YUV[0][1]            = 10034   ;
        pstDmCfg->s16M33RGB2YUV[0][2]            = 1013    ;
        pstDmCfg->s16M33RGB2YUV[1][0]            = -1643   ;
        pstDmCfg->s16M33RGB2YUV[1][1]            = -5531   ;
        pstDmCfg->s16M33RGB2YUV[1][2]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][0]            = 7175    ;
        pstDmCfg->s16M33RGB2YUV[2][1]            = -6518   ;
        pstDmCfg->s16M33RGB2YUV[2][2]            = -659    ;
        pstDmCfg->u16ScaleRGB2YUV                = 14      ;
        pstDmCfg->s16InOffsetRGB2YUV[0]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[1]          = 0       ;
        pstDmCfg->s16InOffsetRGB2YUV[2]          = 0       ;
        pstDmCfg->s16OutOffsetRGB2YUV[0]         = 64      ;
        pstDmCfg->s16OutOffsetRGB2YUV[1]         = 512     ;
        pstDmCfg->s16OutOffsetRGB2YUV[2]         = 512     ;
        pstDmCfg->u16ClipMinRGB2YUV              = 0       ;
        pstDmCfg->u16ClipMaxRGB2YUV              = 1023    ;
#endif

        pstDmCfg->stDitherCfg.u32_dither_thr_max      = 65535 ;
        pstDmCfg->stDitherCfg.u32_dither_thr_min      = 0 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y0       = 2147483647;//31'h7fffffff;
        pstDmCfg->stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
        pstDmCfg->stDitherCfg.u32_dither_sed_w1       = 2140000000 ;
    }

    return HI_SUCCESS;

}



HI_VOID VDP_DRV_SetHiHdrAjust(HI_PQ_HDR_CFG  *HiHdrCfg,DmCfg_t  *pstDmCfg)
{
//接口转换

    pstDmCfg->vdm_csc_bind = HiHdrCfg->bGMapPosSel;
    //pstDmCfg->stDitherCfg.u32_dither_domain_mode = HiHdrCfg->bDITHER_MODE;//98mv200dither mode 不需要配置


    pstDmCfg->vdm_yuv2rgb_en                 = HiHdrCfg->bY2REn;
    pstDmCfg->vdm_degmm_en                   = HiHdrCfg->bDegammaEn;
    pstDmCfg->vdm_csc_en                     = HiHdrCfg->bGMapEn ;
    pstDmCfg->vdm_tmap_en                    = HiHdrCfg->bTMapEn;
    pstDmCfg->vdm_gmm_en                     = HiHdrCfg->bGammaEn;
    pstDmCfg->vdm_dither_en                  = HiHdrCfg->bDitherEn ;
    pstDmCfg->vdm_rgb2yuv_en                 = HiHdrCfg->bR2YEn ;
    pstDmCfg->vdm_v0_csc_en                  = HiHdrCfg->bY2REn ;
    pstDmCfg->vdm_v1_csc_en                  = 0 ;
    pstDmCfg->vdm_v0_gamma_en                = HiHdrCfg->bGammaEn ;
    pstDmCfg->vdm_v1_gamma_en                = 0;



    /********** YUV2RGB **********/


    pstDmCfg->s16M33YUV2RGB0[0][0]          = HiHdrCfg->stY2R.as16M33Y2R[0][0]  ; //BT2020
    pstDmCfg->s16M33YUV2RGB0[0][1]          = HiHdrCfg->stY2R.as16M33Y2R[0][1]  ;
    pstDmCfg->s16M33YUV2RGB0[0][2]          = HiHdrCfg->stY2R.as16M33Y2R[0][2]  ;
    pstDmCfg->s16M33YUV2RGB0[1][0]          = HiHdrCfg->stY2R.as16M33Y2R[1][0]  ;
    pstDmCfg->s16M33YUV2RGB0[1][1]          = HiHdrCfg->stY2R.as16M33Y2R[1][1]  ;
    pstDmCfg->s16M33YUV2RGB0[1][2]          = HiHdrCfg->stY2R.as16M33Y2R[1][2]  ;
    pstDmCfg->s16M33YUV2RGB0[2][0]          = HiHdrCfg->stY2R.as16M33Y2R[2][0]  ;
    pstDmCfg->s16M33YUV2RGB0[2][1]          = HiHdrCfg->stY2R.as16M33Y2R[2][1]  ;
    pstDmCfg->s16M33YUV2RGB0[2][2]          = HiHdrCfg->stY2R.as16M33Y2R[2][2]  ;
    pstDmCfg->s16M33YUV2RGB1[0][0]          = HiHdrCfg->stY2R.as16M33Y2R[0][0]  ; //BT709
    pstDmCfg->s16M33YUV2RGB1[0][1]          = HiHdrCfg->stY2R.as16M33Y2R[0][1]  ;
    pstDmCfg->s16M33YUV2RGB1[0][2]          = HiHdrCfg->stY2R.as16M33Y2R[0][2]  ;
    pstDmCfg->s16M33YUV2RGB1[1][0]          = HiHdrCfg->stY2R.as16M33Y2R[1][0]  ;
    pstDmCfg->s16M33YUV2RGB1[1][1]          = HiHdrCfg->stY2R.as16M33Y2R[1][1]  ;
    pstDmCfg->s16M33YUV2RGB1[1][2]          = HiHdrCfg->stY2R.as16M33Y2R[1][2]  ;
    pstDmCfg->s16M33YUV2RGB1[2][0]          = HiHdrCfg->stY2R.as16M33Y2R[2][0]  ;
    pstDmCfg->s16M33YUV2RGB1[2][1]          = HiHdrCfg->stY2R.as16M33Y2R[2][1]  ;
    pstDmCfg->s16M33YUV2RGB1[2][2]          = HiHdrCfg->stY2R.as16M33Y2R[2][2]  ;
    pstDmCfg->u16ScaleYUV2RGB               = HiHdrCfg->stY2R.u16ScaleY2R;


    pstDmCfg->s32InOffsetYUV2RGB0[0]        = HiHdrCfg->stY2R.as16DcInY2R[0];
    pstDmCfg->s32InOffsetYUV2RGB0[1]        = HiHdrCfg->stY2R.as16DcInY2R[1];
    pstDmCfg->s32InOffsetYUV2RGB0[2]        = HiHdrCfg->stY2R.as16DcInY2R[2];

    pstDmCfg->s32OutOffsetYUV2RGB0[0]       = HiHdrCfg->stY2R.as16DcOutY2R[0];
    pstDmCfg->s32OutOffsetYUV2RGB0[1]       = HiHdrCfg->stY2R.as16DcOutY2R[1];
    pstDmCfg->s32OutOffsetYUV2RGB0[2]       = HiHdrCfg->stY2R.as16DcOutY2R[2];

    pstDmCfg->u16ClipMinYUV2RGB             = HiHdrCfg->stY2R.u16ClipMinY2R;
    pstDmCfg->u16ClipMaxYUV2RGB             = HiHdrCfg->stY2R.u16ClipMaxY2R;



    /********** DeGamma ***********/

    pstDmCfg->DePQPara.gmm_x1_step          = HiHdrCfg->stDeGmm.au32Step[0];
    pstDmCfg->DePQPara.gmm_x2_step          = HiHdrCfg->stDeGmm.au32Step[1];
    pstDmCfg->DePQPara.gmm_x3_step          = HiHdrCfg->stDeGmm.au32Step[2];
    pstDmCfg->DePQPara.gmm_x4_step          = HiHdrCfg->stDeGmm.au32Step[3];
    pstDmCfg->DePQPara.gmm_x1_pos           = HiHdrCfg->stDeGmm.au32Pos[0]    ;
    pstDmCfg->DePQPara.gmm_x2_pos           = HiHdrCfg->stDeGmm.au32Pos[1]    ;
    pstDmCfg->DePQPara.gmm_x3_pos           = HiHdrCfg->stDeGmm.au32Pos[2]    ;
    pstDmCfg->DePQPara.gmm_x4_pos           = HiHdrCfg->stDeGmm.au32Pos[3]    ;
    pstDmCfg->DePQPara.gmm_x1_num           = HiHdrCfg->stDeGmm.au32Num[0];
    pstDmCfg->DePQPara.gmm_x2_num           = HiHdrCfg->stDeGmm.au32Num[1];
    pstDmCfg->DePQPara.gmm_x3_num           = HiHdrCfg->stDeGmm.au32Num[2];
    pstDmCfg->DePQPara.gmm_x4_num           = HiHdrCfg->stDeGmm.au32Num[3];


    //HiHdrCfg->stDeGmm.u32LUT;

    /********** ToneMapping **********/
    pstDmCfg->u16M3LumaCal[0]               = HiHdrCfg->stTMAP.au16M3LumaCal[0];
    pstDmCfg->u16M3LumaCal[1]               = HiHdrCfg->stTMAP.au16M3LumaCal[1];
    pstDmCfg->u16M3LumaCal[2]               = HiHdrCfg->stTMAP.au16M3LumaCal[2];
    pstDmCfg->u16ScaleLumaCal               = HiHdrCfg->stTMAP.u16ScaleLumaCal;
    pstDmCfg->u16ScaleCoefTM                = HiHdrCfg->stTMAP.u16ScaleCoefTM;
    pstDmCfg->u32ClipMinTM                  = HiHdrCfg->stTMAP.u32ClipMinTM;
    pstDmCfg->u32ClipMaxTM                  = HiHdrCfg->stTMAP.u32ClipMaxTM;
    pstDmCfg->s32TMOff0                     = HiHdrCfg->stTMAP.as32TMOff[0];
    pstDmCfg->s32TMOff1                     = HiHdrCfg->stTMAP.as32TMOff[1];
    pstDmCfg->s32TMOff2                     = HiHdrCfg->stTMAP.as32TMOff[2];


    //HiHdrCfg->stTMAP.u32LUTTM;

    pstDmCfg->TMPara.gmm_x1_step            = HiHdrCfg->stTMAP.au32StepTM[0];
    pstDmCfg->TMPara.gmm_x2_step            = HiHdrCfg->stTMAP.au32StepTM[1];
    pstDmCfg->TMPara.gmm_x3_step            = HiHdrCfg->stTMAP.au32StepTM[2];
    pstDmCfg->TMPara.gmm_x4_step            = HiHdrCfg->stTMAP.au32StepTM[3];
    pstDmCfg->TMPara.gmm_x5_step            = HiHdrCfg->stTMAP.au32StepTM[4];
    pstDmCfg->TMPara.gmm_x6_step            = HiHdrCfg->stTMAP.au32StepTM[5];
    pstDmCfg->TMPara.gmm_x7_step            = HiHdrCfg->stTMAP.au32StepTM[6];
    pstDmCfg->TMPara.gmm_x8_step            = HiHdrCfg->stTMAP.au32StepTM[7];
    pstDmCfg->TMPara.gmm_x1_pos             = HiHdrCfg->stTMAP.au32PosTM[0];
    pstDmCfg->TMPara.gmm_x2_pos             = HiHdrCfg->stTMAP.au32PosTM[1];
    pstDmCfg->TMPara.gmm_x3_pos             = HiHdrCfg->stTMAP.au32PosTM[2];
    pstDmCfg->TMPara.gmm_x4_pos             = HiHdrCfg->stTMAP.au32PosTM[3];
    pstDmCfg->TMPara.gmm_x5_pos             = HiHdrCfg->stTMAP.au32PosTM[4];
    pstDmCfg->TMPara.gmm_x6_pos             = HiHdrCfg->stTMAP.au32PosTM[5];
    pstDmCfg->TMPara.gmm_x7_pos             = HiHdrCfg->stTMAP.au32PosTM[6];
    pstDmCfg->TMPara.gmm_x8_pos             = HiHdrCfg->stTMAP.au32PosTM[7];
    pstDmCfg->TMPara.gmm_x1_num             = HiHdrCfg->stTMAP.au32NumTM[0];
    pstDmCfg->TMPara.gmm_x2_num             = HiHdrCfg->stTMAP.au32NumTM[1];
    pstDmCfg->TMPara.gmm_x3_num             = HiHdrCfg->stTMAP.au32NumTM[2];
    pstDmCfg->TMPara.gmm_x4_num             = HiHdrCfg->stTMAP.au32NumTM[3];
    pstDmCfg->TMPara.gmm_x5_num             = HiHdrCfg->stTMAP.au32NumTM[4];
    pstDmCfg->TMPara.gmm_x6_num             = HiHdrCfg->stTMAP.au32NumTM[5];
    pstDmCfg->TMPara.gmm_x7_num             = HiHdrCfg->stTMAP.au32NumTM[6];
    pstDmCfg->TMPara.gmm_x8_num             = HiHdrCfg->stTMAP.au32NumTM[7];




    /************ GamutMapping ***********/

    pstDmCfg->s16M33Src2Disp[0][0]          = HiHdrCfg->stGMAP.as16M33GMAP[0][0];
    pstDmCfg->s16M33Src2Disp[0][1]          = HiHdrCfg->stGMAP.as16M33GMAP[0][1];
    pstDmCfg->s16M33Src2Disp[0][2]          = HiHdrCfg->stGMAP.as16M33GMAP[0][2];
    pstDmCfg->s16M33Src2Disp[1][0]          = HiHdrCfg->stGMAP.as16M33GMAP[1][0];
    pstDmCfg->s16M33Src2Disp[1][1]          = HiHdrCfg->stGMAP.as16M33GMAP[1][1];
    pstDmCfg->s16M33Src2Disp[1][2]          = HiHdrCfg->stGMAP.as16M33GMAP[1][2];
    pstDmCfg->s16M33Src2Disp[2][0]          = HiHdrCfg->stGMAP.as16M33GMAP[2][0];
    pstDmCfg->s16M33Src2Disp[2][1]          = HiHdrCfg->stGMAP.as16M33GMAP[2][1];
    pstDmCfg->s16M33Src2Disp[2][2]          = HiHdrCfg->stGMAP.as16M33GMAP[2][2];
    pstDmCfg->u16ScaleSrc2Disp              = HiHdrCfg->stGMAP.u16ScaleGMAP;
    pstDmCfg->u32ClipMinCSC                 = HiHdrCfg->stGMAP.u32ClipMinGMAP;
    pstDmCfg->u32ClipMaxCSC                 = HiHdrCfg->stGMAP.u32ClipMaxGMAP;




    /************* Gamma ***********************/
    //HiHdrCfg->stGmm.u32LUT;

    pstDmCfg->GammaPara.gmm_x1_step         = HiHdrCfg->stGmm.au32Step[0];
    pstDmCfg->GammaPara.gmm_x2_step         = HiHdrCfg->stGmm.au32Step[1];
    pstDmCfg->GammaPara.gmm_x3_step         = HiHdrCfg->stGmm.au32Step[2];
    pstDmCfg->GammaPara.gmm_x4_step         = HiHdrCfg->stGmm.au32Step[3];
    pstDmCfg->GammaPara.gmm_x5_step         = HiHdrCfg->stGmm.au32Step[4];
    pstDmCfg->GammaPara.gmm_x6_step         = HiHdrCfg->stGmm.au32Step[5];
    pstDmCfg->GammaPara.gmm_x7_step         = HiHdrCfg->stGmm.au32Step[6];
    pstDmCfg->GammaPara.gmm_x8_step         = HiHdrCfg->stGmm.au32Step[7];
    pstDmCfg->GammaPara.gmm_x1_pos          = HiHdrCfg->stGmm.au32Pos[0];
    pstDmCfg->GammaPara.gmm_x2_pos          = HiHdrCfg->stGmm.au32Pos[1];
    pstDmCfg->GammaPara.gmm_x3_pos          = HiHdrCfg->stGmm.au32Pos[2];
    pstDmCfg->GammaPara.gmm_x4_pos          = HiHdrCfg->stGmm.au32Pos[3];
    pstDmCfg->GammaPara.gmm_x5_pos          = HiHdrCfg->stGmm.au32Pos[4];
    pstDmCfg->GammaPara.gmm_x6_pos          = HiHdrCfg->stGmm.au32Pos[5];
    pstDmCfg->GammaPara.gmm_x7_pos          = HiHdrCfg->stGmm.au32Pos[6];
    pstDmCfg->GammaPara.gmm_x8_pos          = HiHdrCfg->stGmm.au32Pos[7];
    pstDmCfg->GammaPara.gmm_x1_num          = HiHdrCfg->stGmm.au32Num[0];
    pstDmCfg->GammaPara.gmm_x2_num          = HiHdrCfg->stGmm.au32Num[1];
    pstDmCfg->GammaPara.gmm_x3_num          = HiHdrCfg->stGmm.au32Num[2];
    pstDmCfg->GammaPara.gmm_x4_num          = HiHdrCfg->stGmm.au32Num[3];
    pstDmCfg->GammaPara.gmm_x5_num          = HiHdrCfg->stGmm.au32Num[4];
    pstDmCfg->GammaPara.gmm_x6_num          = HiHdrCfg->stGmm.au32Num[5];
    pstDmCfg->GammaPara.gmm_x7_num          = HiHdrCfg->stGmm.au32Num[6];
    pstDmCfg->GammaPara.gmm_x8_num          = HiHdrCfg->stGmm.au32Num[7];



    /************** RGB2YUV ********************/

    pstDmCfg->s16M33RGB2YUV[0][0]           = HiHdrCfg->stR2Y.as16M33R2Y[0][0];
    pstDmCfg->s16M33RGB2YUV[0][1]           = HiHdrCfg->stR2Y.as16M33R2Y[0][1];
    pstDmCfg->s16M33RGB2YUV[0][2]           = HiHdrCfg->stR2Y.as16M33R2Y[0][2];
    pstDmCfg->s16M33RGB2YUV[1][0]           = HiHdrCfg->stR2Y.as16M33R2Y[1][0];
    pstDmCfg->s16M33RGB2YUV[1][1]           = HiHdrCfg->stR2Y.as16M33R2Y[1][1];
    pstDmCfg->s16M33RGB2YUV[1][2]           = HiHdrCfg->stR2Y.as16M33R2Y[1][2];
    pstDmCfg->s16M33RGB2YUV[2][0]           = HiHdrCfg->stR2Y.as16M33R2Y[2][0];
    pstDmCfg->s16M33RGB2YUV[2][1]           = HiHdrCfg->stR2Y.as16M33R2Y[2][1];
    pstDmCfg->s16M33RGB2YUV[2][2]           = HiHdrCfg->stR2Y.as16M33R2Y[2][2];
    pstDmCfg->u16ScaleRGB2YUV               = HiHdrCfg->stR2Y.u16ScaleR2Y;
    pstDmCfg->s16InOffsetRGB2YUV[0]         = HiHdrCfg->stR2Y.as16DcInR2Y[0];
    pstDmCfg->s16InOffsetRGB2YUV[1]         = HiHdrCfg->stR2Y.as16DcInR2Y[1];
    pstDmCfg->s16InOffsetRGB2YUV[2]         = HiHdrCfg->stR2Y.as16DcInR2Y[2];
    pstDmCfg->s16OutOffsetRGB2YUV[0]        = HiHdrCfg->stR2Y.as16DcOutR2Y[0];
    pstDmCfg->s16OutOffsetRGB2YUV[1]        = HiHdrCfg->stR2Y.as16DcOutR2Y[1];
    pstDmCfg->s16OutOffsetRGB2YUV[2]        = HiHdrCfg->stR2Y.as16DcOutR2Y[2];
    pstDmCfg->u16ClipMinRGB2YUV             = HiHdrCfg->stR2Y.u16ClipMinR2Y_Y;
    pstDmCfg->u16ClipMaxRGB2YUV             = HiHdrCfg->stR2Y.u16ClipMaxR2Y_Y;



}

HI_S32 VDP_DRV_SetVidBaseCfg(VDP_HISI_HDR_CFG_S *pstHdrCfg)
{
//     HI_U32 ii;
    static VDP_DRV_ADDR_INFO_S stAddrInfo;
    static VDP_DRV_CROP_INFO_S stCropInfo;
    static VDP_DRV_ZME_INFO_S  stZmeInfo;
    HI_U32 u32ChmRatio = 1;//1:422/444; 2:420
    VDP_BKG_S stBkg;

    if(pstHdrCfg->stHdrVdpCfgExt.bRegionEn == HI_FALSE)
    {
        VDP_VID_SetLayerEnable (VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetRegUp(VDP_LAYER_VID0);
        return HI_SUCCESS;
    }
    VDP_CBM_Clear_MixvPrio(VDP_CBM_MIXV0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, VDP_LAYER_VID0,pstHdrCfg->stHdrVdpCfgExt.u32LayerZorder);

    VDP_VID_SetLayerEnable (VDP_LAYER_VID0, HI_TRUE);
    VDP_VID_SetInDataFmt               (VDP_LAYER_VID0, pstHdrCfg->stSrcInfo.enInFmt);
    VDP_VID_SetReadMode                (VDP_LAYER_VID0, pstHdrCfg->stHdrVdpCfgExt.enReadMode, pstHdrCfg->stHdrVdpCfgExt.enReadMode);

    if (pstHdrCfg->stSrcInfo.enSrc3dMode == VDP_DISP_MODE_2D)
    {
        VDP_VID_Set16RegionSmmuLumBypass (VDP_LAYER_VID0, 0, !pstHdrCfg->stSrcInfo.bSmmuEn);
        VDP_VID_Set16RegionSmmuChromBypass (VDP_LAYER_VID0, 0, !pstHdrCfg->stSrcInfo.bSmmuEn);
    }
    else
    {
    //  VDP_VID_Set16RegionSmmuLumBypass (VDP_LAYER_VID0, 0, !HiHdrCfg->stSrcInfo.bSmmuEn);
    //  VDP_VID_Set16RegionSmmuChromBypass (VDP_LAYER_VID0, 0, !HiHdrCfg->stSrcInfo.bSmmuEn);
    //  VDP_VID_Set16RegionSmmuLumBypass (VDP_LAYER_VID0, 1, !HiHdrCfg->stSrcInfo.bSmmuEn);
    //  VDP_VID_Set16RegionSmmuChromBypass (VDP_LAYER_VID0, 1, !HiHdrCfg->stSrcInfo.bSmmuEn);
    }

    VDP_VID_SetDataWidth               (VDP_LAYER_VID0, pstHdrCfg->stSrcInfo.enDataWidth);
    VDP_VID_SetNoSecFlag               (VDP_LAYER_VID0, !pstHdrCfg->stSrcInfo.bSecureEn);

    VDP_VID_SetFlipEnable              (VDP_LAYER_VID0, pstHdrCfg->stHdrVdpCfgExt.bFlipEn);
    VDP_VID_SetTileDcmpEnable          (VDP_LAYER_VID0, pstHdrCfg->stSrcInfo.bDcmpEn);
    VDP_VID_SetIfirMode (VDP_LAYER_VID0, VDP_IFIRMODE_6TAPFIR);

    stCropInfo.stSrcRect   = pstHdrCfg->stSrcInfo.stSrcRect;
    stCropInfo.stCropRect  = pstHdrCfg->stCropRect;
    stCropInfo.enInFmt     = pstHdrCfg->stSrcInfo.enInFmt;
    stCropInfo.enReadMode  = pstHdrCfg->stHdrVdpCfgExt.enReadMode;
    stCropInfo.enDispMode  = pstHdrCfg->stHdrVdpCfgExt.enDispMode;
    stCropInfo.bFlipEn     = pstHdrCfg->stHdrVdpCfgExt.bFlipEn;
    stCropInfo.u32ChmRatio = u32ChmRatio;
    stCropInfo.enDrawMode  = pstHdrCfg->stHdrVdpCfgExt.enDrawMode;

    memcpy(&stCropInfo.stAddr, &pstHdrCfg->stSrcInfo.stAddr, VDP_ADDR_BUTT * sizeof(VDP_ADDR_S));
    VDP_DRV_SetVidCrop                 (VDP_LAYER_VID0, &stCropInfo, &stAddrInfo);

    //----------------------------------------------------------------------
    //addr setting
    //----------------------------------------------------------------------
        stAddrInfo.bFlipEn     = pstHdrCfg->stHdrVdpCfgExt.bFlipEn;
        stAddrInfo.u32Hgt      = pstHdrCfg->stCropRect.u32Hgt;
        stAddrInfo.u32ChmRatio = u32ChmRatio;
        stAddrInfo.enInFmt     = pstHdrCfg->stSrcInfo.enInFmt;
        stAddrInfo.enDispMode  = pstHdrCfg->stHdrVdpCfgExt.enDispMode;
        stAddrInfo.bDcmpEn     = pstHdrCfg->stSrcInfo.bDcmpEn;

        VDP_DRV_SetVidAddr(VDP_LAYER_VID0, &stAddrInfo);

    //----------------------------------------------------------------------
    //zme setting
    //----------------------------------------------------------------------
    stZmeInfo.stInRect    = stCropInfo.stCropRect;
    stZmeInfo.stOutRect   = pstHdrCfg->stVideoRect;
    stZmeInfo.stZme2InRect   = pstHdrCfg->stHdrVdpCfgExt.stZme2InRect;//add by h226871
    //stZmeInfo.stZme1OutRect   = HiHdrCfg->stZme1OutRect;//add by zjy

    stZmeInfo.enInFmt         = pstHdrCfg->stSrcInfo.enInFmt;
    stZmeInfo.enReadMode      = pstHdrCfg->stHdrVdpCfgExt.enReadMode;
    stZmeInfo.enDispMode      = pstHdrCfg->stHdrVdpCfgExt.enDispMode;
    stZmeInfo.enDrawMode      = pstHdrCfg->stHdrVdpCfgExt.enDrawMode;
    stZmeInfo.enDrawPixelMode = pstHdrCfg->stHdrVdpCfgExt.enDrawPixelMode;

    stZmeInfo.enHzmeMode  = pstHdrCfg->stHdrVdpCfgExt.enHzmeMode;
    stZmeInfo.bHzmeEn     = pstHdrCfg->stHdrVdpCfgExt.bHzmeEn;


    memcpy(&stZmeInfo.pstZmeDrvPara, &pstHdrCfg->pstZmeDrvPara,  sizeof(VDP_VZME_DRV_PARA_S));


    VDP_DRV_SetVidZme                  (VDP_LAYER_VID0, &stZmeInfo);
    VDP_VID_SetVideoPos                (VDP_LAYER_VID0, pstHdrCfg->stVideoRect);
    VDP_VID_SetDispPos                 (VDP_LAYER_VID0, pstHdrCfg->stDispRect);
    //----------------------------------------------------------------------
    //VP0 setting
    //----------------------------------------------------------------------
    if (pstHdrCfg->eHiHdrMode == HIHDR_HDR10_IN_SDR_OUT
        || pstHdrCfg->eHiHdrMode == HIHDR_HLG_IN_SDR_OUT
        || pstHdrCfg->eHiHdrMode == HIHDR_AVS_IN_SDR_OUT
        || pstHdrCfg->eHiHdrMode == HIHDR_TECHNI_IN_SDR_OUT
        || pstHdrCfg->eHiHdrMode == HIHDR_HLG_IN_HDR10_OUT
       )
    {
        stBkg.u32BkgY  = 0x0;
        stBkg.u32BkgU  = 0x0;
        stBkg.u32BkgV  = 0x0;
        stBkg.u32BkgA  = 0xff;
    }
    else
    {
        stBkg.u32BkgY  = 0x40;
        stBkg.u32BkgU  = 0x200;
        stBkg.u32BkgV  = 0x200;
        stBkg.u32BkgA  = 0xff;
    }

    VDP_CBM_SetMixerBkg(VDP_CBM_MIXV0, stBkg);


    VDP_VP_SetVideoPos             (VDP_LAYER_VP0, pstHdrCfg->stVdpDispInfo.stFmtResolution);
    VDP_VP_SetDispPos              (VDP_LAYER_VP0, pstHdrCfg->stVdpDispInfo.stFmtResolution);
    VDP_VP_SetInReso               (VDP_LAYER_VP0, pstHdrCfg->stVdpDispInfo.stFmtResolution);

    VDP_VP_SetLayerGalpha          (VDP_LAYER_VP0, 0xff);

    //VDP_DRV_SetVidCsc(VDP_LAYER_VID0, pstHdrCfg->enInClrSpace, pstHdrCfg->enOutClrSpace);

    return HI_SUCCESS;

}



HI_S32 VDP_DRV_SetHisiHdrMode(VDP_HISI_HDR_CFG_S *pstHdrCfg,HI_PQ_HDR_CFG  *pstPqHdrCfg)
{
    DmCfg_t  pstDmCfg;


    memset((void *)&pstDmCfg,0,sizeof(pstDmCfg));

    pstDmCfg.VdmSceneMode                   = pstHdrCfg->eHiHdrMode ;

    pstDmCfg.vdm_tmap_in_bdp                = 1 ;//1:20bit for vdm , 0:16bit for gdm
#if !CBB_CFG_NOT_FROM_PQ
    VDP_DRV_SetHiHdrAjust(pstPqHdrCfg,&pstDmCfg);
#endif

    VDP_VID_SetMuteEnable(VDP_LAYER_VID0, pstHdrCfg->bMuteEn);
    VDP_VP_SetMuteEnable(VDP_LAYER_VP0, pstHdrCfg->bMuteEn);
    VDP_VID_SetLayerGalpha(VDP_LAYER_VID0, 0xff);
//send coef !!!
    if(pstHdrCfg->eHiHdrMode != HIHDR_HDR10_IN_HDR10_OUT)
    {
        Vdp_Drv_SetVdmCoef_gmm      (pstHdrCfg->eHiHdrMode, pstPqHdrCfg);
        Vdp_Drv_SetVdmCoef_Degmm    (pstHdrCfg->eHiHdrMode, pstPqHdrCfg);
        Vdp_Drv_SetVdmCoef_Tmapping (pstHdrCfg->eHiHdrMode, pstPqHdrCfg);
    }

    VDP_DRV_SetVidBaseCfg(pstHdrCfg);
    VDP_DRV_GetHiHdrCfg(pstHdrCfg,&pstDmCfg);
    VDP_DRV_SetHiHdrCfg(&pstDmCfg);

//gfx
    //send coef !!!

    Vdp_Drv_SetGdmCoef_Tmmp(pstHdrCfg->enHiHdrGfxMode);
    Vdp_Drv_SetGdmCoef_Gmma(pstHdrCfg->enHiHdrGfxMode);
    //setting
    VDP_DRV_SetGdmMode(pstHdrCfg->enHiHdrGfxMode);
    VDP_VID_SetRegUp(VDP_LAYER_VID0);
    VDP_VP_SetRegUp(VDP_LAYER_VP0);

    return HI_SUCCESS;
//    free(pstDmCfg);
}


HI_VOID VDP_DRV_HiSiHdrMute(HI_BOOL bMuteEn)
{
    VDP_VP_SetMuteEnable   (VDP_LAYER_VP0, bMuteEn);

    VDP_VP_SetRegUp(VDP_LAYER_VP0);

    return;
}


HI_VOID VDP_DRV_HiSiHdrClean(HI_BOOL bclear)
{
    if (!bclear)
    {
        VDP_VID_SetVdmVdmGmmEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmTmapEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmDegmmEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmRgb2yuvEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmV0CscEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmDitherEn(VDP_LAYER_VID0, HI_FALSE);
        VDP_VID_SetVdmVdmYuv2rgbEn(VDP_LAYER_VID0, HI_FALSE);

        VDP_GP_SetGdmGdmGmmEn(VDP_LAYER_GP0, HI_FALSE);
        VDP_GP_SetGdmGdmTmapEn(VDP_LAYER_GP0, HI_FALSE);
        VDP_GP_SetGdmGdmDegmmEn(VDP_LAYER_GP0, HI_FALSE);
        VDP_GP_SetGdmGdmEn(VDP_LAYER_GP0, HI_FALSE);
    }
    else
    {
        //do nothing.
    }

    return;
}





