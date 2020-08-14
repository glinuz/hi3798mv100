#include "vpss_cbb_alg.h"
#include "vpss_cbb_reg.h"

HI_VOID VPSS_AlgCfg_BaseFunc(CBB_REG_ADDR_S stRegAddr, CBB_ALG_E eAlgId, HI_VOID *pArg1, HI_VOID *pArg2)
{
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
	
	switch ( eAlgId )
	{
	    case CBB_ALG_ZME:
	        
	        break;
	    case CBB_ALG_DEI:
	        {
		    	CBB_ALG_INFO_S *pstDEIInfo;
		        pstDEIInfo = (CBB_ALG_INFO_S *)pArg1;

				VPSS_DRV_Set_VPSS_DEI_ADDR(pstVpssNode, pstDEIInfo->u32ParaAddr);
				VPSS_DRV_Set_dei_en(pstVpssNode, pstDEIInfo->bEnable);
			}
	        break;
	    case CBB_ALG_TNR:
		    {
		    	CBB_ALG_INFO_S *pstTnrInfo;
		        pstTnrInfo = (CBB_ALG_INFO_S *)pArg1;

				VPSS_DRV_Set_VPSS_TNR_ADDR(pstVpssNode, pstTnrInfo->u32ParaAddr);
				
				VPSS_DRV_Set_VPSS_TNR_FILR_MODE_ymmode(pstVpssNode, pstTnrInfo->u32Ymotionmode);
				
				VPSS_DRV_Set_tnr_en(pstVpssNode, pstTnrInfo->bEnable);
			}
	        break;
	    case CBB_ALG_TNR_MC:
			{
				CBB_ALG_INFO_S *pstTnrMCInfo;
				pstTnrMCInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_VPSS_TNR_MAME_CTRL_mcTnr(pstVpssNode, pstTnrMCInfo->bEnable);
			}
			break;
	    case CBB_ALG_SNR:
			{
				CBB_ALG_INFO_S *pstSnrInfo;
				pstSnrInfo = (CBB_ALG_INFO_S *)pArg1;

				//db,dm, nr的总开关， SNR包括db,dm,nr
				VPSS_DRV_Set_VPSS_SNR_ADDR(pstVpssNode, pstSnrInfo->u32ParaAddr);
				VPSS_DRV_Set_snr_en(pstVpssNode, pstSnrInfo->bEnable);
			}
			break;
	    case CBB_ALG_DB:
		    {
		        CBB_ALG_INFO_S *pstDBInfo;

		        pstDBInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_VPSS_DB_CTRL_db(pstVpssNode, pstDBInfo->bEnable);
			}
	        break;
		case CBB_ALG_DB_DET:
			{
		        CBB_ALG_INFO_S *pstDBDETInfo;

		        pstDBDETInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_blk_det_en(pstVpssNode, pstDBDETInfo->bEnable);
			}
			break;
		case CBB_ALG_NR:
			{
				CBB_ALG_INFO_S *pstNRInfo;

		        pstNRInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_VPSS_DB_CTRL_nr(pstVpssNode, pstNRInfo->bEnable);
			}
			break;
	    case CBB_ALG_DM:
			{
				CBB_ALG_INFO_S *pstDMInfo;			
				pstDMInfo = (CBB_ALG_INFO_S *)pArg1;

		        VPSS_DRV_Set_VPSS_DB_CTRL_dm(pstVpssNode, pstDMInfo->bEnable);
			}
			break;
	    case CBB_ALG_DS:
			{

			}	        
	        break;
		case CBB_ALG_IGBM:
			{
				CBB_ALG_INFO_S 		*pstIgbmInfo;
				pstIgbmInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_igbm_en(pstVpssNode, pstIgbmInfo->bEnable);
			}
			break;
	    case CBB_ALG_IFMD:
			{
				CBB_ALG_INFO_S 		*pstIfmdInfo;
				pstIfmdInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_ifmd_en(pstVpssNode, pstIfmdInfo->bEnable);
			}
			break;
	    case CBB_ALG_MCDEI:
			{
		    	CBB_ALG_INFO_S *pstMCDEIInfo;
		        pstMCDEIInfo = (CBB_ALG_INFO_S *)pArg1;
				VPSS_DRV_Set_mcdi_en(pstVpssNode, pstMCDEIInfo->bEnable);
			}
			break;
	    default:
	        break;
	}
	
}

