#include "hi_type.h"
//#include "vdp_hal_hdr.h"
#include "vdp_drv_coef.h"
#include "vdp_hal_hdr.h"
#include "vdp_drv_comm.h"
//lut for hdr
#include "KdmLutPQ2L.h"
#include "KdmTypeFxp.h"
#include "KdmLutL2GX.h"
#include "KdmLutL2GA.h"
#include "KdmLutL2GB.h"
#include "KdmLutL2PQX.h"
#include "KdmLutL2PQA.h"
#include "KdmLutL2PQB.h"
#include "KdmLutPQ2L.h"

extern const uint32_t depq1024Lut[1024];
#if 1//王正提供rgb->bt2020表
HI_U32 gdm_gmm_rgb2bt2020lut[64] =
{
	0	 ,
	369  ,
	737  ,
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
#if 1//王正提供hdr10->hdr10 自研通路表
HI_U32 gdm_gmm_hihdrlut[64] =
{
	0    ,
7698 ,
9827 ,
11247,
12334,
13224,
13980,
14641,
15229,
15759,
16243,
16688,
17100,
17485,
17846,
18186,
18507,
18811,
19894,
20812,
21612,
22320,
22957,
23537,
24068,
24560,
25017,
25444,
25845,
26224,
26582,
26921,
27245,
27554,
27849,
28132,
29158,
30051,
30842,
31551,
32196,
32786,
33330,
33835,
34307,
34749,
35165,
35558,
35931,
36285,
36622,
36945,
37253,
37549,
37833,
38106,
38369,
38623,
39558,
40385,
41127,
41800,
42416,
42767,

};
#endif
#if 1//王正提供hdr10->sdr 自研通路表
HI_U32 gdm_gmm_hislflut[64] =
{
	0	 ,
	8730 ,
	10472,
	11572,
	12392,
	13050,
	13604,
	14083,
	14507,
	14887,
	15232,
	15549,
	15842,
	16115,
	16370,
	16610,
	16837,
	17052,
	17814,
	18461,
	19024,
	19524,
	19974,
	20384,
	20760,
	21109,
	21435,
	21740,
	22026,
	22298,
	22555,
	22799,
	23033,
	23256,
	23470,
	23675,
	24424,
	25080,
	25666,
	26195,
	26678,
	27123,
	27536,
	27922,
	28284,
	28625,
	28948,
	29254,
	29545,
	29823,
	30089,
	30344,
	30589,
	30825,
	31052,
	31272,
	31484,
	31689,
	32450,
	33132,
	33750,
	34315,
	34837,
	35137

};
#endif

#if 1 //20160105//ok
HI_U32 gdm_gmm_lut[64] =
{
0 ,
4485 ,
5927 ,
6921 ,
7698 ,
8343 ,
8898 ,
9388 ,
9827 ,
10226,
10592,
10931,
11247,
11543,
11821,
12085,
12334,
12572,
13424,
14154,
14794,
15366,
15884,
16358,
16794,
17199,
17578,
17933,
18268,
18584,
18885,
19171,
19444,
19705,
19956,
20196,
21074,
21842,
22527,
23145,
23708,
24226,
24706,
25154,
25572,
25966,
26338,
26690,
27025,
27343,
27648,
27939,
28218,
28486,
28744,
28993,
29233,
29465,
30321,
31083,
31770,
32395,
32969,
33500
//33297
};
#endif

HI_U32 gdm_degmm_lut[64]={
/*
0 ,
1 ,
2 ,
3 ,
4 ,
5 ,
6 ,
7 ,
8 ,
9 ,
10,
11,
12,
13,
14,
15,
16,
17,
18,
19,
20,
21,
22,
23,
24,
25,
26,
27,
28,
29,
30,
31,
32,
33,
34,
35,
36,
37,
38,
39,
40,
41,
42,
43,
44,
45,
46,
47,
48,
49,
50,
51,
52,
53,
54,
55,
56,
57,
58,
59,
60,
61,
62,
63
*/
8602 ,
9591 ,
9648 ,
10439,
11118,
11709,
12232,
12708,
13139,
13535,
13904,
14254,
14574,
14881,
15171,
15443,
15706,
15954,
16193,
16423,
16641,
16858,
17057,
17256,
17448,
17629,
17810,
17984,
18149,
18314,
18476,
18627,
18778,
18928,
19072,
19210,
19348,
19485,
19614,
19740,
21495,
22867,
23993,
24950,
25785,
26528,
27198,
27807,
28364,
28878,
29357,
29805,
30227,
30624,
30997,
31356,
31693,
32016,
32326,
32620,
32906,
33177,
33441,
33694

};


extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;

HI_S32 VDP_DRV_SetDmGCvmCoef(HI_S16 * ptmLutI, HI_S16 * ptmLutS, HI_S16 * psmLutI, HI_S16 * psmLutS)
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
	HI_U8  * addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GCVM] ;//add by hyx
#else
	HI_U8  * addr ;//add by hyx

#endif

	//VDP_HDR_SetVCvmVcvmCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_CVM]);

	VDP_HDR_SetGCvmCoefAddr(gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GCVM]);

	
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
	VDP_HDR_SetGcvmCoefParaup();

    //VDP_FCLOSE(fp_dm_cvm_coef);

    return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetDmCvmCoef(HI_S16 * ptmLutI, HI_S16 * ptmLutS, HI_S16 * psmLutI, HI_S16 * psmLutS)
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

#if 1
HI_VOID VDP_DRV_SetDmDegammaCoef (DmKsFxp_t *pKs) 
{
//    cout << "vSetDmDegmmCoef start to work" << endl;
#if VDP_CBB_FPGA
   HI_U8  *input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_DEGAMMA] ;//add by hyx
   HI_U8  *output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_DEGAMMA];//add by hyx
#elif EDA_TEST
    HI_U8  *input_addr  = pVdpReg->VDM_COEFAD.bits.vdm_coef_addr ;//need modify when copy to it
    HI_U8  *ioutput_addr = pVdpReg->CVM1_DEGAMMA_COEFAD.bits.degamma_coef_addr ;//need modify when copy to it
#else
    HI_U32 input_addr = 0 ;
    HI_U32 output_addr = 0 ;
#endif

   // bOpenStiFile(STI_FILE_COEF_DM);
    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;

    void*  p_coef_array[1]    ;
    HI_U32 lut_length[1]      ;
    HI_U32 coef_bit_length[1] ;

    FILE*   fp_dm_input_degmm_coef=NULL;    
    FILE*   fp_dm_output_degmm_coef=NULL;    
    
   // if ((fp_dm_input_degmm_coef = fopen("golden/dm_input_degmm_coef.dat","wb")) == NULL)
   // {
    ///    printf("Error! file dm_input_degmm_coef.dat can't open!\n");
   // }
    
   // if ((fp_dm_output_degmm_coef = fopen("golden/dm_output_degmm_coef.dat","wb")) == NULL)
   // {
   //     printf("Error! file dm_output_degmm_coef.dat can't open!\n");
   // }

	VDP_HDR_SetVDmDegmmCoefAddr    (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_DEGAMMA]);
	VDP_HDR_SetVCvmDegmmCoefAddr   (gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_OUT_DEGAMMA]);

    //send coef cfg(input degamma)
    if(pKs->ksIMap.eotfParam.eotf == KEotfBt1886)
    {
        p_coef_array[0]    = (HI_S32*)pKs->ksIMap.g2L ; 
        lut_length[0]      = 256                     ; 
        coef_bit_length[0] = 32                      ; 
    }
    else
    {
        p_coef_array[0]    = (HI_S32*)depq1024Lut     ; 
        lut_length[0]      = 1024                     ; 
        coef_bit_length[0] = 32                       ; 
    }


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

    //output degamma coef send
    p_coef_array[0]    = (HI_S32*)depq1024Lut ; 
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

	VDP_HDR_SetDmInputDegmmParaup  ();
	VDP_HDR_SetDmOutputDegmmParaup ();

	
    //fclose(fp_dm_input_degmm_coef);
    //fclose(fp_dm_output_degmm_coef);
    //printf("[coef] Dm_degamma coef send OK!\n"); 
    //bCloseStiFile(STI_FILE_COEF_DM);
}
#endif


HI_VOID VDP_DRV_SetDmgammaCoef(DmKsFxp_t *pKs)
{
#if FPGA_TEST
    //HI_U8  *addr = 0 ;
    HI_U8 *input_addr = 0 ;
    HI_U8 *output_addr = 0 ;
#elif EDA_TEST
    HI_U8 *input_addr  = pVdpReg->VDM_COEFAD1.bits.vdm_coef_addr1 ;//need modify when copy to it
    HI_U8 *output_addr = pVdpReg->CVM1_GAMMA_COEFAD.bits.gamma_coef_addr ;//need modify when copy to it
#endif
    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;
    
    void* p_coef_array_l2g []     = {l2gLutX,l2gLutA,l2gLutB};
    void* p_coef_array_l2pq []    = {l2pqLutX,l2pqLutA,l2pqLutB};
    void ** p_coef_array;
    
    HI_U32 lut_length[]     = {128,128,128};
    HI_U32 coef_bit_length[]= {32,28,16};

    FILE* fp_dm_input_gmm_coef=NULL;
    FILE* fp_dm_output_gmm_coef=NULL;
	#if  0
    if ((fp_dm_input_gmm_coef = fopen("./golden/dm_input_gmm_coef.dat","wb")) == NULL)
    {
        printf("Error! file dm_input_gmm_coef.dat can't open!\n");
    }
    if ((fp_dm_output_gmm_coef = fopen("./golden/dm_output_gmm_coef.dat","wb")) == NULL)
    {
        printf("Error! file dm_output_gmm_coef.dat can't open!\n");
    }

    //coef gen
    stCoefGen.p_coef         = my_l2pqLutX        ; 
    stCoefGen.p_coef_new     = l2pqLutX           ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (HI_U32)(((HI_U64)1<<32)-1)          ; 
    stCoefGen.coef_min       = 0                  ; 

    VDP_FUNC_GenCoef(&stCoefGen) ; 

    stCoefGen.p_coef         = my_l2pqLutA        ; 
    stCoefGen.p_coef_new     = l2pqLutA           ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (1<<28)-1          ; 
    stCoefGen.coef_min       = 0                  ; 

    VDP_FUNC_GenCoef(&stCoefGen) ; 

    stCoefGen.p_coef         = my_l2pqLutB        ; 
    stCoefGen.p_coef_new     = l2pqLutB           ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (1<<16)-1          ; 
    stCoefGen.coef_min       = 0                  ; 

    VDP_FUNC_GenCoef(&stCoefGen) ; 

    stCoefGen.p_coef         = my_l2gLutX         ; 
    stCoefGen.p_coef_new     = l2gLutX            ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (HI_U32)(((HI_U64)1<<32)-1)          ; 
    stCoefGen.coef_min       = 0                  ; 

    VDP_FUNC_GenCoef(&stCoefGen) ; 

    stCoefGen.p_coef         = my_l2gLutA         ; 
    stCoefGen.p_coef_new     = l2gLutA            ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (1<<28)-1          ; 
    stCoefGen.coef_min       = 0                  ; 

    VDP_FUNC_GenCoef(&stCoefGen) ; 

    stCoefGen.p_coef         = my_l2gLutB     ; 
    stCoefGen.p_coef_new     = l2gLutB        ; 
    stCoefGen.ps32_coef_temp = NULL               ; 
    stCoefGen.coef_data_type = COEF_DATA_TYPE_U32 ; 
    stCoefGen.length         = 128                ; 
    stCoefGen.coef_data_mode = enCoefMode         ; 
    stCoefGen.coef_max       = (1<<16)-1          ; 
    stCoefGen.coef_min       = 0                  ;

    VDP_FUNC_GenCoef(&stCoefGen) ;
#endif
#if VDP_CBB_FPGA
	 input_addr  = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_IN_GAMMA] ;//add by hyx
	 output_addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_OUT_GAMMA] ;//add by hyx


	//addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GDM_GAMMA];//add by hyx
	//VDP_GP_SetGdmGammAddr  (0,gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GDM_GAMMA]);
	VDP_GP_SetGdmGammAddr  (0,gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_IN_GAMMA]);
	 
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

    
    //send coef output gamma
    if(pKs->ksOMap.oetfParam.oetf == KEotfBt1886)
    {
        p_coef_array = p_coef_array_l2g;
    }
    else
    {
        p_coef_array = p_coef_array_l2pq;
    }
        
    stCoefSend.coef_addr         = output_addr              ; 
    stCoefSend.fp_coef           = fp_dm_output_gmm_coef    ; 
    stCoefSend.p_coef_array      = p_coef_array             ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 

    VDP_FUNC_SendCoef(&stCoefSend);

	VDP_HDR_SetDmInputGammaParaup  ();
	VDP_HDR_SetDmOutputGammaParaup ();
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_GMM);

    //fclose(fp_dm_output_gmm_coef);
   // fclose(fp_dm_input_gmm_coef);
}



