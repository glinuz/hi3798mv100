#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(srctree),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(shell cd $(KERNEL_DIR)/../../.. && /bin/pwd)
    else
    SDK_DIR := $(shell cd $(CURDIR)/../../../.. && /bin/pwd)
    endif

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ifeq ($(CFG_HI_HDMI_SUPPORT_1_4),y)
HDMI_VER := hdmi_1_4
else ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)
HDMI_VER := hdmi_2_0
endif

HDMI_DRV_DIR := $(MSP_DIR)/drv/hdmi/$(HDMI_VER)

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/hdmi                   \
                -I$(MSP_DIR)/drv/hdmi/$(HDMI_VER)       \
                -I$(MSP_DIR)/api/sio                    \
                -I$(MSP_DIR)/api/higo/include       	\
                -I$(COMMON_DRV_INCLUDE)/$(CFG_HI_CHIP_TYPE)

ifeq ($(CFG_HI_TEE_SUPPORT), y)
EXTRA_CFLAGS += -I$(TEE_DRV_INCLUDE)
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_1_4),y)
EXTRA_CFLAGS += -I$(MSP_DIR)/drv/hdmi/$(HDMI_VER)/hdmi_drv

EXTRA_CFLAGS += -DDVI_SUPPORT

ifeq ($(CFG_PRODUCT_TYPE), android)
EXTRA_CFLAGS += -DANDROID_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP), y)
EXTRA_CFLAGS += -DHDCP_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_3D), y)
EXTRA_CFLAGS += -DSUPPORT_3D
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_CEC), y)
EXTRA_CFLAGS += -DCEC_SUPPORT
endif

EXTRA_CFLAGS += -DDEBUG_SUPPORT

else ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)
EXTRA_CFLAGS += -DCONFIG_HDMI_STB_SDK

EXTRA_CFLAGS += -DHDMI_DVI_SUPPORT
EXTRA_CFLAGS += -DHDMI_SCDC_SUPPORT

ifeq ($(CFG_PRODUCT_TYPE), android)
EXTRA_CFLAGS += -DHDMI_ANDROID_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_DVI), y)
EXTRA_CFLAGS += -DHDMI_DVI_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_HDCP), y)
EXTRA_CFLAGS += -DHDMI_HDCP_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_3D), y)
EXTRA_CFLAGS += -DHDMI_3D_SUPPORT
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_CEC), y)
EXTRA_CFLAGS += -DHDMI_CEC_SUPPORT
endif

ifeq ($(CFG_HI_HDR_SUPPORT), y)
EXTRA_CFLAGS += -DHDMI_HDR_SUPPORT
endif

ifeq ($(CFG_HI_TEE_SUPPORT), y)
EXTRA_CFLAGS += -DHDMI_TEE_SUPPORT
endif

ifeq ($(findstring fpga, $(CFG_HI_BOOT_REG_NAME)), fpga)
EXTRA_CFLAGS += -DHDMI_FPGA_SUPPORT
endif

EXTRA_CFLAGS += -DHDMI_DEBUG_SUPPORT
endif


ifdef CFG_HI_HDMI_DEBUG_MEM_SIZE
EXTRA_CFLAGS += -DABS_BUF_DEBUG_LEN=$(CFG_HI_HDMI_DEBUG_MEM_SIZE)
endif

ifeq ($(CFG_HI_HDMI_SUPPORT_DEBUG), y)
EXTRA_CFLAGS += -DHDMI_DEBUG
endif

#open it when emi is enable default
#EXTRA_CFLAGS += -DHDMI_EMI_ENABLE

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_hdmi

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

ifeq ($(CFG_HI_HDMI_SUPPORT_1_4),y)
$(MOD_NAME)-y   +=  $(HDMI_VER)/hdmi_intf_k.o                   \
                    $(HDMI_VER)/hdmi_intf.o                     \
                    $(HDMI_VER)/drv_hdmi.o                      \
                    $(HDMI_VER)/drv_global.o                    \
                    $(HDMI_VER)/drv_reg_proc.o                  \
                    $(HDMI_VER)/drv_compatibility.o             \
                    $(HDMI_VER)/hdmi_drv/drv_hlviic.o           \
                    $(HDMI_VER)/hdmi_drv/si_audio.o             \
                    $(HDMI_VER)/hdmi_drv/si_cec.o               \
                    $(HDMI_VER)/hdmi_drv/si_crc16.o             \
                    $(HDMI_VER)/hdmi_drv/si_de.o                \
                    $(HDMI_VER)/hdmi_drv/si_delay.o             \
                    $(HDMI_VER)/hdmi_drv/si_edid.o              \
                    $(HDMI_VER)/hdmi_drv/si_eeprom.o            \
                    $(HDMI_VER)/hdmi_drv/si_fifo.o              \
                    $(HDMI_VER)/hdmi_drv/si_hdcp.o              \
                    $(HDMI_VER)/hdmi_drv/si_hdmitx.o            \
                    $(HDMI_VER)/hdmi_drv/si_infofrm.o           \
                    $(HDMI_VER)/hdmi_drv/si_infpkttb.o          \
                    $(HDMI_VER)/hdmi_drv/si_isr.o               \
                    $(HDMI_VER)/hdmi_drv/si_mddc.o              \
                    $(HDMI_VER)/hdmi_drv/si_regio.o             \
                    $(HDMI_VER)/hdmi_drv/si_sha.o               \
                    $(HDMI_VER)/hdmi_drv/si_sha_hw.o            \
                    $(HDMI_VER)/hdmi_drv/si_timer.o             \
                    $(HDMI_VER)/hdmi_drv/si_txvidp.o            \
                    $(HDMI_VER)/hdmi_drv/si_txvptbl.o           \
                    $(HDMI_VER)/hdmi_drv/si_ucec.o              \
                    $(HDMI_VER)/hdmi_drv/si_video.o             \
                    $(HDMI_VER)/hdmi_drv/si_vmode.o             \
                    $(HDMI_VER)/hdmi_drv/si_vmtables.o          \
                    $(HDMI_VER)/hdmi_drv/si_phy.o

ifeq ($(CFG_HI_HDMI_SUPPORT_DEBUG), y)
$(MOD_NAME)-y += $(HDMI_VER)/drv_abs_debug.o
$(MOD_NAME)-y += $(HDMI_VER)/drv_hdmi_debug.o
endif

else ifeq ($(CFG_HI_HDMI_SUPPORT_2_0),y)


####################################################################
# hi3798cv200 HAL start
####################################################################
ifeq ($(CFG_HI_CHIP_TYPE),hi3798cv200_a)
CTRL_DIR := hal/ctrl/imagev200
EMI_DIR  := hal/emi
PRODUCT_DIR	:= product/HifoneB02
LINUX_VERSION := osal/hisiv200
endif

ifeq ($(CFG_HI_CHIP_TYPE),hi3798cv200)
CTRL_DIR := hal/ctrl/imagev200
EMI_DIR  := hal/emi
PRODUCT_DIR	:= product/Hi3798CV200
LINUX_VERSION := osal/hisiv200
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200 hi3798cv200_a),)
EXTRA_CFLAGS += -DHDMI_SUPPORT_LOGIC_SILICON

EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/hal
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(LINUX_VERSION)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(PRODUCT_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/common
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/cpi
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tpg
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/vtg
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/pll
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/cra
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx/hdcp
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx/scdc
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx/tx_hdmi
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx/videopath
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/driver/tx/tx_mhl
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(EMI_DIR)

#EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/edid
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/log
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/malloc
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/obj
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/seq
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/time
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/mtx
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/library/video

EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/platform

DRV_OBJS := $(HDMI_VER)/$(CTRL_DIR)/driver/common/si_drv_common.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/cpi/si_drv_cpi.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/si_drv_tx.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/tx_hdmi/si_mod_tx_hdmi.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/scdc/si_mod_tx_scdc.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/videopath/si_mod_videopath.o   \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/videopath/si_mod_videopath_cra.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/hdcp/si_mod_tx_hdcp.o   \
            $(HDMI_VER)/$(CTRL_DIR)/driver/cra/si_drv_cra.o  \
            $(HDMI_VER)/$(CTRL_DIR)/library/log/si_lib_log.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/malloc/si_lib_malloc.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/obj/si_lib_obj.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/seq/si_lib_seq.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/time/si_lib_time.o \
            $(HDMI_VER)/$(CTRL_DIR)/platform/platform.o \
            $(HDMI_VER)/$(CTRL_DIR)/platform/sii_time.o

#DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/driver/tpg/si_drv_tpg.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/vtg/si_drv_vtg.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/pll/si_drv_pll_vo.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/pll/si_drv_pll_vo_cfg.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/video/si_lib_video.o \
            $(HDMI_VER)/$(CTRL_DIR)/library/mtx/si_lib_mtx.o

#DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/driver/pebbles/si_drv_pebbles.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/tx_mhl/si_mod_tx_mhl.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/tx_mhl/emsc/si_mod_tx_cbus_emsc.o \
            $(HDMI_VER)/$(CTRL_DIR)/driver/tx/tx_mhl/mdt/si_mod_tx_cbus_mdt.o

DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal.o
DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/hal_cec.o

DRV_OBJS += $(HDMI_VER)/$(EMI_DIR)/hdmi_emi.o

endif
####################################################################
# hi3798cv200 HAL end
####################################################################

####################################################################
# hifonev300 HAL start
####################################################################
ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hifonev300),)
EXTRA_CFLAGS += -DHIFONEV300
endif 

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798mv200 hi3798mv200_a hi3796mv200),)
EXTRA_CFLAGS += -DHI3798MV200
EXTRA_CFLAGS += -DHDMI_EMI_ENABLE
endif 

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3798cv200),)
EXTRA_CFLAGS += -DHI3798CV200
endif 

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hifonev300 hifonev200 hi3798mv200 hi3798mv200_a hi3796mv200),)
EXTRA_CFLAGS += -DHDMI_SUPPORT_LOGIC_HISIV100
				  
