#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../../.. && /bin/pwd)
    endif

    include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
VERSION := pq_v4_0
MOD_NAME := hi_pq

HAL_DIR := hal
MNG_DIR := mng

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

ifeq ($(CFG_HI_64BIT_SYSTEM),y)
ASM_DST_DIR := mng/64bit
else
ASM_DST_DIR := mng/32bit
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
CHIP := 3798cv200
#module :=  comm table zme csc dei
module :=  comm table tools zme csc dei tnr snr gfxzme gfxcsc db dm fmd dering deshoot artds dci sharpen acm hdr
endif


ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a),)
CHIP := 3798mv200
module :=  comm table tools zme csc dei fmd gfxcsc gfxzme hdr db dm tnr snr acc lccolor sharpen mcnr artds
endif

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DRV_REG_INCLUDE)                \
                -I$(MSP_DIR)/api/pq/include             \
                -I$(MSP_DIR)/api/higo/include           \
                -I$(MSP_DIR)/drv/pq/$(VERSION)/mng      \
                -I$(MSP_DIR)/drv/pq/$(VERSION)/mng/$(CHIP) \
                -I$(MSP_DIR)/drv/pq/$(VERSION)/hal/$(CHIP) \
                -I$(MSP_DIR)/drv/pq/$(VERSION)/hal/common  \
                -I$(MSP_DIR)/drv/pq/$(VERSION)

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

ifdef CFG_HI_SMMU_SUPPORT
EXTRA_CFLAGS += -DHI_PQ_SMMU_SUPPORT
endif

#EXTRA_CFLAGS += -DHI_PQ_4K_TNR_SUPPORT

$(MOD_NAME)-y +=    drv_pq.o                        \
                    drv_pq_intf.o                   \
                    drv_pq_comm.o                   \
                    drv_pq_table.o                  \
                    drv_pq_proc.o                   \
                    $(HAL_DIR)/common/pq_hal_comm.o 
					
ifneq ($(findstring table, $(module)),)
$(MOD_NAME)-y +=  $(HAL_DIR)/$(CHIP)/pq_hal_table_default.o
EXTRA_CFLAGS += -DPQ_ALG_TABLE
endif

ifneq ($(findstring  tools, $(module)),)
$(MOD_NAME)-y += drv_pq_tools.o  
EXTRA_CFLAGS += -DPQ_ALG_TOOLS      
endif

ifneq ($(findstring csc, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_csc.o 
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_csc.o   
EXTRA_CFLAGS += -DPQ_ALG_CSC
endif

ifneq ($(findstring zme, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_zme.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_zme.o  
EXTRA_CFLAGS += -DPQ_ALG_ZME
endif

ifneq ($(findstring  dei, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_dei.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_dei.o  
$(MOD_NAME)-y +=    $(ASM_DST_DIR)/pq_mng_dei_alg.o  
EXTRA_CFLAGS += -DPQ_ALG_DEI          
endif

ifneq ($(findstring  fmd, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_fmd.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_ifmd.o  
$(MOD_NAME)-y +=    $(ASM_DST_DIR)/pq_mng_ifmd_alg.o  
EXTRA_CFLAGS += -DPQ_ALG_FMD          
endif

ifneq ($(findstring  dci, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_dci.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_dci.o  
EXTRA_CFLAGS += -DPQ_ALG_DCI          
endif

ifneq ($(findstring  acm, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_acm.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_acm.o  
EXTRA_CFLAGS += -DPQ_ALG_ACM         
endif

ifneq ($(findstring  lccolor, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_lcacm.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_lcacm.o  
EXTRA_CFLAGS += -DPQ_ALG_LCACM         
endif

ifneq ($(findstring  acc, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_acc.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_acc.o  
EXTRA_CFLAGS += -DPQ_ALG_ACC
endif

ifneq ($(findstring  tnr, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_tnr.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/$(CHIP)/pq_mng_tnr.o  
EXTRA_CFLAGS += -DPQ_ALG_TNR        
endif

ifneq ($(findstring  mcnr, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_mcnr.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/$(CHIP)/pq_mng_mcnr.o  
EXTRA_CFLAGS += -DPQ_ALG_MCNR        
endif

ifneq ($(findstring  snr, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_snr.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_snr.o  
EXTRA_CFLAGS += -DPQ_ALG_SNR        
endif

ifneq ($(findstring  sharpen, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_sharpen.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_sharpen.o  
EXTRA_CFLAGS += -DPQ_ALG_SHARPEN        
endif

ifneq ($(findstring  db, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_db.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/$(CHIP)/pq_mng_db.o  
$(MOD_NAME)-y +=    $(ASM_DST_DIR)/$(CHIP)/pq_mng_db_alg.o
EXTRA_CFLAGS += -DPQ_ALG_DB       
endif

ifneq ($(findstring  dm, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_dm.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/$(CHIP)/pq_mng_dm.o  
$(MOD_NAME)-y +=    $(ASM_DST_DIR)/$(CHIP)/pq_mng_dm_alg.o  
EXTRA_CFLAGS += -DPQ_ALG_DM       
endif

ifneq ($(findstring  dering, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_dering.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_dering.o  
EXTRA_CFLAGS += -DPQ_ALG_DERING       
endif

ifneq ($(findstring  deshoot, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_deshoot.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_deshoot.o  
EXTRA_CFLAGS += -DPQ_ALG_DESHOOT       
endif

ifneq ($(findstring  artds, $(module)),)
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_artds.o  
EXTRA_CFLAGS += -DPQ_ALG_ARTDS      
endif

ifneq ($(findstring  gfxcsc, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_gfxcsc.o  
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_gfxbt2020.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_gfxcsc.o  
EXTRA_CFLAGS += -DPQ_ALG_GFXCSC      
endif

ifneq ($(findstring  gfxzme, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_gfxzme.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_gfxzme.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_gfxzme_coef.o 
EXTRA_CFLAGS += -DPQ_ALG_GFXZME       
endif

ifneq ($(findstring  hdr, $(module)),)
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_hdr.o  
$(MOD_NAME)-y +=    $(MNG_DIR)/pq_mng_hdr.o  
$(MOD_NAME)-y +=    $(ASM_DST_DIR)/pq_mng_hdr_alg.o  
EXTRA_CFLAGS += -DPQ_ALG_HDR      
endif

ifeq ($(CFG_HI_CHIP_TYPE), hi3798cv200)
ifneq ($(findstring db,$(module))$(findstring tnr,$(module))$(findstring snr,$(module)), )
$(MOD_NAME)-y +=    $(HAL_DIR)/$(CHIP)/pq_hal_algchange.o  
endif
endif

asflags-y := -I$(COMMON_DRV_INCLUDE)
ifeq ($(findstring -DHI_ADVCA_FUNCTION_RELEASE, $(CFG_HI_CFLAGS)), -DHI_ADVCA_FUNCTION_RELEASE)
asflags-y += -DCONFIG_REMOVE_STRING
endif

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: prepare unprepare all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR)  ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko

