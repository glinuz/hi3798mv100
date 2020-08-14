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
CFLAGS  :=  -I$(COMMON_DIR)/api/stat \
            -I$(COMMON_DIR)/api/sys \
            -I$(COMMON_DIR)/api/log \
            -I$(COMMON_DIR)/api/mem \
            -I$(COMMON_DIR)/api/mmz \
            -I$(COMMON_DIR)/api/flash/include \
            -I$(COMMON_DIR)/api/module \
            -I$(COMMON_DIR)/drv/module \
            -I$(COMMON_DIR)/drv/mem

SRCS := sys/hi_common.c \
        mmz/mpi_mmz.c \
        mem/mpi_mem.c \
        mem/mpi_memmap.c \
        module/mpi_module.c \
        log/mpi_log.c \
        stat/mpi_stat.c \
        memdev/mpi_memdev.c \
        userproc/mpi_userproc.c \
        osal/hi_osal.c

ifeq ($(HI_MEMMGR_SUPPORT),y)
CFLAGS += -DCMN_MMGR_SUPPORT

SRCS += mem/mpi_mutils.c  \
        mem/mpi_mmgr.c
endif

SRCS += flash/src/hi_flash.c \
        flash/src/nand.c \
        flash/src/spi_raw.c \
        flash/src/nand_raw.c \
        flash/src/emmc_raw.c \
        flash/src/cmdline_parts.c

LIB_NAME := libhi_common

HEADER_FILES := $(COMMON_DIR)/include/hi_common.h   \
                $(COMMON_DIR)/include/hi_debug.h    \
                $(COMMON_DIR)/include/hi_module.h   \
                $(COMMON_DIR)/include/hi_flash.h    \
                $(COMMON_DIR)/include/hi_type.h

include $(SDK_DIR)/hi_rules.mak

