/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_comm.h
  Version       : Initial Draft
  Author        : pengjunwei 00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_COMM_H__
#define __PQ_HAL_COMM_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_reg_common.h"
#include "hi_drv_pq.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VPSS_REGS_ADDR    0xf8cb0000UL
#define VDP_REGS_ADDR     0xf8cc0000UL
#define SPECIAL_REGS_ADDR 0xfffe0000UL

#define REG_BASE_ADDR_MASK     0xffff0000
#define REG_OFFSET_ADDR_MASK   0x0000ffff

#define VPSS_HANDLE_NUM    33
#define VDP_VP_MAX         2

#define VID_MAX            5 /* in 98c equal to 8; but equal to 5 in 98m and 98m_a*/

#define PQ_VP_OFFSET      0x800
#define PQ_VID_OFFSET     0x800
#define PQ_WBC_OFFSET     0x400

#define PQ_GP_MAX          2
#define PQ_GP_OFFSET       0x800

#define VPSS_PQ_REG_SIZE(start, end)\
    (offsetof(S_CAS_REGS_TYPE, end) + sizeof(HI_U32) -\
     offsetof(S_CAS_REGS_TYPE, start))

#define VDP_PQ_REG_SIZE(start, end)\
    (offsetof(S_VDP_REGS_TYPE, end) + sizeof(HI_U32) -\
     offsetof(S_VDP_REGS_TYPE, start))



#define PQ_TRACE(level, range, fmt...) \
    do{\
        if (level <= range)\
        {\
            HI_ERR_PQ(fmt);\
        }\
    }while(0)

#define PQ_CHECK_NULL_PTR(ptr) \
    do{\
        if (NULL == ptr){\
            HI_ERR_PQ("ptr is NULL!\n");\
            return HI_FAILURE;\
        }\
    }while(0)

#define PQ_CHECK_OVER_RANGE(ID, MaxID) \
    do{\
        if (ID >= MaxID){\
            HI_ERR_PQ("[%d] over range!\n",ID);\
            return HI_FAILURE;\
        }\
    }while(0)

#define PQ_DBG() \
    do{\
        HI_DEBUG_PQ("\n");\
    }while(0)

