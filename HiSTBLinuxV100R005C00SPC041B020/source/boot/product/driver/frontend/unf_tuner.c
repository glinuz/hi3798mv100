/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_tuner.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 * main\1    2007-08-03   w54542      NULL            Create this file.
 * main\1    2007-11-10   w54542      NULL            modify this file.
 ***********************************************************************************/
/*#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/io.h>*/

#include "hi_type.h"
//#include "hi_common.h"
#include "hi_reg.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include <common.h>
//#include "hi_struct.h"

//#include "common_dev.h"
//#include "common_proc.h"
//#include "common_mem.h"
//#include "common_stat.h"
//#include "hi_kernel_adapt.h"

#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#include "hi_reg_common.h"

//#include "cd1616.h"
#include "alps_tdae.h"
#include "tda18250.h"
#include "tda18250b.h"
#include "tmx7070x.h"
#include "mxl203.h"
#include "tdcc.h"
//#include "xg_3bl.h"
//#include "mt_2081.h"
#include "mxl603.h"
#include "r_820c.h"
#include "av2011.h"
#include "sharp_qm1d1c004x.h"
#include "RDA5815.h"
#include "M88TS2022.h"
#include "M88TC3800.h"
//#include "hi_unf_ecs.h"
#include "si2147.h"

#include "isl9492.h"
#include "mps8125.h"
//#include "tda6651.h"

//#include "hi_gpio.h"

#define SAT_C_MIN (3000)
#define SAT_C_MAX (4200)
#define SAT_KU_MIN (10600)
#define SAT_KU_MAX (12750)
#define SAT_IF_MIN_KHZ (950000)
#define SAT_IF_MAX_KHZ (2150000)
#define SAT_C_MIN_KHZ (3000000)
#define SAT_C_MAX_KHZ (4200000)
#define SAT_KU_MIN_KHZ (10600000)
#define SAT_KU_MAX_KHZ (12750000)

#define SAT_SYMBOLRATE_MAX (60000000)//(45000000)

#define SAT_IF_MIN (950)
#define SAT_IF_MAX (2150)
#define SAT_IF_C_H_MIN (1550)
#define SAT_IF_C_H_MAX (2150)
#define SAT_IF_C_L_MIN (950)
#define SAT_IF_C_L_MAX (1550)
#define SAT_IF_KU_H_MIN_1 (1100)
#define SAT_IF_KU_H_MAX_1 (2150)
#define SAT_IF_KU_H_MIN_2 (950)
#define SAT_IF_KU_H_MAX_2 (2000)
#define SAT_IF_KU_L_MIN (950)
#define SAT_IF_KU_L_MAX (1950)

#define SAT_LO_C_L (5150)
#define SAT_LO_C_H (5750)
#define SAT_LO_KU_L_1 (9750)
#define SAT_LO_KU_H_1 (10600)
#define SAT_LO_KU_L_2 (9750)
#define SAT_LO_KU_H_2 (10750)

#define SAT_DOWNLINK_FREQ_KU_MID (11700)

#define DISEQC_DELAY_TIME_MS (15)

HI_U32 g_u32CrgEnableFlag = 0;

typedef struct
{
    HI_UNF_TUNER_FE_LNB_CONFIG_S     stLNBConfig;/* LNB configuration */
    HI_UNF_TUNER_FE_LNB_POWER_E     enLNBPower; /* LNB power */
    HI_UNF_TUNER_FE_POLARIZATION_E  enPolar;    /* LNB polarization */
    HI_UNF_TUNER_FE_LNB_22K_E       enLNB22K;   /* LNB 22K on or off */
#if 0
    pthread_t*                      pBlindScanMonitor; /* Blind scan thread */
    //pthread_t*                      pConnectMonitor; /* connect thread using while the symbol rate is low*/
    HI_BOOL                         bBlindScanStop; /* Blind scan stop flag */
    HI_BOOL                         bBlindScanBusy; /* Blind scan stop flag */
    //HI_BOOL                         bConnectStop; /* Blind scan stop flag */
    //HI_UNF_TUNER_SWITCH_22K_E       enSavedSwitch22K;
#endif
    HI_UNF_TUNER_SWITCH_22K_E       enSwitch22K;
    HI_UNF_TUNER_SWITCH_TONEBURST_E enToneBurst;
} TUNER_STATUS_SAT_S;


/*HI_DECLARE_MUTEX(g_TunerMutex);
static atomic_t s_atomicTunerAvailble = ATOMIC_INIT(1);*/

static HI_BOOL s_bTunerInited = HI_FALSE;
//static HI_UNF_TUNER_ATTR_S s_strDeftTunerAttr[TUNER_NUM];
static TUNER_STATUS_SAT_S s_stSatPara[UNF_TUNER_NUM];
static HI_UNF_TUNER_ATTR_S s_strCurTunerAttr[UNF_TUNER_NUM];

/*static struct stTunerResumeInfo
{
    TUNER_LNB_OUT_E enLNBOut;
    HI_U32 u32Continuous22K;
    HI_UNF_TUNER_SAT_ATTR_S stResumeData;
} s_stTunerResumeInfo[TUNER_NUM] = {{0}, {0}, {0}};*/

HI_U32 s_u32ResetGpioNo[TUNER_NUM] = {0};
HI_U32 g_u32ResetCruGpioNo =0;
TUNER_OPS_S g_stTunerOps[TUNER_NUM] = {{0},{0},{0},{0},{0}};
HI_BOOL g_bLockFlag = HI_FALSE;



#if (3 == TUNER_NUM)

HI_U32 g_xtal_clk_table[TUNER_NUM] = {XTAL_CLK, XTAL_CLK, XTAL_CLK};
HI_U32 g_demod_address[TUNER_NUM] = {QAM_PORT0_ADDR, QAM_PORT1_ADDR, QAM_PORT2_ADDR};
HI_U32 g_halftuner_address[TUNER_NUM] = {TUNER_PORT0_ADDR, TUNER_PORT1_ADDR, TUNER_PORT2_ADDR};

static HI_UNF_TUNER_LOCK_STATUS_E s_enTunerStat[TUNER_NUM] = { HI_UNF_TUNER_SIGNAL_DROPPED,
    HI_UNF_TUNER_SIGNAL_DROPPED,
    HI_UNF_TUNER_SIGNAL_DROPPED };
#elif (1 == TUNER_NUM)

HI_U32 g_xtal_clk_table[TUNER_NUM] = {XTAL_CLK};
HI_U32 g_demod_address[TUNER_NUM] = {QAM_PORT0_ADDR};
HI_U32 g_halftuner_address[TUNER_NUM] = {TUNER_PORT0_ADDR};

static HI_UNF_TUNER_LOCK_STATUS_E s_enTunerStat[TUNER_NUM] = { HI_UNF_TUNER_SIGNAL_DROPPED };
#endif

//wait_queue_head_t g_QamSpecialProcessWQ;
//static struct task_struct *pThread;

//extern HI_VOID get_chiptype(HI_CHIP_TYPE_E *penChipType, HI_U32 *pu32ChipVersion);

