#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "hi_type.h"
#include "hi_loader_info_v2.h"
#include "loader_dbinfo.h"

#define LOADER_D_PID_MIN                (32)
#define LOADER_D_PID_MAX                (8190)
#define LOADER_D_FREQ_MIN               (100)
#define LOADER_D_FREQ_MAX               (900)
#define LOADER_D_SYMBLE_MIN             (100)
#define LOADER_D_SYMBLE_MAX             (9000)

#define LOADER_D_BANDWIDTH_MIN          (6000)
#define LOADER_D_BANDWIDTH_MAX          (9000)
#define LOADER_D_DEVICEITEM_LEN         (128)                   /**< Max length of device information items name */

#define LOADER_D_FLASHMAP_DEVICEINFO	"deviceinfo"

#define LOADER_PrintFuncErr(Func, ErrCode)      printf("> [%s][%u] Call [%s] return [0x%08x]\n", __FUNCTION__, __LINE__, #Func, ErrCode);
#define LOADER_PrintErrInfo(ErrInfo)            printf("> [%s][%u] Error info: %s \n", __FUNCTION__, __LINE__, ErrInfo);
#define LOADER_Notice(pszFormat...)             printf("> "); printf(pszFormat);
#define LOADER_Print(pszFormat...)              printf(pszFormat);
#define LOADER_CHECK(func)                              \
    {                                                       \
        HI_S32 s32ErrCode = func;                           \
        if (HI_SUCCESS != s32ErrCode)                       \
        {                                                   \
            LOADER_PrintFuncErr(#func, s32ErrCode);         \
        }                                                   \
    }
#define LOADER_CHECK_INPUTPARAM(val)                    \
    {                                                       \
        if (val)                                            \
        {                                                   \
            LOADER_PrintErrInfo("Input Error!");            \
            return -1;                                      \
        }                                                   \
    }


#define MODIFY_NUMBER(x) do {\
        HI_U32 num; \
        if (0 == get_input_number(&num)) { \
            (x) = num; \
        }}while(0)

#define MODIFY_IP(ip) do { \
        char *pstrip; \
        if (0 == get_input(&pstrip)) { \
            (ip) = inet_addr(pstrip);\
        }}while(0)

#define IP_PRINT(x) do {\
        unsigned int ip = (unsigned int)(x);\
        printf("%d.%d.%d.%d", (int)((ip) & 0xff), (int)((ip >> 8) & 0xff), \
               (int)((ip >> 16)  & 0xff), (int)((ip >> 24) & 0xff));\
    }while(0)

HI_VOID ShowUsageInfo(HI_VOID)
{
    LOADER_Notice("usage: sample_loader [-t trigger] [-s set] [-g get] [[command] arg].\n");
    LOADER_Notice("command as follows:\n");
    LOADER_Notice("sample_loader -t                  -- configure loader upgrade parameter and trigger it run.\n");
    LOADER_Notice("sample_loader -s deviceinfo       -- configure deviceinfo.\n");
    LOADER_Notice("sample_loader -s sw               -- configure software.\n");
    LOADER_Notice("sample_loader -g deviceinfo       -- get and display deviceinfo info.\n");
    LOADER_Notice("sample_loader -g sw               -- get and display software version info.\n");

    return;
}

static HI_CHAR* ltrim(HI_CHAR* pcString)
{
    HI_CHAR* pc = HI_NULL_PTR;

    if (HI_NULL_PTR == pcString)
    {
        return HI_NULL_PTR;
    }

    pc = pcString;

    while (*pc)
    {
        if ((0x20 != *pc) && ('\t' != *pc) && ('\n' != *pc) && ('\r' != *pc))
        {
            return pc;
        }
        pc++;
    }

    return pc;
}

static void rtrim(HI_CHAR* pcString)
{
    HI_CHAR* pc = HI_NULL_PTR;

    if (HI_NULL_PTR == pcString)
    {
        return;
    }

    if (!strlen(pcString))
    {
        return;
    }

    pc = pcString + 1;
    while (*pc)
    {
        if ((0x20 == *pc) || ('\t' == *pc) || ('\n' == *pc) || ('\r' == *pc))
        {
            *pc = 0;
            return;
        }
        pc++;
    }
}

static HI_CHAR g_aucBuffer[128];

static int get_input( char** ppstr )
{
    HI_U32  u32Bytes;
    HI_CHAR*  pc;

    if (HI_NULL_PTR == ppstr)
    {
        return -1;
    }

    memset(g_aucBuffer, 0x00, sizeof(g_aucBuffer));
    LOADER_Print(" ? ");
    fflush(stdout);
    read( 0, g_aucBuffer, 127);

    pc = ltrim(&g_aucBuffer[0]);
    rtrim(pc);
    u32Bytes = strlen(pc);
    if (u32Bytes == 0)
    {
        /* <CR> pressed as only input, don't modify current
         * location and move to next. "-" pressed will go back.
         */
        return -1;
    }
    else
    {
        *ppstr = pc;
        return 0;
    }
}

static int get_input_number(HI_U32* pnum)
{
    HI_U32 u32Bytes;
    char  acBuf[128] = {0};       /* console I/O buffer   */

    LOADER_Print(" ? ");
    fflush(stdout);
    read(0, acBuf, 127);

    u32Bytes = strlen(acBuf);

    if (u32Bytes == 0)
    { return -1; }
    else
    {
        char* endp;
        /* hex */
        if (!strncmp(acBuf, "0x", sizeof("0x") - 1))
        { *pnum = strtoul(acBuf, &endp, 16); }
        else
        { *pnum = strtoul(acBuf, &endp, 10); }

        u32Bytes = (HI_U32)(endp - acBuf);
        if (u32Bytes)
        { return 0; }
        else
        { return -1; }
    }
}

static HI_U32 OTA_GetModulationIndex(HI_UNF_MODULATION_TYPE_E enModulationType)
{
    switch (enModulationType)
    {
        case HI_UNF_MOD_TYPE_QAM_16:
            return 0;
        case HI_UNF_MOD_TYPE_QAM_32:
            return 1;
        case HI_UNF_MOD_TYPE_QAM_64:
            return 2;
        case HI_UNF_MOD_TYPE_QAM_128:
            return 3;
        case HI_UNF_MOD_TYPE_QAM_256:
            return 4;
        default:
            break;
    }
    return 0;
}

static HI_UNF_MODULATION_TYPE_E OTA_GetModulationType(HI_U32 u32ModulationIndex)
{
    switch (u32ModulationIndex)
    {
        case 0:
            return HI_UNF_MOD_TYPE_QAM_16;
        case 1:
            return HI_UNF_MOD_TYPE_QAM_32;
        case 2:
            return HI_UNF_MOD_TYPE_QAM_64;
        case 3:
            return HI_UNF_MOD_TYPE_QAM_128;
        case 4:
            return HI_UNF_MOD_TYPE_QAM_256;
        default:
            break;
    }
    return HI_UNF_MOD_TYPE_BUTT;
}


