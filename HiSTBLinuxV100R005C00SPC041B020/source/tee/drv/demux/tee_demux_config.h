#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
/*
 * these chips support tee drv has some risk.
 * such as it maybe operating same 0x3B80(or 0x3B84) registers at both secure and nonsecure side of same time.
 * so restrict its use as much as possible.
 */
#define DMX_SET_BASE                    0xF9C00000

#define DMX_CNT                         7
#define DMX_REC_CNT                     7
#define DMX_RAMPORT_CNT                 3
#define DMX_CHANNEL_CNT                 96
#define DMX_OQ_CNT                      128

#define DMX_KEY_CNT                     32
#define DMX_SEC_KEY_OFFSET              16   /* fake secure key. */

#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#elif defined(CHIP_TYPE_hi3798cv200)
#define DMX_SET_BASE                    0xF9C00000

#define DMX_MMU_VERSION_1
#define DMX_MMU_BASE                    0xF9BF0000

#define DMX_CNT                         7
#define DMX_REC_CNT                     32
#define DMX_RAMPORT_CNT                 4
#define DMX_CHANNEL_CNT                 96
#define DMX_OQ_CNT                      128

#define DMX_KEY_CNT                     32
#define DMX_SEC_KEY_OFFSET              16

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT

#elif defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#define DMX_SET_BASE                    0xF9C00000

#define DMX_MMU_VERSION_2
#define DMX_MMU_BASE                    DMX_SET_BASE

#define DMX_CNT                         5
#define DMX_REC_CNT                     5
#define DMX_RAMPORT_CNT                 3
#define DMX_CHANNEL_CNT                 96
#define DMX_OQ_CNT                      128

#define DMX_KEY_CNT                     16
#define DMX_SEC_KEY_OFFSET              8

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT
#define DMX_SECURE_ENCRYPT_KEY_SUPPORT
#define DMX_SECURE_RAM_PORT_SUPPORT     /* support secure play and encrypt record. */

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT

#elif defined(CHIP_TYPE_hi3796mv200)
#define DMX_SET_BASE                    0xF9C00000

#define DMX_MMU_VERSION_2
#define DMX_MMU_BASE                    DMX_SET_BASE

#define DMX_CNT                         7
#define DMX_REC_CNT                     32
#define DMX_RAMPORT_CNT                 7
#define DMX_CHANNEL_CNT                 96
#define DMX_OQ_CNT                      128

#define DMX_KEY_CNT                     32
#define DMX_SEC_KEY_OFFSET              16

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT
#define DMX_SECURE_ENCRYPT_KEY_SUPPORT
#define DMX_SECURE_RAM_PORT_SUPPORT     /* support secure play and encrypt record. */

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT

#endif
