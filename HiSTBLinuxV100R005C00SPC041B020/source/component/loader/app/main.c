/*******************************************************************************
 *             Copyright 2013 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: main.c
 * Description:
 *       HiLoader entry definition here.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 ******************************************************************************/
#include "hi_unf_pdm.h"

#include "com_osal.h"
#include "com_debug.h"
#include "com_config.h"

#include "loader_dbinfo.h"
#include "protocol.h"
#include "burn.h"
#include "download_fs.h"

#include "ui_config.h"
#include "ui_keyled.h"
#include "ui_display.h"
#include "ui_gfx.h"
#include "ui_language.h"
#include "ui_win_setting.h"
#include "ui_win_main.h"
#include "ui_win_msgbox.h"

#ifdef HI_LOADER_BOOTLOADER
#include <common.h>
#include <command.h>
#else
#include <sys/reboot.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static LDDB_LOADER_INFO_S   g_stLoaderInfo;
static HI_BOOL              g_bFirstBurn = HI_TRUE;

static HI_VOID printLoaderInformation(LDDB_LOADER_INFO_S* pstInfoParam)
{
    COM_INFO(" **************** Loader Information ****************\n");
    COM_INFO(" bNeedUpgrade       : %u\n", pstInfoParam->stTAGParamData.bNeedUpgrade);
    COM_INFO(" bForceUpgrade      : %u\n", pstInfoParam->stTAGParamData.bForceUpgrade);
    COM_INFO(" bUsbBurn           : %u\n", pstInfoParam->stTAGParamData.bUsbBurn);
    COM_INFO(" bTagDestroy        : %u\n", pstInfoParam->stPrivateParam.bTagDestroy);
    COM_INFO(" u32FailedCnt       : %u\n", pstInfoParam->stPrivateParam.u32FailedCnt);
    COM_INFO(" enUpgradeType      : %u\n", pstInfoParam->stUPDParamData.enUpgradeType);
    COM_INFO(" enUpgradeMode      : %u\n", pstInfoParam->stUPDParamData.enUpgradeMode);
    COM_INFO(" u32SoftwareVersion : %u\n", pstInfoParam->stUPDParamData.u32SoftwareVersion);

    switch (pstInfoParam->stUPDParamData.enUpgradeType)
    {
        case HI_LOADER_UPGRADE_TYPE_USB:
        {
            COM_INFO(" as8FileName        : %s\n", pstInfoParam->stUPDParamData.unParam.stUSBParam.as8FileName);
            break;
        }

        case HI_LOADER_UPGRADE_TYPE_OTA:
        {
            LDDB_PARAM_OTA_S* pstOTAParam = &pstInfoParam->stUPDParamData.unParam.stOTAParam;

            COM_INFO(" enSigType          : %u\n", pstOTAParam->enSigType);
            COM_INFO(" u32TunerID         : %u\n", pstOTAParam->u32TunerID);
            COM_INFO(" u32Pid             : %u\n", pstOTAParam->u32Pid);

            if (HI_UNF_TUNER_SIG_TYPE_CAB == pstOTAParam->enSigType)
            {
                LDDB_CONNECT_PARAM_CAB_S* pstConnectParam = &pstOTAParam->unParam.stCab.stConnectParam;
                COM_INFO(" u32Freq            : %u\n", pstConnectParam->u32Freq);
                COM_INFO(" u32SymbolRate      : %u\n", pstConnectParam->u32SymbolRate);
                COM_INFO(" enModType          : %u\n", pstConnectParam->enModType);
                COM_INFO(" bReverse           : %u\n", pstConnectParam->bReverse);
            }
            else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstOTAParam->enSigType)
            {
                LDDB_CONNECT_PARAM_SAT_S* pstConnectParam = &pstOTAParam->unParam.stSat.stConnectParam;
                COM_INFO(" u32Freq            : %u\n", pstConnectParam->u32Freq);
                COM_INFO(" u32SymbolRate      : %u\n", pstConnectParam->u32SymbolRate);
                COM_INFO(" enPolar            : %u\n", pstConnectParam->enPolar);
                COM_INFO(" u32ScrambleValue   : %u\n", pstConnectParam->u32ScrambleValue);
            }
            else if (((HI_UNF_TUNER_SIG_TYPE_DVB_T & pstOTAParam->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DVB_T2 & pstOTAParam->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ISDB_T & pstOTAParam->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ATSC_T & pstOTAParam->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DTMB & pstOTAParam->enSigType) != 0))
            {
                LDDB_CONNECT_PARAM_TER_S* pstConnectParam = &pstOTAParam->unParam.stTer.stConnectParam;
                COM_INFO(" u32Freq            : %u\n", pstConnectParam->u32Freq);
                COM_INFO(" u32BandWidth       : %u\n", pstConnectParam->u32BandWidth);
                COM_INFO(" enModType          : %u\n", pstConnectParam->enModType);
                COM_INFO(" bReverse           : %u\n", pstConnectParam->bReverse);
                COM_INFO(" enChannelMode      : %u\n", pstConnectParam->enChannelMode);
                COM_INFO(" enDVBTPrio         : %u\n", pstConnectParam->enDVBTPrio);
            }
            else
            {
                COM_INFO(" Error Loader Param !");
            }

            break;
        }

        case HI_LOADER_UPGRADE_TYPE_IP:
        {
            HI_LOADER_PARAM_IP_S* pstIPParam = &pstInfoParam->stUPDParamData.unParam.stIPParam;

            COM_INFO(" enProtType         : %u\n", pstIPParam->enProtType);
            COM_INFO(" enIPCfgType        : %u\n", pstIPParam->enIPCfgType);

            COM_PrintIPAddr(" ipOurIp            ", pstIPParam->ipOurIp);
            COM_PrintIPAddr(" ipGateway          ", pstIPParam->ipGateway);
            COM_PrintIPAddr(" ipSubnetMask       ", pstIPParam->ipSubnetMask);
            COM_PrintIPAddr(" ipServer           ", pstIPParam->ipServer);

            COM_INFO(" ipServerPort       : %u\n", pstIPParam->ipServerPort);
            COM_INFO(" u32FileSize        : %u\n", pstIPParam->u32FileSize);
            COM_INFO(" as8FileName        : %s\n", pstIPParam->as8FileName);
            COM_INFO(" as8UserName        : %s\n", pstIPParam->as8UserName);
            break;
        }

        case HI_LOADER_UPGRADE_TYPE_FS:
        {
            COM_INFO(" as8FileName        : %s\n", pstInfoParam->stUPDParamData.unParam.stFSParam.as8FileName);
            COM_INFO(" as8PartitonName    : %s\n", pstInfoParam->stUPDParamData.unParam.stFSParam.as8PartitonName);
            break;
        }

        default:
        {
            COM_INFO(" Error Loader Param !\n");
        }
    }

    COM_INFO(" ****************************************************\n");
    return;
}