static HI_S32 OTA_Cable(HI_LOADER_PARAM_OTA_S* pstOTAParam)
{
    HI_U32 tmp;
    HI_U32 u32QAMIndex = 0;
    HI_UNF_CAB_CONNECT_PARA_S* pstConnectParam = &pstOTAParam->unParam.stCab.stConnectParam;

    LOADER_Notice("Input PID(%u~%u)[%d]", LOADER_D_PID_MIN, LOADER_D_PID_MAX, pstOTAParam->u32Pid);

    MODIFY_NUMBER(pstOTAParam->u32Pid);
    if ((pstOTAParam->u32Pid < LOADER_D_PID_MIN)
        || (pstOTAParam->u32Pid > LOADER_D_PID_MAX))
    {
        LOADER_Notice("pid error should be %u~%u\n", LOADER_D_PID_MIN, LOADER_D_PID_MAX);
        return -1;
    }

    LOADER_Notice("Input Frequency(MHz)(%u~%u)[%d]", LOADER_D_FREQ_MIN, LOADER_D_FREQ_MAX, pstConnectParam->u32Freq / 1000);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32Freq  = tmp * 1000;
    }

    if ((pstConnectParam->u32Freq / 1000 < LOADER_D_FREQ_MIN)
        || (pstConnectParam->u32Freq / 1000 > LOADER_D_FREQ_MAX))
    {
        LOADER_Notice("frequency error, should be %u~%u\n", LOADER_D_FREQ_MIN, LOADER_D_FREQ_MAX);
        return -1;
    }

    LOADER_Notice("Input Symbol rate(KS/s)(%u~%u)[%d]", LOADER_D_SYMBLE_MIN, LOADER_D_SYMBLE_MAX, pstConnectParam->u32SymbolRate / 1000);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32SymbolRate  = tmp * 1000;
    }

    if ((pstConnectParam->u32SymbolRate / 1000 < LOADER_D_SYMBLE_MIN)
        || (pstConnectParam->u32SymbolRate / 1000 > LOADER_D_SYMBLE_MAX))
    {
        LOADER_Notice("symbol_rate error, should be %u~%u\n", LOADER_D_SYMBLE_MIN, LOADER_D_SYMBLE_MAX);
        return -1;
    }

    u32QAMIndex = OTA_GetModulationIndex(pstConnectParam->enModType);
    LOADER_Notice("Select Modulation(0-16Qam,1-32Qam,2-64Qam,3-128Qam,4-256Qam)[%d]", u32QAMIndex);
    if (0 == get_input_number(&u32QAMIndex))
    {
        pstConnectParam->enModType = OTA_GetModulationType(u32QAMIndex);
    }

    if (HI_UNF_MOD_TYPE_BUTT <= pstConnectParam->enModType)
    {
        LOADER_Notice("modulation error, should be 0~4\n");
        return -1;
    }

    pstConnectParam->bReverse = HI_FALSE;
    return HI_SUCCESS;
}

static HI_S32 OTA_Terrestrial(HI_LOADER_PARAM_OTA_S* pstOTAParam)
{
    HI_U32 tmp = 0;
    HI_U32 u32QAMIndex = 0;
    HI_UNF_TER_CONNECT_PARA_S* pstConnectParam = &pstOTAParam->unParam.stTer.stConnectParam;

    LOADER_Notice("Input PID(%u~%u)[%d]", LOADER_D_PID_MIN, LOADER_D_PID_MAX, pstOTAParam->u32Pid);

    MODIFY_NUMBER(pstOTAParam->u32Pid);
    if ((pstOTAParam->u32Pid < LOADER_D_PID_MIN)
        || (pstOTAParam->u32Pid > LOADER_D_PID_MAX))
    {
        LOADER_Notice("PID error should be %u~%u\n", LOADER_D_PID_MIN, LOADER_D_PID_MAX);
        return -1;
    }

    LOADER_Notice("Input Frequency(MHz)(%u~%u)[%d]", LOADER_D_FREQ_MIN, LOADER_D_FREQ_MAX, pstConnectParam->u32Freq / 1000);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32Freq = tmp * 1000;
    }

    if ((pstConnectParam->u32Freq / 1000 < LOADER_D_FREQ_MIN)
        || (pstConnectParam->u32Freq / 1000 > LOADER_D_FREQ_MAX))
    {
        LOADER_Notice("Frequency error, should be %u~%u\n", LOADER_D_FREQ_MIN, LOADER_D_FREQ_MAX);
        return -1;
    }

    LOADER_Notice("Input BandWidth(KHz)(%u~%u)[%d]", LOADER_D_BANDWIDTH_MIN, LOADER_D_BANDWIDTH_MAX, pstConnectParam->u32BandWidth);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32BandWidth = tmp;
    }

    if ((pstConnectParam->u32BandWidth  < LOADER_D_BANDWIDTH_MIN)
        || (pstConnectParam->u32BandWidth > LOADER_D_BANDWIDTH_MAX))
    {
        LOADER_Notice("BandWidth error, should be %u~%u\n", LOADER_D_BANDWIDTH_MIN, LOADER_D_BANDWIDTH_MAX);
        return -1;
    }


    u32QAMIndex = OTA_GetModulationIndex(pstConnectParam->enModType);
    LOADER_Notice("Select Modulation(0-16Qam,1-32Qam,2-64Qam,3-128Qam,4-256Qam)[%d]", u32QAMIndex);
    if (0 == get_input_number(&u32QAMIndex))
    {
        pstConnectParam->enModType = OTA_GetModulationType(u32QAMIndex);
    }

    if (HI_UNF_MOD_TYPE_BUTT <= pstConnectParam->enModType)
    {
        LOADER_Notice("modulation error, should be 0~4\n");
        return -1;
    }

    if (HI_UNF_TUNER_SIG_TYPE_DVB_T == pstOTAParam->enSigType)
    {
        LOADER_Notice("Select Priority type, 0:no priority, 1:high priority,2:low priority[%d]", pstConnectParam->enDVBTPrio);
        MODIFY_NUMBER(pstConnectParam->enDVBTPrio);
        if (pstConnectParam->enDVBTPrio > 2)
        {
            LOADER_Notice("priority type error, should be 0~2\n");
            return -1;
        }
    }
    else if (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == pstOTAParam->enSigType)
    {
        LOADER_Notice("Select ChannelMode type, 0:only base signal, 1:base and lite signal[%d]", pstConnectParam->enChannelMode);
        MODIFY_NUMBER(pstConnectParam->enChannelMode);
        if (pstConnectParam->enChannelMode > 1)
        {
            LOADER_Notice("ChannelMode error, should be 0~1\n");
            return -1;
        }

        LOADER_Notice("Input PLPId [%d]", pstOTAParam->unParam.stTer.u32PLPId);
        MODIFY_NUMBER(pstOTAParam->unParam.stTer.u32PLPId);
    }

    return HI_SUCCESS;
}

