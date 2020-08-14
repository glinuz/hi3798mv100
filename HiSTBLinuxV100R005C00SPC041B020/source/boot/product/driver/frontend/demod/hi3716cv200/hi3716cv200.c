/***********************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi3130.c
 * Description:
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16   w54542    NULL                Create this file.
 ***********************************************************************************/

/*#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <asm/io.h>*/

//#include "common_sys.h"
#include "hi_type.h"
#include "hi_reg.h"
//#include "drv_i2c_ext.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
//#include "hi_drv_proc.h"
//#include "hi_drv_sys.h"
//#include "drv_tuner_ext.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"

#include <common.h>

#define SYMRATE_DEVISION 4800   /* KHz */

int gettime_ms(void)        {
  /* <porting> Replace 'clock' by whatever is necessary to return the system time in milliseconds */
  //return (int)clock()*1000/CLOCKS_PER_SEC;
		unsigned long sec;
  		unsigned short msec;
		unsigned short usec;
  
		do_gettime(&sec,&msec,&usec);
			
		return (int)(msec +sec*1000 );
}


//extern wait_queue_head_t g_QamSpecialProcessWQ;

/* TDA18250 HI3130V200 CONFIG */
static REGVALUE_S s_stTda18250Qam16[]=
{
#include "qamdata/TDA18250/TDA18250_QAM16_V200.txt"
};

static REGVALUE_S s_stTda18250Qam32[]=
{
#include "qamdata/TDA18250/TDA18250_QAM32_V200.txt"
};

static REGVALUE_S s_stTda18250Qam64[]=
{
#include "qamdata/TDA18250/TDA18250_QAM64_V200.txt"
};

static REGVALUE_S s_stTda18250Qam128[]=
{
#include "qamdata/TDA18250/TDA18250_QAM128_V200.txt"
};

static REGVALUE_S s_stTda18250Qam256[]=
{
#include "qamdata/TDA18250/TDA18250_QAM256_V200.txt"
};

static REGVALUE_S *s_astTda18250QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTda18250Qam16, s_stTda18250Qam32, s_stTda18250Qam64, s_stTda18250Qam128, s_stTda18250Qam256
};

/* TDA18250B HI3130V200 CONFIG */
static REGVALUE_S s_stTda18250bQam16[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM16_V200.txt"
};

static REGVALUE_S s_stTda18250bQam32[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM32_V200.txt"
};

static REGVALUE_S s_stTda18250bQam64[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM64_V200.txt"
};

static REGVALUE_S s_stTda18250bQam128[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM128_V200.txt"
};

static REGVALUE_S s_stTda18250bQam256[]=
{
#include "qamdata/TDA18250B/TDA18250B_QAM256_V200.txt"
};

static REGVALUE_S *s_astTda18250bQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTda18250bQam16, s_stTda18250bQam32, s_stTda18250bQam64, s_stTda18250bQam128, s_stTda18250bQam256
};

/* SI2147 HI3716cv200 CONFIG */
static REGVALUE_S s_stSi2147Qam16[]=
{
#include "qamdata/SI2147/SI2147_QAM16_V200.txt"
};

static REGVALUE_S s_stSi2147Qam32[]=
{
#include "qamdata/SI2147/SI2147_QAM32_V200.txt"
};

static REGVALUE_S s_stSi2147Qam64[]=
{
#include "qamdata/SI2147/SI2147_QAM64_V200.txt"
};

static REGVALUE_S s_stSi2147Qam128[]=
{
#include "qamdata/SI2147/SI2147_QAM128_V200.txt"
};

static REGVALUE_S s_stSi2147Qam256[]=
{
#include "qamdata/SI2147/SI2147_QAM256_V200.txt"
};

static REGVALUE_S *s_astSi2147QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stSi2147Qam16, s_stSi2147Qam32, s_stSi2147Qam64, s_stSi2147Qam128, s_stSi2147Qam256
};


/* MXL603 hi3716cV200 CONFIG */
static REGVALUE_S s_stMxl603Qam16[]=
{
#include "qamdata/MXL603/MXL603_QAM16_V200.txt"
};

static REGVALUE_S s_stMxl603Qam32[]=
{
#include "qamdata/MXL603/MXL603_QAM32_V200.txt"
};

static REGVALUE_S s_stMxl603Qam64[]=
{
#include "qamdata/MXL603/MXL603_QAM64_V200.txt"

};

static REGVALUE_S s_stMxl603Qam128[]=
{
#include "qamdata/MXL603/MXL603_QAM128_V200.txt"
};

static REGVALUE_S s_stMxl603Qam256[]=
{
#include "qamdata/MXL603/MXL603_QAM256_V200.txt"
};

static REGVALUE_S *s_astMxl603QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stMxl603Qam16, s_stMxl603Qam32, s_stMxl603Qam64, s_stMxl603Qam128, s_stMxl603Qam256
};
/* CD1616 HI3716CV200 CONFIG */
static REGVALUE_S s_stCd1616Qam16[]=
{
#include "qamdata/CD1616/CD1616_QAM16_V200.txt"
};

static REGVALUE_S s_stCd1616Qam32[]=
{
#include "qamdata/CD1616/CD1616_QAM32_V200.txt"
};

static REGVALUE_S s_stCd1616Qam64[]=
{
#include "qamdata/CD1616/CD1616_QAM64_V200.txt"
};

static REGVALUE_S s_stCd1616Qam128[]=
{
#include "qamdata/CD1616/CD1616_QAM128_V200.txt"
};

static REGVALUE_S s_stCd1616Qam256[]=
{
#include "qamdata/CD1616/CD1616_QAM256_V200.txt"
};

static REGVALUE_S *s_astCd1616QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stCd1616Qam16, s_stCd1616Qam32, s_stCd1616Qam64, s_stCd1616Qam128, s_stCd1616Qam256
};

/* ALPS_TDAE HI3716CV200 CONFIG */
static REGVALUE_S s_stAlpsTdaeQam16[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM16_V200.txt"
};

static REGVALUE_S s_stAlpsTdaeQam32[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM32_V200.txt"
};

static REGVALUE_S s_stAlpsTdaeQam64[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM64_V200.txt"
};

static REGVALUE_S s_stAlpsTdaeQam128[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM128_V200.txt"
};

static REGVALUE_S s_stAlpsTdaeQam256[]=
{
#include "qamdata/ALPS_TDAE/ALPS_TDAE_QAM256_V200.txt"
};

static REGVALUE_S *s_astAlpsTdaeQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stAlpsTdaeQam16, s_stAlpsTdaeQam32, s_stAlpsTdaeQam64, s_stAlpsTdaeQam128, s_stAlpsTdaeQam256
};

/* TMX70707 HI3716CV200 CONFIG */
static REGVALUE_S s_stTmx70707Qam16[]=
{
#include "qamdata/TMX70707/TMX70707_QAM16_V200.txt"
};

static REGVALUE_S s_stTmx70707Qam32[]=
{
#include "qamdata/TMX70707/TMX70707_QAM32_V200.txt"
};

static REGVALUE_S s_stTmx70707Qam64[]=
{
#include "qamdata/TMX70707/TMX70707_QAM64_V200.txt"
};

static REGVALUE_S s_stTmx70707Qam128[]=
{
#include "qamdata/TMX70707/TMX70707_QAM128_V200.txt"
};

static REGVALUE_S s_stTmx70707Qam256[]=
{
#include "qamdata/TMX70707/TMX70707_QAM256_V200.txt"
};

static REGVALUE_S *s_astTmx70707QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTmx70707Qam16, s_stTmx70707Qam32, s_stTmx70707Qam64, s_stTmx70707Qam128, s_stTmx70707Qam256
};

/* MXL203 HI3716CV200 CONFIG */
static REGVALUE_S s_stMxl203Qam16[]=
{
#include "qamdata/MXL203/MXL203_QAM16_V200.txt"
};

static REGVALUE_S s_stMxl203Qam32[]=
{
#include "qamdata/MXL203/MXL203_QAM32_V200.txt"
};

static REGVALUE_S s_stMxl203Qam64[]=
{
#include "qamdata/MXL203/MXL203_QAM64_V200.txt"

};

static REGVALUE_S s_stMxl203Qam128[]=
{
#include "qamdata/MXL203/MXL203_QAM128_V200.txt"
};

static REGVALUE_S s_stMxl203Qam256[]=
{
#include "qamdata/MXL203/MXL203_QAM256_V200.txt"
};

static REGVALUE_S *s_astMxl203QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stMxl203Qam16, s_stMxl203Qam32, s_stMxl203Qam64, s_stMxl203Qam128, s_stMxl203Qam256
};
/* TDCC HI3716CV200 CONFIG */
static REGVALUE_S s_stTdccQam16[]=
{
#include "qamdata/TDCC/TDCC_QAM16_V200.txt"
};

static REGVALUE_S s_stTdccQam32[]=
{
#include "qamdata/TDCC/TDCC_QAM32_V200.txt"
};

static REGVALUE_S s_stTdccQam64[]=
{
#include "qamdata/TDCC/TDCC_QAM64_V200.txt"
};

static REGVALUE_S s_stTdccQam128[]=
{
#include "qamdata/TDCC/TDCC_QAM128_V200.txt"
};

static REGVALUE_S s_stTdccQam256[]=
{
#include "qamdata/TDCC/TDCC_QAM256_V200.txt"
};

static REGVALUE_S *s_astTdccQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stTdccQam16, s_stTdccQam32, s_stTdccQam64, s_stTdccQam128, s_stTdccQam256
};
/* MT2081 HI3716CV200 CONFIG */
static REGVALUE_S s_stMt2081Qam16[]=
{
#include "qamdata/MT2081/MT2081_QAM16_V200.txt"
};

static REGVALUE_S s_stMt2081Qam32[]=
{
#include "qamdata/MT2081/MT2081_QAM32_V200.txt"
};

static REGVALUE_S s_stMt2081Qam64[]=
{
#include "qamdata/MT2081/MT2081_QAM64_V200.txt"
};

static REGVALUE_S s_stMt2081Qam128[]=
{
#include "qamdata/MT2081/MT2081_QAM128_V200.txt"
};

static REGVALUE_S s_stMt2081Qam256[]=
{
#include "qamdata/MT2081/MT2081_QAM256_V200.txt"
};

static REGVALUE_S *s_astMt2081QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stMt2081Qam16, s_stMt2081Qam32, s_stMt2081Qam64, s_stMt2081Qam128, s_stMt2081Qam256
};

/* R820C HI3716CV200 CONFIG */
static REGVALUE_S s_stR820CQam16[]=
{
#include "qamdata/R820C/R820C_QAM16_V200.txt"
};

static REGVALUE_S s_stR820CQam32[]=
{
#include "qamdata/R820C/R820C_QAM32_V200.txt"
};

static REGVALUE_S s_stR820CQam64[]=
{
#include "qamdata/R820C/R820C_QAM64_V200.txt"
};

static REGVALUE_S s_stR820CQam128[]=
{
#include "qamdata/R820C/R820C_QAM128_V200.txt"
};

static REGVALUE_S s_stR820CQam256[]=
{
#include "qamdata/R820C/R820C_QAM256_V200.txt"
};

static REGVALUE_S *s_astR820CQamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stR820CQam16, s_stR820CQam32, s_stR820CQam64, s_stR820CQam128, s_stR820CQam256
};

/* M88TC3800 HI3716MV410 CONFIG */
static REGVALUE_S s_stM88tc3800Qam16[]=
{
#include "qamdata/M88TC3800/M88TC3800_QAM16_V200.txt"
};

static REGVALUE_S s_stM88tc3800Qam32[]=
{
#include "qamdata/M88TC3800/M88TC3800_QAM32_V200.txt"
};

static REGVALUE_S s_stM88tc3800Qam64[]=
{
#include "qamdata/M88TC3800/M88TC3800_QAM64_V200.txt"
};

static REGVALUE_S s_stM88tc3800Qam128[]=
{
#include "qamdata/M88TC3800/M88TC3800_QAM128_V200.txt"
};

static REGVALUE_S s_stM88tc3800Qam256[]=
{
#include "qamdata/M88TC3800/M88TC3800_QAM256_V200.txt"
};

static REGVALUE_S *s_astM88tc3800QamRegs[HI_TUNER_QAM_TYPE_MAX] =
{
    s_stM88tc3800Qam16, s_stM88tc3800Qam32, s_stM88tc3800Qam64, s_stM88tc3800Qam128, s_stM88tc3800Qam256
};

static char *s_stHi3130v200QamRegName[]=
{
#include "qamdata/QAMPRINT_V200.txt"
};

static HI_VOID hi3716cv200_select_qam_block(HI_U32 u32TunerPort);
//static HI_VOID hi3716cv200_select_hard_ip_block(HI_U32 u32TunerPort);

#define HI3716CV200_I2C_CHECK(tunerport) \
 do\
    {\
        HI_U8 u8Tmp = 0;\
        if (HI_SUCCESS != qam_read_byte(tunerport, AGC_CTRL_1_ADDR, &u8Tmp))\
        {\
            HI_ERR_TUNER("x5hdqamv200_i2c_check failure,please check i2c:%d\n", g_stTunerOps[tunerport].enI2cChannel);\
            return HI_FAILURE;\
        }\
    } while (0)

