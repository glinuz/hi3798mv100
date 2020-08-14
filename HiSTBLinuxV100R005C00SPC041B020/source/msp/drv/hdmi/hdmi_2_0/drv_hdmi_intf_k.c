#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/delay.h>

#include "hdmi_hal.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_platform.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_ioctl.h"
#include "hdmi_platform.h"


#if defined(CONFIG_HDMI_STB_SDK)
#include "drv_hdmi_ext.h"
#include "hi_drv_hdmi.h"
#include "hi_unf_disp.h"
#include "hi_drv_pdm.h"

#define HDMI_USAMPLERATE_2_KSAMPLERATE(usamplerate, ksamplerate) \
do{\
    if (HI_UNF_SAMPLE_RATE_UNKNOWN == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_UNKNOWN;\
    } \
    else if (HI_UNF_SAMPLE_RATE_8K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_8K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_11K == usamplerate) \
    { \
        ksamplerate = HDMI_SAMPLE_RATE_11K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_12K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_12K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_16K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_16K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_22K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_22K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_24K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_24K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_32K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_32K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_44K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_44K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_48K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_48K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_88K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_88K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_96K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_96K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_176K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_176K;\
    }\
    else if (HI_UNF_SAMPLE_RATE_192K == usamplerate)\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_192K;\
    }\
    else\
    {\
        ksamplerate = HDMI_SAMPLE_RATE_UNKNOWN;\
    }\
}while(0)

#define HDMI_KSAMPLERATE_2_USAMPLERATE(ksamplerate, usamplerate) \
do{\
    if (HDMI_SAMPLE_RATE_UNKNOWN == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_UNKNOWN;\
    } \
    else if (HDMI_SAMPLE_RATE_8K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_8K;\
    }\
    else if (HDMI_SAMPLE_RATE_11K == ksamplerate) \
    { \
        usamplerate = HI_UNF_SAMPLE_RATE_11K;\
    }\
    else if (HDMI_SAMPLE_RATE_12K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_12K;\
    }\
    else if (HDMI_SAMPLE_RATE_16K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_16K;\
    }\
    else if (HDMI_SAMPLE_RATE_22K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_22K;\
    }\
    else if (HDMI_SAMPLE_RATE_24K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_24K;\
    }\
    else if (HDMI_SAMPLE_RATE_32K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_32K;\
    }\
    else if (HDMI_SAMPLE_RATE_44K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_44K;\
    }\
    else if (HDMI_SAMPLE_RATE_48K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_48K;\
    }\
    else if (HDMI_SAMPLE_RATE_88K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_88K;\
    }\
    else if (HDMI_SAMPLE_RATE_96K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_96K;\
    }\
    else if (HDMI_SAMPLE_RATE_176K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_176K;\
    }\
    else if (HDMI_SAMPLE_RATE_192K == ksamplerate)\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_192K;\
    }\
    else\
    {\
        usamplerate = HI_UNF_SAMPLE_RATE_BUTT;\
    }\
}while(0)

#define HDMI_UEXTCOLORIMETRY_2_KEXTCOLORIMETRY(uextcolorimetry, kextcolorimetry) \
do{\
    if (HDMI_COLORIMETRY_XVYCC_601 == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;\
    } \
    else if (HDMI_COLORIMETRY_XVYCC_709 == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_XV_YCC_709;\
    }\
    else if (HDMI_COLORIMETRY_S_YCC_601 == uextcolorimetry) \
    { \
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_S_YCC_601;\
    }\
    else if (HDMI_COLORIMETRY_ADOBE_YCC_601 == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601;\
    }\
    else if (HDMI_COLORIMETRY_ADOBE_RGB == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB;\
    }\
    else if (HDMI_COLORIMETRY_2020_CONST_LUMINOUS == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_2020_const_luminous;\
    }\
    else if (HDMI_COLORIMETRY_2020_NON_CONST_LUMINOUS == uextcolorimetry)\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous;\
    }\
    else\
    {\
        kextcolorimetry = HDMI_EXTENDED_COLORIMETRY_BUTT;\
    }\
}while(0)

#define HDMI_KEXTCOLORIMETRY_2_UEXTCOLORIMETRY(kextcolorimetry, uextcolorimetry) \
do{\
    if (HDMI_EXTENDED_COLORIMETRY_XV_YCC_601 == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_XVYCC_601;\
    } \
    else if (HDMI_EXTENDED_COLORIMETRY_XV_YCC_709 == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_XVYCC_709;\
    }\
    else if (HDMI_EXTENDED_COLORIMETRY_S_YCC_601 == kextcolorimetry) \
    { \
        uextcolorimetry = HDMI_COLORIMETRY_S_YCC_601;\
    }\
    else if (HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601 == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_ADOBE_YCC_601;\
    }\
    else if (HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_ADOBE_RGB;\
    }\
    else if (HDMI_EXTENDED_COLORIMETRY_2020_const_luminous == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_2020_CONST_LUMINOUS;\
    }\
    else if (HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous == kextcolorimetry)\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_2020_NON_CONST_LUMINOUS;\
    }\
    else\
    {\
        uextcolorimetry = HDMI_COLORIMETRY_RESERVED;\
    }\
}while(0)

#define HDMI_UCOLORSPACE_2_KCOLORSPACE(ucolorspace, kcolorspace) \
do{\
    if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == ucolorspace)\
    {\
        kcolorspace = HDMI_COLORSPACE_RGB;\
    }\
    else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR422 == ucolorspace)\
    {\
        kcolorspace = HDMI_COLORSPACE_YCbCr422;\
    }\
    else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == ucolorspace) \
    {\
        kcolorspace = HDMI_COLORSPACE_YCbCr444;\
    }\
    else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR420 == ucolorspace) \
    {\
        kcolorspace = HDMI_COLORSPACE_YCbCr420;\
    }\
    else\
    {\
        kcolorspace = HDMI_COLORSPACE_BUTT;\
    }\
}while(0)

#define HDMI_KCOLORSPACE_2_UCOLORSPACE(kcolorspace, ucolorspace) \
do{\
    if (HDMI_COLORSPACE_RGB == kcolorspace)\
    {\
        ucolorspace = HI_UNF_HDMI_VIDEO_MODE_RGB444;\
    }\
    else if (HDMI_COLORSPACE_YCbCr422 == kcolorspace)\
    {\
        ucolorspace = HI_UNF_HDMI_VIDEO_MODE_YCBCR422;\
    }\
    else if (HDMI_COLORSPACE_YCbCr444 == kcolorspace) \
    {\
        ucolorspace = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;\
    }\
    else if (HDMI_COLORSPACE_YCbCr420 == kcolorspace) \
    {\
        ucolorspace = HI_UNF_HDMI_VIDEO_MODE_YCBCR420;\
    }\
}while(0)

#define HDMI_UVIDEOMODE_2_KCOLORSPACE(uvideomode, kcolorspace) \
do{\
    if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == uvideomode)\
    {\
        kcolorspace = HDMI_COLORSPACE_RGB;\
    }\
    else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR422 == uvideomode)\
    {\
        kcolorspace = HDMI_COLORSPACE_YCbCr422;\
    }\
    else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == uvideomode) \
    {\
        kcolorspace = HDMI_COLORSPACE_YCbCr444;\
    }\
    else\
    {\
        kcolorspace = HDMI_COLORSPACE_BUTT;\
    }\
}while(0)

#define HDMI_KCOLORSPACE_2_UVIDEOMODE(kcolorspace, uvideomode) \
do{\
    if (HDMI_COLORSPACE_RGB == kcolorspace)\
    {\
        uvideomode = HI_UNF_HDMI_VIDEO_MODE_RGB444;\
    }\
    else if (HDMI_COLORSPACE_YCbCr422 == kcolorspace)\
    {\
        uvideomode = HI_UNF_HDMI_VIDEO_MODE_YCBCR422;\
    }\
    else if (HDMI_COLORSPACE_YCbCr444 == kcolorspace) \
    {\
        uvideomode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;\
    }\
    else\
    {\
        uvideomode = HI_UNF_HDMI_VIDEO_MODE_BUTT;\
    }\
}while(0)

#define HDMI_UDEEPCOLOR_2_KDEEPCOLOR(udeepcolor, kdeepcolor) \
do{\
    if (HI_UNF_HDMI_DEEP_COLOR_24BIT == udeepcolor)\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_24BIT;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == udeepcolor)\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_30BIT;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == udeepcolor)\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_36BIT;\
    }\
    else\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_24BIT;\
    }\
}while(0)

#define HDMI_USCANINFO_2_KSCANMODE(uscaninfo, kscanmode) \
do{\
    if (HDMI_SCAN_INFO_NO_DATA == uscaninfo)\
    {\
        kscanmode = HDMI_SCAN_MODE_NONE;\
    }\
    else if (HDMI_SCAN_INFO_OVERSCANNED == uscaninfo)\
    {\
        kscanmode = HDMI_SCAN_MODE_OVERSCAN;\
    }\
    else if (HDMI_SCAN_INFO_UNDERSCANNED == uscaninfo) \
    {\
        kscanmode = HDMI_SCAN_MODE_UNDERSCAN;\
    }\
    else\
    {\
        kscanmode = HDMI_SCAN_MODE_BUTT;\
    }\
}while(0)

