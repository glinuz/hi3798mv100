//描述每个项目逻辑共性的部分，比如逻辑基地址，节点地址，位宽枚举，数据格式等。
//
//
#ifndef __VPSS_CBB_COMMON_H__
#define __VPSS_CBB_COMMON_H__

//#include <stdio.h>
//#include <string.h>
#include "hi_type.h"
#include "hi_reg_common.h"
#include "vpss_cbb_hal.h"

#define DEF_CBB_MAX_NODE_NUM VPSS_HAL_NODE_BUTT

#define VPSS_MULTI_VER 4096
#define VPSS_MULTI_HOR 1048576


typedef struct
{
	HI_U32 u32BasePhyAddr;   //逻辑物理基地址
	HI_U8 *pu8BaseVirAddr;   //逻辑虚拟基地址

	HI_U32 u32NodePhyAddr;   //DDR中节点物理基地址
	HI_U8 *pu8NodeVirAddr;   //DDR中节点虚拟基地址
}CBB_REG_ADDR_S;

typedef enum 
{
	CBB_BITWIDTH_8BIT = 0,
	CBB_BITWIDTH_10BIT,
	CBB_BITWIDTH_12BIT,
	CBB_BITWIDTH_10BIT_CTS,
	CBB_BITWIDTH_BUTT,
}CBB_BITWIDTH_E;

typedef struct 
{
	HI_U32 u32PhyAddr_YHead;
	HI_U32 u32PhyAddr_Y;
	HI_U8* pu8VirAddr_Y;
	
	HI_U32 u32PhyAddr_CHead;
	HI_U32 u32PhyAddr_C;
	HI_U8* pu8VirAddr_C;

	HI_U32 u32Stride_Y;
	HI_U32 u32Stride_C;
	
    HI_U32  u32Head_Stride;
    HI_U32  u32Head_Size;
}CBB_FRAME_ADDR_S;

typedef enum 
{
	CBB_PIXEL_FMT_SP420_2X1,
	CBB_PIXEL_FMT_SP420_1X2,
	CBB_PIXEL_FMT_SP422,
	CBB_PIXEL_FMT_SP420_TILE,
	CBB_PIXEL_FMT_SP422_TILE,
	CBB_PIXEL_FMT_P420,
    CBB_PIXEL_FMT_P422,
    CBB_PIXEL_FMT_PACKAGEYUYV,
    CBB_PIXEL_FMT_PACKAGEYVYU,
    CBB_PIXEL_FMT_PACKAGEUYVY,
}CBB_PIXEL_FMT_E;

typedef enum
{
    AXI_BURST_LEN_16 = 0x0,
    AXI_BURST_LEN_8, 
    AXI_BURST_LEN_4, 
}CBB_AXI_BURST_LEN_E;


typedef struct 
{
    HI_U32 u32NodePhyAddr[DEF_CBB_MAX_NODE_NUM];
    HI_U8* pu8NodeVirAddr[DEF_CBB_MAX_NODE_NUM];
    HI_U32 u32NodeValid[DEF_CBB_MAX_NODE_NUM];
}CBB_START_CFG_S;

#if 0
typedef struct
{
  HI_U32 u32X;
  HI_U32 u32Y;
  HI_U32 u32Wth;
  HI_U32 u32Hgt;
} HI_RECT_S;
#endif

typedef struct
{
    HI_U32 u32ROutStd;
    HI_U32 u32WOutStd;
}CBB_OUTSTANDING_S;

typedef struct 
{
    CBB_AXI_BURST_LEN_E eBurstLength;
    CBB_OUTSTANDING_S   stOutStanding;
}CBB_AXI_CFG_S;


typedef enum 
{
	/* vhd0通道垂直预缩放模式：
		0：垂直缩小2倍；
		1：垂直缩小4倍；
		2：垂直缩小8倍；
		3：抽底场。
	*/
	CBB_FIR_X2 = 0,
	CBB_FIR_X4 = 1,
	CBB_FIR_X8 = 2,
	CBB_FIR_EXT_BOTTOM = 3, //extract bottom field
	CBB_FIR_BUTT,
} CBB_FIR_MODE_E;

typedef enum 
{
	CBB_TUNOUT_2 = 0,
	CBB_TUNOUT_4 = 1,
	CBB_TUNOUT_8 = 2,
	CBB_TUNOUT_16 = 3,
	CBB_TUNOUT_BUTT,
} CBB_TUNOUT_MODE_E;


typedef enum
{
  CBB_COLOR_BLACK = 0,
  CBB_COLOR_RED,
  CBB_COLOR_WHITE,
  CBB_COLOR_GREEN,
  CBB_COLOR_BLUE ,
  CBB_COLOR_BUTT,
} CBB_COLOR_E;

typedef enum
{
  CBB_FREAM_BUF_8BIT = 0,
  CBB_FREAM_BUF_2BIT_LB,
  CBB_FREAM_BUF_CMP_HEADER,
  CBB_FREAM_BUF_BUTT,
  CBB_FREAM_BUF_CNT = CBB_FREAM_BUF_BUTT,
} CBB_FRAME_BUF_TYPE_E;


typedef enum
{
    CBB_ZME_MODE_HOR = 0 ,
    CBB_ZME_MODE_VER     ,

    CBB_ZME_MODE_HORL    ,
    CBB_ZME_MODE_HORC    ,
    CBB_ZME_MODE_VERL    ,
    CBB_ZME_MODE_VERC    ,

    CBB_ZME_MODE_ALL     ,
    CBB_ZME_MODE_NONL    ,

    CBB_ZME_MODE_BUTT
} CBB_ZME_MODE_E;


#endif
