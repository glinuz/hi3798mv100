#include "vpss_ctrl.h"
#include "vpss_common.h"
#include "hi_drv_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if DEF_VPSS_STATIC
HI_U32 g_u32LogicStart = 0;
HI_U32 g_u32LogicEnd = 0;

HI_U32 g_u32LogicTime[15];
HI_U32 g_u32LogicCycle[2][15];
HI_U32 g_u32Pos = 0;
#endif
HI_U8 *g_pAlgModeString[4] = {
    "off",
    "on",
    "auto",
    "butt",
};
HI_U8 *g_pInstState[3] = {
    "stop",
    "working",
    "butt",
};
HI_U8 *g_pProgDetectString[4] = {
    "P",
    "I",
    "auto",
    "butt",
};
HI_U8 *g_pRotationString[5] = {
    "00",
    "90",
    "180",
    "270",
    "butt",
};
HI_U8 *g_pDeiString[9] = {
    "off",
    "auto",
    "2 field",
    "3 field",
    "4 field",
    "5 field",
    "6 field",
    "7 field",
    "butt",
};

HI_U8 *g_pSrcMutualString[3] = {
    "src active",
    "vpss active",
    "butt",
};

HI_U8 *g_pPixString[14] = {
    "YCbCr420",
    "YCrCb420",
    "YCbCr411",
    "YCbCr422",
    "YCrCb422",
    "YCbCr422_2X1",
    "YCrCb422_2X1",
    "YCbCr420_cmp",
    "YCrCb420_cmp",
    "YCbCr422_cmp",
    "YCrCb422_cmp",
    "YCbCr422_2X1_cmp",
    "YCrCb422_2X1_cmp",
    "butt",
};

HI_U8 *g_pAspString[8] = {
    "Full",
    "LBOX",
    "PANSCAN",
    "COMBINED",
    "FULL_H",
    "FULL_V",
    "CUSTOMER",
    "butt",
};

HI_U8 *g_pSrcModuleString[10] = {
    "Vdec",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Vi",
    "Venc",
    "Unknow"
};

HI_U8 *g_pBufTypeString[3] = {
    "vpss",
    "usr",
    "unknow",
};
HI_U8 *g_pRotateString[5] = {
    "Rotation_00",
    "Rotation_90",
    "Rotation_180",
    "Rotation_270",
    "Rotation_butt",
};
HI_U8 *g_pCscString[20] = {
    "UNKNOWN",
    "DEFAULT",
    "BT601_YUV",
    "BT601_YUV",
    "BT601_RGB",
    "BT601_RGB",
    "NTSC1953",
    "BT470_SYSTEM_M",
    "BT470_SYSTEM_BG",
    "BT709_YUV",
    "BT709_YUV",
    "BT709_RGB",
    "BT709_RGB",
    "REC709",
    "SMPT170M",
    "SMPT240M",
    "BT878",
    "XVYCC",
    "JPEG",
    "BUTT",
};
static VPSS_CTRL_S g_stVpssCtrl[VPSS_IP_BUTT] =
{
    {
        .bIPVaild = HI_FALSE,
        .enIP     = VPSS_IP_0,
        .u32VpssIrqNum = VPSS0_IRQ_NUM,
        .pVpssIntService = VPSS0_CTRL_IntService,
        .isr_name = "VPSS0_ISR",
        .s32IsVPSSOpen = 0,
    },

    {
        .bIPVaild = HI_TRUE,
        .enIP     = VPSS_IP_1,
        .u32VpssIrqNum = VPSS1_IRQ_NUM,
        .pVpssIntService = VPSS1_CTRL_IntService,
        .isr_name = "VPSS1_ISR",
        .s32IsVPSSOpen = 0,
    }
};

HI_VOID VPSS_CTRL_InitInstList(VPSS_IP_E enIp)
{
    HI_U32 u32Count;
    VPSS_INST_CTRL_S *pstInsList;

    pstInsList = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
    rwlock_init(&(pstInsList->stListLock));
    pstInsList->u32Target = 0;
    pstInsList->u32InstanceNum = 0;

    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count ++)
    {
        pstInsList->pstInstPool[u32Count] = HI_NULL;
    }

}

HI_S32 VPSS_CTRL_RegistISR(VPSS_IP_E enIp)
{
    if (request_irq(g_stVpssCtrl[enIp].u32VpssIrqNum, g_stVpssCtrl[enIp].pVpssIntService,
                        IRQF_DISABLED, g_stVpssCtrl[enIp].isr_name, &(g_stVpssCtrl[enIp].hVpssIRQ)))
    {
        VPSS_FATAL("VPSS%d registe IRQ failed!\n",(HI_U32)enIp);
        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}

HI_S32 VPSS_CTRL_UnRegistISR(VPSS_IP_E enIp)
{
    free_irq(g_stVpssCtrl[enIp].u32VpssIrqNum, &(g_stVpssCtrl[enIp].hVpssIRQ));
    return HI_SUCCESS;
}

HI_U32 VPSS_CTRL_GetDistributeIP(VPSS_IP_E *penVpssIp)
{
    HI_U32 u32MinInstCount = VPSS_INSTANCE_MAX_NUMB;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        if((HI_TRUE == g_stVpssCtrl[i].bIPVaild) && (u32MinInstCount > g_stVpssCtrl[i].stInstCtrlInfo.u32InstanceNum))
        {
            u32MinInstCount = g_stVpssCtrl[i].stInstCtrlInfo.u32InstanceNum;
            *penVpssIp = i;
            s32Ret = HI_SUCCESS;
        }
    }

    return s32Ret;
}


HI_S32 VPSS_CTRL_SyncDistributeIP(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_IPMODE_E enIpmode)
{
    HI_U32 i;
    VPSS_IP_E enDstVpssIp;
    VPSS_IP_E enSrcVpssIp;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    unsigned long u32LockFlag;
    VPSS_INSTANCE_S *pstChgInst = HI_NULL;

    //step1:get instance real ip
    //step2:change instance to new ip
    //step2.1:if the new ip has instance ,change it together
    if(HI_DRV_VPSS_IPMODE_BUTT <= enIpmode)
    {
        VPSS_FATAL("Invalid Ipmode %d.",enIpmode);
        return HI_FAILURE;
    }

    if((HI_FALSE == g_stVpssCtrl[VPSS_IP_0].bIPVaild)
        || (HI_FALSE == g_stVpssCtrl[VPSS_IP_1].bIPVaild))
    {
        VPSS_ERROR("Can not change IP %d,single ctrl",enIpmode);
        return HI_FAILURE;
    }

    if(HI_DRV_VPSS_IPMODE_IP0 == enIpmode)
    {
        enDstVpssIp = VPSS_IP_0;
    }
    else
    {
        enDstVpssIp = VPSS_IP_1;
    }

    pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);
    if(enDstVpssIp == pstInstance->CtrlID)
    {
        return HI_SUCCESS;
    }

    enSrcVpssIp = pstInstance->CtrlID;

    //we need exchange instance to keep balance
    if(0 < pstInstCtrlInfo->u32InstanceNum)
	{
		pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);

		read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(HI_NULL != pstInstCtrlInfo->pstInstPool[i])
			{
				pstChgInst = pstInstCtrlInfo->pstInstPool[i];
				pstInstCtrlInfo->pstInstPool[i] = pstInstance;
				pstInstance->CtrlID = enDstVpssIp;
				break;
			}
		}

		read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		if (HI_NULL == pstChgInst)
		{
			VPSS_ERROR("Can't get DstInst\n");
			return HI_FAILURE;
		}

		pstInstCtrlInfo = &(g_stVpssCtrl[enSrcVpssIp].stInstCtrlInfo);

		read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(pstInstance == pstInstCtrlInfo->pstInstPool[i])
			{
				pstInstCtrlInfo->pstInstPool[i] = pstChgInst;
				pstChgInst->CtrlID = enSrcVpssIp;
				break;
			}
		}
		read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	}
    else
    {
        pstInstCtrlInfo = &(g_stVpssCtrl[enSrcVpssIp].stInstCtrlInfo);

        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
        {
            if(pstInstance == pstInstCtrlInfo->pstInstPool[i])
            {
                pstInstCtrlInfo->pstInstPool[i] = HI_NULL;
                pstInstCtrlInfo->u32InstanceNum--;
                break;
            }
        }

        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);

        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
        {
            if(HI_NULL == pstInstCtrlInfo->pstInstPool[i])
            {
                pstInstCtrlInfo->pstInstPool[i] = pstInstance;
                pstInstance->CtrlID = enDstVpssIp;
                pstInstCtrlInfo->u32InstanceNum++;
                break;
            }
        }

        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    }

    return HI_SUCCESS;
}

VPSS_INSTANCE_S *VPSS_CTRL_GetServiceInstance(VPSS_IP_E enIp)
{
    HI_S32 s32LockRet;
    HI_S32 s32CheckRet;
    HI_U32 u32CycleTime;
    HI_U32 u32CurPos;
    unsigned long  u32LockFlag;
    VPSS_INST_CTRL_S  *pstInstCtrlInfo;
    VPSS_INSTANCE_S *pstInstance;

    pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
    u32CycleTime = 0;
    u32CurPos = pstInstCtrlInfo->u32Target;

    while(u32CycleTime < VPSS_INSTANCE_MAX_NUMB)
    {
        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        pstInstance = pstInstCtrlInfo->pstInstPool[u32CurPos];
        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        if(pstInstance == HI_NULL)
        {
            u32CurPos = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
            u32CycleTime++;
            continue;
        }

        s32LockRet = HI_SUCCESS;//VPSS_OSAL_TryLock(&(pstInstance->stInstLock));
        //s32LockRet = VPSS_OSAL_TryLock(&(pstInstance->stInstLock));
        if (s32LockRet == HI_SUCCESS)
		{
			pstInstance->u32CheckCnt++;
			if (jiffies - pstInstance->u32LastCheckTime > HZ)
			{
				pstInstance->u32CheckRate = pstInstance->u32CheckCnt;
				pstInstance->u32CheckSucRate = pstInstance->u32CheckSucCnt;
				pstInstance->u32BufRate = pstInstance->u32BufCnt;
				pstInstance->u32BufSucRate = pstInstance->u32BufSucCnt;
				pstInstance->u32SrcRate = pstInstance->u32SrcCnt;
				pstInstance->u32SrcSucRate = pstInstance->u32SrcSucCnt;
				pstInstance->u32CheckCnt = 0;
				pstInstance->u32CheckSucCnt = 0;
				pstInstance->u32BufCnt = 0;
				pstInstance->u32BufSucCnt = 0;
				pstInstance->u32SrcCnt = 0;
				pstInstance->u32SrcSucCnt = 0;

				pstInstance->u32ImgRate
					= pstInstance->u32ImgCnt - pstInstance->u32ImgLastCnt;
				pstInstance->u32ImgSucRate
					= pstInstance->u32ImgSucCnt - pstInstance->u32ImgLastSucCnt;
				pstInstance->u32ImgLastCnt = pstInstance->u32ImgCnt;
				pstInstance->u32ImgLastSucCnt = pstInstance->u32ImgSucCnt;
				pstInstance->u32LastCheckTime = jiffies;
			}

			if (pstInstance->enState == INSTANCE_STATE_WORING)
			{
				(HI_VOID)VPSS_INST_SyncUsrCfg(pstInstance);

				VPSS_INST_SyncTvpCfg(pstInstance);

				s32CheckRet = VPSS_INST_CheckInstAvailable(pstInstance);
				if(s32CheckRet == HI_SUCCESS)
				{
					pstInstCtrlInfo->u32Target = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
					pstInstance->u32CheckSucCnt++;
					//VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
					return pstInstance;
				}
				else
				{
					//VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
				}
			}
		}

        u32CurPos = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
        u32CycleTime++;
    }

    return HI_NULL;
}

HI_S32 VPSS_CTRL_FixTask(VPSS_IP_E enIp, HI_DRV_BUF_ADDR_E enLR, VPSS_TASK_S *pstTask)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstCur;
    VPSS_FB_NODE_S *pstFrmNode = HI_NULL;
    VPSS_IN_INTF_S stInIntf;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->pstPqCfg = &pstInst->stPqRegData;

    s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

	if (HI_DRV_BUF_ADDR_MAX <= enLR)
	{
		VPSS_ERROR("Invalid Para enLR %d\n",enLR);
		return HI_FAILURE;
	}

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

    /* INInfo */
    s32Ret = stInIntf.pfnGetProcessImage(&pstInst->stInEntity, &pstCur);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
        return HI_FAILURE;
    }
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI))
#else
	if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field)
#endif
    {
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_PQ_WBC_INFO_S stVpssWbcInfo;
        HI_PQ_CFG_INFO_S stVpssCfgInfo;

        stVpssWbcInfo.u32HandleNo = pstInst->ID;
        stVpssWbcInfo.u32Width = pstCur->u32Width;
        stVpssWbcInfo.u32Height = pstCur->u32Height;
		stVpssWbcInfo.bProgressive = pstCur->bProgressive;
        stVpssWbcInfo.pstVPSSWbcReg = &(pstInst->stPqWbcReg);

        stVpssWbcInfo.s32FieldOrder = !pstCur->bTopFieldFirst;
        if(pstCur->enFieldMode == HI_DRV_FIELD_TOP)
        {
            stVpssWbcInfo.s32FieldMode = 0;
        }
        else if(pstCur->enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            stVpssWbcInfo.s32FieldMode = 1;
        }
        else
        {
            VPSS_ERROR("Dei Error.\n");
            stVpssWbcInfo.s32FieldMode = 0;
        }
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstCur->u32Priv;

        stVpssWbcInfo.u32FrameRate = pstCur->u32FrameRate;
        stVpssWbcInfo.bPreInfo = HI_FALSE;

        stVpssWbcInfo.stVdecInfo.IsProgressiveFrm = pstPriv->ePictureMode;
        stVpssWbcInfo.stVdecInfo.IsProgressiveSeq = pstPriv->eSampleType;
        stVpssWbcInfo.stVdecInfo.RealFrmRate = 2500;

        DRV_PQ_SetAlgCalcCfg(&stVpssWbcInfo, &stVpssCfgInfo);

        if (stVpssCfgInfo.s32FieldOrder != 2)
        {
            if(pstInst->stInEntity.stStreamInfo.u32RealTopFirst == HI_FALSE)
            {
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
                if(pstCur->enFieldMode == HI_DRV_FIELD_TOP
                    && pstCur->enBufValidMode != HI_DRV_FIELD_ALL)
#else
				if(pstCur->enFieldMode == HI_DRV_FIELD_TOP)
#endif
                {
                    HI_BOOL bTopFirst;
                    bTopFirst = !stVpssCfgInfo.s32FieldOrder;
                    stInIntf.pfnGetInfo(&pstInst->stInEntity,
                            VPSS_IN_INFO_CORRECT_FIELD,
                            HI_DRV_BUF_ADDR_MAX,
                            &bTopFirst);
                }
            }
            else if(pstInst->stInEntity.stStreamInfo.u32RealTopFirst == HI_TRUE)
            {
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
				if(pstCur->enFieldMode == HI_DRV_FIELD_BOTTOM
                    && pstCur->enBufValidMode != HI_DRV_FIELD_ALL)
#else
				if(pstCur->enFieldMode == HI_DRV_FIELD_BOTTOM)
#endif
                {
                    HI_BOOL bTopFirst;
                    bTopFirst = !stVpssCfgInfo.s32FieldOrder;
                    stInIntf.pfnGetInfo(&pstInst->stInEntity,
                            VPSS_IN_INFO_CORRECT_FIELD,
                            HI_DRV_BUF_ADDR_MAX,
                            &bTopFirst);
                }
            }
            else
            {

            }
        }
    }
	else
	{
        HI_PQ_WBC_INFO_S stVpssWbcInfo;
        HI_PQ_CFG_INFO_S stVpssCfgInfo;

		memset(&stVpssWbcInfo,0,sizeof(HI_PQ_WBC_INFO_S));
		memset(&stVpssCfgInfo,0,sizeof(HI_PQ_CFG_INFO_S));

        stVpssWbcInfo.u32HandleNo = pstInst->ID;
        stVpssWbcInfo.u32Width = pstCur->u32Width;
        stVpssWbcInfo.u32Height = pstCur->u32Height;
		stVpssWbcInfo.bProgressive = pstCur->bProgressive;
        stVpssWbcInfo.pstVPSSWbcReg = &(pstInst->stPqWbcReg);

        DRV_PQ_SetAlgCalcCfg(&stVpssWbcInfo, &stVpssCfgInfo);
	}


    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_R
        || pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L)
    {
        VPSS_INST_SetHalFrameInfo(pstCur,
                                  &pstHalInfo->stInInfo,
                                  HI_DRV_BUF_ADDR_LEFT);
    }
    else
    {
        VPSS_INST_SetHalFrameInfo(pstCur,
                                  &pstHalInfo->stInInfo,
                                  enLR);

        pstHalInfo->stInInfo.u32TunnelAddr = pstCur->u32TunnelPhyAddr;
    }

    if(pstHalInfo->stInInfo.u32Width <= 1920)
    {
        VPSS_RWZB_IMG_S stRwzbImage;
        #if 1
        /*RWZB*/
        if(pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM
           || pstHalInfo->stInInfo.bProgressive == HI_TRUE)
        {
            VPSS_RWZB_GetRwzbData(&(pstInst->stRwzbInfo), &pstHalInfo->stRwzbInfo);
        }

        stRwzbImage.bProgressive = pstHalInfo->stInInfo.bProgressive;
        stRwzbImage.enFieldMode = pstHalInfo->stInInfo.enFieldMode;
        stRwzbImage.u32Height = pstHalInfo->stInInfo.u32Height;
        stRwzbImage.u32Width = pstHalInfo->stInInfo.u32Width;

        VPSS_RWZB_GetRwzbInfo(&(pstInst->stRwzbInfo), &pstHalInfo->stRwzbInfo, &stRwzbImage);

        #endif
    }

	/*****************************DEI or TNR cfg*******************************/
	VPSS_CHECK_NULL(stInIntf.pfnSetInfo);

	stInIntf.pfnSetInfo(&(pstInst->stInEntity), VPSS_SET_INFO_INPUT_CFG, enLR, pstHalInfo);

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L
            || pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_R)
        {
            pstFrmNode = pstTask->pstFrmNode[i*2];
        }
        else
        {
            pstFrmNode = pstTask->pstFrmNode[i*2 + enLR];
        }

