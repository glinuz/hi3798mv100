#ifdef HI_DISP_BUILD_FULL
#include <linux/kernel.h>
#endif

#include "vdp_define.h"
#include "hi_reg_common.h"
//#include "hi_reg_vdp.h"
#include "drv_disp_com.h"
#include "vdp_drv_vid.h"
#include "vdp_hal_comm.h"
#include "hal_specific_rwzb.h"
#include "vdp_fpga_define.h"
#include "vdp_software_selfdefine.h"


#define DHD_YUV_TO_HDMI 0


extern  volatile S_VDP_REGS_TYPE *pVdpReg;

static  HI_VOID Local_SyncRegisterOperation(HI_VOID)
{
   #ifndef __DISP_PLATFORM_BOOT__
   mb();
   #endif

   DISP_UDELAY(10);

   return;
}


HI_BOOL VDP_CheckResetFinished(HI_VOID)
{
    U_VDPVERSION1 VDPVERSION1;
    U_VDPVERSION2 VDPVERSION2;

    VDPVERSION1.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDPVERSION1.u32));
    VDPVERSION2.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VDPVERSION2.u32));

    return !((VDPVERSION1.u32 != 0x00000000) || (VDPVERSION2.u32 != 0x00000000));
}

#if 1
#ifndef __DISP_PLATFORM_BOOT__

HI_VOID VDP_VID_SetRegionEnable_sdk(HI_U32 u32Data, HI_U32 u32Num, HI_U32 u32bEnable )
{
    volatile U_V0_16REGIONENL V0_16REGIONENL;

    if (u32Data >= VID_MAX)
    {
        //HI_ERR_DISP("Select Wrong Video Layer ID\n");
        return ;
    }

    if (u32Num >= VID_REGION_NUM)
    {
        //HI_ERR_DISP("Select Wrong region ID\n");
        return ;
    }

    V0_16REGIONENL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));
    if(u32bEnable)
    {
        V0_16REGIONENL.u32 = (1 << u32Num) | V0_16REGIONENL.u32;
    }
    else
    {
        V0_16REGIONENL.u32 = (~(1 << u32Num)) & V0_16REGIONENL.u32;
    }

    if (V0_16REGIONENL.u32)
    {
        VDP_VID_SetLayerEnable(u32Data, 1);
    }
    else
    {
        VDP_VID_SetLayerEnable(u32Data, 0);
    }

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET), V0_16REGIONENL.u32);

    return ;
}
#endif
HI_VOID  VDP_DRIVER_SetVirtualAddr(S_VDP_REGS_TYPE *pstVdpReg)
{
    pVdpReg = (S_VDP_REGS_TYPE *)pstVdpReg;
}


HI_VOID VDP_DISP_GetIntMask (HI_U32 *pu32masktype)
{
    /* Dispaly interrupt mask enable */
    *pu32masktype = VDP_RegRead((HI_ULONG)&(pVdpReg->VOINTMSK.u32));

    return ;
}


