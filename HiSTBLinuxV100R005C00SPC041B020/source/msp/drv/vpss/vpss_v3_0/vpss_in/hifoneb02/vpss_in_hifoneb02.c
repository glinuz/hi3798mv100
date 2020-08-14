
#include "vpss_in_hifoneb02.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_S32 VPSS_IN_VFREE_V1(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (pstEntity->pstSrcImagesList)
    {
        s32Ret |= VPSS_IMG_DeInit(pstEntity->pstSrcImagesList);
        
        VPSS_VFREE(pstEntity->pstSrcImagesList);
    }

    if (pstEntity->pstHisInfo)
    {
        s32Ret |= VPSS_HIS_DeInit(pstEntity->pstHisInfo);
        
        VPSS_VFREE(pstEntity->pstHisInfo);
    }

    //t00273561
    if (pstEntity->pstSttWbc[0])
        VPSS_VFREE(pstEntity->pstSttWbc[0]);
    if (pstEntity->pstSttWbc[1])
        VPSS_VFREE(pstEntity->pstSttWbc[1]);
 
    return s32Ret;
}

HI_S32 VPSS_IN_VMALLOC_V1(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret;
    
    pstEntity->pstSrcImagesList = 
        VPSS_VMALLOC(sizeof(VPSS_IMAGELIST_INFO_S));
    if (!pstEntity->pstSrcImagesList)
    {
        goto V1_VMALLOC_ERROR;
    }

    s32Ret = VPSS_IMG_Init(pstEntity->pstSrcImagesList);
    if (HI_SUCCESS != s32Ret)
    {
        goto V1_VMALLOC_ERROR;
    }

    pstEntity->pstHisInfo = 
        VPSS_VMALLOC(sizeof(VPSS_HIS_INFO_S));
    if (!pstEntity->pstHisInfo)
    {
        goto V1_VMALLOC_ERROR;
    }

    s32Ret = VPSS_HIS_Init(pstEntity->pstHisInfo,pstEntity->bSecure);
    if (HI_SUCCESS != s32Ret)
    {
        goto V1_VMALLOC_ERROR;
    }
        
    pstEntity->pstSttWbc[0] = (VPSS_STTWBC_S*)VPSS_VMALLOC(sizeof(VPSS_STTWBC_S)*1);
    if (HI_NULL == pstEntity->pstSttWbc[0])
    {
        VPSS_ERROR("malloc VPSS_STTWBC_S failed\n");
        goto V1_VMALLOC_ERROR;
    }
    memset(pstEntity->pstSttWbc[0],0,sizeof(VPSS_STTWBC_S));

    pstEntity->pstSttWbc[1] = (VPSS_STTWBC_S*)VPSS_VMALLOC(sizeof(VPSS_STTWBC_S));
    if (HI_NULL == pstEntity->pstSttWbc[1])
    {
        VPSS_ERROR("malloc VPSS_STTWBC_S failed\n");
        goto V1_VMALLOC_ERROR;
    }
    memset(pstEntity->pstSttWbc[1],0,sizeof(VPSS_STTWBC_S));

    return HI_SUCCESS;
    
V1_VMALLOC_ERROR:
    return HI_FAILURE;
}

HI_BOOL VPSS_IN_CheckImage_V1(HI_DRV_VIDEO_FRAME_S *pstImage)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_BOOL bSupported = HI_TRUE;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);

    if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
        return HI_TRUE;
    }    
    if (pstImage->ePixFormat != HI_DRV_PIX_FMT_NV12
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV21
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV12_CMP
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV21_CMP
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV12_TILE
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV21_TILE
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV12_TILE_CMP
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV21_TILE_CMP
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV16
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV61
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV16_2X1
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_NV61_2X1
        && pstImage->ePixFormat !=  HI_DRV_PIX_FMT_YUV400
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV_444
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV422_2X1
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV422_1X2
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV420p
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV411
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV410p
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUYV
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YVYU
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_UYVY
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_ARGB8888
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_ABGR8888
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_ARGB1555
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_ABGR1555
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_RGB565
        )

    {
        VPSS_FATAL("In image can't be processed Pixformat %d\n",pstImage->ePixFormat);
        bSupported = HI_FALSE;
    }

    if (pstImage->eFrmType >= HI_DRV_FT_BUTT)
    {
        VPSS_FATAL("In image can't be processed FrmType %d\n",pstImage->eFrmType);
        bSupported = HI_FALSE;
    }

    if ((pstImage->u32Height < VPSS_FRAME_MIN_HEIGHT) || (pstImage->u32Width < VPSS_FRAME_MIN_WIDTH)
        || (pstImage->u32Height > VPSS_FRAME_MAX_HEIGHT) || (pstImage->u32Width > VPSS_FRAME_MAX_WIDTH))
    {
        VPSS_FATAL("In image can't be processed H %d W %d\n",
            pstImage->u32Height,
            pstImage->u32Width);
        bSupported = HI_FALSE;
    } 
    
    return bSupported;
}