#ifdef ZME_2L_TEST
		if( HI_TRUE == pstTask->pstInstance->stPort[i].bNeedRotate)
		{
			pstFrmNode = pstTask->pstFrmNodeRoBuf[i];
		}
		else if( HI_TRUE == pstTask->pstInstance->stPort[i].bNeedZME2L)
		{
			pstFrmNode = pstTask->pstFrmNodeZME1L;
		}
#endif
        if (pstFrmNode!= HI_NULL)
		{
			pstHalInfo->astPortInfo[i].bEnable = HI_TRUE;
			pstHalInfo->astPortInfo[i].bConfig = HI_FALSE;
			pstHalInfo->astPortInfo[i].bCmpLoss = VPSS_ISLOSS_CMP;//@f00241306 for cmp

			if ((pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
					&& (pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV21_CMP
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV21
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV61_2X1))
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_TRUE;
			}
			else if((pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP)
					&& (pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV12
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV12_CMP
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV16_2X1))
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_TRUE;
			}
			else
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_FALSE;
			}

#ifdef ZME_2L_TEST
			VPSS_INST_SetOutFrameInfo(pstInst, i,
					&pstFrmNode->stBuffer, &pstFrmNode->stOutFrame, enLR);
#else
			//如果开启了旋转，使用旋转BUFFER
			if ((HI_DRV_VPSS_ROTATION_90 == pstInst->stPort[i].enRotation)
					|| (HI_DRV_VPSS_ROTATION_270 == pstInst->stPort[i].enRotation))
			{
				VPSS_INST_SetOutFrameInfo(pstInst, i,
						&g_stVpssCtrl[enIp].stRoBuf[i], &pstFrmNode->stOutFrame, enLR);
			}
			else
			{
				VPSS_INST_SetOutFrameInfo(pstInst, i,
						&pstFrmNode->stBuffer, &pstFrmNode->stOutFrame, enLR);
			}
#endif

			VPSS_INST_SetHalFrameInfo(&pstFrmNode->stOutFrame,
					&pstHalInfo->astPortInfo[i].stOutInfo, enLR);

			VPSS_INST_GetInCrop(pstInst, i, &pstHalInfo->astPortInfo[i].stInCropRect);

			if (   pstHalInfo->stInInfo.u32Width == 704
					&& (pstHalInfo->stInInfo.u32Height == 576 || pstHalInfo->stInInfo.u32Height == 480)
					&& (pstHalInfo->stInInfo.u32Height == pstHalInfo->astPortInfo[i].stOutInfo.u32Height)
					&& pstHalInfo->astPortInfo[i].stOutInfo.u32Width == 720
					&& pstInst->stPort[i].eAspMode == HI_DRV_ASP_RAT_MODE_FULL)
			{
				pstHalInfo->astPortInfo[i].stVideoRect.s32Width =
					pstHalInfo->stInInfo.u32Width;
				pstHalInfo->astPortInfo[i].stVideoRect.s32Height =
					pstHalInfo->stInInfo.u32Height;
				pstHalInfo->astPortInfo[i].stVideoRect.s32X = 8;
				pstHalInfo->astPortInfo[i].stVideoRect.s32Y = 0;
				pstHalInfo->astPortInfo[i].stOutCropRect.s32X = 0;
				pstHalInfo->astPortInfo[i].stOutCropRect.s32Y = 0;
				pstHalInfo->astPortInfo[i].stOutCropRect.s32Width =
					pstHalInfo->stInInfo.u32Width;
				pstHalInfo->astPortInfo[i].stOutCropRect.s32Height =
					pstHalInfo->stInInfo.u32Height;
			}
			else
			{
				VPSS_INST_GetVideoRect(pstInst, i, &pstHalInfo->astPortInfo[i].stInCropRect,
						&pstHalInfo->astPortInfo[i].stVideoRect,
						&pstHalInfo->astPortInfo[i].stOutCropRect);
				VPSS_DBG_INFO("Incrop W H %d %d\n", 
							pstHalInfo->astPortInfo[i].stInCropRect.s32Width,
							pstHalInfo->astPortInfo[i].stInCropRect.s32Height);
				VPSS_DBG_INFO("VRect W H %d %d\n", 
							pstHalInfo->astPortInfo[i].stVideoRect.s32Width,
							pstHalInfo->astPortInfo[i].stVideoRect.s32Height);
				VPSS_DBG_INFO("ORect W H %d %d\n", 
							pstHalInfo->astPortInfo[i].stOutCropRect.s32Width,
							pstHalInfo->astPortInfo[i].stOutCropRect.s32Height);				
			}

			memcpy(&(pstFrmNode->stOutFrame.stLbxInfo),
					&(pstHalInfo->astPortInfo[i].stVideoRect),
					sizeof(HI_RECT_S));

			VPSS_INST_GetRotate(pstInst,i,&pstHalInfo->astPortInfo[i],pstCur);
		}
        else
        {
            pstHalInfo->astPortInfo[i].bEnable = HI_FALSE;
        }
    }

    pstHalInfo->bOutLowDelay = pstTask->bOutLowDelay;
    return HI_SUCCESS;

}

HI_BOOL VPSS_CTRL_CheckVirtualStart(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_BOOL bStart = HI_FALSE;

	HI_U32 u32Cnt;

    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
	VPSS_HAL_PORT_INFO_S *pstPortInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

	for(u32Cnt = 0 ; u32Cnt < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Cnt ++)
	{
		pstPortInfo = &(pstHalInfo->astPortInfo[u32Cnt]);

		if (pstPortInfo->bEnable == HI_TRUE
			&& pstPortInfo->bConfig == HI_FALSE)
		{
			bStart = HI_TRUE;
		}
	}

	return bStart;
}
HI_S32 VPSS_CTRL_Start2DTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->enNodeType = VPSS_INST_Check2DNodeType(pstInst);

    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
    {
        pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_FIELD] = HI_TRUE;
        VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL]
				= HI_TRUE;
			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL);
		}
    }
    else if(pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FRAME
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_HIGH_SPEED
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD)
    {
		pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D] = HI_TRUE;

		VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_VIRTUAL]
				= HI_TRUE;
			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_VIRTUAL);
		}
    }
	else if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L)
	{
		pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_SPLIT_L] = HI_TRUE;

		VPSS_HAL_SetNodeInfo(enIp,pstHalInfo,VPSS_HAL_TASK_NODE_SPLIT_L);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_FRAME;

			pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_VIRTUAL]
				= HI_TRUE;

			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_VIRTUAL);
		}
	}
	else
	{
		VPSS_ERROR("Invalid Node Type %d\n",pstHalInfo->enNodeType);
	}

    return HI_SUCCESS;
}

HI_BOOL VPSS_CTRL_Check2DStart(VPSS_TASK_S *pstTask)
{
    HI_U32 i;

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if (pstTask->pstFrmNode[i*2] != HI_NULL)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_BOOL VPSS_CTRL_CheckRotateStart(VPSS_TASK_S *pstTask, HI_U32 PortId)
{
	VPSS_PORT_S *pstPort;
	pstPort = &(pstTask->pstInstance->stPort[PortId]);

    if((HI_DRV_VPSS_ROTATION_DISABLE == pstPort->enRotation)
        ||(HI_DRV_VPSS_ROTATION_180 == pstPort->enRotation)
        || (HI_FALSE == pstPort->bEnble))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL VPSS_CTRL_CheckZME1LStart(VPSS_TASK_S *pstTask, HI_U32 PortId)
{
	VPSS_PORT_S *pstPort;
	pstPort = &(pstTask->pstInstance->stPort[PortId]);

    if(( HI_TRUE == pstTask->bZME2lTaskNeed[PortId]) && (HI_TRUE == pstPort->bEnble))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

#ifdef ZME_2L_TEST
/******************************************************************************
brief      : CNcomment :  check rotate and ZME_2L task
		   :  本函数功能:
			  1  根据输入图像信息(解码图像)检测与判定是否需要做二级缩放及旋转操作。并记录每个port相关标志。
			  2  如果需要做二级缩放(旋转)，则分配所需要的缓存图像buffer。
attention  :
param[in]  : enIp      :  vpss ID
			 pstTask   :
retval     : HI_S32
see        :
author	   :f00241306
******************************************************************************/

HI_U32 VPSS_CTRL_Zme2lAndRotateCfg(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_U32	i = 0, u32DstW = 0, u32DstH =0 , u32RotateNeed = 0;

	HI_S32 s32Ret				 = HI_FAILURE;
    VPSS_INSTANCE_S     *pstInst = HI_NULL;
	HI_DRV_VIDEO_FRAME_S *pstCur = HI_NULL;
    VPSS_IN_INTF_S stInIntf;

    HI_U32 u32BufSize			 = 0;
    HI_U32 u32BufStride			 = 0;
	HI_DRV_VIDEO_FRAME_S *pstFrm = HI_NULL;
	VPSS_BUFFER_S        *pstBuf = HI_NULL;

	VPSS_PORT_S			 *pstPort[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER] = {HI_NULL, HI_NULL, HI_NULL};
	HI_PQ_ZME_WIN_S      stZmeWin;

    pstInst = pstTask->pstInstance;
	memset(pstTask->bZME2lTaskNeed, 0, sizeof(HI_BOOL)*DEF_HI_DRV_VPSS_PORT_MAX_NUMBER);

	s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);
    s32Ret = stInIntf.pfnGetProcessImage(&pstInst->stInEntity, &pstCur);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
        return HI_FAILURE;
    }

	for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
		pstPort[i]	= &pstInst->stPort[i];

		if( HI_TRUE == pstPort[i]->bEnble && VPSS_INVALID_HANDLE != pstPort[i]->s32PortId )
		{
			 /*	 如果需要做旋转操作，则记录当前port旋转标志 并 申请相应的缓存buffer*/
			if (VPSS_CTRL_CheckRotateStart(pstTask, i))
			{
				pstPort[i]->bNeedRotate = HI_TRUE;
				u32RotateNeed++;
				if ( HI_NULL == pstTask->pstFrmNodeRoBuf[i])  //只分配一次缓存帧存
				{
					pstTask->pstFrmNodeRoBuf[i] = (VPSS_FB_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_FB_NODE_S));
				}

				if ( HI_NULL != pstTask->pstFrmNodeRoBuf[i])
				{
					pstFrm = &pstTask->pstFrmNodeRoBuf[i]->stOutFrame;
					pstBuf = &pstTask->pstFrmNodeRoBuf[i]->stBuffer;

					if (pstPort[i]->s32OutputWidth != 0 && pstPort[i]->s32OutputHeight != 0)
					{
						pstFrm->u32Width  = pstPort[i]->s32OutputHeight;
						pstFrm->u32Height = pstPort[i]->s32OutputWidth;
					}
					else
					{
						pstFrm->u32Width = pstCur->u32Height;
						pstFrm->u32Height = pstCur->u32Width;
					}

					VPSS_OSAL_CalBufSize(&u32BufSize,
								&u32BufStride,
								pstFrm->u32Height,
								pstFrm->u32Width,
								pstPort[i]->eFormat,
								pstPort[i]->enOutBitWidth);

					if ((pstBuf->stMemBuf.u32Size != u32BufSize)
						|| (pstBuf->u32Stride != u32BufStride))
					{
						if (pstBuf->stMemBuf.u32Size != 0)
						{
							VPSS_OSAL_FreeMem(&(pstBuf->stMemBuf));
						}

						if (!pstInst->bSecure)
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,u32BufSize,
							"VPSS_RoBuf",
							"VPSS",
							&(pstBuf->stMemBuf));
						}
						else
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,u32BufSize,
							"VPSS_RoBuf",
							"VPSS",
							&(pstBuf->stMemBuf));
						}
						if (s32Ret != HI_SUCCESS)
						{
							VPSS_FATAL("Alloc RoBuf Failed\n");
						}

						pstBuf->u32Stride = u32BufStride;
					}
				}
			}

            else
            {
				if (HI_NULL != pstTask->pstFrmNodeRoBuf[i])
				{
					//释放旋转内存，节省SDK空间占用
					if(0 != pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf.u32Size)
					{
						VPSS_OSAL_FreeMem(&(pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf));
						pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf.u32Size = 0;
						pstTask->pstFrmNodeRoBuf[i]->stBuffer.u32Stride = 0;
					}

					VPSS_VFREE(pstTask->pstFrmNodeRoBuf[i]);
					pstTask->pstFrmNodeRoBuf[i] = HI_NULL;
				}
            }
		}
	}

	if( u32RotateNeed > 0)  //如果有旋转任务，则不再做二级缩放
	{
		return HI_SUCCESS;
	}

	stZmeWin.stPort0Win.u32Width = pstPort[0]->s32OutputWidth;
	stZmeWin.stPort0Win.u32Height= pstPort[0]->s32OutputHeight;

	stZmeWin.stPort1Win.u32Width = pstPort[1]->s32OutputWidth;
	stZmeWin.stPort1Win.u32Height= pstPort[1]->s32OutputHeight;

	if(  0 != u32DstW && 0 != u32DstH)  //只分配一次缓存帧存

	{
		if (HI_NULL == pstTask->pstFrmNodeZME1L) //只分配一次缓存帧存
		{
			pstTask->pstFrmNodeZME1L = (VPSS_FB_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_FB_NODE_S));
		}

		if ( HI_NULL != pstTask->pstFrmNodeZME1L)
		{
			pstFrm = &pstTask->pstFrmNodeZME1L->stOutFrame;
			pstBuf = &pstTask->pstFrmNodeZME1L->stBuffer;

			pstFrm->u32Width	= u32DstW;
			pstFrm->u32Height	= u32DstH;

			VPSS_OSAL_CalBufSize(&u32BufSize,
								&u32BufStride,
								pstFrm->u32Height,
								pstFrm->u32Width,
								HI_DRV_PIX_FMT_NV21,
								HI_DRV_PIXEL_BITWIDTH_10BIT);

			if ((pstBuf->stMemBuf.u32Size != u32BufSize)
				|| (pstBuf->u32Stride != u32BufStride))
			{
				if (pstBuf->stMemBuf.u32Size != 0)
				{
					VPSS_OSAL_FreeMem(&(pstBuf->stMemBuf));
				}
                if (!pstInst->bSecure)
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,u32BufSize,
							"VPSS_ZME1LBuf",
							"VPSS",
							&(pstBuf->stMemBuf));
						}
						else
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,u32BufSize,
							"VPSS_ZME1LBuf",
							"VPSS",
							&(pstBuf->stMemBuf));
						}
				if (s32Ret != HI_SUCCESS)
				{
					VPSS_FATAL("Alloc ZME1LBuf Failed\n");
				}

				pstBuf->u32Stride = u32BufStride;
			}
		}
	}
    else
    {
		if ( HI_NULL != pstTask->pstFrmNodeZME1L)
		{
			//释放二级缩放内存，节省SDK空间占用
			if(0 != pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size)
			{
				VPSS_OSAL_FreeMem(&(pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf));
				pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size = 0;
				pstTask->pstFrmNodeZME1L->stBuffer.u32Stride = 0;

				VPSS_VFREE(pstTask->pstFrmNodeZME1L);
				pstTask->pstFrmNodeZME1L = HI_NULL;
			}
		}
    }

	return s32Ret;
}


