#include "hi_type.h"
#include "vdp_hal_vid.h"
#include "vdp_func_pq_csc.h"



HI_S32 VDP_FUNC_GetCSCCoef(HI_PQ_CSC_CRTL_S* pstCscCtrl, VDP_CSC_MODE_E enCscMode, HI_PQ_CSC_COEF_S* pstCscCoef, HI_PQ_CSC_DCCOEF_S* pstCscDcCoef)
{
	HI_S32 s32Pre	= 1 << 10;
	//HI_U32 u32OutDcPre = 64 * u32Pre;
	HI_S32 s32OutDcPre = 4;
	HI_S32 s32InDcPre = 4;
    if(enCscMode == VDP_CSC_RGB2YUV_601)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(0.299  * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(0.587  * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)(0.114  * s32Pre);

            pstCscCoef->csc_coef10     = (HI_S32)(-0.172 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)(-0.339 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)(0.511  * s32Pre);

            pstCscCoef->csc_coef20     = (HI_S32)(0.511  * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)(-0.428 * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)(-0.083 * s32Pre);

            pstCscDcCoef->csc_in_dc2  = 0 * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = 0 * s32InDcPre;
            pstCscDcCoef->csc_in_dc0  = 0 * s32InDcPre;

            pstCscDcCoef->csc_out_dc2 =  16 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 = 128 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc0 = 128 * s32OutDcPre;
        }
        else if(enCscMode == VDP_CSC_YUV2RGB_601)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(    1  * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(    0  * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)(1.371  * s32Pre);

            pstCscCoef->csc_coef10     = (HI_S32)(     1 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)(-0.698 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)(-0.336 * s32Pre);

            pstCscCoef->csc_coef20     = (HI_S32)(    1  * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)(1.732  * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)(    0  * s32Pre);

            pstCscDcCoef->csc_in_dc2  = -16  * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = -128 * s32InDcPre;
            pstCscDcCoef->csc_in_dc0  = -128 * s32InDcPre;

            pstCscDcCoef->csc_out_dc2 =  0 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 =  0 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc0 =  0 * s32OutDcPre;
        }
        else if(enCscMode == VDP_CSC_RGB2YUV_709)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(0.213  * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(0.715  * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)(0.072  * s32Pre);
            pstCscCoef->csc_coef10     = (HI_S32)(-0.117 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)(-0.394 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)( 0.511 * s32Pre);
            pstCscCoef->csc_coef20     = (HI_S32)( 0.511 * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)(-0.464 * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)(-0.047 * s32Pre);

            pstCscDcCoef->csc_in_dc2  = 0 * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = 0 * s32InDcPre;
            pstCscDcCoef->csc_in_dc0  = 0 * s32InDcPre;

            pstCscDcCoef->csc_out_dc2 = 16  * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 = 128 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc0 = 128 * s32OutDcPre;
        }
        else if(enCscMode == VDP_CSC_YUV2RGB_709)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(    1  * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(    0  * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)(1.540  * s32Pre);

            pstCscCoef->csc_coef10     = (HI_S32)(     1 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)(-0.183 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)(-0.459 * s32Pre);

            pstCscCoef->csc_coef20     = (HI_S32)(    1  * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)(1.816  * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)(    0  * s32Pre);

            pstCscDcCoef->csc_in_dc0  = -16  * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = -128 * s32InDcPre;
            pstCscDcCoef->csc_in_dc2  = -128 * s32InDcPre;

            pstCscDcCoef->csc_out_dc0 = 0 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 = 0 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc2 = 0 * s32OutDcPre;
        }
        else if(enCscMode == VDP_CSC_YUV2YUV_709_601)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(     1 * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(-0.116 * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)( 0.208 * s32Pre);
            pstCscCoef->csc_coef10     = (HI_S32)(     0 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)( 1.017 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)( 0.114 * s32Pre);
            pstCscCoef->csc_coef20     = (HI_S32)(     0 * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)( 0.075 * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)( 1.025 * s32Pre);

            pstCscDcCoef->csc_in_dc0  = -16  * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = -128 * s32InDcPre;
            pstCscDcCoef->csc_in_dc2  = -128 * s32InDcPre;

            pstCscDcCoef->csc_out_dc0 =   16 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 =  128 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc2 =  128 * s32OutDcPre;
        }
        else if(enCscMode == VDP_CSC_YUV2YUV_601_709)
        {
            pstCscCoef->csc_coef00     = (HI_S32)(     1 * s32Pre);
            pstCscCoef->csc_coef01     = (HI_S32)(-0.116 * s32Pre);
            pstCscCoef->csc_coef02     = (HI_S32)( 0.208 * s32Pre);

            pstCscCoef->csc_coef10     = (HI_S32)(     0 * s32Pre);
            pstCscCoef->csc_coef11     = (HI_S32)( 1.017 * s32Pre);
            pstCscCoef->csc_coef12     = (HI_S32)( 0.114 * s32Pre);

            pstCscCoef->csc_coef20     = (HI_S32)(     0 * s32Pre);
            pstCscCoef->csc_coef21     = (HI_S32)( 0.075 * s32Pre);
            pstCscCoef->csc_coef22     = (HI_S32)( 1.025 * s32Pre);

            pstCscDcCoef->csc_in_dc2  = -16  * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = -128 * s32InDcPre;
            pstCscDcCoef->csc_in_dc0  = -128 * s32InDcPre;

            pstCscDcCoef->csc_out_dc2 =   16 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 =  128 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc0 =  128 * s32OutDcPre;
        }
        else
        {
            pstCscCoef->csc_coef00     = 1 * s32Pre;
            pstCscCoef->csc_coef01     = 0 * s32Pre;
            pstCscCoef->csc_coef02     = 0 * s32Pre;

            pstCscCoef->csc_coef10     = 0 * s32Pre;
            pstCscCoef->csc_coef11     = 1 * s32Pre;
            pstCscCoef->csc_coef12     = 0 * s32Pre;

            pstCscCoef->csc_coef20     = 0 * s32Pre;
            pstCscCoef->csc_coef21     = 0 * s32Pre;
            pstCscCoef->csc_coef22     = 1 * s32Pre;

            pstCscDcCoef->csc_in_dc2  = -16  * s32InDcPre;
            pstCscDcCoef->csc_in_dc1  = -128 * s32InDcPre;
            pstCscDcCoef->csc_in_dc0  = -128 * s32InDcPre;

            pstCscDcCoef->csc_out_dc2 =  16 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc1 = 128 * s32OutDcPre;
            pstCscDcCoef->csc_out_dc0 = 128 * s32OutDcPre;
        }
	return HI_SUCCESS;
}


HI_S32 VDP_FUNC_GetWcgGmmCoef(HI_U32 u32Layer, HI_U16 *pu16GmmCoef)
{

	return HI_SUCCESS;
}

HI_S32 VDP_FUNC_GetWcgDegmmCoef(HI_U32 u32Layer, HI_U16 *pu16DegmmCoef)
{
	return HI_SUCCESS;
}
