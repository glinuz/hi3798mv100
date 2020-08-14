# hi3798mx  hi3798cv2x hi3798cv2x
PLATFORM_DIR_NAME ?= hi3798cv2x

##########################################
# stub for mainline
##########################################
WITH_CHIP_HI3650 := 0
WITH_CHIP_HI6250 := 1
WITH_CHIP_HI3660 := 2
TRUSTEDCORE_CHIP_CHOOSE := 999

##########################################
# config trustedcore 
##########################################
LPAE_SUPPORT = 0

##########################################
# set product name for api get_target_product()
##########################################
TARGET_PRODUCT = DEVCHIP
TRUSTEDCORE_DEVCHIP_CFLAGS = -DTARGET_PRODUCT_VAL=\"$(TARGET_PRODUCT)\"

###########################################################
# definition for hi_linux
###########################################################
ifeq ($(strip $(OS_PLATFORM_TYPE)), LINUX)
ifeq ($(O),)
OUT = ${TRUSTEDCORE_DIR}/release/out
else
OUT = $(O)
endif

TOOLCHAINS_DIR = $(OUT)/toolchains
TOOLCHAINS_PACKAGE = ${TRUSTEDCORE_DIR}/toolchains/gcc.tar.bz2
CROSS_COMPILE1?=$(TOOLCHAINS_DIR)/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
CROSS_COMPILE2?=$(TOOLCHAINS_DIR)/gcc/linux-x86/arm/arm-linux-androideabi-4.8/bin/arm-linux-androideabi-

MAKE       =make
AR1        =$(CROSS_COMPILE1)ar
CC1        =$(CROSS_COMPILE1)gcc
LD1        =$(CROSS_COMPILE1)ld
OBJCOPY1   =$(CROSS_COMPILE1)objcopy
AR2        =$(CROSS_COMPILE2)ar
CC2        =$(CROSS_COMPILE2)gcc
LD2        =$(CROSS_COMPILE2)ld
OBJCOPY2   =$(CROSS_COMPILE2)objcopy
CFLAGS     =
LDFLAGS    =
RM         =-rm -rf
MKDIR      =-mkdir -p
COPY       =cp
TRUSTEDCORE_RELEASE_DIR=${TRUSTEDCORE_DIR}/release
endif

##########################################
# clear cflags
##########################################
TRUSTEDCORE_THIRDPARTY_LIBS :=

##########################################
# include platform chip config
##########################################
include $(TRUSTEDCORE_DIR)/platform/${PLATFORM_DIR}/${PLATFORM_DIR_NAME}/platform_chip.mk
include $(TRUSTEDCORE_DIR)/platform/${PLATFORM_DIR}/${PLATFORM_DIR_NAME}/platform_flags.mk
include $(TRUSTEDCORE_DIR)/platform/${PLATFORM_DIR}/platform_release_flags.mk