HI_S32 VPSS_CTRL_StartZME2LTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask, HI_U32 PortId)
{
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstInputFrame;
    VPSS_FB_NODE_S *pstOutputInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    //set ZME_2L input frame info
    pstInputFrame = &(pstTask->pstFrmNodeZME1L->stOutFrame);
    VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);

    //set ZME_2L output frame info
    pstOutputInfo = pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT];
	VPSS_INST_SetOutFrameInfo(pstInst, PortId, &pstOutputInfo->stBuffer, &pstOutputInfo->stOutFrame, HI_DRV_BUF_ADDR_LEFT);
    VPSS_INST_SetHalFrameInfo(&pstOutputInfo->stOutFrame, &pstHalInfo->astPortInfo[PortId].stOutInfo, HI_DRV_BUF_ADDR_LEFT);

	VPSS_INST_GetInCrop(pstInst, PortId, &pstHalInfo->astPortInfo[PortId].stInCropRect);
    if (   pstHalInfo->stInInfo.u32Width == 704
        && (pstHalInfo->stInInfo.u32Height == 576 || pstHalInfo->stInInfo.u32Height == 480)
        && (pstHalInfo->stInInfo.u32Height == pstHalInfo->astPortInfo[PortId].stOutInfo.u32Height)
        && pstHalInfo->astPortInfo[PortId].stOutInfo.u32Width == 720)
    {
        pstHalInfo->astPortInfo[PortId].stVideoRect.s32Width =
                            pstHalInfo->stInInfo.u32Width;
        pstHalInfo->astPortInfo[PortId].stVideoRect.s32Height =
                            pstHalInfo->stInInfo.u32Height;
        pstHalInfo->astPortInfo[PortId].stVideoRect.s32X = 8;
        pstHalInfo->astPortInfo[PortId].stVideoRect.s32Y = 0;
    }
    else
    {
        VPSS_INST_GetVideoRect(pstInst, PortId, &pstHalInfo->astPortInfo[PortId].stInCropRect,
            &pstHalInfo->astPortInfo[PortId].stVideoRect,
            &pstHalInfo->astPortInfo[PortId].stOutCropRect);
    }

	pstHalInfo->enNodeType = VPSS_HAL_NODE_ZME_2L;
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_ZME_L2 + PortId] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_ZME_L2 + PortId );
	return HI_SUCCESS;
}
#endif

HI_S32 VPSS_CTRL_StartSplitTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->enNodeType = VPSS_HAL_NODE_UHD_SPLIT_R;

    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_SPLIT_R] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_SPLIT_R);

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Start3DTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->enNodeType = VPSS_INST_Check3DNodeType(pstInst);

    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_RIGHT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_3D_R] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_3D_R);

    return HI_SUCCESS;
}

HI_BOOL VPSS_CTRL_Check3DStart(VPSS_TASK_S *pstTask)
{
    HI_U32 i;

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if (pstTask->pstFrmNode[i*2 + 1] != HI_NULL)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_BOOL VPSS_CTRL_CheckUHDSplitStart(VPSS_TASK_S *pstTask)
{
    HI_BOOL bSplitStart = HI_FALSE;
    VPSS_IN_INTF_S stInIntf = {0};
    HI_DRV_VIDEO_FRAME_S *pstImage;

	VPSS_CHECK_NULL(pstTask->pstInstance);

    (HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

    (HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
                        &pstImage);

	if (pstImage->u32Width > 1920)
	{
		bSplitStart = HI_TRUE;
	}
	else
	{
		bSplitStart = HI_FALSE;
	}

    return bSplitStart;
}


HI_S32 VPSS_CTRL_StartRotateTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask, HI_U32 PortId)
{
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstInputFrame;
    VPSS_FB_NODE_S *pstOutputInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

#ifdef ZME_2L_TEST
	//set rotation input frame info
	pstInputFrame = &(pstTask->pstFrmNodeRoBuf[PortId]->stOutFrame);
	VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);
#else
    //set rotation input frame info
    pstInputFrame = &(pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT]->stOutFrame);
    VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);

#endif

	//set rotation output frame info
    pstOutputInfo = pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT];
    VPSS_INST_SetRotationOutFrameInfo(pstInst, PortId,
                    &pstOutputInfo->stBuffer, &pstOutputInfo->stOutFrame, HI_DRV_BUF_ADDR_LEFT);

	if ((HI_DRV_VPSS_ROTATION_90 == pstInst->stPort[PortId].enRotation)
			|| (HI_DRV_VPSS_ROTATION_270 == pstInst->stPort[PortId].enRotation))
	{
		pstOutputInfo->stOutFrame.stLbxInfo.s32X =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Y;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Y =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32X;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Width =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Height;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Height =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Width;
	}
	else
	{
		memcpy(&(pstOutputInfo->stOutFrame.stLbxInfo),
				&(pstHalInfo->astPortInfo[PortId].stVideoRect),
				sizeof(HI_RECT_S));
	}

    VPSS_INST_SetHalFrameInfo(&pstOutputInfo->stOutFrame,
        &pstHalInfo->astPortInfo[PortId].stOutInfo, HI_DRV_BUF_ADDR_LEFT);

    //rotation Y
    pstHalInfo->enNodeType = VPSS_HAL_NODE_ROTATION_Y;
    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_RO_Y + PortId * 2] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_RO_Y + PortId * 2);

    //rotation C
    pstHalInfo->enNodeType = VPSS_HAL_NODE_ROTATION_C;
    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_RO_C + PortId * 2] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_RO_C + PortId * 2);

    return HI_SUCCESS;
}
HI_S32 VPSS_CTRL_GetOutBufferRect(HI_RECT_S stOriRect,HI_RECT_S *pstRevisedRect)
{
	HI_U32 u32OriW = 0,u32OriH = 0;
	HI_U32 u32DstW = 0,u32DstH = 0;
	HI_U32 u32RatioW;
	HI_U32 u32RatioH;
	HI_U32 u32TmpH;
	HI_U32 u32TmpW;
	HI_U32 u32WidthLevel;
	HI_U32 u32HeightLevel;

	u32WidthLevel = 3840;
	u32HeightLevel = 2160;

	u32OriW = stOriRect.s32Width;
	u32OriH = stOriRect.s32Height;

	if (u32OriW < u32WidthLevel && u32OriH < u32HeightLevel)
	{
		u32DstW = u32OriW;
		u32DstH = u32OriH;
	}
	else if (u32OriW >= u32WidthLevel && u32OriH >= u32HeightLevel)
	{
		u32DstW = u32WidthLevel;
		u32DstH = u32HeightLevel;
	}
	else
	{
		u32RatioW = u32OriW*2048/u32WidthLevel;
		u32RatioH = u32OriH*2048/u32HeightLevel;

		if (u32RatioW > u32RatioH)
		{
			u32TmpW = u32OriW*2048/u32RatioW;
			u32TmpH = u32OriH*2048/u32RatioW;
		}
		else
		{
			u32TmpW = u32OriW*2048/u32RatioH;
			u32TmpH = u32OriH*2048/u32RatioH;
		}

		u32TmpW = u32TmpW & 0xfffffffe;
		u32TmpH = u32TmpH & 0xfffffffc;

		u32DstW = u32TmpW;
		u32DstH = u32TmpH;
	}

	pstRevisedRect->s32Width = u32DstW;
	pstRevisedRect->s32Height = u32DstH;

	return HI_SUCCESS;
}

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
HI_BOOL VPSS_CTRL_CheckLowDelay(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	return HI_FALSE;
}
#else
HI_BOOL VPSS_CTRL_CheckLowDelay(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_BOOL bLowDelayTask = HI_FALSE;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_IN_INTF_S stInIntf = {0};
    HI_DRV_VIDEO_FRAME_S *pstImage;  
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    HI_U32 u32EnablePort = 0;
    HI_BOOL bRotation = HI_FALSE;
    
    if (pstTask->pstInstance)
    {	
		pstInstance = pstTask->pstInstance;
        
        (HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

        VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);
    
        (HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
                            &pstImage);

        
        for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
        {
            pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
            if (pstPort->bEnble)
            {
                u32EnablePort++;

                if (pstPort->enRotation != HI_DRV_VPSS_ROTATION_DISABLE)
                {
                    bRotation = HI_TRUE;
                }

                if (pstPort->bTunnelEnable)
                {
                    bLowDelayTask = HI_TRUE;
                }
            }
        }

        if (pstImage->u32Width > 1920)
        {
            bLowDelayTask = HI_FALSE;
        }
        
        if (pstImage->bSecure)
        {
            bLowDelayTask = HI_FALSE;
        }
        
        if (pstImage->eFrmType != HI_DRV_FT_NOT_STEREO)
        {
            bLowDelayTask = HI_FALSE;
        }
        
        if (u32EnablePort > 1 || bRotation == HI_TRUE)
        {
            bLowDelayTask = HI_FALSE;
        }
    }
    else
    {
        bLowDelayTask = HI_FALSE;
    }

    return bLowDelayTask;
}
#endif

HI_S32 VPSS_CTRL_CreateTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32StoreH;
    HI_U32 u32StoreW;
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    HI_DRV_VPSS_BUFLIST_CFG_S*  pstBufListCfg;
    VPSS_IN_INTF_S stInIntf = {0};
    HI_DRV_VIDEO_FRAME_S *pstImage;
	HI_DRV_VIDEO_PRIVATE_S *pstPriv;

    /*
        Traversal instance list to find a Available inst
        available means two requirement:
        1.one undo image
        2.at least two writting space
     */
    pstTask->pstInstance = VPSS_CTRL_GetServiceInstance(enIp);

    if (HI_NULL == pstTask->pstInstance)
    {
        return HI_FAILURE;
    }

    /*
        get the image info to  inform user out buffer size
    */
    (HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

    (HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
                        &pstImage);

    /***********config out buffer************************/
    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstBufListCfg = &(pstPort->stFrmInfo.stBufListCfg);

        if (pstPort->s32PortId != VPSS_INVALID_HANDLE &&
            pstPort->bEnble == HI_TRUE)
        {
            if (pstPort->s32OutputHeight == 0 && pstPort->s32OutputWidth == 0)
            {
                HI_RECT_S stInRect;
                VPSS_INST_GetInCrop(pstTask->pstInstance, u32Count, &stInRect);
                u32StoreW = (HI_U32)stInRect.s32Width;
                u32StoreH = (HI_U32)stInRect.s32Height;

                //u32StoreH = pstStreamInfo->u32StreamH;
                //u32StoreW = pstStreamInfo->u32StreamW;
            }
            else
			{
				if(pstBufListCfg->eBufType != HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
				{
					u32StoreH = pstPort->s32OutputHeight;
					u32StoreW = pstPort->s32OutputWidth;
				}
				else
				{
					HI_RECT_S stInRect;
					HI_RECT_S stBufferRect;
					VPSS_INST_GetInCrop(pstTask->pstInstance, u32Count, &stInRect);

					VPSS_CTRL_GetOutBufferRect(stInRect,&stBufferRect);

					u32StoreW = (HI_U32)stBufferRect.s32Width;
					u32StoreH = (HI_U32)stBufferRect.s32Height;
				}
			}

			pstPriv = (HI_DRV_VIDEO_PRIVATE_S*) & (pstImage->u32Priv[0]);
			if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstPort->bCurDropped = HI_FALSE;
			}
			else
			{
				pstPort->bCurDropped = VPSS_INST_CheckIsDropped(pstTask->pstInstance,
						pstPort->u32MaxFrameRate,
						pstPort->u32OutCount);
			}
			if (pstPort->bCurDropped == HI_FALSE)
			{
				/*2D image -> 1 outFrame 1 buffer*/
				if (pstImage->eFrmType == HI_DRV_FT_NOT_STEREO
						|| pstPort->b3Dsupport == HI_FALSE)
				{
					pstTask->pstFrmNode[u32Count * 2] =
						VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
								u32StoreH, u32StoreW,
								pstPort->eFormat,
								pstPort->enOutBitWidth,
								pstTask->pstInstance->bSecure);
					pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;

				}
				/*3D image -> 1 outFrame 2 buffer*/
				else
				{
					pstTask->pstFrmNode[u32Count * 2] =
						VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
								u32StoreH, u32StoreW,
								pstPort->eFormat,
								pstPort->enOutBitWidth,
								pstTask->pstInstance->bSecure);
					pstTask->pstFrmNode[u32Count * 2 + 1] =
						VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
								u32StoreH, u32StoreW,
								pstPort->eFormat,
								pstPort->enOutBitWidth,
								pstTask->pstInstance->bSecure);
					if (pstTask->pstFrmNode[u32Count * 2] == HI_NULL
							|| pstTask->pstFrmNode[u32Count * 2 + 1] == HI_NULL)
					{

						if (pstTask->pstFrmNode[u32Count * 2] != HI_NULL)
						{
							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
									pstTask->pstFrmNode[u32Count * 2],
									VPSS_FB_TYPE_NORMAL);
						}

						if (pstTask->pstFrmNode[u32Count * 2 + 1] != HI_NULL)
						{
							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
									pstTask->pstFrmNode[u32Count * 2 + 1],
									VPSS_FB_TYPE_NORMAL);
						}
						pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
						pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
					}
				}


