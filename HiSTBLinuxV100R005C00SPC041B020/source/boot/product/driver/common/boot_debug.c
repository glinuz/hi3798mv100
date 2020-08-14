#include <uboot.h>
#include <stdarg.h>
#include "hi_common.h"

#ifdef HI_BOOT_LOG_SUPPORT

#define HI_BOOT_MAX_LOG_LEN     256
#define HI_BOOT_MAX_MODULE_NAME (16)

typedef struct tagMODULE_LOG_PARAM_S
{
    HI_CHAR aszName[HI_BOOT_MAX_MODULE_NAME];
    HI_LOG_LEVEL_E enLevel;
}MODULE_LOG_PARAM_S;

static MODULE_LOG_PARAM_S g_astModuleLogLevel[HI_ID_BUTT];

//static HI_CHAR g_aszLogLevel[HI_LOG_LEVEL_BUTT][8] = {"FATAL", "ERROR", "WARN ", "INFO ", "DEBUG"};

HI_VOID HI_LOG_Init(HI_VOID)
{
    HI_S32 i;

    /* Init array */
    memset(g_astModuleLogLevel, 0, sizeof(g_astModuleLogLevel));

    /* Init level */
    for (i = 0; i < HI_ID_BUTT; i++)
    {
        g_astModuleLogLevel[i].enLevel = HI_LOG_LEVEL_FATAL;
    }

    /* Init name */
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_SYS].aszName, "common", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_CA].aszName, "advca", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_DEMUX].aszName, "demux", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_DISP].aszName, "disp", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_FLASH].aszName, "flash", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_TUNER].aszName, "frontend", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_GPIO].aszName, "gpio", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_HDMI].aszName, "hdmi", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_FB].aszName, "higo", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_I2C].aszName, "i2c", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_IR].aszName, "ir", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_KEYLED].aszName, "keyled", HI_BOOT_MAX_MODULE_NAME);
    /* For pdm and db */
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_PDM].aszName, "pdm", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_PMOC].aszName, "pmoc", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_LOADER].aszName, "loader", HI_BOOT_MAX_MODULE_NAME);
    HI_OSAL_Strncpy(g_astModuleLogLevel[HI_ID_FASTPLAY].aszName, "mce", HI_BOOT_MAX_MODULE_NAME);
}

const HI_CHAR* HI_LOG_GetModuleName(HI_MOD_ID_E enModule)
{
    if (enModule >= HI_ID_BUTT)
    {
        return HI_NULL;
    }

    return g_astModuleLogLevel[enModule].aszName;
}

HI_VOID HI_LOG_SetLevel(HI_MOD_ID_E enModule, HI_U32 u32LogLevel)
{
    HI_S32 i;

    if (u32LogLevel >= HI_LOG_LEVEL_BUTT)
    {
        return;
    }

    if (enModule >= HI_ID_BUTT)
    {
        for (i = 0; i < (HI_S32)HI_ID_BUTT; i++)
        {
            g_astModuleLogLevel[i].enLevel = (HI_LOG_LEVEL_E)u32LogLevel;
        }
    }
    else
    {
        g_astModuleLogLevel[enModule].enLevel = (HI_LOG_LEVEL_E)u32LogLevel;
    }
}

HI_VOID HI_LogOut(HI_U32 u32Level, HI_MOD_ID_E enModId, HI_U8 *pFuncName, HI_U32 u32LineNum, const char *format, ...)
{
    HI_S32      LogLen;
    HI_CHAR     LogStr[HI_BOOT_MAX_LOG_LEN] = {0};
    va_list     args;

    if ((enModId >= HI_ID_BUTT) || (u32Level > g_astModuleLogLevel[enModId].enLevel))
    {
        return;
    }

    if (u32Level >= HI_LOG_LEVEL_BUTT)
    {
        u32Level = HI_LOG_LEVEL_INFO;
    }

    va_start(args, format);
    LogLen = HI_OSAL_Vsnprintf(LogStr, HI_BOOT_MAX_LOG_LEN, format, args);
    va_end(args);

    if (LogLen < 0)
    {
        printf("HI_OSAL_Vsnprintf call error in %s\n", __FUNCTION__);
        return;
    }

    LogStr[HI_BOOT_MAX_LOG_LEN-1] = '\0';

    //printf("%s[%d]%s: %s", pFuncName, u32LineNum, g_aszLogLevel[u32Level], LogStr);

    return;
}
#else
HI_VOID HI_LOG_Init(HI_VOID)
{
}

HI_VOID HI_LOG_SetLevel(HI_MOD_ID_E enModule, HI_U32 u32LogLevel)
{
}

HI_VOID HI_LogOut(HI_U32 u32Level, HI_MOD_ID_E enModId, HI_U8 *pFuncName, HI_U32 u32LineNum, const char *format, ...)
{
}

const HI_CHAR* HI_LOG_GetModuleName(HI_MOD_ID_E enModule)
{
    return HI_NULL;
}
#endif

