/*========================================================================
Open MAX   Component: Video Decoder
This module contains the class definition for openMAX encoder component.
file:	vdec_drv_ctx.c
author: liucan l00180788
date:	26, DEC, 2011.
==========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <sys/types.h>


#include "omx_venc_drv.h"
#include "OMX_Types.h"
#include "drv_venc_ioctl.h"
#include "hi_drv_struct.h"

#include "hi_mpi_mem.h"
#include "omx_dbg.h"
#include "omx_venc_buf_mng.h"

#define VENC_CHECK_POINT_NULL( pSt )\
    do{\
        if (0 == pSt)\
        {\
           DEBUG_PRINT_ERROR("\n %s() input null point!!\n", __func__);\
           return -1;\
         }\
    }while(0)

static OMX_S32 venc_check_static_attr( venc_channel_S* pChnAttr)
{
     VENC_CHECK_POINT_NULL(pChnAttr);
     HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs = &pChnAttr->chan_cfg.VencUnfAttr;
     
     if( (pVencUnfAttrs->enVencType != HI_UNF_VCODEC_TYPE_H264) && (pVencUnfAttrs->enVencType != HI_UNF_VCODEC_TYPE_HEVC) )
     {
       DEBUG_PRINT_ERROR("\n %s() ,NOT support Type(%d) VENC channel.\n", __func__,pVencUnfAttrs->enVencType);
       return -1;
     }
     
     if ((pVencUnfAttrs->u32Height < HI_VENC_MIN_HEIGTH)
        || (pVencUnfAttrs->u32Width < HI_VENC_MIN_WIDTH))
     {
        DEBUG_PRINT_ERROR("Picture Width(%u) or Heigth(%u) too small.\n", pVencUnfAttrs->u32Width, pVencUnfAttrs->u32Height);
        return -1;
     }
     
     if ((pVencUnfAttrs->u32Height > HI_VENC_MAX_HEIGTH) || (pVencUnfAttrs->u32Width > HI_VENC_MAX_WIDTH))
     {
        DEBUG_PRINT_ERROR("Picture Width(%u) or Heigth(%u) too large.\n", pVencUnfAttrs->u32Width, pVencUnfAttrs->u32Height);
        return -1;
     }

     if ((pVencUnfAttrs->u32Height % HI_VENC_PIC_SZIE_ALIGN) || (pVencUnfAttrs->u32Width % HI_VENC_PIC_SZIE_ALIGN))
     {
        DEBUG_PRINT_ERROR("Picture Width(%u) or Heigth(%u) invalid, should N*%d.\n", pVencUnfAttrs->u32Width, pVencUnfAttrs->u32Height,
                    HI_VENC_PIC_SZIE_ALIGN);
        return -1;
     }

     if (pVencUnfAttrs->u32RotationAngle != 0)
     {
        DEBUG_PRINT_ERROR("u32RotationAngle(%u) only support 0 now.\n", pVencUnfAttrs->u32RotationAngle);
        return -1;
     }

     if (!((0 == pVencUnfAttrs->bSlcSplitEn) || (1 == pVencUnfAttrs->bSlcSplitEn )))
     {
        DEBUG_PRINT_ERROR("bSlcSplitEn(%u) is invalid.\n", pVencUnfAttrs->bSlcSplitEn);
        return -1;
     }

     /*if ((pChnAttr->chan_cfg.SplitSize < HI_VENC_MIN_SPLIT_SIZE)
        || (pChnAttr->chan_cfg.SplitSize > HI_VENC_MAX_SPLIT_SIZE))
     {
        DEBUG_PRINT_ERROR("u32SplitSize(%u) is invalid.\n", pChnAttr->chan_cfg.SplitSize);
        return -1;
     }*/

     return 0;  
}

static OMX_S32 venc_check_active_attr(venc_channel_S* pChnAttr)
{
     VENC_CHECK_POINT_NULL(pChnAttr);
     HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs = &pChnAttr->chan_cfg.VencUnfAttr;
     if ((pVencUnfAttrs->u32TargetBitRate < HI_VENC_MIN_bps)
        || (pVencUnfAttrs->u32TargetBitRate  > HI_VENC_MAX_bps))
     {
        DEBUG_PRINT_ERROR("TargetBitRate(%u) is invalid.\n", pVencUnfAttrs->u32TargetBitRate);
        return -1;
     }
     
     if ((pVencUnfAttrs->u32InputFrmRate < HI_VENC_MIN_fps)
        || (pVencUnfAttrs->u32InputFrmRate  > HI_VENC_MAX_fps))
     {
        DEBUG_PRINT_ERROR("InputFrmRate(%u) is invalid.\n", pVencUnfAttrs->u32InputFrmRate );
        return -1;
     }

     if ((pVencUnfAttrs->u32TargetFrmRate  < HI_VENC_MIN_fps)
        || (pVencUnfAttrs->u32TargetFrmRate > pVencUnfAttrs->u32InputFrmRate))
     {
        DEBUG_PRINT_ERROR("TargetFrmRate(%u) is invalid, should 1~inputFrameRate.\n", pVencUnfAttrs->u32TargetFrmRate);
        return -1;
     }

     if (pVencUnfAttrs->u32Gop < HI_VENC_MIN_GOP)
     {
        DEBUG_PRINT_ERROR("Gop(%u) is invalid, should > %u.\n", pVencUnfAttrs->u32Gop, HI_VENC_MIN_GOP);
        return -1;
     }
     
     if (pVencUnfAttrs->u32MinQp > HI_VENC_MAX_QP)
     {
        DEBUG_PRINT_ERROR("MinQp(%u) is invalid, should < %u.\n", pVencUnfAttrs->u32MinQp, HI_VENC_MAX_QP);
        return -1;
     }
     if (pVencUnfAttrs->u32MaxQp > HI_VENC_MAX_QP)
     {
        DEBUG_PRINT_ERROR("MaxQp(%u) is invalid, should < %u.\n", pVencUnfAttrs->u32MaxQp, HI_VENC_MAX_QP);
        return -1;
     }
     
     if (pVencUnfAttrs->u32MinQp >= pVencUnfAttrs->u32MaxQp)
     {
        DEBUG_PRINT_ERROR("MinQp(%u) is invalid, should < MaxQp(%u).\n", pVencUnfAttrs->u32MinQp ,pVencUnfAttrs->u32MaxQp);
        return -1;
     }
     
     if (pVencUnfAttrs->u8Priority >= HI_VENC_MAX_PRIORITY )
     {
        DEBUG_PRINT_ERROR("u8Priority(%u) is invalid, should < u8MaxPriority(%u).\n", pVencUnfAttrs->u8Priority ,HI_VENC_MAX_PRIORITY);
        return -1;
     } 
     
     return 0;
}

static HI_VOID omx_H264e_PutTrailingBits(tBitStream *pBS)
{
    OMX_VENC_BsPutBits31(pBS, 1, 1);

    if (pBS->totalBits & 7)
    {
        OMX_VENC_BsPutBits31(pBS, 0, 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_U32 omx_H264e_makeSPS(HI_U8 *pSPSBuf, const OMXVenc_H264e_SPS_S *pSPS)
{
    HI_U32 code, TotalMb, profile_idc, level_idc,direct_8x8_interence_flag;
    int bits;

    tBitStream BS;

    OMX_VENC_BsOpenBitStream(&BS, (HI_U32 *)pSPSBuf);

    TotalMb = pSPS->FrameWidthInMb * pSPS->FrameHeightInMb;

    if (TotalMb <= 99)
    {
        level_idc = 10;
    }
    else if (TotalMb <= 396)
    {
        level_idc = 20;
    }
    else if (TotalMb <= 792)
    {
        level_idc = 21;
    }
    else if (TotalMb <= 1620)
    {
        level_idc = 30;
    }
    else if (TotalMb <= 3600)
    {
        level_idc = 31;
    }
    else if (TotalMb <= 5120)
    {
        level_idc = 32;
    }
    else if (TotalMb <= 8192)
    {
        level_idc = 41;
    }
    else
    {
        level_idc = 0;
    }

    if (TotalMb < 1620)
    {
        direct_8x8_interence_flag = 0;
    }
    else
    {
        direct_8x8_interence_flag = 1;
    }
	

    //profile_idc = 66;
    profile_idc = pSPS->ProfileIDC;

    OMX_VENC_BsPutBits32(&BS, 1, 32);

    OMX_VENC_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    OMX_VENC_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    OMX_VENC_BsPutBits31(&BS, 7, 5); // nal_unit_type

    OMX_VENC_BsPutBits31(&BS, profile_idc, 8);
    OMX_VENC_BsPutBits31(&BS, 0x00, 8);
    OMX_VENC_BsPutBits31(&BS, level_idc, 8);

    OMX_VENC_BsPutBits31(&BS, 1, 1); // ue, sps_id = 0

   if(100 == pSPS->ProfileIDC)   //just for high profile
   {
        OMX_VENC_BsPutBits31(&BS, 0x2, 3);
        OMX_VENC_BsPutBits31(&BS, 0xC, 4);
   }

    OMX_VENC_BsPutBits31(&BS, 1, 1); // ue, log2_max_frame_num_minus4 = 0

    OMX_VENC_BsPutBits31(&BS, 3, 3); // ue, pic_order_cnt_type = 2
    OMX_VENC_BsPutBits31(&BS, 2, 3); // ue, num_ref_frames = 1 (or 2)
    OMX_VENC_BsPutBits31(&BS, 0, 1); // u1, gaps_in_frame_num_value_allowed_flag

    ue_vlc(bits, code, (pSPS->FrameWidthInMb - 1));
    OMX_VENC_BsPutBits31(&BS, code, bits);
    ue_vlc(bits, code, (pSPS->FrameHeightInMb - 1));
    OMX_VENC_BsPutBits31(&BS, code, bits);

    OMX_VENC_BsPutBits31(&BS, 1, 1); // u1, frame_mbs_only_flag = 1 (or 0)

    if (0)                // !pSPS->FrameMbsOnlyFlag
    {
        OMX_VENC_BsPutBits31(&BS, 0, 1); // mb_adaptive_frame_field_flag = 0
        OMX_VENC_BsPutBits31(&BS, 1, 1); // direct_8x8_inference_flag
    }
    else
    {
        OMX_VENC_BsPutBits31(&BS, direct_8x8_interence_flag, 1); // direct_8x8_inference_flag
    }

    {
        int bFrameCropping = ((pSPS->FrameCropLeft | pSPS->FrameCropRight |
                               pSPS->FrameCropTop | pSPS->FrameCropBottom) != 0);

        OMX_VENC_BsPutBits31(&BS, bFrameCropping, 1);

        if (bFrameCropping)
        {
            ue_vlc(bits, code, pSPS->FrameCropLeft);
            OMX_VENC_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropRight);
            OMX_VENC_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropTop);
            OMX_VENC_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropBottom);
            OMX_VENC_BsPutBits31(&BS, code, bits);
        }
    }
    OMX_VENC_BsPutBits31(&BS, 0, 1); // vui_parameters_present_flag
    omx_H264e_PutTrailingBits(&BS);

	DEBUG_PRINT("VENC h264 profile_id:%d,level_id:%d,num_ref_frames: %d\n",profile_idc,level_idc,1);
    return (HI_U32)BS.totalBits;
}


static HI_U32 omx_H264e_MakePPS(HI_U8 *pPPSBuf, const OMXVenc_H264e_PPS_S *pPPS)
{
    HI_U32 code;
    int bits;
    HI_U32 b = pPPS->H264CabacEn ? 1 : 0;

    tBitStream BS;

#ifdef __VENC_S40V200_CONFIG__
    HI_U8 zz_scan_table[64] = 
    {
         0,  1,  8, 16,  9,  2,  3, 10,
        17, 24, 32, 25, 18, 11,  4,  5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13,  6,  7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };
#endif

    OMX_VENC_BsOpenBitStream(&BS, (HI_U32 *)pPPSBuf);

    OMX_VENC_BsPutBits32(&BS, 1, 32);

    OMX_VENC_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    OMX_VENC_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    OMX_VENC_BsPutBits31(&BS, 8, 5); // nal_unit_type

    OMX_VENC_BsPutBits31(&BS, 1, 1); // pps_id = 0
    OMX_VENC_BsPutBits31(&BS, 1, 1); // sps_id = 0

    OMX_VENC_BsPutBits31(&BS, b, 1); // entropy_coding_mode_flag = 0
    OMX_VENC_BsPutBits31(&BS, 0, 1); // pic_order_present_flag
    OMX_VENC_BsPutBits31(&BS, 1, 1); // num_slice_groups_minus1
    OMX_VENC_BsPutBits31(&BS, 1, 1); // num_ref_idx_l0_active_minus1 = 0 (or 1)
    OMX_VENC_BsPutBits31(&BS, 1, 1); // num_ref_idx_l1_active_minus1 = 0
    OMX_VENC_BsPutBits31(&BS, 0, 3); // weighted_pred_flag & weighted_bipred_idc
    OMX_VENC_BsPutBits31(&BS, 3, 2); // pic_init_qp_minus26 & pic_init_qs_minus26

    se_vlc(bits, code, pPPS->ChrQpOffset); // chroma_qp_index_offset
    OMX_VENC_BsPutBits31(&BS, code, bits);

    OMX_VENC_BsPutBits31(&BS, 1, 1);                // deblocking_filter_control_present_flag
    OMX_VENC_BsPutBits31(&BS, pPPS->ConstIntra, 1); // constrained_intra_pred_flag

    OMX_VENC_BsPutBits31(&BS, 0, 1);                // redundant_pic_cnt_present_flag

    if (pPPS->H264HpEn)
    {

      int i, j;
 #ifdef __VENC_S40V200_CONFIG__
      OMX_VENC_BsPutBits31(&BS, 1, 1); // transform_8x8_mode_flag
 #else
      OMX_VENC_BsPutBits31(&BS, 0, 1); // transform_8x8_mode_flag
 #endif
      OMX_VENC_BsPutBits31(&BS, 1, 1); // pic_scaling_matrix_present_flag
      
      for(i = 0; i < 6; i++)
      {
        OMX_VENC_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag
        
        se_vlc(bits, code, 8);  
		OMX_VENC_BsPutBits31(&BS, code, bits); /* all be 16 */
        for(j = 0; j < 15; j++)
		{
		    OMX_VENC_BsPutBits31(&BS, 1, 1);
        }
      }
      
#ifdef __VENC_S40V200_CONFIG__
      for(i = 0; i < 2; i++)
      {
        int lastScale, currScale, deltaScale;
		HI_S32 *pScale8x8 = &pPPS->Scale8x8;
        HI_S32 *pList = pScale8x8;
     
        if(i==1) pList = pScale8x8 + 64;
        
        OMX_VENC_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag
        
        for(lastScale = 8, j = 0; j < 64; j++)
        {
          currScale  = (int)(pList[zz_scan_table[j]]);
          deltaScale = currScale - lastScale;
          if     (deltaScale < -128) deltaScale += 256;
          else if(deltaScale >  127) deltaScale -= 256;
          se_vlc(bits, code, deltaScale);
          OMX_VENC_BsPutBits31(&BS, code, bits);
          lastScale = currScale;
        }
      }
#endif	  
      se_vlc(bits, code, pPPS->ChrQpOffset); 
	  OMX_VENC_BsPutBits31(&BS, code, bits);
    }

    omx_H264e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_S32 omx_venc_flushStrmHeader( venc_drv_context *drv_ctx )
{
    omx_sps_pps_data_S* pSpsPpsData = NULL;
	HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs     = NULL;
	OMXVenc_H264e_SPS_S *pSPS       = NULL;
	OMXVenc_H264e_PPS_S *pPPS       = NULL;
    HI_U8 i = 0;
	HI_U8 Quant8_intra_default[64] =
    {
     6,10,13,16,18,23,25,27,
    10,11,16,18,23,25,27,29,
    13,16,18,23,25,27,29,31,
    16,18,23,25,27,29,31,33,
    18,23,25,27,29,31,33,36,
    23,25,27,29,31,33,36,38,
    25,27,29,31,33,36,38,40,
    27,29,31,33,36,38,40,42
    };

    HI_U8 Quant8_inter_default[64] =
    {
     9,13,15,17,19,21,22,24,
    13,13,17,19,21,22,24,25,
    15,17,19,21,22,24,25,27,
    17,19,21,22,24,25,27,28,
    19,21,22,24,25,27,28,30,
    21,22,24,25,27,28,30,32,
    22,24,25,27,28,30,32,33,
    24,25,27,28,30,32,33,35
    };
	
	VENC_CHECK_POINT_NULL(drv_ctx);

   //check if venc has been init!
   if(drv_ctx->video_driver_fd < 0)
   {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
   }
   
   pSpsPpsData  = &drv_ctx->stStrmHeadData;
   pVencUnfAttrs = &drv_ctx->venc_chan_attr.chan_cfg.VencUnfAttr;
   pSPS         = &pSpsPpsData->stSPSData;
   pPPS         = &pSpsPpsData->stPPSData;
   if (pVencUnfAttrs->enVencType != HI_UNF_VCODEC_TYPE_H264)
   {
      DEBUG_PRINT_ERROR("ERROR: venc dose not support (%d),just support h264 !!\n",pVencUnfAttrs->enVencType);
      return -1;
   }
   
    /* make sps & pps & VOL stream */
	memset (&pSpsPpsData->stSPSData,0,sizeof(OMXVenc_H264e_SPS_S));
    memset (&pSpsPpsData->stPPSData,0,sizeof(OMXVenc_H264e_PPS_S));
    
    switch(pVencUnfAttrs->enVencProfile)
    {
       case HI_UNF_H264_PROFILE_BASELINE:
       	pSPS->ProfileIDC = 66;
       	break;
       case HI_UNF_H264_PROFILE_MAIN:
       	pSPS->ProfileIDC = 77;
       	break;
       case HI_UNF_H264_PROFILE_HIGH:
       	pSPS->ProfileIDC = 100;
       	break;
       default:
       	pSPS->ProfileIDC = 100;
       	break;
    }
    
    pSPS->FrameWidthInMb  = (pVencUnfAttrs->u32Width + 15) >> 4;
    pSPS->FrameHeightInMb = (pVencUnfAttrs->u32Height+ 15) >> 4;
    pSPS->FrameCropLeft = 0;
    pSPS->FrameCropTop    = 0;
    pSPS->FrameCropRight  = (pSPS->FrameWidthInMb  * 16 - pVencUnfAttrs->u32Width) >> 1;
    pSPS->FrameCropBottom = (pSPS->FrameHeightInMb * 16 - pVencUnfAttrs->u32Height) >> 1;
    
    pPPS->ChrQpOffset = 0;   //pEncPara->ChrQpOffset; as same as drv default
    pPPS->ConstIntra  = 0;   //pEncPara->ConstIntra;
    pPPS->H264HpEn    = (pVencUnfAttrs->enVencProfile == HI_UNF_H264_PROFILE_BASELINE)? 0 : 1;   //pEncPara->H264HpEn;
    pPPS->H264CabacEn = (pVencUnfAttrs->enVencProfile == HI_UNF_H264_PROFILE_BASELINE)? 0 : 1;

	for(i = 0; i < 64; i++)    pPPS->Scale8x8[i]      = Quant8_intra_default[i];
    for(i = 0; i < 64; i++)    pPPS->Scale8x8[i+64]   = Quant8_inter_default[i];
	    
    pSpsPpsData->SpsBits = omx_H264e_makeSPS(pSpsPpsData->SpsStream, pSPS);
    pSpsPpsData->PpsBits = omx_H264e_MakePPS(pSpsPpsData->PpsStream, pPPS);
	
    return HI_SUCCESS;
}

OMX_U32 channel_get_current_level(OMX_U32 EncWidth,OMX_U32 EncHeight)
{
    OMX_U32 TotalMb = 0;
	OMX_U32 WidthInMB = (EncWidth + 15) >> 4;
	OMX_U32 HeightInMB = (EncHeight + 15) >> 4;
	OMX_U32 level_idc;
	TotalMb = WidthInMB * HeightInMB;

    if (TotalMb <= 99)
    {
        level_idc = 10;
    }
    else if (TotalMb <= 396)
    {
        level_idc = 20;
    }
    else if (TotalMb <= 792)
    {
        level_idc = 21;
    }
    else if (TotalMb <= 1620)
    {
        level_idc = 30;
    }
    else if (TotalMb <= 3600)
    {
        level_idc = 31;
    }
    else if (TotalMb <= 5120)
    {
        level_idc = 32;
    }
    else if (TotalMb <= 8192)
    {
        level_idc = 41;
    }
    else
    {
        level_idc = 0;
    }
	return level_idc;
}

OMX_S32 channel_create(venc_drv_context *drv_ctx)
{
   VENC_INFO_CREATE_S stVencChnCreate;
   OMX_S32 s32Ret = 0;
   
   HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs;
   VENC_CHECK_POINT_NULL(drv_ctx);

   //check if venc has been init!
   if(drv_ctx->video_driver_fd < 0)
   {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
   }

   if( venc_check_static_attr( &(drv_ctx->venc_chan_attr) ) )
   {
      DEBUG_PRINT_ERROR("ERROR: venc channel attributes is not valid!!\n");
      return -1;
   }
   
   if( venc_check_active_attr( &(drv_ctx->venc_chan_attr) ) )
   {
      DEBUG_PRINT_ERROR("ERROR: venc channel attributes is not valid!!\n");
      return -1;
   }

   pVencUnfAttrs = &drv_ctx->venc_chan_attr.chan_cfg.VencUnfAttr;
   memcpy(&stVencChnCreate.stAttr.VencUnfAttr,pVencUnfAttrs,sizeof(HI_UNF_VENC_CHN_ATTR_S));

   if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 1280)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
      //stVencChnCreate.stAttr.u32StrmBufSize  = 1920 * 1080 * 3;//1920 * 1080 * 2;   
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 1920 * 1080 * 2;   
   }
   else if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 720)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_720P;
      //stVencChnCreate.stAttr.u32StrmBufSize  = 1920 * 1080 * 2;//1280 * 720 * 2;  
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 1280 * 720 * 2;  
   }
   else if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 352)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_D1;
      //stVencChnCreate.stAttr.u32StrmBufSize  = 1920 * 1080 * 2;//720 * 576 * 2; 
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 720 * 576 * 2; 
   }
   else
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_CIF;
      //stVencChnCreate.stAttr.u32StrmBufSize  = 1920 * 1080 * 2;//352 * 288 * 2; 
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 352 * 288 * 2; 
   }   

   stVencChnCreate.bOMXChn = HI_TRUE;
   stVencChnCreate.stAttr.bAutoRequestIFrm = drv_ctx->venc_chan_attr.chan_cfg.bAutoRequestIFrm;
   stVencChnCreate.stAttr.bPrependSpsPps = drv_ctx->venc_chan_attr.chan_cfg.bPrependSpsPps;
   stVencChnCreate.stAttr.bOmxChn = HI_TRUE;
   stVencChnCreate.stAttr.ControlRateType = drv_ctx->venc_chan_attr.chan_cfg.ControlRateType;
   s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_CREATE_CHN, &stVencChnCreate);
   if (0 != s32Ret)
   {
        DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
   }
   
    drv_ctx->venc_chan_attr.chan_handle = stVencChnCreate.hVencChn;
	drv_ctx->venc_chan_attr.state       = 2;                     
    DEBUG_PRINT("\n create channel %lu ok\n",drv_ctx->venc_chan_attr.chan_handle);
    return 0; 
}

OMX_S32 channel_destroy(venc_drv_context *drv_ctx)
{
    OMX_S32 s32Ret = 0;
    
    VENC_CHECK_POINT_NULL(drv_ctx);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_DESTROY_CHN, &(drv_ctx->venc_chan_attr.chan_handle));
    if (0 != s32Ret)
    {
        return s32Ret;
    }  

    drv_ctx->venc_chan_attr.chan_handle = HI_INVALID_HANDLE;
	drv_ctx->venc_chan_attr.state       = 0; 
    return s32Ret;
}


OMX_S32 channel_start(venc_drv_context *drv_ctx)
{
    OMX_S32 s32Ret = 0;
    VENC_CHECK_POINT_NULL(drv_ctx);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_START_RECV_PIC, &(drv_ctx->venc_chan_attr.chan_handle));
    if(s32Ret != 0)
    {
       DEBUG_PRINT_ERROR("\n~~~%s failed!! %ld\n",__func__,s32Ret);
    }
    else DEBUG_PRINT("\n~~~%s success!!\n",__func__);

	drv_ctx->venc_chan_attr.state       = 1; 
    return s32Ret;
}


OMX_S32 channel_stop(venc_drv_context *drv_ctx)
{
    OMX_S32 s32Ret = 0;
    VENC_CHECK_POINT_NULL(drv_ctx);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_STOP_RECV_PIC, &(drv_ctx->venc_chan_attr.chan_handle));
    drv_ctx->venc_chan_attr.state       = 2; 
    return s32Ret;

}

