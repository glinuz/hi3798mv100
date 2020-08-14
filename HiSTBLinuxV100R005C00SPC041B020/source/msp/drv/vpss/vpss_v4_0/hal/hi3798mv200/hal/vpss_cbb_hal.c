/******************************************************************************

 Copyright @ Hisilicon Technologies Co., Ltd. 1998-2015. All rights reserved.

 ******************************************************************************
  File Name     : vpss_hal_3798mv200.c
  Version       : Initial Draft
  Author        : z00178013
  Created       : 2016/1/6
  Last Modified :
  Description   : vpss hal 3798mv200 source file
  Function List :
  History       :
  1.Date        : 2016/1/6
    Author      : z00178013
    Modification: Created file

******************************************************************************/
#include "vpss_osal.h"
#include "vpss_reg_struct.h"
#include "vpss_cbb_hal.h"
#include "vpss_cbb_info.h"
#include "vpss_cbb_func.h"
#include "vpss_cbb_frame.h"
#include "vpss_cbb_alg.h"
#include "vpss_cbb_intf.h"
#include "vpss_cbb_reg.h"


/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern HI_VOID VPSS_HAL_Assert(HI_S32 s32NodeId, volatile S_VPSS_REGS_TYPE *vpss_reg, HI_BOOL bForcePrintInfo);

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/
HI_S32 VPSS_HAL_SDDeiBuffer_Alloc(VPSS_IP_E enIP, HI_BOOL bSecure);
HI_VOID VPSS_HAL_SDDeiBuffer_Free(VPSS_IP_E enIP);
HI_S32 VPSS_HAL_Set_TNROutPutCur(CBB_REG_ADDR_S stRegAddr);

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

PF_NODE_CFG_FUNC pfSetNodeInfo[VPSS_HAL_NODE_BUTT] =
{
    VPSS_HAL_Set_2DFrame_Node,
    VPSS_HAL_Set_2DFrame_Node, //virtual
    VPSS_HAL_Set_2DDei_Node,
    VPSS_HAL_Set_2DDei_Node, //virtual
    VPSS_HAL_Set_2DField_Node,
    VPSS_HAL_Set_2DField_Node, //virtual
    VPSS_HAL_Set_2DSDFirst_Node,
    VPSS_HAL_Set_2DSDSecond_Node,
    VPSS_HAL_Set_2DSDSecond_Node, //virtual

    VPSS_HAL_Set_CTS10_Node,
    VPSS_HAL_Set_CTS10_Node, //virtual

    VPSS_HAL_Set_2D_UHDFrame_Node, //4K zme


    VPSS_HAL_Set_3DFrameL_Node,
    VPSS_HAL_Set_3DFrameR_Node,

    VPSS_HAL_Set_RotaY_Node,
    VPSS_HAL_Set_RotaY_Node,//virtual
    VPSS_HAL_Set_RotaC_Node,
    VPSS_HAL_Set_RotaC_Node,//virtual

    VPSS_HAL_Set_HDR_Node, //VPSS_HAL_NODE_HDR_DETILE_NOALG
    VPSS_HAL_SetNode_H265_Step1_Interlace,  //VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE
    VPSS_HAL_SetNode_H265_Step2_Dei,//VPSS_HAL_NODE_2D_H265_STEP2_DEI

    VPSS_HAL_Set_2DDei_Last2Field_Node,
    VPSS_HAL_Set_2DDei_Last2Field_Node
};

VPSS_HAL_CTX_S  stHalCtx[VPSS_IP_BUTT] =
{
    {
        .bInit = HI_FALSE,
        .bClockEn = HI_FALSE,
        .u32LogicVersion = HAL_VERSION_3798M,
        .u32BaseRegPhy   = VPSS0_BASE_ADDR,
        .pu8BaseRegVir   = 0,
    },

    {
        .bInit = HI_FALSE,
        .bClockEn = HI_FALSE,
        .u32LogicVersion = HAL_VERSION_3798M,
        .u32BaseRegPhy   = VPSS1_BASE_ADDR,
        .pu8BaseRegVir   = 0,
    }
};


/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

#define VPSS_HAL_ENABLE_NODE(nodeid) do {\
        VPSS_DBG_INFO("Enable node:%s\n", (#nodeid));\
        bNodeList[(nodeid)] = HI_TRUE;\
    } while (0)

#define VPSS_HAL_DISABLE_NODE(nodeid) do {\
        VPSS_DBG_INFO("Disable node:%s\n", (#nodeid));\
        bNodeList[(nodeid)] = HI_FALSE;\
    } while (0)


#define VPSS_PIXEL_FMT_420(pix) (\
                                 (HI_DRV_PIX_FMT_NV12 == (pix)) || (HI_DRV_PIX_FMT_NV21 == (pix)) \
                                 || (HI_DRV_PIX_FMT_NV12_CMP == (pix)) || (HI_DRV_PIX_FMT_NV21_CMP == (pix)) \
                                 || (HI_DRV_PIX_FMT_NV12_TILE == (pix)) || (HI_DRV_PIX_FMT_NV12_TILE == (pix)))

#define VPSS_HAL_REG_BLOCK_SIZE 256     //print 256bytes per block
#define VPSS_HAL_REG_LINE_SIZE  16      //print 16bytes per line
#define VPSS_HAL_REG_UNIT_SIZE  4       //4bytes per reg

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/


HI_S32 VPSS_HAL_Init(VPSS_IP_E enIP)
{
    HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (pstHalCtx->bInit)
    {
        VPSS_INFO("VPSS IP%d, Already Init\n", enIP);
        return HI_SUCCESS;
    }

    /* 申请和分配NODE节点 */
    s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_RegBuf", HI_NULL,
                                    VPSS_REG_SIZE * VPSS_HAL_NODE_BUTT, 0, &pstHalCtx->stRegBuf);

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_RegBuf Failed\n");
        return HI_FAILURE;
    }

    for (i = 0; i < VPSS_HAL_NODE_BUTT; i++)
    {
        pstHalCtx->au32AppPhy[i] = pstHalCtx->stRegBuf.u32StartPhyAddr
                                   + VPSS_REG_SIZE * i;

        pstHalCtx->apu8AppVir[i] = pstHalCtx->stRegBuf.pu8StartVirAddr
                                   + VPSS_REG_SIZE * i;

        VPSS_DBG_INFO("VPSS Node:%d phy:%#x vir:%#x  size1:%u size2:%u\n", i,
                      pstHalCtx->au32AppPhy[i],
                      (HI_U32)(long)pstHalCtx->apu8AppVir[i],
                      (HI_U32)sizeof(VPSS_REG_S),
                      (HI_U32)VPSS_REG_SIZE);
    }


    /* 映射寄存器地址 */
    // pstHalCtx->pu32BaseRegVir = IO_ADDRESS(pstHalCtx->u32BaseRegPhy);

    pstHalCtx->pu8BaseRegVir
        = ioremap_nocache(pstHalCtx->u32BaseRegPhy, VPSS_REG_SIZE);

    if (HI_NULL == pstHalCtx->pu8BaseRegVir)
    {
        VPSS_FATAL("VPSS io_address VPSS_REG(%#x) Failed\n", pstHalCtx->u32BaseRegPhy);
        HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
        return HI_FAILURE;
    }
    else
    {
        VPSS_DBG_INFO("VPSS regmap  phy:%#x-> vir:%#x\n", pstHalCtx->u32BaseRegPhy, (unsigned int)(long)pstHalCtx->pu8BaseRegVir);
    }

#ifdef VPSS_SUPPORT_OUT_TUNNEL

#ifdef HI_VPSS_SMMU_SUPPORT
    s32Ret = HI_DRV_SMMU_AllocAndMap("VPSS_LowDelay_mmu",
                                     4096, 0, &pstHalCtx->stLowDelayBuf_MMU);

    VPSS_DBG_INFO("VPSS alloc LowDelayBuf smmu:%#x vir:%#x\n",
               pstHalCtx->stLowDelayBuf_MMU.u32StartSmmuAddr,
               pstHalCtx->stLowDelayBuf_MMU.pu8StartVirAddr);

#else
    s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_LowDelay_mmz",
                                    4096, 0, &pstHalCtx->stLowDelayBuf_MMZ);

    VPSS_DBG_INFO("VPSS alloc LowDelayBuf mmz:%#x vir:%#x\n",
               pstHalCtx->stLowDelayBuf_MMZ.u32StartPhyAddr,
               pstHalCtx->stLowDelayBuf_MMZ.pu8StartVirAddr);
#endif

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_LowDelayBuf Failed\n");
        HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
        return HI_FAILURE;
    }
#endif

    pstHalCtx->bInit = HI_TRUE;

    VPSS_HAL_SetClockEn(enIP, HI_FALSE);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_DeInit(VPSS_IP_E enIP)
{
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (!pstHalCtx->bInit)
    {
        VPSS_INFO("VPSS IP%d, Already DeInit\n", enIP);
        return HI_SUCCESS;
    }

    VPSS_HAL_SetClockEn(enIP, HI_FALSE);

    HI_ASSERT(pstHalCtx->pu8BaseRegVir != 0);

    if (pstHalCtx->pu8BaseRegVir != 0)
    {
        iounmap(pstHalCtx->pu8BaseRegVir);
        pstHalCtx->pu8BaseRegVir = 0;
    }

    HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
    memset(&pstHalCtx->stRegBuf, 0, sizeof(MMZ_BUFFER_S));
#ifdef VPSS_SUPPORT_OUT_TUNNEL
#ifdef HI_VPSS_SMMU_SUPPORT
    HI_DRV_SMMU_UnmapAndRelease(&pstHalCtx->stLowDelayBuf_MMU);
    memset(&(pstHalCtx->stLowDelayBuf_MMU), 0x0, sizeof(pstHalCtx->stLowDelayBuf_MMU));
#else
    HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stLowDelayBuf_MMZ);
    memset(&(pstHalCtx->stLowDelayBuf_MMZ), 0x0, sizeof(pstHalCtx->stLowDelayBuf_MMZ));
#endif
#endif
    VPSS_HAL_SDDeiBuffer_Free(enIP);

    pstHalCtx->bInit = HI_FALSE;

    return HI_SUCCESS;
}

HI_VOID VPSS_HAL_DumpReg_print(HI_ULONG ulBaseAddr, HI_ULONG ulStartAddr, HI_ULONG ulEndAddr, HI_BOOL bOnlyNoZero)
{
    HI_ULONG i;
    HI_U32   u32RegValue;

    for (i = ulStartAddr; i < ulEndAddr; i+=VPSS_HAL_REG_UNIT_SIZE)
    {
        if (i % (VPSS_HAL_REG_BLOCK_SIZE) == 0)
        {
            printk("\n-------------------- 0x%.4x --------------------", (HI_U32)i);
        }

        if (i % VPSS_HAL_REG_LINE_SIZE == 0)
        {
            printk("\n0x%.4x: ", (HI_U32)i);
        }

        u32RegValue = *((HI_U32*)(ulBaseAddr + i));

        if ((0x0 == u32RegValue) && (bOnlyNoZero))
        {
            printk("         ");
        }
        else
        {
            printk("%.8x ", u32RegValue);
        }
    }

    return;
}

HI_VOID VPSS_HAL_DumpReg(VPSS_IP_E enIP, VPSS_HAL_NODE_TYPE_E enNodeId, HI_BOOL bRegValue)
{
    HI_ULONG ulRegBase;
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;
    HI_BOOL  bDoNotPrintZero = HI_FALSE;
    pstHalCtx = &stHalCtx[enIP];

    printk("\n####################################");
    if (bRegValue)
    {
        ulRegBase = (HI_ULONG)(pstHalCtx->pu8BaseRegVir);
        printk("\nDump Reg value PhyAddr %08x VirAddr %8lx\n",
            pstHalCtx->u32BaseRegPhy, ulRegBase);
    }
    else
    {
        ulRegBase = (HI_ULONG)(pstHalCtx->apu8AppVir[enNodeId]);
        printk("\nDump Config in DDR, node:%d PhyAddr %8lx VirAddr %8lx\n",
           enNodeId,
           (unsigned long)pstHalCtx->au32AppPhy[enNodeId],
           (unsigned long)pstHalCtx->apu8AppVir[enNodeId]);
    }
    printk("\n####################################");

    VPSS_HAL_DumpReg_print(ulRegBase, 0x0, 0x100, bDoNotPrintZero);

    VPSS_HAL_DumpReg_print(ulRegBase, 0x2F8, 0x600, bDoNotPrintZero);

    VPSS_HAL_DumpReg_print(ulRegBase, 0x1000, 0x11A0, bDoNotPrintZero);

    VPSS_HAL_DumpReg_print(ulRegBase, 0x2000, 0x2400, bDoNotPrintZero);

    VPSS_HAL_DumpReg_print(ulRegBase, 0x3800, 0x3B00, bDoNotPrintZero);

}
#ifdef VPSS_SUPPORT_PROC_V2
HI_S32 VPSS_HAL_GetNodeAddr(VPSS_IP_E enIP, HI_U32 u32NodeId, HI_U8 **pstRegAddr)
{
    VPSS_HAL_CTX_S *pstHalCtx;

    pstHalCtx = &stHalCtx[enIP];
    if (u32NodeId >= VPSS_HAL_NODE_BUTT)
    {
        return HI_FAILURE;
    }
    *pstRegAddr = pstHalCtx->apu8AppVir[u32NodeId];
    return HI_SUCCESS;
}
#endif
HI_S32 VPSS_HAL_H265RefListDeInit(VPSS_HAL_RefList_S *pstRefList)
{
	HI_S32 s32Idx;
    HI_U32 u32BaseAddr = 0;
#ifdef HI_VPSS_SMMU_SUPPORT
#ifdef HI_TEE_SUPPORT
    HI_S32 s32Ret = HI_SUCCESS;
#endif
#endif

	if (pstRefList->bRefListValid)
	{
#ifdef HI_VPSS_SMMU_SUPPORT
#ifdef HI_TEE_SUPPORT
        if (0 != pstRefList->stRefListBuf_tee.u32Size)
        {
            u32BaseAddr = pstRefList->stRefListBuf_tee.u32StartSmmuAddr;
            s32Ret = HI_DRV_SECSMMU_Release(&(pstRefList->stRefListBuf_tee));
            if (s32Ret != HI_SUCCESS)
            {
               VPSS_FATAL("Free VPSS_H265RefBuf_TEE Failed\n");
               return HI_FAILURE;
            }
    		memset(&pstRefList->stRefListBuf_tee, 0, sizeof(pstRefList->stRefListBuf_tee));
        }
#endif
        if (0 != pstRefList->stRefListBuf_mmu.u32Size)
        {
            u32BaseAddr = pstRefList->stRefListBuf_mmu.u32StartSmmuAddr;
            HI_DRV_SMMU_Release(&(pstRefList->stRefListBuf_mmu));
            memset(&pstRefList->stRefListBuf_mmu, 0, sizeof(pstRefList->stRefListBuf_mmu));
        }
#else
        u32BaseAddr = pstRefList->stRefListBuf_mmz.u32StartPhyAddr;
        HI_DRV_MMZ_Release(&(pstRefList->stRefListBuf_mmz));
        memset(&pstRefList->stRefListBuf_mmz, 0, sizeof(pstRefList->stRefListBuf_mmz));
#endif

        VPSS_DBG_INFO("VPSS_HAL_H265RefListDeInit free %x\n", u32BaseAddr);

		for (s32Idx = 0; s32Idx < DEF_VPSS_HAL_REF_LIST_NUM; s32Idx++)
		{
			memset(&(pstRefList->stRefListAddr[s32Idx]), 0x0, sizeof(pstRefList->stRefListAddr[s32Idx]));
			pstRefList->abRefNodeValid[s32Idx] = HI_FALSE;
		}
	}

	pstRefList->bRefListValid = HI_FALSE;
	pstRefList->u32RefListHead = 0;

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_H265RefListInit(VPSS_HAL_RefList_S *pstRefList, HI_S32 s32Width, HI_S32 s32Height, HI_DRV_PIX_FORMAT_E
enFormat, HI_BOOL bSecure)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32StartAddr;
    HI_U32 u32YSize;
    HI_U32 u32FstFrameOffset;
	HI_U32 u32FieldIdx;

	if (pstRefList->bRefListValid)
	{
		VPSS_DBG_INFO("VPSS_HAL_H265RefListInit has been called!\n");

		VPSS_HAL_H265RefListDeInit(pstRefList);
		pstRefList->bRefListValid = HI_FALSE;
	}

	if ((HI_DRV_PIX_FMT_NV12 != enFormat)
			&& (HI_DRV_PIX_FMT_NV21 != enFormat)
			&& (HI_DRV_PIX_FMT_NV12_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV21_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV12_TILE_CMP != enFormat)
			&& (HI_DRV_PIX_FMT_NV21_TILE_CMP != enFormat))
	{
		VPSS_FATAL("Unsupport pixel format:%d\n", enFormat);
		return HI_FAILURE;
	}

#ifdef HI_VPSS_SMMU_SUPPORT
    if (bSecure)
    {
#ifdef HI_TEE_SUPPORT
        s32Ret = HI_DRV_SECSMMU_Alloc("VPSS_H265RefBuf_TEE",
                s32Width*s32Height * 3 / 2 * DEF_VPSS_HAL_REF_LIST_NUM,
                0,
                &(pstRefList->stRefListBuf_tee));
        u32StartAddr = pstRefList->stRefListBuf_tee.u32StartSmmuAddr;
#else
        s32Ret = HI_DRV_SMMU_Alloc( "VPSS_H265RefBuf_MMU",
                s32Width*s32Height * 3 / 2 * DEF_VPSS_HAL_REF_LIST_NUM,
                0,
                &(pstRefList->stRefListBuf_mmu));
        u32StartAddr = pstRefList->stRefListBuf_mmu.u32StartSmmuAddr;
#endif
    }
    else
    {
        s32Ret = HI_DRV_SMMU_Alloc("VPSS_H265RefBuf_MMU",
                s32Width*s32Height * 3 / 2 * DEF_VPSS_HAL_REF_LIST_NUM,
                0,
                &(pstRefList->stRefListBuf_mmu));
        u32StartAddr = pstRefList->stRefListBuf_mmu.u32StartSmmuAddr;
    }

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_H265RefBuf_MMU Failed\n");
        return HI_FAILURE;
    }

#else
    s32Ret = HI_DRV_MMZ_Alloc("VPSS_H265RefBuf_MMZ",
                        "VPSS",
                        s32Width*s32Height * 3 / 2 * DEF_VPSS_HAL_REF_LIST_NUM,
                        0,
                        &(pstRefList->stRefListBuf_mmz));
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_H265RefBuf_MMZ Failed\n");
        return HI_FAILURE;
    }
    u32StartAddr = pstRefList->stRefListBuf_mmz.u32StartPhyAddr;
#endif

    VPSS_DBG_INFO("VPSS_HAL_H265RefListInit malloc W H adr %d %d %x\n", s32Width, s32Height, u32StartAddr);

	for (u32FieldIdx = 0; u32FieldIdx < DEF_VPSS_HAL_REF_LIST_NUM; u32FieldIdx++)
	{
		pstRefList->abRefNodeValid[u32FieldIdx] = HI_FALSE;
	}

    u32YSize = s32Width*s32Height*2;
    pstRefList->stRefListAddr[0].u32PhyAddr_Y = u32StartAddr;
    pstRefList->stRefListAddr[0].u32PhyAddr_C = u32StartAddr + u32YSize;
    pstRefList->stRefListAddr[0].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[0].u32Stride_C = s32Width;

    pstRefList->stRefListAddr[1].u32PhyAddr_Y = u32StartAddr + s32Width;
    pstRefList->stRefListAddr[1].u32PhyAddr_C = u32StartAddr + u32YSize + s32Width;
    pstRefList->stRefListAddr[1].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[1].u32Stride_C = s32Width;

	u32FstFrameOffset = s32Width*s32Height*3;

    pstRefList->stRefListAddr[2].u32PhyAddr_Y = u32StartAddr + u32FstFrameOffset;
    pstRefList->stRefListAddr[2].u32PhyAddr_C = u32StartAddr + u32FstFrameOffset + u32YSize;
    pstRefList->stRefListAddr[2].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[2].u32Stride_C = s32Width;

    pstRefList->stRefListAddr[3].u32PhyAddr_Y = u32StartAddr + u32FstFrameOffset + s32Width;
    pstRefList->stRefListAddr[3].u32PhyAddr_C = u32StartAddr + u32FstFrameOffset + u32YSize + s32Width;
    pstRefList->stRefListAddr[3].u32Stride_Y = s32Width;
    pstRefList->stRefListAddr[3].u32Stride_C = s32Width;

	pstRefList->bRefListValid = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetClockEn(VPSS_IP_E enIP, HI_BOOL bClockEn)
{
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    if (pstHalCtx->bClockEn == bClockEn)
    {
        return HI_SUCCESS;
    }

    if (bClockEn)
    {
        VPSS_DRV_SetClockEn(HI_TRUE);
        VPSS_DRV_Set_VPSS_MISCELLANEOUS((S_VPSS_REGS_TYPE*)pstHalCtx->pu8BaseRegVir,
												0x0,
												0x1,
												0x1,
												0x64,
												0x6,
												0x6);

        VPSS_DRV_SetTimeOut((S_VPSS_REGS_TYPE*)pstHalCtx->pu8BaseRegVir, DEF_LOGIC_TIMEOUT);
        VPSS_DRV_SetIntMask((S_VPSS_REGS_TYPE*)pstHalCtx->pu8BaseRegVir, 0x3e);
        VPSS_DRV_ClrInt((S_VPSS_REGS_TYPE*)pstHalCtx->pu8BaseRegVir, 0xf);
    }
    else
    {
        VPSS_DRV_SetClockEn(HI_FALSE);
    }

    pstHalCtx->bClockEn = bClockEn;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_StartLogic(VPSS_IP_E enIP, HI_BOOL abNodeVaild[VPSS_HAL_NODE_BUTT])
{
    VPSS_HAL_CTX_S* pstHalCtx;
    CBB_START_CFG_S stStartCfg;
    CBB_REG_ADDR_S  stRegAddr;
    HI_S32			s32NodeIdx;
    CBB_AXI_CFG_S   stAxiCfg;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(abNodeVaild);
    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    VPSS_MEM_CLEAR(stStartCfg);
    VPSS_MEM_CLEAR(stRegAddr);

    stRegAddr.pu8BaseVirAddr = pstHalCtx->pu8BaseRegVir;
    stRegAddr.u32BasePhyAddr = pstHalCtx->u32BaseRegPhy;

    for (s32NodeIdx = 0; s32NodeIdx < VPSS_HAL_NODE_BUTT; s32NodeIdx++)
    {
        stStartCfg.u32NodePhyAddr[s32NodeIdx] = pstHalCtx->au32AppPhy[s32NodeIdx];
        stStartCfg.pu8NodeVirAddr[s32NodeIdx] = pstHalCtx->apu8AppVir[s32NodeIdx];
        stStartCfg.u32NodeValid[s32NodeIdx] = abNodeVaild[s32NodeIdx];
    }

    stAxiCfg.stOutStanding.u32ROutStd = 0x0F;
    stAxiCfg.stOutStanding.u32WOutStd = 0x07;
    stAxiCfg.eBurstLength             = AXI_BURST_LEN_16;

    VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_CONFIG_AXI, (HI_VOID*)&stAxiCfg, HI_NULL);
    VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_START_LOGIC, (HI_VOID*)&stStartCfg, HI_NULL);

    return HI_SUCCESS;
}

HI_BOOL VPSS_HAL_NeedStartVirtual(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo)
{
    HI_U32 u32PortId;
    VPSS_HAL_PORT_INFO_S* pstPortInfo;
    HI_U32 u32PortCnt;

    //search from port0 to DEF_HI_DRV_VPSS_PORT_MAX_NUMBER
    for (u32PortId = 0, u32PortCnt = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
    {
        pstPortInfo = &(pstHalInfo->astPortInfo[u32PortId]);

        if (pstPortInfo->bEnable == HI_TRUE
            && pstPortInfo->bConfig == HI_FALSE)
        {
            u32PortCnt++;
        }
    }

    switch (u32PortCnt)
    {
        case 1:
        {
            return HI_FALSE;
        }

        case 2:
        {
            return HI_TRUE;
        }

        default:
        {
            VPSS_FATAL("Invalid port count:%d\n", u32PortCnt);
        }
        break;
    }

    return HI_FALSE;
}

HI_BOOL VPSS_HAL_NeedRotation(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo)
{
    VPSS_HAL_PORT_INFO_S*	pstPortInfo;
    HI_S32 					s32PortId;
    HI_BOOL					bNeedRota;

    bNeedRota = HI_FALSE;

    for ( s32PortId = 0; s32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; s32PortId++ )
    {
        pstPortInfo = &(pstHalInfo->astPortInfo[s32PortId]);

        if (((HI_DRV_VPSS_ROTATION_90 == pstPortInfo->enRotation)
             || (HI_DRV_VPSS_ROTATION_270 == pstPortInfo->enRotation))
            && (pstPortInfo->bEnable) ) // && (!pstPortInfo->bConfig))
        {
            pstPortInfo->bNeedRota	= HI_TRUE;
            bNeedRota 				= HI_TRUE;
        }
        else
        {
            pstPortInfo->bNeedRota = HI_FALSE;
        }
    }

    return bNeedRota;
}

HI_BOOL	VPSS_HAL_RotaBufConfig(HI_DRV_VIDEO_FRAME_S* pstFrm, VPSS_BUFFER_S* pstBuf)
{
    HI_U32 u32PhyAddr, u32Stride;
    HI_DRV_BUF_ADDR_E enBufLR = HI_DRV_BUF_ADDR_LEFT;

    /* 填充地址信息 */
    u32PhyAddr = pstBuf->stMemBuf.u32StartPhyAddr;
    u32Stride  = pstBuf->u32Stride;

    pstFrm->stBufAddr[enBufLR].u32Stride_Y	=  u32Stride;
    pstFrm->stBufAddr[enBufLR].u32Stride_C	=  u32Stride;


    /* stride 在buffer申请时是使用height计算的, 此时计算偏移时stride*width */

    if (pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP
        || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1_CMP
        || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1_CMP
       )
    {
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_YHead = u32PhyAddr;
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_Y =
            pstFrm->stBufAddr[enBufLR].u32PhyAddr_YHead + pstFrm->u32Height * 16;
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_CHead =
            pstFrm->stBufAddr[enBufLR].u32PhyAddr_Y + u32Stride * pstFrm->u32Height;

        if (pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV12_CMP
            || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP)
        {
            pstFrm->stBufAddr[enBufLR].u32PhyAddr_C =
                pstFrm->stBufAddr[enBufLR].u32PhyAddr_CHead + pstFrm->u32Height * 16 / 2;
        }
        else
        {
            pstFrm->stBufAddr[enBufLR].u32PhyAddr_C =
                pstFrm->stBufAddr[enBufLR].u32PhyAddr_CHead + pstFrm->u32Height * 16;
        }
    }
    else if (pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV12
             || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV21
             || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1
             || pstFrm->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1)
    {
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_Y =  u32PhyAddr;
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_C =  u32PhyAddr + u32Stride * pstFrm->u32Height;
        pstFrm->stBufAddr[enBufLR].vir_addr_y = (HI_U64)(HI_SIZE_T)(pstBuf->stMemBuf.pu8StartVirAddr);
        pstFrm->stBufAddr[enBufLR].vir_addr_c = (HI_U64)(HI_SIZE_T)
                                                (pstBuf->stMemBuf.pu8StartVirAddr + u32Stride * pstFrm->u32Height);
    }
    else if (pstFrm->ePixFormat == HI_DRV_PIX_FMT_ARGB8888
             || pstFrm->ePixFormat == HI_DRV_PIX_FMT_ABGR8888)
    {
        pstFrm->stBufAddr[enBufLR].u32PhyAddr_Y =  u32PhyAddr;
    }
    else
    {
        VPSS_FATAL("Invalid Out pixFormat %d,can't get addr\n",
                   pstFrm->ePixFormat);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_BOOL VPSS_HAL_AllocateRotaBuf(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo)
{
    VPSS_HAL_PORT_INFO_S*	pstPortInfo;
    HI_S32 					s32PortId;
    HI_BOOL					bNeedRota;
    HI_U32					u32BufSize = 0;
    HI_U32					u32BufStride = 0;
    HI_S32					s32Ret;
    VPSS_FB_NODE_S*			 pstBuf;


    bNeedRota = HI_FALSE;

    for ( s32PortId = 0; s32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; s32PortId++ )
    {
        pstPortInfo = &(pstHalInfo->astPortInfo[s32PortId]);
        pstBuf = &(pstPortInfo->stFrmNodeRoBuf);

        if ((pstPortInfo->bEnable) && (!pstPortInfo->bConfig) && (pstPortInfo->bNeedRota))
        {
            /* 以Node1(W*H->ZME(H*W))时的输出的信息为依据, 此时stride要根据H计算.  Node2 H*W->W*H */
            VPSS_OSAL_CalBufSize(&u32BufSize,
                                 &u32BufStride,
                                 pstPortInfo->stOutInfo.u32Width,
                                 pstPortInfo->stOutInfo.u32Height,
                                 pstPortInfo->stOutInfo.enFormat,
                                 pstPortInfo->stOutInfo.enBitWidth);

            if ((pstBuf->stBuffer.stMemBuf.u32Size != u32BufSize)
                || (pstBuf->stBuffer.u32Stride != u32BufStride))
            {
                if (pstBuf->stBuffer.stMemBuf.u32Size != 0)
                {
                    VPSS_OSAL_FreeMem(&(pstBuf->stBuffer.stMemBuf));
                }

                if (!pstHalInfo->stInInfo.bSecure)
                {
                    s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL, u32BufSize,
                                                   HI_NULL,
                                                   "VPSS_RotaBuf_ns",
                                                   &(pstBuf->stBuffer.stMemBuf));
                }
                else
                {
                    s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE, u32BufSize,
                                                   HI_NULL,
                                                   "VPSS_RotaBuf_s",
                                                   &(pstBuf->stBuffer.stMemBuf));
                }

                if (s32Ret != HI_SUCCESS)
                {
                    VPSS_FATAL("Alloc VPSS_RotaBuf_%s Failed\n",
                               (pstHalInfo->stInInfo.bSecure) ? "secure" : "normal");

                    return HI_FAILURE;
                }

                pstBuf->stBuffer.u32Stride = u32BufStride;

                pstBuf->stOutFrame.u32Width  = pstPortInfo->stOutInfo.u32Height;
                pstBuf->stOutFrame.u32Height = pstPortInfo->stOutInfo.u32Width;
                pstBuf->stOutFrame.enBitWidth = pstPortInfo->stOutInfo.enBitWidth;
                pstBuf->stOutFrame.ePixFormat = pstPortInfo->stOutInfo.enFormat;

                VPSS_DBG_INFO("RotaBuf  malloc W H Sride %d %d %d\n",
                              pstBuf->stOutFrame.u32Width,
                              pstBuf->stOutFrame.u32Height,
                              u32BufStride);

                pstBuf->stOutFrame.ePixFormat = pstPortInfo->stOutInfo.enFormat;
                VPSS_HAL_RotaBufConfig(&(pstBuf->stOutFrame), &(pstBuf->stBuffer));
            }
        }
        else
        {
            if (pstBuf->stBuffer.stMemBuf.u32Size != 0)
            {
                VPSS_OSAL_FreeMem(&(pstBuf->stBuffer.stMemBuf));
                pstBuf->stBuffer.stMemBuf.u32Size = 0;
                pstBuf->stBuffer.u32Stride = 0;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Task2Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, VPSS_HAL_TASK_TYPE_E enTaskNodeId, HI_BOOL bNodeList[VPSS_HAL_NODE_BUTT])
{
    HI_S32	s32NodeId;
    HI_BOOL	bNeedVirtualNode;
    HI_BOOL bNeedRota;

    for (s32NodeId = 0; s32NodeId < VPSS_HAL_NODE_BUTT; s32NodeId++)
    {
        bNodeList[s32NodeId] = HI_FALSE;
    }

    bNeedVirtualNode = HI_FALSE;

    if (VPSS_HAL_NeedStartVirtual(enIP, pstHalInfo))
    {
        bNeedVirtualNode = HI_TRUE;
    }

    bNeedRota = HI_FALSE;

    if (VPSS_HAL_NeedRotation(enIP, pstHalInfo))
    {
        bNeedRota = HI_TRUE;
    }


#if 0 //debug use
    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_DEI);
    return HI_SUCCESS;
#endif
#if defined(HI_NVR_SUPPORT)
    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_UHD);
    return HI_SUCCESS;
#endif

    if (VPSS_HAL_TASK_NODE_H265_DEI == enTaskNodeId)
    {
        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE);
        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_H265_STEP2_DEI);

        if (bNeedRota)
        {
            if (HI_SUCCESS == VPSS_HAL_AllocateRotaBuf(enIP, pstHalInfo))
            {
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_Y);
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_C);
            }
        }
        return HI_SUCCESS;
    }

    if (VPSS_HAL_TASK_NODE_HDR != enTaskNodeId)
    {
        if ((pstHalInfo->stInInfo.u32Width > 1920)
            && (HI_DRV_PIXEL_BITWIDTH_10BIT_CTS != pstHalInfo->astPortInfo[0].stOutInfo.enBitWidth))
        {
            VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_UHD);

            if (bNeedRota)
            {
                if (HI_SUCCESS == VPSS_HAL_AllocateRotaBuf(enIP, pstHalInfo))
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_Y);
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_C);
                }
            }

            return HI_SUCCESS;
        }
    }

    switch ( enTaskNodeId )
    {
        case VPSS_HAL_TASK_NODE_2D_FIELD:
            if ((HI_FALSE == pstHalInfo->stInInfo.bProgressive)
                && VPSS_ALG_IS_SD(pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height))
            {
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_SD_FIRST);
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_SD_SECOND);

                if (bNeedVirtualNode)
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_SD_VIRTUAL);
                }
            }
            else
            {
                if (pstHalInfo->stInInfo.bProgressive)
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FIELD);

                    if (bNeedVirtualNode)
                    {
                        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FIELD_VIRTUAL);
                    }
                }
                else
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_DEI);

                    if (bNeedVirtualNode)
                    {
                        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_DEI_VIRTUAL);
                    }
                }
            }

            if (bNeedRota)
            {
                if (HI_SUCCESS == VPSS_HAL_AllocateRotaBuf(enIP, pstHalInfo))
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_Y);
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_C);
                }
            }

            break;

        case VPSS_HAL_TASK_NODE_2D_FRAME:
            if (HI_DRV_PIXEL_BITWIDTH_10BIT_CTS == pstHalInfo->astPortInfo[0].stOutInfo.enBitWidth)
            {
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_CTS10);

                if (bNeedVirtualNode)
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_CTS10_VIRTUAL);
                }
            }
            else if ((HI_DRV_PIXEL_BITWIDTH_8BIT == pstHalInfo->stInInfo.enBitWidth)
                     || (HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth))

            {
                VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FRAME);

                if (bNeedVirtualNode)
                {
                    VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FRAME_VIRTUAL);
                }

                if (bNeedRota)
                {
                    if (HI_SUCCESS == VPSS_HAL_AllocateRotaBuf(enIP, pstHalInfo))
                    {
                        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_Y);
                        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_ROTATION_C);
                    }
                }
            }
            break;

        case VPSS_HAL_TASK_NODE_3D:
            VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_3D_FRAME_R);
            break;

		case VPSS_HAL_TASK_NODE_HDR:
	        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_HDR_DETILE_NOALG);
			break;
		case VPSS_HAL_TASK_LOWQUALITY:
	        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FIELD);
			if (bNeedVirtualNode)
			{
				VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_FIELD_VIRTUAL);
			}
			break;
        case VPSS_HAL_TASK_NODE_2D_LAST2FIELD:
	        VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_DEI_LAST_2FIELD);
			if (bNeedVirtualNode)
			{
				VPSS_HAL_ENABLE_NODE(VPSS_HAL_NODE_2D_DEI_LAST_2FIELD_VIRTUAL);
			}
            break;
        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetTaskInfo(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, VPSS_HAL_TASK_TYPE_E enTaskId,
                            HI_BOOL	bNodeList[VPSS_HAL_NODE_BUTT])
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;
    HI_U32*  pu32AppVir, u32AppPhy;
    HI_S32	s32NodeId;
    HI_BOOL	abNodeList[VPSS_HAL_NODE_BUTT];

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);
    VPSS_HAL_CHECK_NODE_ID_VAILD(enTaskId);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    s32Ret = VPSS_HAL_Task2Node(enIP, pstHalInfo, enTaskId, abNodeList);
    if (HI_SUCCESS != s32Ret)
    {
		VPSS_FATAL("Task %d translate to node fail!\n", enTaskId);
    }

    for (s32NodeId = 0; s32NodeId < VPSS_HAL_NODE_BUTT; s32NodeId++)
    {
        if (abNodeList[s32NodeId])
        {
            pu32AppVir = (HI_U32*)pstHalCtx->apu8AppVir[s32NodeId];
            u32AppPhy = pstHalCtx->au32AppPhy[s32NodeId];

            bNodeList[s32NodeId] = HI_TRUE;

            if (HI_NULL != pfSetNodeInfo[s32NodeId])
            {
                pstHalInfo->enNodeType = s32NodeId;
                s32Ret = pfSetNodeInfo[s32NodeId](enIP, pstHalInfo, pu32AppVir, u32AppPhy);

                if (HI_SUCCESS != s32Ret)
                {
                    VPSS_FATAL("Config node fail, id=%d\n", s32NodeId);
                    return s32Ret;
                }

#if 0
                if ((VPSS_HAL_NODE_2D_DEI_LAST_2FIELD == s32NodeId))
                {
                    VPSS_HAL_DumpReg(enIP, s32NodeId, HI_FALSE);
                    VPSS_HAL_Assert(s32NodeId, (volatile S_VPSS_REGS_TYPE*)pu32AppVir, HI_TRUE);
                }
#endif
                VPSS_HAL_Assert(s32NodeId, (volatile S_VPSS_REGS_TYPE*)pu32AppVir, HI_FALSE);
            }
            else
            {
                VPSS_FATAL("Config node func is NULL, id=%d\n", s32NodeId);
                return HI_FAILURE;
            }
        }
    }

    return s32Ret;
}

HI_S32 VPSS_HAL_SetDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, CBB_REG_ADDR_S stRegAddr)
{
    HI_U32				VPSS_DEI_ADDR;
    VPSS_DIESTCFG_S*	 pstDeiSt;
    CBB_INFO_S			stDeiSttCfg;
    CBB_ALG_INFO_S		stDeiAlgCfg;
    S_VPSS_REGS_TYPE*	 pstVpssNode;  //DDR中节点首地址


    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

    VPSS_MEM_CLEAR(stDeiSttCfg);
    VPSS_MEM_CLEAR(stDeiAlgCfg);

    pstVpssNode = (S_VPSS_REGS_TYPE*)stRegAddr.pu8NodeVirAddr;


    VPSS_DRV_Set_bfield(pstVpssNode, HI_TRUE);
    VPSS_DRV_Set_bfield_first(pstVpssNode, pstHalInfo->stInInfo.bTopFirst);

    switch (pstHalInfo->stInInfo.enFieldMode)
    {
        case HI_DRV_FIELD_BOTTOM:
            VPSS_DRV_Set_bfield_mode(pstVpssNode, HI_TRUE);
            break;

        case HI_DRV_FIELD_TOP:
            VPSS_DRV_Set_bfield_mode(pstVpssNode, HI_FALSE);
            break;

        default:
            VPSS_FATAL("No spt field Type:%d!\n", pstHalInfo->stInInfo.enFieldMode);
            return HI_FAILURE;
    }


    VPSS_DEI_ADDR = stRegAddr.u32NodePhyAddr + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DIECTRL) - sizeof(HI_U32);
    //printk("stRegAddr.u32NodePhyAddr: %x, offset:%d ", stRegAddr.u32NodePhyAddr,  VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32));

    //RWZB
    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        VPSS_DRV_Set_VPSS_DIECTRL_outsel(pstVpssNode, HI_TRUE, HI_TRUE);
    }

    VPSS_DRV_Set_VPSS_DIECTRL_lc_mode(pstVpssNode, 1, 1); // 4 field mode



    pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;
    stDeiSttCfg.u32RPhyAddr	= pstDeiSt->u32PreAddr;
    stDeiSttCfg.u32WPhyAddr	= pstDeiSt->u32CurAddr;
    stDeiSttCfg.u32Stride = pstDeiSt->u32Stride;

#if 0
    printk("DEI st cfg : ppre:%x  pre:%x cur:%x stride:%u\n",
           pstDeiSt->u32PPreAddr,
           pstDeiSt->u32PreAddr,
           pstDeiSt->u32CurAddr,
           pstDeiSt->u32Stride);
