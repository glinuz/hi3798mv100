/***********************************************************
 *                      Copyright    , 2009-2014, Hisilicon Tech. Co., Ltd.
 *                                   ALL RIGHTS RESERVED
 * FileName:  upgrd_usb.c
 * Description: usb upgrade mode
 *
 * History:
 * Version   Date           Author            DefectNum      Description
 * main\1    2009-2-12    chenling102556      NULL          Create this file.
 ************************************************************/
#include "com_osal.h"
#include "com_debug.h"
#include "com_config.h"
#include "hi_unf_frontend.h"
#include "hi_error_mpi.h"
#include "download_ota.h"
#include "frontend_config.h"
#include "hi_unf_i2c.h"

#ifdef HI_LOADER_BOOTLOADER
#include <uboot.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OTA_D_DMXID             (0)

#define OTA_CONNECT_CHECKTIMES          (10)
#define OTA_CONNECT_CHECKINTERVAL       (50)    //ms
#define OTA_CONNECT_TIMEOUT             (2000)  //ms

#define OTA_ACQUIRE_TRYTIMES            (1000)  //ms
#define OTA_ACQUIRE_TIMEOUT             (10)    //ms


static HI_HANDLE    s_hChannel = HI_INVALID_HANDLE;
static HI_HANDLE    s_hFilter  = HI_INVALID_HANDLE;
static HI_U32       s_u32ChannelPid = 0;

#ifdef LOADER_USE_GPIO
static HI_U32       s_u32I2cChannel = 0;
#endif


#ifdef LOADER_USE_GPIO
static HI_S32 dlotaTunerCreateI2cChannel(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_UNF_I2C_CreateGpioI2c(&s_u32I2cChannel, GET_TUNER_GPIOSCL(), GET_TUNER_GPIOSDA());
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_I2C_CreateGpioI2c, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerDestoryI2cChannel(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_UNF_I2C_DestroyGpioI2c(s_u32I2cChannel);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_I2C_DestroyGpioI2c, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerGetI2cChannel(HI_VOID)
{
    return s_u32I2cChannel;
}

#endif

#ifdef HI_LOADER_TUNER_SAT
HI_S32 dlotaTunerSetSatAttr(HI_U32 u32TunerID, HI_LOADER_PARAM_SAT_S* pstParam)
{
    HI_UNF_TUNER_SAT_ATTR_S stSatAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stSatAttr, 0x00, sizeof(stSatAttr));
    GET_SAT_TUNER_CONFIG(stSatAttr);

    s32Ret = HI_UNF_TUNER_SetSatAttr(u32TunerID, &stSatAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_SetSatAttr, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_TUNER_SetLNBPower(u32TunerID, pstParam->enLNBPower);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_SetLNBPower, s32Ret);
        return COM_ERR_DEVICE;
    }

    /* If LNB power off, 22K signal can't be sent, switch and motor configuration will be not effective */
    if ((HI_UNF_TUNER_FE_LNB_POWER_ON == pstParam->enLNBPower)
        || (HI_UNF_TUNER_FE_LNB_POWER_ENHANCED == pstParam->enLNBPower))
    {
#ifdef DISEQC_SUPPORT
        /* Wait DiSEqC switch to start */
        COM_Delayms(50);

        /* Use DiSEqC 1.1 Switch */
        s32Ret = HI_UNF_TUNER_DISEQC_Switch16Port(u32TunerID, &pstParam->stSwitch16Port);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_DISEQC_Switch16Port, s32Ret);
            return COM_ERR_DEVICE;
        }

        /* Use DiSEqC 1.0 Switch */
        s32Ret = HI_UNF_TUNER_DISEQC_Switch4Port(u32TunerID, &pstParam->stSwitch4Port);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_DISEQC_Switch4Port, s32Ret);
            return COM_ERR_DEVICE;
        }

#endif/* DISEQC_SUPPORT */

        /* Use 22K switch */
        s32Ret = HI_UNF_TUNER_Switch22K(u32TunerID, pstParam->enSwitch22K);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_Switch22K, s32Ret);
            return COM_ERR_DEVICE;
        }
    }

    s32Ret = HI_UNF_TUNER_SetLNBConfig(u32TunerID, &pstParam->stLNBConfig);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_SetLNBConfig, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}
#endif

