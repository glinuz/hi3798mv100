#ifndef __BASE_H__
#define __BASE_H__

#include "hi_type.h"

//#define HI_ADVCA_RELEASE

#define HI_CHIP_TYPE_HI3716M     0
#define HI_CHIP_TYPE_HI3716H     1
#define HI_CHIP_TYPE_HI3716C     2
#define HI_CHIP_TYPE_HI3719M     9

#define HI_CHIP_TYPE_HI3796C     0x20
#define HI_CHIP_TYPE_HI3798C     0x21
#define HI_CHIP_TYPE_HI3796M     0x22
#define HI_CHIP_TYPE_HI3798M     0x23
#define HI_CHIP_TYPE_HI3798C_A   0x41
#define HI_CHIP_TYPE_HI3798C_B   0x42
#define HI_CHIP_TYPE_HI3798M_A   0x43

#define HI_CHIP_VERSION_V100     0x100
#define HI_CHIP_VERSION_V200     0x200
#define HI_CHIP_VERSION_V310     0x310
#define HI_CHIP_VERSION_V410     0x410
#define HI_CHIP_VERSION_V420     0x420


#define NORMAL_WAKEUP 0
#define USB_WAKEUP 1
#define ETH_WAKEUP 2

#define HI_UNF_PMOC_WKUP_IR 0
#define HI_UNF_PMOC_WKUP_KEYLED 1
#define HI_UNF_PMOC_WKUP_TIMEOUT 2
#define HI_UNF_PMOC_WKUP_ETH 3
#define HI_UNF_PMOC_WKUP_USB 4
#define HI_UNF_PMOC_WKUP_GPIO 5

#define CA_VENDOR_ID_NAGRA      0x01

#define CFG_BASE_ADDR (0xF8000000)
#define SC_GEN32_ADDR (0xF8000F00)
#define PMC_BASE_ADDR (0xF8A23000)
#define DDRC0_BASE_ADDR (0xFF588000)
#define DDRC1_BASE_ADDR (0xFF589000)
#define DDRC0_BASE_ADDR_98CV200_A (0xF8A38000)
#define DDRC1_BASE_ADDR_98CV200_A (0xF8A39000)
#define DDR_BASE_ADDR (0xF8A31000)
#define CRG_BASE_ADDR (0xF8A22000)
#define DATA_BASE_ADDR (0xF8407000)
#define TIMER_BASE_ADDR (0xF8002000)
#define IR_BASE_ADDR  (0xF8001000)

#define DDRPHY0_BASE_ADDR (0xFF58C000)
#define DDRPHY1_BASE_ADDR (0xFF58D000)
#define DDRPHY0_BASE_ADDR_98CV200_A (0xF8A3C000)
#define DDRPHY1_BASE_ADDR_98CV200_A (0xF8A3E000)


#define DDRPHY_BASE_ADDR (0xF8A38000)
#define DATA_PHY_BASE_ADDR (DATA_BASE_ADDR + 0x100)

/* DDRCA */
#define REG_DDRCA_RANDOM0	0xF8000F90
#define REG_DDRCA_RANDOM1	0xF8000F94
#define DATA_DDRCA_RANDOM_ADDR (DATA_BASE_ADDR + 0x700)


#define MCU_CTRL       0x00
#define LOW_POWER_CTRL 0x40
#define MCU_IO_REUSE   0x44
#define MCU_SRST_CTRL  0x48
#define DDR_PHY_ISO    0x58

#define DDR_PHYCTRL0   0x78

#define PERI_PMC0  0x00
#define PERI_PMC2  0x08
#define PERI_PMC44 0xB0
#define PERI_PMC46 0xB8
#define PERI_PMC47 0xBC
#define PERI_PMC76 0x130
#define PERI_PMC77 0x134

#define DDR_SREF   0x00
#define DDR_STATUS 0x0294

#define CRG_20     0x50
#define PLL_LOCK   0x150

#define SC_GEN0    0x80
#define SC_GEN6    0x98
#define SC_GEN7    0x9c
#define SC_GEN10   0xa8
#define SC_GEN11   0xac
#define SC_GEN12   0xb0
#define SC_GEN13   0xb4
#define SC_GEN27   0xec
#define SC_GEN59   0xf6c

/* Add for advanced CA to check if suspend happen */
#define SC_GEN15    0xbc

/*save the ddr of 51ram which saved wake check base addr and hash*/
#define SC_GEN19    0xcc
#define CHECK_VECTOR_BASE_ADDR (DATA_BASE_ADDR + 0xE00)

/* use this common reg to save ddr phy reg restore address of c51  */
#define SC_GEN16  0xc0

#define DATA_CHIP 	   (0x0)
#define DATA_WDGON     (0x04)
#define DATA_DBGMASK   (0x08)
#define DATA_GPIO0     (0x0c)
#define DATA_GPIO1     (0x10)

#define DATA_DISPMODE  (0x14)
#define DATA_DISPVAL   (0x18)
#define DATA_TIMEVAL   (0x1c)