CTRL_DIR := hal/ctrl/hisiv100
PHY_DIR  := hal/phy/hisiv100
EMI_DIR  := hal/emi
PRODUCT_DIR := product/hisiv100
LINUX_VERSION := osal/hisiv200

EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/hal
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(LINUX_VERSION)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(PRODUCT_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(PRODUCT_DIR)/regs
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(PHY_DIR)
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(PHY_DIR)/regs
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/regs
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/include
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(CTRL_DIR)/hwdriver
EXTRA_CFLAGS += -I$(HDMI_DRV_DIR)/$(EMI_DIR)


DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_aon.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_audio_path.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_cec.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_ctrl.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_hdcp.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_mcu.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_tx.o\
			$(HDMI_VER)/$(CTRL_DIR)/regs/hdmi_reg_video_path.o\

DRV_OBJS += $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_intf.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_machine.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_ddc.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_ncts.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_ctrl.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_scdc.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_hdcp14.o\
			$(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_hdcp22.o\
            $(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_cec.o\
			$(HDMI_VER)/$(CTRL_DIR)/hdmi_hal_debug.o\
							
							
DRV_OBJS += $(HDMI_VER)/$(PHY_DIR)/hdmi_hal_phy.o \
			$(HDMI_VER)/$(PHY_DIR)/regs/hdmi_reg_dphy.o

DRV_OBJS += $(HDMI_VER)/$(EMI_DIR)/hdmi_emi.o
			
DRV_OBJS += $(HDMI_VER)/$(PRODUCT_DIR)/regs/hdmi_reg_crg.o
            
endif
####################################################################
# hifonev300 HAL end
####################################################################
DRV_OBJS += $(HDMI_VER)/$(PRODUCT_DIR)/hdmi_product_define.o

DRV_OBJS += $(HDMI_VER)/$(LINUX_VERSION)/hdmi_osal.o 	

DRV_OBJS += $(HDMI_VER)/drv_hdmi_infoframe.o \
            $(HDMI_VER)/drv_hdmi_event.o \
            $(HDMI_VER)/drv_hdmi_edid.o \
            $(HDMI_VER)/drv_hdmi_edid_test.o \
            $(HDMI_VER)/drv_hdmi_intf.o \
            $(HDMI_VER)/drv_hdmi_intf_k.o \
			$(HDMI_VER)/drv_hdmi_common.o \
			$(HDMI_VER)/drv_hdmi_compatibility.o\
			$(HDMI_VER)/drv_hdmi_debug.o \
			$(HDMI_VER)/drv_hdmi_log.o\

			
ifeq ($(CFG_HI_PROC_SUPPORT),y)
DRV_OBJS += $(HDMI_VER)/drv_hdmi_proc.o
endif

ifeq ($(CFG_HI_TEE_SUPPORT), y)			
DRV_OBJS += $(HDMI_VER)/drv_hdmi_tee.o
endif

            
$(MOD_NAME)-y := $(DRV_OBJS)
endif

REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "modules.builtin"

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:

	
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(HI_KERNEL_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)find ./ -name "*.d" $(foreach file, $(REMOVED_FILES), -o -name $(file)) | xargs rm -rf

install: all
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(HI_MODULE_DIR)/

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko

