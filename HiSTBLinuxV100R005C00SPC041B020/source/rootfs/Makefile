################################################################################
#    ./Makefile
#    Create By CaiZhiyong
#
################################################################################
# Compiler:    
#     make CROSS_COMPILE=arm-hisiv200-linux- PREFIX=~/tftp/
#
# Command format:
#    make <option>
#    Options:
#        CROSS_COMPILE=<cross compile>   - cross compile tools
#        PREFIX=<dir>                    - image path
#        clean                           - clean all subpath
#        <dir>.clean                     - clean dir
#        <dir>.build                     - build dir
#        <dir>.menuconfig                - menuconfig dir
#
# Examples:
#        make CROSS_COMPILE=arm-hisiv200-linux- PREFIX=~/tftp/
#
#    menuconfig "busybox" complie "busybox"
#        make busybox/.menuconfig
#        make busybox/.build
#        make busybox/.clean
#
######################export val################################################ 
#  CFG_HI_EXPORT_FLAG
#  SERVER_UNTILS_DIR
#  HI_ROOTFS_DIR
#  CFG_HI_NAND_SUPPORT
#  CFG_HI_SPI_SUPPORT
#  CFG_HI_EMMC_SUPPORT
#  CFG_HI_NAND_PAGE_SIZE
#  CFG_HI_NAND_ECC_TYPE
#  CFG_HI_ROOTFS_STRIP
#  CFG_HI_ROOTFS_CRAMFS
#  CFG_HI_ROOTFS_JFFS2
#  CFG_HI_ROOTFS_YAFFS
#  CFG_HI_ROOTFS_SQUASHFS
#  CFG_HI_BUSYBOX_CFG_NAME     //FIX ME:if uses busybox mdev or not?
#  CFG_HI_ETH_SUPPORT
#  CONFIG_SUPPORT_CA_RELEASE   //FIX ME:CA
#  CFG_HI_LIBCPP_SUPPORT
#  CFG_HI_LOADER_TYPE          //loader type: OS; UBOOT; n

######################### inside val ###########################################
#
#  CONFIG_MINI_ROOTFS          //FIX ME: delete it!
#  CROSS_COMPILE
#  PREFIX          //FIX ME
#  CONFIG_CA_RELEASE
#  BUILD_ROOT
#  MKFLAGS
#  CONFIG_LIBCPP_SUPPORT
#  CONFIG_BUSYBOX_MDEV     //FIX ME: if use busybox mdev or not?
#  CONFIG_ETH_SUPPORT
################################################################################

ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(shell pwd)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

CROSS_COMPILE         := $(CFG_HI_ARM_TOOLCHAINS_NAME)-

CONFIG_CA_RELEASE     := \
	$(if $(CONFIG_SUPPORT_CA_RELEASE),$(CONFIG_SUPPORT_CA_RELEASE),n)
CONFIG_LIBCPP_SUPPORT := \
	$(if $(CFG_HI_LIBCPP_SUPPORT),$(CFG_HI_LIBCPP_SUPPORT),n)
CONFIG_ETH_SUPPORT    := \
	$(if $(CFG_HI_ETH_SUPPORT),$(CFG_HI_ETH_SUPPORT),n)
CONFIG_ADVCA_TYPE    := \
	$(if $(CFG_HI_ADVCA_TYPE),$(CFG_HI_ADVCA_TYPE),n)

ifeq ($(findstring mdev,$(CFG_HI_BUSYBOX_CFG_NAME)),mdev)
CONFIG_BUSYBOX_MDEV   := y
endif

CFG_HI_LOADER_TYPE    := \
	$(if $(CFG_HI_LOADER_TYPE),$(CFG_HI_LOADER_TYPE),n)

PREFIX := $(HI_ROOTFS_DIR)

################################################################################
export AR=
export AS=
export LD=
export CPP=
export CC=
export NM=
export STRIP=
export OBJCOPY=
export OBJDUMP=
################################################################################
GREEN       = "\033[01;32m"
YELLOW      = "\033[01;33m"
NORMAL      = "\033[00m"
BLUE        = "\033[34m"
RED         = "\033[31m"
PURPLE      = "\033[35m"
CYAN        = "\033[36m"
UNDERLINE   = "\033[02m"