static HI_S32 OTA_Satellite(HI_LOADER_PARAM_OTA_S* pstOTAParam)
{
    HI_U32 tmp;
    HI_UNF_SAT_CONNECT_PARA_S* pstConnectParam = &pstOTAParam->unParam.stSat.stConnectParam;

    LOADER_Notice("Input PID(%u~%u)[%d]", LOADER_D_PID_MIN, LOADER_D_PID_MAX, pstOTAParam->u32Pid);

    MODIFY_NUMBER(pstOTAParam->u32Pid);
    if ((pstOTAParam->u32Pid < LOADER_D_PID_MIN)
        || (pstOTAParam->u32Pid > LOADER_D_PID_MAX))
    {
        LOADER_Notice("PID error should be %u~%u\n", LOADER_D_PID_MIN, LOADER_D_PID_MAX);
        return -1;
    }

    LOADER_Notice("Input Frequency(MHz)[%d]", pstConnectParam->u32Freq / 1000);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32Freq = tmp * 1000;
    }

    LOADER_Notice("Input Symbol Rate(KS/s)[%d]", pstConnectParam->u32SymbolRate / 1000);
    if (0 == get_input_number(&tmp))
    {
        pstConnectParam->u32SymbolRate = tmp * 1000;
    }

    LOADER_Notice("Select Polarization Type(0-Horizontal,1-Vertical, 2-Left hand, 3-Right hand)[%d]", pstConnectParam->enPolar);
    MODIFY_NUMBER(pstConnectParam->enPolar);
    if (pstConnectParam->enPolar > 3)
    {
        LOADER_Notice("Polarization type error, should be 0~3\n");
        return -1;
    }

    LOADER_Notice("Select LNB Power type(0-LNB Power Off, 1-LNB Power On)[%d]", pstOTAParam->unParam.stSat.enLNBPower);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.enLNBPower);
    if (pstOTAParam->unParam.stSat.enLNBPower > 1)
    {
        LOADER_Notice("LNB Power type Error!");
        return -1;
    }
    else if (1 == pstOTAParam->unParam.stSat.enLNBPower)
    {
        LOADER_Notice("Select LNB type(0-SINGLE, 1-DUAL, 2-UNICABLE)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.enLNBType);
        MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.enLNBType);

        if (HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY == pstOTAParam->unParam.stSat.stLNBConfig.enLNBType)
        {
            LOADER_Notice("Input LNB Frequency(MHZ)[%d]",  pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO = tmp;
            }

            pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO = pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO;


            return -1;
        }
        else if (HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY == pstOTAParam->unParam.stSat.stLNBConfig.enLNBType)
        {
            LOADER_Notice("Input LNB Low Frequency(MHZ)[%d]",  pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO = tmp;
            }

            LOADER_Notice("Input LNB High Frequency(MHZ)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO = tmp;
            }

            LOADER_Notice("Select LNBBand(0-c, 1-ku)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand);
            MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand);
            if ((pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 0)
                && (pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 1))
            {
                LOADER_Notice("LNBBand error, should be 0~1\n");
                return -1;
            }
        }
        else if (HI_UNF_TUNER_FE_LNB_UNICABLE == pstOTAParam->unParam.stSat.stLNBConfig.enLNBType)
        {
            pstOTAParam->unParam.stSat.stLNBConfig.enLNBType = HI_UNF_TUNER_FE_LNB_UNICABLE;

            LOADER_Notice("Input LNB Low Frequency(MHZ)[%d]",  pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO = tmp;
            }

            LOADER_Notice("Input LNB High Frequency(MHZ)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO = tmp;
            }

            LOADER_Notice("Select LNBBand(0-c, 1-ku)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand);
            MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand);
            if ((pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 0)
                && (pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 1))
            {
                LOADER_Notice("LNBBand error, should be 0~1\n");
                return -1;
            }

            LOADER_Notice("Input UNIC_SCRNO(0~7)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u8UNIC_SCRNO);
            MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.u8UNIC_SCRNO);
            if (pstOTAParam->unParam.stSat.stLNBConfig.u8UNIC_SCRNO > 7)
            {
                LOADER_Notice("SCR number error, should be 0~7\n");
                return -1;
            }

            LOADER_Notice("Input UNICIF Frequency(MHZ)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u32UNICIFFreqMHz);
            if (0 == get_input_number(&tmp))
            {
                pstOTAParam->unParam.stSat.stLNBConfig.u32UNICIFFreqMHz = tmp;
            }

            LOADER_Notice("Select LNBBand(0-PortA, 1-PortB)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.enSatPosn);
            MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.enSatPosn);
            if ((pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 0)
                && (pstOTAParam->unParam.stSat.stLNBConfig.enLNBBand != 1))
            {
                LOADER_Notice("unicable multi-switch port error, should be 0~1\n");
                return -1;
            }
        }
        else
        {
            LOADER_Notice("LNB type error, should be(0-SINGLE, 1-DUAL, 2-UNICABLE)\n");
            return -1;
        }
    }
    else
    {
        LOADER_Notice("Input LNB Low Frequency(MHZ)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO);
        MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.u32LowLO);

        LOADER_Notice("Input LNB High Frequency(MHZ)[%d]", pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO);
        MODIFY_NUMBER(pstOTAParam->unParam.stSat.stLNBConfig.u32HighLO);
    }

    LOADER_Notice("Select 22K Switch(0-none, 1-0, 2-22)[%d]", pstOTAParam->unParam.stSat.enSwitch22K);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.enSwitch22K);
    if (pstOTAParam->unParam.stSat.enSwitch22K >= HI_UNF_TUNER_SWITCH_22K_BUTT)
    {
        LOADER_Notice("22K Switch Error!");
        return -1;
    }

    LOADER_Notice("Select DiSEqC1.1 Port(0-Disable, 1-port1, 2-port2,etc)[%d]", pstOTAParam->unParam.stSat.stSwitch16Port.enPort);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.stSwitch16Port.enPort);
    if (pstOTAParam->unParam.stSat.stSwitch16Port.enPort >= HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT)
    {
        LOADER_Notice("DiSEqC1.1 Port Error!");
        return -1;
    }

    LOADER_Notice("Select DiSEqC1.1 device level(0-1.x one way, 1-2.x two way)[%d]", pstOTAParam->unParam.stSat.stSwitch16Port.enLevel);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.stSwitch16Port.enLevel);
    if ((pstOTAParam->unParam.stSat.stSwitch16Port.enLevel != 0)
        && (pstOTAParam->unParam.stSat.stSwitch16Port.enLevel != 1))
    {
        LOADER_Notice("unicable multi-switch port error, should be 0~1\n");
        return -1;
    }

    LOADER_Notice("Select DiSEqC1.0 Port(0-Disable, 1-port1, 2-port2,etc)[%d]", pstOTAParam->unParam.stSat.stSwitch4Port.enPort);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.stSwitch4Port.enPort);
    if (pstOTAParam->unParam.stSat.stSwitch4Port.enPort > 4)
    {
        LOADER_Notice("DiSEqC1.0 Port Error!");
        return -1;
    }

    LOADER_Notice("Select DiSEqC1.0 device level(0-1.x one way, 1-2.x two way)[%d]", pstOTAParam->unParam.stSat.stSwitch4Port.enLevel);
    MODIFY_NUMBER(pstOTAParam->unParam.stSat.stSwitch4Port.enLevel);
    if ((pstOTAParam->unParam.stSat.stSwitch4Port.enLevel != 0)
        && (pstOTAParam->unParam.stSat.stSwitch4Port.enLevel != 1))
    {
        LOADER_Notice("unicable multi-switch port error, should be 0~1\n");
        return -1;
    }

    pstOTAParam->unParam.stSat.stSwitch4Port.enPolar = pstOTAParam->unParam.stSat.stConnectParam.enPolar;
    if (HI_UNF_TUNER_SWITCH_22K_NONE == pstOTAParam->unParam.stSat.enSwitch22K)
    {
        pstOTAParam->unParam.stSat.stSwitch4Port.enLNB22K = HI_UNF_TUNER_FE_LNB_22K_OFF;
    }
    else
    {
        pstOTAParam->unParam.stSat.stSwitch4Port.enLNB22K = HI_UNF_TUNER_FE_LNB_22K_ON;
    }


    return HI_SUCCESS;
}

static HI_S32 IP(HI_LOADER_PARAM_IP_S* pstIPParam)
{
    char* pstr;

    LOADER_Notice("Select protocol type(0-TFTP, 1-FTP, 2-HTTP)");
    MODIFY_NUMBER(pstIPParam->enProtType);
    if (pstIPParam->enProtType >= HI_LOADER_IPPROT_BUTT)
    {
        LOADER_Notice("protocol type error!\n");
        return -1;
    }

    LOADER_Notice("Select IP type(0-static, 1-dynamic)");
    MODIFY_NUMBER(pstIPParam->enIPCfgType);
    if (pstIPParam->enIPCfgType > HI_LOADER_IPCFG_BUTT)
    {
        LOADER_Notice("IP type error!\n");
        return -1;
    }

    if (HI_LOADER_IPCFG_STATIC == pstIPParam->enIPCfgType)
    {
        LOADER_Notice("Input IP address[");
        IP_PRINT(pstIPParam->ipOurIp);
        LOADER_Print("]");
        MODIFY_IP(pstIPParam->ipOurIp);

        LOADER_Notice("Input IP gateway[");
        IP_PRINT(pstIPParam->ipGateway);
        LOADER_Print("]");
        MODIFY_IP(pstIPParam->ipGateway);

        LOADER_Notice("Input subnet mask[");
        IP_PRINT(pstIPParam->ipSubnetMask);
        LOADER_Print("]");
        MODIFY_IP(pstIPParam->ipSubnetMask);
    }

    LOADER_Notice("Input server IP address[");
    IP_PRINT(pstIPParam->ipServer);
    LOADER_Print("]");
    MODIFY_IP(pstIPParam->ipServer);

    LOADER_Notice("Input server port(default:");
    if (HI_LOADER_IPPROT_TFTP == pstIPParam->enProtType)
    {
        LOADER_Print("69");
        pstIPParam->ipServerPort = 69;
    }
    else if (HI_LOADER_IPPROT_FTP == pstIPParam->enProtType)
    {
        LOADER_Print("21");
        pstIPParam->ipServerPort = 21;
    }
    else if (HI_LOADER_IPPROT_HTTP == pstIPParam->enProtType)
    {
        LOADER_Print("80");
        pstIPParam->ipServerPort = 80;
    }
    LOADER_Print(")");
    MODIFY_NUMBER(pstIPParam->ipServerPort);

    LOADER_Notice("Input upgrade file name[%s]", pstIPParam->as8FileName);
    if (0 == get_input(&pstr))
    {
        strncpy((char*)pstIPParam->as8FileName, pstr, sizeof(pstIPParam->as8FileName));
        pstIPParam->as8FileName[sizeof(pstIPParam->as8FileName) - 1] = '\0';
    }

    if (HI_LOADER_IPPROT_FTP == pstIPParam->enProtType)
    {
        LOADER_Notice("Input user name[%s]", pstIPParam->as8UserName);
        if (0 == get_input(&pstr))
        {
            strncpy((char*)pstIPParam->as8UserName, pstr, sizeof(pstIPParam->as8UserName));
            pstIPParam->as8UserName[sizeof(pstIPParam->as8UserName) - 1] = '\0';
        }

        LOADER_Notice("Input password[%s]", pstIPParam->as8Password);
        if (0 == get_input(&pstr))
        {
            strncpy((char*)pstIPParam->as8Password, pstr, sizeof(pstIPParam->as8Password));
            pstIPParam->as8Password[sizeof(pstIPParam->as8Password) - 1] = '\0';
        }
    }
    return 0;
}

