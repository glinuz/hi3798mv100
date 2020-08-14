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
EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/vdec/vdec_v1.0         \
                -I$(MSP_DIR)/drv/vdec/vdec_v1.0         \
                -I$(MSP_DIR)/drv/vfmw/vfmw_v4.0/scene/stb  \
                -I$(MSP_DIR)/drv/vfmw/vfmw_v4.0

ifeq ($(CFG_HI_TEE_SUPPORT),y)
EXTRA_CFLAGS += -I$(TEE_DRV_INCLUDE)
EXTRA_CFLAGS += -I$(KERNEL_SRC_DIR)/drivers/hisilicon/sec_mmz
endif

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420),)
EXTRA_CFLAGS += -DHi3716MV410
endif

ifeq ($(CFG_HI_VDEC_DFS_SUPPORT),y)
EXTRA_CFLAGS += -DHI_VDEC_DFS_SUPPORT=1
else
EXTRA_CFLAGS += -DHI_VDEC_DFS_SUPPORT=0
endif

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Hi3716MV410 or Hi3716MV420 config start
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC=0
endif

ifneq "$(origin CFG_HI_VIDEO_MAX_REF_FRAME_NUM)" "undefined"
EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM)
endif

ifneq "$(origin CFG_HI_VIDEO_MAX_DISP_FRAME_NUM)" "undefined"
EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM)
endif

ifneq "$(origin CFG_HI_VIDEO_MAX_VDH_BUF)" "undefined"
EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF=$(CFG_HI_VIDEO_MAX_VDH_BUF)
endif
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
# Hi3716MV410 or Hi3716MV420 config end
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#===============================================================================
# 256M ddr memory config flags
#===============================================================================
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P_IN_256))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_256=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_256=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC_IN_256))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_256=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_256=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_2160P_IN_256))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_256=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_256=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_4K_IN_256))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_256=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_256=0
endif

EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM_IN_256=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM_IN_256)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM_IN_256=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM_IN_256)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF_IN_256=$(CFG_HI_VIDEO_MAX_VDH_BUF_IN_256)

#===============================================================================
# 512M ddr memory config flags
#===============================================================================
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P_IN_512))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_512=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_512=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC_IN_512))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_512=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_512=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_2160P_IN_512))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_512=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_512=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_4K_IN_512))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_512=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_512=0
endif

EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM_IN_512=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM_IN_512)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM_IN_512=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM_IN_512)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF_IN_512=$(CFG_HI_VIDEO_MAX_VDH_BUF_IN_512)

#===============================================================================
# 768M ddr memory config flags
#===============================================================================
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P_IN_768))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_768=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_768=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC_IN_768))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_768=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_768=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_2160P_IN_768))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_768=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_768=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_4K_IN_768))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_768=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_768=0
endif

EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM_IN_768=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM_IN_768)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM_IN_768=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM_IN_768)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF_IN_768=$(CFG_HI_VIDEO_MAX_VDH_BUF_IN_768)
#===============================================================================
# 1024M ddr memory config flags
#===============================================================================
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P_IN_1024))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_1024=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_1024=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC_IN_1024))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_1024=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_1024=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_2160P_IN_1024))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_1024=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_1024=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_4K_IN_1024))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_1024=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_1024=0
endif

EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM_IN_1024=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM_IN_1024)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM_IN_1024)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF_IN_1024=$(CFG_HI_VIDEO_MAX_VDH_BUF_IN_1024)

#===============================================================================
# 2048M ddr memory config flags
#===============================================================================
ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_1080P_IN_2048))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_2048=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_1080P_IN_2048=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_MVC_IN_2048))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_2048=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_MVC_IN_2048=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_2160P_IN_2048))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_2048=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_2160P_IN_2048=0
endif

ifeq (y, $(CFG_HI_VIDEO_PRE_CAP_4K_IN_2048))
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_2048=1
else
EXTRA_CFLAGS += -DHI_VIDEO_PRE_CAP_4K_IN_2048=0
endif

EXTRA_CFLAGS += -DHI_VIDEO_MAX_REF_FRAME_NUM_IN_2048=$(CFG_HI_VIDEO_MAX_REF_FRAME_NUM_IN_2048)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048=$(CFG_HI_VIDEO_MAX_DISP_FRAME_NUM_IN_2048)

EXTRA_CFLAGS += -DHI_VIDEO_MAX_VDH_BUF_IN_2048=$(CFG_HI_VIDEO_MAX_VDH_BUF_IN_2048)

#===============================================================================
# memory config end
#===============================================================================

EXTRA_CFLAGS += -DHI_VDEC_MAX_INSTANCE=16

ifeq (y,$(CFG_HI_VDEC_DEINTERLACE_SUPPORT))
EXTRA_CFLAGS += -DHI_VDEC_DEINTERLACE_SUPPORT=1
else
EXTRA_CFLAGS += -DHI_VDEC_DEINTERLACE_SUPPORT=0
endif

ifeq (y,$(CFG_HI_VDEC_SVDEC_SUPPORT))
EXTRA_CFLAGS += -DHI_VDEC_SVDEC_SUPPORT=1
else
EXTRA_CFLAGS += -DHI_VDEC_SVDEC_SUPPORT=0
endif

ifeq (HD_SIMPLE,$(CFG_HI_VFMW_DECODE_CAPABILITY))
EXTRA_CFLAGS += -DHI_VDEC_HD_SIMPLE=1
else
EXTRA_CFLAGS += -DHI_VDEC_HD_SIMPLE=0
endif

ifeq (y,$(CFG_HI_VDEC_USERDATA_CC_SUPPORT))
EXTRA_CFLAGS += -DHI_VDEC_USERDATA_CC_SUPPORT=1
else
EXTRA_CFLAGS += -DHI_VDEC_USERDATA_CC_SUPPORT=0
endif

ifeq ($(CFG_HI_TEE_SUPPORT),y)
EXTRA_CFLAGS += -DHI_TEE_SUPPORT
endif

ifeq (y,$(CFG_HI_VDEC_PRE_ALLOC_VDHMMZ_SUPPORT))
EXTRA_CFLAGS += -DPRE_ALLOC_VDEC_VDH_MMZ=1
else
EXTRA_CFLAGS += -DPRE_ALLOC_VDEC_VDH_MMZ=0
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(MSP_DIR)/drv/vfmw/vfmw_v4.0/Module.symvers

MOD_NAME := hi_vdec

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y   :=  drv_vdec_buf_mng.o      \
                    drv_vdec_intf.o         \
                    drv_vdec_intf_k.o       \
                    drv_vdec_pts_recv.o     \
                    drv_vdec_alg_frd.o      \
                    drv_vdec_alg_api_frd.o

ifeq (y,$(CFG_HI_VDEC_USERDATA_CC_SUPPORT))
$(MOD_NAME)-y   += drv_vdec_usrdata.o
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

