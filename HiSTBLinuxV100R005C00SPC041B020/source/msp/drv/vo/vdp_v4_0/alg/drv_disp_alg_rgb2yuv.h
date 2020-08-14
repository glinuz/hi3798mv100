
#ifndef __DRV_DISP_ALG_RGB2YUV_H__
#define __DRV_DISP_ALG_RGB2YUV_H__

#include "hi_drv_video.h"

typedef struct
{
    HI_U8 u8Red;
    HI_U8 u8Green;
    HI_U8 u8Blue;

    HI_U8 u8Y;
    HI_U8 u8Cb;
    HI_U8 u8Cr;
}ALG_COLOR_S;

HI_VOID DISP_ALG_CscRgb2Yuv(ALG_COLOR_S *pstRgbColor, ALG_COLOR_S *pYuvColor);

#endif /*__DRV_DISP_ALG_RGB2YUV_H__*/

