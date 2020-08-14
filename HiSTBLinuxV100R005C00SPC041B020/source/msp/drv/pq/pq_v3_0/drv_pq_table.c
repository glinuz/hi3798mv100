/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_pq_table.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/11/06
  Description   :

******************************************************************************/

#include "pq_hal_comm.h"
#include "hi_drv_proc.h"
#include "drv_pq_table.h"
#include "pq_hal_table_default.h"
#include "hi_math.h"

#define PQ_BIN_MODULE_NUM_MAX 32

DEFINE_SEMAPHORE(g_PqMutex);

#define DRV_PQ_Lock()      \
    do{         \
        if(down_interruptible(&g_PqMutex))   \
        {       \
            HI_ERR_PQ("ERR: PQ intf lock error!\n");  \
        }       \
    }while(0)

#define DRV_PQ_UnLock()      \
    do{         \
        up(&g_PqMutex);    \
    }while(0)


static HI_BOOL sg_bPqTableInitial   = HI_FALSE;
static HI_U32  sg_u32PhyListNum     = PHY_REG_MAX;
static HI_U32  sg_u32MultiListNum   = PHY_REG_MAX;
static PQ_PHY_REG_S* sg_pstPhyReg   = HI_NULL;
static PQ_PHY_REG_S sg_astMultitReg[PHY_REG_MAX];
static HI_PQ_BIN_MODULE_POS_S sg_astBinPos[PQ_BIN_MODULE_NUM_MAX];

static HI_U32 PQ_MNG_FindMoudleOrder(HI_U32 u32Moudle);

/**
 \brief 去初始化pq table;
 \attention \n
无

 \param[in]

 \retval ::HI_SUCCESS

 */
