/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************
File Name   : drv_pq_define.h
Author      : p00203646
Version     : v3.0
Abstract    :
*****************************************************************************/

#ifndef _DRV_PQ_DEFINE_V3_H_
#define _DRV_PQ_DEFINE_V3_H_

#include "hi_type.h"

/*****************************************************************************
PQ Software Define
*****************************************************************************/
#define PQ_VERSION      "1.0.2.0"
#define PQ_DEF_NAME     "pqparam"
#define PQ_MAGIC_NUM    "HIPQ"
#if defined(CHIP_TYPE_hi3798mv100)
#define PQ_CHIP_NAME    "3798MV100"
#elif defined(CHIP_TYPE_hi3796mv100)
#define PQ_CHIP_NAME    "3798MV100"
#elif defined(CHIP_TYPE_hi3716dv100)
#define PQ_CHIP_NAME    "3798MV100"
#elif defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
#define PQ_CHIP_NAME    "HiFoneB2"
#elif defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#define PQ_CHIP_NAME    "3716MV410"
#endif



/*****************************************************************************
PQ Head File Define
*****************************************************************************/
#define STR_LEN_8                8
#define STR_LEN_32               32
#define STR_LEN_80               80
#define STR_LEN_256              256
#define STR_LEN_1024             1024
#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b) || defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#define ACM_Y_NUM                9
#define ACM_S_NUM                13
#define ACM_H_NUM                29
#endif
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
#define ACM_Y_NUM                5
#define ACM_S_NUM                7
#define ACM_H_NUM                29
#endif
#define PHY_REG_MAX              2048
#define PQ_ALG_MODULE_MAX        32


/*****************************************************************************
PQ Tools Virtual Command
*****************************************************************************/

#define PQ_CMD_VIRTUAL_BIN_EXPORT                                 0xffffff00
#define PQ_CMD_VIRTUAL_BIN_IMPORT                                 0xffffff04
#define PQ_CMD_VIRTUAL_BIN_FIXED                                  0xffffff08
#define PQ_CMD_VIRTUAL_DRIVER_VERSION                             0xffffffff

#define PQ_CMD_VIRTUAL_HD_CTRL                                    0xffff0001
#define PQ_CMD_VIRTUAL_SD_CTRL                                    0xffff0002
#define PQ_CMD_VIRTUAL_DEMO_CTRL                                  0xffff0003
#define PQ_CMD_VIRTUAL_SOURCE_SELECT                              0xffff0004
#define PQ_CMD_VIRTUAL_READ_REGTYPE                               0xffff0005
#define PQ_CMD_VIRTUAL_OUT_MODE                                   0xffff0006

#define PQ_CMD_VIRTUAL_ACM_RECMD_LUT                              0xffff0100
#define PQ_CMD_VIRTUAL_ACM_BLUE_LUT                               0xffff0101
#define PQ_CMD_VIRTUAL_ACM_GREEN_LUT                              0xffff0102
#define PQ_CMD_VIRTUAL_ACM_BG_LUT                                 0xffff0103
#define PQ_CMD_VIRTUAL_ACM_FLESH_LUT                              0xffff0104
#define PQ_CMD_VIRTUAL_ACM_6BCOLOR_LUT                            0xffff0105

#define PQ_CMD_VIRTUAL_DCI_LUT                                    0xffff0200
#define PQ_CMD_VIRTUAL_DCI_BS_LUT                                 0xffff0201

#define PQ_CMD_VIRTUAL_GAMM_PARA		                          0xffff0301
#define PQ_CMD_VIRTUAL_DEGAMM_PARA 	                              0xffff0302
#define PQ_CMD_VIRTUAL_GFX_GAMM_PARA	                          0xffff0303
#define PQ_CMD_VIRTUAL_GFX_DEGAMM_PARA                            0xffff0304

#define PQ_CMD_VIRTUAL_HDR_TM_LUT                                 0xffff0401

#define PQ_CMD_VIRTUAL_ACC_LUT                                    0xffff0500


/*****************************************************************************
PQ Bin Define
*****************************************************************************/

/**
* PQ Param
**/

/*PQ Module类型*/
typedef enum hiPQ_ALG_MODULE_E
{
    PQ_ALG_MODULE_DCI = 0,
    PQ_ALG_MODULE_ACM,
    PQ_ALG_MODULE_BUTT,
} PQ_ALG_MODULE_E;

