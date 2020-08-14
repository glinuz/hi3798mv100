#include "base.h"
#include "gpio.h"
#include "keyled.h"
#include "ir.h"
#include "timer.h"

HI_U8 g_u8ChipType = 0;
HI_U16 g_u16ChipId = 0;
static HI_U8 g_u8HighWakeUp;

static DATA_U32_S g_u32GpioPort;
static HI_U8 kltype = 0;

HI_U8 g_u8KeyEnterPmoc = 0x0;

HI_U8 g_u8GpioIEV = 0; //0:Falling edge or low level; 1: Rising edge or high level
HI_U8 g_u8GpioIBE = 0; //0:single edge; 1:double edge
HI_U8 g_u8GpioIS  = 0; //0:edge; 1: level

HI_U32 g_u32DateBaseAddr = DATA_BASE_ADDR;
HI_BOOL g_bAdvCaSupport = HI_FALSE;

HI_BOOL g_bBootSuspend = HI_FALSE;

#define DDRPHY_ARRAY_COUNT 4
#define DDRPHY_REG_COUNT 20
#define RESUME_FLAG 0x66031013

#define MCU_LOAD_CODE 0x12345678
#define ENTER_PMOC_FLAG 0x12345678
#define MCU_BOOT_SUSPEND_FLAG 0x12345678

#define KEYLED_TYPE_GPIOKEY 5

extern HI_VOID ADVCA_RUN_CHECK(HI_VOID);
extern xdata HI_U8 g_u8CaVendorId;

extern HI_U32 u32UartBaseAddr;

/*************************************************************************
                        main  module
*************************************************************************/
HI_U8 intStatus, intStatus1, intStatus2;
HI_VOID intr_process() interrupt 4
{
    HI_U8 i;

    if (HI_FALSE != pmocflag)
    {
        return;
    }

    EA = 0;
    INT_MASK_0 = 0x0;
    INT_MASK_1 = 0x0;
    INT_MASK_2 = 0x0;

    intStatus  = INT_STATUS_IP0;
    intStatus1 = INT_STATUS_IP1;
    intStatus2 = INT_STATUS_IP2;

    if (((HI_CHIP_TYPE_HI3796C == g_u8ChipType) || (HI_CHIP_TYPE_HI3798C == g_u8ChipType))
        && (HI_CHIP_VERSION_V100 == g_u16ChipId))
    {
        if (intStatus1 & 0x20) //bit13
        {
            for (i = 0; i < 4; i++)
            {
                if (g_u32GpioPort.val8[i] != 0)
                {
                    gpio_isr(g_u32GpioPort.val8[i]);
                }
            }
        }
    }
    else
    {
        if (intStatus & 0x10) //bit4
        {
            for (i = 0; i < 4; i++)
            {
                if (g_u32GpioPort.val8[i] != 0)
                {
                    gpio_isr(g_u32GpioPort.val8[i]);
                    if (pmocflag == HI_TRUE)
                    {
                        intStatus &= 0xef;
                    }
                }
            }
        }
    }

    /* The keyled who use GPIO to get key has no interrupt, so it needs other */
    /* interrupts to help itself to check key.                                */
#if (defined KEYLED_CT1642_INNER) || (defined KEYLED_74HC164)
    if ((intStatus & 0x10)) //bit4
#endif
    {
        if (kltype != KEYLED_TYPE_GPIOKEY)
        {
            KEYLED_Isr();
        }
    }

    if (intStatus & 0x02) //bit1
    {
        TIMER_Isr();
    }
    else if (intStatus & 0x08) //bit3
    {
        IR_Isr();
    }

    if (intStatus2 & 0x10) //bit20
    {
        /* clear the interrupt */
        regAddr.val32 = 0xf9840a00;
        read_regVal();
        regData.val8[3] = 0x60;
        write_regVal();

        pmocflag = HI_TRUE;
        pmocType = HI_UNF_PMOC_WKUP_ETH;
    }
    else if (intStatus2 & 0x01) //bit 16
    {
        //printf_char('u');
        pmocflag = HI_TRUE;
        pmocType = HI_UNF_PMOC_WKUP_USB;
    }

    if (pmocflag == HI_FALSE)
    {
        INT_MASK_0 = 0x1a;
        INT_MASK_1 = 0x20;
        INT_MASK_2 = 0x11;
    }

    EA = 1;

    return;
}

HI_VOID MCUInit(HI_VOID)
{
    EA = 0;
    ARM_INTR_MASK = 0xff;  // intr mask
    INT_MASK_0 = 0x1a;     //  key ir timer intr
    INT_MASK_1 = 0x20;     // GPIO_STB1
    INT_MASK_2 = 0x11;     //GSF usb
    INT_MASK_3 = 0x00;

    RI = 0;
    TI = 0;
    ES = 1;

    pmocflag = HI_FALSE;

    return;
}

HI_VOID GetBaseParams(HI_VOID)
{
    /* Get the chip type */
    regAddr.val32 = CFG_BASE_ADDR + SC_GEN12;
    read_regVal();
    g_u8ChipType = regData.val8[3];
    g_u16ChipId  = regData.val16[0];
    g_bAdvCaSupport = regData.val8[2];
    if ((HI_CHIP_TYPE_HI3716M == regData.val8[3])
        && ((HI_CHIP_VERSION_V410 == regData.val16[0]) || (HI_CHIP_VERSION_V420 == regData.val16[0])))
    {
        g_u32DateBaseAddr = SC_GEN32_ADDR;
    }

    /* Get the boot suspend flag */
    regAddr.val32 = DATA_BOOT_SUSPEND_FLAG;
    read_regVal();
    if (regData.val32 == MCU_BOOT_SUSPEND_FLAG)
    {
        g_bBootSuspend = HI_TRUE;
    }

    return;
}

