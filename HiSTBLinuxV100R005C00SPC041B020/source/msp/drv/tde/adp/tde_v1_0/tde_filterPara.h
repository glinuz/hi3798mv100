/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : tde_filterpara.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                                 Created file        
******************************************************************************/
#ifndef  __TDE_FILTERPARA_H__
#define  __TDE_FILTERPARA_H__

#ifdef __cplusplus
      #if __cplusplus
extern "C"
{
      #endif
#endif /* __cplusplus */


#define TDE_PARA_TABLE_SIZE 32
#define TDE_PARA_TABLE_ORG_SIZE 40
#define TDE_PARA_TABLE_NUM  12

#define TDE_PARA_HTABLE_ORG_SIZE 33 * 4
#define TDE_PARA_HTABLE_SIZE 160 	/* use 160 bytes,for that argument table by cofigure giving to hardware,needing snap to 16 bytes */
#define TDE_PARA_HTABLE_NUM  6

#define TDE_PARA_VTABLE_ORG_SIZE 40
#define TDE_PARA_VTABLE_SIZE TDE_PARA_TABLE_SIZE
#define TDE_PARA_VTABLE_NUM  TDE_PARA_TABLE_NUM

HI_U8 OrgVfCoef[TDE_PARA_VTABLE_ORG_SIZE * TDE_PARA_VTABLE_NUM] = 
{
/* zoom ratio >1.1 */
0x00,  0x00,    0x40,  0x00, 0x00, 
0x00,  0xfe,    0x3d,  0x05, 0x00, 
0x00,  0xfd,    0x36,  0x0d, 0x00, 
0x00,  0xfd,    0x2d,  0x17, 0xff, 
0x00,  0xfe,    0x21,  0x23, 0xfe, 
0x00,  0xff,    0x16,  0x2e, 0xfd, 
0x00,  0x00,    0x0c,  0x37, 0xfd, 
0x00,  0x00,    0x04,  0x3e, 0xfe, 
/* zoom ratio in (1, 1.1] */
0x00,  0x00,    0x40,  0x00, 0x00, 
0x04,  0xfa,    0x3c,  0x09, 0xfd, 
0x06,  0xf5,    0x39,  0x13, 0xf9, 
0x08,  0xf3,    0x31,  0x1f, 0xf5, 
0x08,  0xf3,    0x28,  0x2a, 0xf3, 
0x07,  0xf5,    0x1d,  0x34, 0xf3, 
0x05,  0xf9,    0x12,  0x3b, 0xf5, 
0x03,  0xfd,    0x07,  0x3f, 0xfa, 
/* zoom ratio in(0.91, 1] */
0x00,  0x00,    0x40,  0x00, 0x00, 
0x04,  0xfa,    0x3c,  0x09, 0xfd, 
0x06,  0xf5,    0x39,  0x13, 0xf9, 
0x08,  0xf3,    0x31,  0x1f, 0xf5, 
0x08,  0xf3,    0x28,  0x2a, 0xf3, 
0x07,  0xf5,    0x1d,  0x34, 0xf3, 
0x05,  0xf9,    0x12,  0x3b, 0xf5, 
0x03,  0xfd,    0x07,  0x3f, 0xfa, 
/* zoom ratio in (0.83, 0.91] */
0x00,  0x00,    0x40,  0x00, 0x00, 
0x04,  0xfa,    0x3c,  0x09, 0xfd, 
0x06,  0xf5,    0x39,  0x13, 0xf9, 
0x08,  0xf3,    0x31,  0x1f, 0xf5, 
0x08,  0xf3,    0x28,  0x2a, 0xf3, 
0x07,  0xf5,    0x1d,  0x34, 0xf3, 
0x05,  0xf9,    0x12,  0x3b, 0xf5, 
0x03,  0xfd,    0x07,  0x3f, 0xfa, 
/* zoom ratio in (0.769, 0.83] */
0xfc,  0x05,    0x3e,  0x05, 0xfc, 
0x00,  0xff,    0x3b,  0x0e, 0xf8, 
0x02,  0xf9,    0x38,  0x18, 0xf5, 
0x05,  0xf5,    0x31,  0x21, 0xf4, 
0x07,  0xf4,    0x27,  0x2a, 0xf4, 
0x08,  0xf4,    0x1e,  0x30, 0xf6, 
0x07,  0xf6,    0x15,  0x35, 0xf9, 
0x06,  0xf9,    0x0b,  0x37, 0xff, 
/* zoom ratio in(0.715, 0.769]*/ 
0xf8,  0x0a,    0x3c,  0x0a, 0xf8, 
0xfb,  0x02,    0x3b,  0x12, 0xf6, 
0xff,  0xfd,    0x35,  0x1b, 0xf4, 
0x01,  0xf8,    0x30,  0x23, 0xf4, 
0x04,  0xf6,    0x27,  0x29, 0xf6, 
0x06,  0xf5,    0x1e,  0x2e, 0xf9, 
0x06,  0xf6,    0x16,  0x31, 0xfd, 
0x07,  0xf8,    0x0d,  0x32, 0x02, 
/* zoom ratio in(0.667, 0.715] */
0xf6,  0x0e,    0x38,  0x0e, 0xf6, 
0xf8,  0x06,    0x38,  0x15, 0xf5, 
0xfb,  0x00,    0x33,  0x1d, 0xf5, 
0xfe,  0xfc,    0x2d,  0x23, 0xf6, 
0x00,  0xf9,    0x26,  0x28, 0xf9, 
0x03,  0xf7,    0x1e,  0x2c, 0xfc, 
0x04,  0xf6,    0x18,  0x2e, 0x00, 
0x05,  0xf7,    0x11,  0x2e, 0x05, 
/* zoom ratio in(0.5, 0.667] */
0xfb,  0x13,    0x24,  0x13, 0xfb, 
0xfa,  0x0f,    0x23,  0x17, 0xfd, 
0xf9,  0x0b,    0x23,  0x1a, 0xff, 
0xf9,  0x07,    0x22,  0x1d, 0x01, 
0xf9,  0x04,    0x1f,  0x20, 0x04, 
0xfa,  0x01,    0x1c,  0x22, 0x07, 
0xfb,  0xff,    0x17,  0x24, 0x0b, 
0xfc,  0xfd,    0x14,  0x24, 0x0f, 
/* zoom ratio in(0.333, 0.5] */
0x05,  0x10,    0x16,  0x10, 0x05, 
0x04,  0x0f,    0x16,  0x11, 0x06, 
0x02,  0x0e,    0x15,  0x13, 0x08, 
0x01,  0x0c,    0x16,  0x14, 0x09, 
0x00,  0x0b,    0x15,  0x15, 0x0b, 
0x00,  0x0a,    0x13,  0x16, 0x0d, 
0xff,  0x08,    0x13,  0x17, 0x0f, 
0xfe,  0x07,    0x12,  0x18, 0x11, 
/* zoom ratio in (0.25, 0.333] */
0x09,  0x0f,    0x10,  0x0f, 0x09, 
0x08,  0x0e,    0x12,  0x0f, 0x09, 
0x07,  0x0e,    0x11,  0x10, 0x0a, 
0x06,  0x0d,    0x11,  0x11, 0x0b, 
0x05,  0x0c,    0x12,  0x11, 0x0c, 
0x04,  0x0c,    0x11,  0x12, 0x0d, 
0x04,  0x0b,    0x11,  0x12, 0x0e, 
0x03,  0x0a,    0x11,  0x13, 0x0f, 
/* zoom ratio in(0.2,0.25] */
0x0a,  0x0e,    0x10,  0x0e, 0x0a, 
0x0a,  0x0e,    0x0f,  0x0e, 0x0b, 
0x09,  0x0d,    0x10,  0x0f, 0x0b, 
0x08,  0x0d,    0x10,  0x0f, 0x0c, 
0x08,  0x0d,    0x0f,  0x0f, 0x0d, 
0x07,  0x0c,    0x10,  0x10, 0x0d, 
0x07,  0x0c,    0x0f,  0x10, 0x0e, 
0x06,  0x0b,    0x10,  0x10, 0x0f, 
/* zoom ratio in<=0.2 */
0x0b,  0x0e,    0x0e,  0x0e, 0x0b, 
0x0b,  0x0d,    0x0f,  0x0e, 0x0b, 
0x0a,  0x0d,    0x0f,  0x0e, 0x0c, 
0x0a,  0x0d,    0x0f,  0x0e, 0x0c, 
0x09,  0x0d,    0x0e,  0x0f, 0x0d, 
0x09,  0x0c,    0x0f,  0x0f, 0x0d, 
0x09,  0x0c,    0x0e,  0x0f, 0x0e, 
0x08,  0x0c,    0x0f,  0x0f, 0x0e
};


/* TDE_PARA_HTABLE_SIZE  unit:byte */
HI_U32 Tde8x32Coef[TDE_PARA_HTABLE_ORG_SIZE / 4 * TDE_PARA_HTABLE_NUM] =
{
/* ratio > 1 */
((HI_U32)(115 & 0xFF) << 24) | ((HI_U32)(13 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(115 & 0xFF) << 24) | ((HI_U32)(10 & 0xFF) << 16) | ((HI_U32)(-7 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(113 & 0xFF) << 24) | ((HI_U32)(7 & 0xFF) << 16) | ((HI_U32)(-6 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(112 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-5 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(112 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(-4 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(111 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(-4 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(108 & 0xFF) << 24) | ((HI_U32)(-4 & 0xFF) << 16) | ((HI_U32)(-3 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(106 & 0xFF) << 24) | ((HI_U32)(-6 & 0xFF) << 16) | ((HI_U32)(-2 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(104 & 0xFF) << 24) | ((HI_U32)(-8 & 0xFF) << 16) | ((HI_U32)(-1 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(102 & 0xFF) << 24) | ((HI_U32)(-10 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(99 & 0xFF) << 24) | ((HI_U32)(-11 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(96 & 0xFF) << 24) | ((HI_U32)(-13 & 0xFF) << 16) | ((HI_U32)(1 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(93 & 0xFF) << 24) | ((HI_U32)(-14 & 0xFF) << 16) | ((HI_U32)(1 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(89 & 0xFF) << 24) | ((HI_U32)(-15 & 0xFF) << 16) | ((HI_U32)(2 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(86 & 0xFF) << 24) | ((HI_U32)(-16 & 0xFF) << 16) | ((HI_U32)(2 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(82 & 0xFF) << 24) | ((HI_U32)(-16 & 0xFF) << 16) | ((HI_U32)(3 & 0xFF) << 8) | (-1 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (13 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (16 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (20 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-11 & 0xFF) << 8) | (24 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-12 & 0xFF) << 8) | (28 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-13 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-13 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-14 & 0xFF) << 8) | (41 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-15 & 0xFF) << 8) | (44 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-16 & 0xFF) << 8) | (49 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-16 & 0xFF) << 8) | (53 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-16 & 0xFF) << 8) | (57 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(-17 & 0xFF) << 8) | (62 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-17 & 0xFF) << 8) | (67 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-17 & 0xFF) << 8) | (71 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-17 & 0xFF) << 8) | (75 & 0xFF),
((HI_U32)(80 & 0xFF) << 24) | ((HI_U32)(-18 & 0xFF) << 16) | ((HI_U32)(3 & 0xFF) << 8) | (-1 & 0xFF),
/* ratio <= 1 && ratio > 0.69 */
((HI_U32)(86 & 0xFF) << 24) | ((HI_U32)(32 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (-1 & 0xFF),
((HI_U32)(85 & 0xFF) << 24) | ((HI_U32)(29 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(85 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(84 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(83 & 0xFF) << 24) | ((HI_U32)(23 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(83 & 0xFF) << 24) | ((HI_U32)(20 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(83 & 0xFF) << 24) | ((HI_U32)(17 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(82 & 0xFF) << 24) | ((HI_U32)(16 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(81 & 0xFF) << 24) | ((HI_U32)(14 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(79 & 0xFF) << 24) | ((HI_U32)(12 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(79 & 0xFF) << 24) | ((HI_U32)(10 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(77 & 0xFF) << 24) | ((HI_U32)(8 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(75 & 0xFF) << 24) | ((HI_U32)(6 & 0xFF) << 16) | ((HI_U32)(-7 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(74 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(-7 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(72 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(-7 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(70 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(-6 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(-10 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (39 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(-9 & 0xFF) << 8) | (41 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (43 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (46 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-3 & 0xFF) << 16) | ((HI_U32)(-8 & 0xFF) << 8) | (49 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-3 & 0xFF) << 16) | ((HI_U32)(-7 & 0xFF) << 8) | (51 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-3 & 0xFF) << 16) | ((HI_U32)(-6 & 0xFF) << 8) | (52 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-4 & 0xFF) << 16) | ((HI_U32)(-6 & 0xFF) << 8) | (55 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-4 & 0xFF) << 16) | ((HI_U32)(-5 & 0xFF) << 8) | (58 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-4 & 0xFF) << 16) | ((HI_U32)(-4 & 0xFF) << 8) | (60 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-5 & 0xFF) << 16) | ((HI_U32)(-3 & 0xFF) << 8) | (62 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-5 & 0xFF) << 16) | ((HI_U32)(-2 & 0xFF) << 8) | (65 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(-5 & 0xFF) << 16) | ((HI_U32)(-1 & 0xFF) << 8) | (66 & 0xFF),
((HI_U32)(68 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(-6 & 0xFF) << 8) | (1 & 0xFF),
/* ratio <= 0.69 && ratio > 0.5 */
((HI_U32)(53 & 0xFF) << 24) | ((HI_U32)(34 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(52 & 0xFF) << 24) | ((HI_U32)(34 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(53 & 0xFF) << 24) | ((HI_U32)(32 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(53 & 0xFF) << 24) | ((HI_U32)(31 & 0xFF) << 16) | ((HI_U32)(4 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(53 & 0xFF) << 24) | ((HI_U32)(30 & 0xFF) << 16) | ((HI_U32)(4 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(53 & 0xFF) << 24) | ((HI_U32)(29 & 0xFF) << 16) | ((HI_U32)(3 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(52 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(3 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(52 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(2 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(52 & 0xFF) << 24) | ((HI_U32)(26 & 0xFF) << 16) | ((HI_U32)(2 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(51 & 0xFF) << 24) | ((HI_U32)(26 & 0xFF) << 16) | ((HI_U32)(1 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(51 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(1 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(51 & 0xFF) << 24) | ((HI_U32)(24 & 0xFF) << 16) | ((HI_U32)(1 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(50 & 0xFF) << 24) | ((HI_U32)(23 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(50 & 0xFF) << 24) | ((HI_U32)(22 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(50 & 0xFF) << 24) | ((HI_U32)(20 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(50 & 0xFF) << 24) | ((HI_U32)(19 & 0xFF) << 16) | ((HI_U32)(0 & 0xFF) << 8) | (-2 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (38 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (39 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (40 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (41 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (42 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (43 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (44 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-2 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (44 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (45 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (46 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (47 & 0xFF),
((HI_U32)(-2 & 0xFF) << 24) | ((HI_U32)(-1 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (47 & 0xFF),
((HI_U32)(48 & 0xFF) << 24) | ((HI_U32)(19 & 0xFF) << 16) | ((HI_U32)(-1 & 0xFF) << 8) | (-2 & 0xFF),
/* ratio <= 0.5 && ratio >0.33 */
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(31 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(31 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(30 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(29 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(42 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(24 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(23 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(22 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(21 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (35 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (35 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(18 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(18 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (38 & 0xFF),
((HI_U32)(38 & 0xFF) << 24) | ((HI_U32)(21 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (0 & 0xFF),
/* ratio <= 0.33 && ratio > 0.02 */
((HI_U32)(38 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (3 & 0xFF),
((HI_U32)(38 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (3 & 0xFF),
((HI_U32)(38 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (3 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(37 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(26 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (2 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(36 & 0xFF) << 24) | ((HI_U32)(24 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(35 & 0xFF) << 24) | ((HI_U32)(24 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(35 & 0xFF) << 24) | ((HI_U32)(23 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(35 & 0xFF) << 24) | ((HI_U32)(22 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(35 & 0xFF) << 24) | ((HI_U32)(22 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (28 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (29 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (29 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(18 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(6 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(6 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(6 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(7 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(1 & 0xFF) << 24) | ((HI_U32)(7 & 0xFF) << 16) | ((HI_U32)(21 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(35 & 0xFF) << 24) | ((HI_U32)(21 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (1 & 0xFF),
/* ratio <= 0.02 */
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(31 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(31 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(30 & 0xFF) << 16) | ((HI_U32)(11 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(29 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(10 & 0xFF) << 8) | (1 & 0xFF),
((HI_U32)(42 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(28 & 0xFF) << 16) | ((HI_U32)(9 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(41 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(27 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(8 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(40 & 0xFF) << 24) | ((HI_U32)(25 & 0xFF) << 16) | ((HI_U32)(7 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(24 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(23 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(22 & 0xFF) << 16) | ((HI_U32)(6 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(39 & 0xFF) << 24) | ((HI_U32)(21 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (0 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (31 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(12 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(1 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (32 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(13 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (33 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(14 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(15 & 0xFF) << 8) | (34 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(2 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (35 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(16 & 0xFF) << 8) | (35 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(17 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(18 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(3 & 0xFF) << 16) | ((HI_U32)(18 & 0xFF) << 8) | (36 & 0xFF),
((HI_U32)(-1 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(19 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(4 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (37 & 0xFF),
((HI_U32)(0 & 0xFF) << 24) | ((HI_U32)(5 & 0xFF) << 16) | ((HI_U32)(20 & 0xFF) << 8) | (38 & 0xFF),
((HI_U32)(38 & 0xFF) << 24) | ((HI_U32)(21 & 0xFF) << 16) | ((HI_U32)(5 & 0xFF) << 8) | (0 & 0xFF)

};

#ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__TDE_FILTERPARA_H__ */
