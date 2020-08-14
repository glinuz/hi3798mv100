#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include <linux/proc_fs.h>
#include <linux/poll.h>

#include <mach/hardware.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>

#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include "hi_drv_dev.h"

#include "hi_type.h"
#include "hi_unf_common.h"
#include "drv_gpioi2c_ext.h"
#include "hi_module.h"
#include "hi_drv_module.h"
#include "drv_i2c_ext.h"
#include "hi_drv_i2c.h"

#include "tlv320aic31.h"
#include "tlv320aic31_def.h"
#include "drv_gpio_ext.h"
#include "hi_reg_common.h"

#define USE_GPIO_I2C

#if defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3716cv200es)
    static int i2cchn = 2;
    #define SLAC_RESET_GPIO_GNUM_S40    2
    #define SLAC_RESET_GPIO_PNUM_S40    4
#ifdef USE_GPIO_I2C
    #define SCL_GPIO_GNUM_S40   2
    #define SCL_GPIO_PNUM_S40   1
    #define SDA_GPIO_GNUM_S40   2
    #define SDA_GPIO_PNUM_S40   2
#endif
#elif defined(CHIP_TYPE_hi3796cv100_a) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100_a) || defined(CHIP_TYPE_hi3798cv100)
    static int i2cchn = 0;
    #define SLAC_RESET_GPIO_GNUM_S40    12
    #define SLAC_RESET_GPIO_PNUM_S40    7
#ifdef USE_GPIO_I2C
    #define SCL_GPIO_GNUM_S40   13
    #define SCL_GPIO_PNUM_S40   2
    #define SDA_GPIO_GNUM_S40   13
    #define SDA_GPIO_PNUM_S40   3
#endif
#elif  defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3719mv100_a)
    static int i2cchn = 1;
    #define SLAC_RESET_GPIO_GNUM_S40    3
    #define SLAC_RESET_GPIO_PNUM_S40    7
#ifdef USE_GPIO_I2C
    #define SCL_GPIO_GNUM_S40   3
    #define SCL_GPIO_PNUM_S40   4
    #define SDA_GPIO_GNUM_S40   3
    #define SDA_GPIO_PNUM_S40   5
#endif
#elif  defined(CHIP_TYPE_hi3798cv200)
    static int i2cchn = 2;
    #define SLAC_RESET_GPIO_GNUM_S40    6
    #define SLAC_RESET_GPIO_PNUM_S40    6
#ifdef USE_GPIO_I2C
    #define SCL_GPIO_GNUM_S40   4
    #define SCL_GPIO_PNUM_S40   4
    #define SDA_GPIO_GNUM_S40   4
    #define SDA_GPIO_PNUM_S40   5
#endif

#else
    #error Must choose chiptype!
#endif

#ifdef USE_GPIO_I2C
#define SDA (((SDA_GPIO_GNUM_S40) * 8) + (SDA_GPIO_PNUM_S40))
#define SCL (((SCL_GPIO_GNUM_S40) * 8) + (SCL_GPIO_PNUM_S40))
static GPIO_I2C_EXT_FUNC_S *s_pGpioI2cExtFunc;
static HI_U32 g_pu32I2cNum;
#endif

#define CHIP_NUM 1
#define DEV_NAME "tlv320aic31"

unsigned int IIC_device_addr[CHIP_NUM] = {0x30};
static unsigned int open_cnt = 0;

#define I2C_AIC31 IIC_device_addr[0]
#define IIS 0
#define DSP 1
#define RIGHT_JUSTIFIED 2

static void time_delay_us(int usec)
{
    int i, j;

    usec = usec;

    for (i = 0; i < usec * 5; i++)
    {
        for (j = 0; j < 47; j++)
        {
            ;
        }
    }
}

#ifdef USE_GPIO_I2C
void tlv320aic31_write(unsigned char chip_addr, unsigned char reg_addr, unsigned char value)
{
    if(s_pGpioI2cExtFunc && s_pGpioI2cExtFunc->pfnGpioI2cWriteExt)
    {
         (s_pGpioI2cExtFunc->pfnGpioI2cWriteExt)(g_pu32I2cNum, chip_addr, reg_addr, 1, (HI_U8 *)&value, 1);
    }
}

int tlv320aic31_read(unsigned char chip_addr, unsigned char reg_addr)
{
    HI_U8 u8Data = 0xff;
    if(s_pGpioI2cExtFunc && s_pGpioI2cExtFunc->pfnGpioI2cReadExt)
    {
        (s_pGpioI2cExtFunc->pfnGpioI2cReadExt)(g_pu32I2cNum, chip_addr, reg_addr, 1, (HI_U8 *)&u8Data, 1);
         return u8Data;
    }
    return u8Data;

}

#else
void tlv320aic31_write(unsigned char chip_addr, unsigned char reg_addr, unsigned char value)
{
    HI_DRV_I2C_Write(i2cchn, chip_addr, reg_addr, 1, &value, 1);
}

int tlv320aic31_read(unsigned char chip_addr, unsigned char reg_addr)
{
    unsigned char data = 0xff;

    HI_DRV_I2C_Read(i2cchn, chip_addr, reg_addr, 1, &data, 1);

    return data;
}

#endif

void tlv320aic31_reg_demp(unsigned int reg_num)
{
    unsigned int i = 0;

    for (i = 0; i < reg_num; i++)
    {
        HI_INFO_AIC3X("reg%.2d =%.2x,", i, tlv320aic31_read(I2C_AIC31, i));
        if ((i + 1) % 8 == 0)
        {
            HI_INFO_AIC3X("\n");
        }
    }
}

