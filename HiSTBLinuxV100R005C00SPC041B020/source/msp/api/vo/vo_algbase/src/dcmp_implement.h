/*******************************************************************
 * File Name:		ttv_enc_if.h
 * Authors:			benqiang
 * Data:			2014.03.07
 * Copyright:		tongtuic technology
 * Description:		video encoder interface
 *
 *******************************************************************/
#ifndef _TTV_CODEC_LC_IF_H_
#define _TTV_CODEC_LC_IF_H_

#ifdef __cplusplus
extern "C" {
#endif



typedef struct  
{
	int is_raw_cmp;                 //1: is raw frame compress mode             0: is not ...
	int is_luma;                    //1: input is luma component                0: input is chroma component
	int is_lossless;                //1: is lossless compress                   0:is lossy compress 

	int frm_width;                  //input frame width
	int frm_height;                 //input frame height
	int bit_dep;                    //pixel bit depth
	
	int is_stride_mem;              //pCmpData format in memory 1: stride          0: frame
	int stride;                     //pCmpData stride(unit is 128bit)

	/* register parameter of ratio control*/
	int budget_bit_mb;              
	int pix_diff_thr;
	int smth_thr;
	int smth_pix_num_thr;
	int adj_sad_thr;
	int adj_sad_bit_thr;
	int qp_change1_bits_thr;
	int qp_change2_bits_thr;
	int max_qp;
	int smth_qp0;
	int smth_qp1;
	int smth_lftbit_thr0;
	int smth_lftbit_thr1;
	int sadbits_ngain;
	int rc_smth_ngain;
	int ri_bits;
	int max_rg_bits_comps;


}CMP_LC_PARM_T;

typedef struct  
{
	int is_raw_cmp;                 //1: is raw frame compress mode             0: is not ...
	int is_luma;                    //1: input is luma component                0: input is chroma component
	int is_lossless;                //1: is lossless compress                   0: is lossy compress

	int frm_width;                  //input frame width
	int frm_height;                 //input frame height
	int bit_dep;                    //pixel bit depth

	int is_stride_mem;              //pCmpData format in memory 1: stride          0: frame
	int stride;                     //pCmpData stride(unit is 128bit)


}DCMP_LC_PARM_T;

void cmp_low_cost(CMP_LC_PARM_T *pCmpLCParm, HI_U16 *pFrameIn, HI_U8 *pCmpData, HI_U32 *pHead);

void dcmp_low_cost(DCMP_LC_PARM_T *pDcmpLCParm, HI_U8 *pCmpData, HI_U16 *pFrameOut, HI_U32 *pHead);

#ifdef __cplusplus
}
#endif

#endif