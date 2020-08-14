################################################################################
#    ./scripts/Makefile
#    Create By CaiZhiYong
#    2011.06.19
################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX ?= $(HI_ROOTFS_DIR)

CROSS_COMPILE := $(CFG_HI_ARM_TOOLCHAINS_NAME)

HI_FPGA := $(if $(findstring fpga,$(CFG_HI_BOOT_REG_NAME)),-fpga,)

CONFIG_LIBCPP_SUPPORT := $(if $(CFG_HI_LIBCPP_SUPPORT),$(CFG_HI_LIBCPP_SUPPORT),n)

CONFIG_ETH_SUPPORT := $(if $(CONFIG_ETH_SUPPORT),$(CONFIG_ETH_SUPPORT),y)

################################################################################
INIT_SCRIPT    := etc/init.d/S99init

ROOTFS         := bin boot dev home lib var/run sys tmp opt proc nfsroot etc \
	mnt root share sbin usr/bin usr/sbin usr/lib usr/local/bin usr/share  \
	usr/local

TARGETS        := $(INIT_SCRIPT) $(ROOTFS) init
#	dev/console dev/ttyS000 dev/ttyAMA1 dev/ttyAMA0

################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(addprefix $(PREFIX)/,$(ROOTFS)):
ifeq ($(CFG_HI_LOADER_TYPE),OS)
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	$(AT)tar -xjf ./rootfs.tar.bz2 -C $(PREFIX)/../
	$(AT)tar -xjf ./rootfs_loader.tar.bz2 -C $(PREFIX)/../
else
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	$(AT)tar -xjf ./rootfs.tar.bz2 -C $(PREFIX)/../
	$(AT)tar -xjf ./libs/$(CFG_HI_ARM_TOOLCHAINS_NAME)$(HI_FPGA).tar.bz2 -C $(PREFIX)
ifeq ($(HI_USER_SPACE_LIB64),y)
	$(AT)tar -xjf ./libs/$(CFG_HI_AARCH64_TOOLCHAINS_NAME)$(HI_FPGA).tar.bz2 -C $(PREFIX)
endif
ifneq ($(CONFIG_LIBCPP_SUPPORT),y)
	rm -rf $(PREFIX)/lib/*c++.so*
	rm -rf $(PREFIX)/lib64/*c++.so*
endif

ifneq ($(CONFIG_ADVCA_TYPE),VERIMATRIX)
ifneq ($(CONFIG_ETH_SUPPORT),y)
	$(AT)rm -rf $(PREFIX)/etc/protocols
	$(AT)rm -rf $(PREFIX)/etc/services
	$(AT)rm -rf $(PREFIX)/etc/init.d/S80network
	$(AT)rm -rf $(PREFIX)/usr/share/udhcpc
endif
endif
endif

$(PREFIX)/$(INIT_SCRIPT):
	@echo "------------ $@ -------------------"
	$(AT)test -d $(@D) || mkdir -p $(@D)
	@(echo '#!/bin/sh'; \
		echo -e '# This file auto create by makefile\n'; \
		echo -e "echo \"Compile file system used $(CROSS_COMPILE)\""; \
		echo -e ${CONFIG_ROOTFS_CMD} 1>/dev/null 2>&1; \
	) > $@
	$(AT)chmod +xr $@

#${PREFIX}/dev/console \
#${PREFIX}/dev/ttyS000 \
#${PREFIX}/dev/ttyAMA1 \
#${PREFIX}/dev/ttyAMA0:
#	@test -d $(@D) || mkdir -p $(@D)
#	mknod ${PREFIX}/dev/ttyAMA0 c 204 64
#	mknod ${PREFIX}/dev/ttyAMA1 c 204 65
#	mknod ${PREFIX}/dev/ttyS000 c 204 64
#	mknod ${PREFIX}/dev/console c 5 1

$(PREFIX)/init:
	$(AT)cd ${PREFIX} && \
		ln -fs sbin/init init && \
	cd -

################################################################################

clean:
	@(for ix in $(TARGETS); do \
		if [ -d $(PREFIX)/$$ix ]; then ( \
			test ! -z "$$(ls -A ${PREFIX}/$${ix})" || \
				rm -rf $(PREFIX)/$$ix; \
		) else ( \
			test ! -e $(PREFIX)/$$ix || \
				rm -rf $(PREFIX)/$$ix; \
		) fi \
	done)
	rm -rf $(PREFIX)/../../scripts/hica_etc/*_etc

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################
