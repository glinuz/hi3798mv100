#ifndef __HAL_IP_NXG_H__
#define __HAL_IP_NXG_H__

#include "vdp_define.h"

typedef struct tagVDP_NXG_CFG_S
{
    HI_U32 dwm_en;
    HI_U32 debug_en;
    HI_U32 frame_rate;
    HI_U32 keyin;
    HI_U32 payload_0;
    HI_U32 payload_1;
    HI_U32 setting[6];
    HI_U32 video_yfpos;
    HI_U32 video_xfpos;
    HI_U32 video_ylpos;
    HI_U32 video_xlpos;
    HI_U32 wm3d_en;
    HI_U32 wm3d_righteye;
}VDP_NXG_CFG_S;

typedef enum tagVDP_NXG_NXG_MODE_E
{
    VDP_NXG_NXG_TYP = 0 ,
    VDP_NXG_NXG_NORM = 1 ,

    VDP_NXG_NXG_BUTT
}VDP_NXG_NXG_MODE_E;

HI_S32 VDP_FUNC_GetNxgCfg(VDP_NXG_NXG_MODE_E NxgMode, HI_U32 iw, HI_U32 ih, VDP_NXG_CFG_S *stCfg);

HI_S32 VDP_DRV_SetNxgMode(HI_U32 enLayer, VDP_NXG_CFG_S *stCfg);

HI_VOID VDP_NXG_SetNxgDwmEn      ( HI_U32 u32Data, HI_U32 dwm_en);
HI_VOID VDP_NXG_SetNxgDebugEn    ( HI_U32 u32Data, HI_U32 debug_en);
HI_VOID VDP_NXG_SetNxgFrameRate  ( HI_U32 u32Data, HI_U32 frame_rate);
HI_VOID VDP_NXG_SetNxgWm3dEn     ( HI_U32 u32Data, HI_U32 wm_3d_en) ;
HI_VOID VDP_NXG_SetNxgWm3dEyeEn  ( HI_U32 u32Data, HI_U32 wm_3d_eye) ;
HI_VOID VDP_NXG_SetNxgKeyin      ( HI_U32 u32Data, HI_U32 keyin);
HI_VOID VDP_NXG_SetNxgPayload    ( HI_U32 u32Data, HI_U32 payload_0,HI_U32 payload_1);
HI_VOID VDP_NXG_SetNxgSetting0   ( HI_U32 u32Data, HI_U32 setting_0);
HI_VOID VDP_NXG_SetNxgSetting1   ( HI_U32 u32Data, HI_U32 setting_1);
HI_VOID VDP_NXG_SetNxgSetting2   ( HI_U32 u32Data, HI_U32 setting_2);
HI_VOID VDP_NXG_SetNxgSetting3   ( HI_U32 u32Data, HI_U32 setting_3);
HI_VOID VDP_NXG_SetNxgSetting4   ( HI_U32 u32Data, HI_U32 setting_4);
HI_VOID VDP_NXG_SetNxgSetting5   ( HI_U32 u32Data, HI_U32 setting_5);

HI_VOID VDP_NXG_SetNxgVideoYfpos ( HI_U32 u32Data, HI_U32 video_yfpos);
HI_VOID VDP_NXG_SetNxgVideoXfpos ( HI_U32 u32Data, HI_U32 video_xfpos);
HI_VOID VDP_NXG_SetNxgVideoYlpos ( HI_U32 u32Data, HI_U32 video_ylpos);
HI_VOID VDP_NXG_SetNxgVideoXlpos ( HI_U32 u32Data, HI_U32 video_xlpos);

#endif//__DRV_IP_NXG_H__