/* check tuner port whether it's in valid range */
static HI_S32 tuner_check_port(HI_U32 u32TunerPort)
{
    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* register tuner standard interface function */
HI_S32 tuner_select_type(HI_U32 u32TunerPort, HI_TunerAttr_S *pstTunerAttr,
                         HI_DemodAttr_S *pstDemodAttr, HI_U32 u32ResetGpioNo)
{
    HI_U8 u8DataTmp = 0;
    HI_U32 u32RegVal = 0;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHIP_TYPE_E enChipType = HI_CHIP_TYPE_BUTT;
    HI_U32 u32ChipVersion = 0;
    HI_U8 u8ChipReset = 0;	
#if defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)|| defined(CHIP_TYPE_hi3796cv100) \
		|| defined(CHIP_TYPE_hi3798cv100)|| defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
		U_PERI_CRG58 unTmpPeriCrg58;
		unTmpPeriCrg58.u32 = g_pstRegCrg->PERI_CRG58.u32;
#endif

    UNUSED(s32Ret);
    UNUSED(enChipType);
    UNUSED(u32ChipVersion);
    UNUSED(u32RegVal);
    UNUSED(u8DataTmp);

    if (HI_SUCCESS != tuner_check_port(u32TunerPort))
    {
        return HI_FAILURE;
    }

    g_stTunerOps[u32TunerPort].enTunerDevType = pstTunerAttr->enTunerDevType;
    g_stTunerOps[u32TunerPort].enDemodDevType = pstDemodAttr->enDemodDevType;
    s_u32ResetGpioNo[u32TunerPort] = u32ResetGpioNo;

    g_stTunerOps[u32TunerPort].u32TunerAddress = pstTunerAttr->u32TunerAddr;
    g_stTunerOps[u32TunerPort].u32DemodAddress = pstDemodAttr->u32DemodAddr;

    g_stTunerOps[u32TunerPort].recalculate_signal_strength = NULL;
    g_stTunerOps[u32TunerPort].manage_after_chipreset = NULL;
    g_stTunerOps[u32TunerPort].tuner_resume = NULL;
    g_stTunerOps[u32TunerPort].enTsType = HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A;

    g_u32ResetCruGpioNo =u32ResetGpioNo;

    switch (g_stTunerOps[u32TunerPort].enDemodDevType)
    {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3130I)
        case HI_UNF_DEMOD_DEV_TYPE_3130I:
        {
            qam_read_byte(u32TunerPort, MCTRL_1_ADDR, &u8DataTmp);
			
            if ((u8DataTmp >> 5) == 1 || (u8DataTmp >> 5) == 3 || (u8DataTmp >> 5) == 4 || (u8DataTmp >> 5)  == 6 || (u8DataTmp >> 5)  == 7)
            {
                if((u8DataTmp >> 5) == 1)
                {
                    TUNER_DEBUG("tuner port %d's demod type is x5hdqammv200 inside\n", u32TunerPort);
                }
                else if((u8DataTmp >> 5) == 3)
                {
                    TUNER_DEBUG("tuner port %d's demod type is x5hdqammv300 inside\n", u32TunerPort);
                }
                else if((u8DataTmp >> 5) == 4)
                {
                    TUNER_DEBUG("tuner port %d's demod type is s40v100qam inside\n", u32TunerPort);
                }
                else if((u8DataTmp >> 5) == 6)
                {
                    TUNER_DEBUG("tuner port %d's demod type is s40v200qam inside\n", u32TunerPort);
                }
                else if((u8DataTmp >> 5) == 7)
                {
                    TUNER_DEBUG("tuner port %d's demod type is hi3716cv200qam inside\n", u32TunerPort);
                }

                /*config adc clk 28.8MHz*/
				
#if defined (CHIP_TYPE_hi3716cv200) || defined (CHIP_TYPE_hi3716mv400)|| defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
				/*config qamadc_clk_sel 28.8MHz,  PERI_CRG58[9:8]:00*/
				unTmpPeriCrg58.bits.qamadc_clk_sel = 0;//PERI_CRG58[9:8]:00
				/*config qamctrl_clk_sel 25MHz , PERI_CRG58[11:10]:00; \
							config qamctrl_clk_div for the same frequence, PERI_CRG58[12]:0*/
				unTmpPeriCrg58.bits.qamctrl_clk_sel = 0;//PERI_CRG58[11:10]:00
				unTmpPeriCrg58.bits.qamctrl_clk_div = 0;//PERI_CRG58[12]:0
				g_pstRegCrg->PERI_CRG58.u32 = unTmpPeriCrg58.u32;
								
#endif				              
								
#if defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)\
    || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
					g_stTunerOps[u32TunerPort].tuner_connect = hi3716cv200_connect;
					g_stTunerOps[u32TunerPort].tuner_get_status = hi3716cv200_get_status;
					/*g_stTunerOps[u32TunerPort].tuner_get_ber = hi3716cv200_get_ber;
					g_stTunerOps[u32TunerPort].tuner_get_snr = hi3716cv200_get_snr;
					g_stTunerOps[u32TunerPort].tuner_set_ts_type = hi3716cv200_set_ts_type;
					g_stTunerOps[u32TunerPort].tuner_get_signal_strength = hi3716cv200_get_signal_strength;
					g_stTunerOps[u32TunerPort].tuner_test_single_agc = hi3716cv200_test_single_agc;
					g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = hi3716cv200_get_freq_symb_offset;
					g_stTunerOps[u32TunerPort].manage_after_chipreset = hi3716cv200_manage_after_chipreset;
					g_stTunerOps[u32TunerPort].tuner_get_rs = hi3716cv200_get_rs;
					g_stTunerOps[u32TunerPort].tuner_connect_timeout = hi3716cv200_connect_timeout;*/
#endif
            }
            
            break;
        }
#endif

#if defined(HI_BOOT_DEMOD_DEV_TYPE_MXL254) || defined(HI_BOOT_DEMOD_DEV_TYPE_MXL214)
        case HI_UNF_DEMOD_DEV_TYPE_MXL254:
        case HI_UNF_DEMOD_DEV_TYPE_MXL214:
        {
			tuner_chip_reset(u32ResetGpioNo);
            g_stTunerOps[u32TunerPort].tuner_init = mxl254_init;
            g_stTunerOps[u32TunerPort].tuner_connect = mxl254_connect;
            g_stTunerOps[u32TunerPort].tuner_get_status = mxl254_get_status;
            /*g_stTunerOps[u32TunerPort].tuner_get_ber = mxl254_get_ber;
            g_stTunerOps[u32TunerPort].tuner_get_snr = mxl254_get_snr;
            g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = mxl254_get_signal_strength;
            g_stTunerOps[u32TunerPort].tuner_set_ts_type = mxl254_set_ts_type;
            g_stTunerOps[u32TunerPort].tuner_connect_timeout =mxl254_connect_timeout;
			g_stTunerOps[u32TunerPort].tuner_get_powerspecdata =  mxl254_get_powerspecdata;*/
            break;
        }
#endif

        #if 0
        case HI_UNF_DEMOD_DEV_TYPE_J83B:
        {
            qam_read_byte(u32TunerPort, MCTRL_1_ADDR, &u8DataTmp);

            if((u8DataTmp >> 5) == 1)
            {
                TUNER_DEBUG("mv200J83B\n");
            }
            else if((u8DataTmp >> 5) == 3)
            {
                TUNER_DEBUG("mv300J83B\n");
            }

            /*config adc clk 25MHz*/
            {
            get_chiptype( &enChipType, &u32ChipVersion );
            if((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == u32ChipVersion))
            {
                HI_REG_READ(IO_ADDRESS(0x101f50a0), u32RegVal);
                u32RegVal = u32RegVal | (0x1 << 21);
                HI_REG_WRITE(IO_ADDRESS(0x101f50a0), u32RegVal);
            }
            else
            {
                /*set QPLL power down*/
                HI_REG_READ(IO_ADDRESS(0x101e003c), u32RegVal);
                u32RegVal = u32RegVal & (~(0x1 << 21));
                HI_REG_WRITE(IO_ADDRESS(0x101e003c), u32RegVal);

                /*set QPLL out 25MHz*/
                HI_REG_READ(IO_ADDRESS(0x101e0038), u32RegVal);
                u32RegVal = (u32RegVal & (~(0x3F << 24))) | (0x23 << 24);
                HI_REG_WRITE(IO_ADDRESS(0x101e0038), u32RegVal);

                HI_REG_READ(IO_ADDRESS(0x101e003c), u32RegVal);
                u32RegVal = (u32RegVal & 0xfffc0000) | 0x00001032;
                HI_REG_WRITE(IO_ADDRESS(0x101e003c), u32RegVal);

                /*set QPLL power up*/
                HI_REG_READ(IO_ADDRESS(0x101e003c), u32RegVal);
                u32RegVal = u32RegVal | (0x1 << 21);
                HI_REG_WRITE(IO_ADDRESS(0x101e003c), u32RegVal);
            }
            }
            g_stTunerOps[u32TunerPort].tuner_connect = j83b_connect;
            g_stTunerOps[u32TunerPort].tuner_get_status = j83b_get_status;
            g_stTunerOps[u32TunerPort].tuner_set_ts_type = j83b_set_ts_type;
            g_stTunerOps[u32TunerPort].manage_after_chipreset = j83b_manage_after_chipreset;
            g_stTunerOps[u32TunerPort].tuner_connect_timeout = j83b_connect_timeout;
            TUNER_DEBUG("tuner port %d's demod type is j83b outside\n", u32TunerPort);
            break;
        }
        #endif
#if 0
        case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
            {
                //set_pin_mux(u32TunerPort);
                s32Ret = copy_from_user(&stSatTunerAttr, pTunerAttr, sizeof(HI_UNF_TUNER_SAT_ATTR_S));
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG("AVL6211 attribute error\n");
                    return HI_FAILURE;
                }

                ptrTunerAttr = &stSatTunerAttr;
                g_stTunerOps[u32TunerPort].tuner_init = avl6211_init;
                g_stTunerOps[u32TunerPort].tuner_connect = avl6211_connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = avl6211_get_status;
                g_stTunerOps[u32TunerPort].tuner_get_ber = avl6211_get_ber;
                g_stTunerOps[u32TunerPort].tuner_get_snr = avl6211_get_snr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = avl6211_get_signal_strength;
                g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = avl6211_get_freq_symb_offset;
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = avl6211_get_signal_info;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = avl6211_set_ts_type;
                g_stTunerOps[u32TunerPort].tuner_blindscan_init   = avl6211_blindscan_init;
                g_stTunerOps[u32TunerPort].tuner_blindscan_action = avl6211_blindscan_action;
                g_stTunerOps[u32TunerPort].tuner_standby = avl6211_standby;
                g_stTunerOps[u32TunerPort].tuner_disable = avl6211_disable;
                g_stTunerOps[u32TunerPort].tuner_setfuncmode = avl6211_set_funcmode;
                break;
            }
#if defined(DEMOD_DEV_TYPE_MXL101)
        case HI_UNF_DEMOD_DEV_TYPE_MXL101:
            {
                s32Ret = copy_from_user(&stTerTunerAttr, pTunerAttr, sizeof(HI_UNF_TUNER_TER_ATTR_S));
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG("MXL101 attribute error\n");
                    return HI_FAILURE;
                }

                ptrTunerAttr = &stTerTunerAttr;
                g_stTunerOps[u32TunerPort].tuner_init = mxl101_init;
                g_stTunerOps[u32TunerPort].tuner_connect = mxl101_connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = mxl101_get_status;
                g_stTunerOps[u32TunerPort].tuner_get_ber = mxl101_get_ber;
                g_stTunerOps[u32TunerPort].tuner_get_snr = mxl101_get_snr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = mxl101_get_signal_strength;
                g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = mxl101_get_freq_symb_offset;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = mxl101_set_ts_type;
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = mxl101_get_signal_info;
                break;
            }
#endif
        case HI_UNF_DEMOD_DEV_TYPE_MN88472:
            {
                s32Ret = copy_from_user(&stTerTunerAttr, pTunerAttr, sizeof(HI_UNF_TUNER_TER_ATTR_S));
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG("MN88472 attribute error\n");
                    return HI_FAILURE;
                }

                ptrTunerAttr = &stTerTunerAttr;
                g_stTunerOps[u32TunerPort].tuner_init = MN88472_Init;
                g_stTunerOps[u32TunerPort].tuner_connect = MN88472_Connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = MN88472_GetStatus;
                g_stTunerOps[u32TunerPort].tuner_get_ber = MN88472_GetBer;
                g_stTunerOps[u32TunerPort].tuner_get_snr = MN88472_GetSnr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = MN88472_GetSignalStrength;
                g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = MN88472_get_freq_symb_offset;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = MN88472_SetTsType;
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = MN88472_get_signal_info;
                g_stTunerOps[u32TunerPort].tuner_setplpid = MN88472_setplpid;
                g_stTunerOps[u32TunerPort].tuner_get_plp_num = MN88472_get_plp_num;
                g_stTunerOps[u32TunerPort].tuner_get_current_plp_type = MN88472_get_current_plp_type;
                g_stTunerOps[u32TunerPort].tuner_connect_timeout = MN88472_connect_timeout;
                break;
            }
        case HI_UNF_DEMOD_DEV_TYPE_IT9170:
            {
#if defined(ITE9170_SUPPORT)
                s32Ret = copy_from_user(&stTerTunerAttr, pTunerAttr, sizeof(HI_UNF_TUNER_TER_ATTR_S));
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG("IT9170 attribute error\n");
                    return HI_FAILURE;
                }

                ptrTunerAttr = &stTerTunerAttr;
                g_stTunerOps[u32TunerPort].tuner_init = it9170_init;
                g_stTunerOps[u32TunerPort].tuner_connect = it9170_connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = it9170_get_status;
                g_stTunerOps[u32TunerPort].tuner_get_ber = it9170_get_ber;
                g_stTunerOps[u32TunerPort].tuner_get_snr = it9170_get_snr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = it9170_get_signal_strength;
                g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = it9170_get_freq_symb_offset;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = it9170_set_ts_type;
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = it9170_get_signal_info;
#endif
                break;
            }
        case HI_UNF_DEMOD_DEV_TYPE_IT9133:
            {
#if defined(ITE9133_SUPPORT)
                s32Ret = copy_from_user(&stTerTunerAttr, pTunerAttr, sizeof(HI_UNF_TUNER_TER_ATTR_S));
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG("IT9133 attribute error\n");
                    return HI_FAILURE;
                }

                ptrTunerAttr = &stTerTunerAttr;
                g_stTunerOps[u32TunerPort].tuner_init = it9133_init;
                g_stTunerOps[u32TunerPort].tuner_connect = it9133_connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = it9133_get_status;
                g_stTunerOps[u32TunerPort].tuner_get_ber = it9133_get_ber;
                g_stTunerOps[u32TunerPort].tuner_get_snr = it9133_get_snr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = it9133_get_signal_strength;
                g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = it9133_get_freq_symb_offset;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = it9133_set_ts_type;
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = it9133_get_signal_info;
#endif
                break;
            }
#endif
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
        case HI_UNF_DEMOD_DEV_TYPE_3136:
                u8ChipReset = 1;/*externel demod need chip reset, internel do not need.*/

        case HI_UNF_DEMOD_DEV_TYPE_3136I:
            {
                //set_pin_mux(u32TunerPort);
                g_stTunerOps[u32TunerPort].tuner_init = hi3136_init;
                g_stTunerOps[u32TunerPort].tuner_set_sat_attr = hi3136_set_sat_attr;
                g_stTunerOps[u32TunerPort].tuner_connect = hi3136_connect;
                g_stTunerOps[u32TunerPort].tuner_get_status = hi3136_get_status;
#ifndef BOOT_COMPILE
                g_stTunerOps[u32TunerPort].tuner_get_ber = hi3136_get_ber;
                g_stTunerOps[u32TunerPort].tuner_get_snr = hi3136_get_snr;
                g_stTunerOps[u32TunerPort].tuner_get_signal_strength  = hi3136_get_signal_strength;
#endif
                //g_stTunerOps[u32TunerPort].tuner_get_freq_symb_offset = hi3136_get_freq_symb_offset;
#ifndef BOOT_COMPILE
                g_stTunerOps[u32TunerPort].tuner_get_signal_info = hi3136_get_signal_info;
                g_stTunerOps[u32TunerPort].tuner_set_ts_type = hi3136_set_ts_type;
                g_stTunerOps[u32TunerPort].tuner_blindscan_init   = hi3136_blindscan_init;
                g_stTunerOps[u32TunerPort].tuner_blindscan_action = hi3136_blindscan_action;
                /*g_stTunerOps[u32TunerPort].tuner_standby = hi3136_standby;
                g_stTunerOps[u32TunerPort].tuner_disable = hi3136_disable;*/
                g_stTunerOps[u32TunerPort].tuner_setfuncmode = hi3136_set_funcmode;
                g_stTunerOps[u32TunerPort].tuner_tp_verify = hi3136_tp_verify;
#endif
                g_stTunerOps[u32TunerPort].tuner_set_ts_out = hi3136_set_ts_out;
                break;
            }
#endif

        default:
            TUNER_DEBUG("%s, %d, %x\n", __func__, __LINE__, g_stTunerOps[u32TunerPort].enDemodDevType);
            return HI_FAILURE;
    }
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)

    if (u8ChipReset)
    {
        tuner_chip_reset(u32ResetGpioNo);
    }
#endif

