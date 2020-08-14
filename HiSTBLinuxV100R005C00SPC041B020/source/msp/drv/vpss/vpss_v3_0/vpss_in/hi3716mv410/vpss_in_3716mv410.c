#include "vpss_in_3716mv410.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_S32 VPSS_IN_VFREE_V3(VPSS_IN_ENTITY_S *pstEntity)
{
    if (pstEntity->pstSrc)
        VPSS_VFREE(pstEntity->pstSrc);
        
    if (pstEntity->pstWbcInfo[0])
        VPSS_VFREE(pstEntity->pstWbcInfo[0]);
    if (pstEntity->pstWbcInfo[1])
        VPSS_VFREE(pstEntity->pstWbcInfo[1]);
        
    if (pstEntity->pstSttWbc[0])
        VPSS_VFREE(pstEntity->pstSttWbc[0]);
    if (pstEntity->pstSttWbc[1])
        VPSS_VFREE(pstEntity->pstSttWbc[1]);
        
    if (pstEntity->pstDieStInfo[0])
        VPSS_VFREE(pstEntity->pstDieStInfo[0]);
    if (pstEntity->pstDieStInfo[1])
        VPSS_VFREE(pstEntity->pstDieStInfo[1]);
        
        
    if (pstEntity->pstNrMadInfo[0])
        VPSS_VFREE(pstEntity->pstNrMadInfo[0]);
    if (pstEntity->pstNrMadInfo[1])
        VPSS_VFREE(pstEntity->pstNrMadInfo[1]);

    return HI_SUCCESS;
}

HI_S32 VPSS_IN_ResetWbc(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S *pstImage)
{
        VPSS_WBC_ATTR_S stWbcAttr;
        VPSS_WBC_S* pstWbcInfo;
        HI_S32 s32Ret;
        pstWbcInfo = (VPSS_WBC_S*)pstEntity->pstWbcInfo[0];
        
        stWbcAttr.enBitWidth = pstImage->enBitWidth;
        stWbcAttr.enRefMode = VPSS_WBC_REF_MODE_NULL;
        stWbcAttr.ePixFormat = pstImage->ePixFormat;
        stWbcAttr.u32Height = pstImage->u32Height;
        stWbcAttr.u32Width = pstImage->u32Width;
		stWbcAttr.u32FrameIndex = pstImage->u32FrameIndex;
		stWbcAttr.u32Pts = pstImage->u32Pts;

		if (HI_TRUE == pstEntity->stStreamInfo.u32StreamProg)
        {
            stWbcAttr.enMode = VPSS_WBC_MODE_NORMAL;
        }
        else
        {
			stWbcAttr.enMode = VPSS_WBC_MODE_4FIELD;//for mv410
        }

        if(pstImage->u32Width > 1920)
        {
                s32Ret = VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
        }
        else
        {
            if (HI_DRV_FT_NOT_STEREO == pstImage->eFrmType)
            {
                
                s32Ret = VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
            }
            else
            {
                
                s32Ret = VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
				pstWbcInfo = (VPSS_WBC_S*)pstEntity->pstWbcInfo[1];
                s32Ret |= VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
            }
        }

        return s32Ret;
}


