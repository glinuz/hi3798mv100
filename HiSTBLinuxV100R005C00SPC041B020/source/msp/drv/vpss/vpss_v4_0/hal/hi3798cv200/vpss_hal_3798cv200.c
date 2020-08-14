#include "vpss_hal_3798cv200.h"
#include "linux/kthread.h"
#include "vpss_common.h"

#include <asm/barrier.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif



SMMU_BUFFER_S g_stDstMMU;


#if 1
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


VPSS_REG_PORT_E VPSS_HAL_AllocPortId(VPSS_HAL_PORT_INFO_S *pstHalPort,
    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER])
{
    if (HI_FALSE == abPortUsed[VPSS_REG_HD])
    {
         abPortUsed[VPSS_REG_HD] = HI_TRUE;
         return VPSS_REG_HD;
    }

    return VPSS_REG_BUTT;
}

ZME_FORMAT_E VPSS_HAL_GetZmeFmt(HI_DRV_PIX_FORMAT_E enFormat)
{
    ZME_FORMAT_E enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP420;

    switch(enFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
        case HI_DRV_PIX_FMT_NV21_CMP:
        case HI_DRV_PIX_FMT_NV12_CMP:
        case HI_DRV_PIX_FMT_NV12_TILE:
        case HI_DRV_PIX_FMT_NV21_TILE:
        case HI_DRV_PIX_FMT_NV12_TILE_CMP:
        case HI_DRV_PIX_FMT_NV21_TILE_CMP:
            enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP420;
            break;
        case HI_DRV_PIX_FMT_NV61_2X1:
        case HI_DRV_PIX_FMT_NV16_2X1:
        case HI_DRV_PIX_FMT_NV61_2X1_CMP:
        case HI_DRV_PIX_FMT_NV16_2X1_CMP:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YVYU:
        case HI_DRV_PIX_FMT_UYVY:
        //case HI_DRV_PIX_FMT_ARGB8888:   //sp420->sp422->csc->rgb
        //case HI_DRV_PIX_FMT_ABGR8888:
        //case HI_DRV_PIX_FMT_KBGR8888:
            enZmeFmt = HI_PQ_ALG_PIX_FORMAT_SP422;
            break;
        default:
            VPSS_FATAL("REG ERROR format %d\n",enFormat);
    }

    return enZmeFmt;
}



HI_S32 VPSS_HAL_SetDnrCfg(VPSS_IP_E enIP,HI_U32* pu32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo)
{
#if 0
    VPSS_HAL_FRAME_S *pstInInfo;
    HI_BOOL bdrEn;
    HI_BOOL bdbEn;
	HI_BOOL bdnrEn;
	HI_BOOL bEnDnrDither;

    pstInInfo = &(pstHalInfo->stInInfo);

    if(pstInInfo->u32Height <= 576 && pstInInfo->u32Width <= 720
		&&(pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_CMP
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE
			|| pstInInfo->enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP
			)
		&&pstInInfo->u32Width >= 128
		&&pstInInfo->u32Height >= 64
		&&((pstInInfo->u32Width % 16) == 0)
		&&((pstInInfo->u32Height % 16) == 0)
		)

    {
		bdnrEn = 1;
		bEnDnrDither = 1;
    }
    else
    {
		bdnrEn = 0;
		bEnDnrDither = 0;
    }

    if(pstInInfo->u32Height <= 1080 && pstInInfo->u32Width <= 1920)
    {
		bdbEn = 1;
		bdrEn = 1;
    }
    else
    {
		bdbEn = 0;
		bdrEn = 0;
    }

    VPSS_REG_SetDREn(pu32AppVir, bdrEn);
    VPSS_REG_SetDBEn(pu32AppVir,bdbEn);
	VPSS_REG_SetDNREn(pu32AppVir,bdnrEn);

	VPSS_REG_SetDnrDitherEn(pu32AppVir, bEnDnrDither);
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetHSCLCfg(VPSS_IP_E enIP,HI_U32* pu32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo)
{
#if 0
    HI_PQ_ZME_PARA_IN_S stZmeDrvPara;
    HI_PQ_ZME_PARA_IN_S *pstZmeDrvPara;
    VPSS_HAL_FRAME_S *pstInInfo;

    pstInInfo = &(pstHalInfo->stInInfo);

    pstZmeDrvPara  = &(stZmeDrvPara);

    memset(pstZmeDrvPara,0,sizeof(HI_PQ_ZME_PARA_IN_S));

    if (pstInInfo->bProgressive != HI_TRUE)
    {
        pstZmeDrvPara->bZmeFrmFmtIn = 1;
    }
    else
    {
        if (pstInInfo->enFieldMode == HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
        }
        else
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 0;
        }
    }
    pstZmeDrvPara->bZmeFrmFmtOut = 1;

    if (pstInInfo->enFieldMode == HI_DRV_FIELD_TOP)
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
    else if (pstInInfo->enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstZmeDrvPara->bZmeBFIn = 1;
    }
    else
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
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
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p
        )
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;
    }
    else
    {
        pstZmeDrvPara->u8ZmeYCFmtIn = 1;//PQ_ALG_ZME_PIX_FORMAT_SP422;
    }

    pstZmeDrvPara->u8ZmeYCFmtOut = 0;//PQ_ALG_ZME_PIX_FORMAT_SP420;

    pstZmeDrvPara->u32ZmeFrmHIn = pstInInfo->u32Height;
    pstZmeDrvPara->u32ZmeFrmWIn = pstInInfo->u32Width;

    pstZmeDrvPara->u32ZmeFrmHOut = pstInInfo->u32Height;

    if (pstInInfo->u32Width % 4 != 0)
    {
        VPSS_ERROR("Invalid image width %d\n",pstInInfo->u32Width);
    }

    //pstZmeDrvPara->u32ZmeFrmWOut = pstInInfo->u32Width/2;
    pstZmeDrvPara->u32ZmeFrmWOut = 1920;

    pstZmeDrvPara->stOriRect.s32X = 0;
    pstZmeDrvPara->stOriRect.s32Y = 0;
    pstZmeDrvPara->stOriRect.s32Width = pstInInfo->u32Width;
    pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
    pstZmeDrvPara->u32InRate = 25000;
    pstZmeDrvPara->u32OutRate = 25000;
    pstZmeDrvPara->bDispProgressive = HI_TRUE;
    pstZmeDrvPara->u32Fidelity = 0;

    DRV_PQ_SetVpssZme(HI_PQ_VPSS_HSCL_LAYER_ZME,
                      (VPSS_REG_S*)pu32AppVir,
                      pstZmeDrvPara,
                      HI_TRUE);
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetZmeCfg(VPSS_IP_E enIP,HI_U32* pu32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo,
                            VPSS_REG_PORT_E enPort,
                            HI_U32 u32PortCnt)
{
#if 1
    VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;
    HI_PQ_ZME_PARA_IN_S stZmeDrvPara;
    HI_PQ_ZME_PARA_IN_S *pstZmeDrvPara;
    VPSS_HAL_FRAME_S *pstInInfo;
    HI_PQ_VPSS_ZME_LAYER_E enPqPort = HI_PQ_VPSS_LAYER_ZME_BUTT;
	HI_RECT_S *pstInCropRect;



	if (VPSS_REG_BUTT <= enPort)
	{
		VPSS_ERROR("Invalid enPort %d\n",enPort);
		return HI_FAILURE;
	}

    pstHalPort = &pstHalInfo->astPortInfo[u32PortCnt];

    pstInInfo = &(pstHalInfo->stInInfo);
	pstInCropRect = &(pstHalPort->stOutCropRect);

    pstZmeDrvPara  = &(stZmeDrvPara);

    memset(pstZmeDrvPara,0,sizeof(HI_PQ_ZME_PARA_IN_S));

#if 0
    if (pstInInfo->bProgressive != HI_TRUE)
    {
        pstZmeDrvPara->bZmeFrmFmtIn = 1;
    }
    else
    {
        if (pstInInfo->enFieldMode == HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 1;
        }
        else
        {
            pstZmeDrvPara->bZmeFrmFmtIn = 0;
        }
    }
#else
	pstZmeDrvPara->bZmeFrmFmtIn = 1;
#endif
    pstZmeDrvPara->bZmeFrmFmtOut = 1;

#if 0
    if (pstInInfo->enFieldMode == HI_DRV_FIELD_TOP)
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
    else if (pstInInfo->enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstZmeDrvPara->bZmeBFIn = 1;
    }
    else
    {
        pstZmeDrvPara->bZmeBFIn = 0;
    }
#else
	pstZmeDrvPara->bZmeBFIn = 0;
#endif
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
        || pstInInfo->enFormat == HI_DRV_PIX_FMT_YUV410p
        )
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



    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
	{
		pstZmeDrvPara->u32ZmeFrmHIn = (pstInCropRect->s32Height/2)&0xfffffffc;
		pstZmeDrvPara->u32ZmeFrmWIn = pstInCropRect->s32Width;
	}
	else if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
    {
        pstZmeDrvPara->u32ZmeFrmHIn = pstInInfo->u32Height;
        pstZmeDrvPara->u32ZmeFrmWIn = pstInInfo->u32Width;
    }
	else
	{
		//ZME's input height/width is VHD0 Crop's output
		pstZmeDrvPara->u32ZmeFrmHIn = pstInCropRect->s32Height;
		pstZmeDrvPara->u32ZmeFrmWIn = pstInCropRect->s32Width;
	}

    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
    {
        VPSS_DBG_INFO("step2 zme in: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWIn, pstZmeDrvPara->u32ZmeFrmHIn);
    }

#if 0
    if (pstInInfo->bProgressive == HI_TRUE)
    {
        if (pstInInfo->enFieldMode != HI_DRV_FIELD_ALL)
        {
            pstZmeDrvPara->u32ZmeFrmHIn = pstZmeDrvPara->u32ZmeFrmHIn*2;
        }
    }
#endif

	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
  	{
	    pstZmeDrvPara->u32ZmeFrmWOut = pstZmeDrvPara->u32ZmeFrmWIn;
	    pstZmeDrvPara->u32ZmeFrmHOut = pstZmeDrvPara->u32ZmeFrmHIn;
		VPSS_DBG_INFO("step1 zme out: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWOut, pstZmeDrvPara->u32ZmeFrmHOut);
  	}
  	else
  	{
        pstZmeDrvPara->u32ZmeFrmWOut = pstHalPort->stVideoRect.s32Width;
        pstZmeDrvPara->u32ZmeFrmHOut = pstHalPort->stVideoRect.s32Height;
		if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
		{
        	VPSS_DBG_INFO("step2 zme out: W H %d %d\n", pstZmeDrvPara->u32ZmeFrmWOut, pstZmeDrvPara->u32ZmeFrmHOut);
        }
  	}

	/*
	for SD port, ZME out should not exceed 1920x1088
	*/
	if ( VPSS_REG_SD == enPort)
	{
		if ( pstZmeDrvPara->u32ZmeFrmHOut >= 1088 ||
			 pstZmeDrvPara->u32ZmeFrmWOut >=1920)
		{
			pstZmeDrvPara->u32ZmeFrmHOut = 1080;
			pstZmeDrvPara->u32ZmeFrmWOut = 1920;
		}
	}


#if 0
    VPSS_ERROR("[ZME]IN: w %d h %d  OUT: w %d h %d\n",
        pstZmeDrvPara->u32ZmeFrmWIn,
        pstZmeDrvPara->u32ZmeFrmHIn,
        pstZmeDrvPara->u32ZmeFrmWOut,
        pstZmeDrvPara->u32ZmeFrmHOut);
	#endif

    pstZmeDrvPara->stOriRect.s32X = 0;
    pstZmeDrvPara->stOriRect.s32Y = 0;
    pstZmeDrvPara->stOriRect.s32Width = pstInInfo->u32Width;
    pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
	pstZmeDrvPara->stOriRect.s32Height = pstInInfo->u32Height;
    pstZmeDrvPara->u32InRate = 25000;
    pstZmeDrvPara->u32OutRate = 25000;
    pstZmeDrvPara->bDispProgressive = HI_TRUE;
    pstZmeDrvPara->u32Fidelity = 0;

    switch(enPort)
    {
        case VPSS_REG_HD:
            enPqPort = HI_PQ_VPSS_PORT0_LAYER_ZME;
            break;
        case VPSS_REG_SD:
            enPqPort = HI_PQ_VPSS_PORT1_LAYER_ZME;
            break;
//        case VPSS_REG_STR:
//            enPqPort = HI_PQ_VPSS_PORT2_LAYER_ZME;
            break;
        default:
            break;
    }

	if ( enPqPort == HI_PQ_VPSS_LAYER_ZME_BUTT)
	{
		VPSS_ERROR("func=%s,enPqPort is %d\n",__func__,enPqPort);
		return HI_FAILURE;
	}

    DRV_PQ_SetVpssZme(enPqPort,
                      (VPSS_REG_S*)pu32AppVir,
                      pstZmeDrvPara,
                      HI_TRUE);
#endif
    return HI_SUCCESS;
}
HI_BOOL VPSS_HAL_CheckNeedCmp(VPSS_HAL_FRAME_S *pstOutFrm)
{
#if 1
    if((HI_DRV_PIX_FMT_NV12_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV21_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV16_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV61_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV16_2X1_CMP == pstOutFrm->enFormat)
        || (HI_DRV_PIX_FMT_NV61_2X1_CMP == pstOutFrm->enFormat)
        )
    {

        return HI_TRUE;
    }
#endif
    return HI_FALSE;
}
/******************************************************************************
brief      : CNcomment:  Config Port Compress info
attention  :
param[in]  :
retval     : HI_S32
see        :
author	   :f00241306
******************************************************************************/

HI_S32 VPSS_HAL_SetPortCmpCfg(HI_U32* pu32AppVir,VPSS_REG_PORT_E ePort, VPSS_HAL_PORT_INFO_S *pstHalPort)
{
#if 0
    HI_BOOL bIs_lossless;
    HI_BOOL bCmp_Mode;
    HI_U32 u32Dw_Mode;
    HI_U32 u32CmpRatioY;
    HI_U32 u32CmpRatioC;
    HI_U32 u32YMaxQp;
    HI_U32 u32CMaxQp;
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;
    ZME_FORMAT_E enOutFormat;
    REG_CMP_DATA_S stYcmpData;
    REG_CMP_DATA_S stCcmpData;
    VPSS_HAL_FRAME_S *pstOutFrm;

    if(VPSS_REG_HD != ePort)
    {
        VPSS_FATAL("Vpss cmp error,only VHD support!!\n");
        return HI_FAILURE;
    }

    pstOutFrm =  &(pstHalPort->stOutInfo);
    enBitWidth = pstHalPort->stOutInfo.enBitWidth;

	if( ! ((HI_DRV_PIXEL_BITWIDTH_8BIT == enBitWidth) || (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth)) )
    {
        VPSS_FATAL("Vpss cmp error,only 8BIT and 10BIT support!!\n");
        return HI_FAILURE;
    }

    u32CmpRatioY = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 140 : 130;
    u32CmpRatioC = (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 140 : 130;
    u32YMaxQp  = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 4 : 2;
    u32CMaxQp  = ( HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 4 : 2;

    if(HI_DRV_PIXEL_BITWIDTH_8BIT == enBitWidth)
    {
        u32YMaxQp = 2;
		u32CMaxQp = 2;
    }
    else
    {
		u32YMaxQp = 4;
		u32CMaxQp = 4;
    }

    bIs_lossless = !(pstHalPort->bCmpLoss);//vdp will control

    bCmp_Mode = HI_FALSE;//cmp mode
    u32Dw_Mode = (HI_DRV_PIXEL_BITWIDTH_10BIT == enBitWidth) ? 0 : 1;

    if(pstOutFrm->u32Width < 128)
    {
        bIs_lossless = HI_TRUE;
    }

	if ( pstOutFrm->u32Width < 32 || pstOutFrm->u32Width > 8192 ||
		 pstOutFrm->u32Height < 32 || pstOutFrm->u32Height > 8192)
	{
		VPSS_FATAL("Vpss cmp error, input IMG Width or Height not support!!\n");
        return HI_FAILURE;
	}

    enOutFormat = VPSS_HAL_GetZmeFmt(pstOutFrm->enFormat);
//calc y cmp config
    stYcmpData.u32Width = pstOutFrm->u32Width ;
    stYcmpData.u32Height = pstOutFrm->u32Height ;
    stYcmpData.u32Cfg0_Mb_bits = 32 * 100 * ((enBitWidth == 0) ? 10 : 8) / u32CmpRatioY;
    stYcmpData.u32Cfg0_Ri_bits = (enBitWidth == 1) ? 3 : 3;
    stYcmpData.u32Cfg0_Max_rg_comp_bits = (enBitWidth == 1) ? 12 : 10;
    stYcmpData.u32Cfg1_Max_qp = u32YMaxQp;
    stYcmpData.u32Cfg1_Rc_smth_ngain = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg1_Sad_bits_ngain = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg2_Pix_diff_thr = (enBitWidth == 1) ? 200 : 50;
    stYcmpData.u32Cfg2_Smth_pix_num_thr = (enBitWidth == 1) ? 4 : 4;
    stYcmpData.u32Cfg2_Smth_thr = (enBitWidth == 1) ? 12 : 3;
    stYcmpData.u32Cfg3_Adj_sad_bit_thr = (enBitWidth == 1) ? 25 : 20;
    stYcmpData.u32Cfg3_Adj_sad_thr = (enBitWidth == 1) ? 1280 : 320;
    stYcmpData.u32Cfg4_Qp_chg1_bits_thr = (enBitWidth == 1) ? 35 : 28;
    stYcmpData.u32Cfg4_Qp_chg2_bits_thr = (enBitWidth == 1) ? 75 : 60;
    stYcmpData.u32Cfg5_Smth_lftbits_thr0 = (enBitWidth == 1) ? 12 : 10;
    stYcmpData.u32Cfg5_Smth_lftbits_thr1 = (enBitWidth == 1) ? 25 : 20;
    stYcmpData.u32Cfg5_Smth_qp0 = (enBitWidth == 1) ? 1 : 1;
    stYcmpData.u32Cfg5_Smth_qp1 = (enBitWidth == 1) ? 2 : 2;
//calc c cmp config
    stCcmpData.u32Width = pstOutFrm->u32Width / 2 ;
    stCcmpData.u32Height = (HI_PQ_ALG_PIX_FORMAT_SP422 == enOutFormat) ? pstOutFrm->u32Height : pstOutFrm->u32Height / 2;
    stCcmpData.u32Cfg0_Mb_bits = 64 * 100 * ((enBitWidth == 0) ? 10 : 8) / u32CmpRatioC;
    stCcmpData.u32Cfg0_Ri_bits = (enBitWidth == 1) ? 7 : 6;
    stCcmpData.u32Cfg0_Max_rg_comp_bits = (enBitWidth == 1) ? 25 : 20;
    stCcmpData.u32Cfg1_Max_qp = u32CMaxQp;
    stCcmpData.u32Cfg1_Rc_smth_ngain = (enBitWidth == 1) ? 0 : 0;
    stCcmpData.u32Cfg1_Sad_bits_ngain = (enBitWidth == 1) ? 1 : 1;
    stCcmpData.u32Cfg2_Pix_diff_thr = (enBitWidth == 1) ? 200 : 50;
    stCcmpData.u32Cfg2_Smth_pix_num_thr = (enBitWidth == 1) ? 4 : 4;
    stCcmpData.u32Cfg2_Smth_thr = (enBitWidth == 1) ? 12 : 3;
    stCcmpData.u32Cfg3_Adj_sad_bit_thr = (enBitWidth == 1) ? 50 : 40;
    stCcmpData.u32Cfg3_Adj_sad_thr = (enBitWidth == 1) ? 1920 : 480;
    stCcmpData.u32Cfg4_Qp_chg1_bits_thr = (enBitWidth == 1) ? 70 : 56;
    stCcmpData.u32Cfg4_Qp_chg2_bits_thr = (enBitWidth == 1) ? 150 : 120;
    stCcmpData.u32Cfg5_Smth_lftbits_thr0 = (enBitWidth == 1) ? 12 : 10;
    stCcmpData.u32Cfg5_Smth_lftbits_thr1 = (enBitWidth == 1) ? 25 : 20;
    stCcmpData.u32Cfg5_Smth_qp0 = (enBitWidth == 1) ? 1 : 1;
    stCcmpData.u32Cfg5_Smth_qp1 = (enBitWidth == 1) ? 2 : 2;

    VPSS_REG_SetPortLCmpEn(pu32AppVir, ePort, HI_TRUE);

    //:Y
	VPSS_REG_SetLossLess_Y(pu32AppVir,bIs_lossless);
	VPSS_REG_SetVhd0CmpMode_Y(pu32AppVir,bCmp_Mode);
	VPSS_REG_SetVhd0DwMode_Y(pu32AppVir,u32Dw_Mode);

	VPSS_REG_SetVhd0LcmpSize_Y(pu32AppVir, stYcmpData.u32Width, stYcmpData.u32Height);

	VPSS_REG_SetVhd0LcmpMbBit_Y(pu32AppVir,stYcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_SetVhd0LcmpRiBit_Y(pu32AppVir, stYcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_SetVhd0LcmpMaxRg_Y(pu32AppVir, stYcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_SetVhd0LcmpMaxQp_Y(pu32AppVir, stYcmpData.u32Cfg1_Max_qp);
	VPSS_REG_SetVhd0LcmpSadBit_Y(pu32AppVir, stYcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_SetVhd0LcmpRcSmth_Y(pu32AppVir, stYcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_SetVhd0LcmpSmthThr_Y(pu32AppVir, stYcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_SetVhd0LcmpSmthPixNum_Y(pu32AppVir, stYcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_SetVhd0LcmpPixDiff_Y(pu32AppVir, stYcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_SetVhd0LcmpAdjSad_Y(pu32AppVir, stYcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_SetVhd0LcmpAdjBit_Y(pu32AppVir, stYcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_SetVhd0LcmpQpChg1_Y(pu32AppVir, stYcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_SetVhd0LcmpQpChg2_Y(pu32AppVir, stYcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_SetVhd0LcmpSmthQp0_Y(pu32AppVir, stYcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_SetVhd0LcmpSmthQp1_Y(pu32AppVir, stYcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_SetVhd0LcmpSmthThr0_Y(pu32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_SetVhd0LcmpSmthThr1_Y(pu32AppVir, stYcmpData.u32Cfg5_Smth_lftbits_thr1);

	//:C
	VPSS_REG_SetLossLess_C(pu32AppVir,bIs_lossless);
	VPSS_REG_SetVhd0CmpMode_C(pu32AppVir,bCmp_Mode);
	VPSS_REG_SetVhd0DwMode_C(pu32AppVir,u32Dw_Mode);

	VPSS_REG_SetVhd0LcmpSize_C(pu32AppVir, stCcmpData.u32Width, stCcmpData.u32Height);

	VPSS_REG_SetVhd0LcmpMbBit_C(pu32AppVir,stCcmpData.u32Cfg0_Mb_bits);
	VPSS_REG_SetVhd0LcmpRiBit_C(pu32AppVir, stCcmpData.u32Cfg0_Ri_bits);
	VPSS_REG_SetVhd0LcmpMaxRg_C(pu32AppVir, stCcmpData.u32Cfg0_Max_rg_comp_bits);

	VPSS_REG_SetVhd0LcmpMaxQp_C(pu32AppVir, stCcmpData.u32Cfg1_Max_qp);
	VPSS_REG_SetVhd0LcmpSadBit_C(pu32AppVir, stCcmpData.u32Cfg1_Sad_bits_ngain);
	VPSS_REG_SetVhd0LcmpRcSmth_C(pu32AppVir, stCcmpData.u32Cfg1_Rc_smth_ngain);

	VPSS_REG_SetVhd0LcmpSmthThr_C(pu32AppVir, stCcmpData.u32Cfg2_Smth_thr);
	VPSS_REG_SetVhd0LcmpSmthPixNum_C(pu32AppVir, stCcmpData.u32Cfg2_Smth_pix_num_thr);
	VPSS_REG_SetVhd0LcmpPixDiff_C(pu32AppVir, stCcmpData.u32Cfg2_Pix_diff_thr);

	VPSS_REG_SetVhd0LcmpAdjSad_C(pu32AppVir, stCcmpData.u32Cfg3_Adj_sad_thr);
	VPSS_REG_SetVhd0LcmpAdjBit_C(pu32AppVir, stCcmpData.u32Cfg3_Adj_sad_bit_thr);

	VPSS_REG_SetVhd0LcmpQpChg1_C(pu32AppVir, stCcmpData.u32Cfg4_Qp_chg1_bits_thr);
	VPSS_REG_SetVhd0LcmpQpChg2_C(pu32AppVir, stCcmpData.u32Cfg4_Qp_chg2_bits_thr);

	VPSS_REG_SetVhd0LcmpSmthQp0_C(pu32AppVir, stCcmpData.u32Cfg5_Smth_qp0);
	VPSS_REG_SetVhd0LcmpSmthQp1_C(pu32AppVir, stCcmpData.u32Cfg5_Smth_qp1);
	VPSS_REG_SetVhd0LcmpSmthThr0_C(pu32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr0);
	VPSS_REG_SetVhd0LcmpSmthThr1_C(pu32AppVir, stCcmpData.u32Cfg5_Smth_lftbits_thr1);
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetPortCropCfg(VPSS_IP_E enIP,
							HI_U32* pu32AppVir,
                            VPSS_HAL_INFO_S *pstHalInfo,
                            VPSS_REG_PORT_E enPort,
                            HI_U32 u32PortCnt)
{
	VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;
	VPSS_HAL_FRAME_S *pstInInfo;
	HI_RECT_S *pstInCropRect;
	HI_U32 u32CropX;
	HI_U32 u32CropY;
	HI_U32 u32CropWidth;
	HI_U32 u32CropHeight;

	pstHalPort = &pstHalInfo->astPortInfo[u32PortCnt];

	pstInInfo = &(pstHalInfo->stInInfo);

		pstInCropRect = &(pstHalPort->stOutCropRect);

		//4pixel align
		pstInCropRect->s32Width = pstInCropRect->s32Width & 0xfffffffc;

		if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
		{
			u32CropX = pstInCropRect->s32X;
			u32CropY = pstInCropRect->s32Y/2;
			u32CropWidth = pstInCropRect->s32Width;
			u32CropHeight = pstInCropRect->s32Height/2;
		}
		else
		{
			u32CropX = pstInCropRect->s32X;
			u32CropY = pstInCropRect->s32Y;
			u32CropWidth = pstInCropRect->s32Width;
			u32CropHeight = pstInCropRect->s32Height;
		}

		if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI)
		{
		    VPSS_DBG_INFO("step2 Crop  X %d Y %d W %d H %d\n",u32CropX,u32CropY,u32CropWidth,u32CropHeight);
		}

		VPSS_DBG_INFO("[CROP]X %d Y %d  W %d H %d\n",
							u32CropX,
							u32CropY,
							u32CropWidth,
							u32CropHeight);

		VPSS_REG_SetPortCropPos(pu32AppVir, enPort, u32CropY,u32CropX);
		VPSS_REG_SetPortCropSize(pu32AppVir, enPort, u32CropHeight,u32CropWidth);

		VPSS_REG_SetPortCropEn(pu32AppVir,HI_TRUE);

	return HI_SUCCESS;
}

/*
*  VPSS_HAL_SetPortCfg函数应该只需要传 VPSS_HAL_PORT_INFO_S数组信息即可，
*  但由于缩放需要源格式 所以传递了VPSS_HAL_INFO_S信息，也同时为后期预缩放场景提供扩展
*/
HI_S32 VPSS_HAL_SetSMMUCfg(VPSS_IP_E enIP,HI_U32 *pu32AppVir, HI_U32 u32AppPhy,
    VPSS_HAL_INFO_S *pstHalInfo)
{
#ifdef HI_VPSS_SMMU_SUPPORT
    HI_U32 u32PageAddr;
    HI_U32 u32ErrReadAddr;
    HI_U32 u32ErrWriteAddr;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;
    pstHalCtx = &stHalCtx[enIP];

    HI_DRV_SMMU_GetPageTableAddr(&u32PageAddr,&u32ErrReadAddr,&u32ErrWriteAddr);

    VPSS_REG_SetSmmuAddr(pstHalCtx->pu8BaseRegVir,u32PageAddr,u32ErrReadAddr,u32ErrWriteAddr);

	VPSS_REG_SetGlobalBypass(pstHalCtx->pu8BaseRegVir,HI_FALSE);

	VPSS_REG_SetSmmuIntEn(pstHalCtx->pu8BaseRegVir,HI_TRUE);
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetMetaTrans(VPSS_IP_E enIP,HI_U32 *pu32AppVir, HI_U32 u32AppPhy,
    VPSS_HAL_INFO_S *pstHalInfo)
{
    HI_U32 u32SrcAddr;
    HI_U32 u32DstAddr;
    HI_U32 u32TranSize;

	//if EL carries no metadata, EL NODE donot need to trans metadata
	if (pstHalInfo->stInInfo.u32MetaSize == 0)
	{
		return HI_SUCCESS;
	}

    u32SrcAddr = pstHalInfo->stInInfo.stAddr_META.u32PhyAddr_Y;
    u32DstAddr = pstHalInfo->astPortInfo[0].stOutInfo.stAddr_META.u32PhyAddr_Y;
    u32TranSize = (pstHalInfo->stInInfo.u32MetaSize/256+1);

    if (u32SrcAddr == 0 || u32DstAddr == 0 || u32TranSize == 0)
    {
        VPSS_ERROR("metadata config error.u32SrcAddr %d u32DstAddr %d u32TranSize %d\n",
            u32SrcAddr,
            u32DstAddr,
            u32TranSize);
		return HI_FAILURE;
    }

#if 1
    VPSS_REG_SetTransMeta(pu32AppVir,u32SrcAddr,u32DstAddr,u32TranSize);
#else
    {
        SMMU_BUFFER_S stSrcSmmuBuf;
        SMMU_BUFFER_S stDstSmmuBuf;
        u32TranSize = pstHalInfo->stInInfo.u32MetaSize;
        stSrcSmmuBuf.u32StartSmmuAddr = u32SrcAddr;
        stSrcSmmuBuf.u32Size = u32TranSize;
        HI_DRV_SMMU_Map(&stSrcSmmuBuf);

        stDstSmmuBuf.u32StartSmmuAddr = u32DstAddr;
        stDstSmmuBuf.u32Size = u32TranSize;
        HI_DRV_SMMU_Map(&stDstSmmuBuf);

        memcpy(stDstSmmuBuf.pu8StartVirAddr,
            stSrcSmmuBuf.pu8StartVirAddr,
            u32TranSize);

		HI_DRV_SMMU_Unmap(&stSrcSmmuBuf);
		HI_DRV_SMMU_Unmap(&stDstSmmuBuf);
    }
   #endif

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetPortCfg(VPSS_IP_E enIP,HI_U32 *pu32AppVir, HI_U32 u32AppPhy,
    VPSS_HAL_INFO_S *pstHalInfo)
{
#if 1
    HI_U32 u32Count;
	HI_U32 u32PortAvailable = DEF_VPSS_HAL_PORT_NUM;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER]
        = {HI_FALSE, HI_FALSE, HI_FALSE};


    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    /* 先默认关闭三个硬件输出通道 */
    VPSS_REG_EnPort(pu32AppVir, VPSS_REG_HD, HI_FALSE);
    //VPSS_REG_EnPort(pu32AppVir, VPSS_REG_STR, HI_FALSE);
    //VPSS_REG_EnPort(pu32AppVir, VPSS_REG_SD,  HI_FALSE);
    //VPSS_REG_SetFidelity(pu32AppVir, HI_FALSE); //tqy ,hifoneb02 no this register bit.

    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER && u32PortAvailable > 0; u32Count ++)
    {
        VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;

        pstHalPort = &pstHalInfo->astPortInfo[u32Count];
        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {
            VPSS_REG_PORT_E enPort = VPSS_REG_BUTT;
            VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
            HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;

            enPort = VPSS_HAL_AllocPortId(pstHalPort, abPortUsed);
            if (enPort == VPSS_REG_BUTT)
            {
                HI_ASSERT(0);
            }
			else
			{
				u32PortAvailable--;
			}

            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;

            /* Flip&Mirro */
            VPSS_REG_SetPortMirrorEn(pu32AppVir, enPort,pstHalPort->bNeedMirror);  //暂时未考虑旋转
            VPSS_REG_SetPortFlipEn(pu32AppVir, enPort,pstHalPort->bNeedFlip);    //暂时未考虑旋转

            /* UV反转 */
			VPSS_REG_SetPortUVInvert(pu32AppVir, enPort, pstOutFrm->bUVInver);

            /* PreZme */
            //VPSS_REG_SetFrmPreZmeEn(pu32AppVir, enPort, HI_FALSE, HI_FALSE);

			/* VHD0's Crop */
			VPSS_HAL_SetPortCropCfg(enIP, pu32AppVir, pstHalInfo, enPort, u32Count);

            /*ZME*/
            VPSS_HAL_SetZmeCfg(enIP,pu32AppVir, pstHalInfo,enPort,u32Count);
			VPSS_REG_SetAddr(pu32AppVir,REG_VPSS_ZME_ADDR,VPSS_ZME_ADDR_GET(u32AppPhy));
            VPSS_REG_SetZmeEnable(pu32AppVir, enPort,REG_ZME_MODE_ALL,HI_TRUE);
            VPSS_REG_SetZmeGlbEnable(pu32AppVir,HI_TRUE);

            /* LBX */
            VPSS_REG_SetLBABg(pu32AppVir, enPort, 0x02080200, 0x7f);
            VPSS_REG_SetLBAVidPos(pu32AppVir, enPort,
                        (HI_U32)pstHalPort->stVideoRect.s32X,
                        (HI_U32)pstHalPort->stVideoRect.s32Y,
                        pstHalPort->stVideoRect.s32Height,
                        pstHalPort->stVideoRect.s32Width);
            VPSS_REG_SetLBADispPos(pu32AppVir, enPort, 0, 0,
                pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetLBAEn(pu32AppVir, enPort, HI_TRUE);


			VPSS_DBG_INFO("lbox disp x y w h %d %d %d %d, vw%d, vh%d\n", pstHalPort->stVideoRect.s32X ,
							pstHalPort->stVideoRect.s32Y,
							pstOutFrm->u32Width,
							pstOutFrm->u32Height,
							pstHalPort->stVideoRect.s32Width,
							pstHalPort->stVideoRect.s32Height);
            /* 输出格式 */
            VPSS_REG_SetFrmSize(pu32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
            VPSS_REG_SetFrmAddr(pu32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
            VPSS_REG_SetFrmStride(pu32AppVir,enPort,pstOutAddr->u32Stride_Y,pstOutAddr->u32Stride_C);
            VPSS_REG_SetFrmFormat(pu32AppVir,enPort,pstOutFrm->enFormat);
            //VPSS_REG_SetFrmBitWidth(pu32AppVir,enPort,pstOutFrm->enBitWidth);
            //memcpy(g_stDstMMU.pu8StartVirAddr,g_stSrcMMU.pu8StartVirAddr,64);
			/* Set Output BitWidth   */
            VPSS_REG_SetVhd0PixW(pu32AppVir,pstOutFrm->enBitWidth);

            /*if bitwidth < 10, dither is neccesary.*/
            if (pstOutFrm->enBitWidth > HI_DRV_PIXEL_BITWIDTH_8BIT)
            {
                VPSS_REG_SetVhd0Dither(pu32AppVir,REG_DITHER_MODE_DITHER,HI_FALSE);
            }
            else
            {
                VPSS_REG_SetVhd0Dither(pu32AppVir,REG_DITHER_MODE_DITHER,HI_TRUE);
			}
#ifdef VPSS_SUPPORT_OUT_TUNNEL
            if (pstHalInfo->bOutLowDelay)
            {
                VPSS_REG_SetTunlEn(pu32AppVir,enPort,HI_TRUE);
                VPSS_REG_SetTunlFinishLine(pu32AppVir,enPort,
                    pstOutFrm->u32Height/10);
#ifdef HI_VPSS_SMMU_SUPPORT
                VPSS_REG_SetTunlAddr(pu32AppVir,enPort,
                    pstHalCtx->stLowDelayBuf_MMU.u32StartSmmuAddr);
#else
                VPSS_REG_SetTunlAddr(pu32AppVir,enPort,
                    pstHalCtx->stLowDelayBuf_MMZ.u32StartPhyAddr);
#endif
            }
            else
#endif
	        {
                VPSS_REG_SetTunlEn(pu32AppVir,enPort,HI_FALSE);
            }

			pstHalPort->bConfig = HI_TRUE;

            VPSS_REG_EnPort(pu32AppVir, enPort, HI_TRUE);


        }
    }
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetPortCfg_Detile(VPSS_IP_E enIP,HI_U32 *pu32AppVir, HI_U32 u32AppPhy,
    VPSS_HAL_INFO_S *pstHalInfo)
{

    HI_U32 u32Count;
	HI_U32 u32PortAvailable = DEF_VPSS_HAL_PORT_NUM;
    HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER]
        = {HI_FALSE, HI_FALSE, HI_FALSE};


    VPSS_REG_EnPort(pu32AppVir, VPSS_REG_HD, HI_FALSE);

    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER && u32PortAvailable > 0; u32Count ++)
    {
        VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;
		VPSS_HAL_FRAME_S *pstInRefInfo;//参考帧信息

        pstHalPort = &pstHalInfo->astPortInfo[u32Count];

        if (pstHalPort->bEnable && pstHalPort->bConfig == HI_FALSE)
        {

            VPSS_REG_PORT_E enPort = VPSS_REG_BUTT;
            VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
            VPSS_HAL_FRAME_S *pstInFrm = HI_NULL;
            HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;
			//fix to detile buffer
			pstInRefInfo = &(pstHalInfo->stDeTileFrame);
            enPort = VPSS_HAL_AllocPortId(pstHalPort, abPortUsed);
            if (enPort == VPSS_REG_BUTT)
            {
                HI_ASSERT(0);
            }
			else
			{
				u32PortAvailable--;
			}

            pstOutFrm = &pstHalPort->stOutInfo;
            pstOutAddr = &pstOutFrm->stAddr;
            pstInFrm = &pstHalInfo->stInInfo;
            /* Flip&Mirro */
            VPSS_REG_SetPortMirrorEn(pu32AppVir, enPort, HI_FALSE);  //暂时未考虑旋转
            VPSS_REG_SetPortFlipEn(pu32AppVir, enPort, HI_FALSE);    //暂时未考虑旋转

            /* UV反转 */
			VPSS_REG_SetPortUVInvert(pu32AppVir, enPort, HI_FALSE);

            /* PreZme */
			//不能关闭prezme
            //VPSS_REG_SetFrmPreZmeEn(pu32AppVir, enPort, HI_FALSE, HI_FALSE);

			/* VHD0's Crop */
			VPSS_REG_SetPortCropEn(pu32AppVir,HI_FALSE);

            /*ZME*/
			VPSS_REG_SetZmeEnable(pu32AppVir, enPort,REG_ZME_MODE_ALL,HI_FALSE);
            VPSS_REG_SetZmeGlbEnable(pu32AppVir,HI_FALSE);
			VPSS_REG_SetLBAEn(pu32AppVir, enPort, HI_FALSE);

            /* 输出格式 */
            VPSS_REG_SetFrmSize(pu32AppVir,enPort,pstInFrm->u32Height, pstInFrm->u32Width);
			//前3场de-tile结果写入ref队列内
			VPSS_REG_SetFrmAddr(pu32AppVir,enPort,pstInRefInfo->stAddr.u32PhyAddr_Y, pstInRefInfo->stAddr.u32PhyAddr_C);
            VPSS_REG_SetFrmStride(pu32AppVir,enPort,pstInRefInfo->stAddr.u32Stride_Y,pstInRefInfo->stAddr.u32Stride_C);
            VPSS_REG_SetFrmFormat(pu32AppVir,enPort,pstInRefInfo->enFormat);

			/* Set Output BitWidth   */
            VPSS_REG_SetVhd0PixW(pu32AppVir,pstOutFrm->enBitWidth);

            /*if bitwidth < 10, dither is neccesary.*/
            if (pstOutFrm->enBitWidth > HI_DRV_PIXEL_BITWIDTH_8BIT)
            {
                VPSS_REG_SetVhd0Dither(pu32AppVir,REG_DITHER_MODE_DITHER,HI_FALSE);
            }
            else
            {
                VPSS_REG_SetVhd0Dither(pu32AppVir,REG_DITHER_MODE_DITHER,HI_TRUE);
            }

			//下一个node还要配置这个port
            //pstHalPort->bConfig = HI_TRUE;
            VPSS_REG_EnPort(pu32AppVir, enPort, HI_TRUE);
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetAllAlgCfgAddr(HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
    #if 0
    VPSS_REG_SetVc1StrCfgAddr(pu32AppVir, u32AppPhy+0x2400);
    VPSS_REG_SetZmeCfgAddr(pu32AppVir, u32AppPhy+0x2000);
    VPSS_REG_SetHspCfgAddr(pu32AppVir, u32AppPhy+0x2100);
    VPSS_REG_SetSnrCfgAddr(pu32AppVir, u32AppPhy+0x3000);
    VPSS_REG_SetDbCfgAddr(pu32AppVir, u32AppPhy+0x2200);
    VPSS_REG_SetDrCfgAddr(pu32AppVir, u32AppPhy+0x2300);
    VPSS_REG_SetDeiCfgAddr(pu32AppVir, u32AppPhy+0x1000);
    VPSS_REG_SetTnrCfgAddr(pu32AppVir, u32AppPhy+0x3800);
    VPSS_REG_SetTnrClutCfgAddr(pu32AppVir, u32AppPhy+0x3b00);
    VPSS_REG_SetEsCfgAddr(pu32AppVir, u32AppPhy+0x2500);
    #endif
    return HI_SUCCESS;
}
HI_S32 VPSS_HAL_SetRwzbCfg(HI_U32 *pu32AppAddr,VPSS_RWZB_INFO_S *pstRwzbInfo)
{
    HI_U32 u32Count;

    if( pstRwzbInfo->u32EnRwzb == 0x1)
    {
        for(u32Count = 0; u32Count < 6; u32Count++)
        {
            VPSS_REG_SetDetBlk(pu32AppAddr,u32Count,&(pstRwzbInfo->u32Addr[u32Count][0]));
                #if 1
                VPSS_INFO("adddr X %d Y %d\n",pstRwzbInfo->u32Addr[u32Count][0],
                                pstRwzbInfo->u32Addr[u32Count][1]);
                                #endif
        }

    }
    VPSS_REG_SetDetEn(pu32AppAddr,pstRwzbInfo->u32EnRwzb);
    VPSS_REG_SetDetMode(pu32AppAddr,pstRwzbInfo->u32Mode);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_DEI_ADDR;

    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

	/* 帧信息配置 */
    pstRef  = &pstHalInfo->stInRefInfo[0].stAddr;
    pstCur  = &pstHalInfo->stInRefInfo[1].stAddr;
    pstNxt1 = &pstHalInfo->stInRefInfo[2].stAddr;
    pstNxt2 = &pstHalInfo->stInInfo.stAddr;

#if 0
    VPSS_ERROR("Ref %#x cur %#x nxt1 %#x nxt2 %#x\n",
                pstRef->u32PhyAddr_Y,
                pstCur->u32PhyAddr_Y,
                pstNxt1->u32PhyAddr_Y,
                pstNxt2->u32PhyAddr_Y);
#endif
    /*dei*/
    VPSS_REG_SetImgReadMod(pu32AppVir, HI_TRUE);
    VPSS_REG_EnDei(pu32AppVir, HI_TRUE);
    VPSS_REG_SetDeiTopFirst(pu32AppVir, pstHalInfo->stInInfo.bTopFirst);

    switch(pstHalInfo->stInInfo.enFieldMode)
    {
        case HI_DRV_FIELD_BOTTOM:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_TRUE);
            break;
        case HI_DRV_FIELD_TOP:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_FALSE);
            break;
        default:
            VPSS_FATAL("No spt field Type:%d!\n", pstHalInfo->stInInfo.enFieldMode);
            return HI_FAILURE;
    }

    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);

    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(pu32AppVir,VPSS_DEI_ADDR);

    VPSS_REG_SetMode(pu32AppVir, REG_DIE_MODE_ALL, 1);//0为5场模式、1为4场模式、2为3场模式

    /*********************************ref**************************************/
    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD, HI_FALSE);
    }
    VPSS_REG_SetImgAddr(pu32AppVir, LAST_FIELD,
                        pstRef->u32PhyAddr_Y,
						pstRef->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);

    /************************* Cur **********************************/
    if (pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_FALSE);
    }
	//VPSS_REG_SetRefSize(pu32AppVir,pstHalInfo->stInRefInfo[1].u32Width,pstHalInfo->stInRefInfo[1].u32Height);
    VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,
                pstCur->u32PhyAddr_Y,
                pstCur->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
    VPSS_REG_SetRefNxtPixBitw(pu32AppVir,pstHalInfo->stInRefInfo[1].enBitWidth);

    /*******************************next1**************************************/
    if (pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD, HI_FALSE);
    }
    VPSS_REG_SetImgAddr(pu32AppVir, NEXT1_FIELD,
                        pstNxt1->u32PhyAddr_Y,
						pstNxt1->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir, NEXT1_FIELD, pstNxt1->u32Stride_Y, pstNxt1->u32Stride_C);
#if 1
    /*****************************next2****************************************/
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgAddr(pu32AppVir, NEXT2_FIELD,
                                pstNxt2->u32PhyAddr_Y,
								pstNxt2->u32PhyAddr_C);

    //VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);
    VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetDeiCfg_First3Field(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_DEI_ADDR;

    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

	/* 帧信息配置 */
	if (0 == pstHalInfo->u32DetileFieldIdx)
	{
		pstRef = pstCur = pstNxt1 = pstNxt2 = &(pstHalInfo->stDeTileFrame.stAddr);
	}
	else if (1 == pstHalInfo->u32DetileFieldIdx)
	{
		pstNxt2 = &(pstHalInfo->stDeTileFrame.stAddr);
		pstRef = pstCur = pstNxt1 = &(pstHalInfo->stInRefInfo[2].stAddr);
	}
	else if (2 == pstHalInfo->u32DetileFieldIdx)
	{
		pstNxt2 = &(pstHalInfo->stDeTileFrame.stAddr);
		pstNxt1 = &(pstHalInfo->stInRefInfo[2].stAddr);
		pstRef = pstCur = &(pstHalInfo->stInRefInfo[1].stAddr);
	}
	else
	{
		VPSS_FATAL("Invalid DetileFieldIdx(%d)!", pstHalInfo->u32DetileFieldIdx);
		return HI_FAILURE;
	}

#if 0
	VPSS_ERROR("Ref %#x cur %#x nxt1 %#x nxt2 %#x\n",
					pstRef->u32PhyAddr_Y,
					pstCur->u32PhyAddr_Y,
					pstNxt1->u32PhyAddr_Y,
					pstNxt2->u32PhyAddr_Y);
#endif

    /*dei*/
    VPSS_REG_SetImgReadMod(pu32AppVir, HI_TRUE);
    VPSS_REG_EnDei(pu32AppVir, HI_TRUE);
    VPSS_REG_SetDeiTopFirst(pu32AppVir, pstHalInfo->stInInfo.bTopFirst);

    switch(pstHalInfo->stInInfo.enFieldMode)
    {
        case HI_DRV_FIELD_BOTTOM:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_TRUE);
            break;
        case HI_DRV_FIELD_TOP:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_FALSE);
            break;
        default:
            VPSS_FATAL("No spt field Type:%d!\n", pstHalInfo->stInInfo.enFieldMode);
            return HI_FAILURE;
    }

    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);

    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(pu32AppVir,VPSS_DEI_ADDR);

    VPSS_REG_SetMode(pu32AppVir, REG_DIE_MODE_ALL, 1);//0为5场模式、1为4场模式、2为3场模式

    /*********************************ref**************************************/
    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD, HI_FALSE);
    }
    VPSS_REG_SetImgAddr(pu32AppVir, LAST_FIELD,
                        pstRef->u32PhyAddr_Y,
						pstRef->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);

    /************************* Cur **********************************/
    if (pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[1].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_FALSE);
    }
	//VPSS_REG_SetRefSize(pu32AppVir,pstHalInfo->stInRefInfo[1].u32Width,pstHalInfo->stInRefInfo[1].u32Height);
    VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,
                pstCur->u32PhyAddr_Y,
                pstCur->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
    VPSS_REG_SetRefNxtPixBitw(pu32AppVir,pstHalInfo->stInRefInfo[1].enBitWidth);

    /*******************************next1**************************************/
    if (pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[2].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD, HI_FALSE);
    }
    VPSS_REG_SetImgAddr(pu32AppVir, NEXT1_FIELD,
                        pstNxt1->u32PhyAddr_Y,
						pstNxt1->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir, NEXT1_FIELD, pstNxt1->u32Stride_Y, pstNxt1->u32Stride_C);

    /*****************************next2****************************************/
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgAddr(pu32AppVir, NEXT2_FIELD,
                                pstNxt2->u32PhyAddr_Y,
								pstNxt2->u32PhyAddr_C);

    //VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);
    VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);


	VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD,HI_FALSE);
	VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD,HI_FALSE);
	VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_FALSE);
	VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD,HI_FALSE);
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetMcDeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)

{
    HI_DRV_VID_FRAME_ADDR_S *pstBlendRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstBlendWbc  = HI_NULL;
    HI_U32 u32BlkCur  = HI_NULL;
    HI_U32 u32BlkRef  = HI_NULL;
    HI_U32 u32BlkNxt1  = HI_NULL;
    HI_U32 u32BlkNxt2  = HI_NULL;
    HI_U32 u32BlkStride = 0;
    HI_U32 u32RgmeCur  = HI_NULL;
    HI_U32 u32RgmeNxt1  = HI_NULL;
    HI_U32 u32RgmeNxt2  = HI_NULL;
    HI_U32 u32RgmeStride = 0;
    HI_U32 u32PrjVCur  = HI_NULL;
    HI_U32 u32PrjVNxt2  = HI_NULL;
    HI_U32 u32PrjVStride  = HI_NULL;
    HI_U32 u32PrjHCur  = HI_NULL;
    HI_U32 u32PrjHNxt2  = HI_NULL;
    HI_U32 u32PrjHStride  = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

    VPSS_REG_SetMcdiEn(pu32AppVir, HI_TRUE);

    if (pstHalInfo->stInInfo.u32Width > 960)
    {
        VPSS_REG_SetMedsEn(pu32AppVir, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetMedsEn(pu32AppVir, HI_FALSE);
    }

	pstBlendRef = &(pstHalInfo->stMcdeiRfrInfo.stBlendRef.stAddr);
	pstBlendWbc = &(pstHalInfo->stMcdeiRfrInfo.stBlendWbc.stAddr);
	VPSS_REG_SetReeAddr(pu32AppVir,
				pstBlendRef->u32PhyAddr_Y,
				pstBlendRef->u32PhyAddr_C,
				pstBlendRef->u32Stride_Y,
				pstBlendRef->u32Stride_C);
    VPSS_REG_SetCueAddr(pu32AppVir,
				pstBlendWbc->u32PhyAddr_Y,
				pstBlendWbc->u32PhyAddr_C,
				pstBlendWbc->u32Stride_Y,
				pstBlendWbc->u32Stride_C);

	u32BlkRef = pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32RefReadAddr;
	u32BlkCur = pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32CurReadAddr;
	u32BlkNxt1 = pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Nx1ReadAddr;
	u32BlkNxt2 = pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Nx2WriteAddr;
	u32BlkStride = pstHalInfo->stMcdeiStInfo.stBlkmvCfg.u32Stride;
	VPSS_REG_SetBlkmvAddr(pu32AppVir,
				u32BlkCur,
				u32BlkRef,
				u32BlkStride,
				u32BlkStride);
	VPSS_REG_SetBlkmvNx1Addr(pu32AppVir,
				u32BlkNxt1,
				u32BlkStride);
	VPSS_REG_SetBlkmvNx2Addr(pu32AppVir,
				u32BlkNxt2,
				u32BlkStride);

	u32RgmeCur = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32CurReadAddr;
	u32RgmeNxt1 = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx1ReadAddr;
	u32RgmeNxt2 = pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Nx2WriteAddr;
    u32RgmeStride =  pstHalInfo->stMcdeiStInfo.stRgmeCfg.u32Stride;
	VPSS_REG_SetRgmvAddr(pu32AppVir,
				u32RgmeCur,
				u32RgmeNxt1,
				u32RgmeStride,
				u32RgmeStride);
	VPSS_REG_SetRgmvNx2Addr(pu32AppVir,
				u32RgmeNxt2,
				u32RgmeStride);

	u32PrjVCur = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32CurReadAddr;
	u32PrjVNxt2 = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Nx2WriteAddr;
	u32PrjVStride = pstHalInfo->stMcdeiStInfo.stPrjvCfg.u32Stride;
	u32PrjHCur = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32CurReadAddr;
	u32PrjHNxt2 = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Nx2WriteAddr;
	u32PrjHStride = pstHalInfo->stMcdeiStInfo.stPrjhCfg.u32Stride;
    VPSS_REG_SetPrjNx2Addr(pu32AppVir,
				u32PrjVNxt2,
				u32PrjHNxt2,
				u32PrjVStride,
				u32PrjHStride);

	VPSS_REG_SetPrjCurAddr(pu32AppVir,
				u32PrjVCur,
				u32PrjHCur,
				u32PrjVStride,
				u32PrjHStride);
	return HI_SUCCESS;
}
HI_S32 VPSS_HAL_SetFieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir, HI_U32 u32AppPhy)
{
#if 1
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_DRV_VID_FRAME_ADDR_S *pstRef = HI_NULL;
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;
	HI_BOOL bPreZme = HI_FALSE;

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_FALSE);
    }


	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把SNR打开 */

#if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetReadPixBitWidth(pu32AppVir,CUR_CHANEL,pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
        VPSS_REG_SetImgSize(pu32AppVir,
            pstHalInfo->stInInfo.u32Width,
    		pstHalInfo->stInInfo.u32Height,
            pstHalInfo->stInInfo.bProgressive);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
        VPSS_REG_SetImgSize(pu32AppVir,
            pstHalInfo->stInInfo.u32Width,
    		pstHalInfo->stInInfo.u32Height/2,
            pstHalInfo->stInInfo.bProgressive);
    }

    /* 输入帧信息 */
    if ((pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12)
        && pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
    	VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);
    }
    else
    {
    	VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);
    }
    else
    {
        VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y*2, pstCur->u32Stride_C*2);
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
        pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);

    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }

	/*@f00241306 for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);

/* 参考帧信息 */
	pstRef = &pstHalInfo->stInRefInfo[0].stAddr;
	//VPSS_REG_SetRefSize(pu32AppVir,pstHalInfo->stInRefInfo[0].u32Width,pstHalInfo->stInRefInfo[0].u32Height);
	VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,pstRef->u32PhyAddr_Y,pstRef->u32PhyAddr_C);
	VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);
    VPSS_REG_SetRefNxtPixBitw(pu32AppVir,pstHalInfo->stInRefInfo[0].enBitWidth);

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_FALSE);
    }

    /* 回写帧信息 */
	pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrYaddr(pu32AppVir, pstRfr->u32PhyAddr_Y);
	VPSS_REG_SetRfrCaddr(pu32AppVir, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(pu32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
    VPSS_REG_SetRfrPixBitw(pu32AppVir,pstHalInfo->stInWbcInfo.enBitWidth);

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
    	if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP)
    	{
    		VPSS_REG_SetPreZme(pu32AppVir,PREZME_DISABLE,PREZME_2X);
    		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
    		bPreZme = HI_TRUE;
    	}
    	else
    	{
    		VPSS_REG_SetPreZme(pu32AppVir,PREZME_DISABLE,PREZME_VFIELD);
    		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
    		bPreZme = HI_TRUE;
    	}
    }
    else
    {
        if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP)
    	{
    		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
    		bPreZme = HI_TRUE;
    	}
    	else
    	{
    		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height/2;
    		bPreZme = HI_TRUE;
    	}
    }


    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
	//t00273561
	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);


	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif
	#endif
	if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);


    VPSS_REG_SetMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr);
    VPSS_REG_SetMadWaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr);
    VPSS_REG_SetMadStride(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32madstride);
    VPSS_REG_SetSNRMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr);
    if (bPreZme == HI_TRUE)
	{
		pstHalInfo->stInInfo.u32Height = pstHalInfo->stInInfo.u32Height*2;
	}
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetHDRNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
#if 1
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_DRV_VID_FRAME_ADDR_S *pstRef = HI_NULL;
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_FALSE);
    }

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把SNR打开 */

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把SNR打开 */
#endif

