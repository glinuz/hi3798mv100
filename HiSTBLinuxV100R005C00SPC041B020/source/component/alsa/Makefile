#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variable
#===============================================================================
ALSA_PREFIX    := /usr

ALSA_LIB       := alsa-lib-1.0.28
ALSA_UTILS     := alsa-utils-1.0.28
ALSA_CONFIG    := $(CURDIR)/etc/asound.conf

ALSA_LIB_DIR := $(HI_OUT_DIR)/obj/source/component/alsa
INSTALL_DIR := $(ALSA_LIB_DIR)/root

ALSA_LIB64_DIR := $(HI_OUT_DIR)/obj64/source/component/alsa
INSTALL_DIR64 := $(ALSA_LIB64_DIR)/root

TARGETS :=

ifeq ($(HI_USER_SPACE_LIB),y)
TARGETS += alsa_lib alsa_utils
endif

ifeq ($(HI_USER_SPACE_LIB64),y)
TARGETS += alsa_lib64 alsa_utils64
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean alsa_utils alsa_lib alsa_utils64 alsa_lib64

all: $(TARGETS)
	$(AT)test -d $(HI_INCLUDE_DIR) || mkdir -p $(HI_INCLUDE_DIR)
	$(AT)test -d $(HI_ROOTFS_DIR)/$(ALSA_PREFIX) || mkdir -p $(HI_ROOTFS_DIR)/$(ALSA_PREFIX)
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)cp -rf $(INSTALL_DIR)/$(ALSA_PREFIX)/include/* $(HI_INCLUDE_DIR)/
	$(AT)cp -arf $(INSTALL_DIR)/$(ALSA_PREFIX)/share  $(HI_ROOTFS_DIR)/$(ALSA_PREFIX)/
else ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)cp -rf $(INSTALL_DIR64)/$(ALSA_PREFIX)/include/* $(HI_INCLUDE_DIR)/
	$(AT)cp -arf $(INSTALL_DIR64)/$(ALSA_PREFIX)/share  $(HI_ROOTFS_DIR)/$(ALSA_PREFIX)/
endif
	$(AT)test -d $(HI_ROOTFS_DIR)/etc || mkdir -p $(HI_ROOTFS_DIR)/etc
	$(AT)cp -af $(ALSA_CONFIG) $(HI_ROOTFS_DIR)/etc/
ifeq ($(HI_USER_SPACE_LIB),y)
	$(AT)test -d $(HI_SHARED_LIB_DIR) || mkdir -p $(HI_SHARED_LIB_DIR)
	$(AT)cp -ralf $(INSTALL_DIR)/$(ALSA_PREFIX)/lib/* $(HI_SHARED_LIB_DIR)/
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)test -d $(HI_SHARED_LIB64_DIR) || mkdir -p $(HI_SHARED_LIB64_DIR)
	$(AT)cp -ralf $(INSTALL_DIR64)/$(ALSA_PREFIX)/lib/* $(HI_SHARED_LIB64_DIR)/
endif

clean:
	$(AT)rm -rf $(INSTALL_DIR) $(ALSA_LIB_DIR) $(ALSA_LIB_DIR)
	$(AT)rm -rf $(INSTALL_DIR64) $(ALSA_LIB64_DIR) $(ALSA_LIB64_DIR)
	$(AT)rm -rf $(HI_ROOTFS_DIR)/etc/$(notdir $(ALSA_CONFIG))
	$(AT)rm -rf $(HI_SHARED_LIB_DIR)/libasound.so*
	$(AT)rm -rf $(HI_SHARED_LIB64_DIR)/libasound.so*

alsa_utils: $(ALSA_LIB_DIR)/$(ALSA_UTILS)/tar
	$(AT)cd $(ALSA_LIB_DIR)/$(ALSA_UTILS) && make && make DESTDIR=$(INSTALL_DIR) install-strip

alsa_utils64: $(ALSA_LIB64_DIR)/$(ALSA_UTILS)/tar
	$(AT)cd $(ALSA_LIB64_DIR)/$(ALSA_UTILS) && make && make DESTDIR=$(INSTALL_DIR64) install-strip

alsa_lib: $(ALSA_LIB_DIR)/$(ALSA_LIB)/tar
	$(AT)cd $(ALSA_LIB_DIR)/$(ALSA_LIB) && make && make DESTDIR=$(INSTALL_DIR) install-strip

alsa_lib64: $(ALSA_LIB64_DIR)/$(ALSA_LIB)/tar
	$(AT)cd $(ALSA_LIB64_DIR)/$(ALSA_LIB) && make && make DESTDIR=$(INSTALL_DIR64) install-strip

$(ALSA_LIB_DIR)/$(ALSA_LIB)/tar:
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(ALSA_LIB_DIR)/$(ALSA_LIB) || (tar xf $(OPEN_SOURCE_DIR)/$(ALSA_LIB).tar.bz2 -C $(ALSA_LIB_DIR); \
        cd $(ALSA_LIB_DIR)/$(ALSA_LIB); \
        sh ./configure CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_ARM_TOOLCHAINS_NAME)-as LD=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_ARM_TOOLCHAINS_NAME)-nm --host=$(CFG_HI_ARM_TOOLCHAINS_NAME) STRIP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objdump --prefix=$(ALSA_PREFIX) --disable-python --enable-shared;)
	$(AT)touch $@

$(ALSA_LIB64_DIR)/$(ALSA_LIB)/tar:
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(ALSA_LIB64_DIR)/$(ALSA_LIB) || (tar xf $(OPEN_SOURCE_DIR)/$(ALSA_LIB).tar.bz2 -C $(ALSA_LIB64_DIR); \
        cd $(ALSA_LIB64_DIR)/$(ALSA_LIB); \
        sh ./configure CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-as LD=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-nm --host=$(CFG_HI_AARCH64_TOOLCHAINS_NAME) STRIP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-objdump  --prefix=$(ALSA_PREFIX) --disable-python --enable-shared;)
	$(AT)touch $@

$(ALSA_LIB_DIR)/$(ALSA_UTILS)/tar: alsa_lib
	$(AT)test -d $(INSTALL_DIR) || mkdir -p $(INSTALL_DIR)
	$(AT)test -d $(ALSA_LIB_DIR)/$(ALSA_UTILS) || (tar xf $(OPEN_SOURCE_DIR)/$(ALSA_UTILS).tar.bz2 -C $(ALSA_LIB_DIR); \
        cd $(ALSA_LIB_DIR)/$(ALSA_UTILS); \
        sh ./configure CC=$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_ARM_TOOLCHAINS_NAME)-as LD=$(CFG_HI_ARM_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_ARM_TOOLCHAINS_NAME)-nm --host=$(CFG_HI_ARM_TOOLCHAINS_NAME) STRIP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_ARM_TOOLCHAINS_NAME)-objdump --prefix=$(ALSA_PREFIX) --disable-alsaconf --disable-alsamixer --disable-xmlto --disable-nls CFLAGS=-I$(INSTALL_DIR)/$(ALSA_PREFIX)/include LDFLAGS=-L$(INSTALL_DIR)/$(ALSA_PREFIX)/lib;)
	$(AT)touch $@

$(ALSA_LIB64_DIR)/$(ALSA_UTILS)/tar: alsa_lib64
	$(AT)test -d $(INSTALL_DIR64) || mkdir -p $(INSTALL_DIR64)
	$(AT)test -d $(ALSA_LIB64_DIR)/$(ALSA_UTILS) || (tar xf $(OPEN_SOURCE_DIR)/$(ALSA_UTILS).tar.bz2 -C $(ALSA_LIB64_DIR); \
        cd $(ALSA_LIB64_DIR)/$(ALSA_UTILS); \
        sh ./configure CC=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-gcc CPP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-cpp AR=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ar AS=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-as LD=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-ld NM=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-nm --host=$(CFG_HI_AARCH64_TOOLCHAINS_NAME) STRIP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-strip OBJDUMP=$(CFG_HI_AARCH64_TOOLCHAINS_NAME)-objdump --prefix=$(ALSA_PREFIX) --disable-alsaconf --disable-alsamixer --disable-xmlto --disable-nls CFLAGS=-I$(INSTALL_DIR64)/$(ALSA_PREFIX)/include LDFLAGS=-L$(INSTALL_DIR64)/$(ALSA_PREFIX)/lib;)
	$(AT)touch $@
