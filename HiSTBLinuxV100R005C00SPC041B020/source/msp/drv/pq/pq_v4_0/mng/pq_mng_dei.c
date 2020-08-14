/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dei.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/06/16
  Description   :

******************************************************************************/

#include "hi_math.h"
#include "hi_drv_mem.h"
#include "pq_mng_dei.h"
#include "pq_mng_dei_alg.h"
#include "drv_pq_table.h"


extern HI_BOOL       g_bMcDiEn;
extern DeiSwAlgCfg_S g_pstDeiSwAlgPara;

/* DEI Init Flag */
static HI_BOOL        sg_bDeiInitFlag  = HI_FALSE;
static HI_BOOL        sg_bDeiDemoEn    = HI_FALSE;
static PQ_DEMO_MODE_E sg_enMode        = PQ_DEMO_ENABLE_R;
static MOTION_CALC_S  sg_MotionCalc;
static HI_S32 sg_s32FRCountMC  = 0;

static HI_Mc_DeiParam_S  *sg_pstMcParm = HI_NULL;
static HI_DEI_Variable_S *sg_pstDeiVar = HI_NULL;


static HI_VOID PQ_MNG_InitSoftWareAlgParm(HI_Mc_DeiParam_S *mParm, HI_DEI_Variable_S *deiVariable)
{
    /* for debug */
    mParm->DG_NUM   = 419;     //no register
    mParm->DG_ROW   = 381 / 2; //no register
    mParm->DG_ROW_O = 381;     //no register
    mParm->DG_BLKV  = mParm->DG_ROW / 4; //no register
    mParm->DG_RGV   = mParm->DG_ROW / 8;  //no register

    mParm->DUMP_EN  = 0; //no register

    /* Debug: Close MC_ONLY */
    mParm->MC_ONLY     = 0;
    mParm->MA_ONLY     = 0;
    mParm->MAVERT_ONLY = 0;   //no register

    mParm->PRJNUM        = 2; // fixed   //no register
    mParm->BIT_DIV       = 20;// fixed   //no register
    mParm->BIT_NORM_O    = 10;// fixed   //no register
    mParm->BIT_SUBMV     = 2; // fixed   //no register
    mParm->BIT_SADADD    = 3; // fixed   //no register
    mParm->BIT_BLKMAGADD = 2; // fixed   //no register

    //[7,3], [8,4], [9,5], [10,6], [11,7], [13,9]
    mParm->CANDIMV_NUM   = 8; // fixed   //no register
    mParm->CANDIRGMV_NUM = 4; // fixed   //no register

    mParm->K_MCMVY = 1; // fixed   //no register

    /*top/bot first in*/
    mParm->FRAME_IN  = 1; // [0,1]   //no register
    mParm->FR_TBCONV = 0; // [0,1] for frame in, 0-top/bot-->top/bot,1-top/bot-->bot/top  //no register
    mParm->ISTOPFST  = 1; // [0,1] 1-top first,0-bot first   //no register

    /*Color Space Conversion*/
    mParm->RGB2YUV_EN = 1; // not used   //no register
    mParm->YUV2RGB_EN = 1; // not used   //no register
    mParm->CSC_MODE   = 1; // not used   //no register
    mParm->AUTOSET_EN = 1; // not used   //no register

    /*YUV444/422/420 */
    mParm->BEINYUV444  = 1; // not used   //no register
    mParm->BEOUTYUV444 = 1; // not used   //no register
    mParm->YUV_FORMAT  = 0; //3;  // [0,2] 0-444,1-422,2-420   //no register

    mParm->FORCE_YUV444 = 0;// not used   //no register
    mParm->CS_UV = 0;       // not used   //no register
    mParm->RS_UV = 0;       // not used   //no register

    /*ME Downscaling*/
    mParm->MEDS_EN = 0;  // [0,1]

    //[1,4,6,4,1]; [0,3,10,3,0]
    mParm->COEF0_DS = 1; // [0,7]   //no register
    mParm->COEF1_DS = 4; // [0,15]  //no register
    mParm->COEF2_DS = 6; // [0,15]  //no register
    mParm->COEF3_DS = 4; // [0,15]  //no register
    mParm->COEF4_DS = 1; // [0,7]   //no register


    /* Motion detection of MADI*/


    /*************************************************************/
    /** ME of MCDI **/
    mParm->BIT_BILIN = 3;  //fixed  //no register
    mParm->MODE_MEP1 = 0;  //fixed  //no register

    /*region mv*/
    mParm->RGNUMH    = 32; //no register
    mParm->RGNUMV    = 32; //no register

    mParm->RGSIZEH = 64;   //fixed   //no register
    mParm->RGSIZEV = 8;    //fixed   //no register
    mParm->RGSTATH = 128;  //fixed   //no register
    mParm->RGSTATV = 16;   //fixed   //no register
    mParm->DIV_RGHEIGHT = 8192;  //fixed   //no register
    mParm->DIV_RGWIDTH  = 32768; //fixed   //no register

    mParm->RGOFFSETH = 32; // [0,63]   //no register
    mParm->RGOFFSETV = 8;  // [0,15]   //no register

    mParm->RGBORDER_EN = 1;   // fixed   //no register
    mParm->RGBORDERH0  = 32;  // fixed   //no register
    mParm->RGBORDERV0  = 8;   // fixed   //no register
    mParm->RGBORDERH1  = 96;  // fixed   //no register
    mParm->RGBORDERV1  = 24;  // fixed   //no register

    mParm->RGLSTOFFSETH = 0; // [0,127]  //no register
    mParm->RGLSTOFFSETV = 0; // [0,31]   //no register

    mParm->RGMVWNDH = 3;  //fixed   //no register
    mParm->RGMVWNDV = 1;  //fixed   //no register

    mParm->MVRANGEH = 32; //fixed   //no register
    mParm->MVRANGEV = 8;  //fixed   //no register

    mParm->CORE_MAG_RG = 3;   // [0,15]
    mParm->LMT_MAG_RG  = 255; // [0,1023]

    mParm->K_RGDIFYCORE = 3;  //2; // [0,15]
    mParm->G_RGDIFYCORE = 1023;    // [0,1023]

    mParm->COEF_SADLPF = 1;   // [0,5]

    mParm->CORE_RGDIFY = 7;   // [0,15]
    mParm->LMT_RGDIFY  = 511; // [0,1023]

    mParm->KMV_RGSAD      = 32;         // [0,127]
    mParm->K_TPDIF_RGSAD  = 63;  //16;  // [0,63]
    mParm->G_TPDIF_RGSAD  = 255;        // [0,511]
    mParm->THMAG_RGMV     = 48;  //96;  // [0,255]
    mParm->TH_SADDIF_RGMV = 128; //256; // [0,511]
    mParm->TH_0MVSAD_RGMV = 256; //512; // [0,1023]
    mParm->K_SADCORE_RGMV = 8;          // [0,15]

    mParm->CORE_MV_RGMVLS  = 2;
    mParm->K_MV_RGMVLS     = 16;      // [0,31]
    mParm->CORE_MAG_RGMVLS = -64;     // [-64,63]
    mParm->K_MAG_RGMVLS    = 12; //8; // [0,15]

    mParm->TH_SAD_RGLS = 8;  // [0,31]
    mParm->TH_MAG_RGLS = 40; // [0,63]
    mParm->K_SAD_RGLS  = 8;  // [0,15]

    mParm->TH_MVADJ_RGMVLS = 8; // [0,15]
    mParm->EN_MVADJ_RGMVLS = 1; // [0,1]

    /* blk mv */
    mParm->BLKMVRANGEH = 32; //fixed   //no register
    mParm->BLKMVRANGEV = 0;  //fixed   //no register

    mParm->HBLKSIZE = 8; //fixed   //no register
    mParm->VBLKSIZE = 4; //fixed   //no register

    /* candidat mv */
    mParm->ADDSAD_NEIBRGMV = 128; //[0,255] //not used  //no register


    /* mv selection */
    mParm->MODE_SELBLKMV = 3;  //1; //fixed  //no register
    mParm->FORCE_MVEN    = 0;  // [0,1]
    mParm->FORCE_MVX     = 0;  // [-127,127]
    //mParm->FORCE_MVY   = 12; // [-31,31]

    /* rgmv-based blkmv dilation */
    mParm->TH_HBLKDIST_MVDLT = 4;       //[0,15]
    mParm->TH_VBLKDIST_MVDLT = 1;       //[0,15]
    mParm->TH_LS_MVDLT       = 8;       //[0,15]
    mParm->TH_RGMVX_MVDLT    = 4;       //[0,15]
    mParm->TH_BLKMVX_MVDLT   = 6; //2;  //[0,7]
    mParm->THH_SAD_MVDLT     = 16;//32; //[0,31]
    mParm->THL_SAD_MVDLT     = 96;//32; //[0,127]

    mParm->G_MAG_MVDLT       = 16;          //[0,63]
    mParm->TH_MAG_MVDLT      = 12;//8; //4; //[0,31]
    mParm->K_SADCORE_MVDLT   = 2;           //[0,7]


    /** MC of MCDI ***/
    mParm->K_CORE_SIMIMV  = 8;        //[0,31]
    mParm->GL_CORE_SIMIMV = 0;        //[0, 7]
    mParm->GH_CORE_SIMIMV = 15;       //[0,63]
    mParm->K_SIMIMVW      = 32; //16; //[0,63]
    mParm->K_RGLSW        = 20;       //[0,31]

    /*RGMV mc weight*/
    mParm->CORE_MVY_MCW = 3;      //[0,7]
    mParm->K_MVY_MCW    = 16;//4; //[0,63]

    mParm->CORE_RGSADADJ_MCW = 64;//128; //[0,511]
    mParm->K_RGSADADJ_MCW    = 8;        //[0,15]

    mParm->K_CORE_VSADDIF  = 16;            //[0,31]
    mParm->GL_CORE_VSADDIF = 8;             //[0,31]
    mParm->GH_CORE_VSADDIF = 64;            //[0,127]
    mParm->K_VSADDIFW      = 8;  //2; //16  //[0,63]

    mParm->RGTB_EN_MCW     = 1;
    mParm->MODE_RGYSAD_MCW = 0;         //[0,1]

    mParm->CORE_RGMAG_MCW = 24;  //64;    //[0,255]
    mParm->X0_RGMAG_MCW   = 64;  //256;   //[0,511]
    mParm->K0_RGMAG_MCW   = 256;        //[0,511]
    mParm->G0_RGMAG_MCW   = 64;  //255;   //[0,255]
    mParm->K1_RGMAG_MCW   = 320; //256;   //[0,511]

    mParm->CORE_RGSAD_MCW = 96;  //128; //[0,255]
    mParm->X0_RGSAD_MCW   = 512;        //[0,1023]
    mParm->K0_RGSAD_MCW   = 160; //128; //[0,511]
    mParm->G0_RGSAD_MCW   = 255;        //[0,255]
    mParm->K1_RGSAD_MCW   = 128;        //[0,511]

    mParm->X0_SMRG_MCW = 16;  // [0,255]
    mParm->K0_SMRG_MCW = 64;  // [0,255]

    mParm->X_RGSAD_MCW = 112; // [0,255]
    mParm->K_RGSAD_MCW = 24;  // [0,63]

    /*temporal mv mc weight*/
    mParm->X0_TPMVDIST_MCW = 255;        //[0,255]
    mParm->K0_TPMVDIST_MCW = 64;  //256; //[0,511]-->[0,127]
    mParm->G0_TPMVDIST_MCW = 0;          //[0,255]
    mParm->K1_TPMVDIST_MCW = 32;  //128; //[0,511]-->[0,127]

    mParm->K_MINMVDIST_MCW = 4;  //[0,63] -->[0,15]
    mParm->K_AVGMVDIST_MCW = 4;  //[0,63] -->[0,15]

    mParm->B_CORE_TPMVDIST_MCW = 2; //[0,7]
    mParm->K_CORE_TPMVDIST_MCW = 2; //[0,15]-->[0,7]

    /*wnd mc weight*/
    mParm->K_DIFHCORE_MCW = 18; //16; //[0,31]
    mParm->K_DIFVCORE_MCW = 32;      //[0,63]

    mParm->K_MAX_CORE_MCW = 8;//4;   // [0,15]
    mParm->K_MAX_DIF_MCW  = 8;//10;  // [0,15]
    mParm->K1_MAX_MAG_MCW = 8;//4;   // [0,15]
    mParm->K0_MAX_MAG_MCW = 8;//12;  // [0,15]

    mParm->K_TBDIF_MCW = 15; // [0,15]
    mParm->K_TBMAG_MCW = 0;  // [0,15]

    mParm->X0_MAG_WND_MCW = 32;           //[0,127]
    mParm->K0_MAG_WND_MCW = 6;  //24; //18; //[0,31]-->[0,15]
    mParm->G0_MAG_WND_MCW = 24; //18;       //[0,63]
    mParm->K1_MAG_WND_MCW = 6;  //24;       //[0,63]-->[0,15]
    mParm->G1_MAG_WND_MCW = 288;//511;      //[0,511]

    mParm->X0_SAD_WND_MCW = 8;  //[0,127]
    mParm->K0_SAD_WND_MCW = 16; //64; //96; // [0,255] -->[0,31]
    mParm->G0_SAD_WND_MCW = 16; //24;       // [0,255]
    mParm->K1_SAD_WND_MCW = 16; //64;       // [0,255] --> [0,31]
    mParm->G1_SAD_WND_MCW = 288;//511;      // [0,511]

    /*vert weight calculation*/
    mParm->K_MAX_HVDIF_DW = 12;//4;  // [0,15]
    mParm->K_DIFV_DW      = 20;//24; // [0,31]

    mParm->GAIN_LPF_DW    = 15;  // [0,15]

    mParm->CORE_BHVDIF_DW = 5;   // [0,15]
    mParm->K_BHVDIF_DW    = 64;  // [0,127]
    mParm->B_BHVDIF_DW    = 0;   // [0,7]
    mParm->B_HVDIF_DW     = 0;   // [0,7]
    mParm->CORE_HVDIF_DW  = 16;  // [0,31]

    mParm->X0_HVDIF_DW = 256;          //[0,511]
    mParm->K0_HVDIF_DW = 8;   //32;      // [0,63] -->[0,15]
    mParm->G0_HVDIF_DW = 128; //[0,511]
    mParm->K1_HVDIF_DW = 8;   //32;     // [0,63] -->[0,15]

    mParm->CORE_MV_DW = -2; //5;         //[-7,7]
    mParm->B_MV_DW  = 56;   //48; //32;  //[0,63]
    mParm->X0_MV_DW = 8;              //[0,31]
    mParm->K0_MV_DW = 16;   //32;       //[0,63]
    mParm->G0_MV_DW = 32;   //16; //8;  //[0,63]
    mParm->K1_MV_DW = 64;            //[0,255]

    mParm->B_MT_DW  =  0; //[0,255]
    mParm->X0_MT_DW = 32; //[0,127]
    mParm->K0_MT_DW = 32; //12; //[0,63]
    mParm->G0_MT_DW = 64; //24; //[0,255]
    mParm->K1_MT_DW = 32; //[0,63]

    mParm->G0_MV_MT = 31; // [0,31]
    mParm->X0_MV_MT = 1;  // [0,3]
    mParm->K1_MV_MT = 20; // [0,31]


    /* mc*/
    mParm->GAIN_UP_MCLPFV = 16;  // [0,16]
    mParm->GAIN_DN_MCLPFV = 16;  // [0,16]
    mParm->GAIN_MCLPFH    = 16;  // [0,16]

    mParm->MCLPF_MODE = 1; //0;   // [0,1]

    mParm->RS_PXLMAG_MCW = 0; //[ 0,1]
    mParm->G_PXLMAG_MCW  = 0; //63; // [0,63] // fixed
    mParm->X_PXLMAG_MCW  = 2; //8;  // [0,7]
    mParm->K_PXLMAG_MCW  = 8; //32; // [0,127]-->[0,31]

    mParm->K_Y_MCW = 16;//64;   // [0,127] -->[0,31]
    //mParm->K_Y_MCBLD = 64;  // [0,127]
    mParm->K_C_MCW = 8;//24;//64;//32;  // [0,127] -->[0,31]
    //mParm->K_C_MCBLD = 64;//32;   // [0,127]

    mParm->X0_MCW_ADJ = 64;   // [0,255]
    mParm->K0_MCW_ADJ = 64;   // [0,255]
    mParm->G0_MCW_ADJ = 1023; // [0,4095]
    mParm->K1_MCW_ADJ = 128;  // [0,255]

    mParm->K_Y_VERTW = 8;//32;   // [0,127] -->[0,31]
    mParm->K_C_VERTW = 15;//8;   // [0,15]  //modify by z214841 2015.7.8
    //mParm->K_CY_VERTW = 15; //8; //[0,15]

    mParm->K_DELTA = 8;   //32; // [0,127] -->[0,31]

    mParm->BDH_MCPOS = 4;     // [0,7]
    mParm->BDV_MCPOS = 4; //1; // [0,7]

    mParm->X0_MV_MC = 2; //1;  // [0,7]
    mParm->K1_MV_MC = 16;//20; // [0,63]


    mParm->C0_MC = 0;    // [0,4095]
    mParm->R0_MC = 0;    // [0,4095]
    mParm->C1_MC = 4095; // [0,4095]
    mParm->R1_MC = 4095; // [0,4095]

    mParm->MCMVRange = 32;//[0,32];

    /*mc weight temporal change*/
    mParm->SCENECHANGE_MC = 0;      //[0,1]
    mParm->X_FRCOUNT_MC   = 8;      //[0,31]
    mParm->K_FRCOUNT_MC   = 48;//16; //[0,127]

    /*Global motion detection*/
    mParm->MTTH0_GMD = 4;  // [0,15]
    mParm->MTTH1_GMD = 12; // [0,31]
    mParm->MTTH2_GMD = 20; // [0,63]
    mParm->MTTH3_GMD = 28; // [0,63]

    mParm->MTFILTEN_GMD = 1; // [0,1]
    mParm->BITSHIFT_GMD = 4; // [0,7]   //no register

    mParm->MTSUM0_GMD = 0; // [0,65535]   //no register
    mParm->MTSUM1_GMD = 0; // [0,65535]   //no register
    mParm->MTSUM2_GMD = 0; // [0,65535]   //no register
    mParm->MTSUM3_GMD = 0; // [0,65535]   //no register

    mParm->COL0_GMD = 0;    //[0,2047]   //no register
    mParm->ROW0_GMD = 0;    //[0,2047]   //no register
    mParm->COL1_GMD = 2047; //[0,2047]   //no register
    mParm->ROW1_GMD = 2047; //[0,2047]   //no register

    mParm->DIVCOL_GMD = 1;  //2^20/width   //no register
    mParm->DIVROW_GMD = 1;  //2^20/height  //no register

    mParm->K_MAXMAG_GMD = 4;
    mParm->K_DIFH_GMD   = 22;
    mParm->K_MAG_GMD    = 96;

    /*progressive GMD*/
    mParm->MTSUM0_GMD_P = 0;
    mParm->MTSUM1_GMD_P = 0;
    mParm->MTSUM2_GMD_P = 0;
    mParm->MTSUM3_GMD_P = 0;

    /* gmd sw alg*/
    mParm->TH_GMDSUM_MD = 1024;
    mParm->K_GMDSUM_MD = 0;//32; //16;
    mParm->G_GMDSUM_MD = 192;


    /****************************************************/
    //mParm->MA_ONLY = 1;

    mParm->MC_LAI_BLDMODE = 1;
    /* Debug: Open Mc blend */
    mParm->MC_NUMT_BLDEN  = 1;
    mParm->McStartR      = 0; //8bit; [0,255]
    mParm->McStartC      = 0; //8bit; [0,255]
    mParm->McEndR        = 0; //8bit; [0,255]
    mParm->McEndC        = 0; //8bit; [0,255]

    /*MA param */
    mParm->bfld_motion_wnd_mode = 1;
    /* Debug: */
    mParm->blong_motion_shf = 0;//1;

    /*other alg param */
    deiVariable->stINTERPINFO.INTER_DIFF_THD0  = 32;
    deiVariable->stINTERPINFO.INTER_DIFF_THD1  = 64;
    deiVariable->stINTERPINFO.INTER_DIFF_THD2  = 255;
    deiVariable->stINTERPINFO.MC_STRENGTH_K0   = 0;
    deiVariable->stINTERPINFO.MC_STRENGTH_K1   = 6;
    deiVariable->stINTERPINFO.MC_STRENGTH_K2   = 0;
    deiVariable->stINTERPINFO.MC_STRENGTH_K3   = 0;
    deiVariable->stINTERPINFO.MC_STRENGTH_G0   = 16;
    deiVariable->stINTERPINFO.MC_STRENGTH_G1   = 16;
    deiVariable->stINTERPINFO.MC_STRENGTH_G2   = 64;
    deiVariable->stINTERPINFO.MC_STRENGTH_G3   = 64;
    deiVariable->stINTERPINFO.MC_STRENGTH_MAXG = 16;
    deiVariable->stINTERPINFO.MC_STRENGTH_MING = 16;

    deiVariable->stMOTIONHIST.MOTION_THD0 = 16; /* 4 means "< 0", 32 means "< 8" for 8-bit depth   no register */
    deiVariable->stMOTIONHIST.MOTION_THD1 = 48; /* 4 means "< 0", 32 means "< 8" for 8-bit depth   no register */
    deiVariable->stMOTIONHIST.MOTION_THD2 = 80; /* 4 means "< 0", 32 means "< 8" for 8-bit depth   no register */
    deiVariable->stMOTIONHIST.MOTION_THD3 = 112;/* 4 means "< 0", 32 means "< 8" for 8-bit depth   no register */
    /*
        //software alg para
        pstGMDFORMC->TH_GMDSUM_MC = 1024;            //[0,4095]
        pstGMDFORMC->K_GMDSUM_MC  = 32;   //0; //16; //[0,255]
        pstGMDFORMC->G_GMDSUM_MC  = 255;  //192;     //[0,255]
        pstGMDFORMC->MTSUM0_GMD   = 0;
        pstGMDFORMC->MTSUM1_GMD   = 0;
        pstGMDFORMC->MTSUM2_GMD   = 0;
        pstGMDFORMC->MTSUM3_GMD   = 0;
    */
    /*edge strength adjustment for MC*/
    deiVariable->stINTERPINFO.MC_EDGE_CORING = 16; //256;
    deiVariable->stINTERPINFO.MC_EDGE_SCALE  = 32; //64;
    deiVariable->stMDINFO.ADJUST_GAIN        = 0;


    deiVariable->stDeiSwAlgCfg.TH_L_GMDDIF_MC   = 20; //no register
    deiVariable->stDeiSwAlgCfg.TH_H_GMDDIF_MC   = 50; //no register
    deiVariable->stDeiSwAlgCfg.TH_MIN_GMDDIF_MC = 30; //no register

    deiVariable->stDeiSwAlgCfg.TH_L_GMDDIF_MC_P = 10; //no register
    deiVariable->stDeiSwAlgCfg.TH_H_GMDDIF_MC_P = 128;//no register

    g_pstDeiSwAlgPara.TH_L_GMDDIF_MC   = 20;   //no register
    g_pstDeiSwAlgPara.TH_H_GMDDIF_MC   = 50;   //no register
    g_pstDeiSwAlgPara.TH_MIN_GMDDIF_MC = 30;   //no register
    g_pstDeiSwAlgPara.TH_L_GMDDIF_MC_P = 10;   //no register
    g_pstDeiSwAlgPara.TH_H_GMDDIF_MC_P = 128;  //no register

    g_pstDeiSwAlgPara.TH_GMD_STAT   = 8;   //no register
    g_pstDeiSwAlgPara.TH_GMD_STAT_P = 8;   //no register
    g_pstDeiSwAlgPara.TH_H_VMVSUM   = 50;  //no register
    g_pstDeiSwAlgPara.TH_L_VMVSUM   = 10;  //no register
    g_pstDeiSwAlgPara.TH_H_HMVSUM   = 100; //no register
    g_pstDeiSwAlgPara.TH_L_HMVSUM   = 10;  //no register
    g_pstDeiSwAlgPara.TH_GMD_MA     = 20;  //no register

}

HI_S32 PQ_MNG_DEI_ParaInit(HI_VOID)
{
    DEI_PARAMETER_S *stDeiPara;
    HI_U32 i;
    HI_U32 HalfValue   = 16;
    HI_U32 Shift       = 5;
    HI_U8 IntpScaleRatio[15] = {4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8};

    HI_U8 DirRatio[15] = {40, 24, 32, 27, 18, 15, 12, 11, 9, 8, 7, 6, 0, 0, 0};
    stDeiPara = (DEI_PARAMETER_S *)HI_KMALLOC(HI_ID_PQ, sizeof(DEI_PARAMETER_S), GFP_KERNEL);
    PQ_CHECK_NULL_PTR(stDeiPara);
    memset(stDeiPara, 0, sizeof(DEI_PARAMETER_S));

    stDeiPara->u32EdgeSmoothRatio = 128;
    stDeiPara->bStinfoStop        = 0;
    stDeiPara->bDieRst            = 0;
    stDeiPara->u8DirMchC          = 0; /*about fmd */
    stDeiPara->u8DirMchL          = 0; /*about fmd */
    stDeiPara->bEdgeSmoothEn      = 0;
    stDeiPara->u8DeiLmmode        = 1;
    stDeiPara->u8DeiChmmode       = 1;
    stDeiPara->bDeiOutSelChm      = 0; /*about fmd */
    stDeiPara->bDeiOutSelLum      = 0; /*about fmd */

    stDeiPara->u8FrmFldBlendMode    = 0;
    stDeiPara->bFrameMotionSmoothEn = 1;
    stDeiPara->u32SCDUsingMax   = 0;
    stDeiPara->bMotionIIREn     = 1;
    stDeiPara->bLumaMfMax       = 0;
    stDeiPara->bChmaMfMax       = 0;
    stDeiPara->bRecModeEn       = 1;
    stDeiPara->u16ChromMaOffset = 8;

    stDeiPara->u32DirRatioVer    = 2;
    stDeiPara->s32MinVerStrength = 320;

    stDeiPara->u32ScaleRange    = 8;
    stDeiPara->u32Bc1MaxDz      = 30;
    stDeiPara->u32Bc1AutoDzGain = 2;
    stDeiPara->u32Bc1Gain       = 8;
    stDeiPara->u32Bc2MaxDz      = 30;
    stDeiPara->u32Bc2AutoDzGain = 2;
    stDeiPara->u32Bc2Gain       = 8;

    for (i = 0; i < 15; i++)
    {
        stDeiPara->au8DirRatio[i] = DirRatio[i];
    }

    for (i = 0; i < 15; i++)
    {
        stDeiPara->au8IntpScaleRatio[i] = IntpScaleRatio[i];
    }

    stDeiPara->u32BcGain      = 64;
    stDeiPara->u32DirThd      = 0;
    stDeiPara->u8EdgeMode     = 1;
    stDeiPara->u8HorEdgeEn    = 0;
    stDeiPara->s32StrengthThd = 16383;

    /*for calc the jitter motion*/
    stDeiPara->u16JitterGain   = 8;
    stDeiPara->u16JitterCoring = 0;

    /*for calc field/feather motion ratio*/
    stDeiPara->u16FldMotionThdLow    = 0;
    stDeiPara->u16FldMotionThdHigh   = 255;
    stDeiPara->s8FldMotionCurveSlope = -2;
    stDeiPara->u8FldMotionGain       = 8;
    stDeiPara->u16FldMotionCoring    = 0;

    /*for calc motion IIR weight*/
    stDeiPara->au16MotionDiffThd[0] = 16;
    stDeiPara->au16MotionDiffThd[1] = 144;
    stDeiPara->au16MotionDiffThd[2] = 208;
    stDeiPara->au16MotionDiffThd[3] = 255;
    stDeiPara->au16MotionDiffThd[4] = 255;
    stDeiPara->au16MotionDiffThd[5] = 255;
    stDeiPara->as16MotionIIRCurveSlope[0] = 2;
    stDeiPara->as16MotionIIRCurveSlope[1] = 2;
    stDeiPara->as16MotionIIRCurveSlope[2] = 0;
    stDeiPara->as16MotionIIRCurveSlope[3] = 0;

    stDeiPara->u16MinMotionIIrRatio = 32;
    stDeiPara->u16MaxMotionIIrRatio = 64;

    /*stDeiPara->u32StartMotionIIRRatio 1~4 需要软件计算*/
    stDeiPara->au16MotionIIRRatio[0] = 32;
    stDeiPara->au16MotionIIRRatio[1] = ALG_CLIP3(0, 127, stDeiPara->au16MotionIIRRatio[0] +
                                       ((stDeiPara->as16MotionIIRCurveSlope[0] * ((stDeiPara->au16MotionDiffThd[1] - stDeiPara->au16MotionDiffThd[0]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16MotionIIRRatio[2] = ALG_CLIP3(0, 127, stDeiPara->au16MotionIIRRatio[1] +
                                       ((stDeiPara->as16MotionIIRCurveSlope[1] * ((stDeiPara->au16MotionDiffThd[2] - stDeiPara->au16MotionDiffThd[1]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16MotionIIRRatio[3] = ALG_CLIP3(0, 127, stDeiPara->au16MotionIIRRatio[2] +
                                       ((stDeiPara->as16MotionIIRCurveSlope[2] * ((stDeiPara->au16MotionDiffThd[3] - stDeiPara->au16MotionDiffThd[2]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16MotionIIRRatio[4] = ALG_CLIP3(0, 127, stDeiPara->au16MotionIIRRatio[3] +
                                       ((stDeiPara->as16MotionIIRCurveSlope[3] * ((stDeiPara->au16MotionDiffThd[4] - stDeiPara->au16MotionDiffThd[3]) << 2) + HalfValue) >> Shift));

    /*for recursive mode*/
    stDeiPara->au8RecModeFldMotionStep[0] = 2;
    stDeiPara->au8RecModeFldMotionStep[1] = 2;
    stDeiPara->au8RecModeFrmMotionStep[0] = 0;
    stDeiPara->au8RecModeFrmMotionStep[1] = 0;

    stDeiPara->au16FrmFldBlendThd[0] = 8;
    stDeiPara->au16FrmFldBlendThd[1] = 72;
    stDeiPara->au16FrmFldBlendThd[2] = 255;
    stDeiPara->au16FrmFldBlendThd[3] = 255;
    stDeiPara->au16FrmFldBlendThd[4] = 255;
    stDeiPara->au16FrmFldBlendThd[5] = 255;
    stDeiPara->as16FrmFldBlendSlope[0] = 8;
    stDeiPara->as16FrmFldBlendSlope[1] = 0;
    stDeiPara->as16FrmFldBlendSlope[2] = 0;
    stDeiPara->as16FrmFldBlendSlope[3] = 0;

    stDeiPara->u16MinFrmFldBlendRatio = 0;
    stDeiPara->u16MaxFrmFldBlendRatio = 64;

    /*au16FrmFldBlendRatio[0]配置好，1 2 3 4需要软件计算*/
    stDeiPara->au16FrmFldBlendRatio[0] = 0;
    stDeiPara->au16FrmFldBlendRatio[1] = ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[0] +
                                         ((stDeiPara->as16FrmFldBlendSlope[0] * ((stDeiPara->au16FrmFldBlendThd[1] - stDeiPara->au16FrmFldBlendThd[0]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrmFldBlendRatio[2] = ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[1] +
                                         ((stDeiPara->as16FrmFldBlendSlope[1] * ((stDeiPara->au16FrmFldBlendThd[2] - stDeiPara->au16FrmFldBlendThd[1]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrmFldBlendRatio[3] = ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[2] +
                                         ((stDeiPara->as16FrmFldBlendSlope[2] * ((stDeiPara->au16FrmFldBlendThd[3] - stDeiPara->au16FrmFldBlendThd[2]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrmFldBlendRatio[4] =  ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[3] +
                                          ((stDeiPara->as16FrmFldBlendSlope[3] * ((stDeiPara->au16FrmFldBlendThd[4] - stDeiPara->au16FrmFldBlendThd[3]) << 2) + HalfValue) >> Shift));

    /* Debug: modify cankaochang */
    stDeiPara->bPpreInfoEn = 1;
    stDeiPara->bPreInfoEn  = 0;

    /*for history motion*/
    stDeiPara->bHisMotionEn         = 1;
    stDeiPara->bHisMotionUsingMode  = 1; //0;
    stDeiPara->bHisMotionWriteMode  = 0;
    stDeiPara->bMotionInfoWriteMode = 0;
    /*for morpological filter*/
    stDeiPara->u8MorFltThd  = 0;
    stDeiPara->u8MorFltSize = 0;
    stDeiPara->bMorFltEn    = 1;
    stDeiPara->bMedBlendEn  = 0;

    /*for comb check*/
    stDeiPara->u16CombChkMinHThd    = 255;
    stDeiPara->u16CombChkMinVThd    = 15;
    stDeiPara->u16CombChkLowerLimit = 10;
    stDeiPara->u16CombChkUpperLimit = 160;
    stDeiPara->u16CombChkEdgeThd    = 64;
    stDeiPara->u16CombChkMdThd      = 30;
    stDeiPara->bCombChkEn           = 0;

    /*for frame motion smooth*/
    stDeiPara->au16FrameMotionSmoothThd[0] = 8;
    stDeiPara->au16FrameMotionSmoothThd[1] = 72;
    stDeiPara->au16FrameMotionSmoothThd[2] = 255;
    stDeiPara->au16FrameMotionSmoothThd[3] = 255;
    stDeiPara->au16FrameMotionSmoothThd[4] = 255;
    stDeiPara->au16FrameMotionSmoothThd[5] = 255;

    stDeiPara->as16FrameMotionSmoothSlope[0] = 8;
    stDeiPara->as16FrameMotionSmoothSlope[1] = 0;
    stDeiPara->as16FrameMotionSmoothSlope[2] = 0;
    stDeiPara->as16FrameMotionSmoothSlope[3] = 0;

    stDeiPara->u16MinFrameMotionSmoothRatio = 0;
    stDeiPara->u16MaxFrameMotionSmoothRatio = 64;

    /* FrameMotionSmoothRatio */
    stDeiPara->au16FrameMotionSmoothRatio[0] = 0;
    stDeiPara->au16FrameMotionSmoothRatio[1] =  ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[0] +
            ((stDeiPara->as16FrameMotionSmoothSlope[0] * ((stDeiPara->au16FrameMotionSmoothThd[1] - stDeiPara->au16FrameMotionSmoothThd[0]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrameMotionSmoothRatio[2] =  ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[1] +
            ((stDeiPara->as16FrameMotionSmoothSlope[0] * ((stDeiPara->au16FrameMotionSmoothThd[2] - stDeiPara->au16FrameMotionSmoothThd[1]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrameMotionSmoothRatio[3] = ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[2] +
            ((stDeiPara->as16FrameMotionSmoothSlope[0] * ((stDeiPara->au16FrameMotionSmoothThd[3] - stDeiPara->au16FrameMotionSmoothThd[2]) << 2) + HalfValue) >> Shift));
    stDeiPara->au16FrameMotionSmoothRatio[4] = ALG_CLIP3(0, 127, stDeiPara->au16FrmFldBlendRatio[3] +
            ((stDeiPara->as16FrameMotionSmoothSlope[0] * ((stDeiPara->au16FrameMotionSmoothThd[4] - stDeiPara->au16FrameMotionSmoothThd[3]) << 2) + HalfValue) >> Shift));

    /* Deubg: modify Global motion Gain */
    /* for small motion adjustment */
    stDeiPara->u16AdjustGain       = 64;
    stDeiPara->u16AdjustCoring     = 0;
    stDeiPara->u16AdjustGainChroma = 64;

    /* global motion */
    stDeiPara->u32SmallmotionThd = sg_MotionCalc.u32SmallMotionThd;
    stDeiPara->u32LargemotionThd = sg_MotionCalc.u32LargeMotionThd;
    stDeiPara->bIgblEn = 1;

    //add by z214841 2015.7.9
    stDeiPara->numt_lpf_en = 0;
    stDeiPara->numt_gain   = 32;
    stDeiPara->numt_coring = 0;

    stDeiPara->ma_gbm_thd0 = 16;
    stDeiPara->ma_gbm_thd1 = 48;
    stDeiPara->ma_gbm_thd2 = 80;
    stDeiPara->ma_gbm_thd3 = 112;

    //modify by z214841 2015.7.9
    PQ_MNG_InitSoftWareAlgParm(&stDeiPara->mcdeiparams, &stDeiPara->mcdeibaseParams);

    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[0]  = 0;
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[1]  = stDeiPara->au8DirRatio[1]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[1] * 4 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[2]  = stDeiPara->au8DirRatio[2]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[2] * 5 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[3]  = stDeiPara->au8DirRatio[3]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[3] * 7 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[4]  = stDeiPara->au8DirRatio[4]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[4] * 9 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[5]  = stDeiPara->au8DirRatio[5]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[5] * 11 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[6]  = stDeiPara->au8DirRatio[6]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[6] * 13 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[7]  = stDeiPara->au8DirRatio[7]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[7] * 15 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[8]  = stDeiPara->au8DirRatio[8]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[8] * 17 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[9]  = stDeiPara->au8DirRatio[9]  == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[9] * 19 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[10] = stDeiPara->au8DirRatio[10] == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[10] * 23 * 3);
    stDeiPara->mcdeibaseParams.stINTERPINFO.EDGE_NORM[11] = stDeiPara->au8DirRatio[11] == 0 ? 0 : 65535 / (stDeiPara->au8DirRatio[11] * 27 * 3);

    HI_KFREE(HI_ID_PQ, stDeiPara);
    return HI_SUCCESS;
}

/* 初始化DEI globalmotion软件相关参数 */
HI_S32 PQ_MNG_DEI_MotionParaInit(HI_VOID)
{
    HI_U16 HalfValue = 4;
    HI_U16 Shift = 3;

    sg_MotionCalc.u32SmallMotionThd        = 16; /* 32; */
    sg_MotionCalc.u32LargeMotionThd        = 192;/* 32; 256; */
    sg_MotionCalc.au16GlobalMotionThd[0]   = 16; /* 32; */
    sg_MotionCalc.au16GlobalMotionThd[1]   = 64; /* 48; */
    sg_MotionCalc.au16GlobalMotionThd[2]   = 80; /* 96; 112; */
    sg_MotionCalc.au16GlobalMotionThd[3]   = 144;/* 208; */
    sg_MotionCalc.au16GlobalMotionThd[4]   = 255;
    sg_MotionCalc.au16GlobalMotionThd[5]   = 255;
    sg_MotionCalc.as16GlobalMotionSlope[0] = 8;
    sg_MotionCalc.as16GlobalMotionSlope[1] = 0;  /* 4; */
    sg_MotionCalc.as16GlobalMotionSlope[2] = 8;  /* 4; */
    sg_MotionCalc.as16GlobalMotionSlope[3] = 0;
    sg_MotionCalc.u16MaxGlobalMotionRatio  = 128;
    sg_MotionCalc.u16MinGlobalMotionRatio  = 16;
    sg_MotionCalc.au16GlobalMotionRatio[0] = 16;
    sg_MotionCalc.au16GlobalMotionRatio[1] = ALG_CLIP3(0, 255, sg_MotionCalc.au16GlobalMotionRatio[0] +
            ((sg_MotionCalc.as16GlobalMotionSlope[0] * (sg_MotionCalc.au16GlobalMotionThd[1] - sg_MotionCalc.au16GlobalMotionThd[0]) + HalfValue) >> Shift));
    sg_MotionCalc.au16GlobalMotionRatio[2] = ALG_CLIP3(0, 255, sg_MotionCalc.au16GlobalMotionRatio[1] +
            ((sg_MotionCalc.as16GlobalMotionSlope[1] * (sg_MotionCalc.au16GlobalMotionThd[2] - sg_MotionCalc.au16GlobalMotionThd[1]) + HalfValue) >> Shift));
    sg_MotionCalc.au16GlobalMotionRatio[3] = ALG_CLIP3(0, 255, sg_MotionCalc.au16GlobalMotionRatio[2] +
            ((sg_MotionCalc.as16GlobalMotionSlope[2] * (sg_MotionCalc.au16GlobalMotionThd[3] - sg_MotionCalc.au16GlobalMotionThd[2]) + HalfValue) >> Shift));
    sg_MotionCalc.au16GlobalMotionRatio[4] = ALG_CLIP3(0, 255, sg_MotionCalc.au16GlobalMotionRatio[3] +
            ((sg_MotionCalc.as16GlobalMotionSlope[3] * (sg_MotionCalc.au16GlobalMotionThd[4] - sg_MotionCalc.au16GlobalMotionThd[3]) + HalfValue) >> Shift));

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : API_GMD_MA
 Description  : calculate the motion gain based on the global motion
 Input        : MotionHist_S *pstMotionHist
 Output       : None
 Return Value : HI_S32
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/11/15
    Author       : t00300798
    Modification : Created function

*****************************************************************************/
static HI_S32 PQ_MNG_API_GMD_MA(const HI_U32 *pu32MotionData)
{
    HI_S32 i;
    static HI_U32 global_motion_current = 255, global_motion_last = 255;
    HI_U32 motion_num[5]     = {0}, motion_sum[5]     = {0};
    HI_U64 global_motion_num = 0,   global_motion_sum = 0;
    HI_U64 global_motion_result;

    motion_num[0] = *(pu32MotionData + 0);
    motion_num[1] = *(pu32MotionData + 1);
    motion_num[2] = *(pu32MotionData + 2);
    motion_num[3] = *(pu32MotionData + 3);
    motion_num[4] = *(pu32MotionData + 4);

    motion_sum[0] = *(pu32MotionData + 5);
    motion_sum[1] = *(pu32MotionData + 6);
    motion_sum[2] = *(pu32MotionData + 7);
    motion_sum[3] = *(pu32MotionData + 8);
    motion_sum[4] = *(pu32MotionData + 9);

    for (i = 1; i < 5; i++)
    {
        global_motion_num += motion_num[i];
        global_motion_sum += motion_sum[i];
    }

    if (global_motion_num == 0)
    {
        global_motion_num = 1;
    }

    global_motion_result = global_motion_sum;
    do_div(global_motion_result , global_motion_num);
    global_motion_result = global_motion_result >> 2;
    global_motion_current = DEI_CLIP3(0, 255, global_motion_result);
    global_motion_last    = global_motion_current;

    return global_motion_current;
}

/*****************************************************************************
 Prototype    : DI_SW_ALG
 Description  : DI软算法
 Input        : Parm_s * pstMcParm         :MCDI的参数
                DeiVariables_S * pstDeiVar :MADI的参数
                DeiSwAlgCfg_S * pstDeiSwAlgPara :软算法参数
                HI_S32 s32Width            :frame width
                HI_S32 s32Height           :frame height
                HI_S32 s32IsSceneChange    :scene change infomation
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/12/15
    Author       : t00300798
    Modification : Created function
   移植: w00273863, 2015.02.03

*****************************************************************************/
HI_S32 PQ_MNG_ALG_GetMcDeiParam(MOTION_INPUT_S *pstMotionInput)
{
    S_STT_REGS_TYPE *pstSTTReg = HI_NULL;
    HI_S32 nAdjK, nAdjK_P;
    HI_S32 nAdjK_STAT = 0;
    HI_S32 nAdjK_STAT_P = 0;
    HI_S32 nVmvSum    = 0;
    HI_S32 nHmvSum    = 0;
    HI_S32 nGlobalMotion;
    HI_S32 nK_FR_MCW;
    HI_S32 K_Y_MCW, K_C_MCW;

    HI_S32 as32Iglb2MtnNum[4];
    HI_S32 as32Iglb1MtnNum[4];
    HI_U32 au32MotionData[10];

    if (HI_FALSE == g_bMcDiEn)
    {
        return HI_SUCCESS;
    }

    PQ_CHECK_NULL_PTR(pstMotionInput);
    PQ_CHECK_NULL_PTR(pstMotionInput->pstMotionReg);
    PQ_CHECK_NULL_PTR(sg_pstMcParm);
    PQ_CHECK_NULL_PTR(sg_pstDeiVar);

    pstSTTReg = pstMotionInput->pstMotionReg;

    PQ_HAL_UpdateBlkRect(pstMotionInput->u32HandleNo, pstMotionInput->u32Width, pstMotionInput->u32Height);

    as32Iglb2MtnNum[0] = pstSTTReg->iglb2_mtn_num0.bits.iglb2_mtn_num0;
    as32Iglb2MtnNum[1] = pstSTTReg->iglb2_mtn_num1.bits.iglb2_mtn_num1;
    as32Iglb2MtnNum[2] = pstSTTReg->iglb2_mtn_num2.bits.iglb2_mtn_num2;
    as32Iglb2MtnNum[3] = pstSTTReg->iglb2_mtn_num3.bits.iglb2_mtn_num3;

    as32Iglb1MtnNum[0] = pstSTTReg->iglb1_mtn_num0.bits.iglb1_mtn_num0;
    as32Iglb1MtnNum[1] = pstSTTReg->iglb1_mtn_num1.bits.iglb1_mtn_num1;
    as32Iglb1MtnNum[2] = pstSTTReg->iglb1_mtn_num2.bits.iglb1_mtn_num2;
    as32Iglb1MtnNum[3] = pstSTTReg->iglb1_mtn_num3.bits.iglb1_mtn_num3;

    au32MotionData[0] = pstSTTReg->iglb_mtn_num0.bits.iglb_mtn_num0;
    au32MotionData[1] = pstSTTReg->iglb_mtn_num1.bits.iglb_mtn_num1;
    au32MotionData[2] = pstSTTReg->iglb_mtn_num2.bits.iglb_mtn_num2;
    au32MotionData[3] = pstSTTReg->iglb_mtn_num3.bits.iglb_mtn_num1;
    au32MotionData[4] = pstSTTReg->iglb_mtn_num4.bits.iglb_mtn_num2;
    au32MotionData[5] = pstSTTReg->iglb_mtn_sum0.bits.iglb_mtn_sum0;
    au32MotionData[6] = pstSTTReg->iglb_mtn_sum1.bits.iglb_mtn_sum1;
    au32MotionData[7] = pstSTTReg->iglb_mtn_sum2.bits.iglb_mtn_sum2;
    au32MotionData[8] = pstSTTReg->iglb_mtn_sum3.bits.iglb_mtn_sum1;
    au32MotionData[9] = pstSTTReg->iglb_mtn_sum4.bits.iglb_mtn_sum2;

    nAdjK         = PQ_MNG_ALG_API_GMD(as32Iglb2MtnNum, pstMotionInput->u32Width, pstMotionInput->u32Height / 2, &nAdjK_STAT);
    nAdjK_P       = PQ_MNG_ALG_API_GMD_P(as32Iglb1MtnNum, pstMotionInput->u32Width, pstMotionInput->u32Height, &nAdjK_STAT_P);
    nGlobalMotion = PQ_MNG_API_GMD_MA(au32MotionData);
    PQ_MNG_ALG_RGMV_HVCHK(&nVmvSum, &nHmvSum, pstMotionInput->pRGMV, pstMotionInput->u32Width, pstMotionInput->u32Height, pstMotionInput->stride);

    sg_pstMcParm->K_HFCORE_GMD_MC = nAdjK;

    /* GMD_P for film *//* 针对逐行单独处理，暂时先注释掉 */
    if ((nAdjK_STAT_P > g_pstDeiSwAlgPara.TH_GMD_STAT_P))
    {
        K_Y_MCW = 4;
        K_C_MCW = 4;
    }
    else
    {
        K_Y_MCW = 64;
        K_C_MCW = 64;
    }

    /* GMD for "to3c2flr2" */
    if (nAdjK_STAT > g_pstDeiSwAlgPara.TH_GMD_STAT)
    {
        sg_pstMcParm->MC_ONLY = 1;
    }
    else
    {
        sg_pstMcParm->MC_ONLY = 0;
    }

    /* VmvSum & global motion */
    if (nVmvSum > g_pstDeiSwAlgPara.TH_H_VMVSUM && nHmvSum < g_pstDeiSwAlgPara.TH_L_HMVSUM && nGlobalMotion > g_pstDeiSwAlgPara.TH_GMD_MA)
    {
        sg_pstDeiVar->stMDINFO.ADJUST_GAIN = ALG_CLIP3(32, 128, nGlobalMotion * 4);

        K_Y_MCW = 4;
        K_C_MCW = 4;
    }
    else
    {
        sg_pstDeiVar->stMDINFO.ADJUST_GAIN = 64;

        K_Y_MCW = 64;
        K_C_MCW = 64;
    }

    /* HmvSum */
    if (nHmvSum > g_pstDeiSwAlgPara.TH_H_HMVSUM && nVmvSum < g_pstDeiSwAlgPara.TH_L_VMVSUM)
    {
        sg_pstDeiVar->stINTERPINFO.MC_EDGE_CORING = 1023;
        sg_pstDeiVar->stINTERPINFO.MC_EDGE_SCALE  = 32;
    }
    else
    {
        sg_pstDeiVar->stINTERPINFO.MC_EDGE_CORING = 0;
        sg_pstDeiVar->stINTERPINFO.MC_EDGE_SCALE  = 32;
    }
#if defined(CHIP_TYPE_hi3798mv200) ||  defined(CHIP_TYPE_hi3798mv200_a)
    if (nHmvSum > g_pstDeiSwAlgPara.TH_H_HMVSUM && nVmvSum < g_pstDeiSwAlgPara.TH_L_VMVSUM)
    {
        sg_pstDeiVar->stMDINFO.rec_mode_fld_motion_step_0 = 3;
        sg_pstDeiVar->stMDINFO.rec_mode_fld_motion_step_1 = 3;
    }
    else
    {
        sg_pstDeiVar->stMDINFO.rec_mode_fld_motion_step_0 = 2;
        sg_pstDeiVar->stMDINFO.rec_mode_fld_motion_step_1 = 2;
    }
#endif

    /* scence change detect temp no use */

    nK_FR_MCW = MIN2(16, MAX2(0, sg_s32FRCountMC - 8) * 48 / 8);

    sg_pstMcParm->nK_MCW_Y   = nK_FR_MCW * K_Y_MCW / 16;
    sg_pstMcParm->nK_MCW_C   = nK_FR_MCW * K_C_MCW / 16;
    sg_pstMcParm->nK_MCBLD_Y = nK_FR_MCW * 64 / 16;
    sg_pstMcParm->nK_MCBLD_C = nK_FR_MCW * 64 / 16;

    sg_s32FRCountMC = (sg_s32FRCountMC == 31) ? sg_s32FRCountMC : (sg_s32FRCountMC + 1);

    sg_pstMcParm->K_C_VERTW = 15 * sg_pstMcParm->K_Y_VERTW / 16; /* 15; 8; [0,15] */

    PQ_HAL_SetSoftReg(pstMotionInput->u32HandleNo, sg_pstMcParm, sg_pstDeiVar);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDeiParam(HI_PQ_WBC_INFO_S *pstVpssWbcInfo)
{
    MOTION_INPUT_S stMotionInput;

    PQ_CHECK_NULL_PTR(pstVpssWbcInfo);

    stMotionInput.u32HandleNo  = pstVpssWbcInfo->u32HandleNo;
    stMotionInput.u32Height    = pstVpssWbcInfo->u32Height;
    stMotionInput.u32Width     = pstVpssWbcInfo->u32Width;
    stMotionInput.pstMotionReg = pstVpssWbcInfo->pstVPSSWbcReg;

    stMotionInput.u32Scd       = pstVpssWbcInfo->u32Scd;
    stMotionInput.pRGMV        = pstVpssWbcInfo->pRGMV;
    stMotionInput.stride       = pstVpssWbcInfo->stride;

    //PQ_MNG_ALG_GetGlobalMotion(&stMotionInput);
    PQ_MNG_ALG_GetMcDeiParam(&stMotionInput);

    return HI_SUCCESS;
}

/* Init DEI */
HI_S32 PQ_MNG_InitDei(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    if (HI_TRUE == sg_bDeiInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret  = PQ_HAL_SetInterlaceMotionEn(u32HandleNo, HI_TRUE);
    s32Ret |= PQ_MNG_DEI_MotionParaInit();
    s32Ret |= PQ_MNG_DEI_ParaInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("dei init error\n");
        return HI_FAILURE;
    }

    sg_pstMcParm = (HI_Mc_DeiParam_S *)HI_KMALLOC(HI_ID_PQ, sizeof(HI_Mc_DeiParam_S), GFP_KERNEL);
    PQ_CHECK_NULL_PTR(sg_pstMcParm);
    memset(sg_pstMcParm, 0, sizeof(HI_Mc_DeiParam_S));

    sg_pstDeiVar = (HI_DEI_Variable_S *)HI_KMALLOC(HI_ID_PQ, sizeof(HI_DEI_Variable_S), GFP_KERNEL);
    if (HI_NULL == sg_pstDeiVar)
    {
        HI_KFREE(HI_ID_PQ, sg_pstMcParm);
        return HI_FAILURE;
    }
    memset(sg_pstDeiVar, 0, sizeof(HI_DEI_Variable_S));

    s32Ret = PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_DEI, SOURCE_MODE_NO, OUTPUT_MODE_NO);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("dei InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDeiInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* DeInit DEI */
HI_S32 PQ_MNG_DeInitDei(HI_VOID)
{
    if (HI_FALSE == sg_bDeiInitFlag)
    {
        return HI_SUCCESS;
    }

    HI_KFREE(HI_ID_PQ, sg_pstMcParm);
    HI_KFREE(HI_ID_PQ, sg_pstDeiVar);

    sg_pstMcParm  = HI_NULL;
    sg_pstDeiVar  = HI_NULL;

    sg_bDeiInitFlag = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetMaDeiEn(HI_BOOL *pbOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    PQ_CHECK_NULL_PTR(pbOnOff);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetMaDeiEn(u32HandleNo, pbOnOff);
            break;
        }
    }
    return s32Ret;
}

HI_S32 PQ_MNG_GetMcDeiEn(HI_BOOL *pbOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    PQ_CHECK_NULL_PTR(pbOnOff);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_GetMcDeiEn(u32HandleNo, pbOnOff);
            break;
        }
    }
    return s32Ret;
}

/* enable or disable dei demo *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_EnableDeiDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;

    sg_bDeiDemoEn = bOnOff;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableDeiDemo(u32HandleNo, bOnOff);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDeiDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDeiDemoEn;
    return HI_SUCCESS;
}

/* set dei demo mode */
HI_S32 PQ_MNG_SetDeiDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;

    sg_enMode = enMode;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetDeiDemoMode(u32HandleNo, (DEI_DEMO_MODE_E)enMode);
        }
    }

    return s32Ret;
}

/* get dei demo mode *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_GetDeiDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}

/* set vpss market coordinate *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_SetDeiDemoModeCoor(HI_U32 u32XPos)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetDeiDemoModeCoor(u32HandleNo, u32XPos);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetMotionCompensateEn(HI_BOOL bOnOff)
{
    HI_U32 u32HandleNo = 0;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DEI, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    g_bMcDiEn = bOnOff;

    return PQ_HAL_SetMotionCompensateEn(u32HandleNo, bOnOff);
}

HI_S32 PQ_MNG_GetMotionCompensateEn(HI_U32 *pu32OnOff)
{
    PQ_CHECK_NULL_PTR(pu32OnOff);
    *pu32OnOff = g_bMcDiEn;

    return HI_SUCCESS;
}

static stPQAlgFuncs stDeiFuncs =
{
    .Init               = PQ_MNG_InitDei,
    .DeInit             = PQ_MNG_DeInitDei,
    .SetEnable          = PQ_MNG_SetMotionCompensateEn,
    .GetEnable          = PQ_MNG_GetMotionCompensateEn,
    .SetDemo            = PQ_MNG_EnableDeiDemo,
    .GetDemo            = PQ_MNG_GetDeiDemo,
    .SetDemoMode        = PQ_MNG_SetDeiDemoMode,
    .GetDemoMode        = PQ_MNG_GetDeiDemoMode,
    .SetDeiParam        = PQ_MNG_SetDeiParam,
    .SetDemoModeCoor    = PQ_MNG_SetDeiDemoModeCoor,
    .GetMaEnable        = PQ_MNG_GetMaDeiEn,
    .GetMcEnable        = PQ_MNG_GetMcDeiEn,
};

HI_S32 PQ_MNG_RegisterDEI(PQ_REG_TYPE_E enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DEI, enType, PQ_BIN_ADAPT_SINGLE, "dei", HI_NULL, &stDeiFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDEI()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DEI);

    return s32Ret;
}


