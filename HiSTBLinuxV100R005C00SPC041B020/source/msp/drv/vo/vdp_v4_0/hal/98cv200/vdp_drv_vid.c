#include "hi_type.h"
#include "vdp_drv_comm.h"
#include "vdp_drv_vid.h"
#include "vdp_hal_hdr.h"
#include "vdp_hal_mmu.h"
#include "vdp_drv_pq_csc.h"
#include "vdp_drv_pq_zme.h"
#include "vdp_reg_define.h"
#include "vdp_hal_comm.h"

#include "vdp_drv_bt2020.h"

#if VDP_CBB_FPGA
#include <linux/string.h>
#include <linux/kernel.h>

#else
#include <string.h>

#endif

extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;
extern  S_VDP_REGS_TYPE *pVdpReg;
static HI_S32 GetVerticalScalerOffset(HI_U32 u32VRatio, VDP_VZME_DRV_PARA_S* pstZmeDrvPara);


HI_VOID VDP_DRV_SetLayerBkgColorAndAlpha(HI_U32 u32LayerId,
        VDP_BKG_S *pstMuteColor,
        VDP_CLR_SPACE_E    enOutClrSpace)
{
    VDP_BKG_S  stLetterBoxColor = {0};
    HI_U32  u32V0Enable = 0, u32V1Enable = 0;

    VDP_VID_SetLayerGalpha             (u32LayerId, 0xff);
    VDP_VID_SetMuteBkg                 (u32LayerId, *pstMuteColor);

    u32V0Enable = VDP_VID_GetVidLayerEnable       (VDP_LAYER_VID0);
    u32V1Enable = VDP_VID_GetVidLayerEnable       (VDP_LAYER_VID1);


    stLetterBoxColor.u32BkgA  = 0xff;

    if (u32LayerId == VDP_LAYER_VID0 || u32LayerId == VDP_LAYER_VID1)
    {
        if (enOutClrSpace == VDP_CLR_SPACE_RGB_2020
                || enOutClrSpace == VDP_CLR_SPACE_RGB_709)
        {
            if ((u32V0Enable == 0x80000000) && (u32V1Enable == 0x80000000))
            {
                stLetterBoxColor.u32BkgY  = 0x40;
                stLetterBoxColor.u32BkgU  = 0x200;
                stLetterBoxColor.u32BkgV  = 0x200;
                VDP_VID_SetLayerBkg(VDP_LAYER_VID0, stLetterBoxColor);
                stLetterBoxColor.u32BkgY  = 0x0;
                stLetterBoxColor.u32BkgU  = 0x0;
                stLetterBoxColor.u32BkgV  = 0x0;
                VDP_VID_SetLayerBkg(VDP_LAYER_VID1, stLetterBoxColor);
            }
            else
            {
                stLetterBoxColor.u32BkgY  = 0x0;
                stLetterBoxColor.u32BkgU  = 0x0;
                stLetterBoxColor.u32BkgV  = 0x0;

                VDP_VID_SetLayerBkg(u32LayerId, stLetterBoxColor);
            }
        }
        else
        {
            stLetterBoxColor.u32BkgY  = 0x40;
            stLetterBoxColor.u32BkgU  = 0x200;
            stLetterBoxColor.u32BkgV  = 0x200;
            VDP_VID_SetLayerBkg(u32LayerId, stLetterBoxColor);

        }
    }
    else
    {
        stLetterBoxColor.u32BkgY  = 0x40;
        stLetterBoxColor.u32BkgU  = 0x200;
        stLetterBoxColor.u32BkgV  = 0x200;
        VDP_VID_SetLayerBkg(u32LayerId, stLetterBoxColor);
    }

    return;
}

