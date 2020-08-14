#include  "vdp_define.h"
#if EDA_TEST
#include "vdp_assert.h"
#include "vdp_basis_comm.h"
#endif

#if  VDP_CBB_FPGA

#include "vdp_hal_comm.h"
#include "vdp_hal_vid.h"
#include "vdp_hal_gfx.h"
#include "vdp_hal_wbc.h"
#include "vdp_hal_chn.h"
#include "hi_reg_common.h"
#include <linux/fcntl.h>
#include "vdp_assert.h"
#define MMU_ADDR_CHECK_DBG 0

#elif VDP_C_COMPILE

#include "vdp_drv_func.h"
#include "vdp_hal_comm.h"
#include "vdp_hal_vid.h"
#include "vdp_hal_gfx.h"
#include "vdp_hal_wbc.h"
#include "vdp_hal_chn.h"
#include "hi_reg_common.h"

#include <linux/fcntl.h>
#include "vdp_assert.h"
#define MMU_ADDR_CHECK_DBG 0
#endif

HI_S32 s32SignConv(HI_S32 s32Data, HI_S32 u32BitWidth)
{
	HI_S32 s32Result;
	
	if(((s32Data >> (u32BitWidth -1))&0x1)== 1)
	{
		s32Result = ((0xffffffff << u32BitWidth) | s32Data);
	}
	else
	{
		s32Result = s32Data;
	}
	return s32Result;	
}



extern S_VDP_REGS_TYPE *pVdpReg;

#if EDA_TEST
extern double clk_axi_t;
extern double clk_apb_t;
extern double clk_ppc_t;
extern double clk_vo_hd_t;
extern double clk_vo_ad_t;
extern double clk_vo_sd_t;


HI_VOID  VDP_ReadLatencyCalc(HI_U32 u32Enable)
{
#if EDA_TEST
    cout << "VDP_ReadLatencyCalc start work:"  << endl;
#endif
     
    HI_U32 total_latency;

    HI_U32 intf_w;
    HI_U32 axi_freq;

    HI_U32 dhd0_latency;
    HI_U32 dhd0_freq;

    HI_U32 dhd1_latency;
    HI_U32 dhd1_freq;

//     HI_U32 line_data_num = 0;
    HI_U32 vid_line_data_layer = 0;
    HI_U32 gfx_line_data_layer = 0;
    HI_U32 pixcel_num = 3;
    
    HI_U32 line_cycle_num = 0;
    HI_U32 ii;

    HI_U32 dhd_en[2];
    HI_U32 vid_en;
    HI_U32 gfx_en;

    HI_U32 vid_ifmt;
    HI_U32 gfx_ifmt;

//     HI_U32 mrg_mode = 0;

	U_DHD0_CTRL   DHD0_CTRL;
    U_DHD0_HSYNC1 DHD0_HSYNC1;

    U_V0_CTRL        V0_CTRL;
//     U_V0_MULTI_MODE  V0_MULTI_MODE;
    U_V0_IRESO       V0_IRESO;

	U_G0_CTRL  G0_CTRL;
    U_G0_IRESO G0_IRESO;

    HI_U32 vid_iw;
    HI_U32 gfx_iw;

    HI_U32 region_num = 1;
    HI_U32 req_byte = 256;

    HI_U32 vid_data_total = 0;
    HI_U32 gfx_data_total = 0;
    HI_U32 chn_data_total = 0;

    HI_U32 tmp_data;

	if(u32Enable == 0)
        return ;

    for(ii=0; ii<2; ii++)
    {
        DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + ii*CHN_OFFSET));
        dhd_en[ii] = DHD0_CTRL.bits.intf_en;
    }

    axi_freq  = 1000/clk_axi_t;
    dhd0_freq = 1000/clk_vo_hd_t;
    dhd1_freq = 1000/clk_vo_sd_t;

    //dhd0 chnnal read latency calc
    if(dhd_en[0] == 1)
    {
        DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)));
        intf_w = DHD0_HSYNC1.bits.hact;


        for(ii=0; ii<2; ii++)
        {
            V0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32)  + ii*VID_OFFSET));
            V0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + ii*VID_OFFSET));

            vid_en       = V0_CTRL.bits.surface_en;
            vid_ifmt     = V0_CTRL.bits.ifmt;
            vid_iw       = V0_IRESO.bits.iw;

            if(vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_422)
                pixcel_num = 2;
            else if(vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_420 || vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_TILE_64 ||  vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_TILE)
                pixcel_num = 1.5;

            //calc every line read data num
            if(vid_en == 1)
            {
                tmp_data = (vid_iw*region_num + (req_byte-1))/req_byte;
                vid_line_data_layer = tmp_data*pixcel_num;
                vid_data_total = vid_data_total + vid_line_data_layer;
            }
        }

        for(ii=0; ii<4; ii++)
        {
            G0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + ii*GFX_OFFSET));
            G0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_IRESO.u32)+ ii*GFX_OFFSET));

            gfx_en       = G0_CTRL.bits.surface_en;
            gfx_ifmt     = G0_CTRL.bits.ifmt;
            gfx_iw       = G0_IRESO.bits.iw;

            if(gfx_ifmt == (HI_U32)VDP_GFX_IFMT_ARGB_8888)
                pixcel_num = 4;

            //calc every line read data num
            if(gfx_en == 1)
            {
                VDP_ASSERT(req_byte != 0);
                tmp_data = (gfx_iw*region_num + (req_byte-1))/req_byte;
                gfx_line_data_layer = tmp_data*pixcel_num;
                gfx_data_total = gfx_data_total + gfx_line_data_layer;
            }
        }

        chn_data_total = vid_data_total + gfx_data_total;
        //VDP_ASSERT(chn_data_total != 0);
        VDP_ASSERT(dhd0_freq != 0);

        line_cycle_num = (axi_freq/dhd0_freq)*intf_w;
#if EDA_TEST
        cout << dec << "line_cycle_num = " << line_cycle_num << endl;
#endif
        if(chn_data_total != 0)
        {
            dhd0_latency = (line_cycle_num + chn_data_total - 1)/chn_data_total;

#if EDA_TEST
            cout << dec << "dhd0_latency = " << dhd0_latency << endl;
#endif
        }
        else
        {
#if EDA_TEST
            cout << "Warnning: all layer is disable! "<< endl;
#endif
        }
    }
    
    if(dhd_en[1] == 1)
    {
        DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)) + 1*CHN_OFFSET);
        intf_w = DHD0_HSYNC1.bits.hact;


        //v3
        V0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32)  + 3*VID_OFFSET));
        V0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + 3*VID_OFFSET));

        vid_en       = V0_CTRL.bits.surface_en;
        vid_ifmt     = V0_CTRL.bits.ifmt;
        vid_iw       = V0_IRESO.bits.iw;

        if(vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_422)
            pixcel_num = 2;
        else if(vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_420 || vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_TILE_64 ||  vid_ifmt == (HI_U32)VDP_VID_IFMT_SP_TILE)
            pixcel_num = 1.5;

        //calc every line read data num
        if(vid_en == 1)
        {
            tmp_data = (vid_iw*region_num + (req_byte-1))/req_byte;
            vid_line_data_layer = tmp_data*pixcel_num;
            vid_data_total = vid_data_total + vid_line_data_layer;
        }

        //G4 
        G0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + 4*GFX_OFFSET));
        G0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_IRESO.u32)+ 4*GFX_OFFSET));

        gfx_en       = G0_CTRL.bits.surface_en;
        gfx_ifmt     = G0_CTRL.bits.ifmt;
        gfx_iw       = G0_IRESO.bits.iw;

        if(gfx_ifmt == (HI_U32)VDP_GFX_IFMT_ARGB_8888)
            pixcel_num = 4;

        //calc every line read data num
        if(gfx_en == 1)
        {
            VDP_ASSERT(req_byte != 0);
            tmp_data = (gfx_iw*region_num + (req_byte-1))/req_byte;
            gfx_line_data_layer = tmp_data*pixcel_num;
            gfx_data_total = gfx_data_total + gfx_line_data_layer;
        }

        chn_data_total = vid_data_total + gfx_data_total;
        //VDP_ASSERT(chn_data_total != 0);
        VDP_ASSERT(dhd0_freq != 0);

        line_cycle_num = (axi_freq/dhd1_freq)*intf_w;
#if EDA_TEST
        cout << dec << "line_cycle_num = " << line_cycle_num << endl;
#endif

        if(chn_data_total != 0)
        {
            dhd0_latency = (line_cycle_num + chn_data_total - 1)/chn_data_total;

#if EDA_TEST
            cout << dec << "dhd1_latency = " << dhd0_latency << endl;
#endif
        }
        else
        {
#if EDA_TEST
            cout << "Warnning: all layer is disable! "<< endl;
#endif
        }
    }

    if(dhd_en[0] == 1 && dhd_en[1] == 1)
        total_latency = 1/(1/((float)dhd0_latency) + 1/(float)(dhd1_latency));
    else if(dhd_en[0] == 1)
        total_latency = dhd0_latency;
    else if(dhd_en[1] == 1)
        total_latency = dhd1_latency;

#if EDA_TEST
    cout << dec << "Total Latency = " << total_latency << endl;
    cout << "VDP_ReadLatencyCalc end of work:"  << endl;
#endif
     
}
#endif

HI_S32 VDP_ConfigCheck(HI_U32 type)
{
#if EDA_TEST
    cout << "vdp config check:"  << endl;
#endif
 
	HI_U32 ii=0;
	HI_U32 jj=0;
    HI_U32 kk=0;

// 	HI_U32 m=0;
// 	HI_U32 n=0;
    
    HI_U32 vid[4] = {0, 1, 3, 4};
    HI_U32 wbc[3] = {0, 1, 3};
	U_MIXV0_MIX  MIXV0_MIX;
	U_CBM_MIX1  CBM_MIX1;

	if(type == 0)
		return HI_FAILURE ;

    VDP_Sys_ConfigCheck();
    
    VDP_MMU_AddrCheck ();

	for(ii=0;ii<(sizeof(vid)/4 -1);ii++)
	{
		U_V0_CTRL V0_CTRL;
		V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + vid[ii] * VID_OFFSET));
        
//         cout << "[cfgcheck] V0_CTRL addr = " << ((HI_U32)(&(pVdpReg->V0_CTRL.u32) + vid[ii] * VID_OFFSET)) <<endl;
//         cout << "[cfgcheck] V0_CTRL.u32 = " << V0_CTRL.u32 <<endl;
		if(V0_CTRL.bits.surface_en == 1)
		{
			VDP_VID_ConfigCheck(vid[ii]);
		}
	}

		VDP_VP_ConfigCheck(0);

    for(jj=0;jj<GFX_MAX;jj++)
	{
		VDP_GFX_ConfigCheck(jj);
	}

    for(kk=0;kk<(sizeof(wbc)/4 -1);kk++)
    {
        U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;
    	WBC_DHD0_CTRL.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32) + wbc[kk] * WBC_OFFSET));
        if (WBC_DHD0_CTRL.bits.wbc_en == HI_TRUE)   
        {
            VDP_WBC_ConfigCheck(wbc[kk]);
        }
    }


//        VDP_WBC_ConfigCheck();
/*	for(m=0;m<VP_MAX;m++)
	{
#if EDA_TEST
		cout << dec << "m = " << m <<endl;
#endif
		VDP_VP_ConfigCheck(m);
	}

	for(jj=0;jj<GFX_MAX;jj++)
	{
#if EDA_TEST
		cout << dec << "jj = " << jj <<endl;
#endif
		VDP_GFX_ConfigCheck(jj);
	}

	for(n=0;n<VP_MAX;n++)
	{
#if EDA_TEST
		cout << dec << "n = " << n <<endl;
#endif
		VDP_GP_ConfigCheck(n);
	}
*/
	//------------------------------------------------------------------------------------
	//MIXV
	//------------------------------------------------------------------------------------
	//if (mixer_prio0 == mixer_prio1, mixer_prio0 must not equal to 0; mixer_prio0 must not equal to mixer_prio1
	MIXV0_MIX.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->MIXV0_MIX.u32)));
	
	VDP_ASSERT(!((MIXV0_MIX.bits.mixer_prio0 == MIXV0_MIX.bits.mixer_prio1) && (MIXV0_MIX.bits.mixer_prio0 != 0x0)));
	VDP_ASSERT(!((MIXV0_MIX.bits.mixer_prio0 == MIXV0_MIX.bits.mixer_prio1) && (MIXV0_MIX.bits.mixer_prio1 != 0x1))); 
	
	//------------------------------------------------------------------------------------
	//CBM
	//------------------------------------------------------------------------------------
	//if (mixer_prio0 == mixer_prio1, mixer_prio0 must not equal to 0; mixer_prio0 must not equal to mixer_prio1
	CBM_MIX1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->CBM_MIX1.u32)));
	
	VDP_ASSERT(!((CBM_MIX1.bits.mixer_prio0 == CBM_MIX1.bits.mixer_prio1) && (CBM_MIX1.bits.mixer_prio0 != 0x0)));
	VDP_ASSERT(!((CBM_MIX1.bits.mixer_prio0 == CBM_MIX1.bits.mixer_prio1) && (CBM_MIX1.bits.mixer_prio1 != 0x1))); 

	return HI_SUCCESS;

}

HI_S32 VDP_VP_ConfigCheck(HI_U32 u32Data)
{
#if EDA_TEST
    cout << "vp" << u32Data << " config check:" << endl;
#endif
	
	U_DHD0_CTRL DHD0_CTRL;
    U_VP0_CTRL VP0_CTRL;
	U_VP0_DOF_CTRL  VP0_DOF_CTRL;
    U_VP0_DOF_STEP VP0_DOF_STEP;
	U_VP0_DCICTRL  VP0_DCICTRL;
	U_VP0_DCIMERBLD    VP0_DCIMERBLD; 
	U_VPWCG_CSC2CTRL  VPWCG_CSC2CTRL;
	U_VP0_CSC_ODC     VP0_CSC_ODC;

    U_VP0_IRESO   VP0_IRESO;    
	U_VP0_DFPOS   VP0_DFPOS;
    U_VP0_DLPOS   VP0_DLPOS;
    U_VP0_VFPOS   VP0_VFPOS;
    U_VP0_VLPOS   VP0_VLPOS;
    U_DHD0_HSYNC1 DHD0_HSYNC1;
    HI_U32 dof_width;
    HI_U32 right_step_org;
    HI_U32 left_step_org;
    HI_S32 right_step;
    HI_S32 left_step;

	VP0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CTRL.u32) + u32Data * VP_OFFSET));
    VP0_DOF_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DOF_CTRL.u32)+ u32Data * VP_OFFSET));
    VP0_DOF_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DOF_STEP.u32)+ u32Data * VP_OFFSET));
//     if(VP0_CTRL.bits.mute_en == 1)
//         return;
	
	//------------------------------------------------------------------------------------
	//DCI
	//------------------------------------------------------------------------------------
	VP0_DCICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCICTRL.u32) + u32Data * VP_OFFSET));
    
// 	if(VP0_DCICTRL.bits.dci_en == 0)
//         return;
   
	VP0_DCIMERBLD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DCIMERBLD.u32) + u32Data * VP_OFFSET));
    
    
	//------------------------------------------------------------------------------------
	//WCG
	//------------------------------------------------------------------------------------
	VPWCG_CSC2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VPWCG_CSC2CTRL.u32)));
	VP0_CSC_ODC.u32    = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_CSC_ODC.u32)));

	//wcg enable, wcg_csc should be sign bit mode
	VDP_ASSERT(!((VPWCG_CSC2CTRL.bits.csc2_en == 0x1) && (VP0_CSC_ODC.bits.csc_sign_mode !=1)));

	//------------------------------------------------------------------------------------
	//VP_LBOX
	//------------------------------------------------------------------------------------
    VP0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32) + u32Data * VID_OFFSET));
    VP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32) + u32Data * VID_OFFSET));
    VP0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32) + u32Data * VID_OFFSET));
    VP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VFPOS.u32) + u32Data * VP_OFFSET));
    VP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_VLPOS.u32) + u32Data * VP_OFFSET));

	//video VX >= DSX; VY >= DSY
	VDP_ASSERT(!(VP0_VFPOS.bits.video_xfpos < VP0_DFPOS.bits.disp_xfpos));
	VDP_ASSERT(!(VP0_VFPOS.bits.video_yfpos < VP0_DFPOS.bits.disp_yfpos));

	//vp DXL-DXS >= input width; DYL-DYS >= in height
	//vp DXL >= VX + output width ;DYL >= VY + outputheight