static HI_S32 loaderBurnData(COM_BUFFER_DATABLOCK_S* pstDataBuffer, HI_U32 u32BufferSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_TRUE == g_bFirstBurn)
    {
        g_bFirstBurn = HI_FALSE;

        g_stLoaderInfo.stPrivateParam.bTagDestroy = HI_TRUE;
        g_stLoaderInfo.stTAGParamData.bNeedUpgrade = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&g_stLoaderInfo);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }
    }

    s32Ret = LOADER_BURN_WriteData(pstDataBuffer, u32BufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_BURN_WriteData, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 loaderUIInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_PDM_DISP_PARAM_S stDispParam;

    COM_DBG_FuncEnter();

    s32Ret = UI_KEYLED_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_KEYLED_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stDispParam, 0x00, sizeof(stDispParam));
    s32Ret = UI_GetDisplayParam(&stDispParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_GetDisplayParam, s32Ret);

        COM_CHECK(UI_KEYLED_DeInit());
        return s32Ret;
    }

    s32Ret = UI_DisplayInit(&stDispParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_DisplayInit, s32Ret);

        COM_CHECK(UI_KEYLED_DeInit());
        return s32Ret;
    }

    s32Ret = UI_GraphicsInit(&stDispParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_GraphicsInit, s32Ret);

        COM_CHECK(UI_DisplayDeInit());
        COM_CHECK(UI_KEYLED_DeInit());
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 loaderUIDeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK(UI_GraphicsDeInit());
    COM_CHECK(UI_DisplayDeInit());
    COM_CHECK(UI_KEYLED_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 loaderClearUpgradeParam(HI_VOID)
{
    LDDB_LOADER_INFO_S stLoaderInfo;
    HI_S32 s32Ret = HI_FAILURE;

    COM_MemSet(&stLoaderInfo, 0x00, sizeof(stLoaderInfo));
    s32Ret = LDDB_SetLoaderInfo(&stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_BOOL loaderCheckEnforceType(LDDB_LOADER_INFO_S* pstLoaderDBInfo)
{
    if ((HI_TRUE == pstLoaderDBInfo->stTAGParamData.bForceUpgrade)
        || (HI_TRUE == pstLoaderDBInfo->stTAGParamData.bUsbBurn))
    {
        return HI_TRUE;
    }

    if ((HI_TRUE == pstLoaderDBInfo->stPrivateParam.bTagDestroy)
        && (COM_CFG_D_RETRY_TIMES <= pstLoaderDBInfo->stPrivateParam.u32FailedCnt))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 loaderCheckPolicy(LDDB_LOADER_INFO_S* pstLoaderDBInfo)
{
    HI_S32 s32Ret = HI_FAILURE;

    printLoaderInformation(pstLoaderDBInfo);

    if (HI_TRUE != pstLoaderDBInfo->stTAGParamData.bNeedUpgrade)
    {
        COM_PrintErrCode(COM_ERR_DATABASE);
        return COM_ERR_DATABASE;
    }

    if ((HI_TRUE != pstLoaderDBInfo->stPrivateParam.bTagDestroy)
        && (COM_CFG_D_RETRY_TIMES <= pstLoaderDBInfo->stPrivateParam.u32FailedCnt + 1))
    {
        COM_CHECK(loaderClearUpgradeParam());
    }
    else
    {
        pstLoaderDBInfo->stPrivateParam.u32FailedCnt++;
        COM_CHECK(LDDB_SetLoaderInfo(pstLoaderDBInfo));
    }

    if (loaderCheckEnforceType(pstLoaderDBInfo))
    {
#ifdef HI_LOADER_MODE_USB
        HI_LOADER_PARAM_S stParameter;

        pstLoaderDBInfo->stUPDParamData.enUpgradeType = HI_LOADER_UPGRADE_TYPE_USB;
        COM_StrNCpy(pstLoaderDBInfo->stUPDParamData.unParam.stUSBParam.as8FileName, COM_CFG_D_UPGRADE_FILENAME, HI_LOADER_FILENAME_LEN - 1);
        COM_MemSet(&stParameter, 0x00, sizeof(&stParameter));
        LDDB_UPDPARAM_CONVERT(&stParameter, &pstLoaderDBInfo->stUPDParamData);
        s32Ret = DOWNLOAD_FS_Init(&stParameter);
        if (HI_SUCCESS == s32Ret)
        {
            COM_CHECK(DOWNLOAD_FS_DeInit());
            return HI_SUCCESS;
        }

        COM_PrintFuncErr(DOWNLOAD_FS_Init, s32Ret);
#endif

#ifdef HI_LOADER_MODE_OTA
#ifdef HI_LOADER_TUNER_CAB
        s32Ret = UI_ShowOtaSettingWin(&pstLoaderDBInfo->stUPDParamData.unParam.stOTAParam);
        if (HI_SUCCESS == s32Ret)
        {
            pstLoaderDBInfo->stUPDParamData.enUpgradeType = HI_LOADER_UPGRADE_TYPE_OTA;
            return HI_SUCCESS;
        }

        COM_PrintFuncErr(UI_ShowOtaSettingWin, s32Ret);
#endif
#endif

        return s32Ret;
    }

    if (HI_LOADER_UPGRADE_TYPE_BUTT <= pstLoaderDBInfo->stUPDParamData.enUpgradeType)
    {
        COM_PrintErrCode(COM_ERR_DATABASE);
        return COM_ERR_DATABASE;
    }

    return HI_SUCCESS;
}

static HI_S32 loaderCheckVersionMatch(LDDB_LOADER_INFO_S* pstLoaderDBInfo, PROT_VERSION_INFO_S* pstUpgradeInfo)
{
    LDDB_DEVICE_INFO_S stDeviceInfo;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_MemSet(&stDeviceInfo, 0x00, sizeof(stDeviceInfo));
    s32Ret = LDDB_GetDeviceInfo(&stDeviceInfo);

    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LDDB_GetDeviceInfo, s32Ret);
        return s32Ret;
    }

    COM_INFO(" **************** Device Information ****************\n");
    COM_INFO(" Manufacturer Identify        : 0x%08x\n", stDeviceInfo.u32OUI);
    COM_INFO(" Area Code                    : 0x%08x\n", stDeviceInfo.u32AreaCode);
    COM_INFO(" Hardware Version             : 0x%08x\n", stDeviceInfo.u32HardwareVersion);
    COM_INFO(" Serial Number                : 0x%08x\n", stDeviceInfo.u32SerialNum);
    COM_INFO(" ****************************************************\n");

    COM_INFO(" ************ Upgrade Stream Information ************\n");
    COM_INFO(" Manufacturer Identify        : 0x%08x\n", pstUpgradeInfo->u32ManufacturerID);
    COM_INFO(" Hardware Version             : 0x%08x\n", pstUpgradeInfo->u32HardwareVersion);
    COM_INFO(" Software Version             : 0x%08x\n", pstUpgradeInfo->u32SoftwareVersion);
    COM_INFO(" Start Serial Number          : 0x%08x\n", pstUpgradeInfo->u32SerialNumberStart);
    COM_INFO(" End Serial Number            : 0x%08x\n", pstUpgradeInfo->u32SerialNumberEnd);
    COM_INFO(" ****************************************************\n");

    /* vendor ID */
    if (pstUpgradeInfo->u32ManufacturerID != stDeviceInfo.u32OUI)
    {
        COM_PrintErrInfo("upgrade stream vendor ID is not same with local stb info.");
        return COM_ERR_IMAGE_UNAVAILABLE;
    }

    /* HW version */
    if (pstUpgradeInfo->u32HardwareVersion != stDeviceInfo.u32HardwareVersion)
    {
        COM_PrintErrInfo("upgrade stream hardware version is not same with local stb info.");
        return COM_ERR_IMAGE_UNAVAILABLE;
    }

    /* HW serial number */
    if ((stDeviceInfo.u32SerialNum < pstUpgradeInfo->u32SerialNumberStart)
        || (stDeviceInfo.u32SerialNum > pstUpgradeInfo->u32SerialNumberEnd))
    {
        COM_PrintErrInfo("local HW serial number is not match with the upgrade stream.");
        return COM_ERR_IMAGE_UNAVAILABLE;
    }

    /* for normal force upgrade, check software version  */
    if (!loaderCheckEnforceType(pstLoaderDBInfo))
    {
        LDDB_SOFTWARE_INFO_S stSoftwareInfo;

        COM_MemSet(&stSoftwareInfo, 0x00, sizeof(stSoftwareInfo));
        s32Ret = LDDB_GetSoftwareInfo(&stSoftwareInfo);

        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(LDDB_GetSoftwareInfo, s32Ret);
            return s32Ret;
        }

        if (pstUpgradeInfo->u32SoftwareVersion <= stSoftwareInfo.u32SoftwareVersion)
        {
            COM_ERROR("upgrade stream software version old than local stb info.");
            COM_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));
            return COM_ERR_IMAGE_UNAVAILABLE;
        }

        COM_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 loaderShowUpgradeResult(HI_S32 s32Ret)
{
    if (HI_SUCCESS != s32Ret)
    {
        switch (s32Ret)
        {
            case COM_ERR_SIGNAL_LOSE:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_SIG_EXPT, 3000);
                COM_CHECK(UI_KEYLED_Display("E01"));
                break;
            }

            case COM_ERR_FLASH_WRITE:
            {
                COM_CHECK(UI_KEYLED_Display("E05"));
                break;
            }

            case COM_ERR_DOWNLOAD_TIMEOUT:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_DOWNLOAD_TIMEOUT, 3000);
                COM_CHECK(UI_KEYLED_Display("E06"));
                break;
            }

            case COM_ERR_IMAGE_UNAVAILABLE:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_NOT_FIND_USB_FILE, 3000);
                COM_CHECK(UI_KEYLED_Display("E07"));
                break;
            }

            case COM_ERR_CONNECT_FAIL:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_NO_RESP, 3000);
                COM_CHECK(UI_KEYLED_Display("E12"));
                break;
            }

            case COM_ERR_IMAGE_NOEXIST:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_IP_NOT_FIND_FILE, 3000);
                COM_CHECK(UI_KEYLED_Display("E13"));
                break;
            }

            case COM_ERR_NETWORK:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_NET_EXPT, 3000);
                COM_CHECK(UI_KEYLED_Display("E14"));
                break;
            }

            case COM_ERR_IPCONFIG:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_SET_IP_ERR, 3000);
                COM_CHECK(UI_KEYLED_Display("E15"));
                break;
            }

            case COM_ERR_TIMEOUT:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_NET_TIMEOUT, 3000);
                COM_CHECK(UI_KEYLED_Display("E16"));
                break;
            }

            default:
            {
                OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FAIL_DOWNLOAD, 3000);
                COM_CHECK(UI_KEYLED_Display("E02"));
                break;
            }
        }
    }
    else
    {
        OSD_ShowMsgBox(UI_LAN_NOTICE, UI_LAN_PT_SUCC, 3000);
        COM_CHECK(UI_KEYLED_Display("Succ"));
    }

    return HI_SUCCESS;
}