#if 0
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetReadPixBitWidth(pu32AppVir,CUR_CHANEL,pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
		   || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
   {
	   VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
		   pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
		   pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
		   pstHalInfo->stInInfo.stAddr.u32Head_Size);
	   VPSS_REG_SetDcmpHeadStride(pu32AppVir,
	   pstHalInfo->stInInfo.stAddr.u32Head_Stride);
	   VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);

   }
   else
   {
	   VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
   }

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306 for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);

/* 参考帧信息 */
	pstRef = &pstHalInfo->stInRefInfo[0].stAddr;
	//VPSS_REG_SetRefSize(pu32AppVir,pstHalInfo->stInRefInfo[0].u32Width,pstHalInfo->stInRefInfo[0].u32Height);
	VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,pstRef->u32PhyAddr_Y,pstRef->u32PhyAddr_C);
	VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);
    VPSS_REG_SetRefNxtPixBitw(pu32AppVir,pstHalInfo->stInRefInfo[0].enBitWidth);

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_FALSE);
    }

/* 回写帧信息 */
	pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrYaddr(pu32AppVir, pstRfr->u32PhyAddr_Y);
	VPSS_REG_SetRfrCaddr(pu32AppVir, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(pu32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
    VPSS_REG_SetRfrPixBitw(pu32AppVir,pstHalInfo->stInWbcInfo.enBitWidth);

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
	//t00273561
	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xc00);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xe0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif
	#endif


    if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);

    VPSS_HAL_SetMetaTrans(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

    return HI_SUCCESS;
}
HI_S32 VPSS_HAL_SetFrameNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
#if 1
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_DRV_VID_FRAME_ADDR_S *pstRef = HI_NULL;
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_FALSE);
    }


	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);

	if ((HI_DRV_FT_SBS == pstHalInfo->stInInfo.eFrmType)
			|| (HI_DRV_FT_TAB == pstHalInfo->stInInfo.eFrmType)
				|| (HI_DRV_FT_FPK == pstHalInfo->stInInfo.eFrmType))
	{
		//3d frame disable wbc,tnr,snr
		VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);
		VPSS_REG_SetTNrEn(pu32AppVir, HI_FALSE);
		VPSS_REG_SetSNrEn(pu32AppVir, HI_FALSE);
	}
	else
	{
		VPSS_REG_SetRfrEn(pu32AppVir, HI_TRUE);/* 默认把回写打开 */
		VPSS_REG_SetTNrEn(pu32AppVir, HI_TRUE);/* 默认把TNR打开 */
		VPSS_REG_SetSNrEn(pu32AppVir, HI_TRUE);/* 默认把SNR打开 */
	}