#if EDA_TEST
	cout << "VP0_DLPOS.bits.disp_xlpos-VP0_DFPOS.bits.disp_xfpos:" << VP0_DLPOS.bits.disp_xlpos-VP0_DFPOS.bits.disp_xfpos << endl;
	cout << "VP0_VFPOS.bits.video_xfpos:" << VP0_VFPOS.bits.video_xfpos << endl;
	cout << "VP0_IRESO.bits.iw:" << VP0_IRESO.bits.iw<< endl;
#endif
	VDP_ASSERT(!((VP0_DLPOS.bits.disp_xlpos) < (VP0_IRESO.bits.iw + VP0_VFPOS.bits.video_xfpos))); 
	VDP_ASSERT(!((VP0_DLPOS.bits.disp_ylpos) < (VP0_IRESO.bits.ih + VP0_VFPOS.bits.video_yfpos))); 
	VDP_ASSERT(!((VP0_DLPOS.bits.disp_xlpos-VP0_DFPOS.bits.disp_xfpos) < VP0_IRESO.bits.iw)); 
	VDP_ASSERT(!((VP0_DLPOS.bits.disp_ylpos-VP0_DFPOS.bits.disp_yfpos) < VP0_IRESO.bits.ih)); 

	//------------------------------------------------------------------------------------
	//VP_dof
	//------------------------------------------------------------------------------------
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)));

   // if((DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_2D))
   // {
   //    VDP_ASSERT(VP0_DOF_CTRL.bits.dof_en == 0x0);
   // }
    
    DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)));
    dof_width = DHD0_HSYNC1.bits.hact + 1;
    right_step_org = VP0_DOF_STEP.bits.right_step;
    left_step_org = VP0_DOF_STEP.bits.left_step;
    right_step = s32SignConv(right_step_org, 8);
    left_step = s32SignConv(left_step_org, 8);

#if EDA_TEST
    cout << "dof_en = " << VP0_DOF_CTRL.bits.dof_en <<endl;
    cout << "dof_width = " << dof_width << endl;
    cout << "right_step = " << VP0_DOF_STEP.bits.right_step << endl;
    cout << "left_step = " << VP0_DOF_STEP.bits.left_step << endl;
#endif

    if(VP0_DOF_CTRL.bits.dof_en == 0x1)
    {
        if(dof_width >= 50)
        {
//            VDP_ASSERT((VP0_DOF_STEP.bits.right_step <= 50) && (VP0_DOF_STEP.bits.right_step >= (0-50)));
//            VDP_ASSERT((VP0_DOF_STEP.bits.left_step <= 50) && (VP0_DOF_STEP.bits.left_step >= (0-50)));
           VDP_ASSERT((right_step >= (0-50)) && (right_step <= 50));
           VDP_ASSERT((left_step >= (0-50)) && (left_step <= 50));
        }
        else
        {
//            VDP_ASSERT((VP0_DOF_STEP.bits.right_step <= dof_width) && (VP0_DOF_STEP.bits.right_step >= (0-dof_width)));
//            VDP_ASSERT((VP0_DOF_STEP.bits.left_step <= dof_width) && (VP0_DOF_STEP.bits.left_step >= (0-dof_width)));
           VDP_ASSERT((right_step >= (0-dof_width)) && (right_step <= dof_width));
           VDP_ASSERT((left_step >= (0-dof_width)) && (left_step <= dof_width));
        }
    }
	
    return HI_SUCCESS;
}

HI_S32 VDP_Sys_ConfigCheck(HI_VOID)
{
#if EDA_TEST
    cout << "VDP_Sys_ConfigCheck" <<endl;
#endif
    U_DHD0_CTRL DHD0_CTRL;
    U_VOCTRL VOCTRL;
    U_DHD0_HSYNC1 DHD0_HSYNC1;
    U_DHD0_VSYNC DHD0_VSYNC;
    U_DHD0_DITHER0_CTRL DHD0_DITHER0_CTRL;
    HI_U32 enChan = 0;

    U_VOAXICTRL VOAXICTRL;
    U_VOMASTERSEL VOMASTERSEL;
    U_V0_PRERD V0_PRERD;
    U_V0_CTRL  V0_CTRL;
    HI_U32 vid[4] = {0,1,3,4};

    HI_U32 v0_en = 0;
    HI_U32 v1_en = 0;
    HI_U32 v3_en = 0;

    HI_U32 v0_req_ctrl = 0;
    HI_U32 v1_req_ctrl = 0;
    HI_U32 v3_req_ctrl = 0;

    HI_U32 v0_req_len  = 0;
    HI_U32 v1_req_len  = 0;
    HI_U32 v3_req_len  = 0;

    HI_U32 i = 0;

    U_G0_CTRL  G0_CTRL;
    HI_U32 gfx[4] = {0,1,4,5};

    HI_U32 g0_en = 0;
    HI_U32 g1_en = 0;
    HI_U32 g3_en = 0;

    HI_U32 g0_req_ctrl = 0;
    HI_U32 g1_req_ctrl = 0;
    HI_U32 g3_req_ctrl = 0;

    HI_U32 g0_req_len  = 0;
    HI_U32 g1_req_len  = 0;
    HI_U32 g3_req_len  = 0;

//v0 coef
    U_V0_HSP               V0_HSP              ; 
    U_V0_VSP               V0_VSP              ; 
    U_V0_HLCOEFAD          V0_HLCOEFAD         ;
    U_V0_HCCOEFAD          V0_HCCOEFAD         ;
    U_V0_VLCOEFAD          V0_VLCOEFAD         ;
    U_V0_VCCOEFAD          V0_VCCOEFAD         ;
    U_V0_ZMECOEFAD         V0_ZMECOEFAD        ;
    
    U_V0_ZME2_HSP          V0_ZME2_HSP         ; /* 0xbac0 */
    U_V0_ZME2_VSP          V0_ZME2_VSP         ; /* 0xbad8 */
    U_V0_ZME2_ZMECOEFAD    V0_ZME2_ZMECOEFAD   ;
    U_V0_ZME2_HLCOEFAD     V0_ZME2_HLCOEFAD    ;
    U_V0_ZME2_HCCOEFAD     V0_ZME2_HCCOEFAD    ;
    U_V0_ZME2_VLCOEFAD     V0_ZME2_VLCOEFAD    ;
    U_V0_ZME2_VCCOEFAD     V0_ZME2_VCCOEFAD    ;
    
    U_VDM_CTRL             VDM_CTRL                          ; /* 0x3000 */
    U_VDM_PARA_DEGMM_ADDR  VDM_PARA_DEGMM_ADDR ;
    U_VDM_PARA_GMM_ADDR    VDM_PARA_GMM_ADDR   ;
    U_VDM_PARA_TMAP_ADDR   VDM_PARA_TMAP_ADDR  ;
    
//gp0 coef                                                   
    U_GDM_CTRL             GDM_CTRL                          ; /* 0x3800 */
    U_GP0_ZME_HSP          GP0_ZME_HSP                       ; /* 0x8140 */
    U_GP0_ZME_VSP          GP0_ZME_VSP                       ; /* 0x814c */
    
    U_GDM_PARA_GMM_ADDR    GDM_PARA_GMM_ADDR   ;
    U_GDM_PARA_TMAP_ADDR   GDM_PARA_TMAP_ADDR  ;
    U_GP0_HCOEFAD          GP0_HCOEFAD         ;
    U_GP0_VCOEFAD          GP0_VCOEFAD         ;
    U_GP0_ZMECOEFAD        GP0_ZMECOEFAD       ;
    
//wbc coef                                                   
    U_WBC_DHD0_CTRL        WBC_DHD0_CTRL          ;
    U_WBC_DHD0_ZME_HSP     WBC_DHD0_ZME_HSP                  ; /* 0x92c0 */
    U_WBC_DHD0_ZME_VSP     WBC_DHD0_ZME_VSP                  ; /* 0x92d8 */
    U_WBC_DHD0_HLCOEFAD    WBC_DHD0_HLCOEFAD   ;
    U_WBC_DHD0_HCCOEFAD    WBC_DHD0_HCCOEFAD   ;
    U_WBC_DHD0_VLCOEFAD    WBC_DHD0_VLCOEFAD   ;
    U_WBC_DHD0_VCCOEFAD    WBC_DHD0_VCCOEFAD   ;
    HI_U32 wbc[3] = {0,1,3};

    for(enChan = 0; enChan <= 0x1; enChan++)
    {
        DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + CHN_OFFSET*enChan));
        VOCTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOCTRL.u32) + CHN_OFFSET*enChan));
        DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32) + CHN_OFFSET*enChan));
        DHD0_VSYNC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32) + CHN_OFFSET*enChan));
        DHD0_DITHER0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_DITHER0_CTRL.u32) + CHN_OFFSET*enChan));

        if(DHD0_CTRL.bits.intf_en)
        {
            if(enChan == 0x0)
            {
                if(DHD0_CTRL.bits.hdmi420_en == 0x0)
                {
                    if(VOCTRL.bits.twochn_mode == 0x1)
                    {
                       VDP_ASSERT(DHD0_CTRL.bits.twochn_debug == 0x1);
                    }
                }
                else
                {
                   VDP_ASSERT((VOCTRL.bits.twochn_en == 0x1) && (VOCTRL.bits.twochn_mode == 0x1) && (DHD0_CTRL.bits.twochn_debug == 0x0));
                }
            }

//             VDP_ASSERT(DHD0_VSYNC.bits.vfb + DHD0_VSYNC.bits.vbb > 13);
        }

    }

    VOAXICTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOAXICTRL.u32)));
    VOMASTERSEL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->VOMASTERSEL.u32)));
    if(VOAXICTRL.bits.m0_mutli_id_o == 0x1)
    {
        for(i = 0; i <= 3; i++)
        {
            V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + vid[i] * VID_OFFSET));
            V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + vid[i] * VID_OFFSET));
            if(i == 0)
            {
                v0_en = V0_CTRL.bits.surface_en;

                if(V0_CTRL.bits.req_ctrl == 0)
                    v0_req_ctrl  = 1;
                else if(V0_CTRL.bits.req_ctrl == 1)
                    v0_req_ctrl  = 2;
                else if(V0_CTRL.bits.req_ctrl == 2)
                    v0_req_ctrl  = 4;
                else if(V0_CTRL.bits.req_ctrl == 3)
                    v0_req_ctrl  = 8;

                if(V0_PRERD.bits.req_len == 0)
                    v0_req_len  = 16;
                else if(V0_PRERD.bits.req_len == 1)
                    v0_req_len  = 8;
                else if(V0_PRERD.bits.req_len == 2)
                    v0_req_len  = 4;

            }
            else if(i == 1)
            {
                v1_en = V0_CTRL.bits.surface_en;
                if(V0_CTRL.bits.req_ctrl == 0)
                    v1_req_ctrl  = 1;
                else if(V0_CTRL.bits.req_ctrl == 1)
                    v1_req_ctrl  = 2;
                else if(V0_CTRL.bits.req_ctrl == 2)
                    v1_req_ctrl  = 4;
                else if(V0_CTRL.bits.req_ctrl == 3)
                    v1_req_ctrl  = 8;

                if(V0_PRERD.bits.req_len == 0)
                    v1_req_len  = 16;
                else if(V0_PRERD.bits.req_len == 1)
                    v1_req_len  = 8;
                else if(V0_PRERD.bits.req_len == 2)
                    v1_req_len  = 4;
            }
            else if(i == 2)
            {
                v3_en = V0_CTRL.bits.surface_en;
                if(V0_CTRL.bits.req_ctrl == 0)
                    v3_req_ctrl  = 1;
                else if(V0_CTRL.bits.req_ctrl == 1)
                    v3_req_ctrl  = 2;
                else if(V0_CTRL.bits.req_ctrl == 2)
                    v3_req_ctrl  = 4;
                else if(V0_CTRL.bits.req_ctrl == 3)
                    v3_req_ctrl  = 8;

                if(V0_PRERD.bits.req_len == 0)
                    v3_req_len  = 16;
                else if(V0_PRERD.bits.req_len == 1)
                    v3_req_len  = 8;
                else if(V0_PRERD.bits.req_len == 2)
                    v3_req_len  = 4;
            }

            if(V0_CTRL.bits.surface_en == 0x1)
            {
                //if((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64) && (V0_CTRL.bits.req_ctrl != 0))
                //{
                    VDP_ASSERT((V0_PRERD.bits.pre_rd_en == 0x1));
                //}
            }
        }
    }

#if EDA_TEST
    cout << dec << "[assert] v0_en       = " << v0_en       << endl; 
    cout << dec << "[assert] v1_en       = " << v1_en       << endl; 
    cout << dec << "[assert] v3_en       = " << v3_en       << endl; 

    cout << dec << "[assert] v0_req_ctrl = " << v0_req_ctrl << endl; 
    cout << dec << "[assert] v1_req_ctrl = " << v1_req_ctrl << endl; 
    cout << dec << "[assert] v3_req_ctrl = " << v3_req_ctrl << endl; 

    cout << dec << "[assert] v0_req_len  = " << v0_req_len  << endl; 
    cout << dec << "[assert] v1_req_len  = " << v1_req_len  << endl; 
    cout << dec << "[assert] v3_req_len  = " << v3_req_len  << endl; 
#endif

    if(v0_en == 1)
        VDP_ASSERT((v0_en==1) && (v0_req_ctrl*v0_req_len <= 128));

    if(v1_en == 1)
        VDP_ASSERT((v1_en==1) && (v1_req_ctrl*v1_req_len <= 32));

    if(v3_en == 1)
        VDP_ASSERT((v3_en==1) && (v3_req_ctrl*v3_req_len <= 16));

    if(VOAXICTRL.bits.m1_mutli_id_o == 0x1)
    {
        for(i = 0; i <= 3; i++)
        {
            V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + vid[i] * VID_OFFSET));
            V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + vid[i] * VID_OFFSET));
            if(V0_CTRL.bits.surface_en == 0x1)
            {
               VDP_ASSERT(!((VOMASTERSEL.bits.v0_master_sel == 0x1) && (V0_PRERD.bits.pre_rd_en == 0x0)));
            }
        }
    }

    for(i = 0; i < 3; i++)
    {
        G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + gfx[i] * GFX_OFFSET));
        if(i == 0)
        {
            g0_en = G0_CTRL.bits.surface_en;

            if(G0_CTRL.bits.req_ctrl == 0)
                g0_req_ctrl  = 1;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g0_req_ctrl  = 2;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g0_req_ctrl  = 4;
            else if(G0_CTRL.bits.req_ctrl == 3)
                g0_req_ctrl  = 8;

            if(G0_CTRL.bits.rlen_sel == 0)
                g0_req_len  = 16;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g0_req_len  = 8;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g0_req_len  = 4;
        }
        else if(i == 1)
        {
            g1_en = G0_CTRL.bits.surface_en;

            if(G0_CTRL.bits.req_ctrl == 0)
                g1_req_ctrl  = 1;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g1_req_ctrl  = 2;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g1_req_ctrl  = 4;
            else if(G0_CTRL.bits.req_ctrl == 3)
                g1_req_ctrl  = 8;

            if(G0_CTRL.bits.rlen_sel == 0)
                g1_req_len  = 16;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g1_req_len  = 8;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g1_req_len  = 4;
        }
        else
        {
            g3_en = G0_CTRL.bits.surface_en;
            g3_req_ctrl = G0_CTRL.bits.req_ctrl;
            if(G0_CTRL.bits.req_ctrl == 0)
                g3_req_ctrl  = 1;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g3_req_ctrl  = 2;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g3_req_ctrl  = 4;
            else if(G0_CTRL.bits.req_ctrl == 3)
                g3_req_ctrl  = 8;

            if(G0_CTRL.bits.rlen_sel == 0)
                g3_req_len  = 16;
            else if(G0_CTRL.bits.req_ctrl == 1)
                g3_req_len  = 8;
            else if(G0_CTRL.bits.req_ctrl == 2)
                g3_req_len  = 4;
        }
    }

#if EDA_TEST
    cout << dec << "[assert] g0_en       = " << g0_en       << endl; 
    cout << dec << "[assert] g1_en       = " << g1_en       << endl; 
    cout << dec << "[assert] g3_en       = " << g3_en       << endl; 

    cout << dec << "[assert] g0_req_ctrl = " << g0_req_ctrl << endl; 
    cout << dec << "[assert] g1_req_ctrl = " << g1_req_ctrl << endl; 
    cout << dec << "[assert] g3_req_ctrl = " << g3_req_ctrl << endl; 

    cout << dec << "[assert] g0_req_len  = " << g0_req_len  << endl; 
    cout << dec << "[assert] g1_req_len  = " << g1_req_len  << endl; 
    cout << dec << "[assert] g3_req_len  = " << g3_req_len  << endl; 
