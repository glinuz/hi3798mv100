/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hi_loader_info.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by:
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/
#include "loader_osal.h"
#include "loader_debug.h"
#include "loader_dbinfo.h"
#include "hi_loader_info.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

HI_S32 HI_LOADER_ReadParameters(HI_LOADER_PARAMETER_S* pstParameter)
{
    LDDB_LOADER_INFO_S stLoaderInfo;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstParameter);

    LDDB_MemSet(&stLoaderInfo, 0x00, sizeof(stLoaderInfo));
    s32Ret = LDDB_GetLoaderInfo(&stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_GetLoaderInfo, s32Ret);
        return s32Ret;
    }

    pstParameter->eUpdateType = stLoaderInfo.stUPDParamData.enUpgradeType;

    switch (stLoaderInfo.stUPDParamData.enUpgradeType)
    {
        case HI_LOADER_UPGRADE_TYPE_OTA:
        {
            LDDB_PARAM_OTA_S* pstOtaParam1 = &(stLoaderInfo.stUPDParamData.unParam.stOTAParam);
            HI_LOADER_OTA_PARA_S* pstOtaParam2 = &(pstParameter->stPara.stOTAPara);

            pstOtaParam2->eSigType = pstOtaParam1->enSigType;

            if (HI_UNF_TUNER_SIG_TYPE_CAB == pstOtaParam1->enSigType)
            {
                pstOtaParam2->unConnPara.stCab.u32OtaPid = pstOtaParam1->u32Pid;
                pstOtaParam2->unConnPara.stCab.u32OtaFreq = pstOtaParam1->unParam.stCab.stConnectParam.u32Freq;
                pstOtaParam2->unConnPara.stCab.u32OtaSymbRate = pstOtaParam1->unParam.stCab.stConnectParam.u32SymbolRate / 1000;
                pstOtaParam2->unConnPara.stCab.u32OtaModulation = pstOtaParam1->unParam.stCab.stConnectParam.enModType;
            }
            else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstOtaParam1->enSigType)
            {
                pstOtaParam2->unConnPara.stSat.u32OtaPid = pstOtaParam1->u32Pid;
                pstOtaParam2->unConnPara.stSat.u32OtaFreq = pstOtaParam1->unParam.stSat.stConnectParam.u32Freq;
                pstOtaParam2->unConnPara.stSat.u32OtaSymbRate = pstOtaParam1->unParam.stSat.stConnectParam.u32SymbolRate / 1000;
                pstOtaParam2->unConnPara.stSat.u32Polar = pstOtaParam1->unParam.stSat.stConnectParam.enPolar;

                pstOtaParam2->unConnPara.stSat.u32LNBPower = pstOtaParam1->unParam.stSat.enLNBPower;
                pstOtaParam2->unConnPara.stSat.u32Switch22K = pstOtaParam1->unParam.stSat.enSwitch22K;
                pstOtaParam2->unConnPara.stSat.st4Port.u32Level = pstOtaParam1->unParam.stSat.stSwitch4Port.enLevel;
                pstOtaParam2->unConnPara.stSat.st4Port.u32LNB22K = pstOtaParam1->unParam.stSat.stSwitch4Port.enLNB22K;
                pstOtaParam2->unConnPara.stSat.st4Port.u32Polar = pstOtaParam1->unParam.stSat.stSwitch4Port.enPolar;
                pstOtaParam2->unConnPara.stSat.st4Port.u32Port = pstOtaParam1->unParam.stSat.stSwitch4Port.enPort;
                pstOtaParam2->unConnPara.stSat.st16Port.u32Level = pstOtaParam1->unParam.stSat.stSwitch16Port.enLevel;
                pstOtaParam2->unConnPara.stSat.st16Port.u32Port = pstOtaParam1->unParam.stSat.stSwitch16Port.enPort;
                pstOtaParam2->unConnPara.stSat.u32LowLO = pstOtaParam1->unParam.stSat.stLNBConfig.u32LowLO;
                pstOtaParam2->unConnPara.stSat.u32HighLO = pstOtaParam1->unParam.stSat.stLNBConfig.u32HighLO;

                if (HI_UNF_TUNER_FE_LNB_UNICABLE == pstOtaParam1->unParam.stSat.stLNBConfig.enLNBType)
                {
                    pstOtaParam2->unConnPara.stSat.u32UnicFlag = HI_TRUE;
                }
            }
            else if (((HI_UNF_TUNER_SIG_TYPE_DVB_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DVB_T2 & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ISDB_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ATSC_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DTMB & pstOtaParam1->enSigType) != 0))
            {
                pstOtaParam2->unConnPara.stTer.u32OtaPid = pstOtaParam1->u32Pid;
                pstOtaParam2->unConnPara.stTer.u32OtaFreq = pstOtaParam1->unParam.stTer.stConnectParam.u32Freq;
                pstOtaParam2->unConnPara.stTer.u32OtaBandWidth = pstOtaParam1->unParam.stTer.stConnectParam.u32BandWidth;
                pstOtaParam2->unConnPara.stTer.u32OtaModulation = pstOtaParam1->unParam.stTer.stConnectParam.enModType;
                pstOtaParam2->unConnPara.stTer.enChannelMode = pstOtaParam1->unParam.stTer.stConnectParam.enChannelMode;
                pstOtaParam2->unConnPara.stTer.enDVBTPrio = pstOtaParam1->unParam.stTer.stConnectParam.enDVBTPrio;
            }
            else
            {
                LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
                return LDDB_ERR_UNSUPPORTED;
            }

            break;
        }

        case HI_LOADER_UPGRADE_TYPE_IP:
        {
            HI_LOADER_PARAM_IP_S* pstIPParam1 = &(stLoaderInfo.stUPDParamData.unParam.stIPParam);
            HI_LOADER_IP_PARA_S* pstIPParam2 = &(pstParameter->stPara.stIPParam);

            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8FileName, (HI_CHAR*)pstIPParam2->as8FileName, HI_LOADER_FILENAME_LEN - 1);
            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8UserName, (HI_CHAR*)pstIPParam2->as8UserName, HI_LOADER_USERNAME_LEN - 1);
            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8Password, (HI_CHAR*)pstIPParam2->as8Password, HI_LOADER_PASSWORD_LEN - 1);

            pstIPParam2->eProtoType     = pstIPParam1->enProtType;
            pstIPParam2->eIpType        = pstIPParam1->enIPCfgType;
            pstIPParam2->ipOurIp        = pstIPParam1->ipOurIp;
            pstIPParam2->ipGateway      = pstIPParam1->ipGateway;
            pstIPParam2->ipSubnetMask   = pstIPParam1->ipSubnetMask;
            pstIPParam2->ipServer       = pstIPParam1->ipServer;
            pstIPParam2->ipServerPort   = pstIPParam1->ipServerPort;
            pstIPParam2->u32FileSize    = pstIPParam1->u32FileSize;
            break;
        }

        case HI_LOADER_UPGRADE_TYPE_USB:
        {
            LDDB_StrNCpy((HI_CHAR*)pstParameter->stPara.stUSBParam.as8FileName,
                         (HI_CHAR*)stLoaderInfo.stUPDParamData.unParam.stUSBParam.as8FileName,
                         LOADER_FILE_NAME_LEN - 1);
            break;
        }

        default:
        {
            LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
            return LDDB_ERR_UNSUPPORTED;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HI_LOADER_WriteParameters(HI_LOADER_PARAMETER_S* pstParameter)
{
    LDDB_LOADER_INFO_S stLoaderInfo;
    HI_S32 s32Ret = HI_FAILURE;

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstParameter);
    LDDB_CHECK_PARAM(HI_LOADER_TYPE_BUTT <= pstParameter->eUpdateType);

    LDDB_MemSet(&stLoaderInfo, 0x00, sizeof(stLoaderInfo));
    stLoaderInfo.stTAGParamData.bNeedUpgrade  = HI_TRUE;
    stLoaderInfo.stTAGParamData.bForceUpgrade = HI_FALSE;
    stLoaderInfo.stTAGParamData.bUsbBurn      = HI_FALSE;

    stLoaderInfo.stPrivateParam.u32FailedCnt  = 0;
    stLoaderInfo.stPrivateParam.bTagDestroy   = HI_FALSE;

    stLoaderInfo.stUPDParamData.enUpgradeType = pstParameter->eUpdateType;
    stLoaderInfo.stUPDParamData.enUpgradeMode = HI_LOADER_UPGRADE_MODE_BASIC;

    switch (stLoaderInfo.stUPDParamData.enUpgradeType)
    {
        case HI_LOADER_UPGRADE_TYPE_OTA:
        {
            LDDB_PARAM_OTA_S* pstOtaParam1 = &stLoaderInfo.stUPDParamData.unParam.stOTAParam;
            HI_LOADER_OTA_PARA_S* pstOtaParam2 = &pstParameter->stPara.stOTAPara;

            pstOtaParam1->enSigType = pstOtaParam2->eSigType;

            if (HI_UNF_TUNER_SIG_TYPE_CAB == pstOtaParam1->enSigType)
            {
                pstOtaParam1->u32Pid = pstOtaParam2->unConnPara.stCab.u32OtaPid;
                pstOtaParam1->unParam.stCab.stConnectParam.u32Freq = pstOtaParam2->unConnPara.stCab.u32OtaFreq;
                pstOtaParam1->unParam.stCab.stConnectParam.u32SymbolRate = pstOtaParam2->unConnPara.stCab.u32OtaSymbRate * 1000;
                pstOtaParam1->unParam.stCab.stConnectParam.enModType = pstOtaParam2->unConnPara.stCab.u32OtaModulation;
                pstOtaParam1->unParam.stCab.stConnectParam.bReverse = HI_FALSE;
            }
            else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstOtaParam1->enSigType)
            {
                pstOtaParam1->u32Pid = pstOtaParam2->unConnPara.stSat.u32OtaPid;
                pstOtaParam1->unParam.stSat.stConnectParam.u32Freq = pstOtaParam2->unConnPara.stSat.u32OtaFreq;
                pstOtaParam1->unParam.stSat.stConnectParam.u32SymbolRate = pstOtaParam2->unConnPara.stSat.u32OtaSymbRate * 1000;
                pstOtaParam1->unParam.stSat.stConnectParam.enPolar = pstOtaParam2->unConnPara.stSat.u32Polar;
                pstOtaParam1->unParam.stSat.stConnectParam.u32ScrambleValue = 0;

                pstOtaParam1->unParam.stSat.enLNBPower  = pstOtaParam2->unConnPara.stSat.u32LNBPower;
                pstOtaParam1->unParam.stSat.enSwitch22K = pstOtaParam2->unConnPara.stSat.u32Switch22K;
                pstOtaParam1->unParam.stSat.stSwitch4Port.enLevel = pstOtaParam2->unConnPara.stSat.st4Port.u32Level;
                pstOtaParam1->unParam.stSat.stSwitch4Port.enLNB22K = pstOtaParam2->unConnPara.stSat.st4Port.u32LNB22K;
                pstOtaParam1->unParam.stSat.stSwitch4Port.enPolar = pstOtaParam2->unConnPara.stSat.st4Port.u32Polar;
                pstOtaParam1->unParam.stSat.stSwitch4Port.enPort  = pstOtaParam2->unConnPara.stSat.st4Port.u32Port;
                pstOtaParam1->unParam.stSat.stSwitch16Port.enLevel = pstOtaParam2->unConnPara.stSat.st16Port.u32Level;
                pstOtaParam1->unParam.stSat.stSwitch16Port.enPort = pstOtaParam2->unConnPara.stSat.st16Port.u32Port;
                pstOtaParam1->unParam.stSat.enSwitch22K = pstOtaParam2->unConnPara.stSat.u32Switch22K;

                if (HI_TRUE == pstOtaParam2->unConnPara.stSat.u32UnicFlag)
                {
                    pstOtaParam1->unParam.stSat.stLNBConfig.enLNBType   = HI_UNF_TUNER_FE_LNB_UNICABLE;
                    pstOtaParam1->unParam.stSat.stLNBConfig.u32LowLO    = 9750;
                    pstOtaParam1->unParam.stSat.stLNBConfig.u32HighLO   = 10600;
                    pstOtaParam1->unParam.stSat.stLNBConfig.enLNBBand   = HI_UNF_TUNER_FE_LNB_BAND_KU;
                    pstOtaParam1->unParam.stSat.stLNBConfig.u8UNIC_SCRNO = 0;
                    pstOtaParam1->unParam.stSat.stLNBConfig.u32UNICIFFreqMHz = 1210;
                    pstOtaParam1->unParam.stSat.stLNBConfig.enSatPosn = HI_UNF_TUNER_SATPOSN_A;
                }
                else
                {
                    if ((pstOtaParam2->unConnPara.stSat.u32LowLO == pstOtaParam2->unConnPara.stSat.u32HighLO)
                        || (0 == pstOtaParam2->unConnPara.stSat.u32LowLO)
                        || (0 == pstOtaParam2->unConnPara.stSat.u32HighLO))
                    {
                        pstOtaParam1->unParam.stSat.stLNBConfig.enLNBType = HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY;
                    }
                    else
                    {
                        pstOtaParam1->unParam.stSat.stLNBConfig.enLNBType = HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY;
                    }

                    pstOtaParam1->unParam.stSat.stLNBConfig.u32LowLO  = pstOtaParam2->unConnPara.stSat.u32LowLO;
                    pstOtaParam1->unParam.stSat.stLNBConfig.u32HighLO = pstOtaParam2->unConnPara.stSat.u32HighLO;

                    if ((6000 > pstOtaParam2->unConnPara.stSat.u32HighLO)
                        && (6000 > pstOtaParam2->unConnPara.stSat.u32LowLO))
                    {
                        pstOtaParam1->unParam.stSat.stLNBConfig.enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_C;
                    }
                    else
                    {
                        pstOtaParam1->unParam.stSat.stLNBConfig.enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_KU;
                    }
                }
            }
            else if (((HI_UNF_TUNER_SIG_TYPE_DVB_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DVB_T2 & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ISDB_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ATSC_T & pstOtaParam1->enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DTMB & pstOtaParam1->enSigType) != 0))
            {
                pstOtaParam1->u32Pid = pstOtaParam2->unConnPara.stTer.u32OtaPid;
                pstOtaParam1->unParam.stTer.stConnectParam.u32Freq = pstOtaParam2->unConnPara.stTer.u32OtaFreq;
                pstOtaParam1->unParam.stTer.stConnectParam.u32BandWidth = pstOtaParam2->unConnPara.stTer.u32OtaBandWidth;
                pstOtaParam1->unParam.stTer.stConnectParam.enModType = pstOtaParam2->unConnPara.stTer.u32OtaModulation;
                pstOtaParam1->unParam.stTer.stConnectParam.enChannelMode = pstOtaParam2->unConnPara.stTer.enChannelMode;
                pstOtaParam1->unParam.stTer.stConnectParam.enDVBTPrio = pstOtaParam2->unConnPara.stTer.enDVBTPrio;
                pstOtaParam1->unParam.stTer.stConnectParam.bReverse = HI_FALSE;
            }
            else
            {
                LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
                return LDDB_ERR_UNSUPPORTED;
            }

            break;
        }

        case HI_LOADER_UPGRADE_TYPE_IP:
        {
            HI_LOADER_PARAM_IP_S* pstIPParam1 = &(stLoaderInfo.stUPDParamData.unParam.stIPParam);
            HI_LOADER_IP_PARA_S* pstIPParam2 = &(pstParameter->stPara.stIPParam);

            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8FileName, (HI_CHAR*)pstIPParam2->as8FileName, HI_LOADER_FILENAME_LEN - 1);
            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8UserName, (HI_CHAR*)pstIPParam2->as8UserName, HI_LOADER_USERNAME_LEN - 1);
            LDDB_StrNCpy((HI_CHAR*)pstIPParam1->as8Password, (HI_CHAR*)pstIPParam2->as8Password, HI_LOADER_PASSWORD_LEN - 1);

            pstIPParam1->enProtType  = pstIPParam2->eProtoType;
            pstIPParam1->enIPCfgType = pstIPParam2->eIpType;
            pstIPParam1->ipOurIp     = pstIPParam2->ipOurIp;
            pstIPParam1->ipGateway   = pstIPParam2->ipGateway;
            pstIPParam1->ipSubnetMask = pstIPParam2->ipSubnetMask;
            pstIPParam1->ipServer    = pstIPParam2->ipServer;
            pstIPParam1->ipServerPort = pstIPParam2->ipServerPort;
            pstIPParam1->u32FileSize = pstIPParam2->u32FileSize;
            break;
        }

        case HI_LOADER_UPGRADE_TYPE_USB:
        {
            LDDB_StrNCpy((HI_CHAR*)stLoaderInfo.stUPDParamData.unParam.stUSBParam.as8FileName,
                         (HI_CHAR*)pstParameter->stPara.stUSBParam.as8FileName,
                         HI_LOADER_FILENAME_LEN - 1);
        }

        default:
        {
            LDDB_PrintErrCode(LDDB_ERR_UNKNOWNED);
            return LDDB_ERR_UNKNOWNED;
        }
    }

    s32Ret = LDDB_SetLoaderInfo(&stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
        return s32Ret;
    }

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_LOADER_ReadSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo)
{
    HI_S32 s32Ret = HI_FAILURE;
    LDDB_DEVICE_INFO_S stDeviceInfo;

    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstStbinfo);

    LDDB_MemSet(&stDeviceInfo, 0x00, sizeof(stDeviceInfo));
    s32Ret = LDDB_GetDeviceInfo(&stDeviceInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LDDB_PrintFuncErr(LDDB_GetDeviceInfo, s32Ret);
        return s32Ret;
    }

    pstStbinfo->u32OUI         = stDeviceInfo.u32OUI;
    pstStbinfo->u32AreaCode    = stDeviceInfo.u32AreaCode;
    pstStbinfo->u32HWVersion   = stDeviceInfo.u32HardwareVersion;
    pstStbinfo->u32HWSerialNum = stDeviceInfo.u32SerialNum;

    LDDB_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HI_LOADER_WriteSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo)
{
    LDDB_DBG_FuncEnter();

    LDDB_CHECK_PARAM(HI_NULL_PTR == pstStbinfo);

    LDDB_PrintErrCode(LDDB_ERR_UNSUPPORTED);
    return LDDB_ERR_UNSUPPORTED;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