HI_S32 PQ_MNG_DeInitPQTable(HI_VOID)
{
    if (sg_bPqTableInitial == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    sg_bPqTableInitial = HI_FALSE;
    return HI_SUCCESS;
}


/*=======================================================
 *函数名称: PQ_MNG_InitPQTable
 *输入参数:  @pstPqParam		PQ Param参数结构体
 	                    @bDefault	      	是否使用默认的参数
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	                    @HI_FAILURE  失败
 *功能描述: 初始化PQTable；初始化全局变量；初始化Multiple类型的参数；
                          记录每个算法的起始位置；如果PQBin加载失败，使用默认值；
 *注意事项: 无
 *=======================================================*/
HI_S32 PQ_MNG_InitPQTable(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32Order = 0;

    HI_U8 u8SourceMode, u8OutputMode;

    if (sg_bPqTableInitial == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    PQ_CHECK_NULL_PTR(pstPqParam);
    sg_pstPhyReg = pstPqParam->stPQPhyReg;

    sg_u32PhyListNum = sizeof(sg_stPhyReg) / sizeof(PQ_PHY_REG_S);
    if (PHY_REG_MAX < sg_u32PhyListNum)
    {
        sg_u32PhyListNum = PHY_REG_MAX;
        HI_ERR_PQ("Phy Register Num[%d] > Max[%d]\n", sg_u32PhyListNum, PHY_REG_MAX);
        return HI_FAILURE;
    }

    /*set default pqparam form code*/
    if (HI_TRUE == bDefault)
    {
        pstPqParam->stPQFileHeader.u32PhyRegNum = sg_u32PhyListNum;

        memset(sg_pstPhyReg, 0, sizeof(PQ_PHY_REG_S)*PHY_REG_MAX);
        for (i = 0; i < sg_u32PhyListNum; i++)
        {
            sg_pstPhyReg[i] = sg_stPhyReg[i];
        }
    }

    /*init variable about pq table*/
    memset(sg_astMultitReg, 0, sizeof(PQ_PHY_REG_S)*PHY_REG_MAX);
    sg_u32MultiListNum = 0;
    for (i = 0; i < PQ_BIN_MODULE_NUM_MAX; i++)
    {
        sg_astBinPos[i].u32StartPos = PHY_REG_MAX - 1;
        sg_astBinPos[i].u32EndPos   = 0;
    }

    for (i = 0; i < sg_u32PhyListNum; i++)
    {
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        /*init start position and end position of pq bin module*/
        u32Order = PQ_MNG_FindMoudleOrder(sg_pstPhyReg[i].u32Module);
        sg_astBinPos[u32Order].u32StartPos = MIN2(sg_astBinPos[u32Order].u32StartPos, i);
        sg_astBinPos[u32Order].u32EndPos   = MAX2(sg_astBinPos[u32Order].u32EndPos, i);

        /*init multiple pq bin*/
        if ((u8SourceMode > 0) || (u8OutputMode > 0))
        {
            sg_astMultitReg[j++] = sg_pstPhyReg[i];
            sg_u32MultiListNum++;
        }
    }

    sg_bPqTableInitial = HI_TRUE;
    return HI_SUCCESS;
}

/* check Timing Source Mode*/
PQ_SOURCE_MODE_E PQ_MNG_CheckSourceMode(HI_U32 u32Width, HI_U32 u32Height)
{
    if (u32Height < 720)
    {
        return SOURCE_MODE_SD;
    }
    else if (u32Height <= 1920)
    {
        return SOURCE_MODE_HD;
    }

    return SOURCE_MODE_UHD;
}

/* check Timing Output Mode*/
PQ_OUTPUT_MODE_E PQ_MNG_CheckOutputMode(HI_BOOL bSRState)
{
    if (HI_TRUE == bSRState)
    {
        return OUTPUT_MODE_UHD;
    }

    return OUTPUT_MODE_HD;
}


/*=======================================================
 *函数名称: PQ_MNG_UpdatePhyList
 *输入参数:  @u32Addr		PQ Param参数结构体
 	                    @u8Lsb	      	是否使用默认的参数
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	                    @HI_FAILURE  失败
 *功能描述: 初始化PQTable；初始化全局变量；初始化Multiple类型的参数；
                          记录每个算法的起始位置；如果PQBin加载失败，使用默认值；
 *注意事项: 无
 *=======================================================*/
HI_S32 PQ_MNG_UpdatePhyList(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U8 u8SourceMode, HI_U8 u8OutputMode, HI_U32 u32Value)
{
    HI_U32 i;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = 0; i < sg_u32PhyListNum; i++)
    {

        if (u32Addr != sg_pstPhyReg[i].u32RegAddr)
        {
            continue;
        }
        if (u8Lsb != sg_pstPhyReg[i].u8Lsb)
        {
            continue;
        }
        if (u8Msb != sg_pstPhyReg[i].u8Msb)
        {
            continue;
        }
        if ((SOURCE_MODE_NO != sg_pstPhyReg[i].u8SourceMode) && (u8SourceMode != sg_pstPhyReg[i].u8SourceMode))
        {
            continue;
        }
        if ((SOURCE_MODE_NO != sg_pstPhyReg[i].u8OutputMode) && (u8OutputMode != sg_pstPhyReg[i].u8OutputMode))
        {
            continue;
        }

        sg_pstPhyReg[i].u32Value = u32Value;
        return i;
    }

    return HI_FAILURE;
}

HI_S32 PQ_MNG_SetVdpReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32RegValue = 0;

    if (PQ_HAL_CheckVdpValid() == HI_TRUE)
    {
        s32Ret = PQ_HAL_ReadRegister(0, u32Addr, &u32RegValue);
        if (HI_SUCCESS != s32Ret)
        { return HI_FAILURE; }
        PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
        s32Ret = PQ_HAL_WriteRegister(0, u32Addr, u32RegValue);
    }
    else
    {
        HI_ERR_PQ("PQ_HAL_CheckVdpValid false\n");
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetVpssReg(HI_U32 u32Addr, HI_U8 u8Lsb, HI_U8 u8Msb, HI_U32 u32Value)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32RegValue = 0;
    HI_S32 i = 0;

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        if (PQ_HAL_CheckVpssValid(i) == HI_FALSE)
        {
            continue;
        }
        s32Ret = PQ_HAL_ReadRegister(i, u32Addr, &u32RegValue);
        if (HI_SUCCESS != s32Ret)
        { return HI_FAILURE; }
        PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
        s32Ret = PQ_HAL_WriteRegister(i, u32Addr, u32RegValue);
    }

    return s32Ret;
}