HI_VOID VDP_OpenClkAndInitCrgConfig(HI_VOID)
{
	
    U_PERI_CRG54 unTmpValue;
	U_PERI_CRG211 unTmpCRG211Value;

    /*secondly,   enable vdp1 clk.*/
    unTmpValue.u32 = g_pstRegCrg->PERI_CRG54.u32;
    unTmpValue.bits.vo_bus_cken      = 1;
    //unTmpValue.bits.vo_cken          = 1;
    unTmpValue.bits.vo_sd_cken       = 1;
    unTmpValue.bits.vo_sdate_cken    = 1;
    unTmpValue.bits.vo_hd_cken       = 1;
    //unTmpValue.bits.vo_hdate_cken    = 1;
    unTmpValue.bits.vdac_ch0_cken    = 1;
    unTmpValue.bits.vo_sd_clk_sel    = 0;
    unTmpValue.bits.vo_sd_clk_div    = 2;
    unTmpValue.bits.vo_hd_clk_sel    = 1;
    unTmpValue.bits.vo_hd_clk_div    = 0;
    unTmpValue.bits.vdac_ch0_clk_sel = 1;
    unTmpValue.bits.hdmi_clk_sel     = 1;
    unTmpValue.bits.vo_sd_hdmi_clk_sel = 0;
    unTmpValue.bits.vdp_clk_sel        = 0;
    unTmpValue.bits.vo_hd_hdmi_clk_sel = 0;
	unTmpValue.bits.vdp_cfg_cken = 1;
	g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
	
	/*secondly,   enable vdp2 clk.*/
    unTmpCRG211Value.u32 = g_pstRegCrg->PERI_CRG211.u32;
	unTmpCRG211Value.bits.vo_hd_bp_clk_sel = 0;
	unTmpCRG211Value.bits.vdp_ini_cksel0 = 1;
	unTmpCRG211Value.bits.vdp_ini_cksel1 = 1;
	unTmpCRG211Value.bits.vdp_hd_clk_div0 = 1;
	unTmpCRG211Value.bits.vdp_hd_clk_div1 = 1;
	unTmpCRG211Value.bits.vdp_hd_cken = 1;
	unTmpCRG211Value.bits.vdp_v0_cken = 1;
	unTmpCRG211Value.bits.vdp_v1_cken = 1;
	unTmpCRG211Value.bits.vdp_g0_cken = 1;
	unTmpCRG211Value.bits.vdp_g1_cken = 1;
	unTmpCRG211Value.bits.vdp_g3_cken = 1;
	unTmpCRG211Value.bits.vdp_wbc_hd_cken = 1;
	unTmpCRG211Value.bits.vdp_wbc_gp_cken = 1;
	unTmpCRG211Value.bits.vdp_wbc_vp_cken = 1;
	unTmpCRG211Value.bits.vdp_wbc_ref_cken = 1;
	unTmpCRG211Value.bits.vdp_wbc_mad_cken = 1;
	unTmpCRG211Value.bits.vdp_sd_cken = 1;
	unTmpCRG211Value.bits.vdp_v3_cken = 1;
	unTmpCRG211Value.bits.vdp_g4_cken = 1;
	unTmpCRG211Value.bits.vo_bp_cken = 1;
	g_pstRegCrg->PERI_CRG211.u32 = unTmpCRG211Value.u32;
    Local_SyncRegisterOperation();

    /*UnReset CRG*/
    unTmpValue.bits.vou_srst_req = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue.u32;
    Local_SyncRegisterOperation();
    return;
}


HI_VOID VDP_MIXV_SetPrio(HI_U32 prioLayer)
{
    U_MIXV0_MIX          MIXV0_MIX;
    MIXV0_MIX.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32));
    if(prioLayer == VDP_LAYER_VID0)
    {
        MIXV0_MIX.bits.mixer_prio0= 0x1;
    }
    else
    {
        MIXV0_MIX.bits.mixer_prio0= 0x2;
    }
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);
}

HI_VOID VDP_MIXV_SetPrio1(HI_U32 prioLayer)
{
    U_MIXV0_MIX          MIXV0_MIX;
    MIXV0_MIX.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32));
    if(prioLayer == VDP_LAYER_VID0)
    {
        MIXV0_MIX.bits.mixer_prio1= 0x1;
    }
    else
    {
        MIXV0_MIX.bits.mixer_prio1= 0x2;
    }
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)), MIXV0_MIX.u32);

}


HI_VOID VDP_DHD_DEFAULT(HI_VOID)
{
   volatile U_DHD0_CTRL DHD0_CTRL;
   volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
   volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;


    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)), DHD0_SYNC_INV.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)), DHD0_CTRL.u32);
	
    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DATA_SEL.u32)), DHD0_DATA_SEL.u32);
}

HI_VOID VDP_DSD_DEFAULT(HI_VOID)
{
   volatile U_DHD0_CTRL DHD0_CTRL;
   volatile U_DHD0_SYNC_INV DHD0_SYNC_INV;
   volatile U_VO_MUX VO_MUX;
   volatile U_DHD0_DATA_SEL DHD0_DATA_SEL;


    DHD0_SYNC_INV.u32 = 0x2000;
    DHD0_CTRL.u32 = 0x00008011;//0x8d0f8000;//0x8ad20011;
    VO_MUX.u32= 0x1;


    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_SYNC_INV.u32)+ 1*CHN_OFFSET), DHD0_SYNC_INV.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+ 1*CHN_OFFSET), DHD0_CTRL.u32);
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_MUX.u32)), VO_MUX.u32);
	
    DHD0_DATA_SEL.u32 = 0;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_DATA_SEL.u32) + 1*CHN_OFFSET), DHD0_DATA_SEL.u32);

}