HI_S32 VPSS_IN_CorrectFieldOrder(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pImage)
{
    VPSS_IN_STREAM_INFO_S *pstStreamInfo;

    pstStreamInfo = &(pstEntity->stStreamInfo);
    
    /*
      * when get first image ,believe its topfirst info
      */
    if(pstStreamInfo->u32RealTopFirst == DEF_TOPFIRST_BUTT)
    {
        if(pImage->bProgressive == HI_TRUE)
        {
            pstStreamInfo->u32RealTopFirst = DEF_TOPFIRST_PROG;
        }
        else
        {
            pstStreamInfo->u32RealTopFirst = 
                        pImage->bTopFieldFirst;
        }
    }
    else
    {
        /*
           * detect progressive
           */
        if (pstStreamInfo->u32RealTopFirst == DEF_TOPFIRST_PROG)
        {
            if(pImage->bProgressive == HI_TRUE)
            {
                
            }
            else
            {
                pstStreamInfo->u32RealTopFirst = 
                    pImage->bTopFieldFirst;
            }
        }
        /*
           * detect interlace
           */
        else
        {
            if(pImage->bProgressive == HI_TRUE)
            {
                pstStreamInfo->u32RealTopFirst = 
                    DEF_TOPFIRST_PROG;
            }
            else
            {
                pImage->bTopFieldFirst = 
                    pstStreamInfo->u32RealTopFirst;
            }
        }
        
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_IN_CorrectProgInfo(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pImage)
{
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
    HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
    
    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pImage->u32Priv[0]);
    pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);

    if (pstFrmPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
        return HI_SUCCESS;
    }

    if (pstEntity->bAlwaysFlushSrc == HI_TRUE)
    {
        pImage->bProgressive = HI_TRUE;
        return  HI_SUCCESS;
    }
    
    if (pstFrmPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
    {
        return HI_SUCCESS;
    }
    
    // belive VI, VI will give the right info
    if (((pImage->hTunnelSrc >> 16) & 0xff) == HI_ID_VI)
    {
        return HI_SUCCESS;
    }

    if (pstFrmPriv->stVideoOriginalInfo.enSource != HI_DRV_SOURCE_DTV)
    {
        return HI_SUCCESS;
    }
    
    if ( 0x2 == (pstVdecPriv->u8Marker &= 0x2)) 
    {
        pImage->bProgressive = HI_TRUE;
        return  HI_SUCCESS;
    }

    if ( pImage->u32Height > 1088 || pImage->u32Width > 1920) 
    {
        pImage->bProgressive = HI_TRUE;
        return  HI_SUCCESS;
    }
    
    if ( pImage->u32Height == 1080 && pImage->u32Width == 1920) 
    {
        if (pImage->u32FrameRate > 50000)
        {
            pImage->bProgressive = HI_TRUE;
            return  HI_SUCCESS;
        }
    }

    if ((pImage->eFrmType == HI_DRV_FT_SBS) || 
        (pImage->eFrmType == HI_DRV_FT_TAB) ||
        (pImage->eFrmType == HI_DRV_FT_FPK))
    {
        pImage->bProgressive = HI_TRUE;
        return HI_SUCCESS;
    }
    
    
    if (pImage->ePixFormat == HI_DRV_PIX_FMT_YUYV
        || pImage->ePixFormat == HI_DRV_PIX_FMT_YVYU
        || pImage->ePixFormat == HI_DRV_PIX_FMT_UYVY)
    {
        pImage->bProgressive = HI_TRUE;
        return HI_SUCCESS;
    }
    
    if (pstEntity->bProgRevise == HI_FALSE)
    {
		pImage->bProgressive = HI_TRUE;
        return HI_SUCCESS;
    }
    if(pstEntity->enProgInfo == HI_DRV_VPSS_PRODETECT_AUTO)
    {
        if (  (HI_UNF_VCODEC_TYPE_REAL8 == pstVdecPriv->entype)
                ||(HI_UNF_VCODEC_TYPE_REAL9 == pstVdecPriv->entype)
                ||(HI_UNF_VCODEC_TYPE_MPEG4 == pstVdecPriv->entype))
        {
            return HI_SUCCESS;
        }
        
        if(pImage->u32Height == 720)
        {
            pImage->bProgressive = HI_TRUE;
        }
        /* un-trust bit-stream information  */
        else if(pImage->u32Height <= 576)
        {
            if ( (240 >= pImage->u32Height) && (320 >= pImage->u32Width) )
            {
                pImage->bProgressive = HI_TRUE;
            }
            else if (pImage->u32Height <= (pImage->u32Width * 9 / 14 ) ) 
            {
                // Rule: wide aspect ratio stream is normal progressive, we think that progressive info is correct.
            }
            else
            {
                pImage->bProgressive = HI_FALSE;
            }
        }
        else
        {

        }
    }
	
    else if(pstEntity->enProgInfo == HI_DRV_VPSS_PRODETECT_INTERLACE)
    {
        pImage->bProgressive = HI_FALSE;
    }
	else if (pstEntity->enProgInfo == HI_DRV_VPSS_PRODETECT_PROGRESSIVE)
	{
        if (pImage->bProgressive == HI_FALSE)
        {
            if (pImage->bTopFieldFirst == HI_TRUE)
            {
                pImage->bProgressive = HI_TRUE;
                pImage->enFieldMode = HI_DRV_FIELD_BOTTOM;
            }
            else
            {
                pImage->bProgressive = HI_TRUE;
                pImage->enFieldMode = HI_DRV_FIELD_TOP;
            }
        }
    }
    else
    {
        VPSS_FATAL("Invalid ProgInfo %d\n",pstEntity->enProgInfo);
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_IN_ReviseImage(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pImage)
{
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;

    /*1.Revise image height to 4X*/
    pImage->u32Height =  pImage->u32Height & 0xfffffffc;
    if (pImage->u32Width > 1920)
    {
        pImage->u32Width =  pImage->u32Width & 0xfffffffc;    
    }  

    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pImage->u32Priv[0]);
    if (pstFrmPriv->stVideoOriginalInfo.enSource == HI_DRV_SOURCE_DTV)
    {
        pstFrmPriv->stVideoOriginalInfo.bInterlace = (pImage->bProgressive == HI_TRUE) ? HI_FALSE : HI_TRUE;
        pstFrmPriv->stVideoOriginalInfo.u32FrmRate = pstEntity->stStreamInfo.u32InRate * 1000;

        if (pstEntity->enSrcCS == HI_DRV_CS_UNKNOWN)
        {
            if (pImage->u32Width >= 1280 || pImage->u32Height >= 720)
            {
                pstFrmPriv->eColorSpace  = HI_DRV_CS_BT709_YUV_LIMITED;
            }
            else
            {
                pstFrmPriv->eColorSpace  = HI_DRV_CS_BT601_YUV_LIMITED;
            }
        }
        else
        {
            pstFrmPriv->eColorSpace = pstEntity->enSrcCS;        
        }
    }  
    
    if (pstEntity->stStreamInfo.u32RealTopFirst != DEF_TOPFIRST_BUTT
        && pstEntity->stStreamInfo.u32StreamProg == pImage->bProgressive)
    {
        pImage->bTopFieldFirst = pstEntity->stStreamInfo.u32RealTopFirst; 
    }
    
    /*
      * 2. 3d addr revise
      * SBS TAB read half image
      * MVC read two addr
      */
    if ((pImage->eFrmType == HI_DRV_FT_SBS) || (pImage->eFrmType == HI_DRV_FT_TAB))
    {
        memcpy(&(pImage->stBufAddr[1]), &(pImage->stBufAddr[0]),
               sizeof(HI_DRV_VID_FRAME_ADDR_S));

        switch(pImage->ePixFormat)
        {
            case HI_DRV_PIX_FMT_NV12_TILE:
            case HI_DRV_PIX_FMT_NV21_TILE:
                /* 对于非压缩的格式，将宽高直接除根据格式进行除2 */
                if(pImage->eFrmType == HI_DRV_FT_SBS)
                {
                    pImage->u32Width = pImage->u32Width/2;
                    if(pImage->u32Width%2 != 0)
                    {
                        VPSS_FATAL("3D image can't be processed W %d\n",
                            pImage->u32Width);
                    }
                }

                if(pImage->eFrmType == HI_DRV_FT_TAB)
                {
                    pImage->u32Height = pImage->u32Height/2;
                    if(pImage->u32Height%2 != 0)
                    {
                        VPSS_FATAL("3D image can't be processed H %d\n",
                            pImage->u32Height);
                    }
                }

                break;
            default:
                 VPSS_FATAL("3D tile image can't be processed ePixFormat %d\n",
                    pImage->ePixFormat);
        }
    }
    
    /*
     *revise frame rate
     */
    if (pImage->u32FrameRate == 0)
    {
        pImage->u32FrameRate = 25000;
    }
    /*
     * if not top/bottom Interleaved,force Split
     */
    if (pImage->enFieldMode != HI_DRV_FIELD_ALL)
    {
        pImage->bProgressive = HI_TRUE;
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_IN_ChangeInRate(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pstSrcImage)
{
    HI_U32 u32HzRate; /*0 -- 100*/
	HI_U32 u32InRate;

    if (pstSrcImage->bProgressive == HI_FALSE 
			&& pstSrcImage->enFieldMode == HI_DRV_FIELD_ALL)
    {
		u32InRate = pstSrcImage->u32FrameRate*2;
	}
	else
	{
		u32InRate = pstSrcImage->u32FrameRate;
	}
    
    u32HzRate = u32InRate / 1000;

    if(u32HzRate < 10)
    {
        u32HzRate = 1;
    }
    else if(u32HzRate < 20)
    {
        u32HzRate = 10;
    }
    else if(u32HzRate < 30)
    {
        u32HzRate = 25;
    }
    else if(u32HzRate < 40)
    {
        u32HzRate = 30;
    }
    else if(u32HzRate < 60)
    {
        u32HzRate = 50;
    }
    else
    {
        u32HzRate = u32HzRate / 10 * 10;
    }
    
    pstEntity->stStreamInfo.u32InRate = u32HzRate;    

    return HI_SUCCESS;
}

HI_S32 VPSS_IN_UpdateStreamInfo(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pstSrcImage)
{
    VPSS_IN_STREAM_INFO_S *pstStreamInfo;
    pstStreamInfo = &(pstEntity->stStreamInfo);

    if(pstStreamInfo->u32StreamProg != pstSrcImage->bProgressive)
    {
        pstStreamInfo->u32IsNewImage = HI_TRUE;
    }   
    else if(pstStreamInfo->u32StreamH != pstSrcImage->u32Height)
    {
        pstStreamInfo->u32IsNewImage = HI_TRUE;
    }
    else if(pstStreamInfo->u32StreamW != pstSrcImage->u32Width)
    {
        pstStreamInfo->u32IsNewImage = HI_TRUE;
    }
    else if(pstStreamInfo->eStreamFrmType != pstSrcImage->eFrmType)
    {
        pstStreamInfo->u32IsNewImage = HI_TRUE;
    }    
    else
    {
        pstStreamInfo->u32IsNewImage = HI_FALSE;
    }

    if(HI_TRUE == pstEntity->stTransFbInfo.bNeedTrans)
    {
        pstStreamInfo->u32IsNewImage = HI_TRUE;
    }

    if(HI_TRUE == pstStreamInfo->u32IsNewImage)
    {
        pstEntity->u32ScenceChgCnt++;
    }
    
    pstStreamInfo->u32StreamInRate = pstSrcImage->u32FrameRate;
    pstStreamInfo->u32StreamTopFirst = pstSrcImage->bTopFieldFirst;
    pstStreamInfo->u32StreamProg = pstSrcImage->bProgressive;
    pstStreamInfo->u32StreamH = pstSrcImage->u32Height;
    pstStreamInfo->u32StreamW = pstSrcImage->u32Width;
    pstStreamInfo->eStreamFrmType = pstSrcImage->eFrmType;
    pstStreamInfo->ePixFormat = pstSrcImage->ePixFormat;
    pstStreamInfo->enBitWidth = pstSrcImage->enBitWidth;   
	pstStreamInfo->u32FrameIndex = pstSrcImage->u32FrameIndex;
	pstStreamInfo->u32Pts = pstSrcImage->u32Pts;
    
    return HI_SUCCESS;
}

HI_S32 VPSS_IN_Refresh_V1(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bSupport = HI_TRUE;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_DRV_VIDEO_FRAME_S stSrcImage;
    HI_DRV_VIDEO_FRAME_S *pstImage;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V1_0);

    VPSS_CHECK_NULL(pstEntity->pfnAcqCallback);

    VPSS_CHECK_NULL(pstEntity->pfnRlsCallback);

    if(VPSS_IMG_CheckImageList(pstEntity->pstSrcImagesList) == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    if (!VPSS_IMG_CheckEmptyNode(pstEntity->pstSrcImagesList))
    {
        return HI_FAILURE;
    }

    memset(&stSrcImage,0,sizeof(HI_DRV_VIDEO_FRAME_S));

    pstImage = &stSrcImage;
    
    pstEntity->pstSrcImagesList->u32GetUsrTotal++;
	pstEntity->stListState.u32GetUsrTotal = pstEntity->pstSrcImagesList->u32GetUsrTotal;
    
    s32Ret = pstEntity->pfnAcqCallback(pstEntity->hSource,pstImage);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

	if( HI_TRUE == pstEntity->stDbginfo.frameEn )
	{
		pstImage->u32FrameRate = pstEntity->stDbginfo.framerate*1000;
	}
	
    if (0)
    {
        HI_DRV_VIDEO_FRAME_S *pstFrm;
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;

        pstFrm = pstImage;
        
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrm->u32Priv[0]);
        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstPriv->u32Reserve[0]);
        

        HI_PRINT("Image Info:Index %d Type %d Format %d W %d H %d Prog %d FieldMode %d PTS %d Rate %d LastFlag %#x Delta %d CodeType %d,SourceType %d,BitWidth %d,u32TunnelPhyAddr %x\n"
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
                pstFrm->u32TunnelPhyAddr,
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

	if (pstImage->bSecure != pstEntity->bSecure)
	{
		pstEntity->bSecure = pstImage->bSecure;
		pstEntity->bSecure = HI_FALSE;
		s32Ret |= VPSS_HIS_DeInit(pstEntity->pstHisInfo);

		VPSS_VFREE(pstEntity->pstHisInfo);

		pstEntity->pstHisInfo = 
			VPSS_VMALLOC(sizeof(VPSS_HIS_INFO_S));

		(HI_VOID)VPSS_HIS_Init(pstEntity->pstHisInfo,pstEntity->bSecure);
	}
    bSupport = VPSS_IN_CheckImage_V1(pstImage);
    if (HI_TRUE != bSupport)
    {
        pstEntity->pstSrcImagesList->u32RelUsrTotal ++;
		pstEntity->stListState.u32RelUsrTotal = pstEntity->pstSrcImagesList->u32RelUsrTotal;
        pstEntity->pfnRlsCallback(pstEntity->hSource,pstImage);
        pstEntity->pstSrcImagesList->u32RelUsrSuccess++;
		pstEntity->stListState.u32RelUsrSuccess = pstEntity->pstSrcImagesList->u32RelUsrSuccess;
        return HI_FAILURE;
    }

    if (0)
    {
        HI_U8 chFile[20] = "vpss_in.yuv";
        VPSS_OSAL_WRITEYUV(pstImage, chFile);
    }

	if(VPSS_TRANS_FB_NeedTrans(&(pstEntity->stTransFbInfo),pstImage))
	{
		if (0)
		{
			HI_U8 chFile[256] = "vpss_trans_frm_in.yuv";
			VPSS_OSAL_WRITEYUV(pstImage, chFile);
		}
		if(HI_SUCCESS == VPSS_TRANS_FB_PutImage(&(pstEntity->stTransFbInfo),pstImage))
		{
			VPSS_INFO("\n-------%d------Trans Image success,frameindex=%d", 
				pstEntity->hSource,pstImage->u32FrameIndex);
		}
		else
		{
			pstEntity->pfnRlsCallback(pstEntity->hSource,pstImage);			 
			VPSS_INFO("\n-------%d------Trans Image failed,frameindex=%d", 
				pstEntity->hSource,pstImage->u32FrameIndex);
		}
		//return failuer,because vpss logic can't work
		return HI_FAILURE;
	}
	
	pstEntity->pstSrcImagesList->u32GetUsrSuccess++;
	pstEntity->stListState.u32GetUsrSuccess = pstEntity->pstSrcImagesList->u32GetUsrSuccess;
		
	
    {
        HI_LD_Event_S evt;
        HI_U32 TmpTime = 0;
		HI_DRV_SYS_GetTimeStampMs(&TmpTime);
        evt.evt_id = EVENT_VPSS_FRM_IN;
        evt.frame = pstImage->u32FrameIndex;
        evt.handle = pstImage->hTunnelSrc;
        evt.time = TmpTime; 
        HI_DRV_LD_Notify_Event(&evt);
    }

    
    if (pstImage->bIsFirstIFrame)
    {
        HI_DRV_STAT_Event(STAT_EVENT_VPSSGETFRM, 0);
    }
        
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
    
    if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
    {
		
    }
    else
    {
        (HI_VOID)VPSS_IN_CorrectProgInfo(pstEntity, pstImage);
        
        (HI_VOID)VPSS_IN_ReviseImage(pstEntity, pstImage);
        
        (HI_VOID)VPSS_IN_ChangeInRate(pstEntity, pstImage);
        
        (HI_VOID)VPSS_IN_CorrectFieldOrder(pstEntity, pstImage);

		if (HI_TRUE == pstEntity->stDbginfo.bDeiDisable)
		{
			pstImage->enFieldMode = HI_DRV_FIELD_ALL;
			pstImage->bProgressive = HI_TRUE;
		}
		else
		{
		}			

        (HI_VOID)VPSS_IN_UpdateStreamInfo(pstEntity, pstImage);
		pstEntity->stTransFbInfo.bNeedTrans = HI_FALSE;

        if (HI_TRUE == pstEntity->stStreamInfo.u32IsNewImage)
        {
            //t00273561
            HI_S32 s32InitListRet = HI_FAILURE;
            VPSS_STTWBC_S* psttWbc;
            psttWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[0];
            VPSS_CHECK_NULL(psttWbc);
            if (HI_DRV_FT_NOT_STEREO == pstImage->eFrmType)
            {
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);
            }
            else
            {
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);
				psttWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[1];
				VPSS_CHECK_NULL(psttWbc);
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);
            }

            pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
            pstEntity->stOriInfo.enSource = pstPriv->stVideoOriginalInfo.enSource;
            pstEntity->stOriInfo.u32Width = pstPriv->stVideoOriginalInfo.u32Width;
            pstEntity->stOriInfo.u32Height = pstPriv->stVideoOriginalInfo.u32Height;
            pstEntity->stOriInfo.u32FrmRate = pstPriv->stVideoOriginalInfo.u32FrmRate;
            pstEntity->stOriInfo.bInterlace = (pstImage->bProgressive == HI_TRUE) ? HI_FALSE : HI_TRUE;
            pstEntity->stOriInfo.enColorSys = pstPriv->stVideoOriginalInfo.enColorSys;
        }
    }
    
    if (HI_SUCCESS != VPSS_IMG_AddNewImg(pstEntity->pstSrcImagesList, pstImage))
    {
        VPSS_ERROR("Add New Image Failed\n");
        pstEntity->pfnRlsCallback(pstEntity->hSource,pstImage);
        return HI_FAILURE;
    }

    if(VPSS_IMG_CheckImageList(pstEntity->pstSrcImagesList) == HI_TRUE)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 VPSS_IN_GetProcessImage_V1(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S **ppstFrame)
{
    return VPSS_IMG_GetProcessImg(pstEntity->pstSrcImagesList,
                               ppstFrame);
}

HI_S32 VPSS_IN_GetInfo_V1(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_INFO_TYPE_E enType,
                                    HI_DRV_BUF_ADDR_E enLR, 
                                    HI_VOID* pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V1_0);
    
    switch(enType)
    {
        case VPSS_IN_INFO_WBC:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                s32Ret = HI_SUCCESS;
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_REF:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                s32Ret = HI_SUCCESS;
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;

        case VPSS_IN_INFO_WBCREG_VA:
            if (HI_DRV_BUF_ADDR_MAX != enLR)
            {
                HI_U32 *pu32stt_w_vir_addr;
                HI_U32 u32stt_w_phy_addr;
                pu32stt_w_vir_addr = (HI_U32 *)pstInfo;
                VPSS_STTINFO_SttWbcGetAddr(pstEntity->pstSttWbc[enLR],
                        &u32stt_w_phy_addr,     
                        pu32stt_w_vir_addr);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;

        case VPSS_IN_INFO_WBCREG_PA:
            if (HI_DRV_BUF_ADDR_MAX != enLR)
            {
                HI_U32 u32stt_w_vir_addr;
                HI_U32 *pu32stt_w_phy_addr;
                pu32stt_w_phy_addr = (HI_U32 *)pstInfo;
                VPSS_STTINFO_SttWbcGetAddr(pstEntity->pstSttWbc[enLR],
                        pu32stt_w_phy_addr,     
                        &u32stt_w_vir_addr);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;

        case VPSS_IN_INFO_STREAM:
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                VPSS_IN_STREAM_INFO_S *pstStreamInfo;
                pstStreamInfo = (VPSS_IN_STREAM_INFO_S *)pstInfo;
                memcpy(pstStreamInfo,&(pstEntity->stStreamInfo),
                        sizeof(VPSS_IN_STREAM_INFO_S));
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_FIELD:
            if (HI_DRV_BUF_ADDR_LEFT == enLR
                || HI_DRV_BUF_ADDR_RIGHT == enLR)
            {
                HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr;
                pstFieldAddr = (HI_DRV_VID_FRAME_ADDR_S *)pstInfo;
                VPSS_IMG_GetFieldAddr(pstEntity->pstSrcImagesList,
                                     pstFieldAddr,
                                     enLR);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_HIS_ADDR:
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                VPSS_HIS_ADDR_S *pstHisAddr;
                pstHisAddr = (VPSS_HIS_ADDR_S *)pstInfo;
                VPSS_HIS_GetAddr(pstEntity->pstHisInfo,
                                     pstHisAddr);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_CORRECT_FIELD:
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                HI_BOOL bTopFirst;
                bTopFirst = *(HI_BOOL *)pstInfo;
                if(bTopFirst == pstEntity->stStreamInfo.u32RealTopFirst)
                {
                    s32Ret = HI_SUCCESS;
                    goto GET_DONE;
                }
                 
                if(pstEntity->stStreamInfo.u32RealTopFirst == DEF_TOPFIRST_BUTT)
                {
                    pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
                    s32Ret = HI_SUCCESS;
                    goto GET_DONE;
                }

                s32Ret = VPSS_IMG_CorrectListOrder(pstEntity->pstSrcImagesList, bTopFirst);

                pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
                
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        default:
            VPSS_ERROR("Invalid Info Type %d\n",enType);
            s32Ret = HI_FAILURE;
    }
    
GET_DONE:
    return s32Ret;
}


HI_S32 VPSS_IN_SetInfo_V1 ( VPSS_IN_ENTITY_S *pstEntity,
                                    VPSS_SET_INFO_TYPE_E enType,
                                    HI_DRV_BUF_ADDR_E enLR,
                                    HI_VOID* pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V1_0);
    
    switch(enType)
    {
    case VPSS_SET_INFO_INPUT_CFG:
		{
			VPSS_HAL_INFO_S *pstHalInfo;

			VPSS_IN_INTF_S stInIntf;
			s32Ret = VPSS_IN_GetIntf(pstEntity, &stInIntf);
			if (HI_SUCCESS != s32Ret)
			{
				VPSS_ERROR("Get Interface Failed\n");
				return HI_FAILURE;
			}

			pstHalInfo = (VPSS_HAL_INFO_S *)pstInfo;
				
			VPSS_CHECK_NULL(stInIntf.pfnGetInfo);
	
			stInIntf.pfnGetInfo(pstEntity, VPSS_IN_INFO_WBCREG_PA, enLR, &pstHalInfo->u32stt_w_phy_addr);
			stInIntf.pfnGetInfo(pstEntity, VPSS_IN_INFO_WBCREG_VA, enLR, &pstHalInfo->u32stt_w_vir_addr);

			switch(pstHalInfo->enNodeType)
			{
				case VPSS_HAL_NODE_2D_FRAME:
				case VPSS_HAL_NODE_2D_Field:
				case VPSS_HAL_NODE_3D_FRAME_R:
					break;
				case VPSS_HAL_NODE_2D_5Field:
					stInIntf.pfnGetInfo(pstEntity,
											VPSS_IN_INFO_FIELD, 
											HI_DRV_BUF_ADDR_LEFT,
											 (HI_VOID*)pstHalInfo->stFieldAddr);
					stInIntf.pfnGetInfo(pstEntity,
											VPSS_IN_INFO_HIS_ADDR, 
											HI_DRV_BUF_ADDR_MAX,
											 (HI_VOID*)&(pstHalInfo->stHisAddr));
					break;
				case VPSS_HAL_NODE_UHD:
				case VPSS_HAL_NODE_UHD_SPLIT_L:
				case VPSS_HAL_NODE_UHD_SPLIT_R:
				case VPSS_HAL_NODE_UHD_HIGH_SPEED:
					/* Do Nothing */
					break;
				default:
					VPSS_FATAL("Node Type(%x) is Not Surport,\n", pstHalInfo->enNodeType);
					return HI_FAILURE;
			}
		}
        break;
    case VPSS_SET_INFO_PTS:
		break;
    default:
        VPSS_ERROR("Invalid set Info Type %d\n",enType);
        s32Ret = HI_FAILURE;
	}
			
	return s32Ret;
}
HI_S32 VPSS_IN_CompleteImage_V1(VPSS_IN_ENTITY_S *pstEntity)
{
	VPSS_STTWBC_S* psttWbc;
			//t00273561
    VPSS_CHECK_NULL(pstEntity->pstSttWbc);
    psttWbc = pstEntity->pstSttWbc[0];
    VPSS_STTINFO_SttWbcComplete(psttWbc);
    if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
    {
        VPSS_STTINFO_SttWbcComplete(pstEntity->pstSttWbc[1]);
    }

    VPSS_IMG_Complete(pstEntity->pstSrcImagesList);

	pstEntity->stListState.u32RelUsrTotal = pstEntity->pstSrcImagesList->u32RelUsrTotal;
	pstEntity->stListState.u32RelUsrSuccess = pstEntity->pstSrcImagesList->u32RelUsrSuccess;

    return HI_SUCCESS;
}

HI_S32 VPSS_IN_Reset_V1(VPSS_IN_ENTITY_S *pstEntity)
{ 
	VPSS_STTWBC_S* psttWbc;
		//t00273561
    VPSS_CHECK_NULL(pstEntity->pstSttWbc);
    psttWbc = pstEntity->pstSttWbc[0];
    VPSS_STTINFO_SttWbcDeInit(psttWbc);
    if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
    {
        VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);
    }

//	memset(&(pstEntity->stStreamInfo), 0, sizeof(VPSS_IN_STREAM_INFO_S));
	pstEntity->stStreamInfo.u32StreamH = 0;
	pstEntity->stStreamInfo.u32StreamW = 0;

    VPSS_IMG_Reset(pstEntity->pstSrcImagesList);
	
	memset(&(pstEntity->stListState),0,sizeof(VPSS_IN_IMAGELIST_STATE_S));
	return HI_SUCCESS;
}

HI_S32 VPSS_IN_GetIntf(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_INTF_S *pstIntf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);

    if (pstEntity->enVersion == VPSS_VERSION_V1_0)
    {
        pstIntf->pfnRefresh = VPSS_IN_Refresh_V1;
        pstIntf->pfnCompleteImage = VPSS_IN_CompleteImage_V1;
        pstIntf->pfnGetProcessImage = VPSS_IN_GetProcessImage_V1;
        pstIntf->pfnReset =  VPSS_IN_Reset_V1;
        pstIntf->pfnGetInfo = VPSS_IN_GetInfo_V1;
		pstIntf->pfnSetInfo = VPSS_IN_SetInfo_V1;
    }
    else 
    {
        VPSS_ERROR("Version %d can't support\n",pstEntity->enVersion);
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 VPSS_IN_Init(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_ENV_S stEnv)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VPSS_CHECK_NULL(pstEntity);

    pstEntity->stStreamInfo.u32RealTopFirst = DEF_TOPFIRST_BUTT;

	pstEntity->bSecure = stEnv.bSecure;
    pstEntity->bSecure = HI_FALSE;
	
    if (stEnv.enVersion == VPSS_VERSION_V1_0)
    {
        pstEntity->enVersion = VPSS_VERSION_V1_0;		
        s32Ret = VPSS_IN_VMALLOC_V1(pstEntity);
        if (HI_SUCCESS != s32Ret)
        {
           goto INIT_ERROR;
        }
    }
    else
    {
        VPSS_ERROR("Version %d can't support\n",stEnv.enVersion);
        s32Ret = HI_FAILURE;
    }
    
    return s32Ret;

INIT_ERROR:
    if (stEnv.enVersion == VPSS_VERSION_V1_0)
    {
        (HI_VOID)VPSS_IN_VFREE_V1(pstEntity);
    }
    else
    {
        VPSS_ERROR("Version %d can't support\n",stEnv.enVersion);
    }
    return HI_FAILURE;
}

HI_S32 VPSS_IN_DeInit(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    if (pstEntity->enVersion == VPSS_VERSION_V1_0)
    {
        VPSS_STTWBC_S* psttWbc;


        //t00273561
        psttWbc = pstEntity->pstSttWbc[0];
        VPSS_CHECK_NULL(pstEntity->pstSttWbc[0]);
        VPSS_STTINFO_SttWbcDeInit(psttWbc);
        if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
        {
            VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);
        }

        s32Ret = VPSS_IN_VFREE_V1(pstEntity);
    }
    else
    {
        VPSS_ERROR("VpssVersion %d can't support\n",pstEntity->enVersion);
        s32Ret = HI_FAILURE;
        goto DEINIT_OUT;
        
    }
DEINIT_OUT:
    return s32Ret;
}

HI_S32 VPSS_IN_SetAttr(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_ATTR_S stAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);

    pstEntity->enProgInfo = stAttr.enProgInfo;
    pstEntity->bProgRevise = stAttr.bProgRevise;
    pstEntity->bAlwaysFlushSrc = stAttr.bAlwaysFlushSrc;
    pstEntity->enSrcCS = stAttr.enSrcCS;

    return s32Ret;
}

HI_S32 VPSS_IN_GetAttr(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    pstAttr->enProgInfo = pstEntity->enProgInfo;
    pstAttr->bProgRevise = pstEntity->bProgRevise;
    pstAttr->bAlwaysFlushSrc = pstEntity->bAlwaysFlushSrc;

    return s32Ret;
}

HI_S32 VPSS_IN_SetSrcMode(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_SOURCE_S stMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);

    pstEntity->hSource = stMode.hSource;
    pstEntity->enMode = stMode.enMode;
    pstEntity->pfnAcqCallback = stMode.pfnAcqCallback;
    pstEntity->pfnRlsCallback = stMode.pfnRlsCallback;

    if (pstEntity->enVersion == VPSS_VERSION_V1_0)
    {
        VPSS_IMG_CALLBACK_S stCallback;
        stCallback.hSrc = pstEntity->hSource;
        stCallback.pfnAcqImage = (PFN_IMG_FUNC)pstEntity->pfnAcqCallback;
        stCallback.pfnRlsImage = (PFN_IMG_FUNC)pstEntity->pfnRlsCallback;
        VPSS_IMG_Regist(pstEntity->pstSrcImagesList, stCallback);
    }
    return s32Ret;
}
#ifdef __cplusplus
 #if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