#if defined(HI_LOADER_TUNER_DVB_T) || defined(HI_LOADER_TUNER_DVB_T2)
static HI_S32 dlotaTunerSetTerAttr(HI_U32 u32TunerID, HI_LOADER_PARAM_OTA_S* pstParam)
{
    HI_UNF_TUNER_TER_ATTR_S stTerAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stTerAttr, 0x00, sizeof(stTerAttr));
    GET_TER_TUNER_CONFIG(stTerAttr);

    s32Ret = HI_UNF_TUNER_SetTerAttr(u32TunerID, &stTerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_SetTerAttr, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}
#endif

static HI_S32 dlotaTunerSetAttr(HI_U32 u32TunerID)
{
    HI_UNF_TUNER_ATTR_S stTunerAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stTunerAttr, 0x00, sizeof(stTunerAttr));
    GET_TUNER_CONFIG(stTunerAttr);

#if defined(LOADER_USE_GPIO)
    stTunerAttr.enI2cChannel = dlotaTunerGetI2cChannel();
#endif

    s32Ret = HI_UNF_TUNER_SetAttr(u32TunerID, &stTunerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_SetAttr, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerConfig(HI_LOADER_PARAM_OTA_S* pstParam)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = dlotaTunerSetAttr(OTA_TUNER_ID);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaTunerSetAttr, s32Ret);
        return s32Ret;
    }

#if (defined(HI_TUNER_OUTPUT_PIN0) && (0 == OTA_TUNER_ID))     \
    || (defined(HI_TUNER1_OUTPUT_PIN0) && (1 == OTA_TUNER_ID)) \
    || (defined(HI_TUNER2_OUTPUT_PIN0) && (2 == OTA_TUNER_ID)) \
    || (defined(HI_TUNER3_OUTPUT_PIN0) && (3 == OTA_TUNER_ID))
    {
        HI_UNF_TUNER_TSOUT_SET_S stTSOut;

        COM_MemSet(&stTSOut, 0x00, sizeof(stTSOut));
        GET_TUNER_TSOUT_CONFIG(stTSOut);
        s32Ret = HI_UNF_TUNER_SetTSOUT(OTA_TUNER_ID, &stTSOut);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_SetTSOUT, s32Ret);
            return s32Ret;
        }
    }
#endif

    if ((0 != (HI_UNF_TUNER_SIG_TYPE_DVB_T & pstParam->enSigType))
        || (0 != (HI_UNF_TUNER_SIG_TYPE_DVB_T2 & pstParam->enSigType))
        || (0 != (HI_UNF_TUNER_SIG_TYPE_ISDB_T & pstParam->enSigType))
        || (0 != (HI_UNF_TUNER_SIG_TYPE_ATSC_T & pstParam->enSigType))
        || (0 != (HI_UNF_TUNER_SIG_TYPE_DTMB & pstParam->enSigType)))
    {
#if defined(HI_LOADER_TUNER_DVB_T) || defined(HI_LOADER_TUNER_DVB_T2)
        s32Ret = dlotaTunerSetTerAttr(OTA_TUNER_ID, pstParam);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(dlotaTunerSetTerAttr, s32Ret);
            return s32Ret;
        }

#else

        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
#endif
    }
    else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstParam->enSigType)
    {
#ifdef HI_LOADER_TUNER_SAT
        s32Ret = dlotaTunerSetSatAttr(OTA_TUNER_ID, &pstParam->unParam.stSat);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(dlotaTunerSetSatAttr, s32Ret);
            return s32Ret;
        }

#else
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
#endif
    }
    else if (HI_UNF_TUNER_SIG_TYPE_CAB != pstParam->enSigType)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerOpen(HI_LOADER_PARAM_OTA_S* pstParam)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_UNF_I2C_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_I2C_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_TUNER_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_Init, s32Ret);
        COM_CHECK(HI_UNF_I2C_DeInit());
        return s32Ret;
    }

    s32Ret = HI_UNF_TUNER_Open(OTA_TUNER_ID);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_Open, s32Ret);
        COM_CHECK(HI_UNF_TUNER_DeInit());
        COM_CHECK(HI_UNF_I2C_DeInit());
        return s32Ret;
    }

#ifdef LOADER_USE_GPIO
    s32Ret = dlotaTunerCreateI2cChannel();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaTunerCreateI2cChannel, s32Ret);
        COM_CHECK(HI_UNF_TUNER_DeInit());
        COM_CHECK(HI_UNF_I2C_DeInit());
        return s32Ret;
    }

#endif

    s32Ret = dlotaTunerConfig(pstParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaTunerConfig, s32Ret);

#ifdef LOADER_USE_GPIO
        COM_CHECK(dlotaTunerDestoryI2cChannel());