BUILD_ROOT  := $(shell pwd)
MKFLAGS     := PREFIX=$(PREFIX) CROSS_COMPILE=$(CROSS_COMPILE) \
		CONFIG_LIBCPP_SUPPORT=$(CONFIG_LIBCPP_SUPPORT) \
		CONFIG_ETH_SUPPORT=$(CONFIG_ETH_SUPPORT) \
		CONFIG_ADVCA_TYPE=$(CONFIG_ADVCA_TYPE) \
		CFG_HI_LOADER_TYPE=$(CFG_HI_LOADER_TYPE)
#		CONFIG_MINI_ROOTFS=$(CONFIG_MINI_ROOTFS)
PHONYS      :=

################################################################################
TOOLS-y := scripts busybox udev

# open source
TOOLS-$(CFG_HI_TOOL_DOSFS_SUPPORT)      += dosfstools
TOOLS-$(CFG_HI_TOOL_E2FSPROGS_SUPPORT)  += e2fsprogs
TOOLS-$(CFG_HI_TOOL_FILECAP_SUPPORT)    += filecap

ifeq ($(CFG_HI_ENG_MODE),y)
TOOLS-$(CFG_HI_TOOL_GDB_SUPPORT)        += gdb
endif

TOOLS-$(CFG_HI_TOOL_IPTABLES_SUPPORT)   += iptables
TOOLS-$(CFG_HI_TOOL_MTDUTILS_SUPPORT)   += mtd-utils
TOOLS-$(CFG_HI_TOOL_REG_SUPPORT)        += reg-tools-1.0.0
TOOLS-$(CFG_HI_TOOL_REG_UART_SUPPORT)   += reg-tools-uart
TOOLS-$(CFG_HI_TOOL_SANDBOX_SUPPORT)    += sandbox
TOOLS-$(CFG_HI_TOOL_PPPD_SUPPORT)       += pppd
TOOLS-$(CFG_HI_TOOL_MIITOOLS_SUPPORT)   += mii-tool
TOOLS-$(CFG_HI_TOOL_IPERF_SUPPORT)      += iperf
TOOLS-$(CFG_HI_LXC_CONTAINER_SUPPORT)   += lxc
TOOLS-$(CFG_HI_TOOL_SYSSTAT_SUPPORT)    += sysstat
TOOLS-$(CFG_HI_PROTOBUF_SUPPORT)        += protobuf

# hisilicon tools
TOOLS-$(CFG_HI_TOOL_SOC_SERVER)         += soc_server
TOOLS-$(CFG_HI_TOOL_MSP_DEBUG_SUPPORT)  += msp_debug
TOOLS-$(CFG_HI_TOOL_PQ_DEBUG_SUPPORT)   += pq_debug

################################################################################

PHONYS += all
all: check $(patsubst %,%.build,$(TOOLS-y))
	@echo;
	@echo 'File system is ready at "$(PREFIX)"'
	@echo;

################################################################################
# Check if path valid
PHONYS += check

check:
	$(AT)test -d $(PREFIX) || mkdir -p $(PREFIX)
	@if ! type $(CROSS_COMPILE)gcc > /dev/null 2>&1; then ( \
		echo "Can't find cross compiler $(CROSS_COMPILE)gcc.";\
		exit 1;\
	) fi

################################################################################

install: all

uninstall:
	$(AT)rm -rf $(HI_ROOTFS_DIR)/*

################################################################################

%.build: check
	@echo -e $(PURPLE)Build $(patsubst %.build,%,$@) ...$(NORMAL)
	make -C $(patsubst %.build,%,$@) $(MKFLAGS) -j1

%.clean:
	make -C $(patsubst %.clean,%,$@) $(MKFLAGS) clean

%.distclean:
	make -C $(patsubst %.distclean,%,$@) $(MKFLAGS) distclean

%.install:
	make -C $(patsubst %.install,%,$@) $(MKFLAGS) install

%.uninstall:
	make -C $(patsubst %.install,%,$@) $(MKFLAGS) uninstall

PHONYS += distclean
distclean: clean $(HOST_CLEAN)
	@for ix in $(wildcard $(BUILD_ROOT)/*); do ( \
		test ! -f $$ix/Makefile || make -C $$ix $(MKFLAGS) distclean; \
	) done

PHONYS += clean
clean: $(patsubst %,%.clean,$(TOOLS-y))
	$(AT)test ! -d $(PREFIX) || rm -rf $(PREFIX)

.PHONY: $(PHONY)
################################################################################