void tlv320_DrvInit(void)
{
    //HI_ERR_AIC3X("tlv320_Init\n ");

    tlv320aic31_write(I2C_AIC31, 0x1, 0x80);
    time_delay_us(50);
#if 0
    /* Mclk =27M,use PLL mode*/
    tlv320aic31_write(I2C_AIC31, 102, 0xc2);
    tlv320aic31_write(I2C_AIC31, 3, 0x82);  //set p=2
    tlv320aic31_write(I2C_AIC31, 4, 0x1c);  //set j=7
    tlv320aic31_write(I2C_AIC31, 5, 0x2c);
    tlv320aic31_write(I2C_AIC31, 6, 0x8 );  //reg5 and reg6 set d=2818
    tlv320aic31_write(I2C_AIC31, 11, 0x1 ); //set r=1

    tlv320aic31_write(I2C_AIC31, 2, 0x00);  //FS=Fsef/6=8khz
#else
    /* Mclk = 12.288M,not PLL mode*/
    tlv320aic31_write(I2C_AIC31, 102, 0x32); //if pll,p=r=1,k=8
    tlv320aic31_write(I2C_AIC31, 101, 0x01);
    tlv320aic31_write(I2C_AIC31, 3, 0x10);

    tlv320aic31_write(I2C_AIC31, 2, 0x00);

    //tlv320aic31_write(I2C_AIC31, 10, 0x08);
#endif

    tlv320aic31_write(I2C_AIC31, 8, 0x30);

    //tlv320aic31_write(I2C_AIC31, 8,  0x20);
    tlv320aic31_write(I2C_AIC31, 9, 0x27 );  //16bits
    //tlv320aic31_write(I2C_AIC31, 15,  0xc );
    //tlv320aic31_write(I2C_AIC31, 16,  0xc );
    tlv320aic31_write(I2C_AIC31, 12, 0x50);  //0 means disabled
#ifdef BOARD_TYPE_fpga
    tlv320aic31_write(I2C_AIC31, 25, 0x0 );
#else
    tlv320aic31_write(I2C_AIC31, 25, 0x80 ); // enable mic bias
#endif
    tlv320aic31_write(I2C_AIC31, 17, 0xf );
    tlv320aic31_write(I2C_AIC31, 18, 0xf0);
#if 1
    tlv320aic31_write(I2C_AIC31, 15, 0);
    tlv320aic31_write(I2C_AIC31, 16, 0);
#ifdef BOARD_TYPE_fpga
    tlv320aic31_write(I2C_AIC31, 19, 0x7); //0x7, ena left adc
    tlv320aic31_write(I2C_AIC31, 22, 0x7); //0x7, ena right adc
#else
    tlv320aic31_write(I2C_AIC31, 19, 0x07); //left adc single_end mode, 0dB volume; ena power
    tlv320aic31_write(I2C_AIC31, 22, 0x07); //right adc single_end mode, 0dB volume; ena power
#endif
    tlv320aic31_write(I2C_AIC31, 28, 0);
    tlv320aic31_write(I2C_AIC31, 31, 0);
    tlv320aic31_write(I2C_AIC31, 26, 0x00);
    tlv320aic31_write(I2C_AIC31, 29, 0x00);
#else
    tlv320aic31_write(I2C_AIC31, 19, 0x4 ); //differential input;Input Level Control(not attenuation);adc power
    tlv320aic31_write(I2C_AIC31, 22, 0x4 );
    tlv320aic31_write(I2C_AIC31, 26, 0x95); //AGC abled;set Target Gain,Attack Time,Decay Time(medium)
    tlv320aic31_write(I2C_AIC31, 27, 0xc0); //input gain control(max (enlarge,adjust value level))
    tlv320aic31_write(I2C_AIC31, 28, 0x2 ); //Hysteresis and Clip disabled;Noise Threshold Control
    tlv320aic31_write(I2C_AIC31, 29, 0x95);
    tlv320aic31_write(I2C_AIC31, 30, 0xc0);
    tlv320aic31_write(I2C_AIC31, 31, 0x2 );
    tlv320aic31_write(I2C_AIC31, 34, 0x0 ); //debounce control
    tlv320aic31_write(I2C_AIC31, 35, 0x0 );
#endif

    //tlv320aic31_write(I2C_AIC31,27,0xfe);//input gain control(max (enlarge,adjust value level))
    //tlv320aic31_write(I2C_AIC31,28,0x0);//Hysteresis and Clip disabled;Noise Threshold Control
    //tlv320aic31_write(I2C_AIC31,30,0xfe);
    //tlv320aic31_write(I2C_AIC31,31,0x0);

#ifndef BOARD_TYPE_fpga
    tlv320aic31_write(I2C_AIC31, 7, 0xa); //set Fsref;dual rate mode or not;D1~D4 set I2S into dac,both dac open
    tlv320aic31_write(I2C_AIC31, 14, 0x80); //high-power outputs for ac-coupled
    tlv320aic31_write(I2C_AIC31, 37, 0xc0);   //left and right dac power
    tlv320aic31_write(I2C_AIC31, 40, 0x80); //Output common-mode voltage 1.65v, disable soft_step volume

    tlv320aic31_write(I2C_AIC31, 41, 0x00); //select DAC_L(R)1 to hi-power;left and right volume dependent controls

    tlv320aic31_write(I2C_AIC31, 42, 0x68); //Output Driver Pop Reduction(medium)

    tlv320aic31_write(I2C_AIC31, 43, 0x08); //left DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)
    tlv320aic31_write(I2C_AIC31, 44, 0x08); //right DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)

    tlv320aic31_write(I2C_AIC31, 47, 0x80); //DAC_L1 is routed to HPLOUT;Output Stage Volume Control(no attenuation)
    tlv320aic31_write(I2C_AIC31, 64, 0x80); //DAC_L1 is routed to HPROUT;Output Stage Volume Control(no attenuation)

    tlv320aic31_write(I2C_AIC31, 51, 0x9f); //left output level control(enlarge,max);output mute,power;
    tlv320aic31_write(I2C_AIC31, 65, 0x9f); //right output level control(enlarge,max);output mute,power;
    /* enable LEFT_LOP/M and RIGHT_LOP/M */
    tlv320aic31_write(I2C_AIC31, 81, 0x00); //0: PGA_L is not routed to LEFT_LOP/M;
    tlv320aic31_write(I2C_AIC31, 82, 0x80); //0: DAC_L1 is routed to LEFT_LOP/M
    tlv320aic31_write(I2C_AIC31, 86, 0x0b); //left LEFT_LOP level 0dB, un-mute, power up;

    tlv320aic31_write(I2C_AIC31, 91, 0x00); //0: PGA_R is not routed to RIGHT_LOP/M;
    tlv320aic31_write(I2C_AIC31, 92, 0x80); //0: DAC_R1 is routed to RIGHT_LOP/M
    tlv320aic31_write(I2C_AIC31, 93, 0x0b); //left LEFT_LOP level 0dB, un-mute, power up;
#else

    tlv320aic31_write(I2C_AIC31, 7, 0xa); //set Fsref;dual rate mode or not;D1~D4 set I2S into dac,both dac open
    tlv320aic31_write(I2C_AIC31, 14, 0x80); //high-power outputs for ac-coupled
    //     tlv320aic31_write(I2C_AIC31,37,0xF0);//left and right dac power
    tlv320aic31_write(I2C_AIC31, 37, 0xc0);   //left and right dac power

    //tlv320aic31_write(I2C_AIC31,38,0x3e);//Short Circuit Protection Control

    tlv320aic31_write(I2C_AIC31, 40, 0x80); //Output common-mode voltage 1.65v, disable soft_step volume

    //tlv320aic31_write(I2C_AIC31,41,0x1);//select DAC_L(R)2 to hi-power;left and right volume dependent controls
    tlv320aic31_write(I2C_AIC31, 41, 0xa0); //select DAC_L(R)2 to hi-power;left and right volume dependent controls

    //tlv320aic31_write(I2C_AIC31,42,0x08);//Output Driver Pop Reduction(medium)
    tlv320aic31_write(I2C_AIC31, 42, 0x68); //Output Driver Pop Reduction(medium)

    // tlv320aic31_write(I2C_AIC31,43,0x0);//left DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)
    // tlv320aic31_write(I2C_AIC31,44,0x0);//right DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)
    tlv320aic31_write(I2C_AIC31, 43, 0x08); //left DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)
    tlv320aic31_write(I2C_AIC31, 44, 0x08); //right DAC channel not muted;DAC Digital Volume Control(attenuation,adjust value level)

    tlv320aic31_write(I2C_AIC31, 47, 0x80); //DAC_L1 is routed to HPLOUT;Output Stage Volume Control(no attenuation)
    tlv320aic31_write(I2C_AIC31, 64, 0x80); //DAC_L1 is routed to HPROUT;Output Stage Volume Control(no attenuation)

    tlv320aic31_write(I2C_AIC31, 51, 0x9f); //left output level control(enlarge,max);output mute,power;
    tlv320aic31_write(I2C_AIC31, 65, 0x9f);
#endif
}