#endif

    if(g0_en == 1 && g1_en == 1 && g3_en == 1)
        VDP_ASSERT(VOCTRL.bits.m0_arb_mode == 0);

    if(g0_en == 1)
        VDP_ASSERT((g0_req_ctrl*g0_req_len <= 128));
    
    if(g1_en == 1)
        VDP_ASSERT((g1_req_ctrl*g1_req_len <= 128));

    if(g3_en == 1)
        VDP_ASSERT((g3_req_ctrl*g3_req_len <= 32));

//req ctrl and mac ctrlcheck in hi3798mv200
    for(i=0; i <= 2; i++)
    {
        V0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + vid[i] * VID_OFFSET));
        V0_PRERD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + vid[i] * VID_OFFSET));
        
        VDP_ASSERT(V0_CTRL.bits.req_ctrl >= V0_PRERD.bits.req_mac_ctrl);
    
    }
    for(i=0; i <= 2; i++)
    {
        G0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + gfx[i] * GFX_OFFSET));
        VDP_ASSERT(G0_CTRL.bits.req_ctrl >= G0_CTRL.bits.req_mac_ctrl);
    }

//COEF ADDR CHECK in hi3798mv200
//v0 coef

    for(i = 0; i <= 1; i++)
    {
        V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + vid[i] * VID_OFFSET));
        V0_HSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + vid[i] * VID_OFFSET));
        V0_VSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + vid[i] * VID_OFFSET));
        V0_ZME2_HSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_HSP.u32) ));
        V0_ZME2_VSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_VSP.u32) ));
        V0_HLCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HLCOEFAD.u32) + vid[i] * VID_OFFSET));
        V0_HCCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HCCOEFAD.u32) + vid[i] * VID_OFFSET));
        V0_VLCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLCOEFAD.u32) + vid[i] * VID_OFFSET));
        V0_VCCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VCCOEFAD.u32) + vid[i] * VID_OFFSET));
        V0_ZMECOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZMECOEFAD.u32) + vid[i] * VID_OFFSET));
        V0_ZME2_HLCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_HLCOEFAD.u32) ));
        V0_ZME2_HCCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_HCCOEFAD.u32) ));
        V0_ZME2_VLCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_VLCOEFAD.u32) ));
        V0_ZME2_VCCOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_VCCOEFAD.u32) ));
        V0_ZME2_ZMECOEFAD.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_ZMECOEFAD.u32) ));
        
        VDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_CTRL.u32)));
        VDM_PARA_DEGMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_DEGMM_ADDR.u32)));
        VDM_PARA_GMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_GMM_ADDR.u32)));
        VDM_PARA_TMAP_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VDM_PARA_TMAP_ADDR.u32)));

        if(V0_CTRL.bits.surface_en == 0x1)
        {
            //v0 zme and v1 cvfir
            if((V0_HSP.bits.hlmsc_en == 0x1) && (V0_HSP.bits.hlfir_en == 0x1))
            {
                VDP_ASSERT(V0_HLCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_HSP.bits.hchmsc_en == 0x1) && (V0_HSP.bits.hchfir_en == 0x1))
            {
                VDP_ASSERT(V0_HCCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_VSP.bits.vlmsc_en == 0x1) && (V0_VSP.bits.vlfir_en == 0x1))
            {
                VDP_ASSERT(V0_VLCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_VSP.bits.vchmsc_en == 0x1) && (V0_VSP.bits.vchfir_en == 0x1))
            {
                VDP_ASSERT(V0_VCCOEFAD.bits.coef_addr % 256 == 0);
            }
            //v0 zme2
            if((V0_ZME2_HSP.bits.hlmsc_en == 0x1) && (V0_ZME2_HSP.bits.hlfir_en == 0x1))
            {
                VDP_ASSERT(V0_ZME2_HLCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_ZME2_HSP.bits.hchmsc_en == 0x1) && (V0_ZME2_HSP.bits.hchfir_en == 0x1))
            {
                VDP_ASSERT(V0_ZME2_HCCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_ZME2_VSP.bits.vlmsc_en == 0x1) && (V0_ZME2_VSP.bits.vlfir_en == 0x1))
            {
                VDP_ASSERT(V0_ZME2_VLCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((V0_ZME2_VSP.bits.vchmsc_en == 0x1) && (V0_ZME2_VSP.bits.vchfir_en == 0x1))
            {
                VDP_ASSERT(V0_ZME2_VCCOEFAD.bits.coef_addr % 256 == 0);
            }
            //vp vhdr                                                   

            if(VDM_CTRL.bits.vdm_en == 0x1)
            {
                if(VDM_CTRL.bits.vdm_degmm_en == 0x1)
                {
                    VDP_ASSERT(VDM_PARA_DEGMM_ADDR.bits.vdm_para_degmm_addr % 256 == 0);
                }
                if(VDM_CTRL.bits.vdm_tmap_en == 0x1)
                {
                    VDP_ASSERT(VDM_PARA_TMAP_ADDR.bits.vdm_para_tmap_addr % 256 == 0);
                }
                if(VDM_CTRL.bits.vdm_gmm_en == 0x1)
                {
                    VDP_ASSERT(VDM_PARA_GMM_ADDR.bits.vdm_para_gmm_addr % 256 == 0);
                }
            }
        }
    }
                                                   
//gp0 coef                                                   
    
    for(i = 0; i <= 1; i++)
    {
        G0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + gfx[i] * GFX_OFFSET));
        GDM_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_CTRL.u32)));
        GDM_PARA_GMM_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_GMM_ADDR.u32)));
        GDM_PARA_TMAP_ADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GDM_PARA_TMAP_ADDR.u32)));
        GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32)));
        GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32)));
        GP0_HCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_HCOEFAD.u32)));
        GP0_VCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VCOEFAD.u32)));
        GP0_ZMECOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZMECOEFAD.u32)));

#if EDA_TEST
        cout << hex << "gp0 hcoef_addr = " << GP0_HCOEFAD.bits.coef_addr << endl;
        cout << hex << "gp0 vcoef_addr = " << GP0_VCOEFAD.bits.coef_addr << endl;
#endif
        
        if(G0_CTRL.bits.surface_en == 0x1)
        {
            if(GDM_CTRL.bits.gdm_en == 0x1)
            {
                if(GDM_CTRL.bits.gdm_tmap_en == 0x1)
                {
                    VDP_ASSERT(GDM_PARA_TMAP_ADDR.bits.gdm_para_tmap_addr % 256 == 0);
                }
                if(GDM_CTRL.bits.gdm_gmm_en == 0x1)
                {
                    VDP_ASSERT(GDM_PARA_GMM_ADDR.bits.gdm_para_gmm_addr % 256 == 0);
                }
            }

            if((GP0_ZME_HSP.bits.hsc_en == 0x1) && ((GP0_ZME_HSP.bits.hfir_en == 0x1) || (GP0_ZME_HSP.bits.hafir_en == 0x1)))
            {
                VDP_ASSERT(GP0_HCOEFAD.bits.coef_addr % 256 == 0);
            }
            if((GP0_ZME_VSP.bits.vsc_en == 0x1) && ((GP0_ZME_VSP.bits.vfir_en == 0x1) || (GP0_ZME_VSP.bits.vafir_en == 0x1)))
            {
                VDP_ASSERT(GP0_VCOEFAD.bits.coef_addr % 256 == 0);
            }
        }
    }
    
                                                   
//wbc coef                                                   

    for(i = 0; i<= 2; i++)
    {
        WBC_DHD0_CTRL.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_ZME_HSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_HSP.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_ZME_VSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_VSP.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_HLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_HLCOEFAD.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_HCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_HCCOEFAD.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_VLCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_VLCOEFAD.u32) + wbc[i] * WBC_OFFSET));
        WBC_DHD0_VCCOEFAD.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_VCCOEFAD.u32) + wbc[i] * WBC_OFFSET));

            //wbc dhd zme and wbc vp zme and wbc gp zme
        if((WBC_DHD0_ZME_HSP.bits.hlmsc_en == 0x1) && (WBC_DHD0_ZME_HSP.bits.hlfir_en == 0x1))
        {
            VDP_ASSERT(WBC_DHD0_HLCOEFAD.bits.coef_addr % 256 == 0);
        }
        if((WBC_DHD0_ZME_HSP.bits.hchmsc_en == 0x1) && (WBC_DHD0_ZME_HSP.bits.hchfir_en == 0x1))
        {
            VDP_ASSERT(WBC_DHD0_HCCOEFAD.bits.coef_addr % 256 == 0);
        }
        if((WBC_DHD0_ZME_VSP.bits.vlmsc_en == 0x1) && (WBC_DHD0_ZME_VSP.bits.vlfir_en == 0x1))
        {
            VDP_ASSERT(WBC_DHD0_VLCOEFAD.bits.coef_addr % 256 == 0);
        }
        if((WBC_DHD0_ZME_VSP.bits.vchmsc_en == 0x1) && (WBC_DHD0_ZME_VSP.bits.vchfir_en == 0x1))
        {
            VDP_ASSERT(WBC_DHD0_VCCOEFAD.bits.coef_addr % 256 == 0);
        }
    }
                                                   
    return HI_SUCCESS;
}

HI_S32 VDP_VID_ConfigCheck(HI_U32 u32Data)
{
#if EDA_TEST
//     cout << "video config check:"  << endl;
    cout << "VID" << u32Data << " config check:"  << endl;
#endif

	U_DHD0_CTRL DHD0_CTRL;
    U_DHD0_VSYNC           DHD0_VSYNC;
    U_V0_IRESO             V0_IRESO;
	U_V0_ORESO             V0_ORESO;
    U_V0_DFPOS             V0_DFPOS;
    U_V0_DLPOS             V0_DLPOS;
    U_V0_VFPOS             V0_VFPOS;
    U_V0_VLPOS             V0_VLPOS;

    U_SPCTRL               SPCTRL       ; 
    U_SPENABLE             SPENABLE     ;
	
    U_V0_CPOS  V0_CPOS;
	U_V0_VSP  V0_VSP;
	U_V0_HSP  V0_HSP;
	U_V0_VSR  V0_VSR;	
    
    U_V0_ZME2_VSP  V0_ZME2_VSP;
	U_V0_ZME2_HSP  V0_ZME2_HSP;

    HI_U32 v0_crop_ow;
//     HI_U32 v0_crop_oh;
    HI_U32 disp_mode;
    HI_U32 v0_iw;
    HI_U32 v0_ih;
    HI_U32 v0_ow;
    HI_U32 v0_oh;
	HI_U32 v0_zme_en;
	HI_U32 v0_zme2_en;

    HI_U32 mrg_mode;
    HI_U32 v0_16mute_en;
    HI_U32 v0_mute_en;
    HI_U32 v0_data_width;
    
    U_VP0_IRESO VP0_IRESO;
	U_V0_NADDR  V0_NADDR;
    U_V0_NCADDR V0_NCADDR;
	U_V0_P0LADDR           V0_P0LADDR;
    U_V0_P0CADDR           V0_P0CADDR;
    U_V0_P0STRIDE  V0_P0STRIDE;
    U_V0_DRAWMODE V0_DRAWMODE;
    U_V0_MULTI_MODE      V0_MULTI_MODE;
    U_V0_TILE_LOFFSET V0_TILE_LOFFSET;
    U_V0_TILE_COFFSET V0_TILE_COFFSET;
    U_V0_DCMP_CTRL V0_DCMP_CTRL;
    U_V0_16REGIONENL V0_16REGIONENL; 
    U_V0_PRERD     V0_PRERD   ; 
    U_V0_16MUTE V0_16MUTE;
    U_V0_P0RESO         V0_P0RESO;
    U_V0_P0VFPOS        V0_P0VFPOS;
    U_V0_P0VLPOS        V0_P0VLPOS;
//     U_V0_P0LADDR        V0_P0LADDR;
//     U_V0_P0CADDR        V0_P0CADDR;
    HI_U32              last_region_laddr = 0;  //initial value
    HI_U32              last_region_caddr = 0;
    HI_U32              region_w = 0;
    HI_U32              region_h = 0;
    HI_U32              i = 0;
	HI_U32 v0_flip_en;
	
    HI_U32 v0_hratio;
    HI_U32 v0_vratio;

	U_V0_CTRL V0_CTRL;
    HI_U32 u32hd_id = 0;

    if((u32Data == 0x0) || (u32Data == 0x1))
        u32hd_id = 0x0;
    else if((u32Data == 0x3) || (u32Data == 0x4))
        u32hd_id = 0x1;


	V0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET));
#if EDA_TEST
    //cout << "V" << u32Data << "_CTRL.u32 = " << V0_CTRL.u32 <<endl;
    //cout << "[VID_cfg_2] V0_CTRL addr = " << ((HI_U32)(&(pVdpReg->V0_CTRL.u32) + u32Data * VID_OFFSET)) <<endl;
#endif

    //if(V0_CTRL.bits.surface_en == 0)
    //    return;

	//------------------------------------------------------------------------------------
	//step 1: read register
	//------------------------------------------------------------------------------------
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)+u32hd_id*CHN_OFFSET));

    DHD0_VSYNC.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_VSYNC.u32) + u32hd_id*CHN_OFFSET));

	V0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32) + u32Data * VID_OFFSET));
	V0_ORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ORESO.u32) + u32Data * VID_OFFSET));

	V0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32) + u32Data * VID_OFFSET));
	V0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32) + u32Data * VID_OFFSET));
	
	V0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DFPOS.u32) + u32Data * VID_OFFSET));
	V0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DLPOS.u32) + u32Data * VID_OFFSET));
	
    V0_CPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CPOS.u32) + u32Data * VID_OFFSET));


	V0_VSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + u32Data * VID_OFFSET));
	V0_HSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + u32Data * VID_OFFSET));
    V0_VSR.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSR.u32) + u32Data * VID_OFFSET));

	V0_ZME2_VSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_VSP.u32) + u32Data * VID_OFFSET));
	V0_ZME2_HSP.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ZME2_HSP.u32) + u32Data * VID_OFFSET));

	
	V0_NADDR.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_NADDR.u32) + u32Data * VID_OFFSET));
	V0_NCADDR.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_NCADDR.u32) + u32Data * VID_OFFSET));
   
	
	V0_P0LADDR.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET));
	V0_P0CADDR.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET));
   
	V0_P0STRIDE.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET));
	
    V0_DRAWMODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DRAWMODE.u32) + u32Data * VID_OFFSET));


    V0_MULTI_MODE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MULTI_MODE.u32) + u32Data * VID_OFFSET));

    V0_16MUTE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16MUTE.u32) + u32Data * VID_OFFSET));

    V0_TILE_LOFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TILE_LOFFSET.u32) + u32Data * VID_OFFSET));
    V0_TILE_COFFSET.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_TILE_COFFSET.u32) + u32Data * VID_OFFSET));
    
    V0_DCMP_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DCMP_CTRL.u32) + u32Data * VID_OFFSET));

    V0_16REGIONENL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_16REGIONENL.u32) + u32Data * VID_OFFSET));

    SPCTRL.u32         = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPCTRL.u32)   + u32Data * VID_OFFSET));    
    SPENABLE.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->SPENABLE.u32) + u32Data * VID_OFFSET));

    V0_PRERD.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_PRERD.u32) + u32Data * VID_OFFSET)); 
	//------------------------------------------------------------------------------------
	//step 2:Judgement 
	//------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------
	//step 2.1: Video surface, V1/V4 ORESO = IRESO
	//------------------------------------------------------------------------------------
   v0_crop_ow = V0_CPOS.bits.src_xlpos - V0_CPOS.bits.src_xfpos + 1;
        
//     if((u32Data == VDP_LAYER_VID1) | (u32Data == VDP_LAYER_VID4) | (u32Data == VDP_LAYER_VID3))
    if((u32Data == VDP_LAYER_VID4) | (u32Data == VDP_LAYER_VID3))
    {
        //b02, only v0 support 3D
#if EDA_TEST
        //cout << dec << "v0_crop_ow = " << v0_crop_ow << endl;
        //cout << dec << "V0_ORESO.bits.ow = " << V0_ORESO.bits.ow << endl;
#endif
       VDP_ASSERT(V0_ORESO.bits.oh == V0_IRESO.bits.ih);
       VDP_ASSERT(v0_crop_ow == (V0_ORESO.bits.ow+1));  //v1/v4 nonsupport ZME
    }

    if(V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_TILE_64)
    {
        VDP_ASSERT(V0_CTRL.bits.dcmp_en == 0);
    }

    VDP_ASSERT(V0_PRERD.bits.pre_rd_en == 0x1);

    //------------------------------------------------------------------------------------
	//step 2.2: Video surface, in_reso of ZME is out_reso of crop
	//------------------------------------------------------------------------------------
