#include "vpss_alg_scd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


HI_S32 SCDetection_core(SCDInput* pSCDInput,  SCDRls* pSCDRls, int flagTB)
{
    int i;
    int k_pixy, k1_histdiff, k2_histdiff, corebin_histdiff, kbin_histdiff, core_histdiff, core_sumdiff, k1_sumdiff, k2_sumdiff, x1_sumdiff, g1_sumdiff, gainH_TF, gainL_TF, core_scwdiff, k_scwdiff_TF, divWidth, divHeight;
    HI_U64 normw, normh;
    HI_S32 k_histdiff;
    HI_S32 difhist, sumhist;
    HI_S32 scw_hist, scw_CF, scw_diff;
    HI_S32 gain_TF;

    HI_S32 K_HISTLPF; //register, in[0,10]
    HI_S32 nI0, nI1, nI2;
    HI_S32 nHist_LPF;

    HI_S32 BINNM, BINBIT;
    HI_S32 BINSTEP, BINBIT0, BINBIT1;

    HI_S32 s32SCHist_CFbuf[32];

    BINNM = 32;
    BINBIT = 5;
    BINSTEP = 10 - 5;
    BINBIT0 = 3;
    BINBIT1 = 2;

    k_pixy = pSCDInput->scdPara.SCDK_pixy;
    k1_histdiff = pSCDInput->scdPara.SCDK1_histdiff;
    k2_histdiff = pSCDInput->scdPara.SCDK2_histdiff;
    corebin_histdiff = pSCDInput->scdPara.SCDCorebin_histdiff;
    kbin_histdiff =  pSCDInput->scdPara.SCDKbin_histdiff;

    core_histdiff = pSCDInput->scdPara.SCDCore_histdiff;
    core_sumdiff = pSCDInput->scdPara.SCDCore_sumdiff;

    k1_sumdiff = pSCDInput->scdPara.SCDK1_sumdiff;
    x1_sumdiff = pSCDInput->scdPara.SCDX1_sumdiff;
    g1_sumdiff = pSCDInput->scdPara.SCDG1_sumdiff;
    k2_sumdiff = pSCDInput->scdPara.SCDK2_sumdiff;

    gainH_TF = pSCDInput->scdPara.SCDGainH_TF;
    gainL_TF = pSCDInput->scdPara.SCDGainL_TF;
    core_scwdiff = pSCDInput->scdPara.SCDCore_scwdiff;
    k_scwdiff_TF = pSCDInput->scdPara.SCDK_scwdiff_TF;

    divWidth = pSCDInput->scdPara.SCDDivWidth;
    divHeight = pSCDInput->scdPara.SCDDivHeight;

    ///////////////Hareware/////////////////////////

    for (i = 0; i < 32; i++)
    {
        s32SCHist_CFbuf[i] = 0;
    }

    ///////////////////////SoftWare///////////////////////////////////
    ////////// GET s32SCHist_CF from HAREWARE
    ////////// GET divWidth divHeight BINSTEP K_HISTLPF BINNUM k2_histdiff k1_histdiff kbin_histdiff corebin_histdiff k1_sumdiff k1_sumdiff k2_sumdiff k1_sumdiff core_scwdiff
    ////////// 	   gainH_TF gainL_TF k_scwdiff_TF AND SO ON from PARAMETER or SETTING
    ////////// SAVE SCW_P1 FOR NEXT FRAME



    for (i = 0; i < BINNM; i++)
    {
        normw = pSCDRls->s32SCHist_CF[i];
        normw = ((normw * divWidth) >> 15);
        normh = ((normw * divHeight) >> (15 + BINSTEP));

        pSCDRls->s32SCHist_CF[i] = normh;
        s32SCHist_CFbuf[i] = normh;
    }


    // LPF
    K_HISTLPF = 4;
    for (i = 0; i < 32; i++)
    {
        nI0 = MAX2(0, i - 1);
        nI1 = i;
        nI2 = MIN2(31, i + 1);
        nHist_LPF = ((K_HISTLPF * (s32SCHist_CFbuf[nI0] + s32SCHist_CFbuf[nI2]) + (32 - 2 * K_HISTLPF) * s32SCHist_CFbuf[nI1]) >> 5);

        pSCDRls->s32SCHist_CF[i] = nHist_LPF;
    }


    sumhist = 0;
    if (flagTB == 0) // top
    {
        for (i = 0; i < BINNM; i++)
        {
            k_histdiff = MAX2( k2_histdiff, k1_histdiff - ((kbin_histdiff * MAX2(0, i - corebin_histdiff)) >> 4) );

            difhist = ABS(pSCDRls->s32SCHist_CF[i] - pSCDRls->s32SCHist_P1T[i]);
            difhist = MAX2(0, difhist - core_histdiff);

            difhist = ((k_histdiff * difhist) >> 6);

            //difhist = CLIP3(0, 64, difhist);

            sumhist = sumhist + difhist;

            pSCDRls->s32SCHist_P1T[i] = pSCDRls->s32SCHist_CF[i];
        }
    }
    else // bot
    {
        for (i = 0; i < BINNM; i++)
        {
            k_histdiff = MAX2( k2_histdiff, k1_histdiff - ((kbin_histdiff * MAX2(0, i - corebin_histdiff)) >> 4) );

            difhist = ABS(pSCDRls->s32SCHist_CF[i] - pSCDRls->s32SCHist_P1B[i]);
            difhist = MAX2(0, difhist - core_histdiff);
            difhist = ((k_histdiff * difhist) >> 6);

            //difhist = CLIP3(0, 64, difhist);

            sumhist = sumhist + difhist;

            pSCDRls->s32SCHist_P1B[i] = pSCDRls->s32SCHist_CF[i];
        }

    }

    //
    sumhist = MAX2(0, sumhist - core_sumdiff);

    if (sumhist < x1_sumdiff)
    { scw_hist = MIN2(g1_sumdiff, ((k1_sumdiff * sumhist) >> 6) ); }
    else
    { scw_hist = MIN2(255, ((k2_sumdiff * (sumhist - x1_sumdiff)) >> 6) ); }

    // temporal filtering
    scw_diff = MAX2(0, ABS(scw_hist - pSCDRls->SCW_P1) - core_scwdiff);
    gain_TF = MAX2(gainL_TF, gainH_TF - ((k_scwdiff_TF * scw_diff) >> 6) );

    scw_CF = ((gain_TF * (pSCDRls->SCW_P1 - scw_hist)) >> 8) + scw_hist; // [0,255]

    // store
    pSCDRls->SCW_P1 = scw_CF;

    return scw_CF;
}


HI_S32	SCDDetection(SCDInput* pSCDInput, SCDRls* pSCDRls)
{
    pSCDInput->scdPara.SCDK_pixy = 16;
    pSCDInput->scdPara.SCDK1_histdiff = 256;
    pSCDInput->scdPara.SCDK2_histdiff = 192;
    pSCDInput->scdPara.SCDCorebin_histdiff = 2;
    pSCDInput->scdPara.SCDKbin_histdiff = 32;

    pSCDInput->scdPara.SCDCore_histdiff = 20;
    pSCDInput->scdPara.SCDCore_sumdiff = 32;

    pSCDInput->scdPara.SCDK1_sumdiff = 32;
    pSCDInput->scdPara.SCDX1_sumdiff = 256;
    pSCDInput->scdPara.SCDG1_sumdiff = 128;
    pSCDInput->scdPara.SCDK2_sumdiff = 128;

    pSCDInput->scdPara.SCDGainH_TF = 0;
    pSCDInput->scdPara.SCDGainL_TF = 0;
    pSCDInput->scdPara.SCDCore_scwdiff = 24;
    pSCDInput->scdPara.SCDK_scwdiff_TF = 72;


    pSCDInput->scdPara.SCDDivWidth = (1 << 20) / pSCDInput->Width;
    pSCDInput->scdPara.SCDDivHeight = (1 << 20) / pSCDInput->Height;


    if ((0 == pSCDInput->Field) || (1 == pSCDInput->Field))
    {

        return SCDetection_core(pSCDInput, pSCDRls, pSCDInput->Field);

    }
    else
    {
        return SCDetection_core(pSCDInput, pSCDRls, 0);
    }

}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