static HI_S32 USB(HI_LOADER_PARAM_USB_S* pstUSBParam)
{
    char* pstr = HI_NULL_PTR;

    LOADER_Notice("specify upgrade file name[%s]", pstUSBParam->as8FileName);

    if (0 == get_input(&pstr))
    {
        if ((strlen(pstr) > HI_LOADER_FILENAME_LEN)
            || (strlen(pstr) == 0))
        {
            LOADER_Notice("Please input a valid filename\n");
            return HI_FAILURE;
        }

        strncpy((char*)pstUSBParam->as8FileName, pstr, HI_LOADER_FILENAME_LEN - 1);
        pstUSBParam->as8FileName[HI_LOADER_FILENAME_LEN - 1] = '\0';
    }
    else
    {
        if (0 == strlen((HI_CHAR *)pstUSBParam->as8FileName))
        {
            LOADER_Notice("Please input a valid filename\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 FS(HI_LOADER_PARAM_FS_S* pstFSParam)
{
    char* pstr = HI_NULL_PTR;

    LOADER_Notice("specify upgrade file name[%s]", pstFSParam->as8FileName);

    if (0 == get_input(&pstr))
    {
        if ((strlen(pstr) > HI_LOADER_FILENAME_LEN)
            || (strlen(pstr) == 0))
        {
            LOADER_Notice("Please input a valid filename\n");
            return HI_FAILURE;
        }

        strncpy((char*)pstFSParam->as8FileName, pstr, HI_LOADER_FILENAME_LEN - 1);
        pstFSParam->as8FileName[HI_LOADER_FILENAME_LEN - 1] = '\0';
    }
    else
    {
        if (0 == strlen((HI_CHAR *)pstFSParam->as8FileName))
        {
            LOADER_Notice("Please input a valid filename\n");
            return HI_FAILURE;
        }
    }

    LOADER_Notice("specify upgrade partition name[%s]", pstFSParam->as8PartitonName);
    if (0 == get_input(&pstr))
    {
        if ((strlen(pstr) > HI_LOADER_PARTITION_NAME_LEN)
            || (strlen(pstr) == 0))
        {
            LOADER_Notice("Please input a valid filename\n");
            return HI_FAILURE;
        }

        strncpy((char*)pstFSParam->as8PartitonName, pstr, HI_LOADER_PARTITION_NAME_LEN - 1);
        pstFSParam->as8PartitonName[HI_LOADER_PARTITION_NAME_LEN - 1] = '\0';
    }
    else
    {
        if (0 == strlen((HI_CHAR *)pstFSParam->as8PartitonName))
        {
            LOADER_Notice("Please input a valid partitonName\n");
            return HI_FAILURE;
        }
    }


    return HI_SUCCESS;
}

static HI_VOID ShowSelect()
{
    HI_LOADER_PARAM_S stLoaderInfo;
    HI_S32 s32Ret = HI_FAILURE;

    LOADER_Notice("\n");
    LOADER_Notice("Upgrade Parameter configuration success! \n");

    memset(&stLoaderInfo, 0x00, sizeof(stLoaderInfo));
    s32Ret = HI_LOADER_GetParameter(&stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_LOADER_GetParameter, s32Ret);
        return;
    }

    switch (stLoaderInfo.enUpgradeType)
    {
        case HI_LOADER_UPGRADE_TYPE_USB:
        {
            LOADER_Notice("Download Type       : USB\n");
            LOADER_Notice("Upgrade Filename    : %s\n", stLoaderInfo.unParam.stUSBParam.as8FileName);

            break;
        }
        case HI_LOADER_UPGRADE_TYPE_FS:
        {
            LOADER_Notice("Download Type       : FS\n");
            LOADER_Notice("Upgrade Filename    : %s\n", stLoaderInfo.unParam.stFSParam.as8FileName);
            LOADER_Notice("Upgrade PartitonName: %s\n", stLoaderInfo.unParam.stFSParam.as8PartitonName);

            break;
        }

        case HI_LOADER_UPGRADE_TYPE_OTA:
        {
            LOADER_Notice("Download Type       : OTA\n");

            if (HI_UNF_TUNER_SIG_TYPE_CAB == stLoaderInfo.unParam.stOTAParam.enSigType)
            {
                HI_UNF_CAB_CONNECT_PARA_S* pstConnectParam = &stLoaderInfo.unParam.stOTAParam.unParam.stCab.stConnectParam;

                LOADER_Notice("Signal type         : Cable\n");
                LOADER_Notice("PID                 : %u\n",       stLoaderInfo.unParam.stOTAParam.u32Pid);
                LOADER_Notice("Frequency           : %u(MHZ)\n",  pstConnectParam->u32Freq / 1000);
                LOADER_Notice("Symrate             : %u(Kbps)\n", pstConnectParam->u32SymbolRate / 1000);
                LOADER_Notice("Reverse             : %u(0:False 1:True)\n", pstConnectParam->bReverse);

                switch (pstConnectParam->enModType)
                {
                    case HI_UNF_MOD_TYPE_QAM_16:
                        LOADER_Notice("Modulation          : 16QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_32:
                        LOADER_Notice("Modulation          : 32QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_64:
                        LOADER_Notice("Modulation          : 64QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_128:
                        LOADER_Notice("Modulation          : 125QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_256:
                        LOADER_Notice("Modulation          : 256QAM\n");
                        break;
                    default:
                        break;
                }
            }
            else if (((HI_UNF_TUNER_SIG_TYPE_DVB_T & stLoaderInfo.unParam.stOTAParam.enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DVB_T2 & stLoaderInfo.unParam.stOTAParam.enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ISDB_T & stLoaderInfo.unParam.stOTAParam.enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_ATSC_T & stLoaderInfo.unParam.stOTAParam.enSigType) != 0)
                     || ((HI_UNF_TUNER_SIG_TYPE_DTMB & stLoaderInfo.unParam.stOTAParam.enSigType) != 0))
            {
                HI_UNF_TER_CONNECT_PARA_S* pstConnectParam = &stLoaderInfo.unParam.stOTAParam.unParam.stTer.stConnectParam;
                LOADER_Notice("Signal type         : Terrestrial DVB-T/T2\n");
                LOADER_Notice("PID                 : %u\n",        stLoaderInfo.unParam.stOTAParam.u32Pid);
                LOADER_Notice("Frequency           : %u(MHZ)\n",   pstConnectParam->u32Freq / 1000);
                LOADER_Notice("BandWidth           : %u(KHZ)\n",   pstConnectParam->u32BandWidth);
                LOADER_Notice("Reverse             : %u(0:False 1:True)\n", pstConnectParam->bReverse);

                switch (pstConnectParam->enModType)
                {
                    case HI_UNF_MOD_TYPE_QAM_16:
                        LOADER_Notice("Modulation          : 16QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_32:
                        LOADER_Notice("Modulation          : 32QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_64:
                        LOADER_Notice("Modulation          : 64QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_128:
                        LOADER_Notice("Modulation          : 125QAM\n");
                        break;
                    case HI_UNF_MOD_TYPE_QAM_256:
                        LOADER_Notice("Modulation          : 256QAM\n");
                        break;
                    default:
                        LOADER_Notice("Modulation          : Invalid Paramater\n");
                        break;
                }

                switch (pstConnectParam->enChannelMode)
                {
                    case HI_UNF_TUNER_TER_MODE_BASE:
                        LOADER_Notice("Channel Mode        : Base channel\n");
                        break;
                    case HI_UNF_TUNER_TER_MODE_LITE:
                        LOADER_Notice("Channel Mode        : Lite channel\n");
                        break;
                    default:
                        LOADER_Notice("Channel Mode        : Invalid Paramater\n");
                        break;
                }

                switch (pstConnectParam->enDVBTPrio)
                {
                    case HI_UNF_TUNER_TS_PRIORITY_NONE:
                        LOADER_Notice("TS Priority         : No priority\n");
                        break;
                    case HI_UNF_TUNER_TS_PRIORITY_HP:
                        LOADER_Notice("TS Priority         : High priority\n");
                        break;
                    case HI_UNF_TUNER_TS_PRIORITY_LP:
                        LOADER_Notice("TS Priority         : Low priority\n");
                        break;
                    default:
                        LOADER_Notice("TS Priority         : Invalid Paramater\n");
                        break;
                }

                LOADER_Notice("PLPId               : %u\n", stLoaderInfo.unParam.stOTAParam.unParam.stTer.u32PLPId);
            }
            else if (HI_UNF_TUNER_SIG_TYPE_SAT == stLoaderInfo.unParam.stOTAParam.enSigType)
            {
                HI_LOADER_PARAM_SAT_S* pstSatParam = &stLoaderInfo.unParam.stOTAParam.unParam.stSat;
                HI_UNF_SAT_CONNECT_PARA_S* pstConnectParam = &pstSatParam->stConnectParam;

                LOADER_Notice("Signal type         : Satellite\n");
                LOADER_Notice("PID                 : %d\n",       stLoaderInfo.unParam.stOTAParam.u32Pid);
                LOADER_Notice("Frequency           : %d(MHZ)\n",  pstConnectParam->u32Freq / 1000);
                LOADER_Notice("SymbolRate          : %d(Kbps)\n", pstConnectParam->u32SymbolRate / 1000);

                switch (pstConnectParam->enPolar)
                {
                    case HI_UNF_TUNER_FE_POLARIZATION_H:
                        LOADER_Notice("Polarization        : Horizontal\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_V:
                        LOADER_Notice("Polarization        : Vertical\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_L:
                        LOADER_Notice("Polarization        : Left_hand\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_R:
                        LOADER_Notice("Polarization        : Right_hand\n");
                        break;
                    default:
                        LOADER_Notice("Polarization        : Invalid Paramater! \n");
                        break;
                }

                LOADER_Notice("ScrambleValue       : %u\n", pstConnectParam->u32ScrambleValue);
                switch (pstSatParam->enLNBPower)
                {
                    case HI_UNF_TUNER_FE_LNB_POWER_OFF:
                        LOADER_Notice("LNB Power           : Power Off\n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_POWER_ON:
                        LOADER_Notice("LNB Power           : Power On 13V/18V\n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_POWER_ENHANCED:
                        LOADER_Notice("LNB Power           : Power On 14V/19V\n");
                        break;
                    default:
                        LOADER_Notice("LNB Power           : Invalid Paramater! \n");
                        break;
                }

                switch (pstSatParam->stLNBConfig.enLNBType)
                {
                    case HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY:
                        LOADER_Notice("LNB type            : Single LO frequency \n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY:
                        LOADER_Notice("LNB type            : Dual LO frequency \n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_UNICABLE:
                        LOADER_Notice("LNB type            : Unicable LNB \n");
                        break;
                    default:
                        LOADER_Notice("LNB type            : Invalid Paramater! \n");
                        break;
                }

                LOADER_Notice("LNB Low Frequency   : %d(MHZ)\n", pstSatParam->stLNBConfig.u32LowLO);
                LOADER_Notice("LNB High Frequency  : %d(MHZ)\n", pstSatParam->stLNBConfig.u32HighLO);

                switch (pstSatParam->stLNBConfig.enLNBBand)
                {
                    case HI_UNF_TUNER_FE_LNB_BAND_C:
                        LOADER_Notice("LNB BandType        : C \n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_BAND_KU:
                        LOADER_Notice("LNB BandType        : Ku \n");
                        break;
                    default:
                        LOADER_Notice("LNB BandType        : Invalid Paramater! \n");
                        break;
                }
                LOADER_Notice("SCR number          : %u(0~7)\n", pstSatParam->stLNBConfig.u8UNIC_SCRNO);
                LOADER_Notice("SCR IF frequency    : %u(MHZ)\n", pstSatParam->stLNBConfig.u32UNICIFFreqMHz);

                switch (pstSatParam->stLNBConfig.enSatPosn)
                {
                    case HI_UNF_TUNER_SATPOSN_A:
                        LOADER_Notice("Unicable switch port: A \n");
                        break;
                    case HI_UNF_TUNER_SATPOSN_B:
                        LOADER_Notice("Unicable switch port: B \n");
                        break;
                    default:
                        LOADER_Notice("Unicable switch port: Invalid Paramater! \n");
                        break;
                }

                switch (pstSatParam->enSwitch22K)
                {
                    case HI_UNF_TUNER_SWITCH_22K_NONE:
                        LOADER_Notice("22K Switch          : none\n");
                        break;
                    case HI_UNF_TUNER_SWITCH_22K_0:
                        LOADER_Notice("22K Switch          : 0 KHz port\n");
                        break;
                    case HI_UNF_TUNER_SWITCH_22K_22:
                        LOADER_Notice("22K Switch          : 22 KHz port\n");
                        break;
                    default:
                        LOADER_Notice("22K Switch          : Invalid Paramater\n");
                        break;
                }

                if (HI_UNF_TUNER_DISEQC_SWITCH_NONE == pstSatParam->stSwitch16Port.enPort)
                {
                    LOADER_Notice("DiSEqC1.1           : SWITCH PORT Disable\n");
                }
                else if (HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT > pstSatParam->stSwitch16Port.enPort)
                {
                    LOADER_Notice("DiSEqC1.1           : SWITCH PORT %d\n", pstSatParam->stSwitch16Port.enPort);
                }
                else
                {
                    LOADER_Notice("DiSEqC1.1           : SWITCH PORT Invalid Paramater \n");
                }

                switch (pstSatParam->stSwitch16Port.enLevel)
                {
                    case HI_UNF_TUNER_DISEQC_LEVEL_1_X:
                        LOADER_Notice("DiSEqC1.1           : Level 1.x \n");
                        break;
                    case HI_UNF_TUNER_DISEQC_LEVEL_2_X:
                        LOADER_Notice("DiSEqC1.1           : Level 2.x \n");
                        break;
                    default:
                        LOADER_Notice("DiSEqC1.1           : Level Invalid Paramater \n");
                        break;
                }

                if (HI_UNF_TUNER_DISEQC_SWITCH_NONE == pstSatParam->stSwitch4Port.enPort)
                {
                    LOADER_Notice("DiSEqC1.0           : SWITCH PORT Disable\n");
                }
                else if (HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT > pstSatParam->stSwitch4Port.enPort)
                {
                    LOADER_Notice("DiSEqC1.0           : SWITCH PORT %d\n", pstSatParam->stSwitch4Port.enPort);
                }
                else
                {
                    LOADER_Notice("DiSEqC1.0           : Invalid Paramater \n");
                }

                switch (pstSatParam->stSwitch4Port.enPolar)
                {
                    case HI_UNF_TUNER_FE_POLARIZATION_H:
                        LOADER_Notice("DiSEqC1.0           : Polarization Horizontal\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_V:
                        LOADER_Notice("DiSEqC1.0           : Polarization Vertical\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_L:
                        LOADER_Notice("DiSEqC1.0           : Polarization Left_hand\n");
                        break;
                    case HI_UNF_TUNER_FE_POLARIZATION_R:
                        LOADER_Notice("DiSEqC1.0           : Polarization Right_hand\n");
                        break;
                    default:
                        LOADER_Notice("DiSEqC1.0           : Polarization Invalid Paramater\n");
                        break;
                }

                switch (pstSatParam->stSwitch4Port.enLNB22K)
                {
                    case HI_UNF_TUNER_FE_LNB_22K_OFF:
                        LOADER_Notice("DiSEqC1.0           : 22K Switch off\n");
                        break;
                    case HI_UNF_TUNER_FE_LNB_22K_ON:
                        LOADER_Notice("DiSEqC1.0           : 22K Switch on\n");
                        break;
                    default:
                        LOADER_Notice("DiSEqC1.0           : 22K Switch Invalid Paramater\n");
                        break;
                }

                switch (pstSatParam->enMotorType)
                {
                    case HI_LOADER_MOTORTYPE_NONE:
                        LOADER_Notice("MotorType           : none\n");
                        break;
                    case HI_LOADER_MOTORTYPE_DISEQC12:
                        LOADER_Notice("MotorType           : DiSEqC1.2 protocol \n");
                        break;
                    case HI_LOADER_MOTORTYPE_USLAS:
                        LOADER_Notice("MotorType           : USLAS protocol \n");
                        break;
                    default:
                        LOADER_Notice("MotorType           : Invalid Paramater\n");
                        break;
                }

                LOADER_Notice("USALS LocalLongitude: %d\n", pstSatParam->stUSALSParam.u16LocalLongitude);
                LOADER_Notice("USALS LocalLatitude : %d\n", pstSatParam->stUSALSParam.u16LocalLatitude);
                LOADER_Notice("USALS SatLongitude  : %d\n", pstSatParam->stUSALSParam.u16SatLongitude);
                LOADER_Notice("USALS Angular       : %d\n", pstSatParam->stUSALSParam.u16Angular);

                switch (pstSatParam->stPosition.enLevel)
                {
                    case HI_UNF_TUNER_DISEQC_LEVEL_1_X:
                        LOADER_Notice("Position Level      : Level 1.x \n");
                        break;
                    case HI_UNF_TUNER_DISEQC_LEVEL_2_X:
                        LOADER_Notice("Position Level      : Level 2.x \n");
                        break;
                    default:
                        LOADER_Notice("Position Level      : Level Invalid Paramater \n");
                        break;
                }

                LOADER_Notice("Position->Pos       : %d\n", pstSatParam->stPosition.u32Pos);

            }

            break;
        }

        case HI_LOADER_UPGRADE_TYPE_IP:
        {
            HI_CHAR as8Buff[16];
            HI_U32 ip;
            char* protocol[] =
            {
                "TFTP",
                "FTP",
                "HTTP"
            };

            char* iptype[] =
            {
                "static ip",
                "dynamic"
            };

            LOADER_Notice("Download type       : IP\n");
            LOADER_Notice("Protocol type       : %s\n", protocol[stLoaderInfo.unParam.stIPParam.enProtType]);
            if (HI_LOADER_IPPROT_FTP == stLoaderInfo.unParam.stIPParam.enProtType)
            {
                LOADER_Notice("User Name           : %s\n", stLoaderInfo.unParam.stIPParam.as8UserName);
                LOADER_Notice("Password            : %s\n", stLoaderInfo.unParam.stIPParam.as8Password);
            }

            LOADER_Notice("IP type             : %s\n", iptype[stLoaderInfo.unParam.stIPParam.enIPCfgType]);

            if (HI_LOADER_IPCFG_STATIC == stLoaderInfo.unParam.stIPParam.enIPCfgType)
            {
                ip = stLoaderInfo.unParam.stIPParam.ipOurIp;
                memset(as8Buff, 0, sizeof(as8Buff));
                sprintf(as8Buff, "%d.%d.%d.%d", (int)((ip) & 0xff), (int)((ip >> 8) & 0xff), \
                              (int)((ip >> 16) & 0xff), (int)((ip >> 24) & 0xff));
                LOADER_Notice("Local IP            : %s\n", as8Buff);

                ip = stLoaderInfo.unParam.stIPParam.ipGateway;
                memset(as8Buff, 0, sizeof(as8Buff));
                sprintf(as8Buff, "%d.%d.%d.%d", (int)((ip) & 0xff), (int)((ip >> 8) & 0xff), \
                              (int)((ip >> 16) & 0xff), (int)((ip >> 24) & 0xff));
                LOADER_Notice("Gateway             : %s\n", as8Buff);

                ip = stLoaderInfo.unParam.stIPParam.ipSubnetMask;
                memset(as8Buff, 0, sizeof(as8Buff));
                sprintf(as8Buff, "%d.%d.%d.%d", (int)((ip) & 0xff), (int)((ip >> 8) & 0xff), \
                              (int)((ip >> 16) & 0xff), (int)((ip >> 24) & 0xff));
                LOADER_Notice("subnet mask         : %s\n", as8Buff);
            }

            ip = stLoaderInfo.unParam.stIPParam.ipServer;
            memset(as8Buff, 0, sizeof(as8Buff));
            sprintf(as8Buff, "%d.%d.%d.%d", (int)((ip) & 0xff), (int)((ip >> 8) & 0xff), \
                          (int)((ip >> 16) & 0xff), (int)((ip >> 24) & 0xff));
            LOADER_Notice("server ip           : %s\n", as8Buff);
            LOADER_Notice("server port         : %d\n", stLoaderInfo.unParam.stIPParam.ipServerPort);

            LOADER_Notice("Upgrade file name   : %s\n", stLoaderInfo.unParam.stIPParam.as8FileName);
            //LOADER_Notice("Upgrade file size   : %d\n", stLoaderInfo.unParam.stIPParam.u32FileSize);

            break;
        }

        default:
            break;
    }

    LOADER_Notice("========================================================\n");
}

HI_S32 TriggerLoaderUpgrd(HI_VOID)
{
    HI_U32 u32Tmp = 0;
    HI_S32 s32Ret = HI_FAILURE;

    HI_LOADER_PARAM_S stLoaderInfo;

    memset(&stLoaderInfo, 0x00, sizeof(stLoaderInfo));
    LOADER_CHECK(HI_LOADER_GetParameter(&stLoaderInfo));

    LOADER_Notice("=========== Upgrade Parameter Configuration ============\n");
    LOADER_Notice("Select upgrade mode(0-Basic, 1-Mandatory, 2-Manual, 3-EmptyFlashBurn)");
    MODIFY_NUMBER(u32Tmp);
    LOADER_CHECK_INPUTPARAM(3 < u32Tmp);

    if (2 == u32Tmp)
    {
        LDDB_LOADER_INFO_S stInfoParam;

        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        LOADER_CHECK(LDDB_GetLoaderInfo(&stInfoParam));

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bForceUpgrade = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }

        LOADER_Notice("\n");
        LOADER_Notice("Manual USB upgrade has been triggered,\n");
        LOADER_Notice("Please make sure the upgrade file named usb_update.bin \n");
        LOADER_Notice("has been put into the USB device, and the USB device\n");
        LOADER_Notice("is connected properly. \n");
        LOADER_Notice("========================================================\n");

        return HI_SUCCESS;
    }

    if (3 == u32Tmp)
    {
        LDDB_LOADER_INFO_S stInfoParam;

        memset(&stInfoParam, 0x00, sizeof(stInfoParam));
        LOADER_CHECK(LDDB_GetLoaderInfo(&stInfoParam));

        stInfoParam.stTAGParamData.bNeedUpgrade = HI_TRUE;
        stInfoParam.stTAGParamData.bUsbBurn = HI_TRUE;
        s32Ret = LDDB_SetLoaderInfo(&stInfoParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(LDDB_SetLoaderInfo, s32Ret);
            return s32Ret;
        }

        LOADER_Notice("\n");
        LOADER_Notice("Empty Flash Burn upgrade has have been triggered,\n");
        LOADER_Notice("Please make sure the upgrade file named usb_update.bin \n");
        LOADER_Notice("has been put into the USB device, and the USB device\n");
        LOADER_Notice("is connected properly. \n");
        LOADER_Notice("========================================================\n");

        return HI_SUCCESS;
    }

    stLoaderInfo.enUpgradeMode = u32Tmp;

    LOADER_Notice("Select upgrade type(0-OTA, 1-IP, 2-USB, 3-FS, 4-Reset)");
    MODIFY_NUMBER(stLoaderInfo.enUpgradeType);
    LOADER_CHECK_INPUTPARAM(HI_LOADER_UPGRADE_TYPE_BUTT <= stLoaderInfo.enUpgradeType);

    if (HI_LOADER_UPGRADE_TYPE_OTA == stLoaderInfo.enUpgradeType)
    {
        LOADER_Notice("Select signal type(0-CAB, 1-SAT, 2-DVB-T, 3-DVB-T2)");
        MODIFY_NUMBER(u32Tmp);
        LOADER_CHECK_INPUTPARAM(3 < u32Tmp);

        if (0 == u32Tmp)
        {
            stLoaderInfo.unParam.stOTAParam.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
            s32Ret = OTA_Cable(&(stLoaderInfo.unParam.stOTAParam));
            if (HI_SUCCESS != s32Ret)
            {
                LOADER_PrintFuncErr(OTA_Cable, s32Ret);
                return s32Ret;
            }
        }
        else if (1 == u32Tmp)
        {
            stLoaderInfo.unParam.stOTAParam.enSigType = HI_UNF_TUNER_SIG_TYPE_SAT;
            s32Ret = OTA_Satellite(&(stLoaderInfo.unParam.stOTAParam));
            if (HI_SUCCESS != s32Ret)
            {
                LOADER_PrintFuncErr(OTA_Satellite, s32Ret);
                return s32Ret;
            }
        }
        else if (2 == u32Tmp)
        {
            stLoaderInfo.unParam.stOTAParam.enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T;
            s32Ret = OTA_Terrestrial(&(stLoaderInfo.unParam.stOTAParam));
            if (HI_SUCCESS != s32Ret)
            {
                LOADER_PrintFuncErr(OTA_Terrestrial, s32Ret);
                return s32Ret;
            }
        }
        else if (3 == u32Tmp)
        {
            stLoaderInfo.unParam.stOTAParam.enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T2;
            s32Ret = OTA_Terrestrial(&(stLoaderInfo.unParam.stOTAParam));
            if (HI_SUCCESS != s32Ret)
            {
                LOADER_PrintFuncErr(OTA_Terrestrial, s32Ret);
                return s32Ret;
            }
        }
    }
    else if (HI_LOADER_UPGRADE_TYPE_IP == stLoaderInfo.enUpgradeType)
    {
        s32Ret = IP(&stLoaderInfo.unParam.stIPParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(IP, s32Ret);
            return s32Ret;
        }
    }
    else if (HI_LOADER_UPGRADE_TYPE_USB == stLoaderInfo.enUpgradeType)
    {
        s32Ret = USB(&stLoaderInfo.unParam.stUSBParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(USB, s32Ret);
            return s32Ret;
        }
    }
    else if (HI_LOADER_UPGRADE_TYPE_FS == stLoaderInfo.enUpgradeType)
    {
        s32Ret = FS(&stLoaderInfo.unParam.stFSParam);
        if (HI_SUCCESS != s32Ret)
        {
            LOADER_PrintFuncErr(FS, s32Ret);
            return s32Ret;
        }
    }
    else
    {
        /** Do Nothing!*/
    }

    s32Ret = HI_LOADER_SetParameter(&stLoaderInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_LOADER_SetParameter, s32Ret);
        return s32Ret;
    }

    ShowSelect();

    return HI_SUCCESS;
}

static HI_S32 UpdateDeviceInfo(LDDB_DEVICE_INFO_S* pstDeviceInfo)
{
    HI_U32 u32FlashSize = 0;
    HI_CHAR ac8TmpBuf[LOADER_D_DEVICEITEM_LEN] = {0};
    HI_CHAR* pcFlashData = HI_NULL_PTR;
    HI_CHAR* pcWritePosition = HI_NULL_PTR;
    HI_CHAR* pcLengthTagPos = HI_NULL_PTR;
    LDDB_HANDLE hFlashHandle = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = LDDB_FlashOpen(LOADER_D_FLASHMAP_DEVICEINFO, &hFlashHandle, &u32FlashSize);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LDDB_FlashOpen, s32Ret);
        return s32Ret;
    }

    pcFlashData = malloc(u32FlashSize);
    if (HI_NULL_PTR == pcFlashData)
    {
        LOADER_PrintErrInfo("No memory resources!");
        LOADER_CHECK(LDDB_FlashClose(hFlashHandle));
        return -1;
    }

    memset(pcFlashData, 0x00, u32FlashSize);
    pcWritePosition = pcFlashData;

    /* reserve MAC , but unused for loader */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "%s", pstDeviceInfo->au8MacAddress);
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* SN */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "%08x", pstDeviceInfo->u32SerialNum);
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* MAGICNUM */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "MAGICNUM=%s", LOADER_D_FLASHMAP_DEVICEINFO);
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* reserve LENGTH(LENGTH=xxxxxxxx) space */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "LENGTH=12345678");
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcLengthTagPos = pcWritePosition ;
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* OUI */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "OUI=%08x", pstDeviceInfo->u32OUI);
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* HW */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "HW=%08x", pstDeviceInfo->u32HardwareVersion);
    strncpy(pcWritePosition, ac8TmpBuf, strlen(ac8TmpBuf) + 1);
    pcWritePosition += strlen(ac8TmpBuf) + 1; /* skip \0 */

    /* update LENGTH */
    snprintf(ac8TmpBuf, sizeof(ac8TmpBuf), "LENGTH=%08x", (HI_U32)(pcWritePosition - pcFlashData));
    strncpy(pcLengthTagPos, ac8TmpBuf, strlen(ac8TmpBuf) + 1);

    if (u32FlashSize < (pcWritePosition - pcFlashData))
    {
        LOADER_PrintErrInfo("There is not enough FLASH space!");
        LOADER_CHECK(LDDB_FlashClose(hFlashHandle));
        return -1;
    }

    s32Ret = LDDB_FlashWrite(hFlashHandle, (HI_U8*)pcFlashData, u32FlashSize);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LDDB_FlashWrite, s32Ret);

        free(pcFlashData);
        pcFlashData = HI_NULL_PTR;

        LOADER_CHECK(LDDB_FlashClose(hFlashHandle));
        hFlashHandle = HI_NULL_PTR;

        return s32Ret;
    }

    free(pcFlashData);
    pcFlashData = HI_NULL_PTR;

    LOADER_CHECK(LDDB_FlashClose(hFlashHandle));
    hFlashHandle = HI_NULL_PTR;

    return HI_SUCCESS;
}

HI_S32 SetDeviceInfo()
{
    LDDB_DEVICE_INFO_S stDeviceInfo;
    HI_S32 s32Ret = HI_FAILURE;

    memset(&stDeviceInfo, 0x00, sizeof(stDeviceInfo));
    s32Ret = LDDB_GetDeviceInfo(&stDeviceInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LDDB_GetDeviceInfo, s32Ret);
    }

    LOADER_Notice("================= Modify Device Info ===================\n");
    LOADER_Notice("Please input new Manufacturer Identification[0x%08x]", stDeviceInfo.u32OUI);
    MODIFY_NUMBER(stDeviceInfo.u32OUI);

    LOADER_Notice("Please input new Hardware Version[0x%08x]", stDeviceInfo.u32HardwareVersion);
    MODIFY_NUMBER(stDeviceInfo.u32HardwareVersion);

    LOADER_Notice("Please input new Serial Number[0x%08x]", stDeviceInfo.u32SerialNum);
    MODIFY_NUMBER(stDeviceInfo.u32SerialNum);

    s32Ret = UpdateDeviceInfo(&stDeviceInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(UpdateDeviceInfo, s32Ret);
        LOADER_Notice("\n");
		LOADER_Notice("Device Information modify failure! \n");
        LOADER_Notice("========================================================\n");
        return s32Ret;
    }

    LOADER_Notice("\n");
    LOADER_Notice("Device Information modify success! \n");
    LOADER_Notice("New Manufacturer Identification: 0x%08x\n", stDeviceInfo.u32OUI);
    LOADER_Notice("New Area Code:                   0x%08x\n", stDeviceInfo.u32AreaCode);
    LOADER_Notice("New Hardware Version:            0x%08x\n", stDeviceInfo.u32HardwareVersion);
    LOADER_Notice("New Serial Number                0x%08x\n", stDeviceInfo.u32SerialNum);
    LOADER_Notice("========================================================\n");

    return HI_SUCCESS;
}

HI_S32 GetDeviceInfo(HI_VOID)
{
    HI_LOADER_DEVICEINFO_S stDeviceInfo;
    HI_S32 s32Ret = HI_FAILURE;

    memset(&stDeviceInfo, 0x00, sizeof(stDeviceInfo));
    s32Ret = HI_LOADER_GetDeviceInfo(&stDeviceInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_LOADER_GetDeviceInfo, s32Ret);
        return s32Ret;
    }

    LOADER_Notice("================== Get Device Info =====================\n");
    LOADER_Notice("Manufacturer Identification: 0x%08x\n", stDeviceInfo.u32OUI);
    LOADER_Notice("Area Code:                   0x%08x\n", stDeviceInfo.u32AreaCode);
    LOADER_Notice("Hardware Version:            0x%08x\n", stDeviceInfo.u32HWVersion);
    LOADER_Notice("Serial Number                0x%08x\n", stDeviceInfo.u32HWSerialNum);
    LOADER_Notice("========================================================\n");

    return HI_SUCCESS;
}

HI_S32 SetSoftwareVersion(HI_CHAR* args)
{
    LDDB_SOFTWARE_INFO_S stSoftwareInfo;
    HI_U32 u32SoftwareVersion;
    HI_S32 s32Ret = HI_FAILURE;

    memset(&stSoftwareInfo, 0x00, sizeof(stSoftwareInfo));
    LOADER_CHECK(LDDB_GetSoftwareInfo(&stSoftwareInfo));

    LOADER_Notice("============== modify software version =================\n");
    LOADER_Notice("Please input new software version[0x%08x]", stSoftwareInfo.u32SoftwareVersion);

    if (0 != get_input_number(&u32SoftwareVersion))
    {
        LOADER_Notice("\n");
        LOADER_Notice("Software version modify failure! \n");
        LOADER_Notice("The current version number is 0x%08x \n", stSoftwareInfo.u32SoftwareVersion);
        LOADER_Notice("========================================================\n");
        LOADER_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));
        return HI_SUCCESS;
    }

    stSoftwareInfo.u32SoftwareVersion = u32SoftwareVersion;
    s32Ret = LDDB_SetSoftwareInfo(&stSoftwareInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LDDB_SetSoftwareInfo, s32Ret);
        LOADER_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));
        return s32Ret;
    }

    LOADER_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));
    memset(&stSoftwareInfo, 0x00, sizeof(stSoftwareInfo));
    s32Ret = LDDB_GetSoftwareInfo(&stSoftwareInfo);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(LDDB_GetSoftwareInfo, s32Ret);
        return s32Ret;
    }

    LOADER_Notice("\n");
    LOADER_Notice("Software version modify success! \n");
    LOADER_Notice("The current version number is 0x%08x \n", stSoftwareInfo.u32SoftwareVersion);
    LOADER_Notice("========================================================\n");

    LOADER_CHECK(LDDB_FreeSoftwareInfo(&stSoftwareInfo));

    return HI_SUCCESS;
}

