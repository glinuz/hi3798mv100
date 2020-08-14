#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/miscdevice.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "hi_type.h"

#include "drv_i2c_ext.h"
#include "hi_drv_file.h"

#include "drv_gpioi2c_ext.h"
#include "drv_gpio_ext.h"
#include "hi_drv_gpioi2c.h"
#include "hi_debug.h"


#include "hi_drv_stat.h"
#include "hi_drv_dev.h"
#include "hi_drv_reg.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_drv_mmz.h"

#include "drv_tuner_ext.h"
#include "drv_tuner_ioctl.h"
#include "hi_drv_diseqc.h"
#include "drv_demod.h"
#include "drv_common.h"

#include "hi3137.h"


extern DISEQC_DRV_STATUS_S s_stDiSEqCDrvStatus[TUNER_NUM];


extern struct file *TunerSpectrumHandle[TUNER_NUM];

struct file *TunerConstellationHandle[TUNER_NUM]  __attribute__((unused)) = {HI_NULL};




#if (1 == HI_PROC_SUPPORT)

HI_S32 str2int(HI_CHAR *str,HI_U32 *pu32Int)
{
    HI_U32 i = 0;
    HI_S32 s32Temp = 0,s32Dec = 0;

    while(str[i] != '\0')
    {
        s32Temp = str[i] - '0';
        if(s32Temp < 0 || s32Temp > 9)
        {
            return HI_FAILURE;
        }

        s32Dec = s32Dec*10+s32Temp;
        i++;
    }

    *pu32Int = (HI_U32)s32Dec;
    return HI_SUCCESS;
}

HI_S32 tuner_proc_read_reg(struct seq_file * p, HI_VOID * v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_U32 u32TunerPort;

    PROC_PRINT(p,"---------Hisilicon TUNER Reg Info---------\n");

    for ( u32TunerPort = 0; u32TunerPort < TUNER_NUM; u32TunerPort++ )
    {
        if ( HI_NULL == g_stTunerOps[u32TunerPort].tuner_connect )
        {
            continue;
        }

        if (g_stTunerOps[u32TunerPort].tuner_get_registers)
        {
            g_stTunerOps[u32TunerPort].tuner_get_registers(u32TunerPort, p);
        }
    }
#endif
    return HI_SUCCESS;
}

static HI_VOID tuner_proc_get_signal_type(HI_UNF_TUNER_SIG_TYPE_E enSigType, HI_CHAR acSignalType[], HI_U32 u32ArrayLen)
{
    switch(enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_CAB:
            strncpy(acSignalType, "DVBC ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_SAT:
            strncpy(acSignalType, "SAT ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            strncpy(acSignalType, "DVBT ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            strncpy(acSignalType, "DVBT2 ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
            strncpy(acSignalType, "ISDBT ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_ATSC_T:
            strncpy(acSignalType, "ATSCT ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_DTMB:
            strncpy(acSignalType, "DTMB ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_J83B:
            strncpy(acSignalType, "J83B ", u32ArrayLen);
            break;
        case HI_UNF_TUNER_SIG_TYPE_ABSS:
            strncpy(acSignalType, "ABSS ", u32ArrayLen);
            break;
        default:
            strncpy(acSignalType, "Unknow ", u32ArrayLen);
            break;
    }
    acSignalType[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_mod_type_c(TUNER_QAM_TYPE_E enQamType, HI_CHAR acQamType[], HI_U32 u32ArrayLen)
{
    switch(enQamType)
    {
        case QAM_TYPE_16:
            strncpy(acQamType, "QAM_16 ", u32ArrayLen);
            break;
        case QAM_TYPE_32:
            strncpy(acQamType, "QAM_32 ", u32ArrayLen);
            break;
        case QAM_TYPE_64:
            strncpy(acQamType, "QAM_64 ", u32ArrayLen);
            break;
        case QAM_TYPE_128:
            strncpy(acQamType, "QAM_128", u32ArrayLen);
            break;
        case QAM_TYPE_256:
            strncpy(acQamType, "QAM_256", u32ArrayLen);
            break;
        default :
            strncpy(acQamType, "QAM_INVALID ", u32ArrayLen);
    }
}

static HI_VOID tuner_proc_get_mod_type_s(HI_UNF_MODULATION_TYPE_E enModType, HI_CHAR acQamType[], HI_U32 u32ArrayLen)
{
    switch (enModType)
    {
        case HI_UNF_MOD_TYPE_QPSK:
            strncpy(acQamType, "QPSK ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_8PSK:
            strncpy(acQamType, "8PSK ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_16APSK:
            strncpy(acQamType, "16APSK ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_32APSK:
            strncpy(acQamType, "32APSK", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_DEFAULT:
        case HI_UNF_MOD_TYPE_QAM_16:
        case HI_UNF_MOD_TYPE_QAM_32:
        case HI_UNF_MOD_TYPE_QAM_64:
        case HI_UNF_MOD_TYPE_QAM_128:
        case HI_UNF_MOD_TYPE_QAM_256:
        case HI_UNF_MOD_TYPE_QAM_512:
        default:
            strncpy(acQamType, "QAM_INVALID ", u32ArrayLen);
    }
}

static HI_VOID tuner_proc_get_mod_type_t(HI_UNF_MODULATION_TYPE_E enModType, HI_CHAR acQamType[], HI_U32 u32ArrayLen)
{
    switch (enModType)
    {
        case HI_UNF_MOD_TYPE_QPSK:
            strncpy(acQamType, "QPSK ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_DQPSK:
            strncpy(acQamType, "DQPSK ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_16:
            strncpy(acQamType, "QAM_16 ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_32:
            strncpy(acQamType, "QAM_32 ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_64:
            strncpy(acQamType, "QAM_64 ", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_256:
            strncpy(acQamType, "QAM_256 ", u32ArrayLen);
            break;
        default:
            strncpy(acQamType, "QAM_INVALID ", u32ArrayLen);
            break;
    }
}

static HI_VOID tuner_proc_get_mod_type_isdbt(HI_UNF_MODULATION_TYPE_E enLayerModType, HI_CHAR acQamType[], HI_U32 u32ArrayLen)
{
    switch (enLayerModType)
    {
        case HI_UNF_MOD_TYPE_QPSK:
            strncpy(acQamType, "QPSK", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_16:
            strncpy(acQamType, "QAM_16", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_64:
            strncpy(acQamType, "QAM_64", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_128:
            strncpy(acQamType, "QAM_128", u32ArrayLen);
            break;
        case HI_UNF_MOD_TYPE_QAM_256:
            strncpy(acQamType, "QAM_256", u32ArrayLen);
            break;
        default:
            strncpy(acQamType, "INVALID", u32ArrayLen);
            break;
    }
   
    acQamType[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_mod_type(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstSignalInfo, HI_CHAR acQamType[], HI_U32 u32ArrayLen)
{
    if ((HI_UNF_DEMOD_DEV_TYPE_NONE == g_stTunerOps[u32TunerPort].enDemodDevType)
        || (HI_UNF_DEMOD_DEV_TYPE_BUTT == g_stTunerOps[u32TunerPort].enDemodDevType))
    {
        acQamType[u32ArrayLen - 1] = '\0';
        return;
    }

    if ((HI_UNF_TUNER_SIG_TYPE_CAB == pstSignalInfo->enSigType)     /* For cable */
        || (HI_UNF_TUNER_SIG_TYPE_J83B == pstSignalInfo->enSigType))
    {
        tuner_proc_get_mod_type_c(g_stTunerOps[u32TunerPort].stCurrPara.enQamType, acQamType, u32ArrayLen);
    }
    else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstSignalInfo->enSigType)   /* For satellite */
    {
        tuner_proc_get_mod_type_s(pstSignalInfo->unSignalInfo.stSat.enModType, acQamType, u32ArrayLen);
    }
    else if ((HI_UNF_TUNER_SIG_TYPE_DVB_T == pstSignalInfo->enSigType)
        || (HI_UNF_TUNER_SIG_TYPE_DVB_T2 == pstSignalInfo->enSigType)  
        || (HI_UNF_TUNER_SIG_TYPE_DTMB == pstSignalInfo->enSigType))  /*for terrestrial*/
    {
        tuner_proc_get_mod_type_t(pstSignalInfo->unSignalInfo.stTer.enModType, acQamType, u32ArrayLen);
    }
    else if (HI_UNF_TUNER_SIG_TYPE_ISDB_T == pstSignalInfo->enSigType)
    {
        /*ISDB-T有三层，此处显示为QAM_INVALID，下面会打印各层的信息*/
    }
    
    acQamType[u32ArrayLen - 1] = '\0';
    return;
}

static HI_VOID tuner_proc_get_demod_type(HI_U32 u32TunerPort, HI_CHAR acDemodType[], HI_U32 u32ArrayLen)
{
    switch( g_stTunerOps[u32TunerPort].enDemodDevType )
    {
        case HI_UNF_DEMOD_DEV_TYPE_3130I:
            strncpy(acDemodType, "3130_inside", u32ArrayLen );
            break;
        case HI_UNF_DEMOD_DEV_TYPE_3130E:
            strncpy(acDemodType, "3130_outside", u32ArrayLen );
            break;
        case HI_UNF_DEMOD_DEV_TYPE_J83B:
            strncpy(acDemodType, "j83b", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
            strncpy(acDemodType, "avl6211", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MXL101:
            strncpy(acDemodType, "mxl101", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MN88472:
            strncpy(acDemodType, "mn88472", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MN88473:
            strncpy(acDemodType, "mn88473", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_IT9170:
            strncpy(acDemodType, "it9170", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_IT9133:
            strncpy(acDemodType, "it9133", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_3136:
            strncpy(acDemodType, "hi3136e", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_CXD2837:
            strncpy(acDemodType, "cxd2837", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_3137:
            strncpy(acDemodType, "hi3137", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MXL254:
            strncpy(acDemodType, "mxl254", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MXL214:
            strncpy(acDemodType, "mxl214", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_TDA18280:
            strncpy(acDemodType, "tda18280", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_HIFDVBC100:
            strncpy(acDemodType, "FDVBC100", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_HIFJ83B100:
            strncpy(acDemodType, "FJ83B100", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_3138:
            strncpy(acDemodType, "hi3138", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_ATBM888X:
            strncpy(acDemodType, "atbm888x", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_MXL683:
            strncpy(acDemodType, "mxl683", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_TP5001:
            strncpy(acDemodType, "TP5001", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_HD2501:
            strncpy(acDemodType, "HD2501", u32ArrayLen);
            break;
        case HI_UNF_DEMOD_DEV_TYPE_AVL6381:
            strncpy(acDemodType, "AVL6381", u32ArrayLen);
            break;
        default:
            strncpy(acDemodType, "unknown demod", u32ArrayLen );
            break;
    }
    
    acDemodType[u32ArrayLen - 1] = '\0';
}


static HI_VOID tuner_proc_get_tuner_type(HI_U32 u32TunerPort, HI_CHAR acTunerType[], HI_U32 u32ArrayLen)
{
    HI_U32 u32NameId = 0;   
    HI_U32 u32NameNum = 0;  //tuner设备的数目
    HI_CHAR *acTunerName[] = {"XG_3BL", "CD1616", "ALPS_TDAE", "TDCC", "TDA18250",   "CD1616_DOUBLE", "MT2081", "TMX7070X", "R820C", "MXL203",
                              "AV2011", "SHARP7903", "MXL101", "MXL603", "IT9170",   "IT9133", "TDA6651", "TDA18250B", "M88TS2022", "RDA5815",
                              "MXL254", "cxd2861", "si2147", "Rafael836", "MXL608",  "MXL214", "TDA18280", "TDA182I5A", "SI2144", "AV2018",
                              "MXL251", "M88TC3800", "MXL601", "MXL683", "AV2026",   "unknown TUNER"};


    u32NameId = g_stTunerOps[u32TunerPort].enTunerDevType;
    u32NameNum = sizeof(acTunerName) / sizeof(acTunerName[0]) - 1;  //最后一个"unkown TUNER"不算在内

    if (u32NameId < u32NameNum)
    {
        strncpy(acTunerType, acTunerName[u32NameId], u32ArrayLen);
    }
    else
    {
        strncpy(acTunerType, acTunerName[u32NameNum], u32ArrayLen);
    }    

    acTunerType[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_snr(HI_U32 u32TunerPort, HI_UNF_TUNER_SIG_TYPE_E enSigType, HI_U32 u32SNR, 
                                    HI_S32 *ps32SnrLgValue, HI_S32 *ps32SnrInteger, HI_S32 *ps32SnrFraction)
{
    HI_S32 s32SnrLgValue = -1;
    HI_S32 s32SnrInteger = 0;
    HI_S32 s32SnrFraction = 0;
    
        
    if (HI_UNF_TUNER_SIG_TYPE_DVB_T == enSigType
            || HI_UNF_TUNER_SIG_TYPE_DVB_T2 == enSigType
            || HI_UNF_TUNER_SIG_TYPE_ISDB_T == enSigType
            || HI_UNF_TUNER_SIG_TYPE_DTMB == enSigType)
    {
        if (HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType
            || HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
        {
            s32SnrLgValue = (HI_S32)tuner_get_100lg_value(u32SNR) - 117;

            if(s32SnrLgValue < 180)
                ;//s32SnrLgValue = s32SnrLgValue;
            else if(s32SnrLgValue < 200)
                s32SnrLgValue += 1;
            else if(s32SnrLgValue < 230)
                s32SnrLgValue += 3;
            else if(s32SnrLgValue < 250)
                s32SnrLgValue += 5;
            else if(s32SnrLgValue < 260)
                s32SnrLgValue += 8;
            else
                s32SnrLgValue += 10;

            s32SnrInteger = s32SnrLgValue / 10;
            s32SnrFraction = abs(s32SnrLgValue % 10);
        }
    }
    else if (HI_UNF_TUNER_SIG_TYPE_SAT == enSigType)
    {
        if (HI_UNF_DEMOD_DEV_TYPE_3136 == g_stTunerOps[u32TunerPort].enDemodDevType
            || HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
        {
            if (u32SNR)
	        {
	            s32SnrLgValue = tuner_get_100lg_value(8192) - tuner_get_100lg_value(u32SNR);
	        }
	        else
	        {
                s32SnrLgValue = 500;
	        }
            
            s32SnrInteger = s32SnrLgValue / 10;
            s32SnrFraction = abs(s32SnrLgValue % 10);
        }
    }
    else if (HI_UNF_TUNER_SIG_TYPE_CAB== enSigType
        || HI_UNF_TUNER_SIG_TYPE_J83B== enSigType)
    {
        u32SNR = u32SNR / tuner_get_pow(2, 11);
        
        if (HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
        {
            switch (g_stTunerOps[u32TunerPort].stCurrPara.enQamType)
	        {
                case QAM_TYPE_16:           
                    s32SnrLgValue = tuner_get_100lg_value(161) - tuner_get_100lg_value(u32SNR);
                    break;

                case QAM_TYPE_32:
                    s32SnrLgValue = tuner_get_100lg_value(180) - tuner_get_100lg_value(u32SNR);
                    break;

                case QAM_TYPE_64:
                    s32SnrLgValue = tuner_get_100lg_value(168) - tuner_get_100lg_value(u32SNR);
                    break;

                case QAM_TYPE_128:
                    s32SnrLgValue = tuner_get_100lg_value(184) - tuner_get_100lg_value(u32SNR);
                    break;

                case QAM_TYPE_256:
                    s32SnrLgValue = tuner_get_100lg_value(170) - tuner_get_100lg_value(u32SNR);
                    break;

                default:
                    s32SnrLgValue = 0;
                    break;
            }
            
            s32SnrInteger = s32SnrLgValue / 10;
            s32SnrFraction = abs(s32SnrLgValue % 10);
        }
    }

    *ps32SnrLgValue = s32SnrLgValue;
    *ps32SnrInteger = s32SnrInteger;
    *ps32SnrFraction = s32SnrFraction;
}

static HI_VOID tuner_proc_get_fec_type_s(HI_UNF_TUNER_FE_FECTYPE_E enSATType, HI_CHAR acFECType[], HI_U32 u32ArrayLen)
{
    switch (enSATType)
    {
        case HI_UNF_TUNER_FE_DVBS:
            strncpy(acFECType, "DVBS", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_DVBS2:
            strncpy(acFECType, "DVBS2", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_DIRECTV:
            strncpy(acFECType, "DIRECTV", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_BUTT:
        default:
            strncpy(acFECType, "Unknown", u32ArrayLen);
            break;
    }

    acFECType[u32ArrayLen - 1] = '\0';
}


static HI_VOID tuner_proc_get_fec_rate_s(HI_UNF_TUNER_FE_FECRATE_E enFECRate, HI_CHAR acFECRate[], HI_U32 u32ArrayLen)
{
    switch (enFECRate)
    {
        case HI_UNF_TUNER_FE_FEC_1_2:
            strncpy(acFECRate, "1/2", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_1_3:
            strncpy(acFECRate, "1/3", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_2_3:
            strncpy(acFECRate, "2/3", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_4:
            strncpy(acFECRate, "3/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_4_5:
            strncpy(acFECRate, "4/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_5_6:
            strncpy(acFECRate, "5/6", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_6_7:
            strncpy(acFECRate, "6/7", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_7_8:
            strncpy(acFECRate, "7/8", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_8_9:
            strncpy(acFECRate, "8/9", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_9_10:
            strncpy(acFECRate, "9/10", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_1_4:
            strncpy(acFECRate, "1/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_2_5:
            strncpy(acFECRate, "2/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_5:
            strncpy(acFECRate, "3/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_AUTO:
            strncpy(acFECRate, "DUMMY", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FECRATE_BUTT:
    	default:
            strncpy(acFECRate, "UNKNOWN", u32ArrayLen);                       
            break;
    }

    acFECRate[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_ccm_mode(HI_UNF_TUNER_CODE_MODULATION_E enCodeModulation, HI_CHAR acCCMMode[], HI_U32 u32ArrayLen)
{
    switch(enCodeModulation)
    {
        case HI_UNF_TUNER_CODE_MODULATION_VCM_ACM:
            strncpy(acCCMMode, "VCM/ACM", u32ArrayLen);
            break;
        case HI_UNF_TUNER_CODE_MODULATION_MULTISTREAM:
            strncpy(acCCMMode, "MultiStream", u32ArrayLen);
            break;
        case HI_UNF_TUNER_CODE_MODULATION_CCM:
        default:
            strncpy(acCCMMode, "CCM", u32ArrayLen);
            break;
    }

    acCCMMode[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_fec_rate_isdbt(HI_UNF_TUNER_FE_FECRATE_E enLayerFECRate, HI_CHAR acFECRate[], HI_U32 u32ArrayLen)
{
    switch (enLayerFECRate)
    {
        case HI_UNF_TUNER_FE_FEC_1_2:
            strncpy(acFECRate, "1/2", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_2_3:
            strncpy(acFECRate, "2/3", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_4:
            strncpy(acFECRate, "3/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_4_5:
            strncpy(acFECRate, "4/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_5_6:
            strncpy(acFECRate, "5/6", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_6_7:
            strncpy(acFECRate, "6/7", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_7_8:
            strncpy(acFECRate, "7/8", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_8_9:
            strncpy(acFECRate, "8/9", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_9_10:
            strncpy(acFECRate, "9/10", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_1_4:
            strncpy(acFECRate, "1/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_1_3:
            strncpy(acFECRate, "1/3", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_2_5:
            strncpy(acFECRate, "2/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_5:
            strncpy(acFECRate, "3/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FECRATE_BUTT:
        case HI_UNF_TUNER_FE_FEC_AUTO:
        default:
            strncpy(acFECRate, "INVALID", u32ArrayLen);
            break;
    }

    acFECRate[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_fec_rate_t(HI_UNF_TUNER_FE_FECRATE_E enFECRate, HI_CHAR acFECRate[], HI_U32 u32ArrayLen)
{
    switch (enFECRate)
    {
        case HI_UNF_TUNER_FE_FEC_1_2:
            strncpy(acFECRate, "1/2", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_2_3:
            strncpy(acFECRate, "2/3", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_4:
            strncpy(acFECRate, "3/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_3_5:
            strncpy(acFECRate, "3/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_4_5:
            strncpy(acFECRate, "4/5", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_5_6:
            strncpy(acFECRate, "5/6", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FEC_7_8:
            strncpy(acFECRate, "7/8", u32ArrayLen);
            break;
        default:
            strncpy(acFECRate, "INVALID", u32ArrayLen);
            break;
    }

    acFECRate[u32ArrayLen - 1] = '\0';
}


static HI_VOID tuner_proc_get_fft_mode(HI_UNF_TUNER_FE_FFT_E enFFTMode, HI_CHAR acFFTMode[], HI_U32 u32ArrayLen)
{
    switch (enFFTMode)
    {
        case HI_UNF_TUNER_FE_FFT_1K:
            strncpy(acFFTMode, "1K", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FFT_2K:
            strncpy(acFFTMode, "2K", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FFT_4K:
            strncpy(acFFTMode, "4K", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FFT_8K:
            strncpy(acFFTMode, "8K", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FFT_16K:
            strncpy(acFFTMode, "16K", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_FFT_32K:
            strncpy(acFFTMode, "32K", u32ArrayLen);
            break;
        default:
            strncpy(acFFTMode, "INVALID", u32ArrayLen);
            break;
    }

    acFFTMode[u32ArrayLen - 1] = '\0';
}

static void tuner_proc_get_gi(HI_UNF_TUNER_FE_GUARD_INTV_E enGuardIntv, HI_CHAR acGI[], HI_U32 u32ArrayLen)
{
    switch (enGuardIntv)
    {
        case HI_UNF_TUNER_FE_GUARD_INTV_1_128:
            strncpy(acGI, "1/128", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_1_32:
            strncpy(acGI, "1/32", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_1_16:
            strncpy(acGI, "1/16", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_1_8:
            strncpy(acGI, "1/8", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_1_4:
            strncpy(acGI, "1/4", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_19_128:
            strncpy(acGI, "19/128", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_GUARD_INTV_19_256:
            strncpy(acGI, "19/256", u32ArrayLen);
            break;
        default:
            strncpy(acGI, "INVALID", u32ArrayLen);
            break;
    }

    acGI[u32ArrayLen - 1] = '\0';
}


static HI_VOID tuner_proc_get_hier_mode(HI_UNF_TUNER_FE_HIERARCHY_E enHierMod, HI_CHAR acHierAlpha[], HI_U32 u32ArrayLen)
{
    switch (enHierMod)
    {
        case HI_UNF_TUNER_FE_HIERARCHY_NO:
            strncpy(acHierAlpha, "NONE", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_HIERARCHY_ALHPA1:
            strncpy(acHierAlpha, "alpha1", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_HIERARCHY_ALHPA2:
            strncpy(acHierAlpha, "alpha2", u32ArrayLen);
            break;
        case HI_UNF_TUNER_FE_HIERARCHY_ALHPA4:
            strncpy(acHierAlpha, "alpha4", u32ArrayLen);
            break;
        default:
            strncpy(acHierAlpha, "INVALID", u32ArrayLen);
            break;
    }

    acHierAlpha[u32ArrayLen - 1] = '\0';
}

static HI_VOID tuner_proc_get_ts_priority(HI_UNF_TUNER_TS_PRIORITY_E enTsPriority, HI_CHAR acTSPrio[], HI_U32 u32ArrayLen)
{
    switch (enTsPriority)
    {
        case HI_UNF_TUNER_TS_PRIORITY_NONE:
            strncpy(acTSPrio, "NONE", u32ArrayLen);
            break;
        case HI_UNF_TUNER_TS_PRIORITY_HP:
            strncpy(acTSPrio, "HP", u32ArrayLen);
            break;
        case HI_UNF_TUNER_TS_PRIORITY_LP:
            strncpy(acTSPrio, "LP", u32ArrayLen);
            break;
        default:
            strncpy(acTSPrio, "INVALID", u32ArrayLen);
            break;
    }

    acTSPrio[u32ArrayLen - 1] = '\0';
}


static HI_VOID tuner_proc_read_isdbt(struct seq_file *p, HI_UNF_TUNER_SIGNALINFO_S *pstSignalInfo)
{
    HI_U8 u8Layer = 0;
    //HI_U8 u8LayerSegNum = 0;
    HI_UNF_TUNER_FE_HIERARCHY_E enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_BUTT;
    HI_UNF_TUNER_FE_FECRATE_E enLayerFECRate = HI_UNF_TUNER_FE_FFT_BUTT;
    HI_UNF_MODULATION_TYPE_E enLayerModType = HI_UNF_MOD_TYPE_BUTT;
    HI_CHAR acQamType[20];
    HI_CHAR acFECRate[10];
    HI_CHAR acHierAlpha[10];


    p += PROC_PRINT(p, "\nEmergencyFlag:\t%d\nPhaseShiftCorr:\t%d\nPartialFlag:\t%d\n",
            pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.u8EmergencyFlag,
            pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.u8PhaseShiftCorr,
            pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.u8PartialFlag);

    for (u8Layer = 0; u8Layer < 3; u8Layer++)
    {
        if (0 == u8Layer)
        {
            if (0 == pstSignalInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerAExist)
            {
                p += PROC_PRINT(p, "LayerA is not exist.\n");
                continue;
            }
            else
            {
                p += PROC_PRINT(p, "LayerA info:\n");
                p += PROC_PRINT(p, "\tLayerSegNum:\t%d\n",pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersAInfoBits.u8LayerSegNum);
                enLayerModType = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersAInfoBits.enLayerModType;
                enLayerFECRate = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersAInfoBits.enLayerFECRate;
                enLayerHierMod = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersAInfoBits.enLayerHierMod;
            }
        }
        else if (1 == u8Layer)
        {
            if (0 == pstSignalInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerBExist)
            {
               p += PROC_PRINT(p, "LayerB is not exist.\n");
                continue;
            }
            else
            {
                p += PROC_PRINT(p, "LayerB info:\n");
                p += PROC_PRINT(p, "\tLayerSegNum:\t%d\n",pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersBInfoBits.u8LayerSegNum);
                enLayerModType = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersBInfoBits.enLayerModType;
                enLayerFECRate = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersBInfoBits.enLayerFECRate;
                enLayerHierMod = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersBInfoBits.enLayerHierMod;
            }
        }
        else //if (2 == u8Layer)
        {
            if (0 == pstSignalInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerCExist)
            {
                p += PROC_PRINT(p, "LayerC is not exist.\n");
                continue;
            }
            else
            {
                p += PROC_PRINT(p, "LayerC info:\n");
                p += PROC_PRINT(p, "\tLayerSegNum:\t%d\n",pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersCInfoBits.u8LayerSegNum);
                enLayerModType = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersCInfoBits.enLayerModType;
                enLayerFECRate = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersCInfoBits.enLayerFECRate;
                enLayerHierMod = pstSignalInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo.stIsdbtLayersCInfoBits.enLayerHierMod;
            }
        }

        /* Mod Type */
        tuner_proc_get_mod_type_isdbt(enLayerModType, acQamType, sizeof(acQamType));
        
        /* Fec rate */
        tuner_proc_get_fec_rate_isdbt(enLayerFECRate, acFECRate, sizeof(acFECRate));

        /* Hier mode */
        tuner_proc_get_hier_mode(enLayerHierMod, acHierAlpha, sizeof(acHierAlpha));
        
        p += PROC_PRINT(p, "\tQam Type:%s, FEC rate:%s, HierMod: %s\n", acQamType, acFECRate, acHierAlpha);
    }

    return;
}


HI_S32 tuner_proc_read(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_U32 u32TunerPort;
    HI_U32 au32SignalStrength[3] = {0};
    HI_U32 u32SignalQuality = 0;
    HI_U32 au32TmpBer[3] = {0};
    HI_U32 u32SNR = 0;
    HI_U32 u32Ber = 0;
    HI_U64 u64TmpBer = 0;
    HI_U32 au32Rs[3] = {0};
    HI_CHAR acQamType[20];
    HI_CHAR acTunerType[20];
    HI_CHAR acDemodType[20];
    HI_CHAR acTunerLock[10];
    HI_CHAR acFECType[10];
    HI_CHAR acFECRate[10];
    HI_CHAR acFFTMode[10];
    HI_CHAR acGI[10];
    HI_CHAR acHierAlpha[10];
    HI_CHAR acTSPrio[10];
    HI_CHAR acCCMMode[10];
    HI_CHAR acSignalType[10];
    HI_UNF_TUNER_LOCK_STATUS_E enTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    HI_UNF_TUNER_SIGNALINFO_S stSignalInfo = {0};
    HI_S32 s32SnrLgValue = -1;
    HI_S32 s32SnrInteger = 0;
    HI_S32 s32SnrFraction = 0;
            

    PROC_PRINT(p,"---------Hisilicon TUNER Info---------\n");
    PROC_PRINT(p,
            "Port: LockStat    I2CChannel    Signal    Frequency(KHz)    SymbRate    QamMode    DemodType    Demodaddr   TunerType    Tuneraddr\n");


    for (u32TunerPort = 0; u32TunerPort < TUNER_NUM; u32TunerPort++)
    {
        if (HI_NULL == g_stTunerOps[u32TunerPort].tuner_connect)
        {
            continue;
        }        

        /* init proc variable */
        memset(&stSignalInfo, 0, sizeof(HI_UNF_TUNER_SIGNALINFO_S));
        enTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        strncpy(acTunerLock, "unlocked", sizeof(acTunerLock));
        acTunerLock[sizeof(acTunerLock) - 1] = '\0';
        strncpy(acSignalType, "Unknow ", sizeof(acSignalType));
        acSignalType[sizeof(acSignalType) - 1] = '\0';
        strncpy(acQamType, "QAM_INVALID ", sizeof(acQamType));
        acQamType[sizeof(acQamType) - 1] = '\0';
        u32SNR = 0;
        u32Ber = 0;
        au32SignalStrength[2] = 0;
        au32SignalStrength[1] = 0;
        au32SignalStrength[0] = 0;
        u32SignalQuality = 0;
        strncpy(acTunerType, "unknown TUNER", sizeof(acTunerType));
        acTunerType[sizeof(acTunerType) - 1] = '\0';
        strncpy(acDemodType, "unknown demod", sizeof(acDemodType));
        acDemodType[sizeof(acDemodType) - 1] = '\0';

        /* get proc info */
        if (g_stTunerOps[u32TunerPort].tuner_get_status)
        {
            g_stTunerOps[u32TunerPort].tuner_get_status(u32TunerPort, &enTunerStatus);
        }

        if ( HI_UNF_TUNER_SIGNAL_LOCKED == enTunerStatus )
        {
            strncpy(acTunerLock, "locked", sizeof(acTunerLock));
            acTunerLock[sizeof(acTunerLock) - 1] = '\0';

            if (g_stTunerOps[u32TunerPort].tuner_get_signal_info)
            {
                g_stTunerOps[u32TunerPort].tuner_get_signal_info(u32TunerPort, &stSignalInfo);

                /* Signal type */
                tuner_proc_get_signal_type(stSignalInfo.enSigType, acSignalType, sizeof(acSignalType));
                
                /* Modulate type */
                tuner_proc_get_mod_type(u32TunerPort, &stSignalInfo, acQamType, sizeof(acQamType));

                /* Snr */
                tuner_proc_get_snr(u32TunerPort, stSignalInfo.enSigType, u32SNR, &s32SnrLgValue, &s32SnrInteger, &s32SnrFraction);
            }
        }

        if (g_stTunerOps[u32TunerPort].tuner_get_ber)
        {
            g_stTunerOps[u32TunerPort].tuner_get_ber(u32TunerPort, au32TmpBer);
        }
        if (g_stTunerOps[u32TunerPort].tuner_get_snr)
        {
            g_stTunerOps[u32TunerPort].tuner_get_snr(u32TunerPort, &u32SNR);
        }
        if (g_stTunerOps[u32TunerPort].tuner_get_signal_strength)
        {
            g_stTunerOps[u32TunerPort].tuner_get_signal_strength(u32TunerPort, au32SignalStrength);
        }
        if (g_stTunerOps[u32TunerPort].tuner_get_signal_quality)
        {
            g_stTunerOps[u32TunerPort].tuner_get_signal_quality(u32TunerPort, &u32SignalQuality);
        }

        if ((HI_UNF_DEMOD_DEV_TYPE_3130I == g_stTunerOps[u32TunerPort].enDemodDevType)
            || (HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType)
            || (HI_UNF_DEMOD_DEV_TYPE_J83B  == g_stTunerOps[u32TunerPort].enDemodDevType))
        {
            u32Ber = (au32TmpBer[0] << 16 | au32TmpBer[1] << 8 | au32TmpBer[2]);
            u64TmpBer = (HI_U64)u32Ber;
            u32Ber = (HI_U32)((u64TmpBer / 8) * 10000000 / (1 << 20));
            au32TmpBer[0] = u32Ber;
            au32TmpBer[1] = 0;
            au32TmpBer[2] = 7;
        }
        if (HI_UNF_DEMOD_DEV_TYPE_MXL254 == g_stTunerOps[u32TunerPort].enDemodDevType)
        {
            u32SNR = u32SNR /10;
        }

        /* Demod type */
        tuner_proc_get_demod_type(u32TunerPort, acDemodType, sizeof(acDemodType));
        
        /* Tuner type */
        tuner_proc_get_tuner_type(u32TunerPort, acTunerType, sizeof(acTunerType));
                
        PROC_PRINT(p,
                " %d:   %s     %5d      %s    %8d   %13d    %7s    %s      0x%02x  %12s      0x%02x\n",
                u32TunerPort,
                acTunerLock,
                g_stTunerOps[u32TunerPort].enI2cChannel,
                acSignalType,
                g_stTunerOps[u32TunerPort].stCurrPara.u32Frequency,
                        g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32SymbolRate,
                acQamType,
                acDemodType,
                g_stTunerOps[u32TunerPort].u32DemodAddress,
                acTunerType,
                g_stTunerOps[u32TunerPort].u32TunerAddress
                );

        if (s32SnrLgValue == -1)
        {
            PROC_PRINT(p, "\nBER:%d.%d*(E-%d),  SNR:%d,  SignalStrength:%d\n", au32TmpBer[0], au32TmpBer[1], au32TmpBer[2], u32SNR, au32SignalStrength[1]);
        }
        else
        {
            PROC_PRINT(p, "\nBER:%d.%d*(E-%d),  SNR:%d.%d,  SignalStrength:%d\n", au32TmpBer[0], au32TmpBer[1], au32TmpBer[2], s32SnrInteger, s32SnrFraction, au32SignalStrength[1]);
        }
        if ((HI_UNF_DEMOD_DEV_TYPE_ATBM888X == g_stTunerOps[u32TunerPort].enDemodDevType)
           || (HI_UNF_DEMOD_DEV_TYPE_AVL6381 == g_stTunerOps[u32TunerPort].enDemodDevType))
        {
            PROC_PRINT(p, "SignalQuality:%d\n", u32SignalQuality);
        }

        if (g_stTunerOps[u32TunerPort].tuner_get_rs)
        {
            g_stTunerOps[u32TunerPort].tuner_get_rs(u32TunerPort, au32Rs);
            PROC_PRINT(p, "\nall_rs_package:%-d,  corrected_rs_package:%d,  error_rs_package:%d\n\n", au32Rs[0],
                       au32Rs[1], au32Rs[2]);
        }

        if ( HI_UNF_TUNER_SIGNAL_DROPPED == enTunerStatus )
        {
            continue;
        }

        /* For DVB-S/S2, print FEC type and rate */
        if (HI_UNF_TUNER_SIG_TYPE_SAT == stSignalInfo.enSigType)
        {
            /* Fec type */
            tuner_proc_get_fec_type_s(stSignalInfo.unSignalInfo.stSat.enSATType, acFECType, sizeof(acFECType));

            /* Fec rate */
            tuner_proc_get_fec_rate_s(stSignalInfo.unSignalInfo.stSat.enFECRate, acFECRate, sizeof(acFECRate));

            PROC_PRINT(p, "\nFEC type:%s,  FEC rate:%s\n", acFECType, acFECRate);

            /* CCM mode */
            if(HI_UNF_DEMOD_DEV_TYPE_3136 == g_stTunerOps[u32TunerPort].enDemodDevType)
            {
                tuner_proc_get_ccm_mode(stSignalInfo.unSignalInfo.stSat.enCodeModulation, acCCMMode, sizeof(acCCMMode));
                
                p += PROC_PRINT(p, "\nCode and Modulation:%s\n", acCCMMode);
            }
        }
        /* For ISDB-T, layer info*/
        else if (HI_UNF_TUNER_SIG_TYPE_ISDB_T == stSignalInfo.enSigType)
        {
            tuner_proc_read_isdbt(p, &stSignalInfo);
        }
        /* For DVB-T/T2, print FEC type and rate */
        else if (HI_UNF_TUNER_SIG_TYPE_DVB_T <= stSignalInfo.enSigType
            && HI_UNF_TUNER_SIG_TYPE_DTMB >= stSignalInfo.enSigType)
        {
            /* Fec rate */
            tuner_proc_get_fec_rate_t(stSignalInfo.unSignalInfo.stTer.enFECRate, acFECRate, sizeof(acFECRate));

            /* FFT mode */
            tuner_proc_get_fft_mode(stSignalInfo.unSignalInfo.stTer.enFFTMode, acFFTMode, sizeof(acFFTMode));

            /* GI */
            tuner_proc_get_gi(stSignalInfo.unSignalInfo.stTer.enGuardIntv, acGI, sizeof(acGI));

            /* Hier mode */
            tuner_proc_get_hier_mode(stSignalInfo.unSignalInfo.stTer.enHierMod, acHierAlpha, sizeof(acHierAlpha));

            /* TS priority */
            tuner_proc_get_ts_priority(stSignalInfo.unSignalInfo.stTer.enTsPriority, acTSPrio, sizeof(acTSPrio));
            

            PROC_PRINT(p, "\nFEC rate:%s, FFT Mode:%s, GI: %s, HierMod: %s, TSPrio: %s\n", acFECRate, acFFTMode, acGI, acHierAlpha, acTSPrio);
        }
    }

#endif

    return HI_SUCCESS;
}

HI_S32 tuner_proc_read_diseqc(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    HI_U32 i,j,k = 0;
    HI_CHAR acMotorPosition[20];
    HI_CHAR acEastLimit[10];//enable or disable
    HI_CHAR acWestLimit[10];//enable or disable
    HI_CHAR acStorePos[MOTOR_STORE_POSITION_MAX_NUM][10];
    HI_CHAR acSwitchPort0[8];
    HI_CHAR acSwitchPort1[8];

    
    PROC_PRINT(p,"---------Hisilicon TUNER DiSEqC Info---------\n");
    PROC_PRINT(p,
                "Port :Switch1.0    Switch1.1   MotorMoveStep   EastLimit   WestLimit\n");

    for(i = 0; i < TUNER_NUM; i++)
    {
        strncpy(acSwitchPort1, "none", sizeof(acSwitchPort1));
        if((HI_UNF_TUNER_SIG_TYPE_SAT != g_stTunerOps[i].enSigType) || (HI_NULL == g_stTunerOps[i].tuner_DiSEqC_send_msg))
        {
            continue;
        }

        if (s_stDiSEqCDrvStatus[i].stPort1_0.enPort >= HI_UNF_TUNER_DISEQC_SWITCH_PORT_1 
            && s_stDiSEqCDrvStatus[i].stPort1_0.enPort <= HI_UNF_TUNER_DISEQC_SWITCH_PORT_4)
        {
            snprintf(acSwitchPort0, sizeof(acSwitchPort0), "Port%d", s_stDiSEqCDrvStatus[i].stPort1_0.enPort);
        }
        else
        {
            strncpy(acSwitchPort0, "disable", sizeof(acSwitchPort0));
        }

        if (s_stDiSEqCDrvStatus[i].stPort1_1.enPort >= HI_UNF_TUNER_DISEQC_SWITCH_PORT_1
            && s_stDiSEqCDrvStatus[i].stPort1_1.enPort <= HI_UNF_TUNER_DISEQC_SWITCH_PORT_16)
        {
            snprintf(acSwitchPort1, sizeof(acSwitchPort1), "Port%d", s_stDiSEqCDrvStatus[i].stPort1_1.enPort);
        }
        else
        {
            strncpy(acSwitchPort1, "disable", sizeof(acSwitchPort1));
        }

        snprintf(acMotorPosition,sizeof(acMotorPosition),"%d step",s_stDiSEqCDrvStatus[i].s16MotorPos);
        if(s_stDiSEqCDrvStatus[i].s16EastLimit)
        {
            strncpy(acEastLimit, "enable", sizeof(acEastLimit));
        }
        else
        {
            strncpy(acEastLimit, "disable", sizeof(acEastLimit));
        }

        if(s_stDiSEqCDrvStatus[i].s16WestLimit)
        {
            strncpy(acWestLimit, "enable", sizeof(acWestLimit));
        }
        else
        {
            strncpy(acWestLimit, "disable", sizeof(acWestLimit));
        }

        PROC_PRINT(p,
                "%4d  %9s    %9s    %13s    %9s %9s\n\n",i,acSwitchPort0,acSwitchPort1,acMotorPosition,acEastLimit,acWestLimit);

        k = 0;
        for(j = 0;j < MOTOR_STORE_POSITION_MAX_NUM;j++)
        {
            if (j <= 31)
            {
                if (s_stDiSEqCDrvStatus[i].stMotorFlag.u32Flags[0] & (1 << j))
                {
                    snprintf(acStorePos[k],10,"Pos%d ",j+1);
                    k++;
                    if (0 == (k % 4))
                    {
                        PROC_PRINT(p,"%s    %s  %s  %s\n",acStorePos[k-4],acStorePos[k-3],acStorePos[k-2],acStorePos[k-1]);
                    }
                }
            }
            else
            {
                if (s_stDiSEqCDrvStatus[i].stMotorFlag.u32Flags[1] & (1 << (j - 32)))
                {
                    snprintf(acStorePos[k],10,"Pos%d ",j+1);
                    k++;
                    if (0 == (k % 4))
                    {
                        PROC_PRINT(p,"%s    %s  %s  %s\n",acStorePos[k-4],acStorePos[k-3],acStorePos[k-2],acStorePos[k-1]);
                    }
                }
            }
            if ((j == MOTOR_STORE_POSITION_MAX_NUM - 1) && (k % 4))
            {
                if (0 == ((k-1)%4))
                {
                    PROC_PRINT(p,"%s\n",acStorePos[k-1]);
                }
                else if (0 == ((k-2)%4))
                {
                    PROC_PRINT(p,"%s    %s\n",acStorePos[k-2],acStorePos[k-1]);
                }
                else
                {
                    PROC_PRINT(p,"%s    %s  %s\n",acStorePos[k-3],acStorePos[k-2],acStorePos[k-1]);
                }
            }
        }
    }
#endif
    return HI_SUCCESS;
}

static HI_S32 tuner_drv_get_proc_arg(HI_CHAR*  chCmd,HI_CHAR*  chArg,HI_U32 u32ArgIndex)
{
    HI_U32 u32Count;
    HI_U32 u32CmdCount;
    HI_U32 u32LogCount;
    HI_U32 u32NewFlag;
    HI_CHAR chArg1[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR chArg2[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR chArg3[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR chArg4[TUNER_MAX_PROC_ARGS_SIZE] = {0};

    /*clear empty space*/
    u32Count = 0;
    u32CmdCount = 0;
    u32LogCount = 1;
    u32NewFlag = 0;
    while(chCmd[u32Count] != 0 && chCmd[u32Count] != '\n' )
    {
        if (chCmd[u32Count] != ' ')
        {
            u32NewFlag = 1;
        }
        else
        {
            if(u32NewFlag == 1)
            {
                u32LogCount++;
                u32CmdCount= 0;
                u32NewFlag = 0;
            }
        }

        if (u32NewFlag == 1)
        {
            switch(u32LogCount)
            {
                case 1:
                    chArg1[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 2:
                    chArg2[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 3:
                    chArg3[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                case 4:
                    chArg4[u32CmdCount] = chCmd[u32Count];
                    u32CmdCount++;
                    break;
                default:
                    break;
            }

            if(u32CmdCount >= TUNER_MAX_PROC_ARGS_SIZE)
            {
                return HI_FAILURE;
            }
        }
        u32Count++;
    }

    switch(u32ArgIndex)
    {
        case 1:
            memcpy(chArg,chArg1,sizeof(HI_CHAR)*TUNER_MAX_PROC_ARGS_SIZE);
            break;
        case 2:
            memcpy(chArg,chArg2,sizeof(HI_CHAR)*TUNER_MAX_PROC_ARGS_SIZE);
            break;
        case 3:
            memcpy(chArg,chArg3,sizeof(HI_CHAR)*TUNER_MAX_PROC_ARGS_SIZE);
            break;
        case 4:
            memcpy(chArg,chArg4,sizeof(HI_CHAR)*TUNER_MAX_PROC_ARGS_SIZE);
            break;
        default:
            break;
    }
    return HI_SUCCESS;
}

HI_S32 tuner_read_spectrum_data(HI_U32 u32TunerPort,HI_U32 u32DataLen)
{
    HI_S32 s32Ret,s32Len = 0;
    HI_S16 i;
    HI_CHAR                 FileName[TUNER_FILE_NAME_LEN] = {0};
    HI_U8 u8DataBuf[30];
    HI_UNF_TUNER_SAMPLE_DATA_S *pSpectrum = HI_NULL;

    if ( HI_UNF_DEMOD_DEV_TYPE_3136 != g_stTunerOps[u32TunerPort].enDemodDevType
        && HI_UNF_DEMOD_DEV_TYPE_3138 != g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        HI_ERR_TUNER("only hi3136 and hi3138 demod can read spectrum data.\n");
        return HI_SUCCESS;
    }

    if(u32DataLen > SAMPLE_DATA_LENGTH)
    {
        HI_ERR_TUNER("u32DataLen max value is 2048.\n");
        return HI_FAILURE;
    }

    pSpectrum = (HI_UNF_TUNER_SAMPLE_DATA_S *)HI_VMALLOC(HI_ID_TUNER, SAMPLE_DATA_LENGTH*sizeof(HI_UNF_TUNER_SAMPLE_DATA_S));
    if (HI_NULL == pSpectrum)
    {
        HI_ERR_TUNER("memory allocate failed\n");

        return HI_ERR_TUNER_FAILED_ALLOC_MEM;
    }

    s32Ret = g_stTunerOps[u32TunerPort].tuner_get_data_sample(u32TunerPort, TUNER_DATA_SRC_ADC, u32DataLen, pSpectrum);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "\n tuner_get_data_sample failed \n");
        goto exit;
    }

    /***
    open file
    ***/
    s32Ret = HI_DRV_FILE_GetStorePath(FileName, TUNER_FILE_NAME_LEN);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("get path failed\n");

        goto exit;
    }


    if (HI_NULL != TunerSpectrumHandle[u32TunerPort])
    {
        HI_ERR_TUNER("already started\n");

        goto exit;
    }

    snprintf(FileName, sizeof(FileName),"%s/hi_spectrum.txt", FileName);

    TunerSpectrumHandle[u32TunerPort] = HI_DRV_FILE_Open(FileName, 1);
    if (!(TunerSpectrumHandle[u32TunerPort]))
    {
        HI_ERR_TUNER("open %s error\n", FileName);

        goto exit;
    }

    HI_DRV_PROC_EchoHelper("Column:2\n");
    memset(u8DataBuf,0,sizeof(u8DataBuf));
    u8DataBuf[0] = '\0';
    snprintf(u8DataBuf,sizeof(u8DataBuf),"\r\nColumn:2\r\n");
    if (TunerSpectrumHandle[u32TunerPort])
    {
        s32Len = HI_DRV_FILE_Write(TunerSpectrumHandle[u32TunerPort], (HI_S8*)u8DataBuf, strlen(u8DataBuf));
        if(strlen(u8DataBuf) != s32Len)
        {
            HI_ERR_TUNER("call HI_DRV_FILE_Write fail.\n");
            goto exit;
        }
    }

    for(i = 0;i < u32DataLen;i++)
    {
        HI_DRV_PROC_EchoHelper("%d,%d\n",pSpectrum[i].s32DataIP,pSpectrum[i].s32DataQP);
        memset(u8DataBuf,0,sizeof(u8DataBuf));
        u8DataBuf[0] = '\0';
        snprintf(u8DataBuf,sizeof(u8DataBuf),"%d,%d\r\n",pSpectrum[i].s32DataIP,pSpectrum[i].s32DataQP);
        /***
        write file
        ***/
        if (TunerSpectrumHandle[u32TunerPort])
        {
            s32Len = HI_DRV_FILE_Write(TunerSpectrumHandle[u32TunerPort], (HI_S8*)u8DataBuf, strlen(u8DataBuf));
            if(strlen(u8DataBuf) != s32Len)
            {
                HI_ERR_TUNER("call HI_DRV_FILE_Write fail.\n");
                goto exit;
            }
        }

    }
    HI_DRV_PROC_EchoHelper("\n");

exit:
    HI_VFREE(HI_ID_TUNER,pSpectrum);
    pSpectrum = HI_NULL;

    /***
    close file
    ***/
    if (TunerSpectrumHandle[u32TunerPort])
    {
        HI_DRV_FILE_Close(TunerSpectrumHandle[u32TunerPort]);
        TunerSpectrumHandle[u32TunerPort] = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 tuner_read_constellation_data(HI_U32 u32TunerPort,HI_U32 u32DataLen)
{
    HI_S32 s32Ret,s32Len = 0;
    HI_S16 i;
    HI_CHAR                 FileName[TUNER_FILE_NAME_LEN] = {0};
    HI_U8 u8DataBuf[30];
    HI_UNF_TUNER_SAMPLE_DATA_S *pConstellation = HI_NULL;

    if ( HI_UNF_DEMOD_DEV_TYPE_3136 != g_stTunerOps[u32TunerPort].enDemodDevType
        && HI_UNF_DEMOD_DEV_TYPE_3138 != g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        HI_ERR_TUNER("only hi3136 and hi3138 demod can read constellation data.\n");
        return HI_SUCCESS;
    }

    if(u32DataLen > SAMPLE_DATA_LENGTH)
    {
        HI_ERR_TUNER("u32DataLen max value is 2048.\n");
        return HI_FAILURE;
    }

     pConstellation = (HI_UNF_TUNER_SAMPLE_DATA_S *)HI_VMALLOC(HI_ID_TUNER, SAMPLE_DATA_LENGTH*sizeof(HI_UNF_TUNER_SAMPLE_DATA_S));
    if (HI_NULL == pConstellation)
    {
        HI_ERR_TUNER("memory allocate failed\n");

        return HI_ERR_TUNER_FAILED_ALLOC_MEM;
    }

    s32Ret = g_stTunerOps[u32TunerPort].tuner_get_data_sample(u32TunerPort, TUNER_DATA_SRC_EQU, u32DataLen, pConstellation);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "\n tuner_get_data_sample failed \n");
        goto exit;
    }

    /***
    open file
    ***/
    s32Ret = HI_DRV_FILE_GetStorePath(FileName, TUNER_FILE_NAME_LEN);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("get path failed\n");

        goto exit;
    }


    if (HI_NULL != TunerConstellationHandle[u32TunerPort])
    {
        HI_ERR_TUNER("already started\n");

        goto exit;
    }

    snprintf(FileName, sizeof(FileName),"%s/hi_constellation.txt", FileName);

    TunerConstellationHandle[u32TunerPort] = HI_DRV_FILE_Open(FileName, 1);
    if (!(TunerConstellationHandle[u32TunerPort]))
    {
        HI_ERR_TUNER("open %s error\n", FileName);

        goto exit;
    }

    HI_DRV_PROC_EchoHelper("Column:2\n");
    memset(u8DataBuf,0,sizeof(u8DataBuf));
    u8DataBuf[0] = '\0';
    snprintf(u8DataBuf,sizeof(u8DataBuf),"\r\nColumn:2\r\n");
    if (TunerConstellationHandle[u32TunerPort])
    {
        s32Len = HI_DRV_FILE_Write(TunerConstellationHandle[u32TunerPort], (HI_S8*)u8DataBuf, strlen(u8DataBuf));
        if(strlen(u8DataBuf) != s32Len)
        {
            HI_ERR_TUNER("call HI_DRV_FILE_Write fail.\n");
            goto exit;
        }
    }

    for(i = 0;i < u32DataLen;i++)
    {
        HI_DRV_PROC_EchoHelper("%d,%d\n",pConstellation[i].s32DataIP,pConstellation[i].s32DataQP);
        memset(u8DataBuf,0,sizeof(u8DataBuf));
        u8DataBuf[0] = '\0';
        snprintf(u8DataBuf,sizeof(u8DataBuf),"%d,%d\r\n",pConstellation[i].s32DataIP,pConstellation[i].s32DataQP);
        /***
        write file
        ***/
        if (TunerConstellationHandle[u32TunerPort])
        {
            s32Len = HI_DRV_FILE_Write(TunerConstellationHandle[u32TunerPort], (HI_S8*)u8DataBuf, strlen(u8DataBuf));
            if(strlen(u8DataBuf) != s32Len)
            {
                HI_ERR_TUNER("call HI_DRV_FILE_Write fail,s32Len:%d\n",s32Len);
                goto exit;
            }
        }

    }
    HI_DRV_PROC_EchoHelper("\n");

exit:
    HI_VFREE(HI_ID_TUNER, pConstellation);
    pConstellation = HI_NULL;

    /***
    close file
    ***/
    if (TunerConstellationHandle[u32TunerPort])
    {
        HI_DRV_FILE_Close(TunerConstellationHandle[u32TunerPort]);
        TunerConstellationHandle[u32TunerPort] = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_VOID tuner_read_cir_plot_data(HI_U32 u32TunerPort,HI_U32 u32DataLen)
{
    TUNER_I2C_DATA_S stI2cData;
    HI_S32 s32Ret,s32Len = 0;
    HI_S16 i,s16Hcir;
    HI_U8 u8DataBuf[20],u8SampBuf[4],u8Status = 0x6A,/*u8DataL = 0,*/u8DataM = 0,u8DataH = 0,u8TempBuf[10];
    HI_CHAR                 FileName[TUNER_FILE_NAME_LEN] = {0};
    static struct file         *TunerCirPlotHandle[TUNER_NUM]  = {HI_NULL};


    if ( HI_UNF_DEMOD_DEV_TYPE_3137 != g_stTunerOps[u32TunerPort].enDemodDevType
        && HI_UNF_DEMOD_DEV_TYPE_3138 != g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        HI_ERR_TUNER("only hi3137 and hi3138 can read cirplot data.\n");
        return;
    }

        //stop mcu
        qam_write_byte(u32TunerPort, 0x8E,0x44);

        //close timeout reset
        qam_write_byte(u32TunerPort, Hi3137_STATE_WAITS,0xFF);
        qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1,0x7);
        qam_write_byte(u32TunerPort, Hi3137_ILA_SEL,0x6A);

        //config enable
        u8DataBuf[0] = 0xE0;
        u8DataBuf[1] = 0x98;
        u8DataBuf[2] = 0x0;
        u8DataBuf[3] = 0x1;
        u8DataBuf[4] = 0x0;
        u8DataBuf[5] = 0x0;
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 6;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        //config trig
        u8DataBuf[0] = 0xE5;
        u8DataBuf[1] = 0x0;
        u8DataBuf[2] = 0x0;
        u8DataBuf[3] = 0x3;
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 4;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        u8DataBuf[0] = 0xEB;
        u8DataBuf[1] = 0x0;
        u8DataBuf[2] = 0x0;
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 3;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        //config sample num
        u8DataBuf[0] = 0xE8;
        u8DataBuf[1] = 0x0;
        u8DataBuf[2] = 0x8;
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 3;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        //select low 32bit data from vec
        u8DataBuf[0] = 0xF0;
        for(i = 0;i < 16;i++)
        {
            u8DataBuf[i+1] = i & 0xff;
        }
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 17;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        //set catch_start
        //first reset logic
        //switch off the c_ad_smp
        qam_read_byte(u32TunerPort, 0x89,&u8Status);
        u8Status &= 0xDF;
        qam_write_byte(u32TunerPort, 0x89,u8Status);

        qam_write_byte(u32TunerPort, 0xEA,0);
        tuner_mdelay(5);
        qam_write_byte(u32TunerPort, 0xEA,0x20);

        //wait for catch done
        tuner_mdelay(5);
        qam_read_byte(u32TunerPort, 0xEA,&u8Status);
        for(i = 0;i < CATCH_DONE_TIMEOUT;i++)
        {
            if(u8Status >= 0x40)
                break;

            tuner_mdelay(5);
            qam_read_byte(u32TunerPort, 0xEA,&u8Status);
        }
        if(i == CATCH_DONE_TIMEOUT)
        {
            HI_ERR_TUNER("cir plot timeout.\n");
            goto exit;
        }

        //read smp from data
        u8DataBuf[0] = 0xED;
        u8DataBuf[1] = 0x0;
        u8DataBuf[2] = 0x0;
        memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
        stI2cData.pu8SendBuf = u8DataBuf;
        stI2cData.u32SendLength = 3;
        tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
            &stI2cData);

        //start address increasing by self when catch data
        //c_stor_addrinc = 0;
        qam_read_byte(u32TunerPort, Hi3137_SDC_CTRL,&u8Status);
        u8Status &= 0xFB;
        qam_write_byte(u32TunerPort, Hi3137_SDC_CTRL,u8Status);

        /***
        open file
        ***/
        s32Ret = HI_DRV_FILE_GetStorePath(FileName, TUNER_FILE_NAME_LEN);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("get path failed\n");

            goto exit;
        }


        if (HI_NULL != TunerCirPlotHandle[u32TunerPort])
        {
            HI_ERR_TUNER("already started\n");

            goto exit;
        }

        snprintf(FileName, sizeof(FileName),"%s/hi_cir_samp.txt", FileName);

        TunerCirPlotHandle[u32TunerPort] = HI_DRV_FILE_Open(FileName, 1);
        if (!(TunerCirPlotHandle[u32TunerPort]))
        {
            HI_ERR_TUNER("open %s error\n", FileName);

            goto exit;
        }

        for(i = 0;i < u32DataLen*4;i++)
        {
            u8DataBuf[0] = 0xEF;
            memset(&stI2cData,0,sizeof(TUNER_I2C_DATA_S));
            stI2cData.pu8SendBuf = u8DataBuf;
            stI2cData.u32SendLength = 1;
            stI2cData.pu8ReceiveBuf = u8SampBuf;
            stI2cData.u32ReceiveLength = 1;
            s32Ret = tuner_i2c_receive_data(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32DemodAddress,
                &stI2cData);

            if(s32Ret != HI_SUCCESS)
            {
                HI_ERR_TUNER("call tuner_i2c_receive_data fail.\n");
                goto exit;
            }
            switch(i%4)
            {
                case 0:
                    //u8DataL = u8SampBuf[0];
                    break;
                case 1:
                    u8DataM = u8SampBuf[0];
                    break;
                case 2:
                    u8DataH = u8SampBuf[0];
                    break;
                case 3:
                    u8Status = u8SampBuf[0];
                    s16Hcir = ((u8DataH & 0x7F)<<3) + ((u8DataM & 0xE0)>>5);
                    //printk("%4d ",s16Hcir);
                    HI_DRV_PROC_EchoHelper("%4d ",s16Hcir);
                    memset(u8TempBuf,0,sizeof(u8TempBuf));
                    u8TempBuf[0] = '\0';
                    snprintf(u8TempBuf,sizeof(u8TempBuf),"%d ",s16Hcir);
                    /***
                    write file
                    ***/
                    if (TunerCirPlotHandle[u32TunerPort])
                    {
                        s32Len = HI_DRV_FILE_Write(TunerCirPlotHandle[u32TunerPort], (HI_S8*)u8TempBuf, strlen(u8TempBuf));
                        if(strlen(u8TempBuf) != s32Len)
                        {
                            HI_ERR_TUNER("call HI_DRV_FILE_Write fail,s32Len:%d\n",s32Len);
                            goto exit;
                        }
                    }
                    break;
            }

            if(i % 64 == 0)
            {
                //printk("\n");
                HI_DRV_PROC_EchoHelper("\r\n");
                memset(u8TempBuf,0,sizeof(u8TempBuf));
                u8TempBuf[0] = '\0';
                snprintf(u8TempBuf,sizeof(u8TempBuf),"\r\n");
                if (TunerCirPlotHandle[u32TunerPort])
                {
                    s32Len = HI_DRV_FILE_Write(TunerCirPlotHandle[u32TunerPort], (HI_S8*)u8TempBuf, strlen(u8TempBuf));
                    if(strlen(u8TempBuf) != s32Len)
                    {
                        HI_ERR_TUNER("call HI_DRV_FILE_Write fail,s32Len:%d\n",s32Len);
                        goto exit;
                    }
                }
            }
        }
        HI_DRV_PROC_EchoHelper("\r\n");
        memset(u8TempBuf,0,sizeof(u8TempBuf));
        u8TempBuf[0] = '\0';
        snprintf(u8TempBuf,sizeof(u8TempBuf),"\r\n");
        if (TunerCirPlotHandle[u32TunerPort])
        {
            s32Len = HI_DRV_FILE_Write(TunerCirPlotHandle[u32TunerPort], (HI_S8*)u8TempBuf, strlen(u8TempBuf));
            if(strlen(u8TempBuf) != s32Len)
            {
                HI_ERR_TUNER("call HI_DRV_FILE_Write fail,s32Len:%d\n",s32Len);
                goto exit;
            }
        }

exit:
        //end address increasing by self when catch data
        //c_stor_addrinc = 1;
        qam_read_byte(u32TunerPort, Hi3137_SDC_CTRL,&u8Status);
        //u8Status |= (1<<2);
        u8Status &= 0xFB;
        qam_write_byte(u32TunerPort, Hi3137_SDC_CTRL,u8Status);

        //start reset
        qam_write_byte(u32TunerPort, Hi3137_STATE_WAITS,0x16);
        qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1,0x1F);

        //start mcu
        qam_write_byte(u32TunerPort, 0x8E,0x33);

        /***
        close file
        ***/
        if (TunerCirPlotHandle[u32TunerPort])
        {
            HI_DRV_FILE_Close(TunerCirPlotHandle[u32TunerPort]);
            TunerCirPlotHandle[u32TunerPort] = HI_NULL;
        }


    return;
}

HI_S32 tuner_proc_write(struct file * file,
                                 const char __user * buffer, size_t count,
                                 loff_t *ppos)
{
    HI_CHAR dat1[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat2[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat3[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat4[TUNER_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR buf[256] = {0};
    HI_U32 u32TunerId = 0,u32DataLen = 0,u32TimeSec = 0,u32DmxPort = 0;
    HI_S32 s32Ret = HI_FAILURE;

    memset(buf, 0, sizeof(buf));
    if (copy_from_user(buf, buffer, count < sizeof(buf) - 1 ? count : sizeof(buf) - 1))    
    {
        HI_ERR_TUNER("FATAL: copy_from_user failed!\n");
        return 0;
    }

    tuner_drv_get_proc_arg(buf,dat1,1);
    tuner_drv_get_proc_arg(buf,dat2,2);
    tuner_drv_get_proc_arg(buf,dat3,3);

    if(0!=strncmp(dat1,"help",4))
    {
        u32TunerId = dat2[0] - '0';
        if(u32TunerId >= TUNER_NUM)
        {
            HI_ERR_TUNER("TunerId %d err,not in range 0--%d\n",u32TunerId,TUNER_NUM-1);
            return HI_FAILURE;
        }
        switch(g_stTunerOps[u32TunerId].enSigType)
        {
            case HI_UNF_TUNER_SIG_TYPE_SAT:
            case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
                break;
            default:
                HI_ERR_TUNER("TunerId=%d 's signal type err,err value is :%d.\n",u32TunerId,g_stTunerOps[u32TunerId].enSigType);
                return HI_FAILURE;
        }

        s32Ret = str2int(dat3,&u32DataLen);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_TUNER("str2int err.\n");
            return HI_FAILURE;
        }
        if(0==strncmp(dat1,"rfdata",6))
        {
            u32DmxPort = u32DataLen;
        }
        else
        {
            switch(u32DataLen)
            {
                case 32:
                case 64:
                case 128:
                case 256:
                case 512:
                case 1024:
                case 2048:
                    break;
                default:
                    HI_ERR_TUNER("Data length err.\n");
                    return HI_FAILURE;
            }
        }
    }

    if(0==strncmp(dat1,"help",4))
    {
        HI_DRV_PROC_EchoHelper("================ read spectrum/constellation/cirplot data ================\n");
        HI_DRV_PROC_EchoHelper("echo 'function name' 'TunerId' 'DataLength' > /proc/msp/tuner\n");
        HI_DRV_PROC_EchoHelper("     'TunerId' range is:0--%d.'DataLength' range is:32--2048,and 'DataLength' = 2^x.\n",TUNER_NUM-1);
        HI_DRV_PROC_EchoHelper("echo  spectrum 0 2048    -- read TunerId=0/DataLength=2048 spectrum data\n");
        HI_DRV_PROC_EchoHelper("echo  constellation 0 2048     -- read TunerId=0/DataLength=2048 constellation data\n");
        HI_DRV_PROC_EchoHelper("echo  cirplot 0 2048    -- read TunerId=0/DataLength=2048 cirplot data\n");
        HI_DRV_PROC_EchoHelper("============================= read rf data ==============================\n");
        HI_DRV_PROC_EchoHelper("echo 'function name' 'TunerId' 'DmxPort' 'Time' > /proc/msp/tuner\n");
        HI_DRV_PROC_EchoHelper("     'TunerId' range is:0--%d.'Time' unit is second.\n",TUNER_NUM-1);
        HI_DRV_PROC_EchoHelper("echo  rfdata 0 32 15    -- read TunerId=0/DmxPort=32/Time=15S rf data\n");
    }
    else if(0==strncmp(dat1,"spectrum",8))
    {
        s32Ret = tuner_read_spectrum_data(u32TunerId, u32DataLen);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_TUNER("call tuner_read_spectrum_data err.\n");
            return HI_FAILURE;
        }
    }
    else if(0==strncmp(dat1,"constellation",13))
    {
        s32Ret = tuner_read_constellation_data(u32TunerId, u32DataLen);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_TUNER("call tuner_read_constellation_data err.\n");
            return HI_FAILURE;
        }
    }
    else if(0==strncmp(dat1,"cirplot",7))
    {
        tuner_read_cir_plot_data(u32TunerId, u32DataLen);
    }
    else if(0==strncmp(dat1,"rfdata",6))
    {
        tuner_drv_get_proc_arg(buf,dat4,4);
        s32Ret = str2int(dat4,&u32TimeSec);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_TUNER("str2int err.\n");
            return HI_FAILURE;
        }

        s32Ret = tuner_get_adc_data(u32TunerId, u32DmxPort,u32TimeSec);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_TUNER("call tuner_get_adc_data err.\n");
            return HI_FAILURE;
        }
    }

    return count;
}












#endif

#ifndef MODULE
#if (1 == HI_PROC_SUPPORT)
EXPORT_SYMBOL(tuner_proc_read);
EXPORT_SYMBOL(tuner_proc_read_reg);
EXPORT_SYMBOL(tuner_proc_write);
#endif
#endif