//    v0_crop_oh = V0_CPOS.bits.src_ylpos - V0_CPOS.bits.src_yfpos + 1;
//    VDP_ASSERT(!(v0_crop_ow == V0_IRESO.bits.iw));
//    VDP_ASSERT(!(v0_crop_oh == V0_IRESO.bits.ih));

    //------------------------------------------------------------------------------------
	//step 2.3: crop
	//------------------------------------------------------------------------------------
#if EDA_TEST
    cout << "[assert] v0_crop_ow = " << v0_crop_ow << endl;
    cout << "[assert] V0_CPOS.bits.src_xfpos = " << V0_CPOS.bits.src_xfpos << endl;
#endif
   VDP_ASSERT(v0_crop_ow%2 == 0);

   VDP_ASSERT(V0_CPOS.bits.src_xfpos%2 == 0);

    //------------------------------------------------------------------------------------
	//step 2.3: ifmt,
	//------------------------------------------------------------------------------------
    //VDP_ASSERT(!(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444));   //B02 nonsupport SP_444
   VDP_ASSERT(V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_TILE);    //3798CV200 only support VDP_VID_IFMT_SP_TILE_64

    if(u32Data != VDP_LAYER_VID0)
    {
       VDP_ASSERT(!(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE));
       VDP_ASSERT(!(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64));
    }

    //------------------------------------------------------------------------------------
	//step 2.4: 3D fmt, judge ireso and oreso
	//------------------------------------------------------------------------------------
    disp_mode = DHD0_CTRL.bits.disp_mode;
    v0_iw = V0_IRESO.bits.iw;
    v0_ih = V0_IRESO.bits.ih;
    v0_ow = V0_ORESO.bits.ow;
    v0_oh = V0_ORESO.bits.oh;
 	v0_zme_en = (V0_VSP.bits.vlmsc_en==0x1) || (V0_VSP.bits.vchmsc_en==0x1) || (V0_HSP.bits.hlmsc_en==0x1) || (V0_HSP.bits.hchmsc_en==0x1);
 	v0_zme2_en= (V0_ZME2_VSP.bits.vlmsc_en==0x1) || (V0_ZME2_VSP.bits.vchmsc_en==0x1) || (V0_ZME2_HSP.bits.hlmsc_en==0x1) || (V0_ZME2_HSP.bits.hchmsc_en==0x1);
    
    if(v0_zme_en == 0x0)
    {
#if EDA_TEST
        //cout << "v0_iw = " << v0_iw <<endl;
        //cout << "v0_crop_ow = " << v0_crop_ow <<endl;
        //cout << "v0_ow = " << v0_ow <<endl;
#endif
        
        if(disp_mode == VDP_DISP_MODE_SBS) 
        {
                VDP_ASSERT(v0_crop_ow*2 == (v0_ow+1));
        }
        else if((disp_mode == VDP_DISP_MODE_LBL) || (disp_mode == VDP_DISP_MODE_TAB))
        {
            VDP_ASSERT((v0_ih+1)*2 == (v0_oh+1));
        }
        else
        {
            VDP_ASSERT(v0_ih == v0_oh);
            VDP_ASSERT(v0_crop_ow == v0_ow+1);
        }
    }
    //------------------------------------------------------------------------------------
	//step 2.5: mutil region 
	//------------------------------------------------------------------------------------
    mrg_mode = V0_MULTI_MODE.bits.mrg_mode;
    v0_16mute_en = V0_16MUTE.u32;
    v0_mute_en = V0_CTRL.bits.mute_en;
    v0_data_width = V0_CTRL.bits.data_width;

    if(((u32Data == VDP_LAYER_VID1) | (u32Data == VDP_LAYER_VID4)) && mrg_mode)
    {
//         VDP_ASSERT(!(v0_16mute_en == 0 && v0_mute_en));  //mutil region mute, layer,mute
        VDP_ASSERT(v0_mute_en == 0);  //mutil region layer mute
        //VDP_ASSERT((v0_iw < 3839) || (v0_ih < 2159));   //mutil region nonsupport 4K
//         VDP_ASSERT(!(v0_data_width == 1));  //mutil region nonsupport 10bit
    }
    
	//------------------------------------------------------------------------------------
	//multi region address and stride
	//------------------------------------------------------------------------------------
    //multi region, stride > reso_w
    if(((u32Data == VDP_LAYER_VID1) || (u32Data == VDP_LAYER_VID4)) && mrg_mode)
    {
        HI_U32 u32Num=0;
        for(i=1; i<=0x8000; i=(i<<1))
        {
            if((V0_16REGIONENL.u32 & i) != 0x0)
            {
                V0_P0STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0STRIDE.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                V0_P0RESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0RESO.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                V0_P0VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VFPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                V0_P0VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0VLPOS.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                V0_P0LADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                V0_P0CADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + u32Data * VID_OFFSET + u32Num * REGION_OFFSET));
                
#if EDA_TEST
//                 cout << "V0_16REGIONENL = " << V0_16REGIONENL.u32 <<endl;
//                 cout << "i = " << i <<endl;
//                 cout << "surface_stride = " << V0_P0STRIDE.bits.surface_stride <<endl;
//                 cout << "w = " << V0_P0RESO.bits.w <<endl;
#endif
                
                region_w = V0_P0VLPOS.bits.video_xlpos - V0_P0VFPOS.bits.video_xfpos +1;
                region_h = V0_P0VLPOS.bits.video_ylpos - V0_P0VFPOS.bits.video_yfpos +1;
                
               VDP_ASSERT(V0_P0STRIDE.bits.surface_stride >= (region_w*v0_data_width/8+15)/16*16); //B02, multi_region nonsupport 10bit
               VDP_ASSERT(V0_P0STRIDE.bits.surface_cstride >= (region_w*v0_data_width/8+15)/16*16); 
//                VDP_ASSERT(V0_P0LADDR.u32 >= last_region_laddr);
//                VDP_ASSERT(V0_P0CADDR.u32 >= last_region_caddr);

                last_region_laddr = V0_P0LADDR.bits.surface_addr + V0_P0STRIDE.bits.surface_stride * region_h; 
                last_region_caddr = V0_P0CADDR.bits.surface_addr + V0_P0STRIDE.bits.surface_cstride * region_h; 
            }
                u32Num++;
        }
    }

    //------------------------------------------------------------------------------------
	//step 2.6: RESO 
	//------------------------------------------------------------------------------------
   VDP_ASSERT((v0_iw+1)%2 == 0);
   VDP_ASSERT((v0_ih+1)%2 == 0);
   VDP_ASSERT((v0_ow+1)%2 == 0);
   VDP_ASSERT((v0_oh+1)%2 == 0);

    if((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x0))
    {
       VDP_ASSERT((v0_ih+1)%4 == 0);
    }

   VDP_ASSERT((V0_VFPOS.bits.video_xfpos)%2 == 0);
   VDP_ASSERT((V0_VFPOS.bits.video_yfpos)%2 == 0);
   VDP_ASSERT((V0_VLPOS.bits.video_xlpos+1)%2 == 0);
   VDP_ASSERT((V0_VLPOS.bits.video_ylpos+1)%2 == 0);
   VDP_ASSERT((V0_DFPOS.bits.disp_xfpos)%2 == 0);
   VDP_ASSERT((V0_DFPOS.bits.disp_yfpos)%2 == 0);
   VDP_ASSERT((V0_DLPOS.bits.disp_xlpos+1)%2 == 0);
   VDP_ASSERT((V0_DLPOS.bits.disp_ylpos+1)%2 == 0);
    
//    VDP_ASSERT(!((disp_mode != VDP_DISP_MODE_2D) && (v0_data_width == 1) && (V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && ((v0_iw+1)%4 != 0)));   //3D, 10bit, sp444, 
    
    if(disp_mode == VDP_DISP_MODE_TAB)
    {
       VDP_ASSERT(v0_iw >= 63);
    }
	
    //------------------------------------------------------------------------------------
	//step 2.7: Tile
	//------------------------------------------------------------------------------------
    if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64)
    {
#if EDA_TEST
        cout << hex << "V0_P0LADDR = " << V0_P0LADDR.u32 <<endl;
        cout << hex << "V0_P0CADDR = " << V0_P0CADDR.u32 <<endl;
        cout << hex << "V0_P0STRIDE.bits.surface_stride = " << V0_P0STRIDE.bits.surface_stride <<endl;
        cout << hex << "V0_P0STRIDE.bits.surface_cstride = " << V0_P0STRIDE.bits.surface_cstride <<endl;
        cout << dec << "V0_IRESO.bits.iw = " << V0_IRESO.bits.iw << endl;
        cout << dec << "V0_TILE_LOFFSET.bits.l_x_offset = " << V0_TILE_LOFFSET.bits.l_x_offset << endl;
#endif

        VDP_ASSERT(V0_P0STRIDE.bits.surface_stride%16 == 0);
        VDP_ASSERT(V0_P0STRIDE.bits.surface_cstride%16 == 0);
        VDP_ASSERT(!(V0_P0STRIDE.bits.surface_stride < V0_IRESO.bits.iw*v0_data_width/8));   
        VDP_ASSERT(!(V0_P0STRIDE.bits.surface_cstride < V0_IRESO.bits.iw*v0_data_width/8));   

        VDP_ASSERT(V0_P0LADDR.u32%16 == 0);
        VDP_ASSERT(V0_P0CADDR.u32%16 == 0);
        VDP_ASSERT(disp_mode == VDP_DISP_MODE_2D);
        
        if(v0_data_width == 1)
        {
            VDP_ASSERT(V0_NADDR.u32 %16 == 0);  
            VDP_ASSERT(V0_NCADDR.u32%16 == 0);
            VDP_ASSERT(V0_NADDR.u32     != 0);  
            VDP_ASSERT(V0_NCADDR.u32    != 0);
        }

        VDP_ASSERT(V0_TILE_LOFFSET.bits.l_x_offset % 16 == 0);
        VDP_ASSERT(V0_TILE_COFFSET.bits.c_x_offset % 16 == 0);
        VDP_ASSERT(V0_TILE_LOFFSET.bits.l_y_offset < 16);
        VDP_ASSERT(V0_TILE_COFFSET.bits.c_y_offset < 8);
    }

    //------------------------------------------------------------------------------------
	//step 2.8: flip
	//------------------------------------------------------------------------------------
	v0_flip_en = V0_CTRL.bits.flip_en;

	if(v0_flip_en == 1)
	{
		//flip enable,input 444, chroma_start_addr = source_addr + chrom_stride * height * 2
// 		HI_U32 chrom_start_addr;
	//VDP_ASSERT(!((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && (V0_VSP.bits.zme_out_fmt != VDP_PROC_FMT_SP_444)));
	}

    //------------------------------------------------------------------------------------
	//step 2.9: DCMP
	//------------------------------------------------------------------------------------
    if(V0_CTRL.bits.dcmp_en == 1)
    {
//        VDP_ASSERT((v0_iw > 62) && (v0_ih > 30));
//        VDP_ASSERT((v0_iw < 3839) && (v0_ih < 2159));
       VDP_ASSERT(!mrg_mode);
        if(v0_iw < 127)
        {
//           VDP_ASSERT(V0_DCMP_CTRL.bits.is_lossless_l && V0_DCMP_CTRL.bits.is_lossless_c);
        }
    }
	//------------------------------------------------------------------------------------
	//ZME
	//------------------------------------------------------------------------------------
    v0_hratio = V0_HSP.bits.hratio;
    v0_vratio = V0_VSR.bits.vratio;
    
//    VDP_ASSERT(!((v0_iw > 3839) && (V0_HSP.bits.hlmsc_en==0x1)));
    //video infmt 420,zme chrom vert enbale

#if EDA_TEST
//     cout << "v0_vratio = " << v0_vratio << endl;
//     cout << "vfir_1tap_en = " << V0_VSP.bits.vfir_1tap_en <<endl;
#endif

    if((u32Data == VDP_LAYER_VID0) | (u32Data == VDP_LAYER_VID1))
    {
        if((DHD0_VSYNC.bits.vact >= 3839) && (v0_iw >= 3839))
        {
            if((v0_vratio >= 4096*5/2) && (v0_vratio <= 4096*3))
            {
                VDP_ASSERT(V0_VSP.bits.vtap_reduce == 0x1);
            }
        }

        if(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_420)
        {
            VDP_ASSERT(V0_VSP.bits.vchmsc_en == 1);
        }

        if(V0_VSP.bits.vfir_1tap_en == 0x1)
        {
            VDP_ASSERT((V0_VSP.bits.vlfir_en && V0_VSP.bits.vchfir_en) && (v0_vratio == 4096));
        }
    }
    
    if(v0_zme_en == 1)
    {
        //VDP_ASSERT(!((disp_mode == VDP_DISP_MODE_SBS) && (v0_vratio > 8192)));
        VDP_ASSERT(!((disp_mode == VDP_DISP_MODE_TAB) && (v0_iw < 63)));
        if(u32Data == VDP_LAYER_VID0)
        {
            VDP_ASSERT(V0_HSP.bits.hfir_order == 0x1);   //3798CV200, ZME only support hfir_order == 0x1
        }
        //VDP_ASSERT(!((v0_iw > 3839) && (V0_HSP.bits.hfir_order != 0x0)));

        if(u32Data == VDP_LAYER_VID1)
        {
            VDP_ASSERT(!((V0_VSP.bits.vlmsc_en == 0x1) && (v0_crop_ow > 2048)));   //3798CV200, CVFIR support ZME
        }

        if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_420 || V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE || V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64) 
        {
            VDP_ASSERT(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_420);
        }
        else if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_422)
        {
            VDP_ASSERT(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_422);
        }
        else if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444)
        {
            VDP_ASSERT(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_444);
        }
        else if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_400)
        {
            VDP_ASSERT(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_422);
        }

        //zme enable, input 420, out put 422
        //zme enable, input 422, out put 422
        //zme enable, input 444, out put 444
        //VDP_ASSERT(!((V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_422) && (V0_VSP.bits.zme_out_fmt != VDP_PROC_FMT_SP_422)));
        //VDP_ASSERT(!((V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_420) && (V0_VSP.bits.zme_out_fmt != VDP_PROC_FMT_SP_422)));
        //VDP_ASSERT(!((V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_444) && (V0_VSP.bits.zme_out_fmt != VDP_PROC_FMT_SP_444)));

        if(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_422)
        {
            VDP_ASSERT(V0_VSP.bits.zme_out_fmt == VDP_PROC_FMT_SP_422);
        }
        else if(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_420)
        {
            //VDP_ASSERT(V0_VSP.bits.zme_out_fmt == VDP_PROC_FMT_SP_422);
        }
        else if(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_444)
        {
            VDP_ASSERT(V0_VSP.bits.zme_out_fmt == VDP_PROC_FMT_SP_444);
        }

    }
	
    if(v0_zme2_en == 1)
    {
        VDP_ASSERT(V0_ZME2_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_422);
        VDP_ASSERT(V0_ZME2_VSP.bits.zme_out_fmt == VDP_PROC_FMT_SP_422);
        if(disp_mode == VDP_DISP_MODE_2D)
        {
            // VDP_ASSERT((v0_ow == 1919) || (v0_ow == 2047));
            VDP_ASSERT((v0_ow+1)%8 == 0);
            VDP_ASSERT(V0_ZME2_HSP.bits.hfir_order == 0x1);   //3798CV200, ZME only support hfir_order == 0x1
        }
    }
    
    //------------------------------------------------------------------------------------
	//sharpen_4K, assertion
	//------------------------------------------------------------------------------------
    if(v0_ow > 1920)
    {
        VDP_ASSERT(SPCTRL.bits.sharpen_en != 1);
    }
    
    if(SPCTRL.bits.sharpen_en)
    {
        if(disp_mode == VDP_DISP_MODE_SBS)
        {
            VDP_ASSERT((v0_ow+1)%4 == 0);
            VDP_ASSERT(v0_ow >= 48);
        }

        if(SPCTRL.bits.debug_en)
        {
            VDP_ASSERT(disp_mode == VDP_DISP_MODE_2D);
            VDP_ASSERT((v0_ow+1)%4 == 0);
        }

        VDP_ASSERT(disp_mode != VDP_DISP_MODE_TAB);
        VDP_ASSERT(disp_mode != VDP_DISP_MODE_LBL);

    }
	//------------------------------------------------------------------------------------
	//IFIR
	//------------------------------------------------------------------------------------
    //video infmt 420/422, ifir enable
    //video infmt 444, ifir unenable	
    if((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_420) || (V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_422))
    {
        VDP_ASSERT(V0_CTRL.bits.ifir_mode != 0x0);
    }
    else if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444)
    {
        VDP_ASSERT(V0_CTRL.bits.ifir_mode == 0x0);
    }