/*=======================================================
 *函数名称: PQ_MNG_LoadPhyList
 *输入参数:  @enRegType		vpss or vdp type
 	                    @enSourceMode	video source mode
 	                    @enOutputMode    video output mode
 	                    @enModule           ALG module
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	                    @HI_FAILURE  失败
 *功能描述:  加载PQBin 物理寄存器参数
 *=======================================================*/
HI_S32 PQ_MNG_LoadPhyList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U8 u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = 0; i < sg_u32PhyListNum; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u32Module     = sg_pstPhyReg[i].u32Module;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if ((REG_TYPE_VPSS == enRegType) && (PQ_HAL_IsVpssReg(u32Addr) != HI_TRUE))
        {
            continue;
        }

        if ((REG_TYPE_VDP == enRegType) && (PQ_HAL_IsVdpReg(u32Addr) != HI_TRUE))
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        if (0 == ( enModule & u32Module))
        {
            continue;
        }

        if (PQ_HAL_IsVpssReg(u32Addr) == HI_TRUE)
        {
            s32Ret = PQ_MNG_SetVpssReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }
        else if (PQ_HAL_IsVdpReg(u32Addr) == HI_TRUE)
        {
            s32Ret = PQ_MNG_SetVdpReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }

        pqprint(PQ_PRN_TABLE, "LoadPhyList,Load Addr:0x%x, SourceMode:%d, OutputMode:%d, Module:%d\n", u32Addr, u8SourceMode, u8OutputMode, u32Module);
    }

    return s32Ret;
}

/*=======================================================
 *函数名称: PQ_MNG_LoadMultiList
 *输入参数:  @enRegType		vpss or vdp type
 	                    @enSourceMode	video source mode
 	                    @enOutputMode    video output mode
 	                    @enModule           ALG module
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	                    @HI_FAILURE  失败
 *功能描述:  加载Multiple 类型的PQBin 物理寄存器参数
 *=======================================================*/
HI_S32 PQ_MNG_LoadMultiList(PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U8 u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = 0; i < sg_u32MultiListNum; i++)
    {
        u32Addr       = sg_astMultitReg[i].u32RegAddr;
        u32Value      = sg_astMultitReg[i].u32Value;
        u32Module     = sg_astMultitReg[i].u32Module;
        u8Lsb         = sg_astMultitReg[i].u8Lsb;
        u8Msb         = sg_astMultitReg[i].u8Msb;
        u8SourceMode  = sg_astMultitReg[i].u8SourceMode;
        u8OutputMode  = sg_astMultitReg[i].u8OutputMode;

        if ((REG_TYPE_VPSS == enRegType) && (PQ_HAL_IsVpssReg(u32Addr) != HI_TRUE))
        {
            continue;
        }

        if ((REG_TYPE_VDP == enRegType) && (PQ_HAL_IsVdpReg(u32Addr) != HI_TRUE))
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        if (0 == ( enModule & u32Module))
        {
            continue;
        }

        if (PQ_HAL_IsVpssReg(u32Addr) == HI_TRUE)
        {
            s32Ret = PQ_MNG_SetVpssReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }
        else if (PQ_HAL_IsVdpReg(u32Addr) == HI_TRUE)
        {
            s32Ret = PQ_MNG_SetVdpReg(u32Addr, u8Lsb, u8Msb, u32Value);
        }

        pqprint(PQ_PRN_TABLE, "LoadMultiList,Load Addr:0x%x, SourceMode:%d, OutputMode:%d, Module:%d\n", u32Addr, u8SourceMode, u8OutputMode, u32Module);
    }

    return HI_SUCCESS;
}

