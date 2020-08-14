#include "keyled.h"
#include "base.h"

/*************************************************************************
                        0  base module
*************************************************************************/

//define pmoc type: ir = 0; keyled = 1; timeout = 2;
volatile HI_U8 pmocType = 0xff;

//define cpu status: running 0xff; suspend 0; resume: 1
volatile HI_U8 pmocflag = HI_FALSE;

/* param */
#define REG_READ_CMD   0x01
#define REG_WRITE_CMD  0x03
#define IRAM_WRITE_CMD 0x07
#define INTR_MCE_CMD   0x01d

DATA_U32_S  regAddr;
DATA_U32_S  regData;

HI_U32 u32UartBaseAddr = 0xf8b00000;

#define trace_char(ch) do{ \
    regData.val8[0] = 0; \
    regData.val8[1] = 0; \
    regData.val8[2] = 0; \
    regData.val8[3] = (ch); \
    regAddr.val32 = u32UartBaseAddr; \
    write_regVal(); \
}while(0)

void printf_char(HI_U8 ch)
{
    if (!(dbgmask & 0x4))
    {
        return;
    }

    regAddr.val32 = u32UartBaseAddr + 0x18;

    while(1)
    {
        read_regVal();
        if((regData.val8[3] & 0x20) == 0)
        {
            break;
        }
        wait_minute_2(20,20);
    }

    wait_minute_2(20,20);
    trace_char(ch);
}

void printf_val(DATA_U32_S stData)
{
    HI_U8 i = 0;

    printf_char(10); //line feed  '\n'
    printf_char(13); //carriage return '\r'
    printf_char('0');
    printf_char('x');

    for (i = 0; i< 4; i++)
    {
        if ((stData.val8[i] / 16) > 9)
        {
            printf_char(stData.val8[i] / 16 + 87);
        }
        else
        {
            printf_char(stData.val8[i] / 16 + 48);
        }
        if ((stData.val8[i] % 16) > 9)
        {
            printf_char(stData.val8[i] % 16 + 87);
        }
        else
        {
            printf_char(stData.val8[i] % 16 + 48);
        }
    }
    printf_char(10); // '\n'
    printf_char(13); //carriage return '\r'
}

#ifndef HI_ADVCA_RELEASE
void printf_str(char *string)
{
    char *p = string;

    for(; *p!='\0'; p++)
    {
        printf_char(*p);
    }
    return;
}

void Hex2Str(HI_U32 u32Hex, HI_U8 *pu8Str)
{
    HI_U32 i;
    HI_U8 u8Tmp;

    for(i = 0; i < 8; i++)
    {
        u8Tmp = (u32Hex >> (28 - (i * 4))) & 0x0F;
        if(u8Tmp >= 0 && u8Tmp <= 9)
        {
            pu8Str[i] = (u8Tmp + 0x30);
        }
        else if(u8Tmp >= 10 && u8Tmp <= 15)
        {
            pu8Str[i] = (u8Tmp + 0x37);
        }
    }
}

void printf_hex(HI_U32 u32Hex)
{
    HI_U8 u8Str[32] = {0};

    Hex2Str(u32Hex, u8Str);

    printf_str("0x");
    printf_str(u8Str);
    printf_str("\r\n");
}
#endif

void read_regVal(void)
{
    ADDR_REG_0 = regAddr.val8[3];
    ADDR_REG_1 = regAddr.val8[2];
    ADDR_REG_2 = regAddr.val8[1];
    ADDR_REG_3 = regAddr.val8[0];
    CMD_REG = REG_READ_CMD;

    while (!(CMD_STATUS_REG & 0x02))
    {
        ;
    }

    CMD_REG = 0x00;
    regData.val8[3] = RD_DATA_REG_0;
    regData.val8[2] = RD_DATA_REG_1;
    regData.val8[1] = RD_DATA_REG_2;
    regData.val8[0] = RD_DATA_REG_3;

    return ;
}

void write_regVal(void)
{
    ADDR_REG_0 = regAddr.val8[3];
    ADDR_REG_1 = regAddr.val8[2];
    ADDR_REG_2 = regAddr.val8[1];
    ADDR_REG_3 = regAddr.val8[0];
    WR_DATA_REG_0 = regData.val8[3];
    WR_DATA_REG_1 = regData.val8[2];
    WR_DATA_REG_2 = regData.val8[1];
    WR_DATA_REG_3 = regData.val8[0];

    CMD_REG = REG_WRITE_CMD;

    while (CMD_STATUS_REG & 0x01)
    {
        ;
    }
    CMD_REG = 0x00;

    return ;
}