#if 1
				if(pstBufListCfg->eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
				{
					/*************************/
					if(pstPort->eFormat == HI_DRV_PIX_FMT_NV12
							|| pstPort->eFormat == HI_DRV_PIX_FMT_NV21)
					{
						pstBufListCfg->u32BufStride = (u32StoreW + 0xf) & 0xfffffff0 ;
						pstBufListCfg->u32BufSize =
							pstBufListCfg->u32BufStride * u32StoreH * 3 / 2;
					}
					else if(pstPort->eFormat == HI_DRV_PIX_FMT_NV16_2X1
							|| pstPort->eFormat == HI_DRV_PIX_FMT_NV61_2X1)
					{
						pstBufListCfg->u32BufStride =  (u32StoreW + 0xf) & 0xfffffff0 ;
						pstBufListCfg->u32BufSize =
							pstBufListCfg->u32BufStride * u32StoreH * 2;
					}
					else
					{
						VPSS_FATAL("Port %x OutFormat%d isn't supported.\n",
								pstPort->s32PortId, pstPort->eFormat);
					}
					/*************************/
					if(pstTask->pstFrmNode[u32Count*2] != HI_NULL)
					{
						s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
								&(pstTask->pstFrmNode[u32Count*2]->stBuffer),u32StoreH,u32StoreW);
						if (s32Ret != HI_SUCCESS)
						{
							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
									pstTask->pstFrmNode[u32Count*2],
									VPSS_FB_TYPE_NORMAL);
							if (pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
							{
								VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
										pstTask->pstFrmNode[u32Count*2+1],
										VPSS_FB_TYPE_NORMAL);
							}
							VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
							return HI_FAILURE;
						}
					}

					if(pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
					{
						s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
								&(pstTask->pstFrmNode[u32Count*2+1]->stBuffer),u32StoreH,u32StoreW);
						if(s32Ret != HI_SUCCESS)
						{
							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
									pstTask->pstFrmNode[u32Count*2],
									VPSS_FB_TYPE_NORMAL);
							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
									pstTask->pstFrmNode[u32Count*2+1],
									VPSS_FB_TYPE_NORMAL);
							VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
							return HI_FAILURE;
						}
					}
				}
				else
				{

				}
#endif
			}
        else
        {
            pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
            pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
        }
    }
        else
        {
            pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
            pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
        }
    }

    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER * 2; u32Count++)
    {
        if (pstTask->pstFrmNode[u32Count] != HI_NULL)
        {
            s32Ret = HI_SUCCESS;
            break;
        }
    }

#if 0
	for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);

		if (pstPort->bEnble == HI_TRUE
				&& pstPort->bCurDropped == HI_FALSE
				&& pstTask->pstFrmNode[u32Count*2] == HI_NULL)
		{
            s32Ret = HI_FAILURE;
            break;
		}
	}
#endif

	if (s32Ret == HI_SUCCESS)
	{
		for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
		{
			pstPort = &((pstTask->pstInstance)->stPort[u32Count]);

			if (pstPort->bEnble == HI_TRUE)
			{
				pstPort->u32OutCount++;
			}
		}
	}
    return s32Ret;

}

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
HI_BOOL VPSS_CTRL_Check2DH265InterlaceStart(VPSS_TASK_S *pstTask)
{
	VPSS_IN_INTF_S stInIntf = {0};
	HI_DRV_VIDEO_FRAME_S *pstImage;
	HI_DRV_VIDEO_PRIVATE_S *pPriInfo;
	HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv; 

	VPSS_CHECK_NULL(pstTask->pstInstance);

	(HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

	VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

	(HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
						&pstImage);

	pPriInfo = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
	pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pPriInfo->u32Reserve[0]);

	if ((HI_UNF_VCODEC_TYPE_HEVC == pstVdecPriv->entype)
			&& (!pstImage->bProgressive)
			&& pstImage->enBufValidMode == HI_DRV_FIELD_ALL)
	{
		VPSS_DBG_INFO("Start detile task...\n");
		return HI_TRUE;
	}
	else
	{
        VPSS_DBG_INFO("Type:%d P:%d BufValidMode:%d\n", 
                    pstVdecPriv->entype, 
                    pstImage->bProgressive,
                    pstImage->enBufValidMode);

		VPSS_HAL_RefListDeInit(&pstTask->pstInstance->stRefList);
		return HI_FALSE;
	}

}
HI_S32 VPSS_CTRL_Start2DTask_H265(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_S32 s32Ret = HI_FAILURE;
	VPSS_HAL_INFO_S *pstHalInfo;
	VPSS_INSTANCE_S* pstInst;


	pstHalInfo = &pstTask->stVpssHalInfo;
	pstInst = pstTask->pstInstance;

	//printk("pstInst->stInEntity.stStreamInfo.u32IsNewImage: %d\n", pstInst->stInEntity.stStreamInfo.u32IsNewImage);

	pstHalInfo->pstRefList = &pstInst->stRefList;
	if ((pstInst->stInEntity.stStreamInfo.u32IsNewImage) || (!pstHalInfo->pstRefList->bRefListValid))
	{
		pstHalInfo->pstRefList = &pstInst->stRefList;
		VPSS_HAL_RefListInit(pstHalInfo->pstRefList, 
								pstInst->stInEntity.stStreamInfo.u32StreamW,
									pstInst->stInEntity.stStreamInfo.u32StreamH,
										pstInst->stPort[0].eFormat,
										pstInst->stInEntity.bSecure);
	}

	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE;
	s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
	HI_ASSERT(s32Ret != HI_FAILURE);
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_H265_INTERLACE] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_H265_INTERLACE);

	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_H265_STEP2_DEI;
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_H265_DEI] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_H265_DEI);

	return HI_SUCCESS;
}
#endif
HI_S32 VPSS_CTRL_StartTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 i;
    HI_BOOL bStart2D = HI_TRUE;
    HI_BOOL bStart3D = HI_FALSE;
#if DEF_VPSS_HIGH_4K
	HI_BOOL bStartUHDSplit = HI_FALSE;
#endif
    HI_S32 s32Ret;

    /* 所有节点都置为HI_FALSE */
    for(i=0; i<VPSS_HAL_TASK_NODE_BUTT;i++)
    {
        pstTask->pstInstance->abNodeVaild[i] = HI_FALSE;
    }

#ifdef ZME_2L_TEST
    s32Ret = VPSS_CTRL_Zme2lAndRotateCfg(enIp, pstTask);
	if (HI_SUCCESS != s32Ret)
	{
		VPSS_ERROR("Check ZME_2L Failed\n");
		return s32Ret;
	}
#endif

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	if (VPSS_CTRL_Check2DH265InterlaceStart(pstTask))
	{		
		s32Ret = VPSS_CTRL_Start2DTask_H265(enIp, pstTask);
		if (HI_SUCCESS != s32Ret)
		{
			VPSS_ERROR("Start VPSS_CTRL_Start2DTask_H265 Task Failed\n");
			return s32Ret;
		}
	}
	else
	{
        bStart2D = VPSS_CTRL_Check2DStart(pstTask);
		if(bStart2D)
		{
			s32Ret = VPSS_CTRL_Start2DTask(enIp, pstTask);
			if (HI_SUCCESS != s32Ret)
			{
				VPSS_ERROR("Start 2D Task Failed\n");
				return s32Ret;
			}

		}
	}   
	
#else
	bStart2D = VPSS_CTRL_Check2DStart(pstTask);
	if(bStart2D)
	{
		s32Ret = VPSS_CTRL_Start2DTask(enIp, pstTask);
		if (HI_SUCCESS != s32Ret)
		{
			VPSS_ERROR("Start 2D Task Failed\n");
			return s32Ret;
		}
	}
#endif
#ifdef ZME_2L_TEST
		//ZME_2L
		for(i = 0; i < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
		{
			if( VPSS_CTRL_CheckRotateStart(pstTask,i) )
			{
				VPSS_CTRL_StartRotateTask(enIp, pstTask, i);
			}
			else  if( VPSS_CTRL_CheckZME1LStart(pstTask,i) )
			{
				VPSS_CTRL_StartZME2LTask(enIp, pstTask, i);
			}
			else
			{
			}
		}
#else
		//rotate
		for(i = 0; i < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
		{
			if(VPSS_CTRL_CheckRotateStart(pstTask,i))
			{
				VPSS_CTRL_StartRotateTask(enIp, pstTask, i);
			}
			else
			{
				//释放旋转内存，节省SDK空间占用
				if(0 != g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf.u32Size)
				{
					VPSS_OSAL_FreeMem(&(g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf));
					g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf.u32Size = 0;
				}
			}
		}
#endif

    bStart3D = VPSS_CTRL_Check3DStart(pstTask);
    if(bStart3D)
    {
        (HI_VOID)VPSS_CTRL_Start3DTask(enIp, pstTask);
    }

	if ( HI_TRUE == pstTask->pstInstance->enCapability.bits.hi3798mv100)
	{
		bStartUHDSplit = VPSS_CTRL_CheckUHDSplitStart(pstTask);
		if (bStartUHDSplit)
		{
			(HI_VOID)VPSS_CTRL_StartSplitTask(enIp, pstTask);
		}
	}

	s32Ret = VPSS_HAL_StartLogic(enIp, pstTask->pstInstance->abNodeVaild);

#if DEF_VPSS_STATIC
	g_u32LogicStart = jiffies;
#endif

    return s32Ret;
}


HI_S32 VPSS_CTRL_GetRwzbData(VPSS_IP_E enIP,VPSS_TASK_S *pstTask)
{
	VPSS_INSTANCE_S* pstInstance;
	HI_U32 u32Count;
	VPSS_RWZB_S *pstRwzb;

	pstInstance = pstTask->pstInstance;
	pstRwzb = &(pstInstance->stRwzbInfo);

	for(u32Count = 0; u32Count < 6 ; u32Count ++)
	{
#if defined(CHIP_TYPE_hi3798mv100_a) || defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
		VPSS_HAL_GetDetPixel(enIP,u32Count,&(pstRwzb->u8RwzbData[u32Count][0]));
#endif

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200_b)
		VPSS_HAL_GetDetPixel(pstTask->stVpssHalInfo.u32stt_w_vir_addr,u32Count,&(pstRwzb->u8RwzbData[u32Count][0]));
#endif
	}

	return HI_SUCCESS;
}


HI_S32 VPSS_CTRL_UpdatePqWbcData(VPSS_INSTANCE_S *pstInstance,PQ_VPSS_WBC_REG_S *pstLocalWbcReg)
{
    HI_PQ_WBC_INFO_S stPqWbcInfo;

    HI_DRV_VIDEO_FRAME_S *pstCur;

    VPSS_IN_INTF_S stInIntf;

    HI_U32 u32BaseRegPhyAddr;
    HI_U32 u32BaseRegVirAddr;

    HI_S32 s32Ret;

	if ( HI_NULL == pstLocalWbcReg)
	{
    //    VPSS_ERROR("Get pstLocalWbcReg is null!!!\n");
        return HI_FAILURE;
	}

    memcpy(&pstInstance->stPqWbcReg,
            pstLocalWbcReg,
            sizeof(PQ_VPSS_WBC_REG_S));

    s32Ret = VPSS_IN_GetIntf(&(pstInstance->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    (HI_VOID)stInIntf.pfnGetProcessImage(&pstInstance->stInEntity, &pstCur);

    stPqWbcInfo.u32Width = pstCur->u32Width;
    stPqWbcInfo.u32Height = pstCur->u32Height;
	stPqWbcInfo.bProgressive = pstCur->bProgressive;
    stPqWbcInfo.u32HandleNo = pstInstance->ID ;
    stPqWbcInfo.pstVPSSWbcReg = &pstInstance->stPqWbcReg;

    (HI_VOID)VPSS_HAL_GetBaseRegAddr(VPSS_IP_0,
                &u32BaseRegPhyAddr,
                &u32BaseRegVirAddr);

    DRV_PQ_GetWbcInfo(&stPqWbcInfo);

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	{
		HI_U32 u32RegSize;
		VPSS_REG_S *pstPhyReg;

		pstPhyReg = (VPSS_REG_S *)u32BaseRegVirAddr;

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_DIEGLBMTNBIN, VPSS_DIEMTNSUM4);

		memcpy((HI_VOID*)&(pstInstance->stPqRegData.VPSS_DIEGLBMTNBIN[0]),
				(HI_VOID*)&(pstPhyReg->VPSS_DIEGLBMTNBIN[0]),
				u32RegSize);

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_PDICHD, VPSS_PDICHD);

		memcpy((HI_VOID*)&(pstInstance->stPqRegData.VPSS_PDICHD),
				(HI_VOID*)&(pstPhyReg->VPSS_PDICHD),
				u32RegSize);

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_PDFRMITDIFF, VPSS_PDLASICNT341);

		memcpy((HI_VOID*)&(pstInstance->stPqRegData.VPSS_PDFRMITDIFF),
				(HI_VOID*)&(pstPhyReg->VPSS_PDFRMITDIFF),
				u32RegSize);
	}
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_StorePrivData(VPSS_BUFFER_S *pstVpssBuf,HI_DRV_VIDEO_FRAME_S *pstPrivFrmData)
{
	HI_U32 u32Addr;
	HI_DRV_VIDEO_FRAME_S *pstPrivFrame;
	MMZ_BUFFER_S *pstPrivDataBuf;

	pstPrivDataBuf = &(pstVpssBuf->stPrivDataBuf);

	u32Addr = pstPrivDataBuf->u32StartPhyAddr;

	pstPrivFrame = phys_to_virt(u32Addr);

	memcpy(pstPrivFrame,pstPrivFrmData,sizeof(HI_DRV_VIDEO_FRAME_S));

	return HI_SUCCESS;
}