HI_VOID VDP_DRV_SetGdmgammaCoef(HI_U32 *u32GdmGammaCoef) 
{
    HI_U8  *addr = 0 ;

    //VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;
    void*  p_coef_array[1]    = {u32GdmGammaCoef};
	
    HI_U32 lut_length[1]      = {GDM_GMM_LUT_SIZE};
    HI_U32 coef_bit_length[1] = {16};

    FILE*   fp_gdm_gamma_coef=NULL;    
    
#if FPGA_TEST
#else
    //HI_U32 addr = 0 ;//need modify when copy to it
    U_GDM_GAMM_ADDR st_gdmgamm_paraaddr;

    st_gdmgamm_paraaddr.u32 = *(&(pVdpReg->GDM_GAMM_ADDR.u32)) ; 

    addr = st_gdmgamm_paraaddr.bits.gamm_coeff_addr ;//need modify when copy to it
#endif
    //if ((fp_gdm_gamma_coef = fopen("golden/gdm_gamma_coef.dat","wb")) == NULL)
    //{
    //    printf("Error! file gdm_gamma_coef.dat can't open!\n");
   // }

#if VDP_CBB_FPGA
		addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GDM_GAMMA];//add by hyx
		VDP_GP_SetGdmGammAddr  (0,gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GDM_GAMMA]);
#endif

    //send coef cfg
    stCoefSend.coef_addr         = addr                       ; 
    stCoefSend.sti_type          = STI_FILE_COEF_GDM           ; 
    stCoefSend.fp_coef           = fp_gdm_gamma_coef         ; 
    stCoefSend.lut_num           = 1                          ; 
    stCoefSend.burst_num         = 1                          ; 
    stCoefSend.cycle_num         = 8                          ; 
    stCoefSend.p_coef_array      = p_coef_array               ; 
    stCoefSend.lut_length        = lut_length                 ; 
    stCoefSend.coef_bit_length   = coef_bit_length            ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 

    VDP_FUNC_SendCoef(&stCoefSend);  
	
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_GMM);
    
}




HI_VOID VDP_DRV_SetGdmDegammaCoef(HI_U32 *u32GdmDeGammaCoef) 
{
    HI_U8  *addr ;
//    VDP_COEF_GEN_CFG    stCoefGen;
    VDP_COEF_SEND_CFG   stCoefSend;
    void*  p_coef_array[1]    = {u32GdmDeGammaCoef};
    HI_U32 lut_length[1]      = {GDM_GMM_LUT_SIZE};
    HI_U32 coef_bit_length[1] = {16};

    FILE*   fp_gdm_degamma_coef=NULL;    

#if FPGA_TEST
    addr = 0 ;
#else
    U_GDM_DEGAMM_ADDR st_gdmdegamm_paraaddr;
    st_gdmdegamm_paraaddr.u32 = *(&(pVdpReg->GDM_DEGAMM_ADDR.u32)) ; 

    addr = st_gdmdegamm_paraaddr.bits.degamm_coeff_addr ;//need modify when copy to it

    //cout << hex << "[gdm] addr = "<< addr  << endl ;
#endif
    
    //if ((fp_gdm_degamma_coef = fopen("golden/gdm_degamma_coef.dat","wb")) == NULL)
    //{
    //    printf("Error! file gdm_degamma_coef.dat can't open!\n");
    //}
#if VDP_CBB_FPGA
		addr = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_GDM_DEGAMMA];//add by hyx
		VDP_GP_SetGdmDegammAddr(0,gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_GDM_DEGAMMA]);
#endif

    //send coef cfg
    stCoefSend.coef_addr         = addr                       ; 
    stCoefSend.sti_type          = STI_FILE_COEF_GDM          ; 
    stCoefSend.fp_coef           = fp_gdm_degamma_coef        ; 
    stCoefSend.lut_num           = 1                          ; 
    stCoefSend.burst_num         = 1                          ; 
    stCoefSend.cycle_num         = 8                          ; 
    stCoefSend.p_coef_array      = p_coef_array               ; 
    stCoefSend.lut_length        = lut_length                 ; 
    stCoefSend.coef_bit_length   = coef_bit_length            ; 
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ; 

    VDP_FUNC_SendCoef(&stCoefSend);    
    
	VDP_HDR_SetGdmParaUpd(VDP_LAYER_GP0,VDP_GP_PARA_HDR_DEGMM);
    //fclose(fp_gdm_degamma_coef);
   // printf("[coef] Gdm_degamma coef send OK!\n"); 
}


