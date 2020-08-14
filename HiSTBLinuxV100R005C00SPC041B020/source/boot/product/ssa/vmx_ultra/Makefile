################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../..

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

################################################################################
CFLAGS += -I$(SSA_DIR)/vmx_ultra/include

VMX_LIBS := libvmx.a

SUB_LIBS := $(VMX_LIBS)

VMX_OBJS := vmx_ssa.o vmx_image.o vmx_cmd.o vmx_lib/vmx_verify_stub.o

SSA_LIBS := ../libssa.a

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall $(VMX_LIBS)

all: $(SSA_LIBS)

clean:
	$(AT)rm -rf $(VMX_LIBS) $(VMX_OBJS)

$(SSA_LIBS): $(VMX_LIBS)
	$(AT)rm -rf $@
	$(AT)rm -rf ar.mak
	$(AT)echo "CREATE $@" >> ar.mak
	$(AT)$(foreach sublib,$(SUB_LIBS),echo "ADDLIB $(sublib)" >> ar.mak;)
	$(AT)echo "SAVE" >> ar.mak
	$(AT)echo "END" >> ar.mak
	$(AT)$(AR) -M < ar.mak

install: all

uninstall:

$(VMX_LIBS): $(VMX_OBJS)
	$(AT)$(AR) -rc $@ $^