HI_VOID VPSS_AlgCfg_Zme(CBB_ALG_ZME_S *pstAlgZmeCfg, CBB_REG_ADDR_S stRegAddr)
{
	return;
#ifdef NEVER
    S_VPSS_REGS_TYPE *pstVpssNode;  //DDR中节点首地址
    HI_U32 u32InWth, u32InHgt;
    HI_U32 u32OutWth, u32OutHgt;
    HI_U32 u32HyRatio, u32HcRatio;
    HI_U32 u32VyRatio, u32VcRatio;
    
    pstVpssNode = (S_VPSS_REGS_TYPE *)stRegAddr.pu8NodeVirAddr;
    VPSS_DRV_GetVhd0ZmeInReso(pstVpssNode, &u32InWth, &u32InHgt);
    VPSS_DRV_GetVhd0ZmeOutReso(pstVpssNode, &u32OutWth, &u32OutHgt);

    u32HyRatio = (VPSS_MULTI_HOR/4096)*(u32OutWth*(4096)/ u32InWth);
    u32VyRatio = (VPSS_MULTI_VER*u32OutHgt) / u32InHgt;
    
    if(pstAlgZmeCfg->enMode==ALG_MODE_DEFAULT)
    {
        VPSS_DRV_SetVhd0HspEn(pstVpssNode, 1, 1);
        VPSS_DRV_SetVhd0HspMid(pstVpssNode, 0, 0);
        VPSS_DRV_SetVhd0HspFir(pstVpssNode, 1, 1);
        VPSS_DRV_SetVhd0HspOrder(pstVpssNode, 0);
        VPSS_DRV_SetVhd0HspRatio(pstVpssNode, u32HyRatio);
        VPSS_DRV_SetVhd0HOffset(pstVpssNode, 0, 0);

        VPSS_DRV_SetVhd0VspEn(pstVpssNode, 1, 1);
        VPSS_DRV_SetVhd0VspMid(pstVpssNode, 0, 0);
        VPSS_DRV_SetVhd0VspFir(pstVpssNode, 1, 1);
        VPSS_DRV_SetVhd0VspRatio(pstVpssNode, u32VyRatio);
        VPSS_DRV_SetVhd0VOffset(pstVpssNode, 0, 0);
    }
    else
    {
        VPSS_DRV_SetVhd0HspEn(pstVpssNode, pstAlgZmeCfg->bHlmscEn, pstAlgZmeCfg->bHchmscEn);
        VPSS_DRV_SetVhd0HspMid(pstVpssNode, pstAlgZmeCfg->bHlmidEn, pstAlgZmeCfg->bHchmidEn);
        VPSS_DRV_SetVhd0HspFir(pstVpssNode, pstAlgZmeCfg->bHlfirEn, pstAlgZmeCfg->bHchfirEn);
        VPSS_DRV_SetVhd0HspOrder(pstVpssNode, pstAlgZmeCfg->u32HfirOrder);
        VPSS_DRV_SetVhd0HspRatio(pstVpssNode, u32HyRatio);
        VPSS_DRV_SetVhd0HOffset(pstVpssNode, pstAlgZmeCfg->u32HorlOffset, pstAlgZmeCfg->u32HorcOffset);

        VPSS_DRV_SetVhd0VspEn(pstVpssNode, pstAlgZmeCfg->bVlmscEn, pstAlgZmeCfg->bVchmscEn);
        VPSS_DRV_SetVhd0VspMid(pstVpssNode, pstAlgZmeCfg->bVlmidEn, pstAlgZmeCfg->bVchmidEn);
        VPSS_DRV_SetVhd0VspFir(pstVpssNode, pstAlgZmeCfg->bVlfirEn, pstAlgZmeCfg->bVchfirEn);
        VPSS_DRV_SetVhd0VspRatio(pstVpssNode, u32VyRatio);
        VPSS_DRV_SetVhd0VOffset(pstVpssNode, pstAlgZmeCfg->u32VerlOffset, pstAlgZmeCfg->u32VercOffset);
    }
#endif /* NEVER */
    
}

