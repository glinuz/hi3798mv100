/******************************************************************************
 *    Copyright (c) 2009-2012 by Hisi.
 *    All rights reserved.
 * ***
 *    Create by Czyong. 2012-09-07
 *
******************************************************************************/

#include <uboot.h>
#include "hi_type.h"
#include "hi_unf_disp.h"
#include "hi_drv_pdm.h"
#include "hi_drv_pq.h"
#include "hi_boot_logo.h"
#include "hi_boot_adp.h"
#include "schedule.h"
#include "hi_common.h"
#include "hi_unf_otp.h"
#include "hi_reg.h"

#include "hi_drv_gpio.h"
#include "hi_drv_i2c.h"

#include "hi_go_common.h"
#include "hi_go_logo.h"

#ifdef HI_BUILD_WITH_KEYLED
#include "hi_unf_keyled.h"
#endif

/***************************** Macro Definition ******************************/
#if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#define SC_GEN20    0xf80000d0
#endif

#ifdef HI_BUILD_WITH_KEYLED
#if defined HI_BOOT_KEYLED_74HC164
#define PRODUCT_LED_VALUE_BOOT               (0xe1c5c5c1)
#elif defined (HI_BOOT_KEYLED_PT6961) || defined(HI_BOOT_KEYLED_PT6964) || defined(HI_BOOT_KEYLED_FD650)
#define PRODUCT_LED_VALUE_BOOT               (0x785c5c7c)
#elif defined HI_BOOT_KEYLED_CT1642
#define PRODUCT_LED_VALUE_BOOT               (0x1e3a3a3e)
#else
#define PRODUCT_LED_VALUE_BOOT               (0x785c5c7c)
#endif
#endif

#ifdef HI_ADVCA_SUPPORT
extern HI_S32 HI_SSA_Entry(HI_VOID);
HI_S32 HI_SSA_Init(HI_VOID);
#endif

#ifdef HI_MINIBOOT_SUPPORT
extern char *getenv(const char *key);
#endif

extern int Android_Qb_CheckRemote(void);
extern HI_S32 HI_OTP_LockIdWord(HI_VOID);

extern void selfboot_init(void);
extern void selfboot_from_udisk(void);
extern int Android_Main(void);

#define IO_ADDRESS(addr) (addr)

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
static HI_BOOL                     g_bDisplayInit = HI_FALSE;
static HI_DISP_PARAM_S             g_stDispParam0;
static HI_DISP_PARAM_S             g_stDispParam1;

/******************************* API declaration *****************************/

/*****************************************************************************
* func             PRODUCT_DisplayInit
* description   :  display初始化
* retval        :  NA
* others:       :  NA
*****************************************************************************/
HI_S32 PRODUCT_DisplayInit(HI_VOID)
{
    HI_S32            s32Ret = HI_SUCCESS;

    if(g_bDisplayInit)
    {
        return HI_SUCCESS;
    }

    s32Ret = HI_ADP_DispInit();
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_MCE("HI_ADP_DispInit err! s32Ret = %x\n", s32Ret);
        return s32Ret;
    }

    g_bDisplayInit = HI_TRUE;

    return HI_SUCCESS;

}


