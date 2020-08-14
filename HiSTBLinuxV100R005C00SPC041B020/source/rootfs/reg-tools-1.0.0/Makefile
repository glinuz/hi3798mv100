ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

PREFIX ?= $(HI_ROOTFS_DIR)

ifneq ($(HI_OUT_DIR),)
REGTOOLS_DIR := $(HI_OUT_DIR)/obj/source/rootfs/reg-tools-1.0.0
else
REGTOOLS_DIR := $(CURDIR)
endif

################################################################################
BUILD_DIR       := $(REGTOOLS_DIR)/tmp
HOST            := $(CFG_HI_ARM_TOOLCHAINS_NAME)
CROSS_COMPILE   := $(HOST)-
STRIP           := $(CROSS_COMPILE)strip
REGTOOLS        := reg-tools-1.0.0
TARGETS         := bin/btools
USER_CFLAGS     ?= -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=softfp -mfpu=vfpv3-d16
CFG_DEFS        := -DCHIP_TYPE_${CFG_HI_CHIP_TYPE}

LN              := $(shell if [ -x "/bin/ln" ]; then echo /bin/ln; else echo ln; fi)

export USER_CFLAGS
################################################################################

all: $(addprefix $(PREFIX)/,$(TARGETS))

$(PREFIX)/%: $(BUILD_DIR)/%
	test -d $(@D) || mkdir -p $(@D)
	install -v -t $(@D) $(BUILD_DIR)/$(REGTOOLS)/btools
	rm -rf $(PREFIX)/bin/him* $(PREFIX)/bin/hiv* $(PREFIX)/bin/hie*
	@cd $(PREFIX)/bin && (${LN} -sf btools himd; \
		${LN} -fs btools himd.l; \
		${LN} -fs btools himm;   \
		${LN} -fs btools himc;   \
		${LN} -fs btools hivd;   \
		${LN} -fs btools hier;   \
		${LN} -fs btools hiew;   \
		${LN} -fs btools hismd;  \
		${LN} -fs btools hismm)

$(addprefix $(BUILD_DIR)/,$(TARGETS)): \
    $(BUILD_DIR)/$(REGTOOLS)/tar
	@-rm -rf $(BUILD_DIR)/$(REGTOOLS)/source/tools/hi_unf_i2c.c
	@-rm -rf $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_log.c
	@-rm -rf $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_memmap.c
	@-rm -rf $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_mem.c
	@-rm -rf $(BUILD_DIR)/$(REGTOOLS)/source/tools/hi_osal.c
	@ln -s $(MSP_DIR)/api/i2c/unf_i2c.c $(BUILD_DIR)/$(REGTOOLS)/source/tools/hi_unf_i2c.c
	@ln -s $(COMMON_DIR)/api/log/mpi_log.c $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_log.c
	@ln -s $(COMMON_DIR)/api/mem/mpi_memmap.c $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_memmap.c
	@ln -s $(COMMON_DIR)/api/mem/mpi_mem.c $(BUILD_DIR)/$(REGTOOLS)/source/tools/mpi_mem.c
	@ln -s $(COMMON_DIR)/api/osal/hi_osal.c $(BUILD_DIR)/$(REGTOOLS)/source/tools/hi_osal.c
	@cd $(BUILD_DIR)/$(REGTOOLS) && \
		make install CROSS_COMPILE=$(CROSS_COMPILE) \
			PREFIX=$(PREFIX) \
			SRC_ROOT=$(SDK_DIR)/source \
			CFG_DEFS=$(CFG_DEFS) && \
	cd -

$(BUILD_DIR)/%/tar:
	test -d $(@D) || mkdir -p $(@D)
	tar -xjf $(notdir $(@:/tar=)).tar.bz2 -C $(BUILD_DIR)
	@touch $@

################################################################################

clean:
	@test ! -d $(BUILD_DIR) || rm -rf $(BUILD_DIR)

menuconfig:
	@echo "not support"

distclean: clean

install:
	@echo "not support"

################################################################################
.PHONY: clean menuconfig distclean install
################################################################################