#if 1
    /* Added begin:l00185424 2011-12-20 For AVL6211 */
    if (g_stTunerOps[u32TunerPort].tuner_init)
    {
        g_stTunerOps[u32TunerPort].tuner_init(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].enTunerDevType);
        //s_stTunerResumeInfo[u32TunerPort].stResumeData = stTunerAttr;
    }
    /* Added begin:l00185424 2011-11-28 For AVL6211 */
#endif
    switch (g_stTunerOps[u32TunerPort].enTunerDevType)
    {
#if defined(HI_BOOT_TUNER_DEV_TYPE_TDA18250)
        case HI_UNF_TUNER_DEV_TYPE_TDA18250:
        {
            s32Ret = tda18250_init_tuner(u32TunerPort);
            if(HI_FAILURE == s32Ret)
            {
                TUNER_DEBUG("tda18250 init error\n");
                return HI_FAILURE;
            }
            g_stTunerOps[u32TunerPort].set_tuner = tda18250_set_tuner;
            //g_stTunerOps[u32TunerPort].tuner_resume = tda18250_tuner_resume;
            //g_stTunerOps[u32TunerPort].tuner_get_signal_strength = tda18250_get_signal_strength;
            TUNER_DEBUG("tuner port %d's halftuner type is tda18250\n", u32TunerPort);
            break;
        }
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_TDA18250B)
        case HI_UNF_TUNER_DEV_TYPE_TDA18250B:
        {  
            s32Ret = tda18250b_init_tuner(u32TunerPort);
            if(HI_FAILURE == s32Ret)
            {
                TUNER_DEBUG("tda18250b init error\n");
                return HI_FAILURE;
            }
            g_stTunerOps[u32TunerPort].set_tuner = tda18250b_set_tuner;
            //g_stTunerOps[u32TunerPort].tuner_resume = tda18250b_tuner_resume;
            //g_stTunerOps[u32TunerPort].tuner_get_signal_strength = tda18250b_get_signal_strength;
            TUNER_DEBUG("tuner port %d's halftuner type is tda18250b\n", u32TunerPort);
            break;
        }        
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_SI2147)
        case HI_UNF_TUNER_DEV_TYPE_SI2147:
            {
		s32Ret = si2147_init_tuner(u32TunerPort);
                if(HI_FAILURE == s32Ret)
                {
                    TUNER_DEBUG("si2147 init error\n");
                    return HI_FAILURE;
                }
                g_stTunerOps[u32TunerPort].set_tuner = si2147_set_tuner;
                g_stTunerOps[u32TunerPort].tuner_resume = si2147_tuner_resume;
				//g_stTunerOps[u32TunerPort].tuner_get_signal_strength = si2147_get_signal_strength;
                TUNER_DEBUG("tuner port %d's halftuner type is si2147\n", u32TunerPort);
                break;
            }
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_M88TC3800)
        case HI_UNF_TUNER_DEV_TYPE_M88TC3800:
        {
            s32Ret = m88tc3800_init_tuner(u32TunerPort);
            if(HI_FAILURE == s32Ret)
            {
                TUNER_DEBUG("m88tc3800 init error\n");
                return HI_FAILURE;
            }
            g_stTunerOps[u32TunerPort].set_tuner = m88tc3800_set_tuner;
            //g_stTunerOps[u32TunerPort].tuner_resume = m88tc3800_tuner_resume;
            //g_stTunerOps[u32TunerPort].tuner_get_signal_strength = m88tc3800_get_signal_strength;
            TUNER_DEBUG("tuner port %d's halftuner type is m88tc3800\n", u32TunerPort);
            break;
        }
#endif


#if defined(HI_BOOT_TUNER_DEV_TYPE_ALPS_TDAE)
        case HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE:
        {
            g_stTunerOps[u32TunerPort].set_tuner = alps_tdae_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is alps_tdae\n", u32TunerPort);
            break;
        }
#endif

#if 0
        case HI_UNF_TUNER_DEV_TYPE_CD1616:
        {
            g_stTunerOps[u32TunerPort].set_tuner = cd1616_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is cd1616\n", u32TunerPort);
            break;
        }

        case HI_UNF_TUNER_DEV_TYPE_CD1616_DOUBLE:
        {
            g_stTunerOps[u32TunerPort].set_tuner = cd1616_double_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is cd1616_double\n", u32TunerPort);
            break;
        }
#endif
#if defined(HI_BOOT_TUNER_DEV_TYPE_TMX7070X)
        case HI_UNF_TUNER_DEV_TYPE_TMX7070X:
        {
            g_stTunerOps[u32TunerPort].set_tuner = tmx7070x_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is tmx7070x\n", u32TunerPort);
            break;
        }
#endif
#if defined(HI_BOOT_TUNER_DEV_TYPE_TDCC)
        case HI_UNF_TUNER_DEV_TYPE_TDCC:
        {
            g_stTunerOps[u32TunerPort].set_tuner = tdcc_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is tdcc\n", u32TunerPort);
            break;
        }
#endif
#if 0
        case HI_UNF_TUNER_DEV_TYPE_XG_3BL:
        {
            g_stTunerOps[u32TunerPort].set_tuner = xg_3bl_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is xg3bl\n", u32TunerPort);
            break;
        }

        case HI_UNF_TUNER_DEV_TYPE_MT2081:
        {
            g_stTunerOps[u32TunerPort].set_tuner = mt2081_set_tuner;
            //g_stTunerOps[u32TunerPort].tuner_resume = mt2081_tuner_resume;
            //g_stTunerOps[u32TunerPort].tuner_get_signal_strength = mt2081_get_signal_strength;
            TUNER_DEBUG("tuner port %d's halftuner type is mt2081\n", u32TunerPort);
            break;
        }
#endif
#if defined(HI_BOOT_TUNER_DEV_TYPE_R820C)
        case HI_UNF_TUNER_DEV_TYPE_R820C:
        {
            s32Ret = r820c_init_tuner(u32TunerPort);
            if(HI_FAILURE == s32Ret)
            {
                TUNER_DEBUG("r820c_init_tuner error\n");
                return HI_FAILURE;
            }
            g_stTunerOps[u32TunerPort].set_tuner = r820c_set_tuner;
            g_stTunerOps[u32TunerPort].tuner_resume = r820c_tuner_resume;
            g_stTunerOps[u32TunerPort].recalculate_signal_strength = r820c_get_signal_strength;
            TUNER_DEBUG("tuner port %d's halftuner type is r820c\n", u32TunerPort);
            break;
        }
#endif
#if defined(HI_BOOT_TUNER_DEV_TYPE_MXL203)
        case HI_UNF_TUNER_DEV_TYPE_MXL203:
        {
            g_stTunerOps[u32TunerPort].set_tuner = mxl203_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is mxl203\n", u32TunerPort);
            break;
        }
#endif
#if defined(HI_BOOT_TUNER_DEV_TYPE_AV2011)
        case HI_UNF_TUNER_DEV_TYPE_AV2011:
        {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
            g_stTunerOps[u32TunerPort].set_tuner = av2011_set_tuner;
#endif
            TUNER_DEBUG("tuner port %d's halftuner type is AV2011\n", u32TunerPort);
            break;
        }
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_SHARP7903)
        case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
        {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
            g_stTunerOps[u32TunerPort].set_tuner = sharp7903_set_tuner;
#endif
            TUNER_DEBUG("tuner port %d's halftuner type is SHARP7903\n", u32TunerPort);
            break;
        }
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_RDA5815)
        case HI_UNF_TUNER_DEV_TYPE_RDA5815:
            {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
                g_stTunerOps[u32TunerPort].set_tuner = RDA5815_set_tuner;
#endif
                TUNER_DEBUG("tuner port %d's halftuner type is RDA5815\n", u32TunerPort);
                break;
            }
#endif

#if defined(HI_BOOT_TUNER_DEV_TYPE_M88TS2022)
        case HI_UNF_TUNER_DEV_TYPE_M88TS2022:
            {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
                g_stTunerOps[u32TunerPort].set_tuner = M88TS2022_set_tuner;
#endif
                TUNER_DEBUG("tuner port %d's halftuner type is M88TS2022\n", u32TunerPort);
                break;
            }
#endif

        case HI_UNF_TUNER_DEV_TYPE_MXL101:
        {
            TUNER_DEBUG("tuner port %d's halftuner type is MXL101\n", u32TunerPort);
            break;
        }
#if defined(HI_BOOT_TUNER_DEV_TYPE_MXL603)
        case HI_UNF_TUNER_DEV_TYPE_MXL603:
        {
            g_stTunerOps[u32TunerPort].set_tuner = mxl603_set_tuner_dvb_c;
            break;
        }
#endif
        case HI_UNF_TUNER_DEV_TYPE_IT9170:
        {
            TUNER_DEBUG("tuner port %d's halftuner type is IT9170\n", u32TunerPort);
            break;
        }
        case HI_UNF_TUNER_DEV_TYPE_IT9133:
        {
            TUNER_DEBUG("tuner port %d's halftuner type is IT9133\n", u32TunerPort);
            break;
        }
#if 0
        case HI_UNF_TUNER_DEV_TYPE_TDA6651:
        {
            g_stTunerOps[u32TunerPort].set_tuner = tda6651_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type is tda6651\n", u32TunerPort);
            break;
        }
