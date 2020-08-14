#ifndef __VPSS_COMMON_H__
#define __VPSS_COMMON_H__

#include"hi_drv_mem.h"
#include"hi_drv_log.h"
#include"hi_drv_sys.h"
#include"hi_drv_stat.h"
#include"hi_drv_video.h"
#include"hi_drv_vpss.h"
#include "hi_debug.h"
#include <linux/list.h>
#include <linux/io.h>
#include <linux/delay.h>
#include"drv_vdec_ext.h"

#define DEF_FILE_NAMELENGTH 40
#define VPSS_ISLOSS_CMP HI_FALSE

typedef struct list_head LIST;

#define ZME_2L_TEST   //@f00241306 
#define FB_DBG 0
#define DEF_VPSS_STATIC 0
#define DEF_VPSS_DEBUG 1
#define DEF_VPSS_HIGH_4K 1

#define VPSS_NAME  "HI_VPSS"

#define VPSS_KMALLOC(fmt...)      HI_KMALLOC  (HI_ID_VPSS, fmt)
#define VPSS_KFREE(fmt...)        HI_KFREE    (HI_ID_VPSS, fmt)
#define VPSS_VMALLOC(fmt...)       HI_VMALLOC   (HI_ID_VPSS, fmt)
#define VPSS_VFREE(fmt...)       HI_VFREE   (HI_ID_VPSS, fmt)

#define VPSS_FATAL(fmt...) \
            HI_FATAL_PRINT(HI_ID_VPSS, fmt)

#define VPSS_ERROR(fmt...) \
            HI_ERR_PRINT(HI_ID_VPSS, fmt)

#define VPSS_WARN(fmt...) \
            HI_WARN_PRINT(HI_ID_VPSS, fmt)

#define VPSS_INFO(fmt...) \
            HI_INFO_PRINT(HI_ID_VPSS, fmt)
#define VPSS_DBG() \
do{\
    HI_PRINT("--->%s %d\n",__func__,__LINE__);\
}while(0)

#define VPSS_CHECK_NULL(ptr)\
do\
{\
    if (HI_NULL == ptr)\
    {\
        VPSS_ERROR("Para is Null\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define VPSS_CHECK_HANDLE(handle)\
do\
{\
    if (HI_INVALID_HANDLE == handle)\
    {\
        VPSS_ERROR("handle is HI_INVALID_HANDLE\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define VPSS_CHECK_VERSION(version,exp)\
do\
{\
    if (version != exp)\
    {\
        VPSS_ERROR("Invalid Version %d.Expect %d\n",version,exp);\
        return HI_FAILURE;\
    }\
}while(0)


#define VPSS_GET_TIME(time)\
do\
{\
    HI_U32 u32Tmp;\
    if(!HI_DRV_SYS_GetTimeStampMs(&u32Tmp))\
    {\
        time = u32Tmp;\
        VPSS_ERROR("Get Time %d \n",time);\
    }\
    else\
    {\
        time = 0;\
        VPSS_ERROR("Get Time Failed\n");\
    }\
}while(0)
#if 1
#define VPSS_DBG_INFO(fmt...)
#else
#define VPSS_DBG_INFO(fmt...) VPSS_FATAL(fmt);
#endif
typedef enum hiVPSS_VERSION_E
{
    VPSS_VERSION_V1_0 = 0,
    VPSS_VERSION_V2_0 ,
    VPSS_VERSION_V3_0 ,
    VPSS_VERSION_BUTT 
}VPSS_VERSION_E;

typedef union{
    // Define the struct bits
    struct
    {
        unsigned int    hi3798mv100          : 1   ; 
        unsigned int    hi3716mv410          : 1   ;  
        unsigned int    hi3798cv200_a        : 1   ;  
        unsigned int    reserve_2            : 1   ; 
        unsigned int    reserve_3            : 28  ;
        
    } bits;
    // Define an unsigned member
    unsigned int    u32;

}VPSS_CAPABILITY_U;


#define VPSS_NTSC_WIDTH  720 
#define VPSS_NTSC_HEIGHT 480
#define VPSS_PAL_WIDTH 720
#define VPSS_PAL_HEIGHT 576
#define VPSS_HD_WIDTH  1280
#define VPSS_HD_HEIGHT 720
#define VPSS_FHD_WIDTH 1920
#define VPSS_FHD_HEIGHT 1080
#define VPSS_UHD_WIDTH  4096
#define VPSS_UHD_HEIGHT 2304

#define VPSS_HEIGHT_ALIGN 0xfffffffc
#define VPSS_WIDTH_ALIGN 0xfffffffe

#define VPSS_FRAME_MIN_WIDTH            64
#define VPSS_FRAME_MIN_HEIGHT           64
#if defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3716mv410)
#define VPSS_FRAME_MAX_WIDTH            1920
#define VPSS_FRAME_MAX_HEIGHT           1088
#else
#define VPSS_FRAME_MAX_WIDTH            4096
#define VPSS_FRAME_MAX_HEIGHT           2304
#endif


#define VPSS_SYS_MEM_MIN			   512

#define VPSS_UHD_LOW_W			1920
#define VPSS_UHD_LOW_H			1080

#define VPSS_UHD_MIDDLE_W		1920
#define VPSS_UHD_MIDDLE_H		2160

#if DEF_VPSS_HIGH_4K
#define VPSS_UHD_HIGH_W			3840
#define VPSS_UHD_HIGH_H			2160
#else
#define VPSS_UHD_HIGH_W			1920
#define VPSS_UHD_HIGH_H			2160
#endif

#endif
