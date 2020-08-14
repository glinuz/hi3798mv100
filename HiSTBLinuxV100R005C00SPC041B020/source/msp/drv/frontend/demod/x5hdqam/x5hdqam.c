/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi3130.c
 * Description:
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16   w54542    NULL                Create this file.
 ***********************************************************************************/

#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <asm/io.h>

#include "hi_type.h"
#include "drv_i2c_ext.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "hi_unf_i2c.h"

#define SYMRATE_DEVISION 4800   /* KHz */

extern wait_queue_head_t g_QamSpecialProcessWQ;

/* CD1616 X5HDQAM CONFIG */
static REGVALUE_S s_stCd1616Qam16[]=
{
#include "qamdata/CD1616/CD1616_QAM16_X5HD.txt"
};

static REGVALUE_S s_stCd1616Qam32[]=
{
#include "qamdata/CD1616/CD1616_QAM32_X5HD.txt"
};

static REGVALUE_S s_stCd1616Qam64[]=
{
#include "qamdata/CD1616/CD1616_QAM64_X5HD.txt"
};

static REGVALUE_S s_stCd1616Qam128[]=
{
#include "qamdata/CD1616/CD1616_QAM128_X5HD.txt"
};

static REGVALUE_S s_stCd1616Qam256[]=
{
#include "qamdata/CD1616/CD1616_QAM256_X5HD.txt"
};



static REGVALUE_S *s_astCd1616QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stCd1616Qam16, s_stCd1616Qam32, s_stCd1616Qam64, s_stCd1616Qam128, s_stCd1616Qam256
};


/* CD1616_DOUBLE X5HDQAM CONFIG */
static REGVALUE_S s_stCd1616DoubleQam16[]=
{
#include "qamdata/CD1616_DOUBLE/CD1616_DOUBLE_QAM16_X5HD.txt"
};

static REGVALUE_S s_stCd1616DoubleQam32[]=
{
#include "qamdata/CD1616_DOUBLE/CD1616_DOUBLE_QAM32_X5HD.txt"
};

static REGVALUE_S s_stCd1616DoubleQam64[]=
{
#include "qamdata/CD1616_DOUBLE/CD1616_DOUBLE_QAM64_X5HD.txt"
};

static REGVALUE_S s_stCd1616DoubleQam128[]=
{
#include "qamdata/CD1616_DOUBLE/CD1616_DOUBLE_QAM128_X5HD.txt"
};

static REGVALUE_S s_stCd1616DoubleQam256[]=
{
#include "qamdata/CD1616_DOUBLE/CD1616_DOUBLE_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astCd1616DoubleQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stCd1616DoubleQam16, s_stCd1616DoubleQam32, s_stCd1616DoubleQam64, s_stCd1616DoubleQam128, s_stCd1616DoubleQam256
};


/* ALPS_TDAE X5HDQAM CONFIG */
static REGVALUE_S s_stAlpsTdaeQam16[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM16_X5HD.txt"
};

static REGVALUE_S s_stAlpsTdaeQam32[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM32_X5HD.txt"
};

static REGVALUE_S s_stAlpsTdaeQam64[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM64_X5HD.txt"
};

static REGVALUE_S s_stAlpsTdaeQam128[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM128_X5HD.txt"
};

static REGVALUE_S s_stAlpsTdaeQam256[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astAlpsTdaeQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stAlpsTdaeQam16, s_stAlpsTdaeQam32, s_stAlpsTdaeQam64, s_stAlpsTdaeQam128, s_stAlpsTdaeQam256
};



/* TDA18250 X5HDQAM CONFIG */
static REGVALUE_S s_stTda18250Qam16[]=
{
#include "qamdata/TDA18250/TDA18250_QAM16_X5HD.txt"
};

static REGVALUE_S s_stTda18250Qam32[]=
{
#include "qamdata/TDA18250/TDA18250_QAM32_X5HD.txt"
};

static REGVALUE_S s_stTda18250Qam64[]=
{
#include "qamdata/TDA18250/TDA18250_QAM64_X5HD.txt"
};

static REGVALUE_S s_stTda18250Qam128[]=
{
#include "qamdata/TDA18250/TDA18250_QAM128_X5HD.txt"
};

static REGVALUE_S s_stTda18250Qam256[]=
{
#include "qamdata/TDA18250/TDA18250_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astTda18250QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTda18250Qam16, s_stTda18250Qam32, s_stTda18250Qam64, s_stTda18250Qam128, s_stTda18250Qam256
};


/* TMX70707 X5HDQAM CONFIG */
static REGVALUE_S s_stTmx70707Qam16[]=
{
#include "qamdata/TMX70707/TMX70707_QAM16_X5HD.txt"
};

static REGVALUE_S s_stTmx70707Qam32[]=
{
#include "qamdata/TMX70707/TMX70707_QAM32_X5HD.txt"
};

static REGVALUE_S s_stTmx70707Qam64[]=
{
#include "qamdata/TMX70707/TMX70707_QAM64_X5HD.txt"
};

static REGVALUE_S s_stTmx70707Qam128[]=
{
#include "qamdata/TMX70707/TMX70707_QAM128_X5HD.txt"
};

static REGVALUE_S s_stTmx70707Qam256[]=
{
#include "qamdata/TMX70707/TMX70707_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astTmx70707QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTmx70707Qam16, s_stTmx70707Qam32, s_stTmx70707Qam64, s_stTmx70707Qam128, s_stTmx70707Qam256
};

/* TDCC X5HDQAM CONFIG */
static REGVALUE_S s_stTdccQam16[]=
{
#include "qamdata/TDCC/TDCC_QAM16_X5HD.txt"
};

static REGVALUE_S s_stTdccQam32[]=
{
#include "qamdata/TDCC/TDCC_QAM32_X5HD.txt"
};

static REGVALUE_S s_stTdccQam64[]=
{
#include "qamdata/TDCC/TDCC_QAM64_X5HD.txt"
};

static REGVALUE_S s_stTdccQam128[]=
{
#include "qamdata/TDCC/TDCC_QAM128_X5HD.txt"
};

static REGVALUE_S s_stTdccQam256[]=
{
#include "qamdata/TDCC/TDCC_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astTdccQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTdccQam16, s_stTdccQam32, s_stTdccQam64, s_stTdccQam128, s_stTdccQam256
};


/* XG_3BL X5HDQAM CONFIG */
static REGVALUE_S s_stXg3blQam16[]=
{
#include "qamdata/XG_3BL/XG_3BL_QAM16_X5HD.txt"
};

static REGVALUE_S s_stXg3blQam32[]=
{
#include "qamdata/XG_3BL/XG_3BL_QAM32_X5HD.txt"
};

static REGVALUE_S s_stXg3blQam64[]=
{
#include "qamdata/XG_3BL/XG_3BL_QAM64_X5HD.txt"
};

static REGVALUE_S s_stXg3blQam128[]=
{
#include "qamdata/XG_3BL/XG_3BL_QAM128_X5HD.txt"
};

static REGVALUE_S s_stXg3blQam256[]=
{
#include "qamdata/XG_3BL/XG_3BL_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astXg3blQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stXg3blQam16, s_stXg3blQam32, s_stXg3blQam64, s_stXg3blQam128, s_stXg3blQam256
};

/* MT2081 X5HDQAM CONFIG */
static REGVALUE_S s_stMt2081Qam16[]=
{
#include "qamdata/MT2081/MT2081_QAM16_X5HD.txt"
};

static REGVALUE_S s_stMt2081Qam32[]=
{
#include "qamdata/MT2081/MT2081_QAM32_X5HD.txt"
};

static REGVALUE_S s_stMt2081Qam64[]=
{
#include "qamdata/MT2081/MT2081_QAM64_X5HD.txt"
};

static REGVALUE_S s_stMt2081Qam128[]=
{
#include "qamdata/MT2081/MT2081_QAM128_X5HD.txt"
};

static REGVALUE_S s_stMt2081Qam256[]=
{
#include "qamdata/MT2081/MT2081_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astMt2081QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stMt2081Qam16, s_stMt2081Qam32, s_stMt2081Qam64, s_stMt2081Qam128, s_stMt2081Qam256
};

/* R820C X5QAM CONFIG */
static REGVALUE_S s_stR820CQam16[]=
{
#include "qamdata/R820C/R820C_QAM16_X5HD.txt"
};

static REGVALUE_S s_stR820CQam32[]=
{
#include "qamdata/R820C/R820C_QAM32_X5HD.txt"
};

static REGVALUE_S s_stR820CQam64[]=
{
#include "qamdata/R820C/R820C_QAM64_X5HD.txt"
};

static REGVALUE_S s_stR820CQam128[]=
{
#include "qamdata/R820C/R820C_QAM128_X5HD.txt"
};

static REGVALUE_S s_stR820CQam256[]=
{
#include "qamdata/R820C/R820C_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astR820CQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stR820CQam16, s_stR820CQam32, s_stR820CQam64, s_stR820CQam128, s_stR820CQam256
};

/* MXL203 X5HD CONFIG */
static REGVALUE_S s_stMxl203Qam16[]=
{
#include "qamdata/MXL203/MXL203_QAM16_X5HD.txt"
};

static REGVALUE_S s_stMxl203Qam32[]=
{
#include "qamdata/MXL203/MXL203_QAM32_X5HD.txt"
};

static REGVALUE_S s_stMxl203Qam64[]=
{
#include "qamdata/MXL203/MXL203_QAM64_X5HD.txt"
};