HI_VOID  VDP_DHD1_CBM_MIX_Init(HI_VOID)
{

	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID3, 0);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID4, 1);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_GP1, 2);

}

HI_VOID  VDP_DHD1_CBM_MIX_DeInit(HI_VOID)
{
	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, 0, 0);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, 0, 1);
	VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, 0, 2);
}

HI_VOID  VDP_DispMask(HI_DRV_DISPLAY_E eChn)
{
	if ( eChn == HI_DRV_DISPLAY_0)
	{
		VDP_DHD1_CBM_MIX_DeInit();
	}
	else
	{
		DISP_ERROR("chan %d,Disp Mask not support !\n",eChn);

	}
}

HI_VOID  VDP_DispUnMask(HI_DRV_DISPLAY_E eChn)
{

	if ( eChn == HI_DRV_DISPLAY_0)
	{
		VDP_DHD1_CBM_MIX_Init();
	}
	else
	{
		DISP_ERROR("chan %d,Disp Mask not support !\n",eChn);
	}

    return;
}


HI_VOID  VDP_DRIVER_Initial(HI_VOID)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
	VDP_BKG_S stBkg;
    memset((void*)&stBkg, 0, sizeof(VDP_BKG_S));

	/*mv410 has only one master, support  rd muliti rd. 
	   and  outstanding can be set  up to 15.*/
    VDP_SetRdOutStd(VDP_MASTER0, 0, 0xf);
    VDP_SetRdOutStd(VDP_MASTER0, 1, 0xf);
    VDP_SetWrOutStd(VDP_MASTER0, 0, 0x7);

    VDP_SetRdOutStd(VDP_MASTER1, 0, 0xf);
    VDP_SetRdOutStd(VDP_MASTER1, 1, 0xf);
    VDP_SetWrOutStd(VDP_MASTER1, 0, 7);

    VDP_SetRdOutStd(VDP_MASTER2, 0, 0xf);
    VDP_SetRdOutStd(VDP_MASTER2, 1, 0xf);
    VDP_SetWrOutStd(VDP_MASTER2, 0, 7);

    VDP_SetRdOutStd(VDP_MASTER3, 0, 0xf);
    VDP_SetRdOutStd(VDP_MASTER3, 1, 0xf);
    VDP_SetWrOutStd(VDP_MASTER3, 0, 7);
	
	VDP_SetRdMultiIdEnable(0, 1);
	
	VDP_SetAxiMidEnable(HI_TRUE);
	
	VDP_VID_SetReqCtrl(VDP_LAYER_VID0,3);
	VDP_VID_SetReqCtrl(VDP_LAYER_VID1,2);
	VDP_GFX_SetReqCtrl(VDP_LAYER_GFX0,3);
	VDP_VID_SetPreReadEnable(VDP_LAYER_VID0,HI_TRUE);
	VDP_VID_SetPreReadEnable(VDP_LAYER_VID1,HI_TRUE);

	VDP_SetTwoChnMode(1);
	VDP_SetTwoChnEnable(HI_TRUE);
	
    /*3rd: about cbm*/
    VDP_CBM_SetMixerBkg(VDP_CBM_MIXV0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIXG0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX0,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX1,stBkg);
    VDP_CBM_SetMixerBkg(VDP_CBM_MIX2,stBkg);
	
    /* initail video mixer */
    VDP_CBM_Clear_MixvPrio(VDP_LAYER_VID0);
    VDP_CBM_Clear_MixvPrio(VDP_LAYER_VID1);

    CBM_MIX1.u32 = pVdpReg->CBM_MIX1.u32;
    CBM_MIX1.bits.mixer_prio1 = 0; /* no layer */
    CBM_MIX1.bits.mixer_prio0 = 0; /* no layer */
    pVdpReg->CBM_MIX1.u32 = CBM_MIX1.u32;
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_VP0, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX0, VDP_CBM_GP0, 1);

    CBM_MIX2.u32 = pVdpReg->CBM_MIX2.u32;
    CBM_MIX2.bits.mixer_prio1 = 0; /* no layer */
    CBM_MIX2.bits.mixer_prio0 = 0; /* no layer */
    pVdpReg->CBM_MIX2.u32 = CBM_MIX2.u32;
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX1, VDP_CBM_VID3, 0);
	
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID3, 0);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_VID4, 1);
    VDP_CBM_SetMixerPrio(VDP_CBM_MIX3, VDP_CBM_GP1, 2);

    //VDP_INTF_DEFAULT();
    VDP_DHD_DEFAULT();
    VDP_DSD_DEFAULT();

    return;
}