HI_S32 VPSS_CTRL_CompleteTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    VPSS_INSTANCE_S* pstInstance;
    VPSS_FB_NODE_S* pstLeftFbNode;
    VPSS_FB_NODE_S* pstRightFbNode;
    HI_BOOL bDropped;
    HI_DRV_VIDEO_FRAME_S *pstTmpFrame = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S* pstPriv;
    HI_DRV_VPSS_BUFFER_TYPE_E ePortType = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
    HI_DRV_VIDEO_FRAME_S *pstCur;
    VPSS_IN_INTF_S stInIntf;
    HI_S32 s32Ret = HI_FAILURE;

    pstInstance = pstTask->pstInstance;

    s32Ret = VPSS_IN_GetIntf(&(pstInstance->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    /*step 0.0:get  info from pq module*/
	for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if ((HI_TRUE == pstPort->bEnble) &&
            ((HI_DRV_VPSS_ROTATION_90 == pstPort->enRotation)
            || (HI_DRV_VPSS_ROTATION_270 == pstPort->enRotation)))
        {
            //correct src info ,because rotation change it
            VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

            s32Ret = stInIntf.pfnGetProcessImage(&pstInstance->stInEntity, &pstCur);
            if (HI_SUCCESS != s32Ret)
            {
                VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
                return HI_FAILURE;
            }

            pstTask->stVpssHalInfo.stInInfo.u32Width = pstCur->u32Width;
            pstTask->stVpssHalInfo.stInInfo.u32Height = pstCur->u32Height;
            pstTask->stVpssHalInfo.stInInfo.bProgressive = pstCur->bProgressive;
            pstTask->stVpssHalInfo.stInInfo.enFieldMode = pstCur->enFieldMode;
            break;
        }
	}

	VPSS_CTRL_UpdatePqWbcData(pstTask->pstInstance,(PQ_VPSS_WBC_REG_S *)pstTask->stVpssHalInfo.u32stt_w_vir_addr);

	VPSS_CTRL_GetRwzbData(enIp,pstTask);

    /*step 1.0 :release done image*/
    VPSS_INST_CompleteImage(pstInstance);

    pstTmpFrame = VPSS_VMALLOC(sizeof(HI_DRV_VIDEO_FRAME_S));
    if (HI_NULL == pstTmpFrame)
    {
        return HI_FAILURE;
    }

    /*step 2:add outframe to outPort list*/
    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId == VPSS_INVALID_HANDLE)
        {
            continue;
        }
        pstLeftFbNode = pstTask->pstFrmNode[u32Count * 2];
        pstRightFbNode = pstTask->pstFrmNode[u32Count * 2 + 1];

        bDropped = HI_FALSE;
        if (pstLeftFbNode != HI_NULL || pstRightFbNode != HI_NULL)
        {
			//Drop 2first frame for interlace stream
            if(HI_FALSE == pstInstance->stInEntity.stStreamInfo.u32StreamProg && HI_NULL != pstInstance->stInEntity.pstWbcInfo[0])
            {
                if((2 == pstInstance->stInEntity.pstWbcInfo[0]->u32CompleteCount)
                    || (3 == pstInstance->stInEntity.pstWbcInfo[0]->u32CompleteCount))
                {
                    bDropped = HI_TRUE;
                }
            }
        }

        if (pstLeftFbNode != HI_NULL )
        {
            memcpy(pstTmpFrame, &(pstLeftFbNode->stOutFrame), sizeof(HI_DRV_VIDEO_FRAME_S));

            if (pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                /*Revise the Port Type to HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE
                    *in order to decide whether report newframe
                    */
                ePortType = HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE;

				if (pstInstance->bStorePrivData)
				{
                    memset(&(pstLeftFbNode->stOutFrame.stLowdelayStat), 0x0, sizeof(HI_DRV_LOWDELAY_STAT_INFO_S));
                    
                    // record report done time for overlay lowdelay
                    (HI_VOID)HI_DRV_SYS_GetTimeStampMs(&(pstLeftFbNode->stOutFrame.stLowdelayStat.u32OmxReportDoneTime));
				
					(HI_VOID)VPSS_CTRL_StorePrivData(&(pstLeftFbNode->stBuffer),
							&(pstLeftFbNode->stOutFrame));
				}

                if (HI_FALSE == bDropped)
                {
                    if (u32Count == 0)
                    {
                        HI_LD_Event_S evt;
                        HI_U32 TmpTime = 0;
                        HI_DRV_SYS_GetTimeStampMs(&TmpTime);
                        evt.evt_id = EVENT_VPSS_FRM_OUT;
                        evt.frame = pstLeftFbNode->stOutFrame.u32FrameIndex;
                        evt.handle = pstLeftFbNode->stOutFrame.hTunnelSrc;
                        evt.time = TmpTime;
                        HI_DRV_LD_Notify_Event(&evt);
                    }

                    VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                                           pstPort->s32PortId,
                                           &(pstLeftFbNode->stOutFrame));
                }
                else
                {
                    VPSS_INST_RelFrmBuffer(pstTask->pstInstance, pstPort->s32PortId,
                                           &(pstPort->stFrmInfo.stBufListCfg),
                                           &(pstLeftFbNode->stBuffer.stMemBuf));
                }

                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                       pstTask->pstFrmNode[u32Count],
                                       VPSS_FB_TYPE_NORMAL);
            }
            else
            {
                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S*) & (pstLeftFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    if ((pstInstance->stInEntity.stStreamInfo.u32InRate > pstPort->u32MaxFrameRate)
                        &&(pstPort->u32MaxFrameRate != 0))
                    {
                        pstLeftFbNode->stOutFrame.u32FrameRate
                            = pstInstance->stInEntity.stStreamInfo.u32InRate / 2 * 1000;
                    }

                    if (u32Count == 0)
                    {
                        HI_LD_Event_S evt;
                        HI_U32 TmpTime = 0;
                        HI_DRV_SYS_GetTimeStampMs(&TmpTime);
                        evt.evt_id = EVENT_VPSS_FRM_OUT;
                        evt.frame = pstLeftFbNode->stOutFrame.u32FrameIndex;
                        evt.handle = pstLeftFbNode->stOutFrame.hTunnelSrc;
                        evt.time = TmpTime;
                        HI_DRV_LD_Notify_Event(&evt);
                    }
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo), pstLeftFbNode);
                }
                else
                {
                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                           pstLeftFbNode,
                                           VPSS_FB_TYPE_NORMAL);
                }
            }
        }

        if (pstRightFbNode != HI_NULL)
        {
            memcpy(&(pstRightFbNode->stOutFrame.stBufAddr[0]),
                   &(pstTmpFrame->stBufAddr[0]),
                   sizeof(HI_DRV_VID_FRAME_ADDR_S));
            if (pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                if (HI_FALSE == bDropped)
                {
                    VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                                           pstPort->s32PortId,
                                           &(pstRightFbNode->stOutFrame));
                }
                else
                {
                    VPSS_INST_RelFrmBuffer(pstTask->pstInstance, pstPort->s32PortId,
                                           &(pstPort->stFrmInfo.stBufListCfg),
                                           &(pstRightFbNode->stBuffer.stMemBuf));
                }

                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                       pstTask->pstFrmNode[u32Count],
                                       VPSS_FB_TYPE_NORMAL);
            }
            else
            {

                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S*) & (pstRightFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo), pstRightFbNode);
                }
                else
                {
                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                           pstRightFbNode,
                                           VPSS_FB_TYPE_NORMAL);
                }
            }
        }
    }

    if (ePortType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
    {
        if (pstInstance->pfUserCallBack != HI_NULL)
        {
            pstInstance->pfUserCallBack(pstInstance->hDst, VPSS_EVENT_NEW_FRAME, HI_NULL);
        }
        else
        {
            VPSS_FATAL("Can't report VPSS_EVENT_NEW_FRAME,pfUserCallBack is NULL");
        }
    }

    VPSS_VFREE(pstTmpFrame);

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_ClearTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_FB_NODE_S *pstFbNode;
    VPSS_IN_INTF_S stInIntf;
    VPSS_INSTANCE_S *pstInst;

    HI_S32 s32Ret = HI_FAILURE;

    pstInst = pstTask->pstInstance;


    s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    //step1:release src buffer
    s32Ret = stInIntf.pfnCompleteImage(&(pstInst->stInEntity));
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("pfnCompleteImage failed!\n");
        return HI_FAILURE;
    }

    //step2:release port buffer
    for(u32Count = 0;u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstFbNode = pstTask->pstFrmNode[u32Count*2];
        if(pstFbNode != HI_NULL)
        {
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMemBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                pstFbNode,
                VPSS_FB_TYPE_NORMAL);
        }

        pstFbNode = pstTask->pstFrmNode[u32Count*2 + 1];
        if(pstFbNode != HI_NULL)
        {

            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMemBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                            pstFbNode,
                            VPSS_FB_TYPE_NORMAL);
        }
    }

    //step3:reset vpss logic
    VPSS_REG_ReSetCRG(pstTask->pstInstance->CtrlID);

    return HI_SUCCESS;

}

HI_S32 VPSS_CTRL_ThreadProc(HI_VOID* pArg)
{
	VPSS_IP_E enIp;
	HI_S32 s32CreateRet   = HI_FAILURE;
	HI_S32 s32StartRet    = HI_FAILURE;
	HI_S32 s32WaitRet     = HI_FAILURE;

	HI_U32 u32NowTime = 0;
	VPSS_CTRL_S *pstVpssCtrl = HI_NULL;

	enIp = (VPSS_IP_E)pArg;
	pstVpssCtrl = &(g_stVpssCtrl[enIp]);

	pstVpssCtrl->s32ThreadPos = 0;

	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stTaskNext), EVENT_UNDO, EVENT_UNDO);
	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stNewTask), EVENT_UNDO, EVENT_UNDO);
	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stTaskLowDelay), EVENT_UNDO, EVENT_UNDO);

	pstVpssCtrl->stTask.u32LastTotal = 0;
	pstVpssCtrl->stTask.u32SuccessTotal = 0;
	pstVpssCtrl->stTask.u32Create = 0;
	pstVpssCtrl->stTask.u32Fail = 0;
	pstVpssCtrl->stTask.u32TimeOut= 0;

	while(!kthread_should_stop())
	{
		HI_S32 s32Ret = HI_FAILURE;
		pstVpssCtrl->stTask.stState = TASK_STATE_READY;

		if(pstVpssCtrl->u32ThreadSleep == 1)
		{
			goto VpssThreadIdle;
		}

		pstVpssCtrl->s32ThreadPos = 1;

		pstVpssCtrl->stTask.u32Create++;

		s32CreateRet =  VPSS_CTRL_CreateTask(enIp, &(pstVpssCtrl->stTask));
		/* create success  running -> waitting */
		if(s32CreateRet == HI_SUCCESS)
		{
			VPSS_INFO("...............CreateTask\n");
			pstVpssCtrl->s32ThreadPos = 2;

			VPSS_HAL_SetClockEn(enIp, HI_TRUE);

            if (VPSS_CTRL_CheckLowDelay(enIp, &(pstVpssCtrl->stTask)))
            {
                pstVpssCtrl->stTask.bOutLowDelay = HI_TRUE;
            }
            else
            {
                pstVpssCtrl->stTask.bOutLowDelay = HI_FALSE;
            }
			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stTaskNext), EVENT_UNDO, EVENT_UNDO);
			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stTaskLowDelay), EVENT_UNDO, EVENT_UNDO);
			s32StartRet = VPSS_CTRL_StartTask(enIp, &(pstVpssCtrl->stTask));
			if (s32StartRet == HI_SUCCESS)
			{
				VPSS_INFO("...............StartTask\n");
				/*
				   start logic running, waitting for irq to wakeup thread
				   */
				pstVpssCtrl->stTask.stState = TASK_STATE_WAIT;

                if (pstVpssCtrl->stTask.bOutLowDelay)
                {
				    s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskLowDelay), HZ);
                }
                else
                {
				    s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskNext), HZ);
                }
                
				if (s32WaitRet == HI_SUCCESS)
				{
					pstVpssCtrl->s32ThreadPos = 3;

					VPSS_CTRL_CompleteTask(enIp, &(pstVpssCtrl->stTask));

					if(jiffies - u32NowTime >= HZ)
					{
						u32NowTime = jiffies;
						pstVpssCtrl->stTask.u32SucRate = pstVpssCtrl->stTask.u32SuccessTotal 
							- pstVpssCtrl->stTask.u32LastTotal;
						pstVpssCtrl->stTask.u32LastTotal = pstVpssCtrl->stTask.u32SuccessTotal;
					}

					pstVpssCtrl->stTask.u32SuccessTotal ++;

					if (pstVpssCtrl->stTask.bOutLowDelay)
                    {
    				    s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskNext), HZ);
    				    if (s32WaitRet != HI_SUCCESS)
    				    {
					        VPSS_FATAL("...............Wait LowDelay Faild\n");
    				    }
                    }
				}
				else
				{
					VPSS_FATAL("...............Wait OutTime Faild\n");
					pstVpssCtrl->s32ThreadPos = 4;
					pstVpssCtrl->stTask.u32TimeOut++;

					VPSS_CTRL_ClearTask(enIp, &(pstVpssCtrl->stTask));
				}
			}
			else
			{
				/* 创建Task成功之后，start必须成功 */
				VPSS_FATAL("...............StartTask Faild\n");
				s32StartRet = HI_FAILURE;

				VPSS_CTRL_ClearTask(enIp, &(pstVpssCtrl->stTask));

				goto  VpssThreadIdle;
			}

		}
		else/*create failed or start failed running -> idle*/
		{
			pstVpssCtrl->stTask.u32Fail++;

VpssThreadIdle:
			pstVpssCtrl->s32ThreadPos = 5;
			pstVpssCtrl->stTask.stState = TASK_STATE_IDLE;

			VPSS_HAL_SetClockEn(enIp, HI_FALSE);

			s32Ret = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stNewTask),HZ/100);
			if(s32Ret == HI_SUCCESS)
			{
				VPSS_INFO("WakeUpThread Success.\n");
			}

			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stNewTask), EVENT_UNDO, EVENT_UNDO);
#ifdef ZME_2L_TEST
			/*当上层出现stop操作时，释放缓存的buffer*/
			if( HI_NULL != pstVpssCtrl->stTask.pstFrmNodeZME1L)
			{
				//释放旋转内存，节省SDK空间占用
				if(0 != pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size)
				{
					VPSS_OSAL_FreeMem(&(pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf));
					pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size = 0;
					pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.u32Stride = 0;

					VPSS_VFREE(pstVpssCtrl->stTask.pstFrmNodeZME1L);
					pstVpssCtrl->stTask.pstFrmNodeZME1L = HI_NULL;
				}
			}
#endif
		}

	}

	pstVpssCtrl->s32ThreadPos = 6;

	VPSS_INFO("s32ThreadPos = %d...\n",pstVpssCtrl->s32ThreadPos);

	return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_CreateThread(VPSS_IP_E enIp)
{
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#else
    struct sched_param param;
#endif
    g_stVpssCtrl[enIp].u32ThreadKilled = 0;
    g_stVpssCtrl[enIp].u32ThreadSleep = 0;
    g_stVpssCtrl[enIp].hThread =
        kthread_create(VPSS_CTRL_ThreadProc, (HI_VOID *)enIp, "HI_VPSS_Process");

	if( IS_ERR(g_stVpssCtrl[enIp].hThread) )
    {
        VPSS_FATAL("Can not create thread.\n");
        return HI_FAILURE;
    }

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#else
    param.sched_priority = 99;
    sched_setscheduler(g_stVpssCtrl[enIp].hThread, SCHED_RR, &param);

#ifndef HI_TEE_SUPPORT
	kthread_bind(g_stVpssCtrl[enIp].hThread, 3);
#endif
#endif

    wake_up_process(g_stVpssCtrl[enIp].hThread);

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DestoryThread(VPSS_IP_E enIp)
{
    HI_S32 s32Ret;

    s32Ret = kthread_stop(g_stVpssCtrl[enIp].hThread);

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Destory Thread Error.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_CreateInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    DRV_PROC_ITEM_S  *pProcItem;

    HI_OSAL_Snprintf(ProcName, 20, "vpss%02x", (HI_U32)(hVPSS));

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);

    if (!pProcItem)
    {
        VPSS_FATAL("Vpss add proc failed!\n");
        return HI_FAILURE;
    }

    pProcItem->data  = (HI_VOID *)hVPSS;
    pProcItem->read  = VPSS_CTRL_ProcRead;
    pProcItem->write = VPSS_CTRL_ProcWrite;

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DestoryInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    HI_OSAL_Snprintf(ProcName, 20, "vpss%02x", (HI_U32)(hVPSS));
    HI_DRV_PROC_RemoveModule(ProcName);
    return HI_SUCCESS;
}

HI_U32 VPSS_CTRL_MallocInstanceId(HI_VOID)
{
    HI_U32 i;
    HI_U32 u32InstCount;
    HI_BOOL bFindVpss;
    HI_U32 u32VpssId;
    VPSS_IP_E enVpssIp;
    unsigned long  u32LockFlag;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;

    u32InstCount = 0;

    for(u32VpssId = 0; u32VpssId < VPSS_IP_BUTT * VPSS_INSTANCE_MAX_NUMB; u32VpssId++)
    {
        bFindVpss = HI_FALSE;

        for(enVpssIp = VPSS_IP_0; enVpssIp < VPSS_IP_BUTT; enVpssIp++)
        {
            pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIp].stInstCtrlInfo);
            read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
            for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
            {
                if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
                {
                    if(u32VpssId == pstInstCtrlInfo->pstInstPool[i]->ID)
                    {
                        bFindVpss = HI_TRUE;
                        u32InstCount++;
                        break;
                    }
                }
            }
            read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        }

        if(!bFindVpss)
        {
            break;
        }
    }

    if(u32InstCount == 0)
    {
        return 0;
    }
    else
    {
        return u32VpssId;
    }
}