/*****************************************************************************
* func             Logo_Main
* description   :  开机logo入口
* retval        :  NA
* others:       :  NA
*****************************************************************************/
HI_S32 Logo_Main(HI_BOOL bNeedFastplay,HI_PDM_MEMINFO_S *pstMemInfo)
{
    HI_S32           s32Ret = HI_SUCCESS;
    HI_U32           u32LogoDataAddr = 0;
    HI_LOGO_PARAM_S  stLogoParam;
#ifdef HI_MCE_SUPPORT
    HI_MCE_PARAM_S   stMceParam;
    HI_U32           u32PlayDataAddr;
#endif
    HI_GO_LOGO_INFO  LocalLogoInfo;

    if(HI_NULL != pstMemInfo)
    {
        s32Ret = HI_DRV_PDM_SetSource(HI_PDM_SRCTYPE_DDR, pstMemInfo);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_MCE("HI_DRV_PDM_SetSource err! s32Ret = %x\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY0, &g_stDispParam0);
    s32Ret|= HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &g_stDispParam1);
    if(HI_SUCCESS != s32Ret)
    {
        HI_INFO_MCE("HI_PDM_GetGrcParam err! s32Ret = %x\n", s32Ret);
        return s32Ret;
    }

    /**========================================================================
     **begin gfx logo
     **========================================================================**/
    s32Ret = HI_DRV_PDM_GetLogoParam(&stLogoParam);
    if(HI_SUCCESS == s32Ret && 1 == stLogoParam.u32CheckFlag)
    {
        s32Ret = PRODUCT_DisplayInit();
        if(HI_SUCCESS != s32Ret)
        {
            HI_INFO_MCE("PRODUCT_DisplayInit err! s32Ret = %x\n", s32Ret);
            return s32Ret;
        }

        s32Ret = HI_DRV_PDM_GetLogoData(stLogoParam.u32LogoLen, &u32LogoDataAddr);
        if(HI_SUCCESS != s32Ret)
        {
            HI_INFO_MCE("HI_DRV_PDM_GetLogoData err! s32Ret = %x\n", s32Ret);
            return s32Ret;
        }

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
        HI_UNF_HDMI_Init();

        if ((g_stDispParam1.stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].enIntfType == HI_UNF_DISP_INTF_TYPE_HDMI)
          &&(g_stDispParam1.stIntf[HI_UNF_DISP_INTF_TYPE_HDMI].unIntf.enHdmi == HI_UNF_HDMI_ID_0))
        {
            HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_0, g_stDispParam1.enFormat);
        }
#endif
    
        memset(&LocalLogoInfo,0x0,sizeof(HI_GO_LOGO_INFO));
        LocalLogoInfo.DispToSD      = (g_stDispParam0.enSrcDisp != HI_UNF_DISPLAY1) ? (HI_TRUE) : (HI_FALSE);
        LocalLogoInfo.Support4KLogo = HI_FALSE;
        LocalLogoInfo.LogoDataLen   = stLogoParam.u32LogoLen;
        LocalLogoInfo.LogoDataBuf   = u32LogoDataAddr;

        LocalLogoInfo.DispColorFmt[CONFIG_GFX_DISP_LOGO_SD] = g_stDispParam0.enPixelFormat;
        LocalLogoInfo.DispVirtScreenWidth[CONFIG_GFX_DISP_LOGO_SD]  = g_stDispParam0.u32VirtScreenWidth;
        LocalLogoInfo.DispVirtScreenHeight[CONFIG_GFX_DISP_LOGO_SD] = g_stDispParam0.u32VirtScreenHeight;

        LocalLogoInfo.DispColorFmt[CONFIG_GFX_DISP_LOGO_HD] = g_stDispParam1.enPixelFormat;
        LocalLogoInfo.DispVirtScreenWidth[CONFIG_GFX_DISP_LOGO_HD]  = g_stDispParam1.u32VirtScreenWidth;
        LocalLogoInfo.DispVirtScreenHeight[CONFIG_GFX_DISP_LOGO_HD] = g_stDispParam1.u32VirtScreenHeight;

        s32Ret = HI_GO_DisplayLogo(&LocalLogoInfo);
        if (HI_SUCCESS != s32Ret){
            return HI_FAILURE;
        }

    }

#ifdef HI_MCE_SUPPORT
    if (bNeedFastplay)
    {
        s32Ret = HI_DRV_PDM_GetMceParam(&stMceParam);
        if(HI_SUCCESS == s32Ret && 1 == stMceParam.u32CheckFlag)
        {
            if(0 != stMceParam.u32PlayDataLen)
            {
                s32Ret = HI_DRV_PDM_GetMceData(stMceParam.u32PlayDataLen, &u32PlayDataAddr);
            }
        }
    }
#endif

    HI_INFO_MCE("boot logo show!!!\n");

    return HI_SUCCESS;
}