#endif

    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_DEI_ST, (HI_VOID*)&stDeiSttCfg, HI_NULL);
    VPSS_DRV_Set_VPSS_DIEHISMODE_pre_info_en(pstVpssNode, HI_TRUE);

    stDeiAlgCfg.u32ParaAddr = VPSS_DEI_ADDR;
    stDeiAlgCfg.bEnable		= HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DEI, (HI_VOID*)&stDeiAlgCfg, HI_NULL);

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetMcDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, CBB_REG_ADDR_S stRegAddr)
{
    HI_DRV_VID_FRAME_ADDR_S* pstBlendRef;
    HI_DRV_VID_FRAME_ADDR_S* pstBlendWbc;

    CBB_ALG_INFO_S		stMcDeiAlgCfg;
    CBB_FRAME_S 		stFrameCfg;
    CBB_INFO_BLKMV_S	stBlkmvCfg;
    CBB_INFO_RGMV_S		stRgmvCfg;
    CBB_INFO_PRJV_S 	stPrjVCfg;
    CBB_INFO_PRJH_S 	stPrjHCfg;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

    VPSS_MEM_CLEAR(stMcDeiAlgCfg);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stBlkmvCfg);
    VPSS_MEM_CLEAR(stRgmvCfg);
    VPSS_MEM_CLEAR(stPrjVCfg);
    VPSS_MEM_CLEAR(stPrjHCfg);

    stMcDeiAlgCfg.bEnable	= HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_MCDEI, (HI_VOID*)&stMcDeiAlgCfg, HI_NULL);

    pstBlendRef = &(pstHalInfo->stMcdeiRfrInfo.stBlendRef.stAddr);
    pstBlendWbc = &(pstHalInfo->stMcdeiRfrInfo.stBlendWbc.stAddr);


    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstBlendRef->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstBlendRef->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstBlendRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstBlendRef->u32Stride_C;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REE, (HI_VOID*)&stFrameCfg, HI_NULL);

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstBlendWbc->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstBlendWbc->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstBlendWbc->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstBlendWbc->u32Stride_C;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_CUE, (HI_VOID*)&stFrameCfg, HI_NULL);

    stBlkmvCfg.u32CurReadAddr 	= pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32CurReadAddr;
    stBlkmvCfg.u32RefReadAddr 	= pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32RefReadAddr;
    stBlkmvCfg.u32Nx1ReadAddr 	= pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Nx1ReadAddr;
    stBlkmvCfg.u32Nx2WriteAddr	= pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Nx2WriteAddr;
    stBlkmvCfg.u32Stride		= pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Stride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_BLKMV, (HI_VOID*)&stBlkmvCfg, HI_NULL);

    stRgmvCfg.u32CurReadAddr	= pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32CurReadAddr;
    stRgmvCfg.u32Nx1ReadAddr	= pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx1ReadAddr;
    stRgmvCfg.u32Nx2WriteAddr	= pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx2WriteAddr;
    stRgmvCfg.u32Stride			= pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Stride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_RGMV, (HI_VOID*)&stRgmvCfg, HI_NULL);

    stPrjVCfg.u32CurReadAddr	= pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32CurReadAddr;
    stPrjVCfg.u32Nx2WriteAddr	= pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Nx2WriteAddr;
    stPrjVCfg.u32Stride			= pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Stride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_PRJ_V, (HI_VOID*)&stPrjVCfg, HI_NULL);

    stPrjHCfg.u32CurReadAddr	= pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32CurReadAddr;
    stPrjHCfg.u32Nx2WriteAddr	= pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Nx2WriteAddr;
    stPrjHCfg.u32Stride			= pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Stride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_PRJ_H, (HI_VOID*)&stPrjHCfg, HI_NULL);

    return HI_SUCCESS;
}



HI_S32 VPSS_HAL_SetPortCfg(VPSS_IP_E enIP, CBB_REG_ADDR_S stRegAddr, VPSS_HAL_INFO_S* pstHalInfo)
{
    CBB_FRAME_S 			stFrmCfg;
    VPSS_HAL_PORT_INFO_S*	pstHalPort;
    HI_U32					u32PortId;
    HI_BOOL 				bUVInver;
    CBB_FUNC_FLIP_S 		stFlipInfo;
    CBB_FUNC_CROP_S 		stFuncCropCfg;
    CBB_FUNC_ZME_S			stFuncZmeCfg;
    CBB_FUNC_LBOX_S 		stLBox;
    CBB_FUNC_VPZME_S 		stFuncVPZmeCfg;
    CBB_FUNC_4PIXEL_S 		stFunc4PixelCfg;
    HI_RECT_S				*pstCropRect;
    HI_PQ_ZME_PARA_IN_S		*pstZmeDrvPara;
    VPSS_HAL_FRAME_S		*pstInInfo;
    VPSS_HAL_FRAME_S		*pstOutFrm;
    HI_DRV_VID_FRAME_ADDR_S	*pstOutAddr;
    CBB_FUNC_TUNNELOUT_S 	stFuncTunnelOutCfg;
	CBB_FUNC_TRANS_S		stTransInfo;
    VPSS_HAL_CTX_S			*pstHalCtx;
	HI_BOOL 				b4PixelClk;
    HI_BOOL                 bVPZMEEnable;


    VPSS_MEM_CLEAR(stFrmCfg);
    VPSS_MEM_CLEAR(stFlipInfo);
    VPSS_MEM_CLEAR(stFuncCropCfg);
    VPSS_MEM_CLEAR(stFuncZmeCfg);
    VPSS_MEM_CLEAR(stLBox);
    VPSS_MEM_CLEAR(stFuncVPZmeCfg);
    VPSS_MEM_CLEAR(stFunc4PixelCfg);
    VPSS_MEM_CLEAR(stFuncTunnelOutCfg);
	VPSS_MEM_CLEAR(stTransInfo);

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (VPSS_HAL_NODE_2D_UHD == pstHalInfo->enNodeType)
    {
        b4PixelClk = HI_TRUE;
    }
    else
    {
        b4PixelClk = HI_FALSE;
    }

    for (u32PortId = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
    {
        pstHalPort = &pstHalInfo->astPortInfo[u32PortId];

        if ((pstHalPort->bEnable)  && (pstHalPort->bConfig == HI_FALSE))
        {
            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;
            pstInInfo = &(pstHalInfo->stInInfo);


            VPSS_PIXLE_UPALIGN_W(pstOutFrm->u32Width, b4PixelClk);
            VPSS_PIXLE_UPALIGN_H(pstOutFrm->u32Height, b4PixelClk);

#ifdef VPSS_SUPPORT_OUT_TUNNEL
            /*tunnel*/
            if (pstHalPort->bOutLowDelay)
            {
                stFuncTunnelOutCfg.bEnable			= HI_TRUE;
                stFuncTunnelOutCfg.eMode			= CBB_TUNOUT_2;
                stFuncTunnelOutCfg.u32FinishLine	= pstOutFrm->u32Height / 10;
#ifdef HI_VPSS_SMMU_SUPPORT
                stFuncTunnelOutCfg.u32WriteAddr		= pstHalCtx->stLowDelayBuf_MMU.u32StartSmmuAddr;
#else
                stFuncTunnelOutCfg.u32WriteAddr		= pstHalCtx->stLowDelayBuf_MMZ.u32StartPhyAddr;
#endif
            }
            else
            {
                stFuncTunnelOutCfg.bEnable = HI_FALSE;
            }
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_OUT, (HI_VOID*)&stFuncTunnelOutCfg, HI_NULL);
#endif

			if (pstHalInfo->enNodeType == VPSS_HAL_NODE_HDR_DETILE_NOALG)
			{
				//if EL carries no metadata, EL NODE donot need to trans metadata
				stTransInfo.bEnable    = (pstHalInfo->stInInfo.u32MetaSize == 0) ? HI_FALSE : HI_TRUE;
				stTransInfo.u32SrcAddr = pstInInfo->stAddr_META.u32PhyAddr_Y;
				stTransInfo.u32DstAddr = pstOutFrm->stAddr_META.u32PhyAddr_Y;
				stTransInfo.u32Size	   = (pstInInfo->u32MetaSize/256)+1;
				stTransInfo.bSecure	   = pstInInfo->bSecure;

                if (stTransInfo.bEnable)
                {
                    if (stTransInfo.u32SrcAddr == 0 || stTransInfo.u32DstAddr == 0 || stTransInfo.u32Size == 0)
                    {
                        VPSS_ERROR("metadata config error.u32SrcAddr %#x u32DstAddr %#x u32TranSize %d\n",
                                        stTransInfo.u32SrcAddr,
                                        stTransInfo.u32DstAddr,
                                        stTransInfo.u32Size);
                        return HI_FAILURE;
                    }
                }
				VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TRANS, (HI_VOID*)&stTransInfo, HI_NULL);
			}


            /*Pre-ZME*/
            if (  (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FIELD)
                || (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FIELD_VIRTUAL)
                || (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_DEI_LAST_2FIELD)
                || (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_DEI_LAST_2FIELD_VIRTUAL)
                )
            {
                bVPZMEEnable = HI_TRUE;
            }
            else
            {
                bVPZMEEnable = HI_FALSE;
            }

            if (HI_TRUE == bVPZMEEnable)
            {
                stFuncVPZmeCfg.bEnable = HI_TRUE;
                if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP)
                {
                    stFuncVPZmeCfg.eFirMode = PREZME_2X;
                }
                else
                {
                    stFuncVPZmeCfg.eFirMode = PREZME_VFIELD;
                }
            }
            else
            {
                stFuncVPZmeCfg.bEnable = HI_FALSE;
            }

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_VPZME, (HI_VOID*)&stFuncVPZmeCfg, HI_NULL);

            if (pstHalPort->bNeedRota)
            {
                pstOutAddr = &(pstHalPort->stFrmNodeRoBuf.stOutFrame.stBufAddr[HI_DRV_BUF_ADDR_LEFT]);
                pstOutAddr->u32Stride_Y = pstOutAddr->u32Stride_C = pstHalPort->stFrmNodeRoBuf.stBuffer.u32Stride;
            }

            /* Flip&Mirro */
            stFlipInfo.bFlipH = pstHalPort->bNeedMirror;
            stFlipInfo.bFlipV = pstHalPort->bNeedFlip;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_FLIP, (HI_VOID*)&stFlipInfo, HI_NULL);

            /* UV反转 */
            bUVInver = pstOutFrm->bUVInver;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_UV, (HI_VOID*)&bUVInver, HI_NULL);

            /* VHD0's Crop */
            pstCropRect = &(pstHalPort->stOutCropRect);

            VPSS_PIXLE_DOWNALIGN_W(pstCropRect->s32Width, HI_TRUE);  //CROP input w is 4pixel align
            VPSS_PIXLE_DOWNALIGN_H(pstCropRect->s32Height, b4PixelClk);//2pixel align whatever 4p/c is enable or disable
			VPSS_PIXLE_DOWNALIGN_W(pstCropRect->s32X, HI_TRUE);
            VPSS_PIXLE_DOWNALIGN_H(pstCropRect->s32Y, b4PixelClk);

            stFuncCropCfg.stInRect.s32X		= pstCropRect->s32X;
            stFuncCropCfg.stInRect.s32Y		= pstCropRect->s32Y;
            stFuncCropCfg.stInRect.s32Width	= pstCropRect->s32Width;
            stFuncCropCfg.stInRect.s32Height = pstCropRect->s32Height;
            stFuncCropCfg.stCropCfg = stFuncCropCfg.stInRect;
            stFuncCropCfg.bEnable = HI_TRUE;

            if (HI_TRUE == bVPZMEEnable)
            {
                stFuncCropCfg.stInRect.s32Y			/= 2;
                stFuncCropCfg.stInRect.s32Height	/= 2;
                stFuncCropCfg.stCropCfg.s32Height	/= 2;
                VPSS_PIXLE_DOWNALIGN_H(stFuncCropCfg.stCropCfg.s32Height, HI_FALSE);//2pixel align when 4p/c is disable
            }

            /*
            	src:10bit dst:10bit-> disable crop , disable zme
            	src:10bit dst:8bit -> cfg src as 8bit, enable crop , enable zme
            	src:10bit dst:cst 10bit -> disable crop, disable zme, enable CTS[CTS10 Node]
            */
            if ((pstHalInfo->stInInfo.enBitWidth ==  HI_DRV_PIXEL_BITWIDTH_10BIT)
                && (pstOutFrm->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT))
            {
                stFuncCropCfg.bEnable = HI_FALSE;
            }

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_HDR_DETILE_NOALG)
            {
                stFuncCropCfg.bEnable = HI_FALSE;
            }

            if ((stFuncCropCfg.bEnable) && bVPZMEEnable)
            {
                VPSS_DBG_INFO("Crop: x y w h %d %d %d %d\n",
                              stFuncCropCfg.stCropCfg.s32X,
                              stFuncCropCfg.stCropCfg.s32Y,
                              stFuncCropCfg.stCropCfg.s32Width,
                              stFuncCropCfg.stCropCfg.s32Height);
            }
            else
            {
                VPSS_DBG_INFO("Crop is disable\n");
            }
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_CROP, (HI_VOID*)&stFuncCropCfg, HI_NULL);


            /*ZME*/
            stFuncZmeCfg.u32ParAddr = stRegAddr.u32NodePhyAddr + 0x2000;
            stFuncZmeCfg.bEnable	= HI_TRUE;
            stFuncZmeCfg.eMode		= CBB_ZME_MODE_ALL;

            pstZmeDrvPara  = &(stFuncZmeCfg.stZmeDrvPara);
            memset(pstZmeDrvPara, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
            if (HI_TRUE == bVPZMEEnable)
            {
                pstZmeDrvPara->bZmeFrmFmtIn = 0; //field
            }
            pstZmeDrvPara->bZmeFrmFmtOut = 1;
            pstZmeDrvPara->bZmeBFIn = 0;
            pstZmeDrvPara->bZmeBFOut = 0;
            pstZmeDrvPara->u32PortID = u32PortId;
            pstZmeDrvPara->u32HandleNo = pstHalInfo->s32InstanceID;
            pstZmeDrvPara->enSceneMode = pstHalInfo->enSceneMode;

            if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p)
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP)
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (HI_TRUE == bVPZMEEnable)
            {
                pstZmeDrvPara->u32ZmeFrmHIn = pstCropRect->s32Height;
                //pstZmeDrvPara->u32ZmeFrmHIn = (pstCropRect->s32Height / 2);
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;

                VPSS_PIXLE_UPALIGN_W(pstZmeDrvPara->u32ZmeFrmWIn, HI_FALSE);
                VPSS_PIXLE_UPALIGN_H(pstZmeDrvPara->u32ZmeFrmHIn, HI_FALSE);
            }
            else
            {
                //ZME's input height/width is VHD0 Crop's output
                pstZmeDrvPara->u32ZmeFrmHIn = pstCropRect->s32Height;
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;
            }

            pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
            pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;

            pstZmeDrvPara->stOriRect.s32X = 0;
            pstZmeDrvPara->stOriRect.s32Y = 0;
            pstZmeDrvPara->stOriRect.s32Width = pstInInfo->u32Width;
            if (HI_TRUE == bVPZMEEnable)
            {
                pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height/2;
            }
            else
            {
                pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
            }
            pstZmeDrvPara->u32InRate = 25000;
            pstZmeDrvPara->u32OutRate = 25000;
            pstZmeDrvPara->bDispProgressive = HI_TRUE;
            pstZmeDrvPara->u32Fidelity = 0;

            if ((pstInInfo->enBitWidth ==  HI_DRV_PIXEL_BITWIDTH_10BIT)
                && (pstOutFrm->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT))
            {
                stFuncZmeCfg.bEnable	= HI_FALSE;
            }

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_HDR_DETILE_NOALG)
            {
                stFuncZmeCfg.bEnable = HI_FALSE;
            }

            if (pstZmeDrvPara->u32ZmeFrmWOut > VPSS_ALG_ZME_W_MAX)
            {
                stFuncZmeCfg.bEnable = HI_FALSE;
            }

            if (stFuncZmeCfg.bEnable)
            {
                VPSS_DBG_INFO("Zme: in w h out w h %u %u %u %u ori w h %u %u\n"
                        "in-fmt : %d out-fmt : %d\n",
                              pstZmeDrvPara->u32ZmeFrmWIn,
                              pstZmeDrvPara->u32ZmeFrmHIn,
                              pstZmeDrvPara->u32ZmeFrmWOut,
                              pstZmeDrvPara->u32ZmeFrmHOut,
                              pstZmeDrvPara->stOriRect.s32Width,
                              pstZmeDrvPara->stOriRect.s32Height,
                              pstZmeDrvPara->bZmeFrmFmtIn,
                              pstZmeDrvPara->bZmeFrmFmtOut);
            }
            else
            {
                VPSS_DBG_INFO("Zme is disable\n");
            }
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_ZME, (HI_VOID*)&stFuncZmeCfg, HI_NULL);

            /* LBX */
            stLBox.eBGColor 		 = CBB_COLOR_BLACK;
            stLBox.stLBoxRect.s32X	 = pstHalPort->stVideoRect.s32X;
            stLBox.stLBoxRect.s32Y	 = pstHalPort->stVideoRect.s32Y;

            if (pstHalPort->bNeedRota)
            {
                stLBox.stLBoxRect.s32Height = pstOutFrm->u32Width;
                stLBox.stLBoxRect.s32Width  = pstOutFrm->u32Height;
                stLBox.stOutRect.s32Width	= pstOutFrm->u32Height;
                stLBox.stOutRect.s32Height	= pstOutFrm->u32Width;
            }
            else
            {
                stLBox.stLBoxRect.s32Height = pstOutFrm->u32Height;
                stLBox.stLBoxRect.s32Width 	= pstOutFrm->u32Width;
                stLBox.stOutRect.s32Width	= pstOutFrm->u32Width;
                stLBox.stOutRect.s32Height	= pstOutFrm->u32Height;
            }
            stLBox.bEnable              = HI_TRUE;

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_HDR_DETILE_NOALG)
            {
                stLBox.bEnable = HI_FALSE;
            }

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_LBOX, (HI_VOID*)&stLBox, HI_NULL);


            /* vhd0 frame infomation */
            if (pstHalPort->bNeedRota)
            {
                stFrmCfg.u32Width	= pstOutFrm->u32Height;
                stFrmCfg.u32Height	= pstOutFrm->u32Width;
            }
            else
            {
                stFrmCfg.u32Width	= pstOutFrm->u32Width;
                stFrmCfg.u32Height	= pstOutFrm->u32Height;
            }

            stFrmCfg.enBitWidth = pstOutFrm->enBitWidth;
            stFrmCfg.enPixelFmt = pstOutFrm->enFormat;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstOutAddr->u32PhyAddr_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstOutAddr->u32PhyAddr_C;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstOutAddr->u32Stride_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstOutAddr->u32Stride_C;

            VPSS_DBG_INFO(

                "Vhd0 Y C StrideY StrideC W H %08x %08x %d %d %d %d\n",
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C,
                stFrmCfg.u32Width,
                stFrmCfg.u32Height);

            if ((CBB_BITWIDTH_10BIT == stFrmCfg.enBitWidth)
                || (CBB_BITWIDTH_10BIT_CTS == stFrmCfg.enBitWidth))
            {
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y = pstOutFrm->stAddr_LB.u32PhyAddr_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C = pstOutFrm->stAddr_LB.u32PhyAddr_C;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstOutFrm->stAddr_LB.u32Stride_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstOutFrm->stAddr_LB.u32Stride_C;
            }
            VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_VHD0, (HI_VOID*)&stFrmCfg, HI_NULL);

            pstHalPort->bConfig = HI_TRUE;

            if (pstHalPort->bNeedRota)
            {
                pstHalPort->bConfig = HI_FALSE; //后面YC翻转的Node还要配置
            }

            break; //每次只处理一个port, Virtual port会再次调用
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetPortCfg_Rota(CBB_REG_ADDR_S stRegAddr, VPSS_HAL_INFO_S* pstHalInfo, HI_BOOL bRotateY)
{
    CBB_FRAME_S stFrmCfg;
    VPSS_HAL_PORT_INFO_S*	pstHalPort;
    HI_U32			u32PortId;
    HI_BOOL 		bUVInver;
    CBB_FUNC_FLIP_S stFlipInfo;
    CBB_FUNC_CROP_S stFuncCropCfg;
    CBB_FUNC_ZME_S	stFuncZmeCfg;
    CBB_FUNC_LBOX_S stLBox;
    CBB_FUNC_ROTATION_YC_S stFuncRotatYCCfg;
    VPSS_HAL_FRAME_S*		pstOutFrm;
    HI_DRV_VID_FRAME_ADDR_S* pstOutAddr;
    HI_DRV_VPSS_ROTATION_E enRotation;
    HI_U32 u32Angle;
    HI_PQ_ZME_PARA_IN_S*	 pstZmeDrvPara;
    VPSS_HAL_FRAME_S*		pstInInfo;
    HI_RECT_S*		pstCropRect;

    VPSS_MEM_CLEAR(stFrmCfg);
    VPSS_MEM_CLEAR(stFlipInfo);
    VPSS_MEM_CLEAR(stFuncCropCfg);
    VPSS_MEM_CLEAR(stFuncZmeCfg);
    VPSS_MEM_CLEAR(stLBox);

    for (u32PortId = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
    {
        pstHalPort = &pstHalInfo->astPortInfo[u32PortId];

        if (pstHalPort->bEnable) // && pstHalPort->bConfig == HI_FALSE)
        {
            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;


            VPSS_PIXLE_UPALIGN_W(pstOutFrm->u32Width, HI_FALSE);
            VPSS_PIXLE_UPALIGN_H(pstOutFrm->u32Height, HI_FALSE);


            /* 旋转配置 */
            enRotation = pstHalInfo->astPortInfo[u32PortId].enRotation;

            switch (enRotation)
            {
                case HI_DRV_VPSS_ROTATION_90:
                    u32Angle = 0x0;
                    break;

                case HI_DRV_VPSS_ROTATION_270:
                    u32Angle = 0x1;
                    break;

                default:
                    u32Angle = 0x1;
                    VPSS_FATAL("Rota Error %d\n", u32Angle);
                    break;
            }

            stFuncRotatYCCfg.bEnable = HI_TRUE;
            stFuncRotatYCCfg.u32Angle = u32Angle;

            if (bRotateY)
            {
                stFuncRotatYCCfg.u32ImgProcessMode = 1;
                VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_ROTATION_Y, (HI_VOID*)&stFuncRotatYCCfg, HI_NULL);
            }
            else
            {
                stFuncRotatYCCfg.u32ImgProcessMode = 2;
                VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_ROTATION_C, (HI_VOID*)&stFuncRotatYCCfg, HI_NULL);
            }

            /* Flip&Mirro */
            stFlipInfo.bFlipH = HI_FALSE;
            stFlipInfo.bFlipV = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_FLIP, (HI_VOID*)&stFlipInfo, HI_NULL);

            /* UV反转 */
            bUVInver = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_UV, (HI_VOID*)&bUVInver, HI_NULL);

            /* VHD0's Crop */
            pstCropRect = &(pstHalPort->stOutCropRect);
            VPSS_PIXLE_DOWNALIGN_W(pstCropRect->s32Width, HI_TRUE);  //4pixel align, whatever 4p/c is enable or not
            VPSS_PIXLE_DOWNALIGN_H(pstCropRect->s32Height, HI_FALSE);//2pixel align when 4p/c is disable

            stFuncCropCfg.stInRect.s32X		= pstCropRect->s32X;
            stFuncCropCfg.stInRect.s32Y		= pstCropRect->s32Y;
            stFuncCropCfg.stInRect.s32Width	= pstCropRect->s32Width;
            stFuncCropCfg.stInRect.s32Height	= pstCropRect->s32Height;
            stFuncCropCfg.stCropCfg = stFuncCropCfg.stInRect;
            stFuncCropCfg.bEnable = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_CROP, (HI_VOID*)&stFuncCropCfg, HI_NULL);

            if (stFuncCropCfg.bEnable)
            {
                VPSS_DBG_INFO("Crop: x y w h %d %d %d %d\n",
                              stFuncCropCfg.stCropCfg.s32X,
                              stFuncCropCfg.stCropCfg.s32Y,
                              stFuncCropCfg.stCropCfg.s32Width,
                              stFuncCropCfg.stCropCfg.s32Height);
            }
            else
            {
                VPSS_DBG_INFO("Crop is disable\n");
            }

            /*ZME*/
            stFuncZmeCfg.u32ParAddr = stRegAddr.u32NodePhyAddr + 0x2000;
            stFuncZmeCfg.bEnable	= HI_FALSE;
            stFuncZmeCfg.eMode		= CBB_ZME_MODE_ALL;

            pstZmeDrvPara  = &(stFuncZmeCfg.stZmeDrvPara);
            memset(pstZmeDrvPara, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
            pstZmeDrvPara->bZmeFrmFmtOut = 1;
            pstZmeDrvPara->bZmeBFIn = 0;
            pstZmeDrvPara->bZmeBFOut = 0;
            pstZmeDrvPara->u32HandleNo = pstHalInfo->s32InstanceID;
            pstZmeDrvPara->enSceneMode = pstHalInfo->enSceneMode;

            pstInInfo = &(pstHalInfo->stInInfo);

            if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p)
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP)
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FIELD)
            {
                pstZmeDrvPara->u32ZmeFrmHIn = (pstCropRect->s32Height / 2);
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;

                VPSS_PIXLE_UPALIGN_W(pstZmeDrvPara->u32ZmeFrmWIn, HI_FALSE);
                VPSS_PIXLE_UPALIGN_H(pstZmeDrvPara->u32ZmeFrmHIn, HI_FALSE);
            }
            else
            {
                //ZME's input height/width is VHD0 Crop's output
                pstZmeDrvPara->u32ZmeFrmHIn = pstCropRect->s32Width;
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Height;
            }

            pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
            pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;
            pstZmeDrvPara->stOriRect.s32X = 0;
            pstZmeDrvPara->stOriRect.s32Y = 0;
            pstZmeDrvPara->stOriRect.s32Width = pstHalInfo->stInInfo.u32Height;
            pstZmeDrvPara->stOriRect.s32Height = pstHalInfo->stInInfo.u32Width;
            pstZmeDrvPara->u32InRate = 25000;
            pstZmeDrvPara->u32OutRate = 25000;
            pstZmeDrvPara->bDispProgressive = HI_TRUE;
            pstZmeDrvPara->u32Fidelity = 0;

            if (stFuncZmeCfg.bEnable)
            {
                VPSS_DBG_INFO("Zme: in w h out w h %u %u %u %u ori w h %u %u\n",
                              pstZmeDrvPara->u32ZmeFrmWIn,
                              pstZmeDrvPara->u32ZmeFrmHIn,
                              pstZmeDrvPara->u32ZmeFrmWOut,
                              pstZmeDrvPara->u32ZmeFrmHOut,
                              pstZmeDrvPara->stOriRect.s32Width,
                              pstZmeDrvPara->stOriRect.s32Height);
            }
            else
            {
                VPSS_DBG_INFO("Zme is disable\n");
            }

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_ZME, (HI_VOID*)&stFuncZmeCfg, HI_NULL);

            /* LBX */
            stLBox.eBGColor 		 = CBB_COLOR_BLACK;
            stLBox.stLBoxRect.s32X	 = pstHalPort->stVideoRect.s32X;
            stLBox.stLBoxRect.s32Y	 = pstHalPort->stVideoRect.s32Y;
            stLBox.stLBoxRect.s32Height = pstOutFrm->u32Height;
            stLBox.stLBoxRect.s32Width = pstOutFrm->u32Width;
            stLBox.stOutRect.s32Width	 = pstOutFrm->u32Width;
            stLBox.stOutRect.s32Height	= pstOutFrm->u32Height;
            stLBox.bEnable			 = HI_FALSE;

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_LBOX, (HI_VOID*)&stLBox, HI_NULL);

            /* vhd0 frame infomation */
            stFrmCfg.u32Width	= pstHalPort->stOutInfo.u32Width;
            stFrmCfg.u32Height	= pstHalPort->stOutInfo.u32Height;

            stFrmCfg.enBitWidth = pstHalPort->stOutInfo.enBitWidth;
            stFrmCfg.enPixelFmt = pstHalPort->stOutInfo.enFormat;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstHalPort->stOutInfo.stAddr.u32PhyAddr_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstHalPort->stOutInfo.stAddr.u32PhyAddr_C;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstHalPort->stOutInfo.stAddr.u32Stride_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstHalPort->stOutInfo.stAddr.u32Stride_C;

            if (CBB_BITWIDTH_10BIT == stFrmCfg.enBitWidth)//judge by output bitwidth
            {
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y = pstHalPort->stOutInfo.stAddr_LB.u32PhyAddr_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C = pstHalPort->stOutInfo.stAddr_LB.u32PhyAddr_C;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstHalPort->stOutInfo.stAddr_LB.u32Stride_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstHalPort->stOutInfo.stAddr_LB.u32Stride_C;
            }

            VPSS_DBG_INFO("RotaNode vhd0 save to %08x %08x %d %d\n",
                          stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                          stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                          stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                          stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

            VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_VHD0, (HI_VOID*)&stFrmCfg, HI_NULL);

            pstHalPort->bConfig = HI_TRUE;

            break; //每次只处理一个port
        }
    }

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetPortCfg_CTS10Bit(CBB_REG_ADDR_S stRegAddr, VPSS_HAL_INFO_S* pstHalInfo)
{
    CBB_FRAME_S stFrmCfg;
    VPSS_HAL_PORT_INFO_S*	pstHalPort;
    HI_U32			u32PortId;
    HI_BOOL 		bUVInver;
    CBB_FUNC_FLIP_S stFlipInfo;
    CBB_FUNC_CROP_S stFuncCropCfg;
    CBB_FUNC_ZME_S	stFuncZmeCfg;
    CBB_FUNC_LBOX_S stLBox;
    HI_RECT_S*		pstCropRect;
    HI_PQ_ZME_PARA_IN_S*	 pstZmeDrvPara;
    VPSS_HAL_FRAME_S*		pstInInfo;
    VPSS_HAL_FRAME_S*		pstOutFrm;
    HI_DRV_VID_FRAME_ADDR_S* pstOutAddr;
    //VPSS_HAL_PORT_INFO_S	stPortTemp;


    VPSS_MEM_CLEAR(stFrmCfg);
    VPSS_MEM_CLEAR(stFlipInfo);
    VPSS_MEM_CLEAR(stFuncCropCfg);
    VPSS_MEM_CLEAR(stFuncZmeCfg);
    VPSS_MEM_CLEAR(stLBox);

    for (u32PortId = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
    {
        pstHalPort = &pstHalInfo->astPortInfo[u32PortId];

        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {
            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;

            VPSS_PIXLE_UPALIGN_W(pstOutFrm->u32Width, HI_TRUE);
            VPSS_PIXLE_UPALIGN_H(pstOutFrm->u32Height, HI_TRUE);


            /* Flip&Mirro */
            stFlipInfo.bFlipH = HI_FALSE;
            stFlipInfo.bFlipV = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_FLIP, (HI_VOID*)&stFlipInfo, HI_NULL);

            /* UV反转 */
            bUVInver = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_UV, (HI_VOID*)&bUVInver, HI_NULL);

            /* VHD0's Crop */
            pstCropRect = &(pstHalPort->stOutCropRect);
            VPSS_PIXLE_DOWNALIGN_W(pstCropRect->s32Width, HI_TRUE);  //4pixel align, whatever 4p/c is enable or not
            VPSS_PIXLE_DOWNALIGN_H(pstCropRect->s32Height, HI_TRUE);//4p/c is enable

            stFuncCropCfg.stInRect.s32X 	= pstCropRect->s32X;
            stFuncCropCfg.stInRect.s32Y 	= pstCropRect->s32Y;
            stFuncCropCfg.stInRect.s32Width = pstCropRect->s32Width;
            stFuncCropCfg.stInRect.s32Height = pstCropRect->s32Height;
            stFuncCropCfg.stCropCfg = stFuncCropCfg.stInRect;
            stFuncCropCfg.bEnable = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_CROP, (HI_VOID*)&stFuncCropCfg, HI_NULL);

            /*ZME*/
            stFuncZmeCfg.u32ParAddr = stRegAddr.u32NodePhyAddr + 0x2000;
            stFuncZmeCfg.bEnable	= HI_FALSE;
            stFuncZmeCfg.eMode		= CBB_ZME_MODE_ALL;

            pstZmeDrvPara  = &(stFuncZmeCfg.stZmeDrvPara);
            memset(pstZmeDrvPara, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
            pstZmeDrvPara->bZmeFrmFmtOut = 1;
            pstZmeDrvPara->bZmeBFIn = 0;
            pstZmeDrvPara->bZmeBFOut = 0;
            pstZmeDrvPara->u32HandleNo = pstHalInfo->s32InstanceID;
            pstZmeDrvPara->enSceneMode = pstHalInfo->enSceneMode;

            pstInInfo = &(pstHalInfo->stInInfo);

            if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p)
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP)
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FIELD)
            {
                pstZmeDrvPara->u32ZmeFrmHIn = (pstCropRect->s32Height / 2);
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;

                VPSS_PIXLE_UPALIGN_W(pstZmeDrvPara->u32ZmeFrmWIn, HI_TRUE);
                VPSS_PIXLE_UPALIGN_H(pstZmeDrvPara->u32ZmeFrmHIn, HI_TRUE);
            }
            else
            {
                //ZME's input height/width is VHD0 Crop's output
                pstZmeDrvPara->u32ZmeFrmHIn = pstCropRect->s32Height;
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;
            }

            if (pstHalPort->bNeedRota)
            {
                pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Width;
                pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Height;
            }
            else
            {
                pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
                pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;
            }

            pstZmeDrvPara->stOriRect.s32X = 0;
            pstZmeDrvPara->stOriRect.s32Y = 0;
            pstZmeDrvPara->stOriRect.s32Width = pstHalInfo->stInInfo.u32Width;
            pstZmeDrvPara->stOriRect.s32Height = pstHalInfo->stInInfo.u32Height;
            pstZmeDrvPara->u32InRate = 25000;
            pstZmeDrvPara->u32OutRate = 25000;
            pstZmeDrvPara->bDispProgressive = HI_TRUE;
            pstZmeDrvPara->u32Fidelity = 0;

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_ZME, (HI_VOID*)&stFuncZmeCfg, HI_NULL);

            /* LBX */
            stLBox.eBGColor 		 = CBB_COLOR_BLACK;
            stLBox.stLBoxRect.s32X	 = pstHalPort->stVideoRect.s32X;
            stLBox.stLBoxRect.s32Y	 = pstHalPort->stVideoRect.s32Y;

            stLBox.stLBoxRect.s32Height = pstOutFrm->u32Height;
            stLBox.stLBoxRect.s32Width	= pstOutFrm->u32Width;
            stLBox.stOutRect.s32Width	= pstOutFrm->u32Width;
            stLBox.stOutRect.s32Height	= pstOutFrm->u32Height;
            stLBox.bEnable				= HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_LBOX, (HI_VOID*)&stLBox, HI_NULL);

            stFrmCfg.u32Width	= pstOutFrm->u32Width;
            stFrmCfg.u32Height	= pstOutFrm->u32Height;

            stFrmCfg.enBitWidth = pstOutFrm->enBitWidth;
            stFrmCfg.enPixelFmt = pstOutFrm->enFormat;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstOutAddr->u32PhyAddr_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstOutAddr->u32PhyAddr_C;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstOutAddr->u32Stride_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstOutAddr->u32Stride_C;

            VPSS_DBG_INFO(

                "2DNode vhd0 save to Y C StrideY StrideY %08x %08x %d %d\n",
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

            if ((CBB_BITWIDTH_10BIT == stFrmCfg.enBitWidth)
                || (CBB_BITWIDTH_10BIT_CTS == stFrmCfg.enBitWidth))
            {
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y = pstOutFrm->stAddr_LB.u32PhyAddr_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C = pstOutFrm->stAddr_LB.u32PhyAddr_C;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstOutFrm->stAddr_LB.u32Stride_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstOutFrm->stAddr_LB.u32Stride_C;
            }

            VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_VHD0, (HI_VOID*)&stFrmCfg, HI_NULL);

            pstHalPort->bConfig = HI_TRUE;

            if (pstHalPort->bNeedRota)
            {
                pstHalPort->bConfig = HI_FALSE;
            }

            break; //每次只处理一个port
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetPortCfg_SD_First_dei(VPSS_IP_E enIP,  CBB_REG_ADDR_S stRegAddr, VPSS_HAL_INFO_S* pstHalInfo)
{
    CBB_FRAME_S stFrmCfg;
    VPSS_HAL_PORT_INFO_S*	pstHalPort;
    HI_U32			u32PortId;
    HI_BOOL 		bUVInver;
    CBB_FUNC_FLIP_S stFlipInfo;
    CBB_FUNC_CROP_S stFuncCropCfg;
    CBB_FUNC_ZME_S	stFuncZmeCfg;
    CBB_FUNC_LBOX_S stLBox;
    HI_RECT_S*		pstCropRect;
    HI_PQ_ZME_PARA_IN_S*	 pstZmeDrvPara;
    VPSS_HAL_FRAME_S*		pstInInfo;
    VPSS_HAL_FRAME_S*		pstOutFrm;
    HI_DRV_VID_FRAME_ADDR_S* pstOutAddr;
    HI_DRV_VID_FRAME_ADDR_S* pstOutAddr_LB;

    VPSS_MEM_CLEAR(stFrmCfg);
    VPSS_MEM_CLEAR(stFlipInfo);
    VPSS_MEM_CLEAR(stFuncCropCfg);
    VPSS_MEM_CLEAR(stFuncZmeCfg);
    VPSS_MEM_CLEAR(stLBox);

    for (u32PortId = 0; u32PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32PortId++)
    {
        pstHalPort = &pstHalInfo->astPortInfo[u32PortId];

        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {
            pstOutFrm = &pstHalPort->stOutInfo;
            //pstOutAddr = &pstOutFrm->stAddr;
            pstOutAddr = &(pstHalInfo->stSDDeiFrame.stAddr);
            pstOutAddr_LB = &(pstHalInfo->stSDDeiFrame.stAddr_LB);

            VPSS_PIXLE_UPALIGN_W(pstOutFrm->u32Width, HI_FALSE);
            VPSS_PIXLE_UPALIGN_H(pstOutFrm->u32Height, HI_FALSE);


            /* Flip&Mirro */
            stFlipInfo.bFlipH = HI_FALSE;
            stFlipInfo.bFlipV = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_FLIP, (HI_VOID*)&stFlipInfo, HI_NULL);

            /* UV反转 */
            bUVInver = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_UV, (HI_VOID*)&bUVInver, HI_NULL);

            /* VHD0's Crop */
            pstCropRect = &(pstHalPort->stOutCropRect);
            VPSS_PIXLE_DOWNALIGN_W(pstCropRect->s32Width, HI_TRUE);  //4pixel align, whatever 4p/c is enable or not
            VPSS_PIXLE_DOWNALIGN_H(pstCropRect->s32Height, HI_FALSE);//2pixel align when 4p/c is disable

            stFuncCropCfg.stInRect.s32X		= pstCropRect->s32X;
            stFuncCropCfg.stInRect.s32Y		= pstCropRect->s32Y;
            stFuncCropCfg.stInRect.s32Width	= pstCropRect->s32Width;
            stFuncCropCfg.stInRect.s32Height	= pstCropRect->s32Height;
            stFuncCropCfg.stCropCfg = stFuncCropCfg.stInRect;

            stFuncCropCfg.bEnable = HI_FALSE;
            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_CROP, (HI_VOID*)&stFuncCropCfg, HI_NULL);

            /*ZME*/
            stFuncZmeCfg.u32ParAddr = stRegAddr.u32NodePhyAddr + 0x2000;
            stFuncZmeCfg.bEnable	= HI_FALSE;
            stFuncZmeCfg.eMode		= CBB_ZME_MODE_ALL;

            pstZmeDrvPara  = &(stFuncZmeCfg.stZmeDrvPara);
            memset(pstZmeDrvPara, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
            pstZmeDrvPara->bZmeFrmFmtOut = 1;
            pstZmeDrvPara->bZmeBFIn = 0;
            pstZmeDrvPara->bZmeBFOut = 0;
            pstZmeDrvPara->u32HandleNo = pstHalInfo->s32InstanceID;
            pstZmeDrvPara->enSceneMode = pstHalInfo->enSceneMode;

            pstInInfo = &(pstHalInfo->stInInfo);

            if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p)
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
                || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP)
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
            }
            else
            {
                pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
            }

            if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FIELD)
            {
                pstZmeDrvPara->u32ZmeFrmHIn = (pstCropRect->s32Height / 2);
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;

                VPSS_PIXLE_UPALIGN_W(pstZmeDrvPara->u32ZmeFrmWIn, HI_FALSE);
                VPSS_PIXLE_UPALIGN_H(pstZmeDrvPara->u32ZmeFrmHIn, HI_FALSE);
            }
            else
            {
                //ZME's input height/width is VHD0 Crop's output
                pstZmeDrvPara->u32ZmeFrmHIn = pstCropRect->s32Height;
                pstZmeDrvPara->u32ZmeFrmWIn = pstCropRect->s32Width;
            }

            pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
            pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;

            pstZmeDrvPara->stOriRect.s32X = 0;
            pstZmeDrvPara->stOriRect.s32Y = 0;
            pstZmeDrvPara->stOriRect.s32Width = pstHalInfo->stInInfo.u32Width;
            pstZmeDrvPara->stOriRect.s32Height = pstHalInfo->stInInfo.u32Height;
            pstZmeDrvPara->u32InRate = 25000;
            pstZmeDrvPara->u32OutRate = 25000;
            pstZmeDrvPara->bDispProgressive = HI_TRUE;
            pstZmeDrvPara->u32Fidelity = 0;

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_ZME, (HI_VOID*)&stFuncZmeCfg, HI_NULL);

            /* LBX */
            stLBox.eBGColor 		 = CBB_COLOR_BLACK;
            stLBox.stLBoxRect.s32X	 = pstHalPort->stVideoRect.s32X;
            stLBox.stLBoxRect.s32Y	 = pstHalPort->stVideoRect.s32Y;
            stLBox.stLBoxRect.s32Height = pstOutFrm->u32Height;
            stLBox.stLBoxRect.s32Width = pstOutFrm->u32Width;
            stLBox.stOutRect.s32Width	 = pstOutFrm->u32Width;
            stLBox.stOutRect.s32Height  = pstOutFrm->u32Height;
            stLBox.bEnable			 = HI_FALSE;

            VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_LBOX, (HI_VOID*)&stLBox, HI_NULL);

            /* vhd0 frame infomation */
            // first node use w h same as input image w h. because zme/crop is disable
            stFrmCfg.u32Width	= pstInInfo->u32Width;
            stFrmCfg.u32Height	= pstInInfo->u32Height;

            VPSS_PIXLE_UPALIGN_W(stFrmCfg.u32Width, HI_FALSE);
            VPSS_PIXLE_UPALIGN_H(stFrmCfg.u32Height, HI_FALSE);

            stFrmCfg.enBitWidth = pstHalPort->stOutInfo.enBitWidth;
            stFrmCfg.enPixelFmt = pstInInfo->enFormat;

            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstOutAddr->u32PhyAddr_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstOutAddr->u32PhyAddr_C;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstOutAddr->u32Stride_Y;
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstOutAddr->u32Stride_C;

