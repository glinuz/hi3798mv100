#include "hi_type.h"
#include "vdp_hal_vid.h"
#include "vdp_drv_coef.h"
#include "drv_pq_ext.h"
#include "vdp_drv_pq_csc.h"
#include "vdp_func_pq_csc.h"
#include "vdp_hal_comm.h"
#include "vdp_drv_comm.h"


extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;

HI_S32 VDP_PQ_SetCSCCoef(HI_PQ_CSC_CRTL_S* pstCscCtrl,VDP_CSC_MODE_E enCbbCscMode, HI_PQ_CSC_MODE_E enCscMode, HI_PQ_CSC_COEF_S* pstCscCoef, HI_PQ_CSC_DCCOEF_S* pstCscDcCoef)
{
#ifdef VDP_PQ_STRATEGY
	DRV_PQ_GetCSCCoef(pstCscCtrl, enCscMode,pstCscCoef, pstCscDcCoef);
#else
	VDP_FUNC_GetCSCCoef(pstCscCtrl, enCbbCscMode,pstCscCoef, pstCscDcCoef);
#endif

    return HI_SUCCESS;

}

HI_S32 VDP_DRV_SetVidWcgGmmCoef(HI_U32 *pu32LutX, HI_U32 *pu32LutA, HI_U32 *pu32LutB)
{
    VDP_COEF_SEND_CFG   stCoefSend;
    
#if EDA_TEST
    HI_U8   * input_addr  = pVdpReg->VDM_COEFAD1.bits.vdm_coef_addr1 ;//need modify when copy to it
    HI_U8   * output_addr = pVdpReg->CVM1_GAMMA_COEFAD.bits.gamma_coef_addr ;//need modify when copy to it
#elif VDP_CBB_FPGA
	HI_U8   * input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA] ;//add by hyx
	//HI_U32 output_addr = gstVdpCoefBufAddr.u32CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA] ;//add by hyx
#else
    HI_U32 input_addr = 0 ;
   // HI_U32 output_addr = 0 ;
#endif
    //VDP_COEF_GEN_CFG    stCoefGen;
    void* p_coef_array []     = {pu32LutX,pu32LutA,pu32LutB};
    
    HI_U32 lut_length[]     = {128,128,128};
    HI_U32 coef_bit_length[]= {32,28,16};

    FILE* fp_dm_input_gmm_coef = NULL;
   // FILE* fp_dm_output_gmm_coef;

	VDP_HDR_SetVDmGammaCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA] );
	VDP_HDR_SetVCvmGammaCoefAddr   (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_GAMMA] );

    //send coef input gamma
    stCoefSend.coef_addr         = input_addr               ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ; 
    stCoefSend.fp_coef           = fp_dm_input_gmm_coef     ; 
    stCoefSend.lut_num           = 3                        ; 
    stCoefSend.burst_num         = 1                        ; 
    stCoefSend.cycle_num         = 1                        ; 
    stCoefSend.p_coef_array      = p_coef_array             ; 
    stCoefSend.lut_length        = lut_length               ; 
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 
    VDP_FUNC_SendCoef(&stCoefSend);

    VDP_FUNC_SendCoef(&stCoefSend);
	VDP_HDR_SetDmInputGammaParaup  ();
	VDP_HDR_SetDmOutputGammaParaup ();

    //fclose(fp_dm_output_gmm_coef);
   // fclose(fp_dm_input_gmm_coef);

    return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetVidWcgDeGmmCoef(HI_U32 u32Layer, HI_U32 *pu32DegmmCoef)
{
#if VDP_CBB_FPGA
  HI_U8   *input_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA] ;//add by hyx
 // HI_U32   output_addr = gstVdpCoefBufAddr.u32CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA];//add by hyx
#elif EDA_TEST
    HI_U8   * input_addr  = pVdpReg->VDM_COEFAD.bits.vdm_coef_addr;//need modify when copy to it
    //HI_U32 output_addr = pVdpReg->CVM1_DEGAMMA_COEFAD.bits.degamma_coef_addr ;//need modify when copy to it
#else
    HI_U32 input_addr = 0 ;
   // HI_U32 output_addr = 0 ;
#endif

   // bOpenStiFile(STI_FILE_COEF_DM);
    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;

    void*  p_coef_array[1] =  {pu32DegmmCoef}   ;
    HI_U32   lut_length[1]      = {256 }                    ; 
    HI_U32   coef_bit_length[1] = {32  }                    ; 

    FILE*   fp_dm_input_degmm_coef=NULL;    
//    FILE*   fp_dm_output_degmm_coef;    
    
    VDP_HDR_SetVDmDegmmCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA]);
    VDP_HDR_SetVCvmDegmmCoefAddr   (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA]);
    //send coef cfg(input degamma)


    stCoefSend.coef_addr         = input_addr          ; 
    stCoefSend.sti_type          = STI_FILE_COEF_DM    ; 
    stCoefSend.fp_coef           = fp_dm_input_degmm_coef  ; 
    stCoefSend.lut_num           = 1                   ; 
    stCoefSend.burst_num         = 1                   ; 
    stCoefSend.cycle_num         = 4                   ; 
    stCoefSend.p_coef_array      = p_coef_array        ; 
    stCoefSend.lut_length        = lut_length          ; 
    stCoefSend.coef_bit_length   = coef_bit_length     ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 

    VDP_FUNC_SendCoef(&stCoefSend);

	VDP_HDR_SetDmInputDegmmParaup  ();
	VDP_HDR_SetDmOutputDegmmParaup ();

    return HI_SUCCESS;
}


HI_S32 VDP_DRV_SetGfxWcgGmmCoef(HI_U32 u32Layer, HI_U16 *pu16GmmCoef)
{

    return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetGfxWcgDeGmmCoef(HI_U32 u32Layer, HI_U16 *pu16DegmmCoef)
{

    return HI_SUCCESS;
}

