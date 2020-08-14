/******************************************************************************
 *
 * Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
 * ******************************************************************************
 * File Name     : drv_cipher_reg.h
 * Version       : Initial
 * Author        : Hisilicon hisecurity team
 * Created       : 2013-12-28
 * Last Modified :
 * Description   : 
 * Function List :
 * History       :
 * ******************************************************************************/

#ifndef __DRV_CIPHER_REG_H__
#define __DRV_CIPHER_REG_H__

/* add include here */
//#include <sw_board.h>

#ifdef __cplusplus
extern "C"
{
#endif
/***************************** Macro Definition ******************************/

/* CRG register base address : 0xF8A22000 */
#define  CIPHER_REG_SYS_CLK_ADDR_PHY            (0xF8A22000)
/*Porting to TrustedCore STB begin*/
//vir=phy
#define CIPHER_SYS_CLK_ADDR CIPHER_REG_SYS_CLK_ADDR_PHY
/*Porting to TrustedCore STB end*/
#define  CIPHER_REG_SYS_CLK_ADDR               CIPHER_SYS_CLK_ADDR // IO_ADDRESS(CIPHER_SYS_CLK_ADDR)
/* 0x00C0:[PERI_CRG48 CA] */
#define  CIPHER_REG_SYS_CLK_CA_ADDR             (CIPHER_REG_SYS_CLK_ADDR + 0xC0)
/* 0x00C4:[PERI_CRG49] SHA */
#define  CIPHER_REG_SYS_CLK_SHA_ADDR            (CIPHER_REG_SYS_CLK_ADDR + 0xC4)

/* CIPHER register addr define */
#define  CIPHER_REG_BASE_ADDR_PHY      			(0xF9A00000)
/*Porting to TrustedCore STB begin*/
//vir=phy
#define CIPHER_BASE_ADDR CIPHER_REG_BASE_ADDR_PHY
/*Porting to TrustedCore STB end*/
#define  CIPHER_REG_BASE_ADDR                  CIPHER_BASE_ADDR // IO_ADDRESS(CIPHER_BASE_ADDR)

#define  CIPHER_REG_CHAN0_CIPHER_DOUT(id)       (CIPHER_REG_BASE_ADDR + 0x0000 + (id)*0)
#define  CIPHER_REG_CHAN0_CIPHER_IVOUT(id)      (CIPHER_REG_BASE_ADDR + 0x0010 + (id)*0)
#define  CIPHER_REG_CHAN_CIPHER_IVOUT(id)       (CIPHER_REG_BASE_ADDR + 0x0010 + (id)*16)
#define  CIPHER_REG_CIPHER_KEY(id)              (CIPHER_REG_BASE_ADDR + 0x0090 + (id)*32)
#define  CIPHER_REG_CTRL_ST0                    (CIPHER_REG_BASE_ADDR + 0x0800)
#define  CIPHER_REG_CTRL_ST1                    (CIPHER_REG_BASE_ADDR + 0x0804)
#define  CIPHER_REG_CTRL_ST2                    (CIPHER_REG_BASE_ADDR + 0x0808)
#define  CIPHER_REG_SRAM_ST0                    (CIPHER_REG_BASE_ADDR + 0x080c)
#define  CIPHER_REG_HDCP_HOST_ROOTKEY       	(CIPHER_REG_BASE_ADDR + 0x810)
#define  CIPHER_REG_HDCP_MODE_CTRL              (CIPHER_REG_BASE_ADDR + 0x820)
#define  CIPHER_REG_SEC_CHN_CFG                 (CIPHER_REG_BASE_ADDR + 0x824)
#define  CIPHER_REG_CHAN0_CIPHER_CTRL           (CIPHER_REG_BASE_ADDR + 0x1000)
#define  CIPHER_REG_CHAN0_CIPHER_IVIN(id)       (CIPHER_REG_BASE_ADDR + 0x1004 + (id)*0)
#define  CIPHER_REG_CHAN0_CIPHER_DIN(id)        (CIPHER_REG_BASE_ADDR + 0x1014 + (id)*0)
#define  CIPHER_REG_CHANn_IBUF_NUM(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 )
#define  CIPHER_REG_CHANn_IBUF_CNT(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x4 )
#define  CIPHER_REG_CHANn_IEMPTY_CNT(id)        (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x8 )
#define  CIPHER_REG_CHANn_INT_ICNTCFG(id)       (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0xC )
#define  CIPHER_REG_CHANn_CIPHER_CTRL(id)       (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x10)
#define  CIPHER_REG_CHANn_SRC_LST_SADDR(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x14)
#define  CIPHER_REG_CHANn_IAGE_TIMER(id)        (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x18)
#define  CIPHER_REG_CHANn_IAGE_CNT(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x1C)
#define  CIPHER_REG_CHANn_SRC_LST_RADDR(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x20)
#define  CIPHER_REG_CHANn_SRC_ADDR(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x24)
#define  CIPHER_REG_CHANn_SRC_LENGTH(id)        (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x28)
#define  CIPHER_REG_CHANn_IN_LEFT_BYTE0(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x2C)
#define  CIPHER_REG_CHANn_IN_LEFT_WORD0(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x30)
#define  CIPHER_REG_CHANn_IN_LEFT_WORD1(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x34)
#define  CIPHER_REG_CHANn_IN_LEFT_WORD2(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x38)
#define  CIPHER_REG_CHANn_OBUF_NUM(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x3C)
#define  CIPHER_REG_CHANn_OBUF_CNT(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x40)
#define  CIPHER_REG_CHANn_OFULL_CNT(id)         (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x44)
#define  CIPHER_REG_CHANn_INT_OCNTCFG(id)       (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x48)
#define  CIPHER_REG_CHANn_DEST_LST_SADDR(id)    (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x4C)
#define  CIPHER_REG_CHANn_OAGE_TIMER(id)        (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x50)
#define  CIPHER_REG_CHANn_OAGE_CNT(id)          (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x54)
#define  CIPHER_REG_CHANn_DEST_LST_RADDR(id)    (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x58)
#define  CIPHER_REG_CHANn_DEST_ADDR(id)         (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x5C)
#define  CIPHER_REG_CHANn_DEST_LENGTH(id)       (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x60)
#define  CIPHER_REG_CHANn_OUT_LEFT_BYTE(id)     (CIPHER_REG_BASE_ADDR + 0x1000 + (id)*128 + 0x64)
#define  CIPHER_REG_INT_STATUS                  (CIPHER_REG_BASE_ADDR + 0x1400 )
#define  CIPHER_REG_INT_EN                      (CIPHER_REG_BASE_ADDR + 0x1404 )
#define  CIPHER_REG_INT_RAW                     (CIPHER_REG_BASE_ADDR + 0x1408 )
#define  RST_STA                                (CIPHER_REG_BASE_ADDR + 0x140C )
#define  CIPHER_REG_CHAN0_CFG                   (CIPHER_REG_BASE_ADDR + 0x1410 )
#define  CIPHER_REG_CHANn_RD_DAT_ADDR_SMMU_BYPASS (CIPHER_REG_BASE_ADDR + 0x1418)
#define  CIPHER_REG_CHANn_WR_DAT_ADDR_SMMU_BYPASS (CIPHER_REG_BASE_ADDR + 0x141C)
#define  CIPHER_REG_BRAM_ST0                    (CIPHER_REG_BASE_ADDR + 0x1420 )
#define  CIPHER_REG_IN_ST0                      (CIPHER_REG_BASE_ADDR + 0x1424 )
#define  CIPHER_REG_IN_ST1                      (CIPHER_REG_BASE_ADDR + 0x1428 )
#define  CIPHER_REG_IN_ST2                      (CIPHER_REG_BASE_ADDR + 0x142C )
#define  CIPHER_REG_IN_ST3                      (CIPHER_REG_BASE_ADDR + 0x1430 )
#define  CIPHER_REG_OUT_ST0                     (CIPHER_REG_BASE_ADDR + 0x1434 )
#define  CIPHER_REG_OUT_ST1                     (CIPHER_REG_BASE_ADDR + 0x1438 )
#define  CIPHER_REG_OUT_ST2                     (CIPHER_REG_BASE_ADDR + 0x143C )

#define  CIPHER_MMU_BASE_ADDR                   (0xF99F0000) //IO_ADDRESS(0xF99F0000)
#define  CIPHER_MMU_GLOBAL_CTR_ADDR             (CIPHER_MMU_BASE_ADDR + 0x00)
#define  CIPHER_MMU_INTMAS_S                    (CIPHER_MMU_BASE_ADDR + 0x10)
#define  CIPHER_MMU_INTRAW_S                    (CIPHER_MMU_BASE_ADDR + 0x14)
#define  CIPHER_MMU_INTSTAT_S                   (CIPHER_MMU_BASE_ADDR + 0x18)
#define  CIPHER_MMU_INTCLR_S                    (CIPHER_MMU_BASE_ADDR + 0x1c) 
#define  CIPHER_MMU_INTMASK_NS                  (CIPHER_MMU_BASE_ADDR + 0x20)  
#define  CIPHER_MMU_INTRAW_NS                   (CIPHER_MMU_BASE_ADDR + 0x24) 
#define  CIPHER_MMU_INTSTAT_NS                  (CIPHER_MMU_BASE_ADDR + 0x28)
#define  CIPHER_MMU_INTCLR_NS                   (CIPHER_MMU_BASE_ADDR + 0x2C)
#define  CIPHER_MMU_SCB_TTBR                    (CIPHER_MMU_BASE_ADDR + 0x208) 
#define  CIPHER_MMU_CB_TTBR                     (CIPHER_MMU_BASE_ADDR + 0x20C)
#define  CIPHER_MMU_ERR_RDADDR_S                (CIPHER_MMU_BASE_ADDR + 0x2f0)
#define  CIPHER_MMU_ERR_WRADDR_S                (CIPHER_MMU_BASE_ADDR + 0x2f4)
#define  CIPHER_MMU_ERR_RDADDR_NS               (CIPHER_MMU_BASE_ADDR + 0x304)
#define  CIPHER_MMU_ERR_WRADDR_NS               (CIPHER_MMU_BASE_ADDR + 0x308)
#define  REG_SYS_MMU_CLK_ADDR_PHY		        (0XF8A22378)

#define  CIPHER_HASH_REG_BASE_ADDR_PHY          (0xF9A10000)
#define  CIPHER_HASH_REG_BASE_ADDR              CIPHER_HASH_REG_BASE_ADDR_PHY //IO_ADDRESS(CIPHER_HASH_REG_BASE_ADDR_PHY)
#define  CIPHER_HASH_REG_TOTALLEN_LOW_ADDR      (CIPHER_HASH_REG_BASE_ADDR + 0x00)
#define  CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR     (CIPHER_HASH_REG_BASE_ADDR + 0x04)
#define  CIPHER_HASH_REG_STATUS_ADDR            (CIPHER_HASH_REG_BASE_ADDR + 0x08)
#define  CIPHER_HASH_REG_CTRL_ADDR              (CIPHER_HASH_REG_BASE_ADDR + 0x0C)
#define  CIPHER_HASH_REG_START_ADDR             (CIPHER_HASH_REG_BASE_ADDR + 0x10)
#define  CIPHER_HASH_REG_DMA_START_ADDR         (CIPHER_HASH_REG_BASE_ADDR + 0x14)
#define  CIPHER_HASH_REG_DMA_LEN                (CIPHER_HASH_REG_BASE_ADDR + 0x18)
#define  CIPHER_HASH_REG_DATA_IN                (CIPHER_HASH_REG_BASE_ADDR + 0x1C)
#define  CIPHER_HASH_REG_REC_LEN1               (CIPHER_HASH_REG_BASE_ADDR + 0x20)
#define  CIPHER_HASH_REG_REC_LEN2               (CIPHER_HASH_REG_BASE_ADDR + 0x24)
#define  CIPHER_HASH_REG_SHA_OUT1               (CIPHER_HASH_REG_BASE_ADDR + 0x30)
#define  CIPHER_HASH_REG_SHA_OUT2               (CIPHER_HASH_REG_BASE_ADDR + 0x34)
#define  CIPHER_HASH_REG_SHA_OUT3               (CIPHER_HASH_REG_BASE_ADDR + 0x38)
#define  CIPHER_HASH_REG_SHA_OUT4               (CIPHER_HASH_REG_BASE_ADDR + 0x3C)
#define  CIPHER_HASH_REG_SHA_OUT5               (CIPHER_HASH_REG_BASE_ADDR + 0x40)
#define  CIPHER_HASH_REG_SHA_OUT6               (CIPHER_HASH_REG_BASE_ADDR + 0x44)
#define  CIPHER_HASH_REG_SHA_OUT7               (CIPHER_HASH_REG_BASE_ADDR + 0x48)
#define  CIPHER_HASH_REG_SHA_OUT8               (CIPHER_HASH_REG_BASE_ADDR + 0x4C)
#define  CIPHER_HASH_REG_MCU_KEY0               (CIPHER_HASH_REG_BASE_ADDR + 0x70)
#define  CIPHER_HASH_REG_MCU_KEY1               (CIPHER_HASH_REG_BASE_ADDR + 0x74)
#define  CIPHER_HASH_REG_MCU_KEY2               (CIPHER_HASH_REG_BASE_ADDR + 0x78)
#define  CIPHER_HASH_REG_MCU_KEY3               (CIPHER_HASH_REG_BASE_ADDR + 0x7C)
#define  CIPHER_HASH_REG_KL_KEY0                (CIPHER_HASH_REG_BASE_ADDR + 0x80)
#define  CIPHER_HASH_REG_KL_KEY1                (CIPHER_HASH_REG_BASE_ADDR + 0x84)
#define  CIPHER_HASH_REG_KL_KEY2                (CIPHER_HASH_REG_BASE_ADDR + 0x88)
#define  CIPHER_HASH_REG_KL_KEY3                (CIPHER_HASH_REG_BASE_ADDR + 0x8C)
#define  CIPHER_HASH_REG_INIT1_UPDATE           (CIPHER_HASH_REG_BASE_ADDR + 0x90) 

/* STB KEY CTRL */
#define  CIPHER_REG_CA_CONFIG_STATE             0xF8A90000 //IO_ADDRESS(0xF8A90000)
#define  CIPHER_REG_STB_KEY_CTRL                0xF8A90034 //IO_ADDRESS(0xF8A90034)
/*Porting to TrustedCore STB begin*/
//vir=phy
#define CIPHER_CA_STATE_ADDR 0xF8A90038
/*Porting to TrustedCore STB end*/
#define  CIPHER_REG_CA_STATE                    CIPHER_CA_STATE_ADDR //IO_ADDRESS(CIPHER_CA_STATE_ADDR)      /*0xF8A90038*/


#define  CIPHER_RSA_REG_BASE_ADDR_PHY          (0xF9A30000)

#define CIPHER_RSA_REG_BASE_RSA		            (CIPHER_RSA_REG_BASE_ADDR_PHY)
#define SEC_RSA_BUSY_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x50)
#define SEC_RSA_MOD_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x54)
#define SEC_RSA_WSEC_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x58)
#define SEC_RSA_WDAT_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x5c)
#define SEC_RSA_RPKT_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x60)
#define SEC_RSA_RRSLT_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x64)
#define SEC_RSA_START_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x68)
#define SEC_RSA_ADDR_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x6C)
#define SEC_RSA_ERROR_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x70)
#define SEC_RSA_CRC16_REG 	                    (CIPHER_RSA_REG_BASE_RSA + 0x74)
#define REG_SYS_RSA_CLK_ADDR_PHY		        (0XF8A22384)

/* Define the union U_PERI_CRG49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sha_cken              : 1   ; /* [0]  */
        unsigned int    reserved_1            : 3   ; /* [3..1]  */
        unsigned int    sha_srst_req          : 1   ; /* [4]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_PERI_CRG49;

#ifdef CHIP_TYPE_hi3716cv200es
typedef union
{
    struct
    {
        HI_U32 ca_kl_bus_cken	    :1; //[0]
        HI_U32 ca_ci_bus_cken	    :1; //[1]
        HI_U32 ca_ci_cken	        :1; //[2]
        HI_U32 otp_bus_cken	        :1; //[3]
        HI_U32 reserved0	        :4; //[7:4]
        HI_U32 ca_kl_srst_req	    :1; //[8]
        HI_U32 ca_ci_srst_req	    :1; //[9]
        HI_U32 otp_srst_req	        :1; //[10]
        HI_U32 reserved1	        :1; //[11]
        HI_U32 ca_clk_sel	        :1; //[12]
        HI_U32 reserved2        	:1; //[13~31]
    }bits;
    HI_U32 u32;
}CIPHER_CA_SYS_CLK_U;       //0xc0
#else
typedef union
{
    struct
    {
        HI_U32 reserved0        :8; //[0~7]
        HI_U32 ca_kl_srst_req	:1; //[8]
        HI_U32 ca_ci_srst_req	:1; //[9]
        HI_U32 otp_srst_req	    :1; //[10]
        HI_U32 reserved1	    :4; //[11]
        HI_U32 ca_ci_clk_sel	:1; //[12]
        HI_U32 reserved2	    :1; //[13~31]
    }bits;
    HI_U32 u32;
}CIPHER_CA_SYS_CLK_U;       //0xc0
#endif

typedef union
{
    struct 
    {
        HI_U32 key_addr       : 8;  //[7:0]
        HI_U32 reserved       : 24; //[31:8]
    }bits;
    HI_U32 u32;
}CIPHER_CA_STB_KEY_CTRL_U;

typedef union
{
    struct 
    {
        HI_U32 st_vld         : 1;  //[0]
        HI_U32 reserved       : 31; //[31:1]
    }bits;
    HI_U32 u32;
}CIPHER_CA_CONFIG_STATE_U;


typedef union
{
    struct
    {
        HI_U32 hash_rdy             : 1;    // [0]
        HI_U32 dma_rdy              : 1;    // [1]
        HI_U32 msg_rdy              : 1;    // [2]
        HI_U32 rec_rdy              : 1;    // [3]
        HI_U32 error_state          : 3;    // [6:4]
        HI_U32 reserved             : 25;   // [31:7]
    } bits;
    HI_U32 u32;
}CIPHER_SHA_STATUS_U;

typedef union
{
    struct
    {
        HI_U32 read_ctrl            : 1;    // [0]
        HI_U32 sha_sel              : 2;    // [2:1]
        HI_U32 hardkey_hmac_flag    : 1;    // [3]
        HI_U32 hardkey_sel          : 1;    // [4]
        HI_U32 small_end_en         : 1;    // [5]
        HI_U32 sha_init_update_en   : 1;    // [6]
        HI_U32 usedbyarm            : 1;    // [7]
        HI_U32 reserved2            : 24;   // [31:8]
    } bits;
    HI_U32 u32;
}CIPHER_SHA_CTRL_U;

typedef union
{
    struct
    {
        HI_U32 sha_start            : 1;    // [0]
        HI_U32 reserved2            : 31;   // [31:1]
    } bits;
    HI_U32 u32;
}CIPHER_SHA_START_U;

typedef union
{
    struct
    {
        HI_U32 sha_cken            : 1;    // [0]
        HI_U32 reserved            : 3;    // [3:1]
        HI_U32 sha_srst_req        : 1;    // [4]
        HI_U32 reserved2           : 27;   // [31:5]
    } bits;
    HI_U32 u32;
}CIPHER_SHA_RST_U;


/* RNG REGISTERS AND STRUCTURES */
#define  REG_RNG_BASE_ADDR_PHY 					(0xF8005000)
/*Porting to TrustedCore STB begin*/
//vir=phy
#if defined (CHIP_TYPE_hi3798cv200) || defined (CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#define RNG_BASE_ADDR REG_RNG_BASE_ADDR_PHY
#define HISEC_COM_TRNG_CTRL				(REG_RNG_BASE_ADDR_PHY + 0x200)
#define HISEC_COM_TRNG_FIFO_DATA	    (REG_RNG_BASE_ADDR_PHY + 0x204)
#define HISEC_COM_TRNG_DATA_ST  		(REG_RNG_BASE_ADDR_PHY + 0x208)
#else
#define  REG_RNG_BASE_ADDR 					REG_RNG_BASE_ADDR_PHY
#define  REG_RNG_NUMBER_ADDR				(REG_RNG_BASE_ADDR + 0x4)
#define  REG_RNG_STAT_ADDR					(REG_RNG_BASE_ADDR + 0x8)
#endif

typedef union
{
    struct
    {
        HI_U32 hdcp_mode_en         : 1;    // [0]
        HI_U32 tx_read              : 1;    // [1]
        HI_U32 hdcp_rootkey_sel     : 2;    // [3:2]
        HI_U32 rx_read              : 1;    // [4]
        HI_U32 rx_sel               : 1;    // [5]
        HI_U32 reserved             : 26;   // [31:6]
    } bits;
    HI_U32 u32;
} CIPHER_HDCP_MODE_CTRL_U;  //Offset:0x820



#ifdef __cplusplus
}
#endif
#endif /* end #ifndef __DRV_CIPHER_REG_H__ */