HI_S32 VDP_DRV_SetVidLayer(HI_U32 u32LayerId, VDP_LAYER_VID_INFO_S *pstInfo)
{
    HI_U32 u32ChmRatio = 1;//1:422/444; 2:420
    HI_U32 u32HgtC = 0;
    static VDP_DRV_ADDR_INFO_S stAddrInfo;
    static VDP_DRV_CROP_INFO_S stCropInfo;
    static VDP_DRV_ZME_INFO_S  stZmeInfo;
    U_VP0_IRESO         VP0_IRESO;
    VDP_RECT_S          stVid1Reso = {0};

    //-------for ec csc---------//
    HI_U32 u32Pre   =0;
    HI_U32 u32DcPre  =0;
    HI_U32 vcvm_csc_en =1;
    HI_U32  u32Scale=0;
    HI_S16 m33Yuv2RgbScale2P=0;
    HI_S16 v_imap_lms2ipt[3][3]={{0,0,0}};
    HI_S32 s32CscDc[3]={0};
    HI_S16 csc_m3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    //-------end for ec csc---------//


    if((u32LayerId == VDP_LAYER_VID0) || (u32LayerId == VDP_LAYER_VID1) )
    {
        //--------add by hyx for ec csc-----------------------//
        VDP_HDR_SetVDmImapRgb2lms(csc_m3);
        VDP_HDR_SetVDmImapRgb2lmsscale2p(0);
        VDP_HDR_SetVDmRgb2lmsEn(1);

         u32Pre = 1 << 10;
         u32DcPre = 4 * 32 ;//1:8bit; 4:10bit

        m33Yuv2RgbScale2P     =  15;     //sGetRandEx(5,31);

        u32Scale    = 1 << (m33Yuv2RgbScale2P - 10);
        u32DcPre    = 4 * 64;


        v_imap_lms2ipt[0][0]      =  (HI_S32)(1  * u32Pre*u32Scale/2);
        v_imap_lms2ipt[0][1]      =  (HI_S32)(0  * u32Pre*u32Scale/2);
        v_imap_lms2ipt[0][2]      =  (HI_S32)(0  * u32Pre*u32Scale/2);
        v_imap_lms2ipt[1][0]      =  (HI_S32)(0 * u32Pre*u32Scale/2);
        v_imap_lms2ipt[1][1]      =  (HI_S32)(1 * u32Pre*u32Scale/2);
        v_imap_lms2ipt[1][2]      =  (HI_S32)(0 * u32Pre*u32Scale/2);
        v_imap_lms2ipt[2][0]      =  (HI_S32)(0 * u32Pre*u32Scale/2);
        v_imap_lms2ipt[2][1]      =  (HI_S32)(0 * u32Pre*u32Scale/2);
        v_imap_lms2ipt[2][2]      =  (HI_S32)(1 * u32Pre*u32Scale/2);
        s32CscDc[0] =  0;
        s32CscDc[1] =  0;
        s32CscDc[2] =  0;

        VDP_HDR_SetCvmSel   (0);
        VDP_HDR_SetVCvmLms2iptEn        (vcvm_csc_en);

        VDP_HDR_SetVCvmImapLms2iptscale2p(m33Yuv2RgbScale2P | (vcvm_csc_en<<5) );

        VDP_HDR_SetVCvmImapLms2ipt       (v_imap_lms2ipt);
        VDP_HDR_SetVCvmImapLms2iptOutDc  (s32CscDc );
        VDP_HDR_SetVDmImapLms2iptmin     (0);
        VDP_HDR_SetVDmImapLms2iptmax     (0x7FFF);

        //-------------end----------------------//
    }

    if (u32LayerId != VDP_LAYER_VID1)
    {
        VDP_VID_SetMuteEnable              (u32LayerId, pstInfo->bMuteEn);
    }


    VDP_DRV_SetLayerBkgColorAndAlpha(u32LayerId, &pstInfo->stMuteColor, pstInfo->enOutClrSpace);
#if 0
    //------------clear hdr cfg -------------------------------------//
    VDP_HDR_SetHdrEnable            (HI_FALSE);
    VDP_HDR_SetVCvmDenormEn         (HI_FALSE);
    VDP_HDR_SetVDmNormEn            (HI_FALSE);
    VDP_HDR_SetVDmInColor           (1);
    VDP_HDR_SetVDmEn                (1);
    VDP_HDR_SetVDmGammaEn           (0);
    VDP_HDR_SetVDmDegammaEn         (0);
    VDP_HDR_SetVDmGammaV0En         (0);
    VDP_HDR_SetVDmGammaV1En         (0);
    VDP_HDR_SetDmHdrDsEn            (0);
    VDP_HDR_SetDmHdrMdEn            ( 0 );
    VDP_HDR_SetGDmEn                ( 0 );
    VDP_HDR_SetGDmDegammaEn         ( 0 );
    VDP_HDR_SetGDmRgb2lmsEn         ( 0 );
    VDP_HDR_SetGDmGammaEn           ( 0 );

    VDP_HDR_SetCvmSel               (0);
    VDP_HDR_SetVCvmLms2iptEn        (1)  ;
    VDP_HDR_SetVCvmEn               (0 ) ;
    VDP_HDR_SetVDmInBits            (1) ;
#endif

    /* clear hdr's zorder layer. */
    VDP_CBM_Clear_MixvPrio(u32LayerId);
    //------------clear hdr cfg end---------------------------------//

    if (((u32LayerId == VDP_LAYER_VID0) || (u32LayerId == VDP_LAYER_VID3))
        && (pstInfo->bRegionEn == HI_FALSE))
    {
        VDP_VID_SetLayerEnable (u32LayerId, HI_FALSE);
        VDP_HDR_SetRegUp();
        VDP_VID_SetRegUp(u32LayerId);
        return HI_SUCCESS;
    }
    else if ((u32LayerId == VDP_LAYER_VID1) && (pstInfo->bRegionEn == HI_FALSE))
    {
        VDP_VID_SetRegionEnable(u32LayerId, pstInfo->u32RegionNO, pstInfo->bRegionEn);

        if (HI_FALSE == VDP_VID_CheckRegionState(u32LayerId))
        {
            VDP_VID_SetLayerEnable (u32LayerId, HI_FALSE);
            VDP_VID_SetAllRegionDisable(u32LayerId);
            VDP_VID_SetRegUp(u32LayerId);
            VDP_HDR_SetRegUp();
            return HI_SUCCESS;
        }
        else
        {
            VDP_VID_SetLayerEnable (u32LayerId, HI_TRUE);
            if (pstInfo->bRegionEn == HI_FALSE)
            {
                VDP_VID_SetRegUp(u32LayerId);
                return HI_SUCCESS;
            }
        }
    }

    VDP_VID_SetLayerEnable (u32LayerId, HI_TRUE);
    if(pstInfo->stSrcInfo.enInFmt == VDP_VID_IFMT_SP_420
    || pstInfo->stSrcInfo.enInFmt == VDP_VID_IFMT_SP_TILE
    || pstInfo->stSrcInfo.enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
        u32HgtC = pstInfo->stCropRect.u32Hgt/2;
        u32ChmRatio = 2;
    }
    else
    {
        u32HgtC = pstInfo->stCropRect.u32Hgt;
        u32ChmRatio = 1;
    }


    //----------------------------------------------------------------------
    // vdp hal setting
    //----------------------------------------------------------------------
    if ((u32LayerId == VDP_LAYER_VID0) || (u32LayerId == VDP_LAYER_VID1))
    {
        VDP_CBM_SetMixerPrio(VDP_CBM_MIXV0, u32LayerId,pstInfo->u32LayerZorder);
    }
    else if (u32LayerId == VDP_LAYER_VID3)
    {
        VDP_CBM_SetMixerPrio(VDP_CBM_MIXV1, u32LayerId,pstInfo->u32LayerZorder);
    }

    VDP_VID_SetUvorder(u32LayerId, pstInfo->stSrcInfo.bUvOrder);
    VDP_VID_SetInDataFmt               (u32LayerId, pstInfo->stSrcInfo.enInFmt);

    /*same with mv410, V3 read according to the interface.*/
    if (u32LayerId == VDP_LAYER_VID3)
    {
        VDP_VID_SetReadMode                (u32LayerId, 0,0);
    }
    else
    {
        VDP_VID_SetReadMode                (u32LayerId, pstInfo->enReadMode, pstInfo->enReadMode);
    }


    if (pstInfo->enDispMode == VDP_DISP_MODE_2D)
    {
        VDP_VID_Set16RegionSmmuLumBypass (u32LayerId, pstInfo->u32RegionNO, !pstInfo->stSrcInfo.bSmmuEn);
        VDP_VID_Set16RegionSmmuChromBypass (u32LayerId, pstInfo->u32RegionNO, !pstInfo->stSrcInfo.bSmmuEn);
    }
    else
    {
        VDP_VID_Set16RegionSmmuLumBypass (u32LayerId, 0, !pstInfo->stSrcInfo.bSmmuEn);
        VDP_VID_Set16RegionSmmuChromBypass (u32LayerId, 0, !pstInfo->stSrcInfo.bSmmuEn);
        VDP_VID_Set16RegionSmmuLumBypass (u32LayerId, 1, !pstInfo->stSrcInfo.bSmmuEn);
        VDP_VID_Set16RegionSmmuChromBypass (u32LayerId, 1, !pstInfo->stSrcInfo.bSmmuEn);
    }

    VDP_VID_SetDataWidth               (u32LayerId, pstInfo->stSrcInfo.enDataWidth);
    //VDP_VID_SetLayerAddr           (u32LayerId, 0, u32LumAddr, u32ChmAddr, u32LumStr, u32ChmStr);
    //VDP_VID_SetLayerAddr           (u32LayerId, 1, u32LumNAddr, u32ChmNAddr, u32LumStr, u32ChmStr);
//  printk("lay%d-%d\n",u32LayerId,pstInfo->stSrcInfo.bSecureEn);
    VDP_VID_SetNoSecFlag(u32LayerId, !pstInfo->stSrcInfo.bSecureEn);
    //----------------------------------------------------------------------
    //process
    //----------------------------------------------------------------------


    VDP_VID_SetFlipEnable              (u32LayerId, pstInfo->bFlipEn);
    VDP_VID_SetTileDcmpEnable          (u32LayerId, pstInfo->stSrcInfo.bDcmpEn);
    VDP_VID_SetIfirMode (u32LayerId, VDP_IFIRMODE_6TAPFIR);

    //----------------------------------------------------------------------
    //multi region
    //----------------------------------------------------------------------
    //VDP_DRV_SetVidAllMultiRegion       ();



    //----------------------------------------------------------------------
    //crop setting
    //----------------------------------------------------------------------
    if ((u32LayerId == VDP_LAYER_VID0) || (u32LayerId == VDP_LAYER_VID1))
    {
        if(((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 720 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 576))
            ||((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 720 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 480)))
        {
            pstInfo->stVideoRect.u32X *=2;
            pstInfo->stVideoRect.u32Wth *=2;
            pstInfo->stDispRect.u32X *=2;
            pstInfo->stDispRect.u32Wth *=2;
            pstInfo->stZme2InRect.u32Wth *=2;
            pstInfo->stVdpDispInfo.stFmtResolution.u32Wth  *=2;

        }
    }
    stCropInfo.stSrcRect   = pstInfo->stSrcInfo.stSrcRect;
    stCropInfo.stCropRect  = pstInfo->stCropRect;
    stCropInfo.enInFmt     = pstInfo->stSrcInfo.enInFmt;
    stCropInfo.enReadMode  = pstInfo->enReadMode;
    stCropInfo.enDispMode  = pstInfo->enDispMode;
    stCropInfo.bFlipEn     = pstInfo->bFlipEn;
    stCropInfo.u32ChmRatio = u32ChmRatio;
    stCropInfo.enDrawMode  = pstInfo->enDrawMode;


    memcpy(&stCropInfo.stAddr, &pstInfo->stSrcInfo.stAddr, VDP_ADDR_BUTT * sizeof(VDP_ADDR_S));

    VDP_DRV_SetVidCrop                 (u32LayerId, &stCropInfo, &stAddrInfo);



    //----------------------------------------------------------------------
    //addr setting
    //----------------------------------------------------------------------
    stAddrInfo.bFlipEn     = pstInfo->bFlipEn;
    stAddrInfo.u32Hgt      = pstInfo->stCropRect.u32Hgt;
    stAddrInfo.u32ChmRatio = u32ChmRatio;
    stAddrInfo.enInFmt     = pstInfo->stSrcInfo.enInFmt;
    stAddrInfo.enDispMode  = pstInfo->enDispMode;
    stAddrInfo.bDcmpEn     = pstInfo->stSrcInfo.bDcmpEn;

    if (u32LayerId == VDP_LAYER_VID0
        || u32LayerId == VDP_LAYER_VID3)
    {
        VDP_DRV_SetVidAddr(u32LayerId, &stAddrInfo);
    }

    //----------------------------------------------------------------------
    //csc setting
    //----------------------------------------------------------------------
    //VDP_PQ_GetCscCoef                  (pstInfo->stSrcInfo.enSrcClrSpace, pstInfo->enOutClrSpace, &stCscCfg);
    //VDP_VID_SetCsc                     ();//add

    //----------------------------------------------------------------------
    //zme setting
    //----------------------------------------------------------------------
    //VDP_DRV_GetZmeCfg                  ();
    stZmeInfo.stInRect    = stCropInfo.stCropRect;
    stZmeInfo.stOutRect   = pstInfo->stVideoRect;
    stZmeInfo.stZme2InRect   = pstInfo->stZme2InRect;//add by h226871
    stZmeInfo.enInFmt     = pstInfo->stSrcInfo.enInFmt;
    stZmeInfo.enReadMode  = pstInfo->enReadMode;
    stZmeInfo.enDispMode  = pstInfo->enDispMode;
    stZmeInfo.enDrawMode  = pstInfo->enDrawMode;

    stZmeInfo.stFmtResolution.u32Wth = pstInfo->stVdpDispInfo.stFmtResolution.u32Wth;
    stZmeInfo.stFmtResolution.u32Hgt = pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt;
    stZmeInfo.enDataWith = pstInfo->stSrcInfo.enDataWidth;
    stZmeInfo.stVdpDispInfo.stFmtResolution.u32Wth = pstInfo->stVdpDispInfo.stFmtResolution.u32Wth;
    stZmeInfo.stVdpDispInfo.stFmtResolution.u32Hgt = pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt;


    memcpy(&stZmeInfo.pstZmeDrvPara, &pstInfo->pstZmeDrvPara,  sizeof(VDP_VZME_DRV_PARA_S));


    VDP_DRV_SetVidZme                  (u32LayerId, &stZmeInfo);

    if ((u32LayerId == VDP_LAYER_VID0) || (u32LayerId == VDP_LAYER_VID1))
     {
      if(((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 1440 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 576))
       ||((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 1440 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 480)))
      {
       if (u32LayerId == VDP_LAYER_VID0)
       {
         VDP_DRV_SetVid0Zme1ForPal(u32LayerId, &stZmeInfo);
           VDP_DRV_SetVidZme2ForPal (u32LayerId, HI_TRUE);
       }
       else  if (u32LayerId == VDP_LAYER_VID1)
       {
           VDP_DRV_SetVid1ZmeForPal (u32LayerId);
       }
      }
     }

    VDP_VID_SetVideoPos                (u32LayerId, pstInfo->stVideoRect);
    VDP_VID_SetDispPos                 (u32LayerId, pstInfo->stDispRect);
    //----------------------------------------------------------------------
    //VP0 setting
    //----------------------------------------------------------------------
    //pstInfo->stVdpDispInfo.stFmtResolution.u32Y= 0;

    if (u32LayerId != VDP_LAYER_VID3)
    {
        VDP_VP_SetVideoPos             (VDP_LAYER_VP0, pstInfo->stVdpDispInfo.stFmtResolution);
        VDP_VP_SetDispPos              (VDP_LAYER_VP0, pstInfo->stVdpDispInfo.stFmtResolution);
        VDP_VP_SetInReso               (VDP_LAYER_VP0, pstInfo->stVdpDispInfo.stFmtResolution);
        VDP_VP_SetLayerGalpha          (VDP_LAYER_VP0, 0xff);
    }
    //--------------------------------------------
    //VP CSC setting
    //--------------------------------------------
    //VDP_VP_SetCscEnable          (VDP_LAYER_VP0, HI_TRUE);
    //VDP_VP_SetCscMode            (VDP_LAYER_VP0, enCscMode);

    //VDP_VP_SetCscEnable          (VDP_LAYER_VP0, HI_TRUE, 0);
    //VDP_VP_SetCscEnable          (VDP_LAYER_VP0, HI_TRUE, 1);

    //VDP_VP_SetThreeDimDofEnable  (VDP_LAYER_VP0, pstCfg->bDofEn);
    //VDP_VP_SetThreeDimDofStep    (VDP_LAYER_VP0, s32LStep, s32RStep);
    VDP_DRV_SetVidCsc(u32LayerId, pstInfo->enInClrSpace, pstInfo->enOutClrSpace);


    //-------------v1 multi region setting---------------//
    if(u32LayerId == VDP_LAYER_VID1)
    {

        VDP_VID_SetMultiModeEnable     (u32LayerId, pstInfo->bRegionModeEn);

        if(pstInfo->bRegionModeEn == 1)
        {
            VP0_IRESO.u32 = VDP_RegRead((HI_ULONG)&pVdpReg->VP0_IRESO.u32);
            stVid1Reso.u32Wth = VP0_IRESO.bits.iw + 1;
            stVid1Reso.u32Hgt = VP0_IRESO.bits.ih + 1;

            VDP_VID_SetOutReso(u32LayerId, stVid1Reso);
            VDP_VID_SetVideoPos                (u32LayerId, stVid1Reso);
            VDP_VID_SetDispPos                 (u32LayerId, stVid1Reso);
     if(((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 1440 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 576))
      ||((pstInfo->stVdpDispInfo.bInterlace  == 1)&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Wth == 1440 )&&(pstInfo->stVdpDispInfo.stFmtResolution.u32Hgt  == 480)))
     {
          stVid1Reso.u32Wth = (VP0_IRESO.bits.iw + 1)/2;
          pstInfo->stVideoRect.u32X /=2;
          pstInfo->stVideoRect.u32Wth /=2;
     }
            VDP_VID_SetInReso                  (u32LayerId, stVid1Reso);
        }

        if(pstInfo->bMuteEn == HI_TRUE)
        {

            VDP_VID_SetInDataFmt               (u32LayerId, VDP_VID_IFMT_SP_420);
            VDP_VID_SetZmeInFmt            (u32LayerId, VDP_PROC_FMT_SP_420);
            VDP_VID_SetZmeOutFmt           (u32LayerId, VDP_PROC_FMT_SP_422);
            VDP_VID_Set16RegionSmmuLumBypass (u32LayerId, pstInfo->u32RegionNO, HI_TRUE);
            VDP_VID_Set16RegionSmmuChromBypass (u32LayerId, pstInfo->u32RegionNO, HI_TRUE);
            VDP_VID_SetRegionMuteEnable    (u32LayerId, pstInfo->u32RegionNO, !pstInfo->bMuteEn);
            VDP_VID_SetRegionAddr          (u32LayerId, pstInfo->u32RegionNO, gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_MUTE_V1_L], gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_MUTE_V1_C], 0x0, 0x0);
        }
        else
        {

            VDP_VID_SetRegionMuteEnable    (u32LayerId, pstInfo->u32RegionNO, pstInfo->bMuteEn);
            VDP_VID_SetRegionAddr          (u32LayerId, pstInfo->u32RegionNO, pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumAddr, pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmAddr, pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32LumStr, pstInfo->stSrcInfo.stAddr[VDP_ADDR_DATA].u32ChmStr);
        }

        VDP_VID_SetRegionEnable        (u32LayerId, pstInfo->u32RegionNO, pstInfo->bRegionEn);
        VDP_VID_SetRegionReso          (u32LayerId, pstInfo->u32RegionNO, pstInfo->stVideoRect);
    }
    else
    {
        VDP_VID_SetMultiModeEnable     (u32LayerId, HI_FALSE);
    }
    //----------------end-------------------//

    VDP_VP_SetMuteEnable              (VDP_LAYER_VP0, HI_FALSE);
    VDP_HDR_SetRegUp();
    VDP_VID_SetRegUp(u32LayerId);
    VDP_VP_SetRegUp(VDP_LAYER_VP0);


    return HI_SUCCESS;
}

HI_S32  VDP_DRV_SetVidCrop (HI_U32 u32LayerId, const VDP_DRV_CROP_INFO_S *pstCropInfo, VDP_DRV_ADDR_INFO_S *pstAddrInfo)
{
    VDP_RECT_S stInRect      = {0};
    VDP_RECT_S stTileCrop    = {0}; //only use pos info
    VDP_RECT_S stCropOutRect = {0}; //linear hor crop
    HI_U32  u32CropX         = 0; //x pos
    HI_U32  u32CropY         = 0; //y pos
    HI_U32 u32LumAddr        = 0 ;
    HI_U32 u32ChmAddr        = 0 ;
    HI_U32 u32LumAddr3D      = 0 ;
    HI_U32 u32ChmAddr3D      = 0 ;
    HI_U32 u32LumAddrTile2bit= 0 ;
    HI_U32 u32ChmAddrTile2bit= 0 ;
    HI_U32 u32TileVblockNum  = 0 ;

    u32CropX = pstCropInfo->stCropRect.u32X;
    u32CropY = pstCropInfo->stCropRect.u32Y;
    stCropOutRect = pstCropInfo->stCropRect;

    if(pstCropInfo->enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
        if(pstCropInfo->bFlipEn == HI_TRUE)
        {

            u32TileVblockNum   = (pstCropInfo->stSrcRect.u32Hgt+ 31)/32;
            u32LumAddr         = pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumStr * (32 * u32TileVblockNum - 16);
            u32ChmAddr         = pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmStr * (16 * u32TileVblockNum - 8);
            u32LumAddrTile2bit = pstCropInfo->stAddr[VDP_ADDR_TILE].u32LumAddr + pstCropInfo->stAddr[VDP_ADDR_TILE].u32LumStr * (u32TileVblockNum - 1) * 32;
            u32ChmAddrTile2bit = pstCropInfo->stAddr[VDP_ADDR_TILE].u32ChmAddr + pstCropInfo->stAddr[VDP_ADDR_TILE].u32ChmStr * (u32TileVblockNum - 1) * 16;

            stTileCrop.u32Y    = (32 * u32TileVblockNum - 16) - u32CropY - pstCropInfo->stCropRect.u32Hgt;

        }
        else
        {
            u32LumAddr         = pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumAddr;
            u32ChmAddr         = pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr;
            u32LumAddrTile2bit = pstCropInfo->stAddr[VDP_ADDR_TILE].u32LumAddr;
            u32ChmAddrTile2bit = pstCropInfo->stAddr[VDP_ADDR_TILE].u32ChmAddr;

            stTileCrop.u32Y    = u32CropY;
        }
        //----------------------------------------------------------------------
        //hardware crop
        //tile crop x_offset must 64 pixel aligned
        //do not support 3d mode in tile format
        //+----------+    +----------+    +----------+
        //|tile crop | -> |in rect   | -> |hor crop  |
        //+----------+    +----------+    +----------+
        //----------------------------------------------------------------------

        stCropOutRect.u32X = u32CropX%64;//2 pixel aligned
        stTileCrop.u32X = u32CropX - stCropOutRect.u32X;
        stInRect.u32Wth = pstCropInfo->stCropRect.u32Wth + stCropOutRect.u32X;
        stInRect.u32Hgt = pstCropInfo->stCropRect.u32Hgt;

    }
    else if(pstCropInfo->enInFmt == VDP_VID_IFMT_SP_TILE)
    {
        //----------------------------------------------------------------------
        //hardware & software crop
        //do not support 3d mode in tile format
        //----------------------------------------------------------------------

        //to do
    }
    else//linear
    {
        //+----------+    +----------+
        //|in rect   | -> |hor crop  |
        //+----------+    +----------+
        if(pstCropInfo->enDispMode == VDP_DISP_MODE_SBS)//sbs mode,position is single eye
        {
            stInRect.u32Wth      = pstCropInfo->stSrcRect.u32Wth  / 1;//hareware
            stCropOutRect.u32X   = stCropOutRect.u32X   / 1;
            stCropOutRect.u32Wth = stCropOutRect.u32Wth / 1;

            stInRect.u32Hgt      = pstCropInfo->stCropRect.u32Hgt ;
        }
        else if(pstCropInfo->enDispMode == VDP_DISP_MODE_TAB || pstCropInfo->enDispMode == VDP_DISP_MODE_LBL)
        {
            stInRect.u32Wth      = pstCropInfo->stSrcRect.u32Wth   ;
            stInRect.u32Hgt      = pstCropInfo->stCropRect.u32Hgt /1;//software
            stCropOutRect.u32Hgt = stCropOutRect.u32Hgt/1;
            u32CropY             = u32CropY/1;
        }
        else
        {
            stInRect.u32Wth      = pstCropInfo->stSrcRect.u32Wth ;
            stInRect.u32Hgt      = stCropOutRect.u32Hgt;
        }
        //---------------------
        //Addr Calc
        //---------------------
        if(pstCropInfo->bFlipEn == HI_TRUE)
        {
            u32LumAddr   = pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumStr * ( pstCropInfo->stCropRect.u32Hgt + u32CropY - 1);
            u32ChmAddr   = pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmStr * ((pstCropInfo->stCropRect.u32Hgt + u32CropY)/pstCropInfo->u32ChmRatio - 1);
            u32LumAddr3D = pstCropInfo->stAddr[VDP_ADDR_3D].u32LumAddr   + pstCropInfo->stAddr[VDP_ADDR_3D].u32LumStr   * ( pstCropInfo->stCropRect.u32Hgt + u32CropY - 1);
            u32ChmAddr3D = pstCropInfo->stAddr[VDP_ADDR_3D].u32ChmAddr   + pstCropInfo->stAddr[VDP_ADDR_3D].u32ChmStr   * ((pstCropInfo->stCropRect.u32Hgt + u32CropY)/pstCropInfo->u32ChmRatio - 1);
        }
        else
        {
            u32LumAddr   = pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumStr * u32CropY;
            u32ChmAddr   = pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr + pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmStr * u32CropY / pstCropInfo->u32ChmRatio;
            u32LumAddr3D = pstCropInfo->stAddr[VDP_ADDR_3D].u32LumAddr  + pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumStr * u32CropY ;
            u32ChmAddr3D = pstCropInfo->stAddr[VDP_ADDR_3D].u32ChmAddr  + pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmStr * u32CropY / pstCropInfo->u32ChmRatio ;
        }

        //----------------------------------------------------------------------
        //software crop for ver
        //----------------------------------------------------------------------
        // draw to do...

    }

    //--------------draw mode setting-------------//
    if(pstCropInfo->enDrawMode == VDP_DRAW_MODE_2)
    {
        stInRect.u32Hgt /=2;
    }
    else if(pstCropInfo->enDrawMode== VDP_DRAW_MODE_4)
    {
        stInRect.u32Hgt /=4;
    }
    else if(pstCropInfo->enDrawMode== VDP_DRAW_MODE_8)
    {
        stInRect.u32Hgt /=8;
    }
    else //no draw
    {
        stInRect.u32Hgt /=1;
    }
    VDP_VID_SetDrawMode (u32LayerId, pstCropInfo->enDrawMode, pstCropInfo->enDrawMode);
    //--------------draw mode setting end -------------//
    VDP_VID_SetInReso                  (u32LayerId, stInRect);
    VDP_VID_SetSrcCropReso             (u32LayerId, stTileCrop);//tile crop
    VDP_VID_SetCropReso                (u32LayerId, stCropOutRect);//linear crop only for hor

    // flip function to do...
    pstAddrInfo->stAddr[VDP_ADDR_DATA].u32LumAddr  = u32LumAddr   ;
    pstAddrInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr  = u32ChmAddr   ;
    pstAddrInfo->stAddr[VDP_ADDR_3D].u32LumAddr    = u32LumAddr3D ;
    pstAddrInfo->stAddr[VDP_ADDR_3D].u32ChmAddr    = u32ChmAddr3D ;
    pstAddrInfo->stAddr[VDP_ADDR_TILE].u32LumAddr  = u32LumAddrTile2bit;
    pstAddrInfo->stAddr[VDP_ADDR_TILE].u32ChmAddr  = u32ChmAddrTile2bit;

    pstAddrInfo->stAddr[VDP_ADDR_DATA].u32LumStr  = pstCropInfo->stAddr[VDP_ADDR_DATA].u32LumStr;
    pstAddrInfo->stAddr[VDP_ADDR_DATA].u32ChmStr  = pstCropInfo->stAddr[VDP_ADDR_DATA].u32ChmStr;
    pstAddrInfo->stAddr[VDP_ADDR_3D].u32LumStr    = pstCropInfo->stAddr[VDP_ADDR_3D].u32LumStr  ;
    pstAddrInfo->stAddr[VDP_ADDR_3D].u32ChmStr    = pstCropInfo->stAddr[VDP_ADDR_3D].u32ChmStr  ;
    pstAddrInfo->stAddr[VDP_ADDR_TILE].u32LumStr  = pstCropInfo->stAddr[VDP_ADDR_TILE].u32LumStr;
    pstAddrInfo->stAddr[VDP_ADDR_TILE].u32ChmStr  = pstCropInfo->stAddr[VDP_ADDR_TILE].u32ChmStr;

    pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32LumAddr  = pstCropInfo->stAddr[VDP_ADDR_HEAD].u32LumAddr;
    pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32ChmAddr  = pstCropInfo->stAddr[VDP_ADDR_HEAD].u32ChmAddr;
    pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32DcmpHeadSize  = pstCropInfo->stAddr[VDP_ADDR_HEAD].u32DcmpHeadSize;
    pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32LumStr  = pstCropInfo->stAddr[VDP_ADDR_HEAD].u32LumStr;

    return HI_SUCCESS;
}

HI_S32 VDP_DRV_SetVidAddr(HI_U32 u32LayerId, const VDP_DRV_ADDR_INFO_S *pstAddrInfo)
{

    //----------------------------------------------------------------------
    //pstAddrInfo->u32LumAddr/u32ChmAddr is the address after CROP process
    //----------------------------------------------------------------------
    HI_U32 u32LumAddr;
    HI_U32 u32ChmAddr;
    HI_U32 u32LumAddr3D;
    HI_U32 u32ChmAddr3D;
    HI_U32 u32LumAddrTile2bit;
    HI_U32 u32ChmAddrTile2bit;

    u32LumAddr   = pstAddrInfo->stAddr[VDP_ADDR_DATA].u32LumAddr;
    u32ChmAddr   = pstAddrInfo->stAddr[VDP_ADDR_DATA].u32ChmAddr;
    u32LumAddr3D = pstAddrInfo->stAddr[VDP_ADDR_3D].u32LumAddr;
    u32ChmAddr3D = pstAddrInfo->stAddr[VDP_ADDR_3D].u32ChmAddr;

    u32LumAddrTile2bit   = pstAddrInfo->stAddr[VDP_ADDR_TILE].u32LumAddr;
    u32ChmAddrTile2bit   = pstAddrInfo->stAddr[VDP_ADDR_TILE].u32ChmAddr;

    VDP_VID_SetLayerAddr           (u32LayerId, 0, u32LumAddr, u32ChmAddr, pstAddrInfo->stAddr[VDP_ADDR_DATA].u32LumStr, pstAddrInfo->stAddr[VDP_ADDR_DATA].u32ChmStr);

    //----------------------------------------------------------------------
    //tile
    //----------------------------------------------------------------------
    if(pstAddrInfo->enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
        VDP_VID_SetTileStride      (u32LayerId, pstAddrInfo->stAddr[VDP_ADDR_TILE].u32LumStr, pstAddrInfo->stAddr[VDP_ADDR_TILE].u32LumStr);

        if(pstAddrInfo->bDcmpEn == HI_TRUE)
        {
            VDP_VID_SetHeadAddr        (u32LayerId, pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32LumAddr, pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32ChmAddr);
            VDP_VID_SetHeadStride      (u32LayerId, pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32LumStr);
            VDP_VID_SetHeadSize        (u32LayerId,  pstAddrInfo->stAddr[VDP_ADDR_HEAD].u32DcmpHeadSize);
        }

        if(pstAddrInfo->enDispMode != VDP_DISP_MODE_2D)
        {
            VDP_PRINT("VDP tile64 format do not support 3D!\n");
            return HI_FAILURE;
        }

        //addr of tile64 2bit data
        VDP_VID_SetLayerAddr           (u32LayerId, 1, u32LumAddrTile2bit, u32ChmAddrTile2bit, pstAddrInfo->stAddr[VDP_ADDR_DATA].u32LumStr, pstAddrInfo->stAddr[VDP_ADDR_DATA].u32ChmStr);
    }

    //----------------------------------------------------------------------
    //3d
    //----------------------------------------------------------------------
    if(pstAddrInfo->enDispMode != VDP_DISP_MODE_2D)
    {
        //addr of 3D right eye data
        VDP_VID_SetLayerAddr           (u32LayerId, 1, u32LumAddr3D, u32ChmAddr3D, pstAddrInfo->stAddr[VDP_ADDR_3D].u32LumStr, pstAddrInfo->stAddr[VDP_ADDR_3D].u32ChmStr);
    }
    //----------------------------------------------------------------------
    //draw mode
    //----------------------------------------------------------------------
    //need to be updated in TV

    return HI_SUCCESS;
}

HI_VOID VDP_DRV_SetVid1ZmeForPal(HI_U32 u32LayerId)
{
        VDP_VID_SetZmeEnable                     (VDP_LAYER_VID1, VDP_ZME_MODE_HOR, HI_TRUE);
  VDP_VID_SetZmeFirEnable      (VDP_LAYER_VID1, VDP_ZME_MODE_HORL, HI_FALSE);
  VDP_VID_SetZmeFirEnable      (VDP_LAYER_VID1, VDP_ZME_MODE_HORC, HI_FALSE);
}
HI_S32 VDP_DRV_SetVid0Zme1ForPal(HI_U32 u32LayerId, VDP_DRV_ZME_INFO_S *pstZmeInfo)
{
    VDP_RECT_S stZme1Out = pstZmeInfo->stOutRect;
    HI_U32  u32ZmeWth = pstZmeInfo->stInRect.u32Wth;
    HI_U32  u32HorRation =1;
 stZme1Out.u32Wth /=2;
        VDP_VID_SetOutReso                  (u32LayerId, stZme1Out);
        VDP_VID_SetZmeHorRatio      (u32LayerId, (u32ZmeWth/2*ZME_HPREC)/stZme1Out.u32Wth*2*u32HorRation);
        return HI_SUCCESS;
}

HI_VOID VDP_DRV_SetVidZme2ForPal(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    if (u32LayerId == 0)
    {
        if(HI_TRUE == bEnable)
        {
            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_HOR, 0*ZME_HPREC);
            VDP_VID_SetZme2HorRatio            (u32LayerId, ZME_HPREC/2);

            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_VER, 0*ZME_VPREC);
            VDP_VID_SetZme2VerRatio            (u32LayerId, ZME_VPREC/1);
        }
        else
        {
            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_HOR, 0*ZME_HPREC);
            VDP_VID_SetZme2HorRatio            (u32LayerId, ZME_HPREC);

            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_VER, 0*ZME_VPREC);
            VDP_VID_SetZme2VerRatio            (u32LayerId, ZME_VPREC);
        }
    }

    return;
}

HI_VOID VDP_DRV_SetVidZme2(HI_U32 u32LayerId, HI_BOOL bEnable)
{
    if (u32LayerId == 0)
    {
        if(HI_TRUE == bEnable)
        {
            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_HOR, 0*ZME_HPREC);
            VDP_VID_SetZme2HorRatio            (u32LayerId, ZME_HPREC/2);

            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_VER, 0*ZME_VPREC);
            VDP_VID_SetZme2VerRatio            (u32LayerId, ZME_VPREC/2);
        }
        else
        {
            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_HOR, 0*ZME_HPREC);
            VDP_VID_SetZme2HorRatio            (u32LayerId, ZME_HPREC);

            VDP_VID_SetZme2Enable              (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2FirEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2MidEnable           (u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
            VDP_VID_SetZme2Phase               (u32LayerId, VDP_ZME_MODE_VER, 0*ZME_VPREC);
            VDP_VID_SetZme2VerRatio            (u32LayerId, ZME_VPREC);
        }
    }

    return;

}

HI_S32 VDP_DRV_GetVidZmeStrategy(VDP_DRV_ZME_INFO_S  *pstZmeInfo,
                                                HI_PQ_ZME_COEF_RATIO_E  *penHiPqZmeCoefRatio,
                                                HI_PQ_ZME_COEF_TYPE_E enPqZmeCoefRatio,
                                                VDP_VID_IFMT_E   enInZmeFmt)
{

    HI_S32 s32RatioHL=100;
    HI_S32 s32RatioHC=100;
    HI_S32 s32RatioVL=100;
    HI_S32 s32RatioVC=100;


    HI_S32 s32Ratio1d0=1*100;
    HI_S32 s32Ratio0d75=3*100/4;
    HI_S32 s32Ratio0d5=1*100/2;
    HI_S32 s32Ratio0d25=1*100/4;
    HI_S32 s32Ratio0d33=1*100/3;
    HI_S32 s32Ratio0d2=1*100/5;

    if (pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_420 || pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE ||  pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
            s32RatioHL  =(pstZmeInfo->stOutRect.u32Wth*100)/pstZmeInfo->stInRect.u32Wth;
            s32RatioHC  =(pstZmeInfo->stOutRect.u32Wth*100)/pstZmeInfo->stInRect.u32Wth;
            s32RatioVL  = pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
            s32RatioVC  = s32RatioVL*2;
    }
    else if (enInZmeFmt == VDP_VID_IFMT_SP_422)
    {
            s32RatioHL  =(pstZmeInfo->stOutRect.u32Wth*100)/pstZmeInfo->stInRect.u32Wth;
            s32RatioHC  =(pstZmeInfo->stOutRect.u32Wth*100)/pstZmeInfo->stInRect.u32Wth;
            s32RatioVL  =pstZmeInfo->stOutRect.u32Hgt*100/pstZmeInfo->stInRect.u32Hgt;
            s32RatioVC  =s32RatioVL;
    }



    switch(enPqZmeCoefRatio)
    {
    case HI_PQ_ZME_COEF_TYPE_LH:
        if(s32RatioHL == s32Ratio1d0)
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        else if((s32RatioHL >= s32Ratio0d75)&&(s32RatioHL < s32Ratio1d0))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
        }
        else if((s32RatioHL >= s32Ratio0d5)&&(s32RatioHL < s32Ratio0d75))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
        }
        else if((s32RatioHL >= s32Ratio0d33)&&(s32RatioHL <s32Ratio0d5))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
        }
        else if((s32RatioHL >= s32Ratio0d25)&&(s32RatioHL < s32Ratio0d33))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
        }
        else if((s32RatioHL >= s32Ratio0d2)&&(s32RatioHL < s32Ratio0d25))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
        }
        else
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        break;
    case HI_PQ_ZME_COEF_TYPE_CH:
        if(s32RatioHC == s32Ratio1d0)
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        else if((s32RatioHC >= s32Ratio0d75)&&(s32RatioHC < s32Ratio1d0))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
        }
        else if((s32RatioHC >= s32Ratio0d5)&&(s32RatioHC < s32Ratio0d75))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
        }
        else if((s32RatioHC >= s32Ratio0d33)&&(s32RatioHC < s32Ratio0d5))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
        }
        else if((s32RatioHC >= s32Ratio0d25)&&(s32RatioHC <s32Ratio0d33))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
        }
        else if((s32RatioHC >= s32Ratio0d2)&&(s32RatioHC < s32Ratio0d25))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
        }
        else
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        break;
    case HI_PQ_ZME_COEF_TYPE_LV:
    case HI_PQ_ZME_COEF_TYPE_CV:
        if(s32RatioVL == s32Ratio1d0)
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        else if((s32RatioVL >= s32Ratio0d75)&&(s32RatioVL < s32Ratio1d0))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_075;
        }
        else if((s32RatioVL >= s32Ratio0d5)&&(s32RatioVL < s32Ratio0d75))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_05;
        }
        else if((s32RatioVL >= s32Ratio0d33)&&(s32RatioVL < s32Ratio0d5))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_033;
        }
        else if((s32RatioVL >= s32Ratio0d25)&&(s32RatioVL <s32Ratio0d33))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_025;
        }
        else if((s32RatioVL >= s32Ratio0d2)&&(s32RatioVL < s32Ratio0d25))
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_0;
        }
        else
        {
            *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        }
        break;

    default:
        *penHiPqZmeCoefRatio =HI_PQ_ZME_COEF_1;
        break;
    }

    return  HI_SUCCESS;
}


HI_S32 VDP_DRV_SetVidZme(HI_U32 u32LayerId, VDP_DRV_ZME_INFO_S *pstZmeInfo)
{
    VDP_RECT_S stZme1Out = pstZmeInfo->stOutRect;
    //HI_U64 u64ZmeWth = pstZmeInfo->stInRect.u32Wth;//hyx
    HI_U32 u32ZmeWth = pstZmeInfo->stInRect.u32Wth;

    HI_PQ_ZME_COEF_RATIO_E enRationHL =HI_PQ_ZME_COEF_1;
    HI_PQ_ZME_COEF_RATIO_E enRationHC =HI_PQ_ZME_COEF_1;
    HI_PQ_ZME_COEF_RATIO_E enRationVL =HI_PQ_ZME_COEF_1;
    HI_PQ_ZME_COEF_RATIO_E enRationVC =HI_PQ_ZME_COEF_1;


    HI_U32 u32ZmeInResoHeight = 0;
    VDP_VZME_RTL_PARA_S   pstZmeRtlPara = {0};
    HI_U32 u32HorRation = 1;
    HI_U32 u32VerRation = 1;
    VDP_COEF_BUF_E enVdpCoefBufZmeHTmp = VDP_COEF_BUF_ZMEH;
    VDP_COEF_BUF_E enVdpCoefBufZmeVTmp = VDP_COEF_BUF_ZMEV;

#ifdef VDP_PQ_STRATEGY
    HI_U32                u32InFmtTmp = 0;

    if (u32LayerId == VDP_LAYER_VID3)
            return HI_SUCCESS;

    if((pstZmeInfo->stZme2InRect.u32Wth == pstZmeInfo->stOutRect.u32Wth/2)
        && (pstZmeInfo->stZme2InRect.u32Hgt ==  pstZmeInfo->stOutRect.u32Hgt/2))
    {
        stZme1Out.u32Wth = pstZmeInfo->stOutRect.u32Wth/2;
        stZme1Out.u32Hgt = pstZmeInfo->stOutRect.u32Hgt/2;
        VDP_DRV_SetVidZme2(u32LayerId, HI_TRUE);
    }
    else
    {
        VDP_DRV_SetVidZme2(u32LayerId, HI_FALSE);
    }

#else
    if((pstZmeInfo->stInRect.u32Wth < pstZmeInfo->stOutRect.u32Wth/2)
        && (pstZmeInfo->stInRect.u32Hgt < pstZmeInfo->stOutRect.u32Hgt/2))
    {
        stZme1Out.u32Wth = pstZmeInfo->stOutRect.u32Wth/2;
        stZme1Out.u32Hgt = pstZmeInfo->stOutRect.u32Hgt/2;

        VDP_DRV_SetVidZme2(u32LayerId, HI_TRUE);
    }
    else
    {
        VDP_DRV_SetVidZme2(u32LayerId, HI_FALSE);
    }
#endif

    u32ZmeInResoHeight = pstZmeInfo->stInRect.u32Hgt;

    if(pstZmeInfo->enDrawMode == VDP_DRAW_MODE_2)
    {
        u32ZmeInResoHeight /=2;
    }
    else if(pstZmeInfo->enDrawMode== VDP_DRAW_MODE_4)
    {
        u32ZmeInResoHeight /=4;
    }
    else if(pstZmeInfo->enDrawMode== VDP_DRAW_MODE_8)
    {
        u32ZmeInResoHeight /=8;
    }
    else //no draw
    {
        u32ZmeInResoHeight /=1;
    }

    if(pstZmeInfo->enDispMode == VDP_DISP_MODE_SBS)//sbs mode,position is single eye
    {
        u32HorRation = 2;
        u32VerRation = 1;
    }
    else if((pstZmeInfo->enDispMode == VDP_DISP_MODE_TAB) || (pstZmeInfo->enDispMode == VDP_DISP_MODE_LBL))
    {
        u32HorRation = 1;
        u32VerRation = 2;
    }
    else
    {
        u32HorRation = 1;
        u32VerRation = 1;
    }
    VDP_VID_SetOutReso                 (u32LayerId, stZme1Out);
    VDP_VID_SetZmeEnable               (u32LayerId, VDP_ZME_MODE_HOR, HI_TRUE);

    VDP_VID_SetZmeFirEnable            (u32LayerId, VDP_ZME_MODE_HORL, pstZmeInfo->pstZmeDrvPara.bZmeFirHL);
    VDP_VID_SetZmeFirEnable            (u32LayerId, VDP_ZME_MODE_HORC, pstZmeInfo->pstZmeDrvPara.bZmeFirHC);
    VDP_VID_SetZmeMidEnable            (u32LayerId, VDP_ZME_MODE_HOR, pstZmeInfo->pstZmeDrvPara.bZmeMedH);

    VDP_VID_SetZmePhase                (u32LayerId, VDP_ZME_MODE_HOR, 0*ZME_HPREC);//to do
    VDP_VID_SetZmeHorRatio             (u32LayerId, (u32ZmeWth/2*ZME_HPREC)/stZme1Out.u32Wth*2*u32HorRation);

    VDP_VID_SetZmeEnable               (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);

    VDP_VID_SetZmeFirEnable            (u32LayerId, VDP_ZME_MODE_VERL, pstZmeInfo->pstZmeDrvPara.bZmeFirVL);
    VDP_VID_SetZmeFirEnable            (u32LayerId, VDP_ZME_MODE_VERC, pstZmeInfo->pstZmeDrvPara.bZmeFirVC);
    VDP_VID_SetZmeMidEnable            (u32LayerId, VDP_ZME_MODE_VER, pstZmeInfo->pstZmeDrvPara.bZmeMedV);

    if ((3840 <= pstZmeInfo->stFmtResolution.u32Wth) && (2160 == pstZmeInfo->stFmtResolution.u32Hgt))
    {

        VDP_VID_SetVtapReduce(u32LayerId, HI_TRUE);
        VDP_VID_SetZmeFirEnable(u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
        VDP_VID_SetZmeFirEnable(u32LayerId, VDP_ZME_MODE_VER, HI_FALSE);
    }
    else
    {
        VDP_VID_SetVtapReduce(u32LayerId, HI_FALSE);
    }

#ifdef VDP_PQ_STRATEGY
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmWIn  = u32ZmeWth;
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmWOut = stZme1Out.u32Wth;
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmHIn  = u32ZmeInResoHeight;
    pstZmeInfo->pstZmeDrvPara.u32ZmeFrmHOut = stZme1Out.u32Hgt;

    if(pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_420 || pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE ||  pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
        u32InFmtTmp = 1;
    }

    pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtIn  = u32InFmtTmp;
    pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtOut = 0;
    pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtIn  = u32InFmtTmp;
    pstZmeInfo->pstZmeDrvPara.u8ZmeYCFmtOut = 0;
    VdpZmeComnSet(&pstZmeInfo->pstZmeDrvPara, &pstZmeRtlPara);
#else
    //s32ZmePhase = 0;
#endif

    VDP_VID_SetZmePhase                (u32LayerId, VDP_ZME_MODE_VERL, pstZmeRtlPara.s32ZmeOffsetVL);
    VDP_VID_SetZmePhase                (u32LayerId, VDP_ZME_MODE_VERC, pstZmeRtlPara.s32ZmeOffsetVC);
    VDP_VID_SetZmePhase                (u32LayerId, VDP_ZME_MODE_VERBL, pstZmeRtlPara.s32ZmeOffsetVLBtm);
    VDP_VID_SetZmePhase                (u32LayerId, VDP_ZME_MODE_VERBC, pstZmeRtlPara.s32ZmeOffsetVCBtm);
    VDP_VID_SetZmeVerRatio             (u32LayerId, (u32ZmeInResoHeight*ZME_VPREC)/stZme1Out.u32Hgt*u32VerRation);
 if((u32LayerId == VDP_LAYER_VID0)
    && (u32ZmeInResoHeight*10/stZme1Out.u32Hgt > 25 )
     && ((pstZmeInfo->stVdpDispInfo.stFmtResolution.u32Wth == 3840) ||
     (pstZmeInfo->stVdpDispInfo.stFmtResolution.u32Hgt == 2160))
 )
 {
     VDP_VID_SetDataWidth               (u32LayerId,VDP_DATA_WTH_8);
 }
 else
 {
   VDP_VID_SetDataWidth               (u32LayerId,pstZmeInfo->enDataWith);
 }
    //VDP_VID_SetZmeCoefAddr         (u32LayerId, VDP_VID_PARA_ZME_VER, u32VerCoefAddr);
    //VDP_VID_SetZmeCoefAddr             (u32LayerId, VDP_VID_PARA_ZME_HOR, u32HorCoefAddr);
#if 1
    enVdpCoefBufZmeHTmp =(u32LayerId == VDP_LAYER_VID0) ? VDP_COEF_BUF_ZMEH : VDP_COEF_BUF_ZMEH_V1;
    enVdpCoefBufZmeVTmp =(u32LayerId == VDP_LAYER_VID0) ? VDP_COEF_BUF_ZMEV : VDP_COEF_BUF_ZMEV_V1;


#ifndef VDP_PQ_STRATEGY
        {
             static HI_PQ_ZME_COEF_RATIO_E enHiPqZmeCoefRatio=HI_PQ_ZME_COEF_1;
             enRationHL  =enHiPqZmeCoefRatio;
             enRationHC  =enHiPqZmeCoefRatio;
             enRationVL  =enHiPqZmeCoefRatio;
             enRationVC  =enHiPqZmeCoefRatio;
        }
#else
        VDP_DRV_GetVidZmeStrategy(pstZmeInfo,&enRationHL,HI_PQ_ZME_COEF_TYPE_LH,pstZmeInfo->enInFmt);
        VDP_DRV_GetVidZmeStrategy(pstZmeInfo,&enRationHC,HI_PQ_ZME_COEF_TYPE_CH,pstZmeInfo->enInFmt);
        VDP_DRV_GetVidZmeStrategy(pstZmeInfo,&enRationVL,HI_PQ_ZME_COEF_TYPE_LV,pstZmeInfo->enInFmt);
        VDP_DRV_GetVidZmeStrategy(pstZmeInfo,&enRationVC,HI_PQ_ZME_COEF_TYPE_CV,pstZmeInfo->enInFmt);
#endif

    DRV_PQ_GetVdpZmeCoef(enRationHL,HI_PQ_ZME_COEF_TYPE_LH,HI_PQ_ZME_TAP_8T32P, (HI_S16*)gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]);
    DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_8LH,HI_PQ_ZME_COEF_TYPE_LH,(gstVdpCoefBufAddr.pu8CoefVirAddr[enVdpCoefBufZmeHTmp]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));


    DRV_PQ_GetVdpZmeCoef(enRationHC,HI_PQ_ZME_COEF_TYPE_CH,HI_PQ_ZME_TAP_4T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
    DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CH,HI_PQ_ZME_COEF_TYPE_CH,(gstVdpCoefBufAddr.pu8CoefVirAddr[enVdpCoefBufZmeHTmp]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));


    DRV_PQ_GetVdpZmeCoef(enRationVL,HI_PQ_ZME_COEF_TYPE_LV,HI_PQ_ZME_TAP_6T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
    DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_6LV,HI_PQ_ZME_COEF_TYPE_LV,(gstVdpCoefBufAddr.pu8CoefVirAddr[enVdpCoefBufZmeVTmp]),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
    DRV_PQ_GetVdpZmeCoef(enRationVC,HI_PQ_ZME_COEF_TYPE_CV,HI_PQ_ZME_TAP_4T32P, (HI_S16*)(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));
    DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_4CV,HI_PQ_ZME_COEF_TYPE_CV,(gstVdpCoefBufAddr.pu8CoefVirAddr[enVdpCoefBufZmeVTmp]+256),(gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_ZME_TMP]));

    VDP_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_HOR , (gstVdpCoefBufAddr.u32CoefPhyAddr[enVdpCoefBufZmeHTmp]));
    VDP_VID_SetZmeCoefAddr(u32LayerId, VDP_VID_PARA_ZME_VER , (gstVdpCoefBufAddr.u32CoefPhyAddr[enVdpCoefBufZmeVTmp]));

    VDP_VID_SetParaUpd(u32LayerId,VDP_VID_PARA_ZME_HOR);
    VDP_VID_SetParaUpd(u32LayerId,VDP_VID_PARA_ZME_VER);
#endif

    if(pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_420 || pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE ||  pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_TILE_64)
    {
        VDP_VID_SetZmeInFmt            (u32LayerId, VDP_PROC_FMT_SP_420);
        VDP_VID_SetZmeOutFmt           (u32LayerId, VDP_PROC_FMT_SP_422);
    }
    else if(pstZmeInfo->enInFmt == VDP_VID_IFMT_SP_444)
    {
        VDP_VID_SetZmeInFmt            (u32LayerId, VDP_PROC_FMT_SP_444);
        VDP_VID_SetZmeOutFmt           (u32LayerId, VDP_PROC_FMT_SP_444);
    }
    else
    {
        VDP_VID_SetZmeInFmt            (u32LayerId, VDP_PROC_FMT_SP_422);
        VDP_VID_SetZmeOutFmt           (u32LayerId, VDP_PROC_FMT_SP_422);
    }

    VDP_VID_SetZmeHfirOrder        (u32LayerId, VDP_ZME_ORDER_VH);



    if(u32LayerId == VDP_LAYER_VID1)
    {

        //CVFIR only
        VDP_VID_SetZmeEnable           (u32LayerId, VDP_ZME_MODE_HOR, HI_FALSE);
        VDP_VID_SetZmeEnable           (u32LayerId, VDP_ZME_MODE_VERL, HI_FALSE);
        VDP_VID_SetZmeEnable           (u32LayerId, VDP_ZME_MODE_VERC, HI_TRUE);

        VDP_VID_SetZmePhase            (u32LayerId, VDP_ZME_MODE_VER, 0);
        VDP_VID_SetZmeFirEnable        (u32LayerId, VDP_ZME_MODE_VER, HI_TRUE);
    }


    return HI_SUCCESS;
}


#if 1
HI_S32 VDP_DRV_SetVidCsc(HI_U32 u32LayerId, VDP_CLR_SPACE_E enInClr, VDP_CLR_SPACE_E enOutClr)
{
    HI_PQ_CSC_CRTL_S stCscCtrl = {0};
    VDP_CSC_MODE_E enCscMode = {0};

    VDP_CSC_MODE_E enCsc2Mode = {0};
    HI_PQ_CSC_MODE_E enPqCscMode = {0};

    VDP_CSC_COEF_S     stCscCoef = {0};
    VDP_CSC_DC_COEF_S  stCscDc = {0};
    HI_PQ_CSC_COEF_S   stPqCscCoef = {0};
    HI_PQ_CSC_DCCOEF_S stPqCscDc = {0};
    HI_S16             S16Yuv2Rgb[3][3] = {{0},{0},{0}};
    HI_S32             S32Yuv2RgbOutDc[3] = {0};
    HI_S32             S32Yuv2RgbInDc[3] = {0};


    //printk("--in--%d-out %d---\n",enInClr,enOutClr);

    if(enInClr == VDP_CLR_SPACE_YUV_601_L && enOutClr == VDP_CLR_SPACE_RGB_601)
    {
        enCscMode   = VDP_CSC_YUV2RGB_601;/* YCbCr_601 L  -> RGB */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_601;/* YCbCr_601 L  -> RGB */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_709_L && enOutClr == VDP_CLR_SPACE_RGB_709)
    {
        enCscMode = VDP_CSC_YUV2RGB_709          ; /* YCbCr_709 L  -> RGB */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_709          ; /* YCbCr_709 L  -> RGB */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_601 && enOutClr == VDP_CLR_SPACE_YUV_601_L)
    {
        enCscMode = VDP_CSC_RGB2YUV_601          ; /* RGB          -> YCbCr_601 L */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_601          ; /* RGB          -> YCbCr_601 L */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_709 && enOutClr == VDP_CLR_SPACE_YUV_709_L)
    {
        enCscMode = VDP_CSC_RGB2YUV_709          ; /* RGB          -> YCbCr_709 L */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_709          ; /* RGB          -> YCbCr_709 L */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_709_L && enOutClr == VDP_CLR_SPACE_YUV_601_L)
    {
        enCscMode = VDP_CSC_YUV2YUV_709_601      ; /* YCbCr_709 L  -> YCbCr_601 L */
        enPqCscMode = HI_PQ_CSC_YUV2YUV_709_601      ; /* YCbCr_709 L  -> YCbCr_601 L */
    }
    else if((enInClr == VDP_CLR_SPACE_YUV_601_L && enOutClr == VDP_CLR_SPACE_YUV_601_L)
          || (enInClr == VDP_CLR_SPACE_YUV_709_L && enOutClr == VDP_CLR_SPACE_YUV_709_L))//to do ...
     {
         enCscMode = VDP_CSC_YUV2YUV              ; /* YCbCr L      -> YCbCr L */
         enPqCscMode = HI_PQ_CSC_YUV2YUV              ; /* YCbCr L      -> YCbCr L */
     }
    else if(enInClr == VDP_CLR_SPACE_YUV_601_L && enOutClr == VDP_CLR_SPACE_YUV_709_L)
    {
        enCscMode = VDP_CSC_YUV2YUV_601_709      ; /* YCbCr_601 L  -> YCbCr_709 L */
        enPqCscMode = HI_PQ_CSC_YUV2YUV_601_709      ; /* YCbCr_601 L  -> YCbCr_709 L */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_709 && enOutClr == VDP_CLR_SPACE_RGB_2020)
    {
        enCscMode = VDP_CSC_RGB2RGB_709_2020     ; /* RGB_709      -> RGB_2020 */
        enPqCscMode = HI_PQ_CSC_RGB2RGB_709_2020     ; /* RGB_709      -> RGB_2020 */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_2020 && enOutClr == VDP_CLR_SPACE_YUV_2020_L)
    {
        enCscMode = VDP_CSC_RGB2YUV_2020_2020_L  ; /* RGB_2020     -> YCbCr_2020 L */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_2020_2020_L  ; /* RGB_2020     -> YCbCr_2020 L */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_2020_L && enOutClr == VDP_CLR_SPACE_RGB_2020)
    {
        enCscMode = VDP_CSC_YUV2RGB_2020_2020_L  ; /* YCbCr_2020 L -> RGB_2020 */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_2020_2020_L  ; /* YCbCr_2020 L -> RGB_2020 */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_2020 && enOutClr == VDP_CLR_SPACE_RGB_709)
    {
        enCscMode = VDP_CSC_RGB2RGB_2020_709     ; /* RGB_2020     -> RGB_709 */
        enPqCscMode = HI_PQ_CSC_RGB2RGB_2020_709     ; /* RGB_2020     -> RGB_709 */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_601 && enOutClr == VDP_CLR_SPACE_RGB_2020)
    {
        enCscMode = VDP_CSC_RGB2RGB_601_2020     ; /* RGB_601      -> RGB_2020 */
        enPqCscMode = HI_PQ_CSC_RGB2RGB_601_2020     ; /* RGB_601      -> RGB_2020 */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_2020 && enOutClr == VDP_CLR_SPACE_RGB_601)
    {
        enCscMode = VDP_CSC_RGB2RGB_2020_601     ; /* RGB_2020     -> RGB_601 */
        enPqCscMode = HI_PQ_CSC_RGB2RGB_2020_601     ; /* RGB_2020     -> RGB_601 */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_2020_L && enOutClr == VDP_CLR_SPACE_YUV_601_L)
    {
        enCscMode = VDP_CSC_YUV2YUV_2020_601_L_L ; /* YCbCr_2020 L -> YCbCr_601 L */
        enPqCscMode = HI_PQ_CSC_YUV2YUV_2020_601_L_L ; /* YCbCr_2020 L -> YCbCr_601 L */
    }

    else if(enInClr == VDP_CLR_SPACE_RGB_2020 && enOutClr == VDP_CLR_SPACE_YUV_601_L)
    {
        enCscMode = VDP_CSC_RGB2YUV_2020_601_L   ; /* RGB_2020     -> YCbCr_601 L */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_2020_601_L   ; /* RGB_2020     -> YCbCr_601 L */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_601 && enOutClr == VDP_CLR_SPACE_RGB_601)
    {
        enCscMode = VDP_CSC_YUV2RGB_601_FULL     ; /* YCbCr_601 F  -> RGB */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_601_FULL     ; /* YCbCr_601 F  -> RGB */
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_709 && enOutClr == VDP_CLR_SPACE_RGB_709)
    {
        enCscMode = VDP_CSC_YUV2RGB_709_FULL     ; /* YCbCr_709 F  -> RGB */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_709_FULL     ; /* YCbCr_709 F  -> RGB */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_601 && enOutClr == VDP_CLR_SPACE_YUV_601)
    {
        enCscMode = VDP_CSC_RGB2YUV_601_FULL     ; /* RGB          -> YCbCr_601 F */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_601_FULL     ; /* RGB          -> YCbCr_601 F */
    }
    else if(enInClr == VDP_CLR_SPACE_RGB_709 && enOutClr == VDP_CLR_SPACE_YUV_709)
    {
        enCscMode = VDP_CSC_RGB2YUV_709_FULL     ; /* RGB          -> YCbCr_709 F */
        enPqCscMode = HI_PQ_CSC_RGB2YUV_709_FULL     ; /* RGB          -> YCbCr_709 F */
    }
    else if((enInClr == VDP_CLR_SPACE_RGB_601 && enOutClr == VDP_CLR_SPACE_RGB_601)
         || (enInClr == VDP_CLR_SPACE_RGB_709 && enOutClr == VDP_CLR_SPACE_RGB_709)
         || (enInClr == VDP_CLR_SPACE_RGB_2020 && enOutClr == VDP_CLR_SPACE_RGB_2020))
    {
        enCscMode = VDP_CSC_RGB2RGB              ; /* RGB          -> RGB */
        enPqCscMode = HI_PQ_CSC_RGB2RGB              ; /* RGB          -> RGB */
    }
    else if((enInClr == VDP_CLR_SPACE_YUV_2020_L && enOutClr == VDP_CLR_SPACE_YUV_2020_L)
         || (enInClr == VDP_CLR_SPACE_YUV_601 && enOutClr == VDP_CLR_SPACE_YUV_2020_L)
          || (enInClr == VDP_CLR_SPACE_YUV_601_L && enOutClr == VDP_CLR_SPACE_YUV_2020_L)
         || (enInClr == VDP_CLR_SPACE_YUV_709 && enOutClr == VDP_CLR_SPACE_YUV_2020_L)
         || (enInClr == VDP_CLR_SPACE_YUV_709_L && enOutClr == VDP_CLR_SPACE_YUV_2020_L)

         )
    {
        enCscMode = VDP_CSC_YUV2YUV              ; /* YCbCr L      -> YCbCr L */
        enPqCscMode = HI_PQ_CSC_YUV2YUV              ; /* YCbCr L      -> YCbCr L */


        VDP_HDR_SetVDmDegammaEn(HI_FALSE);
        VDP_HDR_SetVDmRgb2lmsEn(HI_TRUE);
        VDP_HDR_SetVDmGammaEn  (HI_FALSE);
        VDP_HDR_SetVDmGammaV0En(HI_FALSE);
        VDP_HDR_SetVDmGammaV1En(HI_FALSE);
    }
    else if(enInClr == VDP_CLR_SPACE_YUV_2020_L && enOutClr == VDP_CLR_SPACE_YUV_709_L)
    {
        enCscMode = VDP_CSC_YUV2YUV              ; /* YCbCr L      -> YCbCr L */
        enPqCscMode = HI_PQ_CSC_YUV2RGB_2020_709_L              ; /* YCbCr L      -> YCbCr L */
        enCsc2Mode = VDP_CSC_RGB2YUV_709;

        //bt2020 yuv ->709 rgb
        VDP_DRV_SetVidBt2020Mode(enPqCscMode);
        //rgb709->yuv709
        VDP_DRV_SetVidRgb2YuvMode(enCsc2Mode);


        enPqCscMode =HI_PQ_CSC_YUV2YUV_2020_709_L_L;

    }
    else if(enInClr == VDP_CLR_SPACE_YUV_2020_L && enOutClr == VDP_CLR_SPACE_RGB_709)
    {
        enCscMode   = VDP_CSC_YUV2RGB_709;
        enPqCscMode = HI_PQ_CSC_YUV2RGB_2020_709_L ;
        enCsc2Mode  = VDP_CSC_RGB2RGB;
        VDP_DRV_SetVidBt2020Mode(enPqCscMode);
        VDP_DRV_SetVidRgb2YuvMode(enCsc2Mode);
        enPqCscMode = HI_PQ_CSC_YUV2RGB_2020_709_L;

    }
     else if(enInClr == VDP_CLR_SPACE_YUV_601_L&& enOutClr == VDP_CLR_SPACE_RGB_709)
    {
        enCscMode   = VDP_CSC_YUV2RGB_601   ;
        enPqCscMode = HI_PQ_CSC_YUV2RGB_601 ;
    }
    else
    {
        VDP_PRINT("VDP Err csc mode in %d out %d!\n",enInClr,enOutClr);
    }

    if(u32LayerId == VDP_LAYER_VID3)
    {
        stCscCtrl.bCSCEn    = HI_TRUE;
        stCscCtrl.u32CscPre= 12;
        stCscCtrl.enCscType = HI_PQ_CSC_TUNING_V3;

        VDP_PQ_SetCSCCoef(&stCscCtrl,enCscMode,enPqCscMode, &stPqCscCoef, &stPqCscDc);

        memcpy(&stCscCoef, &stPqCscCoef, sizeof(VDP_CSC_COEF_S));
        memcpy(&stCscDc,   &stPqCscDc,   sizeof(VDP_CSC_DC_COEF_S));

        VDP_VID_SetCscEnable          (u32LayerId, HI_TRUE);
        VDP_VID_SetCscCoef            (u32LayerId,stCscCoef);
        VDP_VID_SetCscDcCoef          (u32LayerId,stCscDc);
    }

    else
    {
        if((enPqCscMode != HI_PQ_CSC_YUV2YUV_2020_709_L_L)
            && (enPqCscMode != HI_PQ_CSC_YUV2RGB_2020_709_L))
        {
            stCscCtrl.bCSCEn    = HI_TRUE;
            stCscCtrl.u32CscPre= 12;
            stCscCtrl.enCscType = HI_PQ_CSC_TUNING_V0;

            VDP_PQ_SetCSCCoef(&stCscCtrl,enCscMode,enPqCscMode, &stPqCscCoef, &stPqCscDc);

            memcpy(&stCscCoef, &stPqCscCoef, sizeof(VDP_CSC_COEF_S));
            memcpy(&stCscDc, &stPqCscDc, sizeof(VDP_CSC_DC_COEF_S));

            S32Yuv2RgbOutDc[0] =  -stPqCscDc.csc_out_dc0*16*1024;
            S32Yuv2RgbOutDc[1] =  -stPqCscDc.csc_out_dc1*16*1024;
            S32Yuv2RgbOutDc[2] =  -stPqCscDc.csc_out_dc2*16*1024;

            S32Yuv2RgbInDc[0] =  stPqCscDc.csc_in_dc0*16;
            S32Yuv2RgbInDc[1] =  stPqCscDc.csc_in_dc1*16;
            S32Yuv2RgbInDc[2] =  stPqCscDc.csc_in_dc2*16;

            S16Yuv2Rgb[0][0]  = (HI_S16)stPqCscCoef.csc_coef00;
            S16Yuv2Rgb[0][1]  = (HI_S16)stPqCscCoef.csc_coef01;
            S16Yuv2Rgb[0][2]  = (HI_S16)stPqCscCoef.csc_coef02;
            S16Yuv2Rgb[1][0]  = (HI_S16)stPqCscCoef.csc_coef10;
            S16Yuv2Rgb[1][1]  = (HI_S16)stPqCscCoef.csc_coef11;
            S16Yuv2Rgb[1][2]  = (HI_S16)stPqCscCoef.csc_coef12;
            S16Yuv2Rgb[2][0]  = (HI_S16)stPqCscCoef.csc_coef20;
            S16Yuv2Rgb[2][1]  = (HI_S16)stPqCscCoef.csc_coef21;
            S16Yuv2Rgb[2][2]  = (HI_S16)stPqCscCoef.csc_coef22;

            VDP_HDR_SetVDmEn               (HI_TRUE);
            VDP_HDR_SetVDmYuv2rgbEn        (HI_TRUE);
            if(u32LayerId == VDP_LAYER_VID0)
            {
                VDP_HDR_SetVDmYuv2rgbV0En      (HI_TRUE);
                VDP_HDR_SetVDmImap0Yuv2rgb     (S16Yuv2Rgb);
                VDP_HDR_SetVDmImapYuv2rgbInDc0  (S32Yuv2RgbInDc);
                VDP_HDR_SetVDmImapYuv2rgbOutDc0  (S32Yuv2RgbOutDc);
                VDP_HDR_SetVDmGammaV0En(HI_FALSE);


            }
            else if(u32LayerId == VDP_LAYER_VID1)
            {
                VDP_HDR_SetVDmYuv2rgbV1En      (HI_TRUE);
                VDP_HDR_SetVDmImap1Yuv2rgb     (S16Yuv2Rgb);
                VDP_HDR_SetVDmImapYuv2rgbOutDc1  (S32Yuv2RgbOutDc);
                VDP_HDR_SetVDmImapYuv2rgbInDc1   (S32Yuv2RgbInDc);
                VDP_HDR_SetVDmGammaV1En(HI_FALSE);
            }

            VDP_HDR_SetVDmImapYuv2rgbscale2p (10);
        }
    }

    return HI_SUCCESS;

}