VPSS_HANDLE VPSS_CTRL_AddInstance(VPSS_INSTANCE_S *pstInstance)
{
    HI_U32 u32VpssId;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    HI_U32 u32Count;
    unsigned long  u32LockFlag;
    VPSS_IP_E enVpssIp;


    enVpssIp = pstInstance->CtrlID;
    pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIp].stInstCtrlInfo);
    u32VpssId = VPSS_CTRL_MallocInstanceId();
    write_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count++)
    {
        if (pstInstCtrlInfo->pstInstPool[u32Count] == HI_NULL)
        {
            pstInstCtrlInfo->pstInstPool[u32Count] = pstInstance;

            break;
        }
    }
    write_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    if (u32Count == VPSS_INSTANCE_MAX_NUMB)
    {
        VPSS_FATAL("Instance Number is Max.\n");

        return VPSS_INVALID_HANDLE;
    }
    else
    {
        pstInstance->ID = u32VpssId;
        pstInstCtrlInfo->u32InstanceNum++;
        return pstInstance->ID;
    }
}

HI_S32 VPSS_CTRL_DelInstance(VPSS_INSTANCE_S* pstInstance)
{
    HI_U32 i;
    VPSS_IP_E enVpssIP;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    unsigned long  u32LockFlag;

    VPSS_CHECK_NULL(pstInstance);

    enVpssIP = pstInstance->CtrlID;
    pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIP].stInstCtrlInfo);

    write_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
    for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
    {
        if(pstInstCtrlInfo->pstInstPool[i] == pstInstance)
        {
            pstInstCtrlInfo->pstInstPool[i] = HI_NULL;
            break;
        }
    }
    write_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    return HI_SUCCESS;
}



HI_S32 VPSS_CTRL_Init(HI_VOID)
{
	HI_S32 s32Ret;
	HI_U32 i,j;

	g_stVpssCtrl[0].bIPVaild = HI_TRUE;
	g_stVpssCtrl[1].bIPVaild = HI_FALSE;


	for(i = 0; i < VPSS_IP_BUTT; i++)
	{
		if(HI_TRUE != g_stVpssCtrl[i].bIPVaild)
		{
			continue;
		}

		if(0 == g_stVpssCtrl[i].s32IsVPSSOpen)
		{
			s32Ret = VPSS_CTRL_RegistISR((VPSS_IP_E)i);

			if(HI_SUCCESS != s32Ret)
			{
				goto VPSS_IP_DEL_INT;
			}

			VPSS_CTRL_InitInstList((VPSS_IP_E)i);

			s32Ret = VPSS_HAL_Init((VPSS_IP_E)i);
			if(HI_SUCCESS != s32Ret)
			{
				goto VPSS_IP_UnRegist_IRQ;
			}

			s32Ret = VPSS_CTRL_CreateThread((VPSS_IP_E)i);
			if (s32Ret != HI_SUCCESS)
			{
				VPSS_FATAL("VPSS_CTRL_CreateThread Failed\n");
				goto VPSS_IP_HAL_DEL_INIT;
			}
		}

		g_stVpssCtrl[i].s32IsVPSSOpen++;

	}

	return HI_SUCCESS;

VPSS_IP_HAL_DEL_INIT:
	(HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)i);
VPSS_IP_UnRegist_IRQ:
	(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)i);

VPSS_IP_DEL_INT:
	for(j = 0; j < i; j++)
	{
		if(HI_TRUE == g_stVpssCtrl[j].bIPVaild)
		{
			(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)j);
			(HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)j);
			(HI_VOID)VPSS_CTRL_DestoryThread((VPSS_IP_E)j);
		}
	}

	return HI_FAILURE;
}

HI_S32 VPSS_CTRL_DelInit(HI_VOID)
{
    HI_U32 i;
    HI_U32 u32Count;
    VPSS_BUFFER_S *pstVpssBuf;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        if(HI_TRUE != g_stVpssCtrl[i].bIPVaild)
        {
            continue;
        }

        if(1 > g_stVpssCtrl[i].s32IsVPSSOpen)
        {
            VPSS_FATAL("CTRL_DelInit Error,vpss hasn't initted.\n");
            return HI_FAILURE;
        }

        g_stVpssCtrl[i].s32IsVPSSOpen--;
        if(1 == g_stVpssCtrl[i].s32IsVPSSOpen)
        {
        for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count ++)
        {
            if (HI_NULL != g_stVpssCtrl[i].stInstCtrlInfo.pstInstPool[u32Count])
            {
                VPSS_FATAL("CTRL_DelInit Error,destroy instance first.\n");
                g_stVpssCtrl[i].s32IsVPSSOpen++;
                return HI_FAILURE;
                }
            }
            //add for low power
            VPSS_HAL_SetClockEn(i, HI_FALSE);

            //release rotation buffer

            for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
            {
            #ifdef ZME_2L_TEST
				if ( g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count] != HI_NULL)
				{
					pstVpssBuf = &(g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count]->stBuffer);
					if (pstVpssBuf->stMemBuf.u32Size != 0)
					{
						(HI_VOID)VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
						pstVpssBuf->u32Stride = 0;
						pstVpssBuf->stMemBuf.u32Size = 0;

						VPSS_VFREE(g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count]);
						g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count] = HI_NULL;
					}

				}

				if ( g_stVpssCtrl[i].stTask.pstFrmNodeZME1L != HI_NULL)
				{
					pstVpssBuf = &(g_stVpssCtrl[i].stTask.pstFrmNodeZME1L->stBuffer);
					if (pstVpssBuf->stMemBuf.u32Size != 0)
					{
						(HI_VOID)VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
						pstVpssBuf->u32Stride = 0;
						pstVpssBuf->stMemBuf.u32Size = 0;

						VPSS_VFREE(g_stVpssCtrl[i].stTask.pstFrmNodeZME1L);
						g_stVpssCtrl[i].stTask.pstFrmNodeZME1L = HI_NULL;
					}
				}
			#else
                pstVpssBuf = &(g_stVpssCtrl[i].stRoBuf[u32Count]);
                if (pstVpssBuf->stMemBuf.u32Size != 0)
                {
					VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
					pstVpssBuf->u32Stride = 0;
					pstVpssBuf->stMemBuf.u32Size = 0;
                }
			#endif
            }
        }

        if(0 == g_stVpssCtrl[i].s32IsVPSSOpen)
        {
            (HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)i);
            (HI_VOID)VPSS_CTRL_DestoryThread((VPSS_IP_E)i);
            (HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)i);
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_SetMceFlag(HI_BOOL bInMCE)
{
    HI_U32 i;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        g_stVpssCtrl[i].bInMCE = bInMCE;
    }

    return HI_SUCCESS;
}

VPSS_HANDLE VPSS_CTRL_CreateInstance(HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    HI_S32 s32Ret;
    HI_S32 s32InstHandle = VPSS_INVALID_HANDLE;
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    s32Ret = VPSS_CTRL_GetDistributeIP(&enVpssIp);
    if(HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("vpss ctrl isn't enable\n");
        return s32InstHandle;
    }

    if (1 >= g_stVpssCtrl[enVpssIp].s32IsVPSSOpen)
    {
        VPSS_FATAL("vpss ctrl isn't opened\n");
        return s32InstHandle;
    }

    pstInstance = (VPSS_INSTANCE_S*)VPSS_VMALLOC(sizeof(VPSS_INSTANCE_S));
    if (pstInstance != HI_NULL)
    {
        s32Ret = VPSS_INST_Init(pstInstance,pstVpssCfg);
        if(HI_SUCCESS != s32Ret)
        {
            VPSS_VFREE(pstInstance);
            return s32InstHandle;
        }

        pstInstance->CtrlID = enVpssIp;

        s32InstHandle = VPSS_CTRL_AddInstance(pstInstance);
        if (s32InstHandle != VPSS_INVALID_HANDLE)
        {
            if (g_stVpssCtrl[enVpssIp].bInMCE == HI_FALSE)
            {
                (HI_VOID)VPSS_CTRL_CreateInstProc(s32InstHandle);
            }
        }
        else
        {
            VPSS_VFREE(pstInstance);
        }
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
        memset(&pstInstance->stRefList, 0x0, sizeof(pstInstance->stRefList));
#endif
        return s32InstHandle;
    }
    else
    {
        VPSS_FATAL("vmalloc instance node failed \n");
        s32Ret = HI_FAILURE;
        return s32InstHandle;
    }

}

HI_S32 VPSS_CTRL_DestoryInstance(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if (pstInstance == HI_NULL)
    {
        VPSS_FATAL("hVPSS(%d) is Not be Vaild Or Created\n", hVPSS);
        return HI_FAILURE;
    }

    if (pstInstance->enState != INSTANCE_STATE_STOP)
    {
        VPSS_FATAL("Instance %#x is still working, please stop first\n");
        return HI_FAILURE;
    }

    /*
      *  when deletting instance
      *  must get lock first to ensure that it isn't being served
     */
    enVpssIp = pstInstance->CtrlID;
    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 1;

    while(g_stVpssCtrl[enVpssIp].s32ThreadPos != 5)
    {
        msleep(10);
    }

    if (g_stVpssCtrl[enVpssIp].bInMCE == HI_FALSE)
    {
        VPSS_CTRL_DestoryInstProc(hVPSS);
    }

    g_stVpssCtrl[enVpssIp].stInstCtrlInfo.u32InstanceNum--;
	
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	VPSS_HAL_RefListDeInit(&pstInstance->stRefList);
#endif

    VPSS_CTRL_DelInstance(pstInstance);
    (HI_VOID)VPSS_INST_DelInit(pstInstance);
    VPSS_VFREE(pstInstance);

    pstInstance = HI_NULL;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 0;
    return HI_SUCCESS;

}
HI_S32 VPSS_CTRL_GetInstanceNumber(VPSS_IP_E enIp,HI_U32 *pu32Number)
{
	HI_U32 i;
	VPSS_INST_CTRL_S *pstInstCtrlInfo;
	unsigned long u32LockFlag;
	HI_U32 u32Cnt = 0;

	pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
	read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
	for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
	{
		if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
		{
			u32Cnt ++;
		}
	}
	read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	*pu32Number = u32Cnt;

	return HI_SUCCESS;
}

VPSS_INSTANCE_S *VPSS_CTRL_GetOnlyInstance(VPSS_IP_E enIp)
{
	HI_U32 u32InstNumber;
	VPSS_INST_CTRL_S *pstInstCtrlInfo;
	VPSS_INSTANCE_S *pstRetPtr = HI_NULL;
    unsigned long u32LockFlag;
    HI_U32 i;

	VPSS_CTRL_GetInstanceNumber(VPSS_IP_0,&u32InstNumber);

	if (u32InstNumber != 1)
	{
		return HI_NULL;
	}

	pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);

	read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
	{
		if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
		{
			pstRetPtr = pstInstCtrlInfo->pstInstPool[i];
		}
	}

	read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	return pstRetPtr;
}

VPSS_INSTANCE_S* VPSS_CTRL_GetInstance(VPSS_HANDLE hVPSS)
{
    HI_U32 i;
    HI_U32 u32IpPos;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    VPSS_INSTANCE_S *pstRetPtr = HI_NULL;
    unsigned long u32LockFlag;


    if ((hVPSS < 0) || (hVPSS >= VPSS_INSTANCE_MAX_NUMB * VPSS_IP_BUTT))
    {
        VPSS_FATAL("Invalid VPSS HANDLE %x.\n",hVPSS);
        return HI_NULL;
    }

    for(u32IpPos = VPSS_IP_0; u32IpPos < VPSS_IP_BUTT; u32IpPos++)
    {
        pstInstCtrlInfo = &(g_stVpssCtrl[u32IpPos].stInstCtrlInfo);
        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
			{
				if(hVPSS == pstInstCtrlInfo->pstInstPool[i]->ID)
				{
					pstRetPtr = pstInstCtrlInfo->pstInstPool[i];
					break;
				}
			}
		}
        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    }

    return pstRetPtr;
}

HI_S32 VPSS_CTRL_WakeUpThread(HI_VOID)
{
    if (g_stVpssCtrl[VPSS_IP_0].s32IsVPSSOpen >= 1)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stNewTask), 1, 0);
    }

    if (g_stVpssCtrl[VPSS_IP_1].s32IsVPSSOpen >= 1)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stNewTask), 1, 0);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Pause(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;
	HI_U32 u32SleepNumb = 0;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    VPSS_CHECK_NULL(pstInstance);

    enVpssIp = pstInstance->CtrlID;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 1;

    while(g_stVpssCtrl[enVpssIp].s32ThreadPos != 5 && u32SleepNumb < 200)
    {
        msleep(10);

		u32SleepNumb ++;
    }

	if (u32SleepNumb >= 200)
	{
		VPSS_ERROR("thread sleep failed\n");
	}

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Resume(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    VPSS_CHECK_NULL(pstInstance);

    enVpssIp = pstInstance->CtrlID;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 0;

    VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[enVpssIp].stNewTask), 1, 0);

    return HI_SUCCESS;
}

irqreturn_t VPSS0_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32State = 0;

    (HI_VOID)VPSS_HAL_GetIntState(VPSS_IP_0,&u32State);

    if(u32State & 0x80)
    {
        VPSS_FATAL("IRQ DCMP ERR    state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x80);
    }

    if(u32State & 0x4)
    {
        VPSS_FATAL("IRQ BUS ERR  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x4);
    }

    if(u32State & 0x2)
    {
        VPSS_FATAL("TIMEOUT  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x2);
    }

    if(u32State & 0x1)
    {
        //VPSS_FATAL("NODE  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x1);

#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[0][g_u32Pos]));
#endif
    }
    if(u32State & 0x40 || u32State & 0x10)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x40);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x10);
        
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskLowDelay),EVENT_DONE,EVENT_UNDO);
    }
    if(u32State & 0x8)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x8);

#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[1][g_u32Pos]));
		g_u32LogicEnd = jiffies;

		g_u32LogicTime[g_u32Pos] = g_u32LogicEnd - g_u32LogicStart;

		g_u32Pos = (g_u32Pos+1)%15;
#endif
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskNext),EVENT_DONE,EVENT_UNDO);
    }

    return IRQ_HANDLED;
}

irqreturn_t VPSS1_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32State = 0;

    (HI_VOID)VPSS_HAL_GetIntState(VPSS_IP_1,&u32State);

    if(u32State & 0x80)
    {
        VPSS_FATAL("IRQ DCMP ERR    state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x80);
    }

    if(u32State & 0x4)
    {
        VPSS_FATAL("IRQ BUS ERR  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x4);
    }

    if(u32State & 0x2)
    {
        VPSS_FATAL("TIMEOUT  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x2);
    }

    if(u32State & 0x70)//   0xf ---> 0xff open tunl mask and dcmp err mask
    {
        VPSS_FATAL(" Tunnel = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x70);
    }

    if(u32State & 0x1)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x1);
    }

    if(u32State & 0x8)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x8);
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stTaskNext),EVENT_DONE,EVENT_UNDO);
    }

    return IRQ_HANDLED;
}

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v)
{
    VPSS_INSTANCE_S* pstInstance;
    VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_IN_ENTITY_S* pstInEntity;
    VPSS_IN_IMAGELIST_STATE_S stImgListState;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
	HI_U8 *pProcPortFmt[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    HI_S32 s32SrcModuleID;
    HI_U32 u32Count;
    pProcItem = p->private;

    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)pProcItem->data);

    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)pProcItem->data);
        return HI_FAILURE;
    }
    pstInEntity = &(pstInstance->stInEntity);
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);

    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");

            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));

    }

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);

		if ( (pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12_CMP) <= 5 &&
			 (pstPortPrc[u32Count]->eFormat >= HI_DRV_PIX_FMT_NV12_CMP) )
		{
			pProcPortFmt[u32Count] = g_pPixString[pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12_CMP + 7] ;
		}
		else if ((pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6 &&
				 (pstPortPrc[u32Count]->eFormat >= HI_DRV_PIX_FMT_NV12))
		{
			pProcPortFmt[u32Count] = g_pPixString[pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12];
		}
		else
		{
			pProcPortFmt[u32Count] = g_pPixString[13];
		}
    }

    s32SrcModuleID = (pstInstance->hDst & 0x00ff0000) >>16;