HI_S32 GetSoftwareVersion(HI_VOID)
{
    HI_U32 u32CurSoftwareVersion;
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = HI_LOADER_GetSoftwareVersion(&u32CurSoftwareVersion);
    if (HI_SUCCESS != s32Ret)
    {
        LOADER_PrintFuncErr(HI_LOADER_GetSoftwareVersion, s32Ret);
        return s32Ret;
    }

    LOADER_Notice("================ Get software version ==================\n");
    LOADER_Notice("The current version number is 0x%08x \n", u32CurSoftwareVersion);
    LOADER_Notice("========================================================\n");

    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    int result = 0;

    HI_SYS_Init();

    while ( (result = getopt(argc, argv, "ts:g:")) != -1 )
    {
        switch (result)
        {
            case 't':
            {
                TriggerLoaderUpgrd();
                return 0;
            }
            case 's':
            {
                if (!strcmp("deviceinfo", optarg))
                {
                    SetDeviceInfo();
                    return 0;
                }
                else if (!strcmp("sw", optarg))
                {
                    SetSoftwareVersion(argv[optind]);
                    return 0;
                }
                break;
            }
            case 'g':
            {
                if (!strcmp("deviceinfo", optarg))
                {
                    GetDeviceInfo();
                    return 0;
                }
                else if (!strcmp("sw", optarg))
                {
                    GetSoftwareVersion();
                    return 0;
                }
                else if (!strcmp("upgradeinfo", optarg))
                {
                    ShowSelect();
                    return 0;
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }

    ShowUsageInfo();
    return -1;
}