/* function */
void read_regVal_big(void)
{
    ADDR_REG_0 = regAddr.val8[3];
    ADDR_REG_1 = regAddr.val8[2];
    ADDR_REG_2 = regAddr.val8[1];
    ADDR_REG_3 = regAddr.val8[0];
    CMD_REG = REG_READ_CMD;

    while (!(CMD_STATUS_REG & 0x02))
    {
        ;
        }

    CMD_REG = 0x00;
    regData.val8[3] = RD_DATA_REG_3;
    regData.val8[2] = RD_DATA_REG_2;
    regData.val8[1] = RD_DATA_REG_1;
    regData.val8[0] = RD_DATA_REG_0;
    return ;
}

#if 0
void write_regVal_Big(void)
{
    ADDR_REG_0 = regAddr.val8[3];
    ADDR_REG_1 = regAddr.val8[2];
    ADDR_REG_2 = regAddr.val8[1];
    ADDR_REG_3 = regAddr.val8[0];
    WR_DATA_REG_0 = regData.val8[0];
    WR_DATA_REG_1 = regData.val8[1];
    WR_DATA_REG_2 = regData.val8[2];
    WR_DATA_REG_3 = regData.val8[3];
    CMD_REG = REG_WRITE_CMD;

    while (CMD_STATUS_REG & 0x01)
    {
        ;
    }

    CMD_REG = 0x00;
    return ;
}
#endif

/* write c51 ram directly, not by bus */
void write_regValEx(void)
{
    *((unsigned char volatile xdata*)regAddr.val32 + 0) = regData.val8[3];
    *((unsigned char volatile xdata*)regAddr.val32 + 1) = regData.val8[2];
    *((unsigned char volatile xdata*)regAddr.val32 + 2) = regData.val8[1];
    *((unsigned char volatile xdata*)regAddr.val32 + 3) = regData.val8[0];

    return ;
}
/*************************************************************************
                        dbg  module
*************************************************************************/

/* valiable*/
//HI_U8   dbgflag = 0;
HI_U8   dbgmask = 0;
HI_U8   wdgon  = 0;
HI_U8 GpioValArray[2] = {0, 0};

/* function */
void dbg_val(HI_U8 pos, HI_U8 val)
{
    HI_U16 dat;

    /* show LED */
    if (dbgmask & 0x2)
    {
        dat = pos * 100 + val;
        dbg_display(dat);

        wait_minute_3(200, 200, 2);
    }

    /* save record to C51 memory */
    if (dbgmask & 0x1)
    {
        regData.val8[0] = 0;
        regData.val8[1] = 0;
        regData.val8[2] = pos;
        regData.val8[3] = val;
        regAddr.val32 = DATA_BASE_ADDR + 0xfc;
        write_regValEx();
    }
    return;
}

#if 0
/*gpio para for power control, power off 12V0 5V0 */
void pwr_getParam(void)
{
    /*record gpio index, between 40 and 47 */
    regAddr.val32 = DATA_GPIO0;
    read_regVal();
    GpioValArray[0] = regData.val8[3];

    /*record gpio output, 1 or 0 */
    regAddr.val32 = DATA_GPIO1;
    read_regVal();
    GpioValArray[1] = regData.val8[3];

    return;
}

void dbg_getParam(void)
{
    wdgon  = 0;
    dbgmask = 0;

    regAddr.val32 = DATA_WDGON;
    read_regVal();
    wdgon = regData.val8[3] & 0x1;

    regAddr.val32 = DATA_DBGMASK;
    read_regVal();
    dbgmask = regData.val8[3] & 0x3;

    pwr_getParam();

    return;
}

void dbg_resetParam(void)
{
#ifdef TEST_ON
    wdgon  = 0;
    dbgmask = 0x3;
#else
    dbg_getParam();
#endif
    return;
}


void dbg_reset(void)
{
    dbgflag = dbgmask;
    return;
}

void dbg_level(HI_U8 l)
{
    dbgflag = (l & dbgmask);

    return;
}
#endif

