#include "REG51.h"
#include "base.h"
#include "gpio.h"

extern HI_U8 g_u8GpioIEV;
extern HI_U8 g_u8GpioIBE;
extern HI_U8 g_u8GpioIS;

void gpio5_dirset_bit(HI_U8 idx, HI_U8 val) reentrant
{
    regAddr.val32 = GPIO5_REG_DIR;
    read_regVal();

    if (val)
    {
        // input
        regData.val8[3] &= ~(1 << idx);
    }
    else
    {
        // output
        regData.val8[3] |= (1 << idx);
    }

    write_regVal();

    return;
}

void gpio5_write_bit(HI_U8 idx, HI_U8 val) reentrant
{
    regAddr.val32  = GPIO5_BASE_ADDR;
    regAddr.val32 += (0x4 << idx);

    if (val)
    {
        regData.val32 = 0;
        regData.val8[3] = (1 << idx);
    }
    else
    {
        regData.val32 = 0;
    }

    write_regVal();

    return;
}

void gpio5_read_bit(HI_U8 idx, HI_U8* val)
{
    HI_U8 tmp = 0;

#if 1
    regAddr.val32 = GPIO5_REG_DIR;
    read_regVal();
    tmp = regData.val8[3] & (1 << idx);

    if (tmp)
    {
        *val = 0xff;
        return;
    }
#endif

    regAddr.val32 = GPIO5_BASE_ADDR + (0x4 << idx);
    read_regVal();
    tmp = regData.val8[3] & (0x1 << idx);

    if (tmp)
    {
        *val = 1;
    }
    else
    {
        *val = 0;
    }

    return;
}

HI_VOID gpio_SetIntType(HI_U8 u8GpioBitNum)
{
    HI_U8 tmp = 0;

    /* get gpio5 number 0-7 */
    u8GpioBitNum = u8GpioBitNum % 8;

    regAddr.val32 = GPIO5_REG_DIR;
    read_regVal();
    tmp = regData.val8[3] & (~(0x1 << u8GpioBitNum));   // 0: input;
    regData.val8[3] = tmp;
    write_regVal();

    regAddr.val32 = GPIO5_REG_IS;
    read_regVal();
    if (g_u8GpioIS & (0x1 << u8GpioBitNum))
    {
        tmp = regData.val8[3] | (0x1 << u8GpioBitNum);   // 1: level
    }
    else
    {
        tmp = regData.val8[3] & (~(0x1 << u8GpioBitNum));   // 0: edge
    }

    regData.val8[3] = tmp;
    write_regVal();

    regAddr.val32 = GPIO5_REG_IEV;
    read_regVal();
    if (g_u8GpioIEV & (0x1 << u8GpioBitNum))
    {
        tmp = regData.val8[3] | (0x1 << u8GpioBitNum);   // 1: up edge interruput
    }
    else
    {
        tmp = regData.val8[3] & (~(0x1 << u8GpioBitNum));   // 0: down edge interruput
    }

    regData.val8[3] = tmp;
    write_regVal();

    regAddr.val32 = GPIO5_REG_IBE;
    read_regVal();
    if (g_u8GpioIBE & (0x1 << u8GpioBitNum))
    {
        tmp = regData.val8[3] | (0x1 << u8GpioBitNum);   // 1: double edge interruput
    }
    else
    {
        tmp = regData.val8[3] & (~(0x1 << u8GpioBitNum));   // 0: single edge interruput
    }

    regData.val8[3] = tmp;
    write_regVal();

    regAddr.val32 = GPIO5_REG_IC;
    regData.val8[3] = 0xff;      // clear all int
    write_regVal();

    return;
}

HI_VOID gpio_IntEnable(HI_U8 u8GpioBitNum, HI_BOOL IsEnable)
{
    HI_U8 tmp = 0;

    /* get gpio5 number 0-7 */
    u8GpioBitNum = u8GpioBitNum % 8;

    if (HI_TRUE == IsEnable)
    {
        regAddr.val32 = GPIO5_REG_IE;
        read_regVal();
        regData.val8[3] |= (0x1 << u8GpioBitNum); //1: enable interruput
        write_regVal();
    }
    else
    {
        regAddr.val32 = GPIO5_REG_IE;
        read_regVal();
        tmp = regData.val8[3] & (~(0x1 << u8GpioBitNum));   // 0:Mask interruput;
        regData.val8[3] = tmp;
        write_regVal();
    }

    return;
}

HI_VOID gpio_isr(HI_U8 u8GpioBitNum)
{
    HI_U8 ScanKey = 0;

    /* get gpio5 number 0-7 */
    u8GpioBitNum = u8GpioBitNum % 8;

    regAddr.val32 = GPIO5_REG_MIS;
    read_regVal();
    ScanKey = regData.val8[3] & (0x1 << u8GpioBitNum);

    if (ScanKey)
    {
        pmocType = HI_UNF_PMOC_WKUP_GPIO;
        pmocflag = HI_TRUE;

        gpio_IntEnable(u8GpioBitNum, HI_FALSE);
    }

    regAddr.val32 = GPIO5_REG_IC;
    regData.val8[3] = (0x1 << u8GpioBitNum);      // clear int
    write_regVal();

    return;
}
