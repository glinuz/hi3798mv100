#include "drv_vbi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef __DISP_PLATFORM_BOOT__
HI_DISP_VBI_STATUS_S g_stVBI[(HI_HANDLE)HI_DRV_DISP_VBI_TYPE_BUTT];

TTX_SEND_S  g_TtxSend;
VBI_SEND_S  g_VbiSend;

HI_VOID DRV_VBI_VbiIsr(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_S32 nBtmFlag;

    nBtmFlag = !pstInfo->stDispInfo.bIsBottomField;
    if (g_VbiSend.CcListFlag != HI_TRUE)
    {
        if (list_empty(&g_VbiSend.CcBlockBusyHead))
        {
            VDP_VBI_CCLineConfig(HI_FALSE, HI_FALSE, 0, 0);
        }
        else
        {
            /*judge whether the first time to enter the node.
            comfire configure both fields CC data in bottom field each time.
        */
            if(g_VbiSend.CcSendStep == CC_SENDSTEP_START)
            {
                if(nBtmFlag)
                    return ;

                g_VbiSend.CcSendStep = CC_SENDSTEP_PROCESSING;
                g_VbiSend.pCcBusyList = g_VbiSend.CcBlockBusyHead.next;
                g_VbiSend.pCcBusyBlock = list_entry(g_VbiSend.pCcBusyList, CC_DATA_BLOCK_S, List);
                g_VbiSend.CcSendStep = CC_SENDSTEP_PROCESSING;
      
                VDP_VBI_CCDataConfig(g_VbiSend.pCcBusyBlock->TopData, g_VbiSend.pCcBusyBlock->BottomData);
                VDP_VBI_CCLineConfig(g_VbiSend.pCcBusyBlock->TopFlag,g_VbiSend.pCcBusyBlock->BottomFlag,g_VbiSend.pCcBusyBlock->TopLine,g_VbiSend.pCcBusyBlock->BottomLine); 


                return;
            }

            list_del(g_VbiSend.pCcBusyList);
            list_add_tail(g_VbiSend.pCcBusyList, &g_VbiSend.CcBlockFreeHead);

            g_VbiSend.CcSendStep = CC_SENDSTEP_START;

        }
    }

    /*
    //config register directly
    if (g_VbiSend.WssData.bEnable)
    {
    DRV_VOU_WSSConfig(HI_TRUE, g_VbiSend.WssData.u16Data);
    }
    */
    /* WssListFlag = HI_FALSE */
    if (g_VbiSend.WssListFlag != HI_TRUE)
    {
        if (!list_empty(&g_VbiSend.WssBlockBusyHead))
        {
            g_VbiSend.pWssBusyList = g_VbiSend.WssBlockBusyHead.next;
            g_VbiSend.pWssBusyBlock = list_entry(g_VbiSend.pWssBusyList, WSS_DATA_BLOCK_S, List);
            list_del(g_VbiSend.pWssBusyList);

            if (!g_VbiSend.WssData.bEnable)
            {
                VDP_VBI_WSSConfig(HI_TRUE, g_VbiSend.pWssBusyBlock->TopData);
            }
            list_add_tail(g_VbiSend.pWssBusyList, &g_VbiSend.WssBlockFreeHead);
        }
    }

    return ;
}
HI_VOID DRV_VBI_TtxIsr(HI_VOID* hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    HI_DRV_DISP_FMT_E enFormat = HI_DRV_DISP_FMT_BUTT;

    if (!VDP_VBI_IntStatus())
    {
        return;
    }
    else
    {
        VDP_VBI_TtxIntClear();
    }
    
    if (g_TtxSend.TtxListFlag)
    {
        g_TtxSend.TtxBusyFlag = HI_FALSE;
        return;
    }

    if (g_TtxSend.pTtxBusyList)
    {
        list_add_tail(g_TtxSend.pTtxBusyList, &g_TtxSend.TtxBlockFreeHead);
        g_TtxSend.pTtxBusyList = NULL;
    }

    if (list_empty(&g_TtxSend.TtxBlockBusyHead))
    {
        g_TtxSend.TtxBusyFlag = HI_FALSE;
        return;
    }

    g_TtxSend.pTtxBusyList = g_TtxSend.TtxBlockBusyHead.next;
    g_TtxSend.pTtxBusyBlock = list_entry(g_TtxSend.pTtxBusyList, TTX_DATA_BLOCK_S, List);
    list_del(g_TtxSend.pTtxBusyList);


    if (pstInfo->stDispInfo.stFmtResolution.s32Width == 720
        && pstInfo->stDispInfo.stFmtResolution.s32Height == 576)
    {
        enFormat = HI_DRV_DISP_FMT_PAL;
    }
    else if (pstInfo->stDispInfo.stFmtResolution.s32Width == 720
        && pstInfo->stDispInfo.stFmtResolution.s32Height == 480)
    {
        enFormat = HI_DRV_DISP_FMT_NTSC;
    }
    else
	{
        enFormat = HI_DRV_DISP_FMT_BUTT;
    }

    /*set mode  for NTSC or PAL*/
    if ((enFormat == HI_DRV_DISP_FMT_PAL)
            || (enFormat == HI_DRV_DISP_FMT_PAL_N)
            || (enFormat == HI_DRV_DISP_FMT_PAL_Nc)
       )
    {
        VDP_VBI_TtxSetMode(0x1);

        /*if 625i select line 7 ~22*/
        g_TtxSend.pTtxBusyBlock->TtxDataLine = 0xffffffff;/*7 ~22 line */
        g_TtxSend.pTtxBusyBlock->TtxDataExtLine = 0x6;/*add sixth line */
    }
    else if ((enFormat == HI_DRV_DISP_FMT_NTSC)
            || (enFormat == HI_DRV_DISP_FMT_NTSC_J)
            || (enFormat == HI_DRV_DISP_FMT_PAL_M))
    {
        VDP_VBI_TtxSetMode(0x2); 

        /*if 525i select line 10 ~21*/
        g_TtxSend.pTtxBusyBlock->TtxDataLine = 0x7ff87ff8;/*10 ~21*/
        g_TtxSend.pTtxBusyBlock->TtxDataExtLine = 0x00;
    }
    
    g_TtxSend.pTtxBusyBlock->TtxPackConut = 0;

    VDP_VBI_TtxSetSeq(g_TtxSend.pTtxBusyBlock->TtxSeqFlag);
    udelay(5);
    VDP_VBI_TtxLineEnable(g_TtxSend.pTtxBusyBlock->TtxDataLine, g_TtxSend.pTtxBusyBlock->TtxDataExtLine);
    udelay(5);
//    VDP_VBI_TtxSetPackOff(TTX_PACKET_LENGTH);
    udelay(5);
    VDP_VBI_TtxSetAddr((g_TtxSend.pTtxBusyBlock->TtxStartAddr), (g_TtxSend.pTtxBusyBlock->TtxEndAddr));
    udelay(5);
    VDP_VBI_TtxSetReady();

    return;
}

HI_S32 DRV_VBI_InitTtx(HI_VOID)
{
    HI_DRV_DISP_CALLBACK_S stTtxISR;
    
    HI_U32    i;
    HI_S32    Ret;

    Ret = HI_DRV_MMZ_AllocAndMap("DISP_TtxPes", HI_NULL, BUFFER_LEN_16K, 0, &g_TtxSend.TtxPesBuf);
    if (Ret != 0)
    {
        HI_ERR_DISP("malloc TtxPes buf failed.\n");
        return HI_FAILURE;
    }

    Ret = HI_DRV_MMZ_AllocAndMap("DISP_TtxData", HI_NULL, TTX_BLOCK_NUM*BUFFER_LEN_7K, 0, &g_TtxSend.TtxDataBuf);
    if (Ret != 0)
    {
        HI_ERR_DISP("malloc TtxData buf failed.\n");
        HI_DRV_MMZ_UnmapAndRelease(&g_TtxSend.TtxPesBuf);
        return HI_FAILURE;
    }

    g_TtxSend.pPesBufVirtAddr = (HI_U8 *)g_TtxSend.TtxPesBuf.pu8StartVirAddr;
    g_TtxSend.TtxPesBufWrite = 0;

    g_TtxSend.pTtxBufVirtAddr = (HI_U8 *)g_TtxSend.TtxDataBuf.pu8StartVirAddr;

    INIT_LIST_HEAD(&g_TtxSend.TtxBlockFreeHead);
    INIT_LIST_HEAD(&g_TtxSend.TtxBlockBusyHead);

    g_TtxSend.pTtxBusyList = NULL;
    g_TtxSend.pTtxBusyBlock = NULL;
    g_TtxSend.TtxBusyFlag = HI_FALSE;
    g_TtxSend.TtxListFlag = HI_FALSE;

    for (i=0; i<TTX_BLOCK_NUM; i++)
    {
        g_TtxSend.TtxDataBlock[i].pTtxDataVirtAddr = (HI_U8 *)(g_TtxSend.TtxDataBuf.pu8StartVirAddr + i*BUFFER_LEN_7K);
        g_TtxSend.TtxDataBlock[i].TtxStartAddr = g_TtxSend.TtxDataBuf.u32StartPhyAddr + i*BUFFER_LEN_7K;
        g_TtxSend.TtxDataBlock[i].TtxPackConut = 0;
        list_add_tail(&g_TtxSend.TtxDataBlock[i].List, &g_TtxSend.TtxBlockFreeHead);
    }

                            
    /* setting of TTX mode */

    VDP_VBI_TtxSetPackOff(TTX_PACKET_LENGTH);
	udelay(20);
	
    VDP_VBI_TtxPiorityHighest(HI_TRUE);
    udelay(20);
	
    VDP_VBI_SetTtxFullPage(HI_FALSE);
    udelay(20);

    /* VBI data filtering enable */
    VDP_VBI_VbiFilterEnable(HI_TRUE);
	udelay(20);

    VDP_VBI_TtxSetMode(0x1);    
	udelay(20);
    
#if 1
    stTtxISR.pfDISP_Callback = DRV_VBI_TtxIsr;
	stTtxISR.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    stTtxISR.hDst = (HI_VOID *)HI_INVALID_HANDLE;

    /*if config ttx data at 90% postion, ttx finish will cast more two flieds time.*/
    DISP_ISR_RegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_0_PERCENT,
                            &stTtxISR);
#endif
    VDP_VBI_TtxEnableInt();
    udelay(20); 

    return HI_SUCCESS;
}


HI_S32 DRV_VBI_DeInitTtx(HI_VOID)
{
    HI_DRV_DISP_CALLBACK_S stTtxISR;

#if 1
    INIT_LIST_HEAD(&g_TtxSend.TtxBlockFreeHead);
    INIT_LIST_HEAD(&g_TtxSend.TtxBlockBusyHead);

    VDP_VBI_TtxIntClear();

    VDP_VBI_TtxDisableInt();
    VDP_VBI_VbiFilterEnable(HI_FALSE);
    VDP_VBI_TtxPiorityHighest(HI_FALSE);
	
	//before unmap data,sleeping can avoid reading invalid data.
	//waiting for the last IRQ
	msleep(20);
	
    stTtxISR.pfDISP_Callback = DRV_VBI_TtxIsr;
	stTtxISR.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    stTtxISR.hDst = (HI_VOID*)HI_INVALID_HANDLE;
    DISP_ISR_UnRegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_0_PERCENT,
                              &stTtxISR);
	
    HI_DRV_MMZ_UnmapAndRelease(&g_TtxSend.TtxPesBuf);
    HI_DRV_MMZ_UnmapAndRelease(&g_TtxSend.TtxDataBuf);
#endif


    return HI_SUCCESS;
}


