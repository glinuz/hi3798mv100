
#ifndef __DRV_DEMUX_CONFIG_H__
#define __DRV_DEMUX_CONFIG_H__

/*=============================================CHIP_TYPE_hi3716cv200=========================================*/
#if defined(CHIP_TYPE_hi3716cv200) || defined (CHIP_TYPE_hi3716mv400)

#define DMX_CLUSTER_TOTAL_SET_CNT       1

#define DMX_SET_1                       1
#define DMX_SET_1_INT_NUM               (82 + 32)
#define DMX_SET_1_BASE                  0xF9C00000

/* single dmx set configs */
#define DMX_CNT                         7

#define DMX_REC_CNT                     DMX_CNT
#define DMX_1WAY_REC_CNT                1

#define DMX_IFPORT_CNT                  1
#define DMX_TSIPORT_CNT                 4
#define DMX_TSOPORT_CNT                 2 /*两路串行或者一路并行*/
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT                 5
#define DMX_TAGPORT_CNT                 0  /* 4路tag deal输出端口*/

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#define DMX_KEY_CNT                     32

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

#define DMX_SCD_CNT                     8
#define DMX_SCD_BYTE_FILTER_CNT         10
#define DMX_SCD_RANGE_FILTER_CNT        7

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         (30)

#define DMX_SUPPORT_RAM_CLK_AUTO_CTL

/*=============================================CHIP_TYPE_hi3716mv410=========================================*/
#elif defined (CHIP_TYPE_hi3716mv410) || defined (CHIP_TYPE_hi3716mv420)

#define DMX_CLUSTER_TOTAL_SET_CNT       1

#define DMX_SET_1                       1
#define DMX_SET_1_INT_NUM               (82 + 32)
#define DMX_SET_1_BASE                  0xF9C00000

/* single dmx set configs */
#define DMX_CNT                         7

#define DMX_REC_CNT                     32
#define DMX_1WAY_REC_CNT                8

#define DMX_IFPORT_CNT                  1
#if      defined(CHIP_TYPE_hi3716mv410)
#define DMX_TSIPORT_CNT                 4
#define DMX_TSOPORT_CNT                 1
#elif   defined(CHIP_TYPE_hi3716mv420)
#define DMX_TSIPORT_CNT                 6
#define DMX_TSOPORT_CNT                 2
#endif
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT                 4
#define DMX_TAGPORT_CNT                 0

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#define DMX_KEY_CNT                     32

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

#define DMX_SCD_VERSION_2               20141022   /* PVR_VERSION : 0A00 */
#define DMX_SCD_CNT                     32
#define DMX_SCD_BYTE_FILTER_CNT         16
#define DMX_SCD_RANGE_FILTER_CNT        24

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         (30)

#define DMX_SUPPORT_RAM_CLK_AUTO_CTL

#define DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE

/*==============================CHIP_TYPE_hi3719cv100,CHIP_TYPE_hi3719mv100==============================*/
#elif defined(CHIP_TYPE_hi3719cv100) || defined (CHIP_TYPE_hi3719mv100)

#define DMX_INT_NUM                     (82 + 32)

#define HI_PVR_BASE                    (0xF9C00000)

#define DMX_CNT                         7

#define DMX_REC_CNT                     DMX_CNT

#ifdef CHIP_TYPE_hi3719cv100
    #define DMX_IFPORT_CNT               1
	#define DMX_TSIPORT_CNT              4
	#define DMX_RAMPORT_CNT              5
#elif defined (CHIP_TYPE_hi3719mv100)
    #define DMX_IFPORT_CNT               0
	#define DMX_TSIPORT_CNT              2
	#define DMX_RAMPORT_CNT              6
#endif

#define DMX_TSOPORT_CNT                0

#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#define DMX_KEY_CNT                     32

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

/*0xf9c03a04 , switch :
0x00： 关闭TS源；
0x01～0x07：分别选择DVB下的TS1～TS7端口码流；
0x10～0x15：分别选择IP下的IP0～IP5内部端口码流。*/
#define DMX_RAM_PORT_OFFSET             15


#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_VERSION_1

#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         (30)

/*=================================CHIP_TYPE_hi3718cv100,CHIP_TYPE_hi3718mv100==================================*/
#elif defined(CHIP_TYPE_hi3718cv100)  || defined (CHIP_TYPE_hi3718mv100) /*not support descrambler*/

#define DMX_INT_NUM                     (82 + 32)

#define HI_PVR_BASE                     (0xF9C00000)

#define DMX_CNT                         7

#define DMX_REC_CNT                     DMX_CNT

#ifdef CHIP_TYPE_hi3718cv100
    #define DMX_IFPORT_CNT               0
	#define DMX_TSIPORT_CNT              0
	#define DMX_RAMPORT_CNT              5
#elif defined (CHIP_TYPE_hi3718mv100)
    #define DMX_IFPORT_CNT               0
	#define DMX_TSIPORT_CNT              0
	#define DMX_RAMPORT_CNT              6
#endif
#define DMX_TSOPORT_CNT                0
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#define DMX_KEY_CNT                     32

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

/*0xf9c03a04 , switch :
0x00： 关闭TS源；
0x01～0x07：分别选择DVB下的TS1～TS7端口码流；
0x10～0x15：分别选择IP下的IP0～IP5内部端口码流。*/
#define DMX_RAM_PORT_OFFSET             15


#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_VERSION_1


#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         (30)
/*=============================================CHIP_TYPE_hi3796cv100/CHIP_TYPE_hi3798cv100=========================================*/
#elif defined(CHIP_TYPE_hi3796cv100) || defined(CHIP_TYPE_hi3798cv100)

#define DMX_INT_NUM                     (82 + 32)/*A9一共有128个中断，A9自己占用32个*/

#define HI_PVR_BASE                     (0xF9C00000)

#define DMX_CNT                         7

#define DMX_REC_CNT                     DMX_CNT

#define DMX_IFPORT_CNT                  2
#define DMX_TSIPORT_CNT                 6
#define DMX_TSOPORT_CNT                 2 /*两路串行或者一路并行*/
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT                 6

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#define DMX_KEY_CNT                     32

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

#define DMX_RAM_PORT_OFFSET             15

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_VERSION_1

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         (30)

/*=============================================CHIP_TYPE_hi3798mv100/CHIP_TYPE_hi3796mv100=========================================*/
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
#define DMX_CLUSTER_TOTAL_SET_CNT       1

#define DMX_SET_1                       1
#define DMX_SET_1_INT_NUM               (32 + 82)
#define DMX_SET_1_BASE                  0xF9C00000

#define DMX_CNT                         7

#define DMX_REC_CNT                     DMX_CNT
#define DMX_1WAY_REC_CNT                1

#define DMX_IFPORT_CNT                  0
#define DMX_TSIPORT_CNT                 2
#define DMX_TSOPORT_CNT                 0
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT                 3
#define DMX_TAGPORT_CNT                 0

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#ifdef DMX_TEE_SUPPORT
#define DMX_KEY_CNT                     16
#else
#define DMX_KEY_CNT                     32
#endif

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

#define DMX_SCD_CNT                     8
#define DMX_SCD_BYTE_FILTER_CNT         10
#define DMX_SCD_RANGE_FILTER_CNT        7

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_SPE_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         30

#define DMX_SUPPORT_RAM_CLK_AUTO_CTL

#define DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE

/*=============================================CHIP_TYPE_hi3798cv200=========================================*/
#elif defined(CHIP_TYPE_hi3798cv200)
#define DMX_CLUSTER_TOTAL_SET_CNT       1

#define DMX_MMU_VERSION_1
#define DMX_MMU_BASE                    0xF9BF0000

#define DMX_SET_1                       1
#define DMX_SET_1_INT_NUM               (32 + 82)
#define DMX_SET_1_BASE                  0xF9C00000

#define DMX_CNT                         7

#define DMX_REC_CNT                     32
#define DMX_1WAY_REC_CNT                8

#define DMX_IFPORT_CNT                  0
#define DMX_TSIPORT_CNT                 6
#define DMX_TSOPORT_CNT                 2
#define DMX_TUNERPORT_CNT               (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT                 4
#define DMX_TAGPORT_CNT                 0

#define DMX_CHANNEL_CNT                 96
#define DMX_AV_CHANNEL_CNT              32
#define DMX_PCR_CHANNEL_CNT             16

#define DMX_FILTER_CNT                  96

#ifdef DMX_TEE_SUPPORT
#define DMX_KEY_CNT                     16
#else
#define DMX_KEY_CNT                     32
#endif

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT

#define DMX_FQ_CNT                      40
#define DMX_OQ_CNT                      128

#define RMX_SUPPORT
#define RMX_CHAN_CNT                    1
#define RMX_PUMP_CNT                    64
#define RMX_PORT_CNT                    4

#define DMX_SCD_VERSION_2               20141022   /* PVR_VERSION : 0A00 */
#define DMX_SCD_CNT                     32
#define DMX_SCD_BYTE_FILTER_CNT         16
#define DMX_SCD_RANGE_FILTER_CNT        24

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

#define DMX_REC_EXCLUDE_PID_SUPPORT
#define DMX_REC_EXCLUDE_PID_NUM         30

#define DMX_SUPPORT_RAM_CLK_AUTO_CTL

#define DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE

#define DMX_DUP_PID_CHANNEL_SUPPORT

/*=============================================CHIP_TYPE_hi3798mv200=========================================*/
#elif defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#define DMX_CLUSTER_TOTAL_SET_CNT    1

#define DMX_MMU_VERSION_2
#define DMX_MMU_BASE                 0xF9C00000

