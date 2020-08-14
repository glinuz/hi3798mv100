#include "drv_vdec_alg.h"

//#define OPTM_ALG_ACMCHECKPRINT 0

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

HI_U32 OPTM_ALG_InPTSSqn_CalNowRate(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 env)
{
    HI_U32 nowRate;
    HI_S32 PtsDiff;
    
    if (1 == env) {
	PtsDiff = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr] - pPtsInfo->InPTSSqn[0];
	PtsDiff = 10 * PtsDiff / pPtsInfo->nowPTSPtr;
    }
    else {
	PtsDiff = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr] - pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr - 1];
	PtsDiff = 10 * PtsDiff;
    }
                        
    if (PtsDiff <= 100000 && PtsDiff >= 120) //fps>0.1 && fps< 80
    {
        nowRate = 1000000 / PtsDiff;
        nowRate = OPTM_ALG_ROUND(nowRate);
        nowRate = nowRate / 10;
    }
    else
    {
        nowRate = 0;
    }
    
    return nowRate;
}
HI_VOID OPTM_ALG_InPTSSqn_ChangeInFrameRate(OPTM_ALG_FRD_S *pPtsInfo,HI_U32 nowRate)
{   
    pPtsInfo->InFrameRate = nowRate;
                            
    if (nowRate > 50)
        pPtsInfo->length = pPtsInfo->InFrameRate / 10;
    else
        pPtsInfo->length = 5;
                            
    if (pPtsInfo->InFrameRate > 300)
        pPtsInfo->StableThreshold = pPtsInfo->InFrameRate/100;
    else
        pPtsInfo->StableThreshold = 3;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