HI_VOID VDP_VDAC_ResetCRG(HI_VOID)
{

    U_PERI_CRG71 PERI_CRG71TMP;
	
    PERI_CRG71TMP.u32 = g_pstRegCrg->PERI_CRG71.u32;

	/*close dac clk*/
    PERI_CRG71TMP.bits.vdac_chop_cken = 0;
    PERI_CRG71TMP.bits.vdac_0_clk_pctrl = 0;
    g_pstRegCrg->PERI_CRG71.u32 = PERI_CRG71TMP.u32;
}



HI_VOID VDP_VDAC_Reset(HI_VOID)
{

	U_VO_DAC_CTRL0   VO_DAC_CTRL0;
	VO_DAC_CTRL0.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->VO_DAC_CTRL0.u32));

	/*Set dac ctrl default value */
	VO_DAC_CTRL0.bits.enctr = HI_TRUE;
	VO_DAC_CTRL0.bits.enextref = HI_TRUE;
	VO_DAC_CTRL0.bits.pdchopper = HI_FALSE;
	VO_DAC_CTRL0.bits.envbg = HI_TRUE;

	VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_CTRL0.u32)),VO_DAC_CTRL0.u32);
    DISP_MSLEEP(1);

    VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_MUX_DAC.u32)), 0);
    DISP_WARN("=========VDAC_DRIVER_Initial====\n");

    return;
}

HI_VOID VDP_CloseClkResetModule(HI_VOID)
{
    U_PERI_CRG54 unTmpValue1;
	U_PERI_CRG211 unTmpCRG211Value;
    HI_U32 i = 0;

    unTmpValue1.u32 = g_pstRegCrg->PERI_CRG54.u32;
    unTmpValue1.bits.vou_srst_req = 1;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;
    Local_SyncRegisterOperation();

    while(HI_FALSE == VDP_CheckResetFinished())
    {
        DISP_MSLEEP(2);
        i ++;

        if (i > 40)
        {
		    DISP_ERROR("=========VDP reset timeout====\n");
            break;
        }
    }

	/*close vdp clk*/
	g_pstRegCrg->PERI_CRG71.bits.vdac_chop_cken = 0;
	
    unTmpValue1.u32 = g_pstRegCrg->PERI_CRG54.u32;
    unTmpValue1.bits.vdac_ch0_cken = 0;
    //unTmpValue1.bits.vo_hdate_cken = 0;
    unTmpValue1.bits.vo_hd_cken = 0;
    unTmpValue1.bits.vo_sdate_cken = 0;
    unTmpValue1.bits.vo_sd_cken = 0;
    //unTmpValue1.bits.vo_cken = 0;
    unTmpValue1.bits.vo_bus_cken = 0;
	unTmpValue1.bits.vdp_cfg_cken = 0;
    g_pstRegCrg->PERI_CRG54.u32 = unTmpValue1.u32;
    Local_SyncRegisterOperation();

    /*close clk gate*/
    unTmpCRG211Value.u32 = g_pstRegCrg->PERI_CRG211.u32;
    unTmpCRG211Value.bits.vdp_ini_cksel0 = 0;
	unTmpCRG211Value.bits.vdp_ini_cksel1 = 0;
	unTmpCRG211Value.bits.vdp_hd_clk_div0 = 0;
	unTmpCRG211Value.bits.vdp_hd_clk_div1 = 0;
	unTmpCRG211Value.bits.vdp_hd_cken = 0;
	unTmpCRG211Value.bits.vdp_v0_cken = 0;
	unTmpCRG211Value.bits.vdp_v1_cken = 0;
	unTmpCRG211Value.bits.vdp_g0_cken = 0;
	unTmpCRG211Value.bits.vdp_g1_cken = 0;
	unTmpCRG211Value.bits.vdp_g3_cken = 0;
	unTmpCRG211Value.bits.vdp_wbc_hd_cken = 0;
	unTmpCRG211Value.bits.vdp_wbc_gp_cken = 0;
	unTmpCRG211Value.bits.vdp_wbc_vp_cken = 0;
	unTmpCRG211Value.bits.vdp_wbc_ref_cken = 0;
	unTmpCRG211Value.bits.vdp_wbc_mad_cken = 0;
	unTmpCRG211Value.bits.vdp_sd_cken = 0;
	unTmpCRG211Value.bits.vdp_v3_cken = 0;
	unTmpCRG211Value.bits.vdp_g4_cken = 0;
	unTmpCRG211Value.bits.vo_bp_cken = 0;
    g_pstRegCrg->PERI_CRG211.u32 = unTmpCRG211Value.u32;
    Local_SyncRegisterOperation();

    return ;
}