HI_S32 DRV_VBI_InitVbi(HI_VOID)
{
    HI_U32    i;
    HI_S32    Ret;
    HI_DRV_DISP_CALLBACK_S stVbiISR;

    Ret = HI_DRV_MMZ_AllocAndMap("DISP_VbiPes", HI_NULL, BUFFER_LEN_16K, 0, &g_VbiSend.VbiPesBuf);
    if (Ret != 0)
    {
        HI_ERR_DISP("malloc VbiPes buf failed.\n");
        return HI_FAILURE;
    }

    g_VbiSend.pPesBufVirtAddr = (HI_U8 *)g_VbiSend.VbiPesBuf.pu8StartVirAddr;
    g_VbiSend.VbiPesBufWrite = 0;

    INIT_LIST_HEAD(&g_VbiSend.CcBlockFreeHead);
    INIT_LIST_HEAD(&g_VbiSend.CcBlockBusyHead);

    for (i=0; i<CC_BLOCK_NUM; i++)
    {
        list_add_tail(&g_VbiSend.CcDataBlock[i].List, &g_VbiSend.CcBlockFreeHead);
    }

    g_VbiSend.pCcBusyList  = NULL;
    g_VbiSend.pCcBusyBlock = NULL;
    g_VbiSend.CcListFlag   = HI_FALSE;
    g_VbiSend.CcSendStep   = CC_SENDSTEP_START;
    
    
    VDP_VBI_CCSetSeq(HI_TRUE);

    INIT_LIST_HEAD(&g_VbiSend.WssBlockFreeHead);
    INIT_LIST_HEAD(&g_VbiSend.WssBlockBusyHead);

    for (i=0; i<CC_BLOCK_NUM; i++)
    {
        list_add_tail(&g_VbiSend.WssDataBlock[i].List, &g_VbiSend.WssBlockFreeHead);
    }

    g_VbiSend.pWssBusyList    = NULL;
    g_VbiSend.pWssBusyBlock   = NULL;
    g_VbiSend.WssListFlag     = HI_FALSE;

    g_VbiSend.WssData.bEnable = HI_FALSE;
    g_VbiSend.WssData.u16Data = 0;

    VDP_VBI_WSSConfig(g_VbiSend.WssData.bEnable, g_VbiSend.WssData.u16Data);

    stVbiISR.pfDISP_Callback = DRV_VBI_VbiIsr;
	stVbiISR.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    stVbiISR.hDst = (HI_VOID *)HI_INVALID_HANDLE;
    
    DISP_ISR_RegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_90_PERCENT,
                            &stVbiISR);
    /*DRV_VOU_WSSSetSeq(HI_TRUE);*/

    return HI_SUCCESS;
}

HI_S32 DRV_VBI_DeInitVbi(HI_VOID)
{
    HI_DRV_DISP_CALLBACK_S stVbiISR;
    
    HI_DRV_MMZ_UnmapAndRelease(&g_VbiSend.VbiPesBuf);

    stVbiISR.pfDISP_Callback = DRV_VBI_VbiIsr;
	stVbiISR.eCallBackPrior = HI_DRV_DISP_CALLBACK_PRIORTY_MIDDLE;
    stVbiISR.hDst = (HI_VOID*)HI_INVALID_HANDLE;
	
#if 1
    DISP_ISR_UnRegCallback(HI_DRV_DISPLAY_0, HI_DRV_DISP_C_INTPOS_90_PERCENT,
                           &stVbiISR);
#endif
    /* shutdown CC WSS */
    VDP_VBI_CCLineConfig(HI_FALSE, HI_FALSE, 0, 0);
    VDP_VBI_WSSConfig(HI_FALSE, 0);

    return HI_SUCCESS;
}



HI_S32 DRV_VBI_CreateChannel(HI_DRV_DISP_VBI_CFG_S* pstCfg, HI_HANDLE *phVbi)
{
    HI_S32 s32Ret;
    


    switch (pstCfg->eType)
	{
		case HI_DRV_DISP_VBI_TTX:
			if (g_stVBI[HI_DRV_DISP_VBI_TTX].bOpen == HI_TRUE) 
			{
				HI_ERR_DISP("DISP:Vbi Channel has been Created !.\n");
				return HI_FAILURE;
			}
			s32Ret = DRV_VBI_InitTtx();
			break;
		case HI_DRV_DISP_VBI_CC:
		case HI_DRV_DISP_VBI_WSS:
			s32Ret = DRV_VBI_InitVbi();
			break;
		case HI_DRV_DISP_VBI_VCHIP:
			s32Ret = HI_ERR_DISP_INVALID_OPT;
		default:
			s32Ret = HI_ERR_DISP_INVALID_OPT;
	}
    
    VDP_VBI_VbiFilterEnable(HI_TRUE);
    
    if (HI_SUCCESS == s32Ret )
    {
        *phVbi = (HI_HANDLE)pstCfg->eType;
        
        g_stVBI[(HI_HANDLE)(*phVbi)].bOpen = HI_TRUE;
        g_stVBI[(HI_HANDLE)(*phVbi)].enVBIType = pstCfg->eType;
        g_stVBI[(HI_HANDLE)(*phVbi)].u32Handle =*phVbi;
    }
    else
    {
        *phVbi = HI_INVALID_HANDLE;
    }
    
    return s32Ret;
}

HI_S32 DRV_VBI_DestoryChannel(HI_HANDLE hVbi)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    HI_DRV_DISP_VBI_TYPE_E  eVBIType;

    eVBIType = (HI_DRV_DISP_VBI_TYPE_E) hVbi;

    VbiCheckType(eVBIType);

    if (HI_FALSE == g_stVBI[hVbi].bOpen)
    {
        HI_ERR_DISP("DISP: Channel has been Destroy !.\n");
        return HI_ERR_DISP_DESTROY_ERR;
    }
     
    switch (eVBIType)
    {
        case HI_DRV_DISP_VBI_TTX:

            s32Ret = DRV_VBI_DeInitTtx();
            break;
        case HI_DRV_DISP_VBI_CC:
        case HI_DRV_DISP_VBI_WSS:
            s32Ret = DRV_VBI_DeInitVbi();
            break;
        default:
            s32Ret = HI_ERR_DISP_INVALID_OPT;
    }
    
    if (HI_SUCCESS == s32Ret )
    {
        g_stVBI[hVbi].bOpen = HI_FALSE;
        g_stVBI[hVbi].enVBIType = HI_DRV_DISP_VBI_TYPE_BUTT;
        g_stVBI[hVbi].u32Handle = HI_INVALID_HANDLE;
    }
    
    return s32Ret;
}