#define HDMI_KSCANMODE_2_USCANINFO(kscanmode, uscaninfo) \
do{\
    if (HDMI_SCAN_MODE_NONE == kscanmode)\
    {\
        uscaninfo = HDMI_SCAN_INFO_NO_DATA;\
    }\
    else if (HDMI_SCAN_MODE_OVERSCAN == kscanmode)\
    {\
        uscaninfo = HDMI_SCAN_INFO_OVERSCANNED;\
    }\
    else if (HDMI_SCAN_MODE_UNDERSCAN == kscanmode) \
    {\
        uscaninfo = HDMI_SCAN_INFO_UNDERSCANNED;\
    }\
    else\
    {\
        uscaninfo = HDMI_SCAN_INFO_FUTURE;\
    }\
}while(0)


#define HDMI_URGBQUANTIZATION_2_KQUANTIZATION(urgbquantization, krgbquantization) \
do{\
    if (HDMI_RGB_QUANTIZATION_DEFAULT_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_DEFAULT;\
    }\
    else if (HDMI_RGB_QUANTIZATION_LIMITED_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_LIMITED;\
    }\
    else if (HDMI_RGB_QUANTIZATION_FULL_RANGE == urgbquantization)\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_FULL;\
    }\
    else\
    {\
        krgbquantization = HDMI_QUANTIZATION_RANGE_BUTT;\
    }\
}while(0)

#define HDMI_KQUANTIZATION_2_URGBQUANTIZATION(krgbquantization, urgbquantization) \
do{\
    if (HDMI_QUANTIZATION_RANGE_DEFAULT == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE ;\
    }\
    else if (HDMI_QUANTIZATION_RANGE_LIMITED == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_LIMITED_RANGE;\
    }\
    else if (HDMI_QUANTIZATION_RANGE_FULL == krgbquantization)\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_FULL_RANGE;\
    }\
    else\
    {\
        urgbquantization = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;\
    }\
}while(0)

#define HDMI_UYCCQUANTIZATION_2_KYCCQUANTIZATION(uyccquantization, kyccquantization) \
do{\
    if (HDMI_YCC_QUANTIZATION_LIMITED_RANGE == uyccquantization)\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;\
    }\
    else if (HDMI_YCC_QUANTIZATION_FULL_RANGE == uyccquantization)\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;\
    }\
    else\
    {\
        kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_BUTT;\
    }\
}while(0)

#define HDMI_KYCCQUANTIZATION_2_UYCCQUANTIZATION(kyccquantization, uyccquantization) \
do{\
    if (HDMI_YCC_QUANTIZATION_RANGE_LIMITED == kyccquantization)\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;\
    }\
    else if (HDMI_YCC_QUANTIZATION_RANGE_FULL == kyccquantization)\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_FULL_RANGE;\
    }\
    else\
    {\
        uyccquantization = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;\
    }\
}while(0)

#define HDMI_KAUDIOCODING_2_UAUDIOCODING(kaudiocoding, uaudiocoding) do{\
    switch(kaudiocoding)\
    {\
    	case HDMI_AUDIO_CODING_TYPE_STREAM:\
    	    uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_PCM:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_AC3:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_MPEG1:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_MP3:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_MPEG2:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_AAC_LC:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_DTS:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_ATRAC:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_DSD:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_EAC3:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_DTS_HD:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD;\
        	break;\
    	case HDMI_AUDIO_CODING_TYPE_MLP:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_DST:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_DST;\
    	    break;\
    	case HDMI_AUDIO_CODING_TYPE_WMA_PRO:\
        	uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO;\
    	    break;\
        case HDMI_AUDIO_CODING_TYPE_RESERVED:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT;\
            break;\
    	case HDMI_AUDIO_CODING_TYPE_BUTT:\
    	default:\
            uaudiocoding=HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT;\
    	    break;\
    }\
}while(0)

#define HDMI_UAUDIOCODING_2_KAUDIOCODING(uaudiocoding,kaudiocoding) do{\
    switch(uaudiocoding)\
    {\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED:\
    	    kaudiocoding=HDMI_AUDIO_CODING_TYPE_STREAM;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM:\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_PCM;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3:\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_AC3;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1:\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_MPEG1;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3:\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_MP3;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2 :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_MPEG2;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_AAC_LC;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_DTS;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_ATRAC;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT:\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_DSD;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_EAC3;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_DTS_HD;\
        	break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_MLP;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_DST :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_DST;\
    	    break;\
    	case HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO :\
        	kaudiocoding=HDMI_AUDIO_CODING_TYPE_WMA_PRO;\
    	    break;\
        case HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT :\
            kaudiocoding=HDMI_AUDIO_CODING_TYPE_RESERVED;\
            break;\
    	default:\
            kaudiocoding=HDMI_AUDIO_CODING_TYPE_BUTT;\
    	    break;\
    }\
}while(0)

#define HDMI_UHDRMODE_2_KHDRMODE(uhdrmode, khdrmode) \
do{\
    if (HI_DRV_HDMI_HDR_USERMODE_SDR == uhdrmode)\
    {\
        khdrmode = HDMI_HDR_USERMODE_SDR;\
    }\
    else if (HI_DRV_HDMI_HDR_USERMODE_HDR10 == uhdrmode)\
    {\
        khdrmode = HDMI_HDR_USERMODE_HDR10;\
    }\
    else if (HI_DRV_HDMI_HDR_USERMODE_DOLBY == uhdrmode)\
    {\
        khdrmode = HDMI_HDR_USERMODE_DOLBY;\
    }\
    else\
    {\
        khdrmode = HDMI_HDR_USERMODE_SDR;\
    }\
}while(0)

#define 	HDMI_FEILD_RATE_DEVIATION	50
#define 	HDMI_PIXL_FREQ_DEVIATION	100
#define 	HDMI_VIC_PC_BASE 			((HDMI_VIC_VIRTUAL_BASE)+0x100)


typedef struct
{
	HI_U32				u32EtablishCode;
	HI_U32				u32Format;
}HDMI_ESTAB_FORMAT_S;


typedef struct
{
	HI_U32				u32Vic;
	HI_U32				u32PixlFreq;				/* KHz */
	HI_U32				u32HorActive;
	HI_U32				u32VerActive;
	HI_U32				u32FieldRate;				/* 0.01Hz */
	HI_U32				u32Format;
}HDMI_FMT_PARAM_S;


static const HDMI_ESTAB_FORMAT_S s_g_stFmtEstabTable[]=
{
	{ HDMI_EDID_ESTABTIMG_VESA_800X600_60		,	HI_UNF_ENC_FMT_VESA_800X600_60	},
	{ HDMI_EDID_ESTABTIMG_VESA_800X600_56		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_640X480_75		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_640X480_72		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_IBM_VGA_640X480_67	,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_IBM_XGA_640X480_60	,	HI_UNF_ENC_FMT_VESA_800X600_60	},
	{ HDMI_EDID_ESTABTIMG_IBM_XGA2_720X400_88	,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_IBM_XGA_720X400_70	,	HI_UNF_ENC_FMT_BUTT 			},

	{ HDMI_EDID_ESTABTIMG_VESA_1280X1024_75 	,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_1024X768_75		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_1024X768_70		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_1024X768_60		,	HI_UNF_ENC_FMT_VESA_1024X768_60 },
	{ HDMI_EDID_ESTABTIMG_IBM_1024X768_87		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_APPLE_MACII_832X624_75,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_800X600_75		,	HI_UNF_ENC_FMT_BUTT 			},
	{ HDMI_EDID_ESTABTIMG_VESA_800X600_72		,	HI_UNF_ENC_FMT_BUTT 			},

	{ HDMI_EDID_ESTABTIMG_VESA_1152X870_75		,	HI_UNF_ENC_FMT_BUTT 			},

};