HI_VOID VDP_DHD_Reset(HI_U32 u32hd_id)
{
    U_DHD0_CTRL DHD0_CTRL;

/*
    struct
    {
        unsigned int    regup                 : 1   ; // [0]
        unsigned int    Reserved_330          : 2   ; // [2..1]
        unsigned int    fp_en                 : 1   ; // [3]
        unsigned int    iop                   : 1   ; // [4]
        unsigned int    Reserved_329          : 7   ; // [11..5]
        unsigned int    gmm_mode              : 1   ; // [12]
        unsigned int    gmm_en                : 1   ; // [13]
        unsigned int    hdmi_mode             : 1   ; // [14]
        unsigned int    Reserved_328          : 5   ; // [19..15]
        unsigned int    fpga_lmt_width        : 7   ; // [26..20]
        unsigned int    fpga_lmt_en           : 1   ; // [27]
        unsigned int    Reserved_327          : 1   ; // [28]
        unsigned int    cbar_sel              : 1   ; // [29]
        unsigned int    cbar_en               : 1   ; // [30]
        unsigned int    intf_en               : 1   ; // [31]
    } bits;
*/
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    DHD0_CTRL.bits.intf_en = 0;
    DHD0_CTRL.bits.cbar_en = 0;
    DHD0_CTRL.bits.fpga_lmt_en = 0;
    DHD0_CTRL.bits.hdmi_mode = DHD_YUV_TO_HDMI;
    DHD0_CTRL.bits.gmm_en = 0;
    DHD0_CTRL.bits.disp_mode = 0;
    DHD0_CTRL.bits.regup = 1;
    VDP_RegWrite(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET),DHD0_CTRL.u32);
}


HI_VOID VDP_VDAC_GetEnable(HI_U32 uVdac, HI_U32 *penable)
{
    return;
}