HI_VOID DRV_VBI_TtxLineParse(TTX_DATA_BLOCK_S *pTtxBlock, HI_U8 LineNum)
{
    if (LineNum & 0x20)
    {

        switch (LineNum & 0x1f)
        {
            case 6:
                pTtxBlock->TtxDataExtLine = pTtxBlock->TtxDataExtLine | 0x4;
                break;

            case 7:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00010000;
                break;

            case 8:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00020000;
                break;
            case 9:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00040000;
                break;
            case 10:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00080000;
                break;
            case 11:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00100000;
                break;
            case 12:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00200000;
                break;
            case 13:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00400000;
                break;
            case 14:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00800000;
                break;
            case 15:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x01000000;
                break;
            case 16:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x02000000;
                break;
            case 17:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x04000000;
                break;
            case 18:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x08000000;
                break;
            case 19:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x10000000;
                break;
            case 20:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x20000000;
                break;
            case 21:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x40000000;
                break;
            case 22:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x80000000;
                break;
            default :
                break;

        }
    }
    else
    {
        switch (LineNum & 0x1f)
        {
            case 5:
                pTtxBlock->TtxDataExtLine = pTtxBlock->TtxDataExtLine | 0x1;
                break;

            case 6:
                pTtxBlock->TtxDataExtLine = pTtxBlock->TtxDataExtLine | 0x2;
                break;

            case 7:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000001;
                break;

            case 8:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000002;
                break;
            case 9:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000004;
                break;
            case 10:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000008;
                break;
            case 11:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000010;
                break;
            case 12:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000020;
                break;
            case 13:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000040;
                break;
            case 14:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000080;
                break;
            case 15:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000100;
                break;
            case 16:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000200;
                break;
            case 17:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000400;
                break;
            case 18:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00000800;
                break;
            case 19:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00001000;
                break;
            case 20:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00002000;
                break;
            case 21:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00004000;
                break;
            case 22:
                pTtxBlock->TtxDataLine = pTtxBlock->TtxDataLine | 0x00008000;
                break;
            default :
                break;
        }
    }
    return;
}





HI_VOID DRV_VBI_TtxDataParse(HI_U8 *pPesBuf, HI_U32 TtxDataLen)
{
    struct list_head *pList;
    TTX_DATA_BLOCK_S  *pTtxBlock;
    HI_U8 *pu8DstBuf, *pu8SrcBuf;
    HI_U32 i, j,k;


    pList = g_TtxSend.TtxBlockFreeHead.next;
    pTtxBlock = list_entry(pList, TTX_DATA_BLOCK_S, List);

    pTtxBlock->TtxDataLine = 0x0;
    pTtxBlock->TtxDataExtLine = 0x0;

    k=0;
    for (i=0; i<TtxDataLen/46; i++)
    {
        if ((*(pPesBuf + i*TTX_RAW_PACKET_LENGTH) == 0x02)  /*EBG no subtitle data*/
          ||(*(pPesBuf + i*TTX_RAW_PACKET_LENGTH) == 0x03)  /*EBG  subtitle data */
          ||(*(pPesBuf + i*TTX_RAW_PACKET_LENGTH) == 0xc0)  /*Inverted Teletext  */
          ||(*(pPesBuf + i*TTX_RAW_PACKET_LENGTH) == 0xc1)  /*unknow */
          ||(*(pPesBuf + i*TTX_RAW_PACKET_LENGTH) == 0xc3)  /*VPS data*/
           )
        {
            /*memcpy((pTtxBlock->pTtxDataVirtAddr + i*TTX_PACKET_LENGTH),
            pPesBuf + i*TTX_RAW_PACKET_LENGTH), TTX_RAW_PACKET_LENGTH);*/
            
           // pu8DstBuf = pTtxBlock->pTtxDataVirtAddr + k*TTX_PACKET_LENGTH;
          
            if( pTtxBlock->TtxPackConut >=39)
                pTtxBlock->TtxPackConut = 39;
            pu8DstBuf = pTtxBlock->pTtxDataVirtAddr + pTtxBlock->TtxPackConut*TTX_PACKET_LENGTH;
            pu8SrcBuf = pPesBuf + i*TTX_RAW_PACKET_LENGTH + 4;

            for(j=0; j<10; j++)
            {
                *pu8DstBuf++ = *pu8SrcBuf++;
                *pu8DstBuf++ = *pu8SrcBuf++;
                *pu8DstBuf++ = *pu8SrcBuf++;
                *pu8DstBuf++ = *pu8SrcBuf++;
                pu8DstBuf   += 12;
            }
            *pu8DstBuf++ = *pu8SrcBuf++;
            *pu8DstBuf++ = *pu8SrcBuf++;
            k++;
            pTtxBlock->TtxPackConut++;
            
            if( pTtxBlock->TtxPackConut >=39)
                break;
            
            DRV_VBI_TtxLineParse(pTtxBlock, *(pPesBuf + i*46 + 2));
        }
        else
        {
            continue;
        }
    }

    /* check the sending order of graph-text data */
    if ((*(pPesBuf + 3) == 0xE4))
    {
        pTtxBlock->TtxSeqFlag = HI_TRUE;
    }
    else
    {
        pTtxBlock->TtxSeqFlag = HI_FALSE;
    }

    //HI_ERR_DISP("TtxPackConut ---->%d\n",pTtxBlock->TtxPackConut);
   if (pTtxBlock->TtxPackConut >15 )
     {
        pTtxBlock->TtxEndAddr = pTtxBlock->TtxStartAddr + (pTtxBlock->TtxPackConut-1)*TTX_PACKET_LENGTH;
    
        g_TtxSend.TtxListFlag = HI_TRUE;

        list_move_tail(pList, &g_TtxSend.TtxBlockBusyHead);

        g_TtxSend.TtxListFlag = HI_FALSE;
     }
    return ;
}

