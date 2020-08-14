#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(CURDIR)/../../..
include $(SDK_DIR)/base.mak
endif

export MKFLAGS

CC  := $(CROSS_COMPILE)gcc
AR  := $(CROSS_COMPILE)ar

gccincdir := $(shell $(CC) -print-file-name=include)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects := adp driver

ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
objects += ssa
endif

ifeq ($(CFG_HI_LOADER_SUPPORT),y)
objects += loader
endif

ifeq ($(CFG_PRODUCT_TYPE),android)
objects += android
endif

objects_clean       := $(addsuffix _clean,$(objects))
objects_install     := $(addsuffix _install,$(objects))
objects_uninstall   := $(addsuffix _uninstall,$(objects))

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(SDK_DIR)/source/boot/product
endif

ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
CFLAGS  :=  -g -Os -fno-strict-aliasing -fno-common -ffixed-r8 \
            -nostdinc -isystem $(MKFLAGS) \
            -fno-builtin -ffreestanding  -pipe -marm  -mabi=aapcs-linux  \
            -mno-thumb-interwork \
            -Wall -Wstrict-prototypes -fno-stack-protector

CFLAGS  +=  -DHI_MINIBOOT_SUPPORT=1

CFLAGS  +=  -I$(BOOT_DIR)/miniboot/include \
            -I$(BOOT_DIR)/miniboot/arm/$(BOOTCONFIG)/include \
            -I$(BOOT_DIR)/miniboot/arm/include
else
CFLAGS  :=  -g -Os -D__KERNEL__ -fno-strict-aliasing -fno-common -ffixed-r8 \
            -nostdinc -isystem $(gccincdir) $(MKFLAGS) \
            -fno-builtin -ffreestanding  -pipe -marm  -mabi=aapcs-linux  \
            -mno-thumb-interwork \
            -Wall -Wstrict-prototypes -fno-stack-protector

CFLAGS  +=  -I$(BOOT_DIR)/fastboot/include
endif

CFLAGS  += $(filter-out -fstack-protector --param ssp-buffer-size=4 ,$(CFG_HI_CFLAGS)) 

CFLAGS  +=  -I$(PRODUCT_DIR)/driver/include              \
            -I$(PRODUCT_DIR)/driver/include_inc          \
            -I$(PRODUCT_DIR)/loader/schedule             \
            -I$(PRODUCT_DIR)/logo/include                \
            -I$(PRODUCT_DIR)/driver/higo/gfx             \
            -I$(PRODUCT_DIR)/driver/higo/gfx/kernel-logo \
            -I$(PRODUCT_DIR)/driver/higo/imagedecode     \
            -I$(PRODUCT_DIR)/adp/include \
            -I$(PRODUCT_DIR)/pdm/include \
            -I$(COMMON_DIR)/include \
            -I$(COMMON_DIR)/drv/include \
            -I$(BOOT_DIR)/miniboot/app

CFLAGS  +=  -DCONFIG_ARM -D__ARM__

export CFLAGS

ifeq ($(CFG_HI_LOADER_SUPPORT),y)
CFLAGS += -DHI_LOADER_SUPPORT

ifeq ($(CFG_HI_APPLOADER_SUPPORT),y)
CFLAGS += -DHI_APPLOADER_SUPPORT
else ifeq ($(CFG_HI_BOOTLOADER_SUPPORT),y)
CFLAGS += -DHI_BOOTLOADER_SUPPORT
endif

endif

ifeq ($(CFG_HI_MCE_SUPPORT),y)
CFLAGS += -DHI_MCE_SUPPORT
endif

ifeq ($(CFG_HI_BUILD_WITH_HDMI_EDID),y)
CFLAGS += -DHI_HDMI_EDID_SUPPORT
endif

ifeq ($(CFG_HI_BUILD_WITH_HDMI_CEC),y)
CFLAGS += -DHI_HDMI_CEC_SUPPORT
endif

ifeq ($(CFG_HI_BUILD_WITH_KEYLED),y)
CFLAGS += -DHI_BUILD_WITH_KEYLED
    ifeq ($(CFG_HI_KEYLED_PT6964_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_PT6964
    endif
    ifeq ($(CFG_HI_KEYLED_PT6961_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_PT6961
    endif
    ifeq ($(CFG_HI_KEYLED_CT1642_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_CT1642
    endif
    ifeq ($(CFG_HI_KEYLED_74HC164_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_74HC164
    endif
    ifeq ($(CFG_HI_KEYLED_FD650_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_FD650
    endif
    ifeq ($(CFG_HI_KEYLED_GPIOKEY_SELECT),y)
    CFLAGS += -DHI_BOOT_KEYLED_GPIOKEY
    endif
endif

#--------------Related Android Macro Define -------------------------
ifeq ($(CFG_PRODUCT_TYPE),android)
CFLAGS  += -DHI_ANDROID_BOOT_SUPPORT

ifeq ($(CFG_HI_BUILD_WITH_IR), y)
CFLAGS  += -DHI_BOOT_IR_SUPPORT
ifeq ($(CFG_HI_REMOTE_RECOVERY_SUPPORT), y)
CFLAGS  += -DHI_REMOTE_RECOVERY_SUPPORT
endif
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
CFLAGS  += -DHI_BOOT_CHECK_TRUSTEDCORE
endif

ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv100 hi3796mv100 hi3716dv100 hi3798cv200 hi3798mv200 hi3798mv200_a),)
CFLAGS += -DHI_ADVCA_USB_BURN_KEY
endif
endif

endif
#-----------------------------------------------------------------

ifeq ($(CFG_HI_PM_POWERUP_MODE1_SUPPORT), y)
CFLAGS += -DHI_PM_POWERUP_MODE1_SUPPORT
endif

ifeq ($(CFG_HI_AVS_SUPPORT), y)
CFLAGS += -DHI_AVS_SUPPORT
endif

LIBS := libproduct.a
SUBLIBS := $(foreach sub,$(objects),$(sub)/lib$(sub).a)
MAINLIB := libmain.a
MAINOBJ := main.o

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#==================================================================
#                          all
#==================================================================
.PHONY: all clean install uninstall $(LIBS)

all: $(LIBS)

clean: $(objects_clean)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf $(MAINLIB)
	$(AT)rm -rf $(MAINOBJ)
	$(AT)rm -rf ar.mak

install: $(objects_install)

uninstall: $(objects_uninstall)

$(MAINLIB): $(MAINOBJ)
	$(AT)$(AR) -rc $@ $^

$(LIBS): $(objects) $(MAINLIB)
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE $@" >> ar.mak
	$(AT)$(foreach sublib,$(SUBLIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "ADDLIB $(MAINLIB)" >> ar.mak
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak

#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

$(objects):
	make -C $@ all

$(objects_clean):
	make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	make -C $(patsubst %_clean,%,$@) install

$(objects_uninstall):
	make -C $(patsubst %_clean,%,$@) uninstall