HI_VOID InitUart1(HI_VOID)
{
    u32UartBaseAddr = 0xf8006000;

    /* set pin mux from uart0 to uart1 */
    regAddr.val32 = 0xf8000044;
    regData.val32 = 0xa00;
    write_regVal();

    /* uart1 init 3M clock */
    regAddr.val32 = 0xf8006024;
    regData.val32 = 0x01;
    write_regVal();

    regAddr.val32 = 0xf8006028;
    regData.val32 = 0x29;
    write_regVal();

    regAddr.val32 = 0xf800602c;
    regData.val32 = 0x70;
    write_regVal();

    regAddr.val32 = 0xf8006030;
    regData.val32 = 0xf01;
    write_regVal();

    regAddr.val32 = 0xf8006034;
    regData.val32 = 0x12;
    write_regVal();

    regAddr.val32 = 0xf8006038;
    regData.val32 = 0x70;
    write_regVal();
}

HI_VOID PrintInitParams(HI_VOID)
{
#ifndef HI_ADVCA_RELEASE
    HI_U8 i;

    printf_str("======== [MCU PARAM] ======== \r\n");
    printf_str("Chip Type:");
    printf_hex(g_u8ChipType);
    printf_str("Chip Version:");
    printf_hex(g_u16ChipId);
    printf_str("Suspend Type:");
    printf_hex(g_u8HighWakeUp);
    printf_str("Reboot Enable:");
    printf_hex(wdgon);
    printf_str("Debug Mask:");
    printf_hex(dbgmask);
    printf_str("Power Gpio No:");
    printf_hex(GpioValArray[0]);
    printf_str("Power Gpio Value:");
    printf_hex(GpioValArray[1]);
    printf_str("Display Mode:");
    printf_hex(time_type);
    printf_str("Display Time Hour:");
    printf_hex(time_hour);
    printf_str("Display Time Minute:");
    printf_hex(time_minute);
    printf_str("Display Time Second:");
    printf_hex(time_second);
    printf_str("WakeUp Time:");
    printf_hex(waittime.val32);
    printf_str("WakeUp key type:");
    printf_hex(kltype);
    printf_str("WakeUp Key:");
    printf_hex(klPmocVal);
    printf_str("IR Type:");
    printf_hex(ir_type);
    printf_str("IR No:");
    printf_hex(ir_pmocnum);

    for (i = 0; i < ir_pmocnum; i++)
    {
        printf_str("IR KeyLow:");
        printf_hex(irPmocLval[i].val32);
        printf_str("IR KeyHigh:");
        printf_hex(irPmocHval[i].val32);
    }

    if (g_bAdvCaSupport)
    {
        printf_str("Ca Vender ID:");
        printf_hex(g_u8CaVendorId);
    }

    printf_str("WakeUpGpioNo:");
    printf_hex(g_u32GpioPort.val32);

    printf_str("======== [MCU Init Ready] ======== \r\n");
#endif

    return;
}

HI_VOID GetInitParams(HI_VOID)
{
    HI_U8 i;

    /* Get the standby scene */
    regAddr.val32 = g_u32DateBaseAddr + DATA_CHIP;
    read_regVal();
    g_u8ChipType = regData.val8[3];
    g_u8HighWakeUp = regData.val8[2];
    g_u16ChipId = regData.val16[0];

    regAddr.val32 = g_u32DateBaseAddr + DATA_WDGON;
    read_regVal();
    wdgon = regData.val8[3] & 0x1;

    /* Get the debug param */
    regAddr.val32 = g_u32DateBaseAddr + DATA_DBGMASK;
    read_regVal();
    dbgmask = regData.val8[3] & 0x7;

    /*record gpio index, between 40 and 47 */
    regAddr.val32 = g_u32DateBaseAddr + DATA_GPIO0;
    read_regVal();
    GpioValArray[0] = regData.val8[3];

    /*record gpio output, 1 or 0 */
    regAddr.val32 = g_u32DateBaseAddr + DATA_GPIO1;
    read_regVal();
    GpioValArray[1] = regData.val8[3];
    g_u8GpioIEV = regData.val8[2];
    g_u8GpioIBE = regData.val8[1];
    g_u8GpioIS  = regData.val8[0];

    regAddr.val32 = g_u32DateBaseAddr + DATA_KEYTYPE;
    read_regVal();
    kltype = regData.val8[3];

    /* Get timer param */
    if (kltype != KEYLED_TYPE_GPIOKEY)
    {
        regAddr.val32 = g_u32DateBaseAddr + DATA_DISPMODE;
        read_regVal();
        time_type = regData.val8[3];

        regAddr.val32 = g_u32DateBaseAddr + DATA_DISPVAL;
        read_regVal();
        if (time_type == TIME_DISPLAY)
        {
            // timer display
            if ((regData.val8[1] >= 24)
                || (regData.val8[2] >= 60)
                || (regData.val8[3] >= 60))
            {
                // default value
                time_hour = 9;
                time_minute = 58;
                time_second = 0;
            }
            else
            {
                time_hour = regData.val8[1];
                time_minute = regData.val8[2];
                time_second = regData.val8[3];
            }
        }
    }

    /* Get the time out of wake up */
    regAddr.val32 = g_u32DateBaseAddr + DATA_TIMEVAL;
    read_regVal();
    waittime.val32 = regData.val32;

    regAddr.val32 = g_u32DateBaseAddr + DATA_KEYVAL;
    read_regVal();
    klPmocVal = regData.val8[3];

    if (g_bAdvCaSupport)
    {
        regAddr.val32 = g_u32DateBaseAddr + DATA_CAVENDOR;
        read_regVal();
        g_u8CaVendorId = regData.val8[3];
    }

    /* Get gpio port param */
    regAddr.val32 = g_u32DateBaseAddr + DATA_GPIOPORT;
    read_regVal();
    g_u32GpioPort.val32 = regData.val32;

    /* Get the IR param */
    regAddr.val32 = g_u32DateBaseAddr + DATA_IRTYPE;
    read_regVal();
    ir_type = regData.val8[3];

    regAddr.val32 = g_u32DateBaseAddr + DATA_IRNUM;
    read_regVal();
    ir_pmocnum = regData.val8[3];

    regAddr.val32 = g_u32DateBaseAddr + DATA_IRVAL;
    for (i = 0; i < ir_pmocnum; i++)
    {
        read_regVal();
        irPmocLval[i].val32 = regData.val32;
        regAddr.val8[3] += 0x4;
        read_regVal();
        irPmocHval[i].val32 = regData.val32;
        regAddr.val8[3] += 0x4;
    }

    PrintInitParams();

    return;
}

void SaveRegToRam(HI_U32 u32DDRBaseAddr, HI_U32 u32DataBaseAddr, HI_U32 u32RegCount)
{
    HI_U8 i, j;
    HI_U32 u32TmpAddr;

    for (i = 0; i < DDRPHY_ARRAY_COUNT; i++)
    {
        regAddr.val32 = u32DataBaseAddr + i * (u32RegCount + 2) * 0x4;
        regData.val32 = u32RegCount;
        write_regValEx();

        regAddr.val32 = u32DataBaseAddr + i * (u32RegCount + 2) * 0x4 + 0x4;
        regData.val32 = u32DDRBaseAddr + 0x210 + i * 0x80;
        write_regValEx();

        for (j = 0; j < u32RegCount; j++)
        {
            regAddr.val32 = u32DDRBaseAddr + 0x210 + i * 0x80 + j * 0x4;
            read_regVal();
            regAddr.val32 = u32DataBaseAddr + i * (u32RegCount + 2) * 0x4 + 0x8 + j * 0x4;
            write_regValEx();
        }
    }

    u32TmpAddr = u32DataBaseAddr + DDRPHY_ARRAY_COUNT * (u32RegCount + 2) * 0x4;
    
    if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        /* save (0x21c + 0x400), (0x220 + 0x400), (0x224 + 0x400) */
        for (i = 0; i < DDRPHY_ARRAY_COUNT; i++)
        {
            regAddr.val32 = u32TmpAddr;
            regData.val32 = 3;
            write_regValEx();

            regAddr.val32 = u32TmpAddr + 0x4;
            regData.val32 = u32DDRBaseAddr + 0x21c + i * 0x80 + 0x400;
            write_regValEx();
            
            for (j = 0; j < 3; j++)
            {
                regAddr.val32 = u32DDRBaseAddr + 0x21c + i * 0x80 + 0x400 + j * 0x4;
                read_regVal();
                regAddr.val32 = u32TmpAddr + 0x8 + j * 0x4;
                write_regValEx();
            }

            u32TmpAddr += 5 * 0x4;

            /* save (0x240 + 0x400) */
            regAddr.val32 = u32TmpAddr;
            regData.val32 = 1;
            write_regValEx();

            regAddr.val32 = u32TmpAddr + 0x4;
            regData.val32 = u32DDRBaseAddr + 0x240 + i * 0x80 + 0x400;
            write_regValEx();

            regAddr.val32 = u32DDRBaseAddr + 0x240 + i * 0x80 + 0x400;
            read_regVal();
            regAddr.val32 = u32TmpAddr + 0x8;
            write_regValEx();

            u32TmpAddr += 3 * 0x4;

            /* save 0x270 0x274 for ddr4*/
            regAddr.val32 = u32TmpAddr;
            regData.val32 = 2;
            write_regValEx();

            regAddr.val32 = u32TmpAddr + 0x4;
            regData.val32 = u32DDRBaseAddr + 0x270 + i * 0x80;
            write_regValEx();

            for (j = 0; j < 2; j++)
            {
                regAddr.val32 = u32DDRBaseAddr + 0x270 + i * 0x80 + j * 0x4;
                read_regVal();
                regAddr.val32 = u32TmpAddr + 0x8 + j * 0x4;
                write_regValEx();
            }
            u32TmpAddr += 4 * 0x4;
            
        }
    }

    /* set (u32DDRBaseAddr + 0x70) bit 20(19 in 98cv200) to 1 */
    regAddr.val32 = u32TmpAddr;
    regData.val32 = 1;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x4;
    regData.val32 = u32DDRBaseAddr + 0x70;
    write_regValEx();

    regAddr.val32 = u32DDRBaseAddr + 0x70;
    read_regVal();
    if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        regData.val8[1] |= 0x08;
    }
    else
    {
        regData.val8[1] |= 0x10;
    }

    regAddr.val32 = u32TmpAddr + 0x8;
    write_regValEx();

    /* set (u32DDRBaseAddr + 0x70) bit 20(19 in 98cv200) to 0 */
    regAddr.val32 = u32TmpAddr + 0xc;
    regData.val32 = 1;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x10;
    regData.val32 = u32DDRBaseAddr + 0x70;
    write_regValEx();

    regAddr.val32 = u32DDRBaseAddr + 0x70;
    read_regVal();
    if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        regData.val8[1] &= 0xf7;
    }
    else
    {
        regData.val8[1] &= 0xef;
    }

    regAddr.val32 = u32TmpAddr + 0x14;
    write_regValEx();

    /* set (u32DDRBaseAddr + 0x70) bit 15 to 1 */
    regAddr.val32 = u32TmpAddr + 0x18;
    regData.val32 = 1;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x1c;
    regData.val32 = u32DDRBaseAddr + 0x04;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x20;
    regData.val32 = 0x8000;
    write_regValEx();

    /* set (u32DDRBaseAddr + 0x70) bit 15 to 0 */
    regAddr.val32 = u32TmpAddr + 0x24;
    regData.val32 = 1;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x28;
    regData.val32 = u32DDRBaseAddr + 0x04;
    write_regValEx();

    regAddr.val32 = u32TmpAddr + 0x2c;
    regData.val32 = 0x0;
    write_regValEx();

    return;
}

