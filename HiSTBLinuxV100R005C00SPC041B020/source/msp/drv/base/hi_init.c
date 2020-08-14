#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>

#include "hi_type.h"

#include "drv_vdec_ext.h"
#include "drv_omxvdec_ext.h"
#include "drv_pmoc_ext.h"
#include "drv_gpio_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_i2c_ext.h"
#include "drv_spi_ext.h"
#include "drv_sci_ext.h"
#include "drv_otp_ext.h"
#include "drv_sci_ext.h"
#include "drv_demux_ext.h"
#include "drv_pvr_ext.h"
#include "drv_sync_ext.h"
#include "drv_adec_ext.h"
#include "hi_drv_sci.h"
#include "drv_keyled_ext.h"
#include "drv_ir_ext.h"
#include "drv_jpeg_ext.h"
#include "drv_jpge_ext.h"
#include "drv_hifb_ext.h"
#include "drv_tde_ext.h"
#include "drv_tuner_ext.h"
#include "drv_png_ext.h"
#include "drv_disp_ext.h"
#include "drv_hdmi_ext.h"
#include "drv_cipher_ext.h"
#include "drv_vfmw_ext.h"
#include "drv_mce_ext.h"
#include "drv_vpss_ext.h"
#include "drv_venc_ext.h"
#include "drv_pdm_ext.h"
#include "drv_aenc_ext.h"
#include "drv_ao_ext.h"
#include "drv_adsp_ext.h"

#include "drv_vi_ext.h"
#include "drv_hdmirx_ext.h"
#include "drv_pq_ext.h"
#include "drv_advca_ext.h"

#ifdef HI_PWM_SUPPORT
#include "drv_pwm_ext.h"
#endif

#ifdef HI_SND_AMP_SUPPORT
#include "drv_amp_ext.h"
#endif

#ifdef HI_SND_SIF_SUPPORT
#include "drv_sif_ext.h"
#endif

#ifdef HI_ADAC_SLIC_SUPPORT
#include "drv_slic_ext.h"
#endif

#ifdef HI_GFX2D_SUPPORT
#include "drv_gfx2d_ext.h"
#endif

#include "drv_ci_ext.h"
#include "drv_cimaxplus_ext.h"
#include "drv_hdmirx_ext.h"

#ifndef MODULE
HI_S32 HI_DRV_LoadModules(HI_VOID)
{
    GPIO_DRV_ModInit();

#ifdef HI_GPIOI2C_SUPPORT
    GPIOI2C_DRV_ModInit();
#endif

#ifndef HI_IR_TYPE_LIRC
    IR_DRV_ModInit();
#endif

    PDM_DRV_ModInit();

    PQ_DRV_ModInit();

    HDMI_DRV_ModInit();

    VDP_DRV_ModInit();

    TDE_DRV_ModInit();

    HIFB_DRV_ModInit();

    JPEG_DRV_ModInit();

    PNG_DRV_ModInit();

#ifdef HI_GFX2D_SUPPORT
    GFX2D_OS_ModInit();
#endif

    CIPHER_DRV_ModInit();

    OTP_DRV_ModInit();

    ADVCA_DRV_ModeInit();

#ifndef HI_RECOVERY_SUPPORT
    I2C_DRV_ModInit();

#ifdef HI_SPI_BUS_SUPPORT
    SPI_DRV_ModInit();
#endif

#ifdef HI_FRONTEND_SUPPORT
    TUNER_DRV_ModInit();
#endif

    DMX_DRV_ModInit();
#endif

#ifdef HI_PWM_SUPPORT
    PWM_DRV_ModInit();
#endif

#if !defined(HI_LOADER_APPLOADER) && !defined(HI_RECOVERY_SUPPORT)
#ifdef HI_PVR_SUPPORT
    PVR_DRV_ModInit();
#endif

    SYNC_DRV_ModInit();

    ADSP_DRV_ModInit();

    AIAO_DRV_ModInit();

#ifdef HI_SND_SIF_SUPPORT
    SIF_DRV_ModInit();
#endif

#ifdef HI_SND_AMP_SUPPORT
    AMP_DRV_ModInit();
#endif

    ADEC_DRV_ModInit();

    VFMW_DRV_ModInit();

#ifdef HI_VDEC_SVDEC_SUPPORT
    SVDEC_ModeInit();
#endif

    VDEC_DRV_ModInit();

#ifdef HI_OMX_SUPPORT
    OMXVDEC_DRV_ModInit();
#endif

    VPSS_DRV_ModInit();

    MCE_DRV_ModInit();

#ifdef HI_JPGE_SUPPORT
    JPGE_DRV_ModInit();
#endif

#endif

#ifdef HI_KEYLED_SUPPORT
    KEYLED_DRV_ModInit();
#endif

#ifdef HI_SCI_SUPPORT
    SCI_DRV_ModInit();
#endif

#ifdef HI_AENC_SUPPORT
    AENC_DRV_ModInit();
#endif

#ifdef HI_VENC_SUPPORT
    VENC_DRV_ModInit();
#endif

#ifdef HI_VI_SUPPORT
    VI_DRV_ModInit();

#ifdef HI_VI_REAL_SUPPORT
#ifdef HI_HDMI_RX_SUPPORT
    HDMIRX_DRV_ModuleInit();
#else
    SIL9293_DRV_ModInit();
#endif
#endif
#endif

/* Please put pmoc in last one for stability*/
#if !defined(HI_RECOVERY_SUPPORT)
    PMOC_DRV_ModInit();
#endif

#ifdef HI_ADAC_SLIC_SUPPORT
    slac_init();
#endif

#ifdef HI_CIPLUS_SUPPORT
    CI_DRV_ModInit();
#endif

#ifdef HI_CI_DEV_CIMAXPLUS
    CIMAXPLUS_DRV_ModInit();
#endif

    return HI_SUCCESS;
}

late_initcall(HI_DRV_LoadModules);
#endif