static const HDMI_FMT_PARAM_S s_g_stFmtParamTable[]=
{
	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
	{						 1, 25175 ,  640 ,480 , 6000 ,	HI_UNF_ENC_FMT_861D_640X480_60	},		// 1. 640 x 480p @ 60 VGA
	{						 2, 27000 ,  720 ,480 , 6000 ,	HI_UNF_ENC_FMT_480P_60			},		// 2,3 720 x 480p
	{						 3, 27000 ,  720 ,480 , 6000 ,	HI_UNF_ENC_FMT_480P_60			},		// 2,3 720 x 480p
	{						 4, 74170 ,  1280,720 , 6000 ,	HI_UNF_ENC_FMT_720P_60			},		// 4   1280 x 720p
	{						 5, 74170 ,  1920,1080, 6000 ,	HI_UNF_ENC_FMT_1080i_60 		},		// 5 1920 x 1080i
	{						 6, 27000 ,  720 ,480 , 6000 ,	HI_UNF_ENC_FMT_NTSC 			},		// 6,7 720 x 480i, pix repl
	{						 7, 27000 ,  720 ,480 , 6000 ,	HI_UNF_ENC_FMT_NTSC 			},		// 6,7 720 x 480i, pix repl
	{						 8, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 8,9(1) 1440 x 240p
	{						 8, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 8,9(2) 1440 x 240p
	{						 9, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 8,9(1) 1440 x 240p
	{						 9, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 8,9(2) 1440 x 240p
	{						10, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 10,11 2880 x 480p
	{						11, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 10,11 2880 x 480p
	{						12, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 12,13(1) 2280 x 240p
	{						12, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 12,13(2) 2280 x 240p
	{						14, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 14, 15 1140 x 480p
	{						16, 148500,  1920,1080, 6000 ,	HI_UNF_ENC_FMT_1080P_60 		},		// 16 1920 x 1080p
	{						17, 27000 ,  720 ,576 , 5000 ,	HI_UNF_ENC_FMT_576P_50			},		// 17,18 720 x 576p
	{						18, 27000 ,  720 ,576 , 5000 ,	HI_UNF_ENC_FMT_576P_50			},		// 17,18 720 x 576p
	{						19, 74250 ,  1280,720 , 5000 ,	HI_UNF_ENC_FMT_720P_50			},		// 19 1280 x 720p
	{						20, 74250 ,  1920,1080, 5000 ,	HI_UNF_ENC_FMT_1080i_50 		},		// 20 1920 x 1080i
	{						21, 27000 ,  1440,576 , 5000 ,	HI_UNF_ENC_FMT_PAL				},		// 21,22 1440 x 576i
	{						22, 27000 ,  1440,576 , 5000 ,	HI_UNF_ENC_FMT_PAL				},		// 21,22 1440 x 576i
	{						23, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 23,24(1) 1440 x 288p
	{						23, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 23,24(2) 1440 x 288p
	{						23, 27000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 23,24(3) 1440 x 288p
	{						25, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 25, 26 2880 x 576p
	{						27, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 27,28(1) 2880 x 288p
	{						27, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 27,28(2) 2880 x 288p
	{						27, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 27,28(3) 2880 x 288p
	{						29, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 29,30 1440 x 576p

	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
	{						31, 148500,  1440,576 , 5000 ,	HI_UNF_ENC_FMT_1080P_50 		},		// 31(1) 1920 x 1080p
	{						32, 74170 ,  1920,1080, 2400 ,	HI_UNF_ENC_FMT_1080P_24 		},		// 32(2) 1920 x 1080p
	{						33, 74250 ,  1920,1080, 2500 ,	HI_UNF_ENC_FMT_1080P_25 		},		// 33(3) 1920 x 1080p
	{						34, 74170 ,  1920,1080, 3000 ,	HI_UNF_ENC_FMT_1080P_30 		},		// 34(4) 1920 x 1080p
	{						35, 108000,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 35, 36 2880 x 480p@59.94/60Hz
	{						37, 108000,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 37, 38 2880 x 576p@50Hz
	{						39, 72000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 39 1920 x 1080i@50Hz
	{						40, 148500,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 40 1920 x 1080i@100Hz
	{						41, 148500,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 41 1280 x 720p@100Hz
	{						42, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 42, 43, 720p x 576p@100Hz
	{						44, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 44, 45, 720p x 576i@100Hz, pix repl
	{						46, 148500,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 46, 1920 x 1080i@119.88/120Hz
	{						47, 148500,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 47, 1280 x 720p@119.88/120Hz
	{						48, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 48, 49 720 x 480p@119.88/120Hz
	{						50, 54000 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 50, 51 720 x 480i@119.88/120Hz
	{						52, 10800 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 52, 53, 720 x 576p@200Hz
	{						54, 10800 ,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 54, 55, 1440 x 720i @200Hz, pix repl
	{						56, 108000,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 56, 57, 720 x 480p @239.76/240Hz
	{						58, 108000,  0	 ,0   , 0	 ,	HI_UNF_ENC_FMT_BUTT 			},		// 58, 59, 1440 x 480i @239.76/240Hz, pix repl


	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
	{ HDMI_VIC_PC_BASE+ 	 0, 40000 , 800 ,600 ,	60 ,	  HI_UNF_ENC_FMT_VESA_800X600_60	},	// 800x600@60
	{ HDMI_VIC_PC_BASE+ 	 1, 65000 , 1024,768 ,	60 ,	  HI_UNF_ENC_FMT_VESA_1024X768_60	},	// 1024x768@60
	{ HDMI_VIC_PC_BASE+ 	 2, 74250 , 1280,720 ,	60 ,	  HI_UNF_ENC_FMT_VESA_1280X720_60	},	// 720x400@70.08
	{ HDMI_VIC_PC_BASE+ 	 3, 83500 , 1280,800 ,	60 ,	  HI_UNF_ENC_FMT_VESA_1280X800_60	},	// 720x400@85.04
	{ HDMI_VIC_PC_BASE+ 	 4, 108000, 1280,1024,	60 ,	  HI_UNF_ENC_FMT_VESA_1280X1024_60	},	// 1280x1024@60
	{ HDMI_VIC_PC_BASE+ 	 5, 85500 , 1360,768 ,	60 ,	  HI_UNF_ENC_FMT_VESA_1360X768_60	},	// 1360x768@60
	{ HDMI_VIC_PC_BASE+ 	 6, 85500 , 1366,768 ,	60 ,	  HI_UNF_ENC_FMT_VESA_1366X768_60	},	// 1360x768@60
	{ HDMI_VIC_PC_BASE+ 	 7, 121750, 1400,1050,	60 ,	  HI_UNF_ENC_FMT_VESA_1400X1050_60	},	// 1360x768@60
	{ HDMI_VIC_PC_BASE+ 	 8, 106500, 1440,900,	60 ,	  HI_UNF_ENC_FMT_VESA_1440X900_60	},	// 800x600@56.25
	{ HDMI_VIC_PC_BASE+ 	 9, 162000, 1600,1200,	60 ,	  HI_UNF_ENC_FMT_VESA_1600X1200_60	},	// 1600x1200@60
	{ HDMI_VIC_PC_BASE+ 	 10,146250, 1680,1050,	60 ,	  HI_UNF_ENC_FMT_VESA_1680X1050_60	},	// 800x600@72.19
	{ HDMI_VIC_PC_BASE+ 	 11,148500, 1920,1080,	60 ,	  HI_UNF_ENC_FMT_VESA_1920X1080_60	},	// 800x600@72.19
	{ HDMI_VIC_PC_BASE+ 	 12,154000, 1920,1200,	60 ,	  HI_UNF_ENC_FMT_VESA_1920X1200_60	},	// 1920x1200@59.95
	{ HDMI_VIC_PC_BASE+ 	 13,234000, 1920,1440,	60 ,	  HI_UNF_ENC_FMT_VESA_1920X1440_60	},	// 1024x768@60
	{ HDMI_VIC_PC_BASE+ 	 14,156750, 2048,1152,	60 ,	  HI_UNF_ENC_FMT_VESA_2048X1152_60	},	// 1024x768@70.07
	//{ HDMI_VIC_PC_BASE+ 	 15, 88750, 1440,900,	60 ,	  HI_UNF_ENC_FMT_VESA_1440X900_60_RB	},	// 800x600@56.25
	{ HDMI_VIC_PC_BASE+ 	 15, 97750, 1600,900,	60 ,	  HI_UNF_ENC_FMT_VESA_1600X900_60_RB	},	// 800x600@56.25
	{ HDMI_VIC_PC_BASE+ 	16, 108000, 0	,0	 ,	0	 ,	  HI_UNF_ENC_FMT_BUTT				},	// 1152x864@75
	{ HDMI_VIC_PC_BASE+ 	17, 108000, 0	,0	 ,	0	 ,	  HI_UNF_ENC_FMT_BUTT				},	// 1152x864@75
	{ HDMI_VIC_PC_BASE+ 	18, 68250 , 0	,0	 ,	0	 ,	  HI_UNF_ENC_FMT_BUTT				},	// 1280x768@59.95
	{ HDMI_VIC_PC_BASE+ 	19, 79500 , 0	,0	 ,	0	 ,	  HI_UNF_ENC_FMT_BUTT				},	// 1280x768@59.87


	/********************VIC,PixFfeq  , HACT ,VACT,FldRate,    EncFmt************************/
	{ 93, 297000,  3840,2160, 2400 ,	HI_UNF_ENC_FMT_3840X2160_24 		},
	{ 94, 297000,  3840,2160, 2500 ,	HI_UNF_ENC_FMT_3840X2160_25 		},
	{ 95, 297000,  3840,2160, 3000 ,	HI_UNF_ENC_FMT_3840X2160_30 		},
	{ 96, 594000,  3840,2160, 5000 ,	HI_UNF_ENC_FMT_3840X2160_50 		},
	{ 97, 594000,  3840,2160, 6000 ,	HI_UNF_ENC_FMT_3840X2160_60 		},

	{ 98, 297000,  4096,2160, 2400 ,	HI_UNF_ENC_FMT_4096X2160_24 		},
	{ 99, 297000,  4096,2160, 2500 ,	HI_UNF_ENC_FMT_4096X2160_25 		},
    { 100, 297000,  4096,2160, 3000 ,	HI_UNF_ENC_FMT_4096X2160_30 		},
	{ 101, 594000,  4096,2160, 5000 ,	HI_UNF_ENC_FMT_4096X2160_50 		},
	{ 102, 594000,  4096,2160, 6000 ,	HI_UNF_ENC_FMT_4096X2160_60 		},

};


static HI_S32 HdmiExtIoctl(unsigned int cmd, void *argp);
static HI_S32 IntfkDisp2HdmiVoAttr(HDMI_DEVICE_S * pstHdmiDev, HDMI_VO_ATTR_S * pstDestAttr, HDMI_VIDEO_ATTR_S * pstSrcAttr);

HI_S32 HI_DRV_HDMI_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
HI_S32 HI_DRV_HDMI_Resume(PM_BASEDEV_S *pdev);
HI_S32 HI_DRV_HDMI_SoftResume(HDMI_VIDEO_ATTR_S *pstVideoAttr);

static HDMI_EXPORT_FUNC_S s_stHdmiExportFuncs = {
    .pfnHdmiInit = HI_DRV_HDMI_Init,
    .pfnHdmiDeinit = HI_DRV_HDMI_Deinit,
    .pfnHdmiOpen  = HI_DRV_HDMI_Open,
    .pfnHdmiClose = HI_DRV_HDMI_Close,
    .pfnHdmiGetPlayStus = HI_DRV_HDMI_GetPlayStatus,
    .pfnHdmiGetAoAttr = HI_DRV_HDMI_GetAOAttr,
    .pfnHdmiGetSinkCapability = HI_DRV_HDMI_GetSinkCapability,
    .pfnHdmiGetAudioCapability = HI_DRV_HDMI_GetAudioCapability,
    .pfnHdmiSetAudioMute = HI_DRV_HDMI_SetAudioMute,
    .pfnHdmiSetAudioUnMute = HI_DRV_HDMI_SetAudioUnMute,
    .pfnHdmiAudioChange = HI_DRV_HDMI_SetAOAttr,
    .pfnHdmiPreFormat = HI_DRV_HDMI_PreFormat,
    .pfnHdmiSetFormat = HI_DRV_HDMI_SetFormat,
    .pfnHdmiDetach = HI_DRV_HDMI_Detach,
    .pfnHdmiAttach = HI_DRV_HDMI_Attach,
    .pfnHdmiResume = HI_DRV_HDMI_Resume,
    .pfnHdmiSuspend = HI_DRV_HDMI_Suspend,
    .pfnHdmiSoftResume = HI_DRV_HDMI_SoftResume,
	.pfnHdmiGetVideoCapability = HI_DRV_HDMI_GetVideoCapability,

#ifdef HDMI_HDR_SUPPORT
    .pfnHdmiSetHdrAttr = HI_DRV_HDMI_SetHdrAttr,
    .pfnHdmiSetHdrMode = HI_DRV_HDMI_SetHdrMode,
#endif
};

#define CHECK_HDMI_OPEN(HdmiId) \
do \
{   \
    if (GetHdmiDevice(HdmiId) == HI_NULL) \
    { \
        HDMI_WARN("The hdmi device id is wrong\n");\
        return HI_FAILURE;\
    }\
    if(GetHdmiDevice(HdmiId)->u32KernelCnt == 0 && GetHdmiDevice(HdmiId)->u32UserCnt == 0) \
    {   \
        HDMI_WARN("The hdmi device not open\n");\
        return HI_FAILURE;\
    } \
}while(0)

#define CHECK_HDMI_PTR_NULL(Ptr) \
do \
{   \
    if (HI_NULL == Ptr) \
    { \
        HDMI_ERR("Input ptr is null!\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define CHECK_HDMI_PTR_NULL_NO_RET(Ptr) \
    do \
    {   \
        if (HI_NULL == Ptr) \
        { \
            HDMI_ERR("Input ptr is null!\n");\
            return ;\
        }\
    }while(0)


#if 0
static HI_U32 Hdmi_Etablish2Format(HI_U32 u32EtablishCode)
{
	HI_U32 i = 0;
	HDMI_ESTAB_FORMAT_S *pstEstabTiming = HI_NULL;

	for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtEstabTable);i++)
	{
		pstEstabTiming = (HDMI_ESTAB_FORMAT_S *)&s_g_stFmtEstabTable[i];
		if (   pstEstabTiming != HI_NULL
			&& pstEstabTiming->u32EtablishCode == u32EtablishCode)
		{
			return pstEstabTiming->u32Format;
		}
	}

	return HI_UNF_ENC_FMT_BUTT;

}

static HI_U32 Hdmi_Vic2Format(HI_U32 u32Vic)
{
	HI_U32 i = 0;
	HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

	for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable);i++)
	{
		pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
		if (   (pstFmtParam != HI_NULL)
			&& (pstFmtParam->u32Vic == u32Vic)	  )
		{
			return pstFmtParam->u32Format;
		}
	}

	return HI_UNF_ENC_FMT_BUTT;

}


static HI_U32 Hdmi_StdTiming2Format(HDMI_EDID_STD_TIMING_S *pstStdTiming)
{
	HI_U32 i = 0;
	HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

	if (pstStdTiming == HI_NULL)
	{
		return HI_UNF_ENC_FMT_BUTT;
	}

	for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable);i++)
	{
		pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
		if (   (pstFmtParam != HI_NULL)
			&& (pstFmtParam->u32HorActive == pstStdTiming->u32HorActive)
			&& (pstFmtParam->u32VerActive == pstStdTiming->u32VerActive)
			&& (pstFmtParam->u32FieldRate >= pstStdTiming->u32RefreshRate)
			&& ((pstFmtParam->u32FieldRate-HDMI_FEILD_RATE_DEVIATION) <= pstStdTiming->u32RefreshRate)	)
		{
			return pstFmtParam->u32Format;
		}
	}

	return HI_UNF_ENC_FMT_BUTT;

}

static HI_U32 Hdmi_PreTiming2Format(HDMI_EDID_PRE_TIMING_S *pstPreTiming)
{
	HI_U32 i = 0;
	HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

	if (pstPreTiming == HI_NULL)
	{
		return HI_UNF_ENC_FMT_BUTT;
	}

	for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable);i++)
	{
		pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
		if (   (pstFmtParam != HI_NULL)
			&& (pstFmtParam->u32HorActive == pstPreTiming->u32HACT)
			&& (pstFmtParam->u32VerActive == pstPreTiming->u32VACT)
			&& (pstFmtParam->u32PixlFreq >= pstPreTiming->u32PixelClk)
			&& ((pstFmtParam->u32PixlFreq-HDMI_PIXL_FREQ_DEVIATION) <= pstPreTiming->u32PixelClk)  )
		{
			return pstFmtParam->u32Format;
		}
	}

	return HI_UNF_ENC_FMT_BUTT;

}
#endif

static HI_S32 Hdmi_CapabilityDrv2Ext(HI_DRV_HDMI_SINK_CAPABILITY_S *pstExtCap,
									HDMI_SINK_CAPABILITY_S *pstDrvCap)
{
	if (HI_NULL==pstDrvCap || HI_NULL==pstExtCap)
	{
		HDMI_ERR("null pointer!\n");
		return HI_FAILURE;
	}

    pstExtCap->stBasicDispPara.u8MaxImageHeight = pstDrvCap->stBasicDispPara.u8MaxImageHeight;
    pstExtCap->stBasicDispPara.u8MaxImageWidth  = pstDrvCap->stBasicDispPara.u8MaxImageWidth;


	return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_PixClkGet(HI_UNF_ENC_FMT_E enFormat,HI_U32 *pu32PixClk)
{
    	HI_U32 i = 0;
	HDMI_FMT_PARAM_S *pstFmtParam = HI_NULL;

	if (pu32PixClk == HI_NULL  )
	{
	    HDMI_ERR("pu32PixClk=null!\n");
		return HI_FAILURE;
	}
    	*pu32PixClk = 0;
	for (i=0;i<HDMI_ARRAY_SIZE(s_g_stFmtParamTable);i++)
	{
		pstFmtParam = (HDMI_FMT_PARAM_S *)&s_g_stFmtParamTable[i];
		if ( (pstFmtParam != HI_NULL) && (pstFmtParam->u32Format == enFormat) )
		{
		    *pu32PixClk = pstFmtParam->u32PixlFreq;
			return HI_SUCCESS;
		}
	}
	return HI_FAILURE;
}

HI_S32 HI_DRV_HDMI_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32          ret = HI_SUCCESS;
    HDMI_DEVICE_ID_E enHdmi = HDMI_DEVICE_ID0;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32SuspendInTime = HDMI_OSAL_GetTimeInMs();

    while (enHdmi < HDMI_DEVICE_ID_BUTT)
    {
        CHECK_HDMI_OPEN(enHdmi);
        ret = HdmiExtIoctl(CMD_HDMI_STOP, &enHdmi);
        if (ret != HI_SUCCESS)
        {
    	    HDMI_ERR("hdmi stop  err!:0x%x\n",ret);
    	    return ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_STOP ok! \n");
        enHdmi++;
    }

    pstHdmiDev->stIntfStatus.u32SuspendOutTime = HDMI_OSAL_GetTimeInMs();

    HI_PRINT("HDMI Suspend OK\n");
    return ret;
}

HI_S32 HI_DRV_HDMI_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32          ret = HI_SUCCESS;
    HDMI_DEVICE_ID_E enHdmi = HDMI_DEVICE_ID0;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32ResumeInTime = HDMI_OSAL_GetTimeInMs();

    while (enHdmi < HDMI_DEVICE_ID_BUTT)
    {
        DRV_HDMI_APP_ATTR_S     stHDMIAttr;
        CHECK_HDMI_OPEN(enHdmi);

        ret = HdmiExtIoctl(CMD_HDMI_OPEN, &enHdmi);
        if (ret != HI_SUCCESS)
        {
    	    HDMI_ERR("hdmi open  err!:0x%x\n",ret);
    	    return ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_OPEN ok! \n");

        memset(&stHDMIAttr, 0, sizeof(stHDMIAttr));
        stHDMIAttr.enHdmiID = enHdmi;
        ret = HdmiExtIoctl(CMD_HDMI_GET_ATTR, &stHDMIAttr);
        if (ret != HI_SUCCESS)
        {
    	    HDMI_ERR("Get hdmi attr err!\n");
    	    return ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_GET_ATTR ok! \n");

        ret = HdmiExtIoctl(CMD_HDMI_SET_ATTR, &stHDMIAttr);
        if (ret != HI_SUCCESS)
        {
    	    HDMI_ERR("set attr err!:0x%x\n", ret);
            return ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_SET_ATTR ok! \n");
#if 0
        ret = HdmiExtIoctl(CMD_HDMI_START, &enHdmi);
        if (ret != HI_SUCCESS)
        {
    	    HDMI_ERR("hdmi stop  err!:0x%x\n",ret);
    	    return ret;
        }
#endif
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_START ok! \n");
        enHdmi++;
    }

    pstHdmiDev->stIntfStatus.u32ResumeOutTime = HDMI_OSAL_GetTimeInMs();

    HI_PRINT("HDMI Resume OK\n");

    return ret;
}

HI_S32 HI_DRV_HDMI_SoftResume(HDMI_VIDEO_ATTR_S *pstVideoAttr)
{
    HI_S32   s32Ret = HI_FAILURE;
    HDMI_DEVICE_ID_E enHdmi = HDMI_DEVICE_ID0;
    DRV_HDMI_VO_ATTR_S  stDrvVoAttr;

    CHECK_HDMI_PTR_NULL(pstVideoAttr);

    while (enHdmi < HDMI_DEVICE_ID_BUTT)
    {
        CHECK_HDMI_OPEN(enHdmi);

        s32Ret = HdmiExtIoctl(CMD_HDMI_OPEN, &enHdmi);
        if (s32Ret != HI_SUCCESS)
        {
    	    HDMI_ERR("hdmi open  err!:0x%x\n",s32Ret);
    	    return s32Ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_OPEN ok! \n");

        IntfkDisp2HdmiVoAttr(GetHdmiDevice(enHdmi),&stDrvVoAttr.stVOAttr, pstVideoAttr);
        stDrvVoAttr.enHdmiID = enHdmi;
        s32Ret = HdmiExtIoctl(CMD_HDMI_SET_VO_ATTR, &stDrvVoAttr);
        if (s32Ret != HI_SUCCESS)
        {
    	    HDMI_ERR("set vo attr err!:0x%x\n", s32Ret);
    	    return s32Ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_SET_VO_ATTR ok! \n");

        s32Ret = HdmiExtIoctl(CMD_HDMI_START, &enHdmi);
        if (s32Ret != HI_SUCCESS)
        {
    	    HDMI_ERR("hdmi stop  err!:0x%x\n",s32Ret);
    	    return s32Ret;
        }
        HDMI_INFO("HdmiExtIoctl CMD_HDMI_START ok! \n");
        enHdmi++;
    }
    HI_PRINT("HDMI SoftResume OK\n");

    return s32Ret;
}
#endif

HI_S32 HI_DRV_HDMI_Init(HI_VOID)
{
#if defined(CONFIG_HDMI_STB_SDK)
    return HI_HDMI_MODULE_Register((HI_U32)HI_ID_HDMI, "HI_HDMI", (HI_VOID *)&s_stHdmiExportFuncs);
#elif defined(CONFIG_HDMI_BVT_SDK)
    return HI_SUCCESS;
#endif
}

HI_VOID HI_DRV_HDMI_Deinit(HI_VOID)
{
#if defined(CONFIG_HDMI_STB_SDK)
    HI_HDMI_MODULE_UnRegister((HI_U32)HI_ID_HDMI);
#elif defined(CONFIG_HDMI_BVT_SDK)
    return ;
#endif
}

#if defined(CONFIG_HDMI_STB_SDK)
HI_S32 HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi)
{

    HI_S32   s32Ret = HI_FAILURE;

    s32Ret = HdmiExtIoctl(CMD_HDMI_OPEN, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("open hdmi err!:0x%x\n", s32Ret);
	    return s32Ret;
    }

    return s32Ret;
}

HI_S32 HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32   s32Ret = HI_FAILURE;

    CHECK_HDMI_OPEN(enHdmi);

    s32Ret = HdmiExtIoctl(CMD_HDMI_STOP, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("stop hdmi err!:0x%x\n", s32Ret);
	    return s32Ret;
    }

    s32Ret = HdmiExtIoctl(CMD_HDMI_CLOSE, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("close hdmi err!:0x%x\n", s32Ret);
	    return s32Ret;
    }

    return HI_SUCCESS;
}
#endif

static HI_S32 HdmiExtIoctl(unsigned int cmd, void *argp)
{
    HI_S32   s32Ret = HI_FAILURE;
    CHECK_HDMI_PTR_NULL(argp);

    s32Ret = DRV_HDMI_CmdProcess(cmd, argp, HI_FALSE);

    return s32Ret;
}

static HI_VOID HdmiHotPlugProcess(HDMI_DEVICE_ID_E enHdmiID)
{
    HI_S32          ret = HI_SUCCESS;
    HDMI_SINK_CAPABILITY_S *pstSinkCap;
    DRV_HDMI_APP_ATTR_S  stHDMIAttr;
    HDMI_APP_ATTR_S     *pstAppAttr;
    DRV_HDMI_STATUS_S   stDrvHdmiStatus;
    //HDMI_EDID_STATUS_S  stStatus;

    HDMI_INFO("\n---HDMI kernel event(no UserCallBack): HOTPLUG. --- \n");

    DRV_HDMI_DefaultActionSet(GetHdmiDevice(enHdmiID), HDMI_DEFAULT_ACTION_HDMI);

    ret = DRV_HDMI_EdidCapabilityGet(&(GetHdmiDevice(enHdmiID)->stEdidInfo), &pstSinkCap);
    if (ret == HDMI_EDID_DATA_INVALID)
    {
        HDMI_WARN("Get sink capability err!\n");
	    //return ;
    }

    HDMI_MEMSET(&stDrvHdmiStatus,0,sizeof(DRV_HDMI_STATUS_S));
    stDrvHdmiStatus.enHdmiID = enHdmiID;
    ret = HdmiExtIoctl(CMD_HDMI_GET_STATUS, &stDrvHdmiStatus);
    if (ret != HI_SUCCESS)
    {
        HDMI_ERR("Get HDMI Status err!\n");
	    return ;
    }

    if (HI_FALSE == stDrvHdmiStatus.stHdmiStatus.bConnected)
    {
       HDMI_ERR("No Connect!\n");
       return;
    }

    HDMI_INFO("Connect !\n");


    memset(&stHDMIAttr, 0, sizeof(stHDMIAttr));
    stHDMIAttr.enHdmiID = enHdmiID;
    ret = HdmiExtIoctl(CMD_HDMI_GET_ATTR, &stHDMIAttr);
    if (ret != HI_SUCCESS)
    {
	    HDMI_ERR("Get hdmi attr err!\n");
	    return ;
    }
    HDMI_INFO("HdmiExtIoctl CMD_HDMI_GET_ATTR ok! \n");


    pstAppAttr = &stHDMIAttr.stAPPAttr;
#if 0
    DRV_HDMI_EdidStatusGet(&(GetHdmiDevice(enHdmiID)->stEdidInfo), &stStatus);

    /*20150828, l232354: HotPlug need to trust the ATTR in BVT product.*/
#if defined(CONFIG_HDMI_STB_SDK)
    if(stStatus.bCapValid)
    {
        pstAppAttr->enOutColorSpace = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        if(HI_TRUE == pstSinkCap->bSupportHdmi)
        {
            pstAppAttr->bEnableHdmi = HI_TRUE;
            if(HI_TRUE != pstSinkCap->stColorSpace.bYCbCr444)
            {
		        pstAppAttr->enOutColorSpace = HI_UNF_HDMI_VIDEO_MODE_RGB444;
            }
        }
        else
        {
	        pstAppAttr->enOutColorSpace = HI_UNF_HDMI_VIDEO_MODE_RGB444;
            //read real edid ok && sink not support hdmi,then we run in dvi mode
            pstAppAttr->bEnableHdmi = HI_FALSE;
        }
    }
    else
    {
        if(DRV_HDMI_DefaultActionGet(GetHdmiDevice(enHdmiID)) == HDMI_DEFAULT_ACTION_HDMI)
        {
            pstAppAttr->bEnableHdmi = HI_TRUE;
        }
        else
        {
            pstAppAttr->bEnableHdmi = HI_FALSE;
        }
    }
#endif
#endif
#if defined(CONFIG_HDMI_STB_SDK)
    if(HI_TRUE == pstAppAttr->bEnableHdmi)
    {
        pstAppAttr->bEnableAudio = HI_TRUE;
        pstAppAttr->bEnableVideo = HI_TRUE;
        pstAppAttr->bEnableAudInfoFrame = HI_TRUE;
        pstAppAttr->bEnableAviInfoFrame = HI_TRUE;
    }
    else
    {
        pstAppAttr->bEnableAudio = HI_FALSE;
        pstAppAttr->bEnableVideo = HI_TRUE;
        pstAppAttr->bEnableAudInfoFrame = HI_FALSE;
        pstAppAttr->bEnableAviInfoFrame = HI_FALSE;
        pstAppAttr->enOutColorSpace = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
#endif

	pstAppAttr->bHDCPEnable = HI_FALSE;
    pstAppAttr->enHDCPMode = HDMI_HDCP_MODE_AUTO;
    ret = HdmiExtIoctl(CMD_HDMI_SET_ATTR, &stHDMIAttr);
    if (ret != HI_SUCCESS)
    {
	    HDMI_ERR("set attr err!:0x%x\n", ret);
    }
    HDMI_INFO("HdmiExtIoctl CMD_HDMI_SET_ATTR ok! \n");

    ret = HdmiExtIoctl(CMD_HDMI_START, &enHdmiID);
    if (ret != HI_SUCCESS)
    {
	    HDMI_ERR("hdmi start  err!:0x%x\n",ret);
	    return ;
    }

    HDMI_INFO("HdmiExtIoctl CMD_HDMI_START ok! \n");

    return;
}

static HI_VOID HdmiHotUnPlugProcess(HDMI_DEVICE_ID_E enHdmiID)
{
    HI_S32          ret = HI_SUCCESS;

    ret = HdmiExtIoctl(CMD_HDMI_STOP, &enHdmiID);
    if (ret != HI_SUCCESS)
    {
	    HDMI_ERR("hdmi stop  err!:0x%x\n",ret);
	    return ;
    }
    HDMI_INFO("HdmiExtIoctl CMD_HDMI_STOP ok! \n");
    return;
}

HI_S32 HI_DRV_HDMI_KernelEventCallback(HI_VOID* data, HDMI_EVENT_E enEvent)
{
    HDMI_DEVICE_ID_E enHdmiID = *(HDMI_DEVICE_ID_E*)data;
	HI_U32  u32NeedHotPlugProcess = HI_TRUE;

	#if defined(CONFIG_HDMI_BVT_SDK)
	HDMI_DEVICE_S *pstHdmiDev = GetHdmiDevice(enHdmiID);
    u32NeedHotPlugProcess = (pstHdmiDev->enRunState & HDMI_RUN_STATE_START) || \
	                      (pstHdmiDev->enRunState & HDMI_RUN_STATE_STOP);
	#endif

	if (u32NeedHotPlugProcess)
    {
        if (enEvent == HDMI_EVENT_HOTPLUG)
	    {
	        HdmiHotPlugProcess(enHdmiID);
	    }
	    else if (enEvent == HDMI_EVENT_HOTUNPLUG)
	    {
	        HdmiHotUnPlugProcess(enHdmiID);
	    }
    }
    return HI_SUCCESS;
}

#if defined(CONFIG_HDMI_STB_SDK)
HI_S32 HI_DRV_HDMI_GetPlayStatus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Status)
{
    HI_S32   s32Ret = HI_FAILURE;
    DRV_HDMI_PLAYSTATUS_S stPlayStatus;

    CHECK_HDMI_PTR_NULL(pu32Status);
    CHECK_HDMI_OPEN(enHdmi);
    stPlayStatus.enHdmiID = enHdmi;
    s32Ret = HdmiExtIoctl(CMD_HDMI_GET_HDMI_PLAYSTATUS, &stPlayStatus);
    if (s32Ret == HI_SUCCESS)
    {
        *pu32Status = stPlayStatus.u32Status;
    }

    return s32Ret;
}


HI_S32 HI_DRV_HDMI_GetSinkCapability(HI_DRV_HDMI_ID_E enHdmi, HI_DRV_HDMI_SINK_CAPABILITY_S *pstSinkCap)
{
	HI_S32	 s32Ret = HI_FAILURE;
	DRV_HDMI_SINK_CAPABILITY_S *pstDrvCap = HI_NULL;

    CHECK_HDMI_PTR_NULL(pstSinkCap);
	CHECK_HDMI_OPEN(enHdmi);

	pstDrvCap = HDMI_KMALLOC(sizeof(DRV_HDMI_SINK_CAPABILITY_S));
	if (pstDrvCap == HI_NULL)
	{
		return HI_FAILURE;
	}

	pstDrvCap->enHdmiID = enHdmi;
	s32Ret = HdmiExtIoctl(CMD_HDMI_GET_SINK_CAPABILITY, pstDrvCap);

	if (s32Ret == HI_SUCCESS)
	{
		s32Ret = Hdmi_CapabilityDrv2Ext(pstSinkCap, &pstDrvCap->stCap);
	}

	if (pstDrvCap)
	{
		HDMI_KFREE(pstDrvCap);
	}

	return s32Ret;
}


HI_S32 HI_DRV_HDMI_GetAudioCapability(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_AUDIO_CAPABILITY_S *pstAudCap)
{
    HI_S32	 s32Ret = HI_FAILURE;
    DRV_HDMI_AUDIO_CAPABILITY_S stTmpAudioCap;

    CHECK_HDMI_PTR_NULL(pstAudCap);
	CHECK_HDMI_OPEN(enHdmi);

	stTmpAudioCap.enHdmiID = enHdmi;
    s32Ret = HdmiExtIoctl(CMD_HDMI_GET_AUDIO_CAPABILITY,&stTmpAudioCap);
	if (s32Ret == HI_SUCCESS)
	{
        HDMI_MEMCPY(pstAudCap,&stTmpAudioCap.stAudioCap,sizeof(HDMI_AUDIO_CAPABILITY_S));
	}

	return s32Ret;
}

HI_S32 HI_DRV_HDMI_SetAudioMute(HI_UNF_HDMI_ID_E enHdmi)
{
    CHECK_HDMI_OPEN(enHdmi);
    DRV_HDMI_AudioOutputEnableSet(GetHdmiDevice(enHdmi), HI_FALSE);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_SetAudioUnMute(HI_UNF_HDMI_ID_E enHdmi)
{
    CHECK_HDMI_OPEN(enHdmi);
    DRV_HDMI_AudioOutputEnableSet(GetHdmiDevice(enHdmi), HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_GetAOAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstAudioAttr)
{
    HI_S32   s32Ret = HI_FAILURE;
    DRV_HDMI_AO_ATTR_S stDrvAOAttr;

    CHECK_HDMI_PTR_NULL(pstAudioAttr);
    CHECK_HDMI_OPEN(enHdmi);
    stDrvAOAttr.enHdmiID = enHdmi;
    s32Ret = HdmiExtIoctl(CMD_HDMI_GET_AO_ATTR, &stDrvAOAttr);
    if (s32Ret == HI_SUCCESS)
    {
        pstAudioAttr->bIsMultiChannel  = stDrvAOAttr.stAOAttr.enChanels > HDMI_AUDIO_FORMAT_2CH ? HI_TRUE : HI_FALSE;
        HDMI_KAUDIOCODING_2_UAUDIOCODING(stDrvAOAttr.stAOAttr.enAudioCode,pstAudioAttr->enAudioCode);
        pstAudioAttr->enBitDepth       = stDrvAOAttr.stAOAttr.enSampleDepth;
        HDMI_KSAMPLERATE_2_USAMPLERATE(stDrvAOAttr.stAOAttr.enSampleFs,pstAudioAttr->enSampleRate);
        pstAudioAttr->enSoundIntf      = stDrvAOAttr.stAOAttr.enSoundIntf;
        pstAudioAttr->u32Channels      = stDrvAOAttr.stAOAttr.enChanels;
        pstAudioAttr->u8DownSampleParm = stDrvAOAttr.stAOAttr.bDownSample;
    }

    return s32Ret;
}


HI_S32 HI_DRV_HDMI_SetAOAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstAudioAttr)
{
    HI_S32   s32Ret = HI_FAILURE;
    DRV_HDMI_AO_ATTR_S stDrvAoAttr;

    CHECK_HDMI_PTR_NULL(pstAudioAttr);
    CHECK_HDMI_OPEN(enHdmi);

    memset(&stDrvAoAttr, 0, sizeof(DRV_HDMI_AO_ATTR_S));
    stDrvAoAttr.enHdmiID = enHdmi;
    //stDrvAoAttr.stAOAttr.enAudioCode   = pstAudioAttr->enAudioCode;
    HDMI_UAUDIOCODING_2_KAUDIOCODING(pstAudioAttr->enAudioCode,stDrvAoAttr.stAOAttr.enAudioCode);
    stDrvAoAttr.stAOAttr.enSampleDepth = pstAudioAttr->enBitDepth;
    HDMI_USAMPLERATE_2_KSAMPLERATE(pstAudioAttr->enSampleRate,stDrvAoAttr.stAOAttr.enSampleFs);
    stDrvAoAttr.stAOAttr.enSoundIntf   = pstAudioAttr->enSoundIntf;
    stDrvAoAttr.stAOAttr.enChanels     = pstAudioAttr->u32Channels;
    stDrvAoAttr.stAOAttr.bDownSample   = pstAudioAttr->u8DownSampleParm;
    HDMI_INFO("SoundIntf:%d, AudioCode:%d, SampleRate:%d, Channels:%d, BitDepth:%d\n",pstAudioAttr->enSoundIntf,pstAudioAttr->enAudioCode,pstAudioAttr->enSampleRate,pstAudioAttr->u32Channels,pstAudioAttr->enBitDepth);
    s32Ret = HdmiExtIoctl(CMD_HDMI_SET_AO_ATTR, &stDrvAoAttr);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("set ao attr err!:0x%x\n",s32Ret);
	    return s32Ret;
    }

    return s32Ret;
}

static HI_S32 IntfkColorSpaceConv(HDMI_VIDEO_ATTR_S *pstSrcAttr, HDMI_VO_ATTR_S *pstDestAttr)
{
    CHECK_HDMI_PTR_NULL(pstSrcAttr);
    CHECK_HDMI_PTR_NULL(pstDestAttr);
    switch (pstSrcAttr->u32ColorSpace)
    {
        case HI_DRV_CS_BT601_YUV_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
            pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_601;
            break;
        case HI_DRV_CS_BT601_YUV_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
            pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_601;
            break;
        case HI_DRV_CS_BT601_RGB_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_LIMITED;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_601;
            break;
        case HI_DRV_CS_BT601_RGB_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_FULL;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_601;
            break;
        case HI_DRV_CS_BT709_YUV_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
            pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_709;
            break;
        case HI_DRV_CS_BT709_YUV_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
            pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_709;
            break;
        case HI_DRV_CS_BT709_RGB_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_LIMITED;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_709;
            break;
        case HI_DRV_CS_BT709_RGB_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_709;
            break;
        case HI_DRV_CS_BT2020_YUV_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__EXTENDED;
            pstDestAttr->enExtendedColorimetry = (HI_DRV_BT2020_CONSTANCE_LUMINACE_SYS == pstSrcAttr->enBt2020Lum)? \
                HDMI_EXTENDED_COLORIMETRY_2020_const_luminous: HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous;
            break;
        case HI_DRV_CS_BT2020_YUV_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_YCbCr444;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__EXTENDED;
            pstDestAttr->enExtendedColorimetry = (HI_DRV_BT2020_CONSTANCE_LUMINACE_SYS == pstSrcAttr->enBt2020Lum)? \
                HDMI_EXTENDED_COLORIMETRY_2020_const_luminous: HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous;
            break;
        case HI_DRV_CS_BT2020_RGB_LIMITED:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_LIMITED;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__EXTENDED;
            pstDestAttr->enExtendedColorimetry = (HI_DRV_BT2020_CONSTANCE_LUMINACE_SYS == pstSrcAttr->enBt2020Lum)? \
                HDMI_EXTENDED_COLORIMETRY_2020_const_luminous: HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous;
            break;
        case HI_DRV_CS_BT2020_RGB_FULL:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_FULL;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__EXTENDED;
            pstDestAttr->enExtendedColorimetry = (HI_DRV_BT2020_CONSTANCE_LUMINACE_SYS == pstSrcAttr->enBt2020Lum)? \
                HDMI_EXTENDED_COLORIMETRY_2020_const_luminous: HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous;
            break;
        default:
            pstDestAttr->enInColorSpace    = HDMI_COLORSPACE_RGB;
            pstDestAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_LIMITED;
			pstDestAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstDestAttr->enColorimetry     = HDMI_COLORIMETRY__ITU_709;
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 IntfkDisp2HdmiVoAttr(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S *pstDestAttr, HDMI_VIDEO_ATTR_S *pstSrcAttr)
{
    HI_BOOL bAspectIs43 = HI_FALSE;

    CHECK_HDMI_PTR_NULL(pstHdmiDev);
    CHECK_HDMI_PTR_NULL(pstDestAttr);
	CHECK_HDMI_PTR_NULL(pstSrcAttr);

    HDMI_UDEEPCOLOR_2_KDEEPCOLOR(pstSrcAttr->enInBitDepth, pstDestAttr->enInBitDepth);
    pstDestAttr->enStereoMode      = pstSrcAttr->enStereoMode;
    pstDestAttr->u32DispFmt        = (HI_U32)pstSrcAttr->enVideoFmt;
    pstDestAttr->enVideoTiming     = DispFmt2HdmiTiming(pstHdmiDev, pstSrcAttr->enVideoFmt);
    pstDestAttr->u32ClkFs          = pstSrcAttr->u32ClkFs * pstSrcAttr->u32PixelRepeat;
    pstDestAttr->u32PixelRepeat    = pstSrcAttr->u32PixelRepeat;
    pstDestAttr->enActiveAspect    = HDMI_ACTIVE_ASPECT_PICTURE;

    IntfkColorSpaceConv(pstSrcAttr, pstDestAttr);

    bAspectIs43 = (  (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_640X480P_60000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_720X480P_60000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_720X576P_50000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X240P_60000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X288P_50000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X480I_60000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576I_50000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576P_50000)
                  || (pstDestAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576I_60000));

    if (HI_TRUE == bAspectIs43)
    {
        pstDestAttr->enPictureAspect = HDMI_PICTURE_ASPECT_4_3;
	}
    else if(   pstDestAttr->enVideoTiming <= HDMI_VIDEO_TIMING_4096X2160P_60000
            && pstDestAttr->enVideoTiming >= HDMI_VIDEO_TIMING_4096X2160P_24000)
    {
        pstDestAttr->enPictureAspect = HDMI_PICTURE_ASPECT_256_135;
    }
    else
    {
        pstDestAttr->enPictureAspect = HDMI_PICTURE_ASPECT_16_9;
    }

    pstDestAttr->bVSyncPol = pstSrcAttr->bVSyncPol;
    pstDestAttr->bHSyncPol = pstSrcAttr->bHSyncPol;
    pstDestAttr->bDEPol    = pstSrcAttr->bDEPol;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_PreFormat(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32   s32Ret     = HI_FAILURE;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32PreformatInTime = HDMI_OSAL_GetTimeInMs();

    s32Ret = HdmiExtIoctl(CMD_HDMI_STOP, &enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("stop hdmi err!:0x%x\n",s32Ret);
	    return s32Ret;
    }
    HDMI_INFO("HI_DRV_HDMI_PreFormat...\n");
    pstHdmiDev->stIntfStatus.u32PreformatOutTime = HDMI_OSAL_GetTimeInMs();

    return s32Ret;
}

HI_S32 HI_DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HDMI_VIDEO_ATTR_S *pstVideoAttr)
{
    HI_S32              s32Ret  = HI_FAILURE;
    DRV_HDMI_VO_ATTR_S  stDrvVoAttr;
    HDMI_AUDIO_ATTR_S   stAudioAttr;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_PTR_NULL(pstVideoAttr);
    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32SetformatInTime = HDMI_OSAL_GetTimeInMs();

    memset(&stDrvVoAttr, 0, sizeof(DRV_HDMI_VO_ATTR_S));

    HDMI_INFO("VideoFmt:%d, StereoMode:%d, ClkFs:%d, PixelRepeat:%d, ColorSpace:%d\n",pstVideoAttr->enVideoFmt,pstVideoAttr->enStereoMode,
        pstVideoAttr->u32ClkFs,pstVideoAttr->u32PixelRepeat,pstVideoAttr->u32ColorSpace);

    IntfkDisp2HdmiVoAttr(GetHdmiDevice(enHdmi),&stDrvVoAttr.stVOAttr, pstVideoAttr);

    stDrvVoAttr.enHdmiID = enHdmi;
    s32Ret = HdmiExtIoctl(CMD_HDMI_SET_VO_ATTR, &stDrvVoAttr);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("set vo attr err!:0x%x\n",s32Ret);
	    return s32Ret;
    }
    s32Ret = HI_DRV_HDMI_GetAOAttr(enHdmi, &stAudioAttr);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("HI_DRV_HDMI_GetAOAttr fail\n");
	    return s32Ret;
    }
    s32Ret = HI_DRV_HDMI_SetAOAttr(enHdmi, &stAudioAttr);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("HI_DRV_HDMI_SetAOAttr fail\n");
	    return s32Ret;
    }
    s32Ret = HdmiExtIoctl(CMD_HDMI_START, &enHdmi);

    pstHdmiDev->stIntfStatus.u32SetformatOutTime = HDMI_OSAL_GetTimeInMs();

    return s32Ret;
}

#ifdef HDMI_HDR_SUPPORT

static HI_VOID Disp2HdmidrAttr(HDMI_DEVICE_S* pstHdmiDev, HDMI_HDR_ATTR_S *pstDrvHdrAttr, HI_DRV_HDMI_HDR_ATTR_S *pstHdrAttr)
{
    CHECK_HDMI_PTR_NULL_NO_RET(pstHdmiDev);
    CHECK_HDMI_PTR_NULL_NO_RET(pstDrvHdrAttr);
    CHECK_HDMI_PTR_NULL_NO_RET(pstHdrAttr);

    pstDrvHdrAttr->enHdrMode        = pstHdrAttr->enHdrMode;
    pstDrvHdrAttr->enUserHdrMode    = pstHdmiDev->stAttr.stHdrAttr.enUserHdrMode;
    pstDrvHdrAttr->enColorimetry    = pstHdrAttr->enColorimetry;
    pstDrvHdrAttr->enEotfType       = pstHdrAttr->enEotfType;
    pstDrvHdrAttr->enMetadataId     = pstHdrAttr->enMetadataId;
    memcpy(&(pstDrvHdrAttr->unDescriptor), &(pstHdrAttr->unDescriptor), sizeof(HDMI_META_DESCRIPTOR_UN));
}

HI_S32 HI_DRV_HDMI_SetHdrMode(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_HDR_USERMODE_E enUserHdrMode)
{
    HI_S32   s32Ret = HI_SUCCESS;
    HDMI_DEVICE_S       *pstHdmiDev = HI_NULL;
    HDMI_APP_ATTR_S     *pstAppAttr = HI_NULL;
    DRV_HDMI_VO_ATTR_S  stDrvVoAttr;
    HDMI_AUDIO_ATTR_S   stAudioAttr;
    HDMI_HDR_USERMODE_E enHdrMode = HDMI_HDR_USERMODE_SDR;

    CHECK_HDMI_OPEN(enHdmi);
    pstHdmiDev = GetHdmiDevice(enHdmi);
    if(pstHdmiDev == HI_NULL || enUserHdrMode >= HI_DRV_HDMI_HDR_USERMODE_BUTT)
    {
        HDMI_ERR("pstHdmiDev is null or enUserHdrMode is illegal.\n");
        return HI_FAILURE;
    }

    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);

    HDMI_UHDRMODE_2_KHDRMODE(enUserHdrMode, enHdrMode);

    HDMI_INFO("User HDR mode change, old(%d)->new(%d)\n", pstHdmiDev->stAttr.stHdrAttr.enUserHdrMode, enHdrMode);
    pstHdmiDev->stAttr.stHdrAttr.enUserHdrMode = enHdrMode;

    if(HDMI_HDR_USERMODE_DOLBY == enHdrMode)
    {
        if(   HDMI_DEEP_COLOR_24BIT != pstAppAttr->enDeepColorMode
           || HDMI_COLORSPACE_YCbCr444 != pstAppAttr->enOutColorSpace)
        {
            HDMI_INFO("force change OutColorSpace(%d) to YCbCr444 and DeepColorMode(%d) 8bit\n", pstAppAttr->enDeepColorMode, pstAppAttr->enDeepColorMode);

            memset(&stDrvVoAttr, 0, sizeof(DRV_HDMI_VO_ATTR_S));
            stDrvVoAttr.enHdmiID = enHdmi;

            s32Ret = HdmiExtIoctl(CMD_HDMI_STOP, &enHdmi);
            s32Ret = HdmiExtIoctl(CMD_HDMI_GET_VO_ATTR, &stDrvVoAttr);
            if (s32Ret != HI_SUCCESS)
            {
        	    HDMI_ERR("HI_DRV_HDMI_GetAOAttr fail\n");
        	    return s32Ret;
            }

            s32Ret = HdmiExtIoctl(CMD_HDMI_SET_VO_ATTR, &stDrvVoAttr);
            if (s32Ret != HI_SUCCESS)
            {
        	    HDMI_ERR("set vo attr err!:0x%x\n",s32Ret);
        	    return s32Ret;
            }
            s32Ret = HI_DRV_HDMI_GetAOAttr(enHdmi, &stAudioAttr);
            if (s32Ret != HI_SUCCESS)
            {
        	    HDMI_ERR("HI_DRV_HDMI_GetAOAttr fail\n");
        	    return s32Ret;
            }
            s32Ret = HI_DRV_HDMI_SetAOAttr(enHdmi, &stAudioAttr);
            if (s32Ret != HI_SUCCESS)
            {
        	    HDMI_ERR("HI_DRV_HDMI_SetAOAttr fail\n");
        	    return s32Ret;
            }
            s32Ret = HdmiExtIoctl(CMD_HDMI_START, &enHdmi);
        }
    }

    return s32Ret;
}

HI_S32 HI_DRV_HDMI_SetHdrAttr(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_HDR_ATTR_S *pstHdrAttr)
{
    HI_S32   s32Ret = HI_FAILURE;
    DRV_HDMI_HDR_S  stDrvHdr;
    HDMI_HDR_ATTR_S *pDrvHdrAttr;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    CHECK_HDMI_PTR_NULL(pstHdrAttr);
    CHECK_HDMI_PTR_NULL(pstHdmiDev);

    if(pstHdrAttr->enHdrMode == HI_DRV_HDMI_HDR_MODE_BUTT)
    {
        return HI_FAILURE;
    }

    memset(&stDrvHdr, 0, sizeof(DRV_HDMI_HDR_S));
    pDrvHdrAttr = &stDrvHdr.stHdrAttr;

    stDrvHdr.enHdmiID = enHdmi;
    Disp2HdmidrAttr(pstHdmiDev, pDrvHdrAttr, pstHdrAttr);

    s32Ret = HdmiExtIoctl(CMD_HDMI_SET_HDR_ATTR, &stDrvHdr);
    if(s32Ret != HI_SUCCESS)
    {
        HDMI_INFO("HI_DRV_HDMI_SetHdrAttr set fail... \n");
        return s32Ret;
    }

    return s32Ret;
}

#endif

HI_S32 HI_DRV_HDMI_GetVideoCapability(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_VIDEO_CAPABILITY_S *pstVideoCap)
{
    HI_S32  s32Ret = HI_FAILURE;
    DRV_HDMI_VIDEO_CAPABILITY_S stTmpVideoCap;

    CHECK_HDMI_PTR_NULL(pstVideoCap);
    CHECK_HDMI_OPEN(enHdmi);

    stTmpVideoCap.enHdmiID = enHdmi;
    s32Ret = HdmiExtIoctl(CMD_HDMI_GET_VIDEO_CAPABILITY, &stTmpVideoCap);
    if (s32Ret == HI_SUCCESS)
    {
        HDMI_MEMCPY(pstVideoCap,&stTmpVideoCap.stVideoCap, sizeof(HDMI_VIDEO_CAPABILITY_S));
    }

    return s32Ret;
}

HI_S32 HI_DRV_HDMI_Detach(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32   s32Ret = HI_FAILURE;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32DeAttachInTime = HDMI_OSAL_GetTimeInMs();

    DRV_HDMI_ThreadStateSet(GetHdmiDevice(enHdmi), HDMI_THREAD_STATE_STOP);
    s32Ret = HI_DRV_HDMI_PreFormat(enHdmi);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("HI_DRV_HDMI_PreFormat fail\n");
	    return s32Ret;
    }

    pstHdmiDev->stIntfStatus.u32DeAttachOutTime = HDMI_OSAL_GetTimeInMs();

    return s32Ret;
}

HI_S32 HI_DRV_HDMI_Attach(HI_UNF_HDMI_ID_E enHdmi, HDMI_VIDEO_ATTR_S *pstVideoAttr)
{
    HI_S32              s32Ret = HI_FAILURE;
    HDMI_DEVICE_S       *pstHdmiDev;

    CHECK_HDMI_PTR_NULL(pstVideoAttr);
    CHECK_HDMI_OPEN(enHdmi);

    pstHdmiDev = GetHdmiDevice(enHdmi);
    pstHdmiDev->stIntfStatus.u32AttachInTime = HDMI_OSAL_GetTimeInMs();

    s32Ret = HI_DRV_HDMI_SetFormat(enHdmi, pstVideoAttr);
    if (s32Ret != HI_SUCCESS)
    {
	    HDMI_ERR("HI_DRV_HDMI_SetFormat fail\n");
	    return s32Ret;
    }
    DRV_HDMI_ThreadStateSet(GetHdmiDevice(enHdmi), HDMI_THREAD_STATE_RUN);

    pstHdmiDev->stIntfStatus.u32AttachOutTime = HDMI_OSAL_GetTimeInMs();

    return s32Ret;
}
#endif