void tlv320_SoftReset(unsigned int chip_num)
{
    /*soft reset*/
    tlv320aic31_write(IIC_device_addr[chip_num],0x1,0x80);
    udelay(50);

    /*CLKDIV_IN uses MCLK*/
    tlv320aic31_write(IIC_device_addr[chip_num], 102, 0x32);

#if 0
    /*PLL disable and select Q value*/
    tlv320aic31_write(IIC_device_addr[chip_num], 3, 0x10);
#else
    /*PLL enable */
    tlv320aic31_write(IIC_device_addr[chip_num], 3, 0x82);/* P=2 */
    tlv320aic31_write(IIC_device_addr[chip_num], 4, 0x1c);/* J=2 */
    tlv320aic31_write(IIC_device_addr[chip_num], 5, 0x2c);
    tlv320aic31_write(IIC_device_addr[chip_num], 6, 0x8);/* reg 5 and 6 set D=2818*/
    tlv320aic31_write(IIC_device_addr[chip_num], 11, 0x1);/* R=1 */
#endif
    /*left and right DAC open*/
    tlv320aic31_write(IIC_device_addr[chip_num], 7,  0xa);/* FSref = 48 kHz */

    /*sample*/
    tlv320aic31_write(IIC_device_addr[chip_num], 2,  0xaa);/* FS = FSref/6 */

    /*ctrl mode*/
    tlv320aic31_write(IIC_device_addr[chip_num], 8,  0xf0);/* master mode */

    /*Audio Serial Data Interface Control*/
    tlv320aic31_write(IIC_device_addr[chip_num], 9,  0x7);/* I2S mode,16bit */

    /*Audio Codec Digital Filter Control Register*/
    tlv320aic31_write(IIC_device_addr[chip_num], 12,  0x50);

    tlv320aic31_write(IIC_device_addr[chip_num], 25,  0x0);
    tlv320aic31_write(IIC_device_addr[chip_num], 17,  0xf);
    tlv320aic31_write(IIC_device_addr[chip_num], 18,  0xf0);

    tlv320aic31_write(IIC_device_addr[chip_num], 15,  0x0);
    tlv320aic31_write(IIC_device_addr[chip_num], 16,  0x0);
    tlv320aic31_write(IIC_device_addr[chip_num], 19,  0x7);
    tlv320aic31_write(IIC_device_addr[chip_num], 22,  0x7);
    tlv320aic31_write(IIC_device_addr[chip_num], 28,  0x0);
    tlv320aic31_write(IIC_device_addr[chip_num], 31,  0x0);


    /*out ac-coupled*/
    tlv320aic31_write(IIC_device_addr[chip_num], 14, 0x80);

    /*left and right DAC power on*/
    tlv320aic31_write(IIC_device_addr[chip_num], 37, 0xc0);

    /*out common-mode voltage*/
    tlv320aic31_write(IIC_device_addr[chip_num], 40, 0x80);

    /*out path select*/
    tlv320aic31_write(IIC_device_addr[chip_num], 41, 0x1);

    /*out path select*/
    tlv320aic31_write(IIC_device_addr[chip_num], 42, 0xa8);

    /*left DAC not muted*/
    tlv320aic31_write(IIC_device_addr[chip_num], 43, 0x0);

    /*right DAC not muted*/
    tlv320aic31_write(IIC_device_addr[chip_num], 44, 0x0);

    tlv320aic31_write(IIC_device_addr[chip_num], 47, 0x80);

    /*HPLOUT is not muted*/
    tlv320aic31_write(IIC_device_addr[chip_num], 51, 0x9f);

    tlv320aic31_write(IIC_device_addr[chip_num], 64, 0x80);
    /*HPROUT is not muted*/
    tlv320aic31_write(IIC_device_addr[chip_num], 65, 0x9f);

    /*out short circuit protection*/
    tlv320aic31_write(IIC_device_addr[chip_num], 38, 0x3e);

}


