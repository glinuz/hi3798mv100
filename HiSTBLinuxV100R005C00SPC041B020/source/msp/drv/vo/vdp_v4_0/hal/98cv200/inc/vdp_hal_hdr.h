#ifndef __HAL_HDR_H__
#define __HAL_HDR_H__

#include "vdp_define.h"
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif

//-------------------------------------------------------------------
//composer
//-------------------------------------------------------------------

// HI_VOID VDP_VID_SetComposerCoefLog2Denom  ( HI_U32 coef_log2_denom);
HI_VOID VDP_VID_SetComposerBlEn              ( HI_U32 bl_en);
HI_VOID VDP_VID_SetComposerBlNumPivotsY      ( HI_U32 bl_num_pivots);
HI_VOID VDP_VID_SetComposerBlBitDepth        ( HI_U32 bl_bit_depth);
HI_VOID VDP_VID_SetComposerBlPolyPivotValueY ( HI_U32 bl_pivot_value[9]);
HI_VOID VDP_VID_SetComposerBlPolyOrderY      ( HI_U32 bl_order[8]);
HI_VOID VDP_VID_SetComposerBlPolyCoefY       ( HI_S32 bl_coef[8][3]);
HI_VOID VDP_VID_SetComposerBlNumPivotsV      ( HI_U32 bl_num_pivots_v);
HI_VOID VDP_VID_SetComposerBlNumPivotsU      ( HI_U32 bl_num_pivots_u);
HI_VOID VDP_VID_SetComposerMappingIdcV       ( HI_U32 mapping_idc_v);
HI_VOID VDP_VID_SetComposerMappingIdcU       ( HI_U32 mapping_idc_u);
HI_VOID VDP_VID_SetComposerBlPolyPivotValueU ( HI_U32 bl_pivot_value[5]);
HI_VOID VDP_VID_SetComposerBlPolyOrderU      ( HI_U32 bl_order[4]);
HI_VOID VDP_VID_SetComposerBlPolyCoefU       ( HI_S32 bl_coef[4][3]);
HI_VOID VDP_VID_SetComposerBlMmrOrderU       ( HI_U32 bl_mmr_order_u);
HI_VOID VDP_VID_SetComposerBlMmrCoefU        ( HI_S32 bl_mmr_coef_u[22]);
HI_VOID VDP_VID_SetComposerBlPolyPivotValueV ( HI_U32 bl_pivot_value[5]);
HI_VOID VDP_VID_SetComposerBlPolyOrderV      ( HI_U32 bl_order[4]);
HI_VOID VDP_VID_SetComposerBlPolyCoefV       ( HI_S32 bl_coef[4][3]);
HI_VOID VDP_VID_SetComposerBlMmrOrderV       ( HI_U32 bl_mmr_order_v);
HI_VOID VDP_VID_SetComposerBlMmrCoefV        ( HI_S32 bl_mmr_coef_v[22]);
HI_VOID VDP_VID_SetComposerVdrBitDepth       ( HI_U32 vdr_bit_depth);
HI_VOID VDP_VID_SetComposerElUpsampleEn      ( HI_U32 el_upsample_en);
HI_VOID VDP_VID_SetComposerElEn              ( HI_U32 el_en);
HI_VOID VDP_VID_SetComposerElBitDepth        ( HI_U32 el_bit_depth);
HI_VOID VDP_VID_SetComposerElNldqOffsetY     ( HI_U32 el_nldq_offset_y);
HI_VOID VDP_VID_SetComposerElNldqCoefY       ( HI_S32 el_nldq_coef_y[3]);
HI_VOID VDP_VID_SetComposerElNldqOffsetU     ( HI_U32 el_nldq_offset_u);
HI_VOID VDP_VID_SetComposerElNldqCoefU       ( HI_S32 el_nldq_coef_u[3]);
HI_VOID VDP_VID_SetComposerElNldqOffsetV     ( HI_U32 el_nldq_offset_v);
HI_VOID VDP_VID_SetComposerElNldqCoefV       ( HI_S32 el_nldq_coef_v[3]);
HI_VOID VDP_HDR_SetGCvmLms2IptMin(HI_U32 g_imap_lms2iptmin);
HI_VOID VDP_HDR_SetGCvmLms2IptMax(HI_U32 g_imap_lms2iptmax);

//-------------------------------------------------------------------
//DM
//-------------------------------------------------------------------
//up sample
HI_VOID VDP_VID_SetComposeElUpsampleMax(HI_U32 el_upsample_max);
HI_VOID VDP_VID_SetComposeElUpsampleMin(HI_U32 el_upsample_min);


//vdm