#define ALG_V_VZME_PRECISION 4096
#define ALG_V_HZME_PRECISION 1048576
/* NOTICE:
   Vdp video zme module is the same as graphic layer G0/G1;
   software don't need separate the progressive frome interlace of the output, hardware process it.
   */
HI_VOID VdpZmeComnSet(VDP_VZME_DRV_PARA_S* pstZmeDrvPara, VDP_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    HI_U32 u32ZmeCWIn, u32ZmeCWOut, u32ZmeCHIn, u32ZmeCHOut;
    HI_S32 s32VOffset, s32HOffset;
    HI_U32 u32ZmeRatioVLReal, u32ZmeRatioVCReal;


    /* calculate luma real zme resolution of input and output */
    pstZmeRtlPara->u32ZmeWIn  = pstZmeDrvPara->u32ZmeFrmWIn;
    pstZmeRtlPara->u32ZmeWOut = pstZmeDrvPara->u32ZmeFrmWOut;
    pstZmeRtlPara->u32ZmeHIn  = pstZmeDrvPara->u32ZmeFrmHIn;
    pstZmeRtlPara->u32ZmeHOut = pstZmeDrvPara->u32ZmeFrmHOut;

    /* when wide > 4096, overflow will occurs */
    if (pstZmeRtlPara->u32ZmeWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHL = (pstZmeRtlPara->u32ZmeWIn / 2 * ALG_V_HZME_PRECISION) / pstZmeRtlPara->u32ZmeWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHL = pstZmeRtlPara->u32ZmeWIn * ALG_V_HZME_PRECISION / pstZmeRtlPara->u32ZmeWOut;
    }
    pstZmeRtlPara->u32ZmeRatioVL = pstZmeRtlPara->u32ZmeHIn * ALG_V_VZME_PRECISION / pstZmeRtlPara->u32ZmeHOut;

    /* calculate chroma zme ratio */
    u32ZmeCWIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == 2 ) ? pstZmeRtlPara->u32ZmeWIn : pstZmeRtlPara->u32ZmeWIn / 2;
    u32ZmeCWOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == 2 ) ? pstZmeRtlPara->u32ZmeWOut : pstZmeRtlPara->u32ZmeWOut / 2;
    u32ZmeCHIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == 1 ) ? pstZmeRtlPara->u32ZmeHIn / 2 : pstZmeRtlPara->u32ZmeHIn;
    u32ZmeCHOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == 1 ) ? pstZmeRtlPara->u32ZmeHOut / 2 : pstZmeRtlPara->u32ZmeHOut;

    /* when wide > 4096, overflow will occurs */
    if (u32ZmeCWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHC = (u32ZmeCWIn / 2 * ALG_V_HZME_PRECISION) / u32ZmeCWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHC = u32ZmeCWIn * ALG_V_HZME_PRECISION / u32ZmeCWOut;
    }
    pstZmeRtlPara->u32ZmeRatioVC = u32ZmeCHIn * ALG_V_VZME_PRECISION / u32ZmeCHOut;

    /* Input Progressive and Output Interlace */
    if (1 == pstZmeDrvPara->bZmeFrmFmtIn && 0 == pstZmeDrvPara->bZmeFrmFmtOut)
    {
        u32ZmeRatioVLReal = pstZmeRtlPara->u32ZmeRatioVL * 2;
        u32ZmeRatioVCReal = pstZmeRtlPara->u32ZmeRatioVC * 2;
    }
    else
    {
        u32ZmeRatioVLReal = pstZmeRtlPara->u32ZmeRatioVL;
        u32ZmeRatioVCReal = pstZmeRtlPara->u32ZmeRatioVC;
    }

    /* calculate luma zme offset */
    pstZmeRtlPara->s32ZmeOffsetHL = 0;

    pstZmeRtlPara->s32ZmeOffsetVL = GetVerticalScalerOffset(u32ZmeRatioVLReal, pstZmeDrvPara);

    /* calculate chroma zme offset */
    s32VOffset = 0, s32HOffset = 0;
    if (1 == pstZmeDrvPara->u8ZmeYCFmtIn) /* 4:2:0 */
    {
        /* horizontal offset for cosited or interspersed format *//* fixed to false; interspersed format */
        s32HOffset = 0;

        /* vertical offset for processive or interlaced input format */
        if (pstZmeDrvPara->bZmeFrmFmtIn) /* processive input for zme */
        {
            s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; /* -1024 >> 1 */
        }
        else /* interlaced input for zme */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input for zme */
            {
                s32VOffset = -((3 * ALG_V_VZME_PRECISION) >> 2) >> 1; /* -3072 >> 1 */
            }
            else /* top field input for zme */
            {
                s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; /* -1024 >> 1 */
            }
        }
    }

    pstZmeRtlPara->s32ZmeOffsetHC = s32HOffset;
    pstZmeRtlPara->s32ZmeOffsetVC = s32VOffset + GetVerticalScalerOffset(u32ZmeRatioVCReal, pstZmeDrvPara);
    pstZmeRtlPara->s32ZmeOffsetVLBtm = GetVerticalScalerOffset(u32ZmeRatioVLReal, pstZmeDrvPara);
    pstZmeRtlPara->s32ZmeOffsetVCBtm = s32VOffset + GetVerticalScalerOffset(u32ZmeRatioVCReal, pstZmeDrvPara);
    pstZmeRtlPara->bZmeOrder = (pstZmeRtlPara->u32ZmeRatioHL >= ALG_V_HZME_PRECISION) ? 0 : 1;

    return;
}

static HI_S32 GetVerticalScalerOffset(HI_U32 u32VRatio, VDP_VZME_DRV_PARA_S* pstZmeDrvPara)
{
    HI_S32 s32VOffset = 0;

    if (pstZmeDrvPara->bZmeFrmFmtIn) /* processive input */
    {
        if (pstZmeDrvPara->bZmeFrmFmtOut) /* processive output */
        {
            s32VOffset = 0;
        }
        else /* interlaced output */
        {
            if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
            {
                s32VOffset = (HI_S32)u32VRatio >> 1;
            }
            else /*top field output */
            {
                s32VOffset = 0;
            }
        }
    }
    else /* interlaced input */
    {
        if (pstZmeDrvPara->bZmeFrmFmtOut) /* processive output */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input */
            {
                s32VOffset = -(ALG_V_VZME_PRECISION >> 1); /* -2048 */
            }
            else /* top field input */
            {
                s32VOffset = 0;
            }
        }
        else /* interlaced output */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input */
            {
                if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
                {
                    s32VOffset = ((HI_S32)u32VRatio - ALG_V_VZME_PRECISION) >> 1;
                }
                else /* top field output */
                {
                    s32VOffset = -(ALG_V_VZME_PRECISION >> 1); /* -2048 */
                }
            }
            else /* top field input */
            {
                if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
                {
                    s32VOffset = (HI_S32)u32VRatio >> 1;
                }
                else /* top field output */
                {
                    s32VOffset = 0;
                }
            }
        }
    }

    return s32VOffset;
}


#endif