/*=======================================================
 *函数名称: PQ_MNG_InitPhyList
 *输入参数:  @u32ID     handle no. or layer ID
                        @enRegType		vpss or vdp type
 	                    @enSourceMode	video source mode
 	                    @enOutputMode   video output mode
 	                    @enModule       ALG module
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	            @HI_FAILURE  失败
 *功能描述: 加载单个算法的PQBin 物理寄存器参数
 *=======================================================*/
HI_S32 PQ_MNG_InitPhyList(HI_U32 u32ID, PQ_REG_TYPE_E enRegType, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, PQ_BIN_MODULE_E enModule)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    S_VDP_REGS_TYPE* pVdpVirReg    = NULL;
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U32 u32RegValue = 0;
    HI_U32 u32OffsetAddr = 0x0;
    HI_U32 u32Order = 0;
    HI_U32 u32StartPos = 0;
    HI_U32 u32EndPos = 0;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    u32Order = PQ_MNG_FindMoudleOrder(enModule);
    u32StartPos = sg_astBinPos[u32Order].u32StartPos;
    u32EndPos = sg_astBinPos[u32Order].u32EndPos;


    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u32Module     = sg_pstPhyReg[i].u32Module;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;
        u32OffsetAddr = u32Addr & REG_OFFSET_ADDR_MASK;

        if (0 == ( enModule & u32Module) )
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        if (PQ_HAL_IsVpssReg(u32Addr) == HI_TRUE)
        {
            pstVpssVirReg = PQ_HAL_GetVpssReg(u32ID);
            PQ_CHECK_NULL_PTR(pstVpssVirReg);

            u32RegValue = *((HI_U32*)((HI_VOID*)pstVpssVirReg + u32OffsetAddr));
            PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
            *(HI_U32*)((HI_VOID*)pstVpssVirReg + u32OffsetAddr) = u32RegValue;
        }
        else if (PQ_HAL_IsVdpReg(u32Addr) == HI_TRUE)
        {
            pVdpVirReg = PQ_HAL_GetVdpReg();
            PQ_CHECK_NULL_PTR(pVdpVirReg);

            u32RegValue = *((HI_U32*)((HI_VOID*)pVdpVirReg + u32OffsetAddr));
            PQ_HAL_SetU32ByBit(&u32RegValue, u8Msb, u8Lsb, u32Value);
            *(HI_U32*)((HI_VOID*)pVdpVirReg + u32OffsetAddr) = u32RegValue;
        }

        pqprint(PQ_PRN_TABLE, "InitPhyList,Load Addr:0x%x, SourceMode:%d, OutputMode:%d, Module:%d\n", u32Addr, u8SourceMode, u8OutputMode, u32Module);
    }

    return HI_SUCCESS;
}

/*=======================================================
 *函数名称: PQ_MNG_PrintTable
 *输入参数:  @enType              打印类型
                               PRN_TABLE_ALL,      所有类型
                               PRN_TABLE_MULTI,  multiple类型
                               PRN_TABLE_SINGLE, 单个地址，地址取决于第二个参数
                           @u32PriAddr		vpss or vdp type

 *输出参数:  无。
 *返回值     :  无
 *功能描述: 打印PQTable 算法参数
 *=======================================================*/