HI_VOID VDP_HDR_SetVDmInEotf              ( HI_U32 v_in_eotf                )  ; 
HI_VOID VDP_HDR_SetVDmInBits              ( HI_U32 v_in_bits                )  ; 
HI_VOID VDP_HDR_SetVDmInColor             ( HI_U32 v_in_color               )  ; 
HI_VOID VDP_HDR_SetVDmGammaEn             ( HI_U32 v_gamma_en               )  ; 
HI_VOID VDP_HDR_SetVDmRgb2lmsEn           ( HI_U32 v_rgb2lms_en             )  ; 
HI_VOID VDP_HDR_SetVDmDegammaEn           ( HI_U32 v_degamma_en             )  ; 
HI_VOID VDP_HDR_SetVDmYuv2rgbEn           ( HI_U32 v_yuv2rgb_en             )  ; 
HI_VOID VDP_HDR_SetVDmYuv2rgbV0En         ( HI_U32 vdm_v0_csc_en            )  ; 
HI_VOID VDP_HDR_SetVDmYuv2rgbV1En         ( HI_U32 vdm_v1_csc_en            )  ; 
HI_VOID VDP_HDR_SetVDmGammaV0En           ( HI_U32 vdm_v0_gmm_en            ) ;
HI_VOID VDP_HDR_SetVDmGammaV1En           ( HI_U32 vdm_v1_gmm_en            ) ;
HI_VOID VDP_HDR_SetVDmEn                  ( HI_U32 v_vdm_en                 )  ; 
HI_VOID VDP_HDR_SetVDmNormEn              ( HI_U32 v_norm_en                )  ; 
HI_VOID VDP_HDR_SetVDmImapRangeMin        ( HI_U32 v_imap_range_min         )  ; 
HI_VOID VDP_HDR_SetVDmImapRange           ( HI_U32 v_imap_range             )  ; 
HI_VOID VDP_HDR_SetVDmImapRangeInv        ( HI_U32 v_imap_range_inv         )  ; 
HI_VOID VDP_HDR_SetVDmImap0Yuv2rgb        ( HI_S16 v_imap_yuv2rgb[3][3]     )  ; 
HI_VOID VDP_HDR_SetVDmImap1Yuv2rgb        ( HI_S16 v_imap_yuv2rgb[3][3]     )  ; 
HI_VOID VDP_HDR_SetVDmImapYuv2rgbscale2p  ( HI_U32 v_imap_yuv2rgbscale2p    )  ; 
HI_VOID VDP_HDR_SetVDmImapYuv2rgbInDc0    ( HI_S32 v_imap_yuv2rgb_in_dc[3]  )  ; 
HI_VOID VDP_HDR_SetVDmImapYuv2rgbInDc1    ( HI_S32 v_imap_yuv2rgb_in_dc[3]  )  ; 
HI_VOID VDP_HDR_SetVDmImapYuv2rgbOutDc0   ( HI_S32 v_imap_yuv2rgb_out_dc[3] )  ; 
HI_VOID VDP_HDR_SetVDmImapYuv2rgbOutDc1   ( HI_S32 v_imap_yuv2rgb_out_dc[3] )  ; 
HI_VOID VDP_HDR_SetVDmImapRgb2lms         ( HI_S16 v_imap_rgb2lms[3][3]     )  ; 
HI_VOID VDP_HDR_SetVDmImapRgb2lmsscale2p  ( HI_U32 v_imap_rgb2lmsscale2p    )  ; 
HI_VOID VDP_HDR_SetVDmImapRgb2lmsmin      ( HI_U32 v_imap_rgb2lmsmin        )  ; 
HI_VOID VDP_HDR_SetVDmImapRgb2lmsmax      ( HI_U32 v_imap_rgb2lmsmax        )  ; 
HI_VOID VDP_HDR_SetVDmDegammaRdEn         ( HI_U32 degamma_rd_en            )  ; 
HI_VOID VDP_HDR_SetVDmImapLms2iptmin      ( HI_U32 v_imap_lms2iptmin        )  ; 
HI_VOID VDP_HDR_SetVDmImapLms2iptmax      ( HI_U32 v_imap_lms2iptmax        )  ; 
HI_VOID VDP_HDR_SetVDmOmapIpt2lmsmin      ( HI_U32 v_omap_ipt2lmsmin        )  ; 
HI_VOID VDP_HDR_SetVDmOmapIpt2lmsmax      ( HI_U32 v_omap_ipt2lmsmax        )  ; 
HI_VOID VDP_HDR_SetVDmOmapLms2rgbmin      ( HI_U32 v_omap_lms2rgbmin        )  ; 
HI_VOID VDP_HDR_SetVDmOmapLms2rgbmax      ( HI_U32 v_omap_lms2rgbmax        )  ; 
HI_VOID VDP_HDR_SetVDmImapIptoff          ( HI_S32 imap_iptoff[3]           )  ; 
HI_VOID VDP_HDR_SetVDmImapIptScale        ( HI_U32 scale                    )  ; 
HI_VOID VDP_HDR_SetVDmGammaLutbRdEn       ( HI_U32 gamma_lutb_rd_en         )  ; 
HI_VOID VDP_HDR_SetVDmGammaLutaRdEn       ( HI_U32 gamma_luta_rd_en         )  ; 
HI_VOID VDP_HDR_SetVDmGammaLutxRdEn       ( HI_U32 gamma_lutx_rd_en         )  ; 
HI_VOID VDP_HDR_SetVDmDegmmCoefAddr       ( HI_U32 vdm_coef_addr            )  ; 
HI_VOID VDP_HDR_SetVDmGammaCoefAddr       ( HI_U32 vdm_coef_addr1           )  ; 
HI_VOID VDP_HDR_SetVCvmGammaCoefAddr      ( HI_U32 gamma_coef_addr          )  ;
HI_VOID VDP_HDR_SetVCvmVcvmCoefAddr       ( HI_U32 vcvm_coef_addr           )  ;


