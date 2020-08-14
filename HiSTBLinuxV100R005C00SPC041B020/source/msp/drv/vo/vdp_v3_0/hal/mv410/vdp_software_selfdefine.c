#ifdef HI_DISP_BUILD_FULL
#include <linux/kernel.h>
#endif

#include "hi_reg_common.h"
#include "vdp_driver.h"
#include "vdp_define.h"
//#include "hi_reg_vdp.h"
#include "drv_disp_com.h"

extern  volatile S_VDP_REGS_TYPE *pVdpReg;

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
HI_VOID  VDP_VID_SetSrOutReso(HI_U32 u32Data, HI_RECT_S stRect)
{
    volatile U_V0_SR_ORESO V0_SR_ORESO;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
       return ;
   }

   V0_SR_ORESO.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->V0_SR_ORESO.u32) + u32Data * VID_OFFSET));
   V0_SR_ORESO.bits.ow = stRect.s32Width - 1;
   V0_SR_ORESO.bits.oh = stRect.s32Height - 1;
   VDP_RegWrite((HI_U32)(&(pVdpReg->V0_SR_ORESO.u32) + u32Data * VID_OFFSET), V0_SR_ORESO.u32);

   return ;
}

HI_VOID VDP_VID_SetSrEnable(HI_U32 u32Data, HI_BOOL bEnable)
{
    volatile U_SR_CTRL SR_CTRL;

   if(u32Data >= VID_MAX)
   {
       HI_PRINT("Error,VDP_VID_SetOutReso() Select Wrong Video Layer ID\n");
       return ;
   }

   SR_CTRL.u32 = VDP_RegRead((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * VID_OFFSET));
   SR_CTRL.bits.sr_en  = bEnable;
   VDP_RegWrite((HI_U32)(&(pVdpReg->SR_CTRL.u32) + u32Data * VID_OFFSET), SR_CTRL.u32);

}

#endif
