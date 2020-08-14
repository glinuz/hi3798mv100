#ifndef __DMA_H__
#define __DMA_H__

#include <platform.h>

/******************************************************************
* DMA   Configuration
******************************************************************/
#define DMA_CHANNEL_ID              (0)

#define DMAC_INT_TC_CLR             (REG_DMA_BASE + 0x8)
#define DMAC_INT_ERR_CLR            (REG_DMA_BASE + 0x10)
#define DMAC_RAW_INT_TC_STATUS      (REG_DMA_BASE + 0x14)
#define DMAC_RAW_INT_ERR_STATUS     (REG_DMA_BASE + 0x18)
#define DMAC_SOFT_LBREQ             (REG_DMA_BASE + 0x28)
#define DMAC_CONFIG                 (REG_DMA_BASE + 0x30)
#define DMAC_SYNC                   (REG_DMA_BASE + 0x34)
#define DMAC_CX_SRC_ADDR(i)         (REG_DMA_BASE + 0x100 + (i) * 0x20)
#define DMAC_CX_DEST_ADDR(i)        (REG_DMA_BASE + 0x104 + (i) * 0x20)
#define DMAC_CX_LLI(i)              (REG_DMA_BASE + 0x108 + (i) * 0x20)
#define DMAC_CX_CONTROL(i)          (REG_DMA_BASE + 0x10C + (i) * 0x20)
#define DMAC_CX_CONFIG(i)           (REG_DMA_BASE + 0x110 + (i) * 0x20)

#define TRANSFER_SIZE_LEN           (12)

extern void global_srst(void);

#define sys_reset() \
    do \
    { \
        global_srst(); \
        global_srst(); \
        global_srst(); \
        while(1); \
    } while(1);

#define TIMEOUT_10S	                 (1000*10)

typedef union
{
    struct
    {
        unsigned int dmac_enable                 : 1; /* [0] */
        unsigned int m1_endian                   : 1; /* [1] */
        unsigned int m2_endian                   : 1; /* [2] */
        unsigned int reserved                    : 29;/* [31:3] */
    } bits;
    unsigned int u32;
} DMAC_CONFIG_U;

typedef union
{
    struct
    {
        unsigned int channel_enable              : 1; /* [0] */
        unsigned int reserved_0                  : 13;/* [13:1] */
        unsigned int int_err                     : 1; /* [14] */
        unsigned int int_tc                      : 1; /* [15] */
        unsigned int reserved_1                  : 16;/* [31:16] */
    } bits;
    unsigned int u32;
} DMAC_CX_CONFIG_U;

typedef union
{
    struct
    {
        unsigned int transfersize                : TRANSFER_SIZE_LEN;/* [11:0] */
        unsigned int sbsize                      : 3; /* [14:12] */
        unsigned int dbsize                      : 3; /* [17:15] */
        unsigned int swidth                      : 3; /* [20:18] */
        unsigned int dwidth                      : 3; /* [23:21] */
        unsigned int src_master                  : 1; /* [24] */
        unsigned int dest_master                 : 1; /* [25] */
        unsigned int src_inc                     : 1; /* [26] */
        unsigned int dest_inc                    : 1; /* [27] */
        unsigned int prot                        : 3; /* [30:28] */
        unsigned int interrupt                   : 1; /* [31] */
    } bits;
    unsigned int u32;
} DMAC_CX_CONTROL_U;

typedef struct
{
    unsigned int dest_inc;      /* 1: increase dest address */
    unsigned int src_inc;       /* 1: increase src address */
    unsigned int burst_size;    /* 0: 1 burst; 1: 4 burst */
} dma_cfg_t;

int read_with_dma(void* dest, const void* src, unsigned int count, dma_cfg_t* cfg);

typedef union
{
    struct
    {
        unsigned int dmac_cken                   : 1; /* [0] */
        unsigned int reserved_0                  : 3; /* [3:1] */
        unsigned int dmac_srst_req               : 1; /* [4] */
        unsigned int reserved_1                  : 27;/* [31:5] */
    } bits;
    unsigned int u32;
} PERI_CRG_DMA_U;

#endif