#endif
        default:
        {
            //g_stTunerOps[u32TunerPort].set_tuner = cd1616_set_tuner;
            TUNER_DEBUG("tuner port %d's halftuner type error\n", u32TunerPort);
            break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 tuner_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
    if (g_stTunerOps[u32TunerPort].tuner_set_sat_attr)
    {
        g_stTunerOps[u32TunerPort].tuner_set_sat_attr(u32TunerPort, pstSatTunerAttr);
    }

    /* For DVB-S/S2 demod, need configurate LNB control device */
    switch (g_stTunerOps[u32TunerPort].enDemodDevType)
    {
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
        case HI_UNF_DEMOD_DEV_TYPE_3136:
        case HI_UNF_DEMOD_DEV_TYPE_3136I:
            {
                g_stTunerOps[u32TunerPort].tuner_set_lnb_out = HI_NULL;
                g_stTunerOps[u32TunerPort].tuner_send_continuous_22K = hi3136_send_continuous_22K;
                //g_stTunerOps[u32TunerPort].tuner_send_tone = hi3136_send_tone;
                g_stTunerOps[u32TunerPort].tuner_DiSEqC_send_msg = hi3136_DiSEqC_send_msg;
                /*g_stTunerOps[u32TunerPort].tuner_DiSEqC_recv_msg = hi3136_DiSEqC_recv_msg;*/

                /* Which device send wave */
                switch (pstSatTunerAttr->enDiSEqCWave)
                {
                        /* Demod send wave, LNB control only supply power */
                    case HI_UNF_TUNER_DISEQCWAVE_NORMAL:
                    default:
                        {
                            switch (pstSatTunerAttr->enLNBCtrlDev)
                            {
#if defined(HI_BOOT_LNB_CTRL_MPS8125)
                                case HI_UNF_LNBCTRL_DEV_TYPE_MPS8125:
                                    g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init = mps8125_init;
                                    //g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_standby = mps8125_standby;
                                    g_stTunerOps[u32TunerPort].tuner_set_lnb_out = mps8125_set_lnb_out;
                                    break;
#endif

#if defined(HI_BOOT_LNB_CTRL_ISL9492)
                                case HI_UNF_LNBCTRL_DEV_TYPE_ISL9492:
                                    g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init = isl9492_init;
                                    //g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_standby = isl9492_standby;
                                    g_stTunerOps[u32TunerPort].tuner_set_lnb_out = isl9492_set_lnb_out;
                                    break;
#endif

                                case HI_UNF_LNBCTRL_DEV_TYPE_NONE:
                                default:
                                    HI_INFO_TUNER("LNB ctrl chiptype error!\n");
                                    return HI_FAILURE;
                            }

                            break;
                        }

                        /* LNB control device send wave */
                    case HI_UNF_TUNER_DISEQCWAVE_ENVELOPE:
                        {
                            switch (pstSatTunerAttr->enLNBCtrlDev)
                            {
#if 0//defined(HI_BOOT_LNB_CTRL_MPS8125)
                                case HI_UNF_LNBCTRL_DEV_TYPE_MPS8125:
                                    g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init = mps8125_init;
                                    //g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_standby = mps8125_standby;
                                    g_stTunerOps[u32TunerPort].tuner_set_lnb_out = mps8125_set_lnb_out;
                                    g_stTunerOps[u32TunerPort].tuner_send_continuous_22K = mps8125_send_continuous_22K;
                                    g_stTunerOps[u32TunerPort].tuner_send_tone = mps8125_send_tone;
                                    g_stTunerOps[u32TunerPort].tuner_DiSEqC_send_msg = mps8125_DiSEqC_send_msg;
                                    g_stTunerOps[u32TunerPort].tuner_DiSEqC_recv_msg = mps8125_DiSEqC_recv_msg;
                                    break;
#endif

#if 0//defined(HI_BOOT_LNB_CTRL_ISL9492)
                                case HI_UNF_LNBCTRL_DEV_TYPE_ISL9492:
                                    g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init = isl9492_init;
                                    //g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_standby = isl9492_standby;
                                    g_stTunerOps[u32TunerPort].tuner_set_lnb_out = isl9492_set_lnb_out;
                                    g_stTunerOps[u32TunerPort].tuner_send_continuous_22K = isl9492_send_continuous_22K;
                                    g_stTunerOps[u32TunerPort].tuner_send_tone = isl9492_send_tone;
                                    g_stTunerOps[u32TunerPort].tuner_DiSEqC_send_msg = isl9492_DiSEqC_send_msg;
                                    g_stTunerOps[u32TunerPort].tuner_DiSEqC_recv_msg = isl9492_DiSEqC_recv_msg;
                                    break;
#endif

                                case HI_UNF_LNBCTRL_DEV_TYPE_NONE:
                                default:
                                    HI_INFO_TUNER("LNB ctrl chiptype error!\n");
                                    return HI_FAILURE;
                            }

                            break;
                        }
                }

                break;
            }
#endif

        default:
            {
                HI_INFO_TUNER("Demod type error! Not sat demod!\n");
                return HI_FAILURE;
            }
    }
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)

    if (g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init)
    {
        g_stTunerOps[u32TunerPort].tuner_lnbctrl_dev_init(u32TunerPort,
                                                          g_stTunerOps[u32TunerPort].enI2cChannel,
                                                          pstSatTunerAttr->u16LNBDevAddress, g_stTunerOps[u32TunerPort].enDemodDevType);
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_GetStatus(HI_U32    u32TunerPort , HI_UNF_TUNER_STATUS_S  *pstTunerStatus)
{
    HI_S32 s32Ret = 0;

    /* get tuner port  number and if tuner port is not supported , return fault */
    if (HI_SUCCESS != tuner_check_port(u32TunerPort))
    {
        return HI_FAILURE;
    }

    /* if tuner get status operation is not setted, return fault */
    if (!g_stTunerOps[u32TunerPort].tuner_get_status) /* check if tuner get status is supported */
    {
        TUNER_DEBUG( "tuner service function ptr is NULL!\n");
        return HI_FAILURE;
    }

    /* get tuner lock status */
    s32Ret = g_stTunerOps[u32TunerPort].tuner_get_status(u32TunerPort, &(pstTunerStatus->enLockStatus));
    if (HI_SUCCESS != s32Ret)
    {
        TUNER_DEBUG( "\n tuner_get_status failed \n");
        return s32Ret;
    }

    s_enTunerStat[u32TunerPort] = pstTunerStatus->enLockStatus;

    return HI_SUCCESS;
}

/* Convert downlink frequency to IF, calculate LNB 22K status synchronously, for connect */
/*Downlink freq dominates the course, i.e, the freq band is decided by the downlink freq*/
static HI_VOID TUNER_DownlinkFreqToIF(HI_UNF_TUNER_FE_LNB_CONFIG_S* pstLNBConfig,
                                      HI_UNF_TUNER_FE_POLARIZATION_E enPolar, HI_U32 u32DownlinkFreq,
                                      HI_U32* pu32IF, HI_UNF_TUNER_FE_LNB_22K_E *penLNB22K)
{
    /* Default */
    if (HI_NULL != penLNB22K)
    {
        *penLNB22K = HI_UNF_TUNER_FE_LNB_22K_OFF;
    }

    if ((SAT_C_MIN_KHZ <= u32DownlinkFreq)
            && (SAT_C_MAX_KHZ >= u32DownlinkFreq))
    {
        pstLNBConfig->enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_C;
    }
    else if ((SAT_KU_MIN_KHZ <= u32DownlinkFreq)
            && (SAT_KU_MAX_KHZ >= u32DownlinkFreq))
    {
        pstLNBConfig->enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_KU;
    }
    else
    {
        HI_ERR_TUNER("Error freq!\n");
        return;
    }

    switch (pstLNBConfig->enLNBBand)
    {
        /* C band, IF = LO - downlink frequency */
    case HI_UNF_TUNER_FE_LNB_BAND_C:
        /* Single LO */
        if ((HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY == pstLNBConfig->enLNBType)
           || (pstLNBConfig->u32HighLO == pstLNBConfig->u32LowLO))
        {
            *pu32IF = pstLNBConfig->u32LowLO * 1000 - u32DownlinkFreq;
        }
        /* Dual LO */
        else
        {
            /* V/R polarization, use high LO */
            if ((HI_UNF_TUNER_FE_POLARIZATION_V == enPolar) || (HI_UNF_TUNER_FE_POLARIZATION_R == enPolar))
            {
                *pu32IF = pstLNBConfig->u32HighLO * 1000 - u32DownlinkFreq;
            }
            /* H/L polarization, use low LO */
            else
            {
                *pu32IF = pstLNBConfig->u32LowLO * 1000 - u32DownlinkFreq;
            }
        }

        break;

    /* Ku band, IF = downlink frequency - LO */
    case HI_UNF_TUNER_FE_LNB_BAND_KU:

        /* Single LO */
        if ((HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY == pstLNBConfig->enLNBType)
           || (pstLNBConfig->u32HighLO == pstLNBConfig->u32LowLO))
        {
            *pu32IF = u32DownlinkFreq - pstLNBConfig->u32LowLO * 1000;
        }
        /* Dual LO */
        else
        {
            /* downlink frequency >= 11700MHz, use high LO */
            if ((u32DownlinkFreq >= SAT_DOWNLINK_FREQ_KU_MID * 1000))
            {
                *pu32IF = u32DownlinkFreq - pstLNBConfig->u32HighLO * 1000;

                /* Ku dual LO LNB use 22K select high LO */
                if (HI_NULL != penLNB22K)
                {
                    *penLNB22K = HI_UNF_TUNER_FE_LNB_22K_ON;
                }
            }
            /* downlink frequency < 11700MHz, use low LO */
            else
            {
                *pu32IF = u32DownlinkFreq - pstLNBConfig->u32LowLO * 1000;
            }
        }

        break;

    default:
        break;
    }
}

#if 1
static HI_S32 TUNER_SetLNBOutAnd22K(HI_U32 u32TunerId,
                                    HI_UNF_TUNER_FE_POLARIZATION_E enPolar, HI_UNF_TUNER_FE_LNB_22K_E enLNB22K)
{
    TUNER_LNB_OUT_S stLNBOut;
    //TUNER_DATA_S stTunerData;
    HI_S32 s32Ret;

    switch (s_stSatPara[u32TunerId].enLNBPower)
    {
    /* 0V */
    case HI_UNF_TUNER_FE_LNB_POWER_OFF:
        stLNBOut.enOut = TUNER_LNB_OUT_0V;
        break;

    /* 13V/18V */
    case HI_UNF_TUNER_FE_LNB_POWER_ON:
        if ((HI_UNF_TUNER_FE_POLARIZATION_V == enPolar) || (HI_UNF_TUNER_FE_POLARIZATION_R == enPolar))
        {
            stLNBOut.enOut = TUNER_LNB_OUT_13V;
        }
        else
        {
            stLNBOut.enOut = TUNER_LNB_OUT_18V;
        }

        break;

        /* 14V/19V */
    case HI_UNF_TUNER_FE_LNB_POWER_ENHANCED:
        if ((HI_UNF_TUNER_FE_POLARIZATION_V == enPolar) || (HI_UNF_TUNER_FE_POLARIZATION_R == enPolar))
        {
            stLNBOut.enOut = TUNER_LNB_OUT_14V;
        }
        else
        {
            stLNBOut.enOut = TUNER_LNB_OUT_19V;
        }
        break;

    default:
        return HI_ERR_TUNER_INVALID_PARA;
    }

    stLNBOut.u32Port = u32TunerId;
    /* if tuner blind scan operation is not setted, return fault */
    if (!g_stTunerOps[u32TunerId].tuner_set_lnb_out)
    {
        HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
        return HI_FAILURE;
    }

    s32Ret = g_stTunerOps[u32TunerId].tuner_set_lnb_out(u32TunerId, stLNBOut.enOut);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_TUNER( "tuner_set_lnb_out failed!\n");
        return s32Ret;
    }

    /* Save polarization status */
    s_stSatPara[u32TunerId].enPolar = enPolar;

    /* 22K signal control. If LNB power off, can't send 22K signal. */
    if (HI_UNF_TUNER_FE_LNB_POWER_OFF != s_stSatPara[u32TunerId].enLNBPower)
    {
        /* If has 22K switch, 22K controlled by switch.
           If hasn't 22K switch, 22K controlled by tuner lock or blind scan.
         */
        if (HI_UNF_TUNER_SWITCH_22K_NONE == s_stSatPara[u32TunerId].enSwitch22K)
        {
            /* if tuner blind scan operation is not setted, return fault */
            if (!g_stTunerOps[u32TunerId].tuner_send_continuous_22K)
            {
                HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
                return HI_FAILURE;
            }

            s32Ret = g_stTunerOps[u32TunerId].tuner_send_continuous_22K(u32TunerId, enLNB22K);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("Set continuous 22K fail.\n");
                return HI_ERR_TUNER_FAILED_LNBCTRL;
            }
        }
    }

    /* Save polarization status */
    s_stSatPara[u32TunerId].enLNB22K = enLNB22K;
    return HI_SUCCESS;
}
#endif

static HI_S32 TUNER_DISEQC_Send22K(HI_U32 u32TunerId, HI_BOOL bStatus)
{
    TUNER_DATA_S stTunerData;
    HI_S32 s32Ret = HI_FAILURE;

    stTunerData.u32Port = u32TunerId;
    stTunerData.u32Data = bStatus ? 1 : 0;
    /* if tuner blind scan operation is not setted, return fault */
    if (!g_stTunerOps[u32TunerId].tuner_send_continuous_22K)
    {
        HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
        return HI_FAILURE;
    }

    s32Ret = g_stTunerOps[u32TunerId].tuner_send_continuous_22K(u32TunerId, stTunerData.u32Data);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Set continuous 22K fail.\n");
        return HI_ERR_TUNER_FAILED_DISEQC;
    }

    return HI_SUCCESS;
}

static HI_S32 TUNER_DISEQC_Stop22K(HI_U32 u32TunerId)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = TUNER_DISEQC_Send22K(u32TunerId, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Stop 22K fail.\n");
        return HI_ERR_TUNER_FAILED_SWITCH;
    }

    /* Save status */
    //s_stSatPara[u32TunerId].enSavedSwitch22K = s_stSatPara[u32TunerId].enSwitch22K;
    return HI_SUCCESS;
}

static HI_S32 TUNER_DISEQC_Resume22K(HI_U32 u32TunerId)
{
    /* Resume */
    if (HI_UNF_TUNER_SWITCH_22K_22 == s_stSatPara[u32TunerId].enSwitch22K/*enSavedSwitch22K*/)
    {
        return TUNER_DISEQC_Send22K(u32TunerId, HI_TRUE);
    }
    else
    {
        return HI_SUCCESS;
    }
}

HI_UNF_TUNER_SWITCH_TONEBURST_E TUNER_DISEQC_GetToneBurstStatus(HI_U32 u32TunerId)
{
    return s_stSatPara[u32TunerId].enToneBurst;
}

