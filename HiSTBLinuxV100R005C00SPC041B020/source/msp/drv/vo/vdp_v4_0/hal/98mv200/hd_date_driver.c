#ifdef HI_DISP_BUILD_FULL
#include <linux/kernel.h>
#endif

#include "vdp_define.h"
#include "hi_reg_common.h"
#include "drv_disp_com.h"
#include "vdp_drv_vid.h"
#include "vdp_hal_comm.h"
#include "hal_specific_rwzb.h"
#include "vdp_fpga_define.h"
#include "vdp_define.h"
#include "vdp_fpga_define.h"
#define DHD_YUV_TO_HDMI 0


#ifdef    __VDP_SUPPORT_HDATE

extern  volatile S_VDP_REGS_TYPE *pVdpReg;

HI_VOID HDATE_SetVideoFmt(HI_U32 u32VideoFmt)
{
        //VDP_hdate_SetVideoFmt
	    volatile  U_HDATE_VIDEO_FORMAT  HDATE_VIDEO_FORMAT;

        HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
        HDATE_VIDEO_FORMAT.bits.video_ft = u32VideoFmt;
    
        VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

}

HI_VOID HDATE_SetVdacDetEnable(HI_U32 u32Data)
{
    //VDP_HDATE_SetVdacDetEnable
	volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.vdac_det_en = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

HI_VOID HDATE_SetVdacDetLine(HI_U32 u32Data)
{
    //VDP_HDATE_SetVdacDetLine
	volatile U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead((&(pVdpReg->HDATE_DACDET1.u32)));
    HDATE_DACDET1.bits.det_line = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_DACDET1.u32)), HDATE_DACDET1.u32);

    return ;
}

HI_VOID HDATE_SetVdacDetHigh(HI_U32 u32Data)
{
    //VDP_HDATE_SetVdacDetHigh
	volatile U_HDATE_DACDET1 HDATE_DACDET1 ;

    HDATE_DACDET1.u32 = VDP_RegRead((&(pVdpReg->HDATE_DACDET1.u32)));
    HDATE_DACDET1.bits.vdac_det_high = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_DACDET1.u32)), HDATE_DACDET1.u32);

    return ;
}

HI_VOID HDATE_SetVdacDetWidth(HI_U32 u32Data)
{
    //VDP_HDATE_SetVdacDetWidth
	volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.det_pixel_wid = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

HI_VOID HDATE_SetVdacDetStart(HI_U32 u32Data)
{
    //VDP_HDATE_SetVdacDetStart
	volatile U_HDATE_DACDET2 HDATE_DACDET2 ;

    HDATE_DACDET2.u32 = VDP_RegRead((&(pVdpReg->HDATE_DACDET2.u32)));
    HDATE_DACDET2.bits.det_pixel_sta = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_DACDET2.u32)), HDATE_DACDET2.u32);

    return ;
}

HI_VOID HDATE_DEFAULT(VDP_DISP_DIGFMT_E enDigFmt)
{
   //VDP_HDATE_DEFAULT
   volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
   volatile U_HDATE_POLA_CTRL HDATE_POLA_CTRL;
   volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;


    if(enDigFmt == VDP_DISP_DIGFMT_1080P30)
    {
        HDATE_POLA_CTRL.u32 = 0x3;
    HDATE_VIDEO_FORMAT.u32 = 0x000000a3;
    HDATE_OUT_CTRL.u32 = 0x000001b0;
    VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), HDATE_POLA_CTRL.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);
    }
    else
    {
        if(enDigFmt == VDP_DISP_DIGFMT_720P)
            HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
        else
            HDATE_VIDEO_FORMAT.u32 = 0x000000a4;

        VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
    }

}

HI_VOID HDATE_MODE(HI_U32 inft_mode)
{

	//VDP_HDATE_MODE  
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(inft_mode == VDP_DISP_DIGFMT_720P)
        HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
    else if(inft_mode == VDP_DISP_DIGFMT_1080I)
         HDATE_VIDEO_FORMAT.u32 = 0x000000a4;

    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

}

HI_VOID HDATE_SetEnable(DISP_VENC_E eDate,HI_U32 enable)
{
	//VDP_DATE_SetEnable
    if (DISP_VENC_HDATE0 == eDate)
    {
        U_HDATE_EN HDATE_EN;

        HDATE_EN.u32 = 0;
        HDATE_EN.bits.hd_en = enable;
        VDP_RegWrite(&(pVdpReg->HDATE_EN.u32), HDATE_EN.u32);
    }

    if (DISP_VENC_SDATE0 == eDate)
    {

    }
}

#define DISP_DATE_SRC_COEF_INDEX_DEFAULT 0
#define DISP_DATE_SRC_COEF_INDEX_480P  1
#define DISP_DATE_SRC_COEF_INDEX_576P  1
#define DISP_DATE_SRC_COEF_INDEX_720P  2
#define DISP_DATE_SRC_COEF_INDEX_1080I 2
#define DISP_DATE_SRC_COEF_INDEX_1080P 3

HI_VOID SDATE_HDATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt)
{
    //VDP_DATE_ResetFmt
	if (DISP_VENC_HDATE0 == eDate)
    {
        U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
        U_HDATE_OUT_CTRL     HDATE_OUT_CTRL;

        VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x03ul);
        VDP_RegWrite((&(pVdpReg->DHD0_SYNC_INV.u32)), 0x02000ul);

        HDATE_VIDEO_FORMAT.u32 = 0;
        
        switch(enFmt)
        {
            case HI_DRV_DISP_FMT_1080P_60:
            case HI_DRV_DISP_FMT_1080P_50:
            case HI_DRV_DISP_FMT_1080P_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a3;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_1080P));
                break;
            case HI_DRV_DISP_FMT_1080i_50:
                /* to resolve vsync for 1080i_50 */
                VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x05ul);
            case HI_DRV_DISP_FMT_1080i_60:
            case HI_DRV_DISP_FMT_1080i_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a4;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_1080I));
                VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x05ul);
                break;
            case HI_DRV_DISP_FMT_720P_60:
            case HI_DRV_DISP_FMT_720P_50:
            case HI_DRV_DISP_FMT_720P_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_720P));
                VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x01ul);
                break;
            case HI_DRV_DISP_FMT_576P_50:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a1;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_576P));
                VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x01ul);
                break;
            case HI_DRV_DISP_FMT_480P_60:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a0;
                /* to resolve rwzb for 480p vsync */
                VDP_RegWrite((&(pVdpReg->DHD0_SYNC_INV.u32)), 0x06000ul);
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_480P));
                break;
            case HI_DRV_DISP_FMT_3840X2160_24:
            case HI_DRV_DISP_FMT_3840X2160_25:
            case HI_DRV_DISP_FMT_3840X2160_30:
            case HI_DRV_DISP_FMT_4096X2160_24:
            case HI_DRV_DISP_FMT_3840X2160_23_976:
            case HI_DRV_DISP_FMT_3840X2160_29_97:
                 /*4k*2k do nothing. to be confirm*/
                break;
            default :
                HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_DEFAULT));

                break;
        }
        VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);


        HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_OUT_CTRL.u32));

        HDATE_OUT_CTRL.bits.src_ctrl = 2;
        HDATE_OUT_CTRL.bits.sync_lpf_en = 1;

        //HDATE_OUT_CTRL.u32 = HDATE_OUT_CTRL.u32 | 0x1000;
        HDATE_OUT_CTRL.bits.video1_sel = 2;
        HDATE_OUT_CTRL.bits.video2_sel = 3;
        HDATE_OUT_CTRL.bits.video3_sel = 1;
        VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);


        // DTS2012080100525 : some sony46 TV can not accept 576P50 siganl correctly
        {
            U_HDATE_CLIP HDATE_CLIP;  // new add

            HDATE_CLIP.u32 = 0;

            // 20 x clk_pix + 4 x clk_dis, now clk_pix = 2*clk_dis for HDATE_OUT_CTRL.bits.src_ctrl = 2
            HDATE_CLIP.bits.clip_bb = 80;
            HDATE_CLIP.bits.clip_fb = 8;
            HDATE_CLIP.bits.clip_disable = 0;
            HDATE_CLIP.bits.clip_thdl = 251;
            VDP_RegWrite((&(pVdpReg->HDATE_CLIP.u32)), HDATE_CLIP.u32);
        }
    }

    if (DISP_VENC_SDATE0 == eDate)
    {
        U_DATE_COEFF0 DATE_COEFF0;
        U_DATE_COEFF21 DATE_COEFF21;		

        switch(enFmt)
        {
            case HI_DRV_DISP_FMT_PAL:
            case HI_DRV_DISP_FMT_PAL_B:
            case HI_DRV_DISP_FMT_PAL_B1:
            case HI_DRV_DISP_FMT_PAL_D:
            case HI_DRV_DISP_FMT_PAL_D1:
            case HI_DRV_DISP_FMT_PAL_G:
            case HI_DRV_DISP_FMT_PAL_H:
            case HI_DRV_DISP_FMT_PAL_K:
            case HI_DRV_DISP_FMT_PAL_I:
                //DATE_COEFF0.u32  = 0x628412dc;
                DATE_COEFF0.u32  = 0x6a8452dc;
                break;
            case HI_DRV_DISP_FMT_PAL_N:
                DATE_COEFF0.u32  = 0x6a8852dc;
                break;

            case HI_DRV_DISP_FMT_PAL_Nc:
                DATE_COEFF0.u32  = 0x6a9052dc;
                break;

            case HI_DRV_DISP_FMT_SECAM_SIN:      /**< SECAM_SIN*/
                DATE_COEFF0.u32  = 0x6aa052dc;
                break;

            case HI_DRV_DISP_FMT_SECAM_COS:      /**< SECAM_COS*/
                DATE_COEFF0.u32  = 0x6ae052dc;
                break;

            //NTSC
            case HI_DRV_DISP_FMT_PAL_M:
                DATE_COEFF0.u32  = 0x449050dc;
                break;

            case HI_DRV_DISP_FMT_PAL_60:
                DATE_COEFF0.u32  = 0x44a050dc;
                break;

            case HI_DRV_DISP_FMT_NTSC:
                DATE_COEFF0.u32  = 0x048450dc;
                break;
            case HI_DRV_DISP_FMT_NTSC_J:
                DATE_COEFF0.u32  = 0x248450dc;
                break;

            case HI_DRV_DISP_FMT_NTSC_443:
                DATE_COEFF0.u32  = 0x04a450dc;
                break;
            default :
                DATE_COEFF0.u32  = 0x628412dc;
                break;
        }	
		

        SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_SDateSrcCoef(0));
		
        // delay time of luma and chroma is too much, set luma_dl tobe zero.
        DATE_COEFF0.bits.luma_dl = 0;
        // close pbpr low pass filter for standard test
        DATE_COEFF0.bits.pbpr_lpf_en = 0;

        VDP_RegWrite((&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
        SDATE_Setc_gain(1);
		
		if ((HI_DRV_DISP_FMT_NTSC == enFmt )
			|| (HI_DRV_DISP_FMT_NTSC_J == enFmt )
			|| (HI_DRV_DISP_FMT_NTSC_443 == enFmt ))
		{
			/* NTSC Htimng*/
			SDATE_SetHTiming(1,(HI_U32 *)&u32SDDATESrcHTimingCoef[0]);
			SDATE_SetLenMode(0);
		}
		else
		{
			/*PAL  Htimng*/	  
			SDATE_SetHTiming(1,(HI_U32 *)&u32SDDATESrcHTimingCoef[1]);
			SDATE_SetLenMode(1);
		}

        // to get good SCH parameters, set video_phase_delta as 0x9A
        VDP_RegWrite((&(pVdpReg->DATE_COEFF22.u32)), SCH_PHASE);


        DATE_COEFF21.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF21.u32));
        DATE_COEFF21.bits.dac0_in_sel = 1;
        DATE_COEFF21.bits.dac1_in_sel = 2;
        DATE_COEFF21.bits.dac2_in_sel = 4;
        DATE_COEFF21.bits.dac3_in_sel = 3;
        DATE_COEFF21.bits.dac4_in_sel = 5;
        DATE_COEFF21.bits.dac5_in_sel = 6;
        VDP_RegWrite((&(pVdpReg->DATE_COEFF21.u32)), DATE_COEFF21.u32);

    }
}

/*zkun_confirm:*/
HI_VOID SDATE_HDATE_SetSignal(HI_DRV_DISP_INTF_ID_E enIntfId,DISP_VENC_E eDate, HI_BOOL bRGBSync)
{
    //VDP_DATE_SetSignal
	//TODO SELECT RGB or YPbPr   and sync mode
    if (HI_DRV_DISP_INTF_YPBPR0 == enIntfId)
    {
        if (DISP_VENC_HDATE0 ==  eDate )
        {
            U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
            /**/
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));

            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 0;/*YCbCr£­>YPbPr*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 1;/*YPbPr insight sync*/
            HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;

            DATE_COEFF0.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF0.u32));
            /*0: yuv   1: rgb*/
            DATE_COEFF0.bits.rgb_en = 0;/* yuv*/

            DATE_COEFF0.bits.sync_mode_sel =   0;
            /*1:no sync
                0:sync_mode_sel is
            */
            DATE_COEFF0.bits.sync_mode_scart = 0;
            VDP_RegWrite((&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
        }
        else
        {/*VGA do nothing
           */
        }

    }
    if (HI_DRV_DISP_INTF_RGB0 == enIntfId)
    {
        if (DISP_VENC_HDATE0 == eDate )
        {
            U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
            /**/
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_U32)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));
            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 1;/*YCbCr¡ª>RGB ITU-R BT709*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 0;/*RGB sync in*/

            if (!bRGBSync)
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 0; /*no sync*/
            else
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;
            DATE_COEFF0.u32 = VDP_RegRead((HI_U32)&(pVdpReg->DATE_COEFF0.u32));
            /*0: yuv   1: rgb*/
            DATE_COEFF0.bits.rgb_en = 1;

             if (!bRGBSync)
            {
                DATE_COEFF0.bits.sync_mode_scart = 1;
                DATE_COEFF0.bits.sync_mode_sel =   0;
            }
            else
            {
                DATE_COEFF0.bits.sync_mode_scart = 0;
                DATE_COEFF0.bits.sync_mode_sel =   1;
            }
            VDP_RegWrite((&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

        }
    }
}

HI_BOOL HDATE_SetOutEn(HI_U32  u32Data)
{
    volatile U_HDATE_EN HDATE_EN;

    HDATE_EN.u32 = VDP_RegRead((&(pVdpReg->HDATE_EN.u32)));
    HDATE_EN.bits.hd_en = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_EN.u32)), HDATE_EN.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetSyncPola(HI_U32  u32Data, HI_U32 u32Data1)
{
	volatile U_HDATE_POLA_CTRL HDATE_POLA_CTRL;
    if(u32Data >= HDATE_SYNCMODE_NULL)
    {
        HI_PRINT("HDATE_SetSyncSel() , Select Wrong Type!\n");
    }

    HDATE_POLA_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_POLA_CTRL.u32)));

    switch(u32Data)
    {
        case HDATE_SYNCMODE_HSYNC_IN:
        {
            HDATE_POLA_CTRL.bits.hsync_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_HSYNC_OUT:
        {
            HDATE_POLA_CTRL.bits.hsync_out_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_VSYNC_IN:
        {
            HDATE_POLA_CTRL.bits.vsync_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_VSYNC_OUT:
        {
            HDATE_POLA_CTRL.bits.vsync_out_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_FID_IN:
        {
            HDATE_POLA_CTRL.bits.fid_in_pola = u32Data1;
            break;
        }
        case HDATE_SYNCMODE_FID_OUT:
        {
            HDATE_POLA_CTRL.bits.fid_out_pola = u32Data1;
            break;
        }
    default:
        {
        HI_PRINT("HDATE_SetSyncPola() , Select Wrong Type!\n");
        }
    }

    VDP_RegWrite((&(pVdpReg->HDATE_POLA_CTRL.u32)), HDATE_POLA_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetSyncMode(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
    if(u32Data < HDATE_SYNCCHN_RGB)
    {
        HI_PRINT("HDATE_SetSyncMode() , Select Wrong Type!\n");
    }

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));

    HDATE_VIDEO_FORMAT.bits.video_out_ctrl = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}


HI_BOOL HDATE_SetSyncChn(HI_U32  u32Data, HI_U32 u32Data1)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(u32Data >= HDATE_SYNCCHN_RGB)
    {
        HI_PRINT("HDATE_SetSyncChn() , Select Wrong Type!\n");
    }

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));

    if(false == u32Data1)
    {
        HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = HDATE_VIDEO_FORMAT.bits.sync_add_ctrl || u32Data;
    }
    else if(true == u32Data1)
    {
        HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = HDATE_VIDEO_FORMAT.bits.sync_add_ctrl && (~u32Data);
    }


    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetOutFmt(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.video_ft = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscMode(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    if(u32Data >= HDATE_CSCMODE_NULL)
    {
        HI_PRINT("HDATE_SetCscMode() , Select Wrong Type!\n");
    }


    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.csc_ctrl = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscRodEn(HI_U32  u32Data)
{
    volatile U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;

    HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)));
    HDATE_VIDEO_FORMAT.bits.csc_round_disable = ~u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCscCoeff(HI_U32 * psts32Data)
{
    volatile U_HDATE_CSC_COEF1 HDATE_CSC_COEF1;
    volatile U_HDATE_CSC_COEF2 HDATE_CSC_COEF2;
    volatile U_HDATE_CSC_COEF3 HDATE_CSC_COEF3;
    volatile U_HDATE_CSC_COEF4 HDATE_CSC_COEF4;
    volatile U_HDATE_CSC_COEF5 HDATE_CSC_COEF5;

    HDATE_CSC_COEF1.u32 = VDP_RegRead((&(pVdpReg->HDATE_CSC_COEF1.u32)));
    HDATE_CSC_COEF2.u32 = VDP_RegRead((&(pVdpReg->HDATE_CSC_COEF2.u32)));
    HDATE_CSC_COEF3.u32 = VDP_RegRead((&(pVdpReg->HDATE_CSC_COEF3.u32)));
    HDATE_CSC_COEF4.u32 = VDP_RegRead((&(pVdpReg->HDATE_CSC_COEF4.u32)));
    HDATE_CSC_COEF5.u32 = VDP_RegRead((&(pVdpReg->HDATE_CSC_COEF5.u32)));

    HDATE_CSC_COEF1.bits.csc_coef_r_y  = psts32Data[0];
    HDATE_CSC_COEF1.bits.csc_coef_r_cb = psts32Data[1];
    HDATE_CSC_COEF2.bits.csc_coef_r_cr = psts32Data[2];

    HDATE_CSC_COEF2.bits.csc_coef_g_y  = psts32Data[3];
    HDATE_CSC_COEF3.bits.csc_coef_g_cb = psts32Data[4];
    HDATE_CSC_COEF3.bits.csc_coef_g_cr = psts32Data[5];

    HDATE_CSC_COEF4.bits.csc_coef_b_y  = psts32Data[6];
    HDATE_CSC_COEF4.bits.csc_coef_b_cb = psts32Data[7];
    HDATE_CSC_COEF5.bits.csc_coef_b_cr = psts32Data[8];

    VDP_RegWrite((&(pVdpReg->HDATE_CSC_COEF1.u32)), HDATE_CSC_COEF1.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_CSC_COEF2.u32)), HDATE_CSC_COEF2.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_CSC_COEF3.u32)), HDATE_CSC_COEF3.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_CSC_COEF4.u32)), HDATE_CSC_COEF4.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_CSC_COEF5.u32)), HDATE_CSC_COEF5.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetOutSel(HI_U32  u32Data,HI_U32 u32Data1)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_OUT_CTRL.u32)));

    switch(u32Data)
    {
        case HDATE_OUTCHN_VID1:
        {
            HDATE_OUT_CTRL.bits.video1_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VID2:
        {
            HDATE_OUT_CTRL.bits.video2_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VID3:
        {
            HDATE_OUT_CTRL.bits.video3_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_HSYNC:
        {
            HDATE_OUT_CTRL.bits.hsync_sel = u32Data1;
            break;
        }
        case HDATE_OUTCHN_VSYNC:
        {
            HDATE_OUT_CTRL.bits.vsync_sel = u32Data1;
            break;
        }
        default:
        {
        HI_PRINT("HDATE_SetOutSel() , Select Wrong Type!\n");
        }
    }

    VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetInSrcMode(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.sd_sel = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetInSrcOsMode(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.src_ctrl = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetLpfEn(HI_U32  u32Data)
{
    volatile U_HDATE_OUT_CTRL HDATE_OUT_CTRL;

    HDATE_OUT_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_OUT_CTRL.u32)));
    HDATE_OUT_CTRL.bits.sync_lpf_en = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetTestData(HI_U32  u32Data)
{
    volatile U_HDATE_TEST HDATE_TEST;

    HDATE_TEST.u32 = VDP_RegRead((&(pVdpReg->HDATE_TEST.u32)));
    HDATE_TEST.bits.test_data = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_TEST.u32)), HDATE_TEST.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetVbiLpfEn(HI_U32  u32Data)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_VBI_CTRL.u32)));
    HDATE_VBI_CTRL.bits.vbi_lpf_en = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCgmsEn(HI_U32  u32Data,HI_U32 u32Data1)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_VBI_CTRL.u32)));

    if(u32Data  == HDATE_CGMS_TYPEA)
    {
        HDATE_VBI_CTRL.bits.cgmsa_add_en = u32Data1;
    }
    else
    {
        HDATE_VBI_CTRL.bits.cgmsb_add_en = u32Data1;
    }

    VDP_RegWrite((&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetCgmsData(HI_U32 u32Data,HI_U32 u32Data1)
{
    switch(u32Data)
    {
        case HDATE_CGMSA_DATA:
        {
            volatile U_HDATE_CGMSA_DATA HDATE_CGMSA_DATA;
            HDATE_CGMSA_DATA.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSA_DATA.u32)));

            HDATE_CGMSA_DATA.bits.cgmsa_data = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSA_DATA.u32)), HDATE_CGMSA_DATA.u32);

            break;
        }
        case HDATE_CGMSB_HEAD:
        {
            volatile U_HDATE_CGMSB_H HDATE_CGMSB_H;
            HDATE_CGMSB_H.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSB_H.u32)));

            HDATE_CGMSB_H.bits.hdate_cgmsb_h = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSB_H.u32)), HDATE_CGMSB_H.u32);

            break;
        }
        case HDATE_CGMSB_DATA1:
        {
            volatile U_HDATE_CGMSB_DATA1 HDATE_CGMSB_DATA1;
            HDATE_CGMSB_DATA1.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSB_DATA1.u32)));

            HDATE_CGMSB_DATA1.bits.cgmsb_data1 = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSB_DATA1.u32)), HDATE_CGMSB_DATA1.u32);

            break;
        }
        case HDATE_CGMSB_DATA2:
        {
            volatile U_HDATE_CGMSB_DATA2 HDATE_CGMSB_DATA2;
            HDATE_CGMSB_DATA2.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSB_DATA2.u32)));

            HDATE_CGMSB_DATA2.bits.cgmsb_data2 = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSB_DATA2.u32)), HDATE_CGMSB_DATA2.u32);

            break;
        }
        case HDATE_CGMSB_DATA3:
        {
            volatile U_HDATE_CGMSB_DATA3 HDATE_CGMSB_DATA3;
            HDATE_CGMSB_DATA3.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSB_DATA3.u32)));

            HDATE_CGMSB_DATA3.bits.cgmsb_data3 = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSB_DATA3.u32)), HDATE_CGMSB_DATA3.u32);

            break;
        }
        case HDATE_CGMSB_DATA4:
        {
            volatile U_HDATE_CGMSB_DATA4 HDATE_CGMSB_DATA4;
            HDATE_CGMSB_DATA4.u32 = VDP_RegRead((&(pVdpReg->HDATE_CGMSB_DATA4.u32)));

            HDATE_CGMSB_DATA4.bits.cgmsb_data4 = u32Data1;
            VDP_RegWrite((&(pVdpReg->HDATE_CGMSB_DATA4.u32)), HDATE_CGMSB_DATA4.u32);

            break;
        }
    default:
        {
        HI_PRINT("HDATE_SetCgmsData() , Select Wrong Type!\n");
        }
    }
    return HI_TRUE;
}

