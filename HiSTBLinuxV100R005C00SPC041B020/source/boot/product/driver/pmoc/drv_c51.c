#include <uboot.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_drv_pmoc.h"
#include "hi_reg.h"
//#include "platform_common.h"  //++

#ifdef HI_MINIBOOT_SUPPORT
#include "stdio.h"
#endif

static HI_U32 wdgon     = 0;
static HI_U32 dbgmask   = 0x2;
static HI_U32 keyVal    = 0x8;
static HI_U32 kltype    = 2;
static HI_U32 timeVal   = 0xffffffff;
static HI_U32 dispMode  = 2;
static HI_U32 irtype    = 4;
static HI_U32 irpmocnum = 3;
static HI_U32 dispvalue = 0x00093a00;
static HI_U32 irValhigh[MCU_IRKEY_MAXNUM] = {0, 0, 0, 0, 0, 0};
static HI_U32 irVallow[MCU_IRKEY_MAXNUM]  = {0x639cff00, 0xef101980, 0xf40b5da0, 0, 0, 0};
static HI_U32 GpioValArray[2] = {0, 0};
static HI_U32 g_u32GpioPortVal = 0x28;

static HI_U32 lu32vC51Code[MCU_CODE_MAXSIZE/4] = {
#include "output.txt"
};
static HI_U8 *ltvVirAddr = NULL;

#define C51_GetVIRAddr(addr)  (ltvVirAddr + ((addr) - C51_BASE))

#define START_PMOC_ENABLE

#define SC_GEN6 (CFG_BASE_ADDR + 0x98)
#define SC_GEN17 (CFG_BASE_ADDR + 0xc4)
void set_core_voltage_ca(void)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    unsigned int regval, hpm_code, hpm_code_average, core_volt_reg_val, corner_type;
    unsigned int max_threshold;
    unsigned int min_threshold;
    unsigned int mid_core_volt;
    unsigned int min_core_volt;


    HI_REG_READ32(SC_GEN6, regval);
    max_threshold = regval >> 16;
    min_threshold = regval & 0xffff;

    HI_REG_READ32(SC_GEN17, regval);
    mid_core_volt = (regval >> 8) & 0xff;
    min_core_volt = regval & 0xff;

    /* set core volt to 1150mv */
    HI_REG_WRITE32(0xf8a23018, 0x4300a7);
	udelay (5 * 1000);

    /* hpm 0             */
    /* clock is 200M, set time division to (200/50 - 1) */
    HI_REG_READ32(0xf8a23058, regval);
    regval &= 0xffffffc0;
    regval |= 3;
    HI_REG_WRITE32(0xf8a23058, regval);

    HI_REG_READ32(0xf8a23064, regval);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    HI_REG_WRITE32(0xf8a23064, regval);

    /* hpm enable */
    HI_REG_READ32(0xf8a23058, regval);
    regval |= (1 << 24);
    HI_REG_WRITE32(0xf8a23058, regval);

    /* hpm monitor enable */
    HI_REG_READ32(0xf8a23058, regval);
    regval |= (1 << 26);
    HI_REG_WRITE32(0xf8a23058, regval);

    udelay (10 * 1000);

    HI_REG_READ32(0xf8a2305c, regval);
    hpm_code = (regval & 0x3ff);
    hpm_code_average = hpm_code;
    hpm_code = ((regval >> 12) & 0x3ff);
    hpm_code_average += hpm_code;

    HI_REG_READ32(0xf8a23060, regval);
    hpm_code = (regval & 0x3ff);
    hpm_code_average += hpm_code;
    hpm_code = ((regval >> 12) & 0x3ff);
    hpm_code_average += hpm_code;

    hpm_code_average = (hpm_code_average >> 2);

    HI_PRINT("\n hpm_code_average = 0x%x   \n", hpm_code_average);

    HI_REG_READ32(0xf8a23018, core_volt_reg_val);
    core_volt_reg_val &= 0xffff;
    if (hpm_code_average >= max_threshold)
    {
        core_volt_reg_val |= mid_core_volt << 16; /* ff chip */
        corner_type = 1;
    }
    else if (hpm_code_average < min_threshold)
    {
        core_volt_reg_val |= min_core_volt << 16; /* ss chip */
        corner_type = 3;
    }
    else
    {
        core_volt_reg_val |= mid_core_volt << 16; /* tt chip */
        corner_type = 2;
    }

    HI_REG_WRITE32(0xf8a23018, core_volt_reg_val);

    HI_REG_READ32(SC_GEN17, regval);
    regval &= 0xffffff;
    regval |= (corner_type << 24);
    HI_REG_WRITE32(SC_GEN17, regval);

    udelay (10 * 1000);
