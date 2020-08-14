#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifneq ($(OUT),)
LOCAL_OUT := $(OUT)/drv/vdp
else
LOCAL_OUT := $(shell pwd)
endif

LIBS    := libsec_$(notdir $(LOCAL_OUT)).a
OBJS    :=  \
				drv_vdp_intf.o \
				drv/drv_verimatrix.o \
				drv/drv_nexguard.o \
				hal/98cv200/vdp_hal_comm.o \
				hal/98cv200/vdp_hal_ip_nxg.o \
				hal/98cv200/vdp_hal_ip_vmx.o \
				hal/98cv200/vdp_hal_ip_vmx_fromother.o 


CFLAGS += \
					-I$(HI_TEE_DRV_DIR)/vdp \
					-I$(HI_TEE_DRV_DIR)/vdp/drv \
					-I$(HI_TEE_DRV_DIR)/vdp/hal \
					-I$(HI_TEE_DRV_DIR)/vdp/hal/98cv200 \
					-I$(HI_TEE_DRV_DIR)/vdp/hal/98cv200/inc \
					-I$(HI_TEE_DRV_DIR)/timer

ALL_OUT_OBJ = $(foreach VAR,$(OBJS),$(addprefix $(LOCAL_OUT)/,$(VAR)))
ALL_OUT_LIBS = $(foreach VAR,$(LIBS),$(addprefix $(LOCAL_OUT)/,$(VAR)))

#===============================================================================
# rules
#===============================================================================
include $(HI_TEE_DRV_DIR)/stb_drv.mak
