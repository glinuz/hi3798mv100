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
#define DHD_YUV_TO_HDMI 0

#define DISP_DATE_SRC_COEF_INDEX_DEFAULT 0
#define DISP_DATE_SRC_COEF_INDEX_480P  1
#define DISP_DATE_SRC_COEF_INDEX_576P  1
#define DISP_DATE_SRC_COEF_INDEX_720P  2
#define DISP_DATE_SRC_COEF_INDEX_1080I 2
#define DISP_DATE_SRC_COEF_INDEX_1080P 3

HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset);

extern  volatile S_VDP_REGS_TYPE *pVdpReg;
HI_VOID SDATE_VBI_CCLineConfig(HI_BOOL TopEnable, HI_BOOL BottomEnable, HI_U16 TopLine, HI_U16 BottomLine)
{
	//VDP_VBI_CCLineConfig
	U_DATE_COEFF11 CCConfig;

	CCConfig.u32 = pVdpReg->DATE_COEFF11.u32;
	CCConfig.bits.cc_enf1 = TopEnable;
	CCConfig.bits.cc_enf2 = BottomEnable;
	CCConfig.bits.date_clf1 = TopLine;
	CCConfig.bits.date_clf2 = BottomLine;
    pVdpReg->DATE_COEFF11.u32 = CCConfig.u32;
	return;
}

HI_VOID SDATE_VBI_CCDataConfig(HI_U16 TopData, HI_U16 BottomData)
{
	//VDP_VBI_CCDataConfig
	U_DATE_COEFF12 CCData;

	CCData.bits.cc_f1data = TopData;
	CCData.bits.cc_f2data = BottomData;
	pVdpReg->DATE_COEFF12.u32 = CCData.u32;
	return;
}

HI_VOID SDATE_VBI_WSSConfig(HI_BOOL WssEnable, HI_U16 WssData)
{
	//VDP_VBI_WSSConfig
	U_DATE_COEFF15 WSSConfig;

	WSSConfig.u32 = pVdpReg->DATE_COEFF15.u32;
	WSSConfig.bits.wss_en = WssEnable;
	WSSConfig.bits.wss_data = WssData;

	pVdpReg->DATE_COEFF15.u32 = WSSConfig.u32;
	return;
}

HI_BOOL SDATE_VBI_IntStatus(HI_VOID)
{
    //VDP_VBI_IntStatus
	return pVdpReg->DATE_ISR.u32 & 0x1;
}

HI_VOID SDATE_VBI_TtxIntClear(HI_VOID)
{
    //VDP_VBI_TtxIntClear
	U_DATE_ISRSTATE DATE_ISRSTATE;

    DATE_ISRSTATE.u32 = pVdpReg->DATE_ISRSTATE.u32;
    DATE_ISRSTATE.u32 = 1;
    pVdpReg->DATE_ISRSTATE.u32 = DATE_ISRSTATE.u32;

	return;
}

HI_VOID SDATE_VBI_TtxSetMode(HI_U32 TtxMode)
{
    //VDP_VBI_TtxSetMode
	U_DATE_COEFF10 TtxConfig;

    TtxConfig.u32 = pVdpReg->DATE_COEFF10.u32;
    TtxConfig.bits.tt_mode = TtxMode;
    pVdpReg->DATE_COEFF10.u32 = TtxConfig.u32;
    return;
}

HI_VOID SDATE_VBI_TtxSetSeq(HI_BOOL TtxSeq)
{
	//VDP_VBI_TtxSetSeq
	U_DATE_COEFF0   DATE_COEFF0;

	DATE_COEFF0.u32 = pVdpReg->DATE_COEFF0.u32;
	DATE_COEFF0.bits.tt_seq = TtxSeq;
	pVdpReg->DATE_COEFF0.u32 = DATE_COEFF0.u32;
    return;
}

HI_VOID SDATE_VBI_TtxLineEnable(HI_U32 WhichRow, HI_U32 WhichExtRow)
{
    //VDP_VBI_TtxLineEnable
	pVdpReg->DATE_COEFF7.u32 = WhichRow;

	pVdpReg->DATE_COEFF20.u32  = WhichExtRow;

    return;
}