HI_VOID DRV_VBI_VbiDataParse(HI_U8 *pPesBuf, HI_BOOL BothField, HI_DRV_DISP_FMT_E enFmt)
{
    struct list_head  *pList;
    CC_DATA_BLOCK_S   *pCCBlock;
    WSS_DATA_BLOCK_S  *pWSSBlock;

    /* CC data-flag */
    if (pPesBuf[0] == 0xc5)
    {
        pList = g_VbiSend.CcBlockFreeHead.next;
        pCCBlock = list_entry(pList, CC_DATA_BLOCK_S, List);

        if (BothField == HI_TRUE)
        {
            pCCBlock->TopFlag = HI_TRUE;
            pCCBlock->TopData = pPesBuf[4];
            pCCBlock->TopData = pCCBlock->TopData << 8;
            pCCBlock->TopData = pCCBlock->TopData + pPesBuf[3];

            pCCBlock->BottomFlag = HI_TRUE;
            if (enFmt < HI_DRV_DISP_FMT_NTSC)
            {
                pCCBlock->TopLine = 21;
                pCCBlock->BottomLine = 334;
            }
            else
            {
                pCCBlock->TopLine = 21;
                pCCBlock->BottomLine = 284;
            }
            pCCBlock->BottomData = pPesBuf[9];
            pCCBlock->BottomData = pCCBlock->BottomData << 8;
            pCCBlock->BottomData = pCCBlock->BottomData + pPesBuf[8];
        }
        else
        {
            /* top field data */
            if (pPesBuf[2] & 0x20)
            {
                pCCBlock->BottomFlag = HI_FALSE;
                pCCBlock->TopFlag = HI_TRUE;
                if (enFmt < HI_DRV_DISP_FMT_NTSC)
                {
                    pCCBlock->TopLine = 21;

                }
                else
                {
                    pCCBlock->TopLine = 21;
                }
                pCCBlock->TopData = pPesBuf[4];
                pCCBlock->TopData = pCCBlock->TopData << 8;
                pCCBlock->TopData = pCCBlock->TopData + pPesBuf[3];
            }
            else
            {
                pCCBlock->TopFlag = HI_FALSE;
                pCCBlock->BottomFlag = HI_TRUE;
                if (enFmt < HI_DRV_DISP_FMT_NTSC)
                {
                    pCCBlock->BottomLine = 334;

                }
                else
                {
                    pCCBlock->BottomLine = 284;
                }
                pCCBlock->BottomData = pPesBuf[4];
                pCCBlock->BottomData = pCCBlock->BottomData << 8;
                pCCBlock->BottomData = pCCBlock->BottomData + pPesBuf[3];
            }
        }
        
        g_VbiSend.CcListFlag = HI_TRUE;
        list_move_tail(pList, &g_VbiSend.CcBlockBusyHead);
        g_VbiSend.CcListFlag = HI_FALSE;
    }
    
    /*WSS data flag*/
    else
    {
        pList = g_VbiSend.WssBlockFreeHead.next;
        pWSSBlock = list_entry(pList, WSS_DATA_BLOCK_S, List);

        pWSSBlock->TopData = pPesBuf[4];
        pWSSBlock->TopData = pWSSBlock->TopData << 8;
        pWSSBlock->TopData = pWSSBlock->TopData + pPesBuf[3];
        g_VbiSend.WssListFlag = HI_TRUE;
        list_move_tail(pList, &g_VbiSend.WssBlockBusyHead);
        g_VbiSend.WssListFlag = HI_FALSE;
    }
    return;
}
HI_S32 DRV_VBI_SendCc_WssData(HI_DRV_DISP_FMT_E enFmt,HI_DRV_DISP_VBI_DATA_S *pstVbiData)
{
    HI_U32      TmpIndex=0;
    HI_U32      PesLenLow8=0, PesLenHigh8=0,PesLen=0,VbiDataLen=0,PesHeadLen=0;
    HI_BOOL     BothField=HI_FALSE;
    HI_BOOL     DataCorrectFlag=HI_TRUE;
    HI_BOOL     SendFlag=HI_FALSE;
    HI_S32      Ret;


    if ((g_VbiSend.VbiPesBufWrite + pstVbiData->u32DataLen) > BUFFER_LEN_16K)
    {
        HI_WARN_DISP("The Vbi Pes buffer is overflow!\n");
    }
    else
    {
        Ret = copy_from_user((g_VbiSend.pPesBufVirtAddr+g_VbiSend.VbiPesBufWrite), (HI_VOID __user *)pstVbiData->pu8DataAddr, pstVbiData->u32DataLen);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_DISP("copy data from user failed.\n");
        }
        /* move write-pointer to the tail of buffer */
        g_VbiSend.VbiPesBufWrite = g_VbiSend.VbiPesBufWrite + pstVbiData->u32DataLen;
        SendFlag = HI_TRUE;
    }

    while(TmpIndex < g_VbiSend.VbiPesBufWrite)
    {
        if (list_empty(&g_VbiSend.CcBlockFreeHead))
        {
            HI_ERR_DISP("The CC buffer is overflow!\n");

            if (TmpIndex != 0)
            {
                /* copy the data left to the top of buffer */
                memcpy(g_VbiSend.pPesBufVirtAddr, (g_VbiSend.pPesBufVirtAddr + TmpIndex), g_VbiSend.VbiPesBufWrite - TmpIndex);

                /* move the write-pointer to the tail of buffer */
                g_VbiSend.VbiPesBufWrite = g_VbiSend.VbiPesBufWrite - TmpIndex;
            }
            break;
        }

        if (list_empty(&g_VbiSend.WssBlockFreeHead))
        {
            HI_ERR_DISP("The WSS buffer is overflow!\n");

            if (TmpIndex != 0)
            {
                /* copy the data left to the top of buffer */
                memcpy(g_VbiSend.pPesBufVirtAddr, (g_VbiSend.pPesBufVirtAddr + TmpIndex),
                        g_VbiSend.VbiPesBufWrite - TmpIndex);

                /* move the write-pointer to the tail of buffer */
                g_VbiSend.VbiPesBufWrite = g_VbiSend.VbiPesBufWrite - TmpIndex;
            }
            break;
        }

        /* the length of the data is too small */
        if ((TmpIndex+VBI_PES_MIN_LEN) > g_VbiSend.VbiPesBufWrite)
        {
            HI_ERR_DISP("The Vbi Pes data is not enough!\n");

            if (TmpIndex != 0)
            {
                /* copy the data left to the top of buffer */
                memcpy(g_VbiSend.pPesBufVirtAddr, (g_VbiSend.pPesBufVirtAddr + TmpIndex),
                        g_VbiSend.VbiPesBufWrite - TmpIndex);

                /* move the write-pointer to the tail of buffer */
                g_VbiSend.VbiPesBufWrite = g_VbiSend.VbiPesBufWrite - TmpIndex;
            }
            break;
        }

        if (0x00 == g_VbiSend.pPesBufVirtAddr[TmpIndex])
        {
            if ((g_VbiSend.pPesBufVirtAddr[TmpIndex+1] == 0x00)
                    && (g_VbiSend.pPesBufVirtAddr[TmpIndex+2] == 0x01)
                    && (g_VbiSend.pPesBufVirtAddr[TmpIndex+3] == 0xbd))
            {
                /* get the length of PES packet */
                PesLenHigh8 = g_VbiSend.pPesBufVirtAddr[TmpIndex+4];
                PesLenLow8 = g_VbiSend.pPesBufVirtAddr[TmpIndex+5];
                PesLen = (PesLenHigh8 << 8) + PesLenLow8;

                if (PesLen <= VBI_PES_MAX_LEN)
                {
                    /* buffer has a complete PES packet */
                    if ((g_VbiSend.VbiPesBufWrite - TmpIndex - 6) >= PesLen)
                    {
                        PesHeadLen = g_VbiSend.pPesBufVirtAddr[TmpIndex+8];

                        /* make sure that ebudata flag exists */
                        if (PesHeadLen < (PesLen - 4))
                        {
                            /* query whether it is EBU data */
                            if (((g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+9] >= 0x10)
                                        && (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+9] <= 0x1f))
                              ||((g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+9] >= 0x99)
                                  && (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+9] <= 0x9b))
                               )
                            {
                                DataCorrectFlag = HI_FALSE;
                                VbiDataLen = PesLen - PesHeadLen - 4;

                                /* the length of the data must >= 5*/
                                if (VbiDataLen >= 5)
                                {
                                    /* CC data flag */
                                    if (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+10] == 0xc5)
                                    {
                                        /* CC data may contain two fields */
                                        if (VbiDataLen >= 10)
                                        {
                                            if (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+15] == 0xc5)
                                            {
                                                DataCorrectFlag = HI_TRUE;
                                                BothField = HI_TRUE;
                                            }
                                            else if (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+15] == 0xff)
                                            {
                                                DataCorrectFlag = HI_TRUE;
                                                BothField = HI_FALSE;
                                            }
                                        }
                                        else
                                        {
                                            DataCorrectFlag = HI_TRUE;
                                            BothField = HI_FALSE;
                                        }
                                    }
                                    /*WSS data flag*/
                                    else if (g_VbiSend.pPesBufVirtAddr[TmpIndex+PesHeadLen+10] == 0xc4)
                                    {
                                        DataCorrectFlag = HI_TRUE;
                                    }
                                    else
                                    {
                                    }

                                    /* if the data is valid, analyze, copy it and add it to queue */
                                    if (HI_TRUE == DataCorrectFlag)
                                    {
                                        DRV_VBI_VbiDataParse((g_VbiSend.pPesBufVirtAddr+TmpIndex+PesHeadLen+10),
                                                BothField, enFmt);
                                    }
                                }

                            }
                        }

                        /* TmpIndex, leap over current packet */
                        TmpIndex = TmpIndex + PesLen + 6;
                        continue;
                    }
                    else
                    {
                        HI_WARN_DISP("The Vbi Pes Packet is not enough!\n");
                        if (TmpIndex != 0)
                        {
                            /* copy the left data to the top of buffer */
                            memcpy(g_VbiSend.pPesBufVirtAddr, (g_VbiSend.pPesBufVirtAddr + TmpIndex),
                                    g_VbiSend.VbiPesBufWrite- TmpIndex);

                            /* move the write-pointer to the tail of buffer */
                            g_VbiSend.VbiPesBufWrite = g_VbiSend.VbiPesBufWrite - TmpIndex;
                        }
                        break;
                    }
                }
                else
                {
                    HI_WARN_DISP("The Vbi Pes length data is error, clear buffer!\n");
                    g_VbiSend.VbiPesBufWrite = 0;
                    break;
                }

            }
        }

        TmpIndex++;
    }

    if (TmpIndex == g_VbiSend.VbiPesBufWrite)
    {
        g_VbiSend.VbiPesBufWrite = 0;
    }

    if (HI_TRUE == SendFlag)
    {
        return HI_SUCCESS;
    }
    else
    {
        return  HI_FAILURE;
    }

    return HI_SUCCESS;  
}