static REGVALUE_S s_stMxl203Qam128[]=
{
#include "qamdata/MXL203/MXL203_QAM128_X5HD.txt"
};

static REGVALUE_S s_stMxl203Qam256[]=
{
#include "qamdata/MXL203/MXL203_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astMxl203QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stMxl203Qam16, s_stMxl203Qam32, s_stMxl203Qam64, s_stMxl203Qam128, s_stMxl203Qam256
};

/* TDA18250B X5HDQAM CONFIG */
static REGVALUE_S s_stTda18250bQam16[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM16_X5HD.txt"
};

static REGVALUE_S s_stTda18250bQam32[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM32_X5HD.txt"
};

static REGVALUE_S s_stTda18250bQam64[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM64_X5HD.txt"
};

static REGVALUE_S s_stTda18250bQam128[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM128_X5HD.txt"
};

static REGVALUE_S s_stTda18250bQam256[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM256_X5HD.txt"
};

static REGVALUE_S *s_astTda18250bQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTda18250bQam16, s_stTda18250bQam32, s_stTda18250bQam64, s_stTda18250bQam128, s_stTda18250bQam256
};

static char *s_QamRegName[]=
{
#include "qamdata/QAMPRINT_X5HD.txt"
};


#define X5HDQAM_I2C_CHECK(tunerport) \
    do\
    {\
        HI_U8 u8Tmp = 0;\
        if (HI_SUCCESS != qam_read_byte(tunerport, AGC_CTRL_1_ADDR, &u8Tmp))\
        {\
            HI_ERR_TUNER("x5hdqam_i2c_check failure,please check i2c:%d\n", g_stTunerOps[tunerport].enI2cChannel);\
            return HI_FAILURE;\
        }\
    } while (0)

static HI_VOID set_symrate(HI_U32 u32TunerPort, HI_U32 u32SymRate)
{
    HI_U32 u32Temp;/*= (HI_U32)(M_CLK * 8192 / nSymRate)*/
    HI_U8  u8Temp = 0;

    u32Temp = (HI_U32)(g_stTunerOps[u32TunerPort].u32XtalClk * 8192 / u32SymRate);


    qam_read_byte(u32TunerPort, TR_CTRL_8_ADDR, &u8Temp);

    u8Temp = (u8Temp & 0xf8) | (u32Temp & 0x70000) >> 16;

    qam_write_byte(u32TunerPort, TR_CTRL_8_ADDR, u8Temp);
    qam_write_byte(u32TunerPort, TR_CTRL_9_ADDR, (u32Temp & 0xff00) >> 8);
    qam_write_byte(u32TunerPort, TR_CTRL_10_ADDR, u32Temp & 0xff);

    switch ( g_stTunerOps[u32TunerPort].u32CurrQamMode )
    {
        case QAM_TYPE_64:
            if ( u32SymRate < SYMRATE_DEVISION )
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x02);
            }
            else
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x08);
            }

            break;

        case QAM_TYPE_128:
            if ( u32SymRate < SYMRATE_DEVISION )
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x01);
            }
            else
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x05);
            }

            break;

        case QAM_TYPE_256:
            if ( u32SymRate < SYMRATE_DEVISION )
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x00);
            }
            else
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x06);
            }

            break;

        default:
            break;

    }
}

HI_VOID x5hdqam_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest )
{
    HI_S32 s32Ret = 0;
    HI_U8  u8RegVal = 0;
    HI_U8  au8Tmp[2] = {0};


    s32Ret = qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8RegVal);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("i2c operation error\n");
        return;
    }

    if ( 0xf8 == u8RegVal )
    {
        pstAgcTest->bLockFlag = HI_TRUE;
        pstAgcTest->bAgcLockFlag = HI_TRUE;
    }
    else if ( 1 == ( u8RegVal & 0x80 ) )
    {
        pstAgcTest->bAgcLockFlag = HI_TRUE;
    }

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 0x1);

    qam_read_byte(u32TunerPort, BAGC_STAT_3_ADDR, &au8Tmp[0]);
    qam_read_byte(u32TunerPort, BAGC_STAT_4_ADDR, &au8Tmp[1]);
    pstAgcTest->u32Agc2 = ((au8Tmp[0]) << 4) | (( au8Tmp[1] >> 4) & 0x0f );

    qam_read_byte(u32TunerPort, BAGC_CTRL_12_ADDR, &pstAgcTest->u8BagcCtrl12 );

    return;

}

static HI_VOID x5hdqam_chip_reset(HI_U32 u32TunerPort, qam_module_e enResetModule)
{
    qam_write_byte(u32TunerPort, AGC_CTRL_3_ADDR, 0x20);

    qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0xff);
    qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);

    /*if (ALL_CHIP == enResetModule)
    {
        qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0xff);
        qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);
    }
    else
    {
        qam_write_bit(u32TunerPort, MCTRL_1_ADDR, (HI_U8)enResetModule, 1);
        qam_write_bit(u32TunerPort, MCTRL_1_ADDR, (HI_U8)enResetModule, 0);
    }*/

    qam_write_byte(u32TunerPort, AGC_CTRL_3_ADDR, 0xef);

}