#if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306 for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

/* 参考帧信息 */
	pstRef = &pstHalInfo->stInRefInfo[0].stAddr;
	//VPSS_REG_SetRefSize(pu32AppVir,pstHalInfo->stInRefInfo[0].u32Width,pstHalInfo->stInRefInfo[0].u32Height);
	VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,pstRef->u32PhyAddr_Y,pstRef->u32PhyAddr_C);
	VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);
    VPSS_REG_SetRefNxtPixBitw(pu32AppVir,pstHalInfo->stInRefInfo[0].enBitWidth);

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_FALSE);
    }

    /* 回写帧信息 */
	pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrYaddr(pu32AppVir, pstRfr->u32PhyAddr_Y);
	VPSS_REG_SetRfrCaddr(pu32AppVir, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(pu32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
    VPSS_REG_SetRfrPixBitw(pu32AppVir,pstHalInfo->stInWbcInfo.enBitWidth);

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
	//t00273561
	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);


	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif
	#endif

    if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);


    VPSS_REG_SetMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr);
    VPSS_REG_SetMadWaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr);
    VPSS_REG_SetMadStride(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32madstride);
    VPSS_REG_SetSNRMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr);

    return HI_SUCCESS;
}

#if defined(HI_NVR_SUPPORT)

HI_S32 VPSS_HAL_SetNVRHighSpeedNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither


    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_FALSE);
    }

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/* for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_FALSE);
    }

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

	VPSS_REG_Set4PixEn(pu32AppVir, HI_TRUE);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

	VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);

    return HI_SUCCESS;
}

#endif //#if defined(HI_NVR_SUPPORT)

HI_S32 VPSS_HAL_ConfigDeTileFrame(	VPSS_HAL_FRAME_S* pstDeTileFrame,
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
    u32WriteAddr = pstHalCtx->stLowDelayBuf_MMU.u32StartSmmuAddr;
#else
    u32WriteAddr = pstHalCtx->stLowDelayBuf_MMZ.u32StartPhyAddr;
#endif

    return u32WriteAddr;
}
#endif

HI_S32 VPSS_HAL_AllocDetileBuffer(VPSS_IP_E enIP,HI_BOOL bSecure)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

#ifdef HI_VPSS_SMMU_SUPPORT
    if (bSecure)
    {
        if (pstHalCtx->stDeTileTEEBuf.u32Size == 0)
        {
#ifdef HI_TEE_SUPPORT
            s32Ret = HI_DRV_SECSMMU_Alloc("VPSS_DETILE_BUF", 1920*1080*2,0, &pstHalCtx->stDeTileTEEBuf);
#else
            s32Ret = HI_DRV_SMMU_Alloc( "VPSS_DETILE_BUF",
				1920*1080*2, 0, &pstHalCtx->stDeTileTEEBuf);
#endif
            if (s32Ret != HI_SUCCESS)
            {
                VPSS_FATAL("Alloc Detile buffer Failed\n");
                return HI_FAILURE;
            }
        }
    }
    else
    {
        if (pstHalCtx->stDeTileMMUBuf.u32Size == 0)
        {
            s32Ret = HI_DRV_SMMU_Alloc( "VPSS_DETILE_BUF",
				1920*1080*2, 0, &pstHalCtx->stDeTileMMUBuf);
            if (s32Ret != HI_SUCCESS)
            {
                VPSS_FATAL("Alloc Detile buffer Failed\n");
                return HI_FAILURE;
            }
        }
    }
#else
    if (pstHalCtx->stDeTileMMZBuf.u32Size == 0)
    {
        s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_DETILE_BUF", "VPSS",1920*1080*2, 0, &(pstHalCtx->stDeTileMMZBuf));
        if (s32Ret != HI_SUCCESS)
        {
            VPSS_FATAL("Alloc Detile buffer Failed\n");
            return HI_FAILURE;
        }
    }
#endif

    return HI_SUCCESS;
}
HI_VOID VPSS_HAL_FreeDetileBuffer(VPSS_IP_E enIP)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

#ifdef HI_VPSS_SMMU_SUPPORT
    if (pstHalCtx->stDeTileMMUBuf.u32Size != 0)
    {
        (HI_VOID)HI_DRV_SMMU_Release(&(pstHalCtx->stDeTileMMUBuf));
        memset(&(pstHalCtx->stDeTileMMUBuf),0,sizeof(SMMU_BUFFER_S));
    }

    if (pstHalCtx->stDeTileTEEBuf.u32Size != 0)
    {
#ifdef HI_TEE_SUPPORT
        (HI_VOID)HI_DRV_SECSMMU_Release(&(pstHalCtx->stDeTileTEEBuf));
#else
        (HI_VOID)HI_DRV_SMMU_Release(&(pstHalCtx->stDeTileTEEBuf));
#endif
        memset(&(pstHalCtx->stDeTileTEEBuf),0,sizeof(SMMU_BUFFER_S));
    }
#else
    if (pstHalCtx->stDeTileMMZBuf.u32Size != 0)
    {
        HI_DRV_MMZ_UnmapAndRelease(&(pstHalCtx->stDeTileMMZBuf));
        memset(&(pstHalCtx->stDeTileMMZBuf),0,sizeof(MMZ_BUFFER_S));
    }
#endif

    return;
}
HI_S32 VPSS_HAL_SetDetileNode_STEP1(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)
{
	HI_DRV_VID_FRAME_ADDR_S *pstNxt2_LB = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	VPSS_HAL_FRAME_S *pInInfo;
	HI_S32 s32Ret;
	HI_U32 u32DetileBufferAddr;
	VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

	VPSS_HAL_CHECK_IP_VAILD(enIP);

	pstHalCtx = &stHalCtx[enIP];

	pInInfo = &(pstHalInfo->stInInfo);

    s32Ret = VPSS_HAL_AllocDetileBuffer(enIP,pInInfo->bSecure);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_Detile Buffer Failed\n");
        return HI_FAILURE;
    }

#ifdef HI_VPSS_SMMU_SUPPORT
    if (pInInfo->bSecure)
    {
	    u32DetileBufferAddr = pstHalCtx->stDeTileTEEBuf.u32StartSmmuAddr;
    }
    else
    {
	    u32DetileBufferAddr = pstHalCtx->stDeTileMMUBuf.u32StartSmmuAddr;
    }
#else
	u32DetileBufferAddr = pstHalCtx->stDeTileMMZBuf.u32StartPhyAddr;
#endif


	s32Ret = VPSS_HAL_ConfigDeTileFrame(&(pstHalInfo->stDeTileFrame),
			u32DetileBufferAddr,
			pInInfo->u32Width,
			pInInfo->u32Height,
			pInInfo->enFormat,
			pInInfo->enBitWidth);

	VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);/* 默认把回写关闭 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR关闭 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR关闭 */

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
	/*tunnel*/
	if (pstHalInfo->stInInfo.u32TunnelAddr)
	{
		VPSS_REG_SetCurTunlEn(pu32AppVir,  HI_TRUE);
		VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
	}
	else
	{
		VPSS_REG_SetCurTunlEn(pu32AppVir, HI_FALSE);
	}

	VPSS_REG_SetDetEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
    if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}
	VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
	{
		VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
				pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
				pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
				pstHalInfo->stInInfo.stAddr.u32Head_Size);
		VPSS_REG_SetDcmpHeadStride(pu32AppVir,
				pstHalInfo->stInInfo.stAddr.u32Head_Stride);
		VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
	}
	else
	{
		VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
	}

	/* 帧信息配置 */
	VPSS_REG_SetReadPixBitWidth(pu32AppVir, NEXT2_FIELD,pstHalInfo->stInInfo.enBitWidth);
	VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

	VPSS_REG_SetImgSize(pu32AppVir,
			pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
			pstHalInfo->stInInfo.bProgressive);

	VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
	VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		pstNxt2_LB = &pstHalInfo->stInInfo.stAddr_LB;
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstNxt2_LB->u32PhyAddr_Y, pstNxt2_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstNxt2_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

	/* 运动信息  DEI*/
	VPSS_REG_SetMcdiEn(pu32AppVir, HI_FALSE);

	pstNxt2 = &pstHalInfo->stInInfo.stAddr;

	//帧模式读入
	VPSS_REG_SetImgReadMod(pu32AppVir, HI_FALSE);
	VPSS_REG_EnDei(pu32AppVir, HI_FALSE); //关闭DEI

	if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
			|| pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
	{
		VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_TRUE);
	}
	else
	{
		VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_FALSE);
	}

	VPSS_REG_SetImgAddr(pu32AppVir, NEXT2_FIELD,
			pstNxt2->u32PhyAddr_Y,
			pstNxt2->u32PhyAddr_C);

	VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);

	/*db/dr*/
	//VPSS_REG_SetDNREn(pu32AppVir, HI_FALSE);

	VPSS_REG_SetIglbEn(pu32AppVir,HI_FALSE);

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);

	/* 输出Port信息 */
	VPSS_HAL_SetPortCfg_Detile(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xa0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);
	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetFirst3FieldNode_STEP2(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)
{
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;

    VPSS_DIESTCFG_S *pstDeiSt = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_TRUE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_TRUE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_TRUE);/* 默认把TNR打开 */

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,  HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir, HI_FALSE);
    }

	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);

    /* 帧信息配置 */
    VPSS_REG_SetReadPixBitWidth(pu32AppVir, NEXT2_FIELD,pstHalInfo->stInInfo.enBitWidth);

    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stDeTileFrame.enFormat);

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
	    pstCur_LB = &(pstHalInfo->stInInfo.stAddr_LB);
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

	pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrYaddr(pu32AppVir, pstRfr->u32PhyAddr_Y);
	VPSS_REG_SetRfrCaddr(pu32AppVir, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(pu32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
    VPSS_REG_SetRfrPixBitw(pu32AppVir,pstHalInfo->stInWbcInfo.enBitWidth);

    /* 运动信息  DEI*/
	pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;
	VPSS_REG_SetStRdAddr(pu32AppVir, pstDeiSt->u32PPreAddr);
	VPSS_REG_SetStWrAddr(pu32AppVir, pstDeiSt->u32PreAddr);
	VPSS_REG_SetStStride(pu32AppVir, pstDeiSt->u32Stride);

	/*DEI*/
	VPSS_HAL_SetDeiCfg_First3Field(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

	if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
	{
		VPSS_REG_SetOutSel(pu32AppVir,REG_DIE_MODE_ALL,HI_TRUE);
		VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
		VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	}

	VPSS_HAL_SetMcDeiCfg(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

#if 1
    VPSS_REG_SetIglbEn(pu32AppVir,HI_TRUE);
	VPSS_REG_SetIfmdEn(pu32AppVir,HI_TRUE);
#else
	/*db/dr*/
    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);
    VPSS_REG_SetIglbEn(pu32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);
#endif
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

    VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);

    VPSS_REG_SetMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr);
    VPSS_REG_SetMadWaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr);
    VPSS_REG_SetMadStride(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32madstride);
    VPSS_REG_SetSNRMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr);
    if (pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr == 0)
    {
        VPSS_ERROR("u32Snrmad_raddr is zero\n");
    }

	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set5FieldNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)
{
#if 1
//    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;

//	HI_U32 u32CurFieldYaddr;
//	HI_U32 u32CurFieldCaddr;
    VPSS_DIESTCFG_S *pstDeiSt = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstRfr  = HI_NULL;

    VPSS_HAL_FreeDetileBuffer(enIP);

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_TRUE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_TRUE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_TRUE);/* 默认把TNR打开 */

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,  HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir, HI_FALSE);
    }

#if 1
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

	if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
        pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }

    /* 帧信息配置 */
    VPSS_REG_SetReadPixBitWidth(pu32AppVir, NEXT2_FIELD,pstHalInfo->stInInfo.enBitWidth);

    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width,
        pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInInfo.u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInInfo.u32Height);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
	    pstCur_LB = &(pstHalInfo->stInInfo.stAddr_LB);
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);

	pstRfr = &pstHalInfo->stInWbcInfo.stAddr;
	VPSS_REG_SetRfrYaddr(pu32AppVir, pstRfr->u32PhyAddr_Y);
	VPSS_REG_SetRfrCaddr(pu32AppVir, pstRfr->u32PhyAddr_C);
	VPSS_REG_SetRfrStride(pu32AppVir, pstRfr->u32Stride_Y, pstRfr->u32Stride_C);
    VPSS_REG_SetRfrPixBitw(pu32AppVir,pstHalInfo->stInWbcInfo.enBitWidth);

//	VPSS_ERROR("wbc addr %#x\n",pstRfr->u32PhyAddr_Y);
/* 运动信息  DEI*/
	pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;
	VPSS_REG_SetStRdAddr(pu32AppVir, pstDeiSt->u32PPreAddr);
	VPSS_REG_SetStWrAddr(pu32AppVir, pstDeiSt->u32PreAddr);
	VPSS_REG_SetStStride(pu32AppVir, pstDeiSt->u32Stride);

	/*DEI*/
	VPSS_HAL_SetDeiCfg(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

	if (pstHalInfo->stRwzbInfo.u32IsRwzb > 0)
	{
        VPSS_REG_SetOutSel(pu32AppVir,REG_DIE_MODE_ALL,HI_TRUE);
		VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
		VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	}
	VPSS_HAL_SetMcDeiCfg(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

    VPSS_REG_SetIglbEn(pu32AppVir,HI_TRUE);

	VPSS_REG_SetIfmdEn(pu32AppVir,HI_TRUE);

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

	//t00273561
//	VPSS_ERROR("stt addr %#x\n",pstHalInfo->u32stt_w_phy_addr);
    VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif
   // VPSS_REG_Set4PixEn(pu32AppVir,HI_FALSE);

#endif

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);


    VPSS_REG_SetMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_raddr);
    VPSS_REG_SetMadWaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Tnrmad_waddr);
    VPSS_REG_SetMadStride(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32madstride);
    VPSS_REG_SetSNRMadRaddr(pu32AppVir,pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr);
    if (pstHalInfo->stNrInfo.stNrMadCfg.u32Snrmad_raddr == 0)
    {
        VPSS_ERROR("u32Snrmad_raddr is zero\n");
    }
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetUHDNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
#if 1
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,HI_FALSE);
    }


	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把SNR打开 */

#if 0
	/*rwzb*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
#endif

    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetReadPixBitWidth(pu32AppVir,CUR_CHANEL,pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    if (HI_DRV_PIX_FMT_NV21_TILE_CMP == pstHalInfo->stInInfo.enFormat
        || HI_DRV_PIX_FMT_NV12_TILE_CMP == pstHalInfo->stInInfo.enFormat)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
        pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }
    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }

#if 1
    if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP
        || pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstHalInfo->stInInfo.u32Height =
                    pstHalInfo->stInInfo.u32Height/2;
        pstCur->u32Stride_Y = pstCur->u32Stride_Y*2;
        pstCur->u32Stride_C = pstCur->u32Stride_C*2;
        if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            pstCur->u32PhyAddr_Y = pstCur->u32PhyAddr_Y + pstCur->u32Stride_Y;
            pstCur->u32PhyAddr_C = pstCur->u32PhyAddr_C+ pstCur->u32Stride_C;
        }
    }
#endif

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306 for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
    VPSS_REG_SetInPixBitw(pu32AppVir,pstHalInfo->stInInfo.enBitWidth);


	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);
    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
	//t00273561
	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);

#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xc00);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xe0);


	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif
	#endif

	VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	VPSS_REG_Set4PixEn(pu32AppVir, HI_TRUE);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_Set3DFrameNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
    if (pstHalInfo->stInInfo.u32Width > VPSS_FHD_WIDTH)
    {
        VPSS_HAL_SetUHDNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
    }
    else
    {
        VPSS_HAL_SetFrameNode(enIP, pstHalInfo, pu32AppVir, u32AppPhy);
    }
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetUHDHighSpeedNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
#if 0
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither

    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

    /*tunnel*/
    VPSS_REG_SetCurTunlEn(pu32AppVir, CUR_CHANEL, HI_FALSE);

    /* 输入源信息 */
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(pu32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

	/*set 4K@P60 Two pix one cycle*/
	VPSS_REG_SetTwoPix(pu32AppVir, HI_TRUE);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither
    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_FALSE);
    }


    pstCur = &pstHalInfo->stInInfo.stAddr;
    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(pu32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}

    /*db/dr/dnr*/
    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);
    VPSS_REG_SetGlobalMotionEn(pu32AppVir,HI_FALSE);
    VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
    //t00273561
    VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

    VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);
#endif
    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetRotateNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy, HI_BOOL bRotateY, HI_U32 u32PortId)
{
#if 1
    #if 1
    HI_DRV_VPSS_ROTATION_E enRotation;
    VPSS_REG_PORT_E enPort = VPSS_REG_HD;
    VPSS_HAL_FRAME_S *pstOutFrm = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstOutAddr = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB = HI_NULL; //@f00241306 for dither
    HI_U32 u32Angle;

    VPSS_REG_ResetAppReg(pu32AppVir, NULL);

    VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);/* 默认把回写打开 */
	VPSS_REG_SetTNrEn(pu32AppVir,HI_FALSE);/* 默认把TNR打开 */
	VPSS_REG_SetSNrEn(pu32AppVir,HI_FALSE);/* 默认把SNR打开 */
     /* 输入源信息 */
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetReadPixBitWidth(pu32AppVir,CUR_CHANEL,pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    /* 帧信息配置 */
    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

    VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);

	VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);

#if 1
    if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_TOP
        || pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
    {
        pstHalInfo->stInInfo.u32Height =
                    pstHalInfo->stInInfo.u32Height/2;
        pstCur->u32Stride_Y = pstCur->u32Stride_Y*2;
        pstCur->u32Stride_C = pstCur->u32Stride_C*2;
        if (pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            pstCur->u32PhyAddr_Y = pstCur->u32PhyAddr_Y + pstCur->u32Stride_Y;
            pstCur->u32PhyAddr_C = pstCur->u32PhyAddr_C+ pstCur->u32Stride_C;
        }
    }
#endif

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    /* 输入帧信息 */
    VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C);

    VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

VPSS_DBG_INFO("Rota: in W H P %d %d %d YS CS %d %d\n", pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
    pstHalInfo->stInInfo.bProgressive, pstCur->u32Stride_Y, pstCur->u32Stride_C);

#if 0 //fix to 8bit
	/*@f00241306 for dither */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C);
		VPSS_REG_SetImgStrideLB(pu32AppVir, pstCur_LB->u32Stride_Y);
	}
#endif

    if (pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInRefInfo[0].enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_TRUE);
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir, CUR_FIELD, HI_FALSE);
    }

    /* 输出配置 */
    pstOutFrm = &pstHalInfo->astPortInfo[u32PortId].stOutInfo;
    pstOutAddr = &pstOutFrm->stAddr;
    VPSS_REG_SetFrmSize(pu32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
    VPSS_REG_SetFrmAddr(pu32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
    VPSS_REG_SetFrmStride(pu32AppVir,enPort,pstOutAddr->u32Stride_Y,pstOutAddr->u32Stride_C);
    VPSS_REG_SetFrmFormat(pu32AppVir,enPort,pstOutFrm->enFormat);
    VPSS_DBG_INFO("Rota: out W H P %d %d YS %d\n", pstOutFrm->u32Width,pstOutFrm->u32Height,pstOutAddr->u32Stride_Y);


    // fix to 8bit
    /* Set Output BitWidth */
    VPSS_REG_SetVhd0PixW(pu32AppVir,pstOutFrm->enBitWidth);
    VPSS_REG_SetVhd0Dither(pu32AppVir,REG_DITHER_MODE_DITHER,HI_FALSE);

    VPSS_REG_EnPort(pu32AppVir, enPort, HI_TRUE);

    /* 旋转配置 */
    enRotation = pstHalInfo->astPortInfo[u32PortId].enRotation;

    switch(enRotation)
    {
        case HI_DRV_VPSS_ROTATION_90:
            u32Angle = 0x0;
            break;
        case HI_DRV_VPSS_ROTATION_270:
            u32Angle = 0x1;
            break;
        default:
            u32Angle = 0x1;
            VPSS_FATAL("Ro Error  %d\n",u32Angle);
            break;
    }

    if (bRotateY)
    {
        VPSS_REG_SetRotation(pu32AppVir,u32Angle,1);
    }
    else
    {
        VPSS_REG_SetRotation(pu32AppVir,u32Angle,2);
    }

	VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);

	//t00273561
    VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);
#ifdef HI_VPSS_SMMU_SUPPORT
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);

	VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
	VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

	VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

    #endif
#endif
    return HI_SUCCESS;
}

#ifdef ZME_2L_TEST //@f00241306
HI_S32 VPSS_HAL_SetZME2LNode(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,HI_U32 *pu32AppVir, HI_U32 u32AppPhy,  HI_U32 u32PortId)
{
#if 0
	HI_DRV_VID_FRAME_ADDR_S *pstCur		= HI_NULL;
	HI_DRV_VID_FRAME_ADDR_S *pstCur_LB	= HI_NULL; //@f00241306 for dither

	HI_BOOL abPortUsed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER] = {HI_FALSE, HI_FALSE, HI_FALSE};
	VPSS_HAL_PORT_INFO_S	*pstHalPort = HI_NULL;
	VPSS_HAL_FRAME_S		*pstOutFrm = HI_NULL;
	VPSS_REG_PORT_E			enPort = VPSS_REG_BUTT;

	pstHalPort = &pstHalInfo->astPortInfo[u32PortId];
	if (pstHalPort->bEnable)
	{
		enPort = VPSS_HAL_AllocPortId(pstHalPort, abPortUsed);
		if (enPort == VPSS_REG_BUTT)
		{
			HI_ASSERT(0);
		}
	}

    pstOutFrm = &pstHalPort->stOutInfo;

    VPSS_REG_ResetAppReg(pu32AppVir, HI_NULL);

	/* 输入帧信息配置 */
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetPixBitW(pu32AppVir, pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

    pstCur = &pstHalInfo->stInInfo.stAddr;
    pstCur_LB = &pstHalInfo->stInInfo.stAddr_LB;//@f00241306 for dither

	/*@f00241306      for dither  */
	if( HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth )
	{
		VPSS_REG_SetImgAddrLB(pu32AppVir, CUR_FIELD, pstCur_LB->u32PhyAddr_Y, pstCur_LB->u32PhyAddr_C, 0);
		VPSS_REG_SetImgStrideLB(pu32AppVir, CUR_FIELD, pstCur_LB->u32Stride_Y, pstCur_LB->u32Stride_C);
	}

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_FALSE);
    }

    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height,
        pstHalInfo->stInInfo.bProgressive);

    VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,
                        pstCur->u32PhyAddr_Y,
                        pstCur->u32PhyAddr_C,
                        0);
    VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

	/* 输出帧信息配置 */
		/*ZME*/
    VPSS_HAL_SetZmeCfg(enIP,pu32AppVir, pstHalInfo,enPort,u32PortId);
	VPSS_REG_SetAddr(pu32AppVir,REG_VPSS_ZME_ADDR,VPSS_ZME_ADDR_GET(pu32AppVir));
	VPSS_REG_SetZmeEn(pu32AppVir, enPort, HI_TRUE);

    /* 输出格式 */
    VPSS_REG_SetFrmSize(pu32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
    VPSS_REG_SetFrmAddr(pu32AppVir,enPort,pstOutFrm->stAddr.u32PhyAddr_Y, pstOutFrm->stAddr.u32PhyAddr_C);
    VPSS_REG_SetFrmStride(pu32AppVir,enPort,pstOutFrm->stAddr.u32Stride_Y,pstOutFrm->stAddr.u32Stride_C);
    VPSS_REG_SetFrmFormat(pu32AppVir,enPort,pstOutFrm->enFormat);

	/* Set Output BitWidth   */
	VPSS_REG_SetPortPixBitW(pu32AppVir,enPort, pstOutFrm->enBitWidth);

	VPSS_REG_EnPort(pu32AppVir, enPort, HI_TRUE);


    /*db/dr/dnr*/
//    VPSS_HAL_SetDnrCfg(enIP,pu32AppVir,pstHalInfo);

//	VPSS_REG_SetGlobalMotionEn(pu32AppVir,HI_FALSE);

//	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);
	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

	VPSS_HAL_SetAlgParaAddr(pu32AppVir,u32AppPhy);
#endif
    return HI_SUCCESS;
}
#endif


HI_S32 VPSS_HAL_SetNode_H265_Step1_Interlace(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
	HI_DRV_VID_FRAME_ADDR_S *pstCur = HI_NULL;
    VPSS_HAL_PORT_INFO_S *pstHalPort = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

	VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetTNrEn(pu32AppVir, HI_FALSE);
	VPSS_REG_SetCurTunlEn(pu32AppVir, HI_FALSE);


	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);
    /*tunnel*/
    if (pstHalInfo->stInInfo.u32TunnelAddr)
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir,  HI_TRUE);
        VPSS_REG_SetCurTunlAddr(pu32AppVir,pstHalInfo->stInInfo.u32TunnelAddr);
    }
    else
    {
        VPSS_REG_SetCurTunlEn(pu32AppVir, HI_FALSE);
    }


    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetImgTile(pu32AppVir,NEXT2_FIELD,HI_FALSE);
    }

    if (pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP
        || pstHalInfo->stInInfo.enFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
    {
        VPSS_REG_SetDcmpHeadAddr(pu32AppVir,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_YHead,
            pstHalInfo->stInInfo.stAddr.u32PhyAddr_CHead,
            pstHalInfo->stInInfo.stAddr.u32Head_Size);
        VPSS_REG_SetDcmpHeadStride(pu32AppVir,
        pstHalInfo->stInInfo.stAddr.u32Head_Stride);
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_TRUE);
    }
    else
    {
        VPSS_REG_SetDcmpEn(pu32AppVir,HI_FALSE);
    }

    VPSS_REG_SetReadPixBitWidth(pu32AppVir, NEXT2_FIELD,pstHalInfo->stInInfo.enBitWidth);
    VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);

	/* 输入帧信息 */
	pstCur = &pstHalInfo->stInInfo.stAddr;
	VPSS_REG_SetImgFormat(pu32AppVir, pstHalInfo->stInInfo.enFormat);
	VPSS_REG_SetImgSize(pu32AppVir,
			pstHalInfo->stInInfo.u32Width,
			pstHalInfo->stInInfo.u32Height,
			HI_TRUE);

	VPSS_DBG_INFO("STEP1: Read nx2 YAddr YS CS %x %d %d\n",
	        pstCur->u32PhyAddr_Y,
	        pstCur->u32Stride_Y, pstCur->u32Stride_C);


	VPSS_REG_SetImgAddr(pu32AppVir,NEXT2_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C);
	VPSS_REG_SetImgStride(pu32AppVir,NEXT2_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);


	/*DBM*/
	VPSS_REG_SetDbmEn(pu32AppVir,HI_FALSE);
	VPSS_REG_SetBlkDetEn(pu32AppVir,HI_FALSE);

	VPSS_REG_SetIglbEn(pu32AppVir,HI_FALSE);
	VPSS_REG_SetIfmdEn(pu32AppVir,HI_FALSE);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);


	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

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
#if 0
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
			stFrame.stBufAddr[0].u32Stride_Y = pstOutAddr->u32Stride_Y;
			stFrame.stBufAddr[0].u32Stride_C = pstOutAddr->u32Stride_C;

            VPSS_DBG_INFO("STEP1: tmp img save: Yaddr w h YS CS %x %d %d %d %d",
              stFrame.stBufAddr[0].u32PhyAddr_Y,
              stFrame.u32Width, stFrame.u32Height, stFrame.stBufAddr[0].u32Stride_Y,
              stFrame.stBufAddr[0].u32Stride_C);
			VPSS_OSAL_WRITEYUV(&stFrame, chFile);
        }
#endif
        /* Flip&Mirro */
        VPSS_REG_SetPortMirrorEn(pu32AppVir, enPort,HI_FALSE);
        VPSS_REG_SetPortFlipEn(pu32AppVir, enPort,HI_FALSE);

        /* UV反转 */
		VPSS_REG_SetPortUVInvert(pu32AppVir, enPort, HI_FALSE);

		VPSS_REG_SetPortCropEn(pu32AppVir,HI_FALSE);

        /*ZME*/
        VPSS_HAL_SetZmeCfg(enIP,pu32AppVir, pstHalInfo,enPort,0);

        /* LBX */
        VPSS_REG_SetLBABg(pu32AppVir, enPort, 0x108080, 0x7f);
        VPSS_REG_SetLBAVidPos(pu32AppVir, enPort,
                                (HI_U32)pstHalPort->stVideoRect.s32X,
                                (HI_U32)pstHalPort->stVideoRect.s32Y,
                                pstHalPort->stVideoRect.s32Height,
                                pstHalPort->stVideoRect.s32Width);

        VPSS_REG_SetLBADispPos(pu32AppVir, enPort, 0, 0,
                               pstOutFrm->u32Height, pstOutFrm->u32Width);
        VPSS_REG_SetLBAEn(pu32AppVir, enPort, HI_FALSE);

        /* 输出格式 */
        VPSS_REG_SetFrmSize(pu32AppVir,enPort,pstOutFrm->u32Height, pstOutFrm->u32Width);
        VPSS_REG_SetFrmAddr(pu32AppVir,enPort,pstOutAddr->u32PhyAddr_Y, pstOutAddr->u32PhyAddr_C);
        VPSS_REG_SetFrmStride(pu32AppVir,enPort,pstOutAddr->u32Stride_Y*2,pstOutAddr->u32Stride_C*2);
        VPSS_REG_SetFrmFormat(pu32AppVir,enPort,HI_DRV_PIX_FMT_NV21);

		VPSS_DBG_INFO("STEP1: vhd0 addr W H YS CS %x %d %d %d %d\n",
                            pstOutAddr->u32PhyAddr_Y,
                            pstOutFrm->u32Width,pstOutFrm->u32Height,
							pstOutAddr->u32Stride_Y*2, pstOutAddr->u32Stride_C*2);


        VPSS_REG_EnPort(pu32AppVir, enPort, HI_TRUE);
    }

#ifdef HI_VPSS_SMMU_SUPPORT
    VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);
    VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);
    VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
    VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);
    VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

	return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetH265DeiCfg(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32* pu32AppVir,HI_U32 u32AppPhy)

{
    HI_U32  VPSS_DEI_ADDR;

    HI_DRV_VID_FRAME_ADDR_S *pstRef  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstCur  = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt1 = HI_NULL;
    HI_DRV_VID_FRAME_ADDR_S *pstNxt2 = HI_NULL;
	HI_U32 u32Head = 0;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);

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

	VPSS_DBG_INFO("Ref Cur Nx1 Nx2 %x %x %x %x\n", pstRef->u32PhyAddr_Y,
								pstCur->u32PhyAddr_Y, pstNxt1->u32PhyAddr_Y, pstNxt2->u32PhyAddr_Y);

    /*dei*/
    VPSS_REG_SetImgReadMod(pu32AppVir, HI_TRUE);
    VPSS_REG_EnDei(pu32AppVir, HI_TRUE);
    VPSS_REG_SetDeiTopFirst(pu32AppVir, pstHalInfo->stInInfo.bTopFirst);

    switch(pstHalInfo->stInInfo.enFieldMode)
    {
        case HI_DRV_FIELD_BOTTOM:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_TRUE);
            break;
        case HI_DRV_FIELD_TOP:
            VPSS_REG_SetDeiFieldMode(pu32AppVir, HI_FALSE);
            break;
        default:
            VPSS_FATAL("No spt field Type:%d!\n", pstHalInfo->stInInfo.enFieldMode);
            return HI_FAILURE;
    }


    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_CHROME,HI_TRUE);
    VPSS_REG_SetModeEn(pu32AppVir,REG_DIE_MODE_LUMA,HI_TRUE);

    VPSS_DEI_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DIECTRL)-sizeof(HI_U32);
    VPSS_REG_SetDeiParaAddr(pu32AppVir,VPSS_DEI_ADDR);

    VPSS_REG_SetMode(pu32AppVir, REG_DIE_MODE_ALL, 1);//0为5场模式、1为4场模式、2为3场模式

    /*********************************ref**************************************/
    VPSS_REG_SetImgTile(pu32AppVir, LAST_FIELD, HI_FALSE);
    VPSS_REG_SetImgAddr(pu32AppVir, LAST_FIELD, pstRef->u32PhyAddr_Y,
						pstRef->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,LAST_FIELD, pstRef->u32Stride_Y, pstRef->u32Stride_C);

    /************************* Cur **********************************/
    VPSS_REG_SetImgTile(pu32AppVir,CUR_FIELD,HI_FALSE);

    VPSS_REG_SetRefWidth(pu32AppVir,pstHalInfo->stInRefInfo[1].u32Width);
    VPSS_REG_SetRefHight(pu32AppVir,pstHalInfo->stInRefInfo[1].u32Height*2);


    VPSS_REG_SetImgAddr(pu32AppVir,CUR_FIELD,pstCur->u32PhyAddr_Y,pstCur->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir,CUR_FIELD, pstCur->u32Stride_Y, pstCur->u32Stride_C);

    /*******************************next1**************************************/
    VPSS_REG_SetImgTile(pu32AppVir, NEXT1_FIELD, HI_FALSE);
    VPSS_REG_SetImgAddr(pu32AppVir, NEXT1_FIELD, pstNxt1->u32PhyAddr_Y,
						pstNxt1->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir, NEXT1_FIELD, pstNxt1->u32Stride_Y, pstNxt1->u32Stride_C);

    /*****************************next2****************************************/
    VPSS_REG_SetImgTile(pu32AppVir, NEXT2_FIELD,HI_FALSE);

    VPSS_REG_SetImgAddr(pu32AppVir, NEXT2_FIELD, pstNxt2->u32PhyAddr_Y,
								pstNxt2->u32PhyAddr_C);
    VPSS_REG_SetImgStride(pu32AppVir, NEXT2_FIELD, pstNxt2->u32Stride_Y, pstNxt2->u32Stride_C);

    return HI_SUCCESS;
}


HI_S32 VPSS_HAL_SetNode_H265_Step2_Dei(VPSS_IP_E enIP, VPSS_HAL_INFO_S *pstHalInfo,
    HI_U32 *pu32AppVir, HI_U32 u32AppPhy)
{
	HI_U32 VPSS_Tnr_ADDR;
	HI_U32 VPSS_Tnr_CLUT_ADDR;
    VPSS_DIESTCFG_S *pstDeiSt = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg);

	VPSS_REG_SetProtEn(pu32AppVir,pstHalInfo->stInInfo.bSecure);

    /* 需要强制设置的模式和开关 */
    VPSS_REG_SetInCropEn(pu32AppVir, HI_FALSE);

	VPSS_REG_SetRfrEn(pu32AppVir, HI_FALSE);
    VPSS_REG_SetTNrEn(pu32AppVir, HI_FALSE);

    VPSS_REG_SetImgFormat(pu32AppVir, HI_DRV_PIX_FMT_NV21);
    VPSS_REG_SetImgSize(pu32AppVir,
        pstHalInfo->stInInfo.u32Width, pstHalInfo->stInInfo.u32Height*2,
        pstHalInfo->stInInfo.bProgressive);

    VPSS_REG_SetReadPixBitWidth(pu32AppVir, NEXT2_FIELD,pstHalInfo->stInInfo.enBitWidth);

	/*DEI*/
	VPSS_HAL_SetH265DeiCfg(enIP, pstHalInfo, pu32AppVir, u32AppPhy);

	/* 运动信息  DEI*/
	pstDeiSt = &pstHalInfo->stDieInfo.stDieStCfg;
	VPSS_REG_SetStRdAddr(pu32AppVir, pstDeiSt->u32PPreAddr);
	VPSS_REG_SetStWrAddr(pu32AppVir, pstDeiSt->u32PreAddr);
	VPSS_REG_SetStStride(pu32AppVir, pstDeiSt->u32Stride);

    /*DBM*/
	if (pstHalInfo->stInInfo.u32Width > 128
	    && pstHalInfo->stInInfo.u32Height > 64)
	{
        VPSS_REG_SetDbmEn(pu32AppVir, HI_TRUE);
	}
	else
	{

        VPSS_REG_SetDbmEn(pu32AppVir, HI_FALSE);
        VPSS_REG_SetBlkDetEn(pu32AppVir, HI_FALSE);
	}


	VPSS_REG_SetIglbEn(pu32AppVir,HI_TRUE);
	VPSS_REG_SetIfmdEn(pu32AppVir,HI_TRUE);

    /*RWZB*/
    VPSS_HAL_SetRwzbCfg(pu32AppVir, &(pstHalInfo->stRwzbInfo));
	if (pstHalInfo->stRwzbInfo.u32IsRwzb)
	{
		VPSS_REG_SetOutSel(pu32AppVir,REG_DIE_MODE_ALL,HI_TRUE);
		VPSS_REG_SetDbmEn(pu32AppVir,HI_FALSE);
	}

    /* 输出Port信息 */
    VPSS_HAL_SetPortCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);

	VPSS_Tnr_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_MODE) - sizeof(HI_U32);
	VPSS_Tnr_CLUT_ADDR = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_TNR_CLUT10) - sizeof(HI_U32);
	VPSS_REG_SetTnrAddr(pu32AppVir,VPSS_Tnr_ADDR,VPSS_Tnr_CLUT_ADDR);

	VPSS_REG_SetSttWrAddr(pu32AppVir, pstHalInfo->u32stt_w_phy_addr);

#ifdef HI_VPSS_SMMU_SUPPORT
    VPSS_REG_SetRchSmmuBypass(pu32AppVir,0x0);

    VPSS_REG_SetWchSmmuBypass(pu32AppVir,0x0);

    VPSS_HAL_SetSMMUCfg(enIP, pu32AppVir, u32AppPhy, pstHalInfo);
#else
    VPSS_REG_SetRchSmmuBypass(pu32AppVir,0xffffffff);

    VPSS_REG_SetWchSmmuBypass(pu32AppVir,0xffffffff);
#endif

    pstHalInfo->pstH265RefList->u32RefListHead = (pstHalInfo->pstH265RefList->u32RefListHead+1)%DEF_VPSS_HAL_REF_LIST_NUM;

    return HI_SUCCESS;
}


