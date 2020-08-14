#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/../../../.. && /bin/pwd)

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
LOADER_INC := -I$(MSP_DIR)/api/higo/include
LOADER_INC += -I$(CURDIR)/include
LOADER_INC += -I$(CURDIR)/common
LOADER_INC += -I$(CURDIR)/ui
LOADER_INC += -I$(COMPONENT_DIR)/loader/api/include
LOADER_INC += -I$(COMPONENT_DIR)/loader/api/src

LOADER_DEF += ${CFG_HI_BOARD_CONFIGS}
LOADER_DEF += -DCHIP_TYPE_${CFG_HI_CHIP_TYPE} 

LOADER_SRC += main.c download.c protocol.c burn.c burn_raw.c burn_ext4sp.c 
LOADER_SRC += common/com_parse.c common/com_debug.c common/com_osal.c common/com_buffer.c
LOADER_SRC += ui/ui_config.c ui/ui_keyled.c ui/ui_gfx.c ui/ui_display.c ui/ui_window.c ui/ui_win_msgbox.c  ui/ui_win_main.c

ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
LOADER_DEF += -DHI_ADVCA_SUPPORT
LOADER_DEF += -DHI_ADVCA_TYPE_$(CFG_HI_ADVCA_TYPE)
ifdef CFG_HI_ADVCA_FUNCTION
    ifeq ($(CFG_HI_ADVCA_FUNCTION),FINAL)
        LOADER_DEF += -DHI_ADVCA_FUNCTION_RELEASE
    else
        LOADER_DEF += -DHI_ADVCA_FUNCTION_$(CFG_HI_ADVCA_FUNCTION)
    endif
endif

ifeq ($(CFG_HI_ADVCA_TYPE), VERIMATRIX)
LOADER_INC += -I$(CURDIR)/ca/vmx/include
LOADER_LIB += -l:libhi_vmx_loader_verify.a
endif

ifeq ($(CFG_HI_ADVCA_TYPE), DCAS)
LOADER_INC += -I$(CURDIR)/ca/dcas/include
endif

LOADER_SRC += ssd_router.c ssd_parse.c
LOADER_LIB += -l:libhi_ca_ssd.a
endif

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
LOADER_DEF += -DHI_LOADER_BOOTLOADER
LOADER_SRC += ui/ui_text.c
endif

ifeq ($(CFG_HI_LOADER_APPLOADER),y)
LOADER_DEF += -DHI_LOADER_APPLOADER
LOADER_SRC += ui/ui_hdmi.c 
endif

ifeq ($(CFG_HI_BUILD_WITH_IR), y)
LOADER_DEF += -DHI_BUILD_WITH_IR
endif

ifeq ($(CFG_HI_LOG_SUPPORT), y)
LOADER_DEF += -DHI_LOG_SUPPORT=1
ifdef CFG_HI_LOG_LEVEL
LOADER_DEF += -DHI_LOG_LEVEL=$(CFG_HI_LOG_LEVEL)
else
LOADER_DEF += -DHI_LOG_LEVEL=1
endif
else
LOADER_DEF += -DHI_LOG_SUPPORT=0
endif

ifeq ($(CFG_HI_APPLOADER_OSD_LANG_CN),y)
LOADER_DEF += -DHI_APPLOADER_OSD_LANG_CN
endif

ifneq ($(CFG_HI_APPLOADER_TUNER_INDEX), )
LOADER_DEF += -DHI_LOADER_TUNER_INDEX=$(CFG_HI_APPLOADER_TUNER_INDEX)
endif

ifneq ($(CFG_HI_BOOTLOADER_TUNER_INDEX), )
LOADER_DEF += -DHI_LOADER_TUNER_INDEX=$(CFG_HI_BOOTLOADER_TUNER_INDEX)
endif