static HI_VOID set_symrate(HI_U32 u32TunerPort, HI_U32 u32SymRate)
{
    HI_U32 u32Temp = 0;/*= (HI_U32)(M_CLK * 8192 / nSymRate);*/
    HI_U8  u8Temp = 0;
   // HI_U8 u8VersionReg = 0;


    hi3716cv200_select_qam_block(u32TunerPort);

    u32Temp = (HI_U32)(g_stTunerOps[u32TunerPort].u32XtalClk * 8192 / u32SymRate);

    qam_read_byte(u32TunerPort, TR_CTRL_8_ADDR, &u8Temp);

    u8Temp = (u8Temp & 0xf8) | (u32Temp & 0x70000) >> 16;

    qam_write_byte(u32TunerPort, TR_CTRL_8_ADDR, u8Temp);
    qam_write_byte(u32TunerPort, TR_CTRL_9_ADDR, (u32Temp & 0xff00) >> 8);
    qam_write_byte(u32TunerPort, TR_CTRL_10_ADDR, u32Temp & 0xff);

    switch (g_stTunerOps[u32TunerPort].u32CurrQamMode)
    {
        case QAM_TYPE_64:
            if ( u32SymRate < SYMRATE_DEVISION )
            {

                qam_write_bit(u32TunerPort, J83B_DEBUG_0_ADDR, 4, 1); //0xc7 ===>0x70
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE12_ADDR, 0x22);//0xf5
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE34_ADDR, 0x22);//0xf6
                qam_write_byte(u32TunerPort, CR_KINT_SCALE12_ADDR, 0x02);//0xf7
                qam_write_byte(u32TunerPort, CR_KINT_SCALE34_ADDR, 0x30);//0xf8
                qam_write_byte(u32TunerPort, DAGC_CTRL_8_ADDR, 0x22); //0xd7
                qam_write_byte(u32TunerPort, DAGC_CTRL_9_ADDR, 0x22);//0xd8
                qam_write_byte(u32TunerPort, DAGC_CTRL_2_ADDR, 0x22);//0xd1

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
                qam_write_bit(u32TunerPort, J83B_DEBUG_0_ADDR, 4, 1); //0xc7 ===>0x70
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE12_ADDR, 0x22);//0xf5
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE34_ADDR, 0x22);//0xf6
                qam_write_byte(u32TunerPort, CR_KINT_SCALE12_ADDR, 0x02);//0xf7
                qam_write_byte(u32TunerPort, CR_KINT_SCALE34_ADDR, 0x30);//0xf8
                qam_write_byte(u32TunerPort, DAGC_CTRL_8_ADDR, 0x22); //0xd7
                qam_write_byte(u32TunerPort, DAGC_CTRL_9_ADDR, 0x22);//0xd8
                qam_write_byte(u32TunerPort, DAGC_CTRL_2_ADDR, 0x22);//0xd1

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
                qam_write_bit(u32TunerPort, J83B_DEBUG_0_ADDR, 4, 1); //0xc7 ===>0x70
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE12_ADDR, 0x22);//0xf5
                qam_write_byte(u32TunerPort, CR_KPROP_SCALE34_ADDR, 0x22);//0xf6
                qam_write_byte(u32TunerPort, CR_KINT_SCALE12_ADDR, 0x02);//0xf7
                qam_write_byte(u32TunerPort, CR_KINT_SCALE34_ADDR, 0x30);//0xf8
                qam_write_byte(u32TunerPort, DAGC_CTRL_8_ADDR, 0x22); //0xd7
                qam_write_byte(u32TunerPort, DAGC_CTRL_9_ADDR, 0x22);//0xd8
                qam_write_byte(u32TunerPort, DAGC_CTRL_2_ADDR, 0x22);//0xd1

                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x00);
            }
            else
            {
                qam_write_byte(u32TunerPort, DEPHASE_GAIN_K_HI_ADDR, 0x03);
            }

            break;

        default:
            break;

    }
}

HI_VOID hi3716cv200_test_single_agc( HI_U32 u32TunerPort, AGC_TEST_S * pstAgcTest )
{
    HI_S32 s32Ret = 0;
    HI_U8  u8RegVal = 0;
    HI_U8  au8Tmp[2] = {0};
   // HI_U8 u8VersionReg = 0;
    HI_U8  u8TemVal = 0;

    hi3716cv200_select_qam_block(u32TunerPort);

    s32Ret = qam_read_byte(u32TunerPort, MCTRL_11_ADDR, &u8RegVal);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("i2c operation error\n");
        return;
    }
    u8TemVal = ( u8RegVal & 0x80 ) >> 7;
    if ( 0xf8 == u8RegVal )
    {
        pstAgcTest->bLockFlag = HI_TRUE;
        pstAgcTest->bAgcLockFlag = HI_TRUE;
    }
    else if ( 1 == u8TemVal )
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

static HI_VOID hi3716cv200_chip_reset(HI_U32 u32TunerPort, qam_module_e enResetModule)
{
    //HI_U8 u8VersionReg = 0;

    hi3716cv200_select_qam_block(u32TunerPort);

    qam_write_byte(u32TunerPort, AGC_CTRL_3_ADDR, 0x20);

    if (ALL_CHIP == enResetModule)
    {
        qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0xff);
        qam_write_byte(u32TunerPort, MCTRL_1_ADDR, 0x00);
    }
    else
    {
        qam_write_bit(u32TunerPort, MCTRL_1_ADDR, (HI_U8)enResetModule, 1);
        qam_write_bit(u32TunerPort, MCTRL_1_ADDR, (HI_U8)enResetModule, 0);
    }

    qam_write_byte(u32TunerPort, AGC_CTRL_3_ADDR, 0xef);

}