#endif
    return;
}

HI_S32  c51_loadCode(void)
{
    HI_U32 i = 0;
    HI_U32 size = 0;
    HI_U32 *dwPtr = NULL;
    HI_CHIP_TYPE_E enChipType;
    HI_CHIP_VERSION_E enChipID;

    HI_DRV_SYS_GetChipVersion(&enChipType, &enChipID);
    
    ltvVirAddr = (HI_U8 *)C51_BASE;
   
    dwPtr = (HI_U32 *)C51_GetVIRAddr(C51_BASE);
    size = sizeof(lu32vC51Code);

    for (i = 0; i < (size >> 2); i++)
    {
        HI_REG_WRITE32((dwPtr + i), lu32vC51Code[i]);
    }

    dwPtr = (HI_U32 *)C51_GetVIRAddr(C51_BASE + C51_DATA);

    HI_REG_WRITE32(SC_GEN12, (enChipType | (enChipID << 16)));
 
    HI_REG_WRITE32((dwPtr + 0), (enChipType | (enChipID << 16)));
    HI_REG_WRITE32((dwPtr + 1), wdgon);
    HI_REG_WRITE32((dwPtr + 2), dbgmask);
    HI_REG_WRITE32((dwPtr + 3), GpioValArray[0]);  /*record gpio index, between 40 and 47 */
    HI_REG_WRITE32((dwPtr + 4), GpioValArray[1]);  /*record gpio output, 1 or 0 */

    HI_REG_WRITE32((dwPtr + 5), dispMode);
    HI_REG_WRITE32((dwPtr + 6), dispvalue);
    HI_REG_WRITE32((dwPtr + 7), timeVal);

    HI_REG_WRITE32((dwPtr + 8), kltype);
    HI_REG_WRITE32((dwPtr + 9), keyVal);

    /* dwPtr + 10 is used as CA vendor ID, reserverd here */

    HI_REG_WRITE32((dwPtr + 11), g_u32GpioPortVal);
    HI_REG_WRITE32((dwPtr + 12), irtype);
    HI_REG_WRITE32((dwPtr + 13), irpmocnum);

    for (i = 0; i < irpmocnum; i++)
    {
        HI_REG_WRITE32((dwPtr + 14 + 2 * i), irVallow[i]);
        HI_REG_WRITE32((dwPtr + 15 + 2 * i), irValhigh[i]);
    }
    
    HI_REG_WRITE32(SC_GEN15, C51_CODE_LOAD_FLAG); //indicate the C51 code is loaded

    return HI_SUCCESS;
}

void LightenBlue(void)
{
    HI_U32 data;

#if defined(CHIP_TYPE_hi3798cv100)   \
|| defined(CHIP_TYPE_hi3798cv100_a) \
|| defined(CHIP_TYPE_hi3796cv100)   \
|| defined(CHIP_TYPE_hi3796cv100_a)
    /* Close red light */
    HI_REG_READ32(0xf8009400, data);
    data |= 0x80;
    HI_REG_WRITE32(0xf8009400, data);

    HI_REG_READ32(0xf8009200, data);
    data |= 0x80;
    HI_REG_WRITE32(0xf8009200, data);
    
    /* Open blue light */
    HI_REG_READ32(0xf8004400, data);
    data |= 0x10;
    HI_REG_WRITE32(0xf8004400, data);

    HI_REG_READ32(0xf8004040, data);
    data &= 0xef;
    HI_REG_WRITE32(0xf8004040, data);
#elif  defined(CHIP_TYPE_hi3798cv200)
    /* set gpio5_0  to green light */
    HI_REG_READ32(0xf8004400, data);
    data |= 0x2;
    HI_REG_WRITE32(0xf8004400, data);

    HI_REG_WRITE32(0xf8004008, 0x0);
#else
    /* set gpio5_5, 5_6  to green light */
    HI_REG_READ32(0xf8004400, data);
    data |= 0x60;
    HI_REG_WRITE32(0xf8004400, data);

    HI_REG_WRITE32(0xf8004180, 0x40);
#endif
}

#ifdef HI_PM_POWERUP_MODE1_SUPPORT
extern unsigned int get_selfboot_type(void);
void boot_suspend(void)
{
    HI_U32 data;
    HI_U32 u32Count = 0;

    /* set flag of boot suspend to mcu */
    HI_REG_WRITE32(MCU_BOOT_SUSPEND_ADDR, MCU_BOOT_SUSPEND_FLAG);

#if defined(CONFIG_BOOTROM_SUPPORT)
    HI_U32 u32SelfbootType ;

    u32SelfbootType = get_selfboot_type();
    if (SELF_BOOT_TYPE_USBHOST == u32SelfbootType)
    {
        HI_PRINT("\nBoot suspend: self boot, jump out.\n");
        LightenBlue();
        c51_loadCode();
	
        /* set bootflag to running status */
        HI_REG_WRITE32(MCU_STATUS_ADDR, KERNEL_RUNNING);
        return;	
    }
#endif    

    /* if press recovery key, jump this MCU code */
    while (1)
    {
#if    defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3798cv100_a) \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3796cv100_a)
        /* read gpio1_stb0 */
        HI_REG_READ32(0xf8009400, data);
        data &= 0xfe; //set bit0 to input
        HI_REG_WRITE32(0xf8009400, data);

        HI_REG_READ32(0xf8009004, data);
#elif  defined(CHIP_TYPE_hi3798cv200)
        /* read gpio7_1 */
        HI_REG_READ32(0xf8b27400, data);
        data &= 0xfd; //set bit1 to input
        HI_REG_WRITE32(0xf8b27400, data);

        HI_REG_READ32(0xf8b27008, data);
#else
        /* read gpio2_0 */
        HI_REG_READ32(0xf8b22400, data);
        data &= 0xfe; //set bit0 to input
        HI_REG_WRITE32(0xf8b22400, data);

        HI_REG_READ32(0xf8b22004, data);
#endif
        if (0 == data)
        {
            u32Count++;
        }
        else
        {
            u32Count = 0;
            break;
        }

        if (u32Count > 20)
        {
            HI_PRINT("\nBoot suspend: gpio detect, jump out to recovery.\n");
            LightenBlue();
            c51_loadCode();
		
	        /* set bootflag to running status */
            HI_REG_WRITE32(MCU_STATUS_ADDR, KERNEL_RUNNING);

	        /* start MCU */
            HI_REG_WRITE32(C51_BASE + C51_START, 0x0);
            udelay(1000);
            HI_REG_WRITE32(C51_BASE + C51_START, 0x1);		
            return;
        }

        udelay (30 * 1000);
    }

    /* If  reset from linux or wake up by watchdog reset, return */ 
    HI_REG_READ32(MCU_STATUS_ADDR, data);
    if (KERNEL_RUNNING != data)
    {
        if (MCU_LOAD_CODE != data)
        {
            /* Load code to mcu in first power up*/
            c51_loadCode();

            /* Need to set in boot reg instead of setting here. Set port to gpio */
            //HI_REG_WRITE32(0xf8008054, 0x1);

#if    defined(CHIP_TYPE_hi3798cv100)   \
    || defined(CHIP_TYPE_hi3798cv100_a) \
    || defined(CHIP_TYPE_hi3796cv100)   \
    || defined(CHIP_TYPE_hi3796cv100_a)
            /* read gpio1_stb1:1 means power to run, 0 means power to standby */
            HI_REG_READ32(0xf8009400, data);
            data &= 0xfd; //set bit1 to input
            HI_REG_WRITE32(0xf8009400, data);

            HI_REG_READ32(0xf8009008, data);
            if (0 == data)
#elif  defined(CHIP_TYPE_hi3798cv200)
            /* read gpio1_7:0 means power to run, 1 means power to standby */
            HI_REG_READ32(0xf8b21400, data);
            data &= 0x7f; //set bit7 to input
            HI_REG_WRITE32(0xf8b21400, data);

            HI_REG_READ32(0xf8b21200, data);
            if (0x80 == data)
#else
            /* read gpio0_7:1 means power to run, 0 means power to standby */
            HI_REG_READ32(0xf8b20400, data);
            data &= 0x7f; //set bit7 to input
            HI_REG_WRITE32(0xf8b20400, data);

            HI_REG_READ32(0xf8b20200, data);
            if (0 == data)
#endif  
            {
                HI_PRINT("\nBoot suspend: enter standby.\n");
                
                /* indicate the C51 code is loaded, offset 0x51c  */
                HI_REG_WRITE32(MCU_STATUS_ADDR, MCU_LOAD_CODE);

                /* start MCU */
                HI_REG_WRITE32(C51_BASE + C51_START, 0x1);

                /* wait for enter standby */
                while (1)
                {
                    ;
                }
            }

            HI_PRINT("\nBoot suspend: continue to boot.\n");
        }
        
        /* set bootflag to running status */
        HI_REG_WRITE32(MCU_STATUS_ADDR, KERNEL_RUNNING);
    }
    
    /* start MCU */
    HI_REG_WRITE32(C51_BASE + C51_START, 0x0);
    udelay(1000);
    HI_REG_WRITE32(C51_BASE + C51_START, 0x1);

    LightenBlue();
    
    return;
}
#endif