HI_VOID SDATE_VBI_TtxSetAddr(HI_U32 StartAddr, HI_U32 EndAddr)
{
    //VDP_VBI_TtxSetAddr
	U_DATE_COEFF8 DATE_COEFF8;
    U_DATE_COEFF9 DATE_COEFF9;

	DATE_COEFF8.u32 = pVdpReg->DATE_COEFF0.u32;
    DATE_COEFF8.bits.tt_staddr = StartAddr;
	pVdpReg->DATE_COEFF8.u32 = DATE_COEFF8.u32;


	DATE_COEFF9.u32 = pVdpReg->DATE_COEFF9.u32;
    DATE_COEFF9.bits.tt_edaddr = EndAddr;
	pVdpReg->DATE_COEFF9.u32 = DATE_COEFF9.u32;
    return;
}

HI_VOID SDATE_VBI_TtxSetReady(HI_VOID)
{
    //VDP_VBI_TtxSetReady
	U_DATE_COEFF10   TtxConfig;

    TtxConfig.u32 = pVdpReg->DATE_COEFF10.u32;
    TtxConfig.bits.tt_ready = 1;
    pVdpReg->DATE_COEFF10.u32 = TtxConfig.u32;
    return;
}

HI_VOID SDATE_VBI_TtxSetPackOff(HI_U32 PackOff)
{
    //VDP_VBI_TtxSetPackOff
	U_DATE_COEFF10  TtxConfig;

    TtxConfig.u32 = pVdpReg->DATE_COEFF10.u32;
    TtxConfig.bits.tt_pktoff = PackOff;
    pVdpReg->DATE_COEFF10.u32 = TtxConfig.u32;
    return;
}

HI_VOID SDATE_VBI_TtxPiorityHighest(HI_BOOL Highest)
{
    //VDP_VBI_TtxPiorityHighest
	U_DATE_COEFF10   TtxConfig;

    TtxConfig.u32 = pVdpReg->DATE_COEFF10.u32;
    TtxConfig.bits.tt_highest = !Highest;
    pVdpReg->DATE_COEFF10.u32 = TtxConfig.u32;
    return;
}

HI_VOID SDATE_VBI_SetTtxFullPage(HI_BOOL Flag)
{
    //VDP_VBI_SetTtxFullPage
	U_DATE_COEFF10   TtxConfig;

    TtxConfig.u32 = pVdpReg->DATE_COEFF10.u32;
    TtxConfig.bits.full_page = Flag;
    pVdpReg->DATE_COEFF10.u32 = TtxConfig.u32;
    return;
}

HI_VOID SDATE_VBI_VbiFilterEnable(HI_BOOL VbiFilterEnable)
{
	//VDP_VBI_VbiFilterEnable
	U_DATE_COEFF0		DATE_COEFF0;

	DATE_COEFF0.u32 = pVdpReg->DATE_COEFF0.u32;
	DATE_COEFF0.bits.vbi_lpf_en = VbiFilterEnable;
	pVdpReg->DATE_COEFF0.u32 = DATE_COEFF0.u32;
	return;
}

HI_VOID SDATE_VBI_TtxEnableInt(HI_VOID)
{
    //VDP_VBI_TtxEnableInt
	U_DATE_ISRMASK DATE_ISRMASK;

    DATE_ISRMASK.u32 = pVdpReg->DATE_ISRMASK.u32;
    DATE_ISRMASK.bits.tt_mask = 0;
    pVdpReg->DATE_ISRMASK.u32 = DATE_ISRMASK.u32;
    return;
}

HI_VOID SDATE_VBI_TtxDisableInt(HI_VOID)
{
    //VDP_VBI_TtxDisableInt
	U_DATE_ISRMASK DATE_ISRMASK;

    DATE_ISRMASK.u32 = pVdpReg->DATE_ISRMASK.u32;
    DATE_ISRMASK.bits.tt_mask = 1;
    pVdpReg->DATE_ISRMASK.u32 = DATE_ISRMASK.u32;

    return;
}

