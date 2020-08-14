/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_tnr.c
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/01/09
 Description   :
******************************************************************************/

#include "hi_type.h"
#include "pq_hal_tnr.h"
#include "pq_hal_comm.h"

static HI_S32 sg_s32TMTYMotionGain = 32;
static HI_S32 sg_s32TMTCMotionGain = 32;

#define S10TOS32(x) ((x>0x1FF)?(x-0x400):(x))


HI_S32 PQ_HAL_SetTNRRegist(HI_U32 u32HandleNo, HI_U16 TLUT_CbCr[][32])
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_CLUT20.bits.data0 = TLUT_CbCr[0][0];
    pstVpssVirReg->VPSS_TNR_CLUT20.bits.data1 = TLUT_CbCr[0][1];
    pstVpssVirReg->VPSS_TNR_CLUT20.bits.data2 = TLUT_CbCr[0][2];
    pstVpssVirReg->VPSS_TNR_CLUT20.bits.data3 = TLUT_CbCr[0][3];
    pstVpssVirReg->VPSS_TNR_CLUT21.bits.data0 = TLUT_CbCr[0][4];
    pstVpssVirReg->VPSS_TNR_CLUT21.bits.data1 = TLUT_CbCr[0][5];
    pstVpssVirReg->VPSS_TNR_CLUT21.bits.data2 = TLUT_CbCr[0][6];
    pstVpssVirReg->VPSS_TNR_CLUT21.bits.data3 = TLUT_CbCr[0][7];
    pstVpssVirReg->VPSS_TNR_CLUT22.bits.data0 = TLUT_CbCr[0][8];
    pstVpssVirReg->VPSS_TNR_CLUT22.bits.data1 = TLUT_CbCr[0][9];
    pstVpssVirReg->VPSS_TNR_CLUT22.bits.data2 = TLUT_CbCr[0][10];
    pstVpssVirReg->VPSS_TNR_CLUT22.bits.data3 = TLUT_CbCr[0][11];
    pstVpssVirReg->VPSS_TNR_CLUT23.bits.data0 = TLUT_CbCr[0][12];
    pstVpssVirReg->VPSS_TNR_CLUT23.bits.data1 = TLUT_CbCr[0][13];
    pstVpssVirReg->VPSS_TNR_CLUT23.bits.data2 = TLUT_CbCr[0][14];
    pstVpssVirReg->VPSS_TNR_CLUT23.bits.data3 = TLUT_CbCr[0][15];
    pstVpssVirReg->VPSS_TNR_CLUT24.bits.data0 = TLUT_CbCr[0][16];
    pstVpssVirReg->VPSS_TNR_CLUT24.bits.data1 = TLUT_CbCr[0][17];
    pstVpssVirReg->VPSS_TNR_CLUT24.bits.data2 = TLUT_CbCr[0][18];
    pstVpssVirReg->VPSS_TNR_CLUT24.bits.data3 = TLUT_CbCr[0][19];
    pstVpssVirReg->VPSS_TNR_CLUT25.bits.data0 = TLUT_CbCr[0][20];
    pstVpssVirReg->VPSS_TNR_CLUT25.bits.data1 = TLUT_CbCr[0][21];
    pstVpssVirReg->VPSS_TNR_CLUT25.bits.data2 = TLUT_CbCr[0][22];
    pstVpssVirReg->VPSS_TNR_CLUT25.bits.data3 = TLUT_CbCr[0][23];
    pstVpssVirReg->VPSS_TNR_CLUT26.bits.data0 = TLUT_CbCr[0][24];
    pstVpssVirReg->VPSS_TNR_CLUT26.bits.data1 = TLUT_CbCr[0][25];
    pstVpssVirReg->VPSS_TNR_CLUT26.bits.data2 = TLUT_CbCr[0][26];
    pstVpssVirReg->VPSS_TNR_CLUT26.bits.data3 = TLUT_CbCr[0][27];
    pstVpssVirReg->VPSS_TNR_CLUT27.bits.data0 = TLUT_CbCr[0][28];
    pstVpssVirReg->VPSS_TNR_CLUT27.bits.data1 = TLUT_CbCr[0][29];
    pstVpssVirReg->VPSS_TNR_CLUT27.bits.data2 = TLUT_CbCr[0][30];
    pstVpssVirReg->VPSS_TNR_CLUT27.bits.data3 = TLUT_CbCr[0][31];
    pstVpssVirReg->VPSS_TNR_CLUT28.bits.data0 = TLUT_CbCr[1][0];
    pstVpssVirReg->VPSS_TNR_CLUT28.bits.data1 = TLUT_CbCr[1][1];
    pstVpssVirReg->VPSS_TNR_CLUT28.bits.data2 = TLUT_CbCr[1][2];
    pstVpssVirReg->VPSS_TNR_CLUT28.bits.data3 = TLUT_CbCr[1][3];
    pstVpssVirReg->VPSS_TNR_CLUT29.bits.data0 = TLUT_CbCr[1][4];
    pstVpssVirReg->VPSS_TNR_CLUT29.bits.data1 = TLUT_CbCr[1][5];
    pstVpssVirReg->VPSS_TNR_CLUT29.bits.data2 = TLUT_CbCr[1][6];
    pstVpssVirReg->VPSS_TNR_CLUT29.bits.data3 = TLUT_CbCr[1][7];
    pstVpssVirReg->VPSS_TNR_CLUT210.bits.data0 = TLUT_CbCr[1][8];
    pstVpssVirReg->VPSS_TNR_CLUT210.bits.data1 = TLUT_CbCr[1][9];
    pstVpssVirReg->VPSS_TNR_CLUT210.bits.data2 = TLUT_CbCr[1][10];
    pstVpssVirReg->VPSS_TNR_CLUT210.bits.data3 = TLUT_CbCr[1][11];
    pstVpssVirReg->VPSS_TNR_CLUT211.bits.data0 = TLUT_CbCr[1][12];
    pstVpssVirReg->VPSS_TNR_CLUT211.bits.data1 = TLUT_CbCr[1][13];
    pstVpssVirReg->VPSS_TNR_CLUT211.bits.data2 = TLUT_CbCr[1][14];
    pstVpssVirReg->VPSS_TNR_CLUT211.bits.data3 = TLUT_CbCr[1][15];
    pstVpssVirReg->VPSS_TNR_CLUT212.bits.data0 = TLUT_CbCr[1][16];
    pstVpssVirReg->VPSS_TNR_CLUT212.bits.data1 = TLUT_CbCr[1][17];
    pstVpssVirReg->VPSS_TNR_CLUT212.bits.data2 = TLUT_CbCr[1][18];
    pstVpssVirReg->VPSS_TNR_CLUT212.bits.data3 = TLUT_CbCr[1][19];
    pstVpssVirReg->VPSS_TNR_CLUT213.bits.data0 = TLUT_CbCr[1][20];
    pstVpssVirReg->VPSS_TNR_CLUT213.bits.data1 = TLUT_CbCr[1][21];
    pstVpssVirReg->VPSS_TNR_CLUT213.bits.data2 = TLUT_CbCr[1][22];
    pstVpssVirReg->VPSS_TNR_CLUT213.bits.data3 = TLUT_CbCr[1][23];
    pstVpssVirReg->VPSS_TNR_CLUT214.bits.data0 = TLUT_CbCr[1][24];
    pstVpssVirReg->VPSS_TNR_CLUT214.bits.data1 = TLUT_CbCr[1][25];
    pstVpssVirReg->VPSS_TNR_CLUT214.bits.data2 = TLUT_CbCr[1][26];
    pstVpssVirReg->VPSS_TNR_CLUT214.bits.data3 = TLUT_CbCr[1][27];
    pstVpssVirReg->VPSS_TNR_CLUT215.bits.data0 = TLUT_CbCr[1][28];
    pstVpssVirReg->VPSS_TNR_CLUT215.bits.data1 = TLUT_CbCr[1][29];
    pstVpssVirReg->VPSS_TNR_CLUT215.bits.data2 = TLUT_CbCr[1][30];
    pstVpssVirReg->VPSS_TNR_CLUT215.bits.data3 = TLUT_CbCr[1][31];
    pstVpssVirReg->VPSS_TNR_CLUT216.bits.data0 = TLUT_CbCr[2][0];
    pstVpssVirReg->VPSS_TNR_CLUT216.bits.data1 = TLUT_CbCr[2][1];
    pstVpssVirReg->VPSS_TNR_CLUT216.bits.data2 = TLUT_CbCr[2][2];
    pstVpssVirReg->VPSS_TNR_CLUT216.bits.data3 = TLUT_CbCr[2][3];
    pstVpssVirReg->VPSS_TNR_CLUT217.bits.data0 = TLUT_CbCr[2][4];
    pstVpssVirReg->VPSS_TNR_CLUT217.bits.data1 = TLUT_CbCr[2][5];
    pstVpssVirReg->VPSS_TNR_CLUT217.bits.data2 = TLUT_CbCr[2][6];
    pstVpssVirReg->VPSS_TNR_CLUT217.bits.data3 = TLUT_CbCr[2][7];
    pstVpssVirReg->VPSS_TNR_CLUT218.bits.data0 = TLUT_CbCr[2][8];
    pstVpssVirReg->VPSS_TNR_CLUT218.bits.data1 = TLUT_CbCr[2][9];
    pstVpssVirReg->VPSS_TNR_CLUT218.bits.data2 = TLUT_CbCr[2][10];
    pstVpssVirReg->VPSS_TNR_CLUT218.bits.data3 = TLUT_CbCr[2][11];
    pstVpssVirReg->VPSS_TNR_CLUT219.bits.data0 = TLUT_CbCr[2][12];
    pstVpssVirReg->VPSS_TNR_CLUT219.bits.data1 = TLUT_CbCr[2][13];
    pstVpssVirReg->VPSS_TNR_CLUT219.bits.data2 = TLUT_CbCr[2][14];
    pstVpssVirReg->VPSS_TNR_CLUT219.bits.data3 = TLUT_CbCr[2][15];
    pstVpssVirReg->VPSS_TNR_CLUT220.bits.data0 = TLUT_CbCr[2][16];
    pstVpssVirReg->VPSS_TNR_CLUT220.bits.data1 = TLUT_CbCr[2][17];
    pstVpssVirReg->VPSS_TNR_CLUT220.bits.data2 = TLUT_CbCr[2][18];
    pstVpssVirReg->VPSS_TNR_CLUT220.bits.data3 = TLUT_CbCr[2][19];
    pstVpssVirReg->VPSS_TNR_CLUT221.bits.data0 = TLUT_CbCr[2][20];
    pstVpssVirReg->VPSS_TNR_CLUT221.bits.data1 = TLUT_CbCr[2][21];
    pstVpssVirReg->VPSS_TNR_CLUT221.bits.data2 = TLUT_CbCr[2][22];
    pstVpssVirReg->VPSS_TNR_CLUT221.bits.data3 = TLUT_CbCr[2][23];
    pstVpssVirReg->VPSS_TNR_CLUT222.bits.data0 = TLUT_CbCr[2][24];
    pstVpssVirReg->VPSS_TNR_CLUT222.bits.data1 = TLUT_CbCr[2][25];
    pstVpssVirReg->VPSS_TNR_CLUT222.bits.data2 = TLUT_CbCr[2][26];
    pstVpssVirReg->VPSS_TNR_CLUT222.bits.data3 = TLUT_CbCr[2][27];
    pstVpssVirReg->VPSS_TNR_CLUT223.bits.data0 = TLUT_CbCr[2][28];
    pstVpssVirReg->VPSS_TNR_CLUT223.bits.data1 = TLUT_CbCr[2][29];
    pstVpssVirReg->VPSS_TNR_CLUT223.bits.data2 = TLUT_CbCr[2][30];
    pstVpssVirReg->VPSS_TNR_CLUT223.bits.data3 = TLUT_CbCr[2][31];
    pstVpssVirReg->VPSS_TNR_CLUT224.bits.data0 = TLUT_CbCr[3][0];
    pstVpssVirReg->VPSS_TNR_CLUT224.bits.data1 = TLUT_CbCr[3][1];
    pstVpssVirReg->VPSS_TNR_CLUT224.bits.data2 = TLUT_CbCr[3][2];
    pstVpssVirReg->VPSS_TNR_CLUT224.bits.data3 = TLUT_CbCr[3][3];
    pstVpssVirReg->VPSS_TNR_CLUT225.bits.data0 = TLUT_CbCr[3][4];
    pstVpssVirReg->VPSS_TNR_CLUT225.bits.data1 = TLUT_CbCr[3][5];
    pstVpssVirReg->VPSS_TNR_CLUT225.bits.data2 = TLUT_CbCr[3][6];
    pstVpssVirReg->VPSS_TNR_CLUT225.bits.data3 = TLUT_CbCr[3][7];
    pstVpssVirReg->VPSS_TNR_CLUT226.bits.data0 = TLUT_CbCr[3][8];
    pstVpssVirReg->VPSS_TNR_CLUT226.bits.data1 = TLUT_CbCr[3][9];
    pstVpssVirReg->VPSS_TNR_CLUT226.bits.data2 = TLUT_CbCr[3][10];
    pstVpssVirReg->VPSS_TNR_CLUT226.bits.data3 = TLUT_CbCr[3][11];
    pstVpssVirReg->VPSS_TNR_CLUT227.bits.data0 = TLUT_CbCr[3][12];
    pstVpssVirReg->VPSS_TNR_CLUT227.bits.data1 = TLUT_CbCr[3][13];
    pstVpssVirReg->VPSS_TNR_CLUT227.bits.data2 = TLUT_CbCr[3][14];
    pstVpssVirReg->VPSS_TNR_CLUT227.bits.data3 = TLUT_CbCr[3][15];
    pstVpssVirReg->VPSS_TNR_CLUT228.bits.data0 = TLUT_CbCr[3][16];
    pstVpssVirReg->VPSS_TNR_CLUT228.bits.data1 = TLUT_CbCr[3][17];
    pstVpssVirReg->VPSS_TNR_CLUT228.bits.data2 = TLUT_CbCr[3][18];
    pstVpssVirReg->VPSS_TNR_CLUT228.bits.data3 = TLUT_CbCr[3][19];
    pstVpssVirReg->VPSS_TNR_CLUT229.bits.data0 = TLUT_CbCr[3][20];
    pstVpssVirReg->VPSS_TNR_CLUT229.bits.data1 = TLUT_CbCr[3][21];
    pstVpssVirReg->VPSS_TNR_CLUT229.bits.data2 = TLUT_CbCr[3][22];
    pstVpssVirReg->VPSS_TNR_CLUT229.bits.data3 = TLUT_CbCr[3][23];
    pstVpssVirReg->VPSS_TNR_CLUT230.bits.data0 = TLUT_CbCr[3][24];
    pstVpssVirReg->VPSS_TNR_CLUT230.bits.data1 = TLUT_CbCr[3][25];
    pstVpssVirReg->VPSS_TNR_CLUT230.bits.data2 = TLUT_CbCr[3][26];
    pstVpssVirReg->VPSS_TNR_CLUT230.bits.data3 = TLUT_CbCr[3][27];
    pstVpssVirReg->VPSS_TNR_CLUT231.bits.data0 = TLUT_CbCr[3][28];
    pstVpssVirReg->VPSS_TNR_CLUT231.bits.data1 = TLUT_CbCr[3][29];
    pstVpssVirReg->VPSS_TNR_CLUT231.bits.data2 = TLUT_CbCr[3][30];
    pstVpssVirReg->VPSS_TNR_CLUT231.bits.data3 = TLUT_CbCr[3][31];
    pstVpssVirReg->VPSS_TNR_CLUT232.bits.data0 = TLUT_CbCr[4][0];
    pstVpssVirReg->VPSS_TNR_CLUT232.bits.data1 = TLUT_CbCr[4][1];
    pstVpssVirReg->VPSS_TNR_CLUT232.bits.data2 = TLUT_CbCr[4][2];
    pstVpssVirReg->VPSS_TNR_CLUT232.bits.data3 = TLUT_CbCr[4][3];
    pstVpssVirReg->VPSS_TNR_CLUT233.bits.data0 = TLUT_CbCr[4][4];
    pstVpssVirReg->VPSS_TNR_CLUT233.bits.data1 = TLUT_CbCr[4][5];
    pstVpssVirReg->VPSS_TNR_CLUT233.bits.data2 = TLUT_CbCr[4][6];
    pstVpssVirReg->VPSS_TNR_CLUT233.bits.data3 = TLUT_CbCr[4][7];
    pstVpssVirReg->VPSS_TNR_CLUT234.bits.data0 = TLUT_CbCr[4][8];
    pstVpssVirReg->VPSS_TNR_CLUT234.bits.data1 = TLUT_CbCr[4][9];
    pstVpssVirReg->VPSS_TNR_CLUT234.bits.data2 = TLUT_CbCr[4][10];
    pstVpssVirReg->VPSS_TNR_CLUT234.bits.data3 = TLUT_CbCr[4][11];
    pstVpssVirReg->VPSS_TNR_CLUT235.bits.data0 = TLUT_CbCr[4][12];
    pstVpssVirReg->VPSS_TNR_CLUT235.bits.data1 = TLUT_CbCr[4][13];
    pstVpssVirReg->VPSS_TNR_CLUT235.bits.data2 = TLUT_CbCr[4][14];
    pstVpssVirReg->VPSS_TNR_CLUT235.bits.data3 = TLUT_CbCr[4][15];
    pstVpssVirReg->VPSS_TNR_CLUT236.bits.data0 = TLUT_CbCr[4][16];
    pstVpssVirReg->VPSS_TNR_CLUT236.bits.data1 = TLUT_CbCr[4][17];
    pstVpssVirReg->VPSS_TNR_CLUT236.bits.data2 = TLUT_CbCr[4][18];
    pstVpssVirReg->VPSS_TNR_CLUT236.bits.data3 = TLUT_CbCr[4][19];
    pstVpssVirReg->VPSS_TNR_CLUT237.bits.data0 = TLUT_CbCr[4][20];
    pstVpssVirReg->VPSS_TNR_CLUT237.bits.data1 = TLUT_CbCr[4][21];
    pstVpssVirReg->VPSS_TNR_CLUT237.bits.data2 = TLUT_CbCr[4][22];
    pstVpssVirReg->VPSS_TNR_CLUT237.bits.data3 = TLUT_CbCr[4][23];
    pstVpssVirReg->VPSS_TNR_CLUT238.bits.data0 = TLUT_CbCr[4][24];
    pstVpssVirReg->VPSS_TNR_CLUT238.bits.data1 = TLUT_CbCr[4][25];
    pstVpssVirReg->VPSS_TNR_CLUT238.bits.data2 = TLUT_CbCr[4][26];
    pstVpssVirReg->VPSS_TNR_CLUT238.bits.data3 = TLUT_CbCr[4][27];
    pstVpssVirReg->VPSS_TNR_CLUT239.bits.data0 = TLUT_CbCr[4][28];
    pstVpssVirReg->VPSS_TNR_CLUT239.bits.data1 = TLUT_CbCr[4][29];
    pstVpssVirReg->VPSS_TNR_CLUT239.bits.data2 = TLUT_CbCr[4][30];
    pstVpssVirReg->VPSS_TNR_CLUT239.bits.data3 = TLUT_CbCr[4][31];
    pstVpssVirReg->VPSS_TNR_CLUT240.bits.data0 = TLUT_CbCr[5][0];
    pstVpssVirReg->VPSS_TNR_CLUT240.bits.data1 = TLUT_CbCr[5][1];
    pstVpssVirReg->VPSS_TNR_CLUT240.bits.data2 = TLUT_CbCr[5][2];
    pstVpssVirReg->VPSS_TNR_CLUT240.bits.data3 = TLUT_CbCr[5][3];
    pstVpssVirReg->VPSS_TNR_CLUT241.bits.data0 = TLUT_CbCr[5][4];
    pstVpssVirReg->VPSS_TNR_CLUT241.bits.data1 = TLUT_CbCr[5][5];
    pstVpssVirReg->VPSS_TNR_CLUT241.bits.data2 = TLUT_CbCr[5][6];
    pstVpssVirReg->VPSS_TNR_CLUT241.bits.data3 = TLUT_CbCr[5][7];
    pstVpssVirReg->VPSS_TNR_CLUT242.bits.data0 = TLUT_CbCr[5][8];
    pstVpssVirReg->VPSS_TNR_CLUT242.bits.data1 = TLUT_CbCr[5][9];
    pstVpssVirReg->VPSS_TNR_CLUT242.bits.data2 = TLUT_CbCr[5][10];
    pstVpssVirReg->VPSS_TNR_CLUT242.bits.data3 = TLUT_CbCr[5][11];
    pstVpssVirReg->VPSS_TNR_CLUT243.bits.data0 = TLUT_CbCr[5][12];
    pstVpssVirReg->VPSS_TNR_CLUT243.bits.data1 = TLUT_CbCr[5][13];
    pstVpssVirReg->VPSS_TNR_CLUT243.bits.data2 = TLUT_CbCr[5][14];
    pstVpssVirReg->VPSS_TNR_CLUT243.bits.data3 = TLUT_CbCr[5][15];
    pstVpssVirReg->VPSS_TNR_CLUT244.bits.data0 = TLUT_CbCr[5][16];
    pstVpssVirReg->VPSS_TNR_CLUT244.bits.data1 = TLUT_CbCr[5][17];
    pstVpssVirReg->VPSS_TNR_CLUT244.bits.data2 = TLUT_CbCr[5][18];
    pstVpssVirReg->VPSS_TNR_CLUT244.bits.data3 = TLUT_CbCr[5][19];
    pstVpssVirReg->VPSS_TNR_CLUT245.bits.data0 = TLUT_CbCr[5][20];
    pstVpssVirReg->VPSS_TNR_CLUT245.bits.data1 = TLUT_CbCr[5][21];
    pstVpssVirReg->VPSS_TNR_CLUT245.bits.data2 = TLUT_CbCr[5][22];
    pstVpssVirReg->VPSS_TNR_CLUT245.bits.data3 = TLUT_CbCr[5][23];
    pstVpssVirReg->VPSS_TNR_CLUT246.bits.data0 = TLUT_CbCr[5][24];
    pstVpssVirReg->VPSS_TNR_CLUT246.bits.data1 = TLUT_CbCr[5][25];
    pstVpssVirReg->VPSS_TNR_CLUT246.bits.data2 = TLUT_CbCr[5][26];
    pstVpssVirReg->VPSS_TNR_CLUT246.bits.data3 = TLUT_CbCr[5][27];
    pstVpssVirReg->VPSS_TNR_CLUT247.bits.data0 = TLUT_CbCr[5][28];
    pstVpssVirReg->VPSS_TNR_CLUT247.bits.data1 = TLUT_CbCr[5][29];
    pstVpssVirReg->VPSS_TNR_CLUT247.bits.data2 = TLUT_CbCr[5][30];
    pstVpssVirReg->VPSS_TNR_CLUT247.bits.data3 = TLUT_CbCr[5][31];
    pstVpssVirReg->VPSS_TNR_CLUT248.bits.data0 = TLUT_CbCr[6][0];
    pstVpssVirReg->VPSS_TNR_CLUT248.bits.data1 = TLUT_CbCr[6][1];
    pstVpssVirReg->VPSS_TNR_CLUT248.bits.data2 = TLUT_CbCr[6][2];
    pstVpssVirReg->VPSS_TNR_CLUT248.bits.data3 = TLUT_CbCr[6][3];
    pstVpssVirReg->VPSS_TNR_CLUT249.bits.data0 = TLUT_CbCr[6][4];
    pstVpssVirReg->VPSS_TNR_CLUT249.bits.data1 = TLUT_CbCr[6][5];
    pstVpssVirReg->VPSS_TNR_CLUT249.bits.data2 = TLUT_CbCr[6][6];
    pstVpssVirReg->VPSS_TNR_CLUT249.bits.data3 = TLUT_CbCr[6][7];
    pstVpssVirReg->VPSS_TNR_CLUT250.bits.data0 = TLUT_CbCr[6][8];
    pstVpssVirReg->VPSS_TNR_CLUT250.bits.data1 = TLUT_CbCr[6][9];
    pstVpssVirReg->VPSS_TNR_CLUT250.bits.data2 = TLUT_CbCr[6][10];
    pstVpssVirReg->VPSS_TNR_CLUT250.bits.data3 = TLUT_CbCr[6][11];
    pstVpssVirReg->VPSS_TNR_CLUT251.bits.data0 = TLUT_CbCr[6][12];
    pstVpssVirReg->VPSS_TNR_CLUT251.bits.data1 = TLUT_CbCr[6][13];
    pstVpssVirReg->VPSS_TNR_CLUT251.bits.data2 = TLUT_CbCr[6][14];
    pstVpssVirReg->VPSS_TNR_CLUT251.bits.data3 = TLUT_CbCr[6][15];
    pstVpssVirReg->VPSS_TNR_CLUT252.bits.data0 = TLUT_CbCr[6][16];
    pstVpssVirReg->VPSS_TNR_CLUT252.bits.data1 = TLUT_CbCr[6][17];
    pstVpssVirReg->VPSS_TNR_CLUT252.bits.data2 = TLUT_CbCr[6][18];
    pstVpssVirReg->VPSS_TNR_CLUT252.bits.data3 = TLUT_CbCr[6][19];
    pstVpssVirReg->VPSS_TNR_CLUT253.bits.data0 = TLUT_CbCr[6][20];
    pstVpssVirReg->VPSS_TNR_CLUT253.bits.data1 = TLUT_CbCr[6][21];
    pstVpssVirReg->VPSS_TNR_CLUT253.bits.data2 = TLUT_CbCr[6][22];
    pstVpssVirReg->VPSS_TNR_CLUT253.bits.data3 = TLUT_CbCr[6][23];
    pstVpssVirReg->VPSS_TNR_CLUT254.bits.data0 = TLUT_CbCr[6][24];
    pstVpssVirReg->VPSS_TNR_CLUT254.bits.data1 = TLUT_CbCr[6][25];
    pstVpssVirReg->VPSS_TNR_CLUT254.bits.data2 = TLUT_CbCr[6][26];
    pstVpssVirReg->VPSS_TNR_CLUT254.bits.data3 = TLUT_CbCr[6][27];
    pstVpssVirReg->VPSS_TNR_CLUT255.bits.data0 = TLUT_CbCr[6][28];
    pstVpssVirReg->VPSS_TNR_CLUT255.bits.data1 = TLUT_CbCr[6][29];
    pstVpssVirReg->VPSS_TNR_CLUT255.bits.data2 = TLUT_CbCr[6][30];
    pstVpssVirReg->VPSS_TNR_CLUT255.bits.data3 = TLUT_CbCr[6][31];
    pstVpssVirReg->VPSS_TNR_CLUT256.bits.data0 = TLUT_CbCr[7][0];
    pstVpssVirReg->VPSS_TNR_CLUT256.bits.data1 = TLUT_CbCr[7][1];
    pstVpssVirReg->VPSS_TNR_CLUT256.bits.data2 = TLUT_CbCr[7][2];
    pstVpssVirReg->VPSS_TNR_CLUT256.bits.data3 = TLUT_CbCr[7][3];
    pstVpssVirReg->VPSS_TNR_CLUT257.bits.data0 = TLUT_CbCr[7][4];
    pstVpssVirReg->VPSS_TNR_CLUT257.bits.data1 = TLUT_CbCr[7][5];
    pstVpssVirReg->VPSS_TNR_CLUT257.bits.data2 = TLUT_CbCr[7][6];
    pstVpssVirReg->VPSS_TNR_CLUT257.bits.data3 = TLUT_CbCr[7][7];
    pstVpssVirReg->VPSS_TNR_CLUT258.bits.data0 = TLUT_CbCr[7][8];
    pstVpssVirReg->VPSS_TNR_CLUT258.bits.data1 = TLUT_CbCr[7][9];
    pstVpssVirReg->VPSS_TNR_CLUT258.bits.data2 = TLUT_CbCr[7][10];
    pstVpssVirReg->VPSS_TNR_CLUT258.bits.data3 = TLUT_CbCr[7][11];
    pstVpssVirReg->VPSS_TNR_CLUT259.bits.data0 = TLUT_CbCr[7][12];
    pstVpssVirReg->VPSS_TNR_CLUT259.bits.data1 = TLUT_CbCr[7][13];
    pstVpssVirReg->VPSS_TNR_CLUT259.bits.data2 = TLUT_CbCr[7][14];
    pstVpssVirReg->VPSS_TNR_CLUT259.bits.data3 = TLUT_CbCr[7][15];
    pstVpssVirReg->VPSS_TNR_CLUT260.bits.data0 = TLUT_CbCr[7][16];
    pstVpssVirReg->VPSS_TNR_CLUT260.bits.data1 = TLUT_CbCr[7][17];
    pstVpssVirReg->VPSS_TNR_CLUT260.bits.data2 = TLUT_CbCr[7][18];
    pstVpssVirReg->VPSS_TNR_CLUT260.bits.data3 = TLUT_CbCr[7][19];
    pstVpssVirReg->VPSS_TNR_CLUT261.bits.data0 = TLUT_CbCr[7][20];
    pstVpssVirReg->VPSS_TNR_CLUT261.bits.data1 = TLUT_CbCr[7][21];
    pstVpssVirReg->VPSS_TNR_CLUT261.bits.data2 = TLUT_CbCr[7][22];
    pstVpssVirReg->VPSS_TNR_CLUT261.bits.data3 = TLUT_CbCr[7][23];
    pstVpssVirReg->VPSS_TNR_CLUT262.bits.data0 = TLUT_CbCr[7][24];
    pstVpssVirReg->VPSS_TNR_CLUT262.bits.data1 = TLUT_CbCr[7][25];
    pstVpssVirReg->VPSS_TNR_CLUT262.bits.data2 = TLUT_CbCr[7][26];
    pstVpssVirReg->VPSS_TNR_CLUT262.bits.data3 = TLUT_CbCr[7][27];
    pstVpssVirReg->VPSS_TNR_CLUT263.bits.data0 = TLUT_CbCr[7][28];
    pstVpssVirReg->VPSS_TNR_CLUT263.bits.data1 = TLUT_CbCr[7][29];
    pstVpssVirReg->VPSS_TNR_CLUT263.bits.data2 = TLUT_CbCr[7][30];
    pstVpssVirReg->VPSS_TNR_CLUT263.bits.data3 = TLUT_CbCr[7][31];
    pstVpssVirReg->VPSS_TNR_CLUT264.bits.data0 = TLUT_CbCr[8][0];
    pstVpssVirReg->VPSS_TNR_CLUT264.bits.data1 = TLUT_CbCr[8][1];
    pstVpssVirReg->VPSS_TNR_CLUT264.bits.data2 = TLUT_CbCr[8][2];
    pstVpssVirReg->VPSS_TNR_CLUT264.bits.data3 = TLUT_CbCr[8][3];
    pstVpssVirReg->VPSS_TNR_CLUT265.bits.data0 = TLUT_CbCr[8][4];
    pstVpssVirReg->VPSS_TNR_CLUT265.bits.data1 = TLUT_CbCr[8][5];
    pstVpssVirReg->VPSS_TNR_CLUT265.bits.data2 = TLUT_CbCr[8][6];
    pstVpssVirReg->VPSS_TNR_CLUT265.bits.data3 = TLUT_CbCr[8][7];
    pstVpssVirReg->VPSS_TNR_CLUT266.bits.data0 = TLUT_CbCr[8][8];
    pstVpssVirReg->VPSS_TNR_CLUT266.bits.data1 = TLUT_CbCr[8][9];
    pstVpssVirReg->VPSS_TNR_CLUT266.bits.data2 = TLUT_CbCr[8][10];
    pstVpssVirReg->VPSS_TNR_CLUT266.bits.data3 = TLUT_CbCr[8][11];
    pstVpssVirReg->VPSS_TNR_CLUT267.bits.data0 = TLUT_CbCr[8][12];
    pstVpssVirReg->VPSS_TNR_CLUT267.bits.data1 = TLUT_CbCr[8][13];
    pstVpssVirReg->VPSS_TNR_CLUT267.bits.data2 = TLUT_CbCr[8][14];
    pstVpssVirReg->VPSS_TNR_CLUT267.bits.data3 = TLUT_CbCr[8][15];
    pstVpssVirReg->VPSS_TNR_CLUT268.bits.data0 = TLUT_CbCr[8][16];
    pstVpssVirReg->VPSS_TNR_CLUT268.bits.data1 = TLUT_CbCr[8][17];
    pstVpssVirReg->VPSS_TNR_CLUT268.bits.data2 = TLUT_CbCr[8][18];
    pstVpssVirReg->VPSS_TNR_CLUT268.bits.data3 = TLUT_CbCr[8][19];
    pstVpssVirReg->VPSS_TNR_CLUT269.bits.data0 = TLUT_CbCr[8][20];
    pstVpssVirReg->VPSS_TNR_CLUT269.bits.data1 = TLUT_CbCr[8][21];
    pstVpssVirReg->VPSS_TNR_CLUT269.bits.data2 = TLUT_CbCr[8][22];
    pstVpssVirReg->VPSS_TNR_CLUT269.bits.data3 = TLUT_CbCr[8][23];
    pstVpssVirReg->VPSS_TNR_CLUT270.bits.data0 = TLUT_CbCr[8][24];
    pstVpssVirReg->VPSS_TNR_CLUT270.bits.data1 = TLUT_CbCr[8][25];
    pstVpssVirReg->VPSS_TNR_CLUT270.bits.data2 = TLUT_CbCr[8][26];
    pstVpssVirReg->VPSS_TNR_CLUT270.bits.data3 = TLUT_CbCr[8][27];
    pstVpssVirReg->VPSS_TNR_CLUT271.bits.data0 = TLUT_CbCr[8][28];
    pstVpssVirReg->VPSS_TNR_CLUT271.bits.data1 = TLUT_CbCr[8][29];
    pstVpssVirReg->VPSS_TNR_CLUT271.bits.data2 = TLUT_CbCr[8][30];
    pstVpssVirReg->VPSS_TNR_CLUT271.bits.data3 = TLUT_CbCr[8][31];
    pstVpssVirReg->VPSS_TNR_CLUT272.bits.data0 = TLUT_CbCr[9][0];
    pstVpssVirReg->VPSS_TNR_CLUT272.bits.data1 = TLUT_CbCr[9][1];
    pstVpssVirReg->VPSS_TNR_CLUT272.bits.data2 = TLUT_CbCr[9][2];
    pstVpssVirReg->VPSS_TNR_CLUT272.bits.data3 = TLUT_CbCr[9][3];
    pstVpssVirReg->VPSS_TNR_CLUT273.bits.data0 = TLUT_CbCr[9][4];
    pstVpssVirReg->VPSS_TNR_CLUT273.bits.data1 = TLUT_CbCr[9][5];
    pstVpssVirReg->VPSS_TNR_CLUT273.bits.data2 = TLUT_CbCr[9][6];
    pstVpssVirReg->VPSS_TNR_CLUT273.bits.data3 = TLUT_CbCr[9][7];
    pstVpssVirReg->VPSS_TNR_CLUT274.bits.data0 = TLUT_CbCr[9][8];
    pstVpssVirReg->VPSS_TNR_CLUT274.bits.data1 = TLUT_CbCr[9][9];
    pstVpssVirReg->VPSS_TNR_CLUT274.bits.data2 = TLUT_CbCr[9][10];
    pstVpssVirReg->VPSS_TNR_CLUT274.bits.data3 = TLUT_CbCr[9][11];
    pstVpssVirReg->VPSS_TNR_CLUT275.bits.data0 = TLUT_CbCr[9][12];
    pstVpssVirReg->VPSS_TNR_CLUT275.bits.data1 = TLUT_CbCr[9][13];
    pstVpssVirReg->VPSS_TNR_CLUT275.bits.data2 = TLUT_CbCr[9][14];
    pstVpssVirReg->VPSS_TNR_CLUT275.bits.data3 = TLUT_CbCr[9][15];
    pstVpssVirReg->VPSS_TNR_CLUT276.bits.data0 = TLUT_CbCr[9][16];
    pstVpssVirReg->VPSS_TNR_CLUT276.bits.data1 = TLUT_CbCr[9][17];
    pstVpssVirReg->VPSS_TNR_CLUT276.bits.data2 = TLUT_CbCr[9][18];
    pstVpssVirReg->VPSS_TNR_CLUT276.bits.data3 = TLUT_CbCr[9][19];
    pstVpssVirReg->VPSS_TNR_CLUT277.bits.data0 = TLUT_CbCr[9][20];
    pstVpssVirReg->VPSS_TNR_CLUT277.bits.data1 = TLUT_CbCr[9][21];
    pstVpssVirReg->VPSS_TNR_CLUT277.bits.data2 = TLUT_CbCr[9][22];
    pstVpssVirReg->VPSS_TNR_CLUT277.bits.data3 = TLUT_CbCr[9][23];
    pstVpssVirReg->VPSS_TNR_CLUT278.bits.data0 = TLUT_CbCr[9][24];
    pstVpssVirReg->VPSS_TNR_CLUT278.bits.data1 = TLUT_CbCr[9][25];
    pstVpssVirReg->VPSS_TNR_CLUT278.bits.data2 = TLUT_CbCr[9][26];
    pstVpssVirReg->VPSS_TNR_CLUT278.bits.data3 = TLUT_CbCr[9][27];
    pstVpssVirReg->VPSS_TNR_CLUT279.bits.data0 = TLUT_CbCr[9][28];
    pstVpssVirReg->VPSS_TNR_CLUT279.bits.data1 = TLUT_CbCr[9][29];
    pstVpssVirReg->VPSS_TNR_CLUT279.bits.data2 = TLUT_CbCr[9][30];
    pstVpssVirReg->VPSS_TNR_CLUT279.bits.data3 = TLUT_CbCr[9][31];
    pstVpssVirReg->VPSS_TNR_CLUT280.bits.data0 = TLUT_CbCr[10][0];
    pstVpssVirReg->VPSS_TNR_CLUT280.bits.data1 = TLUT_CbCr[10][1];
    pstVpssVirReg->VPSS_TNR_CLUT280.bits.data2 = TLUT_CbCr[10][2];
    pstVpssVirReg->VPSS_TNR_CLUT280.bits.data3 = TLUT_CbCr[10][3];
    pstVpssVirReg->VPSS_TNR_CLUT281.bits.data0 = TLUT_CbCr[10][4];
    pstVpssVirReg->VPSS_TNR_CLUT281.bits.data1 = TLUT_CbCr[10][5];
    pstVpssVirReg->VPSS_TNR_CLUT281.bits.data2 = TLUT_CbCr[10][6];
    pstVpssVirReg->VPSS_TNR_CLUT281.bits.data3 = TLUT_CbCr[10][7];
    pstVpssVirReg->VPSS_TNR_CLUT282.bits.data0 = TLUT_CbCr[10][8];
    pstVpssVirReg->VPSS_TNR_CLUT282.bits.data1 = TLUT_CbCr[10][9];
    pstVpssVirReg->VPSS_TNR_CLUT282.bits.data2 = TLUT_CbCr[10][10];
    pstVpssVirReg->VPSS_TNR_CLUT282.bits.data3 = TLUT_CbCr[10][11];
    pstVpssVirReg->VPSS_TNR_CLUT283.bits.data0 = TLUT_CbCr[10][12];
    pstVpssVirReg->VPSS_TNR_CLUT283.bits.data1 = TLUT_CbCr[10][13];
    pstVpssVirReg->VPSS_TNR_CLUT283.bits.data2 = TLUT_CbCr[10][14];
    pstVpssVirReg->VPSS_TNR_CLUT283.bits.data3 = TLUT_CbCr[10][15];
    pstVpssVirReg->VPSS_TNR_CLUT284.bits.data0 = TLUT_CbCr[10][16];
    pstVpssVirReg->VPSS_TNR_CLUT284.bits.data1 = TLUT_CbCr[10][17];
    pstVpssVirReg->VPSS_TNR_CLUT284.bits.data2 = TLUT_CbCr[10][18];
    pstVpssVirReg->VPSS_TNR_CLUT284.bits.data3 = TLUT_CbCr[10][19];
    pstVpssVirReg->VPSS_TNR_CLUT285.bits.data0 = TLUT_CbCr[10][20];
    pstVpssVirReg->VPSS_TNR_CLUT285.bits.data1 = TLUT_CbCr[10][21];
    pstVpssVirReg->VPSS_TNR_CLUT285.bits.data2 = TLUT_CbCr[10][22];
    pstVpssVirReg->VPSS_TNR_CLUT285.bits.data3 = TLUT_CbCr[10][23];
    pstVpssVirReg->VPSS_TNR_CLUT286.bits.data0 = TLUT_CbCr[10][24];
    pstVpssVirReg->VPSS_TNR_CLUT286.bits.data1 = TLUT_CbCr[10][25];
    pstVpssVirReg->VPSS_TNR_CLUT286.bits.data2 = TLUT_CbCr[10][26];
    pstVpssVirReg->VPSS_TNR_CLUT286.bits.data3 = TLUT_CbCr[10][27];
    pstVpssVirReg->VPSS_TNR_CLUT287.bits.data0 = TLUT_CbCr[10][28];
    pstVpssVirReg->VPSS_TNR_CLUT287.bits.data1 = TLUT_CbCr[10][29];
    pstVpssVirReg->VPSS_TNR_CLUT287.bits.data2 = TLUT_CbCr[10][30];
    pstVpssVirReg->VPSS_TNR_CLUT287.bits.data3 = TLUT_CbCr[10][31];
    pstVpssVirReg->VPSS_TNR_CLUT288.bits.data0 = TLUT_CbCr[11][0];
    pstVpssVirReg->VPSS_TNR_CLUT288.bits.data1 = TLUT_CbCr[11][1];
    pstVpssVirReg->VPSS_TNR_CLUT288.bits.data2 = TLUT_CbCr[11][2];
    pstVpssVirReg->VPSS_TNR_CLUT288.bits.data3 = TLUT_CbCr[11][3];
    pstVpssVirReg->VPSS_TNR_CLUT289.bits.data0 = TLUT_CbCr[11][4];
    pstVpssVirReg->VPSS_TNR_CLUT289.bits.data1 = TLUT_CbCr[11][5];
    pstVpssVirReg->VPSS_TNR_CLUT289.bits.data2 = TLUT_CbCr[11][6];
    pstVpssVirReg->VPSS_TNR_CLUT289.bits.data3 = TLUT_CbCr[11][7];
    pstVpssVirReg->VPSS_TNR_CLUT290.bits.data0 = TLUT_CbCr[11][8];
    pstVpssVirReg->VPSS_TNR_CLUT290.bits.data1 = TLUT_CbCr[11][9];
    pstVpssVirReg->VPSS_TNR_CLUT290.bits.data2 = TLUT_CbCr[11][10];
    pstVpssVirReg->VPSS_TNR_CLUT290.bits.data3 = TLUT_CbCr[11][11];
    pstVpssVirReg->VPSS_TNR_CLUT291.bits.data0 = TLUT_CbCr[11][12];
    pstVpssVirReg->VPSS_TNR_CLUT291.bits.data1 = TLUT_CbCr[11][13];
    pstVpssVirReg->VPSS_TNR_CLUT291.bits.data2 = TLUT_CbCr[11][14];
    pstVpssVirReg->VPSS_TNR_CLUT291.bits.data3 = TLUT_CbCr[11][15];
    pstVpssVirReg->VPSS_TNR_CLUT292.bits.data0 = TLUT_CbCr[11][16];
    pstVpssVirReg->VPSS_TNR_CLUT292.bits.data1 = TLUT_CbCr[11][17];
    pstVpssVirReg->VPSS_TNR_CLUT292.bits.data2 = TLUT_CbCr[11][18];
    pstVpssVirReg->VPSS_TNR_CLUT292.bits.data3 = TLUT_CbCr[11][19];
    pstVpssVirReg->VPSS_TNR_CLUT293.bits.data0 = TLUT_CbCr[11][20];
    pstVpssVirReg->VPSS_TNR_CLUT293.bits.data1 = TLUT_CbCr[11][21];
    pstVpssVirReg->VPSS_TNR_CLUT293.bits.data2 = TLUT_CbCr[11][22];
    pstVpssVirReg->VPSS_TNR_CLUT293.bits.data3 = TLUT_CbCr[11][23];
    pstVpssVirReg->VPSS_TNR_CLUT294.bits.data0 = TLUT_CbCr[11][24];
    pstVpssVirReg->VPSS_TNR_CLUT294.bits.data1 = TLUT_CbCr[11][25];
    pstVpssVirReg->VPSS_TNR_CLUT294.bits.data2 = TLUT_CbCr[11][26];
    pstVpssVirReg->VPSS_TNR_CLUT294.bits.data3 = TLUT_CbCr[11][27];
    pstVpssVirReg->VPSS_TNR_CLUT295.bits.data0 = TLUT_CbCr[11][28];
    pstVpssVirReg->VPSS_TNR_CLUT295.bits.data1 = TLUT_CbCr[11][29];
    pstVpssVirReg->VPSS_TNR_CLUT295.bits.data2 = TLUT_CbCr[11][30];
    pstVpssVirReg->VPSS_TNR_CLUT295.bits.data3 = TLUT_CbCr[11][31];
    pstVpssVirReg->VPSS_TNR_CLUT296.bits.data0 = TLUT_CbCr[12][0];
    pstVpssVirReg->VPSS_TNR_CLUT296.bits.data1 = TLUT_CbCr[12][1];
    pstVpssVirReg->VPSS_TNR_CLUT296.bits.data2 = TLUT_CbCr[12][2];
    pstVpssVirReg->VPSS_TNR_CLUT296.bits.data3 = TLUT_CbCr[12][3];
    pstVpssVirReg->VPSS_TNR_CLUT297.bits.data0 = TLUT_CbCr[12][4];
    pstVpssVirReg->VPSS_TNR_CLUT297.bits.data1 = TLUT_CbCr[12][5];
    pstVpssVirReg->VPSS_TNR_CLUT297.bits.data2 = TLUT_CbCr[12][6];
    pstVpssVirReg->VPSS_TNR_CLUT297.bits.data3 = TLUT_CbCr[12][7];
    pstVpssVirReg->VPSS_TNR_CLUT298.bits.data0 = TLUT_CbCr[12][8];
    pstVpssVirReg->VPSS_TNR_CLUT298.bits.data1 = TLUT_CbCr[12][9];
    pstVpssVirReg->VPSS_TNR_CLUT298.bits.data2 = TLUT_CbCr[12][10];
    pstVpssVirReg->VPSS_TNR_CLUT298.bits.data3 = TLUT_CbCr[12][11];
    pstVpssVirReg->VPSS_TNR_CLUT299.bits.data0 = TLUT_CbCr[12][12];
    pstVpssVirReg->VPSS_TNR_CLUT299.bits.data1 = TLUT_CbCr[12][13];
    pstVpssVirReg->VPSS_TNR_CLUT299.bits.data2 = TLUT_CbCr[12][14];
    pstVpssVirReg->VPSS_TNR_CLUT299.bits.data3 = TLUT_CbCr[12][15];
    pstVpssVirReg->VPSS_TNR_CLUT2100.bits.data0 = TLUT_CbCr[12][16];
    pstVpssVirReg->VPSS_TNR_CLUT2100.bits.data1 = TLUT_CbCr[12][17];
    pstVpssVirReg->VPSS_TNR_CLUT2100.bits.data2 = TLUT_CbCr[12][18];
    pstVpssVirReg->VPSS_TNR_CLUT2100.bits.data3 = TLUT_CbCr[12][19];
    pstVpssVirReg->VPSS_TNR_CLUT2101.bits.data0 = TLUT_CbCr[12][20];
    pstVpssVirReg->VPSS_TNR_CLUT2101.bits.data1 = TLUT_CbCr[12][21];
    pstVpssVirReg->VPSS_TNR_CLUT2101.bits.data2 = TLUT_CbCr[12][22];
    pstVpssVirReg->VPSS_TNR_CLUT2101.bits.data3 = TLUT_CbCr[12][23];
    pstVpssVirReg->VPSS_TNR_CLUT2102.bits.data0 = TLUT_CbCr[12][24];
    pstVpssVirReg->VPSS_TNR_CLUT2102.bits.data1 = TLUT_CbCr[12][25];
    pstVpssVirReg->VPSS_TNR_CLUT2102.bits.data2 = TLUT_CbCr[12][26];
    pstVpssVirReg->VPSS_TNR_CLUT2102.bits.data3 = TLUT_CbCr[12][27];
    pstVpssVirReg->VPSS_TNR_CLUT2103.bits.data0 = TLUT_CbCr[12][28];
    pstVpssVirReg->VPSS_TNR_CLUT2103.bits.data1 = TLUT_CbCr[12][29];
    pstVpssVirReg->VPSS_TNR_CLUT2103.bits.data2 = TLUT_CbCr[12][30];
    pstVpssVirReg->VPSS_TNR_CLUT2103.bits.data3 = TLUT_CbCr[12][31];
    pstVpssVirReg->VPSS_TNR_CLUT2104.bits.data0 = TLUT_CbCr[13][0];
    pstVpssVirReg->VPSS_TNR_CLUT2104.bits.data1 = TLUT_CbCr[13][1];
    pstVpssVirReg->VPSS_TNR_CLUT2104.bits.data2 = TLUT_CbCr[13][2];
    pstVpssVirReg->VPSS_TNR_CLUT2104.bits.data3 = TLUT_CbCr[13][3];
    pstVpssVirReg->VPSS_TNR_CLUT2105.bits.data0 = TLUT_CbCr[13][4];
    pstVpssVirReg->VPSS_TNR_CLUT2105.bits.data1 = TLUT_CbCr[13][5];
    pstVpssVirReg->VPSS_TNR_CLUT2105.bits.data2 = TLUT_CbCr[13][6];
    pstVpssVirReg->VPSS_TNR_CLUT2105.bits.data3 = TLUT_CbCr[13][7];
    pstVpssVirReg->VPSS_TNR_CLUT2106.bits.data0 = TLUT_CbCr[13][8];
    pstVpssVirReg->VPSS_TNR_CLUT2106.bits.data1 = TLUT_CbCr[13][9];
    pstVpssVirReg->VPSS_TNR_CLUT2106.bits.data2 = TLUT_CbCr[13][10];
    pstVpssVirReg->VPSS_TNR_CLUT2106.bits.data3 = TLUT_CbCr[13][11];
    pstVpssVirReg->VPSS_TNR_CLUT2107.bits.data0 = TLUT_CbCr[13][12];
    pstVpssVirReg->VPSS_TNR_CLUT2107.bits.data1 = TLUT_CbCr[13][13];
    pstVpssVirReg->VPSS_TNR_CLUT2107.bits.data2 = TLUT_CbCr[13][14];
    pstVpssVirReg->VPSS_TNR_CLUT2107.bits.data3 = TLUT_CbCr[13][15];
    pstVpssVirReg->VPSS_TNR_CLUT2108.bits.data0 = TLUT_CbCr[13][16];
    pstVpssVirReg->VPSS_TNR_CLUT2108.bits.data1 = TLUT_CbCr[13][17];
    pstVpssVirReg->VPSS_TNR_CLUT2108.bits.data2 = TLUT_CbCr[13][18];
    pstVpssVirReg->VPSS_TNR_CLUT2108.bits.data3 = TLUT_CbCr[13][19];
    pstVpssVirReg->VPSS_TNR_CLUT2109.bits.data0 = TLUT_CbCr[13][20];
    pstVpssVirReg->VPSS_TNR_CLUT2109.bits.data1 = TLUT_CbCr[13][21];
    pstVpssVirReg->VPSS_TNR_CLUT2109.bits.data2 = TLUT_CbCr[13][22];
    pstVpssVirReg->VPSS_TNR_CLUT2109.bits.data3 = TLUT_CbCr[13][23];
    pstVpssVirReg->VPSS_TNR_CLUT2110.bits.data0 = TLUT_CbCr[13][24];
    pstVpssVirReg->VPSS_TNR_CLUT2110.bits.data1 = TLUT_CbCr[13][25];
    pstVpssVirReg->VPSS_TNR_CLUT2110.bits.data2 = TLUT_CbCr[13][26];
    pstVpssVirReg->VPSS_TNR_CLUT2110.bits.data3 = TLUT_CbCr[13][27];
    pstVpssVirReg->VPSS_TNR_CLUT2111.bits.data0 = TLUT_CbCr[13][28];
    pstVpssVirReg->VPSS_TNR_CLUT2111.bits.data1 = TLUT_CbCr[13][29];
    pstVpssVirReg->VPSS_TNR_CLUT2111.bits.data2 = TLUT_CbCr[13][30];
    pstVpssVirReg->VPSS_TNR_CLUT2111.bits.data3 = TLUT_CbCr[13][31];
    pstVpssVirReg->VPSS_TNR_CLUT2112.bits.data0 = TLUT_CbCr[14][0];
    pstVpssVirReg->VPSS_TNR_CLUT2112.bits.data1 = TLUT_CbCr[14][1];
    pstVpssVirReg->VPSS_TNR_CLUT2112.bits.data2 = TLUT_CbCr[14][2];
    pstVpssVirReg->VPSS_TNR_CLUT2112.bits.data3 = TLUT_CbCr[14][3];
    pstVpssVirReg->VPSS_TNR_CLUT2113.bits.data0 = TLUT_CbCr[14][4];
    pstVpssVirReg->VPSS_TNR_CLUT2113.bits.data1 = TLUT_CbCr[14][5];
    pstVpssVirReg->VPSS_TNR_CLUT2113.bits.data2 = TLUT_CbCr[14][6];
    pstVpssVirReg->VPSS_TNR_CLUT2113.bits.data3 = TLUT_CbCr[14][7];
    pstVpssVirReg->VPSS_TNR_CLUT2114.bits.data0 = TLUT_CbCr[14][8];
    pstVpssVirReg->VPSS_TNR_CLUT2114.bits.data1 = TLUT_CbCr[14][9];
    pstVpssVirReg->VPSS_TNR_CLUT2114.bits.data2 = TLUT_CbCr[14][10];
    pstVpssVirReg->VPSS_TNR_CLUT2114.bits.data3 = TLUT_CbCr[14][11];
    pstVpssVirReg->VPSS_TNR_CLUT2115.bits.data0 = TLUT_CbCr[14][12];
    pstVpssVirReg->VPSS_TNR_CLUT2115.bits.data1 = TLUT_CbCr[14][13];
    pstVpssVirReg->VPSS_TNR_CLUT2115.bits.data2 = TLUT_CbCr[14][14];
    pstVpssVirReg->VPSS_TNR_CLUT2115.bits.data3 = TLUT_CbCr[14][15];
    pstVpssVirReg->VPSS_TNR_CLUT2116.bits.data0 = TLUT_CbCr[14][16];
    pstVpssVirReg->VPSS_TNR_CLUT2116.bits.data1 = TLUT_CbCr[14][17];
    pstVpssVirReg->VPSS_TNR_CLUT2116.bits.data2 = TLUT_CbCr[14][18];
    pstVpssVirReg->VPSS_TNR_CLUT2116.bits.data3 = TLUT_CbCr[14][19];
    pstVpssVirReg->VPSS_TNR_CLUT2117.bits.data0 = TLUT_CbCr[14][20];
    pstVpssVirReg->VPSS_TNR_CLUT2117.bits.data1 = TLUT_CbCr[14][21];
    pstVpssVirReg->VPSS_TNR_CLUT2117.bits.data2 = TLUT_CbCr[14][22];
    pstVpssVirReg->VPSS_TNR_CLUT2117.bits.data3 = TLUT_CbCr[14][23];
    pstVpssVirReg->VPSS_TNR_CLUT2118.bits.data0 = TLUT_CbCr[14][24];
    pstVpssVirReg->VPSS_TNR_CLUT2118.bits.data1 = TLUT_CbCr[14][25];
    pstVpssVirReg->VPSS_TNR_CLUT2118.bits.data2 = TLUT_CbCr[14][26];
    pstVpssVirReg->VPSS_TNR_CLUT2118.bits.data3 = TLUT_CbCr[14][27];
    pstVpssVirReg->VPSS_TNR_CLUT2119.bits.data0 = TLUT_CbCr[14][28];
    pstVpssVirReg->VPSS_TNR_CLUT2119.bits.data1 = TLUT_CbCr[14][29];
    pstVpssVirReg->VPSS_TNR_CLUT2119.bits.data2 = TLUT_CbCr[14][30];
    pstVpssVirReg->VPSS_TNR_CLUT2119.bits.data3 = TLUT_CbCr[14][31];
    pstVpssVirReg->VPSS_TNR_CLUT2120.bits.data0 = TLUT_CbCr[15][0];
    pstVpssVirReg->VPSS_TNR_CLUT2120.bits.data1 = TLUT_CbCr[15][1];
    pstVpssVirReg->VPSS_TNR_CLUT2120.bits.data2 = TLUT_CbCr[15][2];
    pstVpssVirReg->VPSS_TNR_CLUT2120.bits.data3 = TLUT_CbCr[15][3];
    pstVpssVirReg->VPSS_TNR_CLUT2121.bits.data0 = TLUT_CbCr[15][4];
    pstVpssVirReg->VPSS_TNR_CLUT2121.bits.data1 = TLUT_CbCr[15][5];
    pstVpssVirReg->VPSS_TNR_CLUT2121.bits.data2 = TLUT_CbCr[15][6];
    pstVpssVirReg->VPSS_TNR_CLUT2121.bits.data3 = TLUT_CbCr[15][7];
    pstVpssVirReg->VPSS_TNR_CLUT2122.bits.data0 = TLUT_CbCr[15][8];
    pstVpssVirReg->VPSS_TNR_CLUT2122.bits.data1 = TLUT_CbCr[15][9];
    pstVpssVirReg->VPSS_TNR_CLUT2122.bits.data2 = TLUT_CbCr[15][10];
    pstVpssVirReg->VPSS_TNR_CLUT2122.bits.data3 = TLUT_CbCr[15][11];
    pstVpssVirReg->VPSS_TNR_CLUT2123.bits.data0 = TLUT_CbCr[15][12];
    pstVpssVirReg->VPSS_TNR_CLUT2123.bits.data1 = TLUT_CbCr[15][13];
    pstVpssVirReg->VPSS_TNR_CLUT2123.bits.data2 = TLUT_CbCr[15][14];
    pstVpssVirReg->VPSS_TNR_CLUT2123.bits.data3 = TLUT_CbCr[15][15];
    pstVpssVirReg->VPSS_TNR_CLUT2124.bits.data0 = TLUT_CbCr[15][16];
    pstVpssVirReg->VPSS_TNR_CLUT2124.bits.data1 = TLUT_CbCr[15][17];
    pstVpssVirReg->VPSS_TNR_CLUT2124.bits.data2 = TLUT_CbCr[15][18];
    pstVpssVirReg->VPSS_TNR_CLUT2124.bits.data3 = TLUT_CbCr[15][19];
    pstVpssVirReg->VPSS_TNR_CLUT2125.bits.data0 = TLUT_CbCr[15][20];
    pstVpssVirReg->VPSS_TNR_CLUT2125.bits.data1 = TLUT_CbCr[15][21];
    pstVpssVirReg->VPSS_TNR_CLUT2125.bits.data2 = TLUT_CbCr[15][22];
    pstVpssVirReg->VPSS_TNR_CLUT2125.bits.data3 = TLUT_CbCr[15][23];
    pstVpssVirReg->VPSS_TNR_CLUT2126.bits.data0 = TLUT_CbCr[15][24];
    pstVpssVirReg->VPSS_TNR_CLUT2126.bits.data1 = TLUT_CbCr[15][25];
    pstVpssVirReg->VPSS_TNR_CLUT2126.bits.data2 = TLUT_CbCr[15][26];
    pstVpssVirReg->VPSS_TNR_CLUT2126.bits.data3 = TLUT_CbCr[15][27];
    pstVpssVirReg->VPSS_TNR_CLUT2127.bits.data0 = TLUT_CbCr[15][28];
    pstVpssVirReg->VPSS_TNR_CLUT2127.bits.data1 = TLUT_CbCr[15][29];
    pstVpssVirReg->VPSS_TNR_CLUT2127.bits.data2 = TLUT_CbCr[15][30];
    pstVpssVirReg->VPSS_TNR_CLUT2127.bits.data3 = TLUT_CbCr[15][31];

    return HI_SUCCESS;
}





/**
 \brief :  Enable or Disable TNr Module;

 \param[in] bNROnOff

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_HAL_EnableTNR(HI_U32 u32HandleNo, HI_BOOL bNROnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_MODE.bits.tnr_y_bypass = !bNROnOff;
    pstVpssVirReg->VPSS_TNR_MODE.bits.tnr_c_bypass = !bNROnOff;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_EnableTNRDemo(HI_U32 u32HandleNo, HI_BOOL bTNRDemoEn)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_MODE.bits.market_en = bTNRDemoEn;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTNRDemoMode(HI_U32 u32HandleNo, TNR_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_MODE.bits.market_mode = (HI_U32)enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrMidStr(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    sg_s32TMTYMotionGain = pstVpssVirReg->VPSS_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k;
    sg_s32TMTCMotionGain = pstVpssVirReg->VPSS_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k;

    return HI_SUCCESS;
}


/**
 \brief Set Tnr Strengh
 \attention \n
无

 \param[in] s32TnrStr  Tnr Strengh[0,100]

 \retval ::HI_SUCCESS

 */

HI_S32 PQ_HAL_SetTnrStr(HI_U32 u32HandleNo, HI_S32 s32TnrStr)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_TFM_YMOTION_CK.bits.tfm_ymotion_k = ((100 - s32TnrStr) * sg_s32TMTYMotionGain + 25) / 50;
    pstVpssVirReg->VPSS_TNR_TFM_CMOTION_CK.bits.tfm_cmotion_k = ((100 - s32TnrStr) * sg_s32TMTCMotionGain + 25) / 50;

    return HI_SUCCESS;
}


/**
  \brief 设置TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrYMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min = pstTNR_YMotionPixmeanRatio->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max = pstTNR_YMotionPixmeanRatio->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0 = pstTNR_YMotionPixmeanRatio->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1 = pstTNR_YMotionPixmeanRatio->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2 = pstTNR_YMotionPixmeanRatio->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3 = pstTNR_YMotionPixmeanRatio->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4 = pstTNR_YMotionPixmeanRatio->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1 = pstTNR_YMotionPixmeanRatio->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2 = pstTNR_YMotionPixmeanRatio->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3 = pstTNR_YMotionPixmeanRatio->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4 = pstTNR_YMotionPixmeanRatio->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5 = pstTNR_YMotionPixmeanRatio->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6 = pstTNR_YMotionPixmeanRatio->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1 = pstTNR_YMotionPixmeanRatio->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2 = pstTNR_YMotionPixmeanRatio->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3 = pstTNR_YMotionPixmeanRatio->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4 = pstTNR_YMotionPixmeanRatio->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的PixMean-Ratio For YMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_YMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrYMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_YMotionPixmeanRatio->s32MappingMin   = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_min;
    pstTNR_YMotionPixmeanRatio->s32MappingMax   = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_CTRL.bits.tm_ymt_meanratio_max;

    pstTNR_YMotionPixmeanRatio->s32MappingR[0]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r0;
    pstTNR_YMotionPixmeanRatio->s32MappingR[1]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r1;
    pstTNR_YMotionPixmeanRatio->s32MappingR[2]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R0.bits.tm_ymt_meanratio_r2;

    pstTNR_YMotionPixmeanRatio->s32MappingR[3]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r3;
    pstTNR_YMotionPixmeanRatio->s32MappingR[4]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_R1.bits.tm_ymt_meanratio_r4;

    pstTNR_YMotionPixmeanRatio->s32MappingT[0]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t1;
    pstTNR_YMotionPixmeanRatio->s32MappingT[1]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t2;
    pstTNR_YMotionPixmeanRatio->s32MappingT[2]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T1.bits.tm_ymt_meanratio_t3;

    pstTNR_YMotionPixmeanRatio->s32MappingT[3]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t4;
    pstTNR_YMotionPixmeanRatio->s32MappingT[4]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t5;
    pstTNR_YMotionPixmeanRatio->s32MappingT[5]  = pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_T2.bits.tm_ymt_meanratio_t6;

    pstTNR_YMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k1);
    pstTNR_YMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K1.bits.tm_ymt_meanratio_k2);

    pstTNR_YMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k3);
    pstTNR_YMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MEANRATIO_K2.bits.tm_ymt_meanratio_k4);

    return HI_SUCCESS;
}





/**
  \brief 设置TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrCMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min = pstTNR_CMotionPixmeanRatio->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max = pstTNR_CMotionPixmeanRatio->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0 = pstTNR_CMotionPixmeanRatio->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1 = pstTNR_CMotionPixmeanRatio->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2 = pstTNR_CMotionPixmeanRatio->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3 = pstTNR_CMotionPixmeanRatio->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4 = pstTNR_CMotionPixmeanRatio->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1 = pstTNR_CMotionPixmeanRatio->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2 = pstTNR_CMotionPixmeanRatio->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3 = pstTNR_CMotionPixmeanRatio->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4 = pstTNR_CMotionPixmeanRatio->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5 = pstTNR_CMotionPixmeanRatio->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6 = pstTNR_CMotionPixmeanRatio->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1 = pstTNR_CMotionPixmeanRatio->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2 = pstTNR_CMotionPixmeanRatio->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3 = pstTNR_CMotionPixmeanRatio->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4 = pstTNR_CMotionPixmeanRatio->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的PixMean-Ratio For CMotion Detection参数
  \attention \n
 无

  \param[in] pstTNR_CMotionPixmeanRatio

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrCMotionPixMeanRatio(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionPixmeanRatio)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_CMotionPixmeanRatio->s32MappingMin = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_min;
    pstTNR_CMotionPixmeanRatio->s32MappingMax = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_CTRL.bits.tm_cmt_meanratio_max;

    pstTNR_CMotionPixmeanRatio->s32MappingR[0]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r0;
    pstTNR_CMotionPixmeanRatio->s32MappingR[1]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r1;
    pstTNR_CMotionPixmeanRatio->s32MappingR[2]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R0.bits.tm_cmt_meanratio_r2;

    pstTNR_CMotionPixmeanRatio->s32MappingR[3]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r3;
    pstTNR_CMotionPixmeanRatio->s32MappingR[4]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_R1.bits.tm_cmt_meanratio_r4;

    pstTNR_CMotionPixmeanRatio->s32MappingT[0]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t1;
    pstTNR_CMotionPixmeanRatio->s32MappingT[1]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t2;
    pstTNR_CMotionPixmeanRatio->s32MappingT[2]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T1.bits.tm_cmt_meanratio_t3;

    pstTNR_CMotionPixmeanRatio->s32MappingT[3]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t4;
    pstTNR_CMotionPixmeanRatio->s32MappingT[4]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t5;
    pstTNR_CMotionPixmeanRatio->s32MappingT[5]  = pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_T2.bits.tm_cmt_meanratio_t6;

    pstTNR_CMotionPixmeanRatio->s32MappingK[0]  = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k1);
    pstTNR_CMotionPixmeanRatio->s32MappingK[1]  = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K1.bits.tm_cmt_meanratio_k2);

    pstTNR_CMotionPixmeanRatio->s32MappingK[2]  = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k3);
    pstTNR_CMotionPixmeanRatio->s32MappingK[3]  = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MEANRATIO_K2.bits.tm_cmt_meanratio_k4);


    return HI_SUCCESS;
}






/**
  \brief 设置TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrYMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min = pstTNR_YMotionMapping->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max = pstTNR_YMotionMapping->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0 = pstTNR_YMotionMapping->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1 = pstTNR_YMotionMapping->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2 = pstTNR_YMotionMapping->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3 = pstTNR_YMotionMapping->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4 = pstTNR_YMotionMapping->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1 = pstTNR_YMotionMapping->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2 = pstTNR_YMotionMapping->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3 = pstTNR_YMotionMapping->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4 = pstTNR_YMotionMapping->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5 = pstTNR_YMotionMapping->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6 = pstTNR_YMotionMapping->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1 = pstTNR_YMotionMapping->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2 = pstTNR_YMotionMapping->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3 = pstTNR_YMotionMapping->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4 = pstTNR_YMotionMapping->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的YMotionMapping参数
  \attention \n
 无

  \param[in] pstTNR_YMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrYMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_YMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_YMotionMapping->s32MappingMin  = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_min;
    pstTNR_YMotionMapping->s32MappingMax  = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_CTRL.bits.tm_ymt_mapping_max;

    pstTNR_YMotionMapping->s32MappingR[0] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r0;
    pstTNR_YMotionMapping->s32MappingR[1] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r1;
    pstTNR_YMotionMapping->s32MappingR[2] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R0.bits.tm_ymt_mapping_r2;

    pstTNR_YMotionMapping->s32MappingR[3] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r3;
    pstTNR_YMotionMapping->s32MappingR[4] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_R1.bits.tm_ymt_mapping_r4;

    pstTNR_YMotionMapping->s32MappingT[0] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t1;
    pstTNR_YMotionMapping->s32MappingT[1] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t2;
    pstTNR_YMotionMapping->s32MappingT[2] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T1.bits.tm_ymt_mapping_t3;

    pstTNR_YMotionMapping->s32MappingT[3] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t4;
    pstTNR_YMotionMapping->s32MappingT[4] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t5;
    pstTNR_YMotionMapping->s32MappingT[5] = pstVpssVirReg->VPSS_TNR_YMOTION_MAP_T2.bits.tm_ymt_mapping_t6;

    pstTNR_YMotionMapping->s32MappingK[0] = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k1);
    pstTNR_YMotionMapping->s32MappingK[1] = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K1.bits.tm_ymt_mapping_k2);

    pstTNR_YMotionMapping->s32MappingK[2] = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k3);
    pstTNR_YMotionMapping->s32MappingK[3] = S10TOS32(pstVpssVirReg->VPSS_TNR_YMOTION_MAP_K2.bits.tm_ymt_mapping_k4);


    return HI_SUCCESS;
}








/**
  \brief 设置TNR的CMotonMapping 参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrCMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min = pstTNR_CMotionMapping->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max = pstTNR_CMotionMapping->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0 = pstTNR_CMotionMapping->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1 = pstTNR_CMotionMapping->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2 = pstTNR_CMotionMapping->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3 = pstTNR_CMotionMapping->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4 = pstTNR_CMotionMapping->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1 = pstTNR_CMotionMapping->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2 = pstTNR_CMotionMapping->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3 = pstTNR_CMotionMapping->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4 = pstTNR_CMotionMapping->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5 = pstTNR_CMotionMapping->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6 = pstTNR_CMotionMapping->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1 = pstTNR_CMotionMapping->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2 = pstTNR_CMotionMapping->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3 = pstTNR_CMotionMapping->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4 = pstTNR_CMotionMapping->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的CMotonMapping参数
  \attention \n
 无

  \param[in] pstTNR_CMotionMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrCMotionMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_CMotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_CMotionMapping->s32MappingMin  = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_min;
    pstTNR_CMotionMapping->s32MappingMax  = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_CTRL.bits.tm_cmt_mapping_max;

    pstTNR_CMotionMapping->s32MappingR[0] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r0;
    pstTNR_CMotionMapping->s32MappingR[1] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r1;
    pstTNR_CMotionMapping->s32MappingR[2] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R0.bits.tm_cmt_mapping_r2;

    pstTNR_CMotionMapping->s32MappingR[3] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r3;
    pstTNR_CMotionMapping->s32MappingR[4] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_R1.bits.tm_cmt_mapping_r4;

    pstTNR_CMotionMapping->s32MappingT[0] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t1;
    pstTNR_CMotionMapping->s32MappingT[1] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t2;
    pstTNR_CMotionMapping->s32MappingT[2] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T1.bits.tm_cmt_mapping_t3;

    pstTNR_CMotionMapping->s32MappingT[3] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t4;
    pstTNR_CMotionMapping->s32MappingT[4] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t5;
    pstTNR_CMotionMapping->s32MappingT[5] = pstVpssVirReg->VPSS_TNR_CMOTION_MAP_T2.bits.tm_cmt_mapping_t6;

    pstTNR_CMotionMapping->s32MappingK[0] = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k1);
    pstTNR_CMotionMapping->s32MappingK[1] = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K1.bits.tm_cmt_mapping_k2);

    pstTNR_CMotionMapping->s32MappingK[2] = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k3);
    pstTNR_CMotionMapping->s32MappingK[3] = S10TOS32(pstVpssVirReg->VPSS_TNR_CMOTION_MAP_K2.bits.tm_cmt_mapping_k4);


    return HI_SUCCESS;
}






/**
  \brief 设置TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrFnlMotionYMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min = pstTNR_FnlMotionYMapping->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max = pstTNR_FnlMotionYMapping->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0 = pstTNR_FnlMotionYMapping->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1 = pstTNR_FnlMotionYMapping->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2 = pstTNR_FnlMotionYMapping->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3 = pstTNR_FnlMotionYMapping->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4 = pstTNR_FnlMotionYMapping->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1 = pstTNR_FnlMotionYMapping->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2 = pstTNR_FnlMotionYMapping->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3 = pstTNR_FnlMotionYMapping->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4 = pstTNR_FnlMotionYMapping->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5 = pstTNR_FnlMotionYMapping->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6 = pstTNR_FnlMotionYMapping->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1 = pstTNR_FnlMotionYMapping->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2 = pstTNR_FnlMotionYMapping->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3 = pstTNR_FnlMotionYMapping->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4 = pstTNR_FnlMotionYMapping->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的Final Y Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionYMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrFnlMotionYMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionYMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_FnlMotionYMapping->s32MappingMin = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_min;
    pstTNR_FnlMotionYMapping->s32MappingMax = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_CTRL.bits.tfm_y_mapping_max;

    pstTNR_FnlMotionYMapping->s32MappingR[0]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r0;
    pstTNR_FnlMotionYMapping->s32MappingR[1]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r1;
    pstTNR_FnlMotionYMapping->s32MappingR[2]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R0.bits.tfm_y_mapping_r2;

    pstTNR_FnlMotionYMapping->s32MappingR[3]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r3;
    pstTNR_FnlMotionYMapping->s32MappingR[4]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_R1.bits.tfm_y_mapping_r4;

    pstTNR_FnlMotionYMapping->s32MappingT[0]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t1;
    pstTNR_FnlMotionYMapping->s32MappingT[1]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t2;
    pstTNR_FnlMotionYMapping->s32MappingT[2]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T1.bits.tfm_y_mapping_t3;

    pstTNR_FnlMotionYMapping->s32MappingT[3]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t4;
    pstTNR_FnlMotionYMapping->s32MappingT[4]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t5;
    pstTNR_FnlMotionYMapping->s32MappingT[5]  = pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_T2.bits.tfm_y_mapping_t6;

    pstTNR_FnlMotionYMapping->s32MappingK[0]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k1);
    pstTNR_FnlMotionYMapping->s32MappingK[1]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K1.bits.tfm_y_mapping_k2);

    pstTNR_FnlMotionYMapping->s32MappingK[2]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k3);
    pstTNR_FnlMotionYMapping->s32MappingK[3]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_Y_MAPPING_K2.bits.tfm_y_mapping_k4);


    return HI_SUCCESS;
}









/**
  \brief 设置TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_SetTnrFnlMotionCMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min = pstTNR_FnlMotionCMapping->s32MappingMin;
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max = pstTNR_FnlMotionCMapping->s32MappingMax;

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0 = pstTNR_FnlMotionCMapping->s32MappingR[0];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1 = pstTNR_FnlMotionCMapping->s32MappingR[1];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2 = pstTNR_FnlMotionCMapping->s32MappingR[2];

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3 = pstTNR_FnlMotionCMapping->s32MappingR[3];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4 = pstTNR_FnlMotionCMapping->s32MappingR[4];

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = pstTNR_FnlMotionCMapping->s32MappingT[0];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = pstTNR_FnlMotionCMapping->s32MappingT[1];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = pstTNR_FnlMotionCMapping->s32MappingT[2];

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = pstTNR_FnlMotionCMapping->s32MappingT[3];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = pstTNR_FnlMotionCMapping->s32MappingT[4];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = pstTNR_FnlMotionCMapping->s32MappingT[5];

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = pstTNR_FnlMotionCMapping->s32MappingK[0];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = pstTNR_FnlMotionCMapping->s32MappingK[1];

    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = pstTNR_FnlMotionCMapping->s32MappingK[2];
    pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = pstTNR_FnlMotionCMapping->s32MappingK[3];


    return HI_SUCCESS;
}




/**
  \brief 获取TNR的Final C Motion Mapping参数
  \attention \n
 无

  \param[in] pstTNR_FnlMotionCMapping

  \retval ::HI_SUCCESS

  */

