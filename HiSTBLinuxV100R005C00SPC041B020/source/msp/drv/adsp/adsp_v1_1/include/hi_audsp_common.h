/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv2bO4a86Ra1cP5w/R608AW9TGITFV3+8N3VYYiVf+Q2SydsXizP+UzuonkR7
tfVi+xF1j+WV6FhB07h48c5xrYuONjte7r00QUpOD9nXfJ6cth0cBDG5lXz0JPrSDOXJMAan
caHFURX50Yd+3IHgsQc/KKmrZlDVizKDiwsNQTIVS5kgjCGaz2y/wLyQPUZm5Q==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_audsp_common.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/10/08
  Description   :
  History       :
  1.Date        : 2013/02/28
    Author      : zgjie
    Modification: Created file

 *******************************************************************************/

#ifndef __HI_AUDSP_COMMON__H__
#define __HI_AUDSP_COMMON__H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************** HIFI DSP subsystem SOC Definition ******************/

#if    defined(CHIP_TYPE_hi3751v100)   \
    || defined(CHIP_TYPE_hi3751v100b)  \
    || defined(CHIP_TYPE_hi3751v600)   \
    || defined(CHIP_TYPE_hi3751lv500)  \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)

#ifndef HI_FPGA
//ASIC
#define DSP0_CLOCK_HZ     (300 * 1000 * 1000)    /*300M asic*/
#else
//FPGA
#define DSP0_CLOCK_HZ     (50 * 1000 * 1000)     /*50M fpga*/
#endif

#elif  defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3796mv200)
#define DSP0_CLOCK_HZ     (300 * 1000 * 1000)    /*300M asic*/
#else
#error YOU MUST DEFINE  CHIP_TYPE!
#endif

#define DSP0_TIMER0_INTERVAL (DSP0_CLOCK_HZ/1000)          /* 1  ms */
#define DSP0_TIMER1_INTERVAL (DSP0_CLOCK_HZ/200)           /* 5  ms */
#define DSP0_TIMER0_INTERVAL_CAST (DSP0_CLOCK_HZ/200)      /* 5  ms */

    /* arm interrupt Definition */
#define ADSP0TOA9_IRQ_NUM (51)

/* dsp interrupt Definition */
#define DSP_INTERRUPT_DSP2DSP                0x0
#define DSP_INTERRUPT_A9TODSP                0x1
#define DSP_INTERRUPT_MCE2DSP                0x2
#define DSP_INTERRUPT_M3TODSP                0x3
#define DSP_INTERRUPT_WDG                    0x4
#define DSP_INTERRUPT_SWIOPERI               0x5
#define DSP_INTERRUPT_TIMER0                 0x6
#define DSP_INTERRUPT_SOFTINT1               0x7
#define DSP_INTERRUPT_SWI1PERI               0x8
#define DSP_INTERRUPT_SWI2PERI               0x9
#define DSP_INTERRUPT_TIMER1                 0xa
#define DSP_INTERRUPT_SOFTINT2               0xb
#define DSP_INTERRUPT_RESERVE0               0xc
#define DSP_INTERRUPT_TIMER2                 0xd
#define DSP_INTERRUPT_NMI                    0xe
#define DSP_INTERRUPT_AIAO2DSP               0xf
#define TIMER0_INT_MASK (1 << DSP_INTERRUPT_TIMER0)
#define TIMER1_INT_MASK (1 << DSP_INTERRUPT_TIMER1)

/* DSP on-chip Sram memory Addr Definition */
#define DSP0_SHARESRAM_BASEADDR    0xffff5000
#define DSP0_SHARESRAM_MAXSIZE     0x01000
#define DSP0_SHARESRAM_AOE_OFFSET  0x0000
#define DSP0_SHARESRAM_AFLT_OFFSET 0x0b00
#define DSP0_SHARESRAM_SYS_OFFSET  0x0f00
#define DSP0_SHARESRAM_SYS_SIZE    0x0040
#define DSP0_SHARESRAM_CHAN0_BASEADDR     (DSP0_SHARESRAM_BASEADDR+DSP0_SHARESRAM_SYS_OFFSET)
#define DSP0_SHARESRAM_CHAN1_BASEADDR     (DSP0_SHARESRAM_BASEADDR+DSP0_SHARESRAM_SYS_OFFSET+DSP0_SHARESRAM_SYS_SIZE)


/* DSP running code&data DDR memory Addr  Definition */
#define DSP_DDR_CODE_BASEADDR 0x3800000    /* 32M - 40M */
#define DSP_DDR_CODE_MAXSIZE  0x0800000


