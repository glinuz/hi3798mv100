#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "drv_demod.h"
    

#include "hal_hiFoneCab100.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

#define DVBC_CHECK_FUNC_RET(s32Ret, pfn) \
    do {\
        s32Ret = pfn;\
        if (HI_SUCCESS != s32Ret)\
        {\
            HI_ERR_TUNER(#pfn" Failure!\n");\
            return HI_FAILURE;\
        }\
    } while(0)

//qamadc perictrl
//ADC start
/*
1.Ramping up the ADC power supplies
2.Set the opm to be 00 after having the stable ADC supply 
3.Assert the I2C reset (i2c_resetz) as low for at least 4ns (the pulse width can be extended if 4ns is too short in your design), then set to high 
4.Then, set opm as 11 for normal operation 
5.After the opm becomes 11, the foreground calibration cycle will start and this cycle will be ended when the signal “adcrdy” is asserted.
*/
HI_S32 HiDTV100_HAL_AD_CFG(HI_BOOL bEnable)
{
    volatile U_PERI_QAM_ADC0 qam_adc0_peri_cfg;
    //volatile U_PERI_QAM_ADC1 qam_adc1_peri_cfg;
    //volatile U_PERI_AGC_SEL  agc_sel_peri_cfg;

    qam_adc0_peri_cfg.u32 = g_pstRegPeri->PERI_QAM_ADC0.u32;
	qam_adc0_peri_cfg.bits.qamadc_clk_sel      = 0x0; 
	qam_adc0_peri_cfg.bits.envcmout = 0x1;
	qam_adc0_peri_cfg.bits.enaccoupling = 0x1;
	qam_adc0_peri_cfg.bits.selof        = 0x1;//0:二进制1:二进制补码 
    //qam_adc1_peri_cfg.u32 = g_pstRegPeri->PERI_QAM_ADC1.u32;
    if(HI_TRUE == bEnable)
    {		
        g_pstRegPeri->PERI_QAM_ADC0.u32 = qam_adc0_peri_cfg.u32;

        tuner_mdelay(1);

        qam_adc0_peri_cfg.bits.opm        = 0x3;
		//qam_adc0_peri_cfg.bits.startcal = 0x1;
        g_pstRegPeri->PERI_QAM_ADC0.u32 = qam_adc0_peri_cfg.u32;

        tuner_mdelay(20);
        qam_adc0_peri_cfg.u32 = g_pstRegPeri->PERI_QAM_ADC0.u32;
        if(!qam_adc0_peri_cfg.bits.adcrdy)
        {
            HI_ERR_TUNER("Start QAM-ADC failed,please check!\n");
            return HI_FAILURE;
        }

        /*qam_adc0_peri_cfg.bits.selof        = 0x1;//0:二进制1:二进制补码 
        //qam_adc0_peri_cfg.bits.peri_qamadc_chsel        = 0x0;//数据I路输出
        g_pstRegPeri->PERI_QAM_ADC0.u32                 = qam_adc0_peri_cfg.u32;*/

    }
    else
    {
        qam_adc0_peri_cfg.u32 = g_pstRegPeri->PERI_QAM_ADC0.u32;
        //qam_adc0_peri_cfg.u32 = 0;
        qam_adc0_peri_cfg.bits.opm        = 0x0;
        g_pstRegPeri->PERI_QAM_ADC0.u32 = qam_adc0_peri_cfg.u32;
    }

    return HI_SUCCESS;
    
}

HI_S32 HiDTV100_HAL_CLK_ENABLE(HI_BOOL bEnable)
{
	//CRG config
    volatile U_PERI_CRG27 PERI_CRG_27;
    volatile U_PERI_CRG58 PERI_CRG_58;

	if(HI_TRUE == bEnable)
	{
		//I2C rst cken
	    PERI_CRG_27.u32 = g_pstRegCrg->PERI_CRG27.u32;
	    //i2c_crg_rst_cken.bits.i2c7_srst_req = 0x0; //tuner
	    PERI_CRG_27.bits.i2c_adc_srst_req = 0x0;
	    PERI_CRG_27.bits.i2c_qam_srst_req = 0x0;
	    //i2c_crg_rst_cken.bits.i2c7_cken = 0x1; //tuner
	    PERI_CRG_27.bits.i2c_adc_cken = 0x1;
	    PERI_CRG_27.bits.i2c_qam_cken = 0x1;
	    g_pstRegCrg->PERI_CRG27.u32 = PERI_CRG_27.u32 ;
	    HI_INFO_TUNER("-->--After I2C rst cken i2c_crg_rst_cken = %d\n",PERI_CRG_27.u32);
	}
	else
	{
		//I2C rst cken
	    PERI_CRG_27.u32 = g_pstRegCrg->PERI_CRG27.u32;
	    //i2c_crg_rst_cken.bits.i2c7_srst_req = 0x0; //tuner
	    PERI_CRG_27.bits.i2c_adc_srst_req = 0x0;
	    PERI_CRG_27.bits.i2c_qam_srst_req = 0x0;
	    //i2c_crg_rst_cken.bits.i2c7_cken = 0x1; //tuner
	    PERI_CRG_27.bits.i2c_adc_cken = 0x0;
	    PERI_CRG_27.bits.i2c_qam_cken = 0x0;
	    g_pstRegCrg->PERI_CRG27.u32 = PERI_CRG_27.u32 ;
	    HI_INFO_TUNER("-->--After I2C rst cken i2c_crg_rst_cken = %d\n",PERI_CRG_27.u32);
	}
	
    PERI_CRG_58.u32 = g_pstRegCrg->PERI_CRG58.u32;
    if(HI_TRUE == bEnable)
    {
        PERI_CRG_58.bits.qamc_srst_req  = 0x0;
        PERI_CRG_58.bits.qamc_demo_cken = 0x1;
        PERI_CRG_58.bits.qamc_adc_cken  = 0x1;
		PERI_CRG_58.bits.qamctrl_clk_sel = 0x0;
    	PERI_CRG_58.bits.qamadc_clk_pctrl = 0x0;
    }
    else
    {
        PERI_CRG_58.bits.qamc_srst_req  = 0x1;
        PERI_CRG_58.bits.qamc_demo_cken = 0x0;
        PERI_CRG_58.bits.qamc_adc_cken  = 0x0;
    }
    g_pstRegCrg->PERI_CRG58.u32 = PERI_CRG_58.u32;    
    
    HI_INFO_TUNER("-->--After soft reset PERI_CRG_165 = %d\n",PERI_CRG_58.u32);
    
    return HI_SUCCESS;
}

HI_S32 HiDTV100_HAL_AD_RESET(HI_VOID)
{
	volatile U_PERI_CRG58    qamadc_crg_cfg;

	//adc reset
	qamadc_crg_cfg.u32 = g_pstRegCrg->PERI_CRG58.u32;
	qamadc_crg_cfg.bits.qamadc_srst_req = 0x1;
	g_pstRegCrg->PERI_CRG58.u32 = qamadc_crg_cfg.u32;
	tuner_mdelay(1);
	qamadc_crg_cfg.bits.qamadc_srst_req = 0x0;
	g_pstRegCrg->PERI_CRG58.u32 = qamadc_crg_cfg.u32;

	return HI_SUCCESS;
}

HI_S32 HiDTV100_HAL_QAMC_RESET(HI_VOID)
{
	volatile U_PERI_CRG58    qamadc_crg_cfg;

	//qamc reset
	qamadc_crg_cfg.u32 = g_pstRegCrg->PERI_CRG58.u32;
	qamadc_crg_cfg.bits.qamc_srst_req = 0x1;
	g_pstRegCrg->PERI_CRG58.u32 = qamadc_crg_cfg.u32;
	tuner_mdelay(1);
	qamadc_crg_cfg.bits.qamc_srst_req = 0x0;
	g_pstRegCrg->PERI_CRG58.u32 = qamadc_crg_cfg.u32;

	return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