//HI_S32 HI_TUNER_Connect(PTR_TUNER_SIGNAL_S pstTunerSignal)
HI_S32 HI_UNF_TUNER_Connect(HI_U32  u32tunerId , const HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32TimeSpan = 0;
    TUNER_ACC_QAM_PARAMS_S stSignal = {0};
    HI_UNF_TUNER_LOCK_STATUS_E enTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    HI_UNF_TUNER_FE_LNB_22K_E enLNB22K;

    if (HI_SUCCESS != tuner_check_port(u32tunerId))
    {
        TUNER_DEBUG( "invalid tuner port: %d\n", u32tunerId);
        return HI_FAILURE;
    }

    /* Cable */
    if (HI_UNF_TUNER_SIG_TYPE_CAB == pstConnectPara->enSigType)
    {
        g_stTunerOps[u32tunerId].enSigType = pstConnectPara->enSigType;
        g_stTunerOps[u32tunerId].stCurrPara.u32Frequency = pstConnectPara->unConnectPara.stCab.u32Freq;
        g_stTunerOps[u32tunerId].stCurrPara.unSRBW.u32SymbolRate = pstConnectPara->unConnectPara.stCab.u32SymbolRate;
        //g_stTunerOps[u32tunerId].stCurrPara.enQamType = pstConnectPara->unConnectPara.stCab.enModType;
        g_stTunerOps[u32tunerId].stCurrPara.bSI = pstConnectPara->unConnectPara.stCab.bReverse;

        stSignal.u32Frequency = pstConnectPara->unConnectPara.stCab.u32Freq;
        stSignal.unSRBW.u32SymbolRate = pstConnectPara->unConnectPara.stCab.u32SymbolRate;

        switch(pstConnectPara->unConnectPara.stCab.enModType)
        {
            case HI_UNF_MOD_TYPE_QAM_16:
                stSignal.enQamType = QAM_TYPE_16;
                g_stTunerOps[u32tunerId].stCurrPara.enQamType = QAM_TYPE_16;
                break;

            case HI_UNF_MOD_TYPE_QAM_32:
                stSignal.enQamType = QAM_TYPE_32;
                g_stTunerOps[u32tunerId].stCurrPara.enQamType = QAM_TYPE_32;
                break;

            case HI_UNF_MOD_TYPE_QAM_64:
            case HI_UNF_MOD_TYPE_DEFAULT:
                stSignal.enQamType = QAM_TYPE_64;
                g_stTunerOps[u32tunerId].stCurrPara.enQamType = QAM_TYPE_64;
                break;

            case HI_UNF_MOD_TYPE_QAM_128:
                stSignal.enQamType = QAM_TYPE_128;
                g_stTunerOps[u32tunerId].stCurrPara.enQamType = QAM_TYPE_128;
                break;

            case HI_UNF_MOD_TYPE_QAM_256:
                stSignal.enQamType = QAM_TYPE_256;
                g_stTunerOps[u32tunerId].stCurrPara.enQamType = QAM_TYPE_256;
                break;
            default:
                TUNER_DEBUG("Tuner HI_UNF_SET_eqamType error:%d\n",pstConnectPara->unConnectPara.stCab.enModType);
                return HI_FAILURE;
        }

        stSignal.bSI = pstConnectPara->unConnectPara.stCab.bReverse;

        /* if tuner connect operation is not setted, return fault */
        if (!g_stTunerOps[u32tunerId].tuner_connect) /* check if tuner conect is supported */
        {
            TUNER_DEBUG( "tuner service function ptr is NULL!\n");
            return HI_FAILURE;
        }

        /* call tuner connect operation */
        s32Ret = g_stTunerOps[u32tunerId].tuner_connect(u32tunerId, &stSignal);
        if (HI_SUCCESS != s32Ret)
        {
            TUNER_DEBUG( "\n tuner_connect failed \n");
            return s32Ret;
        }

        while (u32TimeSpan < u32TimeOut)
        {
            if (g_stTunerOps[u32tunerId].tuner_get_status) /* check if tuner get status is supported */
            {
                /* get tuner lock status */
                s32Ret = g_stTunerOps[u32tunerId].tuner_get_status(u32tunerId, &enTunerStatus);
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG( "\n tuner_get_status failed \n");
                    return s32Ret;
                }
            }

            if(HI_UNF_TUNER_SIGNAL_LOCKED == enTunerStatus)
            {
                return HI_SUCCESS;
            }
            else
            {
                udelay(10 * 1000);
                u32TimeSpan += 10;
            }
        }

    }
    /* Satellite */
    else if (HI_UNF_TUNER_SIG_TYPE_SAT == pstConnectPara->enSigType)
    {
        if (!((SAT_C_MIN_KHZ <= pstConnectPara->unConnectPara.stSat.u32Freq)
                && (SAT_C_MAX_KHZ >= pstConnectPara->unConnectPara.stSat.u32Freq))
            && !((SAT_KU_MIN_KHZ <= pstConnectPara->unConnectPara.stSat.u32Freq)
                && (SAT_KU_MAX_KHZ >= pstConnectPara->unConnectPara.stSat.u32Freq)))
        {
            HI_ERR_TUNER("Input parameter(u32Freq) invalid: %d\n",
                                pstConnectPara->unConnectPara.stSat.u32Freq);
            return HI_ERR_TUNER_INVALID_PARA;
        }

        if (SAT_SYMBOLRATE_MAX < pstConnectPara->unConnectPara.stSat.u32SymbolRate)
        {
            HI_ERR_TUNER("Input parameter(u32SymbolRate) invalid: %d\n",
                                pstConnectPara->unConnectPara.stSat.u32SymbolRate);
            return HI_ERR_TUNER_INVALID_PARA;
        }

        if (HI_UNF_TUNER_FE_POLARIZATION_BUTT <= pstConnectPara->unConnectPara.stSat.enPolar)
        {
            HI_ERR_TUNER("Input parameter(enPolar) invalid: %d\n",
                                pstConnectPara->unConnectPara.stSat.enPolar);
            return HI_ERR_TUNER_INVALID_PARA;

        }

        g_stTunerOps[u32tunerId].enSigType = pstConnectPara->enSigType;
        g_stTunerOps[u32tunerId].stCurrPara.u32Frequency = pstConnectPara->unConnectPara.stSat.u32Freq;
        g_stTunerOps[u32tunerId].stCurrPara.unSRBW.u32SymbolRate = pstConnectPara->unConnectPara.stSat.u32SymbolRate;
        g_stTunerOps[u32tunerId].stCurrPara.enPolar = pstConnectPara->unConnectPara.stSat.enPolar;

        /* Convert downlink frequency to IF */
        TUNER_DownlinkFreqToIF(&(s_stSatPara[u32tunerId].stLNBConfig), pstConnectPara->unConnectPara.stSat.enPolar,
                               pstConnectPara->unConnectPara.stSat.u32Freq, &(stSignal.u32Frequency), &enLNB22K);

        stSignal.unSRBW.u32SymbolRate = pstConnectPara->unConnectPara.stSat.u32SymbolRate;
        stSignal.enPolar = pstConnectPara->unConnectPara.stSat.enPolar;

#if 1
        /* LNB power and 22K signal switch */
        s32Ret = TUNER_SetLNBOutAnd22K(u32tunerId, pstConnectPara->unConnectPara.stSat.enPolar, enLNB22K);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("TUNER_SetLNBOutAnd22K fail.\n");
        }
#endif

        /* if tuner connect operation is not setted, return fault */
        if (!g_stTunerOps[u32tunerId].tuner_connect) /* check if tuner conect is supported */
        {
            TUNER_DEBUG( "tuner service function ptr is NULL!\n");
            return HI_FAILURE;
        }

        /* call tuner connect operation */
        s32Ret = g_stTunerOps[u32tunerId].tuner_connect(u32tunerId, &stSignal);
        if (HI_SUCCESS != s32Ret)
        {
            TUNER_DEBUG( "\n tuner_connect failed \n");
            return s32Ret;
        }

        if (0 == u32TimeOut)
        {
            return HI_SUCCESS;
        }

        while (u32TimeSpan < u32TimeOut)
        {
            if (g_stTunerOps[u32tunerId].tuner_get_status) /* check if tuner get status is supported */
            {
                /* get tuner lock status */
                s32Ret = g_stTunerOps[u32tunerId].tuner_get_status(u32tunerId, &enTunerStatus);
                if (HI_SUCCESS != s32Ret)
                {
                    TUNER_DEBUG( "\n tuner_get_status failed \n");
                    return s32Ret;
                }
            }

            if (HI_UNF_TUNER_SIGNAL_LOCKED == enTunerStatus)
            {
                TUNER_DEBUG("Tuner LOCKED!\n");
                return HI_SUCCESS;
            }
            else
            {
                udelay(10 * 1000);
                u32TimeSpan += 10;
            }
        }

        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

static HI_VOID tuner_enable_crg(HI_VOID)
{
#if defined (CHIP_TYPE_hi3716cv200) || defined (CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
     U_PERI_CRG57 unTmpPeriCrg57;
     U_PERI_CRG58 unTmpPeriCrg58;
     U_PERI_QAM    unTmpPeriQam;
    g_u32CrgEnableFlag= 1;

    unTmpPeriCrg57.u32 = g_pstRegCrg->PERI_CRG57.u32;
    unTmpPeriCrg58.u32 = g_pstRegCrg->PERI_CRG58.u32;
    unTmpPeriQam.u32 = g_pstRegPeri->PERI_QAM.u32;

    {
        /*select QAM TSCLK source 150MHz,PERI_CRG57[19:18]:01 ; QAM TSCLK 16 div,PERI_CRG57[23:20] :0111,*/
        unTmpPeriCrg57.bits.qam_ts_clk_sel =  0x01;
        unTmpPeriCrg57.bits.qam_ts_clk_div =0x7;
        /*TS_CLK_OUT config  the opposite phase,PERI_CRG57[16]:1;*/
        /*ads_clk config the same phase,PERI_CRG57[17]:0*/
        unTmpPeriCrg57.bits.qam_tsout_pctrl = 0x1;
        unTmpPeriCrg57.bits.qam_adsclk_pctrl = 0;
        /*config qam_*_cken open, PERI_CRG57[6:0]:1111111*/
        unTmpPeriCrg57.bits.qam_2x_cken = 1;
        unTmpPeriCrg57.bits.qam_1x_cken = 1;

        unTmpPeriCrg57.bits.qam_j83b_cken = 1;
        unTmpPeriCrg57.bits.qam_j83ac_cken = 1;
        unTmpPeriCrg57.bits.qam_ads_cken = 1;

        unTmpPeriCrg57.bits.qam_ts_cken = 1;
        unTmpPeriCrg57.bits.qam_tsout_cken = 1;

        /*don't reset qam_ads_srst_req and qam_ts_srst_req, PERI_CRG57[13:12]:00*/
        unTmpPeriCrg57.bits.qam_ads_srst_req = 0;
        unTmpPeriCrg57.bits.qam_ts_srst_req = 0;
        /*don't reset qam_2x_srst_req and qam_1x_srst_req, PERI_CRG57[9:8]:00*/
        unTmpPeriCrg57.bits.qam_2x_srst_req = 0;
        unTmpPeriCrg57.bits.qam_1x_srst_req = 0;

        /*config adc clk 28.8MHz*/
        /*config qamadc_clk_sel 28.8MHz,  PERI_CRG58[9:8]:00*/
        unTmpPeriCrg58.bits.qamadc_clk_sel = 0x00;

        /*config qamctrl_clk_sel 28.8MHz , PERI_CRG58[11:10]:00; \
            config qamctrl_clk_div for the same frequence, PERI_CRG58[12]:0*/
        unTmpPeriCrg58.bits.qamctrl_clk_sel = 0x00;
        unTmpPeriCrg58.bits.qamctrl_clk_div = 0;

        unTmpPeriQam.bits.qam_i2c_devaddr =0x50;
    }
    g_pstRegCrg->PERI_CRG57.u32 = unTmpPeriCrg57.u32;
    g_pstRegCrg->PERI_CRG58.u32 = unTmpPeriCrg58.u32;
    g_pstRegPeri->PERI_QAM.u32 = unTmpPeriQam.u32;
#endif
}

