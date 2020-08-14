#ifndef __VPSS_CBB_FRAME_H__
#define __VPSS_CBB_FRAME_H__

#include "vpss_cbb_common.h"


typedef struct
{
	HI_U32				u32Width;
	HI_U32				u32Height;
	HI_RECT_S			stOffset;
	CBB_FRAME_ADDR_S	stAddr[CBB_FREAM_BUF_CNT];// 定义多组地址使用bit宽度区分
	CBB_BITWIDTH_E		enBitWidth;
	HI_DRV_PIX_FORMAT_E	enPixelFmt;
	HI_BOOL				bSmmu;
	HI_BOOL 			bSecure;
	HI_BOOL 			bCompress;
	HI_BOOL 			bProgressive;
	HI_BOOL				bTopFirst;
	HI_BOOL				bTop;
	
	//VC_1    stVcInfo;
}CBB_FRAME_S;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern HI_S32 CBB_CfgFrm_ReadCur(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_ReadNx1(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_ReadNx2(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_ReadRee(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_ReadRef(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_WriteCue(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_S32 CBB_CfgFrm_WriteRfr(CBB_FRAME_S *pstFrmCfg, CBB_REG_ADDR_S stRegAddr, HI_BOOL bEnable);
extern HI_S32 CBB_CfgFrm_WriteVhd0(CBB_FRAME_S *pstFrmCfg, HI_VOID* pArgs, CBB_REG_ADDR_S stRegAddr);
extern HI_U32 CBB_FrameFmtTrans( HI_DRV_PIX_FORMAT_E	enPixelFmt );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