#if 0
            printk("VPSS sd fst save addr Y C stridY stridC %x %x %d %d\n",
                   stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                   stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                   stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                   stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);
#endif

            if (CBB_BITWIDTH_10BIT == stFrmCfg.enBitWidth)//judge by output bitwidth
            {
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y = pstOutAddr_LB->u32PhyAddr_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C = pstOutAddr_LB->u32PhyAddr_C;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstOutAddr_LB->u32Stride_Y;
                stFrmCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstOutAddr_LB->u32Stride_C;
            }

            VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_VHD0, (HI_VOID*)&stFrmCfg, HI_NULL);

            //pstHalPort->bConfig = HI_TRUE; sd-dei-second node use
            break; //每次只处理一个port
        }
    }

    return HI_SUCCESS;
}

/*
HI_BOOL bYdata, 亮度或色度标识, 1:亮度, 0色度
HI_BOOL bField, 场,帧标志, 0:帧, 1:场
*/
HI_S32 VPSS_HAL_Get_RwzbPixel(HI_U8* pu8PixelBaseAddr_vir,
                              VPSS_RWZB_INFO_S* pstRwzbInfo, HI_U32 u32YCMode, HI_BOOL bField)
{
    HI_U32 ii;
    HI_U32 jj;
    HI_U32 u32_blk_xpos;
    HI_U32 u32_blk_ypos;

    if ((HI_NULL == pu8PixelBaseAddr_vir) || (HI_NULL == pstRwzbInfo))
    {
        VPSS_FATAL("VPSS get rwzb with null pointer(nx2)!\n");
        return HI_FAILURE;
    }


    for (ii = 0; ii < 6; ii++)
    {
        u32_blk_xpos = (pstRwzbInfo->u32Addr)[ii][0] << 3;
        u32_blk_ypos = (pstRwzbInfo->u32Addr)[ii][1] << 3;

        if ((u32_blk_xpos <= pstRwzbInfo->u32Width)
            && (u32_blk_ypos <= pstRwzbInfo->u32Height))
        {
            /* 软件去访问NX2的数据时, 场模式也不需要Y/2 */
            if (0 == u32YCMode) //亮度
            {
                //VPSS_DBG_INFO("\r\n%d- X Y %d %d\t", ii, u32_blk_xpos, u32_blk_ypos);
                for (jj = 0; jj < 8; jj++)
                {
                    pstRwzbInfo->u8Data[ii][jj] =
                        *(pu8PixelBaseAddr_vir + ((u32_blk_ypos * pstRwzbInfo->u32Width) + u32_blk_xpos) + jj); //8bit

                    //VPSS_DBG_INFO("%d ", pstRwzbInfo->u8Data[ii][jj]);
                }
            }
        }
        else
        {
            return HI_VPSS_RET_UNSUPPORT;
        }
    }
    //VPSS_DBG_INFO("\r\n");

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_GetDetPixel(HI_U8* pDstData, HI_U8* pSrcData)
{
    VPSS_SAFE_MEMCPY(pDstData, pSrcData, sizeof(HI_U8)*PIX_NUM_IN_PATTERN);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_RWZB_Calc(VPSS_HAL_INFO_S* pstHalInfo, HI_DRV_VID_FRAME_ADDR_S* pstNext2)
{
    HI_U8* pvir_addr_y = HI_NULL;
    HI_U8* pvir_addr_y_header = HI_NULL;
    HI_BOOL	bCmpInput;
    HI_S32 s32Ret;

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

    {
        bCmpInput = HI_TRUE;
    }
    else
    {
        bCmpInput = HI_FALSE;
    }

    pstHalInfo->stRwzbInfo.u32Width = pstNext2->u32Stride_Y; //下面软件访问像素点时,要按照stride计算偏移

    if (HI_NULL == pstNext2->vir_addr_y)
    {
        if (bCmpInput)
        {
            pvir_addr_y_header = VPSS_OSAL_MEMMap(pstHalInfo->stInInfo.bSecure, pstNext2->u32PhyAddr_YHead);
            pvir_addr_y 	   = pvir_addr_y_header + pstNext2->u32Head_Size;
        }
        else
        {
            pvir_addr_y = VPSS_OSAL_MEMMap(pstHalInfo->stInInfo.bSecure, pstNext2->u32PhyAddr_Y);
            pvir_addr_y_header =  pvir_addr_y;
        }

        if (HI_NULL != pvir_addr_y_header)
        {
            s32Ret = VPSS_HAL_Get_RwzbPixel(pvir_addr_y, &pstHalInfo->stRwzbInfo, pstHalInfo->stRwzbInfo.u32Mode, HI_FALSE);
            s32Ret |= VPSS_OSAL_MEMUnmap(pstHalInfo->stInInfo.bSecure,
                                         pstNext2->u32PhyAddr_Y, pvir_addr_y_header);

            if ((HI_SUCCESS != s32Ret) && (HI_VPSS_RET_UNSUPPORT != s32Ret))
            {
                VPSS_FATAL("VPSS_HAL_Get_RwzbPixel fail!\n");
                return HI_FAILURE;
            }
        }
        else
        {
            VPSS_FATAL("Nx2 phyaddr map to viraddr fail!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        pvir_addr_y = (HI_U8*)(HI_SIZE_T)pstNext2->vir_addr_y;
        s32Ret = VPSS_HAL_Get_RwzbPixel(pvir_addr_y, &pstHalInfo->stRwzbInfo, pstHalInfo->stRwzbInfo.u32Mode, HI_FALSE);
        if ((HI_SUCCESS != s32Ret) && (HI_VPSS_RET_UNSUPPORT != s32Ret))
        {
            VPSS_FATAL("VPSS_HAL_Get_RwzbPixel fail!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}
#ifdef VPSS_SUPPORT_OUT_TUNNEL
HI_S32 VPSS_HAL_TunnelOut_GetBufAddr(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32 u32PortID)
{
    VPSS_HAL_CTX_S*	 pstHalCtx;
    HI_U32			u32WriteAddr;

    if (enIP >= VPSS_IP_BUTT)
    {
        VPSS_ERROR("VPSS IP%d, is Not Vaild\n", enIP);
        return HI_NULL;
    }

    pstHalCtx = &stHalCtx[enIP];

#ifdef HI_VPSS_SMMU_SUPPORT
    //Always pass MMZ addr to VENC
    u32WriteAddr = VPSS_OSAL_SmmuToPhyAddr(pstHalCtx->stLowDelayBuf_MMU.u32StartSmmuAddr);
#else
    u32WriteAddr = pstHalCtx->stLowDelayBuf_MMZ.u32StartPhyAddr;
#endif

    return u32WriteAddr;
}
#endif


//NODE configuration
//VPSS_HAL_NODE_2D_FRAME = 0, 	 //2D逐行
HI_S32 VPSS_HAL_Set_2DFrame_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;
    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;
    HI_DRV_VID_FRAME_ADDR_S* pstRef;

    HI_BOOL	bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S			stFrameCfg;
    CBB_FUNC_TUNNELIN_S	stTunnelIn;
    CBB_FUNC_SMMU_S		stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S 		stDBInfo;
    CBB_ALG_INFO_S 		stDBDETInfo;
    CBB_ALG_INFO_S 		stNrInfo;
    CBB_ALG_INFO_S 		stSnrInfo;
    CBB_ALG_INFO_S 		stTnrInfo;
    CBB_ALG_INFO_S 		stMcTnrInfo;
    CBB_ALG_INFO_S 		stIfmdInfo;
    CBB_INFO_S 			sttInfoCfg;
#ifdef VPSS_SUPPORT_ALG_MCNR
    CBB_INFO_RGMV_S		stRgmvCfg;
    CBB_INFO_PRJV_S 	stPrjVCfg;
    CBB_INFO_PRJH_S 	stPrjHCfg;
    CBB_ALG_INFO_S		stDeiAlgCfg;
#endif
    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stIfmdInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    if (VPSS_ALG_IS_SD(pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height))
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stNrInfo.bEnable 	= HI_FALSE;
        stSnrInfo.bEnable	= HI_FALSE;  //此时打开SNR也没有意义
        stTnrInfo.bEnable	= HI_TRUE;
    }
    else
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stNrInfo.bEnable    = HI_FALSE;
        stSnrInfo.bEnable	= HI_FALSE; //D1 720以上都是逐行，不开
        stTnrInfo.bEnable	= HI_TRUE;
    }
    if (VPSS_IS_YUV420_FMT(pstHalInfo->stInInfo.enFormat))
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stSnrInfo.bEnable   = HI_FALSE;
    }

#ifdef VPSS_SUPPORT_ALG_MCNR
    if (VPSS_ALG_IS_MCNR_ENABLE(pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height))
    {
        stMcTnrInfo.bEnable = HI_TRUE;
    }
    else
    {
        stMcTnrInfo.bEnable	= HI_FALSE;
    }
#else
	stMcTnrInfo.bEnable	= HI_FALSE;
#endif
    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_TRUE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr

    switch (pstHalInfo->stInInfo.enFormat)
    {
        case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
        case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            stTnrInfo.bEnable = HI_FALSE;
            break;

        default:
            break;
    }

    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stSnrInfo.bEnable	= HI_FALSE;
        stTnrInfo.bEnable	= HI_FALSE;
    }

    if (HI_FALSE == stTnrInfo.bEnable)
    {
        stMcTnrInfo.bEnable = HI_FALSE;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stTnrInfo.bEnable = HI_FALSE;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

#ifdef VPSS_SUPPORT_ALG_MCNR
    if ((HI_TRUE == stMcTnrInfo.bEnable) && (HI_TRUE == stTnrInfo.bEnable))
    {
        VPSS_MEM_CLEAR(stDeiAlgCfg);
        VPSS_MEM_CLEAR(stRgmvCfg);
        VPSS_MEM_CLEAR(stPrjVCfg);
        VPSS_MEM_CLEAR(stPrjHCfg);

        stRgmvCfg.u32CurReadAddr    = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32CurReadAddr;
        stRgmvCfg.u32Nx1ReadAddr    = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx1ReadAddr;
        stRgmvCfg.u32Nx2WriteAddr   = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx2WriteAddr;
        stRgmvCfg.u32Stride         = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Stride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_RGMV, (HI_VOID*)&stRgmvCfg, HI_NULL);

        VPSS_DBG_INFO("RGMV cur nx1 nx2 stride %x %x %x %d\n", stRgmvCfg.u32CurReadAddr,
            stRgmvCfg.u32Nx1ReadAddr, stRgmvCfg.u32Nx2WriteAddr, stRgmvCfg.u32Stride );

        stPrjVCfg.u32CurReadAddr    = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32CurReadAddr;
        stPrjVCfg.u32Nx2WriteAddr   = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Nx2WriteAddr;
        stPrjVCfg.u32Stride         = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Stride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_PRJ_V, (HI_VOID*)&stPrjVCfg, HI_NULL);

        VPSS_DBG_INFO("PrjV cur nx2 stride %x %x %d\n", stPrjVCfg.u32CurReadAddr,
            stPrjVCfg.u32Nx2WriteAddr, stPrjVCfg.u32Stride);

        stPrjHCfg.u32CurReadAddr    = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32CurReadAddr;
        stPrjHCfg.u32Nx2WriteAddr   = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Nx2WriteAddr;
        stPrjHCfg.u32Stride         = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Stride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_PRJ_H, (HI_VOID*)&stPrjHCfg, HI_NULL);

        VPSS_DBG_INFO("PrjH cur nx2 stride %x %x %d\n",  stPrjHCfg.u32CurReadAddr,
                  stPrjHCfg.u32Nx2WriteAddr,  stPrjHCfg.u32Stride );

        stDeiAlgCfg.u32ParaAddr = stRegAddr.u32NodePhyAddr + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DIECTRL) - sizeof(HI_U32);
        stDeiAlgCfg.bEnable     = HI_FALSE;
        VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DEI, (HI_VOID*)&stDeiAlgCfg, HI_NULL);
    }
#endif
    /* ifmd */
    stIfmdInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

    /* next2 config */
    pstNext2 = &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat; 	// TODO: 做格式转换
    stFrameCfg.u32Width			= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive		= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y 	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y 	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size 		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_DBG_INFO("2DNode nx2 read from %08x %08x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rwzb */
    if (pstHalInfo->stRwzbInfo.u32EnRwzb)
    {
        VPSS_HAL_RWZB_Calc(pstHalInfo, pstNext2);
    }

    /* nx1 config */
    /* no need to config nx1 in 2DFrame */

    /* cur config */
    pstCur = &pstHalInfo->stInRefInfo[0].stAddr;
    stFrameCfg.u32Width		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    if (HI_DRV_PIXEL_BITWIDTH_10BIT != pstHalInfo->stInInfo.enBitWidth)
    {
        pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
        // TODO: ref. rfr 都 固定10bit回写, 计算buffer大小时也要按照10bit算
        stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
        stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
        stFrameCfg.u32Width	  = pstHalInfo->stInWbcInfo.u32Width;
        stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
    }
    else
    {
        bRfrEnableFlag = HI_FALSE;
    }

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* ref config */
    pstRef		= &pstHalInfo->stInRefInfo[0].stAddr;
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRef->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REF, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    if (stSnrInfo.bEnable)
    {
        /* snr mad */
        stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_2DDei_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;

    HI_DRV_VID_FRAME_ADDR_S* pstRef;
    HI_DRV_VID_FRAME_ADDR_S* pstNxt1;

    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stAlgInfo;
    CBB_INFO_S			sttInfoCfg;
    CBB_ALG_INFO_S		stMcDeiAlgCfg;


    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stMcDeiAlgCfg);


    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /* channel config */
    pstRef		= &pstHalInfo->stInRefInfo[0].stAddr;
    pstCur		= &pstHalInfo->stInRefInfo[1].stAddr;
    pstNxt1 	= &pstHalInfo->stInRefInfo[2].stAddr;
    pstNext2	= &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB	= &pstHalInfo->stInInfo.stAddr_LB;
    pstRfr 		= &pstHalInfo->stInWbcInfo.stAddr;
#if 0
    VPSS_DBG_INFO("Ref  phy:%08x, vir:%x\n", pstRef->u32PhyAddr_Y, (HI_U32)pstRef->vir_addr_y);
    VPSS_DBG_INFO("Cur  phy:%08x, vir:%x\n", pstCur->u32PhyAddr_Y, (HI_U32)pstCur->vir_addr_y);
    VPSS_DBG_INFO("Nx1  phy:%08x, vir:%x\n", pstNxt1->u32PhyAddr_Y, (HI_U32)pstNxt1->vir_addr_y);
    VPSS_DBG_INFO("Nx2  phy:%08x, vir:%x\n", pstNext2->u32PhyAddr_Y, (HI_U32)pstNext2->vir_addr_y);
    VPSS_DBG_INFO("Rfr  phy:%08x, vir:%x stid:%d %d\n", pstRfr->u32PhyAddr_Y, (HI_U32)pstRfr->vir_addr_y,
                                    pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
#endif

    //pstRef = pstCur = pstNxt1 = pstNext2; //Debug use. all ref to nx2



    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    //>SD < FHD时, 只开MA-TNR, DEI

    /* db */
    /* db_det */
    if (VPSS_ALG_IS_SD(pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height))
    {
        stDBDETInfo.bEnable = HI_TRUE;
        stDBInfo.bEnable = HI_TRUE;
        stNrInfo.bEnable = HI_TRUE;
    }
    else
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable = HI_FALSE;
        stNrInfo.bEnable = HI_FALSE;
    }

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_TRUE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stSnrInfo.bEnable	= HI_FALSE; //只支持标清的SNR
    stTnrInfo.bEnable	= HI_TRUE;  //只需开MA-TNR, 关闭MC-TNR
    stMcTnrInfo.bEnable = HI_FALSE;

    if (VPSS_IS_YUV420_FMT(pstHalInfo->stInInfo.enFormat))
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stSnrInfo.bEnable   = HI_FALSE;
    }

    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stSnrInfo.bEnable	= HI_FALSE;
        stTnrInfo.bEnable	= HI_FALSE;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);


    /* next2 config */
    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;	// TODO: 做格式转换
    stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive 	= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)
    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rwzb */
    if (pstHalInfo->stRwzbInfo.u32EnRwzb)
    {
        VPSS_HAL_RWZB_Calc(pstHalInfo, pstNext2);
    }

    /* nx1 config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNxt1->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNxt1->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNxt1->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNxt1->u32Stride_C;
    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[2].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[2].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX1, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* cur config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[1].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[1].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* ref config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRef->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REF, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
    // TODO: ref. rfr 都 固定10bit回写, 计算buffer大小时也要按照10bit算
    stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
    stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
    stFrameCfg.u32Width   = pstHalInfo->stInWbcInfo.u32Width;
    stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, stRegAddr);

#if 0
    VPSS_HAL_SetMcDeiCfg(enIP, pstHalInfo, pu32AppVir, u32AppPhy);
#else
    stMcDeiAlgCfg.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_MCDEI, (HI_VOID*)&stMcDeiAlgCfg, HI_NULL);
#endif

    /* igbm */
    stAlgInfo.bEnable = HI_TRUE;
    //stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IGBM, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* ifmd */
    stAlgInfo.bEnable = HI_TRUE;
    //stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

#ifdef NEVER
    /* snr mad */
    stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
    stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
#endif /* NEVER */

    return HI_SUCCESS;
}



HI_S32 VPSS_HAL_Set_2DField_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    //VPZME, 单场模式开

    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;
    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stIfmdInfo;
    CBB_INFO_S			sttInfoCfg;

    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stIfmdInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    if ((pstHalInfo->stInInfo.u32Width > 128
         && pstHalInfo->stInInfo.u32Height > 64)
        && (VPSS_ALG_IS_SD(pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height)))
    {
        stDBDETInfo.bEnable	= HI_TRUE;
        stDBInfo.bEnable		= HI_TRUE;
        stSnrInfo.bEnable		= HI_TRUE;
        //stSnrInfo.bEnable		= HI_FALSE;
    }
    else
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable 	= HI_FALSE;
        stSnrInfo.bEnable	= HI_FALSE;
    }

    if (VPSS_IS_YUV420_FMT(pstHalInfo->stInInfo.enFormat))
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stSnrInfo.bEnable   = HI_FALSE;
    }

    stNrInfo.bEnable 	= HI_FALSE;
    stTnrInfo.bEnable	= HI_FALSE;
    bRfrEnableFlag		= HI_FALSE;
    stMcTnrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr

    switch (pstHalInfo->stInInfo.enFormat)
    {
        case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
        case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            stTnrInfo.bEnable = HI_FALSE;
            break;

        default:
            break;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

    /* ifmd */
    stIfmdInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

    /* next2 config */
    pstNext2 = &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;	// TODO: 做格式转换
    stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive 	= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X	= 0;
    stFrameCfg.stOffset.s32Y	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_DBG_INFO("2DNode nx2 read from %08x %08x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* nx1 config */
    /* no need to config nx1 in 2DFrame */

    /* cur config */
    pstCur = &pstHalInfo->stInRefInfo[0].stAddr;
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
    // TODO: ref. rfr 都 固定10bit回写, 计算buffer大小时也要按照10bit算
    stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
    stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
    stFrameCfg.u32Width   = pstHalInfo->stInWbcInfo.u32Width;
    stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    if (stSnrInfo.bEnable)
    {
        /* snr mad */
        stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
    }

    return HI_SUCCESS;

}

HI_S32 VPSS_HAL_SDDeiBuffer_Alloc(VPSS_IP_E enIP, HI_BOOL bSecure)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];


#ifdef HI_VPSS_SMMU_SUPPORT

    if (bSecure)
    {
        if (pstHalCtx->stSDDeiBuf_TEE.u32Size == 0)
        {
#ifdef HI_TEE_SUPPORT
            s32Ret = HI_DRV_SECSMMU_Alloc("VPSS_DEI_MMU_S", 720 * 576 * 2, 0, &pstHalCtx->stSDDeiBuf_TEE);
#else
            s32Ret = HI_DRV_SMMU_AllocAndMap("VPSS_DEI_MMU_NS", 720 * 576 * 2, 0, &(pstHalCtx->stSDDeiBuf_TEE));
#endif
        }
    }
    else
    {
        if (pstHalCtx->stSDDeiBuf_MMU.u32Size == 0)
        {
            s32Ret = HI_DRV_SMMU_AllocAndMap("VPSS_DEI_MMU_NS", 720 * 576 * 2, 0, &(pstHalCtx->stSDDeiBuf_MMU));
        }
    }

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc sd dei mmu buffer failed\n");
        return HI_FAILURE;
    }