/*note:this function should not be deleted,reserved for debug*/
#if 0
HI_VOID hi3716cv200_get_registers(HI_U32 u32TunerPort, void *p)
{
    HI_U8   u8Value = 0;
    HI_S32  i = 0;
    HI_S32  s32RegSum = 0;
    HI_U32  u32QamType = 0;
    REGVALUE_S *pstReg = HI_NULL;
   // HI_U8 u8VersionReg = 0;


       hi3716cv200_select_qam_block(u32TunerPort);


    u32QamType = g_stTunerOps[u32TunerPort].u32CurrQamMode;
    pstReg = s_astTda18250QamRegs[u32QamType];
    s32RegSum = sizeof(s_stTda18250Qam64)/sizeof(REGVALUE_S);

    for( i = 0; i < s32RegSum; i++ )
    {
        if ( 0 == i % 3 )
        {
            PROC_PRINT(p, "\n");
        }
        qam_read_byte(u32TunerPort, pstReg->u8Addr, &u8Value);
        pstReg++;
        PROC_PRINT(p, "{%s  ,0x%02x },  ", s_stHi3130v200QamRegName[i], u8Value);
    }
    PROC_PRINT(p, "\n\n");

}
#endif
static HI_VOID hi3716cv200_set_customreg_value(HI_U32 u32TunerPort, HI_BOOL bInversion, HI_U32 u32SymbolRate)
{
    HI_U8 u8Val = 0;
#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)
    U_PERI_CRG57 unTmpPeriCrg57;
#endif

    hi3716cv200_select_qam_block(u32TunerPort);

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
#if defined(CHIP_TYPE_hi3716cv200es) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)
    unTmpPeriCrg57.u32 = g_pstRegCrg->PERI_CRG57.u32;
    if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50 == g_stTunerOps[u32TunerPort].enTsType
       || HI_UNF_TUNER_OUTPUT_MODE_SERIAL == g_stTunerOps[u32TunerPort].enTsType)
    {
        /*select QAM TSCLK source 150MHz,PERI_CRG57[19:18]:01 ; QAM TSCLK 2 div,PERI_CRG57[23:20]:0000 */

        unTmpPeriCrg57.bits.qam_ts_clk_sel =  0x01;
        unTmpPeriCrg57.bits.qam_ts_clk_div = 0x0;
    }
    else
    {
        /*select QAM TSCLK source 150MHz,PERI_CRG57[19:18]:01 ; QAM TSCLK 16 div,PERI_CRG57[23:20] :0111,*/

         unTmpPeriCrg57.bits.qam_ts_clk_sel =  0x01;
         unTmpPeriCrg57.bits.qam_ts_clk_div = 0x7;
    }
     g_pstRegCrg->PERI_CRG57.u32 = unTmpPeriCrg57.u32;

  #endif
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

static HI_VOID hi3716cv200_select_qam_block(HI_U32 u32TunerPort)
{
    HI_U8 u8Tmp = 0;

    /*set block ctrl reg, qam*/
    qam_read_byte(u32TunerPort, FOUR_REG_SEL_ADDR, &u8Tmp);
    u8Tmp = (u8Tmp & (~0x3));
    qam_write_byte(u32TunerPort, FOUR_REG_SEL_ADDR, u8Tmp);
    return;
}


