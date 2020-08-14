#include "si_typedefs.h"
#include "hi_drv_hdmi.h"


#ifndef __TX_PHY_H__
#define __TX_PHY_H__


#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3798cv100)

// 65Mhz
#define OSC_CLK_SELECT  65000000

#elif  defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv310) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)

// 50Mhz
#define OSC_CLK_SELECT  50000000

#define PHY_OE_ADDR         0x00
#define     RG_TX_RSTB      0x01

#define PHY_PWD_ADDR        0x01
#define     RG_TX_EN        0x01

#define PHY_AUD_ADDR        0x02

#define PHY_PLL1_ADDR       0x03

#define PHY_PLL2_ADDR       0x04
#define     MASK_DEEPCOLOR  0x03

#define PHY_DRV_ADDR        0x05

#define PHY_CLK_ADDR        0x06


#endif
//#define BOARD_TYPE_S40V2_fpga

#if    defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3716mv420) \
    || defined(CHIP_TYPE_hi3716mv410)
typedef enum
{
    HI_HDMI_PHY_AUDIO_CLK_5P3   = 0x00,
    HI_HDMI_PHY_AUDIO_CLK_5     = 0x01,
    HI_HDMI_PHY_AUDIO_CLK_1     = 0x02,
    HI_HDMI_PHY_AUDIO_CLK_5P2   = 0x04,
    HI_HDMI_PHY_AUDIO_CLK_BUTT
}HI_HDMI_PHY_AUDIO_CLK_E;

#define CLK_100MHZ      100000000
#endif

HI_S32 SI_TX_PHY_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value);
HI_S32 SI_TX_PHY_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value);

HI_S32 SI_TX_PHY_HighBandwidth(HI_BOOL bTermEn);
HI_S32 SI_TX_PHY_4KRisingTime(HI_BOOL b4KFmt);


HI_S32 SI_TX_PHY_GetOutPutEnable(void);

HI_S32 SI_TX_PHY_DisableHdmiOutput(void);
HI_S32 SI_TX_PHY_EnableHdmiOutput(void);

void SI_TX_PHY_INIT(void);

HI_S32 SI_TX_PHY_PowerDown(HI_BOOL bPwdown);

HI_S32 SI_TX_PHY_SetDeepColor(HI_U8 bDeepColor);

HI_S32 SI_TX_PHY_DVIEncoder(HI_BOOL bEnable);

HI_S32 SI_Adjust_SwingCtrl(HI_UNF_ENC_FMT_E fmt, HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode); 
#endif

