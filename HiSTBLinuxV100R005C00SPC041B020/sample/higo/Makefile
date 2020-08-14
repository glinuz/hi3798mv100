#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_animation \
    sample_bitmapfont \
    sample_dec \
    sample_effect \
    sample_encoder \
    sample_fillrect \
    sample_gdev \
    sample_logo \
    sample_performance \
    sample_surface \
    sample_wm

ifeq ($(CFG_HIFB_SCROLLTEXT_SUPPORT),y)
SAMPLE_IMAGES += sample_scrolltext
endif

ifeq ($(CFG_HIFB_STEREO3D_SUPPORT),y)
SAMPLE_IMAGES += sample_stereo
endif

SAMPLE_IMAGES += sample_text

COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

DEPEND_LIBS := $(HI_LIBS) -leffect

include $(SAMPLE_DIR)/hi_sample_rules.mak