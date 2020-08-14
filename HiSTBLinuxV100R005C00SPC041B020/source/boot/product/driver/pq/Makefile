#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../../..
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_MINIBOOT_SUPPORT),y)
BOOT_DIR  := $(SDK_DIR)/source/boot/miniboot
else
BOOT_DIR  := $(SDK_DIR)/source/boot/fastboot
endif

ifneq ($(O),)
PRODUCT_DIR := $(O)/product
else
PRODUCT_DIR := $(BOOT_DIR)/../product
endif

PQ_DIR := $(PRODUCT_DIR)/driver/pq

CFLAGS  += -I$(PRODUCT_DIR)/driver/include
CFLAGS  += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS  += -I$(BOOT_DIR)/arch/arm/include
CFLAGS  += -I$(BOOT_DIR)/include
CFLAGS  += -DHI_BUILD_IN_BOOT
OBJS    := $(PQ_DIR)/drv_pq.o

ifeq ($(CFG_HI_PQ_V1_0), y)
CFLAGS  += -DHI_PQ_VERSION_V1
else ifeq ($(CFG_HI_PQ_V2_0), y)
CFLAGS  += -DHI_PQ_VERSION_V2
else ifeq ($(CFG_HI_PQ_V3_0), y)
CFLAGS  += -DHI_PQ_VERSION_V3
else ifeq ($(CFG_HI_PQ_V4_0), y)
CFLAGS  += -DHI_PQ_VERSION_V4
OBJS    += $(PQ_DIR)/pq_hal_gfxcsc.o $(PQ_DIR)/pq_mng_gfxcsc.o 
OBJS    += $(PQ_DIR)/pq_mng_gfxzme.o $(PQ_DIR)/pq_hal_gfxzme.o $(PQ_DIR)/pq_mng_gfxzme_coef.o 
OBJS    += $(PQ_DIR)/pq_hal_comm.o
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
CHIP := 3798cv200
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
CHIP := 3798mv200
endif

LIBS    := $(PQ_DIR)/libpq.a

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall $(LIBS) prepare

all: $(LIBS)

prepare:
ifeq ($(CFG_HI_PQ_V4_0), y)
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/mng/pq_mng_gfxcsc.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/mng/pq_mng_gfxzme.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/mng/pq_mng_gfxzme_coef.h $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/mng/pq_mng_gfxzme_coef.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/$(CHIP)/pq_hal_gfxcsc.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/$(CHIP)/pq_hal_gfxcsc.h $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/$(CHIP)/pq_hal_gfxzme.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/$(CHIP)/pq_hal_gfxzme.h $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/common/pq_hal_comm.c $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/hal/common/pq_hal_comm.h $(PQ_DIR)/
	$(AT)cp -rf $(MSP_DIR)/drv/pq/pq_v4_0/include/drv_pq_ext.h $(PQ_DIR)/
endif

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)

install: all

uninstall:

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^
