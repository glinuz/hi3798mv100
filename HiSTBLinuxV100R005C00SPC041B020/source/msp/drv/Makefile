#=======================================
#	export variable
#=======================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#=======================================
#	local variable
#======================================
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3718mv100 hi3719mv100 hi3716mv410),)
JPGE_SUPPORT := n
else
JPGE_SUPPORT := y
endif

objects := gpio ir pdm pq hdmi vo hifb tde jpeg png cipher otp advca ddr


ifneq ($(CFG_HI_RECOVERY_SUPPORT),y)
objects += i2c demux pm
endif

ifneq ($(CFG_HI_LOADER_APPLOADER),y)
ifneq ($(CFG_HI_RECOVERY_SUPPORT),y)
objects += sync adsp aiao adec vfmw vdec vpss mce omxvdec

ifeq ($(CFG_HI_PWM_SUPPORT),y)
objects += pwm
endif

ifeq ($(JPGE_SUPPORT),y)
objects += jpge
endif

endif
endif

ifeq ($(CFG_HI_GFX2D_SUPPORT),y)
objects += gfx2d
endif

ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
objects += frontend
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
objects += pvr
endif

ifeq ($(CFG_HI_GPU_SUPPORT),y)
objects += gpu
endif

ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
objects += keyled
endif

ifeq ($(CFG_HI_SPI_BUS_SUPPORT),y)
objects += spi
endif

ifeq ($(CFG_HI_SCI_SUPPORT),y)
objects += sci
endif

ifeq ($(CFG_HI_AENC_SUPPORT),y)
objects += aenc
endif

ifeq ($(CFG_HI_VENC_SUPPORT),y)
objects += venc
endif

ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
objects += ci
ifeq ($(CFG_HI_CI_DEV_CIMAXPLUS),y)
objects += cimaxplus
endif
endif

ifeq ($(CFG_HI_VI_SUPPORT),y)
objects += vi
objects += vad
endif

ifeq ($(CFG_HI_ADAC_SUPPORT),y)
objects += adac
endif

ifeq ($(HI_DRV_BUILDTYPE),y)
objects += base
endif

obj-y += ${addsuffix /,${objects}}