HI_S32 PQ_HAL_GetTnrFnlMotionCMapping(HI_U32 u32HandleNo, TNR_MAPPING_S* pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstTNR_FnlMotionCMapping->s32MappingMin = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min;
    pstTNR_FnlMotionCMapping->s32MappingMax = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max;

    pstTNR_FnlMotionCMapping->s32MappingR[0]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0;
    pstTNR_FnlMotionCMapping->s32MappingR[1]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1;
    pstTNR_FnlMotionCMapping->s32MappingR[2]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2;

    pstTNR_FnlMotionCMapping->s32MappingR[3]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3;
    pstTNR_FnlMotionCMapping->s32MappingR[4]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4;

    pstTNR_FnlMotionCMapping->s32MappingT[0]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1;
    pstTNR_FnlMotionCMapping->s32MappingT[1]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2;
    pstTNR_FnlMotionCMapping->s32MappingT[2]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3;

    pstTNR_FnlMotionCMapping->s32MappingT[3]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4;
    pstTNR_FnlMotionCMapping->s32MappingT[4]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5;
    pstTNR_FnlMotionCMapping->s32MappingT[5]  = pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6;

    pstTNR_FnlMotionCMapping->s32MappingK[0]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1);
    pstTNR_FnlMotionCMapping->s32MappingK[1]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2);

    pstTNR_FnlMotionCMapping->s32MappingK[2]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3);
    pstTNR_FnlMotionCMapping->s32MappingK[3]  = S10TOS32(pstVpssVirReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4);


    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrFmotionMapping(HI_U32 u32HandleNo, TNR_FMOTION_MAPPING_S* pstFmotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_CLUT50.bits.data0 = pstFmotionMapping->s16YFmotion[0];
    pstVpssVirReg->VPSS_TNR_CLUT50.bits.data1 = pstFmotionMapping->s16YFmotion[1];
    pstVpssVirReg->VPSS_TNR_CLUT50.bits.data2 = pstFmotionMapping->s16YFmotion[2];
    pstVpssVirReg->VPSS_TNR_CLUT50.bits.data3 = pstFmotionMapping->s16YFmotion[3];

    pstVpssVirReg->VPSS_TNR_CLUT51.bits.data0 = pstFmotionMapping->s16YFmotion[4];
    pstVpssVirReg->VPSS_TNR_CLUT51.bits.data1 = pstFmotionMapping->s16YFmotion[5];
    pstVpssVirReg->VPSS_TNR_CLUT51.bits.data2 = pstFmotionMapping->s16YFmotion[6];
    pstVpssVirReg->VPSS_TNR_CLUT51.bits.data3 = pstFmotionMapping->s16YFmotion[7];

    pstVpssVirReg->VPSS_TNR_CLUT52.bits.data0 = pstFmotionMapping->s16YFmotion[8];
    pstVpssVirReg->VPSS_TNR_CLUT52.bits.data1 = pstFmotionMapping->s16YFmotion[9];
    pstVpssVirReg->VPSS_TNR_CLUT52.bits.data2 = pstFmotionMapping->s16YFmotion[10];
    pstVpssVirReg->VPSS_TNR_CLUT52.bits.data3 = pstFmotionMapping->s16YFmotion[11];

    pstVpssVirReg->VPSS_TNR_CLUT53.bits.data0 = pstFmotionMapping->s16YFmotion[12];
    pstVpssVirReg->VPSS_TNR_CLUT53.bits.data1 = pstFmotionMapping->s16YFmotion[13];
    pstVpssVirReg->VPSS_TNR_CLUT53.bits.data2 = pstFmotionMapping->s16YFmotion[14];
    pstVpssVirReg->VPSS_TNR_CLUT53.bits.data3 = pstFmotionMapping->s16YFmotion[15];

    pstVpssVirReg->VPSS_TNR_CLUT54.bits.data0 = pstFmotionMapping->s16YFmotion[16];
    pstVpssVirReg->VPSS_TNR_CLUT54.bits.data1 = pstFmotionMapping->s16YFmotion[17];
    pstVpssVirReg->VPSS_TNR_CLUT54.bits.data2 = pstFmotionMapping->s16YFmotion[18];
    pstVpssVirReg->VPSS_TNR_CLUT54.bits.data3 = pstFmotionMapping->s16YFmotion[19];

    pstVpssVirReg->VPSS_TNR_CLUT55.bits.data0 = pstFmotionMapping->s16YFmotion[20];
    pstVpssVirReg->VPSS_TNR_CLUT55.bits.data1 = pstFmotionMapping->s16YFmotion[21];
    pstVpssVirReg->VPSS_TNR_CLUT55.bits.data2 = pstFmotionMapping->s16YFmotion[22];
    pstVpssVirReg->VPSS_TNR_CLUT55.bits.data3 = pstFmotionMapping->s16YFmotion[23];

    pstVpssVirReg->VPSS_TNR_CLUT56.bits.data0 = pstFmotionMapping->s16YFmotion[24];
    pstVpssVirReg->VPSS_TNR_CLUT56.bits.data1 = pstFmotionMapping->s16YFmotion[25];
    pstVpssVirReg->VPSS_TNR_CLUT56.bits.data2 = pstFmotionMapping->s16YFmotion[26];
    pstVpssVirReg->VPSS_TNR_CLUT56.bits.data3 = pstFmotionMapping->s16YFmotion[27];

    pstVpssVirReg->VPSS_TNR_CLUT57.bits.data0 = pstFmotionMapping->s16YFmotion[28];
    pstVpssVirReg->VPSS_TNR_CLUT57.bits.data1 = pstFmotionMapping->s16YFmotion[29];
    pstVpssVirReg->VPSS_TNR_CLUT57.bits.data2 = pstFmotionMapping->s16YFmotion[30];
    pstVpssVirReg->VPSS_TNR_CLUT57.bits.data3 = pstFmotionMapping->s16YFmotion[31];
    //COLOR
    pstVpssVirReg->VPSS_TNR_CLUT60.bits.data0 = pstFmotionMapping->s16CFmotion[0];
    pstVpssVirReg->VPSS_TNR_CLUT60.bits.data1 = pstFmotionMapping->s16CFmotion[1];
    pstVpssVirReg->VPSS_TNR_CLUT60.bits.data2 = pstFmotionMapping->s16CFmotion[2];
    pstVpssVirReg->VPSS_TNR_CLUT60.bits.data3 = pstFmotionMapping->s16CFmotion[3];

    pstVpssVirReg->VPSS_TNR_CLUT61.bits.data0 = pstFmotionMapping->s16CFmotion[4];
    pstVpssVirReg->VPSS_TNR_CLUT61.bits.data1 = pstFmotionMapping->s16CFmotion[5];
    pstVpssVirReg->VPSS_TNR_CLUT61.bits.data2 = pstFmotionMapping->s16CFmotion[6];
    pstVpssVirReg->VPSS_TNR_CLUT61.bits.data3 = pstFmotionMapping->s16CFmotion[7];

    pstVpssVirReg->VPSS_TNR_CLUT62.bits.data0 = pstFmotionMapping->s16CFmotion[8];
    pstVpssVirReg->VPSS_TNR_CLUT62.bits.data1 = pstFmotionMapping->s16CFmotion[9];
    pstVpssVirReg->VPSS_TNR_CLUT62.bits.data2 = pstFmotionMapping->s16CFmotion[10];
    pstVpssVirReg->VPSS_TNR_CLUT62.bits.data3 = pstFmotionMapping->s16CFmotion[11];

    pstVpssVirReg->VPSS_TNR_CLUT63.bits.data0 = pstFmotionMapping->s16CFmotion[12];
    pstVpssVirReg->VPSS_TNR_CLUT63.bits.data1 = pstFmotionMapping->s16CFmotion[13];
    pstVpssVirReg->VPSS_TNR_CLUT63.bits.data2 = pstFmotionMapping->s16CFmotion[14];
    pstVpssVirReg->VPSS_TNR_CLUT63.bits.data3 = pstFmotionMapping->s16CFmotion[15];

    pstVpssVirReg->VPSS_TNR_CLUT64.bits.data0 = pstFmotionMapping->s16CFmotion[16];
    pstVpssVirReg->VPSS_TNR_CLUT64.bits.data1 = pstFmotionMapping->s16CFmotion[17];
    pstVpssVirReg->VPSS_TNR_CLUT64.bits.data2 = pstFmotionMapping->s16CFmotion[18];
    pstVpssVirReg->VPSS_TNR_CLUT64.bits.data3 = pstFmotionMapping->s16CFmotion[19];

    pstVpssVirReg->VPSS_TNR_CLUT65.bits.data0 = pstFmotionMapping->s16CFmotion[20];
    pstVpssVirReg->VPSS_TNR_CLUT65.bits.data1 = pstFmotionMapping->s16CFmotion[21];
    pstVpssVirReg->VPSS_TNR_CLUT65.bits.data2 = pstFmotionMapping->s16CFmotion[22];
    pstVpssVirReg->VPSS_TNR_CLUT65.bits.data3 = pstFmotionMapping->s16CFmotion[23];

    pstVpssVirReg->VPSS_TNR_CLUT66.bits.data0 = pstFmotionMapping->s16CFmotion[24];
    pstVpssVirReg->VPSS_TNR_CLUT66.bits.data1 = pstFmotionMapping->s16CFmotion[25];
    pstVpssVirReg->VPSS_TNR_CLUT66.bits.data2 = pstFmotionMapping->s16CFmotion[26];
    pstVpssVirReg->VPSS_TNR_CLUT66.bits.data3 = pstFmotionMapping->s16CFmotion[27];

    pstVpssVirReg->VPSS_TNR_CLUT67.bits.data0 = pstFmotionMapping->s16CFmotion[28];
    pstVpssVirReg->VPSS_TNR_CLUT67.bits.data1 = pstFmotionMapping->s16CFmotion[29];
    pstVpssVirReg->VPSS_TNR_CLUT67.bits.data2 = pstFmotionMapping->s16CFmotion[30];
    pstVpssVirReg->VPSS_TNR_CLUT67.bits.data3 = pstFmotionMapping->s16CFmotion[31];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetTnrFmotionMapping(HI_U32 u32HandleNo, TNR_FMOTION_MAPPING_S* pstFmotionMapping)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstFmotionMapping->s16YFmotion[0] = pstVpssVirReg->VPSS_TNR_CLUT50.bits.data0;
    pstFmotionMapping->s16YFmotion[1] = pstVpssVirReg->VPSS_TNR_CLUT50.bits.data1;
    pstFmotionMapping->s16YFmotion[2] = pstVpssVirReg->VPSS_TNR_CLUT50.bits.data2;
    pstFmotionMapping->s16YFmotion[3] = pstVpssVirReg->VPSS_TNR_CLUT50.bits.data3;

    pstFmotionMapping->s16YFmotion[4] = pstVpssVirReg->VPSS_TNR_CLUT51.bits.data0;
    pstFmotionMapping->s16YFmotion[5] = pstVpssVirReg->VPSS_TNR_CLUT51.bits.data1;
    pstFmotionMapping->s16YFmotion[6] = pstVpssVirReg->VPSS_TNR_CLUT51.bits.data2;
    pstFmotionMapping->s16YFmotion[7] = pstVpssVirReg->VPSS_TNR_CLUT51.bits.data3;

    pstFmotionMapping->s16YFmotion[8] = pstVpssVirReg->VPSS_TNR_CLUT52.bits.data0;
    pstFmotionMapping->s16YFmotion[9] = pstVpssVirReg->VPSS_TNR_CLUT52.bits.data1;
    pstFmotionMapping->s16YFmotion[10] = pstVpssVirReg->VPSS_TNR_CLUT52.bits.data2;
    pstFmotionMapping->s16YFmotion[11] = pstVpssVirReg->VPSS_TNR_CLUT52.bits.data3;

    pstFmotionMapping->s16YFmotion[12] = pstVpssVirReg->VPSS_TNR_CLUT53.bits.data0;
    pstFmotionMapping->s16YFmotion[13] = pstVpssVirReg->VPSS_TNR_CLUT53.bits.data1;
    pstFmotionMapping->s16YFmotion[14] = pstVpssVirReg->VPSS_TNR_CLUT53.bits.data2;
    pstFmotionMapping->s16YFmotion[15] = pstVpssVirReg->VPSS_TNR_CLUT53.bits.data3;

    pstFmotionMapping->s16YFmotion[16] = pstVpssVirReg->VPSS_TNR_CLUT54.bits.data0;
    pstFmotionMapping->s16YFmotion[17] = pstVpssVirReg->VPSS_TNR_CLUT54.bits.data1;
    pstFmotionMapping->s16YFmotion[18] = pstVpssVirReg->VPSS_TNR_CLUT54.bits.data2;
    pstFmotionMapping->s16YFmotion[19] = pstVpssVirReg->VPSS_TNR_CLUT54.bits.data3;

    pstFmotionMapping->s16YFmotion[20] = pstVpssVirReg->VPSS_TNR_CLUT55.bits.data0;
    pstFmotionMapping->s16YFmotion[21] = pstVpssVirReg->VPSS_TNR_CLUT55.bits.data1;
    pstFmotionMapping->s16YFmotion[22] = pstVpssVirReg->VPSS_TNR_CLUT55.bits.data2;
    pstFmotionMapping->s16YFmotion[23] = pstVpssVirReg->VPSS_TNR_CLUT55.bits.data3;

    pstFmotionMapping->s16YFmotion[24] = pstVpssVirReg->VPSS_TNR_CLUT56.bits.data0;
    pstFmotionMapping->s16YFmotion[25] = pstVpssVirReg->VPSS_TNR_CLUT56.bits.data1;
    pstFmotionMapping->s16YFmotion[26] = pstVpssVirReg->VPSS_TNR_CLUT56.bits.data2;
    pstFmotionMapping->s16YFmotion[27] = pstVpssVirReg->VPSS_TNR_CLUT56.bits.data3;

    pstFmotionMapping->s16YFmotion[28] = pstVpssVirReg->VPSS_TNR_CLUT57.bits.data0;
    pstFmotionMapping->s16YFmotion[29] = pstVpssVirReg->VPSS_TNR_CLUT57.bits.data1;
    pstFmotionMapping->s16YFmotion[30] = pstVpssVirReg->VPSS_TNR_CLUT57.bits.data2;
    pstFmotionMapping->s16YFmotion[31] = pstVpssVirReg->VPSS_TNR_CLUT57.bits.data3;
    //COLOR
    pstFmotionMapping->s16CFmotion[0] = pstVpssVirReg->VPSS_TNR_CLUT60.bits.data0;
    pstFmotionMapping->s16CFmotion[1] = pstVpssVirReg->VPSS_TNR_CLUT60.bits.data1;
    pstFmotionMapping->s16CFmotion[2] = pstVpssVirReg->VPSS_TNR_CLUT60.bits.data2;
    pstFmotionMapping->s16CFmotion[3] = pstVpssVirReg->VPSS_TNR_CLUT60.bits.data3;

    pstFmotionMapping->s16CFmotion[4] = pstVpssVirReg->VPSS_TNR_CLUT61.bits.data0;
    pstFmotionMapping->s16CFmotion[5] = pstVpssVirReg->VPSS_TNR_CLUT61.bits.data1;
    pstFmotionMapping->s16CFmotion[6] = pstVpssVirReg->VPSS_TNR_CLUT61.bits.data2;
    pstFmotionMapping->s16CFmotion[7] = pstVpssVirReg->VPSS_TNR_CLUT61.bits.data3;

    pstFmotionMapping->s16CFmotion[8] = pstVpssVirReg->VPSS_TNR_CLUT62.bits.data0;
    pstFmotionMapping->s16CFmotion[9] = pstVpssVirReg->VPSS_TNR_CLUT62.bits.data1;
    pstFmotionMapping->s16CFmotion[10] = pstVpssVirReg->VPSS_TNR_CLUT62.bits.data2;
    pstFmotionMapping->s16CFmotion[11] = pstVpssVirReg->VPSS_TNR_CLUT62.bits.data3;

    pstFmotionMapping->s16CFmotion[12] = pstVpssVirReg->VPSS_TNR_CLUT63.bits.data0;
    pstFmotionMapping->s16CFmotion[13] = pstVpssVirReg->VPSS_TNR_CLUT63.bits.data1;
    pstFmotionMapping->s16CFmotion[14] = pstVpssVirReg->VPSS_TNR_CLUT63.bits.data2;
    pstFmotionMapping->s16CFmotion[15] = pstVpssVirReg->VPSS_TNR_CLUT63.bits.data3;

    pstFmotionMapping->s16CFmotion[16] = pstVpssVirReg->VPSS_TNR_CLUT64.bits.data0;
    pstFmotionMapping->s16CFmotion[17] = pstVpssVirReg->VPSS_TNR_CLUT64.bits.data1;
    pstFmotionMapping->s16CFmotion[18] = pstVpssVirReg->VPSS_TNR_CLUT64.bits.data2;
    pstFmotionMapping->s16CFmotion[19] = pstVpssVirReg->VPSS_TNR_CLUT64.bits.data3;

    pstFmotionMapping->s16CFmotion[20] = pstVpssVirReg->VPSS_TNR_CLUT65.bits.data0;
    pstFmotionMapping->s16CFmotion[21] = pstVpssVirReg->VPSS_TNR_CLUT65.bits.data1;
    pstFmotionMapping->s16CFmotion[22] = pstVpssVirReg->VPSS_TNR_CLUT65.bits.data2;
    pstFmotionMapping->s16CFmotion[23] = pstVpssVirReg->VPSS_TNR_CLUT65.bits.data3;

    pstFmotionMapping->s16CFmotion[24] = pstVpssVirReg->VPSS_TNR_CLUT66.bits.data0;
    pstFmotionMapping->s16CFmotion[25] = pstVpssVirReg->VPSS_TNR_CLUT66.bits.data1;
    pstFmotionMapping->s16CFmotion[26] = pstVpssVirReg->VPSS_TNR_CLUT66.bits.data2;
    pstFmotionMapping->s16CFmotion[27] = pstVpssVirReg->VPSS_TNR_CLUT66.bits.data3;

    pstFmotionMapping->s16CFmotion[28] = pstVpssVirReg->VPSS_TNR_CLUT67.bits.data0;
    pstFmotionMapping->s16CFmotion[29] = pstVpssVirReg->VPSS_TNR_CLUT67.bits.data1;
    pstFmotionMapping->s16CFmotion[30] = pstVpssVirReg->VPSS_TNR_CLUT67.bits.data2;
    pstFmotionMapping->s16CFmotion[31] = pstVpssVirReg->VPSS_TNR_CLUT67.bits.data3;

    return HI_SUCCESS;
}