#else

    if (pstHalCtx->stSDDeiBuf_MMZ.u32Size == 0)
    {
        s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_SDDEI_MMZ", "VPSS", 720 * 576 * 2, 0, &(pstHalCtx->stSDDeiBuf_MMZ));

        if (s32Ret != HI_SUCCESS)
        {
            VPSS_FATAL("Alloc sd dei mmz buffer failed\n");
            return HI_FAILURE;
        }
    }

#endif

    return HI_SUCCESS;
}
HI_VOID VPSS_HAL_SDDeiBuffer_Free(VPSS_IP_E enIP)
{
    VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

#ifdef HI_VPSS_SMMU_SUPPORT

    if (pstHalCtx->stSDDeiBuf_MMU.u32Size != 0)
    {
        HI_DRV_SMMU_UnmapAndRelease(&(pstHalCtx->stSDDeiBuf_MMU));
        memset(&(pstHalCtx->stSDDeiBuf_MMU), 0, sizeof(SMMU_BUFFER_S));
    }

    if (pstHalCtx->stSDDeiBuf_TEE.u32Size != 0)
    {
#ifdef HI_TEE_SUPPORT
        (HI_VOID)HI_DRV_SECSMMU_Release(&(pstHalCtx->stSDDeiBuf_TEE));
#else
        HI_DRV_SMMU_UnmapAndRelease(&(pstHalCtx->stSDDeiBuf_TEE));
#endif
        memset(&(pstHalCtx->stSDDeiBuf_TEE), 0, sizeof(SMMU_BUFFER_S));
    }

#else

    if (pstHalCtx->stSDDeiBuf_MMZ.u32Size != 0)
    {
        HI_DRV_MMZ_UnmapAndRelease(&(pstHalCtx->stSDDeiBuf_MMZ));
        memset(&(pstHalCtx->stSDDeiBuf_MMZ), 0, sizeof(MMZ_BUFFER_S));
    }

#endif

    return;
}

HI_S32 VPSS_HAL_ConfigSDDeiFrame(	VPSS_HAL_FRAME_S* pstDeTileFrame,
                                    HI_U32 u32BufferStartAddr,
                                    HI_U32 u32Width,
                                    HI_U32 u32Height,
                                    HI_DRV_PIX_FORMAT_E ePixFormat,
                                    HI_DRV_PIXEL_BITWIDTH_E	enBitWidth)
{
    HI_U32 u32Stride;
    HI_U32 u32NodeBuffSize;
    HI_U32 u32RetStride;
    HI_U32 u32RetSize;
    HI_U32 u32PhyAddr;

    if (HI_DRV_PIX_FMT_NV21_TILE_CMP == ePixFormat
        || HI_DRV_PIX_FMT_NV12_TILE_CMP == ePixFormat
        || HI_DRV_PIX_FMT_NV21_TILE == ePixFormat
        || HI_DRV_PIX_FMT_NV12_TILE == ePixFormat
        || HI_DRV_PIX_FMT_NV21 == ePixFormat
        || HI_DRV_PIX_FMT_NV12 == ePixFormat)
    {
        ePixFormat = HI_DRV_PIX_FMT_NV21;
    }
    else
    {
        ePixFormat = HI_DRV_PIX_FMT_NV61_2X1;
    }

    if (HI_DRV_PIXEL_BITWIDTH_8BIT == enBitWidth)
    {
        u32RetStride = HI_ALIGN_8BIT_YSTRIDE(u32Width);
    }
    else if (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth)
    {
        u32RetStride = HI_ALIGN_10BIT_COMP_YSTRIDE(u32Width);
    }
    else
    {
        VPSS_FATAL("Unsupport BitWidth %d.\n", enBitWidth);
        return HI_FAILURE;
    }

    if (HI_DRV_PIX_FMT_NV21 == ePixFormat)
    {
        u32RetSize = u32Height * u32RetStride * 3 / 2;
    }
    else
    {
        u32RetSize = u32Height * u32RetStride * 2;
    }

    u32Stride = u32RetStride;
    u32NodeBuffSize = u32RetSize;

    u32PhyAddr = u32BufferStartAddr;

    pstDeTileFrame->u32Width = u32Width;
    pstDeTileFrame->u32Height = u32Height;
    pstDeTileFrame->enBitWidth = enBitWidth;
    pstDeTileFrame->enFormat = ePixFormat;
    pstDeTileFrame->stAddr.u32Stride_Y = u32Stride;
    pstDeTileFrame->stAddr.u32Stride_C = u32Stride;

    pstDeTileFrame->stAddr.u32PhyAddr_Y = u32PhyAddr;
    pstDeTileFrame->stAddr.u32PhyAddr_C = u32PhyAddr + u32Stride * u32Height;

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_Set_2DSDFirst_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;

    HI_DRV_VID_FRAME_ADDR_S* pstRef;
    HI_DRV_VID_FRAME_ADDR_S* pstNxt1;

    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;

    HI_BOOL				bRfrEnableFlag;
    HI_U32				u32PageAddr;
    HI_U32				u32ErrReadAddr;
    HI_U32				u32ErrWriteAddr;
    HI_S32				s32Ret;
    HI_U32				u32DetileBufferAddr;
    VPSS_HAL_CTX_S*		 pstHalCtx;
    VPSS_HAL_FRAME_S*	 pInInfo;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stAlgInfo;
    CBB_INFO_S			sttInfoCfg;


    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);


    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    pstHalCtx = &stHalCtx[enIP];
    pInInfo = &(pstHalInfo->stInInfo);


    s32Ret = VPSS_HAL_SDDeiBuffer_Alloc(enIP, pInInfo->bSecure);

    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("VPSS_HAL_SDDeiBuffer_Alloc fail!\n");
        return s32Ret;
    }

#ifdef HI_VPSS_SMMU_SUPPORT

    if (pInInfo->bSecure)
    {
        u32DetileBufferAddr = pstHalCtx->stSDDeiBuf_TEE.u32StartSmmuAddr;
    }
    else
    {
        u32DetileBufferAddr = pstHalCtx->stSDDeiBuf_MMU.u32StartSmmuAddr;
    }

#else
    u32DetileBufferAddr = pstHalCtx->stSDDeiBuf_MMZ.u32StartPhyAddr;
#endif

    s32Ret = VPSS_HAL_ConfigSDDeiFrame(&(pstHalInfo->stSDDeiFrame),
                                       u32DetileBufferAddr,
                                       pInInfo->u32Width,
                                       pInInfo->u32Height,
                                       pInInfo->enFormat,
                                       pInInfo->enBitWidth);

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /* channel config */
    pstRef		= &pstHalInfo->stInRefInfo[0].stAddr;
    pstCur		= &pstHalInfo->stInRefInfo[1].stAddr;
    pstNxt1 	= &pstHalInfo->stInRefInfo[2].stAddr;
    pstNext2	= &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

#if 0
    printk("\n--------------------------------------------\n");
    printk("pstRef	phy:%u, vir:%u\n", pstRef->u32PhyAddr_Y, (HI_U32)pstRef->vir_addr_y);
    printk("pstCur	phy:%u, vir:%u\n", pstCur->u32PhyAddr_Y, (HI_U32)pstCur->vir_addr_y);
    printk("pstNxt1  phy:%u, vir:%u\n", pstNxt1->u32PhyAddr_Y, (HI_U32)pstNxt1->vir_addr_y);
    printk("pstNext2  phy:%u, vir:%u\n", pstNext2->u32PhyAddr_Y, (HI_U32)pstNext2->vir_addr_y);
#endif


    VPSS_DBG_INFO("2DSDFst NX2 read from %08x stride %d \n", pstNext2->u32PhyAddr_Y, pstNext2->u32Stride_Y);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    //>SD < FHD时, 只开MA-TNR, DEI

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable = HI_FALSE;
    stNrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_TRUE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stSnrInfo.bEnable	= HI_FALSE; //node-seconed enable snr
    stTnrInfo.bEnable	= HI_TRUE;	//开MA-TNR, MC-TNR
    stMcTnrInfo.bEnable = HI_TRUE;

    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stSnrInfo.bEnable	= HI_FALSE;
        stTnrInfo.bEnable	= HI_FALSE;
    }
    if (HI_FALSE == stTnrInfo.bEnable)
    {
        stMcTnrInfo.bEnable = HI_FALSE;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

    /* next2 config */
    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;	// TODO: 做格式转换
    stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive 	= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

//    printk("stFrameCfg.bTopFirst : %u\n", stFrameCfg.bTopFirst);
 //   printk("stFrameCfg.bTop : %u\n", stFrameCfg.bTop);

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)
    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rwzb */
    if (pstHalInfo->stRwzbInfo.u32EnRwzb)
    {
        VPSS_HAL_RWZB_Calc(pstHalInfo, pstNext2);
    }

    /* nx1 config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNxt1->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNxt1->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNxt1->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNxt1->u32Stride_C;
    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[2].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[2].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX1, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* cur config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[1].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[1].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* ref config */
    stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRef->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REF, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
    // TODO: ref. rfr 都 固定10bit回写, 计算buffer大小时也要按照10bit算
    stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
    stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
    stFrameCfg.u32Width   = pstHalInfo->stInWbcInfo.u32Width;
    stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;

//    printk("wbc   phy:%u, vir:%u\n", pstRfr->u32PhyAddr_Y, (HI_U32)pstRfr->vir_addr_y);



    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg_SD_First_dei(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, stRegAddr);

    if (pstHalInfo->stRwzbInfo.u32IsRwzb == 0)
    {
        VPSS_HAL_SetMcDeiCfg(enIP, pstHalInfo, stRegAddr);
    }
    /* igbm */
    stAlgInfo.bEnable = HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IGBM, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* ifmd */
    stAlgInfo.bEnable = HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    return HI_SUCCESS;
}

//HI_U32 g_u32DbgTemCnt = 0;
//HI_DRV_VIDEO_FRAME_S g_DbgstFrame;

HI_S32 VPSS_HAL_Set_2DSDSecond_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stAlgInfo;
    CBB_INFO_S			sttInfoCfg;
    CBB_ALG_INFO_S		stMcDeiAlgCfg;
    CBB_ALG_INFO_S		stDeiAlgCfg;


    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stMcDeiAlgCfg);
    VPSS_MEM_CLEAR(stDeiAlgCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /* channel config */
    pstNext2	= &(pstHalInfo->stSDDeiFrame.stAddr);
    pstNext2_LB = &(pstHalInfo->stSDDeiFrame.stAddr_LB);

#if 0

    if (g_u32DbgTemCnt++ == 200)
    {
        HI_U8 chFile[32] = "vpss_sddei_node2_in.yuv";
        VPSS_HAL_CTX_S* pstHalCtx = HI_NULL;
        HI_U64  u64Addr;


        pstHalCtx = &stHalCtx[enIP];

        g_DbgstFrame.u32Width = pstHalInfo->stSDDeiFrame.u32Width;
        g_DbgstFrame.u32Height = pstHalInfo->stSDDeiFrame.u32Height;
        g_DbgstFrame.ePixFormat = HI_DRV_PIX_FMT_NV21;
        g_DbgstFrame.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;

        u64Addr = (unsigned long)pstHalCtx->stSDDeiBuf_MMU.pu8StartVirAddr;
        g_DbgstFrame.stBufAddr[0].vir_addr_y = u64Addr;
        g_DbgstFrame.stBufAddr[0].vir_addr_c = (u64Addr + 720 * 576);

        g_DbgstFrame.stBufAddr[0].u32Stride_Y = pstHalInfo->stSDDeiFrame.stAddr.u32Stride_Y;
        g_DbgstFrame.stBufAddr[0].u32Stride_C = pstHalInfo->stSDDeiFrame.stAddr.u32Stride_C;

        VPSS_OSAL_WRITEYUV(&g_DbgstFrame, chFile);
    }

#endif

#if 0
    printk("pstRef	phy:%u, vir:%u\n", pstRef->u32PhyAddr_Y, (HI_U32)pstRef->vir_addr_y);
    printk("pstCur	phy:%u, vir:%u\n", pstCur->u32PhyAddr_Y, (HI_U32)pstCur->vir_addr_y);
    printk("pstNxt1  phy:%u, vir:%u\n", pstNxt1->u32PhyAddr_Y, (HI_U32)pstNxt1->vir_addr_y);
    printk("pstNext2  phy:%u, vir:%u\n", pstNext2->u32PhyAddr_Y, (HI_U32)pstNext2->vir_addr_y);
#endif

    /*tunnel*/
    stTunnelIn.bEnable = HI_FALSE; //second node's tunnel-in fix to disable
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    //>SD < FHD时, 只开MA-TNR, DEI

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_TRUE;
    stDBInfo.bEnable = HI_TRUE;
    stNrInfo.bEnable = HI_TRUE;

    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable = HI_FALSE;
        stNrInfo.bEnable = HI_FALSE;
    }

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_FALSE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stSnrInfo.bEnable	= HI_TRUE;
    stTnrInfo.bEnable	= HI_FALSE;
    stMcTnrInfo.bEnable = HI_FALSE;

    if (VPSS_IS_YUV420_FMT(pstHalInfo->stInInfo.enFormat))
    {
        stDBDETInfo.bEnable = HI_FALSE;
        stDBInfo.bEnable    = HI_FALSE;
        stSnrInfo.bEnable   = HI_FALSE;
    }

    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stSnrInfo.bEnable	= HI_FALSE;
        stTnrInfo.bEnable	= HI_FALSE;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);


    /* next2 config */
    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;	// TODO: 做格式转换
    stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive 	= HI_TRUE; //pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;
#if 0
    printk("VPSS sd snd read addr Y C stridY stridC %x %x %d %d\n",
           stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
           stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
           stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
           stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);
#endif

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    stFrameCfg.bCompress = HI_FALSE;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);


    /* nx1 config */
    //no need to cfg nx1
    /* cur config */
    //no need to cfg cur
    /* ref config */
    //no need to cfg ref

    /* rfr config */
    //DO　NOT!!! cfg rfr

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);


    stDeiAlgCfg.bEnable 	= HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DEI, (HI_VOID*)&stDeiAlgCfg, HI_NULL);

    stMcDeiAlgCfg.bEnable 	= HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_MCDEI, (HI_VOID*)&stMcDeiAlgCfg, HI_NULL);

    /* igbm */
    stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IGBM, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* ifmd */
    stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

    /* snr mad */
    stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
    stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_CTS10_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;
    //	HI_DRV_VID_FRAME_ADDR_S *pstCur;
    //	HI_DRV_VID_FRAME_ADDR_S *pstRfr;

    HI_BOOL	bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S			stFrameCfg;
    CBB_FUNC_TUNNELIN_S	stTunnelIn;
    CBB_FUNC_SMMU_S		stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S 		stDBInfo;
    CBB_ALG_INFO_S 		stDBDETInfo;
    CBB_ALG_INFO_S 		stNrInfo;
    CBB_ALG_INFO_S 		stSnrInfo;
    CBB_ALG_INFO_S 		stTnrInfo;
    CBB_ALG_INFO_S 		stMcTnrInfo;
    CBB_ALG_INFO_S 		stIfmdInfo;
    CBB_INFO_S 			sttInfoCfg;
    CBB_FUNC_CTS10BIT_S stFuncCTS10bitCfg;
    CBB_FUNC_4PIXEL_S   stFunc4PixelCfg;

    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stIfmdInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stFuncCTS10bitCfg);
    VPSS_MEM_CLEAR(stFunc4PixelCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    //4p/c 区分不同的NODE 开关, cts. 4k缩放

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/

    stTunnelIn.bEnable = HI_FALSE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable 	= HI_FALSE;
    stNrInfo.bEnable 	= HI_FALSE;
    stSnrInfo.bEnable	= HI_FALSE;  //此时打开SNR也没有意义
    stTnrInfo.bEnable	= HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_FALSE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stMcTnrInfo.bEnable	= HI_FALSE;

    switch (pstHalInfo->stInInfo.enFormat)
    {
        case HI_DRV_PIX_FMT_NV12_TILE: /* 12 tile  */
        case HI_DRV_PIX_FMT_NV21_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_YUV400_TILE: /* 21 tile  */
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            stTnrInfo.bEnable = HI_FALSE;
            break;

        default:
            break;
    }

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

    /* ifmd */
    stIfmdInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

    /* next2 config */
    pstNext2 = &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat; 	// TODO: 做格式转换
    stFrameCfg.u32Width			= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive		= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y 	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if (( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
        || ( HI_DRV_PIXEL_BITWIDTH_10BIT_CTS == pstHalInfo->stInInfo.enBitWidth ))
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y 	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size 		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_DBG_INFO("NX2 read from %x %x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);


    stFuncCTS10bitCfg.bEnable = HI_TRUE;
    stFuncCTS10bitCfg.bBitSel_LOW10 = HI_FALSE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_CTS10BIT, (HI_VOID*)&stFuncCTS10bitCfg, HI_NULL);

    stFunc4PixelCfg.bEnable = HI_TRUE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_SPEED_UP_4PIXEL, (HI_VOID*)&stFunc4PixelCfg, HI_NULL);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg_CTS10Bit(stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    if (stSnrInfo.bEnable)
    {
        /* snr mad */
        stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
    }

    return HI_SUCCESS;
}


//VPSS_HAL_NODE_2D_UHD = 11, 	 //4K
HI_S32 VPSS_HAL_Set_2D_UHDFrame_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;
    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;

    HI_BOOL	bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S			stFrameCfg;
    CBB_FUNC_TUNNELIN_S	stTunnelIn;
    CBB_FUNC_SMMU_S		stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S 		stDBInfo;
    CBB_ALG_INFO_S 		stDBDETInfo;
    CBB_ALG_INFO_S 		stNrInfo;
    CBB_ALG_INFO_S 		stSnrInfo;
    CBB_ALG_INFO_S 		stTnrInfo;
    CBB_ALG_INFO_S 		stMcTnrInfo;
    CBB_ALG_INFO_S 		stIfmdInfo;
    CBB_FUNC_4PIXEL_S	stFunc4PixelCfg;
    CBB_INFO_S 			sttInfoCfg;

    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stIfmdInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stFunc4PixelCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable = HI_FALSE;
    stNrInfo.bEnable = HI_FALSE;
    stSnrInfo.bEnable	= HI_FALSE; //D1 720以上都是逐行，不开
    stTnrInfo.bEnable	= HI_FALSE;
    bRfrEnableFlag		= HI_FALSE;
    stMcTnrInfo.bEnable	= HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);


    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stTnrInfo.bEnable = HI_FALSE;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

    /* ifmd */
    stIfmdInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

    /* next2 config */
    pstNext2 = &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat; 	// TODO: 做格式转换
    stFrameCfg.u32Width			= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive		= pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y 	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y 	= pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size 		= pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_DBG_INFO("2DNode nx2 read from %08x %08x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rwzb */
    if (pstHalInfo->stRwzbInfo.u32EnRwzb)
    {
        VPSS_HAL_RWZB_Calc(pstHalInfo, pstNext2);
    }

    /* nx1 config */
    /* no need to config nx1 in 2DFrame */

    /* cur config */
    pstCur = &pstHalInfo->stInRefInfo[0].stAddr;
    stFrameCfg.u32Width		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
    stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    if (HI_DRV_PIXEL_BITWIDTH_10BIT != pstHalInfo->stInInfo.enBitWidth)
    {
        pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
        // TODO: ref. rfr 都 固定10bit回写, 计算buffer大小时也要按照10bit算
        stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
        stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
        stFrameCfg.u32Width	  = pstHalInfo->stInWbcInfo.u32Width;
        stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
    }
    else
    {
        bRfrEnableFlag = HI_FALSE;
    }

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    stFunc4PixelCfg.bEnable = HI_TRUE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_SPEED_UP_4PIXEL, (HI_VOID*)&stFunc4PixelCfg, HI_NULL);


    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    if (stSnrInfo.bEnable)
    {
        /* snr mad */
        stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
    }

    return HI_SUCCESS;
}




HI_S32 VPSS_HAL_Set_3DFrameL_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_S32 s32RetCode = HI_SUCCESS;
    s32RetCode = VPSS_HAL_Set_2DFrame_Node(enIP, pstHalInfo, pu32AppVir, u32AppPhy);
    return s32RetCode;
}

HI_S32 VPSS_HAL_Set_3DFrameR_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    HI_S32 s32RetCode = HI_SUCCESS;
    s32RetCode = VPSS_HAL_Set_2DFrame_Node(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

    return s32RetCode;
}

#if 0
HI_U32 g_DBGFramCnt = 0;
HI_DRV_VIDEO_FRAME_S g_DBGstFrame;
#endif

HI_S32 VPSS_HAL_SetRotateNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy, HI_BOOL bRotateY, HI_U32 u32PortId)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VIDEO_FRAME_S    *pRotaFrame;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stAlgInfo;
    CBB_INFO_S			sttInfoCfg;
    CBB_ALG_INFO_S		stMcDeiAlgCfg;
    CBB_ALG_INFO_S		stDeiAlgCfg;


    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stMcDeiAlgCfg);
    VPSS_MEM_CLEAR(stDeiAlgCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /* channel config */
    pstNext2	= &(pstHalInfo->astPortInfo[u32PortId].stFrmNodeRoBuf.stOutFrame.stBufAddr[HI_DRV_BUF_ADDR_LEFT]);
    pRotaFrame = &(pstHalInfo->astPortInfo[u32PortId].stFrmNodeRoBuf.stOutFrame);

    VPSS_DBG_INFO(

        "RotaNode read from Y C StrideY StrideY %08x %08x %d %d\n",
        pstNext2->u32PhyAddr_Y,
        pstNext2->u32PhyAddr_C,
        pstNext2->u32Stride_Y,
        pstNext2->u32Stride_C
    );

#if 0

    if (g_DBGFramCnt++ == 200)
    {
        HI_U8 chFile[32] = "vpss_rota_in.yuv";

        g_DBGstFrame.u32Width = pRotaFrame->u32Width;
        g_DBGstFrame.u32Height = pRotaFrame->u32Height;
        g_DBGstFrame.ePixFormat = HI_DRV_PIX_FMT_NV21;
        g_DBGstFrame.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
        g_DBGstFrame.stBufAddr[0] = *pstNext2;

        g_DBGstFrame.stBufAddr[0].u32Stride_Y = pstNext2->u32Stride_Y;
        g_DBGstFrame.stBufAddr[0].u32Stride_C = pstNext2->u32Stride_C;

        VPSS_FATAL("RotaNode save temp img  W H stridY stridC %d %d %d %d\n",
                   g_DBGstFrame.u32Width, g_DBGstFrame.u32Height,
                   g_DBGstFrame.stBufAddr[0].u32Stride_Y,
                   g_DBGstFrame.stBufAddr[0].u32Stride_C);
        VPSS_OSAL_WRITEYUV(&g_DBGstFrame, chFile);
    }

#endif

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    //>SD < FHD时, 只开MA-TNR, DEI

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable = HI_FALSE;
    stNrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag		= HI_FALSE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stSnrInfo.bEnable	= HI_FALSE;
    stTnrInfo.bEnable	= HI_FALSE;
    stMcTnrInfo.bEnable = HI_FALSE;

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);


    /* next2 config */
    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt		= HI_DRV_PIX_FMT_NV21; //pstHalInfo->stInInfo.enFormat;	// TODO: 做格式转换

    stFrameCfg.u32Width 		= pRotaFrame->u32Width;
    stFrameCfg.u32Height		= pRotaFrame->u32Height;
    stFrameCfg.bProgressive 	= HI_TRUE; //pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X 	= 0;
    stFrameCfg.stOffset.s32Y 	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

    stFrameCfg.bCompress = HI_FALSE;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* nx1 config */
    //no need to cfg nx1
    /* cur config */
    //no need to cfg cur
    /* ref config */
    //no need to cfg ref

    /* rfr config */
    //DO　NOT!!! cfg rfr

    /* vhd0 config */
    VPSS_HAL_SetPortCfg_Rota(stRegAddr, pstHalInfo, bRotateY);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);


    stDeiAlgCfg.bEnable 	= HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DEI, (HI_VOID*)&stDeiAlgCfg, HI_NULL);

    stMcDeiAlgCfg.bEnable	= HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_MCDEI, (HI_VOID*)&stMcDeiAlgCfg, HI_NULL);

    /* igbm */
    stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IGBM, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* ifmd */
    stAlgInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

    /* snr mad */
    stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
    stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_RotaY_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    VPSS_HAL_SetRotateNode(enIP, pstHalInfo, pu32AppVir, u32AppPhy, HI_TRUE, 0);
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_RotaC_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    VPSS_HAL_SetRotateNode(enIP, pstHalInfo, pu32AppVir, u32AppPhy, HI_FALSE, 0);
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_HDR_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
	HI_DRV_VID_FRAME_ADDR_S* pstNext2;
	HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;
	HI_DRV_VID_FRAME_ADDR_S* pstCur;
	HI_DRV_VID_FRAME_ADDR_S* pstRfr;

	HI_BOOL bRfrEnableFlag;
	HI_U32 u32PageAddr;
	HI_U32 u32ErrReadAddr;
	HI_U32 u32ErrWriteAddr;

	CBB_REG_ADDR_S		stRegAddr;
	CBB_REG_ADDR_S		stRegAddr_MMU;
	CBB_FRAME_S 		stFrameCfg;
	CBB_FUNC_TUNNELIN_S stTunnelIn;
	CBB_FUNC_SMMU_S 	stSMMUCfg;
	CBB_INFO_S			stMadInfo_Tnr;
	CBB_INFO_S			stMadInfo_Snr;
	CBB_ALG_INFO_S		stDBInfo;
	CBB_ALG_INFO_S		stDBDETInfo;
	CBB_ALG_INFO_S		stNrInfo;
	CBB_ALG_INFO_S		stSnrInfo;
	CBB_ALG_INFO_S		stTnrInfo;
	CBB_ALG_INFO_S		stMcTnrInfo;
	CBB_ALG_INFO_S		stIfmdInfo;
	CBB_FUNC_4PIXEL_S	stFunc4PixelCfg;
	CBB_INFO_S			sttInfoCfg;

	VPSS_MEM_CLEAR(stRegAddr);
	VPSS_MEM_CLEAR(stRegAddr_MMU);
	VPSS_MEM_CLEAR(stFrameCfg);
	VPSS_MEM_CLEAR(stTunnelIn);
	VPSS_MEM_CLEAR(stSMMUCfg);
	VPSS_MEM_CLEAR(stMadInfo_Tnr);
	VPSS_MEM_CLEAR(stMadInfo_Snr);
	VPSS_MEM_CLEAR(stDBInfo);
	VPSS_MEM_CLEAR(stDBDETInfo);
	VPSS_MEM_CLEAR(stNrInfo);
	VPSS_MEM_CLEAR(stSnrInfo);
	VPSS_MEM_CLEAR(stTnrInfo);
	VPSS_MEM_CLEAR(stMcTnrInfo);
	VPSS_MEM_CLEAR(stIfmdInfo);
	VPSS_MEM_CLEAR(sttInfoCfg);
	VPSS_MEM_CLEAR(stFunc4PixelCfg);

	stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
	stRegAddr.u32NodePhyAddr = u32AppPhy;

	VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

	/*tunnel*/
	if (pstHalInfo->stInInfo.u32TunnelAddr)
	{
		stTunnelIn.bEnable = HI_TRUE;
		stTunnelIn.u32ReadInterval = 0;
		stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
	}
	else
	{
		stTunnelIn.bEnable = HI_FALSE;
	}

	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

	/* db */
	/* db_det */
	stDBDETInfo.bEnable = HI_FALSE;
	stDBInfo.bEnable 	= HI_FALSE;
	stNrInfo.bEnable 	= HI_FALSE;
	stSnrInfo.bEnable	= HI_FALSE; //D1 720以上都是逐行，不开
	stTnrInfo.bEnable	= HI_FALSE;
	bRfrEnableFlag		= HI_FALSE;
	stMcTnrInfo.bEnable = HI_FALSE;

	stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);


	/* snr, tnr */
	//In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
	stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

	//TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
	stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

	if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
	{
		stTnrInfo.u32Ymotionmode = 1;
	}
	else
	{
		stTnrInfo.u32Ymotionmode = 0;
	}

	if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		stTnrInfo.bEnable = HI_FALSE;
	}

	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

	/* ifmd */
	stIfmdInfo.bEnable = HI_FALSE;
	VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

	/* next2 config */
	pstNext2 = &pstHalInfo->stInInfo.stAddr;
	pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;

	stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
	stFrameCfg.enBitWidth		= pstHalInfo->stInInfo.enBitWidth;
	stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;
	stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
	stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height;
	stFrameCfg.bProgressive 	= pstHalInfo->stInInfo.bProgressive;
	stFrameCfg.stOffset.s32X	= 0;
	stFrameCfg.stOffset.s32Y	= 0;


	stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

	if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
	{
		stFrameCfg.bTop = HI_TRUE;
	}
	else
	{
		stFrameCfg.bTop = HI_FALSE;
	}

	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y	= pstNext2->u32PhyAddr_Y;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C	= pstNext2->u32PhyAddr_C;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;

	if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y	= pstNext2_LB->u32PhyAddr_Y;
		stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C	= pstNext2_LB->u32PhyAddr_C;
		stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y	= pstNext2_LB->u32Stride_Y;
		stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C	= pstNext2_LB->u32Stride_C;
	}

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
		|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
		|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
		|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)

	{
		stFrameCfg.bCompress = HI_TRUE;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead	= pstNext2->u32PhyAddr_YHead;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead	= pstNext2->u32PhyAddr_CHead;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size		= pstNext2->u32Head_Size;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride	= pstNext2->u32Head_Stride;
	}
	else
	{
		stFrameCfg.bCompress = HI_FALSE;
	}

	VPSS_DBG_INFO("HDREL nx2 read from %08x %08x %d %d\n",
				  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
				  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
				  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
				  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

	VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

	/* rwzb */
	if (pstHalInfo->stRwzbInfo.u32EnRwzb)
	{
		VPSS_HAL_RWZB_Calc(pstHalInfo, pstNext2);
	}

	/* nx1 config */
	/* no need to config nx1 in HDRFrame */

	/* cur config */
	pstCur = &pstHalInfo->stInRefInfo[0].stAddr;
	stFrameCfg.u32Width 	= pstHalInfo->stInInfo.u32Width;
	stFrameCfg.u32Height	= pstHalInfo->stInInfo.u32Height;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
	stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

	stFrameCfg.enPixelFmt = pstHalInfo->stInRefInfo[0].enFormat;
	stFrameCfg.enBitWidth = pstHalInfo->stInRefInfo[0].enBitWidth;
	VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

	/* rfr config */
	if (HI_DRV_PIXEL_BITWIDTH_10BIT != pstHalInfo->stInInfo.enBitWidth)
	{
		pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
		stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
		stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
		stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
		stFrameCfg.u32Width   = pstHalInfo->stInWbcInfo.u32Width;
		stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
	}
	else
	{
		bRfrEnableFlag = HI_FALSE;
	}

	VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

	/* vhd0 config */
	VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

#if 0
	stFunc4PixelCfg.bEnable = HI_TRUE;
	VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_SPEED_UP_4PIXEL, (HI_VOID*)&stFunc4PixelCfg, HI_NULL);
#endif

	//逐隔行都要配置
	sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
	VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

	/* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
	stSMMUCfg.bGlobalBypass = HI_FALSE;
	stSMMUCfg.bIntEnable = HI_TRUE;
	stSMMUCfg.u32Ptw_pf = 0x3;
	stSMMUCfg.u32BypassFlag_RCH = 0x0;
	stSMMUCfg.u32BypassFlag_WCH = 0x0;

	HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
	stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
	stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
	stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
	stSMMUCfg.bGlobalBypass = HI_TRUE;
	stSMMUCfg.bIntEnable = HI_FALSE;
	stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
	stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

	stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
	stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
	VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


	/* tnr mad */
	stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
	stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
	stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
	VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

	if (stSnrInfo.bEnable)
	{
		/* snr mad */
		stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
		VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
	}

	return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetNode_H265_Step1_Interlace(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstNext2_LB;

    HI_DRV_VID_FRAME_ADDR_S* pstRef;
    HI_DRV_VID_FRAME_ADDR_S* pstNxt1;

    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;


    CBB_REG_ADDR_S      stRegAddr;
    CBB_REG_ADDR_S      stRegAddr_MMU;
    CBB_FRAME_S         stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S     stSMMUCfg;
    CBB_INFO_S          stMadInfo_Tnr;
    CBB_INFO_S          stMadInfo_Snr;
    CBB_ALG_INFO_S      stDBInfo;
    CBB_ALG_INFO_S      stDBDETInfo;
    CBB_ALG_INFO_S      stNrInfo;
    CBB_ALG_INFO_S      stSnrInfo;
    CBB_ALG_INFO_S      stTnrInfo;
    CBB_ALG_INFO_S      stMcTnrInfo;
    CBB_ALG_INFO_S      stAlgInfo;
    CBB_INFO_S          sttInfoCfg;
    CBB_ALG_INFO_S      stMcDeiAlgCfg;
    CBB_FRAME_S             stFrmCfg;
    VPSS_HAL_PORT_INFO_S*   pstHalPort;
    HI_BOOL                 bUVInver;
    CBB_FUNC_FLIP_S         stFlipInfo;
    CBB_FUNC_CROP_S         stFuncCropCfg;
    CBB_FUNC_ZME_S          stFuncZmeCfg;
    CBB_FUNC_LBOX_S         stLBox;
    CBB_FUNC_VPZME_S        stFuncVPZmeCfg;
    CBB_FUNC_4PIXEL_S       stFunc4PixelCfg;
    CBB_FUNC_TUNNELOUT_S    stFuncTunnelOutCfg;
    CBB_FUNC_TRANS_S        stTransInfo;
    VPSS_HAL_FRAME_S        *pstInInfo;
    HI_PQ_ZME_PARA_IN_S     *pstZmeDrvPara;


    VPSS_MEM_CLEAR(stFrmCfg);
    VPSS_MEM_CLEAR(stFlipInfo);
    VPSS_MEM_CLEAR(stFuncCropCfg);
    VPSS_MEM_CLEAR(stFuncZmeCfg);
    VPSS_MEM_CLEAR(stLBox);
    VPSS_MEM_CLEAR(stFuncVPZmeCfg);
    VPSS_MEM_CLEAR(stFunc4PixelCfg);
    VPSS_MEM_CLEAR(stFuncTunnelOutCfg);
    VPSS_MEM_CLEAR(stTransInfo);
    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stMcDeiAlgCfg);


    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    /* channel config */
    pstRef      = &pstHalInfo->stInRefInfo[0].stAddr;
    pstCur      = &pstHalInfo->stInRefInfo[1].stAddr;
    pstNxt1     = &pstHalInfo->stInRefInfo[2].stAddr;
    pstNext2    = &pstHalInfo->stInInfo.stAddr;
    pstNext2_LB = &pstHalInfo->stInInfo.stAddr_LB;
    pstRfr      = &pstHalInfo->stInWbcInfo.stAddr;
    pstInInfo   = &(pstHalInfo->stInInfo);
#if 0
    VPSS_DBG_INFO("Ref  phy:%08x, vir:%x\n", pstRef->u32PhyAddr_Y, (HI_U32)pstRef->vir_addr_y);
    VPSS_DBG_INFO("Cur  phy:%08x, vir:%x\n", pstCur->u32PhyAddr_Y, (HI_U32)pstCur->vir_addr_y);
    VPSS_DBG_INFO("Nx1  phy:%08x, vir:%x\n", pstNxt1->u32PhyAddr_Y, (HI_U32)pstNxt1->vir_addr_y);
    VPSS_DBG_INFO("Nx2  phy:%08x, vir:%x\n", pstNext2->u32PhyAddr_Y, (HI_U32)pstNext2->vir_addr_y);
    VPSS_DBG_INFO("Rfr  phy:%08x, vir:%x\n", pstRfr->u32PhyAddr_Y, (HI_U32)pstRfr->vir_addr_y);
#endif
    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        stTunnelIn.bEnable = HI_TRUE;
        stTunnelIn.u32ReadInterval = 0;
        stTunnelIn.u32ReadAddr = pstHalInfo->stInInfo.u32TunnelAddr;
    }
    else
    {
        stTunnelIn.bEnable = HI_FALSE;
    }

    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable = HI_FALSE;
    stNrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag      = HI_FALSE;
    stSnrInfo.bEnable   = HI_FALSE;
    stTnrInfo.bEnable   = HI_FALSE;
    stMcTnrInfo.bEnable = HI_FALSE;

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);


    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);


    /* next2 config */
    stFrameCfg.bSecure          = pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth       = pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt       = pstHalInfo->stInInfo.enFormat;    // TODO: 做格式转换
    stFrameCfg.u32Width         = pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height        = pstHalInfo->stInInfo.u32Height;
    stFrameCfg.bProgressive     = HI_TRUE; //pstHalInfo->stInInfo.bProgressive;
    stFrameCfg.stOffset.s32X    = 0;
    stFrameCfg.stOffset.s32Y    = 0;

    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstNext2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstNext2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y   = pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C   = pstNext2->u32Stride_C;

    if ( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_Y   = pstNext2_LB->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32PhyAddr_C   = pstNext2_LB->u32PhyAddr_C;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_Y    = pstNext2_LB->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_2BIT_LB].u32Stride_C    = pstNext2_LB->u32Stride_C;
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP)
    {
        stFrameCfg.bCompress = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_YHead  = pstNext2->u32PhyAddr_YHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_CHead  = pstNext2->u32PhyAddr_CHead;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Size      = pstNext2->u32Head_Size;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Head_Stride    = pstNext2->u32Head_Stride;
    }
    else
    {
        stFrameCfg.bCompress = HI_FALSE;
    }

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);



	VPSS_DBG_INFO("STEP1: Read nx2 YAddr YS CS %x %d %d\n",
	        pstNext2->u32PhyAddr_Y,
	        pstNext2->u32Stride_Y, pstNext2->u32Stride_C);

    pstHalPort = &pstHalInfo->astPortInfo[0];
    if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
    {
        VPSS_REG_PORT_E enPort = VPSS_REG_BUTT;
        VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
        HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;

        enPort = VPSS_REG_HD;

        pstOutFrm = &pstHalInfo->stInInfo;
        pstOutAddr = &(pstHalInfo->pstH265RefList->stRefListAddr[pstHalInfo->pstH265RefList->u32RefListHead]);
		pstHalInfo->pstH265RefList->abRefNodeValid[pstHalInfo->pstH265RefList->u32RefListHead] = HI_TRUE;

        if (0)
        {
			HI_U8 chFile[64] = "vpss_detile.yuv";
			HI_DRV_VIDEO_FRAME_S stFrame;

            memset(&stFrame, 0x0, sizeof(stFrame));

			stFrame.u32Width = pstHalInfo->stInInfo.u32Width;
			stFrame.u32Height = pstHalInfo->stInInfo.u32Height;
			stFrame.ePixFormat = HI_DRV_PIX_FMT_NV21;
			stFrame.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
			stFrame.bSecure = HI_FALSE;
			stFrame.stBufAddr[0] = *pstOutAddr;
			stFrame.stBufAddr[0].u32Stride_Y = pstOutAddr->u32Stride_Y * 2;
			stFrame.stBufAddr[0].u32Stride_C = pstOutAddr->u32Stride_C * 2;

            VPSS_DBG_INFO("STEP1: tmp img save: Yaddr w h YS CS %x %d %d %d %d",
              stFrame.stBufAddr[0].u32PhyAddr_Y,
              stFrame.u32Width, stFrame.u32Height, stFrame.stBufAddr[0].u32Stride_Y,
              stFrame.stBufAddr[0].u32Stride_C);
			VPSS_OSAL_WRITEYUV(&stFrame, chFile);
        }

        stFuncTunnelOutCfg.bEnable = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_OUT, (HI_VOID*)&stFuncTunnelOutCfg, HI_NULL);

        stFuncVPZmeCfg.bEnable = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_VPZME, (HI_VOID*)&stFuncVPZmeCfg, HI_NULL);

        stFlipInfo.bFlipH = HI_FALSE;
        stFlipInfo.bFlipV = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_FLIP, (HI_VOID*)&stFlipInfo, HI_NULL);

        bUVInver = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_UV, (HI_VOID*)&bUVInver, HI_NULL);

        stFuncCropCfg.bEnable = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_CROP, (HI_VOID*)&stFuncCropCfg, HI_NULL);

        /*ZME*/
        stFuncZmeCfg.u32ParAddr = stRegAddr.u32NodePhyAddr + 0x2000;
        stFuncZmeCfg.bEnable    = HI_FALSE;
        stFuncZmeCfg.eMode      = CBB_ZME_MODE_ALL;

        pstZmeDrvPara  = &(stFuncZmeCfg.stZmeDrvPara);
        memset(pstZmeDrvPara, 0, sizeof(HI_PQ_ZME_PARA_IN_S));
        pstZmeDrvPara->bZmeFrmFmtIn = 1;
        pstZmeDrvPara->bZmeFrmFmtOut = 1;
        pstZmeDrvPara->bZmeBFIn = 0;
        pstZmeDrvPara->bZmeBFOut = 0;

        if (pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV61
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_NV16
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV422_1X2
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV420p
            || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p)
        {
            pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
        }
        else
        {
            pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
        }

        if (pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21
            || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12
            || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV21_CMP
            || pstHalPort->stOutInfo.enFormat == HI_DRV_PIX_FMT_NV12_CMP)
        {
            pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
        }
        else
        {
            pstZmeDrvPara->u8ZmeYCFmtOut = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
        }

        pstZmeDrvPara->u32ZmeFrmHIn = pstInInfo->u32Height;
        pstZmeDrvPara->u32ZmeFrmWIn = pstInInfo->u32Width;

        pstZmeDrvPara->u32ZmeFrmHOut = pstZmeDrvPara->u32ZmeFrmHIn;
        pstZmeDrvPara->u32ZmeFrmWOut = pstZmeDrvPara->u32ZmeFrmWIn;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_ZME, (HI_VOID*)&stFuncZmeCfg, HI_NULL);

        stLBox.bEnable = HI_FALSE;
        VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_VHD0_LBOX, (HI_VOID*)&stLBox, HI_NULL);


        stFrmCfg.enBitWidth = pstOutFrm->enBitWidth;
        stFrmCfg.enPixelFmt = HI_DRV_PIX_FMT_NV21;
        stFrmCfg.u32Width	= pstOutFrm->u32Width;
        stFrmCfg.u32Height	= pstOutFrm->u32Height;
        stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstOutAddr->u32PhyAddr_Y;
        stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstOutAddr->u32PhyAddr_C;
        stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstOutAddr->u32Stride_Y * 2;
        stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstOutAddr->u32Stride_C * 2;

        VPSS_DBG_INFO(
            "Vhd0 Y C StrideY StrideC W H %08x %08x %d %d %d %d\n",
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
            stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C,
            stFrmCfg.u32Width,
            stFrmCfg.u32Height);

        VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_VHD0, (HI_VOID*)&stFrmCfg, HI_NULL);
		VPSS_DBG_INFO("STEP1: vhd0 addr W H YS CS %x %d %d %d %d\n",
                            pstOutAddr->u32PhyAddr_Y,
                            pstOutFrm->u32Width,pstOutFrm->u32Height,
							stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
							stFrmCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);
    }

        /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetNode_H265_Step2_Dei(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
	HI_U32 u32Head;
    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr  = HI_NULL;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S      stRegAddr;
    CBB_REG_ADDR_S      stRegAddr_MMU;
    CBB_FRAME_S         stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S     stSMMUCfg;
    CBB_INFO_S          stMadInfo_Tnr;
    CBB_INFO_S          stMadInfo_Snr;
    CBB_ALG_INFO_S      stDBInfo;
    CBB_ALG_INFO_S      stDBDETInfo;
    CBB_ALG_INFO_S      stNrInfo;
    CBB_ALG_INFO_S      stSnrInfo;
    CBB_ALG_INFO_S      stTnrInfo;
    CBB_ALG_INFO_S      stMcTnrInfo;
    CBB_ALG_INFO_S      stAlgInfo;
    CBB_INFO_S          sttInfoCfg;
    CBB_ALG_INFO_S      stMcDeiAlgCfg;

    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stAlgInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);
    VPSS_MEM_CLEAR(stMcDeiAlgCfg);


    VPSS_HAL_CHECK_IP_VAILD(enIP);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);


    u32Head = pstHalInfo->pstH265RefList->u32RefListHead;

    //需要考虑前4场时，每个缓冲BUFFER的有效性
	if (!pstHalInfo->pstH265RefList->abRefNodeValid[0])
	{
		VPSS_FATAL("!!pstHalInfo->pstH265RefList->abRefNodeValid[0]!");
	}

	if (!pstHalInfo->pstH265RefList->abRefNodeValid[1])
	{
		pstRef	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstNxt2 = &pstHalInfo->pstH265RefList->stRefListAddr[0];
	}
	else if (!pstHalInfo->pstH265RefList->abRefNodeValid[2])
	{
		pstRef	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstNxt2 = &pstHalInfo->pstH265RefList->stRefListAddr[1];
	}
	else if (!pstHalInfo->pstH265RefList->abRefNodeValid[3])
	{
		pstRef	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstCur	= &pstHalInfo->pstH265RefList->stRefListAddr[0];
		pstNxt1 = &pstHalInfo->pstH265RefList->stRefListAddr[1];
		pstNxt2 = &pstHalInfo->pstH265RefList->stRefListAddr[2];
	}
	else
	{
		pstRef	= &pstHalInfo->pstH265RefList->stRefListAddr[(u32Head+1)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstCur	= &pstHalInfo->pstH265RefList->stRefListAddr[(u32Head+2)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstNxt1 = &pstHalInfo->pstH265RefList->stRefListAddr[(u32Head+3)%DEF_VPSS_HAL_REF_LIST_NUM];
		pstNxt2 = &pstHalInfo->pstH265RefList->stRefListAddr[(u32Head)];
	}

    //pstRef = pstCur = pstNxt1 = pstNxt2;

	VPSS_DBG_INFO("Ref Cur Nx1 Nx2 %x %x %x %x\n", pstRef->u32PhyAddr_Y,
								pstCur->u32PhyAddr_Y, pstNxt1->u32PhyAddr_Y, pstNxt2->u32PhyAddr_Y);

    if (0)
    {
    	HI_U8 chFile[64] = "vpss_detile2.yuv";
    	HI_DRV_VIDEO_FRAME_S stFrame;

        memset(&stFrame, 0x0, sizeof(stFrame));

    	stFrame.u32Width = 1920;
    	stFrame.u32Height = 1080;
    	stFrame.ePixFormat = HI_DRV_PIX_FMT_NV21;
    	stFrame.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    	stFrame.bSecure = HI_FALSE;
    	stFrame.stBufAddr[0] = *pstNxt2;
    	stFrame.stBufAddr[0].u32Stride_Y = pstNxt2->u32Stride_Y;
    	stFrame.stBufAddr[0].u32Stride_C = pstNxt2->u32Stride_C;

        VPSS_DBG_INFO("STEP1: tmp img save: Yaddr w h YS CS %x %d %d %d %d",
          stFrame.stBufAddr[0].u32PhyAddr_Y,
          stFrame.u32Width, stFrame.u32Height, stFrame.stBufAddr[0].u32Stride_Y,
          stFrame.stBufAddr[0].u32Stride_C);
    	VPSS_OSAL_WRITEYUV(&stFrame, chFile);
    }

    stTunnelIn.bEnable = HI_FALSE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    /* db */
    /* db_det */
    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable = HI_FALSE;
    stNrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    bRfrEnableFlag      = HI_FALSE;
    //snr_en 是db,dm, nr的总开关， SNR包括db,dm,nr
    stSnrInfo.bEnable   = HI_FALSE; //DEI与SNR共用BUFFER, 开DEI则关闭SNR
    stTnrInfo.bEnable   = HI_TRUE;  //只需开MA-TNR, 关闭MC-TNR
    stMcTnrInfo.bEnable = HI_FALSE;

#if 0 //98mv200 do rwzb in software, no need to disable tnr,snr
    if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
    {
        stSnrInfo.bEnable   = HI_FALSE;
        stTnrInfo.bEnable   = HI_FALSE;
    }
#endif

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);


    /* next2 config */
    stFrameCfg.bSecure          = pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth       = pstHalInfo->stInInfo.enBitWidth;
    stFrameCfg.enPixelFmt       = HI_DRV_PIX_FMT_NV21;
    stFrameCfg.u32Width         = pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height        = pstHalInfo->stInInfo.u32Height * 2;
    stFrameCfg.bProgressive     = HI_FALSE;
    stFrameCfg.stOffset.s32X    = 0;
    stFrameCfg.stOffset.s32Y    = 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstNxt2->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstNxt2->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y   = pstNxt2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C   = pstNxt2->u32Stride_C;
    stFrameCfg.bCompress = HI_FALSE;

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* nx1 config */
    stFrameCfg.u32Width     = pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height    = pstHalInfo->stInInfo.u32Height * 2;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstNxt1->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstNxt1->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y   = pstNxt1->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C   = pstNxt1->u32Stride_C;
    stFrameCfg.enPixelFmt = HI_DRV_PIX_FMT_NV21;
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX1, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* cur config */
    stFrameCfg.u32Width     = pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height    =  pstHalInfo->stInInfo.u32Height * 2;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstCur->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstCur->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstCur->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstCur->u32Stride_C;

    stFrameCfg.enPixelFmt = HI_DRV_PIX_FMT_NV21;
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* ref config */
    stFrameCfg.u32Width     = pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height    =  pstHalInfo->stInInfo.u32Height * 2;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRef->u32Stride_C;

    stFrameCfg.enPixelFmt = HI_DRV_PIX_FMT_NV21;
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REF, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* rfr config */
    pstRfr         = &pstHalInfo->stInWbcInfo.stAddr;
	bRfrEnableFlag = HI_TRUE;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRfr->u32PhyAddr_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRfr->u32PhyAddr_C;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRfr->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRfr->u32Stride_C;
    stFrameCfg.enBitWidth = pstHalInfo->stInWbcInfo.enBitWidth;
    stFrameCfg.enPixelFmt = pstHalInfo->stInWbcInfo.enFormat;
    stFrameCfg.u32Width   = pstHalInfo->stInWbcInfo.u32Width;
    stFrameCfg.u32Height  = pstHalInfo->stInWbcInfo.u32Height;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);


    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, stRegAddr);

    stMcDeiAlgCfg.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_MCDEI, (HI_VOID*)&stMcDeiAlgCfg, HI_NULL);

    /* igbm */
    stAlgInfo.bEnable = HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IGBM, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* ifmd */
    stAlgInfo.bEnable = HI_TRUE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stAlgInfo, HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);

    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    pstHalInfo->pstH265RefList->u32RefListHead = (pstHalInfo->pstH265RefList->u32RefListHead+1)%
                                                    DEF_VPSS_HAL_REF_LIST_NUM;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set_2DDei_Last2Field_Node(VPSS_IP_E enIP, VPSS_HAL_INFO_S* pstHalInfo, HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    //VPZME, 单场模式开

    HI_DRV_VID_FRAME_ADDR_S* pstNext2;
    HI_DRV_VID_FRAME_ADDR_S* pstCur;
    HI_DRV_VID_FRAME_ADDR_S* pstRfr;
    HI_DRV_VID_FRAME_ADDR_S* pstRef;
    VPSS_HAL_FRAME_S         *pstInInfo;

    HI_BOOL bRfrEnableFlag;
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;

    CBB_REG_ADDR_S		stRegAddr;
    CBB_REG_ADDR_S		stRegAddr_MMU;
    CBB_FRAME_S 		stFrameCfg;
    CBB_FUNC_TUNNELIN_S stTunnelIn;
    CBB_FUNC_SMMU_S 	stSMMUCfg;
    CBB_INFO_S			stMadInfo_Tnr;
    CBB_INFO_S			stMadInfo_Snr;
    CBB_ALG_INFO_S		stDBInfo;
    CBB_ALG_INFO_S		stDBDETInfo;
    CBB_ALG_INFO_S		stNrInfo;
    CBB_ALG_INFO_S		stSnrInfo;
    CBB_ALG_INFO_S		stTnrInfo;
    CBB_ALG_INFO_S		stMcTnrInfo;
    CBB_ALG_INFO_S		stIfmdInfo;
    CBB_INFO_S			sttInfoCfg;

    VPSS_MEM_CLEAR(stRegAddr);
    VPSS_MEM_CLEAR(stRegAddr_MMU);
    VPSS_MEM_CLEAR(stFrameCfg);
    VPSS_MEM_CLEAR(stTunnelIn);
    VPSS_MEM_CLEAR(stSMMUCfg);
    VPSS_MEM_CLEAR(stMadInfo_Tnr);
    VPSS_MEM_CLEAR(stMadInfo_Snr);
    VPSS_MEM_CLEAR(stDBInfo);
    VPSS_MEM_CLEAR(stDBDETInfo);
    VPSS_MEM_CLEAR(stNrInfo);
    VPSS_MEM_CLEAR(stSnrInfo);
    VPSS_MEM_CLEAR(stTnrInfo);
    VPSS_MEM_CLEAR(stMcTnrInfo);
    VPSS_MEM_CLEAR(stIfmdInfo);
    VPSS_MEM_CLEAR(sttInfoCfg);

    stRegAddr.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr.u32NodePhyAddr = u32AppPhy;
    pstInInfo = &pstHalInfo->stInInfo;

    VPSS_REG_ResetAppReg((S_VPSS_REGS_TYPE*)pu32AppVir, pstHalInfo->pstPqCfg);

    stTunnelIn.bEnable = HI_FALSE;
    VPSS_CBB_FUNC_SetConfig(stRegAddr, CBB_FUNC_TUNNEL_IN, (HI_VOID*)&stTunnelIn, HI_NULL);

    stDBDETInfo.bEnable = HI_FALSE;
    stDBInfo.bEnable 	= HI_FALSE;
    stSnrInfo.bEnable	= HI_FALSE;
    stNrInfo.bEnable 	= HI_FALSE;
    stTnrInfo.bEnable	= HI_TRUE;
    stMcTnrInfo.bEnable = HI_FALSE;

    stDBInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB, (HI_VOID*)&stDBInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_DB_DET, (HI_VOID*)&stDBDETInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_NR, (HI_VOID*)&stNrInfo, HI_NULL);

    /* snr, tnr */
    //In 98mv200 all of SNR,DB,DM use DB'coef addr(VPSS_DB_CTRL).
    stSnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_DB_CTRL) - sizeof(HI_U32);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_SNR, (HI_VOID*)&stSnrInfo, HI_NULL);

    //TNR分为 MA-TNR,  MC-TNR, MC需要单独设置开关, MA的开关即全局tnr开关
    stTnrInfo.u32ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL, VPSS_TNR_CONTRL) - sizeof(HI_U32);

    if VPSS_PIXEL_FMT_420(pstHalInfo->stInInfo.enFormat)
    {
        stTnrInfo.u32Ymotionmode = 1;
    }
    else
    {
        stTnrInfo.u32Ymotionmode = 0;
    }

    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR, (HI_VOID*)&stTnrInfo, HI_NULL);
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_TNR_MC, (HI_VOID*)&stMcTnrInfo, HI_NULL);

    VPSS_HAL_Set_TNROutPutCur(stRegAddr);

    /* ifmd */
    stIfmdInfo.bEnable = HI_FALSE;
    VPSS_AlgCfg_BaseFunc(stRegAddr, CBB_ALG_IFMD, (HI_VOID*)&stIfmdInfo, HI_NULL);

    /* next2 config */
    pstNext2 = &pstHalInfo->stInInfo.stAddr;

    stFrameCfg.bSecure			= pstHalInfo->stInInfo.bSecure;
    stFrameCfg.enBitWidth		= HI_DRV_PIXEL_BITWIDTH_8BIT;
    stFrameCfg.enPixelFmt		= pstHalInfo->stInInfo.enFormat;
    stFrameCfg.u32Width 		= pstHalInfo->stInInfo.u32Width;
    stFrameCfg.u32Height		= pstHalInfo->stInInfo.u32Height ;
    stFrameCfg.bProgressive 	= HI_TRUE;
    stFrameCfg.stOffset.s32X	= 0;
    stFrameCfg.stOffset.s32Y	= 0;


    stFrameCfg.bTopFirst = pstHalInfo->stInInfo.bTopFirst;

    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.bTop = HI_TRUE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstNext2->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstNext2->u32PhyAddr_C;
    }
    else
    {
        stFrameCfg.bTop = HI_FALSE;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstNext2->u32PhyAddr_Y - pstNext2->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstNext2->u32PhyAddr_C - pstNext2->u32Stride_C;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y	= pstNext2->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C	= pstNext2->u32Stride_C;
    stFrameCfg.bCompress = HI_FALSE;

    VPSS_DBG_INFO("nx2 read from %08x %08x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_NX2, (HI_VOID*)&stFrameCfg, HI_NULL);

    /* nx1 config */
    /* no need to config nx1 */

    /* cur config */
    pstCur = pstNext2;
    stFrameCfg.u32Width 	= pstInInfo->u32Width;
    stFrameCfg.u32Height	= pstInInfo->u32Height;
    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstCur->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstCur->u32PhyAddr_C;
    }
    else
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y  = pstCur->u32PhyAddr_Y - pstCur->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C  = pstCur->u32PhyAddr_C - pstCur->u32Stride_C;
    }
    stFrameCfg.enPixelFmt = pstInInfo->enFormat;
    // in VPSS_CTRL_FixTask,  pstInInfo->enBitWidth has been revised to 8bit
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_CUR, (HI_VOID*)&stFrameCfg, HI_NULL);

    VPSS_DBG_INFO("cur read from %08x %08x %d %d\n",
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y,
                  stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C);

    /* ref */
    pstRef = pstCur;
    stFrameCfg.u32Width 	= pstInInfo->u32Width;
    stFrameCfg.u32Height	= pstInInfo->u32Height;
    if (HI_DRV_FIELD_TOP == pstHalInfo->stInInfo.enFieldMode)
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C;
    }
    else
    {
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_Y = pstRef->u32PhyAddr_Y - pstRef->u32Stride_Y;
        stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32PhyAddr_C = pstRef->u32PhyAddr_C - pstRef->u32Stride_C;
    }

    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_Y  = pstRef->u32Stride_Y;
    stFrameCfg.stAddr[CBB_FREAM_BUF_8BIT].u32Stride_C  = pstRef->u32Stride_C;

    stFrameCfg.enPixelFmt = pstInInfo->enFormat;
    // in VPSS_CTRL_FixTask,  pstInInfo->enBitWidth has been revised to 8bit
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_10BIT;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_READ_REF, (HI_VOID*)&stFrameCfg, HI_NULL);


    /* rfr config */
    pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
    stFrameCfg.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
    bRfrEnableFlag		= HI_FALSE;
    VPSS_CBB_FRAME_SetConfig(stRegAddr, CBB_FRAME_WRITE_RFR, (HI_VOID*)&stFrameCfg, &bRfrEnableFlag);

    /* vhd0 config */
    VPSS_HAL_SetPortCfg(enIP, stRegAddr, pstHalInfo);

    //逐隔行都要配置
    sttInfoCfg.u32WPhyAddr = pstHalInfo->stSttAddr.u32stt_w_phy_addr;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_STT, (HI_VOID*)&sttInfoCfg, (HI_VOID*)HI_NULL);

    /* smmu */
#ifdef HI_VPSS_SMMU_SUPPORT
    stSMMUCfg.bGlobalBypass = HI_FALSE;
    stSMMUCfg.bIntEnable = HI_TRUE;
    stSMMUCfg.u32Ptw_pf = 0x3;
    stSMMUCfg.u32BypassFlag_RCH = 0x0;
    stSMMUCfg.u32BypassFlag_WCH = 0x0;

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr, &u32ErrReadAddr, &u32ErrWriteAddr);
    stSMMUCfg.u32CB_TTBR = stSMMUCfg.u32SCB_TTBR = u32PageAddr;
    stSMMUCfg.u32Err_s_rd_addr = stSMMUCfg.u32Err_ns_rd_addr = u32ErrReadAddr;
    stSMMUCfg.u32Err_s_wr_addr = stSMMUCfg.u32Err_ns_wr_addr = u32ErrWriteAddr;
#else
    stSMMUCfg.bGlobalBypass = HI_TRUE;
    stSMMUCfg.bIntEnable = HI_FALSE;
    stSMMUCfg.u32BypassFlag_RCH = 0xffffffff;
    stSMMUCfg.u32BypassFlag_WCH = 0xffffffff;
#endif

    stRegAddr_MMU.pu8NodeVirAddr = (HI_U8*)pu32AppVir;
    stRegAddr_MMU.pu8BaseVirAddr = stHalCtx[enIP].pu8BaseRegVir;
    VPSS_CBB_FUNC_SetConfig(stRegAddr_MMU, CBB_FUNC_SMMU, (HI_VOID*)&stSMMUCfg, HI_NULL);


    /* tnr mad */
    stMadInfo_Tnr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr;
    stMadInfo_Tnr.u32WPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr;
    stMadInfo_Tnr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
    VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_TNR_MAD, (HI_VOID*)&stMadInfo_Tnr, HI_NULL);

    if (stSnrInfo.bEnable)
    {
        /* snr mad */
        stMadInfo_Snr.u32RPhyAddr = pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr;
        stMadInfo_Snr.u32Stride   = pstHalInfo->stNrInfo.stNrMadCfg.u32madstride;
        VPSS_CBB_INFO_SetConfig(stRegAddr, CBB_INFO_SNR_MAD, (HI_VOID*)&stMadInfo_Snr, HI_NULL);
    }

    return HI_SUCCESS;

}

HI_S32 VPSS_HAL_Set_TNROutPutCur(CBB_REG_ADDR_S stRegAddr)
{
    //disable sharemotionen, in case of Input image's W is not 4pixel algin
    VPSS_DRV_Set_VPSS_TNR_CONTRL((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0,0,1,1,1);
    VPSS_DRV_Set_VPSS_TNR_FILR_MODE((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0, 1, 1);
    VPSS_DRV_Set_VPSS_TNR_SCENE_CHANGE((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT0((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT1((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT2((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT3((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT4((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT5((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT6((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_YBLENDING_LUT7((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT0((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT1((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT2((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT3((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT4((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT5((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT6((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);
    VPSS_DRV_Set_VPSS_TNR_CBLENDING_LUT7((S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr, 0,0,0,0);

    return HI_SUCCESS;
}