#define DSP_DDR_CACHE_DMA_SUPPORT   /* default */
//#define DSP_DDR_DMAREMAP_SUPPORT
#if defined (DSP_DDR_CACHE_DMA_SUPPORT)
/* DSP DMA buffer is cacheable, dsp invalidate dcache before read dma & dsp writeback dcache after write dma */
#undef DSP_DDR_DMAREMAP_SUPPORT
#elif defined (DSP_DDR_DMAREMAP_SUPPORT)
/* DSP DMA buffer is uncacheable, remap DMA ddr memory to DSP_DDR_DMAREMAP_MAP_ADDR avoid conflict with
dsp running(code&data) memory DSP_DDR_DMAREMAP_BEG_ADDR~DSP_DDR_DMAREMAP_END_ADDR */
#undef DSP_DDR_CACHE_DMA_SUPPORT
#else
/* DSP DMA buffer & running(code&data) memory is uncache */
#undef DSP_DDR_CACHE_DMA_SUPPORT
#undef DSP_DDR_DMAREMAP_SUPPORT
#endif

#define DSP_DDR_DMAREMAP_BEG_ADDR  0x00000000    /* 512M,(0x00000000~0x1fffffff) */
#define DSP_DDR_DMAREMAP_END_ADDR  0x20000000    /* 512M, 0x20000000 */
#define DSP_DDR_DMAREMAP_MAP_ADDR  0xc0000000    /* 6*512M，must sure dsp never use this phy addr(0xc0000000~0xdfffffff) */

#define DSP_DEBUG_REG_NUM 4
#define DSP_PRIVDEBUG_REG_NUM 4

/* Define the union U_ADSP_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dis_lowpower          : 1   ; /* [0]  */
        unsigned int    Reserved_2            : 3   ; /* [3..1]  */
        unsigned int    fsm                   : 4   ; /* [7..4]  */
        unsigned int    Reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADSP_ATTR;

/* Define the union U_ADSP_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aoe_percent           : 8   ; /* [7..0]  */
        unsigned int    aflt_percent          : 8   ; /* [15..8]  */
        unsigned int    idle_percent          : 8  ;  /* [23..16]  */
        unsigned int    Reserved              : 8   ; /* [31..24]  */

    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADSP_STATISTICS;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sys_cmd               : 2   ; /* [1..0]  */
        unsigned int    Reserved_0            : 1   ; /* [2]  */
        unsigned int    sys_cmd_done          : 1   ; /* [3]  */
        unsigned int    sys_cmd_return_value  : 4   ; /* [7..4]  */

        unsigned int    aoe_cmd               : 2   ; /* [9..8]  */
        unsigned int    Reserved_1            : 1   ; /* [10]  */
        unsigned int    aoe_cmd_done          : 1   ; /* [11]  */
        unsigned int    aoe_cmd_return_value  : 4   ; /* [15..12]  */

        unsigned int    aflt_cmd               : 2   ; /* [17..16]  */
        unsigned int    Reserved_2            : 1   ; /* [18]  */
        unsigned int    aflt_cmd_done          : 1   ; /* [19]  */
        unsigned int    aflt_cmd_return_value  : 4   ; /* [23..20]  */

        unsigned int    Reserved_3            : 8   ; /* [31..24]  */

    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADSP_CTRL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    day               : 8   ; /* [7..0]  */
        unsigned int    month             : 8   ; /* [15..8]  */
        unsigned int    year              : 8   ; /* [23..16]  */
        unsigned int    Reserved          : 8  ; /* [31..24]  */

    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADSP_DATE;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    second           : 8   ; /* [7..0]  */
        unsigned int    minute           : 8   ; /* [15..8]  */
        unsigned int    hour             : 8   ; /* [23..16]  */
        unsigned int    Reserved         : 8   ; /* [31..24]  */

    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ADSP_TIME;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_ADSP_ATTR          ADSP_ATTR;
    volatile U_ADSP_STATISTICS    ADSP_STATISTICS;
    volatile U_ADSP_CTRL          ADSP_SYSCTRL;
    volatile unsigned int         ADSP_TIMER1_CNT;
    volatile unsigned int         ADSP_TIMER0_CNT;   /* reserved */
    volatile U_ADSP_DATE          ADSP_DATE;
    volatile U_ADSP_TIME          ADSP_TIME;
    volatile unsigned int         ADSP_DEBUG[DSP_DEBUG_REG_NUM];
    volatile unsigned int         ADSP_PRIVDEBUG[DSP_PRIVDEBUG_REG_NUM];
    volatile unsigned int         ADSP_RESERVED[5-DSP_PRIVDEBUG_REG_NUM];

} S_ADSP_CHN_REGS_TYPE;