//    VDP_ASSERT(V0_CTRL.bits.ifir_mode != 0x0);	
	
	//------------------------------------------------------------------------------------
	//ADDRESS
	//------------------------------------------------------------------------------------
    if(((u32Data == VDP_LAYER_VID0) || (u32Data == VDP_LAYER_VID3)) || (((u32Data == VDP_LAYER_VID1) || (u32Data == VDP_LAYER_VID4)) && (V0_16REGIONENL.u32 == 0x0)))   //v0/v3 or v1/v4 multi region disable
    {
        //intput 8bit mode, input addr should be 128bit align(low 1bit must 0)
//        VDP_ASSERT(!((V0_CTRL.bits.data_width == 0x0) && (((V0_P0LADDR.u32 | V0_NADDR.u32) & 0xf) != 0x0)));  
//        VDP_ASSERT(!((V0_CTRL.bits.data_width == 0x0) && (((V0_P0CADDR.u32 | V0_NCADDR.u32) & 0xf) != 0x0))); 

       VDP_ASSERT(((V0_P0LADDR.u32 | V0_NADDR.u32)  & 0xf) == 0x0);  
       VDP_ASSERT(((V0_P0CADDR.u32 | V0_NCADDR.u32) & 0xf) == 0x0); 
       
       //all stride should be 128bit align(low 4bit must 0) 
//        VDP_ASSERT(!((V0_CTRL.bits.surface_en == 0x1) && (((V0_P0STRIDE.bits.surface_stride & 0xf) != 0x0) || ((V0_P0STRIDE.bits.surface_cstride & 0xf) != 0x0))));
       VDP_ASSERT((V0_P0STRIDE.bits.surface_stride  & 0xf) == 0x0);
       VDP_ASSERT((V0_P0STRIDE.bits.surface_cstride & 0xf) == 0x0);


       if(v0_data_width == 0)
       {
            //420/422 input , lu/ch stride  >= input width
           VDP_ASSERT(!((V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_stride < V0_IRESO.bits.iw)));   
           VDP_ASSERT(!((V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_cstride < V0_IRESO.bits.iw)));   
            
            //444 input , lu stride  > output width , ch stride > 2* output width 
           VDP_ASSERT(!((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_stride < V0_IRESO.bits.iw)));
           VDP_ASSERT(!((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_cstride < 2*V0_IRESO.bits.iw)));
       }
       else 
       {
           if(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64)
           {
               VDP_ASSERT(!((V0_P0STRIDE.bits.surface_stride < V0_IRESO.bits.iw)));   
               VDP_ASSERT(!((V0_P0STRIDE.bits.surface_cstride < V0_IRESO.bits.iw)));   
           }
           else
           {
               VDP_ASSERT(!((V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_stride*8/10 < V0_IRESO.bits.iw)));   
               VDP_ASSERT(!((V0_CTRL.bits.ifmt != VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_cstride*8/10 < V0_IRESO.bits.iw)));   
           }
            
            //444 input , lu stride  > output width , ch stride > 2* output width 
           VDP_ASSERT(!((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_stride*8/10 < V0_IRESO.bits.iw)));
           VDP_ASSERT(!((V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_444) && (V0_P0STRIDE.bits.surface_cstride*8/10 < 2*V0_IRESO.bits.iw)));

       }
    }

    //420 input , height , ih = real_height - 1
    //progressive:(ih+1)%2 == 0;interlace:(ih+1)%4 == 0;
    VDP_ASSERT(!((V0_CTRL.bits.ifmt  == VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x1) && (V0_IRESO.bits.ih %2 != 1))); 
    VDP_ASSERT(!((V0_CTRL.bits.ifmt  == VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x1) && (V0_IRESO.bits.ih %2 != 1)));

    VDP_ASSERT(!((V0_CTRL.bits.ifmt  == VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x0) && ((V0_IRESO.bits.ih+1) %4 != 0))); 
    VDP_ASSERT(!((V0_CTRL.bits.ifmt  == VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x0) && ((V0_IRESO.bits.ih+1) %4 != 0)));

    VDP_ASSERT(!((V0_CTRL.bits.ifmt  != VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x1) && (V0_IRESO.bits.ih %2 != 1))); 
    VDP_ASSERT(!((V0_CTRL.bits.ifmt  != VDP_VID_IFMT_SP_420) && (DHD0_CTRL.bits.iop == 0x1) && (V0_IRESO.bits.ih %2 != 1)));

	
	//------------------------------------------------------------------------------------
	//V0_LBOX
	//------------------------------------------------------------------------------------
    
    if(((u32Data == VDP_LAYER_VID1) || (u32Data == VDP_LAYER_VID4)) && mrg_mode)
     {
        VDP_ASSERT((V0_DLPOS.bits.disp_xlpos - V0_DFPOS.bits.disp_xfpos) == v0_ow);
        VDP_ASSERT((V0_DLPOS.bits.disp_ylpos - V0_DFPOS.bits.disp_yfpos) == v0_oh);
         //mrg_mode == 1; video = disp; b02 only V1/V4 support muilt region, and muilt region nonsupport lbox
         VP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32)));
//         VDP_ASSERT((V0_DLPOS.bits.disp_xlpos - V0_DFPOS.bits.disp_xfpos) == VP0_IRESO.bits.iw);
//         VDP_ASSERT((V0_DLPOS.bits.disp_ylpos - V0_DFPOS.bits.disp_yfpos) == VP0_IRESO.bits.ih);
     }
    
    if(disp_mode == VDP_DISP_MODE_SBS)
    {
       VDP_ASSERT(V0_DFPOS.bits.disp_xfpos%4 == 0x0);
//        VDP_ASSERT(V0_DFPOS.bits.disp_yfpos%4 == 0x0);
    }

    //video VX >= DSX; VY >= DSY
    VDP_ASSERT(V0_VFPOS.bits.video_xfpos >= V0_DFPOS.bits.disp_xfpos);
    VDP_ASSERT(V0_VFPOS.bits.video_yfpos >= V0_DFPOS.bits.disp_yfpos);

    //video DXL-DXS >= output width  ;DYL-DYS >= outputheight
    //video DXL >= VX + output width ;DYL >= VY + outputheight
#if EDA_TEST
    //cout << "V0_DLPOS.bits.disp_xlpos-V0_DFPOS.bits.disp_xfpos:" << V0_DLPOS.bits.disp_xlpos-V0_DFPOS.bits.disp_xfpos << endl;
    //cout << "V0_VFPOS.bits.video_xfpos:" << V0_VFPOS.bits.video_xfpos << endl;
    //cout << "V0_ORESO.bits.ow:" << V0_ORESO.bits.ow<< endl;
#endif
    VDP_ASSERT(V0_DLPOS.bits.disp_xlpos >= (V0_ORESO.bits.ow + V0_VFPOS.bits.video_xfpos)); 
    VDP_ASSERT(V0_DLPOS.bits.disp_ylpos >= (V0_ORESO.bits.oh + V0_VFPOS.bits.video_yfpos)); 
    VDP_ASSERT((V0_DLPOS.bits.disp_xlpos - V0_DFPOS.bits.disp_xfpos) >= V0_ORESO.bits.ow ); 
    VDP_ASSERT((V0_DLPOS.bits.disp_ylpos - V0_DFPOS.bits.disp_yfpos) >= V0_ORESO.bits.oh ); 

    //------------------------------------------------------------------------------------
	//3D process
	//------------------------------------------------------------------------------------
    
     if(disp_mode != VDP_DISP_MODE_2D)
     {
//         VDP_ASSERT(!(V0_VSP.bits.zme_in_fmt == VDP_PROC_FMT_SP_444));
        VDP_ASSERT(!(V0_CTRL.bits.ifmt == VDP_VID_IFMT_SP_TILE_64));
     }
     //------------------------------------------------------------------------------------
     //tnr snrVDP_ASSERT
     //------------------------------------------------------------------------------------

    if((u32Data == VDP_LAYER_VID0))
    {
        //VDP_ASSERT( V0_PRERD.bits.tnr_en == V0_PRERD.bits.snr_en ); 

        //------------------------------------------------------------------------------------
        //dbm dbdetectVDP_ASSERT
        //------------------------------------------------------------------------------------
        U_V0_DB_CTRL             V0_DB_CTRL;
        U_V0_DBD_LUTWGT0         V0_DBD_LUTWGT0;
        U_V0_DBD_LUTWGT1         V0_DBD_LUTWGT1;
        U_V0_DBD_HCRDX8BINLUT0   V0_DBD_HCRDX8BINLUT0;
        U_V0_DBD_HCRDX8BINLUT1   V0_DBD_HCRDX8BINLUT1;
        U_V0_DBD_HCRDX8BINLUT2   V0_DBD_HCRDX8BINLUT2;
        U_V0_DBD_HCRDX8BINLUT3   V0_DBD_HCRDX8BINLUT3;
        U_V0_DBD_HYRDX8BINLUT0   V0_DBD_HYRDX8BINLUT0;
        U_V0_DBD_HYRDX8BINLUT1   V0_DBD_HYRDX8BINLUT1;
        U_V0_DBD_HYRDX8BINLUT2   V0_DBD_HYRDX8BINLUT2;
        U_V0_DBD_HYRDX8BINLUT3   V0_DBD_HYRDX8BINLUT3;
        U_V0_DBD_VYRDX8BINLUT0   V0_DBD_VYRDX8BINLUT0;
        U_V0_DBD_VYRDX8BINLUT1   V0_DBD_VYRDX8BINLUT1;
        U_V0_DBD_VYRDX8BINLUT2   V0_DBD_VYRDX8BINLUT2;
        U_V0_DBD_VYRDX8BINLUT3   V0_DBD_VYRDX8BINLUT3;
        U_V0_DBD_BLKSIZE         V0_DBD_BLKSIZE;
        U_V0_DB_CTRL1            V0_DB_CTRL1;


        V0_DB_CTRL.u32           = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL.u32)            + u32Data * VID_OFFSET)); 
        V0_DBD_LUTWGT0.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT0.u32)        + u32Data * VID_OFFSET));
        V0_DBD_LUTWGT1.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_LUTWGT1.u32)        + u32Data * VID_OFFSET));
        V0_DBD_HCRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT0.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HCRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT1.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HCRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT2.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HCRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HCRDX8BINLUT3.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HYRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT0.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HYRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT1.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HYRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT2.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_HYRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_HYRDX8BINLUT3.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_VYRDX8BINLUT0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT0.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_VYRDX8BINLUT1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT1.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_VYRDX8BINLUT2.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT2.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_VYRDX8BINLUT3.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_VYRDX8BINLUT3.u32 ) + u32Data * VID_OFFSET));
        V0_DBD_BLKSIZE.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DBD_BLKSIZE.u32       ) + u32Data * VID_OFFSET));
        V0_DB_CTRL1.u32          = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_DB_CTRL1.u32)           + u32Data * VID_OFFSET)); 





        //VDP_ASSERT(V0_DB_CTRL.bits.db_chr_ver_en ==0);                                           
        if((V0_DB_CTRL.bits.db_lum_ver_en ==1) || (V0_DB_CTRL.bits.db_lum_hor_en ==1))
            //         ||(V0_DB_CTRL.bits.db_chr_hor_en ==1))
        { 
            if(V0_DB_CTRL1.bits.db_det_en == 1 && (((V0_ORESO.bits.ow+1) >1920)||( (V0_ORESO.bits.oh +1)>1080)))
            {
                VDP_ASSERT((V0_DB_CTRL.bits.db_lum_ver_en ==0)); 
                //        && (V0_DB_CTRL.bits.db_chr_hor_en ==0));     
            }

        }
        if(V0_PRERD.bits.dbm_en ==1)
        {
            VDP_ASSERT((V0_ORESO.bits.ow+1) >=128); 
            VDP_ASSERT((V0_ORESO.bits.oh+1) >=64); 
        }

        if(( V0_PRERD.bits.dbm_det_en == 1) || ( V0_DB_CTRL.bits.det_hy_en==1) 
                || (V0_DB_CTRL.bits.det_hc_en == 1) || (V0_DB_CTRL.bits.det_vy_en ==1))
        { 
#if 0
            VDP_ASSERT(( V0_DBD_LUTWGT1.bits.det_lut_wgt15 >= V0_DBD_LUTWGT1.bits.det_lut_wgt14 )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt14 >= V0_DBD_LUTWGT1.bits.det_lut_wgt13 )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt13 >= V0_DBD_LUTWGT1.bits.det_lut_wgt12 )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt12 >= V0_DBD_LUTWGT1.bits.det_lut_wgt11 )&& 
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt11 >= V0_DBD_LUTWGT1.bits.det_lut_wgt10 )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt10 >= V0_DBD_LUTWGT1.bits.det_lut_wgt9  )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt9  >= V0_DBD_LUTWGT1.bits.det_lut_wgt8  )&&
                    ( V0_DBD_LUTWGT1.bits.det_lut_wgt8  >= V0_DBD_LUTWGT0.bits.det_lut_wgt7  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt7  >= V0_DBD_LUTWGT0.bits.det_lut_wgt6  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt6  >= V0_DBD_LUTWGT0.bits.det_lut_wgt5  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt5  >= V0_DBD_LUTWGT0.bits.det_lut_wgt4  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt4  >= V0_DBD_LUTWGT0.bits.det_lut_wgt3  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt3  >= V0_DBD_LUTWGT0.bits.det_lut_wgt2  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt2  >= V0_DBD_LUTWGT0.bits.det_lut_wgt1  )&&
                    ( V0_DBD_LUTWGT0.bits.det_lut_wgt1  >= V0_DBD_LUTWGT0.bits.det_lut_wgt0  ));
            VDP_ASSERT((V0_DBD_HCRDX8BINLUT3.bits.hc_rdx8bin_lut6 > V0_DBD_HCRDX8BINLUT2.bits.hc_rdx8bin_lut5 )&&
                    (V0_DBD_HCRDX8BINLUT2.bits.hc_rdx8bin_lut5 > V0_DBD_HCRDX8BINLUT2.bits.hc_rdx8bin_lut4 )&&
                    (V0_DBD_HCRDX8BINLUT2.bits.hc_rdx8bin_lut4 > V0_DBD_HCRDX8BINLUT1.bits.hc_rdx8bin_lut3 )&&
                    (V0_DBD_HCRDX8BINLUT1.bits.hc_rdx8bin_lut3 > V0_DBD_HCRDX8BINLUT1.bits.hc_rdx8bin_lut2 )&&
                    (V0_DBD_HCRDX8BINLUT1.bits.hc_rdx8bin_lut2 > V0_DBD_HCRDX8BINLUT0.bits.hc_rdx8bin_lut1 )&&
                    (V0_DBD_HCRDX8BINLUT0.bits.hc_rdx8bin_lut1 > V0_DBD_HCRDX8BINLUT0.bits.hc_rdx8bin_lut0 ));


            VDP_ASSERT((V0_DBD_HYRDX8BINLUT3.bits.hy_rdx8bin_lut6 > V0_DBD_HYRDX8BINLUT2.bits.hy_rdx8bin_lut5 )&&
                    (V0_DBD_HYRDX8BINLUT2.bits.hy_rdx8bin_lut5 > V0_DBD_HYRDX8BINLUT2.bits.hy_rdx8bin_lut4 )&&
                    (V0_DBD_HYRDX8BINLUT2.bits.hy_rdx8bin_lut4 > V0_DBD_HYRDX8BINLUT1.bits.hy_rdx8bin_lut3 )&&
                    (V0_DBD_HYRDX8BINLUT1.bits.hy_rdx8bin_lut3 > V0_DBD_HYRDX8BINLUT1.bits.hy_rdx8bin_lut2 )&&
                    (V0_DBD_HYRDX8BINLUT1.bits.hy_rdx8bin_lut2 > V0_DBD_HYRDX8BINLUT0.bits.hy_rdx8bin_lut1 )&&
                    (V0_DBD_HYRDX8BINLUT0.bits.hy_rdx8bin_lut1 > V0_DBD_HYRDX8BINLUT0.bits.hy_rdx8bin_lut0 ));


            VDP_ASSERT((V0_DBD_VYRDX8BINLUT3.bits.vy_rdx8bin_lut6 > V0_DBD_VYRDX8BINLUT2.bits.vy_rdx8bin_lut5 )&&
                    (V0_DBD_VYRDX8BINLUT2.bits.vy_rdx8bin_lut5 > V0_DBD_VYRDX8BINLUT2.bits.vy_rdx8bin_lut4 )&&
                    (V0_DBD_VYRDX8BINLUT2.bits.vy_rdx8bin_lut4 > V0_DBD_VYRDX8BINLUT1.bits.vy_rdx8bin_lut3 )&&
                    (V0_DBD_VYRDX8BINLUT1.bits.vy_rdx8bin_lut3 > V0_DBD_VYRDX8BINLUT1.bits.vy_rdx8bin_lut2 )&&
                    (V0_DBD_VYRDX8BINLUT1.bits.vy_rdx8bin_lut2 > V0_DBD_VYRDX8BINLUT0.bits.vy_rdx8bin_lut1 )&&
                    (V0_DBD_VYRDX8BINLUT0.bits.vy_rdx8bin_lut1 > V0_DBD_VYRDX8BINLUT0.bits.vy_rdx8bin_lut0 ));