#define DISP_FASTBOOTUP_FLAG       0xabcd0000
extern HI_U32 Disp_GetFastbootupFlag(HI_VOID);
HI_VOID VDP_RegReStore(HI_U8 *pu8RegBackAddr)
{

    S_VDP_REGS_TYPE  *pVdpBackReg;

    pVdpBackReg = (S_VDP_REGS_TYPE *)pu8RegBackAddr;
    /*because we will call DISP_ISR_Resume to resume the isr,se here we do not restore it.*/
    pVdpBackReg->VOINTMSK.u32 = 0;

	/*smmu */
	/*F000~F600*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->SMMU_SCR.u32)), (HI_VOID *)(&(pVdpBackReg->SMMU_SCR.u32)), 0x530);

    /* save Reg */
	/*0000~0150*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->VOCTRL.u32)), (HI_VOID *)(&(pVdpBackReg->VOCTRL.u32)), 0x100*2);
	/*0300~0400*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->WBC_DHD_LOCATE.u32)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD_LOCATE.u32)), 0x100);
	/*0400~0500*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->COEF_DATA.u32)), (HI_VOID *)(&(pVdpBackReg->COEF_DATA.u32)), 0x100);
	
    /*video layer */
	/*0800~3000*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->V0_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->V0_CTRL.u32)), 0x800*5);

	/*HDR process*/
	/*3000~3fff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->COMPOSER_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->COMPOSER_CTRL.u32)), 0xfff);
	
    /*VP0 VP1*/
	/*6000~6500*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->VP0_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->VP0_CTRL.u32)), 0x500);

    /*DWBC0*/
	/*9000~93ff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->WBC_DHD0_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->WBC_DHD0_CTRL.u32)), 0x3ff);
	
	/*TNR*/
	/*a000~a900*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->WBC_ME_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->WBC_ME_CTRL.u32)), 0x900);

    /*MIXV*/
	/*b000~b100*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->MIXV0_BKG.u32)), (HI_VOID *)(&(pVdpBackReg->MIXV0_BKG.u32)), 0x100);
	
	/*CBM*/
	/*b400~b700*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->CBM_BKG1.u32)), (HI_VOID *)(&(pVdpBackReg->CBM_BKG1.u32)), 0x300);

	/*SR*/
	/*b800~bbff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->SR_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->SR_CTRL.u32)), 0x300);

	/*warmark*/
	/*bc00~bcff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->V0_DWM_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->V0_DWM_CTRL.u32)), 0x100);

    /*DHDx*/
	/*c000~c800*/
	  if (Disp_GetFastbootupFlag() !=  DISP_FASTBOOTUP_FLAG)
    {
    	MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->DHD0_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->DHD0_CTRL.u32)), 0x400*2);
	}

    /*DATE*/
	/*cc00~cf00*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->HDATE_VERSION.u32)), (HI_VOID *)(&(pVdpBackReg->HDATE_VERSION.u32)), 0x2dc);
	/*other */
	/*d000~d6ff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->HDMI_BUF_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->HDATE_VERSION.u32)), 0x6ff);

	/*DB,DR SNR */
	/*d800~efff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpReg->V0_DB_CTRL.u32)), (HI_VOID *)(&(pVdpBackReg->V0_DB_CTRL.u32)), 0x1800);


}

HI_VOID VDP_RegSave(HI_U8 *pu8RegBackAddr)
{

    S_VDP_REGS_TYPE  *pVdpBackReg;
    pVdpBackReg = (S_VDP_REGS_TYPE *)pu8RegBackAddr;

	/*smmu */
	/*F000~F600*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->SMMU_SCR.u32)), (HI_VOID *)(&(pVdpReg->SMMU_SCR.u32)), 0x530);

    /* save Reg */
	/*0000~0150*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->VOCTRL.u32)), (HI_VOID *)(&(pVdpReg->VOCTRL.u32)), 0x100*2);
	/*0300~0400*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->WBC_DHD_LOCATE.u32)), (HI_VOID *)(&(pVdpReg->WBC_DHD_LOCATE.u32)), 0x100);
	/*0400~0500*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->COEF_DATA.u32)), (HI_VOID *)(&(pVdpReg->COEF_DATA.u32)), 0x100);
	
    /*video layer */
	/*0800~3000*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->V0_CTRL.u32)), (HI_VOID *)(&(pVdpReg->V0_CTRL.u32)), 0x800*5);

	/*HDR process*/
	/*3000~3fff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->COMPOSER_CTRL.u32)), (HI_VOID *)(&(pVdpReg->COMPOSER_CTRL.u32)), 0xfff);
	
    /*VP0 VP1*/
	/*6000~6500*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->VP0_CTRL.u32)), (HI_VOID *)(&(pVdpReg->VP0_CTRL.u32)), 0x500);

    /*DWBC0*/
	/*9000~93ff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->WBC_DHD0_CTRL.u32)), (HI_VOID *)(&(pVdpReg->WBC_DHD0_CTRL.u32)), 0x3ff);
	
	/*TNR*/
	/*a000~a900*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->WBC_ME_CTRL.u32)), (HI_VOID *)(&(pVdpReg->WBC_ME_CTRL.u32)), 0x900);

    /*MIXV*/
	/*b000~b100*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->MIXV0_BKG.u32)), (HI_VOID *)(&(pVdpReg->MIXV0_BKG.u32)), 0x100);
	
	/*CBM*/
	/*b400~b700*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->CBM_BKG1.u32)), (HI_VOID *)(&(pVdpReg->CBM_BKG1.u32)), 0x300);

	/*SR*/
	/*b800~bbff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->SR_CTRL.u32)), (HI_VOID *)(&(pVdpReg->SR_CTRL.u32)), 0x300);

	/*warmark*/
	/*bc00~bcff*/
	MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->V0_DWM_CTRL.u32)), (HI_VOID *)(&(pVdpReg->V0_DWM_CTRL.u32)), 0x100);

    /*DHDx*/
	/*c000~c800*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->DHD0_CTRL.u32)), (HI_VOID *)(&(pVdpReg->DHD0_CTRL.u32)), 0x400*2);

    /*DATE*/
	/*cc00~cf00*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->HDATE_VERSION.u32)), (HI_VOID *)(&(pVdpReg->HDATE_VERSION.u32)), 0x2dc);
	/*other */
	/*d000~d6ff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->HDMI_BUF_CTRL.u32)), (HI_VOID *)(&(pVdpReg->HDATE_VERSION.u32)), 0x6ff);

	/*DB,DR SNR */
	/*d800~efff*/
    MEMCOPY_VDP((HI_VOID *)(&(pVdpBackReg->V0_DB_CTRL.u32)), (HI_VOID *)(&(pVdpReg->V0_DB_CTRL.u32)), 0x1800);

}

HI_VOID VDP_VDAC_SetReset(HI_U32 uVdac, HI_BOOL bReset)
{
#if 0
    U_PERI_CRG71 PERI_CRG71Tmp;

    PERI_CRG71Tmp.u32 = g_pstRegCrg->PERI_CRG71.u32;

    /*pay attention, no reset?*/
#endif

    return;
}

HI_VOID VDP_VDAC_ResetFmt(DISP_VENC_E enVenc ,HI_U32 uVdac, HI_DRV_DISP_FMT_E enFmt,HI_U32 u32CustomPixClk)
{
    // set vdac crg reset state
    VDP_VDAC_SetReset(uVdac, HI_TRUE);

    switch(uVdac)
    {
        case 3:
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_C_CTRL.u32)), VDAC_DEFAULT_VALUE);
            break;

        case 0:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC1_MASK_RESET;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_R_CTRL.u32)), VDAC_DEFAULT_VALUE);
            break;

        case 1:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC2_MASK_RESET;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_G_CTRL.u32)), VDAC_DEFAULT_VALUE);

            break;
        case 2:
            //g_pstRegCrg->PERI_CRG71.u32 = g_pstRegCrg->PERI_CRG71.u32 | X5HD2_VDAC3_MASK_RESET;
            VDP_RegWrite(((HI_ULONG)&(pVdpReg->VO_DAC_B_CTRL.u32)), VDAC_DEFAULT_VALUE);

            break;
        default:
            return;
    }
    return;
}

HI_VOID VDP_DISP_GetIntfEnable(HI_U32 u32hd_id, HI_U32 *pbTrue)
{
    U_DHD0_CTRL DHD0_CTRL;
    if(u32hd_id >= CHN_MAX)
    {
        HI_ERR_DISP("Select Wrong CHANNEL ID\n");
        return ;
    }

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));
    *pbTrue = DHD0_CTRL.bits.intf_en;
}


HI_U32 VDP_DISP_GetMaskIntSta(HI_U32 u32intmask)
{
    U_VOMSKINTSTA VOMSKINTSTA;

    if (!pVdpReg)
    {
        HI_ERR_DISP("pVdpReg is is a null\n");
        VOMSKINTSTA.u32 = 0;
    }
    else
    {
        /* read interrupt status */
        VOMSKINTSTA.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMSKINTSTA.u32)));
    }

    return (VOMSKINTSTA.u32 & u32intmask);
}

HI_VOID VDP_DISP_SetHdmiClk  (HI_U32 u32hd_id, HI_U32 u32hdmi_clkdiv)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    if ( u32hd_id)
    {
        /*SD*/

        /*chan select :      0:sd ,1: hd*/
        PERI_CRG54Tmp.bits.hdmi_clk_sel = 0;
    }
    else
    {
        /*chan select :      0:sd ,1: hd*/
        PERI_CRG54Tmp.bits.hdmi_clk_sel = 1;
    }
	
    g_pstRegCrg->PERI_CRG54.u32= PERI_CRG54Tmp.u32;
}


HI_VOID VDP_VDAC_SetClockEnable(HI_U32 uVdac, HI_U32 enable)
{

    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    switch(uVdac)
    {
        case 0:
            PERI_CRG54Tmp.bits.vdac_ch0_cken = (enable == HI_TRUE)? 1 : 0;
            break;

        default:
            break;
    }

    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;


    return;
}

HI_VOID  VDP_WBC_SetZmeInFmt_Define1(VDP_LAYER_WBC_E enLayer, VDP_PROC_FMT_E u32Fmt)
{
	#if 0
    U_V0_VSP V0_VSP;
    U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;
    WBC_DHD0_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32));
    
    if(WBC_DHD0_CTRL.bits.v3_zme_sel != VDP_WBC_ZME_USE_WBC_VP && (enLayer == VDP_LAYER_WBC_HD0))
    {
        HI_ERR_DISP("Select Wrong Video Layer ID\n");
        return ;
    }

    if(enLayer == VDP_LAYER_WBC_HD0 )
    {
        V0_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VDP_LAYER_VID3 * VID_OFFSET));
        V0_VSP.bits.zme_in_fmt = u32Fmt;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VDP_LAYER_VID3 * VID_OFFSET, V0_VSP.u32); 
