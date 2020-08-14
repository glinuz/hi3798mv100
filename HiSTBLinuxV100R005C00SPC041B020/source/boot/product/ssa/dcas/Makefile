################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR  := $(shell pwd)/../../../../..

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
BOOT_DIR := $(SDK_DIR)/source/boot/miniboot
else
BOOT_DIR := $(SDK_DIR)/source/boot/fastboot
endif

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(BOOT_DIR)/../product
endif

SSA_DIR := $(PRODUCT_DIR)/ssa

CFLAGS   += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS   += -I$(BOOT_DIR)/arch/arm/include
CFLAGS   += -I$(BOOT_DIR)/include
CFLAGS	 += -I$(PRODUCT_DIR)/driver/higo/include
CFLAGS   += -I$(LOGO_DIR)/include
CFLAGS   += -I$(COMPONENT_DIR)/loader/api/include
CFLAGS   += -I$(COMPONENT_DIR)/loader/api/src
CFLAGS   += -DHI_LOADER_BOOTLOADER
################################################################################
CFLAGS += -I$(SSA_DIR)/dcas/include

DCS_LIBS := libdcas.a

SUB_LIBS := $(DCS_LIBS)

DCS_OBJS := dcas_ssa.o dcas_image.o dcas_verify.o dcas_cipher.o dcas_flash.o

SSA_LIBS := ../libssa.a

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall $(DCS_LIBS)

all: $(SSA_LIBS)

clean:
	$(AT)rm -rf $(DCS_LIBS) $(DCS_OBJS)

$(SSA_LIBS): $(DCS_LIBS)
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE $@" >> ar.mak
	$(AT)$(foreach sublib,$(SUB_LIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak

install: all

uninstall:

$(DCS_LIBS): $(DCS_OBJS)
	$(AT)$(AR) -rc $@ $^