HI_VOID SDATE_VBI_CCSetSeq(HI_BOOL CCSeq)
{
	//VDP_VBI_CCSetSeq
	U_DATE_COEFF1  DATE_COEFF1;

	DATE_COEFF1.u32 = pVdpReg->DATE_COEFF1.u32;
	DATE_COEFF1.bits.cc_seq = CCSeq;
	pVdpReg->DATE_COEFF1.u32 = DATE_COEFF1.u32;
    return;
}

HI_VOID SDATE_HDATE_SetSrcCoef(DISP_VENC_E eDate, HI_U32 *pu32Src13)
{
    //VDP_DATE_SetSrcCoef
	HI_U32 *pu32Src13tmp = pu32Src13;

    if (DISP_VENC_HDATE0 == eDate)
    {
#ifdef __VDP_SUPPORT_HDATE
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF1.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF2.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF3.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF4.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF5.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF6.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF7.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF8.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF9.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF10.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF11.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF12.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF13.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF14.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF15.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF16.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF17.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->HDATE_SRC_13_COEF18.u32), *pu32Src13tmp++);
#endif
    }
    else if (DISP_VENC_SDATE0 == eDate)
    {
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF50.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF51.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF52.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF53.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF54.u32), *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF55.u32), *pu32Src13tmp++);

        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF55.u32) + 4,   *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF55.u32) + 8,   *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF55.u32) + 0xc, *pu32Src13tmp++);
        VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF55.u32) + 0x10,*pu32Src13tmp++);

        
    }
}

HI_BOOL  SDATE_SetIreEn(HI_U32 u32Data)
{
	volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32));
    DATE_COEFF0.bits.dis_ire = u32Data;
    VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32), DATE_COEFF0.u32);

    return HI_TRUE;
}

HI_VOID SDATE_SetDACDetEn(DISP_VENC_E enDate, HI_U32 enable)
{
    //VDP_DATE_SetDACDetEn
	return;
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
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));

            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 0;/*YCbCr£­>YPbPr*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 1;/*YPbPr insight sync*/
            HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;

            DATE_COEFF0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32));
            /*0: yuv   1: rgb*/
            DATE_COEFF0.bits.rgb_en = 0;/* yuv*/

            DATE_COEFF0.bits.sync_mode_sel =   0;
            /*1:no sync
                0:sync_mode_sel is
            */
            DATE_COEFF0.bits.sync_mode_scart = 0;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
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
            HDATE_VIDEO_FORMAT.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_VIDEO_FORMAT.u32));
            HDATE_VIDEO_FORMAT.bits.csc_ctrl = 1;/*YCbCr¡ª>RGB ITU-R BT709*/
            HDATE_VIDEO_FORMAT.bits.video_out_ctrl = 0;/*RGB sync in*/

            if (!bRGBSync)
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 0; /*no sync*/
            else
                HDATE_VIDEO_FORMAT.bits.sync_add_ctrl = 2; /*default : sync on G/Y */
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);
        }
        else if (DISP_VENC_SDATE0 == eDate )
        {
            U_DATE_COEFF0 DATE_COEFF0;
            DATE_COEFF0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32));
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
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

        }
    }
}

HI_BOOL  SDATE_Setc_gain(HI_U32 u32Data)
{
    volatile U_DATE_COEFF1 DATE_COEFF1;

    DATE_COEFF1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DATE_COEFF1.u32)));
    DATE_COEFF1.bits.c_gain = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF1.u32)), DATE_COEFF1.u32);

    return HI_TRUE;
}

HI_VOID SDATE_SetHTiming(HI_U32 u32Chan, HI_U32 *pu32SrcCoef)
{
    //VDP_DATE_SetHTiming
	if ((u32Chan >1 ) ||(HI_NULL == pu32SrcCoef) )
        return;
    
    /* rwzb need,  we do date adjust instead of csc.*/
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF24.u32), *pu32SrcCoef++);    
    //VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF61.u32), *pu32SrcCoef++);
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF66.u32), *pu32SrcCoef++);
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF67.u32), *pu32SrcCoef++);
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF68.u32), *pu32SrcCoef++);
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF69.u32), *pu32SrcCoef++);
   // VDP_RegWrite((HI_ULONG)&(pVdpReg->DATE_COEFF70.u32), *pu32SrcCoef++);
	
	//VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF84.u32) + 6*4), *pu32SrcCoef++);
	//VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF84.u32) + 7*4), *pu32SrcCoef++);
}

/* set length u32Data of one line */
HI_BOOL  SDATE_SetLenMode(HI_U32 u32Data)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)));
    DATE_COEFF0.bits.length_sel = u32Data;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);

    return HI_TRUE;
}


HI_U32 u32SDDATESrcHTimingCoef[][10]=
{
    /* NTSC  HTiming*/
    {0x0f12c00 ,  0x1 ,  0xf ,   0x35a ,  0x40 ,  0x14,0x2ca, 0x80001b, 0x10000e5},        
   /* PAL  HTiming*/
    {0x00f12C99 ,  0x1 ,  0 ,   0 ,  0 ,  0,0, 0x80001b, 0x10000e5},
};

HI_VOID SDATE_HDATE_ResetFmt(DISP_VENC_E eDate, HI_DRV_DISP_FMT_E enFmt)
{
    //VDP_DATE_ResetFmt
	if (DISP_VENC_HDATE0 == eDate)
    {
		#ifdef __VDP_SUPPORT_HDATE
        U_HDATE_VIDEO_FORMAT HDATE_VIDEO_FORMAT;
        U_HDATE_OUT_CTRL     HDATE_OUT_CTRL;

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x03ul);
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)), 0x02000ul);

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
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x05ul);
            case HI_DRV_DISP_FMT_1080i_60:
            case HI_DRV_DISP_FMT_1080i_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a4;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_1080I));
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x05ul);
                break;
            case HI_DRV_DISP_FMT_720P_60:
            case HI_DRV_DISP_FMT_720P_50:
            case HI_DRV_DISP_FMT_720P_59_94:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a2;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_720P));
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x01ul);
                break;
            case HI_DRV_DISP_FMT_576P_50:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a1;
                SDATE_HDATE_SetSrcCoef(eDate, VDP_Get_HDateSrc13Coef(DISP_DATE_SRC_COEF_INDEX_576P));
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_POLA_CTRL.u32)), 0x01ul);
                break;
            case HI_DRV_DISP_FMT_480P_60:
                HDATE_VIDEO_FORMAT.u32 = 0x000000a0;
                /* to resolve rwzb for 480p vsync */
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)), 0x06000ul);
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
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_VIDEO_FORMAT.u32)), HDATE_VIDEO_FORMAT.u32);


        HDATE_OUT_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->HDATE_OUT_CTRL.u32));

        HDATE_OUT_CTRL.bits.src_ctrl = 2;
        HDATE_OUT_CTRL.bits.sync_lpf_en = 1;

        //HDATE_OUT_CTRL.u32 = HDATE_OUT_CTRL.u32 | 0x1000;
        HDATE_OUT_CTRL.bits.video1_sel = 2;
        HDATE_OUT_CTRL.bits.video2_sel = 3;
        HDATE_OUT_CTRL.bits.video3_sel = 1;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_OUT_CTRL.u32)), HDATE_OUT_CTRL.u32);


        // DTS2012080100525 : some sony46 TV can not accept 576P50 siganl correctly
        {
            U_HDATE_CLIP HDATE_CLIP;  // new add

            HDATE_CLIP.u32 = 0;

            // 20 x clk_pix + 4 x clk_dis, now clk_pix = 2*clk_dis for HDATE_OUT_CTRL.bits.src_ctrl = 2
            HDATE_CLIP.bits.clip_bb = 80;
            HDATE_CLIP.bits.clip_fb = 8;
            HDATE_CLIP.bits.clip_disable = 0;
            HDATE_CLIP.bits.clip_thdl = 251;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->HDATE_CLIP.u32)), HDATE_CLIP.u32);
        }
		
		#endif
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

        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
        SDATE_Setc_gain(C_GAIN);
		
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
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF22.u32)), SCH_PHASE);


        DATE_COEFF21.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF21.u32));
        DATE_COEFF21.bits.dac0_in_sel = 1;
        DATE_COEFF21.bits.dac1_in_sel = 2;
        DATE_COEFF21.bits.dac2_in_sel = 4;
        DATE_COEFF21.bits.dac3_in_sel = 3;
        DATE_COEFF21.bits.dac4_in_sel = 5;
        DATE_COEFF21.bits.dac5_in_sel = 6;
        VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF21.u32)), DATE_COEFF21.u32);

    }
}

HI_VOID SDATE_SetDACDET(DISP_VENC_E enDate,HI_U32 u32DetLine,HI_U32 u32DetPixelSta)
{
    //VDP_DATE_SetDACDET
	return ;
}


static HI_U32 s_VDACLink[3][HI_DRV_DISP_VDAC_SIGNAL_BUTT] =
{/*0  1  2  3  4     5  6  7  8  9    10*/
    {4, 4, 4, 5, 6,    4, 4, 4, 5, 6,    4},/*HDATE0*/
    {0, 0, 1, 2, 3,    0, 0, 1, 2, 3,    0},/*SDATE0*/
    {7, 7, 7, 7, 7,    7, 7, 7, 8, 9,    7},/*VGA0*/
};

HI_VOID SDATE_VDAC_SetLink(DISP_VENC_E eDate, HI_U32 uVdac, HI_DRV_DISP_VDAC_SIGNAL_E signal)
{
    //VDP_VDAC_SetLink
	U_VO_MUX_DAC VO_MUX_DAC;

    //HI_PRINT("VDP_VDAC_SetLink date=%d, dac=%d, sig=%d\n", eDate, uVdac, signal);

    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);

    VO_MUX_DAC.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_MUX_DAC.u32));
    switch (uVdac)
    {
        case 0:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC0_MASK_RESET;
            VO_MUX_DAC.bits.dac0_sel = s_VDACLink[eDate][signal];
            break;
        case 1:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC1_MASK_RESET;
            VO_MUX_DAC.bits.dac1_sel = s_VDACLink[eDate][signal];
            break;

        case 2:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC2_MASK_RESET;
            VO_MUX_DAC.bits.dac2_sel = s_VDACLink[eDate][signal];
            break;

        case 3:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC3_MASK_RESET;
            VO_MUX_DAC.bits.dac3_sel = s_VDACLink[eDate][signal];
            break;

        default :
            return;
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_MUX_DAC.u32)), VO_MUX_DAC.u32);

    //TODO SELECT CLK
    return;
}

HI_VOID VDP_VDAC_SetEnable(HI_U32 uVdac, HI_U32 enable,HI_U32 u32DelayMs)
	{
		U_VO_DAC_C_CTRL VO_DAC_C_CTRL;

	
		// set vdac crg reset state
		VDP_VDAC_SetReset(uVdac, HI_TRUE);
	
		VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32));
		VO_DAC_C_CTRL.bits.dac_en = 1;
		VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32)), VO_DAC_C_CTRL.u32);

	
		if (enable)
		{
			U_VO_DAC_CTRL0 VO_DAC_CTRL0;
			VDP_VDAC_SetReset(uVdac, HI_FALSE);
			
			VO_DAC_CTRL0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_CTRL0.u32));
			VO_DAC_CTRL0.bits.enctr =  1;
			VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_CTRL0.u32)), VO_DAC_CTRL0.u32);
	   }
	
		return;
	}


HI_VOID SDATE_HDATE_VBI_SetCgmsTypeAEn(DISP_VENC_E enChanel,HI_BOOL bEnTypeA)
{
	//VDP_VBI_SetCgmsTypeAEn
	U_HDATE_VBI_CTRL HDATE_VBI_CTRL; 

	U_DATE_COEFF13 DATE_COEFF13;

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
		HDATE_VBI_CTRL.bits.cgmsa_add_en = bEnTypeA;
		pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF13.u32 = pVdpReg->DATE_COEFF13.u32;
		DATE_COEFF13.bits.cg_enf1 = bEnTypeA;
		DATE_COEFF13.bits.cg_enf2 = bEnTypeA;
		pVdpReg->DATE_COEFF13.u32 = DATE_COEFF13.u32;
	}
	else
	{

	}
}
HI_VOID SDATE_HDATE_VBI_SetCgmsTypeA(DISP_VENC_E enChanel,HI_U32 u32Data)
{
	U_HDATE_CGMSA_DATA HDATE_CGMSA_DATA;

	U_DATE_COEFF13	DATE_COEFF13;

	U_DATE_COEFF14	DATE_COEFF14;

	SDATE_HDATE_VBI_SetCgmsTypeAEn(enChanel,HI_FALSE);

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_CGMSA_DATA.u32 = pVdpReg->HDATE_CGMSA_DATA.u32;
		HDATE_CGMSA_DATA.bits.cgmsa_data = u32Data;
		pVdpReg->HDATE_CGMSA_DATA.u32 = HDATE_CGMSA_DATA.u32;
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF13.u32 = pVdpReg->DATE_COEFF13.u32;
		DATE_COEFF13.bits.cg_f1data = u32Data;
		pVdpReg->DATE_COEFF13.u32 = DATE_COEFF13.u32;

		DATE_COEFF14.u32 = pVdpReg->DATE_COEFF14.u32;
		DATE_COEFF14.bits.cg_f2data = u32Data;
		pVdpReg->DATE_COEFF14.u32 = DATE_COEFF14.u32;
	}

	SDATE_HDATE_VBI_SetCgmsTypeAEn(enChanel,HI_TRUE);
}

HI_VOID SDATE_HDATE_VBI_SetWssEn(DISP_VENC_E enChanel,HI_BOOL bEnWss)
{
	U_DATE_COEFF15 DATE_COEFF15;

	if (enChanel == DISP_VENC_HDATE0)
	{
		HI_PRINT("Invalid Wss Chanel %d\n",enChanel);
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF15.u32 = pVdpReg->DATE_COEFF15.u32;
		DATE_COEFF15.bits.wss_en = bEnWss;
		pVdpReg->DATE_COEFF15.u32 = DATE_COEFF15.u32;
	}

	return;
}

HI_VOID SDATE_HDATE_VBI_SetWssData(DISP_VENC_E enChanel,HI_U32 u32Data)
{
	U_DATE_COEFF15 DATE_COEFF15;

	if (enChanel == DISP_VENC_HDATE0)
	{
		HI_PRINT("Invalid Wss Chanel %d\n",enChanel);
	}
	else if (enChanel == DISP_VENC_SDATE0)
	{
		DATE_COEFF15.u32 = pVdpReg->DATE_COEFF15.u32;
		DATE_COEFF15.bits.wss_data = u32Data;
		pVdpReg->DATE_COEFF15.u32 = DATE_COEFF15.u32;
	}

	return;
}


HI_S32 SDATE_MVCN_DATE_SetMcvn(HI_U32 *pu32Value)
{
   //VDP_MVCN_DATE_SetMcvn
	pVdpReg->DATE_COEFF2.u32 = pu32Value[0];
    pVdpReg->DATE_COEFF3.u32 = pu32Value[1];
    pVdpReg->DATE_COEFF4.u32 = pu32Value[2];
    pVdpReg->DATE_COEFF5.u32 = pu32Value[3];
    pVdpReg->DATE_COEFF6.u32 = pu32Value[4];

    return HI_SUCCESS;
}

HI_VOID VDP_VDAC_SetGain(HI_U32 Gain)
{
#if 0
    U_VO_DAC_C_CTRL VO_DAC_C_CTRL;
    U_VO_DAC_R_CTRL VO_DAC_R_CTRL;
    U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);
    switch (uVdac)
    {
        case 3:
            if (enable)
            {
                VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32));
                VO_DAC_C_CTRL.bits.ct_c = Gain;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32)), VO_DAC_C_CTRL.u32);
            }
            else
            {
                VO_DAC_C_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32));
                VO_DAC_C_CTRL.bits.ct_c = 0;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32)), VO_DAC_C_CTRL.u32);
            }
            break;
        case 0:
            if (enable)
            {
                VO_DAC_R_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_R_CTRL.u32));
                VO_DAC_R_CTRL.bits.ct_r = Gain;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_R_CTRL.u32)), VO_DAC_R_CTRL.u32);
            }
            else
            {
                VO_DAC_R_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_R_CTRL.u32));
                VO_DAC_R_CTRL.bits.ct_r = 0;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_R_CTRL.u32)), VO_DAC_R_CTRL.u32);
            }
            break;
        case 1:
            if (enable)
            {
                VO_DAC_G_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_G_CTRL.u32));
                VO_DAC_G_CTRL.bits.ct_g = 1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_G_CTRL.u32)), VO_DAC_G_CTRL.u32);
            }
            else
            {
                VO_DAC_G_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_G_CTRL.u32));
                VO_DAC_G_CTRL.bits.ct_g = 0;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_G_CTRL.u32)), VO_DAC_G_CTRL.u32);
            }
            break;
        case 2:
            if (enable)
            {
                VO_DAC_B_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_B_CTRL.u32));
                VO_DAC_B_CTRL.bits.ct_b = 1;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_B_CTRL.u32)), VO_DAC_B_CTRL.u32);
            }
            else
            {
                VO_DAC_B_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_B_CTRL.u32));
                VO_DAC_B_CTRL.bits.ct_b = 0;
                VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_B_CTRL.u32)), VO_DAC_B_CTRL.u32);
            }
            break;
        default :
            break;
    }

    if (enable)
    {
        VDP_VDAC_SetReset(uVdac, HI_FALSE);
    }
	
#endif

    return;
}

HI_VOID SDATE_SYNC_SetSrcCoef(HI_U32 coef1,HI_U32 coef2,HI_U32 coef3,HI_U32 coef4)
{
#if 0
    volatile U_DATE_COEFF90 DATE_COEFF90;
	volatile U_DATE_COEFF91 DATE_COEFF91;

    DATE_COEFF90.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF90.u32));
    DATE_COEFF90.bits.date_sync_lpf_coef1 = coef1;
    DATE_COEFF90.bits.date_sync_lpf_coef2 = coef2;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF90.u32)), DATE_COEFF90.u32);

	DATE_COEFF91.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF91.u32));
    DATE_COEFF91.bits.date_sync_lpf_coef3 = coef3;
    DATE_COEFF91.bits.date_sync_lpf_coef4 = coef4;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF91.u32)), DATE_COEFF91.u32);
#endif

    return;
}

HI_VOID SDATE_SCH_SetPhaseCoef(HI_U32 coef)
{
    volatile U_DATE_COEFF22 DATE_COEFF22;

    DATE_COEFF22.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF22.u32));
    DATE_COEFF22.bits.video_phase_delta = coef;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF22.u32)), DATE_COEFF22.u32);
    
	return;
}

HI_VOID SDATE_CLIPS_SetEn(HI_U32 clips)
{
	#if 0
    volatile U_DATE_COEFF59 DATE_COEFF59;

    DATE_COEFF59.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF59.u32));
    DATE_COEFF59.u32 = clips;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF59.u32)), DATE_COEFF59.u32);
	#endif
    
	return;
}

HI_VOID SDATE_CVBS_SetGain(HI_U32 cvbs_gain_en,HI_U32 ycvbs_gain,HI_U32 u_gain,HI_U32 v_gain)
{
	
#if 0
    volatile U_DATE_COEFF57 DATE_COEFF57;

    DATE_COEFF57.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF57.u32));
    DATE_COEFF57.bits.cvbs_gain_en = cvbs_gain_en;
	DATE_COEFF57.bits.ycvbs_gain = ycvbs_gain;
	DATE_COEFF57.bits.u_gain = u_gain;
	DATE_COEFF57.bits.v_gain = v_gain;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF57.u32)), DATE_COEFF57.u32);
#endif
    
	return;
}

HI_VOID SDATE_LUMA_SetDelay(HI_U32 delay)
{
    volatile U_DATE_COEFF0 DATE_COEFF0;

    DATE_COEFF0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32));
    DATE_COEFF0.bits.luma_dl = delay;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DATE_COEFF0.u32)), DATE_COEFF0.u32);
    
	return;
}