static HI_S32 loaderDoPartitionReset(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    OSD_ShowMsgBox(UI_LAN_NOTICE, UI_LAN_FACTORY_NOTICE, 0);

    s32Ret = DOWNLOAD_FS_PartitionReset();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_FS_PartitionReset, s32Ret);

        COM_Delayms(5000);
        OSD_ShowMsgBox(UI_LAN_WARNING, UI_LAN_FACTORY_FAILED, 5000);
        return s32Ret;
    }

    COM_CHECK(loaderClearUpgradeParam());

    COM_Delayms(5000);
    COM_CHECK(UI_KEYLED_Display("Succ"));
    COM_CHECK(OSD_ShowMsgBox(UI_LAN_NOTICE, UI_LAN_FACTORY_SUCCESS, 3000));

    return HI_SUCCESS;
}
static HI_S32 loaderDoUpgradeProcedure(HI_VOID)
{
    HI_U32 u32PartitionNum = 0;
    PROT_VERSION_INFO_S stUpgradeVer;
    HI_LOADER_PARAM_S stParameter;
    PROT_PARTITION_INFO_S astPartitionInfo[COM_CFG_D_CNT_MODULE];
    LDDB_SOFTWARE_INFO_S stSoftwareInfo;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK(UI_KEYLED_Display("LoAd"));

    s32Ret = UI_ShowProgressWin();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_ShowProgressWin, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stParameter, 0x00, sizeof(&stParameter));
    LDDB_UPDPARAM_CONVERT(&stParameter, &g_stLoaderInfo.stUPDParamData);

    s32Ret = LOADER_PROTOCOL_Init(&stParameter, loaderBurnData, UI_UpdateDowloadProgress);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_Init, s32Ret);
        COM_CHECK(UI_HideProgressWin());
        return s32Ret;
    }

    COM_MemSet(&stUpgradeVer, 0x00, sizeof(&stUpgradeVer));
    s32Ret = LOADER_PROTOCOL_GetVersionInfo(&stUpgradeVer);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_GetVersionInfo, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        COM_CHECK(UI_HideProgressWin());
        return s32Ret;
    }

    if (HI_TRUE != g_stLoaderInfo.stTAGParamData.bUsbBurn)
    {
        s32Ret = loaderCheckVersionMatch(&g_stLoaderInfo, &stUpgradeVer);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(loaderCheckVersionMatch, s32Ret);
            COM_CHECK(LOADER_PROTOCOL_DeInit());
            COM_CHECK(UI_HideProgressWin());
            return s32Ret;
        }
    }

    COM_MemSet(astPartitionInfo, 0x00, sizeof(astPartitionInfo));
    s32Ret = LOADER_PROTOCOL_GetPartitionInfo(astPartitionInfo, COM_CFG_D_CNT_MODULE, &u32PartitionNum);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_GetPartitionInfo, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        COM_CHECK(UI_HideProgressWin());
        return s32Ret;
    }

    s32Ret = LOADER_BURN_Init(astPartitionInfo, u32PartitionNum, UI_UpdateBurnProgress);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_BURN_Init, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        COM_CHECK(UI_HideProgressWin());
        return s32Ret;
    }

    COM_CHECK(UI_KEYLED_Display("burn"));

    s32Ret = LOADER_PROTOCOL_Process();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_Process, s32Ret);
        COM_CHECK(LOADER_BURN_DeInit());
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        COM_CHECK(UI_HideProgressWin());

        return s32Ret;
    }

    COM_MemSet(&stSoftwareInfo, 0x00, sizeof(stSoftwareInfo));
    COM_CHECK(LDDB_GetSoftwareInfo(&stSoftwareInfo));
    stSoftwareInfo.u32SoftwareVersion = stUpgradeVer.u32SoftwareVersion;
    COM_CHECK(LDDB_SetSoftwareInfo(&stSoftwareInfo));
    COM_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));

    COM_CHECK(loaderClearUpgradeParam());

    COM_CHECK(LOADER_BURN_DeInit());
    COM_CHECK(LOADER_PROTOCOL_DeInit());

    COM_Delayms(5000);
    COM_CHECK(UI_HideProgressWin());

    COM_CHECK(UI_KEYLED_Display("Succ"));
    COM_CHECK(OSD_ShowMsgBox(UI_LAN_NOTICE, UI_LAN_PT_SUCC, 3000));

    return HI_SUCCESS;
}