HI_BOOL HDATE_SetMvEn(HI_U32  u32Data)
{
    volatile U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = VDP_RegRead((&(pVdpReg->HDATE_VBI_CTRL.u32)));
    HDATE_VBI_CTRL.bits.mv_en = u32Data;
    VDP_RegWrite((&(pVdpReg->HDATE_VBI_CTRL.u32)), HDATE_VBI_CTRL.u32);

    return HI_TRUE;
}

HI_BOOL HDATE_SetOvsCoef(HI_S32 * psts32Data)
{
    volatile U_HDATE_SRC_13_COEF1   HDATE_SRC_13_COEF1;
    volatile U_HDATE_SRC_13_COEF2   HDATE_SRC_13_COEF2;
    volatile U_HDATE_SRC_13_COEF3   HDATE_SRC_13_COEF3;
    volatile U_HDATE_SRC_13_COEF4   HDATE_SRC_13_COEF4;
    volatile U_HDATE_SRC_13_COEF5   HDATE_SRC_13_COEF5;
    volatile U_HDATE_SRC_13_COEF6   HDATE_SRC_13_COEF6;
    volatile U_HDATE_SRC_13_COEF7   HDATE_SRC_13_COEF7;
    volatile U_HDATE_SRC_13_COEF8   HDATE_SRC_13_COEF8;
    volatile U_HDATE_SRC_13_COEF9   HDATE_SRC_13_COEF9;
    volatile U_HDATE_SRC_13_COEF10  HDATE_SRC_13_COEF10;
    volatile U_HDATE_SRC_13_COEF11  HDATE_SRC_13_COEF11;
    volatile U_HDATE_SRC_13_COEF12  HDATE_SRC_13_COEF12;
    volatile U_HDATE_SRC_13_COEF13  HDATE_SRC_13_COEF13;
    volatile U_HDATE_SRC_13_COEF14  HDATE_SRC_13_COEF14;
    volatile U_HDATE_SRC_13_COEF15  HDATE_SRC_13_COEF15;
    volatile U_HDATE_SRC_13_COEF16  HDATE_SRC_13_COEF16;
    volatile U_HDATE_SRC_13_COEF17  HDATE_SRC_13_COEF17;
    volatile U_HDATE_SRC_13_COEF18  HDATE_SRC_13_COEF18;

    volatile U_HDATE_SRC_24_COEF1   HDATE_SRC_24_COEF1;
    volatile U_HDATE_SRC_24_COEF2   HDATE_SRC_24_COEF2;
    volatile U_HDATE_SRC_24_COEF3   HDATE_SRC_24_COEF3;
    volatile U_HDATE_SRC_24_COEF4   HDATE_SRC_24_COEF4;
    volatile U_HDATE_SRC_24_COEF5   HDATE_SRC_24_COEF5;
    volatile U_HDATE_SRC_24_COEF6   HDATE_SRC_24_COEF6;
    volatile U_HDATE_SRC_24_COEF7   HDATE_SRC_24_COEF7;
    volatile U_HDATE_SRC_24_COEF8   HDATE_SRC_24_COEF8;
    volatile U_HDATE_SRC_24_COEF9   HDATE_SRC_24_COEF9;
    volatile U_HDATE_SRC_24_COEF10  HDATE_SRC_24_COEF10;
    volatile U_HDATE_SRC_24_COEF11  HDATE_SRC_24_COEF11;
    volatile U_HDATE_SRC_24_COEF12  HDATE_SRC_24_COEF12;
    volatile U_HDATE_SRC_24_COEF13  HDATE_SRC_24_COEF13;
    volatile U_HDATE_SRC_24_COEF14  HDATE_SRC_24_COEF14;
    volatile U_HDATE_SRC_24_COEF15  HDATE_SRC_24_COEF15;
    volatile U_HDATE_SRC_24_COEF16  HDATE_SRC_24_COEF16;
    volatile U_HDATE_SRC_24_COEF17  HDATE_SRC_24_COEF17;
    volatile U_HDATE_SRC_24_COEF18  HDATE_SRC_24_COEF18;

    HDATE_SRC_13_COEF1.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF1.u32 )));
    HDATE_SRC_13_COEF2.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF2.u32 )));
    HDATE_SRC_13_COEF3.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF3.u32 )));
    HDATE_SRC_13_COEF4.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF4.u32 )));
    HDATE_SRC_13_COEF5.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF5.u32 )));
    HDATE_SRC_13_COEF6.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF6.u32 )));
    HDATE_SRC_13_COEF7.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF7.u32 )));
    HDATE_SRC_13_COEF8.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF8.u32 )));
    HDATE_SRC_13_COEF9.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF9.u32 )));
    HDATE_SRC_13_COEF10.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF10.u32)));
    HDATE_SRC_13_COEF11.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF11.u32)));
    HDATE_SRC_13_COEF12.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF12.u32)));
    HDATE_SRC_13_COEF13.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF13.u32)));
    HDATE_SRC_13_COEF14.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF14.u32)));
    HDATE_SRC_13_COEF15.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF15.u32)));
    HDATE_SRC_13_COEF16.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF16.u32)));
    HDATE_SRC_13_COEF17.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF17.u32)));
    HDATE_SRC_13_COEF18.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_13_COEF18.u32)));

    HDATE_SRC_24_COEF1.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF1.u32 )));
    HDATE_SRC_24_COEF2.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF2.u32 )));
    HDATE_SRC_24_COEF3.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF3.u32 )));
    HDATE_SRC_24_COEF4.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF4.u32 )));
    HDATE_SRC_24_COEF5.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF5.u32 )));
    HDATE_SRC_24_COEF6.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF6.u32 )));
    HDATE_SRC_24_COEF7.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF7.u32 )));
    HDATE_SRC_24_COEF8.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF8.u32 )));
    HDATE_SRC_24_COEF9.u32  = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF9.u32 )));
    HDATE_SRC_24_COEF10.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF10.u32)));
    HDATE_SRC_24_COEF11.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF11.u32)));
    HDATE_SRC_24_COEF12.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF12.u32)));
    HDATE_SRC_24_COEF13.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF13.u32)));
    HDATE_SRC_24_COEF14.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF14.u32)));
    HDATE_SRC_24_COEF15.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF15.u32)));
    HDATE_SRC_24_COEF16.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF16.u32)));
    HDATE_SRC_24_COEF17.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF17.u32)));
    HDATE_SRC_24_COEF18.u32 = VDP_RegRead((&(pVdpReg->HDATE_SRC_24_COEF18.u32)));

    HDATE_SRC_13_COEF1.bits.coef_tap1_1   = psts32Data[( 1-1)*4+0];
    HDATE_SRC_13_COEF2.bits.coef_tap2_1   = psts32Data[( 2-1)*4+0];
    HDATE_SRC_13_COEF3.bits.coef_tap3_1   = psts32Data[( 3-1)*4+0];
    HDATE_SRC_13_COEF4.bits.coef_tap4_1   = psts32Data[( 4-1)*4+0];
    HDATE_SRC_13_COEF5.bits.coef_tap5_1   = psts32Data[( 5-1)*4+0];
    HDATE_SRC_13_COEF6.bits.coef_tap6_1   = psts32Data[( 6-1)*4+0];
    HDATE_SRC_13_COEF7.bits.coef_tap7_1   = psts32Data[( 7-1)*4+0];
    HDATE_SRC_13_COEF8.bits.coef_tap8_1   = psts32Data[( 8-1)*4+0];
    HDATE_SRC_13_COEF9.bits.coef_tap9_1   = psts32Data[( 9-1)*4+0];
    HDATE_SRC_13_COEF10.bits.coef_tap10_1 = psts32Data[(10-1)*4+0];
    HDATE_SRC_13_COEF11.bits.coef_tap11_1 = psts32Data[(11-1)*4+0];
    HDATE_SRC_13_COEF12.bits.coef_tap12_1 = psts32Data[(12-1)*4+0];
    HDATE_SRC_13_COEF13.bits.coef_tap13_1 = psts32Data[(13-1)*4+0];
    HDATE_SRC_13_COEF14.bits.coef_tap14_1 = psts32Data[(14-1)*4+0];
    HDATE_SRC_13_COEF15.bits.coef_tap15_1 = psts32Data[(15-1)*4+0];
    HDATE_SRC_13_COEF16.bits.coef_tap16_1 = psts32Data[(16-1)*4+0];
    HDATE_SRC_13_COEF17.bits.coef_tap17_1 = psts32Data[(17-1)*4+0];
    HDATE_SRC_13_COEF18.bits.coef_tap18_1 = psts32Data[(18-1)*4+0];

    HDATE_SRC_24_COEF1.bits.coef_tap1_2   = psts32Data[( 1-1)*4+1];
    HDATE_SRC_24_COEF2.bits.coef_tap2_2   = psts32Data[( 2-1)*4+1];
    HDATE_SRC_24_COEF3.bits.coef_tap3_2   = psts32Data[( 3-1)*4+1];
    HDATE_SRC_24_COEF4.bits.coef_tap4_2   = psts32Data[( 4-1)*4+1];
    HDATE_SRC_24_COEF5.bits.coef_tap5_2   = psts32Data[( 5-1)*4+1];
    HDATE_SRC_24_COEF6.bits.coef_tap6_2   = psts32Data[( 6-1)*4+1];
    HDATE_SRC_24_COEF7.bits.coef_tap7_2   = psts32Data[( 7-1)*4+1];
    HDATE_SRC_24_COEF8.bits.coef_tap8_2   = psts32Data[( 8-1)*4+1];
    HDATE_SRC_24_COEF9.bits.coef_tap9_2   = psts32Data[( 9-1)*4+1];
    HDATE_SRC_24_COEF10.bits.coef_tap10_2 = psts32Data[(10-1)*4+1];
    HDATE_SRC_24_COEF11.bits.coef_tap11_2 = psts32Data[(11-1)*4+1];
    HDATE_SRC_24_COEF12.bits.coef_tap12_2 = psts32Data[(12-1)*4+1];
    HDATE_SRC_24_COEF13.bits.coef_tap13_2 = psts32Data[(13-1)*4+1];
    HDATE_SRC_24_COEF14.bits.coef_tap14_2 = psts32Data[(14-1)*4+1];
    HDATE_SRC_24_COEF15.bits.coef_tap15_2 = psts32Data[(15-1)*4+1];
    HDATE_SRC_24_COEF16.bits.coef_tap16_2 = psts32Data[(16-1)*4+1];
    HDATE_SRC_24_COEF17.bits.coef_tap17_2 = psts32Data[(17-1)*4+1];
    HDATE_SRC_24_COEF18.bits.coef_tap18_2 = psts32Data[(18-1)*4+1];

    HDATE_SRC_13_COEF1.bits.coef_tap1_3   = psts32Data[( 1-1)*4+2];
    HDATE_SRC_13_COEF2.bits.coef_tap2_3   = psts32Data[( 2-1)*4+2];
    HDATE_SRC_13_COEF3.bits.coef_tap3_3   = psts32Data[( 3-1)*4+2];
    HDATE_SRC_13_COEF4.bits.coef_tap4_3   = psts32Data[( 4-1)*4+2];
    HDATE_SRC_13_COEF5.bits.coef_tap5_3   = psts32Data[( 5-1)*4+2];
    HDATE_SRC_13_COEF6.bits.coef_tap6_3   = psts32Data[( 6-1)*4+2];
    HDATE_SRC_13_COEF7.bits.coef_tap7_3   = psts32Data[( 7-1)*4+2];
    HDATE_SRC_13_COEF8.bits.coef_tap8_3   = psts32Data[( 8-1)*4+2];
    HDATE_SRC_13_COEF9.bits.coef_tap9_3   = psts32Data[( 9-1)*4+2];
    HDATE_SRC_13_COEF10.bits.coef_tap10_3 = psts32Data[(10-1)*4+2];
    HDATE_SRC_13_COEF11.bits.coef_tap11_3 = psts32Data[(11-1)*4+2];
    HDATE_SRC_13_COEF12.bits.coef_tap12_3 = psts32Data[(12-1)*4+2];
    HDATE_SRC_13_COEF13.bits.coef_tap13_3 = psts32Data[(13-1)*4+2];
    HDATE_SRC_13_COEF14.bits.coef_tap14_3 = psts32Data[(14-1)*4+2];
    HDATE_SRC_13_COEF15.bits.coef_tap15_3 = psts32Data[(15-1)*4+2];
    HDATE_SRC_13_COEF16.bits.coef_tap16_3 = psts32Data[(16-1)*4+2];
    HDATE_SRC_13_COEF17.bits.coef_tap17_3 = psts32Data[(17-1)*4+2];
    HDATE_SRC_13_COEF18.bits.coef_tap18_3 = psts32Data[(18-1)*4+2];

    HDATE_SRC_24_COEF1.bits.coef_tap1_4   = psts32Data[( 1-1)*4+3];
    HDATE_SRC_24_COEF2.bits.coef_tap2_4   = psts32Data[( 2-1)*4+3];
    HDATE_SRC_24_COEF3.bits.coef_tap3_4   = psts32Data[( 3-1)*4+3];
    HDATE_SRC_24_COEF4.bits.coef_tap4_4   = psts32Data[( 4-1)*4+3];
    HDATE_SRC_24_COEF5.bits.coef_tap5_4   = psts32Data[( 5-1)*4+3];
    HDATE_SRC_24_COEF6.bits.coef_tap6_4   = psts32Data[( 6-1)*4+3];
    HDATE_SRC_24_COEF7.bits.coef_tap7_4   = psts32Data[( 7-1)*4+3];
    HDATE_SRC_24_COEF8.bits.coef_tap8_4   = psts32Data[( 8-1)*4+3];
    HDATE_SRC_24_COEF9.bits.coef_tap9_4   = psts32Data[( 9-1)*4+3];
    HDATE_SRC_24_COEF10.bits.coef_tap10_4 = psts32Data[(10-1)*4+3];
    HDATE_SRC_24_COEF11.bits.coef_tap11_4 = psts32Data[(11-1)*4+3];
    HDATE_SRC_24_COEF12.bits.coef_tap12_4 = psts32Data[(12-1)*4+3];
    HDATE_SRC_24_COEF13.bits.coef_tap13_4 = psts32Data[(13-1)*4+3];
    HDATE_SRC_24_COEF14.bits.coef_tap14_4 = psts32Data[(14-1)*4+3];
    HDATE_SRC_24_COEF15.bits.coef_tap15_4 = psts32Data[(15-1)*4+3];
    HDATE_SRC_24_COEF16.bits.coef_tap16_4 = psts32Data[(16-1)*4+3];
    HDATE_SRC_24_COEF17.bits.coef_tap17_4 = psts32Data[(17-1)*4+3];
    HDATE_SRC_24_COEF18.bits.coef_tap18_4 = psts32Data[(18-1)*4+3];


    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF1.u32 )), HDATE_SRC_13_COEF1.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF2.u32 )), HDATE_SRC_13_COEF2.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF3.u32 )), HDATE_SRC_13_COEF3.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF4.u32 )), HDATE_SRC_13_COEF4.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF5.u32 )), HDATE_SRC_13_COEF5.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF6.u32 )), HDATE_SRC_13_COEF6.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF7.u32 )), HDATE_SRC_13_COEF7.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF8.u32 )), HDATE_SRC_13_COEF8.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF9.u32 )), HDATE_SRC_13_COEF9.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF10.u32)), HDATE_SRC_13_COEF10.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF11.u32)), HDATE_SRC_13_COEF11.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF12.u32)), HDATE_SRC_13_COEF12.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF13.u32)), HDATE_SRC_13_COEF13.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF14.u32)), HDATE_SRC_13_COEF14.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF15.u32)), HDATE_SRC_13_COEF15.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF16.u32)), HDATE_SRC_13_COEF16.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF17.u32)), HDATE_SRC_13_COEF17.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_13_COEF18.u32)), HDATE_SRC_13_COEF18.u32);

    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF1.u32 )), HDATE_SRC_24_COEF1.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF2.u32 )), HDATE_SRC_24_COEF2.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF3.u32 )), HDATE_SRC_24_COEF3.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF4.u32 )), HDATE_SRC_24_COEF4.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF5.u32 )), HDATE_SRC_24_COEF5.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF6.u32 )), HDATE_SRC_24_COEF6.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF7.u32 )), HDATE_SRC_24_COEF7.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF8.u32 )), HDATE_SRC_24_COEF8.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF9.u32 )), HDATE_SRC_24_COEF9.u32 );
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF10.u32)), HDATE_SRC_24_COEF10.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF11.u32)), HDATE_SRC_24_COEF11.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF12.u32)), HDATE_SRC_24_COEF12.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF13.u32)), HDATE_SRC_24_COEF13.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF14.u32)), HDATE_SRC_24_COEF14.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF15.u32)), HDATE_SRC_24_COEF15.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF16.u32)), HDATE_SRC_24_COEF16.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF17.u32)), HDATE_SRC_24_COEF17.u32);
    VDP_RegWrite((&(pVdpReg->HDATE_SRC_24_COEF18.u32)), HDATE_SRC_24_COEF18.u32);

    return  HI_TRUE;
}


#endif