void SaveRegToRamEx(HI_U32 u32DDRBaseAddr, HI_U32 u32DataBaseAddr, HI_U32 u32RegCount)
{
    HI_U8 i, j;

    for (i = 0; i < DDRPHY_ARRAY_COUNT; i++)
    {
        for (j = 0; j < u32RegCount; j++)
        {
            regAddr.val32 = u32DDRBaseAddr + 0x210 + i * 0x80 + j * 0x4;
            read_regVal();
            regAddr.val32 = u32DataBaseAddr + i * u32RegCount * 0x4 + j * 0x4;
            write_regVal();
        }
    }

    return;
}

void DDRPHYRegSave(void)
{
    HI_U32 u32DDRPHY0BaseAddr = 0;
    HI_U32 u32DDRPHY1BaseAddr = 0;
    HI_U32 u32DataBaseAddr, u32DDRPHYCount;
    HI_U8 u8Force = 0;

    if (HI_CHIP_TYPE_HI3798C_A == g_u8ChipType)
    {
        u32DDRPHYCount = 13;
        u32DDRPHY0BaseAddr = DDRPHY0_BASE_ADDR_98CV200_A;
        u32DDRPHY1BaseAddr = DDRPHY1_BASE_ADDR_98CV200_A;
    }
    else if ((HI_CHIP_TYPE_HI3798C_B == g_u8ChipType)
             || ((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
             || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
             || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        u32DDRPHYCount = 13;
        u32DDRPHY0BaseAddr = DDRPHY0_BASE_ADDR_98CV200_A;
    }
    else if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) || (HI_CHIP_TYPE_HI3796C == g_u8ChipType))
             && (HI_CHIP_VERSION_V100 == g_u16ChipId))
    {
        u32DDRPHYCount = 20;
        u32DDRPHY0BaseAddr = DDRPHY0_BASE_ADDR;
        u32DDRPHY1BaseAddr = DDRPHY1_BASE_ADDR;
    }
    else
    {
        u32DDRPHYCount = 20;
        u32DDRPHY0BaseAddr = DDRPHY_BASE_ADDR;
    }

    if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
        || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        u8Force = 1;
    }

    if ((g_bAdvCaSupport) || (1 == u8Force))
    {
        u32DataBaseAddr = DATA_PHY_BASE_ADDR;
        SaveRegToRam(u32DDRPHY0BaseAddr, u32DataBaseAddr, u32DDRPHYCount);

        if (u32DDRPHY1BaseAddr != 0)
        {
            u32DataBaseAddr += (DDRPHY_ARRAY_COUNT * (u32DDRPHYCount + 2) + 12) * 0x4;
            SaveRegToRam(u32DDRPHY1BaseAddr, u32DataBaseAddr, u32DDRPHYCount);
        }

        u32DataBaseAddr += (DDRPHY_ARRAY_COUNT * (u32DDRPHYCount + 2) + 12) * 0x4;

        if (1 == u8Force)
        {
           u32DataBaseAddr +=  DDRPHY_ARRAY_COUNT * 12 * 0x4;
        }
        
        /*  set the last value to 0, so bootrom will recognize the end */
        regAddr.val32 = u32DataBaseAddr;
        regData.val32 = 0;
        write_regValEx();

        /* save the address of c51 ram to SC_GEN16 for bootrom restoring ddr phy reg */
        regAddr.val32 = CFG_BASE_ADDR + SC_GEN16;
        regData.val32 = DATA_PHY_BASE_ADDR;
        write_regVal();
    }
    else
    {
        if ((HI_CHIP_TYPE_HI3716M == g_u8ChipType)
            && ((HI_CHIP_VERSION_V410 == g_u16ChipId) || (HI_CHIP_VERSION_V420 == g_u16ChipId)))
        {
            regAddr.val32 = REG_DDRCA_RANDOM0;
            read_regVal();
            regAddr.val32 = DATA_DDRCA_RANDOM_ADDR;
            write_regValEx();

            regAddr.val32 = REG_DDRCA_RANDOM1;
            read_regVal();
            regAddr.val32 = DATA_DDRCA_RANDOM_ADDR + 0x4;
            write_regValEx();
        }

        /* Save DDRPHY0 Regs to DATA_PHY0_BASE_ADDR */
        SaveRegToRamEx(u32DDRPHY0BaseAddr, DATA_PHY0_BASE_ADDR, u32DDRPHYCount);

        /* Save DDRPHY1 Regs to DATA_PHY1_BASE_ADDR */
        if (u32DDRPHY1BaseAddr != 0)
        {
            SaveRegToRamEx(u32DDRPHY1BaseAddr, DATA_PHY1_BASE_ADDR, u32DDRPHYCount);
        }
    }

    return;
}

void DDREnterSelf(void)
{
    if ((HI_CHIP_TYPE_HI3796C == g_u8ChipType)
        || (HI_CHIP_TYPE_HI3798C == g_u8ChipType)
        || (HI_CHIP_TYPE_HI3798M_A == g_u8ChipType)
        || (HI_CHIP_TYPE_HI3798C_A == g_u8ChipType)
        || (HI_CHIP_TYPE_HI3798C_B == g_u8ChipType)
        || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
    {
        HI_U32 u32DDRC0BaseAddr, u32DDRC1BaseAddr, u32DDRPHY0BaseAddr, u32DDRPHY1BaseAddr;

        if (((HI_CHIP_TYPE_HI3798C_A == g_u8ChipType) || (HI_CHIP_TYPE_HI3798C_B == g_u8ChipType))
            || ((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
        {
            u32DDRC0BaseAddr = DDRC0_BASE_ADDR_98CV200_A;
            u32DDRC1BaseAddr = DDRC1_BASE_ADDR_98CV200_A;
            u32DDRPHY0BaseAddr = DDRPHY0_BASE_ADDR_98CV200_A;
            u32DDRPHY1BaseAddr = DDRPHY1_BASE_ADDR_98CV200_A;
        }
        else
        {
            u32DDRC0BaseAddr = DDRC0_BASE_ADDR;
            u32DDRC1BaseAddr = DDRC1_BASE_ADDR;
            u32DDRPHY0BaseAddr = DDRPHY0_BASE_ADDR;
            u32DDRPHY1BaseAddr = DDRPHY1_BASE_ADDR;
        }

        /* Config DDR to self-refresh state */
        regAddr.val32 = u32DDRC0BaseAddr + DDR_SREF;
        read_regVal();
        regData.val8[3] = 0x01;
        write_regVal();

        /* Whether DDR change to self-refresh state */
        while (1)
        {
            regAddr.val32 = u32DDRC0BaseAddr + DDR_STATUS;
            read_regVal();
            if ((regData.val8[3] & 0x1) == 0x1) //bit0
            {
                break;
            }
        }

        regAddr.val32 = u32DDRPHY0BaseAddr + DDR_PHYCTRL0;
        regData.val32 = 0x2800;
        write_regVal();

        if ((HI_CHIP_TYPE_HI3798C_B != g_u8ChipType) && (HI_CHIP_VERSION_V200 != g_u16ChipId))
        {
            regAddr.val32 = u32DDRC1BaseAddr + DDR_SREF;
            read_regVal();
            regData.val8[3] = 0x01;
            write_regVal();

            while (1)
            {
                dbg_val(0x11, 0x3);
                regAddr.val32 = u32DDRC1BaseAddr + DDR_STATUS;
                read_regVal();
                if ((regData.val8[3] & 0x1) == 0x1) //bit0
                {
                    break;
                }
            }

            regAddr.val32 = u32DDRPHY1BaseAddr + DDR_PHYCTRL0;
            regData.val32 = 0x2800;
            write_regVal();
        }

        /* enable DDRPHY ISO */
        regAddr.val32 = CFG_BASE_ADDR + DDR_PHY_ISO;
        read_regVal();
        regData.val8[3] &= 0xf0; //bit0-1 DDRPHY0 bit2-3 DDRPHY1
        regData.val8[3] |= 0x0A;
        write_regVal();
    }
    else
    {
        /* Config DDR to self-refresh state */
        regAddr.val32 = DDR_BASE_ADDR + DDR_SREF;
        read_regVal();
        regData.val8[3] |= 0x01;
        write_regVal();

        /* Whether DDR change to self-refresh state */
        while (1)
        {
            regAddr.val32 = DDR_BASE_ADDR + DDR_STATUS;
            read_regVal();
            if ((regData.val8[3] & 0x1) == 0x1) //bit0
            {
                break;
            }
        }

        regAddr.val32 = DDRPHY_BASE_ADDR + DDR_PHYCTRL0;
        read_regVal();
        regData.val8[2] &= 0xe8; //set bit 12 to 0;bit10:8 to 0
        regData.val8[3] &= 0x7f; //set bit 7 to 0;
        write_regVal();

        /* enable DDRPHY ISO */
        regAddr.val32 = CFG_BASE_ADDR + DDR_PHY_ISO;
        read_regVal();
        regData.val8[3] &= 0xfc;
        regData.val8[3] |= 0x02; //bit1:0 TODO need to confirm the value.//0x03
        write_regVal();
    }
}

void GetTimePeriod(void)
{
    HI_U32 u32TimePeriod = 0;

    regAddr.val32 = g_u32DateBaseAddr + DATA_TIMEVAL;
    read_regVal();
    u32TimePeriod = regData.val32 - waittime.val32;

    /* save standby period in 51 ram */
    regAddr.val32 = DATA_PERIOD;
    regData.val32 = u32TimePeriod;
    write_regValEx();

#ifndef HI_ADVCA_RELEASE
    printf_str("Suspend Period:");
    printf_hex(u32TimePeriod);
#endif

    return;
}

void SystemPowerDown(void)
{
    /*set the delay 100ms between power up and  revocation of reset*/
    /* (1000 / 12) * 0x124f80 ns  */
    regAddr.val32 = 0xf8000038;
    regData.val32 = 0x124f80;
    write_regVal();

    regAddr.val32 = 0xf8000008;
    regData.val32 = 0x0;
    write_regVal();

    while (1)
    {
        regAddr.val32 = 0xf8000040;
        read_regVal();
        if ((regData.val8[3] & 0xf0) == 0x60) //bit[7:4]
        {
            break;
        }
    }

    return;
}

HI_VOID CpuCheckWfi(HI_VOID)
{
    while (0 == g_u8KeyEnterPmoc)
    {
        regAddr.val32 = 0xf840E51C;
        read_regVal();
        if (regData.val32 == MCU_LOAD_CODE)
        {
            break;
        }

        /* check core0 WFI status */
        regAddr.val32 = PMC_BASE_ADDR + PERI_PMC2;
        read_regVal();
        if ((regData.val8[3] & 0x1) == 0x1)     //bit 0 : 1 means WFI
        {
            break;
        }

        /* check scu and L2 idle */
        regAddr.val32 = PMC_BASE_ADDR + PERI_PMC46;
        read_regVal();
        if ((regData.val8[3] & 0x06) == 0x06)         //bit 2:1
        {
            break;
        }
    }

    return;
}

HI_VOID ClockPowerDown(HI_VOID)
{
    //choose ddr timer to 24M
    regAddr.val32 = CRG_BASE_ADDR + 0x128;
    read_regVal();
    regData.val32 = 0x304;
    write_regVal();
    regData.val32 = 0x704;
    write_regVal();

    /* GPU Power Down */
    regAddr.val32 = PMC_BASE_ADDR + 0xb0;
    read_regVal();
    regData.val8[3] = 0x0c;
    write_regVal();

    //switch bus to 24M
    regAddr.val32 = CRG_BASE_ADDR + 0x58;
    regData.val32 = 0;
    write_regVal();

    //APLL  power down
    regAddr.val32 = CRG_BASE_ADDR + 0x04;
    read_regVal();
    regData.val8[1] |= 0x10;
    write_regVal();

    if ((HI_CHIP_TYPE_HI3798M != g_u8ChipType)
        && (HI_CHIP_TYPE_HI3796M != g_u8ChipType)
        && (HI_CHIP_TYPE_HI3716M != g_u8ChipType))
    {
        //VPLL  power down
        regAddr.val32 = CRG_BASE_ADDR + 0x24;
        read_regVal();
        regData.val8[1] |= 0x10;
        write_regVal();
    }

    //HPLL  power down
    regAddr.val32 = CRG_BASE_ADDR + 0x2c;
    read_regVal();
    regData.val8[1] |= 0x10;
    write_regVal();

    //QPLL  power down.
    regAddr.val32 = CRG_BASE_ADDR + 0x44;
    read_regVal();
    regData.val8[1] |= 0x10;
    write_regVal();

    return;
}

HI_VOID SystemPowerDownMultiStep(HI_VOID)
{
    /* Isolation zone configuration */
    regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
    read_regVal();
    regData.val8[3] |= 0x8;
    write_regVal();

    //printf_char('j');

    /* Power-down area reset */
    regAddr.val32 = CFG_BASE_ADDR + MCU_SRST_CTRL;
    read_regVal();
    regData.val8[0] |= 0x10;
    write_regVal();

    /* after reset system, wait 1.5s, then power down, for EMMC */
    if (0x1 == g_u8KeyEnterPmoc)
    {
        wait_minute_3(45, 210, 210);
    }

    //printf_char('i');

    if (!(dbgmask & 0x4))
    {
        /* standby powoff output 0: power down */
        regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
        read_regVal();
        regData.val8[3] &= 0xfd;             //bit 1
        write_regVal();
    }

    return;
}

void SystemSuspend(void)
{
    if (((HI_CHIP_TYPE_HI3796C == g_u8ChipType) || (HI_CHIP_TYPE_HI3798C == g_u8ChipType))
        && (HI_CHIP_VERSION_V100 == g_u16ChipId))
    {
        if (!g_bAdvCaSupport)
        {
            CpuCheckWfi();
        }

        /* GPU Power Down */
        regAddr.val32 = PMC_BASE_ADDR + PERI_PMC44;
        read_regVal();
        regData.val8[3] = 0x0c; //bit2=1 bit 3=1
        write_regVal();

        /* Enable CPU Isolation */
        regAddr.val32 = PMC_BASE_ADDR + PERI_PMC0;
        read_regVal();
        regData.val8[3] |= 0x04; //bit2=1
        write_regVal();
    }

    /* CPU Reset*/
    regAddr.val32 = PMC_BASE_ADDR + PERI_PMC47;
    read_regVal();
    regData.val8[2] |= 0x02; //bit9
    write_regVal();

    if ((g_u8CaVendorId != CA_VENDOR_ID_NAGRA) || (!g_bAdvCaSupport))
    {
        if ((0x0 == wdgon) && (0x0 == g_u8KeyEnterPmoc))
        {
            DDREnterSelf();
        }
    }

    /* change MCU bus clock to 24M / 8 */
    regAddr.val32 = CFG_BASE_ADDR + MCU_CTRL;
    read_regVal();
    regData.val8[3] &= 0xfc; //bit [1:0] = 0x0
    write_regVal();

    if (NORMAL_WAKEUP != g_u8HighWakeUp)
    {
        ClockPowerDown();
    }

    if (!(dbgmask & 0x4))
    {
        if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
        {
            //need to do nothing
        }
        else
        {
            /* Config bus core idle state */
            regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
            read_regVal();
            regData.val8[3] |= 0x40; //bit 6 to 1;
            write_regVal();

            /* Whether bus core change to idle state */
            while (0 == g_u8KeyEnterPmoc)
            {
                regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
                read_regVal();
                if ((regData.val8[3] & 0x80) == 0x80) //bit7
                {
                    break;
                }
            }
        }

        if (NORMAL_WAKEUP == g_u8HighWakeUp)
        {
            //printf_char('h');
            if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
                || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
                || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
            {
                SystemPowerDown();
            }
            else
            {
                SystemPowerDownMultiStep();
            }
        }
    }

    return;
}

HI_VOID SystemResume(HI_VOID)
{
    if (!(dbgmask & 0x4) || (HI_CHIP_TYPE_HI3719M == g_u8ChipType))
    {
        if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
            || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
        {
            if (g_u8HighWakeUp != NORMAL_WAKEUP)
            {
                SystemPowerDown();
            }

            /* power up */
            regAddr.val32 = 0xf8000008;
            regData.val32 = 0x1;
            write_regVal();

            while (1)
            {
                wait_minute_2(10, 10);
            }
        }
        else
        {
            //check usb or network wakeup
            if (g_u8HighWakeUp != NORMAL_WAKEUP)
            {
                /* Power-down area reset */
                regAddr.val32 = CFG_BASE_ADDR + MCU_SRST_CTRL;
                read_regVal();
                regData.val8[0] |= 0x10;
                write_regVal();
            }
            else
            {
                /* standby powoff output 1: power up */
                regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
                read_regVal();
                regData.val8[3] |= 0x02;
                write_regVal();

                /* delay for power stable and should be more than 100ms for customer*/
                wait_minute_3(3, 210, 210);

                /* Isolation zone resume */
                regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
                read_regVal();
                regData.val8[3] &= 0xf7;
                write_regVal();
            }

            /* revocation of Power-down area reset */
            regAddr.val32 = CFG_BASE_ADDR + MCU_SRST_CTRL;
            read_regVal();
            regData.val8[0] &= 0xef; //bit28
            write_regVal();
        }

        /* Config bus ack to low */
        regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
        read_regVal();
        regData.val8[3] &= 0xbf; //bit 6 to 0;
        write_regVal();

        /* Whether bus core change to idle state */
        while (1)
        {
            regAddr.val32 = CFG_BASE_ADDR + LOW_POWER_CTRL;
            read_regVal();
            if ((regData.val8[2] & 0x01) == 0x0) //bit8
            {
                break;
            }
        }
    }

    /* IOCONFIG FOR DTS2013073105107 */
    regAddr.val32 = 0xf8a210c4;
    regData.val32 = 0x0;
    write_regVal();
    regAddr.val8[3] = 0xc8;
    write_regVal();
    regAddr.val8[3] = 0xcc;
    write_regVal();
    regAddr.val8[3] = 0xd0;
    write_regVal();
    regAddr.val8[3] = 0xd4;
    write_regVal();
    regAddr.val8[3] = 0xd8;
    write_regVal();

    return;
}

void WaitforEnterFlag(void)
{
    while (1)
    {
        wait_minute_2(200, 200);

        /* read kernel flag to break; */
        regAddr.val32 = DATA_ENTER_FLAG;
        read_regVal();
        if (ENTER_PMOC_FLAG == regData.val32)
        {
            regData.val32 = 0x0;
            write_regVal();
            break;
        }
        else if (TEMP_CHECK_TO_SUSPEND == regData.val32)
        {
            regData.val32 = 0x0;
            write_regVal();
            g_u8KeyEnterPmoc = 0x1;
            break;
        }
    }
}

void WaitforEnterStandbyFlag(void)
{
    HI_U8 u8Data = 0xff;
    HI_U32 u32Count = 0;

    regAddr.val32 = DATA_SUSPEND_FLAG;
    read_regVal();

    /* first power, jump wait */
    if (regData.val32 == MCU_LOAD_CODE)
    {
        return;
    }

    /* Get gpio port param */
    regAddr.val32 = g_u32DateBaseAddr + DATA_GPIOPORT;
    read_regVal();
    g_u32GpioPort.val8[3] = regData.val8[3];

    while (1)
    {
        wait_minute_2(10, 10);

        /* read kernel flag to break; */
        regAddr.val32 = DATA_ENTER_FLAG;
        read_regVal();
        if (ENTER_PMOC_FLAG == regData.val32)
        {
            regData.val32 = 0x0;
            write_regVal();
            break;
        }
        else if (TEMP_CHECK_TO_SUSPEND == regData.val32)
        {
            regData.val32 = 0x0;
            write_regVal();
            g_u8KeyEnterPmoc = 0x1;
            break;
        }

        if (g_u32GpioPort.val8[3] == 0)
        {
            continue;
        }

        gpio5_read_bit(g_u32GpioPort.val8[3] % 8, &u8Data);
        if (0 == u8Data)
        {
            u32Count++;
            if (u32Count > 1000)
            {
                g_u8KeyEnterPmoc = 0x1;
                u32Count = 0;
            }
        }
        else
        {
            if (0x1 == g_u8KeyEnterPmoc)
            {
                u32Count++;
                if (u32Count > 1000)
                {
                    break;
                }
            }
            else
            {
                u32Count = 0;
            }
        }
    }
}

#if 0
void LightenBlue(void)
{
    if (((HI_CHIP_TYPE_HI3796C == g_u8ChipType) || (HI_CHIP_TYPE_HI3798C == g_u8ChipType))
        && (HI_CHIP_VERSION_V100 == g_u16ChipId))
    {
        /* Close GPIO1_STB7 light */
        regAddr.val32 = 0xf8009400;
        read_regVal();
        regData.val32 |= 0x80;
        write_regVal();

        regAddr.val32 = 0xf8009200;
        regData.val32 = 0x80;
        write_regVal();

        /* Open GPIO0_STB4 light */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x10;
        write_regVal();

        regAddr.val32 = 0xf8004040;
        regData.val32 = 0x0;
        write_regVal();
    }
    else if ((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
    {
        /* GPIO5_1 */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x02;
        write_regVal();

        regAddr.val32 = 0xf8004008;
        regData.val32 = 0x0;
        write_regVal();
    }
    else
    {
        /* GPIO5_5, 5_6 */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x60;
        write_regVal();

        regAddr.val32 = 0xf8004180;
        regData.val32 = 0x40;
        write_regVal();
    }
}

void LightenRed(void)
{
    if (((HI_CHIP_TYPE_HI3796C == g_u8ChipType) || (HI_CHIP_TYPE_HI3798C == g_u8ChipType))
        && (HI_CHIP_VERSION_V100 == g_u16ChipId))
    {
        /* Close GPIO0_STB4 light */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x10;
        write_regVal();

        regAddr.val32 = 0xf8004040;
        regData.val32 = 0x10;
        write_regVal();

        /* Open GPIO1_STB7 light */
        regAddr.val32 = 0xf8009400;
        read_regVal();
        regData.val32 |= 0x80;
        write_regVal();

        regAddr.val32 = 0xf8009200;
        regData.val32 = 0x0;
        write_regVal();
    }
    else if ((HI_CHIP_TYPE_HI3796M == g_u8ChipType) || (HI_CHIP_TYPE_HI3798M == g_u8ChipType))
    {
        /* GPIO5_5, 5_6 */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x60;
        write_regVal();

        regAddr.val32 = 0xf8004180;
        regData.val32 = 0x20;
        write_regVal();
    }
    else
    {
        /* GPIO5_1 */
        regAddr.val32 = 0xf8004400;
        read_regVal();
        regData.val32 |= 0x02;
        write_regVal();

        regAddr.val32 = 0xf8004008;
        regData.val32 = 0x2;
        write_regVal();
    }
}

#endif

HI_VOID SaveStandbyFlag(HI_VOID)
{
    regAddr.val32 = DATA_SUSPEND_FLAG;
    read_regVal();
    if ((regData.val32 != MCU_LOAD_CODE)
        && (0x0 == wdgon)
        && (0x0 == g_u8KeyEnterPmoc))
    {
        // Set resume flag for boot.
        if (g_bAdvCaSupport)
        {
            regAddr.val32 = CFG_BASE_ADDR + SC_GEN0;
        }
        else
        {
            if (((HI_CHIP_TYPE_HI3798C == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
                || ((HI_CHIP_TYPE_HI3798M_A == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId))
                || ((HI_CHIP_TYPE_HI3798M == g_u8ChipType) && (HI_CHIP_VERSION_V200 == g_u16ChipId)))
            {
                regAddr.val32 = CFG_BASE_ADDR + SC_GEN0;
            }
            else
            {
                regAddr.val32 = CFG_BASE_ADDR + SC_GEN27;
            }
        }

        regData.val32 = RESUME_FLAG;
        write_regVal();
    }

    // record wakeup type in C51 Ram
    regAddr.val32 = DATA_WAKEUPTYPE;
    regData.val32 = 0x0;
    regData.val8[3] = pmocType;
    write_regValEx();

    return;
}

void main()
{
    HI_U8 i;

    while (1)
    {
        MCUInit();

        GetBaseParams();

        if (g_bAdvCaSupport)
        {
            ADVCA_RUN_CHECK();
        }
        else
        {
            if (g_bBootSuspend == HI_FALSE)
            {
                WaitforEnterFlag();
            }
            else
            {
                WaitforEnterStandbyFlag();

                //LightenRed();
            }
        }

#ifndef HI_ADVCA_RELEASE
        dbgmask = 4;
        printf_str("Enter MCU \r\n");
#endif
        GetInitParams();

        if (kltype != KEYLED_TYPE_GPIOKEY)
        {
            KEYLED_Init();
        }

        TIMER_Init();

        for (i = 0; i < 4; i++)
        {
            if (g_u32GpioPort.val8[i] != 0)
            {
                gpio_SetIntType(g_u32GpioPort.val8[i]);
            }
        }

        if ((g_u8CaVendorId != CA_VENDOR_ID_NAGRA) || (!g_bAdvCaSupport))
        {
            if ((0x0 == wdgon) && (0x0 == g_u8KeyEnterPmoc))
            {
                DDRPHYRegSave();
            }
        }

        SystemSuspend();

        if (kltype != KEYLED_TYPE_GPIOKEY)
        {
            KEYLED_Early_Display();
        }

        TIMER_Enable();

        for (i = 0; i < 4; i++)
        {
            if (g_u32GpioPort.val8[i] != 0)
            {
                gpio_IntEnable(g_u32GpioPort.val8[i], HI_TRUE);
            }
        }

        IR_Init();
        IR_Start();

#ifndef HI_ADVCA_RELEASE
        printf_str("Enter while circle \r\n");
#endif

        while (1)
        {
            wait_minute_2(10, 10);
            EA = 0;
            if (pmocflag)
            {
                break;
            }

            EA = 1;
        }

#ifndef HI_ADVCA_RELEASE
        printf_str("Quit while circle \r\n");
#endif

        if (kltype != KEYLED_TYPE_GPIOKEY)
        {
            KEYLED_Disable();
        }

        IR_Disable();

        TIMER_Disable();

        SaveStandbyFlag();

        GetTimePeriod();

        // resume system
        SystemResume();

#ifndef HI_ADVCA_RELEASE
        printf_str("Resume from MCU \r\n");
        printf_str("Resume Type:");
        printf_hex(pmocType);
#endif

        // wait for cpu power up to shut down mcu.
        while (!g_bAdvCaSupport)
        {
            wait_minute_2(10, 10);
        }
    }

    return;
}
