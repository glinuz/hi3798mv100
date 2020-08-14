
#ifndef __DRV_REG_PROC_H__
#define __DRV_REG_PROC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

//HI3716Cv200 Series
#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)   \
    || defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3716mv410)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3716mv310)

#define HDMI_TX_BASE_ADDR     0xf8ce0000L
#define     HDMI_TX_SLV0_ADDR_BASE 0xf8ce0000L
#define     HDMI_TX_SLV1_ADDR_BASE 0xf8ce0400L
#define     HDMI_TX_CEC_ADDR_BASE  0xf8ce0800L

#define HDMI_TX_BASE_ADDR     0xf8ce0000L
#define     HDMI_TX_SLV0_ADDR_BASE 0xf8ce0000L
#define     HDMI_TX_SLV1_ADDR_BASE 0xf8ce0400L
#define     HDMI_TX_CEC_ADDR_BASE  0xf8ce0800L

#define HDMI_TX_PHY_ADDR      0xf8ce1800L

#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)

#define HDMI_TX_BASE_ADDR     0xff100000L
#define     HDMI_TX_SLV0_ADDR_BASE 0xff100000L
#define     HDMI_TX_SLV1_ADDR_BASE 0xff100400L
#define     HDMI_TX_CEC_ADDR_BASE  0xf8ce0800L

#define HDMI_TX_PHY_ADDR      0xff101800L

#endif


#define HDMI_REG_WRITE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define HDMI_REG_READ(addr) (*(volatile unsigned int *)(addr))




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_REG_PROC_H__ */