static HI_VOID hi3716cv200_set_reg_value(HI_U32 u32TunerPort)
{
    HI_S32  i = 0;
    HI_S32  s32RegSum;
    HI_U32  u32QamType;
    REGVALUE_S *pstReg;
    //HI_U8 u8VersionReg = 0;


   hi3716cv200_select_qam_block(u32TunerPort);


    /*set for qam 16/32/64/128/256*/
    u32QamType = g_stTunerOps[u32TunerPort].u32CurrQamMode;

   switch( g_stTunerOps[u32TunerPort].enTunerDevType )
   {
        case HI_UNF_TUNER_DEV_TYPE_TDA18250:
        {
            pstReg = s_astTda18250QamRegs[u32QamType];
            break;
        }

		case HI_UNF_TUNER_DEV_TYPE_TDA18250B:
        {
            pstReg = s_astTda18250bQamRegs[u32QamType];
            break;
        }

      case HI_UNF_TUNER_DEV_TYPE_MXL603:
        {
            pstReg = s_astMxl603QamRegs[u32QamType];
            break;
        }
      case HI_UNF_TUNER_DEV_TYPE_CD1616:
      {
            pstReg = s_astCd1616QamRegs[u32QamType];
            break;
      }
        case HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE:
        {
            pstReg = s_astAlpsTdaeQamRegs[u32QamType];
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
		case HI_UNF_TUNER_DEV_TYPE_SI2147:
        {
            pstReg = s_astSi2147QamRegs[u32QamType];
            break;
        }
	 case HI_UNF_TUNER_DEV_TYPE_M88TC3800:
        {
            pstReg = s_astM88tc3800QamRegs[u32QamType];
            break;
        }
        default:
        {
            pstReg = s_astTda18250QamRegs[u32QamType];
            break;
        }
    }

    //s32RegSum = sizeof(s_stCd1616Qam128)/sizeof(REGVALUE_S);
    s32RegSum = sizeof(s_stTda18250Qam128)/sizeof(REGVALUE_S);

    for(i=0; i<s32RegSum; i++)
    {
        qam_write_byte(u32TunerPort, pstReg->u8Addr, pstReg->u8Value);
        pstReg++;
    }
    if((1==u32TunerPort) && (HI_UNF_TUNER_DEV_TYPE_TDA18250B == g_stTunerOps[u32TunerPort].enTunerDevType))
    {
        //plus 18250B IF:5.0+0.25M
        if(4 == u32QamType)
        {
            qam_write_byte(u32TunerPort, FS_CTRL_1_ADDR, 0x15);
            qam_write_byte(u32TunerPort, FS_CTRL_2_ADDR, 0xbe);
        }
    else
    {
         qam_write_byte(u32TunerPort, FS_CTRL_1_ADDR, 0x15);
            qam_write_byte(u32TunerPort, FS_CTRL_2_ADDR, 0xcc);
    }

    }
   if((2==u32TunerPort) && (HI_UNF_TUNER_DEV_TYPE_TDA18250B == g_stTunerOps[u32TunerPort].enTunerDevType))
    {
        //plus 18250B IF:5.0-0.5M
        if(4 == u32QamType)
        {
            qam_write_byte(u32TunerPort, FS_CTRL_1_ADDR, 0x14);
            qam_write_byte(u32TunerPort, FS_CTRL_2_ADDR, 0x86);
        }
    else
    {
         qam_write_byte(u32TunerPort, FS_CTRL_1_ADDR, 0x14);
            qam_write_byte(u32TunerPort, FS_CTRL_2_ADDR, 0x86);
    }

    }
}

HI_S32 hi3716cv200_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U8  u8Status = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i = 0;
   // HI_U8 u8VersionReg = 0;


    hi3716cv200_select_qam_block(u32TunerPort);

   // HI_ASSERT(HI_NULL != penTunerStatus);
    HI_TUNER_CHECKPOINTER(penTunerStatus);
    HI3716CV200_I2C_CHECK(u32TunerPort);

    for (i = 0; i < 4; i++)  //prevent twittering
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

#define V200_CONNCECT_TIMEOUT   200     /* ms */
#define v200_CLOSE_EQU_TIME 200
HI_VOID hi3716cv200_manage_after_chipreset(HI_U32 u32TunerPort)
{
    HI_S32  s32Cnt = 0;
    HI_BOOL bStartFlag = HI_FALSE;
    HI_U8   u8IsReset = 0;

    HI_U8  u8LockRegVal = 0;
    HI_U8  u8Tmp = 0;
    HI_U32 u32Timeout = 0;

    HI_U32 u32StartTime = 0;
    HI_U32 u32EndTime = 0;
    HI_U32 u32EquResetTime = 0;

    #define V200_CONNECT_CHECK_FREQ 20      /* ms */
    #define EQU_CTRL_11_ADDR        EQU_BASE_ADDR + 0x09

    u32StartTime = gettime_ms();

    hi3716cv200_select_qam_block(u32TunerPort);
    //do_gettimeofday(&start_tv);

    if ((QAM_TYPE_256 == g_stTunerOps[u32TunerPort].u32CurrQamMode) ||
             (QAM_TYPE_128 == g_stTunerOps[u32TunerPort].u32CurrQamMode))
    {
        while  (u32Timeout < V200_CONNCECT_TIMEOUT)
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

                u32EndTime = gettime_ms();
                u32EquResetTime = u32EndTime - u32StartTime;
                if (u32EquResetTime > v200_CLOSE_EQU_TIME)
                {
                    u8Tmp = u8Tmp & 0x7;

                    if (u8Tmp >= 0x3)
                    {
                        bStartFlag = HI_TRUE;
                    }

                    if (u8IsReset == 0)
                    {
                        qam_write_byte(u32TunerPort, EQU_CTRL_6_ADDR, (0x07 + 0x40));
                        qam_write_byte(u32TunerPort, EQU_CTRL_7_ADDR, 0x78);
                        qam_write_byte(u32TunerPort, EQU_CTRL_8_ADDR, 0);
                        qam_write_byte(u32TunerPort, EQU_CTRL_9_ADDR, 0);
                        qam_write_byte(u32TunerPort, EQU_CTRL_10_ADDR, 0);

                        /*don't update tap: ffe 12~19, dfe 8~24*/
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x30);
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x40);
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x70);
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x80);
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x90);
                        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0xa0);

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
                }

                msleep( V200_CONNECT_CHECK_FREQ );
                u32Timeout = u32Timeout + V200_CONNECT_CHECK_FREQ;
            }
        } /*while*/
    }

    if (u32EquResetTime > v200_CLOSE_EQU_TIME)
    {
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x3f);
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x4f);
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x7f);
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x8f);
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0x9f);
        qam_write_byte(u32TunerPort, EQU_CTRL_11_ADDR, 0xaf);
    }
    return;
}

static HI_U32 s_u32hi716cv200ConnectTimeout = V200_CONNCECT_TIMEOUT;
HI_VOID hi3716cv200_connect_timeout(HI_U32 u32ConnectTimeout)
{
    s_u32hi716cv200ConnectTimeout = u32ConnectTimeout;
    return;
}

HI_S32 hi3716cv200_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_S32 s32FuncRet = HI_SUCCESS;
    HI_U32 u32SymbolRate = 0;

    hi3716cv200_select_qam_block(u32TunerPort);

    reset_special_process_flag(HI_FALSE);

    //HI_ASSERT(HI_NULL != pstChannel);
    HI_TUNER_CHECKPOINTER(pstChannel);
    HI3716CV200_I2C_CHECK(u32TunerPort);



    g_stTunerOps[u32TunerPort].u32CurrQamMode = pstChannel->enQamType;
    u32SymbolRate = pstChannel->unSRBW.u32SymbolRate / 1000; /* KHz */

    /*step 1:config tuner register*/
    if(!g_stTunerOps[u32TunerPort].set_tuner)
    {
        HI_ERR_TUNER( "tuner service function ptr is NULL!\n");
        return -1;
    }

    s32FuncRet = (*g_stTunerOps[u32TunerPort].set_tuner)(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, pstChannel->u32Frequency);
    if (HI_SUCCESS != s32FuncRet)
    {
        HI_ERR_TUNER( "set tuner error\n");
        return s32FuncRet;
    }

    /*step 2:config qam register */
    hi3716cv200_set_reg_value(u32TunerPort);     /*set for qam and tunner*/


    {
        switch( g_stTunerOps[u32TunerPort].u32CurrQamMode )
        {
        case QAM_TYPE_64:
            qam_write_byte(u32TunerPort, 0x43, 0x59);
            break;
        case QAM_TYPE_128:
            qam_write_byte(u32TunerPort, 0x43, 0x65);
            break;
        case QAM_TYPE_256:
            qam_write_byte(u32TunerPort, 0x43, 0x6a);
            break;
        default:
            qam_write_byte(u32TunerPort, 0x43, 0x20);
            break;
        }
    }

    /*set for different usr request about symbolrate, ts type, demo in or out,  inversion or not*/
    hi3716cv200_set_customreg_value(u32TunerPort, pstChannel->bSI, u32SymbolRate);

    /*step 3:reset chip*/
    hi3716cv200_chip_reset(u32TunerPort, ALL_CHIP);

#if 0
    if(s_u32hi3130v200ConnectTimeout < v200_CLOSE_EQU_TIME)
    {
        s_u32hi3130v200ConnectTimeout = V200_CONNCECT_TIMEOUT;
        return HI_SUCCESS;
    }

    if (g_stTunerOps[u32TunerPort].manage_after_chipreset)
        g_stTunerOps[u32TunerPort].manage_after_chipreset(u32TunerPort);
#endif

    return HI_SUCCESS;
}

HI_S32 hi3716cv200_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    HI_U8  au8RegVal[2] = {0};
    HI_U32 u32TmpSigStrength = 0;
    HI_U32 i;

    hi3716cv200_select_qam_block(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32SignalStrength);
    HI3716CV200_I2C_CHECK(u32TunerPort);


    for ( i = 0; i < 64; i++ )
    {
        qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 0x1);

        qam_read_byte(u32TunerPort, BAGC_STAT_3_ADDR, &au8RegVal[0]);

        qam_read_byte(u32TunerPort, BAGC_STAT_4_ADDR, &au8RegVal[1]);

        pu32SignalStrength[1] = ( au8RegVal[0]  << 4 ) | ( (au8RegVal[1] >> 4) & 0x0f );

        u32TmpSigStrength += pu32SignalStrength[1];

    }

    u32TmpSigStrength =     u32TmpSigStrength >> 6;
    pu32SignalStrength[1] = u32TmpSigStrength;

    if(g_stTunerOps[u32TunerPort].recalculate_signal_strength)
    {
        g_stTunerOps[u32TunerPort].recalculate_signal_strength(u32TunerPort, pu32SignalStrength);
    }

    return HI_SUCCESS;
}

HI_S32 hi3716cv200_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    HI_U32 u32BerThres = 4; /* modify by chenxu 2008-01-15 AI7D02092 */
    HI_U8  u8Temp   = 0;
    HI_U8  u8Val = 0;
    HI_S32 i = 0;

    hi3716cv200_select_qam_block(u32TunerPort);


    //HI_ASSERT(HI_NULL != pu32ber);
    HI_TUNER_CHECKPOINTER(pu32ber);
    HI3716CV200_I2C_CHECK(u32TunerPort);

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 0); /*disable*/

    u8Val = (u32BerThres) << 2;
    qam_write_byte( u32TunerPort, BER_1_ADDR, u8Val );

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 1); /*enable*/

    for (i = 0; i < 150; i++)
    {
        msleep(10);
        qam_read_bit(u32TunerPort, BER_1_ADDR, 7, &u8Temp);
        if (!u8Temp)
        {
            break;
        }
    }

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

    if (i >= 150)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, BER_2_ADDR, &u8Val);
    pu32ber[0] = u8Val;
    qam_read_byte(u32TunerPort, BER_3_ADDR, &u8Val);
    pu32ber[1] = u8Val;
    qam_read_byte(u32TunerPort, BER_4_ADDR, &u8Val);
    pu32ber[2] = u8Val;

    return HI_SUCCESS;
}

