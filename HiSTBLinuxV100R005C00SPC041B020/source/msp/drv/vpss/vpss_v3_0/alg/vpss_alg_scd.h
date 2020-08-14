#ifndef __VPSS_ALG_SCD_H__
#define __VPSS_ALG_SCD_H__
#include "hi_type.h"
#include "hi_math.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


typedef struct hiSCDRls
{
    HI_S32 SCW_P1;
    HI_S32 s32SCHist_P1T[32];
    HI_S32 s32SCHist_P1B[32];
	HI_S32 s32SCHist_CF[32];	
} SCDRls;

typedef struct hiSCDParameter
{
    int SCDK_pixy;			// [0,16]
    int SCDK1_histdiff;		// [0,1023]	/////////////////////////////////////////////////Main ADJ
    int SCDK2_histdiff;		// [0,1023]	/////////////////////////////////////////////////Main ADJ
    int SCDCorebin_histdiff;	// [0,7]
    int SCDKbin_histdiff;	// [0,255]

    int SCDCore_histdiff;		// [0,255]
    int SCDCore_sumdiff;		// [0,255]

    int SCDK1_sumdiff;		// [0,255]		/////////////////////////////////////////////////Main ADJ
    int SCDX1_sumdiff;		// [0,1023]		/////////////////////////////////////////////////Main ADJ
    int SCDG1_sumdiff;		// [0,1023]		/////////////////////////////////////////////////Main ADJ
    int SCDK2_sumdiff;		// [0,511]		/////////////////////////////////////////////////Main ADJ

    int SCDGainH_TF;			//224; // [0,255]
    int SCDGainL_TF;			//32;  // [0,255]
    int SCDCore_scwdiff;		// [0,255]
    int SCDK_scwdiff_TF;		// [0,255]

    int SCDDivWidth;				// (1<<20)/width  -->[0,?]
    int SCDDivHeight;				// (1<<20)/height -->[0,?]
} SCDParameter;

typedef struct hiSCDInput
{
    int  Width;
    int  Height;
    int  Field; //0:HI_DRV_FIELD_TOP 1:HI_DRV_FIELD_BOTTOM 2:HI_DRV_FIELD_ALL
    SCDParameter scdPara;
} SCDInput;



/*--------------------------------------------------------------------------------*
Function        :SCDDetection(SCDInput* pSCDInput,SCDRls *pSCDRls,HI_S32 *pSCDReg)
Description    : SCD场景切换参数计算，返回0-255间的调整参数值
Input            :pSCDInput, 码流信息，宽，高，逐隔行信息
		      :pSCDRls,上次计算结果，以及统计信息,函数使用后，更
		      
Output          :NA
Return          :SCD调整参数
Others          :NA
*--------------------------------------------------------------------------------*/

HI_S32	SCDDetection(SCDInput* pSCDInput, SCDRls* pSCDRls);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif //end of __VPSS_ALG_SCD_H__

