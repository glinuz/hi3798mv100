/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCG6A3KHwW/DEcLM2NcwtJs7QdH8sM1n3S/1wSu
i69XAWS08VJHI1kx+SuVHjl6tqJ7qcpzxJKUZgsOYOxf1E2ntQ1oIqA2glcCLl2IOd/driV+
7lkrm9CyvKEvmuur3s5nJVgz9eo7x5yrrP73AmQRw9gBuVfFmWWAY7pGpuTGxQ==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

#include    "syntax.h"
#include    "real8.h"
#include    "public.h"
#include    "bitstream.h"
#include    "vfmw.h"

#include    "vdm_hal.h"
#include    "postprocess.h"
#include    "vfmw_ctrl.h"
#include    "fsp.h"


#ifdef REAL8_ENABLE



SINT32 REAL8DEC_Init(RV8_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData)
{

    return 0;
}


VOID REAL8DEC_Destroy(RV8_CTX_S *pCtx)
{
    return;
}


SINT32 REAL8DEC_Decode(RV8_CTX_S *pCtx, DEC_STREAM_PACKET_S *pPacket)
{
    return RV8_VFMW_FALSE;
}


SINT32 REAL8DEC_RecycleImage(RV8_CTX_S *pCtx, UINT32 ImgID)
{
    return RV8_VFMW_FALSE;
}


SINT32 REAL8DEC_GetRemainImg(RV8_CTX_S *pCtx)
{
    return -1;
}


UINT32 REAL8DEC_VDMPostProc(RV8_CTX_S *pCtx, SINT32 ErrRatio, UINT32 Mb0QpInCurrPic)
{
    return RV8_VFMW_FALSE;
}


SINT32 REAL8DEC_GetImageBuffer( RV8_CTX_S *pCtx )
{
    return 0;
}
VOID REAL8DEC_Support(SINT32 *flag)
{
    *flag = 0;
    return;
}


#endif //REAL8_VFMW_ENABLE