#endif
        COM_CHECK(HI_UNF_TUNER_DeInit());
        COM_CHECK(HI_UNF_I2C_DeInit());
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerClose(HI_VOID)
{
    COM_DBG_FuncEnter();

#ifdef LOADER_USE_GPIO
    COM_CHECK(dlotaTunerDestoryI2cChannel());
#endif

    COM_CHECK(HI_UNF_TUNER_Close(OTA_TUNER_ID));
    COM_CHECK(HI_UNF_TUNER_DeInit());
    COM_CHECK(HI_UNF_I2C_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaTunerLock
(
    HI_LOADER_PARAM_OTA_S*      pstOtaParam,
    HI_UNF_TUNER_LOCK_STATUS_E* penStatus
)
{
    HI_S32 i = 0;
    HI_UNF_TUNER_CONNECT_PARA_S stConnectParam;
    HI_UNF_TUNER_STATUS_S stTunerStatus;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stTunerStatus, 0x00, sizeof(stTunerStatus));
    COM_MemSet(&stConnectParam, 0x00, sizeof(stConnectParam));
    stConnectParam.enSigType = pstOtaParam->enSigType;

    if (0 != (HI_UNF_TUNER_SIG_TYPE_CAB & pstOtaParam->enSigType))
    {
        COM_MemCpy(&stConnectParam.unConnectPara.stCab,
                   &pstOtaParam->unParam.stCab.stConnectParam,
                   sizeof(stConnectParam.unConnectPara.stCab));
    }
    else if (0 != (HI_UNF_TUNER_SIG_TYPE_SAT & pstOtaParam->enSigType))
    {
        COM_MemCpy(&stConnectParam.unConnectPara.stSat,
                   &pstOtaParam->unParam.stSat.stConnectParam,
                   sizeof(stConnectParam.unConnectPara.stSat));
    }
    else if ((0 != (HI_UNF_TUNER_SIG_TYPE_DVB_T & pstOtaParam->enSigType))
             || (0 != (HI_UNF_TUNER_SIG_TYPE_DVB_T2 & pstOtaParam->enSigType))
             || (0 != (HI_UNF_TUNER_SIG_TYPE_ISDB_T & pstOtaParam->enSigType))
             || (0 != (HI_UNF_TUNER_SIG_TYPE_ATSC_T & pstOtaParam->enSigType))
             || (0 != (HI_UNF_TUNER_SIG_TYPE_DTMB & pstOtaParam->enSigType)))
    {
        COM_MemCpy(&stConnectParam.unConnectPara.stTer,
                   &pstOtaParam->unParam.stTer.stConnectParam,
                   sizeof(stConnectParam.unConnectPara.stTer));
    }
    else
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = HI_UNF_TUNER_Connect(OTA_TUNER_ID, &stConnectParam, OTA_CONNECT_TIMEOUT);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_TUNER_Connect, s32Ret);
        return s32Ret;
    }

    for (i = 0; i < OTA_CONNECT_CHECKTIMES; i++)
    {
        s32Ret = HI_UNF_TUNER_GetStatus(OTA_TUNER_ID, &stTunerStatus);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_GetStatus, s32Ret);
            continue;
        }

        if (HI_UNF_TUNER_SIGNAL_LOCKED == stTunerStatus.enLockStatus)
        {
            *penStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
            return HI_SUCCESS;
        }

        COM_Delayms(OTA_CONNECT_CHECKINTERVAL);
    }

    *penStatus = HI_UNF_TUNER_SIGNAL_DROPPED;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaDemuxAttachTSPort(HI_U32 Dmxid)
{
    HI_UNF_DMX_PORT_E enDmxPort = 0;
    HI_UNF_DMX_PORT_ATTR_S stDmxPortAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    enDmxPort = GET_DEMUX_PORT();

    COM_MemSet(&stDmxPortAttr, 0x00, sizeof(stDmxPortAttr));
    s32Ret = HI_UNF_DMX_GetTSPortAttr(enDmxPort, &stDmxPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_GetTSPortAttr, s32Ret);
        return COM_ERR_DEVICE;
    }

    switch (enDmxPort)
    {
        case HI_UNF_DMX_PORT_TSI_0:
        {
            stDmxPortAttr.enPortType = HI_TSI0_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI0_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI0_CLK_PHASE_REVERSE;
            break;
        }

        case HI_UNF_DMX_PORT_TSI_1:
        {
            stDmxPortAttr.enPortType = HI_TSI1_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI1_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI1_CLK_PHASE_REVERSE;
            break;
        }

        case HI_UNF_DMX_PORT_TSI_2:
        {
            stDmxPortAttr.enPortType = HI_TSI2_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI2_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI2_CLK_PHASE_REVERSE;
            break;
        }

        case HI_UNF_DMX_PORT_TSI_3:
        {
            stDmxPortAttr.enPortType = HI_TSI3_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI3_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI3_CLK_PHASE_REVERSE;
            break;
        }

        default:
        {
            stDmxPortAttr.enPortType = HI_IF0_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_IF0_BIT_SELECTOR;
            break;
        }
    }

    s32Ret = HI_UNF_DMX_SetTSPortAttr(enDmxPort, &stDmxPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_SetTSPortAttr, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_DMX_AttachTSPort(Dmxid, enDmxPort);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_AttachTSPort, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaDemuxDetachTSPort(HI_U32 u32DmxId)
{
    COM_DBG_FuncEnter();

    COM_CHECK(HI_UNF_DMX_DetachTSPort(u32DmxId));

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 dlotaDemuxInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = dlotaDemuxAttachTSPort(OTA_D_DMXID);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaDemuxAttachTSPort, s32Ret);

        COM_CHECK(HI_UNF_DMX_DeInit());
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}
static HI_S32 dlotaDemuxDeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK(dlotaDemuxDetachTSPort(OTA_D_DMXID));
    COM_CHECK(HI_UNF_DMX_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_OTA_Init(HI_LOADER_PARAM_S* pstLoaderParam)
{
    HI_UNF_TUNER_LOCK_STATUS_E enStatus = HI_UNF_TUNER_SIGNAL_BUTT;
    HI_LOADER_PARAM_OTA_S* pstOTAParam = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    pstOTAParam = &pstLoaderParam->unParam.stOTAParam;

    s_u32ChannelPid = pstOTAParam->u32Pid;

    s32Ret = dlotaTunerOpen(pstOTAParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaTunerOpen, s32Ret);
        return s32Ret;
    }

    s32Ret = dlotaDemuxInit();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaDemuxInit, s32Ret);
        COM_CHECK(dlotaTunerClose());
        return s32Ret;
    }

    s32Ret = dlotaTunerLock(pstOTAParam, &enStatus);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(dlotaTunerLock, s32Ret);
        COM_CHECK(dlotaDemuxDeInit());
        COM_CHECK(dlotaTunerClose());
        return s32Ret;
    }

    if (HI_UNF_TUNER_SIGNAL_LOCKED != enStatus)
    {
        COM_CHECK(dlotaDemuxDeInit());
        COM_CHECK(dlotaTunerClose());
        COM_PrintErrCode(COM_ERR_SIGNAL_LOSE);
        return COM_ERR_SIGNAL_LOSE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_OTA_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    if (HI_INVALID_HANDLE != s_hChannel)
    {
        COM_CHECK(HI_UNF_DMX_CloseChannel(s_hChannel));

        if (HI_INVALID_HANDLE != s_hFilter)
        {
            COM_CHECK(HI_UNF_DMX_DetachFilter(s_hFilter, s_hChannel));
            COM_CHECK(HI_UNF_DMX_DestroyFilter(s_hFilter));
            s_hFilter  = HI_INVALID_HANDLE;
        }

        COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
        s_hChannel = HI_INVALID_HANDLE;
    }

    COM_CHECK(dlotaDemuxDeInit());
    COM_CHECK(dlotaTunerClose());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_OTA_GetData(HI_VOID* pbuffer, HI_U32 u32BufferSize, HI_U32* pu32DataLen)
{
    HI_U32 i = 0;
    HI_U32 u32AcquiredNum = 0;
    HI_UNF_DMX_DATA_S stDmxData;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pbuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataLen);
    COM_CHECK_PARAM(0 == u32BufferSize);

    for (i = 0; i < OTA_ACQUIRE_TRYTIMES; i++)
    {
        COM_MemSet(&stDmxData, 0x00, sizeof(stDmxData));
        s32Ret = HI_UNF_DMX_AcquireBuf(s_hChannel, 1, &u32AcquiredNum, &stDmxData, OTA_ACQUIRE_TIMEOUT);
        if (HI_SUCCESS != s32Ret)
        {
            COM_Delayms(OTA_ACQUIRE_TIMEOUT);
            continue;
        }

        if (0 < u32AcquiredNum)
        {
            break;
        }

        COM_Delayms(OTA_ACQUIRE_TIMEOUT);
    }

    if (OTA_ACQUIRE_TRYTIMES == i)
    {
        HI_UNF_TUNER_STATUS_S stStatus;

        COM_MemSet(&stStatus, 0x00, sizeof(stStatus));
        s32Ret = HI_UNF_TUNER_GetStatus(OTA_TUNER_ID, &stStatus);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_TUNER_GetStatus, s32Ret);
            return s32Ret;
        }

        if (HI_UNF_TUNER_SIGNAL_LOCKED != stStatus.enLockStatus)
        {
            COM_PrintErrCode(COM_ERR_SIGNAL_LOSE);
            return COM_ERR_SIGNAL_LOSE;
        }

        COM_PrintErrCode(COM_ERR_DOWNLOAD_TIMEOUT);
        return COM_ERR_DOWNLOAD_TIMEOUT;
    }

    if ((0 == stDmxData.u32Size)
        || (HI_NULL_PTR == stDmxData.pu8Data))
    {
        COM_PrintErrCode(COM_ERR_EXTERNAL);
        return COM_ERR_EXTERNAL;
    }

    if (stDmxData.u32Size > u32BufferSize)
    {
        COM_CHECK(HI_UNF_DMX_ReleaseBuf(s_hChannel, 1, &stDmxData));

        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    COM_MemCpy(pbuffer, stDmxData.pu8Data, stDmxData.u32Size);
    *pu32DataLen = stDmxData.u32Size;

    COM_CHECK(HI_UNF_DMX_ReleaseBuf(s_hChannel, 1, &stDmxData));

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_OTA_SetFilter(HI_UNF_DMX_FILTER_ATTR_S* pstFilterAttr)
{
    HI_U32 u32DmxId = OTA_D_DMXID;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstFilterAttr);

    if (HI_INVALID_HANDLE == s_hChannel)
    {
        COM_MemSet(&stChAttr, 0x00, sizeof(stChAttr));

        s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&stChAttr);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_DMX_GetChannelDefaultAttr, s32Ret);
            return COM_ERR_DEVICE;
        }

        stChAttr.enChannelType  = HI_UNF_DMX_CHAN_TYPE_SEC;
        stChAttr.enCRCMode      = HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD;
        stChAttr.enOutputMode   = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        s32Ret = HI_UNF_DMX_CreateChannel(u32DmxId, &stChAttr, &s_hChannel);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_DMX_CreateChannel, s32Ret);
            return COM_ERR_DEVICE;
        }
    }
    else
    {
        s32Ret = HI_UNF_DMX_CloseChannel(s_hChannel);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_UNF_DMX_CloseChannel, s32Ret);

            COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
            s_hChannel = HI_INVALID_HANDLE;

            return COM_ERR_DEVICE;
        }
    }

    s32Ret = HI_UNF_DMX_SetChannelPID(s_hChannel, s_u32ChannelPid);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_SetChannelPID, s32Ret);

        COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
        s_hChannel = HI_INVALID_HANDLE;

        return COM_ERR_DEVICE;
    }

    if (HI_INVALID_HANDLE != s_hFilter)
    {
        COM_CHECK(HI_UNF_DMX_DetachFilter(s_hFilter, s_hChannel));
        COM_CHECK(HI_UNF_DMX_DestroyFilter(s_hFilter));

        s_hFilter = HI_INVALID_HANDLE;
    }

    s32Ret = HI_UNF_DMX_CreateFilter(u32DmxId, pstFilterAttr, &s_hFilter);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_CreateFilter, s32Ret);

        COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
        s_hChannel = HI_INVALID_HANDLE;

        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_DMX_AttachFilter(s_hFilter, s_hChannel);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_AttachFilter, s32Ret);

        COM_CHECK(HI_UNF_DMX_DestroyFilter(s_hFilter));
        s_hFilter = HI_INVALID_HANDLE;

        COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
        s_hChannel = HI_INVALID_HANDLE;

        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(s_hChannel);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_DMX_OpenChannel, s32Ret);

        COM_CHECK(HI_UNF_DMX_DetachFilter(s_hFilter, s_hChannel));

        COM_CHECK(HI_UNF_DMX_DestroyFilter(s_hFilter));
        s_hFilter = HI_INVALID_HANDLE;

        COM_CHECK(HI_UNF_DMX_DestroyChannel(s_hChannel));
        s_hChannel = HI_INVALID_HANDLE;

        return COM_ERR_DEVICE;

    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