//gdm
HI_VOID VDP_HDR_SetGDmGammaEn             ( HI_U32 g_gamma_en               )  ; 
HI_VOID VDP_HDR_SetGDmRgb2lmsEn           ( HI_U32 g_rgb2lms_en             )  ; 
HI_VOID VDP_HDR_SetGDmDegammaEn           ( HI_U32 g_degamma_en             )  ; 
HI_VOID VDP_HDR_SetGDmEn                  ( HI_U32 g_gdm_en                 )  ; 
HI_VOID VDP_HDR_SetGDmImapRgb2lms         ( HI_S16 g_imap_rgb2lms[3][3]     )  ; 
HI_VOID VDP_HDR_SetGDmImapRgb2lmsscale2p  ( HI_U32 g_imap_rgb2lmsscale2p    )  ; 
HI_VOID VDP_HDR_SetGDmImapRgb2lmsmin      ( HI_U32 g_imap_rgb2lmsmin        )  ; 
HI_VOID VDP_HDR_SetGDmImapRgb2lmsmax      ( HI_U32 g_imap_rgb2lmsmax        )  ; 
HI_VOID VDP_HDR_SetGDmImapRgb2lmsOutDc    ( HI_S32 g_imap_rgb2lms_out_dc[3] )  ; 
HI_VOID VDP_HDR_SetGDmGammaRdEn           ( HI_U32 gamma_rd_en              )  ; 
HI_VOID VDP_HDR_SetGDmDegammaRdEn         ( HI_U32 degamma_rd_en            )  ; 
HI_VOID VDP_HDR_SetGDmDegmmStep           ( HI_U32 g_degmm_step[4]          )  ; 
HI_VOID VDP_HDR_SetGDmGammaStep           ( HI_U32 g_gmm_step[4]            )  ; 
HI_VOID VDP_HDR_SetGDmDegmmNum            ( HI_U32 g_degmm_num[4]           ) ;
HI_VOID VDP_HDR_SetGDmGmmNum              ( HI_U32 g_gmm_num[4]             ) ;
HI_VOID VDP_HDR_SetGDmDegmmPos            ( HI_U32 g_degmm_pos[4]           )  ; 
HI_VOID VDP_HDR_SetGDmGammaPos            ( HI_U32 g_gmm_pos[4]             )  ; 

//cvm