static HI_VOID tuner_enable_adc(HI_VOID)
{

#if defined (CHIP_TYPE_hi3716cv200)|| defined (CHIP_TYPE_hi3716mv400)|| defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    HI_U32 i=30;
    U_PERI_QAM_ADC0 unTmpPeriQamADC0;
    U_PERI_QAM_ADC1 unTmpPeriQamADC1;

    unTmpPeriQamADC0.u32 = g_pstRegPeri->PERI_QAM_ADC0.u32;
    unTmpPeriQamADC1.u32 = g_pstRegPeri->PERI_QAM_ADC1.u32;
    {
      /*ADC power down:PERI_QAM_ADC0[1:0]:00*/
      unTmpPeriQamADC0.bits.opm = 00;
      /*ADC i2c_resetz :0 */
      unTmpPeriQamADC1.bits.i2c_resetz = 0;
    }
    g_pstRegPeri->PERI_QAM_ADC0.u32 = unTmpPeriQamADC0.u32;
    msleep(1);
    g_pstRegPeri->PERI_QAM_ADC1.u32 = unTmpPeriQamADC1.u32;
    msleep(1);
     /*ADC i2c_resetz :1 */
    unTmpPeriQamADC1.bits.i2c_resetz = 1;
    g_pstRegPeri->PERI_QAM_ADC1.u32 = unTmpPeriQamADC1.u32;
    msleep(1);
    {
      /*ADC Normal Operation:PERI_QAM_ADC0[1:0]:11*/
      unTmpPeriQamADC0.bits.opm = 3;
    }

    g_pstRegPeri->PERI_QAM_ADC0.u32 = unTmpPeriQamADC0.u32;

    while(!g_pstRegPeri->PERI_QAM_ADC0.bits.adcrdy)
    {
     if(!i)
     {
          HI_ERR_TUNER( "ADC is not ready\n");
          break;
     }
     msleep(1);
     i--;
    }
#endif

}


HI_VOID reset_special_process_flag(HI_BOOL flag)
{
    g_bLockFlag = flag;
    return;
}

HI_S32 HI_UNF_TUNER_Init(HI_VOID)
{
    HI_U32 i;

    if( s_bTunerInited )
    {
        return HI_SUCCESS;
    }

    tuner_enable_crg();

    for (i=0; i<TUNER_NUM; i++)
    {
#if 0
        /* Default single frequency, C band SAT_LO_C_L */
        s_stSatPara[i].pBlindScanMonitor = HI_NULL;
        //s_stSatPara[0].pConnectMonitor   = HI_NULL;
        s_stSatPara[i].bBlindScanStop  = HI_FALSE;
        s_stSatPara[i].bBlindScanBusy  = HI_FALSE;
        //s_stSatPara[0].bConnectStop       = HI_FALSE;
#endif
        s_stSatPara[i].stLNBConfig.enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_C;
        s_stSatPara[i].stLNBConfig.enLNBType = HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY;
        s_stSatPara[i].stLNBConfig.u32LowLO  = SAT_LO_C_L;
        s_stSatPara[i].stLNBConfig.u32HighLO = SAT_LO_C_L;
        s_stSatPara[i].enLNBPower  = HI_UNF_TUNER_FE_LNB_POWER_ON;
        s_stSatPara[i].enSwitch22K = HI_UNF_TUNER_SWITCH_22K_NONE;
        s_stSatPara[i].enToneBurst = HI_UNF_TUNER_SWITCH_TONEBURST_NONE;
    }

    g_stTunerOps[0].u32TunerAddress = TUNER_PORT0_ADDR;
    g_stTunerOps[0].u32DemodAddress = QAM_PORT0_ADDR;

#if (3 == TUNER_NUM)
    g_stTunerOps[1].u32TunerAddress = TUNER_PORT1_ADDR;
    g_stTunerOps[2].u32TunerAddress = TUNER_PORT2_ADDR;

    g_stTunerOps[1].u32DemodAddress = QAM_PORT1_ADDR;
    g_stTunerOps[2].u32DemodAddress = QAM_PORT2_ADDR;
#endif

    /* init all tuner ops as none operation */
    for (i = 0; i < TUNER_NUM; i++)
    {
        g_stTunerOps[i].u32XtalClk = XTAL_CLK;
        g_stTunerOps[i].u32CurrQamMode = QAM_TYPE_64;
        g_stTunerOps[i].u8AdcType = QAM_AD_INSIDE;
        g_stTunerOps[i].u8AgcOutputSel = QAM_AGC_CMOS_OUTPUT;
        g_stTunerOps[i].u8AdcDataFmt = 1;
        g_stTunerOps[i].enTsType = HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A;
        g_stTunerOps[i].enI2cChannel = i;

        g_stTunerOps[i].tuner_connect = NULL;
        g_stTunerOps[i].tuner_get_status = NULL;
        g_stTunerOps[i].tuner_connect_timeout = NULL;

        s_bTunerInited = HI_TRUE;
#if 0
        g_stTunerOps[i].tuner_get_ber = NULL;
        g_stTunerOps[i].tuner_get_snr = NULL;
        g_stTunerOps[i].tuner_get_signal_strength = NULL;
        g_stTunerOps[i].tuner_get_snr = NULL;
        g_stTunerOps[i].tuner_set_ts_type = NULL;
        g_stTunerOps[i].set_tuner   = NULL;
        g_stTunerOps[i].tuner_test_single_agc = NULL;
        g_stTunerOps[i].tuner_resume = NULL;
        g_stTunerOps[i].tuner_get_rs = NULL;
        g_stTunerOps[i].tuner_get_registers = NULL;


        /* Added begin:l00185424 2011-11-28 For DVB-S/S2 */
        g_stTunerOps[i].tuner_init = NULL;
        g_stTunerOps[i].tuner_get_signal_info = NULL;
        g_stTunerOps[i].tuner_blindscan_init   = NULL;
        g_stTunerOps[i].tuner_blindscan_action = NULL;
        g_stTunerOps[i].tuner_lnbctrl_dev_init = NULL;
        g_stTunerOps[i].tuner_lnbctrl_dev_standby = NULL;
        g_stTunerOps[i].tuner_set_lnb_out = NULL;
        g_stTunerOps[i].tuner_send_tone = NULL;
        g_stTunerOps[i].tuner_DiSEqC_send_msg = NULL;
        g_stTunerOps[i].tuner_DiSEqC_recv_msg = NULL;
        g_stTunerOps[i].tuner_disable = NULL;
        g_stTunerOps[i].tuner_setfuncmode = NULL;
        g_stTunerOps[i].tuner_setplpid = NULL;
        g_stTunerOps[i].tuner_standby = NULL;
        /* Added end:l00185424 2011-11-28 For DVB-S/S2 */
#endif
    }
#if 0
    init_waitqueue_head(&g_QamSpecialProcessWQ);
    pThread = kthread_create(x5hdqam_special_process, NULL, "x5hdqam_sop");
    wake_up_process(pThread);
#endif
    return HI_SUCCESS;
}

