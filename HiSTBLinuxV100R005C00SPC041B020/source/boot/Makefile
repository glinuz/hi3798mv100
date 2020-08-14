################################################################################
#    source/boot/Makefile
#    Create By Lyj
#    2012.09.17
################################################################################

ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

ifeq ($(CFG_PRODUCT_TYPE),android)
ifeq ($(TARGET_BUILD_VARIANT),user)
CFG_HI_USER_MODE := y
endif
endif

export CROSS_COMPILE=$(CFG_HI_ARM_TOOLCHAINS_NAME)-

ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
include miniboot.mak
else
include fastboot.mak
endif