ifeq ($(CFG_HI_KEYLED_SUPPORT),y)
	LOADER_DEF += -DHI_BUILD_WITH_KEYLED
	ifeq ($(CFG_HI_KEYLED_PT6964_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_PT6964
	endif
	ifeq ($(CFG_HI_KEYLED_PT6961_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_PT6961
	endif
	ifeq ($(CFG_HI_KEYLED_CT1642_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_CT1642
	endif
	ifeq ($(CFG_HI_KEYLED_74HC164_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_74HC164
	endif
	ifeq ($(CFG_HI_KEYLED_FD650_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_FD650
	endif
	ifeq ($(CFG_HI_KEYLED_GPIOKEY_SELECT),y)
	LOADER_DEF += -DHI_KEYLED_GPIOKEY
	endif
endif

#===============================================================================
# Download mode Config
#===============================================================================
ifneq ($(CFG_HI_APPLOADER_MODE_IP)_$(CFG_HI_BOOTLOADER_MODE_IP),_)
LOADER_DEF += -DHI_LOADER_MODE_IP
LOADER_INC += -I$(COMPONENT_DIR)/curl/include
LOADER_LIB += -l:libcurl.a
LOADER_SRC += download_ip.c
endif

ifneq ($(CFG_HI_APPLOADER_MODE_OTA)_$(CFG_HI_BOOTLOADER_MODE_OTA),_)
LOADER_DEF += -DHI_LOADER_MODE_OTA
LOADER_SRC += download_ota.c

ifneq ($(CFG_HI_APPLOADER_TUNER_CAB)_$(CFG_HI_BOOTLOADER_TUNER_CAB),_)
LOADER_DEF += -DHI_LOADER_TUNER_CAB
LOADER_SRC += ui/ui_win_setting.c 
endif

ifneq ($(CFG_HI_APPLOADER_TUNER_DVB_T)_$(CFG_HI_BOOTLOADER_TUNER_DVB_T),_)
LOADER_DEF += -DHI_LOADER_TUNER_DVB_T
endif

ifneq ($(CFG_HI_APPLOADER_TUNER_DVB_T2)_$(CFG_HI_BOOTLOADER_TUNER_DVB_T2),_)
LOADER_DEF += -DHI_LOADER_TUNER_DVB_T2
endif

ifneq ($(CFG_HI_APPLOADER_TUNER_SAT)_$(CFG_HI_BOOTLOADER_TUNER_SAT),_)
LOADER_DEF += -DHI_LOADER_TUNER_SAT
ifeq ($(CFG_HI_DISEQC_SUPPORT),y)
LOADER_DEF += -DDISEQC_SUPPORT
endif
endif

ifneq ($(CFG_HI_APPLOADER_TUNER_TER)_$(CFG_HI_BOOTLOADER_TUNER_TER),_)
LOADER_DEF += -DHI_LOADER_TUNER_TER
endif
endif

ifneq ($(CFG_HI_APPLOADER_MODE_USB)_$(CFG_HI_BOOTLOADER_MODE_USB),_)
LOADER_DEF += -DHI_LOADER_MODE_USB
LOADER_SRC += download_fs.c
endif

ifneq ($(CFG_HI_APPLOADER_MODE_FS)_$(CFG_HI_BOOTLOADER_MODE_FS),_)
LOADER_DEF += -DHI_LOADER_MODE_FS
LOADER_SRC += download_fs.c
endif

#===============================================================================
# Protocol Type Config
#===============================================================================

ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv310 hi3716mv320 hi3716mv330 hi3110ev500),)
LOADER_DEF += -DHI_LOADER_PROTOCOL_VER1
else ifneq ($(findstring $(CFG_HI_CHIP_TYPE), hi3716mv410 hi3716mv420 hi3798mv100 hi3796mv100),)
LOADER_DEF += -DHI_LOADER_PROTOCOL_VER2
else
LOADER_DEF += -DHI_LOADER_PROTOCOL_VER3
endif

ifneq ($(CFG_HI_APPLOADER_PROTOCOL_SSU_OTA)_$(CFG_HI_BOOTLOADER_PROTOCOL_SSU_OTA),_)
LOADER_DEF += -DHI_LOADER_PROTOCOL_SSU_OTA
LOADER_SRC += ssu_acquisition.c ssu_parse.c
ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
LOADER_SRC += protocol_ssd_ssu.c 
else
LOADER_SRC += protocol_ssu.c
endif
endif

ifneq ($(CFG_HI_APPLOADER_PROTOCOL_HISI_OTA)_$(CFG_HI_BOOTLOADER_PROTOCOL_HISI_OTA),_)
LOADER_DEF += -DHI_LOADER_PROTOCOL_HISI_OTA
LOADER_SRC += hiota_acquisition.c hiota_parse.c
ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
LOADER_SRC += protocol_ssd_hiota.c
else
LOADER_SRC += protocol_hiota.c 
endif
endif

ifneq ($(CFG_HI_APPLOADER_FS_PROTOCOL_HISI_FILE)_$(CFG_HI_APPLOADER_IP_PROTOCOL_HISI_FILE)_$(CFG_HI_APPLOADER_USB_PROTOCOL_HISI_FILE)_$(CFG_HI_BOOTLOADER_USB_PROTOCOL_HISI_FILE),___)
LOADER_DEF += -DHI_LOADER_PROTOCOL_HISI_FILE
LOADER_SRC += hifile_acquisition.c hifile_parse.c
ifeq ($(CFG_HI_ADVCA_SUPPORT),y)
LOADER_SRC += protocol_ssd_hifile.c
else
LOADER_SRC += protocol_hifile.c 
endif 
endif

LOADER_OBJS := $(patsubst %.c,%.o,$(LOADER_SRC))

####################################################################################

ifeq ($(CFG_HI_LOADER_BOOTLOADER),y)
include BootLoader.mak
endif

ifeq ($(CFG_HI_LOADER_APPLOADER),y)
include AppLoader.mak
endif