HI_VOID VDP_HDR_SetCvmSel                 ( HI_U32 cvm_sel                  )  ; 
HI_VOID VDP_HDR_SetCvmEn                  ( HI_U32 cvm_en                   )  ; 
HI_VOID VDP_HDR_SetCvmOutColor            ( HI_U32 cvm_out_color            )  ; 
HI_VOID VDP_HDR_SetCvmOutBits             ( HI_U32 cvm_out_bits             )  ; 
HI_VOID VDP_HDR_SetVCvmOutOetf            ( HI_U32 cvm_out_oetf             )  ; 
HI_VOID VDP_HDR_SetVCvmDenormEn           ( HI_U32 denorm_en                )  ; 
HI_VOID VDP_HDR_SetVCvmRgb2yuvEn          ( HI_U32 v_rgb2yuv_en             )  ; 
HI_VOID VDP_HDR_SetVCvmMGammaEn           ( HI_U32 v_gamma_en               )  ; 
HI_VOID VDP_HDR_SetVCvmLms2rgbEn          ( HI_U32 v_lms2rgb_en             )  ; 
HI_VOID VDP_HDR_SetVCvmDegammaEn          ( HI_U32 v_degamma_en             )  ; 
HI_VOID VDP_HDR_SetVCvmIpt2lmsEn          ( HI_U32 v_ipt2lms_en             )  ; 
HI_VOID VDP_HDR_SetVCvmEn                 ( HI_U32 vcvm_en                  )  ; 
HI_VOID VDP_HDR_SetVCvmLms2iptEn          ( HI_U32 v_lms2ipt_en             )  ; 
HI_VOID VDP_HDR_SetVCvmImapLms2ipt        ( HI_S16 v_imap_lms2ipt[3][3]     )  ; 
HI_VOID VDP_HDR_SetVCvmImapLms2iptscale2p ( HI_U32 v_imap_lms2iptscale2p    )  ; 
HI_VOID VDP_HDR_SetVCvmOmapIpt2lms        ( HI_S16 v_omap_ipt2lms[3][3]     )  ; 
HI_VOID VDP_HDR_SetVCvmOmapIpt2lmsscale2p ( HI_U32 v_omap_ipt2lmsscale2p    )  ; 
HI_VOID VDP_HDR_SetVCvmOmapLms2rgb        ( HI_S16 v_omap_lms2rgb[3][3]     )  ; 
HI_VOID VDP_HDR_SetVCvmOmapLms2rgbscale2p ( HI_U32 v_omap_lms2rgbscale2p    )  ; 
HI_VOID VDP_HDR_SetVCvmOmapRgb2yuv        ( HI_S16 v_omap_rgb2yuv[3][3]     )  ; 
HI_VOID VDP_HDR_SetVCvmOmapRgb2yuvscale2p ( HI_U32 v_omap_rgb2yuvscale2p    )  ; 
HI_VOID VDP_HDR_SetVCvmOmapRgb2yuvOutDc   ( HI_S32  * v_omap_rgb2yuv_out_dc )  ; 
HI_VOID VDP_HDR_SetVCvmOmapRangeOver      ( HI_U32 v_omap_range_over        )  ; 
HI_VOID VDP_HDR_SetVCvmOmapRangeMin       ( HI_U32 v_omap_range_min         )  ; 
HI_VOID VDP_HDR_SetVCvmSslutRdEn          ( HI_U32 vcvm_sslut_rd_en         )  ; 
HI_VOID VDP_HDR_SetVCvmTslutRdEn          ( HI_U32 vcvm_tslut_rd_en         )  ; 
HI_VOID VDP_HDR_SetVCvmSilutRdEn          ( HI_U32 vcvm_silut_rd_en         )  ; 
HI_VOID VDP_HDR_SetVCvmTilutRdEn          ( HI_U32 vcvm_tilut_rd_en         )  ; 
HI_VOID VDP_HDR_SetVCvmL2pqLutbRdEn       ( HI_U32 l2pq_lutb_rd_en          )  ; 
HI_VOID VDP_HDR_SetVCvmL2pqLutaRdEn       ( HI_U32 l2pq_luta_rd_en          )  ; 
HI_VOID VDP_HDR_SetVCvmL2pqLutxRdEn       ( HI_U32 l2pq_lutx_rd_en          )  ; 
HI_VOID VDP_HDR_SetVCvmOmapIptoff         ( HI_S32 omap_iptoff[3]           )  ; 
HI_VOID VDP_HDR_SetVCvmOmapIptScale       ( HI_U32 scale                    )  ; 
HI_VOID VDP_HDR_SetVCvmPq2lRdEn           ( HI_U32 pq2l_rd_en               )  ; 
HI_VOID VDP_HDR_SetVCvmDegmmCoefAddr      ( HI_U32 degamma_coef_addr        )  ; 

//gcvm

