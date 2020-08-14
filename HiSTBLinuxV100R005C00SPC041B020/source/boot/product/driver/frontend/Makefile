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

COMMON_DIR  := $(PRODUCT_DIR)/driver/common

CFLAGS  += -I$(PRODUCT_DIR)/driver/include
CFLAGS   += -I$(PRODUCT_DIR)/driver/include_inc
CFLAGS  += -I$(BOOT_DIR)/arch/arm/include
CFLAGS  += -I$(BOOT_DIR)/include
CFLAGS  += -I$(PRODUCT_DIR)/driver/frontend/include
CFLAGS  += -I$(PRODUCT_DIR)/driver/frontend/demod/AVL_DVBSx_SDK/AVL_DVBSx/include
CFLAGS  += -I$(PRODUCT_DIR)/driver/frontend/demod/AVL_DVBSx_SDK/BSP
#CFLAGS  += -I$(SDK_DIR)/source/common/include

CFLAGS += -I$(SDK_DIR)/source/common/include
CFLAGS  += -DCHIP_TYPE_${CFG_HI_CHIP_TYPE}
CFLAGS  += -DBOOT_COMPILE

LIBS    := libfrontend.a
OBJS    := unf_tuner.o

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_ALPS_TDAE),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_ALPS_TDAE
OBJS   += tuner/alps_tdae.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_TDCC),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_TDCC
OBJS   += tuner/tdcc.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_TDA18250),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_TDA18250
OBJS   += tuner/tda18250/tda18250.o \
          tuner/tda18250/tmbslTDA18250.o \
          tuner/tda18250/tmbslTDA18250Instance.o \
          tuner/tda18250/tmddTDA18250.o \
          tuner/tda18250/tmddTDA18250Instance.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_TDA18250B),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_TDA18250B
OBJS   += tuner/tda18250b/tda18250b.o \
	  tuner/tda18250b/tmbslTDA18250A.o \
	  tuner/tda18250b/tmbslTDA18250A_Advanced.o 
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_SI2147),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_SI2147
OBJS   += tuner/si2147/Si2147.o \
	  tuner/si2147/Si2147_L0_Connection.o \
	  tuner/si2147/Si2147_L1_API.o \
	  tuner/si2147/Si2147_L1_Commands.o \
	  tuner/si2147/Si2147_L1_Properties.o \
	  tuner/si2147/Si2147_L2_API.o \
	  tuner/si2147/Si2147_Properties_Strings.o \
	  tuner/si2147/Si2147_User_Properties.o  			  
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_TMX7070X),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_TMX7070X
OBJS   += tuner/tmx7070x.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_R820C),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_R820C
OBJS   += tuner/R820C/I2C_Sys.o \
	  tuner/R820C/R820C.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_MXL203),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_MXL203
OBJS   += tuner/mxl203/mxl203.o \
	  tuner/mxl203/MxL203RF.o \
	  tuner/mxl203/MxL_User_Define.o \
	  tuner/mxl203/MxL203RF_API.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_AV2011),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_AV2011
OBJS   += tuner/av2011.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_SHARP7903),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_SHARP7903
OBJS   += tuner/sharp_qm1d1c004x.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_M88TS2022),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_M88TS2022
OBJS   += tuner/M88TS2022.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_RDA5815),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_RDA5815
OBJS   += tuner/RDA5815.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_MXL603),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_MXL603
OBJS   += tuner/MXL603/MxL603_OEM_Drv.o \
	  tuner/MXL603/MxL603_TunerApi.o \
	  tuner/MXL603/MxL603_TunerCfg.o \
	  tuner/MXL603/MxL603.o
endif

ifeq ($(CFG_HI_BOOT_TUNER_TYPE_M88TC3800),y)
CFLAGS += -DHI_BOOT_TUNER_DEV_TYPE_M88TC3800
OBJS   += tuner/m88tc3800/mt_fe_tn_tc3800.o
endif

ifeq ($(CFG_HI_BOOT_DEMOD_TYPE_HI3130I),y)
CFLAGS += -DHI_BOOT_DEMOD_DEV_TYPE_HI3130I
OBJS   += demod/hi3716cv200/hi3716cv200.o
endif

ifeq ($(CFG_HI_BOOT_DEMOD_TYPE_HI3136),y)
CFLAGS += -DHI_BOOT_DEMOD_DEV_TYPE_HI3136
OBJS   += demod/hi3136/hi3136.o
endif

ifeq ($(CFG_HI_BOOT_DEMOD_TYPE_MXL254),y)
HI_BOOT_DEMOD_MXL254_MXL214 := y
endif

ifeq ($(CFG_HI_BOOT_DEMOD_TYPE_MXL214),y)
HI_BOOT_DEMOD_MXL254_MXL214 := y
endif

ifeq ($(HI_BOOT_DEMOD_MXL254_MXL214),y)
CFLAGS +=  -DHI_BOOT_DEMOD_DEV_TYPE_MXL254
OBJS   +=  demod/MXL254/MxL254.o  \
                demod/MXL254/MxL_HRCLS_CalCtrl.o \
                demod/MXL254/MxL_HRCLS_CommonApi.o \
                demod/MXL254/MxL_HRCLS_DebugApi.o \
                demod/MXL254/MxL_HRCLS_DemodApi.o \
                demod/MXL254/MxL_HRCLS_OEM_Drv.o \
                demod/MXL254/MxL_HRCLS_OobApi.o \
                demod/MXL254/MxL_HRCLS_PhyCtrl.o \
                demod/MXL254/MxL_HRCLS_SerdesApi.o \
                demod/MXL254/MxL_HRCLS_XptApi.o
endif

ifneq ($(CFG_HI_BOOT_LNB_CTRL_MPS8125)_$(CFG_HI_BOOT_LNB_CTRL_ISL9492),_)
OBJS   += lnbctrl/lnbctrl_common.o
endif

ifeq ($(CFG_HI_BOOT_LNB_CTRL_MPS8125),y)
CFLAGS += -DHI_BOOT_LNB_CTRL_MPS8125
OBJS   += lnbctrl/mps8125.o
endif

ifeq ($(CFG_HI_BOOT_LNB_CTRL_ISL9492),y)
CFLAGS += -DHI_BOOT_LNB_CTRL_ISL9492
OBJS   += lnbctrl/isl9492.o
endif

ifeq ($(CFG_HI_BOOT_DISEQC_SUPPORT),y)
CFLAGS += -DHI_BOOT_DISEQC_SUPPORT
OBJS   += unf_diseqc.o
endif

#===============================================================================
# rules
#===============================================================================

.PHONY: all clean install uninstall $(LIBS)

all: $(LIBS)

clean:
	$(AT)rm -rf $(OBJS)
	$(AT)rm -rf $(LIBS)

install: all

uninstall:

$(LIBS): $(OBJS)
	$(AT)$(AR) -rc $@ $^
