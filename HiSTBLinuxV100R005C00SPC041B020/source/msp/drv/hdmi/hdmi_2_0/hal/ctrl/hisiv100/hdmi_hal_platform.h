
/******************************************************************************
  File Name     : hdmi_hal_platform.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_PLATFORM_H__
#define __HDMI_HAL_PLATFORM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if 0
typedef enum
{
    HDMI_CLK_TYPE_CRG,
    HDMI_CLK_TYPE_PHY,
}HDMI_CLK_TYPE_E;
#endif

typedef enum
{
    HDMITX_CTRL_ID_DIV_2X,
    HDMITX_CTRL_ID_DIV_4X,
    HDMITX_CTRL_ID_DIV_6X,
    HDMITX_CTRL_ID_DIV_8X,
    
}HDMI_CTRL_ID_DIV_E;



typedef enum
{
    HDMITX_CTRL_PIXEL_DIV_2X,
    HDMITX_CTRL_PIXEL_DIV_4X,
    HDMITX_CTRL_PIXEL_DIV_6X,
    HDMITX_CTRL_PIXEL_DIV_8X,

}HDMI_CTRL_PIXEL_DIV_E;


typedef struct 
{
    HDMI_CTRL_ID_DIV_E      enIdDiv;
    HDMI_CTRL_PIXEL_DIV_E   enPixelDiv;
}HDMI_CTRL_DIV_S;



HI_S32 HAL_HDMI_PlatformHwReset(HI_BOOL bReset);
HI_S32 HAL_HDMI_PlatformSwReset(HI_BOOL bReset);
HI_S32 HAL_HDMI_PlatformClkGet(HDMI_CTRL_DIV_S *pstCtrlDiv,HDMI_CTRL_PIXEL_DIV_E *penPixDiv);
HI_S32 HAL_HDMI_PlatformClkCfg(HDMI_CLK_TYPE_E enClkType);
HI_S32 HAL_HDMI_PlatformClkDiv(HDMI_CTRL_DIV_S *pstClkDiv);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_PLATFORM_H__ */   