/*note:this function should not be deleted,reserved for debug*/
HI_VOID x5hdqam_get_registers(HI_U32 u32TunerPort, void *p)
{
    HI_U8   u8Value = 0;
    HI_S32  i = 0;
    HI_S32  s32RegSum = 0;
    HI_U32  u32QamType = 0;
    REGVALUE_S *pstReg = HI_NULL;

    u32QamType = g_stTunerOps[u32TunerPort].u32CurrQamMode;
    pstReg = s_astCd1616QamRegs[u32QamType];
    s32RegSum = sizeof(s_stCd1616Qam64)/sizeof(REGVALUE_S);

    for( i = 0; i < s32RegSum; i++ )
    {
        if ( 0 == i % 3 )
        {
            PROC_PRINT(p, "\n");
        }
        qam_read_byte(u32TunerPort, pstReg->u8Addr, &u8Value);
        pstReg++;
        PROC_PRINT(p, "{%s  ,0x%02x },  ", s_QamRegName[i], u8Value);
    }
    PROC_PRINT(p, "\n\n");

}

static HI_VOID x5hdqam_set_customreg_value(HI_U32 u32TunerPort, HI_BOOL bInversion, HI_U32 u32SymbolRate)
{

    HI_U8 u8Val;
    HI_U32 u32RegVal;
    HI_U8 * pu8VirAddr = HI_NULL;

    /*set adc type*/
    if ( QAM_AD_OUTSIDE == g_stTunerOps[u32TunerPort].u8AdcType )
    {
        qam_write_bit(u32TunerPort, MCTRL_8_ADDR, 7, 1);
    }
    else
    {
        qam_write_bit(u32TunerPort, MCTRL_8_ADDR, 7, 0);
    }


    if ( QAM_AGC_CMOS_OUTPUT == g_stTunerOps[u32TunerPort].u8AgcOutputSel )
    {
        qam_write_bit( u32TunerPort, MCTRL_5_ADDR, 7, 0 );
        qam_write_bit( u32TunerPort, MCTRL_8_ADDR, 2, 0 );
    }
    else    /* OD */
    {
        qam_write_bit( u32TunerPort, MCTRL_5_ADDR, 7, 1 );
        qam_write_bit( u32TunerPort, MCTRL_8_ADDR, 2, 1 );
    }

    pu8VirAddr = (HI_U8 *)ioremap_nocache(0x101f50a0, 0x10);
    if(HI_NULL == pu8VirAddr)
    {
        HI_ERR_TUNER("ioremap_nocache map error.\n");
        return;
    }
    HI_REG_READ(pu8VirAddr, u32RegVal);
    if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50 == g_stTunerOps[u32TunerPort].enTsType)
    {
        u32RegVal = (u32RegVal & ~(0x1 << 17)) | (0X1 << 18);/*QAM TSCLK 50M*/
        HI_REG_WRITE(pu8VirAddr, u32RegVal);
    }
    else if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL == g_stTunerOps[u32TunerPort].enTsType)
    {
        u32RegVal = u32RegVal  | (0X3 << 17);/*QAM TSCLK 74.25M*/
        HI_REG_WRITE(pu8VirAddr, u32RegVal);
    }
    else
    {
        u32RegVal = u32RegVal  & ~(0X3 << 17);/*QAM TSCLK 13.5M*/
        HI_REG_WRITE(pu8VirAddr, u32RegVal);
    }

    iounmap(pu8VirAddr);
    
    /*ts type control*/
    switch (g_stTunerOps[u32TunerPort].enTsType)
    {
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
        {
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 6, 1);  /* dvb */
            qam_write_bit(u32TunerPort, TS_CTRL_1_ADDR, 3, 0);  /* no  bserial */
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 4, 0);
            break;
    }
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50:
        {
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 6, 0);  /* not dvb */
            qam_write_bit(u32TunerPort, TS_CTRL_1_ADDR, 3, 1);  /* bserial */
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 4, 1);
            break;
        }
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
        {
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 6, 0);  /* not dvb */
            qam_write_bit(u32TunerPort, TS_CTRL_1_ADDR, 3, 0);  /* no bserial */
            qam_write_bit(u32TunerPort, TS_CTRL_2_ADDR, 4, 0);
            break;
        }
        default:
            break;
    }

    /*inversion control */
    if(bInversion)
    {
        qam_write_bit(u32TunerPort, BS_CTRL_1_ADDR, 3, 1);
    }
    else
    {
        qam_write_bit(u32TunerPort, BS_CTRL_1_ADDR, 3, 0);
    }

    set_symrate(u32TunerPort, u32SymbolRate);


    qam_read_byte(u32TunerPort, MCTRL_5_ADDR, &u8Val);

    if(g_stTunerOps[u32TunerPort].u8AdcDataFmt == 0)
    {
        u8Val &= 0xF7;
    }
    else
    {
        u8Val |= 0x08;
    }

    qam_write_byte(u32TunerPort, MCTRL_5_ADDR, u8Val);

}