/** Audio DSP Code definition*/
/** CNcomment:音频处理器模块代码标识定义 */
typedef enum hiADSP_CODEID_E
{
    /* dsp manage module */
    ADSP_CODE_SYSTEM = 0x0000,  /* dsp system  */
    ADSP_CODE_AOE,   /* audio output engine  */
    ADSP_CODE_AFLT,  /* audio aflt engine  */

    /* dsp decoder module */
    ADSP_CODE_DEC_MP2 = 0x0100,   /* MPEG audio layer 1, 2 */
    ADSP_CODE_DEC_MP3,            /* MPEG audio layer 1, 2 or 3 */
    ADSP_CODE_DEC_AAC,
    ADSP_CODE_DEC_DDP,
    ADSP_CODE_DEC_DTS,
    ADSP_CODE_DEC_TRUEHD,
    ADSP_CODE_DEC_WMASTD,
    ADSP_CODE_DEC_WMAPRO,
    ADSP_CODE_DEC_DRA,

    /* dsp encoder module */
    ADSP_CODE_ENC_AAC = 0x200,

    /* dsp codec module */
    ADSP_CODE_CODEC_AMRNB = 0x400,
    ADSP_CODE_CODEC_AMRWB,

    /* dsp SRS advance effect */
    ADSP_CODE_ADV_SRS_STUDIOSOUND_3D = 0x800,
    ADSP_CODE_ADV_SRS_STUDIOSOUND_HD = 0x800,

    /* dsp Dolby advance effect */
    ADSP_CODE_ADV_DOLBYDV258 = 0x810,
} ADSP_CODEID_E;

#ifdef HI_SND_DSP_HW_SUPPORT
/* support ADSP_Printk() function,you can read dsp log through "cat /proc/dsp_log" */
//#define HI_SND_DSP_PROC_LOG_SUPPORT1
#endif

#ifdef HI_SND_DSP_PROC_LOG_SUPPORT

struct ADSP_Ram_Buffer_S
{
    HI_U32    u32Sig;            /* check whether this ram region used by dsp log producer */
    HI_U32    u32Size;        /* the size of the ram region */
    HI_U32    u32BufferAddr;    /* start address of the log buffer */
    HI_U32    u32HeaderSize;    /* size of this struct */
    volatile    HI_U32    u32Addr;        /* the start address of the ram region */
    volatile    HI_U32    u32BufferSize;    /* how many buffer size actually used */
    volatile    HI_U32    u32Start;        /* the offset of the buffer currently writing to */
    volatile    HI_U32    u32ReadAddr;        /* the offset of the buffer currently reading from */
    HI_U8    au8Data[8];
};

struct ADSP_Ram_Zone_S
{
    HI_VOID *pVaddr;
    struct ADSP_Ram_Buffer_S *pstBuffer;
};

#define LOG_ADDR  0x42A00000
#define LOG_SIZE  0x14000
#define HOLE_SIZE 0x1000
#define RAM_SIG   (0x474E4159)

#endif

typedef enum
{
    ADSP_MEMCPY_DSP2DSP = 0x0,
    ADSP_MEMCPY_DSP2ARM,
    ADSP_MEMCPY_ARM2DSP,
    ADSP_MEMCPY_ARM2ARM
} ADSP_MEMCPY_DIR_E;

typedef struct
{
    volatile unsigned int         ADDR_LOW;
    volatile unsigned int         ADDR_HIGH;
} S_AOE_ADDR_TYPE;

/*
 * 作用: ADSP_WriteAddr将地址写入寄存器，ADSP_ReadAddr从寄存器中取出地址
 * addr支持类型: 1、HI_VOID* 2、32位物理地址 3、内核虚拟地址(32位系统中为32bit，64位系统中为64bit)
 * reg必须为S_AOE_ADDR_TYPE结构体类型
 */
/* 使用中间变量传递64位虚拟地址，避免出现强制使用时的对齐问题 */
#define ADSP_WriteAddr(addr, reg) \
    do { \
        S_AOE_ADDR_TYPE reg_tmp = {0x0, 0x0}; \
        *((typeof(addr)*)(&(reg_tmp))) = addr; \
        reg.ADDR_LOW = reg_tmp.ADDR_LOW; \
        reg.ADDR_HIGH = reg_tmp.ADDR_HIGH; \
    } while (0)

#if defined (__XTENSA__)
#define ADSP_ReadAddr(reg, addr) \
    do { \
        addr = *((HI_VOID**)(&(reg))); \
    } while (0)
#else
#define ADSP_ReadAddr(reg, addr) \
    do { \
        S_AOE_ADDR_TYPE reg_tmp = {0x0, 0x0}; \
        reg_tmp.ADDR_LOW = reg.ADDR_LOW; \
        reg_tmp.ADDR_HIGH = reg.ADDR_HIGH; \
        addr = *((typeof(addr)*)(&(reg_tmp))); \
    } while (0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
