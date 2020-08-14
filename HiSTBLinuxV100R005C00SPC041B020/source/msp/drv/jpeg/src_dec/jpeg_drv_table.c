/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_table.c
Version            : Initial Draft
Author            :
Created            : 2015/02/02
Description        : CNcomment: 设置量化表和哈夫曼表信息 CNend\n
Function List     :


History           :
Date                Author                Modification
2015/02/02            y00181162              Created file
******************************************************************************/

/*********************************add include here******************************/
#ifndef HI_BUILD_IN_MINI_BOOT
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "hi_jpeg_reg.h"
#include "jpeg_drv_parse.h"
#include "jpeg_drv_error.h"
#include "hi_gfx_sys_k.h"

#ifdef CONFIG_JPEG_OMX_FUNCTION

/***************************** Macro Definition ******************************/

#define  LU_TABL             0
#define  CU_TABL             1
#define  MAX_TAB_NUM         2
#define CALC_SWAP(a,b)       do{ a=a+b; b=a-b; a=a-b; } while(0)

/******************** to see which include file we want to use***************/



/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

static HI_U8 gs_u8DefaultACTable[] = {
            /**< for luma AC *//** CNcomment:亮度AC */ 0x10,
            0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D,
            0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
            0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
            0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
            0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
            0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
            0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
            0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
            0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
            0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
            0xF9, 0xFA,
            /**< for chrom AC *//** CNcomment:色度AC */ 0x11,
            0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
            0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
            0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
            0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
            0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
            0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
            0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
            0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
            0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
            0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
            0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
            0xF9, 0xFA
        };

static HI_U8 gs_u8DefaultDCTable[] = {
            /**< for luma DC *//** CNcomment:亮度DC */ 0x00,
            0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
            /**< for chrom DC *//** CNcomment:色度DC */ 0x01,
            0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B
        };


static JPEG_HUFF_TBL gs_huff_ptr[MAX_TAB_NUM] = { { {0}, {0}, 0} };

static HI_U32 gs_huffcode[MAX_TAB_NUM][256]  = {{0}};

static HI_U32 gs_hac_symbol_tab[256]          = {0};

/******************************* API forward declarations *******************/

static HI_S32  JPEG_SetDcTable(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo);
static HI_VOID JPEG_SetAcTable(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo);

/******************************* API realization *****************************/

HI_VOID DRV_JPEG_SetDqt(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo)
{

     HI_U16  *QCr    = NULL;
     HI_U16  *QCb    = NULL;
     HI_U16  *QY     = NULL;
     HI_S32 s32Cnt   = 0;

     HI_S32 Q[MAX_DCT_SIZE] = {0};
     HI_S32 quant_tbl_no[MAX_NUM_QUANT_TBLS] = {0};

     HI_U16  QT_TABLE_Cy[MAX_DCT_SIZE] = {
                  16, 11, 10, 16, 24, 40, 51, 61,
                  12, 12, 14, 19, 26, 58, 60, 55,
                  14, 13, 16, 24, 40, 57, 69, 56,
                  14, 17, 22, 29, 51, 87, 80, 62,
                  18, 22, 37, 56, 68, 109,103,77,
                  24, 35, 55, 64, 81, 104,113,92,
                  49, 64, 78, 87, 103,121,120,101,
                  72, 92, 95, 98, 112,100,103,99
             };

     HI_U16  QT_TABLE_CbCr[MAX_DCT_SIZE] = {
                  17, 18, 24, 47, 99, 99, 99, 99,
                  18, 21, 26, 66, 99, 99, 99, 99,
                  24, 26, 56, 99, 99, 99, 99, 99,
                  47, 66, 99, 99, 99, 99, 99, 99,
                  99, 99, 99, 99, 99, 99, 99, 99,
                  99, 99, 99, 99, 99, 99, 99, 99,
                  99, 99, 99, 99, 99, 99, 99, 99,
                  99, 99, 99, 99, 99, 99, 99, 99
             };

     if (HI_FALSE == pstImgInfo->QuantTbl[0].bHasQuantTbl){
         QY    = (unsigned short *)QT_TABLE_Cy;
         QCb = (unsigned short *)QT_TABLE_CbCr;
         QCr = (unsigned short *)QT_TABLE_CbCr;
     }else{
         for (s32Cnt = 0; s32Cnt < pstImgInfo->s32NumComponent; s32Cnt++){
             quant_tbl_no[s32Cnt] = pstImgInfo->stComponentInfo[s32Cnt].s32QuantTblNo;
         }
         QY  = pstImgInfo->QuantTbl[quant_tbl_no[0]].quantval;
         QCb = pstImgInfo->QuantTbl[quant_tbl_no[1]].bHasQuantTbl == HI_FALSE ? QY  : pstImgInfo->QuantTbl[quant_tbl_no[1]].quantval;
         QCr = pstImgInfo->QuantTbl[quant_tbl_no[2]].bHasQuantTbl == HI_FALSE ? QCb : pstImgInfo->QuantTbl[quant_tbl_no[2]].quantval;
     }

     for (s32Cnt = 0; s32Cnt < MAX_DCT_SIZE; s32Cnt++){
         Q[s32Cnt] = QY[s32Cnt] + (QCb[s32Cnt]<<8) + (QCr[s32Cnt]<<16);
     }

     jpeg_reg_writebuf((HI_VOID *)Q, JPGD_REG_QUANT, sizeof(Q));

     return;
}


HI_S32 DRV_JPEG_SetDht(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_GFX_Memset(&gs_huffcode[0][0],0,sizeof(gs_huffcode));
    HI_GFX_Memset(gs_huff_ptr,0,sizeof(gs_huff_ptr));
    HI_GFX_Memset(gs_hac_symbol_tab,0,sizeof(gs_hac_symbol_tab));

    s32Ret = JPEG_SetDcTable(pstOutMsg,pstImgInfo);
    if (HI_SUCCESS != s32Ret){
        return HI_FAILURE;
    }

    JPEG_SetAcTable(pstOutMsg,pstImgInfo);

    return HI_SUCCESS;
}


static HI_U32 jpeg_table_dechuff(const JPEG_HUFF_TBL *huff_tbl, unsigned int bit[256])
{

     HI_U32 idx     = 0;
     HI_U32 cnt     = 0;
     HI_U32 loc     = 0;
     HI_U32 value   = 0;
     HI_U32 max_idx = 0;

     HI_GFX_Memset(bit,0,sizeof(bit[0]));

     for (idx = 1; idx < 17; idx++){
           if (0 != huff_tbl->bits[idx]){
                 max_idx = idx;
                 for (cnt = huff_tbl->bits[max_idx]; cnt>0; cnt--){
                     bit[loc] = value;
                     loc++;
                     value++;
                 }
           }
           value<<=1;
     }

     return max_idx;
}



static HI_S32 JPEG_SetDcTable(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo)
{
     HI_U32 cnt          = 0;
     HI_U32 u32Index   = 0;
     HI_U32 sum_syms    =0;
     HI_U32 syms       = 0;
     HI_U32 temp       = 0;
     HI_U32 index1      = 0;

     HI_U32 dc_hufflen[MAX_TAB_NUM][12] = {{0}};
     HI_U32 dc_sym[MAX_TAB_NUM][12]        = {{0}};
     HI_U32 max_idx[MAX_TAB_NUM]         = {0};
     HI_U32 hdc_tab[12]                 = {0};

     HI_U8 *pTmp  = (HI_U8*)gs_u8DefaultDCTable;

     if ((HI_FALSE == pstImgInfo->DcHuffTbl[0].bHasHuffTbl) || (HI_TRUE == pstImgInfo->DcHuffTbl[2].bHasHuffTbl)){
         pTmp+=1;
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL].bits[1]), pTmp, 16);
         pTmp   += 16;
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL].huffval[0]), pTmp, 12);
         pTmp   += 12;
         pTmp   += 1;
         HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL].bits[1]), pTmp, 16);
         pTmp   += 16;
         HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL].huffval[0]), pTmp, 12);
         pTmp   += 12;
     }else{
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL]), &pstImgInfo->DcHuffTbl[0], sizeof(gs_huff_ptr[LU_TABL]));
         if(HI_FALSE == pstImgInfo->DcHuffTbl[1].bHasHuffTbl){
             HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL]),&pstImgInfo->DcHuffTbl[0],sizeof(gs_huff_ptr[LU_TABL]));
         }else{
             HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL]),&pstImgInfo->DcHuffTbl[1],sizeof(gs_huff_ptr[LU_TABL]));
         }
     }
     max_idx[LU_TABL] = jpeg_table_dechuff(&gs_huff_ptr[LU_TABL],gs_huffcode[LU_TABL]);
     max_idx[CU_TABL] = jpeg_table_dechuff(&gs_huff_ptr[CU_TABL],gs_huffcode[CU_TABL]);

     for (cnt = 0; cnt < MAX_TAB_NUM; cnt++){
         temp     = 0;
         sum_syms = 0;
         for (u32Index = 0; u32Index < max_idx[cnt]; u32Index++){
             syms = gs_huff_ptr[cnt].bits[u32Index+1];
             if (syms){
                 sum_syms +=syms;
                 while(syms--){
                     dc_hufflen[cnt][temp] = u32Index + 1;
                     dc_sym[cnt][temp] = gs_huff_ptr[cnt].huffval[temp];
                     temp++;
                 }
             }
         }
         for (u32Index = 0; u32Index < sum_syms; u32Index++){
             for (index1=u32Index+1;index1<sum_syms;index1++){
                 if (dc_sym[cnt][u32Index]>dc_sym[cnt][index1]){
                     CALC_SWAP(dc_sym[cnt][u32Index],dc_sym[cnt][index1]);
                     CALC_SWAP(dc_hufflen[cnt][u32Index],dc_hufflen[cnt][index1]);
                     CALC_SWAP(gs_huffcode[cnt][u32Index],gs_huffcode[cnt][index1]);
                 }
             }
         }
         if (0 == cnt){
             for (u32Index = 0;u32Index < sum_syms; u32Index++){

                 if (dc_sym[0][u32Index] >= 12){
                     return HI_FAILURE;
                 }
                 *(hdc_tab + dc_sym[0][u32Index]) = \
                           ( (*(hdc_tab + dc_sym[0][u32Index])) & 0xfffff000 ) \
                         | ( (dc_hufflen[0][u32Index] & 0xf)<<8 ) \
                         | ( (gs_huffcode[0][u32Index] & 0xff) );
             }
         }else{
             for (u32Index = 0;u32Index < sum_syms; u32Index++){
                     if (dc_sym[cnt][u32Index] >= 12){
                         return HI_FAILURE;
                     }
                     *(hdc_tab + dc_sym[cnt][u32Index]) = \
                           ( (*(hdc_tab + dc_sym[1][u32Index])) & 0xff000fff ) \
                         | ( (dc_hufflen[1][u32Index] & 0xf)<<20 ) \
                         | ( (gs_huffcode[1][u32Index] & 0xff)<<12 );
             }
        }
     }

     jpeg_reg_writebuf(hdc_tab,JPGD_REG_HDCTABLE,48);

     return HI_SUCCESS;

}


static HI_VOID JPEG_SetAcTable(HI_DRV_JPEG_OUTMSG_S *pstOutMsg,IMAG_INFO_S *pstImgInfo)
{
     HI_U32 cnt          = 0;
     HI_U32 u32Index   = 0;
     HI_U32 pre_index  = 0;
     HI_U32 sum_syms    =0;
     HI_U32 syms       = 0;

     HI_U32 min_tab[MAX_TAB_NUM][16]    = {{0}};
     HI_U32 base_tab[MAX_TAB_NUM][16]   = {{0}};
     HI_U32 max_idx[MAX_TAB_NUM]         = {0};
     HI_U32 hac_min_tab[8]              = {0};
     HI_U32 hac_base_tab[8]                = {0};


     HI_U8 *pTmp  = (HI_U8*)gs_u8DefaultACTable;

     if ((HI_FALSE == pstImgInfo->AcHuffTbl[0].bHasHuffTbl) || (HI_TRUE == pstImgInfo->AcHuffTbl[2].bHasHuffTbl)){
         pTmp+=1;
         HI_GFX_Memset(&(gs_huff_ptr[LU_TABL].bits[0]),0,17);
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL].bits[1]), pTmp, 16);
         pTmp   += 16;
         HI_GFX_Memset(&(gs_huff_ptr[LU_TABL].huffval[0]),0,256);
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL].huffval[0]), pTmp, 162);
         pTmp   += 162;

         pTmp+=1;
         HI_GFX_Memset(&(gs_huff_ptr[CU_TABL].bits[0]),0,17);
         HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL].bits[1]), pTmp, 16);
         pTmp   += 16;
         HI_GFX_Memset(&(gs_huff_ptr[CU_TABL].huffval[0]),0,256);
         HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL].huffval[0]), pTmp, 162);
     }else{
         HI_GFX_Memset(&(gs_huff_ptr[LU_TABL].bits[0]),0,17);
         HI_GFX_Memset(&(gs_huff_ptr[LU_TABL].huffval[0]),0,256);
         HI_GFX_Memset(&(gs_huff_ptr[CU_TABL].bits[0]),0,17);
         HI_GFX_Memset(&(gs_huff_ptr[CU_TABL].huffval[0]),0,256);
         HI_GFX_Memcpy(&(gs_huff_ptr[LU_TABL]), &pstImgInfo->AcHuffTbl[0], sizeof(gs_huff_ptr[LU_TABL]));

         if (HI_FALSE == &pstImgInfo->AcHuffTbl[1].bHasHuffTbl){
             HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL]),&pstImgInfo->AcHuffTbl[0],sizeof(gs_huff_ptr[LU_TABL]));
         }else{
             HI_GFX_Memcpy(&(gs_huff_ptr[CU_TABL]),&pstImgInfo->AcHuffTbl[1],sizeof(gs_huff_ptr[LU_TABL]));
         }
     }
     max_idx[LU_TABL] = jpeg_table_dechuff(&gs_huff_ptr[0],gs_huffcode[LU_TABL]);
     max_idx[CU_TABL] = jpeg_table_dechuff(&gs_huff_ptr[1],gs_huffcode[CU_TABL]);

     for (cnt = 0; cnt < MAX_TAB_NUM; cnt++){
         sum_syms=0;
         for (u32Index = 0; u32Index < 16; u32Index++){
             syms = gs_huff_ptr[cnt].bits[u32Index+1];
             pre_index = u32Index?(u32Index-1):(0);

             if (u32Index < max_idx[cnt]){

                 min_tab[cnt][u32Index] = (min_tab[cnt][pre_index]+gs_huff_ptr[cnt].bits[u32Index])<<1;
                 if (syms){
                     base_tab[cnt][u32Index]=sum_syms-min_tab[cnt][u32Index];
                 }
                 sum_syms += gs_huff_ptr[cnt].bits[u32Index+1];
             }else{
                 min_tab[cnt][u32Index]=~0;
             }

         }

         if (0 == cnt){
             for (u32Index=0;u32Index<sum_syms;u32Index++){
                 gs_hac_symbol_tab[u32Index] |= ( (unsigned int)(gs_huff_ptr[0].huffval[u32Index]) );
             }
         }else{
             for (u32Index=0;u32Index<sum_syms;u32Index++){
                 gs_hac_symbol_tab[u32Index] |= ( (unsigned int)(gs_huff_ptr[1].huffval[u32Index])<<8 );
             }
         }
     }

     for (u32Index=0;u32Index<8;u32Index++){

         hac_min_tab[u32Index] =   ( (min_tab[CU_TABL][2*u32Index+1] & 0xff)<<24 )  \
                                 | ( (min_tab[CU_TABL][2*u32Index] & 0xff)<<16    )   \
                                 | ( (min_tab[LU_TABL][2*u32Index+1] & 0xff)<<8    )   \
                                 | (min_tab[LU_TABL][2*u32Index] & 0xff);

         hac_base_tab[u32Index] =   ((base_tab[CU_TABL][2*u32Index+1] & 0xff)<<24 ) \
                                  | ((base_tab[CU_TABL][2*u32Index] & 0xff)<<16     )  \
                                  | ((base_tab[LU_TABL][2*u32Index+1] & 0xff)<<8  ) \
                                  | (base_tab[LU_TABL][2*u32Index] & 0xff);

     }

     jpeg_reg_writebuf(hac_min_tab,        JPGD_REG_HACMINTABLE,  32);
     jpeg_reg_writebuf(hac_base_tab,       JPGD_REG_HACBASETABLE, 32);
     jpeg_reg_writebuf(gs_hac_symbol_tab,  JPGD_REG_HACSYMTABLE,  0x2c0);

     return;
}
#endif