#if DEF_VPSS_STATIC

	PROC_PRINT(p,
	"-----Logic Process Jiffies----|" "Node 0 Cycle     Node 1 Cycle\n"
	);

	for (u32Count = 0; u32Count < 15; u32Count++)
	{
		PROC_PRINT(p,
		"%d                    |%d              %d\n",
		g_u32LogicTime[u32Count],
		g_u32LogicCycle[0][u32Count],
		g_u32LogicCycle[1][u32Count]);
	}
#else
    PROC_PRINT(p,
        "--------VPSS%04x---------------|"   "------------------------PortInfo------------------------|\n"
        "ID               :0x%-8x   |"       "ID               :0x%-8x  |0x%-8x  |0x%-8x  |\n"
        "State            :%-10s   |"        "State            :%-3s         |%-3s         |%-3s         |\n"
        "Priority         :%-10d   |"        "PixelFormat      :%-12s|%-12s|%-12s|\n"
        "QuickOutPut      :%-10s   |"        "Resolution       :%4d*%-4d   |%4d*%-4d   |%4d*%-4d   |\n"
        "SourceID         :%-6s(%02x)   |"   "ColorSpace       :%-12s|%-12s|%-12s|\n"
        "Version          :%-10s   |"        "DispPixelAR(W/H) :%2d/%-2d       |%2d/%-2d       |%2d/%-2d       |\n"
        "                               |"   "Aspect Mode      :%-12s|%-12s|%-12s|\n"
        "                               |"   "Support3DStream  :%-12s|%-12s|%-12s|\n"
        "                               |"   "MaxFrameRate     :%-5d       |%-5d       |%-5d       |\n"
        "-------- Algorithm-------------|"   "*LowDelay        :%-12s|%-12s|%-12s|\n"
        "P/I Setting   :%-10s      |"        "HorizonFlip      :%-12s|%-12s|%-12s|\n"
        "Deinterlace   :%-10s      |"        "VerticalFlip     :%-12s|%-12s|%-12s|\n"
        "Sharpness     :%-10s      |"        "Rotation         :%-12s|%-12s|%-12s|\n"
#ifdef ZME_2L_TEST
		"*ProgRevise   :%-10s      |"		 "*ZME1LResolution :%4d*%-4d   |%4d*%-4d   |%4d*%-4d   |\n"
#else
        "*ProgRevise   :%-10s      |"        "                              |            |            |\n"
#endif
        "                               |"   "                              |            |            |\n"
        "--------Detect Info------------|"   "                              |            |            |\n"
        "TopFirst(Src):%6s(%-6s)   |"        "                              |            |            |\n"
        "InRate(Src)  :%6d(%-6d)   |"        "                              |            |            |\n"
		"*Trans       :%8s         |"        "                              |            |            |\n"
        "Progressive/Interlace(Src):%-1s(%-1s)|"        "                              |            |            |\n",
        /* attribute */
        pstInstance->ID,
        pstInstance->ID,
                                                pstPortPrc[0]->s32PortId,
                                                pstPortPrc[1]->s32PortId,
                                                pstPortPrc[2]->s32PortId,
        g_pInstState[pstInstance->enState],
                                                g_pAlgModeString[pstInstance->stPort[0].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[1].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[2].bEnble],

        0,
												pProcPortFmt[0],
												pProcPortFmt[1],
												pProcPortFmt[2],

        g_pAlgModeString[pstInstance->bAlwaysFlushSrc],
                                                pstPortPrc[0]->s32OutputWidth,
                                                pstPortPrc[0]->s32OutputHeight,
                                                pstPortPrc[1]->s32OutputWidth,
                                                pstPortPrc[1]->s32OutputHeight,
                                                pstPortPrc[2]->s32OutputWidth,
                                                pstPortPrc[2]->s32OutputHeight,

        (s32SrcModuleID >= HI_ID_VFMW && s32SrcModuleID <= HI_ID_VENC)?
         g_pSrcModuleString[s32SrcModuleID - HI_ID_VFMW]:
        (s32SrcModuleID == 0?g_pSrcModuleString[0]:
         g_pSrcModuleString[9]),
         (pstInstance->hDst & 0x000000ff),
                                                g_pCscString[pstPortPrc[0]->eDstCS],
                                                g_pCscString[pstPortPrc[1]->eDstCS],
                                                g_pCscString[pstPortPrc[2]->eDstCS],
         DEF_SDK_VERSIO_LOG,
                                                pstPortPrc[0]->stDispPixAR.u32ARw,
                                                pstPortPrc[0]->stDispPixAR.u32ARh,
                                                pstPortPrc[1]->stDispPixAR.u32ARw,
                                                pstPortPrc[1]->stDispPixAR.u32ARh,
												pstPortPrc[2]->stDispPixAR.u32ARw,
                                                pstPortPrc[2]->stDispPixAR.u32ARh,

                                                g_pAspString[pstPortPrc[0]->eAspMode],
                                                g_pAspString[pstPortPrc[1]->eAspMode],
                                                g_pAspString[pstPortPrc[2]->eAspMode],

                                                g_pAlgModeString[pstPortPrc[0]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[1]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[2]->b3Dsupport],

                                                pstPortPrc[0]->u32MaxFrameRate,
                                                pstPortPrc[1]->u32MaxFrameRate,
                                                pstPortPrc[2]->u32MaxFrameRate,
        /*alg config*/                             g_pAlgModeString[pstPortPrc[0]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[1]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[2]->bTunnelEnable],

        g_pProgDetectString[pstInEntity->enProgInfo],
                                                g_pAlgModeString[pstPortPrc[0]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bHoriFlip],
        g_pDeiString[pstInstance->stProcCtrl.eDEI],
                                                g_pAlgModeString[pstPortPrc[0]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bVertFlip],
        g_pAlgModeString[pstInstance->stProcCtrl.eSharpness],
                                                g_pRotationString[pstPortPrc[0]->enRotation],
                                                g_pRotationString[pstPortPrc[1]->enRotation],
                                                g_pRotationString[pstPortPrc[2]->enRotation],
#ifdef ZME_2L_TEST
		g_pAlgModeString[pstInEntity->bProgRevise],
												pstInstance->stPort[0].u32ZME1LWidth,
												pstInstance->stPort[0].u32ZME1LHeight,
												pstInstance->stPort[1].u32ZME1LWidth,
												pstInstance->stPort[1].u32ZME1LHeight,
												pstInstance->stPort[2].u32ZME1LWidth,
												pstInstance->stPort[2].u32ZME1LHeight,
#else
        g_pAlgModeString[pstInEntity->bProgRevise],
#endif
        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        ((pstStreamInfo->u32RealTopFirst == 0)?"Bottom":"Top"):"NA",
        (pstStreamInfo->u32StreamTopFirst == 0)?"Bottom":"Top",

        pstStreamInfo->u32InRate*1000,pstStreamInfo->u32StreamInRate,
		(pstInstance->stInEntity.stTransFbInfo.bNeedTrans == HI_TRUE)? "ON":"OFF",


        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        "I":"P",
        (pstStreamInfo->u32StreamTopFirst == 0)?"I":"P"
        );
    #endif
    #if 1
    PROC_PRINT(p,
    "-----SourceFrameList Info------|"  "--------------------OutFrameList Info-------------------|\n"
    "      (source to vpss)         |"  "                     (vpss to sink)                     |\n"
    "*Mutual Mode  :%-11s     |"        "BufManager       :%-10s  |%-10s  |%-10s  |\n"
    "                               |"  "BufNumber        :%-2d+%-2d       |%-2d+%-2d       |%-2d+%-2d       |\n"
    "GetSrcImgHZ(Try/OK)  :%3d/%-3d  |" "BufFul           :%-2d          |%-2d          |%-2d          |\n"
    "GetOutBufHZ(Try/OK)  :%3d/%-3d  |" "BufEmpty         :%-2d          |%-2d          |%-2d          |\n"
    "ProcessHZ(Try/OK)    :%3d/%-3d  |" "AcquireHZ        :%-10d  |%-10d  |%-10d  |\n"
    "Acquire(Try/OK):               |"  "Acquire(Try/OK):              |            |            |\n"
    " %10d/%-10d         |"             " %10d/%-10d%10d/%-10d%10d/%-10d\n"
    "Release(Try/OK):               |"  "Release(Try/OK):              |            |            |\n"
    " %10d/%-10d         |"             " %10d/%-10d%10d/%-10d%10d/%-10d\n",

    g_pSrcMutualString[pstInstance->eSrcImgMode],
                                        g_pBufTypeString[pstPortPrc[0]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[1]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[2]->stBufListCfg.eBufType],

    pstPortPrc[0]->stBufListCfg.u32BufNumber,
    pstPortPrc[0]->stFbPrc.u32ExtListNumb,
    pstPortPrc[1]->stBufListCfg.u32BufNumber,
    pstPortPrc[1]->stFbPrc.u32ExtListNumb,
    pstPortPrc[2]->stBufListCfg.u32BufNumber,
    pstPortPrc[2]->stFbPrc.u32ExtListNumb,


    pstInstance->u32ImgRate,
    pstInstance->u32ImgSucRate,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb,
    pstInstance->u32BufRate,
    pstInstance->u32BufSucRate,
                                        pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stFbPrc.u32EmptyListNumb,
    pstInstance->u32CheckRate,
    pstInstance->u32CheckSucRate,
                                        pstPortPrc[0]->stFbPrc.u32GetHZ,
                                        pstPortPrc[1]->stFbPrc.u32GetHZ,
                                        pstPortPrc[2]->stFbPrc.u32GetHZ,


    stImgListState.u32GetUsrTotal,      stImgListState.u32GetUsrSuccess,
    pstPortPrc[0]->stFbPrc.u32GetTotal, pstPortPrc[0]->stFbPrc.u32GetSuccess,
    pstPortPrc[1]->stFbPrc.u32GetTotal, pstPortPrc[1]->stFbPrc.u32GetSuccess,
    pstPortPrc[2]->stFbPrc.u32GetTotal, pstPortPrc[2]->stFbPrc.u32GetSuccess,
    stImgListState.u32RelUsrTotal,      stImgListState.u32RelUsrSuccess,
    pstPortPrc[0]->stFbPrc.u32RelTotal, pstPortPrc[0]->stFbPrc.u32RelSuccess,
    pstPortPrc[1]->stFbPrc.u32RelTotal, pstPortPrc[1]->stFbPrc.u32RelSuccess,
    pstPortPrc[2]->stFbPrc.u32RelTotal, pstPortPrc[2]->stFbPrc.u32RelSuccess
    );


    #endif