#define ALG_CLIP3(low,high,x)  (MAX2(MIN2((high),(x)),(low)))

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define pqprint(type, fmt, arg...)  PQ_HAL_PrintMsg(type, fmt, ##arg)
#else
#define pqprint(type, fmt, arg...)
#endif

/*PQ Bin Module类型*/
typedef enum hiPQ_PRN_TYPE_E
{
    PQ_PRN_NOTHING  = 0,          /*不打印信息，初始化时使用*/
    PQ_PRN_FATAL    = 0x1,        /*致命异常（fatal error），比如未知异常*/
    PQ_PRN_ERROR    = 0x2,        /*一般异常（error），比如语法错误*/
    PQ_PRN_WARNING  = 0x4,        /*告警异常*/
    PQ_PRN_INFO     = 0x8,        /*通知信息*/
    PQ_PRN_DBG      = 0x10,       /*调试信息*/

    PQ_PRN_VPSS     = 0x20,       /*VPSS 调试信息*/
    PQ_PRN_VDP      = 0x40,       /*VDP 调试信息*/
    PQ_PRN_TABLE    = 0x80,       /*PQ Table 调试信息打印*/

    PQ_PRN_DEI      = 0x100,      /*DEI 算法调试信息打印*/
    PQ_PRN_FMD_READ = 0x200,      /*FMD_READ 算法调试信息打印*/
    PQ_PRN_FMD_CALC = 0x400,      /*FMD_CALC 算法调试信息打印*/
    PQ_PRN_HSHARPEN = 0x800,      /*HSHARPEN 算法调试信息打印*/
    PQ_PRN_DNR      = 0x1000,     /*DNR 算法调试信息打印*/
    PQ_PRN_DCI      = 0x2000,     /*DCI 算法调试信息打印*/
    PQ_PRN_ACM      = 0x4000,     /*ACM 算法调试信息打印*/
    PQ_PRN_SHARPEN  = 0x10000,    /*SHARPEN 算法调试信息打印*/
    PQ_PRN_SR       = 0x20000,    /*SR 算法调试信息打印*/
    PQ_PRN_DB_READ  = 0x40000,    /*DB_READ 算法调试信息打印*/
    PQ_PRN_DB_CALC  = 0x80000,    /*DB_CALC 算法调试信息打印*/
    PQ_PRN_DM       = 0x100000,   /*DM 算法调试信息打印*/
    PQ_PRN_CSC      = 0x200000,   /*CSC 算法调试信息打印*/
    PQ_PRN_ZME      = 0x400000,   /*ZME 算法调试信息打印*/
    PQ_PRN_TNR      = 0x800000,   /*TNR 算法调试信息打印*/
    PQ_PRN_SNR      = 0x1000000,   /*SNR 算法调试信息打印*/

    PQ_PRN_ALWS     = 0xffffffff  /*不受控打印*/
} PQ_PRN_TYPE_E;

typedef enum hiPQ_HAL_LAYER_VP_E
{
    VDP_LAYER_VP0   = 0,
    VDP_LAYER_VP1   = 1,
    VDP_LAYER_DHD0  = 2,

    VDP_LAYER_VP_BUTT

} PQ_HAL_LAYER_VP_E;


typedef enum hiPQ_HAL_LAYER_VID_E
{
    PQ_HAL_LAYER_VID0  = 0, /* VDP_LAYER_V0 */
    PQ_HAL_LAYER_VID1  = 1, /* VDP_LAYER_V1 */
    PQ_HAL_LAYER_VID2  = 2, /* VDP_LAYER_V2 */
    PQ_HAL_LAYER_VID3  = 3, /* VDP_LAYER_V3 */
    PQ_HAL_LAYER_VID4  = 4, /* VDP_LAYER_V4 */

    //PQ_HAL_LAYER_VID5  = 5, /* VDP_LAYER_V5 */
    //PQ_HAL_LAYER_VID6  = 6, /* for FI pixel */ /* using in VDP */
    //PQ_HAL_LAYER_VID7  = 7, /* for FI info  */ /* using in VDP */

    PQ_HAL_LAYER_VID_BUTT

} PQ_HAL_LAYER_VID_E;


typedef struct hiPQ_MMZ_BUF_S
{
    HI_U8 *pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
} PQ_MMZ_BUF_S;

#if defined(CHIP_TYPE_hi3798cv200)
extern PQ_MMZ_BUF_S     g_stIPSelAlgBuffer;
extern S_VDP_REGS_TYPE *g_pstIPSelVdpAlg;
#endif

HI_S32  PQ_HAL_MMZ_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, PQ_MMZ_BUF_S *pstMBuf);

HI_VOID PQ_HAL_MMZ_UnmapAndRelease(PQ_MMZ_BUF_S *pstMBuf);

HI_S32  PQ_HAL_AllocAndMap(const char *bufname, char *zone_name, HI_U32 u32Size, int align, PQ_MMZ_BUF_S *pstMBuf);

HI_VOID PQ_HAL_UnmapAndRelease(PQ_MMZ_BUF_S *pstMBuf);

/**
 \brief 初始化HAL模块，申请本地寄存器内存;
 \attention \n
  无

 \param[in] none

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_Init(HI_VOID);

/**
 \brief 去初始化HAL模块,释放本地寄存器内存;
 \attention \n
  无

 \param[in] none

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_Deinit(HI_VOID);

/**
 \brief 更新VPSS寄存器;
 \attention \n
  无

 \param[in] u32HandleNo  : VPSS通道号
 \param[in] *pstVPSSReg  : 由驱动传过来的寄存器指针

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdateVpssReg(HI_U32 u32HandleNo, S_CAS_REGS_TYPE *pstVPSSReg, S_VPSSWB_REGS_TYPE *pstVPSSWbcReg);


/**
 \brief 更新VDP寄存器;
 \attention \n
  无

 \param[in] *pstVDPReg  : 由驱动传过来的VPSS寄存器指针

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_UpdateVdpReg(S_VDP_REGS_TYPE *pstVDPReg);


/**
 \brief 检查VPSS当前u32HandleNo对应虚拟寄存器是否有效;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::HI_BOOL

 */
HI_BOOL PQ_HAL_CheckVpssValid(HI_U32 u32HandleNo);

/**
 \brief 检查VDP寄存器是否有效;
 \attention \n
  无

 \param[in] none

 \retval ::HI_BOOL

 */
HI_BOOL PQ_HAL_CheckVdpValid(HI_VOID);

/**
 \brief 获取VPSS当前寄存器;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::VPSS寄存器指针

 */
S_CAS_REGS_TYPE *PQ_HAL_GetVpssReg(HI_U32 u32HandleNo);

/**
 \brief 获取VPSS WBC INFO;
 \attention \n
  无

 \param[in] u32HandleNo

 \retval ::VPSS WBC INFO

 */
S_VPSSWB_REGS_TYPE *PQ_HAL_GetVpssWbcReg(HI_U32 u32HandleNo);

/**
 \brief 获取VDP当前寄存器;
 \attention \n
  无

 \param[in] none

 \retval ::VDP寄存器指针

 */
S_VDP_REGS_TYPE *PQ_HAL_GetVdpReg(HI_VOID);

/**
 \brief 写寄存器;
 \attention \n
  无

 \param[in] u32HandleNo
 \param[in] u32RegAddr
 \param[in] u32Value

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_WriteRegister(HI_U32 u32HandleNo, HI_U32 u32RegAddr, HI_U32 u32Value);

/**
 \brief 读寄存器;
 \attention \n
  无

 \param[in] u32HandleNo
 \param[in] u32RegAddr
 \param[out] *pu32Value

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_HAL_ReadRegister(HI_U32 u32HandleNo, HI_U32 u32RegAddr, HI_U32 *pu32Value);

/**
 \brief 按照起始BIT位复制U32参数;
 \attention \n
  无

 \param[in] u32Src
 \param[in] u32SrcStartBit
 \param[out] *pu32Dst
 \param[in] u32DstStartBit
 \param[in] u32Num

 \retval ::

 */
HI_VOID PQ_HAL_CopyU32ByBit(HI_U32 u32Src, HI_U32 u32SrcStartBit, HI_U32 *pu32Dst, HI_U32 u32DstStartBit, HI_U32 u32Num);

/**
 \brief 根据bit位更新U32参数
 \attention \n
  无

 \param[out] pulData,;
 \param[in] ucBitNo

 \retval ::HI_SUCCESS

 */
HI_VOID  PQ_HAL_U32SetBit( HI_U32 *pulData, HI_U8 ucBitNo);

/**
 \brief 将U32对应的bit位清零
 \attention \n
  无

 \param[out] pulData,;
 \param[in] ucBitNo

 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_U32ClearBit( HI_U32 *pulData, HI_U8 ucBitNo);

/**
 \brief 从U32参数根据最高位和最低获取对应的值
 \attention \n
  无

 \param[in]  ulData,  ucMaxBit,  ucMinBit;

 \retval ::HI_SUCCESS

 */
HI_U32 PQ_HAL_GetU32ByBit( HI_U32 ulData, HI_U8 ucMaxBit, HI_U8 ucMinBit);

/**
 \brief 根据最高位和最低位设置U32参数对应的值
 \attention \n
  无

 \param[in] pulData, ucMaxBit, ucMinBit, ulValue;

 \retval ::HI_SUCCESS

 */
HI_VOID PQ_HAL_SetU32ByBit( HI_U32 *pulData, HI_U8 ucMaxBit, HI_U8 ucMinBit, HI_U32 ulValue);
HI_BOOL PQ_HAL_IsVpssReg(HI_U32 u32RegAddr);
HI_BOOL PQ_HAL_IsVdpReg(HI_U32 u32RegAddr);
HI_BOOL PQ_HAL_IsSpecialReg(HI_U32 u32RegAddr);
HI_U32  PQ_HAL_RegRead(volatile HI_U32 *a);
HI_VOID PQ_HAL_RegWrite(volatile HI_U32 *a, HI_U32 value);
HI_S32  PQ_HAL_PrintMsg(HI_U32 type, const HI_S8 *format, ...);
HI_S32  PQ_HAL_SetPrintType(HI_U32 type);
HI_VOID PQ_HAL_CopyRegBySize(HI_U32 *pDstReg, HI_U32 *pSrcReg, HI_U32 u32RegSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __PQ_HAL_COMM_H__ */