HI_VOID PQ_MNG_PrintTable(PQ_PRN_TABLE_TYPE_E enType, HI_U32 u32PriAddr)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_U32 i;
    HI_U32 u32Addr, u32Value, u32Module;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    HI_U32 u32ListNum = 0;
    PQ_PHY_REG_S* pstReg   = sg_pstPhyReg;

    if (sg_pstPhyReg == HI_NULL)
    {
        return;
    }

    HI_DRV_PROC_EchoHelper("\n");
    HI_DRV_PROC_EchoHelper("List\t");
    HI_DRV_PROC_EchoHelper("Addr\t\t");
    HI_DRV_PROC_EchoHelper("Lsb\t");
    HI_DRV_PROC_EchoHelper("Msb\t");
    HI_DRV_PROC_EchoHelper("SourceMode\t");
    HI_DRV_PROC_EchoHelper("OutputMode\t");
    HI_DRV_PROC_EchoHelper("Module\t");
    HI_DRV_PROC_EchoHelper("Value\t");


    if (PRN_TABLE_MULTI == enType)
    {
        u32ListNum = sg_u32MultiListNum;
        pstReg     = sg_astMultitReg;
    }
    else
    {
        u32ListNum = sg_u32PhyListNum;
        pstReg     = sg_pstPhyReg;
    }

    for (i = 0; i < u32ListNum; i++)
    {
        u32Addr       = pstReg[i].u32RegAddr;
        u32Value      = pstReg[i].u32Value;
        u32Module     = pstReg[i].u32Module;
        u8Lsb         = pstReg[i].u8Lsb;
        u8Msb         = pstReg[i].u8Msb;
        u8SourceMode  = pstReg[i].u8SourceMode;
        u8OutputMode  = pstReg[i].u8OutputMode;

        if ((PRN_TABLE_SINGLE == enType) && (u32PriAddr != u32Addr))
        {
            continue;
        }

        HI_DRV_PROC_EchoHelper("\n");
        HI_DRV_PROC_EchoHelper("[%d]\t", i);
        HI_DRV_PROC_EchoHelper("0x%x\t", u32Addr);
        HI_DRV_PROC_EchoHelper("%d\t", u8Lsb);
        HI_DRV_PROC_EchoHelper("%d\t", u8Msb);
        HI_DRV_PROC_EchoHelper("%d\t\t", u8SourceMode);
        HI_DRV_PROC_EchoHelper("%d\t\t", u8OutputMode);
        HI_DRV_PROC_EchoHelper("0x%x\t", u32Module);
        HI_DRV_PROC_EchoHelper("%d\t", u32Value);

    }

    HI_DRV_PROC_EchoHelper("\n");
    HI_DRV_PROC_EchoHelper("\n");

#endif
}

/*=======================================================
 *函数名称: PQ_MNG_GetSoftTable
 *输入参数:  @u32Lut		      软算法变量查找表ID
 	                    @enSourceMode	video source mode
 	                    @enOutputMode    video output mode
 	                    @u32DefaultValue    默认值
 *输出参数:  无。
 *返回值     :  软算法变量值
 *功能描述:  查找算法表，获取软算法变量的值，如果查找
                           失败，返回默认值
 *=======================================================*/
HI_U32 PQ_MNG_GetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultValue)
{
    HI_S32 i;
    HI_U32 u32Addr, u32Value;
    HI_U8  u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = sg_u32PhyListNum - 1; i > 0; i--)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if (HI_FALSE == PQ_HAL_IsSpecialReg(u32Addr))
        {
            break;
        }

        if (u32Addr != u32Lut)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        return u32Value;

    }

    return u32DefaultValue;

}

/*=======================================================
 *函数名称: PQ_MNG_SetSoftTable
 *输入参数:  @u32Lut		      软算法变量查找表ID
 	                    @enSourceMode	video source mode
 	                    @enOutputMode    video output mode
 	                    @u32Value            软算法变量值
 *输出参数:  无。
 *返回值     :  @HI_SUCCESS 成功
 *	                    @HI_FAILURE  失败
 *功能描述:  设置软算法变量的值
 *=======================================================*/
HI_S32 PQ_MNG_SetSoftTable(HI_U32 u32Lut, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32Value)
{
    HI_S32 i;
    HI_U32 u32Addr;
    HI_U8  u8SourceMode, u8OutputMode;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = sg_u32PhyListNum - 1; i > 0; i--)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if (u32Addr != u32Lut)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        sg_pstPhyReg[i].u32Value = u32Value;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_U32 PQ_MNG_GetArraySoftTable(HI_U32 u32Lut, HI_U32* pArray, HI_U32 u32Num, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode, HI_U32 u32DefaultArray[])
{
    HI_S32 i;
    HI_U32 u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8Widthsb, u8SourceMode, u8OutputMode;
    static HI_U32 u32ArrayNum = 0;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = sg_u32PhyListNum - 1; i > 0; i--)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if (HI_FALSE == PQ_HAL_IsSpecialReg(u32Addr))
        {
            break;
        }

        if (u32Addr != u32Lut)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != enSourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != enOutputMode))
        {
            continue;
        }

        if (u8Lsb > u8Msb)
        {
            break;
        }
        else
        {
            u8Widthsb = u8Msb - u8Lsb + 1;
        }

        if ((u8Lsb / u8Widthsb) > u32Num - 1)
        {
            break;
        }

        *(pArray + (u8Lsb / u8Widthsb)) = u32Value; /* Length need to divide Bits Width */
        u32ArrayNum++;

        /* compare to u32Num, but not u32Num-1, after every loop calc u32ArrayNum++ */
        if (u32ArrayNum >= u32Num)
        {
            u32ArrayNum = 0;
            return HI_SUCCESS;
        }

    }

    memcpy(pArray, u32DefaultArray, u32Num * sizeof(HI_U32));
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetReg(HI_PQ_REGISTER_S* pstAttr, PQ_SOURCE_MODE_E enSourceMode, PQ_OUTPUT_MODE_E enOutputMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32Num = -1;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);
    s32Num = PQ_MNG_UpdatePhyList(pstAttr->u32RegAddr, pstAttr->u8Lsb, pstAttr->u8Msb, \
                                  pstAttr->u8SourceMode, pstAttr->u8OutputMode, pstAttr->u32Value);
    if (HI_FAILURE == s32Num)
    {
        pqprint(PQ_PRN_TABLE, "Warning! not find Register[Address:0x%x, Bit:%d~%d],SourceMode:[%d],OutputMode:[%d]\n", \
                pstAttr->u32RegAddr, pstAttr->u8Lsb, pstAttr->u8Msb, pstAttr->u8SourceMode, pstAttr->u8OutputMode);
    }
    else
    {
        if ((SOURCE_MODE_NO != sg_pstPhyReg[s32Num].u8SourceMode) \
            && (pstAttr->u8SourceMode != enSourceMode))
        {
            HI_ERR_PQ("Current Source Mode:[%d], Set Source Mode:[%d],Not Set Physical Reg\n", \
                      enSourceMode, pstAttr->u8SourceMode);
            return HI_SUCCESS;
        }

        if ((OUTPUT_MODE_NO != sg_pstPhyReg[s32Num].u8OutputMode) \
            && (pstAttr->u8OutputMode != enOutputMode))
        {
            HI_ERR_PQ("Current Output Mode:[%d], Set Output Mode:[%d],Not Set Physical Reg\n", \
                      enOutputMode, pstAttr->u8OutputMode);
            return HI_SUCCESS;
        }
    }

    if (PQ_HAL_IsVpssReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        s32Ret = PQ_MNG_SetVpssReg(pstAttr->u32RegAddr, pstAttr->u8Lsb, \
                                   pstAttr->u8Msb, pstAttr->u32Value);
    }
    else if (PQ_HAL_IsVdpReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        s32Ret = PQ_MNG_SetVdpReg(pstAttr->u32RegAddr, pstAttr->u8Lsb, \
                                  pstAttr->u8Msb, pstAttr->u32Value);
    }
    else if (PQ_HAL_IsSpecialReg(pstAttr->u32RegAddr) == HI_TRUE)
    {
        s32Ret = PQ_MNG_SetSoftTable(pstAttr->u32RegAddr, pstAttr->u8SourceMode, \
                                     pstAttr->u8OutputMode, pstAttr->u32Value);
    }
    else
    {
        HI_ERR_PQ("not VPSS/VDP/Special Register!\n");
        return HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetReg(HI_PQ_REGISTER_S* pstAttr)
{
    HI_U32 u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    HI_U32 i;

    PQ_CHECK_NULL_PTR(sg_pstPhyReg);

    for (i = 0; i < PHY_REG_MAX; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if (u32Addr != pstAttr->u32RegAddr)
        {
            continue;
        }

        if (u8Lsb != pstAttr->u8Lsb)
        {
            continue;
        }

        if (u8Msb != pstAttr->u8Msb)
        {
            continue;
        }

        if ((SOURCE_MODE_NO != u8SourceMode) && (u8SourceMode != pstAttr->u8SourceMode))
        {
            continue;
        }

        if ((OUTPUT_MODE_NO != u8OutputMode) && (u8OutputMode != pstAttr->u8OutputMode))
        {
            continue;
        }

        pstAttr->u32Value = u32Value;
        return HI_SUCCESS;
    }

    HI_ERR_PQ("Error! not find Phy Register List[Address:0x%x, Bit:%d~%d],SourceMode:[%d], OutputMode:[%d]\n", \
              pstAttr->u32RegAddr, pstAttr->u8Lsb, pstAttr->u8Msb, pstAttr->u8SourceMode, pstAttr->u8OutputMode);

    return HI_FAILURE;
}

HI_U32 PQ_MNG_GetPhyListNum(HI_VOID)
{
    return sg_u32PhyListNum;
}

HI_U32 PQ_MNG_GetMultiListNum(HI_VOID)
{
    if (sg_bPqTableInitial == HI_TRUE)
    {
        return sg_u32MultiListNum;
    }

    return sg_u32PhyListNum;
}

HI_S32 PQ_MNG_GetPqMultitReg( PQ_PHY_REG_S** ppstPqReg)
{
    DRV_PQ_Lock();

    if (sg_bPqTableInitial == HI_TRUE)
    {
        *ppstPqReg = sg_astMultitReg;
    }
    else
    {
        *ppstPqReg = sg_pstPhyReg;
    }

    DRV_PQ_UnLock();

    return HI_SUCCESS;
}

static HI_U32 PQ_MNG_FindMoudleOrder(HI_U32 u32Moudle)
{
    HI_U32 u32Order = PQ_BIN_MODULE_NUM_MAX - 1;

    switch (u32Moudle)
    {
        case PQ_BIN_MODULE_DEI:
            u32Order = 0;
            break;
        case PQ_BIN_MODULE_FMD:
            u32Order = 1;
            break;
        case PQ_BIN_MODULE_SHARPEN:
            u32Order = 2;
            break;
        case PQ_BIN_MODULE_DNR:
            u32Order = 3;
            break;
        case PQ_BIN_MODULE_DCI:
            u32Order = 4;
            break;
        case PQ_BIN_MODULE_ACM:
            u32Order = 5;
            break;
        case PQ_BIN_MODULE_HSHARPEN:
            u32Order = 6;
            break;
        case PQ_BIN_MODULE_SR:
            u32Order = 7;
            break;
        case PQ_BIN_MODULE_DB:
            u32Order = 8;
            break;
        case PQ_BIN_MODULE_DM:
            u32Order = 9;
            break;
        case PQ_BIN_MODULE_TNR:
            u32Order = 10;
            break;
        case PQ_BIN_MODULE_DS:
            u32Order = 11;
            break;
        default:
            break;
    }

    return u32Order;
}