HI_S32 VPSS_IN_VMALLOC_V3(VPSS_IN_ENTITY_S *pstEntity)
{
    pstEntity->pstSrc = (VPSS_SRC_S*)VPSS_VMALLOC(sizeof(VPSS_SRC_S));
    if (HI_NULL == pstEntity->pstSrc)
    {
        VPSS_ERROR("malloc VPSS_SRC_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstSrc,0,sizeof(VPSS_SRC_S));
    
    pstEntity->pstWbcInfo[0] = (VPSS_WBC_S*)VPSS_VMALLOC(sizeof(VPSS_WBC_S));
    if (HI_NULL == pstEntity->pstWbcInfo[0])
    {
        VPSS_ERROR("malloc VPSS_WBC_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstWbcInfo[0],0,sizeof(VPSS_WBC_S));

    pstEntity->pstWbcInfo[1] = (VPSS_WBC_S*)VPSS_VMALLOC(sizeof(VPSS_WBC_S));
    if (HI_NULL == pstEntity->pstWbcInfo[1])
    {
        VPSS_ERROR("malloc VPSS_WBC_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstWbcInfo[1],0,sizeof(VPSS_WBC_S));
    
    pstEntity->pstSttWbc[0] = (VPSS_STTWBC_S*)VPSS_VMALLOC(sizeof(VPSS_STTWBC_S));
    if (HI_NULL == pstEntity->pstSttWbc[0])
    {
        VPSS_ERROR("malloc VPSS_STTWBC_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstSttWbc[0],0,sizeof(VPSS_STTWBC_S));

    pstEntity->pstSttWbc[1] = (VPSS_STTWBC_S*)VPSS_VMALLOC(sizeof(VPSS_STTWBC_S));
    if (HI_NULL == pstEntity->pstSttWbc[1])
    {
        VPSS_ERROR("malloc VPSS_STTWBC_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstSttWbc[1],0,sizeof(VPSS_STTWBC_S));
    
    pstEntity->pstDieStInfo[0] = (VPSS_DIESTINFO_S*)VPSS_VMALLOC(sizeof(VPSS_DIESTINFO_S));
    if (HI_NULL == pstEntity->pstDieStInfo[0])
    {
        VPSS_ERROR("malloc VPSS_DIESTINFO_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstDieStInfo[0],0,sizeof(VPSS_DIESTINFO_S));

    pstEntity->pstDieStInfo[1] = (VPSS_DIESTINFO_S*)VPSS_VMALLOC(sizeof(VPSS_DIESTINFO_S));
    if (HI_NULL == pstEntity->pstDieStInfo[1])
    {
        VPSS_ERROR("malloc VPSS_DIESTINFO_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstDieStInfo[1],0,sizeof(VPSS_DIESTINFO_S));

    pstEntity->pstNrMadInfo[0] = (VPSS_NRMADINFO_S*)VPSS_VMALLOC(sizeof(VPSS_NRMADINFO_S));
    if (HI_NULL == pstEntity->pstNrMadInfo[0])
    {
        VPSS_ERROR("malloc VPSS_NRMADINFO_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstNrMadInfo[0],0,sizeof(VPSS_NRMADINFO_S));

    pstEntity->pstNrMadInfo[1] = (VPSS_NRMADINFO_S*)VPSS_VMALLOC(sizeof(VPSS_NRMADINFO_S));
    if (HI_NULL == pstEntity->pstNrMadInfo[1])
    {
        VPSS_ERROR("malloc VPSS_NRMADINFO_S failed\n");
        goto V3_VMALLOC_ERROR;
    }
    memset(pstEntity->pstNrMadInfo[1],0,sizeof(VPSS_NRMADINFO_S));

    
    return HI_SUCCESS;

V3_VMALLOC_ERROR:
    return HI_FAILURE;
}

HI_BOOL VPSS_IN_CheckImage_V3(HI_DRV_VIDEO_FRAME_S *pstImage)
{
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_BOOL bSupported = HI_TRUE;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);

    if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG) 
    {
        return HI_FALSE;
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
        && pstImage->ePixFormat != HI_DRV_PIX_FMT_YUV400
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

    if (((pstImage->u32Height < VPSS_FRAME_MIN_HEIGHT) ||  (pstImage->u32Width < VPSS_FRAME_MIN_WIDTH)) 
		|| (pstImage->u32Width > VPSS_FRAME_MAX_WIDTH))
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
    
    if ( (pImage->u32Height == 1080 || pImage->u32Height == 1088)
			&& pImage->u32Width == 1920) 
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

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
	/* HEVC do not use resolution to justic P/I type, only use FieldMode */
	if (HI_UNF_VCODEC_TYPE_HEVC == pstVdecPriv->entype)
	{
        if (pImage->enFieldMode != HI_DRV_FIELD_ALL
            && pImage->enBufValidMode == HI_DRV_FIELD_ALL)
		{
			pImage->bProgressive = HI_FALSE;
		}

		return HI_SUCCESS;
	}	
#endif

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
	HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;

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
	pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
     
    if ((pImage->enFieldMode != HI_DRV_FIELD_ALL) && (pstVdecPriv->entype != HI_UNF_VCODEC_TYPE_HEVC))
    {
        pImage->bProgressive = HI_TRUE;
//        pImage->enFieldMode = HI_DRV_FIELD_ALL;
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
	else if (pstStreamInfo->bStreamByPass == HI_TRUE)
	{
        pstStreamInfo->u32IsNewImage = HI_TRUE;
		pstStreamInfo->bStreamByPass = HI_FALSE;
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
    pstStreamInfo->enCurFieldMode = pstSrcImage->enFieldMode;
    return HI_SUCCESS;
}

HI_BOOL VPSS_IN_CheckNeedNr(VPSS_IN_ENTITY_S *pstInEntity)
{
    if((pstInEntity->stStreamInfo.u32StreamW <= 1920) && (pstInEntity->stStreamInfo.u32StreamH <= 1088))
    {
        return HI_TRUE;
    }
	
    return HI_FALSE;
}

HI_VOID VPSS_IN_CopyHalFrameInfo(HI_DRV_VIDEO_FRAME_S *pstFrame,
    VPSS_HAL_FRAME_S *pstHalFrm, HI_DRV_BUF_ADDR_E enBufLR)
{
    if(pstFrame == HI_NULL || pstHalFrm == HI_NULL)
    {
        VPSS_FATAL("func=%s,pstFrame(%p), pstHalFrm(%p) NULL\n", __func__,pstFrame, pstHalFrm);
        return;
    }
    pstHalFrm->eFrmType = pstFrame->eFrmType;
    pstHalFrm->u32Width = pstFrame->u32Width;
    pstHalFrm->u32Height = pstFrame->u32Height;
    
    pstHalFrm->enFormat = pstFrame->ePixFormat;
    pstHalFrm->enFieldMode = pstFrame->enFieldMode;
    pstHalFrm->bProgressive = pstFrame->bProgressive;
    memcpy(&pstHalFrm->stAddr, &pstFrame->stBufAddr[enBufLR], sizeof(HI_DRV_VID_FRAME_ADDR_S));
    pstHalFrm->bCompressd = pstFrame->bCompressd;
    pstHalFrm->enBitWidth = pstFrame->enBitWidth;
    pstHalFrm->bTopFirst = pstFrame->bTopFieldFirst;

    return ;
}

HI_VOID VPSS_IN_ClearAllAlgBuffer(VPSS_IN_ENTITY_S *pstEntity)
{
    VPSS_WBC_S* pstWbcInfo;
    VPSS_STTWBC_S* psttWbc;
    VPSS_DIESTINFO_S* pstDieStInfo;
    VPSS_NRMADINFO_S* pstNrMadInfo;

	pstWbcInfo = pstEntity->pstWbcInfo[0];
    psttWbc = pstEntity->pstSttWbc[0];
    pstDieStInfo = pstEntity->pstDieStInfo[0];
    pstNrMadInfo = pstEntity->pstNrMadInfo[0];

    (HI_VOID)VPSS_WBC_DeInit(pstWbcInfo);
    (HI_VOID)VPSS_STTINFO_DieDeInit(pstDieStInfo);
    (HI_VOID)VPSS_STTINFO_NrDeInit(pstNrMadInfo);
    (HI_VOID)VPSS_STTINFO_SttWbcDeInit(psttWbc);

    (HI_VOID)VPSS_WBC_DeInit(pstEntity->pstWbcInfo[1]);
    (HI_VOID)VPSS_STTINFO_DieDeInit(pstEntity->pstDieStInfo[1]);
    (HI_VOID)VPSS_STTINFO_NrDeInit(pstEntity->pstNrMadInfo[1]);
    (HI_VOID)VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]); 

    return ;
}


HI_S32 VPSS_IN_GetSrcInitMode_V3(VPSS_IN_ENTITY_S *pstEntity)
{
    VPSS_IN_STREAM_INFO_S *pstStreamInfo;

    VPSS_CHECK_NULL(pstEntity);

    VPSS_CHECK_VERSION(pstEntity->enVersion, VPSS_VERSION_V3_0);
    
    pstStreamInfo = &(pstEntity->stStreamInfo);

    if(HI_TRUE == pstStreamInfo->u32StreamProg)
    {
        return SRC_MODE_FRAME;
    }
#if 0
    else if((720 == pstStreamInfo->u32StreamW) && (480 == pstStreamInfo->u32StreamH))
    {
        return SRC_MODE_NTSC;
    }
    else if((720 == pstStreamInfo->u32StreamW) && (576 == pstStreamInfo->u32StreamH))
    {
        return SRC_MODE_PAL;
    }
#endif
    else
    {
//        return SRC_MODE_FIELD;
        return SRC_MODE_NTSC;
    }

}

HI_S32 VPSS_IN_Refresh_V3(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bSupport = HI_TRUE;
    HI_S32 s32InitListRet = HI_FAILURE;
	unsigned long flags;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    HI_DRV_VIDEO_FRAME_S *pstImage;
	HI_DRV_VIDEO_FRAME_S *pstData;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V3_0);

    VPSS_CHECK_NULL(pstEntity->pfnAcqCallback);

    VPSS_CHECK_NULL(pstEntity->pfnRlsCallback);

    pstImage = &pstEntity->stSrcImage;

    /*
    if there are pictures not complete, do not receive picture again!!@f00241306
    */
	if (pstEntity->pstSrc->bInit)
	{
		if (pstEntity->pstSrc->bEnding != HI_TRUE)
		{
			VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin), &flags);
			s32Ret = VPSS_SRC_GetProcessImage(pstEntity->pstSrc, &pstData);
			VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin), &flags);
			if (HI_SUCCESS == s32Ret)
			{
				return HI_SUCCESS;
			}
		}
		else
		{
			if (!VPSS_SRC_CheckIsEmpty(pstEntity->pstSrc))
			{
				return HI_SUCCESS;
			}
			else
			{
				pstEntity->pstSrc->bEnding = HI_FALSE;	
				pstEntity->pstSrc->bEndingCompleteCnt = 0;
			}
		}
	}

	pstEntity->stListState.u32GetUsrTotal++;
    s32Ret = pstEntity->pfnAcqCallback(pstEntity->hSource,pstImage);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    bSupport = VPSS_IN_CheckImage_V3(pstImage);
    if (HI_TRUE != bSupport)
	{
		HI_DRV_VIDEO_PRIVATE_S *pstLastPriv;
		pstLastPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);

		if (pstLastPriv->u32LastFlag != DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
		{
			pstEntity->pfnRlsCallback(pstEntity->hSource,pstImage);
		}
		else if (pstLastPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG)
		{
			VPSS_SRC_S* pstSrcInfo;
			pstSrcInfo = (VPSS_SRC_S*)pstEntity->pstSrc;

			VPSS_SRC_Flush(pstSrcInfo,pstEntity->stStreamInfo.u32StreamProg);
		}
		else
		{
		
		}

		pstEntity->stListState.u32RelUsrTotal++;
		pstEntity->stListState.u32RelUsrSuccess++;
		return HI_FAILURE;
	}

	if( HI_TRUE == pstEntity->stDbginfo.frameEn )
	{
		pstImage->u32FrameRate = pstEntity->stDbginfo.framerate*1000;
	}

    if (0)
    {
        HI_U8 chFile[20] = "vpss_in.yuv";
		if (pstImage->u32FrameIndex == 0x10)
        VPSS_OSAL_WRITEYUV(pstImage, chFile);
    }

    if (0)
    {
        HI_DRV_VIDEO_FRAME_S *pstFrm;
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;

        pstFrm = pstImage;
        
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrm->u32Priv[0]);
        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstPriv->u32Reserve[0]);
        

        HI_PRINT("Image Info:Index %d Type %d Format %d W %d H %d Prog %d topfirst %d FieldMode %d PTS %d Rate %d LastFlag %#x Delta %d CodeType %d,SourceType %d,BitWidth %d\n"
                 "           L:Y %#x C %#x YH %#x CH %#x YS %d CS %d \n"
                 "           R:Y %#x C %#x YH %#x CH %#x YS %d CS %d \n",
                pstFrm->u32FrameIndex,
                pstFrm->eFrmType,
                pstFrm->ePixFormat,
                pstFrm->u32Width,
                pstFrm->u32Height,
                pstFrm->bProgressive,
				pstFrm->bTopFieldFirst,
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

	if(VPSS_TRANS_FB_NeedTrans(&(pstEntity->stTransFbInfo),pstImage))
	{
		HI_DRV_VIDEO_PRIVATE_S *pstPriv;

		pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstImage->u32Priv;

		pstPriv->bByPassVpss = HI_TRUE;
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
			VPSS_ERROR("\n-------%d------Trans Image failed,frameindex=%d", 
				pstEntity->hSource,pstImage->u32FrameIndex);
		}

		//return failuer,because vpss logic can't work
		return HI_FAILURE;
	}
	else
	{
		HI_DRV_VIDEO_PRIVATE_S *pstPriv;

		pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstImage->u32Priv;

		pstPriv->bByPassVpss = HI_FALSE;
	}

	pstEntity->stListState.u32GetUsrSuccess++;
	
    (HI_VOID)VPSS_IN_CorrectProgInfo(pstEntity, pstImage);

    (HI_VOID)VPSS_IN_ReviseImage(pstEntity, pstImage);
        
    (HI_VOID)VPSS_IN_ChangeInRate(pstEntity, pstImage);

	if (HI_DRV_FIELD_ALL != pstEntity->enProcessField
			&& pstImage->enFieldMode == HI_DRV_FIELD_ALL)
	{
		pstImage->bProgressive = HI_TRUE;
		pstImage->enFieldMode = pstEntity->enProcessField;	
	}

	if (HI_TRUE == pstEntity->stDbginfo.bDeiDisable)
	{
		pstImage->enFieldMode = HI_DRV_FIELD_ALL;
		pstImage->bProgressive = HI_TRUE;
	}

    (HI_VOID)VPSS_IN_UpdateStreamInfo(pstEntity, pstImage);

	pstEntity->stTransFbInfo.bNeedTrans = HI_FALSE;

    if (HI_TRUE == pstEntity->stStreamInfo.u32IsNewImage)
    {   
        #if 1
        VPSS_SRC_ATTR_S stSrcAttr;
        VPSS_WBC_ATTR_S stWbcAttr;
        VPSS_NR_ATTR_S stNrAttr;

        VPSS_SRC_S* pstSrcInfo;
        VPSS_WBC_S* pstWbcInfo;
        VPSS_STTWBC_S* psttWbc;
        VPSS_DIESTINFO_S* pstDieStInfo;
        VPSS_NRMADINFO_S* pstNrMadInfo;
        unsigned long flags;
        
        pstSrcInfo = (VPSS_SRC_S*)pstEntity->pstSrc;
        pstWbcInfo = (VPSS_WBC_S*)pstEntity->pstWbcInfo[0];
        psttWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[0];
        pstDieStInfo = (VPSS_DIESTINFO_S*)pstEntity->pstDieStInfo[0];
        pstNrMadInfo = (VPSS_NRMADINFO_S*)pstEntity->pstNrMadInfo[0];

		VPSS_IN_ClearAllAlgBuffer(pstEntity);

        VPSS_CHECK_NULL(pstSrcInfo);
        VPSS_CHECK_NULL(pstWbcInfo);
        VPSS_CHECK_NULL(psttWbc);
        VPSS_CHECK_NULL(pstDieStInfo);
        VPSS_CHECK_NULL(pstNrMadInfo);
        
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
        pstEntity->stOriInfo.enSource = pstPriv->stVideoOriginalInfo.enSource;
        pstEntity->stOriInfo.u32Width = pstPriv->stVideoOriginalInfo.u32Width;
        pstEntity->stOriInfo.u32Height = pstPriv->stVideoOriginalInfo.u32Height;
        pstEntity->stOriInfo.u32FrmRate = pstPriv->stVideoOriginalInfo.u32FrmRate;
        pstEntity->stOriInfo.bInterlace = (pstImage->bProgressive == HI_TRUE) ? HI_FALSE : HI_TRUE;
        pstEntity->stOriInfo.enColorSys = pstPriv->stVideoOriginalInfo.enColorSys;

        //if first image , take its fieldorder
        pstEntity->stStreamInfo.u32RealTopFirst = pstImage->bTopFieldFirst;            
                   
        stSrcAttr.hSrcModule = pstEntity->hSource;
        stSrcAttr.pfnRlsImage = (PFN_SRC_FUNC)pstEntity->pfnRlsCallback;
        
        stWbcAttr.enBitWidth = pstImage->enBitWidth;
        stWbcAttr.enRefMode = VPSS_WBC_REF_MODE_NULL;
        stWbcAttr.ePixFormat = pstImage->ePixFormat;
        	stWbcAttr.u32Height = pstImage->u32Height;
        	stWbcAttr.u32Width = pstImage->u32Width;
		stWbcAttr.u32FrameIndex = pstImage->u32FrameIndex;
		stWbcAttr.u32Pts = pstImage->u32Pts;
        stNrAttr.u32Height = pstImage->u32Height;
        stNrAttr.u32Width = pstImage->u32Width;
        
        stSrcAttr.enMode = VPSS_IN_GetSrcInitMode_V3(pstEntity);
        
        if (HI_TRUE == pstEntity->stStreamInfo.u32StreamProg)
        {
            stWbcAttr.enMode = VPSS_WBC_MODE_NORMAL;
            stNrAttr.enMode = NR_MODE_FRAME;
        }
        else
        {              
        //    stWbcAttr.enMode = VPSS_WBC_MODE_5FIELD;
        //    stNrAttr.enMode = NR_MODE_5FIELD;
			stWbcAttr.enMode = VPSS_WBC_MODE_4FIELD;//for mv410
            stNrAttr.enMode = NR_MODE_3FIELD; //for mv410
        }
        
        if(pstImage->u32Width > 1920)
        {
            VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
            s32InitListRet = VPSS_SRC_Init(pstSrcInfo, stSrcAttr);
            VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
            
            s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);            
        }
        else
        {
            if (HI_DRV_FT_NOT_STEREO == pstImage->eFrmType)
            {
				HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
    			HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
    			pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
    			pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
                VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
                s32InitListRet = VPSS_SRC_Init(pstSrcInfo, stSrcAttr);
                VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);

				if (pstImage->u32Width <= 720
						&& pstImage->u32Height <= 576)
				{
					s32InitListRet |= VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
					s32InitListRet |= VPSS_STTINFO_NrInit(pstNrMadInfo, &stNrAttr);
					pstEntity->bEnableTNR   = HI_TRUE;
				}
				else
				{
					pstEntity->bEnableTNR   = HI_FALSE;
				}
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420) 
                if (pstImage->bProgressive != HI_TRUE
                    && pstImage->enFieldMode != HI_DRV_FIELD_ALL
                    && pstImage->enBufValidMode == HI_DRV_FIELD_ALL
					&& HI_UNF_VCODEC_TYPE_HEVC == pstVdecPriv->entype)
                {
                    s32InitListRet |= VPSS_STTINFO_DieInit(pstDieStInfo, 
                        stWbcAttr.u32Width, 
                        stWbcAttr.u32Height*2);
                }
                else
#endif
                {
                    s32InitListRet |= VPSS_STTINFO_DieInit(pstDieStInfo, 
                        stWbcAttr.u32Width, 
                        stWbcAttr.u32Height);
                }
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);
            }
            else
            {
                VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
                s32InitListRet = VPSS_SRC_Init(pstSrcInfo, stSrcAttr);
                VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
                
                s32InitListRet |= VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
				pstWbcInfo = (VPSS_WBC_S*)pstEntity->pstWbcInfo[1];
                s32InitListRet |= VPSS_WBC_Init(pstWbcInfo, &stWbcAttr);
                s32InitListRet |= VPSS_STTINFO_DieInit(pstDieStInfo, stWbcAttr.u32Width, stWbcAttr.u32Height);
				pstDieStInfo = (VPSS_DIESTINFO_S*)pstEntity->pstDieStInfo[1];
                s32InitListRet |= VPSS_STTINFO_DieInit(pstDieStInfo, stWbcAttr.u32Width, stWbcAttr.u32Height);
                s32InitListRet |= VPSS_STTINFO_NrInit(pstNrMadInfo, &stNrAttr);
				pstNrMadInfo = (VPSS_NRMADINFO_S*)pstEntity->pstNrMadInfo[1];
                s32InitListRet |= VPSS_STTINFO_NrInit(pstNrMadInfo , &stNrAttr);
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc);
				psttWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[1];
                s32InitListRet |= VPSS_STTINFO_SttWbcInit(psttWbc );
            }
        }
        
        if(HI_SUCCESS != s32InitListRet)
        {
            s32Ret = HI_FAILURE;
            goto REFRESH_V3_OUT;
        }
        #endif
        s32Ret = VPSS_SRC_PutImage(pstSrcInfo, (VPSS_SRC_DATA_S*)pstImage);
		pstEntity->stListState.u32PutSrcCount = pstSrcInfo->u32PutSrcCount;
		pstEntity->bResetWBC = HI_FALSE;
    }
    else
    {
        VPSS_SRC_S* pstSrcInfo;
        
        pstSrcInfo = (VPSS_SRC_S*)pstEntity->pstSrc;
        
        VPSS_CHECK_NULL(pstSrcInfo);

        s32Ret = VPSS_SRC_PutImage(pstSrcInfo, (VPSS_SRC_DATA_S*)pstImage);
		pstEntity->stListState.u32PutSrcCount = pstSrcInfo->u32PutSrcCount;
    }
    
    if (pstEntity->bResetWBC == HI_TRUE)	{
        VPSS_IN_ResetWbc(pstEntity,pstImage);
        pstEntity->bResetWBC = HI_FALSE;
    }
    
REFRESH_V3_OUT:
    return s32Ret;
}

HI_S32 VPSS_IN_GetProcessImage_V3(VPSS_IN_ENTITY_S *pstEntity,HI_DRV_VIDEO_FRAME_S **ppstFrame)
{
    VPSS_SRC_S *pstSrc;
	VPSS_WBC_S *pstWbc;
    HI_S32 s32Ret = HI_SUCCESS;
    unsigned long flags;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V3_0);

    VPSS_CHECK_NULL(pstEntity->pstSrc);

    pstSrc = (VPSS_SRC_S *)pstEntity->pstSrc;

	if (pstSrc->bEnding != HI_TRUE)
	{
		VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
		s32Ret = VPSS_SRC_GetProcessImage(pstSrc,ppstFrame);
		VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
	}
	else
	{
		pstWbc = pstEntity->pstWbcInfo[0];

		if (pstEntity->bEnableTNR)
		{
			s32Ret = VPSS_WBC_GetCurInfo(pstWbc,ppstFrame);
			if (s32Ret == HI_SUCCESS)
			{
				(*ppstFrame)->bProgressive = HI_TRUE;
			}
		}
		else
		{
			VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
			s32Ret = VPSS_SRC_GetLastImageInfo(pstSrc,ppstFrame);
			VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
		}
	}
    
    return s32Ret;
}

HI_S32 VPSS_IN_GetInfo_V3(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_INFO_TYPE_E enType,
                                    HI_DRV_BUF_ADDR_E enLR, 
                                    HI_VOID* pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V3_0);

    switch(enType)
    {
        case VPSS_IN_INFO_WBC:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                HI_DRV_VIDEO_FRAME_S **ppstData;
                VPSS_WBC_S* pstWbc;
                
                ppstData = (HI_DRV_VIDEO_FRAME_S **)pstInfo;
                
                pstWbc = (VPSS_WBC_S*)pstEntity->pstWbcInfo[enLR];
                
                s32Ret = VPSS_WBC_GetWbcInfo(pstWbc, ppstData);
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
                HI_DRV_VIDEO_FRAME_S** ppstRef;
                VPSS_WBC_S* pstWbc;
				VPSS_SRC_S* pstSrc;

                ppstRef = (HI_DRV_VIDEO_FRAME_S **)pstInfo;
                if (pstEntity->bEnableTNR)
                {
                    pstWbc = (VPSS_WBC_S*)pstEntity->pstWbcInfo[enLR];

                    s32Ret = VPSS_WBC_GetRefInfo(pstWbc, ppstRef);
                }
                else
                {
					pstSrc = (VPSS_SRC_S*)pstEntity->pstSrc;

					s32Ret = VPSS_SRC_GetRefInfo(pstSrc,ppstRef);
                }
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_SRC:
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_WBCREG_VA:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                VPSS_STTWBC_S *pstStWbc;
                HI_U32 *pu32VirAddr;
                
                pstStWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[enLR];

                pu32VirAddr = (HI_U32 *)pstInfo;
                
                *pu32VirAddr = (HI_U32)pstStWbc->stMMZBuf.pu8StartVirAddr;

                s32Ret = HI_SUCCESS;
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_WBCREG_PA:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                VPSS_STTWBC_S *pstStWbc;
                HI_U32 *pu32VirAddr;
                
                pstStWbc = (VPSS_STTWBC_S*)pstEntity->pstSttWbc[enLR];

                pu32VirAddr = (HI_U32 *)pstInfo;
                
                *pu32VirAddr = pstStWbc->stMMZBuf.u32StartPhyAddr;

                s32Ret = HI_SUCCESS;
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

        case VPSS_IN_INFO_NR:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                VPSS_NRMADCFG_S *pstNrCfg;
                VPSS_NRMADINFO_S* pstNrMadInfo;
                
                pstNrCfg = (VPSS_NRMADCFG_S *)pstInfo;
                pstNrMadInfo = (VPSS_NRMADINFO_S*)pstEntity->pstNrMadInfo[enLR];
                
                s32Ret = VPSS_STTINFO_NrGetInfo(pstNrMadInfo, pstNrCfg);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_DIE:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                VPSS_DIESTINFO_S* pstDieStInfo;
                VPSS_DIESTCFG_S* pstDieStCfg;

                
                pstDieStCfg = (VPSS_DIESTCFG_S *)pstInfo;
                pstDieStInfo = (VPSS_DIESTINFO_S*)pstEntity->pstDieStInfo[enLR];
                
                s32Ret = VPSS_STTINFO_DieGetInfo(pstDieStInfo, pstDieStCfg);
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_CCREF:
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                VPSS_SRC_S *pstSrc;
                VPSS_SRC_DATA_S *pPtrPreData = HI_NULL;
                VPSS_SRC_DATA_S *pPtrPpreData = HI_NULL;
                VPSS_SRC_DATA_S **pstCCCl;
                pstSrc = (VPSS_SRC_S*)pstEntity->pstSrc;
                
                pstCCCl = (VPSS_SRC_DATA_S **)pstInfo;
                
                s32Ret = VPSS_SRC_GetPreImgInfo(pstSrc, &pPtrPreData, &pPtrPpreData);
                if (HI_SUCCESS == s32Ret)
                {
                    pstCCCl[0] = pPtrPreData;
                    pstCCCl[1] = pPtrPpreData;
                    s32Ret = HI_SUCCESS;
                }
                else
                {
                    s32Ret = HI_FAILURE;
                }
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_WBCMODE:
            if (HI_DRV_BUF_ADDR_LEFT == enLR 
                || HI_DRV_BUF_ADDR_RIGHT == enLR )
            {
                VPSS_WBC_S *pstWbcInfo;
                VPSS_WBC_MODE_E *penMode;
                
                pstWbcInfo = (VPSS_WBC_S*)pstEntity->pstWbcInfo[enLR];
                
                penMode = (VPSS_WBC_MODE_E *)pstInfo;
                
                *penMode = pstWbcInfo->stWbcAttr.enMode;
                
                s32Ret = HI_SUCCESS;
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
            break;
        case VPSS_IN_INFO_CORRECT_FIELD:
		#if 1
            if (HI_DRV_BUF_ADDR_MAX == enLR)
            {
                HI_BOOL bTopFirst;
                bTopFirst = *(HI_BOOL *)pstInfo;
                if(bTopFirst == pstEntity->stStreamInfo.u32RealTopFirst)
                {
                    s32Ret = HI_SUCCESS;
					return s32Ret;
                }
                 
                if(pstEntity->stStreamInfo.u32RealTopFirst == DEF_TOPFIRST_BUTT)
                {
                    pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
                    s32Ret = HI_SUCCESS;
					return s32Ret;
                }

                s32Ret = VPSS_SRC_CorrectListOrder(pstEntity->pstSrc, bTopFirst);

                pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
                
            }
            else
            {
                VPSS_ERROR("Invalid L/R Type %d\n",enLR);
                s32Ret = HI_FAILURE;
            }
		#else
			if (HI_DRV_BUF_ADDR_MAX == enLR)
			{
				HI_BOOL bTopFirst;
				HI_DRV_VIDEO_FRAME_S *pstCur;
				unsigned long flags; 
	
				bTopFirst = *(HI_BOOL *)pstInfo;
				if(bTopFirst == pstEntity->stStreamInfo.u32RealTopFirst)
				{
					s32Ret = HI_SUCCESS;
					return s32Ret;
				}
				 
				if(pstEntity->stStreamInfo.u32RealTopFirst == DEF_TOPFIRST_BUTT)
				{
					pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
					s32Ret = HI_SUCCESS;
					return s32Ret;
				}
			
				//s32Ret = VPSS_SRC_CorrectListOrder(pstEntity->pstSrc, bTopFirst);
				VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
				s32Ret = VPSS_SRC_GetProcessImage(pstEntity->pstSrc, &pstCur);
				if (HI_FAILURE == s32Ret)
				{
					VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
					VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
					return HI_FAILURE;
				}

				if(HI_FALSE == pstEntity->stStreamInfo.u32StreamProg)
				{   //the stream order changed,we drop one field
					if(pstCur->enFieldMode == pstEntity->stStreamInfo.enCurFieldMode)
					{
						s32Ret = VPSS_SRC_MoveNext(pstEntity->pstSrc, 1);
						s32Ret |= VPSS_SRC_GetProcessImage(pstEntity->pstSrc, &pstCur);
						if (HI_SUCCESS != s32Ret)
						{
							VPSS_INFO("VPSS_SRC_GetProcessImage failed!\n");
						}
					}
				}

				pstEntity->stStreamInfo.enCurFieldMode = pstCur->enFieldMode;
				VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
			
				pstEntity->stStreamInfo.u32RealTopFirst = bTopFirst;
				
			}
			else
			{
				VPSS_ERROR("Invalid L/R Type %d\n",enLR);
				s32Ret = HI_FAILURE;
			}
		#endif
            break;
        default:
            VPSS_ERROR("Invalid Info Type %d\n",enType);
            s32Ret = HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 VPSS_IN_SetInfo_V3 ( VPSS_IN_ENTITY_S *pstEntity,
                                    VPSS_SET_INFO_TYPE_E enType,
                                    HI_DRV_BUF_ADDR_E enLR,
                                    HI_VOID* pstInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    VPSS_CHECK_VERSION(pstEntity->enVersion,VPSS_VERSION_V3_0);
    
    switch(enType)
    {
    case VPSS_SET_INFO_INPUT_CFG:
		{		
			HI_S32 i;
			HI_U32 u32RefCount;    
			VPSS_HAL_INFO_S *pstHalInfo;	
			HI_DRV_VIDEO_FRAME_S *pstCur;
			HI_DRV_VIDEO_FRAME_S *pstRef[4] = {HI_NULL,HI_NULL,HI_NULL,HI_NULL};
			HI_DRV_VIDEO_FRAME_S *pstWbc;

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

			s32Ret = stInIntf.pfnGetProcessImage(pstEntity, &pstCur);
			if (HI_SUCCESS != s32Ret)
			{
				VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
				return HI_FAILURE;
			}
			switch(pstHalInfo->enNodeType)
			{
				case VPSS_HAL_NODE_2D_FRAME:
				case VPSS_HAL_NODE_2D_Field:
				case VPSS_HAL_NODE_3D_FRAME_R:
				if (pstEntity->bEnableTNR)
				{
					pstHalInfo->stNrInfo.bNrEn = HI_TRUE;
					stInIntf.pfnGetInfo( pstEntity,
											VPSS_IN_INFO_NR, 
											enLR,
											 (HI_VOID*)&(pstHalInfo->stNrInfo.stNrMadCfg));
				}
				else
				{
					pstHalInfo->stNrInfo.bNrEn = HI_FALSE;	
				}
					break;
				case VPSS_HAL_NODE_2D_5Field:
                case VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE:
                case VPSS_HAL_NODE_2D_H265_STEP2_DEI:
					pstHalInfo->stDieInfo.bBottom_first = pstEntity->stStreamInfo.u32RealTopFirst;
					stInIntf.pfnGetInfo(pstEntity,
											VPSS_IN_INFO_DIE, 
											enLR,
											 (HI_VOID*)&(pstHalInfo->stDieInfo.stDieStCfg));
			
				if (pstEntity->bEnableTNR)
				{
					pstHalInfo->stNrInfo.bNrEn = HI_TRUE;
					stInIntf.pfnGetInfo(pstEntity,
											VPSS_IN_INFO_NR, 
											enLR,
											(HI_VOID*)&(pstHalInfo->stNrInfo.stNrMadCfg));
				}
				else
				{
					pstHalInfo->stNrInfo.bNrEn = HI_FALSE;
				}
					break;
				case VPSS_HAL_NODE_UHD:
				case VPSS_HAL_NODE_UHD_SPLIT_L:
				case VPSS_HAL_NODE_UHD_SPLIT_R:
					/* Do Nothing */
					break;
				default:
					VPSS_FATAL("Node Type(%x) is Not Surport,\n", pstHalInfo->enNodeType);
					return HI_FAILURE;
			}

			if(VPSS_IN_CheckNeedNr(pstEntity))
			{
				if (pstEntity->bEnableTNR)
				{
					stInIntf.pfnGetInfo( pstEntity,
							VPSS_IN_INFO_WBC, 
							enLR,
							(HI_VOID*)&pstWbc);

					/*keep ref frame pts equal cur pts*/
					pstWbc->u32FrameIndex = pstCur->u32FrameIndex;
					pstWbc->u32Pts = pstCur->u32Pts;
					memcpy(pstWbc->u32Priv,pstCur->u32Priv,sizeof(HI_U32)*DEF_HI_DRV_FRAME_INFO_SIZE);
					VPSS_IN_CopyHalFrameInfo(pstWbc, &pstHalInfo->stInWbcInfo, enLR);
				}

				stInIntf.pfnGetInfo( pstEntity,
									VPSS_IN_INFO_REF, 
									enLR,
									(HI_VOID*)pstRef);
									
                if (pstEntity->bEnableTNR)
                {
					u32RefCount = pstEntity->pstWbcInfo[enLR]->stWbcAttr.enMode - 2;
                }
                else
                {
                    if (pstEntity->pstSrc->enMode == SRC_MODE_FRAME)
                    {
                        u32RefCount = 0;
                    }
                    else
                    {
                        u32RefCount = 2;
                    }
                }
        
				if(pstRef[u32RefCount] != HI_NULL)
				{
					VPSS_IN_CopyHalFrameInfo(pstRef[u32RefCount], &pstHalInfo->stInRefInfo[u32RefCount], enLR);

					for(i = (u32RefCount-1); i >= 0; i--)
					{   
						if(pstRef[i] != HI_NULL)
						{ 
							VPSS_IN_CopyHalFrameInfo(pstRef[i], &pstHalInfo->stInRefInfo[i], enLR);
						}
						else
						{   
							pstRef[i] = pstRef[i+1];
							VPSS_IN_CopyHalFrameInfo(pstRef[i], &pstHalInfo->stInRefInfo[i], enLR);
						}
					}
				}
				else
				{
					for(i = 0; i <= u32RefCount; i++)
					{
						VPSS_IN_CopyHalFrameInfo(pstCur, &pstHalInfo->stInRefInfo[i], enLR);
					}
				} 
			}

			pstHalInfo->bEnableTNR = pstEntity->bEnableTNR;
		}
        break;
    case VPSS_SET_INFO_PTS:
		{
			HI_DRV_VIDEO_FRAME_S *pstFrm ;
			HI_DRV_VIDEO_FRAME_S *pstRef[3] = {HI_NULL,HI_NULL,HI_NULL};    
			VPSS_IN_INTF_S stInIntf;
			s32Ret = VPSS_IN_GetIntf(pstEntity, &stInIntf);
			if (HI_SUCCESS != s32Ret)
			{
				VPSS_ERROR("Get Interface Failed\n");
				return HI_FAILURE;
			}

			pstFrm = (HI_DRV_VIDEO_FRAME_S *)pstInfo;
			/* keep pts equal src frame*/
			if(HI_FALSE == pstEntity->stStreamInfo.u32StreamProg)
			{
				s32Ret = stInIntf.pfnGetInfo( pstEntity,
											 VPSS_IN_INFO_REF, 
											 enLR,
											(HI_VOID*)pstRef);
						
				if (HI_FAILURE == s32Ret)
				{
					VPSS_FATAL("VPSS_WBC_GetWbcInfo failed!\n");
					return HI_FAILURE;
				}

				if(HI_NULL == pstRef[1])
				{
					pstFrm->u32FrameIndex = pstEntity->stStreamInfo.u32FrameIndex;
					pstFrm->u32Pts = pstEntity->stStreamInfo.u32Pts;
				}
				else
				{
					pstFrm->u32FrameIndex = pstRef[1]->u32FrameIndex;
					pstFrm->u32Pts = pstRef[1]->u32Pts;
					memcpy(pstFrm->u32Priv,pstRef[1]->u32Priv,sizeof(HI_U32)*DEF_HI_DRV_FRAME_INFO_SIZE);
				}
			}
		}
		break;
    default:
        VPSS_ERROR("Invalid set Info Type %d\n",enType);
        s32Ret = HI_FAILURE;
	}
			
	return s32Ret;
}



HI_S32 VPSS_IN_CompleteImage_V3(VPSS_IN_ENTITY_S *pstEntity)
{   
    VPSS_SRC_S* pstSrcInfo;
    VPSS_WBC_S* pstWbcInfo;
    VPSS_STTWBC_S* psttWbc;
    VPSS_DIESTINFO_S* pstDieStInfo;
    VPSS_NRMADINFO_S* pstNrMadInfo;

    VPSS_CHECK_NULL(pstEntity);
    VPSS_CHECK_NULL(pstEntity->pstSrc);
    VPSS_CHECK_NULL(pstEntity->pstWbcInfo);
    VPSS_CHECK_NULL(pstEntity->pstSttWbc);
    VPSS_CHECK_NULL(pstEntity->pstDieStInfo);
    VPSS_CHECK_NULL(pstEntity->pstNrMadInfo);


    pstSrcInfo = pstEntity->pstSrc;
    pstWbcInfo = pstEntity->pstWbcInfo[0];
    
    psttWbc = pstEntity->pstSttWbc[0];
    pstDieStInfo = pstEntity->pstDieStInfo[0];
    pstNrMadInfo = pstEntity->pstNrMadInfo[0];

    VPSS_SRC_CompleteImage(pstSrcInfo,HI_NULL);

	pstEntity->stListState.u32CompleteSrcCount = pstSrcInfo->u32CompleteSrcCount;
	pstEntity->stListState.u32ReleaseSrcCount = pstSrcInfo->u32ReleaseSrcCount;

    if(pstEntity->stStreamInfo.u32StreamW <= 1920)
	{
		if (pstEntity->stStreamInfo.u32StreamW <= 720
				&& pstEntity->stStreamInfo.u32StreamH <= 576)
		{
			VPSS_WBC_Complete(pstWbcInfo);
			VPSS_STTINFO_NrComplete(pstNrMadInfo);
		}
		VPSS_STTINFO_DieComplete(pstDieStInfo);
		VPSS_STTINFO_SttWbcComplete(psttWbc);

		if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
		{
			VPSS_WBC_Complete(pstEntity->pstWbcInfo[1]);
			VPSS_STTINFO_DieComplete(pstEntity->pstDieStInfo[1]);
			VPSS_STTINFO_NrComplete(pstEntity->pstNrMadInfo[1]);
			VPSS_STTINFO_SttWbcComplete(pstEntity->pstSttWbc[1]);
		}
	}
    else
    {
        VPSS_STTINFO_SttWbcComplete(psttWbc);
        VPSS_STTINFO_SttWbcComplete(pstEntity->pstSttWbc[1]);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_IN_Reset_V3(VPSS_IN_ENTITY_S *pstEntity)
{
    VPSS_SRC_S* pstSrcInfo;
    VPSS_WBC_S* pstWbcInfo;
    
    VPSS_STTWBC_S* psttWbc;
    VPSS_DIESTINFO_S* pstDieStInfo;
    VPSS_NRMADINFO_S* pstNrMadInfo;

    VPSS_CHECK_NULL(pstEntity);
    VPSS_CHECK_NULL(pstEntity->pstSrc);
    VPSS_CHECK_NULL(pstEntity->pstWbcInfo);
    VPSS_CHECK_NULL(pstEntity->pstSttWbc);
    VPSS_CHECK_NULL(pstEntity->pstDieStInfo);
    VPSS_CHECK_NULL(pstEntity->pstNrMadInfo);
    
    pstSrcInfo = pstEntity->pstSrc;
    pstWbcInfo = pstEntity->pstWbcInfo[0];
    psttWbc = pstEntity->pstSttWbc[0];
    pstDieStInfo = pstEntity->pstDieStInfo[0];
    pstNrMadInfo = pstEntity->pstNrMadInfo[0];

    /*reset image list*/
    //:TODO:在调用此接口时，VPSS正在处理数据，如何保证同步,加入锁策略
    VPSS_SRC_Reset(pstSrcInfo);
    
    if(pstEntity->enMode == VPSS_IN_MODE_USRACTIVE)
    {
        //:TODO:拉模式下上层是否会调用RESET
    }

    if(pstEntity->stStreamInfo.u32StreamW <= 1920)
    {
        VPSS_WBC_Reset(pstWbcInfo);
        VPSS_STTINFO_DieReset(pstDieStInfo);
        VPSS_STTINFO_NrReset(pstNrMadInfo);
        VPSS_STTINFO_SttWbcReset(psttWbc);
        
        if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
        {
            VPSS_WBC_Reset(pstEntity->pstWbcInfo[1]);
            VPSS_STTINFO_DieReset(pstEntity->pstDieStInfo[1]);
            VPSS_STTINFO_NrReset(pstEntity->pstNrMadInfo[1]);
            VPSS_STTINFO_SttWbcReset(pstEntity->pstSttWbc[1]);
        }
    }
    else
    {
        VPSS_STTINFO_SttWbcReset(psttWbc);
        VPSS_STTINFO_SttWbcReset(psttWbc+1);
    }

    #if 0
    if(pstEntity->enMode == VPSS_IN_MODE_USRACTIVE)
    {
        VPSS_SRCIN_DeInit(&pstEntity->stSrcIn);
    }
    #endif
    
    if(pstEntity->stStreamInfo.u32StreamW <= 1920)
    {
        VPSS_WBC_DeInit(pstWbcInfo);
        VPSS_STTINFO_DieDeInit(pstDieStInfo);
        VPSS_STTINFO_NrDeInit(pstNrMadInfo);
        VPSS_STTINFO_SttWbcDeInit(psttWbc);

        if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
        {
            VPSS_WBC_DeInit(pstEntity->pstWbcInfo[1]);
            VPSS_STTINFO_DieDeInit(pstEntity->pstDieStInfo[1]);
            VPSS_STTINFO_NrDeInit(pstEntity->pstNrMadInfo[1]);
            VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);
        }
    }
	else
	{
		VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[0]);	
		VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);	
	}

	memset(&(pstEntity->stStreamInfo),0,sizeof(VPSS_IN_STREAM_INFO_S));
	
	pstEntity->stStreamInfo.u32RealTopFirst = DEF_TOPFIRST_BUTT;

	memset(&(pstEntity->stListState),0,sizeof(VPSS_IN_IMAGELIST_STATE_S));

    
	return HI_SUCCESS;
}

HI_S32 VPSS_IN_GetIntf(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_INTF_S *pstIntf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);

    if (pstEntity->enVersion == VPSS_VERSION_V3_0)
    {
        pstIntf->pfnRefresh         = VPSS_IN_Refresh_V3;
        pstIntf->pfnCompleteImage   = VPSS_IN_CompleteImage_V3;
        pstIntf->pfnGetProcessImage = VPSS_IN_GetProcessImage_V3;
        pstIntf->pfnReset           = VPSS_IN_Reset_V3;
        pstIntf->pfnGetInfo         = VPSS_IN_GetInfo_V3;
		pstIntf->pfnSetInfo         = VPSS_IN_SetInfo_V3;
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
    
    if (stEnv.enVersion == VPSS_VERSION_V3_0)
    {   
        pstEntity->enVersion = VPSS_VERSION_V3_0;
       
        s32Ret = VPSS_IN_VMALLOC_V3(pstEntity);
        if (HI_SUCCESS != s32Ret)
        {
           goto INIT_ERROR;
        }
        
        VPSS_OSAL_InitSpin(&(pstEntity->stSrcSpin));
    }
    else
    {
        VPSS_ERROR("Version %d can't support\n",stEnv.enVersion);
        s32Ret = HI_FAILURE;
    }
    
    return s32Ret;


INIT_ERROR:
    if (stEnv.enVersion == VPSS_VERSION_V3_0)
    {
        (HI_VOID)VPSS_IN_VFREE_V3(pstEntity);
    }
    else
    {

    }
    return HI_FAILURE;
}

HI_S32 VPSS_IN_DeInit(VPSS_IN_ENTITY_S *pstEntity)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    if (pstEntity->enVersion == VPSS_VERSION_V3_0)
    {
        VPSS_SRC_S* pstSrcInfo;
        VPSS_WBC_S* pstWbcInfo;
        VPSS_STTWBC_S* psttWbc;
        VPSS_DIESTINFO_S* pstDieStInfo;
        VPSS_NRMADINFO_S* pstNrMadInfo;
        unsigned long flags;    
        
        VPSS_CHECK_NULL(pstEntity);
        VPSS_CHECK_NULL(pstEntity->pstSrc);
        VPSS_CHECK_NULL(pstEntity->pstWbcInfo[0]);
        VPSS_CHECK_NULL(pstEntity->pstSttWbc[0]);
        VPSS_CHECK_NULL(pstEntity->pstDieStInfo[0]);
        VPSS_CHECK_NULL(pstEntity->pstNrMadInfo[0]);

        pstSrcInfo = pstEntity->pstSrc;
        pstWbcInfo = pstEntity->pstWbcInfo[0];
        psttWbc = pstEntity->pstSttWbc[0];
        pstDieStInfo = pstEntity->pstDieStInfo[0];
        pstNrMadInfo = pstEntity->pstNrMadInfo[0];
        
        VPSS_OSAL_DownSpin(&(pstEntity->stSrcSpin),&flags);
        VPSS_SRC_DeInit(pstSrcInfo);
        VPSS_OSAL_UpSpin(&(pstEntity->stSrcSpin),&flags);
        
        // TODO: 推送模式
        if(pstEntity->enMode == VPSS_IN_MODE_USRACTIVE)
        {
            //VPSS_SRCIN_DeInit(&pstInstance->stSrcIn);
        }

		VPSS_IN_ClearAllAlgBuffer(pstEntity);
			
        if(pstEntity->stStreamInfo.u32StreamW <= 1920)
        {
			if (pstEntity->stStreamInfo.u32StreamW <= 720
					&& pstEntity->stStreamInfo.u32StreamH <= 576)
			{
				VPSS_WBC_DeInit(pstWbcInfo);
				VPSS_STTINFO_NrDeInit(pstNrMadInfo);
			}
            VPSS_STTINFO_DieDeInit(pstDieStInfo);
            VPSS_STTINFO_SttWbcDeInit(psttWbc);

            if(pstEntity->stStreamInfo.eStreamFrmType != HI_DRV_FT_NOT_STEREO)
            {
                VPSS_WBC_DeInit(pstEntity->pstWbcInfo[1]);
                VPSS_STTINFO_DieDeInit(pstEntity->pstDieStInfo[1]);
                VPSS_STTINFO_NrDeInit(pstEntity->pstNrMadInfo[1]);
                VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);
            }
        }
		else
		{
			VPSS_STTINFO_SttWbcDeInit(psttWbc);	
			VPSS_STTINFO_SttWbcDeInit(pstEntity->pstSttWbc[1]);  
		}
		
        s32Ret = VPSS_IN_VFREE_V3(pstEntity);
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
    pstEntity->bEnableTNR = stAttr.bEnableTNR;
	pstEntity->enProcessField = stAttr.enProcessField;
            /*
                enable -> disable
                do nothing
                disable -> enable
                reset wbc
                */
    if (pstEntity->bEnableTNR != stAttr.bEnableTNR)	{  
        if (stAttr.bEnableTNR == HI_TRUE)
        {
            pstEntity->bResetWBC = HI_TRUE;
        }
    }
    
    return s32Ret;
    
}

HI_S32 VPSS_IN_GetAttr(VPSS_IN_ENTITY_S *pstEntity,VPSS_IN_ATTR_S *pstAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    VPSS_CHECK_NULL(pstEntity);
    
    pstAttr->enProgInfo = pstEntity->enProgInfo;
    pstAttr->bProgRevise = pstEntity->bProgRevise;
    pstAttr->bAlwaysFlushSrc = pstEntity->bAlwaysFlushSrc;
    pstAttr->enSrcCS = pstEntity->enSrcCS;
    pstAttr->bEnableTNR = pstEntity->bEnableTNR;
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
    return s32Ret;
}

#ifdef __cplusplus
 #if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