//Fpga MasterMode = 1;I2s SioMode =0;
void tlv320_SetMaster(HI_U32 MasterMode)
{
    if (MasterMode == 1)
    {
        tlv320aic31_write(I2C_AIC31, 8, 0x30);
    }
    else
    {
        tlv320aic31_write(I2C_AIC31, 8, 0xf0);
    }
}

void tlv320_SetBitWidth(HI_U32 SioMode, HI_U32 BitWidth)
{
    Serial_Int_Ctrl serial_int_ctrl;

    serial_int_ctrl.b8 = tlv320aic31_read(I2C_AIC31, 9);
    if (SioMode == IIS)
    {
        serial_int_ctrl.bit.transfer_mode = 0;
        if (BitWidth == 16)
        {
            serial_int_ctrl.bit.data_length = 0;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 20)
        {
            serial_int_ctrl.bit.data_length = 1;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 24)
        {
            serial_int_ctrl.bit.data_length = 2;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 32)
        {
            serial_int_ctrl.bit.data_length = 3;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
    }
    else if (SioMode == DSP)
    {
        serial_int_ctrl.bit.transfer_mode = 1;
        if (BitWidth == 16)
        {
            serial_int_ctrl.bit.data_length = 0;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 20)
        {
            serial_int_ctrl.bit.data_length = 1;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 24)
        {
            serial_int_ctrl.bit.data_length = 2;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 32)
        {
            serial_int_ctrl.bit.data_length = 3;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
    }
    else if (SioMode == RIGHT_JUSTIFIED)
    {
        serial_int_ctrl.bit.transfer_mode = 2;
        if (BitWidth == 16)
        {
            serial_int_ctrl.bit.data_length = 0;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 20)
        {
            serial_int_ctrl.bit.data_length = 1;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 24)
        {
            serial_int_ctrl.bit.data_length = 2;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
        else if (BitWidth == 32)
        {
            serial_int_ctrl.bit.data_length = 3;
            tlv320aic31_write(I2C_AIC31, 9, serial_int_ctrl.b8);
        }
    }
}

void tlv320_SetSampleRateDrv(HI_U32 RateSample)
{
#if 1
    tlv320aic31_write(I2C_AIC31, 2, 0x00);
#else
    switch (RateSample)
    {
    case 8:
        tlv320aic31_write(I2C_AIC31, 2, 0x66);
        break;
    case 11:
        tlv320aic31_write(I2C_AIC31, 2, 0x66);
        break;
    case 12:
        tlv320aic31_write(I2C_AIC31, 2, 0x66);
        break;
    case 16:
        tlv320aic31_write(I2C_AIC31, 2, 0x22);
        break;
    case 22:
        tlv320aic31_write(I2C_AIC31, 2, 0x22);
        break;
    case 24:
        tlv320aic31_write(I2C_AIC31, 2, 0x22);
        break;
    case 32:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
        break;
    case 44:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
        break;
    case 48:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
        break;
    case 88:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
        break;
    case 96:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
    case 176:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
    case 192:
        tlv320aic31_write(I2C_AIC31, 2, 0x00);
    default:
        break;
    }
#endif

}


HI_S32 tlv320_SetBitwidth(HI_S32 CodecId, HI_UNF_BIT_DEPTH_E enBitWidth)
{
    switch (enBitWidth)
    {
    case HI_UNF_BIT_DEPTH_16:
    {
        tlv320_SetBitWidth(IIS, 16);
        break;
    }

    case HI_UNF_BIT_DEPTH_18:
    {
        tlv320_SetBitWidth(IIS, 18);
        break;
    }

    case HI_UNF_BIT_DEPTH_20:
    {
        tlv320_SetBitWidth(IIS, 20);
        break;
    }

    case HI_UNF_BIT_DEPTH_24:
    {
        tlv320_SetBitWidth(IIS, 24);
        break;
    }

    default:
        HI_ERR_AIC3X("Invalid dac bitwidth\n");
    }

    return 0;
}

HI_VOID tlv320_SetSlotOffset(HI_U32 u32SlotOffset)
{
    tlv320aic31_write(I2C_AIC31, 10, u32SlotOffset & 0xff);
}

HI_VOID tlv320_SetBclkMode(HI_U32 Bclk_Mode)
{
    HI_U32 value = tlv320aic31_read(I2C_AIC31, 9);
    switch (Bclk_Mode)
    {
        case 0:
            tlv320aic31_write(I2C_AIC31, 9, value&(~(1<<3)));
            break;
        case 1:
            tlv320aic31_write(I2C_AIC31, 9, value|(1<<3));
            break;
        default:
            HI_ERR_AIC3X("Invalid dac Bclk Mode\n");
    }
}

/* set CODEC main/slave model */
HI_S32 TLV_CODEC_SetMSMode(HI_S32 CodecId, AUD_CODEC_AO_MS_MODE_E enMsMode)
{
    switch (enMsMode)
    {
    case AUD_CODEC_AO_MODE_MASTER:
    {
        tlv320_SetMaster(1);
        break;
    }

    case AUD_CODEC_AO_MODE_SLAVE:
    {
        tlv320_SetMaster(0);
        break;
    }
    default:
    {
        HI_ERR_AIC3X("Invalid dac ms mode\n");
    }
    }

    return 0;
}

HI_S32 TLV_CODEC_SetPcmFormat(HI_S32 CodecId, HI_UNF_BIT_DEPTH_E enBitWidth, AUD_CODEC_AO_DATA_E enDataFormat,
                              HI_U32 u32SlotOffset)
{
    switch (enDataFormat)
    {
    case AUD_CODEC_AO_DATA_I2S:
    {
        //HI_ERR_AIC3X("I2S u32SlotOffset = %d\n ",u32SlotOffset);
        tlv320_SetBitWidth(IIS, (HI_U32)enBitWidth);
        tlv320_SetSlotOffset(u32SlotOffset);
        break;
    }

    case AUD_CODEC_AO_DATA_DSP:
    {
        //HI_ERR_AIC3X("DSP u32SlotOffset = %d\n ",u32SlotOffset);
        tlv320_SetBitWidth(DSP, (HI_U32)enBitWidth);
        tlv320_SetSlotOffset(u32SlotOffset);
        break;
    }
    default:
    {
        HI_ERR_AIC3X("Invalid dac ms mode\n");
    }
    }

    if (0)
    {
        unsigned char data;
        data = tlv320aic31_read(I2C_AIC31, 9);
        HI_ERR_AIC3X("I2c[%d] I2C_AIC31[9]=0x%x\n ", I2C_AIC31, data & 0xff);
        data = tlv320aic31_read(I2C_AIC31, 10);
        HI_ERR_AIC3X("I2c[%d] I2C_AIC31[10]=0x%x\n ", I2C_AIC31, data & 0xff);
    }

    return 0;
}

HI_S32 tlv320_SetDataFormat(HI_S32 CodecId, AUD_CODEC_AO_DATA_E enDataFormat)
{
    return 0;
}


HI_S32 TLV_CODEC_SetSampleRate(HI_S32 CodecId, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    switch (enSampleRate)
    {
    case HI_UNF_SAMPLE_RATE_8K:
    {
        tlv320_SetSampleRateDrv(8);
        break;
    }

    case HI_UNF_SAMPLE_RATE_11K:
    {
        tlv320_SetSampleRateDrv(11);
        break;
    }

    case HI_UNF_SAMPLE_RATE_12K:
    {
        tlv320_SetSampleRateDrv(12);
        break;
    }
    case HI_UNF_SAMPLE_RATE_16K:
    {
        tlv320_SetSampleRateDrv(16);
        break;
    }
    case HI_UNF_SAMPLE_RATE_22K:
    {
        tlv320_SetSampleRateDrv(22);
        break;
    }
    case HI_UNF_SAMPLE_RATE_24K:
    {
        tlv320_SetSampleRateDrv(24);
        break;
    }
    case HI_UNF_SAMPLE_RATE_32K:
    {
        tlv320_SetSampleRateDrv(32);
        break;
    }
    case HI_UNF_SAMPLE_RATE_44K:
    {
        tlv320_SetSampleRateDrv(44);
        break;
    }
    case HI_UNF_SAMPLE_RATE_48K:
    {
        tlv320_SetSampleRateDrv(48);
        break;
    }
    case HI_UNF_SAMPLE_RATE_88K:
    {
        tlv320_SetSampleRateDrv(88);
        break;
    }
    case HI_UNF_SAMPLE_RATE_96K:
    {
        tlv320_SetSampleRateDrv(96);
        break;
    }

    case HI_UNF_SAMPLE_RATE_192K:
    {
        tlv320_SetSampleRateDrv(192);
        break;
    }

    default:
    {
        HI_ERR_AIC3X("Invalid dac sample rate\n");
    }
    }

    return 0;
}

HI_VOID TLV_CODEC_SetBclkMode(HI_S32 CodecId, HI_S32 BclkMode)
{
    switch (BclkMode)
    {
        case CONTINUOUS_TRANSFER_MODE:
            tlv320_SetBclkMode(0);
            break;
        case CONST_256_CLOCK_TRANSFER_MODE:
            tlv320_SetBclkMode(1);
            break;
        default:
            HI_ERR_AIC3X("Invalid dac BclkModes\n");
    }
}


HI_S32 TLV_CODEC_Init(HI_S32 CodecId)
{
    tlv320_DrvInit();

    TLV_CODEC_SetMSMode(0, AUD_CODEC_AO_MODE_MASTER);
    TLV_CODEC_SetSampleRate(0, HI_UNF_SAMPLE_RATE_48K);
    TLV_CODEC_SetPcmFormat(0, HI_UNF_BIT_DEPTH_32, AUD_CODEC_AO_DATA_I2S, 0);


    if (1)
    {
        unsigned char data;
        data = tlv320aic31_read(I2C_AIC31, 9);
        HI_INFO_AIC3X("I2c[%d] I2C_AIC31[9]=0x%x\n ", I2C_AIC31, data & 0xff);
        data = tlv320aic31_read(I2C_AIC31, 10);
        HI_INFO_AIC3X("I2c[%d] I2C_AIC31[10]=0x%x\n ", I2C_AIC31, data & 0xff);
    }

    return 0;
}

static int tlv320aic31_device_init(unsigned int num)
{
    /* inite codec configs.*/
    unsigned char data;

    tlv320aic31_write(I2C_AIC31, 0x2, 0xaa);
    msleep(1);
    data = tlv320aic31_read(I2C_AIC31, 0x2);
    if (data != 0xaa)
    {
        HI_ERR_AIC3X("init aic31(%d) error, want 0xaa, get=%#x\n", num, data);
        return -1;
    }
    HI_INFO_AIC3X("test ac31\n");
    TLV_CODEC_Init(num);
    //tlv320aic31_reg_demp(102);

    return 0;
}

/*
 *  device open. set counter
 */
static int tlv320aic31_open(struct inode * inode, struct file * file)
{
    if (0 == open_cnt++)
    {
        unsigned int i;
        unsigned int chip_count = CHIP_NUM;

#ifdef USE_GPIO_I2C
        s_pGpioI2cExtFunc = HI_NULL;
        HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&s_pGpioI2cExtFunc);
#endif
        for (i = 0; i < chip_count; i++)
        {
            if (tlv320aic31_device_init(i) < 0)
            {
                return -1;
            }
        }

        HI_INFO_AIC3X("tlv320aic31 driver init successful!");

        return 0;
    }

    return -1;
}

/*
 *  Close device, Do nothing!
 */
static int tlv320aic31_close(struct inode *inode, struct file *file)
{
    open_cnt--;
    return 0;
}

static long tlv320aic31_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    HI_U32 chip_num = 0;
    Audio_Ctrl temp;
    Audio_Ctrl *audio_ctrl;
    TLV320_IfAttr_S temp1;
    TLV320_IfAttr_S *tlv320_attr;
    Codec_Datapath_Setup_Ctrl codec_datapath_setup_ctrl;
    DAC_OUTPUT_SWIT_CTRL dac_output_swit_ctrl;
    DAC_POWER_CTRL dac_power_ctrl;
    In1_Adc_Ctrl in1_adc_ctrl;
    In2_Adc_Ctrl_Sample in2_adc_ctrl_sample;
    Adc_Pga_Dac_Gain_Ctrl adc_pga_dac_gain_ctrl;
    Line_Hpcom_Out_Ctrl line_hpcom_out_ctrl;
    Serial_Int_Ctrl serial_int_ctrl;
    Ctrl_Mode ctrl_mode;

    if (argp != NULL)
    {
        if (cmd == SET_I2S_MODE )
        {
            if (copy_from_user(&temp1, argp, sizeof(TLV320_IfAttr_S)))
            {
                return -EFAULT;
            }
            tlv320_attr = (TLV320_IfAttr_S *)(&temp1);
        }
        else
        {
            if (copy_from_user(&temp, argp, sizeof(Audio_Ctrl)))
            {
                return -EFAULT;
            }
            audio_ctrl = (Audio_Ctrl *)(&temp);
            chip_num = audio_ctrl->chip_num;
        }
    }


    switch (cmd)
    {
    case IN2LR_2_LEFT_ADC_CTRL:
        in2_adc_ctrl_sample.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 17);
        in2_adc_ctrl_sample.bit.in2l_adc_input_level_sample = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 17, in2_adc_ctrl_sample.b8);
        break;
    case IN2LR_2_RIGTH_ADC_CTRL:
        in2_adc_ctrl_sample.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 18);
        in2_adc_ctrl_sample.bit.in2r_adc_input_level_sample = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 18, in2_adc_ctrl_sample.b8);

        break;
    case IN1L_2_LEFT_ADC_CTRL:
        in1_adc_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 19);
        in1_adc_ctrl.bit.in1_adc_input_level = audio_ctrl->input_level;
        in1_adc_ctrl.bit.adc_ch_power_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 19, in1_adc_ctrl.b8);
        break;
    case IN1R_2_RIGHT_ADC_CTRL:
        in1_adc_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 22);
        in1_adc_ctrl.bit.in1_adc_input_level = audio_ctrl->input_level;
        in1_adc_ctrl.bit.adc_ch_power_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 22, in1_adc_ctrl.b8);
        break;
    case PGAL_2_HPLOUT_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 46);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 46, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACL1_2_HPLOUT_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 47);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 47, adc_pga_dac_gain_ctrl.b8);
        break;
    case HPLOUT_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 51);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 51, line_hpcom_out_ctrl.b8);
        break;
    case PGAL_2_HPLCOM_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 53);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 53, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACL1_2_HPLCOM_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 54);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 54, adc_pga_dac_gain_ctrl.b8);
        break;
    case HPLCOM_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 58);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 58, line_hpcom_out_ctrl.b8);
        break;
    case PGAR_2_HPROUT_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 63);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 63, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACR1_2_HPROUT_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 64);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 64, adc_pga_dac_gain_ctrl.b8);
        break;
    case HPROUT_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 65);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 65, line_hpcom_out_ctrl.b8);
        break;
    case PGAR_2_HPRCOM_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 70);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 70, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACR1_2_HPRCOM_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 71);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 71, adc_pga_dac_gain_ctrl.b8);
        break;
    case HPRCOM_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 72);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 72, line_hpcom_out_ctrl.b8);
        break;
    case PGAL_2_LEFT_LOP_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 81);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 81, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACL1_2_LEFT_LOP_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 82);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 82, adc_pga_dac_gain_ctrl.b8);
        break;
    case LEFT_LOP_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 86);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        line_hpcom_out_ctrl.bit.power_status = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 86, line_hpcom_out_ctrl.b8);
        break;
    case PGAR_2_RIGHT_LOP_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 91);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 91, adc_pga_dac_gain_ctrl.b8);
        break;
    case DACR1_2_RIGHT_LOP_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 92);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 92, adc_pga_dac_gain_ctrl.b8);
        break;
    case RIGHT_LOP_OUTPUT_LEVEL_CTRL:
        line_hpcom_out_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 93);
        line_hpcom_out_ctrl.bit.if_mute = audio_ctrl->if_mute_route;
        line_hpcom_out_ctrl.bit.output_level = audio_ctrl->input_level;
        line_hpcom_out_ctrl.bit.power_status = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 93, line_hpcom_out_ctrl.b8);
        break;
    case SET_ADC_SAMPLE:
        in2_adc_ctrl_sample.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 2);
        in2_adc_ctrl_sample.bit.in2l_adc_input_level_sample = audio_ctrl->sample;
        tlv320aic31_write(IIC_device_addr[chip_num], 2, in2_adc_ctrl_sample.b8);
        break;
    case SET_DAC_SAMPLE:
        in2_adc_ctrl_sample.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 2);
        in2_adc_ctrl_sample.bit.in2r_adc_input_level_sample = audio_ctrl->sample;
        tlv320aic31_write(IIC_device_addr[chip_num], 2, in2_adc_ctrl_sample.b8);
        break;
    case SET_DATA_LENGTH:
        serial_int_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 9);
        ;
        serial_int_ctrl.bit.data_length = audio_ctrl->data_length;
        tlv320aic31_write(IIC_device_addr[chip_num], 9, serial_int_ctrl.b8);
        break;
    case SET_TRANSFER_MODE:
        serial_int_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 9);
        serial_int_ctrl.bit.transfer_mode = audio_ctrl->trans_mode;
        tlv320aic31_write(IIC_device_addr[chip_num], 9, serial_int_ctrl.b8);
        break;
    case SET_CTRL_MODE:
        tlv320aic31_write(IIC_device_addr[chip_num], 0x1, 0x80);
        udelay(50);
        ctrl_mode.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 8);
        ctrl_mode.bit.bit_clock_dic_ctrl  = audio_ctrl->ctrl_mode;
        ctrl_mode.bit.work_clock_dic_ctrl = audio_ctrl->ctrl_mode;
        ctrl_mode.bit.bit_work_dri_ctrl = audio_ctrl->ctrl_mode;
        tlv320aic31_write(IIC_device_addr[chip_num], 8, ctrl_mode.b8);
        break;
    case LEFT_DAC_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 43);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 43, adc_pga_dac_gain_ctrl.b8);
        break;
    case RIGHT_DAC_VOL_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 44);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 44, adc_pga_dac_gain_ctrl.b8);
        break;
    case LEFT_DAC_POWER_SETUP:
        codec_datapath_setup_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 7);
        codec_datapath_setup_ctrl.bit.left_dac_datapath_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 7, codec_datapath_setup_ctrl.b8);
        dac_power_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 37);
        dac_power_ctrl.bit.left_dac_power_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 37, dac_power_ctrl.b8);
        break;
    case RIGHT_DAC_POWER_SETUP:
        codec_datapath_setup_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 7);
        codec_datapath_setup_ctrl.bit.right_dac_datapath_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 7, codec_datapath_setup_ctrl.b8);
        dac_power_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 37);
        dac_power_ctrl.bit.right_dac_power_ctrl = audio_ctrl->if_powerup;
        tlv320aic31_write(IIC_device_addr[chip_num], 37, dac_power_ctrl.b8);
        break;
    case DAC_OUT_SWITCH_CTRL:
        dac_output_swit_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 41);
        dac_output_swit_ctrl.bit.left_dac_swi_ctrl  = audio_ctrl->dac_path;
        dac_output_swit_ctrl.bit.right_dac_swi_ctrl = audio_ctrl->dac_path;
        tlv320aic31_write(IIC_device_addr[chip_num], 41, dac_output_swit_ctrl.b8);
        break;
    case LEFT_ADC_PGA_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 15);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 15, adc_pga_dac_gain_ctrl.b8);
        break;
    case RIGHT_ADC_PGA_CTRL:
        adc_pga_dac_gain_ctrl.b8 = tlv320aic31_read(IIC_device_addr[chip_num], 16);
        adc_pga_dac_gain_ctrl.bit.if_mute_route = audio_ctrl->if_mute_route;
        adc_pga_dac_gain_ctrl.bit.input_vol_level_ctrl = audio_ctrl->input_level;
        tlv320aic31_write(IIC_device_addr[chip_num], 16, adc_pga_dac_gain_ctrl.b8);
        break;
    case SOFT_RESET_CTRL:
        tlv320_SoftReset(chip_num);
        break;
    case TLV320AIC31_REG_DUMP:
        tlv320aic31_reg_demp(102);
        break;
    case SET_I2S_MODE:
        TLV_CODEC_SetSampleRate(0,tlv320_attr->enSampleRate );
        TLV_CODEC_SetPcmFormat(0, tlv320_attr->enBitwidth, tlv320_attr->enDataFormat, tlv320_attr->u32SlotOffset);
        TLV_CODEC_SetMSMode(0, tlv320_attr->enMsMode);
        TLV_CODEC_SetBclkMode(0,tlv320_attr->is256ConstBclkMode);
