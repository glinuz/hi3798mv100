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
            -I$(COMMON_DIR)/api/log \
            -I$(COMMON_DIR)/api/mmz \
            -I$(COMMON_DIR)/api/mem \
            -I$(COMMON_DIR)/api/memdev \
            -I$(COMMON_DIR)/api/module \
            -I$(COMMON_DIR)/api/include \
            -I$(COMMON_DIR)/drv/include

MSP_DEBUG := msp_debug

OBJS := msp_debug.o \
        losys.o \
        api/log/mpi_log.o \
        api/sys/hi_common.o \
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

OBJS := $(addprefix $(OBJ_DIR), $(OBJS))

LIB = -Wl,--start-group -lpthread -lm -Wl,--end-group

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all: $(OBJ_DIR)$(MSP_DEBUG)
	$(AT)test -d $(HI_ROOTFS_DIR)/usr/bin || mkdir -p $(HI_ROOTFS_DIR)/usr/bin
	$(AT)cp $(OBJ_DIR)$(MSP_DEBUG) $(HI_ROOTFS_DIR)/usr/bin/

clean:
	$(AT)rm -rf $(OBJ_DIR) $(MSP_DEBUG) api *.o $(HI_ROOTFS_DIR)/usr/bin/$(MSP_DEBUG)

$(OBJ_DIR)$(MSP_DEBUG): $(OBJS)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc -o $@ $^ $(LIB)

$(OBJ_DIR)%.o : $(COMMON_DIR)/%.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

$(OBJ_DIR)%.o : %.c
	$(AT)test -d $(dir $@) || mkdir -p $(dir $@)
	$(AT)$(CFG_HI_ARM_TOOLCHAINS_NAME)-gcc $(CFLAGS) $(FLOAT_OPTIONS) -o $@ -c $^