#define DMX_SET_1                    1
#define DMX_SET_1_INT_NUM            (32 + 82)
#define DMX_SET_1_BASE               0xF9C00000

#define DMX_CNT                      5

#define DMX_REC_CNT                  DMX_CNT
#define DMX_1WAY_REC_CNT             1

#define DMX_IFPORT_CNT               0
#define DMX_TSIPORT_CNT              4
#define DMX_TSOPORT_CNT              1
#define DMX_TUNERPORT_CNT            (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT              3
#define DMX_TAGPORT_CNT              0

#define DMX_CHANNEL_CNT              96
#define DMX_AV_CHANNEL_CNT           32
#define DMX_PCR_CHANNEL_CNT          4

#define DMX_FILTER_CNT               64

#ifdef DMX_TEE_SUPPORT
#define DMX_KEY_CNT                  8
#else
#define DMX_KEY_CNT                  16
#endif

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT
#define DMX_SECURE_RAM_PORT_SUPPORT

#define DMX_FQ_CNT                   40
#define DMX_OQ_CNT                   128

#define DMX_SCD_VERSION_2            20141022  /* PVR_VERSION : 0A00 */
#define DMX_SCD_CNT                  32
#define DMX_SCD_BYTE_FILTER_CNT      16
#define DMX_SCD_RANGE_FILTER_CNT     24

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

/* faga always close filtered pulse and ansi open it */
/* 98mv200 didn't use Filtered pulse */ 
//#define DMX_SUPPORT_RAM_CLK_AUTO_CTL
//#define DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE

#define DMX_DUP_PID_CHANNEL_SUPPORT

/*=============================================CHIP_TYPE_hi3796mv200=========================================*/
#elif defined(CHIP_TYPE_hi3796mv200)
#define DMX_CLUSTER_TOTAL_SET_CNT     1

#define DMX_MMU_VERSION_2
#define DMX_MMU_BASE                 0xF9C00000

#define DMX_SET_1                     1
#define DMX_SET_1_INT_NUM            (32 + 82)
#define DMX_SET_1_BASE               0xF9C00000

#define DMX_CNT                       7

#define DMX_REC_CNT                  32
#define DMX_1WAY_REC_CNT              8

#define DMX_IFPORT_CNT                1
#define DMX_TSIPORT_CNT               8
#define DMX_TSOPORT_CNT               2
#define DMX_TUNERPORT_CNT            (DMX_IFPORT_CNT + DMX_TSIPORT_CNT)
#define DMX_RAMPORT_CNT               7
#define DMX_TAGPORT_CNT               0

#define DMX_CHANNEL_CNT              96
#define DMX_AV_CHANNEL_CNT           32
#define DMX_PCR_CHANNEL_CNT           4

#define DMX_FILTER_CNT               96

#ifdef DMX_TEE_SUPPORT
#define DMX_KEY_CNT                  16
#else
#define DMX_KEY_CNT                  32
#endif

#define DMX_SECURE_CHANNEL_SUPPORT
#define DMX_SECURE_KEY_SUPPORT
#define DMX_SECURE_RAM_PORT_SUPPORT

#define DMX_FQ_CNT                   40
#define DMX_OQ_CNT                  128

#define RMX_SUPPORT
#define RMX_CHAN_CNT                  1
#define RMX_PUMP_CNT                 64
#define RMX_PORT_CNT                  4

#define DMX_SCD_VERSION_2            20141022  /* PVR_VERSION : 0A00 */
#define DMX_SCD_CNT                  32
#define DMX_SCD_BYTE_FILTER_CNT      16
#define DMX_SCD_RANGE_FILTER_CNT     24

#define DMX_SCD_NEW_FLT_SUPPORT

#define DMX_DESCRAMBLER_TYPE_CSA3_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SPE_V2_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_SUPPORT
#define DMX_DESCRAMBLER_TYPE_AES_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_NS_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_ECB_SUPPORT
#define DMX_DESCRAMBLER_TYPE_SMS4_CBC_SUPPORT
#define DMX_DESCRAMBLER_TYPE_DES_IPTV_SUPPORT
#define DMX_DESCRAMBLER_TYPE_TDES_SUPPORT

#define DMX_TUNER_PORT_SERIAL_2BIT_AND_SERIAL_NOSYNC_SUPPORT

#define DMX_RAM_PORT_SET_LENGTH_SUPPORT
#define DMX_RAM_PORT_AUTO_SCAN_SUPPORT

#define DMX_SECTION_PARSE_NOPUSI_SUPPORT

#define DMX_FILTER_DEPTH_SUPPORT

/* faga always close filtered pulse and ansi open it */
//#define DMX_SUPPORT_RAM_CLK_AUTO_CTL
//#define DMX_SUPPORT_DMX_CLK_DYNAMIC_TUNE

#define DMX_DUP_PID_CHANNEL_SUPPORT
#else

#error must define CHIP_TYPE

#endif

#endif  // __DRV_DEMUX_CONFIG_H__