#if 0
        WBC_DHD0_ZME_VSP.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_VSP.u32));
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = u32Fmt;
        VDP_RegWrite((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_VSP.u32), WBC_DHD0_ZME_VSP.u32); 
#endif
    }

    #endif
    return ;
}

HI_U32 VDP_Disp_GetVdpVersion(HI_U32 *pu32VersionL,HI_U32 *pu32VersionH)
{
    *pu32VersionL =  0x76756F76UL;
    *pu32VersionH =  0x30313134UL;
   return HI_SUCCESS;
}

HI_VOID VDP_SelectClk(HI_U32 u32VDPClkMode)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;
    PERI_CRG54Tmp.bits.vdp_clk_sel = u32VDPClkMode;
    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;
}



HI_S32 VDP_DISP_SelectChanClkDiv(HI_DRV_DISPLAY_E eChn, HI_U32 u32Div)
{
    U_PERI_CRG54 PERI_CRG54Tmp;

    PERI_CRG54Tmp.u32 = g_pstRegCrg->PERI_CRG54.u32;

    if (HI_DRV_DISPLAY_1 == eChn)
    {
        PERI_CRG54Tmp.bits.vo_hd_clk_div = u32Div;
    }
    else
    {
        PERI_CRG54Tmp.bits.vo_sd_clk_div = u32Div;
    }

    g_pstRegCrg->PERI_CRG54.u32 = PERI_CRG54Tmp.u32;

    return HI_SUCCESS;
}

HI_VOID VDP_VBI_SetCgmsTypeBEn(DISP_VENC_E enChanel, HI_BOOL bEnTypeB)
{
	U_HDATE_VBI_CTRL HDATE_VBI_CTRL; 

	if (enChanel == DISP_VENC_HDATE0)
	{
		HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
		HDATE_VBI_CTRL.bits.cgmsb_add_en = bEnTypeB;
		pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
	}
	else
	{
		HI_ERR_DISP("enChanel=%d\n", enChanel);
	}
}

HI_VOID VDP_MVCN_SetMcvnEnable(HI_BOOL bEnable)
{
    U_HDATE_VBI_CTRL HDATE_VBI_CTRL;

    HDATE_VBI_CTRL.u32 = pVdpReg->HDATE_VBI_CTRL.u32;
    HDATE_VBI_CTRL.bits.mv_en = bEnable;
    pVdpReg->HDATE_VBI_CTRL.u32 = HDATE_VBI_CTRL.u32;
}


HI_U32 VDP_WBC_GetAlwaysProFlag(HI_BOOL *bAlwaysPro)
{
    *bAlwaysPro = 0;
    return HI_SUCCESS;
}


HI_U32 VDP_DHD_GetDispMode(HI_U32 u32Data)
{
    U_DHD0_CTRL DHD0_CTRL;
    DHD0_CTRL.u32 = VDP_RegRead((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32));

    return DHD0_CTRL.bits.iop;
}

HI_VOID VDP_DISP_GetVtThd(HI_U32 u32hd_id, HI_U32 u32uthdnum, HI_U32 *pu32vtthd)
{
    volatile U_DHD0_VTTHD DHD0_VTTHD;
    volatile U_DHD0_VTTHD3 DHD0_VTTHD3;

    if (u32hd_id >= CHN_MAX)
    {
        HI_ERR_DISP("Select Wrong CHANNEL ID\n");
        return ;
    }

    if(u32uthdnum == 1) 
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        *pu32vtthd = DHD0_VTTHD.bits.vtmgthd1;
    }
    else if(u32uthdnum == 2) 
    {
        DHD0_VTTHD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD.u32)+u32hd_id*CHN_OFFSET));
        *pu32vtthd = DHD0_VTTHD.bits.vtmgthd2;             
    }
    else if(u32uthdnum== 3)
    {
        DHD0_VTTHD3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VTTHD3.u32)+u32hd_id*CHN_OFFSET));
        *pu32vtthd = DHD0_VTTHD3.bits.vtmgthd3;
    }
}



#endif
