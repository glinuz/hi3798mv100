#include "vpss_info.h"
#include "drv_vdec_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 VPSS_DBG_DbgInit(VPSS_DBG_S *pstDbg)
{
    HI_U32 u32Count;


    pstDbg->stInstDbg.unInfo.u32 = 0;

    pstDbg->stInstDbg.u32LastH = 0;
    pstDbg->stInstDbg.u32LastM = 0;
    pstDbg->stInstDbg.u32LastS = 0;

	/*TEST*/
    //pstDbg->stInstDbg.unInfo.bits.imginfo = 1;

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstDbg->stPortDbg[u32Count].unInfo.u32 = 0;

		pstDbg->stPortDbg[u32Count].u32LastH = 0;
		pstDbg->stPortDbg[u32Count].u32LastM = 0;
		pstDbg->stPortDbg[u32Count].u32LastS = 0;
        //pstDbg->stPortDbg[u32Count].unInfo.bits.frameinfo = 1;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_DBG_DbgDeInit(VPSS_DBG_S *pstDbg)
{
    return HI_SUCCESS;
}

HI_S32 VPSS_DBG_SendDbgCmd(VPSS_DBG_S *pstDbg,VPSS_DBG_CMD_S *pstCmd)
{
    HI_U32  u32Count = 0;

    VPSS_DBG_PORT_S *pstPortDbg;
    VPSS_DBG_INST_S *pstInstDbg;

	HI_U32 u32Hour = 0;
	HI_U32 u32Minute = 0;
	HI_U32 u32Second = 0;

	(HI_VOID)VPSS_OSAL_GetCurTime(&u32Hour,&u32Minute,&u32Second);

    switch(pstCmd->enDbgType)
    {
        case DBG_W_YUV:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->unInfo.bits.writeyuv = HI_TRUE;
                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.bits.writeyuv = HI_TRUE;
                    break;
                default:
                    break;
            }
            break;
        case DBG_W_STREAM:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->unInfo.bits.writestream = HI_TRUE;
					pstInstDbg->u32LastH = u32Hour;
					pstInstDbg->u32LastM = u32Minute;
					pstInstDbg->u32LastS = u32Second;
                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.bits.writestream = HI_TRUE;
					pstPortDbg->u32LastH = u32Hour;
					pstPortDbg->u32LastM = u32Minute;
					pstPortDbg->u32LastS = u32Second;
                    break;
                default:
                    break;
            }
            break;
        case DBG_INFO_FRM:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->unInfo.bits.imginfo = HI_TRUE;
                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.bits.frameinfo = HI_TRUE;
                    break;
                default:
                    break;
            }
            break;
        case DBG_INFO_ASP:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.bits.asp = HI_TRUE;
                    break;
                default:
                    break;
            }
            break;
        case DBG_INFO_NONE:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->unInfo.u32 = 0;
                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count =  pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.u32 = 0;
                    break;
                default:
                    break;
            }
            break;
        case DBG_SET_UHD_LOW:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->u32UhdSetW = VPSS_UHD_LOW_W;
					pstInstDbg->u32UhdSetH = VPSS_UHD_LOW_H;
                    break;
				default:
					VPSS_ERROR("Invalid Setting Dst %d\n",pstCmd->hDbgPart);
			}
			break;
        case DBG_SET_UHD_MID:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->u32UhdSetW = VPSS_UHD_MIDDLE_W;
					pstInstDbg->u32UhdSetH = VPSS_UHD_MIDDLE_H;
                    break;
				default:
					VPSS_ERROR("Invalid Setting Dst %d\n",pstCmd->hDbgPart);
			}
			break;
        case DBG_SET_UHD_HIGH:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->u32UhdSetW = VPSS_UHD_HIGH_W;
					pstInstDbg->u32UhdSetH = VPSS_UHD_HIGH_H;
                    break;
				default:
					VPSS_ERROR("Invalid Setting Dst %d\n",pstCmd->hDbgPart);
			}
			break;
        case DBG_SET_UHD_USR:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
                    pstInstDbg->u32UhdSetW = 0;
					pstInstDbg->u32UhdSetH = 0;
                    break;
				default:
					VPSS_ERROR("Invalid Setting Dst %d\n",pstCmd->hDbgPart);
			}
			break;
		case DBG_SET_UV_INVERT:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    pstPortDbg->unInfo.bits.uv_invert = pstCmd->hDbgValue;

					VPSS_INFO("set prot%d uv Invert %s !!\n", u32Count,
						   (pstPortDbg->unInfo.bits.uv_invert == HI_TRUE) ? "ON":"OFF");
                    break;
                default:
                    VPSS_ERROR("Invalid Port %d\n",pstCmd->hDbgPart);
			}
			break;
		case DBG_SET_CMP_ON:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_PORT0_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
					pstPortDbg->unInfo.bits.cmp = 1;
					VPSS_INFO("set prot%d cmp on !!\n", u32Count);
                    break;
                default:
                    VPSS_ERROR("Invalid Port %d, only HD port support cmp\n",u32Count);
			}
			break;
		case DBG_SET_CMP_OFF:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_PORT0_ID:
                    u32Count = pstCmd->hDbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
					pstPortDbg->unInfo.bits.cmp = 2;
					VPSS_INFO("set prot%d cmp off !!\n", u32Count);
                    break;
                default:
                    VPSS_ERROR("Invalid Port %d, only HD port support cmp\n",u32Count);
			}
			break;
        case DBG_SET_I2P:
            switch(pstCmd->hDbgPart)
            {
                case DEF_DBG_SRC_ID:
                    pstInstDbg = &(pstDbg->stInstDbg);
					pstInstDbg->unInfo.bits.dei = pstCmd->hDbgValue ;

					VPSS_INFO("set interlace to progress %s !!\n",
						   (pstInstDbg->unInfo.bits.dei == HI_TRUE) ? "ON":"OFF");
                    break;
                default:
                    VPSS_ERROR("Invalid input %d\n",pstCmd->hDbgPart);
            }
            break;
        default:
            VPSS_FATAL("Cmd isn't Supported.\n");
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_DBG_ReplyDbgCmd(VPSS_DBG_S *pstDbg, VPSS_DEBUG_E enCmd, HI_VOID* para1, HI_VOID* para2)
{
    HI_DRV_VIDEO_FRAME_S *pstFrm;
    HI_U32 u32Count;
    HI_U32 u32DbgPart;
    VPSS_DBG_PORT_S *pstPortDbg;
    HI_DRV_VID_FRAME_ADDR_S stTmpAddr;
    HI_S8 chFile[DEF_FILE_NAMELENGTH];
	HI_U32 u32Hour = 0;
	HI_U32 u32Minute = 0;
	HI_U32 u32Second = 0;

    u32DbgPart = *((HI_U32*)para1);

	(HI_VOID)VPSS_OSAL_GetCurTime(&u32Hour,&u32Minute,&u32Second);

    switch (enCmd)
    {
        case DBG_W_YUV:
            pstFrm = (HI_DRV_VIDEO_FRAME_S *)para2;
            switch (u32DbgPart)
            {
                case DEF_DBG_SRC_ID:
					if (pstDbg->stInstDbg.unInfo.bits.writestream)
					{
						u32Hour = pstDbg->stInstDbg.u32LastH;
						u32Minute = pstDbg->stInstDbg.u32LastM;
						u32Second = pstDbg->stInstDbg.u32LastS;
					}
                    if (pstDbg->stInstDbg.unInfo.bits.writeyuv
                        || pstDbg->stInstDbg.unInfo.bits.writestream)
                    {
                        HI_OSAL_Snprintf(chFile,
                                DEF_FILE_NAMELENGTH, "vpss_src_%dX%d_%02d_%02d_%02d.yuv",
                                pstFrm->u32Width,pstFrm->u32Height,u32Hour,u32Minute,u32Second);
                        VPSS_OSAL_WRITEYUV(pstFrm, chFile);
                        if (pstFrm->eFrmType == HI_DRV_FT_FPK)
                        {
                            HI_OSAL_Snprintf(chFile,
                                DEF_FILE_NAMELENGTH, "vpss_src_right_%dX%d_%02d_%02d_%02d.yuv",
                                pstFrm->u32Width,pstFrm->u32Height,u32Hour,u32Minute,u32Second);

                            memcpy(&stTmpAddr,&pstFrm->stBufAddr[0],sizeof(HI_DRV_VID_FRAME_ADDR_S));

                            memcpy(&(pstFrm->stBufAddr[0]),
                                   &(pstFrm->stBufAddr[1]),
                                   sizeof(HI_DRV_VID_FRAME_ADDR_S));

                            VPSS_OSAL_WRITEYUV(pstFrm, chFile);

                            memcpy(&(pstFrm->stBufAddr[0]),
                                   &(stTmpAddr),
                                   sizeof(HI_DRV_VID_FRAME_ADDR_S));
                        }
                        if (pstDbg->stInstDbg.unInfo.bits.writeyuv)
							pstDbg->stInstDbg.unInfo.bits.writeyuv = HI_FALSE;
                    }

                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
                    u32Count = u32DbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);

					if (pstPortDbg->unInfo.bits.writestream)
					{
						u32Hour = pstPortDbg->u32LastH;
						u32Minute = pstPortDbg->u32LastM;
						u32Second = pstPortDbg->u32LastS;
					}
                    if (pstPortDbg->unInfo.bits.writeyuv
                        || pstPortDbg->unInfo.bits.writestream)
                    {
                        HI_OSAL_Snprintf(chFile,
                                DEF_FILE_NAMELENGTH, "vpss_p%d_%dX%d_%02d_%02d_%02d.yuv",u32Count,
                                pstFrm->u32Width, pstFrm->u32Height,u32Hour,u32Minute,u32Second);
                        VPSS_OSAL_WRITEYUV(pstFrm, chFile);
                        if (pstFrm->eFrmType == HI_DRV_FT_FPK)
                        {
                            HI_OSAL_Snprintf(chFile,
                                DEF_FILE_NAMELENGTH, "vpss_p%d_right_%dX%d_%02d_%02d_%02d.yuv",u32Count,
                                pstFrm->u32Width, pstFrm->u32Height,u32Hour,u32Minute,u32Second);

                            memcpy(&stTmpAddr,&pstFrm->stBufAddr[0],sizeof(HI_DRV_VID_FRAME_ADDR_S));

                            memcpy(&(pstFrm->stBufAddr[0]),
                                   &(pstFrm->stBufAddr[1]),
                                   sizeof(HI_DRV_VID_FRAME_ADDR_S));

                            VPSS_OSAL_WRITEYUV(pstFrm, chFile);

                            memcpy(&(pstFrm->stBufAddr[0]),
                                   &(stTmpAddr),
                                   sizeof(HI_DRV_VID_FRAME_ADDR_S));
                        }
                        if (pstPortDbg->unInfo.bits.writeyuv)
                            pstPortDbg->unInfo.bits.writeyuv = HI_FALSE;
                    }
                    break;
                default:
                    VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
                    break;
            }
            break;
        case DBG_INFO_FRM:
            pstFrm = (HI_DRV_VIDEO_FRAME_S *)para2;
            switch (u32DbgPart)
            {
                case DEF_DBG_SRC_ID:
                    if (pstDbg->stInstDbg.unInfo.bits.imginfo)
                    {
                        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
                        HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
                        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrm->u32Priv[0]);
                        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstPriv->u32Reserve[0]);


                        HI_PRINT("Image Info:Index %d Type %d Format %d W %d H %d Prog %d FieldMode %d PTS %d Rate %d LastFlag %#x Delta %d CodeType %d,SourceType %d,BitWidth %d\n"
                                 "           L:Y %#x C %#x YH %#x CH %#x YS %d CS %d \n"
                                 "           R:Y %#x C %#x YH %#x CH %#x YS %d CS %d \n",
                                pstFrm->u32FrameIndex,
                                pstFrm->eFrmType,
                                pstFrm->ePixFormat,
                                pstFrm->u32Width,
                                pstFrm->u32Height,
                                pstFrm->bProgressive,
                                pstFrm->enFieldMode,
                                pstFrm->u32Pts,
                                pstFrm->u32FrameRate,
                                pstPriv->u32LastFlag,
                                pstVdecPriv->s32InterPtsDelta,
                                pstVdecPriv->entype,
                                pstPriv->stVideoOriginalInfo.enSource,
                                pstFrm->enBitWidth,
                                pstFrm->stBufAddr[0].u32PhyAddr_Y,
                                pstFrm->stBufAddr[0].u32PhyAddr_C,
                                pstFrm->stBufAddr[0].u32PhyAddr_YHead,
                                pstFrm->stBufAddr[0].u32PhyAddr_CHead,
                                pstFrm->stBufAddr[0].u32Stride_Y,
                                pstFrm->stBufAddr[0].u32Stride_C,
                                pstFrm->stBufAddr[1].u32PhyAddr_Y,
                                pstFrm->stBufAddr[1].u32PhyAddr_C,
                                pstFrm->stBufAddr[1].u32PhyAddr_YHead,
                                pstFrm->stBufAddr[1].u32PhyAddr_CHead,
                                pstFrm->stBufAddr[1].u32Stride_Y,
                                pstFrm->stBufAddr[1].u32Stride_C);
                    }
                    break;
                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:

                    u32Count = u32DbgPart - DEF_DBG_PORT0_ID;
                    pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
                    if (pstPortDbg->unInfo.bits.frameinfo)
                    {
                        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
                        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrm->u32Priv[0]);

                        HI_PRINT("Frame Info:Index %d Type %d Format %d W %d H %d LW %d LH %d PTS %d Rate %d Cnt %d Fidelity %d u32LastFlag %d,oriField %d,BitWidth %d,u32TunnelPhyAddr %x AspW %d AspH %d\n",
                                pstFrm->u32FrameIndex,
                                pstFrm->eFrmType,
                                pstFrm->ePixFormat,
                                pstFrm->u32Width,
                                pstFrm->u32Height,
                                pstFrm->stLbxInfo.s32Width,
                                pstFrm->stLbxInfo.s32Height,
                                pstFrm->u32Pts,
                                pstFrm->u32FrameRate,
                                pstPriv->u32FrmCnt,
                                pstPriv->u32Fidelity,
                                pstPriv->u32LastFlag,
                                pstPriv->eOriginField,
                                pstFrm->enBitWidth,
                                pstFrm->u32TunnelPhyAddr,
								pstFrm->u32AspectWidth,
								pstFrm->u32AspectHeight);
                    }
                    break;
                default:
                    VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
                    break;
            }
            break;
        case DBG_INFO_ASP:
            break;
        case DBG_SET_UHD:
			{
				VPSS_DBG_INST_S *pstInstDbg;
				HI_U32 *pu32LevelW;
				HI_U32 *pu32LevelH;

				pstInstDbg = &(pstDbg->stInstDbg);
				pu32LevelW = (HI_U32 *)para1;
				pu32LevelH = (HI_U32 *)para2;

				if (pstInstDbg->u32UhdSetW != 0 && pstInstDbg->u32UhdSetH != 0)
				{
					*pu32LevelW = pstInstDbg->u32UhdSetW;
					*pu32LevelH = pstInstDbg->u32UhdSetH;
				}
			}
            break;
        case DBG_SET_UV_INVERT:
            switch (u32DbgPart)
            {
				HI_BOOL *bUV_Invert;

                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
					u32Count = u32DbgPart - DEF_DBG_PORT0_ID;
					pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
					bUV_Invert = (HI_BOOL *)para2;

					if (pstPortDbg->unInfo.bits.uv_invert)
					{
						*bUV_Invert = HI_TRUE;
					}
					else
					{
						*bUV_Invert = HI_FALSE;
					}
					break;
				default:
				VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
			}
            break;
		case DBG_SET_CMP_ON:
            switch (u32DbgPart)
            {
				HI_U32 *bCmpflag;

                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
					u32Count = u32DbgPart - DEF_DBG_PORT0_ID;
					pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
					bCmpflag = (HI_U32 *)para2;

					if (pstPortDbg->unInfo.bits.cmp == 1 )
					{
						*bCmpflag = 1;
					}
					break;
				default:
				VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
			}
            break;
		case DBG_SET_CMP_OFF:
            switch (u32DbgPart)
            {
				HI_U32 *bCmpflag;

                case DEF_DBG_PORT0_ID:
                case DEF_DBG_PORT1_ID:
                case DEF_DBG_PORT2_ID:
					u32Count = u32DbgPart - DEF_DBG_PORT0_ID;
					pstPortDbg = &(pstDbg->stPortDbg[u32Count]);
					bCmpflag = (HI_U32 *)para2;

					if (pstPortDbg->unInfo.bits.cmp == 2 )
					{
						*bCmpflag = 2;
					}
					break;
				default:
				VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
			}
            break;
        case DBG_SET_I2P:
            switch (u32DbgPart)
            {
				HI_BOOL *bDei;
                case DEF_DBG_SRC_ID:
					bDei = (HI_BOOL *)para2;

					if (pstDbg->stInstDbg.unInfo.bits.dei)
					{
						*bDei = HI_TRUE;
					}
					else
					{
						*bDei = HI_FALSE;
					}
					break;
				default:
				VPSS_FATAL("Invalid para2 %#x\n",u32DbgPart);
			}
            break;
        case DBG_SET_FRMRATE:
            {
				HI_BOOL *frameEn;
				HI_U32  *framerate;

				frameEn = (HI_BOOL *)para1;
				framerate = (HI_U32 *)para2;

				if (pstDbg->stInstDbg.unInfo.bits.frameEn)
				{
					*frameEn = HI_TRUE;
				}
				else
				{
					*frameEn = HI_FALSE;
				}

				*framerate = pstDbg->stInstDbg.unInfo.bits.framerate;
			}
            break;
        default:
            VPSS_FATAL("Invalid para1 cmd=%#x\n",enCmd);
            break;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