#endif

            if(( (V0_ORESO.bits.ow+1)>1920)||((V0_ORESO.bits.oh +1)>1080) )
            {
                VDP_ASSERT(( V0_DB_CTRL.bits.det_hc_en== 0) && (V0_DB_CTRL.bits.det_vy_en == 0));

            }
#if 0
            VDP_ASSERT(V0_DBD_BLKSIZE.bits.vy_max_blk_size >=8);
            VDP_ASSERT(V0_DBD_BLKSIZE.bits.vy_max_blk_size <=64);
            VDP_ASSERT((V0_DBD_BLKSIZE.bits.hy_max_blk_size >=8));
            VDP_ASSERT((V0_DBD_BLKSIZE.bits.hy_max_blk_size <=64));
            VDP_ASSERT((V0_DBD_BLKSIZE.bits.hc_max_blk_size >=8));
            VDP_ASSERT((V0_DBD_BLKSIZE.bits.hc_max_blk_size <=64));
            VDP_ASSERT((V0_DBD_BLKSIZE.bits.min_blk_size >=4));
            VDP_ASSERT(V0_DBD_BLKSIZE.bits.hy_max_blk_size   >= V0_DBD_BLKSIZE.bits.min_blk_size );
            VDP_ASSERT(V0_DBD_BLKSIZE.bits.hc_max_blk_size   >= V0_DBD_BLKSIZE.bits.min_blk_size );
            VDP_ASSERT(V0_DBD_BLKSIZE.bits.vy_max_blk_size   >= V0_DBD_BLKSIZE.bits.min_blk_size );
#endif
            if(V0_PRERD.bits.dbm_det_en ==0)
            {
                //VDP_ASSERT((V0_DB_CTRL.bits.det_hy_en ==0) && ( V0_DB_CTRL.bits.det_hc_en  == 0) && ( V0_DB_CTRL.bits.det_vy_en ==0));
                VDP_ASSERT((V0_DB_CTRL.bits.det_hy_en ==0));
                VDP_ASSERT((V0_DB_CTRL.bits.det_hc_en ==0));
                VDP_ASSERT((V0_DB_CTRL.bits.det_vy_en ==0));
            }
            if(V0_PRERD.bits.dbm_det_en ==1)
            {
                VDP_ASSERT(V0_PRERD.bits.dbm_en ==1);
            }
        }

    }

	return HI_SUCCESS;	
}

//add by chenlunhai
HI_S32 VDP_WBC_ConfigCheck(HI_U32 u32Data)
{
	    HI_U32 wbc_ireso_w = 0;
	    HI_U32 wbc_ireso_h = 0;
		U_WBC_DHD0_CTRL    WBC_DHD0_CTRL;
		U_WBC_DHD_LOCATE   WBC_DHD_LOCATE;
		U_DHD0_CTRL        DHD0_CTRL;
		U_WBC_DHD0_ORESO   WBC_DHD0_ORESO;
		U_WBC_DHD0_STRIDE  WBC_DHD0_STRIDE;   
		U_WBC_DHD0_YADDR   WBC_DHD0_YADDR;
		U_WBC_DHD0_CADDR   WBC_DHD0_CADDR;
// 		U_WBC_DHD0_HPZME   WBC_DHD0_HPZME;
		U_WBC_DHD0_ZME_HSP WBC_DHD0_ZME_HSP;
		U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;
		U_WBC_DHD0_DITHER_CTRL WBC_DHD0_DITHER_CTRL;
		U_DHD0_HSYNC1       DHD0_HSYNC1;
		U_VP0_DFPOS         VP0_DFPOS;
		U_VP0_DLPOS         VP0_DLPOS;
		U_V0_VFPOS          V0_VFPOS;
		U_V0_VLPOS          V0_VLPOS;
        U_V0_CPOS           V0_CPOS;
        U_V0_IRESO          V0_IRESO;
        U_V0_ORESO          V0_ORESO;
        U_VP0_IRESO         VP0_IRESO;
	    U_WBC_DHD0_HCDS     WBC_DHD0_HCDS;
        U_V0_HSP            V0_HSP;
        U_V0_VSP            V0_VSP;

	    WBC_DHD0_CTRL.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32)     + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD_LOCATE.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD_LOCATE.u32)))                           ; 
	    WBC_DHD0_ORESO.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_ORESO.u32)    + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_YADDR.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_YADDR.u32)    + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_CADDR.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CADDR.u32)    + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_STRIDE.u32      = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_STRIDE.u32)   + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_ZME_HSP.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_HSP.u32)  + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_ZME_VSP.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_ZME_VSP.u32)  + u32Data * WBC_OFFSET)) ; 
	    WBC_DHD0_DITHER_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_DITHER_CTRL.u32)))                     ; 
	    DHD0_CTRL.u32            = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)))                                ; 
	    DHD0_HSYNC1.u32          = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)))                              ; 
	    VP0_DFPOS.u32            = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DFPOS.u32)))                                ; 
	    VP0_DLPOS.u32            = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_DLPOS.u32)))                                ; 
	    V0_VFPOS.u32             = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VFPOS.u32)))                                 ; 
	    V0_VLPOS.u32             = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VLPOS.u32)))                                 ; 
        
        V0_CPOS.u32              = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CPOS.u32)))                                  ; 
	    V0_IRESO.u32             = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_IRESO.u32)))                                 ; 
	    V0_ORESO.u32             = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_ORESO.u32)))                                 ; 
	    VP0_IRESO.u32            = VDP_RegRead(((HI_ULONG)&(pVdpReg->VP0_IRESO.u32)))                                ; 
	    WBC_DHD0_HCDS.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_HCDS.u32)))                            ; 
	    V0_HSP.u32               = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_HSP.u32) + VDP_LAYER_VID3 * VID_OFFSET))     ; 
	    V0_VSP.u32               = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_VSP.u32) + VDP_LAYER_VID3 * VID_OFFSET))     ; 

        //---------------------------------------------------------
        //addr and stride 16bytes match,stride > width of each row
        //---------------------------------------------------------
        VDP_ASSERT((WBC_DHD0_YADDR.bits.wbcaddr % 0x10) == 0x0);
        VDP_ASSERT((WBC_DHD0_CADDR.bits.wbccaddr % 0x10) == 0x0);
        VDP_ASSERT((WBC_DHD0_STRIDE.bits.wbclstride % 0x10) == 0x0);
        VDP_ASSERT((WBC_DHD0_STRIDE.bits.wbccstride % 0x10) == 0x0);
        
        if( DHD0_CTRL.bits.p2i_en == 1 )
        {
            if(WBC_DHD0_CTRL.bits.wbc_en == 1)
            {
                VDP_ASSERT((WBC_DHD0_CTRL.bits.mode_out != 0));
            }
        }

        //---------------------------------------------------------
        // dither mode must be set 1  when dither enable
        //---------------------------------------------------------
        if(WBC_DHD0_DITHER_CTRL.bits.dither_en == 1)
        {
            VDP_ASSERT((WBC_DHD0_DITHER_CTRL.bits.dither_mode == 1));
        }

        if(u32Data == 0)   // WBC_DHD
        {
            //wbc outreso max : 1920*1080
            VDP_ASSERT(((WBC_DHD0_ORESO.bits.ow +1)  <= 1920));
            VDP_ASSERT(((WBC_DHD0_ORESO.bits.oh +1)  <= 1080));
            //VDP_ASSERT(wbc_ireso_w <= 4096);
            //VDP_ASSERT(wbc_ireso_h <= 2360);
            VDP_ASSERT(WBC_DHD0_HCDS.bits.hcds_en == HI_TRUE);
        }
        else if(u32Data == 1)   // WBC_GP
        {
            //wbc outreso max : 720*576
            VDP_ASSERT(((WBC_DHD0_ORESO.bits.ow +1)  <= 720));
            VDP_ASSERT(((WBC_DHD0_ORESO.bits.oh +1)  <= 576));
            //VDP_ASSERT(wbc_ireso_w <= 4096);
            //VDP_ASSERT(wbc_ireso_h <= 2304);
        }
        else if(u32Data == 3)   // WBC_VP
        {
            if(WBC_DHD0_CTRL.bits.wbc_en == HI_TRUE)   
            {
                //-------------------------------------------- 
                //for general assertion
                //---------------------------------------------------------
                //out reso even reso + 1
                //---------------------------------------------------------
                VDP_ASSERT((WBC_DHD0_ORESO.bits.ow +1) % 2 == 0x0);
                //---------------------------------------------------------

                //-------------------------------------------- 
                //wbc_ireso from vp 0 and vid0 before zme 2 
                //---------------------------------------------------------
                //cout << "WBC_DHD0_CTRL.bits.wbc_point_sel = " << WBC_DHD0_CTRL.bits.wbc_point_sel << endl;
                if (WBC_DHD0_CTRL.bits.wbc_point_sel == VDP_LAYER_CONN_VPACSC) 
                {
                    wbc_ireso_w = VP0_IRESO.bits.iw + 1;
                    wbc_ireso_h = VP0_IRESO.bits.ih + 1;
                }    
                else if (WBC_DHD0_CTRL.bits.wbc_point_sel == VDP_LAYER_CONN_VIDAZME) 
                {
                    wbc_ireso_w = V0_ORESO.bits.ow + 1;
                    wbc_ireso_h = V0_ORESO.bits.oh + 1;
                }    
                //out reso : before zme &  vp : 4096*2304; 
                if ((WBC_DHD0_CTRL.bits.wbc_point_sel == VDP_LAYER_CONN_VPACSC) ||(WBC_DHD0_CTRL.bits.wbc_point_sel == VDP_LAYER_CONN_VIDBZME ))
                {
                    VDP_ASSERT(wbc_ireso_w <= 4096);
                    VDP_ASSERT(wbc_ireso_h <= 2360);
                }
                //wbc outreso max : 720*576
                VDP_ASSERT(((WBC_DHD0_ORESO.bits.ow +1)  <= 720));
                VDP_ASSERT(((WBC_DHD0_ORESO.bits.oh +1)  <= 576));
                //add connect to vp hcds must open; connect to zme after and before hcds must close
                if(WBC_DHD0_CTRL.bits.wbc_point_sel == VDP_LAYER_CONN_VPACSC)
                {
                    VDP_ASSERT(WBC_DHD0_HCDS.bits.hcds_en == HI_TRUE);
                }

                //---------------------------------------------------------
                //SBS:two eyes wbc out reso is 4 times ; one eye wbc out reso is 2 times
                //---------------------------------------------------------
                if (DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_SBS)
                {
                    if(WBC_DHD0_CTRL.bits.three_d_mode == VDP_WBC_3D_MODE_TWOEYE)
                    {
                        VDP_ASSERT(((WBC_DHD0_ORESO.bits.ow +1) % 4 == 0x0));
                    }
                    else
                    {
                        VDP_ASSERT(((WBC_DHD0_ORESO.bits.ow +1) % 2 == 0x0));
                    }
                }
                //SBS TAB FP must singel eye wbc
                if (DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_SBS || DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_TAB || DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_FP)
                {
                    VDP_ASSERT(WBC_DHD0_CTRL.bits.three_d_mode != VDP_WBC_3D_MODE_TWOEYE)
                }
                //fp not inter
                if (DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_FP)
                {
                    VDP_ASSERT((WBC_DHD0_CTRL.bits.mode_out == VDP_RMODE_PROGRESSIVE)||((WBC_DHD0_CTRL.bits.mode_out == VDP_RMODE_INTERFACE)&&(DHD0_CTRL.bits.iop == HI_TRUE)))
                }
                //---------------------------------------------------------
                //ZME must uvoder, if en not enable ,fir and mid must close
                //if fir not enable,mid must close
                //---------------------------------------------------------
            }
        }

		return HI_SUCCESS;	
}

// WBC_ASEERT END


