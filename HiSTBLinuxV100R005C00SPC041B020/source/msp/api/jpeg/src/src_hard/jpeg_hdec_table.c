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
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      : get table
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/

/***************************** add include here*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpeglib.h"
#include "jerror.h"

#include "jpeg_hdec_adp.h"
#include "jpeg_hdec_api.h"
#include "hi_gfx_sys.h"

/***************************** Macro Definition ********************************/
#define  LU                 0
#define  CH                 1
#define  MAX_TAB            2

#define CAI_JPEG_SWAP(a,b)  do{ a=a+b; b=a-b; a=a-b; } while(0)

/***************************** Structure Definition ****************************/

/***************************** Global Variable declaration *********************/

/***************************** API forward declarations ************************/
static inline HI_VOID JPEG_HDEC_GetQuantTable(const struct jpeg_decompress_struct *cinfo);

static HI_VOID JPEG_HDEC_GetHuffmanTable    (const struct jpeg_decompress_struct *cinfo);
static HI_VOID JPEG_HDEC_GetHuffmanDcTable  (const struct jpeg_decompress_struct *cinfo);
static HI_VOID JPEG_HDEC_GetHuffmanAcTable  (const struct jpeg_decompress_struct *cinfo);
static inline HI_U32 JPEG_HDEC_HuffmanDec   (const JHUFF_TBL *huff_tbl, unsigned int bit[256]);


/******************************* API realization *****************************/

/*****************************************************************************
* func          : JPEG_HDEC_GetTable
* description   : get table, include quant table and huffman table
                  CNcomment: 获取量化表和哈夫曼表   CNend\n
* param[in]     : cinfo       CNcomment: 解码对象   CNend\n
* retval        : HI_TRUE     CNcomment: support    CNend\n
* retval        : HI_FALSE    CNcomment: unsupport  CNend\n
* others:       : NA
*****************************************************************************/
HI_BOOL JPEG_HDEC_GetTable(j_decompress_ptr cinfo)
{
    JPEG_HDEC_GetQuantTable(cinfo);
    JPEG_HDEC_GetHuffmanTable(cinfo);
    return HI_TRUE;
}


static HI_VOID JPEG_HDEC_GetQuantTable(const struct jpeg_decompress_struct *cinfo)
{

     HI_U16  *QCr = NULL;
     HI_U16  *QCb = NULL;
     HI_U16  *QY  = NULL;
     HI_S32 s32Cnt = 0;
     HI_S32 quant_tbl_no[NUM_QUANT_TBLS] = {0};

     jpeg_component_info  *compptr = NULL;
     JPEG_HDEC_HANDLE_S_PTR  pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     HI_U16 T81ProtocolLuQuantTable[DCTSIZE2] = {
                                       16, 11, 10, 16, 24, 40, 51, 61,
                                       12, 12, 14, 19, 26, 58, 60, 55,
                                       14, 13, 16, 24, 40, 57, 69, 56,
                                       14, 17, 22, 29, 51, 87, 80, 62,
                                       18, 22, 37, 56, 68, 109,103,77,
                                       24, 35, 55, 64, 81, 104,113,92,
                                       49, 64, 78, 87, 103,121,120,101,
                                       72, 92, 95, 98, 112,100,103,99
                                      };
     HI_U16 T81ProtocolChQuantTable[DCTSIZE2] = {
                                         17, 18, 24, 47, 99, 99, 99, 99,
                                         18, 21, 26, 66, 99, 99, 99, 99,
                                         24, 26, 56, 99, 99, 99, 99, 99,
                                         47, 66, 99, 99, 99, 99, 99, 99,
                                         99, 99, 99, 99, 99, 99, 99, 99,
                                         99, 99, 99, 99, 99, 99, 99, 99,
                                         99, 99, 99, 99, 99, 99, 99, 99,
                                         99, 99, 99, 99, 99, 99, 99, 99
                                        };

     if (NULL == cinfo->quant_tbl_ptrs[0]){
         QY  = (unsigned short *)T81ProtocolLuQuantTable;
         QCb = (unsigned short *)T81ProtocolChQuantTable;
         QCr = (unsigned short *)T81ProtocolChQuantTable;
     }else{
         for (s32Cnt = 0, compptr = cinfo->comp_info; s32Cnt < cinfo->num_components; s32Cnt++, compptr++){
              quant_tbl_no[s32Cnt] = compptr->quant_tbl_no;
         }

         if (NULL == cinfo->quant_tbl_ptrs[quant_tbl_no[0]]){
             ERREXIT1(cinfo, JERR_NO_QUANT_TABLE, quant_tbl_no[0]);
         }
         QY  = cinfo->quant_tbl_ptrs[quant_tbl_no[0]]->quantval;
         QCb = (cinfo->quant_tbl_ptrs[quant_tbl_no[1]] == NULL) ? (QY)  : (cinfo->quant_tbl_ptrs[quant_tbl_no[1]]->quantval);
         QCr = (cinfo->quant_tbl_ptrs[quant_tbl_no[2]] == NULL) ? (QCb) : (cinfo->quant_tbl_ptrs[quant_tbl_no[2]]->quantval);
     }

     for (s32Cnt = 0; s32Cnt < DCTSIZE2; s32Cnt++){
         pJpegHandle->s32QuantTab[s32Cnt] = QY[s32Cnt] + (QCb[s32Cnt] << 8) + (QCr[s32Cnt] << 16);
     }

     return;
}


static HI_VOID JPEG_HDEC_GetHuffmanTable(const struct jpeg_decompress_struct *cinfo)
{
    JPEG_HDEC_GetHuffmanDcTable(cinfo);
    JPEG_HDEC_GetHuffmanAcTable(cinfo);
    return;
}

HI_VOID JPEG_HDEC_GetHuffmanDcTable(const struct jpeg_decompress_struct *cinfo)
{

      HI_U32 cnt        = 0;
      HI_U32 u32Index   = 0;
      HI_U32 sum_syms   =0;
      HI_U32 syms       = 0;
      HI_U32 temp       = 0;
      HI_U32 index1     = 0;

      HI_U32 huffcode[MAX_TAB][256]  = {{0}};
      HI_U32 dc_hufflen[MAX_TAB][12] = {{0}};
      HI_U32 dc_sym[MAX_TAB][12]     = {{0}};
      HI_U32 max_idx[MAX_TAB]        = {0};

      JHUFF_TBL DcHuffmanTablePtr[MAX_TAB] = { { {0}, {0}, 0} };
      JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

      const HI_U8 u8DefaultDCTable[] = {
          /** luma DC */0x00,
          0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
          /** chrom DC */0x01,
          0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B
      };
      HI_U8 *pDcDefaultTable = (HI_U8*)u8DefaultDCTable;

      if ((NULL == cinfo->dc_huff_tbl_ptrs[0]) || (NULL != cinfo->dc_huff_tbl_ptrs[2])){
          pDcDefaultTable += 1;
          HI_GFX_Memcpy(&(DcHuffmanTablePtr[LU].bits[1]), pDcDefaultTable, 16);
          pDcDefaultTable += 16;
          HI_GFX_Memcpy(&(DcHuffmanTablePtr[LU].huffval[0]), pDcDefaultTable, 12);
          pDcDefaultTable += 12;

          pDcDefaultTable += 1;
          HI_GFX_Memcpy(&(DcHuffmanTablePtr[CH].bits[1]), pDcDefaultTable, 16);
          pDcDefaultTable += 16;
          HI_GFX_Memcpy(&(DcHuffmanTablePtr[CH].huffval[0]), pDcDefaultTable, 12);
          pDcDefaultTable += 12;
      }else {
          HI_GFX_Memcpy(&(DcHuffmanTablePtr[LU]), cinfo->dc_huff_tbl_ptrs[0], sizeof(DcHuffmanTablePtr[LU]));
          if (NULL == cinfo->dc_huff_tbl_ptrs[1]){
              HI_GFX_Memcpy(&(DcHuffmanTablePtr[CH]),cinfo->dc_huff_tbl_ptrs[0],sizeof(DcHuffmanTablePtr[LU]));
          }else {
              HI_GFX_Memcpy(&(DcHuffmanTablePtr[CH]),cinfo->dc_huff_tbl_ptrs[1],sizeof(DcHuffmanTablePtr[LU]));
          }
      }

      max_idx[LU] = JPEG_HDEC_HuffmanDec(&DcHuffmanTablePtr[LU], huffcode[LU]);
      max_idx[CH] = JPEG_HDEC_HuffmanDec(&DcHuffmanTablePtr[CH], huffcode[CH]);

      for (cnt = 0; cnt < MAX_TAB; cnt++){

          temp     = 0;
          sum_syms = 0;

          for (u32Index = 0; u32Index < max_idx[cnt]; u32Index++){
              syms = DcHuffmanTablePtr[cnt].bits[u32Index+1];
              if (syms){
                  sum_syms +=syms;
                  while (syms--){
                      dc_hufflen[cnt][temp] = u32Index + 1;
                      dc_sym[cnt][temp]     = DcHuffmanTablePtr[cnt].huffval[temp];
                      temp++;
                  }
              }
          }

          for (u32Index = 0; u32Index < sum_syms; u32Index++){
              for (index1=u32Index+1;index1<sum_syms;index1++){
                  if (dc_sym[cnt][u32Index]>dc_sym[cnt][index1]){
                      CAI_JPEG_SWAP(dc_sym[cnt][u32Index],dc_sym[cnt][index1]);
                      CAI_JPEG_SWAP(dc_hufflen[cnt][u32Index],dc_hufflen[cnt][index1]);
                      CAI_JPEG_SWAP(huffcode[cnt][u32Index],huffcode[cnt][index1]);
                  }
              }
          }

          if (0 == cnt){
              for (u32Index = 0;u32Index < sum_syms; u32Index++){
                  if (dc_sym[0][u32Index] >= 12){
                      ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
                  }
                  *(pJpegHandle->u32HuffDcTab + dc_sym[0][u32Index]) =
                                      ( (*(pJpegHandle->u32HuffDcTab + dc_sym[0][u32Index])) & 0xfffff000 )
                                    | ( (dc_hufflen[0][u32Index] & 0xf)<<8 ) \
                                    | ( (huffcode[0][u32Index] & 0xff) );
              }

        }else{
              for (u32Index = 0;u32Index < sum_syms; u32Index++){
                  if (dc_sym[cnt][u32Index] >= 12){
                      ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
                  }
                  *(pJpegHandle->u32HuffDcTab + dc_sym[cnt][u32Index]) =
                                        ( (*(pJpegHandle->u32HuffDcTab + dc_sym[1][u32Index])) & 0xff000fff )
                                      | ( (dc_hufflen[1][u32Index] & 0xf)<<20 ) \
                                      | ( (huffcode[1][u32Index] & 0xff)<<12 );
              }

        }

      }

      return;
}



static HI_VOID JPEG_HDEC_GetHuffmanAcTable(const struct jpeg_decompress_struct *cinfo)
{
     HI_U32 cnt          = 0;
     HI_U32 u32Index   = 0;
     HI_U32 pre_index  = 0;
     HI_U32 sum_syms    =0;
     HI_U32 syms       = 0;

     HI_U32 huffcode[MAX_TAB][256]  = {{0}};
     HI_U32 min_tab[MAX_TAB][16]    = {{0}};
     HI_U32 base_tab[MAX_TAB][16]   = {{0}};
     HI_U32 max_idx[MAX_TAB]        = {0};

     JHUFF_TBL AcHuffmanTablePtr[MAX_TAB] = { { {0}, {0}, 0} };
     JPEG_HDEC_HANDLE_S_PTR    pJpegHandle = (JPEG_HDEC_HANDLE_S_PTR)(cinfo->client_data);

     const HI_U8 u8DefaultACTable[] = {
         /** luma AC */0x10,
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
         /** chrom AC */0x11,
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
     HI_U8 *pAcDefaultTable  = (HI_U8*)u8DefaultACTable;


     if ((NULL == cinfo->ac_huff_tbl_ptrs[0]) || (NULL != cinfo->ac_huff_tbl_ptrs[2])){
         pAcDefaultTable += 1;
         HI_GFX_Memset(&(AcHuffmanTablePtr[LU].bits[0]),0,17);
         HI_GFX_Memcpy(&(AcHuffmanTablePtr[LU].bits[1]), pAcDefaultTable, 16);
         pAcDefaultTable += 16;
         HI_GFX_Memset(&(AcHuffmanTablePtr[LU].huffval[0]),0,256);
         HI_GFX_Memcpy(&(AcHuffmanTablePtr[LU].huffval[0]), pAcDefaultTable, 162);
         pAcDefaultTable += 162;

         pAcDefaultTable += 1;
         HI_GFX_Memset(&(AcHuffmanTablePtr[CH].bits[0]),0,17);
         HI_GFX_Memcpy(&(AcHuffmanTablePtr[CH].bits[1]), pAcDefaultTable, 16);
         pAcDefaultTable += 16;
         HI_GFX_Memset(&(AcHuffmanTablePtr[CH].huffval[0]),0,256);
         HI_GFX_Memcpy(&(AcHuffmanTablePtr[CH].huffval[0]), pAcDefaultTable, 162);
     }else {
         HI_GFX_Memset(&(AcHuffmanTablePtr[LU].bits[0]),0,17);
         HI_GFX_Memset(&(AcHuffmanTablePtr[LU].huffval[0]),0,256);
         HI_GFX_Memset(&(AcHuffmanTablePtr[CH].bits[0]),0,17);
         HI_GFX_Memset(&(AcHuffmanTablePtr[CH].huffval[0]),0,256);
         HI_GFX_Memcpy(&(AcHuffmanTablePtr[LU]), cinfo->ac_huff_tbl_ptrs[0], sizeof(AcHuffmanTablePtr[LU]));

         if (NULL == cinfo->ac_huff_tbl_ptrs[1]){
             HI_GFX_Memcpy(&(AcHuffmanTablePtr[CH]),cinfo->ac_huff_tbl_ptrs[0],sizeof(AcHuffmanTablePtr[LU]));
         }else {
             HI_GFX_Memcpy(&(AcHuffmanTablePtr[CH]),cinfo->ac_huff_tbl_ptrs[1],sizeof(AcHuffmanTablePtr[LU]));
         }
     }
     max_idx[LU] = JPEG_HDEC_HuffmanDec(&AcHuffmanTablePtr[0],huffcode[LU]);
     max_idx[CH] = JPEG_HDEC_HuffmanDec(&AcHuffmanTablePtr[1],huffcode[CH]);

     for (cnt = 0; cnt < MAX_TAB; cnt++){
         sum_syms = 0;
         for (u32Index = 0; u32Index < 16; u32Index++){
             syms = AcHuffmanTablePtr[cnt].bits[u32Index+1];
             pre_index = u32Index?(u32Index-1):(0);

             if (u32Index < max_idx[cnt]){
                 min_tab[cnt][u32Index] = (min_tab[cnt][pre_index]+AcHuffmanTablePtr[cnt].bits[u32Index])<<1;
                 if (syms){
                     base_tab[cnt][u32Index]=sum_syms-min_tab[cnt][u32Index];
                 }
                 sum_syms += AcHuffmanTablePtr[cnt].bits[u32Index+1];
             }else {
                 min_tab[cnt][u32Index]=~0;
             }
         }

         if (0 == cnt){
             for (u32Index = 0; u32Index < sum_syms; u32Index++){
                 pJpegHandle->u32HufAcSymbolTab[u32Index] |= ( (unsigned int)(AcHuffmanTablePtr[0].huffval[u32Index]));
             }
         }else {
             for (u32Index = 0; u32Index < sum_syms; u32Index++){
                 pJpegHandle->u32HufAcSymbolTab[u32Index] |= ( (unsigned int)(AcHuffmanTablePtr[1].huffval[u32Index]) << 8);
             }
         }
     }

     for (u32Index = 0; u32Index < 8;u32Index++){
         pJpegHandle->u32HufAcMinTab[u32Index] =   ( ( min_tab[CH][2 * u32Index + 1] & 0xff) << 24)
                                                 | ( ( min_tab[CH][2 * u32Index] & 0xff) << 16)
                                                 | ( ( min_tab[LU][2 * u32Index + 1] & 0xff) << 8)
                                                 | ( ( min_tab[LU][2 * u32Index] & 0xff));

         pJpegHandle->u32HufAcBaseTab[u32Index] =  ( ( base_tab[CH][2 * u32Index + 1] & 0xff) << 24)
                                                 | ( ( base_tab[CH][2 * u32Index] & 0xff) << 16)
                                                 | ( ( base_tab[LU][2 * u32Index + 1] & 0xff) << 8)
                                                 | ( ( base_tab[LU][2 * u32Index] & 0xff));
     }

}


static inline HI_U32 JPEG_HDEC_HuffmanDec(const JHUFF_TBL *huff_tbl, unsigned int bit[256])
{
      HI_U32 idx     = 0;
      HI_U32 cnt     = 0;
      HI_U32 loc     = 0;
      HI_U32 value   = 0;
      HI_U32 max_idx = 0;

      HI_GFX_Memset(bit,0,sizeof(bit[0]));

      for (idx = 1; idx < 17; idx++){
          if (0 == huff_tbl->bits[idx]){
             value <<= 1;
             continue;
          }

          max_idx = idx;
          for (cnt = huff_tbl->bits[max_idx]; cnt > 0; cnt--){
              bit[loc] = value;
              loc++;
              value++;
          }
          value <<= 1;
      }

      return max_idx;
}