#if 0
        switch (audio_ctrl->i2s_mode)
        {
        case 0:
            HI_INFO_AIC3X("config TLVAIC31 I2S Mode \n");
            TLV_CODEC_SetSampleRate(0, HI_UNF_SAMPLE_RATE_48K);
            TLV_CODEC_SetPcmFormat(0, HI_UNF_BIT_DEPTH_32, AUD_CODEC_AO_DATA_I2S, 0);
            break;

        case 1:
            HI_INFO_AIC3X("config TLVAIC31 standard pcm  Mode \n");
            TLV_CODEC_SetSampleRate(0, HI_UNF_SAMPLE_RATE_48K);
            TLV_CODEC_SetPcmFormat(0, HI_UNF_BIT_DEPTH_32, AUD_CODEC_AO_DATA_DSP, 1);
            break;
        case 2:
            HI_INFO_AIC3X("config TLVAIC31 private pcm  Mode \n");
            TLV_CODEC_SetSampleRate(0, HI_UNF_SAMPLE_RATE_48K);
            TLV_CODEC_SetPcmFormat(0, HI_UNF_BIT_DEPTH_32, AUD_CODEC_AO_DATA_DSP, 0);
            break;
        }
        if (audio_ctrl->i2s_master)
        {
            HI_INFO_AIC3X(" TLVAIC31 work at msater Mode \n");
            TLV_CODEC_SetMSMode(0, AUD_CODEC_AO_MODE_SLAVE);
        }
        else
        {
            HI_INFO_AIC3X(" TLVAIC31 work at slave Mode \n");
            TLV_CODEC_SetMSMode(0, AUD_CODEC_AO_MODE_MASTER);
        }

#endif

        break;
    default:
        break;
    }

    return 0;
}

/*
 *  The various file operations we support.
 */

static struct file_operations tlv320aic31_fops =
{
    .owner          = THIS_MODULE,
    .unlocked_ioctl = tlv320aic31_ioctl,
    .open           = tlv320aic31_open,
    .release        = tlv320aic31_close
};


//#define TLV320AIC31_PM

#ifdef TLV320AIC31_PM
static int tlv320aic31_suspend (struct himedia_basedev *pdev, pm_message_t state)
{
    int ret;
    if (open_cnt){
        HI_ERR_AIC3X("tlv320aic31_suspend err: close first \n");
        return -1;
    }
    HI_ERR_AIC3X("tlv320aic31_suspend ok \n");
    return 0;
}

static int tlv320aic31_resume  (struct himedia_basedev *pdev)
{
    HI_ERR_AIC3X("tlv320aic31_resume ok \n");
    return 0;
}

static struct himedia_baseops tlv320aic31_baseOps = {
    .probe  = NULL,
    .remove = NULL,
    .shutdown = NULL,
    .prepare  = NULL,
    .complete = NULL,
    .suspend  = tlv320aic31_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume  = tlv320aic31_resume
};
#endif


#ifdef TLV320AIC31_PM
static struct himedia_device  tlv320aic31_dev = {
    .minor = HIMEDIA_DYNAMIC_MINOR,
    .name  = DEV_NAME,
    .owner = THIS_MODULE,
    .app_ops = &tlv320aic31_fops,
    .base_ops = &tlv320aic31_baseOps
};
#else
static struct miscdevice tlv320aic31_dev =
{
    MISC_DYNAMIC_MINOR,
    DEV_NAME,
    &tlv320aic31_fops,
};
#endif