HI_S32 DRV_VBI_SendTtxData(HI_DRV_DISP_TTX_DATA_S *pstTtxData)
{
    HI_U32      TmpIndex=0;
    HI_U32      PesLenLow8=0, PesLenHigh8=0,PesLen=0,TtxDataLen=0;
    HI_BOOL     DataCorrectFlag=HI_TRUE;
    HI_BOOL     SendFlag=HI_FALSE;
    HI_U32      i;
    HI_S32      Ret;

	if(g_stVBI[HI_DRV_DISP_VBI_TTX].bOpen != HI_TRUE)
	{
		HI_ERR_DISP("Ttx channel isn't opened\n");
		return HI_FAILURE;
	}

    if ((g_TtxSend.TtxPesBufWrite + pstTtxData->u32DataLen) > BUFFER_LEN_16K)
    {
        HI_WARN_DISP("The Ttx Pes buffer is overflow!\n");
    }
    else
    {

		Ret = copy_from_user((g_TtxSend.pPesBufVirtAddr+g_TtxSend.TtxPesBufWrite), (HI_VOID __user *)pstTtxData->pu8DataAddr, pstTtxData->u32DataLen);
		if (Ret != HI_SUCCESS)
		{
			HI_ERR_DISP("copy data from user failed.\n");
			return HI_FAILURE;
		}

        /* move write-pointer to the tail of buffer */
        g_TtxSend.TtxPesBufWrite = g_TtxSend.TtxPesBufWrite + pstTtxData->u32DataLen;
        SendFlag = HI_TRUE;
    }

    while(TmpIndex < g_TtxSend.TtxPesBufWrite)
    {
        if (list_empty(&g_TtxSend.TtxBlockFreeHead))
        {
            HI_WARN_DISP("The Ttx buffer is overflow!\n");

            if (TmpIndex != 0)
            {
                /* copy the data left to the top of the buffer */
                memcpy(g_TtxSend.pPesBufVirtAddr, (g_TtxSend.pPesBufVirtAddr + TmpIndex), g_TtxSend.TtxPesBufWrite - TmpIndex);

                /* move the write-pointer to the tail of the buffer */
                g_TtxSend.TtxPesBufWrite = g_TtxSend.TtxPesBufWrite - TmpIndex;
            }
            break;
        }

        /* the size of data is too little */
        if ((TmpIndex+TTX_PES_MIN_LEN) > g_TtxSend.TtxPesBufWrite)
        {
            HI_WARN_DISP("The Ttx Pes data is not enough!\n");

            if (TmpIndex != 0)
            {
                /* copy the data left to the tail of the buffer */
                memcpy(g_TtxSend.pPesBufVirtAddr, (g_TtxSend.pPesBufVirtAddr + TmpIndex), g_TtxSend.TtxPesBufWrite - TmpIndex);

                /* move the write-pointer to the tail of the buffer*/
                g_TtxSend.TtxPesBufWrite = g_TtxSend.TtxPesBufWrite - TmpIndex;
            }
            break;
        }

        if (0x00 == g_TtxSend.pPesBufVirtAddr[TmpIndex])
        {
            if ((g_TtxSend.pPesBufVirtAddr[TmpIndex+1] == 0x00) && (g_TtxSend.pPesBufVirtAddr[TmpIndex+2] == 0x01) && (g_TtxSend.pPesBufVirtAddr[TmpIndex+3] == 0xbd))
            {
                /* get the PES packet length */
                PesLenHigh8 = g_TtxSend.pPesBufVirtAddr[TmpIndex+4];
                PesLenLow8 = g_TtxSend.pPesBufVirtAddr[TmpIndex+5];
                PesLen = (PesLenHigh8 << 8) + PesLenLow8;

                if (PesLen <= TTX_PES_MAX_LEN)
                {
                    /* In buffer, there is a complete PES packet */
                    if ((g_TtxSend.TtxPesBufWrite - TmpIndex - 6) >= PesLen)
                    {
                        /* query whether it is EBU data */
                        if (((g_TtxSend.pPesBufVirtAddr[TmpIndex+45] >= 0x10) && (g_TtxSend.pPesBufVirtAddr[TmpIndex+45] <= 0x1f))
                                ||((g_TtxSend.pPesBufVirtAddr[TmpIndex+45] >= 0x99) && (g_TtxSend.pPesBufVirtAddr[TmpIndex+45] <= 0x9b))
                           )
                        {
                            TtxDataLen = PesLen - 40;

                            /* check whether each TTX packet is valid or not */
                            DataCorrectFlag = HI_FALSE;

                            for (i=0; i<TtxDataLen/46; i++)
                            {
                                
                                if ((g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0x02)
                                        ||(g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0x03)
                                        ||(g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0xc0)
                                        ||(g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0xc1)
                                        ||(g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0xc3)
                                   )
                                {
                                    DataCorrectFlag = HI_TRUE;
    
                                    continue;
                                }
                                else if (g_TtxSend.pPesBufVirtAddr[TmpIndex+i*46+46] == 0xff)
                                {
                                    break;
                                }
                            }

                            /* if the data is valid, analyze, copy it and add it to the queue */
                            if ((HI_TRUE == DataCorrectFlag) && (i != 0))
                            {
                                DRV_VBI_TtxDataParse((g_TtxSend.pPesBufVirtAddr+TmpIndex+46), TtxDataLen);
                            }

                        }

                        /* TmpIndex, jump current packet */
                        TmpIndex = TmpIndex + PesLen + 6;
                        continue;

                    }
                    else
                    {
                        HI_WARN_DISP("The Ttx Pes Packet is not enough!\n");
                        if (TmpIndex != 0)
                        {
                            /* copy the data left to the top of buffer */
                            memcpy(g_TtxSend.pPesBufVirtAddr, (g_TtxSend.pPesBufVirtAddr + TmpIndex), g_TtxSend.TtxPesBufWrite - TmpIndex);

                            /* move the write-pointer to the tail of buffer */
                            g_TtxSend.TtxPesBufWrite = g_TtxSend.TtxPesBufWrite - TmpIndex;
                        }
                        break;
                    }
                }
                else
                {
                    HI_ERR_DISP("The Ttx Pes length data is error, clear buffer!\n");
                    g_TtxSend.TtxPesBufWrite = 0;
                    break;
                }

            }
        }

        TmpIndex++;
    }

    if (TmpIndex == g_TtxSend.TtxPesBufWrite)
    {
        g_TtxSend.TtxPesBufWrite = 0;
    }

    /* start a sending of TTX data */

    if (HI_FALSE == g_TtxSend.TtxBusyFlag)
    {
        if (g_TtxSend.pTtxBusyList)
        {
            list_add_tail(g_TtxSend.pTtxBusyList, &g_TtxSend.TtxBlockFreeHead);
            g_TtxSend.pTtxBusyList = NULL;
        }

        /* Not-null of the linked list */
     
        if (!list_empty(&g_TtxSend.TtxBlockBusyHead))
        {
            g_TtxSend.pTtxBusyList = g_TtxSend.TtxBlockBusyHead.next;
            g_TtxSend.pTtxBusyBlock = list_entry(g_TtxSend.pTtxBusyList, TTX_DATA_BLOCK_S, List);
            list_del(g_TtxSend.pTtxBusyList);

            VDP_VBI_TtxSetSeq(g_TtxSend.pTtxBusyBlock->TtxSeqFlag);
			/*first config 10 ~21 line ,in order to increase ttx send. */
            g_TtxSend.pTtxBusyBlock->TtxDataLine = 0x7ff87ff8;
            VDP_VBI_TtxLineEnable(g_TtxSend.pTtxBusyBlock->TtxDataLine, g_TtxSend.pTtxBusyBlock->TtxDataExtLine);
            VDP_VBI_TtxSetAddr((g_TtxSend.pTtxBusyBlock->TtxStartAddr), (g_TtxSend.pTtxBusyBlock->TtxEndAddr));
            VDP_VBI_TtxSetReady();

            g_TtxSend.TtxBusyFlag = HI_TRUE;
        }
    }

    if (HI_TRUE == SendFlag)
    {
        return HI_SUCCESS;
    }
    else
    {
        return  HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_VBI_SendData(HI_HANDLE hVbi, HI_DRV_DISP_FMT_E enFmt,HI_DRV_DISP_VBI_DATA_S* pstVbiData)
{ 
    HI_S32 s32Ret = HI_SUCCESS;
    HI_DRV_DISP_TTX_DATA_S stTtxData;

    HI_DRV_DISP_VBI_TYPE_E  eVBIType;
    
    eVBIType = (HI_DRV_DISP_VBI_TYPE_E) hVbi;

    VbiCheckType(eVBIType);
    
    switch (pstVbiData->eType)
    {
        case HI_DRV_DISP_VBI_TTX:
            stTtxData.pu8DataAddr = pstVbiData->pu8DataAddr;
            stTtxData.u32DataLen = pstVbiData->u32DataLen;
            s32Ret = DRV_VBI_SendTtxData(&stTtxData);
            break;
        case HI_DRV_DISP_VBI_CC:
            s32Ret = DRV_VBI_SendCc_WssData(enFmt,pstVbiData);
            break;
        case HI_DRV_DISP_VBI_WSS:
            /*not support!*/
            break;
        case HI_DRV_DISP_VBI_VCHIP:
        default:
            return HI_ERR_DISP_INVALID_OPT;
    }

    return s32Ret;
}

HI_S32 DRV_VBI_Init()
{
	HI_U32 u32Cnt;

	HI_DISP_VBI_STATUS_S *pstStatus;

	for (u32Cnt = 0; u32Cnt < HI_DRV_DISP_VBI_TYPE_BUTT; u32Cnt++)
	{
		pstStatus = &(g_stVBI[u32Cnt]);

		pstStatus->enVBIType = HI_DRV_DISP_VBI_TYPE_BUTT;
		pstStatus->bOpen = HI_FALSE;
		pstStatus->u32Handle = HI_INVALID_HANDLE;
	}

	return HI_SUCCESS;		
}

HI_S32 DRV_VBI_DeInit()
{
	HI_U32 u32Cnt;
	HI_S32 s32Ret;
	HI_DISP_VBI_STATUS_S *pstStatus;

	for (u32Cnt = 0; u32Cnt < HI_DRV_DISP_VBI_TYPE_BUTT; u32Cnt++)
	{
		pstStatus = &(g_stVBI[u32Cnt]);
		
		if (pstStatus->bOpen)
		{
			switch (pstStatus->enVBIType)
			{
				case HI_DRV_DISP_VBI_TTX:
					s32Ret = DRV_VBI_DeInitTtx();
					break;
				case HI_DRV_DISP_VBI_CC:
				case HI_DRV_DISP_VBI_WSS:
					s32Ret = DRV_VBI_DeInitVbi();
					break;
				default:
					s32Ret = HI_ERR_DISP_INVALID_OPT;
			}
		}

		pstStatus->enVBIType = HI_DRV_DISP_VBI_TYPE_BUTT;
		pstStatus->bOpen = HI_FALSE;
		pstStatus->u32Handle = HI_INVALID_HANDLE;
	}

	return HI_SUCCESS;		
}
#else
HI_VOID DRV_VBI_VbiIsr(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    return;
}
HI_VOID DRV_VBI_TtxIsr(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    return;
}
HI_S32 DRV_VBI_InitTtx(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_DeInitTtx(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_InitVbi(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_DeInitVbi(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_CreateChannel(HI_DRV_DISP_VBI_CFG_S* pstCfg, HI_HANDLE *phVbi)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_DestoryChannel(HI_HANDLE hVbi)
{
    return HI_SUCCESS;
}

HI_S32 DRV_VBI_SendData(HI_HANDLE hVbi, HI_DRV_DISP_FMT_E enFmt,HI_DRV_DISP_VBI_DATA_S* pstVbiData)
{
    return HI_SUCCESS;
}


HI_S32 DRV_VBI_Init()
{
	return HI_SUCCESS;		
}

HI_S32 DRV_VBI_DeInit()
{
	return HI_SUCCESS;		
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