HI_S32 VDP_GFX_ConfigCheck(HI_U32 u32Data)
{
#if EDA_TEST
    cout << "Gfx" << u32Data << " config check:"  << endl;
#endif
	
	U_G0_CTRL  G0_CTRL;
    U_G0_VFPOS G0_VFPOS;
    U_G0_VLPOS G0_VLPOS;
    U_G0_DFPOS G0_DFPOS;
    U_G0_DLPOS G0_DLPOS;
    U_G0_IRESO G0_IRESO;
    U_G0_DOF_CTRL  G0_DOF_CTRL;
    U_G0_DOF_STEP  G0_DOF_STEP;
    U_G0_CBMPARA G0_CBMPARA;

    U_GP0_VFPOS GP0_VFPOS;
    U_GP0_VLPOS GP0_VLPOS;
    U_GP0_DFPOS GP0_DFPOS;
    U_GP0_DLPOS GP0_DLPOS;
    U_GP0_IRESO GP0_IRESO;
    U_GP0_ORESO GP0_ORESO;

    U_G0_STRIDE G0_STRIDE;

	U_G0_SMMU_BYPASS0 G0_SMMU_BYPASS0;

    U_GP0_ZME_HSP GP0_ZME_HSP;
    U_GP0_ZME_VSP GP0_ZME_VSP;

	U_DHD0_CTRL DHD0_CTRL;
    U_DHD0_HSYNC1 DHD0_HSYNC1;

	U_SMMU_SCR SMMU_SCR;

    HI_U32 gfx_right_step_org ;
    HI_U32 gfx_left_step_org  ;
    HI_S32 gfx_right_step     ;
    HI_S32 gfx_left_step      ;

    HI_U32 mmu_co ;
    mmu_co = (u32Data < 4) ? 0 : 1 ; 

	G0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32) + u32Data * GFX_OFFSET));
    G0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_VFPOS.u32) + u32Data * GFX_OFFSET));
    G0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_VLPOS.u32) + u32Data * GFX_OFFSET));
    G0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DFPOS.u32)+ u32Data * GFX_OFFSET));
    G0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DLPOS.u32)+ u32Data * GFX_OFFSET));
    G0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_IRESO.u32)+ u32Data * GFX_OFFSET));
    G0_DOF_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_CTRL.u32))+ u32Data * GFX_OFFSET);
    G0_DOF_STEP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_DOF_STEP.u32))+ u32Data * GFX_OFFSET);
    G0_CBMPARA.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CBMPARA.u32)+ u32Data * GFX_OFFSET));
    G0_STRIDE.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_STRIDE.u32)+ u32Data * GFX_OFFSET));

    GP0_VFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VFPOS.u32) + mmu_co * GP_OFFSET));
    GP0_VLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_VLPOS.u32) + mmu_co * GP_OFFSET));
    GP0_DFPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_DFPOS.u32) + mmu_co * GP_OFFSET));
    GP0_DLPOS.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_DLPOS.u32) + mmu_co * GP_OFFSET));
    GP0_IRESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_IRESO.u32) + mmu_co * GP_OFFSET));
    GP0_ORESO.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ORESO.u32) + mmu_co * GP_OFFSET));


    GP0_ZME_HSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_HSP.u32) + mmu_co * GP_OFFSET));
    GP0_ZME_VSP.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->GP0_ZME_VSP.u32) + mmu_co * GP_OFFSET));

    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32)));
    DHD0_HSYNC1.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_HSYNC1.u32)+mmu_co*CHN_OFFSET));

	SMMU_SCR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SMMU_SCR.u32)));
	G0_SMMU_BYPASS0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_SMMU_BYPASS0.u32) + u32Data * GFX_OFFSET));

    if(G0_CTRL.bits.surface_en == 0x1)
    {
        if(u32Data == VDP_LAYER_GFX0)
        {
            if(G0_CTRL.bits.dcmp_en == 1)
            {
                VDP_ASSERT(G0_CTRL.bits.ifmt == 0x68);
                VDP_ASSERT(!((G0_CTRL.bits.lossless_a == 0x0) && (G0_CTRL.bits.lossless == 0x1)));
            }

        }
        if((u32Data == VDP_LAYER_GFX0) )//|| ((u32Data == VDP_LAYER_GFX1)))
        {
#if GFX_BT_TEST

#else
           VDP_ASSERT(((G0_IRESO.bits.iw+1) %2 == 0) && ((G0_IRESO.bits.iw+1) <= 1920));
           VDP_ASSERT(((G0_IRESO.bits.ih+1) %2 == 0) && ((G0_IRESO.bits.ih+1) <= 1080));
#endif
        }
        if(u32Data == VDP_LAYER_GFX1)
        {
           VDP_ASSERT((G0_IRESO.bits.iw+1) <= 1920);
           VDP_ASSERT((G0_IRESO.bits.ih+1) <= 1080);
        }
        if(u32Data == VDP_LAYER_GFX3)
        {
           VDP_ASSERT((G0_IRESO.bits.iw+1) <= 256);
           VDP_ASSERT((G0_IRESO.bits.ih+1) <= 256);
           VDP_ASSERT((G0_CTRL.bits.ifmt != 0x0) || (G0_CTRL.bits.ifmt != 0x10) || (G0_CTRL.bits.ifmt != 0x20)
                     || (G0_CTRL.bits.ifmt != 0x30) || (G0_CTRL.bits.ifmt != 0x38) || (G0_CTRL.bits.ifmt != 0x46));
        }
        if(u32Data == VDP_LAYER_GFX4)
        {
           VDP_ASSERT(((G0_IRESO.bits.iw+1) %2 == 0) && ((G0_IRESO.bits.iw+1) <= 720));
           VDP_ASSERT(((G0_IRESO.bits.ih+1) %2 == 0) && ((G0_IRESO.bits.ih+1) <= 576));
           VDP_ASSERT((G0_CTRL.bits.ifmt != 0x0) || (G0_CTRL.bits.ifmt != 0x10) || (G0_CTRL.bits.ifmt != 0x20)
                     || (G0_CTRL.bits.ifmt != 0x30) || (G0_CTRL.bits.ifmt != 0x38) || (G0_CTRL.bits.ifmt != 0x46));
        }
        if((u32Data == VDP_LAYER_GFX3) || (u32Data == VDP_LAYER_GFX4))
        {
           VDP_ASSERT((G0_CTRL.bits.req_ctrl == 0x0) || (G0_CTRL.bits.req_ctrl == 0x1));
        } 

        if((DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_2D))
        {
           VDP_ASSERT(G0_DOF_CTRL.bits.dof_en == 0x0);
        }
        if((DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_SBS))
        {
           VDP_ASSERT(((G0_VLPOS.bits.video_xlpos - G0_VFPOS.bits.video_xfpos) %2 == 1));
           VDP_ASSERT((GP0_IRESO.bits.iw+1) %4 == 0);
           //VDP_ASSERT((DHD0_HSYNC1.bits.hact+1) %4 == 0);
        }
        if((DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_TAB))
        {
           VDP_ASSERT(((G0_VLPOS.bits.video_ylpos - G0_VFPOS.bits.video_yfpos) %2 == 1));
           VDP_ASSERT((G0_IRESO.bits.iw+1) >= 2);
           VDP_ASSERT((G0_IRESO.bits.ih+1) >= 2);
           if((GP0_ZME_HSP.bits.hsc_en == 1) || (GP0_ZME_VSP.bits.vsc_en == 1))
           { 
//                VDP_ASSERT((GP0_ORESO.bits.oh+1) <= 2*(GP0_IRESO.bits.ih+1));
               VDP_ASSERT((GP0_IRESO.bits.iw+1) >= 64);
           }
           if(GP0_ZME_VSP.bits.vsc_en == 1)
           { 
               VDP_ASSERT((GP0_IRESO.bits.ih+1) <= 2*(GP0_ORESO.bits.oh+1));
           }
        }

        gfx_right_step_org = G0_DOF_STEP.bits.right_step;
        gfx_left_step_org  = G0_DOF_STEP.bits.left_step;
        gfx_right_step     = s32SignConv(gfx_right_step_org, 8);
        gfx_left_step      = s32SignConv(gfx_left_step_org, 8);

        if(G0_DOF_CTRL.bits.dof_en == 1)
        { 

            if((G0_IRESO.bits.iw+1) >= 50)
            {
                VDP_ASSERT((gfx_right_step <= 50) && (gfx_right_step >= (0-50)));
                VDP_ASSERT((gfx_left_step  <= 50) && (gfx_left_step  >= (0-50)));
            }
            else 
            {
                 VDP_ASSERT((gfx_right_step <= (G0_IRESO.bits.iw+1)) && (gfx_right_step >= (0-(G0_IRESO.bits.iw+1))));
                 VDP_ASSERT((gfx_left_step <= (G0_IRESO.bits.iw+1))  && (gfx_left_step >= (0-(G0_IRESO.bits.iw+1))));
            }
        }
        if((GP0_IRESO.bits.iw+1) != (GP0_ORESO.bits.ow+1))
        {
            VDP_ASSERT(GP0_ZME_HSP.bits.hsc_en == 1) ;
            VDP_ASSERT((GP0_IRESO.bits.iw+1) %2 == 0);
            if((DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_2D) && (DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_FP))
            {
                if(DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_SBS)
                    VDP_ASSERT((GP0_IRESO.bits.iw+1) %4 == 0);
            }
        }
        if((GP0_IRESO.bits.ih+1) != (GP0_ORESO.bits.oh+1))
        {
            VDP_ASSERT(GP0_ZME_VSP.bits.vsc_en == 1) ;
                VDP_ASSERT((GP0_IRESO.bits.ih+1) %2 == 0);
            if((DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_2D) && (DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_FP))
            {
                if(DHD0_CTRL.bits.disp_mode == VDP_DISP_MODE_TAB)
                    VDP_ASSERT((GP0_IRESO.bits.ih+1) %4 == 0);
            }
        }

       // if((SMMU_SCR.bits.glb_bypass == 0) && (DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_2D))
       // {
       //     VDP_ASSERT(G0_SMMU_BYPASS0.bits.r_bypass_3d == G0_SMMU_BYPASS0.bits.bypass_2d);
       // }
        if(SMMU_SCR.bits.glb_bypass == 1)
        {
            VDP_ASSERT(G0_SMMU_BYPASS0.bits.bypass_2d == 1);
            if(DHD0_CTRL.bits.disp_mode != VDP_DISP_MODE_2D)
            {
                VDP_ASSERT(G0_SMMU_BYPASS0.bits.r_bypass_3d == 1);
            }
        }
        // premult en
        if(G0_CTRL.bits.ifmt == 0x49)
        {
            VDP_ASSERT(G0_CBMPARA.bits.premult_en == 0);
        }
       // if(G0_CBMPARA.bits.premult_en == 1)
       // {
       //     VDP_ASSERT(G0_CBMPARA.bits.galpha != 1);
       // }

#if GFX_BT_TEST
#else
        //if(SMMU_SCR.bits.glb_bypass == 0)
        //{
        //    if((G0_SMMU_BYPASS0.bits.bypass_2d == 0) || (G0_SMMU_BYPASS0.bits.r_bypass_3d == 0))
        //    {
        //        VDP_ASSERT(G0_STRIDE.bits.surface_stride <= 0x400);
        //    }
        //}
#endif
       
        VDP_ASSERT(((GP0_IRESO.bits.iw+1) %2 == 0) && ((GP0_IRESO.bits.iw+1) <= 1920));
        VDP_ASSERT(((GP0_IRESO.bits.ih+1) %2 == 0) && ((GP0_IRESO.bits.ih+1) <= 1080));
        VDP_ASSERT(((GP0_ORESO.bits.ow+1) %2 == 0));
        VDP_ASSERT(((GP0_ORESO.bits.oh+1) %2 == 0));

        VDP_ASSERT((G0_VFPOS.bits.video_xfpos == G0_DFPOS.bits.disp_xfpos) &&
                  (G0_VFPOS.bits.video_yfpos == G0_DFPOS.bits.disp_yfpos) && 
                  (G0_VLPOS.bits.video_xlpos == G0_DLPOS.bits.disp_xlpos)  &&
                  (G0_VLPOS.bits.video_ylpos == G0_DLPOS.bits.disp_ylpos) );
        
        if(u32Data == VDP_LAYER_GP0)
        {
            VDP_ASSERT((GP0_VFPOS.bits.video_xfpos == GP0_DFPOS.bits.disp_xfpos) &&
                    (GP0_VFPOS.bits.video_yfpos == GP0_DFPOS.bits.disp_yfpos) && 
                    (GP0_VLPOS.bits.video_xlpos == GP0_DLPOS.bits.disp_xlpos)  &&
                    (GP0_VLPOS.bits.video_ylpos == GP0_DLPOS.bits.disp_ylpos) );
        
        }

        if(u32Data == VDP_LAYER_GFX4)
        {
            VDP_ASSERT((G0_IRESO.bits.iw == GP0_IRESO.bits.iw) && (G0_IRESO.bits.ih == GP0_IRESO.bits.ih));
        }
        else  
        {
            VDP_ASSERT((G0_IRESO.bits.iw <= GP0_IRESO.bits.iw) && (G0_IRESO.bits.ih <= GP0_IRESO.bits.ih));
        }


    }



	return HI_SUCCESS;

}

HI_S32 VDP_GP_ConfigCheck(HI_U32 u32Data)
{
#if EDA_TEST
    cout << "gp config check:"  << endl;
#endif
	
	return HI_FAILURE;

}

HI_S32 VDP_MMU_AddrCheck ()
{
#if EDA_TEST
    cout << "MMU Addr Config Check:"  << endl;

    U_SMMU_SCR             SMMU_SCR             ; 

    U_V0_CTRL              V0_CTRL              ; 
    U_G0_CTRL              G0_CTRL              ; 
    U_WBC_DHD0_CTRL        WBC_DHD0_CTRL        ; 
    U_WBC_GP0_CTRL         WBC_GP0_CTRL         ;
    U_V0_MULTI_MODE        V0_MULTI_MODE        ;
    U_V0_P0CTRL            V0_P0CTRL            ;
    U_V0_P1CTRL            V0_P1CTRL            ;
    U_V0_P2CTRL            V0_P2CTRL            ;
    U_V0_P3CTRL            V0_P3CTRL            ;
    U_V0_P4CTRL            V0_P4CTRL            ;
    U_V0_P5CTRL            V0_P5CTRL            ;
    U_V0_P6CTRL            V0_P6CTRL            ;
    U_V0_P7CTRL            V0_P7CTRL            ;
    U_V0_P8CTRL            V0_P8CTRL            ;
    U_V0_P9CTRL            V0_P9CTRL            ;
    U_V0_P10CTRL           V0_P10CTRL            ;
    U_V0_P11CTRL           V0_P11CTRL            ;
    U_V0_P12CTRL           V0_P12CTRL            ;
    U_V0_P13CTRL           V0_P13CTRL            ;
    U_V0_P14CTRL           V0_P14CTRL            ;
    U_V0_P15CTRL           V0_P15CTRL            ;

    U_V0_P0LADDR           V0_P0LADDR           ;
    U_V0_P0CADDR           V0_P0CADDR           ;

    U_V0_NADDR             V0_NADDR             ; 
    U_V0_NCADDR            V0_NCADDR            ; 

    U_G0_ADDR              G0_ADDR              ; 
    U_G0_NADDR             G0_NADDR             ; 

    U_WBC_DHD0_YADDR       WBC_DHD0_YADDR       ;
    U_WBC_DHD0_CADDR       WBC_DHD0_CADDR       ;

    U_WBC_GP0_YADDR        WBC_GP0_YADDR        ;

    U_V0_SMMU_BYPASS0      V0_SMMU_BYPASS0      ; 
    U_G0_SMMU_BYPASS0      G0_SMMU_BYPASS0      ; 
    U_WBC_DHD0_SMMU_BYPASS WBC_DHD0_SMMU_BYPASS ; 
    U_WBC_GP0_SMMU_BYPASS  WBC_GP0_SMMU_BYPASS  ; 
    U_WBC_ME_SMMU_BYPASS   WBC_ME_SMMU_BYPASS   ; 
    U_WBC_FI_SMMU_BYPASS   WBC_FI_SMMU_BYPASS   ; 

    U_DHD0_CTRL            DHD0_CTRL            ; 

    HI_U32 vid_en = 0;
    HI_U32 gfx_en = 0;
    HI_U32 wbc_en = 0;

    SMMU_SCR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->SMMU_SCR.u32)));
    HI_U32 smmu_glb_bypass = SMMU_SCR.bits.glb_bypass;

#if MMU_ADDR_CHECK_DBG
    cout << dec << "smmu_glb_bypass = " << smmu_glb_bypass << endl;
#endif
    //just dhd0 has 3d mode
    DHD0_CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->DHD0_CTRL.u32) + 1*CHN_OFFSET));

    HI_U32 ii;

    //vido layer config check
    for(ii=0; ii<4; ii++)
    {
        V0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_CTRL.u32)  + ii*VID_OFFSET));

        vid_en = V0_CTRL.bits.surface_en;

        if(vid_en == 1)
        {
            cout << "V" << ii << " Mmu Addr Config Check Begin:"  << endl;

            V0_MULTI_MODE.u32   = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_MULTI_MODE.u32) + ii * VID_OFFSET));

            V0_P0LADDR.u32      = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + ii * VID_OFFSET));
            V0_P0CADDR.u32      = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + ii * VID_OFFSET));

            V0_NADDR.u32        = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_NADDR.u32)  + ii * VID_OFFSET));
            V0_NCADDR.u32       = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_NCADDR.u32) + ii * VID_OFFSET));

            V0_SMMU_BYPASS0.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_SMMU_BYPASS0.u32) + ii*VID_OFFSET));
#if MMU_ADDR_CHECK_DBG
            cout << dec << "v" << ii << "_mmu_l_bypass = " <<  V0_SMMU_BYPASS0.bits.r0_l_bypass << endl;
            cout << dec << "v" << ii << "_mmu_c_bypass = " <<  V0_SMMU_BYPASS0.bits.r0_c_bypass << endl;
#endif

            HI_U32 region_l_bypass[16] = {0}; 
            HI_U32 region_c_bypass[16] = {0}; 

            region_l_bypass[0]  = V0_SMMU_BYPASS0.bits.r0_l_bypass;
            region_l_bypass[1]  = V0_SMMU_BYPASS0.bits.r1_l_bypass;
            region_l_bypass[2]  = V0_SMMU_BYPASS0.bits.r2_l_bypass;
            region_l_bypass[3]  = V0_SMMU_BYPASS0.bits.r3_l_bypass;
            region_l_bypass[4]  = V0_SMMU_BYPASS0.bits.r4_l_bypass;
            region_l_bypass[5]  = V0_SMMU_BYPASS0.bits.r5_l_bypass;
            region_l_bypass[6]  = V0_SMMU_BYPASS0.bits.r6_l_bypass;
            region_l_bypass[7]  = V0_SMMU_BYPASS0.bits.r7_l_bypass;
            region_l_bypass[8]  = V0_SMMU_BYPASS0.bits.r8_l_bypass;
            region_l_bypass[9]  = V0_SMMU_BYPASS0.bits.r9_l_bypass;
            region_l_bypass[10] = V0_SMMU_BYPASS0.bits.r10_l_bypass;
            region_l_bypass[11] = V0_SMMU_BYPASS0.bits.r11_l_bypass;
            region_l_bypass[12] = V0_SMMU_BYPASS0.bits.r12_l_bypass;
            region_l_bypass[13] = V0_SMMU_BYPASS0.bits.r13_l_bypass;
            region_l_bypass[14] = V0_SMMU_BYPASS0.bits.r14_l_bypass;
            region_l_bypass[15] = V0_SMMU_BYPASS0.bits.r15_l_bypass;

            region_c_bypass[0]  = V0_SMMU_BYPASS0.bits.r0_c_bypass;
            region_c_bypass[1]  = V0_SMMU_BYPASS0.bits.r1_c_bypass;
            region_c_bypass[2]  = V0_SMMU_BYPASS0.bits.r2_c_bypass;
            region_c_bypass[3]  = V0_SMMU_BYPASS0.bits.r3_c_bypass;
            region_c_bypass[4]  = V0_SMMU_BYPASS0.bits.r4_c_bypass;
            region_c_bypass[5]  = V0_SMMU_BYPASS0.bits.r5_c_bypass;
            region_c_bypass[6]  = V0_SMMU_BYPASS0.bits.r6_c_bypass;
            region_c_bypass[7]  = V0_SMMU_BYPASS0.bits.r7_c_bypass;
            region_c_bypass[8]  = V0_SMMU_BYPASS0.bits.r8_c_bypass;
            region_c_bypass[9]  = V0_SMMU_BYPASS0.bits.r9_c_bypass;
            region_c_bypass[10] = V0_SMMU_BYPASS0.bits.r10_c_bypass;
            region_c_bypass[11] = V0_SMMU_BYPASS0.bits.r11_c_bypass;
            region_c_bypass[12] = V0_SMMU_BYPASS0.bits.r12_c_bypass;
            region_c_bypass[13] = V0_SMMU_BYPASS0.bits.r13_c_bypass;
            region_c_bypass[14] = V0_SMMU_BYPASS0.bits.r14_c_bypass;
            region_c_bypass[15] = V0_SMMU_BYPASS0.bits.r15_c_bypass;