#define DATA_KEYTYPE   (0x20)
#define DATA_KEYVAL    (0x24)
#define DATA_CAVENDOR  (0x28)
#define DATA_GPIOPORT  (0x2c)
#define DATA_IRTYPE    (0x30)
#define DATA_IRNUM     (0x34)
#define DATA_IRVAL     (0x38)
#define DATA_PHY0_BASE_ADDR    (DATA_BASE_ADDR + 0x100)
#define DATA_PHY1_BASE_ADDR    (DATA_BASE_ADDR + 0x300)
#define DATA_ENTER_FLAG  	(CFG_BASE_ADDR + SC_GEN13)
#define DATA_SUSPEND_FLAG   (DATA_BASE_ADDR + 0x51c)
#define DATA_BOOT_SUSPEND_FLAG (DATA_BASE_ADDR + 0x52c)
#define DATA_WAKEUPKEYLOW 	(DATA_BASE_ADDR + 0xff0)
#define DATA_WAKEUPKEYHIGH 	(DATA_BASE_ADDR + 0xff4)
#define DATA_WAKEUPTYPE 	(DATA_BASE_ADDR + 0xff8)
#define DATA_PERIOD    		(DATA_BASE_ADDR + 0xffc)

#define TIMER_LOAD     0x0
#define TIMER_VALUE    0x04
#define TIMER_CONTROL  0x08
#define TIMER_INTCLR   0x0c
#define TIMER_RIS      0x10
#define TIMER_MIS      0x14
#define TIMER_BGLOAD   0x18

#define IR_EN          0x0
#define IR_CFG         0x04
#define IR_LEADS       0x08
#define IR_LEADE       0x0c
#define IR_SLEADE      0x10
#define IR_B0          0x14
#define IR_B1          0x18
#define IR_BUSY        0x1c
#define IR_DATAH       0x20
#define IR_DATAL       0x24
#define IR_INT_MASK    0x28
#define IR_INT_STATUS  0x2c
#define IR_INT_CLR     0x30
#define IR_START       0x34

#define IR_PMOC_NUM  0x6

extern void printf_char(HI_U8 ch);
extern void printf_val(DATA_U32_S stData);

#ifndef HI_ADVCA_RELEASE
extern void printf_str(char *string);
extern void printf_hex(HI_U32 u32Hex);
#else
#define printf_str
#define printf_hex
#endif

extern HI_U8 g_u8KeyEnterPmoc;
extern HI_U32 g_u32DateBaseAddr;

//Add for advanced CA to check if suspend happen
#define PMOC_CHECK_TO_SUSPEND     0x80510001
#define PMOC_RUNTIME_CHECK_OK     0x80510003
#define TEMP_CHECK_TO_SUSPEND     0x11111111
#define CHECK_PMOC_STATUS() regAddr.val32 = (CFG_BASE_ADDR + SC_GEN15);\
    read_regVal(); \
    if(PMOC_CHECK_TO_SUSPEND == regData.val32) \
    {\
        return; \
    }\
    regAddr.val32 = DATA_ENTER_FLAG;\
    read_regVal(); \
    if(TEMP_CHECK_TO_SUSPEND == regData.val32) \
    {\
        g_u8KeyEnterPmoc = 0x1;\
        return; \
    }

#define wait_minute_1(x) do{ \
    HI_U8 i; \
    for(i = 0; i < x; i++); \
}while(0);


#define wait_minute_2(x,y) do{ \
    HI_U8 i, j; \
    for(i = 0; i < x; i++) {\
        for(j = 0; j < y; j++); \
    }\
}while(0);

#define wait_minute_3(x,y,z) do{ \
    HI_U8 i, j, k; \
    for(i = 0; i < x; i++) {\
        for(j = 0; j < y; j++) {\
            for(k = 0; k < z; k++); \
        } \
    } \
}while(0);

/* variable*/
extern volatile HI_U8 pmocType;
extern volatile HI_U8 pmocflag;

extern DATA_U32_S  regAddr;
extern DATA_U32_S  regData;

extern void read_regVal(void);
extern void write_regVal(void);
extern void write_regValEx(void);

extern HI_U8  dbgflag;
extern HI_U8  dbgmask;
extern HI_U8  wdgon;
extern HI_U8 GpioValArray[2];

extern HI_U8  klPmocVal;

extern HI_U8 ir_type;
extern HI_U8 ir_pmocnum;
extern idata DATA_U32_S irPmocHval[IR_PMOC_NUM];
extern idata DATA_U32_S irPmocLval[IR_PMOC_NUM];

extern HI_U8  time_hour;
extern HI_U8  time_minute;
extern HI_U8  time_second;
extern HI_U8  time_dot;
extern HI_U8  time_type;
//extern DATA_U32_S  channum;
extern DATA_U32_S  waittime;

extern void dbg_val(HI_U8 pos, HI_U8 val);
extern void dbg_resetParam(void);

extern void dbg_reset(void);
extern void dbg_level(HI_U8 l);

#endif