HI_S32  HI_UNF_TUNER_DeInit(HI_VOID)
{
    //kthread_stop(pThread);
    s_bTunerInited = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_Open (HI_U32  u32tunerId)
{
    HI_S32    Ret = 0;
    tuner_enable_crg();
    tuner_enable_adc();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_Close(HI_U32    u32tunerId)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_GetDeftAttr(HI_U32 u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr)
{
#if 0
    if( !s_bTunerInited )
    {
        API_DEBUG("TUNER UNF hasn't been Inited\n");
        return HI_FAILURE;
    }

    if(TUNER_NUM <= u32tunerId)
    {
        API_DEBUG("Input parameter(u32tunerId) invalid,invalid tunerId is: %d\n",u32tunerId);
        return HI_FAILURE;
    }

    if(HI_NULL == pstTunerAttr)
    {
        API_DEBUG("Input parameter(pstTunerAttr) invalid\n");
        return HI_FAILURE;
    }

    memcpy(pstTunerAttr, &s_strDeftTunerAttr[u32tunerId], sizeof(HI_UNF_TUNER_ATTR_S));
#endif

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_SetAttr(HI_U32  u32tunerId , const HI_UNF_TUNER_ATTR_S *pstTunerAttr)
{
    HI_S32 s32Result;
    //TUNER_DATA_S stTunerData;
    //TUNER_DATABUF_S stDataBuf = {0};
    HI_TunerAttr_S stTuner = {HI_UNF_TUNER_DEV_TYPE_BUTT, 0};
    HI_DemodAttr_S stDemod = {HI_UNF_DEMOD_DEV_TYPE_BUTT, 0};

    if(TUNER_NUM <= u32tunerId)
    {
        TUNER_DEBUG("Input parameter(u32tunerId)invalid,invalid tunerId is: %d\n",u32tunerId);
        return HI_FAILURE;
    }

    if(HI_NULL == pstTunerAttr)
    {
        TUNER_DEBUG("Input parameter(pstTunerAttr)invalid\n");
        return HI_FAILURE;
    }

    /* Modified begin: l00185424 2011-11-26 Support satellite */
    if (HI_UNF_TUNER_SIG_TYPE_BUTT <= pstTunerAttr->enSigType)
    {
        TUNER_DEBUG("Input parameter(pstTunerAttr)invalid, sigType unsupported:%d\n",pstTunerAttr->enSigType);
        return HI_FAILURE;
    }

#if 0
    /* Check satellite tuner attribute */
    if (HI_UNF_TUNER_SIG_TYPE_SAT == pstTunerAttr->enSigType)
    {
        if (HI_UNF_TUNER_RFAGC_BUTT <= pstTunerAttr->unTunerAttr.stSat.enRFAGC)
        {
            API_DEBUG("Input parameter(enRFAGC)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_TUNER_IQSPECTRUM_BUTT <= pstTunerAttr->unTunerAttr.stSat.enIQSpectrum)
        {
            API_DEBUG("Input parameter(enIQSpectrum)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_TUNER_TSCLK_POLAR_BUTT <= pstTunerAttr->unTunerAttr.stSat.enTSClkPolar)
        {
            API_DEBUG("Input parameter(enTSClkPolar)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_TUNER_TS_FORMAT_BUTT <= pstTunerAttr->unTunerAttr.stSat.enTSFormat)
        {
            API_DEBUG("Input parameter(enTSFormat)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_TUNER_TS_SERIAL_PIN_BUTT <= pstTunerAttr->unTunerAttr.stSat.enTSSerialPIN)
        {
            API_DEBUG("Input parameter(enTSSerialPIN)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_TUNER_DISEQCWAVE_BUTT <= pstTunerAttr->unTunerAttr.stSat.enDiSEqCWave)
        {
            API_DEBUG("Input parameter(enDiSEqCWave)invalid\n");
            return HI_FAILURE;
        }
        if (HI_UNF_LNBCTRL_DEV_TYPE_BUTT <= pstTunerAttr->unTunerAttr.stSat.enLNBCtrlDev)
        {
            API_DEBUG("Input parameter(enLNBCtrlDev)invalid\n");
            return HI_FAILURE;
        }
    }
#endif

    /* Modified end: l00185424 2011-11-26 Support satellite */
    if(16 <= pstTunerAttr->enI2cChannel)
    {
        TUNER_DEBUG("Input parameter(pstTunerAttr->enI2cChannell) invalid:%d\n", pstTunerAttr->enI2cChannel);
        return HI_FAILURE;
    }

    if(HI_UNF_TUNER_OUTPUT_MODE_BUTT <= pstTunerAttr->enOutputMode)
    {
        TUNER_DEBUG("Input parameter(pstTunerAttr->enOutputMode) invalid:%d\n", pstTunerAttr->enOutputMode);
        return HI_FAILURE;
    }

#if 0
    stTunerData.u32Data = (HI_U32)(pstTunerAttr->enI2cChannel);
    stTunerData.u32Port = u32tunerId;
    s32Result = ioctl(s_s32TunerFd, TUNER_SELECT_I2C_CMD, (HI_U32)&stTunerData);

    if (HI_SUCCESS != s32Result)
    {
        API_DEBUG("Tuner TUNER_SELECT_I2C_CMD error\n");
        return HI_ERR_TUNER_FAILED_SELECTI2CCHANNEL;
    }
#endif
    g_stTunerOps[u32tunerId].enI2cChannel = pstTunerAttr->enI2cChannel;

    //stDataBuf.u32Port = u32tunerId;
    stTuner.enTunerDevType = pstTunerAttr->enTunerDevType;
    stTuner.u32TunerAddr= pstTunerAttr->u32TunerAddr;
    stDemod.enDemodDevType = pstTunerAttr->enDemodDevType;
    stDemod.u32DemodAddr = pstTunerAttr->u32DemodAddr;
    //stDataBuf.u32DataBuf[0] = (HI_U32)&stDemod;
    //stDataBuf.u32DataBuf[1] = (HI_U32)&stTuner;

    /* Added begin: l00185424, for outer demod configure, 2012-01-19 */
    //stDataBuf.u32DataBuf[2] = (HI_U32)&(pstTunerAttr->unTunerAttr);
    /* Added end: l00185424, for outer demod configure, 2012-01-19 */
    //s32Result = ioctl(s_s32TunerFd, TUNER_SELECT_TYPE_CMD, (HI_U32)&stDataBuf);

    s32Result = tuner_select_type(u32tunerId, &stTuner, &stDemod, pstTunerAttr->u32ResetGpioNo);

    if (HI_SUCCESS != s32Result)
    {
        TUNER_DEBUG("Tuner HI_TUNER_SelectTuner error\n");
        return s32Result;
    }

#if 0
    stTunerData.u32Data = pstTunerAttr->enOutputMode ;
    stTunerData.u32Port = u32tunerId;

    s32Result = ioctl(s_s32TunerFd, TUNER_SET_TSTYPE_CMD, (HI_U32)&stTunerData);

    if (HI_SUCCESS != s32Result)
    {
        API_DEBUG("Tuner HI_UNF_TUNER_SetTsType error\n");
        return HI_FAILURE;
    }
#endif

    memcpy(&s_strCurTunerAttr[u32tunerId], pstTunerAttr, sizeof(HI_UNF_TUNER_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_SetSatAttr(HI_U32   u32tunerId , const HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
    HI_S32 s32Ret;
    //TUNER_DATA_S stTunerData;

    if(UNF_TUNER_NUM <= u32tunerId)
    {
        HI_ERR_TUNER("Input parameter(u32tunerId)invalid,invalid tunerId is: %d\n",u32tunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if(HI_NULL == pstSatTunerAttr)
    {
        HI_ERR_TUNER("Input parameter(pstTunerAttr)invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_SIG_TYPE_SAT != s_strCurTunerAttr[u32tunerId].enSigType)
    {
        HI_ERR_TUNER("Current sig type is not satellite!\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_RFAGC_BUTT <= pstSatTunerAttr->enRFAGC)
    {
        HI_ERR_TUNER("Input parameter(enRFAGC)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_TUNER_IQSPECTRUM_BUTT <= pstSatTunerAttr->enIQSpectrum)
    {
        HI_ERR_TUNER("Input parameter(enIQSpectrum)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_TUNER_TSCLK_POLAR_BUTT <= pstSatTunerAttr->enTSClkPolar)
    {
        HI_ERR_TUNER("Input parameter(enTSClkPolar)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_TUNER_TS_FORMAT_BUTT <= pstSatTunerAttr->enTSFormat)
    {
        HI_ERR_TUNER("Input parameter(enTSFormat)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_TUNER_TS_SERIAL_PIN_BUTT <= pstSatTunerAttr->enTSSerialPIN)
    {
        HI_ERR_TUNER("Input parameter(enTSSerialPIN)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_TUNER_DISEQCWAVE_BUTT <= pstSatTunerAttr->enDiSEqCWave)
    {
        HI_ERR_TUNER("Input parameter(enDiSEqCWave)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }
    if (HI_UNF_LNBCTRL_DEV_TYPE_BUTT <= pstSatTunerAttr->enLNBCtrlDev)
    {
        HI_ERR_TUNER("Input parameter(enLNBCtrlDev)invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    //stTunerData.u32Port = u32tunerId;
    //stTunerData.u32Data = (HI_U32)pstSatTunerAttr;
    //s32Ret = ioctl(s_s32TunerFd, TUNER_SETSATATTR_CMD, (HI_U32)&stTunerData);
    s32Ret = tuner_set_sat_attr(u32tunerId, (HI_UNF_TUNER_SAT_ATTR_S *)pstSatTunerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("Tuner TUNER_SETSATATTR_CMD error\n");
        return HI_ERR_TUNER_FAILED_SETSATATTR;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_SetLNBConfig( HI_U32 u32TunerId, const HI_UNF_TUNER_FE_LNB_CONFIG_S *pstLNB)
{
    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid,invalid tunerId is: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstLNB)
    {
        HI_ERR_TUNER("Input parameter(pstLNB) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_FE_LNB_TYPE_BUTT <= pstLNB->enLNBType)
    {
        HI_ERR_TUNER("Input parameter(pstLNB->enLNBType) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_FE_LNB_BAND_BUTT <= pstLNB->enLNBBand)
    {
        HI_ERR_TUNER("Input parameter(pstLNB->enLNBBand) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY == pstLNB->enLNBType)
    {
        if (pstLNB->u32LowLO != pstLNB->u32HighLO)
        {
            HI_ERR_TUNER("Input parameter invalid\n");
            return HI_ERR_TUNER_INVALID_PARA;
        }
    }

    if (pstLNB->u32LowLO > pstLNB->u32HighLO)
    {
        HI_ERR_TUNER("Input parameter invalid, lowLO is bigger than highLO\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_FE_LNB_BAND_C == pstLNB->enLNBBand)
    {
        if ((pstLNB->u32LowLO > 7500) || (pstLNB->u32HighLO > 7500))
        {
            HI_ERR_TUNER("Invalid LO freq\n");
            return HI_ERR_TUNER_INVALID_PARA;
        }
    }
    else
    {
        if ((pstLNB->u32LowLO <= 7500) || (pstLNB->u32HighLO <= 7500))
        {
            HI_ERR_TUNER("Invalid LO freq\n");
            return HI_ERR_TUNER_INVALID_PARA;
        }
    }

    s_stSatPara[u32TunerId].stLNBConfig = *pstLNB;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_SetLNBPower(HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_POWER_E enLNBPower)
{
    TUNER_LNB_OUT_S stLNBOut;
    HI_S32 s32Ret;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid,invalid tunerId is: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_FE_LNB_POWER_BUTT <= enLNBPower)
    {
        HI_ERR_TUNER("Input parameter(pstLNB) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    stLNBOut.u32Port = u32TunerId;

    switch (enLNBPower)
    {
    case HI_UNF_TUNER_FE_LNB_POWER_ON:
        stLNBOut.enOut = TUNER_LNB_OUT_18V;
        break;

    case HI_UNF_TUNER_FE_LNB_POWER_ENHANCED:
        stLNBOut.enOut = TUNER_LNB_OUT_19V;
        break;

    case HI_UNF_TUNER_FE_LNB_POWER_OFF:
        stLNBOut.enOut = TUNER_LNB_OUT_0V;
        break;

    default:
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* if tuner blind scan operation is not setted, return fault */
    if (!g_stTunerOps[u32TunerId].tuner_set_lnb_out)
    {
        HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
        return HI_FAILURE;
    }

    s32Ret = g_stTunerOps[u32TunerId].tuner_set_lnb_out(u32TunerId, stLNBOut.enOut);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_TUNER( "tuner_set_lnb_out failed!\n");
        return s32Ret;
    }

    s_stSatPara[u32TunerId].enLNBPower = enLNBPower;
    return HI_SUCCESS;
}

HI_S32 HI_UNF_TUNER_Switch22K(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_22K_E enPort)
{
    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid,invalid tunerId is: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_UNF_TUNER_SWITCH_22K_BUTT <= enPort)
    {
        HI_ERR_TUNER("Input parameter(enPort) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Save */
    s_stSatPara[u32TunerId].enSwitch22K = enPort;

    if (HI_UNF_TUNER_SWITCH_22K_0 == enPort)
    {
        return TUNER_DISEQC_Send22K(u32TunerId, HI_FALSE);
    }
    else if (HI_UNF_TUNER_SWITCH_22K_22 == enPort)
    {
        return TUNER_DISEQC_Send22K(u32TunerId, HI_TRUE);
    }
    else
    {
        return HI_SUCCESS;
    }
}

#ifdef HI_BOOT_DISEQC_SUPPORT
HI_S32 TUNER_DISEQC_SendRecvMessage(HI_U32 u32TunerId,
                                           const HI_UNF_TUNER_DISEQC_SENDMSG_S * pstSendMsg,
                                           HI_UNF_TUNER_DISEQC_RECVMSG_S * pstRecvMsg)
{
    TUNER_DISEQC_SENDMSG_S stSend;
    TUNER_DISEQC_RECVMSG_S stRecv;
    TUNER_DATA_S stTunerData = {0};
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32RepeatTimes;
    HI_U32 u32RepeatTime = 0;
    HI_BOOL bSendTone = HI_FALSE;

    if (UNF_TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId) invalid,invalid tunerId is: %d\n", u32TunerId);
        return HI_ERR_TUNER_INVALID_PORT;
    }

    if (HI_NULL == pstSendMsg)
    {
        HI_ERR_TUNER("Input parameter(pstSendMsg) invalid\n");
        return HI_ERR_TUNER_INVALID_POINT;
    }

    if (HI_UNF_TUNER_DISEQC_LEVEL_BUTT <= pstSendMsg->enLevel)
    {
        HI_ERR_TUNER("Input parameter(enLevel) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_TUNER_SWITCH_TONEBURST_BUTT <= pstSendMsg->enToneBurst)
    {
        HI_ERR_TUNER("Input parameter(enToneBurst) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_DISEQC_MSG_MAX_LENGTH < pstSendMsg->u8Length)
    {
        HI_ERR_TUNER("Input parameter(u8Length) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    if (HI_UNF_DISEQC_MAX_REPEAT_TIMES < pstSendMsg->u8RepeatTimes)
    {
        HI_ERR_TUNER("Input parameter(u8RepeatTimes) invalid\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* pstRecvMsg can be NULL */

    /* Handle tone burst */
    stTunerData.u32Port = u32TunerId;
    switch (pstSendMsg->enToneBurst)
    {
    case HI_UNF_TUNER_SWITCH_TONEBURST_NONE:
        bSendTone = HI_FALSE;
        break;

    case HI_UNF_TUNER_SWITCH_TONEBURST_0:
        bSendTone = HI_TRUE;
        stTunerData.u32Data = 0;
        break;

    case HI_UNF_TUNER_SWITCH_TONEBURST_1:
        bSendTone = HI_TRUE;
        stTunerData.u32Data = 1;
        break;
    default:
        HI_ERR_TUNER("Input parameter invalid!\n");
        return HI_ERR_TUNER_INVALID_PARA;
    }

    /* Stop continuous 22K */
    (HI_VOID)TUNER_DISEQC_Stop22K(u32TunerId);
    usleep(DISEQC_DELAY_TIME_MS * 1000);

    /* Send command */
    stSend.u32Port = u32TunerId;
    stSend.stSendMsg = *pstSendMsg;
    u32RepeatTimes = pstSendMsg->u8RepeatTimes;
    //while (/*u32RepeatTimes >= 0*/1)
    for(;;)
    {
        /* Handle repeat */
        if (u32RepeatTime == 1)
        {
            stSend.stSendMsg.au8Msg[0] += 1;
        }

        /* Send command */
        /* if tuner unsupport DiSEqC, return fault */
        if (!g_stTunerOps[u32TunerId].tuner_DiSEqC_send_msg) /* check if tuner get freq and symb offset is supported */
        {
            HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
            return HI_FAILURE;
        }

        s32Ret = g_stTunerOps[u32TunerId].tuner_DiSEqC_send_msg(u32TunerId, &(stSend.stSendMsg));
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("Send DiSEqC message fail.\n");
            return HI_ERR_TUNER_FAILED_DISEQC;
        }

        /* After send command, delay 15ms */
        usleep(DISEQC_DELAY_TIME_MS * 1000);

        /* Send tone */
        if (bSendTone)
        {
            /* if tuner blind scan operation is not setted, return fault */
            if (!g_stTunerOps[u32TunerId].tuner_send_tone)
            {
                HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
                return HI_FAILURE;
            }

            s32Ret = g_stTunerOps[u32TunerId].tuner_send_tone(u32TunerId, stTunerData.u32Data);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("Send tone fail.\n");
                return HI_ERR_TUNER_FAILED_DISEQC;
            }

            /* After send tone, delay 15ms */
            usleep(DISEQC_DELAY_TIME_MS * 1000);
        }

        if (u32RepeatTimes == 0)
        {
            break;
        }

        u32RepeatTimes--;
        u32RepeatTime++;
    }

    /* Recv msessage */
    stRecv.u32Port = u32TunerId;
    stRecv.pstRecvMsg = pstRecvMsg;
    if (HI_NULL != pstRecvMsg)
    {
        if (HI_UNF_TUNER_DISEQC_LEVEL_2_X == pstSendMsg->enLevel)
        {
            /* if tuner unsupport DiSEqC, return fault */
            if (!g_stTunerOps[u32TunerId].tuner_DiSEqC_recv_msg) /* check if tuner get freq and symb offset is supported */
            {
                HI_INFO_TUNER( "tuner service function ptr is NULL!\n");
                return HI_FAILURE;
            }

            s32Ret = g_stTunerOps[u32TunerId].tuner_DiSEqC_recv_msg(u32TunerId, stRecv.pstRecvMsg);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("Recv DiSEqC message fail.\n");
                return HI_ERR_TUNER_FAILED_DISEQC;
            }

            usleep(DISEQC_DELAY_TIME_MS * 1000);
        }
        else
        {
            stRecv.pstRecvMsg->enStatus = HI_UNF_TUNER_DISEQC_RECV_UNSUPPORT;
            stRecv.pstRecvMsg->u8Length = 0;
        }
    }

    (HI_VOID)TUNER_DISEQC_Resume22K(u32TunerId);

    return HI_SUCCESS;
}
#endif

HI_S32 HI_UNF_TUNER_SetTSOUT(HI_U32 u32TunerId, HI_UNF_TUNER_TSOUT_SET_S *pstTSOUT)
{
    HI_S32 s32Ret = 0;

    if(TUNER_NUM <= u32TunerId)
    {
        HI_ERR_TUNER("Input parameter(u32TunerId)invalid,invalid tunerId is: %d\n", u32TunerId);
        return HI_FAILURE;
    }

    if(HI_NULL == pstTSOUT)
    {
        HI_ERR_TUNER("Input parameter(pstTSOUT)invalid\n");
        return HI_FAILURE;
    }

    /* if tuner get status operation is not setted, return fault */
    if (!g_stTunerOps[u32TunerId].tuner_set_ts_out) /* check if tuner_get_current_plp_type is supported */
    {
        HI_ERR_TUNER( "tuner service function ptr is NULL!\n");
        return HI_FAILURE;
    }

    /* tuner_set_ts_out */
    s32Ret = g_stTunerOps[u32TunerId].tuner_set_ts_out(u32TunerId, pstTSOUT);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "\n tuner_get_status failed \n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

/*
 * config to decide if i2c can be switch to control tuner
 */
HI_VOID qam_config_i2c_out(HI_U32 u32TunerPort, HI_BOOL bTuner)
{
    if (bTuner)
    {
        (HI_VOID)qam_write_bit(u32TunerPort, MCTRL_7_ADDR, 7, 1); /*set I2C_repeater_en 1*/
    }
}

HI_S32 tuner_i2c_receive_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct)
{
    HI_U32  u32I2cNo = 0;
    HI_U32  u32DevAddr = 0;
    HI_U32  u32RegAddr = 0;
    HI_U32  u32RegNum = 0;
    HI_U32  u32DataLen = 0;
    HI_U8   *pu8Data = NULL;
    HI_S32  s32Ret = HI_SUCCESS;

    u32I2cNo = u32I2cChannel;
    if (HI_I2C_MAX_NUM_USER < u32I2cNo)
    {
        return HI_FAILURE;
    }

    u32DevAddr = u8DevAddress;
    u32RegAddr = *(pstDataStruct->pu8SendBuf);
    u32RegNum = pstDataStruct->u32SendLength;

    u32DataLen = pstDataStruct->u32ReceiveLength;
    pu8Data = pstDataStruct->pu8ReceiveBuf;

    if (6 >= u32I2cNo)
    {
        s32Ret = HI_DRV_I2C_Read(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        //s32Ret = I2C_Read(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }
    else
    {
        /*s32Ret = gpio_i2c_read_ext(u32I2cNo, (HI_U8)u32DevAddr, (HI_U8)u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        if(HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }*/
    }

    return HI_SUCCESS;
}

HI_S32 tuner_i2c_receive_data_si(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct)
{
    HI_U32  u32I2cNo = 0;
    HI_U32  u32DevAddr = 0;
    HI_U32  u32RegAddr = 0;
    HI_U32  u32RegNum = 0;
    HI_U32  u32DataLen = 0;
    HI_U8   *pu8Data = NULL;
    HI_S32  s32Ret = HI_SUCCESS;

    u32I2cNo = u32I2cChannel;
    if (HI_I2C_MAX_NUM_USER < u32I2cNo)
    {
        return HI_FAILURE;
    }

    u32DevAddr = u8DevAddress;
    u32RegAddr = *(pstDataStruct->pu8SendBuf);
    u32RegNum = pstDataStruct->u32SendLength;

    u32DataLen = pstDataStruct->u32ReceiveLength;
    pu8Data = pstDataStruct->pu8ReceiveBuf;

    if (6 >= u32I2cNo)
    {
        s32Ret = HI_DRV_I2C_Read_si(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        //s32Ret = I2C_Read(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        if (HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }
    else
    {
        /*s32Ret = gpio_i2c_read_ext(u32I2cNo, (HI_U8)u32DevAddr, (HI_U8)u32RegAddr, u32RegNum, pu8Data, u32DataLen);
        if(HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }*/
    }


    return HI_SUCCESS;
}


HI_S32 qam_read_byte(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 *pu8RegVal)
{
    TUNER_I2C_DATA_S stI2cdataStr = {0};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8Data = 0;

    if (HI_NULL == pu8RegVal)
    {
        TUNER_DEBUG( "pointer(pu8RegVal) is null\n");
        return -1;
    }

    if (u32TunerPort >= TUNER_NUM)
    {
        TUNER_DEBUG("invalid TunerPort: %d\n", u32TunerPort);
        return -1;
    }

    stI2cdataStr.pu8ReceiveBuf = &u8Data;
    stI2cdataStr.u32ReceiveLength = 1;
    stI2cdataStr.pu8SendBuf = &u8RegAddr;
    stI2cdataStr.u32SendLength = 1;

    if (HI_UNF_DEMOD_DEV_TYPE_3136I == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        //printk("%s, %d, HI3136I_I2C_CHAN = %d, %x\n", __func__, __LINE__, HI3136I_I2C_CHAN, g_stTunerOps[u32TunerPort].u32DemodAddress);
        s32Ret = tuner_i2c_receive_data(HI3136I_I2C_CHAN,
            g_stTunerOps[u32TunerPort].u32DemodAddress, &stI2cdataStr);
    }
    else
    {
        s32Ret = tuner_i2c_receive_data(g_stTunerOps[u32TunerPort].enI2cChannel,
            g_stTunerOps[u32TunerPort].u32DemodAddress, &stI2cdataStr);
    }
    if (HI_SUCCESS == s32Ret)
    {
        *pu8RegVal = u8Data;
    }

    return s32Ret;
}

HI_S32 qam_read_bit(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8BitNum, HI_U8 *pu8BitVal)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8  u8Tmp = 0;

    if (HI_NULL == pu8BitVal)
    {
        TUNER_DEBUG( "pointer(pu8BitVal) is null\n");
        return -1;
    }

    s32Ret = qam_read_byte(u32TunerPort, u8RegAddr, &u8Tmp);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if ((1 << u8BitNum) == (u8Tmp & (1 << u8BitNum)))
    {
        *pu8BitVal = 1;
    }
    else
    {
        *pu8BitVal = 0;
    }

    return HI_SUCCESS;
}


HI_S32 tuner_i2c_send_data(HI_U32 u32I2cChannel, HI_U8 u8DevAddress, PTR_TUNER_I2C_DATA_S pstDataStruct)
{
    HI_U32  u32I2cNo = 0;
    HI_U32  u32DevAddr = 0;
    HI_U32  u32RegAddr = 0;
    HI_U32  u32DataLen = 0;
    HI_U32  u32RegNum = 0;
    HI_U8   *pData = NULL;
    HI_S32  s32Ret = HI_SUCCESS;

    u32I2cNo = u32I2cChannel;
    if (HI_I2C_MAX_NUM_USER < u32I2cNo)
    {
        return HI_FAILURE;
    }

    u32DevAddr = u8DevAddress;
    u32RegAddr = *(pstDataStruct->pu8SendBuf);
    u32RegNum  = 1;

    pData      = pstDataStruct->pu8SendBuf + 1;
    u32DataLen = pstDataStruct->u32SendLength - 1;

    if (6 >= u32I2cNo)
    {
        s32Ret = HI_DRV_I2C_Write(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pData, u32DataLen);
        //s32Ret = I2C_Write(u32I2cNo, (HI_U8)u32DevAddr, u32RegAddr, u32RegNum, pData, u32DataLen);
        if(HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }
    }
    else
    {
        /*s32Ret = gpio_i2c_write_ext(u32I2cNo, (HI_U8)u32DevAddr, (HI_U8)u32RegAddr, u32RegNum, pData, u32DataLen);
        if(HI_SUCCESS != s32Ret)
        {
            return s32Ret;
        }  */
    }

    return HI_SUCCESS;
}

HI_S32 qam_write_byte(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8RegVal)
{

    TUNER_I2C_DATA_S stI2cDataStr = {0};
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 au8SendData[2] = {0};

    au8SendData[0] = u8RegAddr;
    au8SendData[1] = u8RegVal;

    stI2cDataStr.pu8ReceiveBuf = HI_NULL;
    stI2cDataStr.u32ReceiveLength = 0;
    stI2cDataStr.pu8SendBuf = au8SendData;
    stI2cDataStr.u32SendLength = 2;
    if(u32TunerPort >= TUNER_NUM)
    {
      return HI_FAILURE;
    }
    if (HI_UNF_DEMOD_DEV_TYPE_3136I == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        //printk("%s, %d, HI3136I_I2C_CHAN = %d, %x\n", __func__, __LINE__, HI3136I_I2C_CHAN, g_stTunerOps[u32TunerPort].u32DemodAddress);
        s32Ret = tuner_i2c_send_data(HI3136I_I2C_CHAN,
            g_stTunerOps[u32TunerPort].u32DemodAddress, &stI2cDataStr);
    }
    else
    {
        s32Ret = tuner_i2c_send_data(g_stTunerOps[u32TunerPort].enI2cChannel,
            g_stTunerOps[u32TunerPort].u32DemodAddress, &stI2cDataStr);
    }

    return s32Ret;
}

HI_S32 qam_write_bit(HI_U32 u32TunerPort, HI_U8 u8RegAddr, HI_U8 u8BitNum, HI_U8 u8BitVal)
{
    HI_U8  u8Data  = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    if (u8BitVal > 1)
    {
        TUNER_DEBUG( "invalid parameter(u8BitVal>1):%d\n", u8BitVal);
        return HI_FAILURE;
    }

    if (u8BitNum > 7)
    {
        TUNER_DEBUG( "invalid parameter(u8BitNum>7):%d\n", u8BitNum);
        return HI_FAILURE;
    }

    s32Ret = qam_read_byte(u32TunerPort, u8RegAddr, &u8Data);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (u8BitVal)
    {
        u8Data |= (1 << u8BitNum);
    }
    else
    {
        u8Data &= ~(1 << u8BitNum);
    }

    s32Ret = qam_write_byte(u32TunerPort, u8RegAddr, u8Data);

    return s32Ret;
}

/* delay x mseconds */
HI_VOID HI_TIMER_mdelay(HI_U32 msec)
{
    HI_U32 cyc = 0;

    for (cyc = 0; cyc < msec; cyc++)
    {
        udelay(1000);
    }
}

HI_S32 tuner_chip_reset(HI_U32 u32ResetGpioNo)
{
    HI_U32 u32Ret = HI_FAILURE;
    u32Ret  = HI_DRV_GPIO_SetDirBit(u32ResetGpioNo, HI_FALSE);
    u32Ret |= HI_DRV_GPIO_WriteBit(u32ResetGpioNo, 0);
    msleep(100);
    u32Ret |= HI_DRV_GPIO_WriteBit(u32ResetGpioNo, 1);
    if (HI_SUCCESS != u32Ret)
    {
        HI_INFO_TUNER("Demod reset failed, ret %#x !\n", u32Ret);
        return u32Ret;
    }

    msleep(50);
    return HI_SUCCESS;
}