typedef struct hiPQ_FILE_HEADER_S
{
    HI_U32          u32FileCheckSum;             /* 参数文件的校验和，File Header（除此变量外）和data的所有逐字节校验和，用于检验参数正确性和判断是否dirty */
    HI_U32          u32ParamSize;                /* 参数文件大小，包括File header和data */
    HI_CHAR         u8Version[STR_LEN_32];       /* 版本号，字符串表示 */
    HI_CHAR         u8Author[STR_LEN_32];        /* 参数调试者签名，字符串表示 */
    HI_CHAR         u8Desc[STR_LEN_1024];        /* 版本描述，字符串表示 */
    HI_CHAR         u8Time[STR_LEN_32];          /* 参数文件生成（烧写）时间，[0] ~ [5]：yy mm dd hh mm ss，[6]~[7]保留。该时间由PQ工具从PC上自动获取，无需用户输入 */
    HI_CHAR         u8ChipName[STR_LEN_32];      /* 参数文件生成时的ChipName*/
    HI_CHAR         u8SDKVersion[STR_LEN_80];    /* 参数文件生成时的SDK版本*/
    HI_U32          u32PhyRegNum;                /* 参数文件保存的物理寄存器个数*/
    HI_U32          u32Offset[PQ_ALG_MODULE_MAX];/* 算法系数偏移地址*/
    HI_U32          u32PhyOffset;                /* 算法系数偏移地址*/
} PQ_FILE_HEADER_S;


/**
* DCI Coef
**/

/*DCI曲线配置表*/
typedef struct hiPQ_DCI_LUT_S
{
    HI_S16 s16WgtCoef0[33];
    HI_S16 s16WgtCoef1[33];
    HI_S16 s16WgtCoef2[33];
    HI_U16 u16Gain0;
    HI_U16 u16Gain1;
    HI_U16 u16Gain2;
} PQ_DCI_LUT_S;

/*DCI Gain*/
typedef struct hiPQ_DCI_GAIN_S
{
    HI_U16 u16Gain0;
    HI_U16 u16Gain1;
    HI_U16 u16Gain2;
} PQ_DCI_GAIN_S;

typedef struct hiPQ_DCI_COEF_S
{
    PQ_DCI_LUT_S  stPqDciLut;
} PQ_DCI_COEF_S;


/**
*ACC Coef
**/
#define ACC_LUT_NUM             5
#define ACC_LUT_RESOLUTION      9

typedef struct hi_PQ_ACC_LUT_S
{
    HI_S32 as32AccLut[ACC_LUT_NUM][ACC_LUT_RESOLUTION];
} PQ_ACC_LUT_S;

/**
* ACM Coef
**/

typedef struct hiPQ_ACM_LUT_S
{
    HI_S16 as16Y[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
    HI_S16 as16H[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
    HI_S16 as16S[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];   /**<  */
} PQ_ACM_LUT_S;

typedef struct hiPQ_ACM_MODE_S
{
    PQ_ACM_LUT_S stModeRecmd;
    PQ_ACM_LUT_S stModeBlue;
    PQ_ACM_LUT_S stModeGreen;
    PQ_ACM_LUT_S stModeBG;
    PQ_ACM_LUT_S stModeFleshtone;
    PQ_ACM_LUT_S stMode6BColor;
} PQ_ACM_MODE_S;


typedef struct hiPQ_ACM_COEF_S
{
    PQ_ACM_MODE_S stModeLut;
} PQ_ACM_COEF_S;

/**
* PQ Phy Register
**/

typedef struct hiPQ_PHY_REG_S
{
    HI_U32 u32RegAddr;     //register addr
    HI_U8  u8Lsb;          //register lsb
    HI_U8  u8Msb;          //register msb
    HI_U8  u8SourceMode;   //video source
    HI_U8  u8OutputMode;   //output mode
    HI_U32 u32Module;      //module
    HI_U32 u32Value;       //register value
} PQ_PHY_REG_S;

/**
* PQ_PARAM_S
**/

typedef struct hiPQ_COEF_S
{
    PQ_DCI_COEF_S               stDciCoef;
    PQ_ACM_COEF_S               stAcmCoef;
} PQ_COEF_S;

typedef struct hiPQ_PARAM_S
{
    PQ_FILE_HEADER_S            stPQFileHeader;
    PQ_COEF_S                   stPQCoef;
    PQ_PHY_REG_S                stPQPhyReg[PHY_REG_MAX];
} PQ_PARAM_S;


#endif