#if MMU_ADDR_CHECK_DBG
            cout << dec << "region_l_bypass[0]  = " << region_l_bypass[0]  << endl; 
            cout << dec << "region_l_bypass[1]  = " << region_l_bypass[1]  << endl; 
            cout << dec << "region_l_bypass[2]  = " << region_l_bypass[2]  << endl; 
            cout << dec << "region_l_bypass[3]  = " << region_l_bypass[3]  << endl; 
            cout << dec << "region_l_bypass[4]  = " << region_l_bypass[4]  << endl; 
            cout << dec << "region_l_bypass[5]  = " << region_l_bypass[5]  << endl; 
            cout << dec << "region_l_bypass[6]  = " << region_l_bypass[6]  << endl; 
            cout << dec << "region_l_bypass[7]  = " << region_l_bypass[7]  << endl; 
            cout << dec << "region_l_bypass[8]  = " << region_l_bypass[8]  << endl; 
            cout << dec << "region_l_bypass[9]  = " << region_l_bypass[9]  << endl; 
            cout << dec << "region_l_bypass[10] = " << region_l_bypass[10] << endl; 
            cout << dec << "region_l_bypass[11] = " << region_l_bypass[11] << endl; 
            cout << dec << "region_l_bypass[12] = " << region_l_bypass[12] << endl; 
            cout << dec << "region_l_bypass[13] = " << region_l_bypass[13] << endl; 
            cout << dec << "region_l_bypass[14] = " << region_l_bypass[14] << endl; 
            cout << dec << "region_l_bypass[15] = " << region_l_bypass[15] << endl; 
                                                                          
            cout << dec << "region_c_bypass[0]  = " << region_c_bypass[0]  << endl; 
            cout << dec << "region_c_bypass[1]  = " << region_c_bypass[1]  << endl; 
            cout << dec << "region_c_bypass[2]  = " << region_c_bypass[2]  << endl; 
            cout << dec << "region_c_bypass[3]  = " << region_c_bypass[3]  << endl; 
            cout << dec << "region_c_bypass[4]  = " << region_c_bypass[4]  << endl; 
            cout << dec << "region_c_bypass[5]  = " << region_c_bypass[5]  << endl; 
            cout << dec << "region_c_bypass[6]  = " << region_c_bypass[6]  << endl; 
            cout << dec << "region_c_bypass[7]  = " << region_c_bypass[7]  << endl; 
            cout << dec << "region_c_bypass[8]  = " << region_c_bypass[8]  << endl; 
            cout << dec << "region_c_bypass[9]  = " << region_c_bypass[9]  << endl; 
            cout << dec << "region_c_bypass[10] = " << region_c_bypass[10] << endl; 
            cout << dec << "region_c_bypass[11] = " << region_c_bypass[11] << endl; 
            cout << dec << "region_c_bypass[12] = " << region_c_bypass[12] << endl; 
            cout << dec << "region_c_bypass[13] = " << region_c_bypass[13] << endl; 
            cout << dec << "region_c_bypass[14] = " << region_c_bypass[14] << endl; 
            cout << dec << "region_c_bypass[15] = " << region_c_bypass[15] << endl; 
#endif

            if(smmu_glb_bypass == 0)
            {
                if(region_l_bypass[0] == 0)
                {
                    VDP_ASSERT(V0_P0LADDR.bits.surface_addr < 0x80000000 && V0_P0LADDR.bits.surface_addr > 0x100000);
                }
                else
                {
                    VDP_ASSERT(V0_P0LADDR.bits.surface_addr >= 0x80000000);
                }

                if(region_c_bypass[0] == 0)
                {
                    VDP_ASSERT(V0_P0CADDR.bits.surface_addr < 0x80000000 && V0_P0CADDR.bits.surface_addr > 0x100000);
                }
                else
                {
                    VDP_ASSERT(V0_P0CADDR.bits.surface_addr >= 0x80000000);
                }
            }


            HI_U32 disp_mode = (ii < 2) ? DHD0_CTRL.bits.disp_mode : 0;
#if MMU_ADDR_CHECK_DBG
            cout << dec << "disp_mode = " << disp_mode << endl;
#endif
            if(disp_mode != 0 || V0_CTRL.bits.ifmt == 0x7)//3D or tile64
            {
                if(smmu_glb_bypass == 0)
                {
					cout << hex << "[assert]V0_NADDR.bits.surface_naddr   = " << V0_NADDR.bits.surface_naddr   << endl;
					cout << hex << "[assert]V0_NCADDR.bits.surface_ncaddr = " << V0_NCADDR.bits.surface_ncaddr << endl;
                    if(region_l_bypass[1] == 0)
                    {
                        VDP_ASSERT(V0_NADDR.bits.surface_naddr < 0x80000000 && V0_NADDR.bits.surface_naddr > 0x100000);
                    }
                    else 
                    {
                        VDP_ASSERT(V0_NADDR.bits.surface_naddr >= 0x80000000);
                    }

                    if(region_c_bypass[1] == 0)
                    {
                        VDP_ASSERT(V0_NCADDR.bits.surface_ncaddr < 0x80000000 && V0_NCADDR.bits.surface_ncaddr > 0x100000);
                    }
                    else
                    {
                        VDP_ASSERT(V0_NCADDR.bits.surface_ncaddr >= 0x80000000);
                    }
                }

            }

            HI_U32 multi_en = (ii==1) ? V0_MULTI_MODE.bits.mrg_mode :0;
            HI_U32 region_en = 0;
#if MMU_ADDR_CHECK_DBG
            cout << dec << "multi_en = " << multi_en << endl;
#endif
            if(multi_en == 1)//multi region
            {
                if(smmu_glb_bypass == 0)
                {
                    for(HI_U32 jj=0; jj<16; jj++)
                    {
                        V0_P0LADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0LADDR.u32) + ii * VID_OFFSET + jj * REGION_OFFSET));
                        V0_P0CADDR.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CADDR.u32) + ii * VID_OFFSET + jj * REGION_OFFSET));

                        if(jj == 0)
                        {
                            V0_P0CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P0CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P0CTRL.bits.region_en;
                        }
                        else if(jj == 1)
                        {
                            V0_P1CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P1CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P1CTRL.bits.region_en;
                        }
                        else if(jj == 2)
                        {
                            V0_P2CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P2CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P2CTRL.bits.region_en;
                        }
                        else if(jj == 3)
                        {
                            V0_P3CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P3CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P3CTRL.bits.region_en;
                        }
                        else if(jj == 4)
                        {
                            V0_P4CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P4CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P4CTRL.bits.region_en;
                        }
                        else if(jj == 5)
                        {
                            V0_P5CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P5CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P5CTRL.bits.region_en;
                        }
                        else if(jj == 6)
                        {
                            V0_P6CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P6CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P6CTRL.bits.region_en;
                        }
                        else if(jj == 7)
                        {
                            V0_P7CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P7CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P7CTRL.bits.region_en;
                        }
                        else if(jj == 8)
                        {
                            V0_P8CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P8CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P8CTRL.bits.region_en;
                        }
                        else if(jj == 9)
                        {
                            V0_P9CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P9CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P9CTRL.bits.region_en;
                        }
                        else if(jj == 10)
                        {
                            V0_P10CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P10CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P10CTRL.bits.region_en;
                        }
                        else if(jj == 11)
                        {
                            V0_P11CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P11CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P11CTRL.bits.region_en;
                        }
                        else if(jj == 12)
                        {
                            V0_P12CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P12CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P12CTRL.bits.region_en;
                        }
                        else if(jj == 13)
                        {
                            V0_P13CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P13CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P13CTRL.bits.region_en;
                        }
                        else if(jj == 14)
                        {
                            V0_P14CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P14CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P14CTRL.bits.region_en;
                        }
                        else if(jj == 15)
                        {
                            V0_P15CTRL.u32 = VDP_RegRead(((HI_ULONG)&(pVdpReg->V0_P15CTRL.u32) + ii * VID_OFFSET));
                            region_en = V0_P15CTRL.bits.region_en;
                        }

                        if(region_en == 1)
                        {
                            if(region_l_bypass[jj] == 0)
                            {
                                VDP_ASSERT(V0_P0LADDR.bits.surface_addr < 0x80000000 && V0_P0LADDR.bits.surface_addr > 0x100000);
                            }
                            else
                            {
                                VDP_ASSERT(V0_P0LADDR.bits.surface_addr >= 0x80000000);
                            }

                            if(region_c_bypass[jj] == 0)
                            {
                                VDP_ASSERT(V0_P0CADDR.bits.surface_addr < 0x80000000 && V0_P0CADDR.bits.surface_addr > 0x100000);
                            }
                            else 
                            {
                                VDP_ASSERT(V0_P0CADDR.bits.surface_addr >= 0x80000000);
                            }
                        }
                        
                    }

                }
            }
        }

        cout << "V" << ii << " Mmu Addr Config Check End"  << endl;
    }

    //gfx layer config check
    for(ii=0; ii<5; ii++)
    {
        G0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_CTRL.u32)  + ii*GFX_OFFSET));

        gfx_en = G0_CTRL.bits.surface_en;

        if(gfx_en == 1)
        {

            cout << "GFX" << ii << " Mmu Addr Config Check Begin:"  << endl;

            G0_ADDR.u32      = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_ADDR.u32)  + ii * GFX_OFFSET));
            G0_NADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_NADDR.u32) + ii * GFX_OFFSET));

            G0_SMMU_BYPASS0.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->G0_SMMU_BYPASS0.u32) + ii * GFX_OFFSET));

            HI_U32 disp_mode = (ii < 4) ? DHD0_CTRL.bits.disp_mode : 0;
            if(smmu_glb_bypass == 0)
            {
                if(disp_mode != 0)
                {
                    if(G0_SMMU_BYPASS0.bits.bypass_2d == 0)
                    {
                        VDP_ASSERT(G0_ADDR.bits.surface_addr < 0x80000000 && G0_ADDR.bits.surface_addr > 0x100000);
                    }
                    else
                    {
                        VDP_ASSERT(G0_ADDR.bits.surface_addr >= 0x80000000);
                    }

                    if(G0_SMMU_BYPASS0.bits.r_bypass_3d == 0)
                    {
                        VDP_ASSERT(G0_NADDR.bits.surface_addr < 0x80000000 && G0_NADDR.bits.surface_addr > 0x100000);
                    }
                    else
                    {
                        VDP_ASSERT(G0_NADDR.bits.surface_addr >= 0x80000000);
                    }
                }
                else
                {
                    if(G0_SMMU_BYPASS0.bits.bypass_2d == 0)
                    {
                        VDP_ASSERT(G0_ADDR.bits.surface_addr < 0x80000000 && G0_ADDR.bits.surface_addr > 0x100000);
                    }
                    else
                    {
                        VDP_ASSERT(G0_ADDR.bits.surface_addr >= 0x80000000);
                    }
                }
            }

            cout << "GFX" << ii << " Mmu Addr Config Check End:"  << endl;
        }
    }

    //wbc dhd config check
    WBC_DHD0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32)));
    wbc_en = WBC_DHD0_CTRL.bits.wbc_en;

    if(wbc_en == 1)
    {
        WBC_DHD0_YADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_YADDR.u32)));
        WBC_DHD0_CADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CADDR.u32)));

        WBC_DHD0_SMMU_BYPASS.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_SMMU_BYPASS.u32)));

        if(smmu_glb_bypass == 0)
        {
            if(WBC_DHD0_SMMU_BYPASS.bits.l_bypass == 0)
            {
                VDP_ASSERT(WBC_DHD0_YADDR.bits.wbcaddr < 0x80000000 && WBC_DHD0_YADDR.bits.wbcaddr > 0x100000);
            }
            else
            {
                VDP_ASSERT(WBC_DHD0_YADDR.bits.wbcaddr >= 0x80000000);
            }

            if(WBC_DHD0_SMMU_BYPASS.bits.c_bypass == 0)
            {
                VDP_ASSERT(WBC_DHD0_CADDR.bits.wbccaddr < 0x80000000 && WBC_DHD0_CADDR.bits.wbccaddr > 0x100000);
            }
            else
            {
                VDP_ASSERT(WBC_DHD0_CADDR.bits.wbccaddr >= 0x80000000);
            }
        }
    }

    //wbc vp0 config check
    WBC_DHD0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CTRL.u32) + 3 * WBC_OFFSET));
    wbc_en = WBC_DHD0_CTRL.bits.wbc_en;

    if(wbc_en == 1)
    {
        cout << "WBC_VP0 Mmu Addr Config Check Begin:"  << endl;

        WBC_DHD0_YADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_YADDR.u32) + 3 * WBC_OFFSET));
        WBC_DHD0_CADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_CADDR.u32) + 3 * WBC_OFFSET));

        WBC_DHD0_SMMU_BYPASS.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_DHD0_SMMU_BYPASS.u32) + 3 * WBC_OFFSET));

        if(smmu_glb_bypass == 0)
        {
            if(WBC_DHD0_SMMU_BYPASS.bits.l_bypass == 0)
            {
                VDP_ASSERT(WBC_DHD0_YADDR.bits.wbcaddr < 0x80000000 && WBC_DHD0_YADDR.bits.wbcaddr > 0x100000);
            }
            else
            {
                VDP_ASSERT(WBC_DHD0_YADDR.bits.wbcaddr >= 0x80000000);
            }

            if(WBC_DHD0_SMMU_BYPASS.bits.c_bypass == 0)
            {
                VDP_ASSERT(WBC_DHD0_CADDR.bits.wbccaddr < 0x80000000 && WBC_DHD0_CADDR.bits.wbccaddr > 0x100000);
            }
            else
            {
                VDP_ASSERT(WBC_DHD0_CADDR.bits.wbccaddr >= 0x80000000);
            }
        }

        cout << "WBC_VP0 Mmu Addr Config Check End"  << endl;
    }

    //wbc gp0 config check
    WBC_GP0_CTRL.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_GP0_CTRL.u32)));
    wbc_en = WBC_GP0_CTRL.bits.wbc_en;

    if(wbc_en == 1)
    {
        cout << "WBC_GP0 Mmu Addr Config Check Begin"  << endl;
        WBC_GP0_YADDR.u32     = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_GP0_YADDR.u32)));

        WBC_GP0_SMMU_BYPASS.u32  = VDP_RegRead(((HI_ULONG)&(pVdpReg->WBC_GP0_SMMU_BYPASS.u32)));

        if(smmu_glb_bypass == 0)
        {
            if(WBC_GP0_SMMU_BYPASS.bits.bypass == 0)
            {
                VDP_ASSERT(WBC_GP0_YADDR.bits.wbcaddr < 0x80000000 && WBC_GP0_YADDR.bits.wbcaddr > 0x100000);
            }
            else
            {
                VDP_ASSERT(WBC_GP0_YADDR.bits.wbcaddr >= 0x80000000);
            }
        }
        cout << "WBC_GP0 Mmu Addr Config Check End"  << endl;
    }

#endif

    return HI_FAILURE;
}