HI_S32 LOADER_App(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = loaderUIInit();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(loaderUIInit, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&g_stLoaderInfo, 0x00, sizeof(&g_stLoaderInfo));
    s32Ret = LDDB_GetLoaderInfo(&g_stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LDDB_GetLoaderInfo, s32Ret);
        COM_CHECK(loaderShowUpgradeResult(s32Ret));
        COM_CHECK(loaderUIDeInit());
        return s32Ret;
    }

    s32Ret = loaderCheckPolicy(&g_stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(loaderCheckPolicy, s32Ret);
        COM_CHECK(loaderShowUpgradeResult(s32Ret));
        COM_CHECK(loaderUIDeInit());
        return s32Ret;
    }

    if (HI_LOADER_UPGRADE_TYPE_RESET == g_stLoaderInfo.stUPDParamData.enUpgradeType)
    {
        s32Ret = loaderDoPartitionReset();
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(loaderDoPartitionReset, s32Ret);
            COM_CHECK(loaderUIDeInit());
            return s32Ret;
        }
    }
    else
    {
        s32Ret = loaderDoUpgradeProcedure();
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(loaderDoUpgradeProcedure, s32Ret);
            COM_CHECK(loaderShowUpgradeResult(s32Ret));
            COM_CHECK(loaderUIDeInit());
            return s32Ret;
        }
    }

    COM_CHECK(loaderUIDeInit());

    COM_DBG_FuncExit();
    return s32Ret;
}