static HI_VOID x5hdqam_set_reg_value(HI_U32 u32TunerPort)
{
    HI_S32 i = 0;
    HI_S32 s32RegSum = 0;
    HI_U32 u32QamType = 0;
    REGVALUE_S *pstReg = HI_NULL;


    /*set for qam 16/32/64/128/256*/
    u32QamType = g_stTunerOps[u32TunerPort].u32CurrQamMode;

    switch( g_stTunerOps[u32TunerPort].enTunerDevType )
    {
        case HI_UNF_TUNER_DEV_TYPE_CD1616:
        {
            pstReg = s_astCd1616QamRegs[u32QamType];
            break;
        }
        case HI_UNF_TUNER_DEV_TYPE_CD1616_DOUBLE:
        {
            pstReg = s_astCd1616DoubleQamRegs[u32QamType];
		    break;
	    }
        case HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE:
        {		
        	pstReg = s_astAlpsTdaeQamRegs[u32QamType];
			break;
        }
        case HI_UNF_TUNER_DEV_TYPE_TDA18250:
        {			
        	pstReg = s_astTda18250QamRegs[u32QamType];
        	break;
        }	     
        case HI_UNF_TUNER_DEV_TYPE_TMX7070X:
        {		
			pstReg = s_astTmx70707QamRegs[u32QamType];
        	break;
        }	
        case HI_UNF_TUNER_DEV_TYPE_TDCC:
        {	
        	pstReg = s_astTdccQamRegs[u32QamType];
        	break;
        }	

        case HI_UNF_TUNER_DEV_TYPE_XG_3BL:
        {
        	pstReg = s_astXg3blQamRegs[u32QamType];
        	break;
        }

        case HI_UNF_TUNER_DEV_TYPE_MT2081:
        {			
        	pstReg = s_astMt2081QamRegs[u32QamType];
        	break;
        } 
        
        case HI_UNF_TUNER_DEV_TYPE_R820C:
        {			
        	pstReg = s_astR820CQamRegs[u32QamType];
        	break;
        }
        case HI_UNF_TUNER_DEV_TYPE_MXL203:
        {			
        	pstReg = s_astMxl203QamRegs[u32QamType];
        	break;
        } 
        case HI_UNF_TUNER_DEV_TYPE_TDA18250B:
        {			
        	pstReg = s_astTda18250bQamRegs[u32QamType];
        	break;
        }
		
        default:
        {
        	pstReg = s_astCd1616QamRegs[u32QamType];
    		break;
        } 	
    } 
  
    s32RegSum = sizeof(s_stCd1616Qam128)/sizeof(REGVALUE_S);

    for(i=0; i<s32RegSum; i++)
    {
        qam_write_byte(u32TunerPort, pstReg->u8Addr, pstReg->u8Value);
        pstReg++;
    }
	return;
}

HI_S32 x5hdqam_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U8  u8Status = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i = 0;


    HI_TUNER_CHECKPOINTER( penTunerStatus);
    X5HDQAM_I2C_CHECK(u32TunerPort);

    for (i = 0; i < 4; i++)  /* prevent twittering */
    {
        s32Ret = qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8Status);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER( "i2c operation error\n");
            return HI_FAILURE;
        }

        if (0xF8 == (u8Status & 0xF8))
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;

            break;
        }
        else
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        }
    }

    return HI_SUCCESS;
}

