/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hiDVBT.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/09/12
  Description   : 
  History       :
  1.Date        : 2014/09/12
    Author      : w00203631
    Modification: Created file
******************************************************************************/
#ifndef __HI_DTV100_PLL_ADC_H__
#define __HI_DTV100_PLL_ADC_H__

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
#include "hi_reg_common.h"


#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    reserved             : 6   ; /* [0:5]  */
        unsigned  char    mode                 : 2   ; /* [7..6] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_HiDTMB_MOD_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    reserved             : 4   ; /* [0:3]  */
        unsigned  char    fdemo_h              : 2   ; /* [5..4] */
        unsigned  char    reserved1            : 2   ; /* [7..6] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMC_FDEMO_H;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    fadc_h               : 2   ; /* [1..0] */
        unsigned  char    reserved             : 6   ; /* [7..2] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMC_FADC_H;



typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    reserved             : 1   ; /* [0]  */
        unsigned  char    c_agc_inverse        : 1   ; /* [1]  */
        unsigned  char    reserved1            : 1   ; /* [2]  */
        unsigned  char    c_iq_swap            : 1   ; /* [3]  */
        unsigned  char    c_adc_twos           : 1   ; /* [4]  */
        unsigned  char    reserved2            : 3   ; /* [7..5]  */
       
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_ADC_AGC_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    my_sel               : 4   ; /* [0:3]  */
        unsigned  char    reserved             : 4   ; /* [7..4] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_HiDTMB_MFSEL_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    reserved1            : 4   ; /* [3..0] */
        unsigned  char    bw                   : 4   ; /* [7..4] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_HiDTMB_BW_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    reserved             : 2   ; /* [1..0] */
        unsigned  char    modu_mode            : 4   ; /* [5..2] */
        unsigned  char    reserved1            : 2   ; /* [7..6] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMC_MODU_MODE;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    cool_reset           : 1   ; /* [0] */
        unsigned  char    hot_reset            : 1   ; /* [1] */
        unsigned  char    reserved1            : 6   ; /* [7..2] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMCT_RESET;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    cool_reset           : 1   ; /* [0] */
        unsigned  char    hot_reset            : 1   ; /* [1] */
        unsigned  char    reserved1            : 6   ; /* [7..2] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_DTMB_RESET;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    pll_thres            : 6   ; /* [5..0] */
        unsigned  char    reserved            	: 1   ; /* [6] */
        unsigned  char    sf_rmv            	: 1   ; /* [7] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMC_SF_RMV_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned  char    open_cr            	: 1   ; /* [0] */
        unsigned  char    open_tr            	: 1   ; /* [1] */
        unsigned  char    scan_fs            	: 1   ; /* [2] */
		unsigned  char    reserved            	: 1   ; /* [3] */
		unsigned  char    cnt_thrsh            : 4   ; /* [7:4] */
    } bits;

    /* Define an unsigned member */
    unsigned  char  u8;

} U_QAMC_TR_CR_CTRL;

HI_S32 HiDTV100_HAL_AD_CFG(HI_BOOL bEnable);
HI_S32 HiDTV100_HAL_CLK_ENABLE(HI_BOOL bEnable);
HI_S32 HiDTV100_HAL_AD_RESET(HI_VOID);
HI_S32 HiDTV100_HAL_QAMC_RESET(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */
#endif