HI_VOID VPSS_HAL_DumpReg(VPSS_IP_E enIP, VPSS_HAL_TASK_NODE_E enTaskNodeId)
{
    HI_U32 i;
    HI_U32 *pu32Reg;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

    pu32Reg = (HI_U32 *)(pstHalCtx->apu8AppVir[enTaskNodeId]);

    printk("\n\nPhyAddr %8x   enTaskNodeId = %d\n\n",
        pstHalCtx->au32AppPhy[enTaskNodeId], enTaskNodeId);

    for(i=0; i<(64*4); i++)
    {
        if(i%(64) == 0)
        {
            printk("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            printk("\n%x0:", (i%64)/4);
        }

        printk("%.8x  ", *(pu32Reg+i));
    }

    printk("\n\n");

    for(i=1024; i<1024+64; i++)
    {
        if(i%(64) == 0)
        {
            printk("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            printk("\n%x0:", (i%64)/4);
        }

        printk("%.8x  ", *(pu32Reg+i));
    }

    printk("\n\n");

    for(i=2048; i<2048+64; i++)
    {
        if(i%(64) == 0)
        {
            printk("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            printk("\n%x0:", (i%64)/4);
        }

        printk("%.8x  ", *(pu32Reg+i));
    }

    printk("\n\n");

    for(i=4096; i<4096+64*3; i++)
    {
        if(i%(64) == 0)
        {
            printk("\nBASE %x", i*4);
        }

        if(i%4 == 0)
        {
            printk("\n%x0:", (i%64)/4);
        }

        printk("%.8x  ", *(pu32Reg+i));
    }
    printk("\n\n");

}

#if 0
/* 以上为内部调用函数 */
/* 以下为外部调用函数 */
#endif




HI_S32 VPSS_HAL_Init(VPSS_IP_E enIP)
{
	HI_U32 i;
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];

    if (pstHalCtx->bInit)
    {
        VPSS_INFO("VPSS IP%d, Already Init\n", enIP);
        return HI_SUCCESS;
    }

    /* 申请和分配NODE节点 */
    s32Ret = HI_DRV_MMZ_AllocAndMap("VPSS_RegBuf", HI_NULL,
        VPSS_REG_SIZE*VPSS_HAL_TASK_NODE_BUTT, 0, &pstHalCtx->stRegBuf);
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Alloc VPSS_RegBuf Failed\n");
        return HI_FAILURE;
    }

    for (i=0; i<VPSS_HAL_TASK_NODE_BUTT; i++)
    {
        pstHalCtx->au32AppPhy[i] = pstHalCtx->stRegBuf.u32StartPhyAddr
                                 + VPSS_REG_SIZE*i;

        pstHalCtx->apu8AppVir[i] = pstHalCtx->stRegBuf.pu8StartVirAddr
                                 + VPSS_REG_SIZE*i;
    }


    /* 映射寄存器地址 */
   // pstHalCtx->pu32BaseRegVir = IO_ADDRESS(pstHalCtx->u32BaseRegPhy);

	pstHalCtx->pu8BaseRegVir
		   = ioremap_nocache(pstHalCtx->u32BaseRegPhy, VPSS_REG_SIZE);

    if (0 == pstHalCtx->pu8BaseRegVir)
    {
        VPSS_FATAL("io_address VPSS_REG(%#x) Failed\n", pstHalCtx->u32BaseRegPhy);
        HI_DRV_MMZ_UnmapAndRelease(&pstHalCtx->stRegBuf);
        return HI_FAILURE;
    }

#ifdef HI_VPSS_SMMU_SUPPORT
    memset(&(pstHalCtx->stDeTileMMUBuf),0,sizeof(SMMU_BUFFER_S));
#else
    memset(&(pstHalCtx->stDeTileMMZBuf),0,sizeof(MMZ_BUFFER_S));
#endif

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

HI_S32 VPSS_HAL_DelInit(VPSS_IP_E enIP)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

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

    VPSS_HAL_FreeDetileBuffer(enIP);

    pstHalCtx->bInit = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetClockEn(VPSS_IP_E enIP, HI_BOOL bClockEn)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    if(pstHalCtx->bClockEn == bClockEn)
    {
        return HI_SUCCESS;
    }

    if (bClockEn)
    {
#if 1
        VPSS_REG_SetClockEn(HI_TRUE);
       //VPSS_REG_ResetAppReg((HI_U32 *)pstHalCtx->pu8BaseRegVir, HI_NULL);
       if (0)
       {
            VPSS_REG_S *pstReg;

            pstReg = (VPSS_REG_S *)pstHalCtx->pu8BaseRegVir;

            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_MISCELLANEOUS.u32), 0x3006466);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_PNEXT), 0x0);
            VPSS_REG_RegWrite((volatile HI_U32*)&(pstReg->VPSS_INTMASK.u32), 0xff);
       }

        VPSS_REG_SetTimeOut((HI_U32 *)pstHalCtx->pu8BaseRegVir, DEF_LOGIC_TIMEOUT);
        VPSS_REG_SetIntMask((HI_U32 *)pstHalCtx->pu8BaseRegVir, 0xfe);
        VPSS_REG_ClearIntState((HI_U32 *)pstHalCtx->pu8BaseRegVir, 0xf);
#endif
    }
    else
    {
        VPSS_REG_SetClockEn(HI_FALSE);
    }

    pstHalCtx->bClockEn = bClockEn;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_GetClockEn(VPSS_IP_E enIP, HI_BOOL *pbClockEn)
{
    //:TODO:后续加入CRG和时钟控制
    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_GetIntState(VPSS_IP_E enIP, HI_U32* pu32IntState)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;
    HI_U32 u32SmmuSeIntState;
    HI_U32 u32SmmuNSIntState;
    VPSS_REG_S *pstReg;
    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pu32IntState);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    pstReg = (VPSS_REG_S *)pstHalCtx->pu8BaseRegVir;

    VPSS_REG_GetIntState((HI_U32 *)pstHalCtx->pu8BaseRegVir, pu32IntState);

    VPSS_REG_GetSmmuIntState(pstHalCtx->pu8BaseRegVir, &u32SmmuSeIntState, &u32SmmuNSIntState);

    if (u32SmmuNSIntState != 0)
    {
        HI_PRINT("SMMU_WRITE_ERR MODULE : %s  NSSTATE : %#x RdAddr:%#x : WrAddr:%#x\n",
            HI_MOD_VPSS,u32SmmuNSIntState,
            pstReg->VPSS_SMMU_FAULT_ADDR_RD_NS,
            pstReg->VPSS_SMMU_FAULT_ADDR_WR_NS);
    }

    if (u32SmmuSeIntState != 0)
    {
        HI_PRINT("SMMU_WRITE_ERR MODULE : %s  SSTATE : %#x RdAddr:%#x : WrAddr:%#x\n",
            HI_MOD_VPSS, u32SmmuSeIntState,
            pstReg->VPSS_SMMU_FAULT_ADDR_RD_S,
            pstReg->VPSS_SMMU_FAULT_ADDR_WR_S);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_ClearIntState(VPSS_IP_E enIP, HI_U32 u32IntState)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    VPSS_REG_ClearIntState((HI_U32 *)pstHalCtx->pu8BaseRegVir, u32IntState);

    VPSS_REG_ClearSmmuInt(pstHalCtx->pu8BaseRegVir);

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetNodeInfo(VPSS_IP_E enIP,
     VPSS_HAL_INFO_S *pstHalInfo,  VPSS_HAL_TASK_NODE_E enTaskNodeId)
{
    HI_S32 s32Ret = HI_FAILURE;
#if 1
#if !defined(HI_NVR_SUPPORT)
    HI_U32 u32PortId = 0;
#endif
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;
    HI_U32*  pu32AppVir, u32AppPhy;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(pstHalInfo);
    VPSS_HAL_CHECK_NODE_ID_VAILD(enTaskNodeId);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    pu32AppVir = (HI_U32 *)pstHalCtx->apu8AppVir[enTaskNodeId];
    u32AppPhy = pstHalCtx->au32AppPhy[enTaskNodeId];

	//pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_FRAME;

#if defined(HI_NVR_SUPPORT)
	s32Ret = VPSS_HAL_SetNVRHighSpeedNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
#else
	switch(pstHalInfo->enNodeType)
    {
        case VPSS_HAL_NODE_2D_Field:
            s32Ret = VPSS_HAL_SetFieldNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_FRAME:
			s32Ret = VPSS_HAL_SetFrameNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_5Field:
            s32Ret = VPSS_HAL_Set5FieldNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
		case VPSS_HAL_NODE_2D_DETILE_STEP1:
			VPSS_HAL_SetDetileNode_STEP1(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
			break;
		case VPSS_HAL_NODE_2D_DETILE_STEP2:
			VPSS_HAL_SetFirst3FieldNode_STEP2(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
			break;
        case VPSS_HAL_NODE_3D_FRAME_R:
            s32Ret = VPSS_HAL_Set3DFrameNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_UHD:
        case VPSS_HAL_NODE_UHD_SPLIT_L:
        case VPSS_HAL_NODE_UHD_SPLIT_R:
            s32Ret = VPSS_HAL_SetUHDNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_UHD_HIGH_SPEED:
            s32Ret = VPSS_HAL_SetUHDHighSpeedNode(enIP, pstHalInfo,  pu32AppVir,u32AppPhy);
            break;
        case VPSS_HAL_NODE_HDR_DETILE_NOALG:
            s32Ret = VPSS_HAL_SetHDRNode(enIP, pstHalInfo,  pu32AppVir,u32AppPhy);
            break;
#ifdef ZME_2L_TEST
        case VPSS_HAL_NODE_ZME_2L:
			u32PortId = enTaskNodeId - VPSS_HAL_TASK_NODE_P0_ZME_L2;
            VPSS_HAL_SetZME2LNode(enIP,pstHalInfo,  pu32AppVir,u32AppPhy,u32PortId);
            break;
#endif
#if 0
        case VPSS_HAL_NODE_2D_3Field:
            //VPSS_HAL_SetFrameNode(pstHalInfo,  pu32AppVir);
            break;
        case VPSS_HAL_NODE_PZME:
            //VPSS_HAL_SetFrameNode(pstHalInfo,  pu32AppVir);
            break;

        case VPSS_HAL_NODE_3DDET:
            //VPSS_HAL_SetFrameNode(pstHalInfo,  pu32AppVir);
            break;
        case VPSS_HAL_NODE_ZME_2L:
			u32PortId = enTaskNodeId - VPSS_HAL_TASK_NODE_P0_ZME_L2;
            VPSS_HAL_SetZME2LNode(enIP,pstHalInfo,  pu32AppVir,u32AppPhy,u32PortId);
            break;
        case VPSS_HAL_NODE_ROTATION_Y:
            //VPSS_HAL_SetFrameNode(pstHalInfo,  pu32AppVir);
            break;
        case VPSS_HAL_NODE_ROTATION_C:
            //VPSS_HAL_SetFrameNode(pstHalInfo,  pu32AppVir);
            break;
#endif
        case VPSS_HAL_NODE_ROTATION_Y:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;//:TODO:优化
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy, HI_TRUE,u32PortId);
            break;
        case VPSS_HAL_NODE_ROTATION_C:
            u32PortId = (enTaskNodeId - VPSS_HAL_TASK_NODE_P0_RO_Y) / 2;
            s32Ret = VPSS_HAL_SetRotateNode(enIP, pstHalInfo,  pu32AppVir, u32AppPhy, HI_FALSE,u32PortId);
            break;
        case VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE:
            VPSS_HAL_SetNode_H265_Step1_Interlace(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;
        case VPSS_HAL_NODE_2D_H265_STEP2_DEI:
            VPSS_HAL_SetNode_H265_Step2_Dei(enIP, pstHalInfo,  pu32AppVir, u32AppPhy);
            break;

        default:
            VPSS_FATAL("No this Node Type:%d!\n", pstHalInfo->enNodeType);
            return HI_FAILURE;

    }
#endif
    (HI_VOID)VPSS_HAL_SetAllAlgCfgAddr( pu32AppVir, u32AppPhy);

#if 0
    VPSS_FATAL("enTaskNodeId = %d, this Node Type:%d!\n", enTaskNodeId, pstHalInfo->enNodeType);
	if ((VPSS_HAL_NODE_2D_DETILE_STEP1 == pstHalInfo->enNodeType)
		||(VPSS_HAL_NODE_2D_DETILE_STEP2 == pstHalInfo->enNodeType))
	{
		VPSS_HAL_DumpReg(enIP,enTaskNodeId);
	}
#endif

#endif
    return s32Ret;
}

HI_S32 VPSS_HAL_StartLogic(VPSS_IP_E enIP,
    HI_BOOL abNodeVaild[VPSS_HAL_TASK_NODE_BUTT])
{
    HI_U32 i = 0;
    VPSS_HAL_TASK_NODE_E enId, enLastVaild = VPSS_HAL_TASK_NODE_BUTT;
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    VPSS_HAL_CHECK_IP_VAILD(enIP);
    VPSS_HAL_CHECK_NULL_PTR(abNodeVaild);

    pstHalCtx = &stHalCtx[enIP];
    VPSS_HAL_CHECK_INIT(pstHalCtx->bInit);

    /* 从后向前链接所有节点 */
    enLastVaild = VPSS_HAL_TASK_NODE_BUTT;
    for(i=0;i<VPSS_HAL_TASK_NODE_BUTT;i++)
    {
        enId = VPSS_HAL_TASK_NODE_BUTT - 1 - i;

        if(HI_TRUE == abNodeVaild[enId])
        {
            if(VPSS_HAL_TASK_NODE_BUTT == enLastVaild)
            {
                VPSS_REG_StartLogic(0, (HI_U32*)pstHalCtx->apu8AppVir[enId]);
            }
            else
            {
                VPSS_REG_StartLogic(pstHalCtx->au32AppPhy[enLastVaild],
                    (HI_U32*)pstHalCtx->apu8AppVir[enId]);
            }

            enLastVaild = enId;
        }
    }

    HI_ASSERT(enLastVaild != VPSS_HAL_TASK_NODE_BUTT);
    if (enLastVaild == VPSS_HAL_TASK_NODE_BUTT)
    {
        VPSS_FATAL("No Node Needs Start\n");
        return HI_FAILURE;
    }
    else
    {
        /* 启动硬件 */
        return VPSS_REG_StartLogic(pstHalCtx->au32AppPhy[enLastVaild],
                            (HI_U32 *)pstHalCtx->pu8BaseRegVir);
    }
}




HI_S32 VPSS_HAL_GetSCDInfo(HI_U32* pu32AppAddr,HI_S32 s32SCDInfo[32])
{
	//return VPSS_REG_GetSCDInfo(pu32AppAddr,s32SCDInfo);
	return HI_SUCCESS;
}


HI_VOID VPSS_HAL_GetDetPixel(HI_U32 *pu32AppAddr,HI_U32 BlkNum, HI_U8* pstData)
{
    VPSS_REG_GetDetPixel(pu32AppAddr,BlkNum,pstData);
}

HI_S32 VPSS_HAL_GetBaseRegAddr(VPSS_IP_E enIP,
                                 HI_U32 *pu32PhyAddr,
                                 HI_U8 **ppu8VirAddr)
{
    VPSS_HAL_CTX_S *pstHalCtx = HI_NULL;

    pstHalCtx = &stHalCtx[enIP];

    *pu32PhyAddr = pstHalCtx->u32BaseRegPhy;
    *ppu8VirAddr = pstHalCtx->pu8BaseRegVir;

    return HI_SUCCESS;
}

HI_S32 VPSS_HAL_SetAlgParaAddr(HI_U32* pu32AppVir,HI_U32 u32AppPhy)
{
#if 1
	HI_U32 u32DBMParaAddr;
	HI_U32 u32SNRParaAddr;
#if 0
	u32Vc1ParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VC1_CTRL0)-sizeof(HI_U32);

	VPSS_REG_SetVc1ParaAddr(pu32AppVir,u32Vc1ParaAddr);

	u32ZmeParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_VHD0_HSP)-sizeof(HI_U32);
	VPSS_REG_SetZmeParaAddr(pu32AppVir,u32ZmeParaAddr);

	u32HspParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_HSPCFG1)-sizeof(HI_U32);
	VPSS_REG_SetHspParaAddr(pu32AppVir,u32HspParaAddr);
#endif
#if 1

	u32SNRParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_SNR_ENABLE)-sizeof(HI_U32);
	u32DBMParaAddr = u32AppPhy + VPSS_REG_SIZE_CALC(VPSS_CTRL,VPSS_DB_CTRL)-sizeof(HI_U32);
	VPSS_REG_SetDBMAddr(pu32AppVir,u32DBMParaAddr);
	VPSS_REG_SetSnrAddr(pu32AppVir,u32SNRParaAddr);
#endif
#endif
	return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */
