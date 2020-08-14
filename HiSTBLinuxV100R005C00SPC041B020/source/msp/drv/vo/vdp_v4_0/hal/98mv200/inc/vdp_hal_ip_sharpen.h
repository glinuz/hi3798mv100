#ifndef __VDP_HAL_IP_SHARPEN_H__
#define __VDP_HAL_IP_SHARPEN_H__

//#include "c_union_define.h"
#include "hi_reg_common.h"
#include "vdp_define.h"

HI_VOID VDP_VID_SetSharpDbgEn         (HI_U32 u32LayerId, HI_U32 bEnable)                                   ;
HI_VOID VDP_VID_SetSharpDemoMode      (HI_U32 u32LayerId, HI_U32 DemoMode)                                  ;
HI_VOID VDP_VID_SetSharpEnable        (HI_U32 u32LayerId, HI_U32 bEnable)                                   ;
HI_VOID VDP_VID_SetSharpDemoPos       (HI_U32 u32LayerId, HI_U32 u32DemoPos)                                ;
HI_VOID VDP_VID_SetSharpDetecEnable   (HI_U32 u32LayerId, HI_U32 bEnable)                                   ;
HI_VOID VDP_VID_SetSharpPeakEnable    (HI_U32 u32LayerId, HI_U32 bEnable)                                   ;
HI_VOID VDP_VID_SetSharpGain          (HI_U32 u32LayerId, HI_U32 u32Gain1,HI_U32 u32Gain2,HI_U32 u32Gain3)  ;
HI_VOID VDP_VID_SetSharpPeakCoring    (HI_U32 u32LayerId, HI_U32 u32Data)                                   ;
HI_VOID VDP_VID_SetSharpPeakShootCtrl (HI_U32 u32LayerId, HI_U32 u32Over, HI_U32 u32Under,HI_U32 u32Mix)    ;
HI_VOID VDP_VID_SetSharpEdgeShootCtrl (HI_U32 u32LayerId, HI_U32 u32Over, HI_U32 u32Under,HI_U32 u32Mix)    ;
HI_VOID VDP_VID_SetSharpTmpH1         (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpTmpH2         (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpTmpV1         (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpTmpV2         (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpEdgeTmpH      (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpEdgeTmpV      (HI_U32 u32LayerId, HI_U32 u32Tmp0, HI_U32 u32Tmp1,HI_U32 u32Tmp2)    ;
HI_VOID VDP_VID_SetSharpEdgeEi        (HI_U32 u32LayerId, HI_U32 u32EiRatio, HI_U32 u32Ei1,HI_U32 u32Ei0)   ;
HI_VOID VDP_VID_SetSharpHighFreq      (HI_U32 u32LayerId, VDP_SHARP_DIR_E enDir, VDP_SHARP_GROUP_S * stCfg) ;

HI_VOID VDP_VID_SetSharpGradMin       (HI_U32 u32LayerId, HI_U32 u32GradMinSlop, HI_U32 u32GradMinThrh,HI_U32 u32GradMinThrl);
HI_VOID VDP_VID_SetSharpGradCtrl      (HI_U32 u32LayerId, HI_U32 u32ChessBoardThr, HI_U32 u32GradMinThr);
HI_VOID VDP_VID_SetSharpShootRatio    (HI_U32 u32LayerId, HI_U32 u32OshootRatiot, HI_U32 u32OshootRatioe,HI_U32 u32UshootRatiot,HI_U32 u32UshootRatioe);
HI_VOID VDP_VID_SetSharpGradContrast  (HI_U32 u32LayerId, HI_U32 u32GradContrastThr, HI_U32 u32GradContrastSlop);
HI_VOID VDP_VID_SetSharpEnsctrl       (HI_U32 u32LayerId, HI_U32 u32Ensctrl);

#endif