#ifdef HI_LOADER_APPLOADER
int main(HI_VOID)
{
    COM_CHECK(COM_LogSystemInit());
    COM_CHECK(LOADER_App());
    COM_CHECK(COM_LogSystemDeInit());

    return HI_SUCCESS;
}
#else

static HI_VOID loaderUpgradeDone()
{
#if defined(HI_LOADER_BOOTLOADER)
    setenv("bootcmd", "reset");
#else
    sync();
    reboot(RB_AUTOBOOT);
#endif
}

/*
 * entry for factory batch USB upgrade scense
 * without check upgrade tag and version info.
 */
HI_S32 loaderDoUsbManddatoryUpgrade(HI_VOID)
{
    HI_U32 u32PartitionNum = 0;
    HI_LOADER_PARAM_S stParameter;
    PROT_PARTITION_INFO_S astPartitionInfo[COM_CFG_D_CNT_MODULE];
    PROT_VERSION_INFO_S stUpgradeVer;

    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK(COM_MemSetAvailableSize(COM_CFG_D_HIPRO_AVAILABLE_MEM));

    COM_MemSet(&g_stLoaderInfo, 0x00, sizeof(g_stLoaderInfo));
    s32Ret = LDDB_GetLoaderInfo(&g_stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LDDB_GetLoaderInfo, s32Ret);
        return s32Ret;
    }

    g_stLoaderInfo.stPrivateParam.u32FailedCnt = 1;
    g_stLoaderInfo.stUPDParamData.enUpgradeType = HI_LOADER_UPGRADE_TYPE_USB;
    COM_StrNCpy(g_stLoaderInfo.stUPDParamData.unParam.stUSBParam.as8FileName,
                COM_CFG_D_UPGRADE_FILENAME,
                HI_LOADER_FILENAME_LEN - 1);

    s32Ret = LDDB_SetLoaderInfo(&g_stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stParameter, 0x00, sizeof(stParameter));
    LDDB_UPDPARAM_CONVERT(&stParameter, &g_stLoaderInfo.stUPDParamData);
    s32Ret = LOADER_PROTOCOL_Init(&stParameter, loaderBurnData, HI_NULL_PTR);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stUpgradeVer, 0x00, sizeof(stUpgradeVer));
    s32Ret = LOADER_PROTOCOL_GetVersionInfo(&stUpgradeVer);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_GetVersionInfo, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        return s32Ret;
    }

    s32Ret = LOADER_PROTOCOL_GetPartitionInfo(astPartitionInfo, COM_CFG_D_CNT_MODULE, &u32PartitionNum);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_GetPartitionInfo, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        return s32Ret;
    }

    s32Ret = LOADER_BURN_Init(astPartitionInfo, u32PartitionNum, HI_NULL_PTR);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_BURN_Init, s32Ret);
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        return s32Ret;
    }

    s32Ret = LOADER_PROTOCOL_Process();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(LOADER_PROTOCOL_Process, s32Ret);
        COM_CHECK(LOADER_BURN_DeInit());
        COM_CHECK(LOADER_PROTOCOL_DeInit());
        return s32Ret;
    }

    COM_CHECK(LOADER_BURN_DeInit());
    COM_CHECK(LOADER_PROTOCOL_DeInit());

    loaderUpgradeDone();

    return s32Ret;
}

static int do_usbburn(cmd_tbl_t* cmdtp, int flag, int argc, char* argv[])
{
    COM_CHECK(loaderDoUsbManddatoryUpgrade());
    loaderUpgradeDone();

    return HI_SUCCESS;
}

U_BOOT_CMD
(
    usbburn,    3,  1,  do_usbburn,
    "burn file from USB device\n",
    "burn file from USB device\n"
);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