HI_S32 hi3716cv200_get_rs(HI_U32 u32TunerPort, HI_U32 *pu32Rs)
{
    HI_U32 u32BerThres = 4; /* modify by chenxu 2008-01-15 AI7D02092 */
    HI_U8  u8Temp    = 0;
    HI_U8  u8Val     = 0;
    HI_S32 i         = 0;
    //HI_U8  u8Addr      = CR_CTRL_21_ADDR;
    //HI_U8  u8RegValue  = 1;
    HI_U32 u8RegValue1 = 0;
    HI_U32 u8RegValue2 = 0;
    HI_U32 u8RegValue3 = 0;
   // HI_U8 u8VersionReg = 0;

    hi3716cv200_select_qam_block(u32TunerPort);

    HI3716CV200_I2C_CHECK(u32TunerPort);

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 0); /*disable*/

    u8Val = (u32BerThres) << 2;
    qam_write_byte( u32TunerPort, BER_1_ADDR, u8Val );

    qam_write_bit(u32TunerPort, RS_CTRL_8_ADDR, 4, 1);

    qam_write_bit(u32TunerPort, BER_1_ADDR, 7, 1); /*enable*/

   qam_write_bit(u32TunerPort, RS_CTRL_8_ADDR, 4, 0);
    for (i = 0; i < 150; i++)
    {
        msleep(10);
        qam_read_bit(u32TunerPort, BER_1_ADDR, 7, &u8Temp);
        if (!u8Temp)
        {
            break;
        }
    }

    qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

    if (i >= 150)
    {

        return HI_FAILURE;
    }

    /* rs package,calculate RS error  package*/
    //qam_write_byte(u32TunerPort, u8Addr,u8RegValue);

    qam_read_byte(u32TunerPort, RS_CTRL_1_ADDR, &u8Temp);
    qam_read_byte(u32TunerPort, RS_CTRL_2_ADDR, &u8Val);
    u8RegValue1 = ( u8Temp << 8 ) | u8Val;

    qam_read_byte(u32TunerPort, RS_CTRL_3_ADDR, &u8Temp);
    qam_read_byte(u32TunerPort, RS_CTRL_4_ADDR, &u8Val);
    u8RegValue2 = ( u8Temp << 8 ) | u8Val;

    qam_read_byte(u32TunerPort, RS_CTRL_5_ADDR, &u8Temp);
    qam_read_byte(u32TunerPort, RS_CTRL_6_ADDR, &u8Val);
    u8RegValue3 = ( u8Temp << 8 ) | u8Val;

    //qam_read_byte( u32TunerPort, RS_CTRL_8_ADDR, &u8RegValue );

    pu32Rs[0] = u8RegValue1;
    pu32Rs[1] = u8RegValue2;
    pu32Rs[2] = u8RegValue3;

    return HI_SUCCESS;
}


HI_S32 hi3716cv200_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    HI_U32 u32Snr = 0;
    HI_U8  u8RdVal = 0;
    HI_U32 i ;
    HI_U32 u32SumSnr = 0;

    hi3716cv200_select_qam_block(u32TunerPort);
    //HI_ASSERT(HI_NULL != pu32SNR);
    HI_TUNER_CHECKPOINTER(pu32SNR);
    HI3716CV200_I2C_CHECK(u32TunerPort);

    for (i=0;i<64;i++)
    {
        qam_write_byte(u32TunerPort, CR_CTRL_21_ADDR, 1);

        qam_read_byte(u32TunerPort, EQU_STAT_2_ADDR, &u8RdVal);
        u32Snr = u8RdVal << 8;
        qam_read_byte(u32TunerPort, EQU_STAT_3_ADDR, &u8RdVal);
        u32Snr  += u8RdVal;
        u32SumSnr += u32Snr;
    }

    u32Snr = u32SumSnr >> 6;
    *pu32SNR = u32Snr;

    return HI_SUCCESS;
}

HI_S32 hi3716cv200_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}

HI_S32 hi3716cv200_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb )
{
    HI_U8  au8Freq[4] = { 0 };
    HI_S32 s32RealFreq = 0;
    HI_U8  au8Symb[2] = { 0 };
    HI_S32 s32RealSymb = 0;

    hi3716cv200_select_qam_block(u32TunerPort);
    //HI_ASSERT(HI_NULL != pu32Freq);
    //HI_ASSERT(HI_NULL != pu32Symb);
    HI_TUNER_CHECKPOINTER(pu32Freq);
    HI_TUNER_CHECKPOINTER(pu32Symb);
    HI3716CV200_I2C_CHECK(u32TunerPort);

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