#define X5HD_CONNCECT_TIMEOUT	1000		/* ms */
#define X5HD_CLOSE_EQU_TIME	400		        /* ms */
HI_VOID x5hdqam_manage_after_chipreset(HI_U32 u32TunerPort)
{
	HI_S32  s32Cnt = 0;
	HI_BOOL bStartFlag = HI_FALSE;
	HI_U8   u8IsReset = 0;

	HI_U8  u8LockRegVal = 0;
	HI_U8  u8Tmp = 0;
	HI_U8  u8Reverse = 0;
	HI_U8  i = 0;
	HI_U8  u8QamRegVal;
	HI_U32 u32Timeout = 0;

	s32Cnt = 0;
	bStartFlag = HI_FALSE;
    
	#define X5HD_CONNECT_CHECK_FREQ	20		/* ms */  

	if (g_stTunerOps[u32TunerPort].enTunerDevType == HI_UNF_TUNER_DEV_TYPE_CD1616)
    {	
		if ((QAM_TYPE_256 == g_stTunerOps[u32TunerPort].u32CurrQamMode) ||
	        (QAM_TYPE_128 == g_stTunerOps[u32TunerPort].u32CurrQamMode))
		{	
			while (u32Timeout < X5HD_CONNCECT_TIMEOUT)
			{
				qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8LockRegVal);

				if (0xf8 == u8LockRegVal)
				{
					mdelay(5);
					
					qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8LockRegVal);

					if (0xf8 != u8LockRegVal)
					{
						continue;
					}
				
					reset_special_process_flag(HI_TRUE);
					wake_up_interruptible(&g_QamSpecialProcessWQ);

					break;

				}
				else
				{
					qam_read_byte(u32TunerPort, QAM_DEBUG_1_ADDR, &u8Tmp);
					u8Tmp = u8Tmp & 0x7;
					
					if (u8Tmp >= 0x3)
					{
						bStartFlag = HI_TRUE;
					}

					if (bStartFlag)
					{				
						s32Cnt++;
					}

					/* 100 ms after start_flag == 1 */
					if ((5 == s32Cnt) && (QAM_TYPE_256 == g_stTunerOps[u32TunerPort].u32CurrQamMode))
					{
						if (1 == u8Reverse)
						{
							qam_write_byte(u32TunerPort, EQU_CTRL_3_ADDR, 0x33);
						}
						else
						{
							qam_write_byte(u32TunerPort, EQU_CTRL_3_ADDR, 0x45);
						}
						
						u8Reverse = 1 - u8Reverse;

						/* reset eq */
						qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x08);
						qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);
						
						
						bStartFlag = HI_FALSE;
						s32Cnt = 0;
						
					}
					else if ((11 == s32Cnt) && (QAM_TYPE_128 == g_stTunerOps[u32TunerPort].u32CurrQamMode))
					{
						/* reset eq */
						qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x08);
						qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);
										
						bStartFlag = HI_FALSE;
						s32Cnt = 0;					

					}

					msleep_interruptible(X5HD_CONNECT_CHECK_FREQ);
					u32Timeout = u32Timeout + X5HD_CONNECT_CHECK_FREQ;
					
				}
			} /* while */
		}
    }
	
    else
    {
		if ((QAM_TYPE_256 == g_stTunerOps[u32TunerPort].u32CurrQamMode) ||
			(QAM_TYPE_128 == g_stTunerOps[u32TunerPort].u32CurrQamMode))
		{

			while  (u32Timeout < X5HD_CONNCECT_TIMEOUT)
			{
				qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8LockRegVal);

				if (0xf8 == u8LockRegVal)
				{	
					mdelay(5);
					
					qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8LockRegVal);

					if (0xf8 != u8LockRegVal)
					{
						continue;
					}
				
					/*reset_special_process_flag(HI_TRUE);
					wake_up_interruptible(&g_QamSpecialProcessWQ);*/
					
					break;

				}
				else
				{
					qam_read_byte(u32TunerPort, QAM_DEBUG_1_ADDR, &u8Tmp);
					if (u32Timeout > X5HD_CLOSE_EQU_TIME)
					{	
						u8Tmp = u8Tmp & 0x7;
					
						if (u8Tmp >= 0x3)
						{	
							bStartFlag = HI_TRUE;
						}
						
						if (u8IsReset == 0)
						{	
							qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x08);
							qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);
								
							bStartFlag = HI_FALSE;
							s32Cnt = 0;
								
							u8IsReset = 1;
						}
							
						if (bStartFlag)
						{				
							s32Cnt++;
						}
							
						if ((20 <= s32Cnt) && (u8Tmp >= 0x3))
						{	
							u8IsReset = 0;
						}
						
						qam_read_byte(u32TunerPort, EQU_CTRL_4_ADDR, &u8QamRegVal);
						u8QamRegVal = u8QamRegVal & 0xFB;
						qam_write_byte(u32TunerPort, EQU_CTRL_4_ADDR, u8QamRegVal);/* Fix the Tap */

						for (i = 27; i < 44; i++)
			        	{	
							qam_write_byte(u32TunerPort, EQU_CTRL_6_ADDR, (i + 0x40));
							qam_write_byte(u32TunerPort, EQU_CTRL_7_ADDR, 0);
							qam_write_byte(u32TunerPort, EQU_CTRL_8_ADDR, 0);
							qam_write_byte(u32TunerPort, EQU_CTRL_9_ADDR, 0);
							qam_write_byte(u32TunerPort, EQU_CTRL_10_ADDR, 0);
						}
						
						u8QamRegVal = u8QamRegVal | 0x04;
						qam_write_byte(u32TunerPort, EQU_CTRL_4_ADDR, u8QamRegVal); /* Tap Auto Update */		
					}

					msleep_interruptible( X5HD_CONNECT_CHECK_FREQ );
					u32Timeout = u32Timeout + X5HD_CONNECT_CHECK_FREQ;
					
				}
			} /* while */
		}
    }
	return;
}