HI_VOID SetLogLevel(HI_VOID)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_CHAR         *pLogLevel = HI_NULL;
    HI_U32          LogLevel = 1;
    HI_CHAR         aszEnvName[64];
    HI_S32          i;
    const HI_CHAR   *pszName;

    HI_LOG_Init();

    pLogLevel = getenv("loglevel");
    if (HI_NULL == pLogLevel)
    {
        HI_LOG_SetLevel(HI_ID_BUTT, 0);
    }
    else
    {
        LogLevel = simple_strtoul_2(pLogLevel, NULL, 10);
        if (LogLevel > HI_TRACE_LEVEL_DBG)
        {
            HI_LOG_SetLevel(HI_ID_BUTT, 0);
        }
        else
        {
            HI_LOG_SetLevel(HI_ID_BUTT, LogLevel);
        }
    }

    for (i=0; i<HI_ID_BUTT; i++)
    {
        pszName = HI_LOG_GetModuleName(i);
        if ((HI_NULL != pszName) && (strlen(pszName) > 0))
        {
            HI_OSAL_Snprintf(aszEnvName, sizeof(aszEnvName), "loglevel-%s", pszName, sizeof(aszEnvName));
            pLogLevel = getenv(aszEnvName);
            if (HI_NULL != pLogLevel)
            {
                LogLevel = simple_strtoul_2(pLogLevel, NULL, 10);
                if (LogLevel > HI_TRACE_LEVEL_DBG)
                {
                    LogLevel = 0;
                }
                HI_LOG_SetLevel(i, LogLevel);
            }
        }
    }

#endif
    return;
}


HI_S32 product_init(HI_VOID)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)

    HI_OTP_LockIdWord();

#ifdef HI_BOOT_IR_SUPPORT
    register_hibernate_cancel(Android_Qb_CheckRemote);
#endif
#endif

    return HI_SUCCESS;
}

extern void set_core_voltage_ca(void);
extern void boot_suspend(void);

#ifdef HI_ADVCA_USB_BURN_KEY
extern int Android_Security_WriteRSAKey(void);
#endif

int fastapp_entry(int argc, char *argv[])
{
    int     LogoEnable = 1;
    char   *pLogo;
#ifdef HI_HDMI_EDID_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
#endif

    HI_PRINT("\nSDK Version: %s\n\n", SDK_VERSION_STRING);

    SetLogLevel();

#if defined(HI_ADVCA_VERIFY_ENABLE) && defined(HI_ADVCA_SUPPORT)
    HI_SSA_Init();
#endif


#ifdef HI_PM_POWERUP_MODE1_SUPPORT
    boot_suspend();
#endif

    HI_PQ_Init();

#ifdef HI_HDMI_EDID_SUPPORT
    Ret = HI_UNF_HDMI_SetBaseParamByEDID();
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PRINT(HI_ID_SYS, "SetBaseParamByEDID err! Ret = %x\n", Ret);
    }
#endif

    HI_DRV_GPIO_Init();
    HI_DRV_I2C_Init();

#ifdef HI_BUILD_WITH_KEYLED
    HI_UNF_KEYLED_Init();
    HI_UNF_KEY_Open();
    HI_UNF_LED_Open();
	HI_UNF_LED_Display(PRODUCT_LED_VALUE_BOOT);
#endif

#if defined(HI_ADVCA_VERIFY_ENABLE) && defined(HI_ADVCA_SUPPORT)
    HI_SSA_Entry();
    return 0;
#endif

    pLogo = getenv("logo");
    if (pLogo)
    {
        if (0 == strncmp(pLogo, "0", sizeof("0")))
        {
            LogoEnable = 0;

        #if defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
            HI_REG_WRITE32(SC_GEN20, 0x2);
        #endif
        }
        else
        {
            LogoEnable = 1;
        }
    }

    if (1 == LogoEnable)
    {
        Logo_Main(HI_TRUE, HI_NULL);
    }

    HI_DRV_PDM_SetTagData();

#ifndef HI_MINIBOOT_SUPPORT
#if defined(CONFIG_BOOTROM_SUPPORT) && defined(HI_ANDROID_BOOT_SUPPORT)
    selfboot_init();

#ifdef HI_ADVCA_USB_BURN_KEY
    Android_Security_WriteRSAKey();
#endif

    selfboot_from_udisk();
#endif
#endif

#ifdef HI_ADVCA_SUPPORT
    #if defined(HI_ANDROID_BOOT_SUPPORT) && defined(HI_ADVCA_TYPE_OTHER)
        Android_Main();
    #endif
#else
    #ifdef HI_LOADER_SUPPORT
        Loader_Main();
    #else
        #ifdef HI_ANDROID_BOOT_SUPPORT
            Android_Main();
        #endif
    #endif
#endif

#ifdef HI_ADVCA_SUPPORT
 #ifdef HI_AVS_SUPPORT
    set_core_voltage_ca();
 #endif
#endif

    return 0;
}