OMX_S32 channel_request_IFrame(venc_drv_context *drv_ctx)
{
    OMX_S32 s32Ret = 0;
    VENC_CHECK_POINT_NULL(drv_ctx);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_REQUEST_I_FRAME, &(drv_ctx->venc_chan_attr.chan_handle));
    if(s32Ret != 0)
    {
       DEBUG_PRINT_ERROR("\n~~~%s failed!! %ld\n",__func__,s32Ret);
    }
    else DEBUG_PRINT("\n~~~%s success!!\n",__func__);

    return s32Ret;
}


OMX_S32 channel_flush_port(venc_drv_context *drv_ctx,
		OMX_U32 flush_dir)
{
#if 0
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;
	OMX_S32 dir;

	if (!drv_ctx || (drv_ctx->chan_handle < 0))
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd = drv_ctx->video_driver_fd;
	msg.chan_num = drv_ctx->chan_handle;
	dir = flush_dir;
	msg.in = &dir;
	msg.out = NULL;

	return ioctl(vdec_fd, VDEC_IOCTL_FLUSH_PORT, (void *)&msg);
#endif
    VENC_INFO_FLUSH_PORT_S stFlushInfo;
    OMX_S32 s32Ret = 0;
    VENC_CHECK_POINT_NULL(drv_ctx);

    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }
    stFlushInfo.hVencChn     = drv_ctx->venc_chan_attr.chan_handle;
    stFlushInfo.u32PortIndex = flush_dir;
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_FLUSH_PORT, &stFlushInfo);
    return s32Ret;
}


OMX_S32 channel_get_msg(venc_drv_context *drv_ctx,venc_msginfo *msginfo)
{
#if 0
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || !msginfo || (drv_ctx->chan_handle < 0))
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd = drv_ctx->video_driver_fd;
	msg.chan_num = drv_ctx->chan_handle;
	msg.out = msginfo;

	return ioctl(vdec_fd, VDEC_IOCTL_CHAN_GET_MSG, (void *)&msg);
#endif

    OMX_S32 s32Ret = 0;
    VENC_INFO_GET_MSG_S stVencMessage;
    
    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(msginfo);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
       DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
       return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    stVencMessage.hVencChn = drv_ctx->venc_chan_attr.chan_handle;
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_GET_MSG, &stVencMessage);
    if (0 != s32Ret)
    {
        return s32Ret;
    } 
    //*msginfo = stVencMessage.msg_info_omx;
    memcpy(msginfo,&stVencMessage.msg_info_omx,sizeof(venc_msginfo));
    return s32Ret;

}


OMX_S32 channel_stop_msg(venc_drv_context *drv_ctx)
{
#if 0
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || drv_ctx->chan_handle < 0)
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd = drv_ctx->video_driver_fd;
	msg.chan_num = drv_ctx->chan_handle;

	return ioctl(vdec_fd, VDEC_IOCTL_CHAN_STOP_MSG, (void *)&msg);
#endif
    return 0;
}


OMX_U32 pts = 1;
OMX_U32 FrameCnt = 0;

OMX_S32 channel_queue_Image(venc_drv_context *drv_ctx,
	venc_user_buf *puser_buf)
{
#if 0
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || !puser_buf || drv_ctx->chan_handle < 0)
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd		= drv_ctx->video_driver_fd;
	msg.chan_num	= drv_ctx->chan_handle;
	msg.in		= puser_buf;
	msg.out	= NULL;

	return ioctl(vdec_fd, VDEC_IOCTL_EMPTY_INPUT_STREAM, (void *)&msg);
#endif
    
    OMX_S32 s32Ret = 0;
    VENC_INFO_QUEUE_FRAME_S stVencQueueFrame; 
    
    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    stVencQueueFrame.hVencChn = drv_ctx->venc_chan_attr.chan_handle;   
    memcpy(&(stVencQueueFrame.stVencFrame_OMX), puser_buf, sizeof(venc_user_buf));
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_QUEUE_FRAME, &stVencQueueFrame);   //LIMINQI
    return s32Ret;  

}

#if 0

OMX_S32 channel_dequeue_Image(venc_drv_context *drv_ctx,
	venc_user_buf *puser_buf)
{
#if 0
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || !puser_buf || drv_ctx->chan_handle < 0)
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd		= drv_ctx->video_driver_fd;
	msg.chan_num	= drv_ctx->chan_handle;
	msg.in		= puser_buf;
	msg.out	= NULL;

	return ioctl(vdec_fd, VDEC_IOCTL_EMPTY_INPUT_STREAM, (void *)&msg);
#endif

    OMX_S32 s32Ret = 0,i = 0;
    VENC_INFO_QUEUE_FRAME_S stVencQueueFrame; 
    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    stVencQueueFrame.hVencChn = drv_ctx->venc_chan_attr.chan_handle;

    
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_DEQUEUE_FRAME, &stVencQueueFrame);   //LIMINQI

	if (!s32Ret)
	{
    	memcpy(puser_buf, &(stVencQueueFrame.stVencFrame_OMX), sizeof(venc_user_buf));
	}

    return s32Ret;  
    
}
#endif

OMX_S32 channel_queue_stream(venc_drv_context *drv_ctx,venc_user_buf *puser_buf)
{
    OMX_S32 s32Ret = 0;
#if 1
    VENC_INFO_QUEUE_FRAME_S stVencQueueStream;

    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    stVencQueueStream.hVencChn      = drv_ctx->venc_chan_attr.chan_handle;
    memcpy(&(stVencQueueStream.stVencFrame_OMX), puser_buf ,sizeof(venc_user_buf));
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_QUEUE_STREAM, &stVencQueueStream);
#endif
    return s32Ret;
}



OMX_S32 channel_release_stream(venc_drv_context *drv_ctx,
	venc_user_buf *puser_buf)
{

    OMX_S32 s32Ret = 0;
    VENC_INFO_ACQUIRE_STREAM_S stVencAcquireStream;

    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    stVencAcquireStream.hVencChn = drv_ctx->venc_chan_attr.chan_handle;

    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_RELEASE_STREAM, &stVencAcquireStream);

    puser_buf->data_len    = 0;
    
    return s32Ret;
}


OMX_S32 channel_get_sps_pps(venc_drv_context *drv_ctx,  DRV_VIDEO_PPS_SPS_DATA *pParameterSet)
{

    OMX_S32 s32Ret = 0;
    omx_sps_pps_data_S *pSpsPpsData = NULL ;
    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(pParameterSet);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }

    if (omx_venc_flushStrmHeader(drv_ctx) != 0)
    {
       DEBUG_PRINT_ERROR("ERROR: omx_venc_flushStrmHeader failed !!\n");
       return -1;
    }

    pSpsPpsData = &drv_ctx->stStrmHeadData;
    memset(pParameterSet,0,sizeof(DRV_VIDEO_PPS_SPS_DATA));
    memcpy(pParameterSet->Data,pSpsPpsData->SpsStream,pSpsPpsData->SpsBits/8);
	memcpy(pParameterSet->Data + pSpsPpsData->SpsBits/8,pSpsPpsData->PpsStream,pSpsPpsData->PpsBits/8);

	pParameterSet->nSize = pSpsPpsData->SpsBits/8 + pSpsPpsData->PpsBits/8;
    DEBUG_PRINT("Get sps&pps success! nSize = %d\n",pParameterSet->nSize);
    return s32Ret;
}

#if 0
OMX_S32 channel_bind_buffer(venc_drv_context *drv_ctx,
	venc_user_buf *puser_buf)
{
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || drv_ctx->chan_handle < 0 || !puser_buf)
	{
		DEBUG_PRINT_ERROR("\n %s() invalid param.", __func__);
		return -1;
	}

	vdec_fd =  drv_ctx->video_driver_fd;
	msg.chan_num = drv_ctx->chan_handle;
	msg.in = puser_buf;
	msg.out = NULL;

	return ioctl(vdec_fd, VDEC_IOCTL_CHAN_BIND_BUFFER, (void *)&msg);
}


OMX_S32 channel_unbind_buffer(venc_drv_context *drv_ctx,
	venc_user_buf *puser_buf)
{
	struct vdec_ioctl_msg msg = {0, NULL, NULL};
	OMX_S32 vdec_fd = -1;

	if (!drv_ctx || !puser_buf || drv_ctx->chan_handle < 0)
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return -1;
	}

	vdec_fd =  drv_ctx->video_driver_fd;
	msg.chan_num = drv_ctx->chan_handle;
	msg.in = puser_buf;
	msg.out = NULL;

	return ioctl(vdec_fd, VDEC_IOCTL_CHAN_UNBIND_BUFFER, (void *)&msg);
}
#endif

void venc_deinit_drv_context(venc_drv_context  *drv_ctx)
{
	if (!drv_ctx)
	{
		DEBUG_PRINT_ERROR("\n %s() failed", __func__);
		return;
	}

    if (drv_ctx->video_driver_fd < 0)
    {
        return;
    }
	if (drv_ctx->venc_chan_attr.chan_handle != 0xffffffff)        //单通道
	{
		if ( channel_destroy(drv_ctx)!= 0)
		{
           DEBUG_PRINT_ERROR("%s()destroy channel Error!\n", __func__);
		}
	}

	close(drv_ctx->video_driver_fd);
	drv_ctx->video_driver_fd = -1;	
}


OMX_S32 venc_init_drv_context(venc_drv_context *drv_ctx)          //打开设备文件，清空设备通道句柄、索引
{
	if (!drv_ctx)
	{
		DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
		return -1;
	}

    if (drv_ctx->video_driver_fd > 0)
    {
       return 0;
    }
    
	drv_ctx->video_driver_fd = open (VENC_NAME, O_RDWR);
	if (drv_ctx->video_driver_fd < 0)
	{
		DEBUG_PRINT_ERROR("%s() open %s failed\n", __func__, VENC_NAME);
		return -1;
	}

    memset(&drv_ctx->venc_chan_attr,   0, sizeof(venc_channel_S));
    memset(&drv_ctx->venc_stream_addr, 0, sizeof(venc_stream_buf_S));
    
    drv_ctx->venc_chan_attr.chan_handle	        = HI_INVALID_HANDLE;
    drv_ctx->venc_stream_addr.pStrmBufVirAddr	= 0;

    return 0;
}

void venc_get_default_attr(venc_drv_context *drv_ctx)          //打开设备文件，清空设备通道句柄、索引
{
    HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs;
	if (!drv_ctx)
	{
		DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
		return;
	}

    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return;
    }

	pVencUnfAttrs = &drv_ctx->venc_chan_attr.chan_cfg.VencUnfAttr;
	pVencUnfAttrs->enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_720P;
	pVencUnfAttrs->enVencProfile      = HI_UNF_H264_PROFILE_MAIN;//HI_UNF_H264_PROFILE_BASELINE;
    pVencUnfAttrs->enVencType	    = HI_UNF_VCODEC_TYPE_H264;
    pVencUnfAttrs->u32Width         = 1280;
    pVencUnfAttrs->u32Height        = 720;
    pVencUnfAttrs->u32RotationAngle = 0;
    pVencUnfAttrs->u32StrmBufSize   = 1280*720*2;
    pVencUnfAttrs->bSlcSplitEn      = 0;
    //pVencUnfAttrs->u32SplitSize	= 1024;
    pVencUnfAttrs->u32Gop           = 100;
    pVencUnfAttrs->bQuickEncode     = 0;
	pVencUnfAttrs->u32DriftRateThr  = (-1);
    pVencUnfAttrs->u32TargetBitRate = 3*1024*1024;
    pVencUnfAttrs->u32TargetFrmRate = 30;
    pVencUnfAttrs->u32InputFrmRate  = 30;
    pVencUnfAttrs->u32MinQp         = 16;
    pVencUnfAttrs->u32MaxQp         = 48;
    pVencUnfAttrs->u8Priority       = 0;    

    drv_ctx->venc_chan_attr.chan_cfg.bOmxChn                = HI_TRUE;
	drv_ctx->venc_chan_attr.chan_cfg.h264Level              = 31;
	drv_ctx->venc_chan_attr.chan_cfg.ControlRateType        = 2;          //OMX_Video_ControlRateConstant
    drv_ctx->venc_chan_attr.chan_cfg.bAutoRequestIFrm       = HI_FALSE;
	drv_ctx->venc_chan_attr.chan_cfg.bPrependSpsPps         = HI_FALSE;
	drv_ctx->venc_chan_attr.state                           = 0;
    return;
}

//========================
OMX_S32 channel_set_attr(venc_drv_context *drv_ctx)
{
   VENC_INFO_CREATE_S stVencChnCreate;
   OMX_S32 s32Ret = 0;
   HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs;
   VENC_CHECK_POINT_NULL(drv_ctx);

   //check if venc has been init!
   if(drv_ctx->video_driver_fd < 0)
   {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
   }

   pVencUnfAttrs = &drv_ctx->venc_chan_attr.chan_cfg.VencUnfAttr;
   if( venc_check_static_attr( &(drv_ctx->venc_chan_attr) ) )
   {
      //DEBUG_PRINT_ERROR("ERROR: venc channel attributes is not valid!!\n");
      s32Ret = -1;
      goto error;
   }
   
   if( venc_check_active_attr( &(drv_ctx->venc_chan_attr) ) )
   {
      //DEBUG_PRINT_ERROR("ERROR: venc channel attributes is not valid!!\n");
      s32Ret = -1;
      goto error;
   }

   stVencChnCreate.hVencChn        = drv_ctx->venc_chan_attr.chan_handle;
   
   memcpy(&stVencChnCreate.stAttr.VencUnfAttr,pVencUnfAttrs,sizeof(HI_UNF_VENC_CHN_ATTR_S));

   stVencChnCreate.stAttr.VencUnfAttr.bQuickEncode     = HI_FALSE;
   stVencChnCreate.stAttr.VencUnfAttr.u32DriftRateThr  = (-1);
   stVencChnCreate.stAttr.VencUnfAttr.u8Priority       = 0;

   if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 1280)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 1920 * 1080 * 2;   
   }
   else if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 720)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_720P;
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 1280 * 720 * 2;  
   }
   else if(stVencChnCreate.stAttr.VencUnfAttr.u32Width > 352)
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_D1;
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 720 * 576 * 2; 
   }
   else
   {
      stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_CIF;
      stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  = 352 * 288 * 2; 
   } 
   
   s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_SET_CHN_ATTR, &stVencChnCreate);
error:   
   if (HI_SUCCESS != s32Ret)
   {
       s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_GET_CHN_ATTR, &stVencChnCreate);
	   pVencUnfAttrs->u32Width   = stVencChnCreate.stAttr.VencUnfAttr.u32Width        ;
	   pVencUnfAttrs->u32Height   = stVencChnCreate.stAttr.VencUnfAttr.u32Height       ;
	   pVencUnfAttrs->enVencType      = (HI_U32)stVencChnCreate.stAttr.VencUnfAttr.enVencType      ;
	   pVencUnfAttrs->bSlcSplitEn    = (HI_U32)stVencChnCreate.stAttr.VencUnfAttr.bSlcSplitEn     ;
	   //pVencUnfAttrs->u32SplitSize     = stVencChnCreate.stAttr.VencUnfAttr.u32SplitSize   ;
	   pVencUnfAttrs->u32Gop           = stVencChnCreate.stAttr.VencUnfAttr.u32Gop          ;
	   pVencUnfAttrs->u32RotationAngle = stVencChnCreate.stAttr.VencUnfAttr.u32RotationAngle;
	   pVencUnfAttrs->u32InputFrmRate  = stVencChnCreate.stAttr.VencUnfAttr.u32InputFrmRate ;
	   pVencUnfAttrs->u32TargetFrmRate = stVencChnCreate.stAttr.VencUnfAttr.u32TargetFrmRate;
	   pVencUnfAttrs->u32TargetBitRate = stVencChnCreate.stAttr.VencUnfAttr.u32TargetBitRate;
	   pVencUnfAttrs->u32MaxQp         = stVencChnCreate.stAttr.VencUnfAttr.u32MaxQp        ;
	   pVencUnfAttrs->u32MinQp         = stVencChnCreate.stAttr.VencUnfAttr.u32MinQp        ;
	   pVencUnfAttrs->u8Priority     = (HI_U32)stVencChnCreate.stAttr.VencUnfAttr.u8Priority      ;
	   pVencUnfAttrs->u32StrmBufSize = stVencChnCreate.stAttr.VencUnfAttr.u32StrmBufSize  ;
	   pVencUnfAttrs->bQuickEncode   = (HI_U16)stVencChnCreate.stAttr.VencUnfAttr.bQuickEncode    ;
	   pVencUnfAttrs->u32DriftRateThr=  stVencChnCreate.stAttr.VencUnfAttr.u32DriftRateThr;
	   pVencUnfAttrs->enCapLevel      = (HI_U32)stVencChnCreate.stAttr.VencUnfAttr.enCapLevel      ;
	   pVencUnfAttrs->enVencProfile   = stVencChnCreate.stAttr.VencUnfAttr.enVencProfile;
	   drv_ctx->venc_chan_attr.chan_cfg.h264Level = channel_get_current_level(pVencUnfAttrs->u32Width,pVencUnfAttrs->u32Height);
	   
   }
   return s32Ret;
}

OMX_S32 channel_get_attr(venc_drv_context *drv_ctx)
{
   VENC_INFO_CREATE_S stVencChnCreate;
   OMX_S32 s32Ret = 0;
   HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs;
   
   VENC_CHECK_POINT_NULL(drv_ctx);

   //check if venc has been init!
   if(drv_ctx->video_driver_fd < 0)
   {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
   }
   stVencChnCreate.hVencChn = drv_ctx->venc_chan_attr.chan_handle;

   s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_GET_CHN_ATTR, &stVencChnCreate);

   if (HI_SUCCESS != s32Ret)
   {
        return s32Ret;
   }

   pVencUnfAttrs =&drv_ctx->venc_chan_attr.chan_cfg.VencUnfAttr;
   memcpy(pVencUnfAttrs, &stVencChnCreate.stAttr.VencUnfAttr,sizeof(HI_UNF_VENC_CHN_ATTR_S));
   
   drv_ctx->venc_chan_attr.chan_cfg.h264Level = channel_get_current_level(pVencUnfAttrs->u32Width,pVencUnfAttrs->u32Height);
   return s32Ret;
}
//=======================


#if 1
OMX_S32 venc_mmap_kerel_vir(venc_drv_context *drv_ctx,venc_user_buf *puser_buf)
{
    OMX_S32 s32Ret = 0;
    VENC_INFO_VENC_MMZ_PHY_S MmzPhyBuf;

    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    MmzPhyBuf.hVencChn         = drv_ctx->venc_chan_attr.chan_handle;
    MmzPhyBuf.phyaddr= puser_buf->bufferaddr_Phy;
    MmzPhyBuf.bufsize = puser_buf->buffer_size;

    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_MMZ_MAP, &MmzPhyBuf);
    if (s32Ret != 0 )
    {
       DEBUG_PRINT_ERROR("%s,%d,MMZ map kernel viraddr failed!!\n",__func__,__LINE__);
       return  -1;
    }
    puser_buf->vir2phy_offset = MmzPhyBuf.vir2phy_offset;
    
    /*DEBUG_PRINT("%s,%d,MMZ phyaddr = 0x%x,userVir = 0x%x,hernelvir = 0x%x,offset = 0x%x\n",
            __func__,__LINE__,stVencBuf.stVencBuf.phyaddr,stVencBuf.stVencBuf.user_viraddr,
            stVencBuf.stVencBuf.kernel_viraddr,puser_buf->vir2phy_offset);*/
    
    return s32Ret;
}

OMX_S32 venc_unmmap_kerel_vir(venc_drv_context *drv_ctx,venc_user_buf *puser_buf)
{
    OMX_S32 s32Ret = 0;
    VENC_INFO_VENC_MMZ_PHY_S MmzPhyBuf;

    VENC_CHECK_POINT_NULL(drv_ctx);
    VENC_CHECK_POINT_NULL(puser_buf);

    //check if venc has been init!
    if(drv_ctx->video_driver_fd < 0)
    {
      DEBUG_PRINT_ERROR("ERROR: venc dose not init !!\n");
      return -1;
    }
    
    if (HI_INVALID_HANDLE == drv_ctx->venc_chan_attr.chan_handle)
    {
        DEBUG_PRINT_ERROR("para hVencChn is invalid.\n");
        return -1;
    }

    MmzPhyBuf.hVencChn       = drv_ctx->venc_chan_attr.chan_handle;
    MmzPhyBuf.phyaddr        = puser_buf->bufferaddr_Phy;
    MmzPhyBuf.bufsize        = puser_buf->buffer_size;
  
    s32Ret = ioctl(drv_ctx->video_driver_fd, CMD_VENC_MMZ_UMMAP, &MmzPhyBuf);
    if (s32Ret != 0 )
    {
       DEBUG_PRINT_ERROR("%s,%d,MMZ unmap kernel viraddr failed!!\n",__func__,__LINE__);
       return  -1;
    }
    return s32Ret;
}


#endif