static HI_U32 s_u32X5hdConnectTimeout = X5HD_CONNCECT_TIMEOUT;
HI_VOID x5hdqam_connect_timeout(HI_U32 u32ConnectTimeout)
{
    s_u32X5hdConnectTimeout = u32ConnectTimeout;
    return;
}
HI_S32 x5hdqam_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{

	HI_S32 s32FuncRet = HI_SUCCESS;
    HI_U32 u32SymbolRate = 0;
	
	reset_special_process_flag(HI_FALSE);

    HI_TUNER_CHECKPOINTER( pstChannel);
    X5HDQAM_I2C_CHECK(u32TunerPort);

    g_stTunerOps[u32TunerPort].u32CurrQamMode = pstChannel->enQamType;
    u32SymbolRate = pstChannel->unSRBW.u32SymbolRate / 1000; /* KHz */

	/*step 1:config tuner register*/
    s32FuncRet = (*g_stTunerOps[u32TunerPort].set_tuner)(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, pstChannel->u32Frequency);
    if (HI_SUCCESS != s32FuncRet)
    {
        HI_ERR_TUNER( "set tuner error\n");
        return s32FuncRet;
    }

    /*step 2:config qam register */
    x5hdqam_set_reg_value(u32TunerPort);	 /*set for qam and tunner*/	  

    /*set for different usr request about symbolrate, ts type, demo in or out,  inversion or not*/
    x5hdqam_set_customreg_value(u32TunerPort, pstChannel->bSI, u32SymbolRate);	  

    /*step 3:reset chip*/
    x5hdqam_chip_reset(u32TunerPort, ALL_CHIP);

    if(s_u32X5hdConnectTimeout < X5HD_CLOSE_EQU_TIME)
    {
        s_u32X5hdConnectTimeout = X5HD_CONNCECT_TIMEOUT;
        return HI_SUCCESS;
    }

    if (g_stTunerOps[u32TunerPort].manage_after_chipreset)
        g_stTunerOps[u32TunerPort].manage_after_chipreset(u32TunerPort);
   
    return HI_SUCCESS;
}

HI_S32 x5hdqam_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    HI_U8 au8RegVal[2] = {0};
	HI_U32 u32TmpSigStrength = 0;
	HI_U32 i = 0;

    HI_TUNER_CHECKPOINTER( pu32SignalStrength);
    X5HDQAM_I2C_CHECK(u32TunerPort);


    for ( i = 0; i < 64; i++ )
    {
	    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 0x1);

		qam_read_byte(u32TunerPort, BAGC_STAT_3_ADDR, &au8RegVal[0]);

		qam_read_byte(u32TunerPort, BAGC_STAT_4_ADDR, &au8RegVal[1]);
		
		pu32SignalStrength[1] = ( au8RegVal[0]  << 4 ) | ( (au8RegVal[1] >> 4) & 0x0f );
		
		u32TmpSigStrength += pu32SignalStrength[1];
   
    }

	u32TmpSigStrength =	u32TmpSigStrength >> 6;
    pu32SignalStrength[1] = u32TmpSigStrength;

    if(g_stTunerOps[u32TunerPort].recalculate_signal_strength)
    {
        g_stTunerOps[u32TunerPort].recalculate_signal_strength(u32TunerPort, pu32SignalStrength);
    }

    return HI_SUCCESS;
}

HI_S32 x5hdqam_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32Ber)
{
    HI_U32 u32BerThres = 4; /* modify by chenxu 2008-01-15 AI7D02092 */
    HI_U8  u8Tmp	 = 0;
    HI_U8  u8Val     = 0;
    HI_S32 i         = 0;

    HI_TUNER_CHECKPOINTER( pu32Ber);
    X5HDQAM_I2C_CHECK(u32TunerPort);

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 0); /* disable */

    u8Val = (u32BerThres) << 2;
    qam_write_byte( u32TunerPort, BER_1_ADDR, u8Val );

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 1); /* enable */

    for (i = 0; i < 40; i++)
    {
        msleep(10); 
        qam_read_bit(u32TunerPort, BER_1_ADDR, 7, &u8Tmp);
        if (0 == u8Tmp)
        {
            break;
        }
    }

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

    if (i >= 40)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, BER_2_ADDR, &u8Val);
    pu32Ber[0] = u8Val;
    qam_read_byte(u32TunerPort, BER_3_ADDR, &u8Val);
    pu32Ber[1] = u8Val;
    qam_read_byte(u32TunerPort, BER_4_ADDR, &u8Val);
    pu32Ber[2] = u8Val;

	return HI_SUCCESS;
}

