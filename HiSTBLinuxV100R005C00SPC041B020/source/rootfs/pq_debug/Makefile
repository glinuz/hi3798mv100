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
OBJ_DIR := $(HI_OUT_DIR)/obj/$(subst $(SDK_DIR)/,,$(CURDIR))/

CFLAGS  :=  $(CFG_HI_CFLAGS)

CFLAGS  +=  -I$(COMMON_DIR)/include \
            -I$(COMMON_DIR)/api/flash/include \
            -I$(COMMON_DIR)/api/log \
            -I$(COMMON_DIR)/api/mmz \
            -I$(COMMON_DIR)/api/mem \
            -I$(COMMON_DIR)/api/memdev \
            -I$(COMMON_DIR)/api/module \
            -I$(COMMON_DIR)/api/include \
            -I$(COMMON_DIR)/drv/include \
            -I$(MSP_DIR)/include \
            -I$(MSP_DIR)/api/include \
            -I$(MSP_DIR)/drv/include

PQ_DEBUG := pq_debug

OBJS := api/log/mpi_log.o \
    	api/sys/hi_common.o \
    	api/flash/src/hi_flash.o \
    	api/flash/src/emmc_raw.o \
    	api/flash/src/nand_raw.o \
    	api/flash/src/spi_raw.o \
    	api/flash/src/cmdline_parts.o \
    	api/flash/src/nand.o \
    	api/mem/mpi_memmap.o \
    	api/mem/mpi_mutils.o \
    	api/mem/mpi_mmgr.o \
    	api/module/mpi_module.o \
    	api/stat/mpi_stat.o \
    	api/mmz/mpi_mmz.o \
    	api/mem/mpi_mem.o \
    	api/memdev/mpi_memdev.o \
    	api/userproc/mpi_userproc.o \
    	api/osal/hi_osal.o

OBJS += mpi_pq.o

OBJS += pq_debug.o pq_parse.o pq_comm.o

OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

LIB = -Wl,--start-group -lpthread -lm -Wl,--end-group

#===============================================================================
# rules
#===============================================================================
all: $(OBJ_DIR)$(PQ_DEBUG)
	$(AT)test -d $(HI_ROOTFS_DIR)/usr/bin || mkdir -p $(HI_ROOTFS_DIR)/usr/bin
	$(AT)cp $(OBJ_DIR)$(PQ_DEBUG) $(HI_ROOTFS_DIR)/usr/bin/

clean:
	$(AT)rm -rf $(OBJ_DIR) $(PQ_DEBUG) api *.o $(HI_ROOTFS_DIR)/usr/bin/$(PQ_DEBUG)

$(OBJ_DIR)$(PQ_DEBUG): $(OBJS)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc -o $@ $^ $(LIB)

$(OBJ_DIR)%.o : $(MSP_DIR)/api/pq/%.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR)%.o : $(COMMON_DIR)/%.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR)%.o : %.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^