static int __INIT__ tlv320aic31_init(void)
{
    unsigned int ret;

    int i;
    unsigned int chip_count = CHIP_NUM;

#if defined (CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined (CHIP_TYPE_hi3719mv100_a)
    g_pstRegIO->ioshare_reg39.u32 = 0x0;  //set tlv320 reset pin
    #ifdef USE_GPIO_I2C
    g_pstRegIO->ioshare_reg36.u32 = 0x0;  //set I2C1_SCL pin
    g_pstRegIO->ioshare_reg37.u32 = 0x5;  //set I2C1_SDA pin
    #else
    g_pstRegIO->ioshare_reg36.u32 = 0x3;  //set I2C1_SCL pin
    g_pstRegIO->ioshare_reg37.u32 = 0x3;  //set I2C1_SDA pin
    #endif
#elif defined(CHIP_TYPE_hi3796cv100_a) || defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100_a) || defined(CHIP_TYPE_hi3798cv100) 
    g_pstRegIO->ioshare_reg120.u32 = 0x2;  //set tlv320 reset pin
    #ifdef USE_GPIO_I2C
    g_pstRegIO->ioshare_reg121.u32 = 0x2;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_reg122.u32 = 0x2;  //set I2C2_SDA pin
    #else
    g_pstRegIO->ioshare_reg121.u32 = 0x0;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_reg122.u32 = 0x3;  //set I2C2_SDA pin
    #endif
#elif defined(CHIP_TYPE_hi3798cv200)
    g_pstRegIO->ioshare_44.u32 = 0x0;  //set tlv320 reset pin
#ifdef USE_GPIO_I2C
    g_pstRegIO->ioshare_34.u32 = 0x0;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_35.u32 = 0x0;  //set I2C2_SDA pin
#else
    g_pstRegIO->ioshare_34.u32 = 0x2;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_35.u32 = 0x2;  //set I2C2_SDA pin
#endif
#else
    g_pstRegIO->ioshare_reg20.u32 = 0x0;  //set tlv320 reset pin
    #ifdef USE_GPIO_I2C
    g_pstRegIO->ioshare_reg17.u32 = 0x0;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_reg18.u32 = 0x0;  //set I2C2_SDA pin
    #else
    g_pstRegIO->ioshare_reg19.u32 = 0x3;  //set I2C2_SCL pin
    g_pstRegIO->ioshare_reg20.u32 = 0x3;  //set I2C2_SDA pin
    #endif
#endif

   //reset
    HI_DRV_GPIO_SetDirBit(SLAC_RESET_GPIO_GNUM_S40*8+SLAC_RESET_GPIO_PNUM_S40, 0);
    HI_DRV_GPIO_WriteBit(SLAC_RESET_GPIO_GNUM_S40*8+SLAC_RESET_GPIO_PNUM_S40, 0);
    msleep(200);

    HI_DRV_GPIO_WriteBit(SLAC_RESET_GPIO_GNUM_S40*8+SLAC_RESET_GPIO_PNUM_S40, 1);

    if ((i2cchn < 0) || (i2cchn >= HI_I2C_MAX_NUM))
    {
        HI_ERR_AIC3X("i2c chn for TLV320 is invalid, should 0~%d, but input is %d.\n", HI_I2C_MAX_NUM, i2cchn);
    }

#ifdef TLV320AIC31_PM
    ret = himedia_register(&tlv320aic31_dev);
#else
    ret = misc_register(&tlv320aic31_dev);
#endif
    if (ret)
    {
        HI_ERR_AIC3X("could not register tlv320aic31 device\n");
        return -1;
    }

    for (i = 0; i < chip_count; i++)
    {
        #ifdef USE_GPIO_I2C
        s_pGpioI2cExtFunc = HI_NULL;
        HI_DRV_MODULE_GetFunction(HI_ID_GPIO_I2C, (HI_VOID**)&s_pGpioI2cExtFunc);
        if(s_pGpioI2cExtFunc && s_pGpioI2cExtFunc->pfnGpioI2cCreateChannel)
        {
            (s_pGpioI2cExtFunc->pfnGpioI2cCreateChannel)(&g_pu32I2cNum,SCL,SDA);//HI_S32 HI_DRV_GPIOI2C_CreateGpioI2c(HI_U32 *pu32I2cNum, HI_U32 u32SCLGpioNo, HI_U32 u32SDAGpioNo)
        }
        else
        {
            HI_ERR_AIC3X("could not get gpioI2c func\n");
        }
        #endif
        if (tlv320aic31_device_init(i) < 0)
        {
            HI_ERR_AIC3X("could not tlv320aic31_device_init tlv320aic31 device\n");
            goto exit;
        }
    }

    HI_INFO_AIC3X("tlv320aic31 driver init successful!\n");

    return ret;

exit :
#ifdef TLV320AIC31_PM
    himedia_deregister(&tlv320aic31_dev);
#else
    misc_deregister(&tlv320aic31_dev);
#endif
#ifdef USE_GPIO_I2C
    (s_pGpioI2cExtFunc->pfnGpioI2cDestroyChannel)(g_pu32I2cNum);
#endif

    HI_ERR_AIC3X("tlv320aic31 device init failed\n");

    return -1;
}

static void __EXIT__ tlv320aic31_exit(void)
{

#ifdef TLV320AIC31_PM
    himedia_deregister(&tlv320aic31_dev);
#else
    misc_deregister(&tlv320aic31_dev);
#endif
#ifdef USE_GPIO_I2C
    (s_pGpioI2cExtFunc->pfnGpioI2cDestroyChannel)(g_pu32I2cNum);
    #endif
    HI_ERR_AIC3X("deregister tlv320aic31");
}

EXPORT_SYMBOL(TLV_CODEC_Init);
EXPORT_SYMBOL(TLV_CODEC_SetPcmFormat);
EXPORT_SYMBOL(TLV_CODEC_SetSampleRate);

module_init(tlv320aic31_init);
module_exit(tlv320aic31_exit);

module_param(i2cchn, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");