HI_S32 x5hdqam_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs)
{
    HI_U32 u32BerThres = 4; 
    HI_U8  u8Tmp     = 0;
    HI_U8  u8Val     = 0;
    HI_S32 i         = 0;

	HI_U8  u8Addr    = CR_CTRL_21_ADDR;
	HI_U8  u8RegValue  = 1;
	HI_U32 u8RegValue1 = 0;
	HI_U32 u8RegValue2 = 0;
	HI_U32 u8RegValue3 = 0;

    X5HDQAM_I2C_CHECK(u32TunerPort);

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 0); /* disable */ 

    u8Val = (u32BerThres) << 2;
    qam_write_byte( u32TunerPort, BER_1_ADDR, u8Val );

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 1); /* enable */

    for (i = 0; i < 40; i++)
    {
        msleep(10); 
        qam_read_bit(u32TunerPort, BER_1_ADDR, 7, &u8Tmp);
        if (0 == u8Tmp)
        {
            break;
        }
    }

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

    if (i >= 40)
    {
        return HI_FAILURE;
    }

	/* rs package,calculate RS error  package*/
	qam_write_byte(u32TunerPort, u8Addr,u8RegValue);

	qam_read_byte(u32TunerPort, RS_CTRL_1_ADDR, &u8Tmp);	
	qam_read_byte(u32TunerPort, RS_CTRL_2_ADDR, &u8Val);
	u8RegValue1 = ( u8Tmp << 8 ) | u8Val;
	
	qam_read_byte(u32TunerPort, RS_CTRL_3_ADDR, &u8Tmp);	
	qam_read_byte(u32TunerPort, RS_CTRL_4_ADDR, &u8Val);
	u8RegValue2 = ( u8Tmp << 8 ) | u8Val;

	qam_read_byte(u32TunerPort, RS_CTRL_5_ADDR, &u8Tmp);	
	qam_read_byte(u32TunerPort, RS_CTRL_6_ADDR, &u8Val);
	u8RegValue3 = ( u8Tmp << 8 ) | u8Val;

	qam_read_byte( u32TunerPort, RS_CTRL_8_ADDR, &u8RegValue );

	pu32Rs[0] = u8RegValue1;
	pu32Rs[1] = u8RegValue2;
	pu32Rs[2] = u8RegValue3;

	return HI_SUCCESS;
}


HI_S32 x5hdqam_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
	HI_U8  u8Tmp = 0;
    HI_U32 u32Snr = 0;
    HI_U32 i ;
    HI_U32 u32SumSnr = 0;	

    HI_TUNER_CHECKPOINTER( pu32SNR);
    X5HDQAM_I2C_CHECK(u32TunerPort);

    for (i=0;i<64;i++)
    {
		qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

		qam_read_byte(u32TunerPort, EQU_STAT_2_ADDR, &u8Tmp);
		u32Snr = u8Tmp << 8;
		qam_read_byte(u32TunerPort, EQU_STAT_3_ADDR, &u8Tmp);
		u32Snr  += u8Tmp;
		u32SumSnr += u32Snr;
	}
	
	u32Snr = 	u32SumSnr >> 6;
    *pu32SNR = u32Snr;

    return HI_SUCCESS;
}

HI_S32 x5hdqam_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}


HI_S32 x5hdqam_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb )
{
	HI_U8  au8Freq[4] = { 0 };
	HI_S32 s32RealFreq = 0;
	HI_U8  au8Symb[2] = { 0 };
	HI_S32 s32RealSymb = 0;

    HI_TUNER_CHECKPOINTER( pu32Freq);
	HI_TUNER_CHECKPOINTER( pu32Symb);
    X5HDQAM_I2C_CHECK(u32TunerPort);

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 0x1);


	/* calc freq */
	qam_read_byte(u32TunerPort, CR_STAT_4_ADDR, &au8Freq[0]);
	au8Freq[0] = au8Freq[0] & 0x0F;
	
	qam_read_byte(u32TunerPort, CR_STAT_5_ADDR, &au8Freq[1]);
	qam_read_byte(u32TunerPort, CR_STAT_6_ADDR, &au8Freq[2]);		
	qam_read_byte(u32TunerPort, CR_STAT_7_ADDR, &au8Freq[3]);

	s32RealFreq = (au8Freq[0]<<24) + (au8Freq[1]<<16) + (au8Freq[2]<<8) + au8Freq[3] ;

	*pu32Freq = s32RealFreq;

	/* calc symbolrate */
	qam_read_byte(u32TunerPort, TR_STAT_1_ADDR, &au8Symb[0]);
	
	qam_read_byte(u32TunerPort, TR_STAT_2_ADDR, &au8Symb[1]);	

	s32RealSymb = au8Symb[0]*256 + au8Symb[1];
	
    *pu32Symb = s32RealSymb;
	
    return HI_SUCCESS;
}
