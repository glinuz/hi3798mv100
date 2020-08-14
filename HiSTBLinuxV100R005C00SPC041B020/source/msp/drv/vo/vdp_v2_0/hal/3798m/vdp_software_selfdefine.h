#ifndef __VDP_SOFTWARE_SELFDEFINE_H_
#define __VDP_SOFTWARE_SELFDEFINE_H_


#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
HI_VOID  VDP_VID_SetSrOutReso(HI_U32 u32Data, HI_RECT_S stRect);
HI_VOID VDP_VID_SetSrEnable(HI_U32 u32Data, HI_BOOL bEnable);
HI_VOID VDP_VID_SetSrZmeMemMode(HI_U32 u32Data, HI_BOOL bMemSelUHD);
#endif

#endif


