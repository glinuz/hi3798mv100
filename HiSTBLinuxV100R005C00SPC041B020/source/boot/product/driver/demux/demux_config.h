
#ifndef __DEMUX_CONFIG_H__
#define __DEMUX_CONFIG_H__

#if defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400)

#define HI_PVR_BASE             0xF9C00000

#define HI_DEMUX_COUNT          7
#define HI_DMX_IFPORT_CNT                  1
#define HI_DMX_TSIPORT_CNT                 4
#define HI_TUNER_PORT_COUNT     (HI_DMX_IFPORT_CNT + HI_DMX_TSIPORT_CNT)
#define HI_OQ_COUNT             128
#define HI_CHANNEL_COUNT        96
#define HI_FILTER_COUNT         96

#elif  defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)   \
    || defined(CHIP_TYPE_hi3718mv100)   \
    || defined(CHIP_TYPE_hi3719mv100)  

#define HI_PVR_BASE             0xF9C00000

#define HI_DEMUX_COUNT          7
#define HI_DMX_IFPORT_CNT                  1
#define HI_DMX_TSIPORT_CNT                 1
#define HI_TUNER_PORT_COUNT      (HI_DMX_IFPORT_CNT + HI_DMX_TSIPORT_CNT)
#define HI_OQ_COUNT             128
#define HI_CHANNEL_COUNT        96
#define HI_FILTER_COUNT         96

#elif  defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
#define HI_PVR_BASE             0xF9C00000

#define HI_DEMUX_COUNT          7
#define HI_DMX_IFPORT_CNT                  1
#if defined(CHIP_TYPE_hi3716mv410)
#define HI_DMX_TSIPORT_CNT                 4
#else
#define HI_DMX_TSIPORT_CNT                 6
#endif
#define HI_TUNER_PORT_COUNT     (HI_DMX_IFPORT_CNT + HI_DMX_TSIPORT_CNT)
#define HI_OQ_COUNT             128
#define HI_CHANNEL_COUNT        96
#define HI_FILTER_COUNT         96

#else

#error "Unknown chip type"

#endif

#endif  // __DEMUX_CONFIG_H__