HI_VOID VDP_HDR_SetGCvmRgb2yuvEn          ( HI_U32 g_rgb2yuv_en             )  ; 
HI_VOID VDP_HDR_SetGCvmEn                 ( HI_U32 gcvm_en                  )  ; 
HI_VOID VDP_HDR_SetGCvmOmapRgb2yuv        ( HI_S32 g_omap_rgb2yuv[3][3]     )  ; 
HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvscale2p ( HI_U32 g_omap_rgb2yuvscale2p    )  ; 
HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvOutDc   ( HI_S32 g_omap_rgb2yuv_out_dc[3] )  ; 
HI_VOID VDP_HDR_SetGcvmX2Pos              ( HI_U32 g_gcvm_x2_pos            )  ; 
HI_VOID VDP_HDR_SetGcvmX1Pos              ( HI_U32 g_gcvm_x1_pos            )  ; 
HI_VOID VDP_HDR_SetGcvmX1B                ( HI_U32 g_gcvm_x1_b              )  ; 
HI_VOID VDP_HDR_SetGcvmX1A                ( HI_U32 g_gcvm_x1_a              )  ; 
HI_VOID VDP_HDR_SetGcvmX2B                ( HI_U32 g_gcvm_x2_b              )  ; 
HI_VOID VDP_HDR_SetGcvmX2A                ( HI_U32 g_gcvm_x2_a              )  ; 
HI_VOID VDP_HDR_SetGcvmX3B                ( HI_U32 g_gcvm_x3_b              )  ; 
HI_VOID VDP_HDR_SetGcvmX3A                ( HI_U32 g_gcvm_x3_a              )  ; 
//other
HI_VOID VDP_HDR_SetDmHdrDsScale2p         ( HI_U32 hdr_ds_scale2p           )  ; 

HI_VOID VDP_HDR_SetDmInputDegmmParaup(HI_VOID);
HI_VOID VDP_HDR_SetDmInputGammaParaup(HI_VOID);
HI_VOID VDP_HDR_SetDmCvmParaup(HI_VOID);
HI_VOID VDP_HDR_SetDmOutputDegmmParaup(HI_VOID);
HI_VOID VDP_HDR_SetDmOutputGammaParaup(HI_VOID);

//metadata
HI_VOID VDP_HDR_SetDmHdrDsEn              ( HI_U32 hdr_ds_en                ) ;
HI_VOID VDP_HDR_SetDmHdrDsMax             ( HI_U32 hdr_ds_max               ) ;
HI_VOID VDP_HDR_SetDmHdrDsMin             ( HI_U32 hdr_ds_min               ) ;
HI_VOID VDP_HDR_SetDmHdrMdLen             ( HI_U32 hdr_md_len               ) ;
HI_VOID VDP_HDR_SetDmHdrMdEn              ( HI_U32 hdr_md_en                ) ;
HI_VOID VDP_HDR_SetDmHdrMdEos             ( HI_U32 hdr_md_eos               ) ;
HI_VOID VDP_HDR_SetDmHdrMdId              ( HI_U32 hdr_md_id                ) ;
HI_VOID VDP_HDR_SetDmHdrMdType            ( HI_U32 hdr_md_type              ) ;
HI_VOID VDP_HDR_SetDmHdrMdVer             ( HI_U32 hdr_md_ver               ) ;
HI_VOID VDP_HDR_SetDmHdrNoMd              ( HI_U32 hdr_no_md                ) ;
HI_VOID VDP_HDR_SetDmMetadataRdEn         ( HI_U32 metadata_rd_en           ) ;
HI_VOID VDP_HDR_SetDmMetadataCoefAddr     ( HI_U32 metadata_coef_addr       ) ;
HI_VOID VDP_HDR_SetDmEdrEn                ( HI_U32 edr_en                   ) ;
HI_VOID VDP_HDR_SetDmMdParaup             (HI_VOID);

HI_VOID VDP_HDR_SetGCvmLms2IptEn(HI_U32 g_lms2ipt_en);
HI_VOID VDP_HDR_SetGCvmLms2Ipt(HI_S16 g_lms2ipt[3][3]);
HI_VOID VDP_HDR_SetGCvmLms2IptScale2p(HI_U32 g_lms2ipt_scale2p);
HI_VOID VDP_HDR_SetGCvmLms2IptOutDc(HI_S32 g_lms2ipt_out_dc[3]);
HI_VOID VDP_HDR_SetAuthEn(HI_U32 gauth_en);


HI_VOID VDP_HDR_SetHdrEnable(HI_U32 hdr_en);
HI_VOID VDP_HDR_SetRegUp(HI_VOID);

HI_VOID VDP_GP_SetGdmDegammAddr(HI_U32 u32Data, HI_U32 u32GdmDegammAddr);
HI_VOID VDP_GP_SetGdmGammAddr(HI_U32 u32Data, HI_U32 u32GdmGammAddr);
HI_VOID VDP_HDR_SetGdmParaUpd(HI_U32 u32Data, VDP_GP_PARA_E enMode);
HI_VOID VDP_HDR_SetVCvmImapLms2iptOutDc(HI_S32  * v_imap_lms2ipt_out_dc);
HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvmin(HI_U32 g_imap_lms2iptmin);
HI_VOID VDP_HDR_SetGCvmOmapRgb2yuvmax(HI_U32 g_imap_lms2iptmax);


#endif