READFREE:
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
    return HI_SUCCESS;

}
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v)
{
	HI_CHAR asNodeState[VPSS_HAL_TASK_NODE_BUTT] = {0};
    VPSS_INSTANCE_S* pstInstance;
	VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_IN_ENTITY_S* pstInEntity;
    VPSS_IN_IMAGELIST_STATE_S stImgListState;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    HI_S32 s32SrcModuleID;
    HI_U32 u32Count;
    pProcItem = p->private;

    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)pProcItem->data);

    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)pProcItem->data);
        return HI_FAILURE;
    }

	pstInEntity = &(pstInstance->stInEntity);
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);

    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");

            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));

    }

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);
    }

    s32SrcModuleID = (pstInstance->hDst & 0x00ff0000) >>16;

	for(u32Count = 0; u32Count < VPSS_HAL_TASK_NODE_BUTT; u32Count++)
	{
		asNodeState[u32Count] = (pstInstance->abNodeVaild[u32Count] == HI_TRUE) ? '1':'0';
	}

    PROC_PRINT(p,
        "--------VPSS%04x---------------|"   "------------------------PortInfo--------------------------|\n"
        "ID               :0x%-8d   |"      "ID               :0x%-8d  |0x%-8d  |0x%-8d    |\n"
        "State/Pause      :%-10s   |"         "State            :%-3s         |%-3s         |%-3s           |\n"
        "SourceFormat     :%-10s   |"        "PixelFormat      :%-12s|%-12s|%-12s  |\n"
        "QuickOutPut      :%-10s   |"        "OutResolution    :%4d*%-4d   |%4d*%-4d   |%4d*%-4d     |\n"
        "SourceID         :%-6s(%02x)   |"   "ColorSpace       :%-12s|%-12s|%-12s  |\n"
        "Version          :%-10s   |"        "DispPixelAR(W/H) :%2d/%-2d       |%2d/%-2d       |%2d/%-2d         |\n"
        "                               |"   "Aspect Mode      :%-12s|%-12s|%-12s  |\n"
        "                               |"   "Support3DStream  :%-12s|%-12s|%-12s  |\n"
        "                               |"   "MaxFrameRate     :%-5d       |%-5d       |%-5d         |\n"
        "-------- Algorithm-------------|"   "*LowDelay        :%-12s|%-12s|%-12s  |\n"
        "P/I Setting   :%-10s      |"        "HorizonFlip      :%-12s|%-12s|%-12s  |\n"
        "Deinterlace   :%-10s      |"        "VerticalFlip     :%-12s|%-12s|%-12s  |\n"
        "Sharpness     :%-10s      |"        "Rotation         :%-12s|%-12s|%-12s  |\n"
		"*Trans        :%-10s      |"        "Incrop           :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
        "*ProgRevise   :%-10s      |"        "Usercrop         :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
		"*BandOptimize :%-10s      |"		 "VideoRect        :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
        "--------Detect Info------------|"   "InResolution     :%4d*%-4d   |%4d*%-4d   |%4d*%-4d     |\n"
        "TopFirst(Src):%6s(%-6s)   |"        "UsercropEn       :%-12s|%-12s|%-12s  |\n"
        "InRate(Src)  :%6d(%-6d)   |"        "										  |\n"
        "Progressive/Interlace(Src):%-1s(%-1s)|""KeyFrameEn       :%-12s|%-12s|%-12s  |\n",

        /* attribute */
        pstInstance->ID,
        pstInstance->ID,
                                                pstPortPrc[0]->s32PortId,
                                                pstPortPrc[1]->s32PortId,
                                                pstPortPrc[2]->s32PortId,
        g_pInstState[pstInstance->enState],
												g_pAlgModeString[pstInstance->stPort[0].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[1].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[2].bEnble],

        ((pstStreamInfo->ePixFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
        g_pPixString[pstStreamInfo->ePixFormat - HI_DRV_PIX_FMT_NV12]:
        g_pPixString[7],
                                                ((pstPortPrc[0]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[0]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],
                                                ((pstPortPrc[1]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[1]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],
                                                ((pstPortPrc[2]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[2]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],

        g_pAlgModeString[pstInstance->bAlwaysFlushSrc],
                                                (pstPortPrc[0]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[0]->s32OutputWidth,
                                                (pstPortPrc[0]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[0]->s32OutputHeight,
                                                (pstPortPrc[1]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[1]->s32OutputWidth,
                                                (pstPortPrc[1]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[1]->s32OutputHeight,
                                                (pstPortPrc[2]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[2]->s32OutputWidth,
                                                (pstPortPrc[2]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[2]->s32OutputHeight,


        (s32SrcModuleID >= HI_ID_VFMW && s32SrcModuleID <= HI_ID_VENC)?
         g_pSrcModuleString[s32SrcModuleID - HI_ID_VFMW]:
        (s32SrcModuleID == 0?g_pSrcModuleString[0]:
         g_pSrcModuleString[9]),
         (pstInstance->hDst & 0x000000ff),
                                                g_pCscString[pstPortPrc[0]->eDstCS],
                                                g_pCscString[pstPortPrc[1]->eDstCS],
                                                g_pCscString[pstPortPrc[2]->eDstCS],
         DEF_SDK_VERSIO_LOG,
                                                pstPortPrc[0]->stDispPixAR.u32ARw,
                                                pstPortPrc[0]->stDispPixAR.u32ARh,
                                                pstPortPrc[1]->stDispPixAR.u32ARw,
                                                pstPortPrc[1]->stDispPixAR.u32ARh,
                                                pstPortPrc[2]->stDispPixAR.u32ARw,
                                                pstPortPrc[2]->stDispPixAR.u32ARh,

                                                g_pAspString[pstPortPrc[0]->eAspMode],
                                                g_pAspString[pstPortPrc[1]->eAspMode],
                                                g_pAspString[pstPortPrc[2]->eAspMode],

                                                g_pAlgModeString[pstPortPrc[0]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[1]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[2]->b3Dsupport],
//         asNodeState,
                                                pstPortPrc[0]->u32MaxFrameRate,
                                                pstPortPrc[1]->u32MaxFrameRate,
                                                pstPortPrc[2]->u32MaxFrameRate,
        /*alg config*/                             g_pAlgModeString[pstPortPrc[0]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[1]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[2]->bTunnelEnable],

        g_pProgDetectString[pstInEntity->enProgInfo],
                                                g_pAlgModeString[pstPortPrc[0]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bHoriFlip],
        g_pDeiString[pstInstance->stProcCtrl.eDEI],
                                                g_pAlgModeString[pstPortPrc[0]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bVertFlip],
        g_pAlgModeString[pstInstance->stProcCtrl.eSharpness],
                                                g_pRotationString[pstPortPrc[0]->enRotation],
                                                g_pRotationString[pstPortPrc[1]->enRotation],
                                                g_pRotationString[pstPortPrc[2]->enRotation],
		(pstInstance->stInEntity.stTransFbInfo.bNeedTrans == HI_TRUE)? "ON":"OFF",

                                                pstInstance->stPort[0].stInRect.s32X,pstInstance->stPort[0].stInRect.s32Y,
                                                pstInstance->stPort[0].stInRect.s32Width,pstInstance->stPort[0].stInRect.s32Height,
                                                pstInstance->stPort[1].stInRect.s32X,pstInstance->stPort[1].stInRect.s32Y,
                                                pstInstance->stPort[1].stInRect.s32Width,pstInstance->stPort[1].stInRect.s32Height,
                                                pstInstance->stPort[2].stInRect.s32X,pstInstance->stPort[2].stInRect.s32Y,
                                                pstInstance->stPort[2].stInRect.s32Width,pstInstance->stPort[2].stInRect.s32Height,

		 g_pAlgModeString[pstInEntity->bProgRevise],
                                                pstInstance->stPort[0].stCropRect.u32LeftOffset,pstInstance->stPort[0].stCropRect.u32TopOffset,
                                                pstInstance->stPort[0].stCropRect.u32RightOffset,pstInstance->stPort[0].stCropRect.u32BottomOffset,
                                                pstInstance->stPort[1].stCropRect.u32LeftOffset,pstInstance->stPort[1].stCropRect.u32TopOffset,
                                                pstInstance->stPort[1].stCropRect.u32RightOffset,pstInstance->stPort[1].stCropRect.u32BottomOffset,
                                                pstInstance->stPort[2].stCropRect.u32LeftOffset,pstInstance->stPort[2].stCropRect.u32TopOffset,
                                                pstInstance->stPort[2].stCropRect.u32RightOffset,pstInstance->stPort[2].stCropRect.u32BottomOffset,
		(pstInstance->bEnableTNR  == HI_TRUE)? "OFF":"ON",
                                                pstInstance->stPort[0].stVideoRect.s32X,pstInstance->stPort[0].stVideoRect.s32Y,
                                                pstInstance->stPort[0].stVideoRect.s32Width,pstInstance->stPort[0].stVideoRect.s32Height,
                                                pstInstance->stPort[1].stVideoRect.s32X,pstInstance->stPort[1].stVideoRect.s32Y,
                                                pstInstance->stPort[1].stVideoRect.s32Width,pstInstance->stPort[1].stVideoRect.s32Height,
                                                pstInstance->stPort[2].stVideoRect.s32X,pstInstance->stPort[2].stVideoRect.s32Y,
                                                pstInstance->stPort[2].stVideoRect.s32Width,pstInstance->stPort[2].stVideoRect.s32Height,

                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,
                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,
                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,

        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        ((pstStreamInfo->u32RealTopFirst == 0)?"Bottom":"Top"):"NA",
        (pstStreamInfo->u32StreamTopFirst == 0)?"Bottom":"Top",

                                                g_pAlgModeString[pstInstance->stPort[0].bUseCropRect],
                                                g_pAlgModeString[pstInstance->stPort[1].bUseCropRect],
                                                g_pAlgModeString[pstInstance->stPort[2].bUseCropRect],
        pstStreamInfo->u32InRate*1000,pstStreamInfo->u32StreamInRate,

        (pstStreamInfo->u32StreamProg == 0)?
        "I":"P",
        (pstStreamInfo->u32StreamTopFirst == 0)?"I":"P" ,
                                                g_pAlgModeString[pstInstance->stPort[0].bOnlyKeyFrame],
                                                g_pAlgModeString[pstInstance->stPort[1].bOnlyKeyFrame],
                                                g_pAlgModeString[pstInstance->stPort[2].bOnlyKeyFrame]
        );


    PROC_PRINT(p,
    "-----SourceFrameList Info------|"  "--------------------OutFrameList Info---------------------|\n"
    "      (source to vpss)         |"  "                     (vpss to sink)                       |\n"
    "*Mutual Mode  :%-11s     |"        "BufManager       :%-10s  |%-10s   |%-10s   |\n"
    "SrcChangeCnt  :%-11d     |"  "BufNumber        :%-2d+%-2d       |%-2d+%-2d        |%-2d+%-2d        |\n"
    "GetSrcImgHZ(Try/OK)  :%3d/%-3d  |" "BufFul           :%-2d          |%-2d           |%-2d           |\n"
    "GetOutBufHZ(Try/OK)  :%3d/%-3d  |" "BufEmpty         :%-2d          |%-2d           |%-2d           |\n"
    "ProcessHZ(Try/OK)    :%3d/%-3d  |" "AcquireHZ        :%-10d  |%-10d   |%-10d   |\n"
    "SrcList(PUT/COMPLETE/RELEASE): |"  "Acquire(Try/OK):              |             |             |\n"
    " %6d/%-6d/%-6d          |"         "              %6d/%-6d   |%6d/%-6d|%6d/%-6d|\n"
    "WbcList(COMPLETE[L/R]):        |"  "Release(Try/OK):              |             |             |\n"
    " %6d/%-6d                 |"       "              %6d/%-6d   |%6d/%-6d|%6d/%-6d|\n"
    "StWbcList(COMPLETE[L/R]):      |"  "OutRate          :%-10d  |%-10d   |%-10d   |\n"
    " %6d/%-6d                 |"       "Releasing        :%-10d  |%-10d   |%-10d   |\n"
    "StDieList(COMPLETE[L/R]):      |"  "Waiting          :%-10d  |%-10d   |%-10d   |\n"
    " %6d/%-6d                 |"       "Working          :%-10d  |%-10d   |%-10d   |\n"
    "StNrlList(COMPLETE[L/R]):      |\n"
    " %6d/%-6d                 |\n",

    g_pSrcMutualString[pstInstance->eSrcImgMode],
                                        g_pBufTypeString[pstPortPrc[0]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[1]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[2]->stBufListCfg.eBufType],

    pstInstance->u32ScenceChgCnt,
    pstPortPrc[0]->stBufListCfg.u32BufNumber,
    pstPortPrc[0]->stFbPrc.u32ExtListNumb,
    pstPortPrc[1]->stBufListCfg.u32BufNumber,
    pstPortPrc[1]->stFbPrc.u32ExtListNumb,
    pstPortPrc[2]->stBufListCfg.u32BufNumber,
    pstPortPrc[2]->stFbPrc.u32ExtListNumb,
    pstInstance->u32ImgRate,
    pstInstance->u32ImgSucRate,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb,
    pstInstance->u32BufRate,
    pstInstance->u32BufSucRate,
                                        pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stFbPrc.u32EmptyListNumb,
    pstInstance->u32CheckRate,
    pstInstance->u32CheckSucRate,
                                        pstPortPrc[0]->stFbPrc.u32GetHZ,
                                        pstPortPrc[1]->stFbPrc.u32GetHZ,
                                        pstPortPrc[2]->stFbPrc.u32GetHZ,

    pstInEntity->pstSrc->u32PutSrcCount,pstInEntity->pstSrc->u32CompleteSrcCount,pstInEntity->pstSrc->u32ReleaseSrcCount,
                                        pstPortPrc[0]->stFbPrc.u32GetTotal, pstPortPrc[0]->stFbPrc.u32GetSuccess,
                                        pstPortPrc[1]->stFbPrc.u32GetTotal, pstPortPrc[1]->stFbPrc.u32GetSuccess,
                                        pstPortPrc[2]->stFbPrc.u32GetTotal, pstPortPrc[2]->stFbPrc.u32GetSuccess,

    pstInEntity->pstWbcInfo[0]->u32CompleteCount,pstInEntity->pstWbcInfo[1]->u32CompleteCount,
                                        pstPortPrc[0]->stFbPrc.u32RelTotal, pstPortPrc[0]->stFbPrc.u32RelSuccess,
                                        pstPortPrc[1]->stFbPrc.u32RelTotal, pstPortPrc[1]->stFbPrc.u32RelSuccess,
                                        pstPortPrc[2]->stFbPrc.u32RelTotal, pstPortPrc[2]->stFbPrc.u32RelSuccess,

                                        pstPortPrc[0]->stFbPrc.u32OutRate,
                                        pstPortPrc[1]->stFbPrc.u32OutRate,
                                        pstPortPrc[2]->stFbPrc.u32OutRate,
    pstInEntity->pstSttWbc[0]->u32Cnt,pstInEntity->pstSttWbc[1]->u32Cnt,
                                        pstPortPrc[0]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[1]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[2]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb - pstPortPrc[0]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb - pstPortPrc[1]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb - pstPortPrc[2]->stFbPrc.u32WaitSinkRlsNumb,
    pstInEntity->pstDieStInfo[0]->u32Cnt,pstInEntity->pstDieStInfo[1]->u32Cnt,
                                        pstPortPrc[0]->stBufListCfg.u32BufNumber + pstPortPrc[0]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb - pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stBufListCfg.u32BufNumber + pstPortPrc[1]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb - pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stBufListCfg.u32BufNumber + pstPortPrc[2]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb - pstPortPrc[2]->stFbPrc.u32EmptyListNumb,

    pstInEntity->pstNrMadInfo[0]->u32Cnt,pstInEntity->pstNrMadInfo[1]->u32Cnt
    );





READFREE:
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
    return HI_SUCCESS;

}

#endif


HI_S32 VPSS_CTRL_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file   *s = file->private_data;
    DRV_PROC_ITEM_S  *pProcItem = s->private;
    VPSS_HANDLE hVpss;
    HI_CHAR  chCmd[60] = {0};
    HI_CHAR  chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg2[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg3[DEF_FILE_NAMELENGTH] = {0};
    HI_S32   s32Ret;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_DBG_INST_S *pstInstDbg;

    hVpss = (VPSS_HANDLE)pProcItem->data;
    pstInstance = VPSS_CTRL_GetInstance(hVpss);
    if (pstInstance == HI_NULL)
    {
        VPSS_FATAL("Can't Get Debug Instance.\n");
        return count;
    }

    if(count > 40)
    {
        VPSS_FATAL("Error:Echo too long.\n");
        return (-1);
    }

    if(copy_from_user(chCmd,buf,count))
    {
        VPSS_FATAL("copy from user failed\n");
        return (-1);
    }

    pstInstDbg = &(pstInstance->stDbgCtrl.stInstDbg);

    VPSS_OSAL_GetProcArg(chCmd, chArg1, 1);
    VPSS_OSAL_GetProcArg(chCmd, chArg2, 2);
    VPSS_OSAL_GetProcArg(chCmd, chArg3, 3);

    if (chArg1[0] == 'h' && chArg1[1] == 'e' && chArg1[2] == 'l' && chArg1[3] == 'p')
    {

        HI_DRV_PROC_EchoHelper("-------------------VPSS debug options--------------------     \n"
               "you can perform VPSS debug with such command                  \n"
               "echo [arg1] [arg2] [arg3] > /proc/msp/vpssXX\n                \n\n"
               "debug action       arg1              arg2              arg3    \n"
               "-------------    ----------   ---------------------  -----------\n"
               " save one yuv     saveyuv     src/port0/port1/port2             \n"
               " print frameinfo  printinfo   src/port0/port1/port2              \n"
               " turn off info    none        src/port0/port1/port2               \n");
        HI_DRV_PROC_EchoHelper("\n"
               " set progress/interlace on/off   i2p		 src				on/off\n"
               " set vpssbypass                setbypass	on/off\n"
               " set uvinvert                   uvinvert   port0/port1/port2	on/off \n"
               " set cmpon                      cmpon      port0/port1/port2		   \n"
               " set cmpoff                     cmpoff     port0/port1/port2		   \n"
               " set rate                       setrate     on/off            framerate\n");
    }
    else
    {
        VPSS_DBG_CMD_S stDbgCmd;

        if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_YUV,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_YUV;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_DBG,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_INFO_FRM;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_NONE,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_INFO_NONE;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_STREAM,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_STREAM;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_LOW,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_LOW;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_MIDDLE,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_MID;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_HIGH,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_HIGH;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_USR,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_USR;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SET_UV,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UV_INVERT;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_CMP_ON,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_CMP_ON;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_CMP_OFF,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_CMP_OFF;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SET_I2P,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_I2P;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETBYPASS,DEF_FILE_NAMELENGTH))
        {
            if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->unInfo.bits.vpssbypass = 1;
            }
            else
            {
                pstInstDbg->unInfo.bits.vpssbypass = 0;
            }

            return count;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETRATE,DEF_FILE_NAMELENGTH))
        {
            if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->unInfo.bits.frameEn = 1;
            }
            else
            {
                pstInstDbg->unInfo.bits.frameEn = 0;
            }

            if(strlen(chArg3) == 0)
            {
                pstInstDbg->unInfo.bits.framerate = pstInstance->stInEntity.stStreamInfo.u32InRate;
            }
            else
            {
                pstInstDbg->unInfo.bits.framerate = (HI_U32)(simple_strtol(chArg3, NULL, 10));
            }

            return count;

        }
        else
        {
            VPSS_FATAL("Cmd Can't Support\n");
            goto PROC_EXIT;
        }

        s32Ret = HI_SUCCESS;
        if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SRC,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_SRC_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_0,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT0_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_1,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT1_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_2,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT2_ID;
        }
        else
        {
            VPSS_FATAL("Invalid para2 %s\n",chArg2);
            goto PROC_EXIT;
        }

        s32Ret = HI_SUCCESS;
        if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgValue = HI_TRUE;
        }
        else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_OFF,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgValue = HI_FALSE;
        }
        else
        {
        }

#if DEF_VPSS_DEBUG
        VPSS_DBG_SendDbgCmd(&(pstInstance->stDbgCtrl), &stDbgCmd);
#endif
    }
PROC_EXIT:

    return count;
}





#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

