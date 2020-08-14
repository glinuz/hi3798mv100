/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     :drv_pq_table.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#ifndef __DRV_PQ_TABLE_H__
#define __DRV_PQ_TABLE_H__

#include "hi_drv_pq.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*soft table pq bin define*/
/* Fmd */
#define PQ_SOFT_IP_DETECT                0xfffe0001
#define PQ_SOFT_FRMRATETHR_L             0xfffe0005
#define PQ_SOFT_FRMRATETHR_H             0xfffe0009


/* DB Init */
#define PQ_SOFT_DB_NUM_WIN               0xfffe0105
#define PQ_SOFT_DB_HYMINBLKNUM           0xfffe0109
#define PQ_SOFT_DB_HCMINBLKNUM           0xfffe010d
#define PQ_SOFT_DB_VYMINBLKNUM           0xfffe0111
#define PQ_SOFT_DB_KEYFRM_NUM            0xfffe0115
#define PQ_SOFT_DB_HYBORDCOUNTHRD        0xfffe0119
#define PQ_SOFT_DB_HCBORDCOUNTHRD        0xfffe011d
#define PQ_SOFT_DB_VYBORDCOUNTHRD        0xfffe0121
#define PQ_SOFT_DB_FRMWINTMP             0xfffe0125
#define PQ_SOFT_DB_FRMSATWINTMP          0xfffe0129

/* DB Filter */
#define PQ_SOFT_DB_HL_DELTA_STRONG_2     0xfffe0141
#define PQ_SOFT_DB_HL_DELTA_STRONG_1     0xfffe0142
#define PQ_SOFT_DB_HL_DELTA_ULT_STR_2    0xfffe0143
#define PQ_SOFT_DB_HL_DELTA_ULT_STR_1    0xfffe0144
#define PQ_SOFT_DB_HL_DELTA_MID_2        0xfffe0145
#define PQ_SOFT_DB_HL_DELTA_MID_1        0xfffe0146

#define PQ_SOFT_DB_LV_DELTA_2            0xfffe0147
#define PQ_SOFT_DB_LV_DELTA_1            0xfffe0148

#define PQ_SOFT_DB_HC_DELTA_2            0xfffe0149
#define PQ_SOFT_DB_HC_DELTA_1            0xfffe014a



#define PQ_SOFT_DB_STR_FADE_1_2          0xfffe0151
#define PQ_SOFT_DB_STR_FADE_1_1          0xfffe0152

#define PQ_SOFT_DB_STR_FADE_1_4_2        0xfffe0153
#define PQ_SOFT_DB_STR_FADE_1_4_1        0xfffe0154

#define PQ_SOFT_DB_STR_FADE_2_2          0xfffe0155
#define PQ_SOFT_DB_STR_FADE_2_1          0xfffe0156

#define PQ_SOFT_DB_STR_FADE_3_2          0xfffe0157
#define PQ_SOFT_DB_STR_FADE_3_1          0xfffe0158

#define PQ_SOFT_DB_STR_V_FADE_2          0xfffe0159
#define PQ_SOFT_DB_STR_V_FADE_1          0xfffe015a



#define PQ_SOFT_DB_DIR_STR               0xfffe0161

#define PQ_SOFT_DB_DIR_GAIN_2            0xfffe0163
#define PQ_SOFT_DB_DIR_GAIN_1            0xfffe0164


#define PQ_SOFT_DB_DYN_PARA_STR_1        0xfffe0171
#define PQ_SOFT_DB_DYN_PARA_STR_2        0xfffe0172
#define PQ_SOFT_DB_DYN_PARA_STR_3        0xfffe0173
#define PQ_SOFT_DB_DYN_PARA_STR_4        0xfffe0174
#define PQ_SOFT_DB_DYN_PARA_STR_5        0xfffe0175
#define PQ_SOFT_DB_DYN_PARA_STR_6        0xfffe0176
#define PQ_SOFT_DB_DYN_PARA_STR_7        0xfffe0177
#define PQ_SOFT_DB_DYN_PARA_STR_8        0xfffe0178
#define PQ_SOFT_DB_DYN_PARA_STR_9        0xfffe0179

#define PQ_SOFT_DB_DYN_PARA_MID_1        0xfffe0181
#define PQ_SOFT_DB_DYN_PARA_MID_2        0xfffe0182
#define PQ_SOFT_DB_DYN_PARA_MID_3        0xfffe0183
#define PQ_SOFT_DB_DYN_PARA_MID_4        0xfffe0184
#define PQ_SOFT_DB_DYN_PARA_MID_5        0xfffe0185
#define PQ_SOFT_DB_DYN_PARA_MID_6        0xfffe0186
#define PQ_SOFT_DB_DYN_PARA_MID_7        0xfffe0187
#define PQ_SOFT_DB_DYN_PARA_MID_8        0xfffe0188
#define PQ_SOFT_DB_DYN_PARA_MID_9        0xfffe0189

#define PQ_SOFT_DB_DYN_PARA_WEK_1        0xfffe0191
#define PQ_SOFT_DB_DYN_PARA_WEK_2        0xfffe0192
#define PQ_SOFT_DB_DYN_PARA_WEK_3        0xfffe0193
#define PQ_SOFT_DB_DYN_PARA_WEK_4        0xfffe0194
#define PQ_SOFT_DB_DYN_PARA_WEK_5        0xfffe0195
#define PQ_SOFT_DB_DYN_PARA_WEK_6        0xfffe0196
#define PQ_SOFT_DB_DYN_PARA_WEK_7        0xfffe0197
#define PQ_SOFT_DB_DYN_PARA_WEK_8        0xfffe0198
#define PQ_SOFT_DB_DYN_PARA_WEK_9        0xfffe0199



/*PQ Bin Module¿‡–Õ*/
typedef enum hiPQ_BIN_MODULE_E
{
    PQ_BIN_MODULE_DEI      = 0x1,
    PQ_BIN_MODULE_FMD      = 0x2,
    PQ_BIN_MODULE_SHARPEN  = 0x4,
    PQ_BIN_MODULE_DNR      = 0x8,
    PQ_BIN_MODULE_DCI      = 0x10,
    PQ_BIN_MODULE_ACM      = 0x20,
    PQ_BIN_MODULE_HSHARPEN = 0x40,
    PQ_BIN_MODULE_SR       = 0x100,
    PQ_BIN_MODULE_DB       = 0x200,
    PQ_BIN_MODULE_DM       = 0x400,
    PQ_BIN_MODULE_TNR      = 0x800,
    PQ_BIN_MODULE_DS       = 0x1000,
    PQ_BIN_MODULE_ALL      = 0xffff,
} PQ_BIN_MODULE_E;

/*PQ Source Mode*/
typedef enum hiPQ_SOURCE_MODE_E
{
    SOURCE_MODE_NO = 0,
    SOURCE_MODE_SD,
    SOURCE_MODE_HD,
    SOURCE_MODE_UHD,
    SOURCE_MODE_ALL,
} PQ_SOURCE_MODE_E;

/*PQ Output Mode*/
typedef enum hiPQ_OUTPUT_MODE_E
{
    OUTPUT_MODE_NO = 0,
    OUTPUT_MODE_SD,
    OUTPUT_MODE_HD,
    OUTPUT_MODE_UHD,
    OUTPUT_MODE_ALL,
} PQ_OUTPUT_MODE_E;


/*PQ Reg Type*/
typedef enum hiPQ_REG_TYPE_E
{
    REG_TYPE_VPSS = 0,
    REG_TYPE_VDP,
    REG_TYPE_ALL,
} PQ_REG_TYPE_E;

/*PQ Reg Type*/
typedef enum hiPQ_PRN_TABLE_TYPE_E
{
    PRN_TABLE_ALL = 0,
    PRN_TABLE_MULTI,
    PRN_TABLE_SINGLE,
} PQ_PRN_TABLE_TYPE_E;

typedef struct hiPQ_BIN_MODULE_POS_S
{
   HI_U32 u32StartPos;
   HI_U32 u32EndPos;   
} HI_PQ_BIN_MODULE_POS_S;

/* check Timing Source Mode*/
PQ_SOURCE_MODE_E PQ_MNG_CheckSourceMode(HI_U32 u32Width, HI_U32 u32Height);
/* check Timing Output Mode*/
PQ_OUTPUT_MODE_E PQ_MNG_CheckOutputMode(HI_BOOL bSRState);

HI_S32 PQ_MNG_DeInitPQTable(HI_VOID);

HI_S32 PQ_MNG_InitPQTable(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault);

HI_S32 PQ_MNG_UpdatePhyList(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U8 u8SourceMode, HI_U8 u8OutputMode, HI_U32 u32Value);

HI_S32 PQ_MNG_SetVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value);

HI_S32 PQ_MNG_SetVpssReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value);

HI_S32 PQ_MNG_LoadPhyList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule);

HI_S32 PQ_MNG_LoadMultiList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule);

HI_S32 PQ_MNG_InitPhyList(HI_U32 u32ID, PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule);

HI_U32 PQ_MNG_GetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultValue);

HI_S32 PQ_MNG_SetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32Value);

HI_U32 PQ_MNG_GetArraySoftTable(HI_U32 u32Lut, HI_U32* pArray, HI_U32 u32Num, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultArray[]);

HI_S32 PQ_MNG_SetReg(HI_PQ_REGISTER_S* pstAttr, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode);

HI_S32 PQ_MNG_GetReg(HI_PQ_REGISTER_S* pstAttr);

HI_VOID PQ_MNG_PrintTable(PQ_PRN_TABLE_TYPE_E enType, HI_U32 u32PriAddr);

HI_U32 PQ_MNG_GetPhyListNum(HI_VOID);

HI_U32 PQ_MNG_GetMultiListNum(HI_VOID);

HI_S32 PQ_MNG_GetPqMultitReg( PQ_PHY_REG_S** pstPqMultitReg);






#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
