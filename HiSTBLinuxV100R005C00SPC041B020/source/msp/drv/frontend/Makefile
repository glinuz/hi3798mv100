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
FRONTEND_DIR := $(MSP_DIR)/drv/frontend

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                                             \
                -I$(COMMON_API_INCLUDE)                                             \
                -I$(COMMON_DRV_INCLUDE)                                             \
                -I$(MSP_UNF_INCLUDE)                                                \
                -I$(MSP_API_INCLUDE)                                                \
                -I$(MSP_DRV_INCLUDE)                                                \
                -I$(FRONTEND_DIR)/include                                           \
                -I$(FRONTEND_DIR)/demod/AVL_DVBSx_SDK/AVL_DVBSx/include             \
                -I$(FRONTEND_DIR)/demod/AVL_DVBSx_SDK/BSP                           \
                -I$(FRONTEND_DIR)/tuner/MXL603                                      \
                -I$(FRONTEND_DIR)/demod/CXD2837/                                    \
                -I$(FRONTEND_DIR)/demod/CXD2837/i2c                                 \
                -I$(FRONTEND_DIR)/demod/CXD2837/dvb_terr                            \
                -I$(FRONTEND_DIR)/demod/CXD2837/dvb_cable                           \
                -I$(FRONTEND_DIR)/demod/CXD2837/tuner                               \
                -I$(FRONTEND_DIR)/demod/CXD2837/tuner/terr_cable_ascot2e            \
                -I$(FRONTEND_DIR)/demod/CXD2837/tuner/terr_cable_ascot2e/refcode    \
                -I$(FRONTEND_DIR)/demod/CXD2837/lib                                 \
                -I$(FRONTEND_DIR)/demod/hd2501                                      \
                -I$(FRONTEND_DIR)/tuner/Rafael836                                   \
                -I$(FRONTEND_DIR)/tuner/SiLabs                                      \
                -I$(FRONTEND_DIR)/tuner/Si2144                                      \
                -I$(FRONTEND_DIR)/tuner/hiFDVBC100                                  \
                -I$(FRONTEND_DIR)/tuner/AV2026                                      \
                -I$(FRONTEND_DIR)/demod/hi3138                                      \
                -I$(FRONTEND_DIR)/demod/AVL6381

EXTRA_CFLAGS += -I$(SOURCE_DIR)/common/include/
EXTRA_CFLAGS += -I$(SOURCE_DIR)/common/drv/include/
EXTRA_CFLAGS += -I$(SOURCE_DIR)/common/api/stat/

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

#################### base #####################
MOD_NAME := hi_tuner

obj-$(HI_DRV_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y := drv_tuner.o
$(MOD_NAME)-y += tuner_intf.o
$(MOD_NAME)-y += tuner_proc.o


$(MOD_NAME)-y += common/tuner_math.o

ifeq ($(CFG_HI_DEMOD_TYPE_HI3130),y)
hi_tuner-y +=   demod/hi3130/hi3130.o
hi_tuner-y +=   demod/hi3130v200/dvb_c/hi3130v200.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3130
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HI3136),y)
hi_tuner-y +=   demod/hi3136/hi3136.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3136
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HI3137),y)
hi_tuner-y +=   demod/hi3137/hi3137.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3137
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HI3138),y)
hi_tuner-y +=	demod/hi3136/hi3136.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3136

hi_tuner-y +=	demod/hi3137/hi3137.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3137

hi_tuner-y += demod/hi3138/hi3138.o
hi_tuner-y += demod/hi3138/hi3138_info.o
hi_tuner-y += demod/hi3138/hi3138_ts.o
hi_tuner-y += demod/hi3138/hi3138_dvbc.o
hi_tuner-y += demod/hi3138/hi3138_dvbs2.o
hi_tuner-y += demod/hi3138/hi3138_dvbt2.o
hi_tuner-y += demod/hi3138/hi3138_isdbt.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3138
endif

ifeq ($(CFG_HI_DEMOD_TYPE_X5HDQAM),y)
hi_tuner-y +=   demod/x5hdqam/x5hdqam.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_X5HDQAM
endif

ifeq ($(CFG_HI_DEMOD_TYPE_X5HDV2QAM),y)
hi_tuner-y +=   demod/x5hdqamv200/x5hdqamv200.o
hi_tuner-y +=   demod/hi3716cv200/dvb_c/hi3716cv200.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_X5HDV2QAM
endif

ifeq ($(CFG_HI_DEMOD_TYPE_J83B),y)
hi_tuner-y +=   demod/j83b/j83b.o
hi_tuner-y +=   demod/hi3716cv200/j83b/hi3716cv200_j83b.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_J83B
    ifeq ($(CFG_HI_DEMOD_TYPE_HI3130),y)
    hi_tuner-y +=   demod/hi3130v200/j83b/hi3130v200_j83b.o
    EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HI3130
    endif
endif

ifeq ($(CFG_HI_DEMOD_TYPE_AVL6211),y)
hi_tuner-y += demod/AVL_DVBSx_SDK/AVL_DVBSx/src/avl.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/avl_dvbsx.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/IBase.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/IBlindScan.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/IBlindScanProcess.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/IDiseqc.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/II2C.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/II2CRepeater.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/IRx.o \
                demod/AVL_DVBSx_SDK/AVL_DVBSx/src/ITuner.o \
                demod/AVL_DVBSx_SDK/BSP/IBSP.o \
                demod/AVL_DVBSx_SDK/AVL6211/AVL6211.o \
                demod/AVL_DVBSx_SDK/AVL6211/AVL6211_patch.dat.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_AVL6211
endif

ifeq ($(CFG_HI_DEMOD_TYPE_TP5001), y)
hi_tuner-y += demod/TP5001/TP5001.o \
              demod/TP5001/tp5001_demod.o 
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_TP5001
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HD2501), y)
hi_tuner-y += demod/hd2501/hd2501_api.o \
              demod/hd2501/hd2501_drv.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HD2501
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MXL101),y)
hi_tuner-y += demod/MXL101/mxl101.o \
                demod/MXL101/MxL101SF_OEM_Drv.o \
                demod/MXL101/MxL101SF_PhyCfg.o \
                demod/MXL101/MxL101SF_PhyCtrlApi.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_MXL101
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MN88472),y)
hi_tuner-y +=   demod/mn88472/MN88472_autoctrl.o \
                demod/mn88472/MN88472_register.o \
                demod/mn88472/MN_DMD_device.o \
                demod/mn88472/MN_DMD_driver.o \
                demod/mn88472/MN_I2C.o \
                demod/mn88472/MN_Tuner.o \
                demod/mn88472/MN88472.o \
                demod/mn88472/MN_DMD_common.o \
                demod/mn88472/MN88472_UserFunction.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_MN88472
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MN88473),y)
hi_tuner-y += demod/mn8847x/MN8847x_autoctrl.o \
                demod/mn8847x/MN8847x_register.o \
                demod/mn8847x/MN_DMD_device.o \
                demod/mn8847x/MN_DMD_driver.o \
                demod/mn8847x/MN_I2C.o \
                demod/mn8847x/MN_TCB.o \
                demod/mn8847x/MN_Tuner.o \
                demod/mn8847x/MN_DMD_common.o \
                demod/mn8847x/MN8847x_UserFunction.o \
                demod/mn8847x/MN8847x.o 
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_MN88473
endif

ifeq ($(CFG_HI_DEMOD_TYPE_IT9170),y)
hi_tuner-y  +=  demod/IT9170/cmd.o \
                demod/IT9170/IT9170.o \
                demod/IT9170/IT9173.o \
                demod/IT9170/samba.o \
                demod/IT9170/standard.o \
                demod/IT9170/tuner.o \
                demod/IT9170/user.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_IT9170
endif

ifeq ($(CFG_HI_DEMOD_TYPE_IT9133),y)
hi_tuner-y  +=  demod/IT9133/cmd.o \
                demod/IT9133/IT9133.o \
                demod/IT9133/Omega.o \
                demod/IT9133/standard.o \
                demod/IT9133/tuner.o \
                demod/IT9133/user.o \
                demod/IT9133/ite9133.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_IT9133
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MXL254),y)
HI_DEMOD_MXL254_MXL214 := y
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MXL214),y)
HI_DEMOD_MXL254_MXL214 := y
endif

ifeq ($(HI_DEMOD_MXL254_MXL214),y)
hi_tuner-y  +=  demod/MXL254/MxL254.o  \
                demod/MXL254/MxL_HRCLS_CalCtrl.o \
                demod/MXL254/MxL_HRCLS_CommonApi.o \
                demod/MXL254/MxL_HRCLS_DebugApi.o \
                demod/MXL254/MxL_HRCLS_DemodApi.o \
                demod/MXL254/MxL_HRCLS_OEM_Drv.o \
                demod/MXL254/MxL_HRCLS_OobApi.o \
                demod/MXL254/MxL_HRCLS_PhyCtrl.o \
                demod/MXL254/MxL_HRCLS_SerdesApi.o \
                demod/MXL254/MxL_HRCLS_XptApi.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_MXL254
endif

ifeq ($(CFG_HI_DEMOD_TYPE_CXD2837),y)
hi_tuner-y  +=  demod/CXD2837/sony_common.o \
                demod/CXD2837/CXD2837.o \
                demod/CXD2837/sony_stopwatch_port.o \
                demod/CXD2837/sony_demod.o \
                demod/CXD2837/sony_hisi_i2c.o \
                demod/CXD2837/sony_integ.o \
                demod/CXD2837/dvb_cable/sony_demod_dvbc.o \
                demod/CXD2837/dvb_cable/sony_demod_dvbc_monitor.o \
                demod/CXD2837/dvb_cable/sony_integ_dvbc.o \
                demod/CXD2837/dvb_terr/sony_demod_dvbt2.o \
                demod/CXD2837/dvb_terr/sony_demod_dvbt2_monitor.o \
                demod/CXD2837/dvb_terr/sony_demod_dvbt.o \
                demod/CXD2837/dvb_terr/sony_demod_dvbt_monitor.o \
                demod/CXD2837/dvb_terr/sony_integ_dvbt_t2.o \
                demod/CXD2837/i2c/sony_i2c.o \
                demod/CXD2837/lib/sony_math.o \
                demod/CXD2837/tuner/terr_cable_ascot2e/sony_tuner_ascot2e.o \
                demod/CXD2837/tuner/terr_cable_ascot2e/refcode/sony_ascot2e.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_CXD2837
endif

ifeq ($(CFG_HI_DEMOD_TYPE_TDA18280),y)
hi_tuner-y  +=  demod/NXP18280/tda18280.o\
                demod/NXP18280/tmbslTDA18280.o \
                demod/NXP18280/tmbslOOBIpInstance.o \
                demod/NXP18280/tmbslOOBIp_cfg.o \
                demod/NXP18280/tmbslOOBIp.o \
                demod/NXP18280/tmbslHCDP_Reg.o \
                demod/NXP18280/tmbslHCDP_Instance.o \
                demod/NXP18280/tmbslHCDP_Advanced.o \
                demod/NXP18280/tmbslHCDP.o \
                demod/NXP18280/tmbslTDA18204.o \
                demod/NXP18280/tmbslTDA18204_RegDef.o  \
                demod/NXP18280/tmutilOM3990.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_TDA18280
endif

ifeq ($(CFG_HI_DEMOD_TYPE_ATBM888X),y)
hi_tuner-y  +=  demod/ATBM888X/atbm888x.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_ATBM888X
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HIFDVBC100),y)
hi_tuner-y  +=  demod/hiFoneCab100/hiF_DVBC100.o \
		demod/hiFoneCab100/hal_hiFoneCab100.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HIFDVBC100
endif

ifeq ($(CFG_HI_DEMOD_TYPE_HIFJ83B100),y)
hi_tuner-y  +=  demod/hiFoneCab100/hiF_J83B100.o \
		demod/hiFoneCab100/hal_hiFoneCab100.o
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_HIFJ83B100
endif

ifeq ($(CFG_HI_DEMOD_TYPE_MXL683),y)
hi_tuner-y += demod/MXL68x/MxL68x_FwDownload.o \
			  demod/MXL68x/MxL68x_OEM_Drv.o \
			  demod/MXL68x/MxL68x_PhyCfg.o \
			  demod/MXL68x/MxL68x_PhyCtrlApi.o \
			  demod/MXL68x/MxL68x_PhySpurTable.o \
              demod/MXL68x/MxL68x.o 
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_MXL683
endif

ifeq ($(CFG_HI_DEMOD_TYPE_AVL6381),y)
hi_tuner-y += demod/AVL6381/AVL6381_Internal.o\
			  demod/AVL6381/AVL6381_iic_rw.o \
			  demod/AVL6381/AVL6381_API.o \
			  demod/AVL6381/AVL6381.o 
EXTRA_CFLAGS += -DDEMOD_DEV_TYPE_AVL6381
endif


DRIV_DIR=$(KECS_DIR)/drv/tuner_pilot

EXTRA_CFLAGS += $(KCFG_CFLAGS)
EXTRA_CFLAGS += -I$(DRV_DIR)/include  -I$(MSP_DIR)/include -I$(FE_DIR)/include
EXTRA_CFLAGS += -I$(FE_DIR)/demod/AVL_DVBSx_SDK/AVL_DVBSx/include \
                -I$(FE_DIR)/demod/AVL_DVBSx_SDK/BSP
EXTRA_CFLAGS += -I$(KECS_DIR)/drv/gpio/


############## tuner type config #############
ifeq ($(CFG_HI_TUNER_TYPE_XG_3BL),y)
hi_tuner-y += tuner/xg_3bl.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_XG_3BL
endif

ifeq ($(CFG_HI_TUNER_TYPE_SI2147),y)
#hi_tuner-y += tuner/SiLabs/Si2147_diy.o
#hi_tuner-y += tuner/SiLabs/Si2157_L1_API.o
#hi_tuner-y += tuner/SiLabs/Si2157_L1_Commands.o
#hi_tuner-y += tuner/SiLabs/Si2157_L1_Properties.o
#hi_tuner-y += tuner/SiLabs/Si2157_L2_API.o
#hi_tuner-y += tuner/SiLabs/Si2157_User_Properties.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_SI2147
endif

ifeq ($(CFG_HI_TUNER_TYPE_RAFAEL836),y)
hi_tuner-y += tuner/Rafael836/R836.o
hi_tuner-y += tuner/Rafael836/Rafael_I2C.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_RAFAEL836
endif

ifeq ($(CFG_HI_TUNER_TYPE_ALPS_TDAE),y)
hi_tuner-y += tuner/alps_tdae.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_ALPS_TDAE
endif

ifeq ($(CFG_HI_TUNER_TYPE_CD1616_LF_GIH_4),y)
hi_tuner-y += tuner/cd1616_double.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_CD1616_LF_GIH_4
endif

ifeq ($(CFG_HI_TUNER_TYPE_CD1616),y)
hi_tuner-y   += tuner/cd1616.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_CD1616
endif

ifeq ($(CFG_HI_TUNER_TYPE_TDA18250),y)
hi_tuner-y  +=  tuner/tda18250/tmbslTDA18250.o \
                tuner/tda18250/tmbslTDA18250Instance.o \
                tuner/tda18250/tmddTDA18250.o \
                tuner/tda18250/tmddTDA18250Instance.o \
                tuner/tda18250/tda18250.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TDA18250
endif

ifeq ($(CFG_HI_TUNER_TYPE_TDA18250B),y)
hi_tuner-y  +=  tuner/tda18250b/tmbslTDA18250A.o \
                tuner/tda18250b/tmbslTDA18250A_Advanced.o \
                tuner/tda18250b/tda18250b.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TDA18250B
endif

ifeq ($(CFG_HI_TUNER_TYPE_si2147),y)
hi_tuner-y += tuner/si2147/Si2147.o \
			  tuner/si2147/Si2147_L0_Connection.o \
			  tuner/si2147/Si2147_L1_API.o \
			  tuner/si2147/Si2147_L1_Commands.o \
			  tuner/si2147/Si2147_L1_Properties.o \
			  tuner/si2147/Si2147_L2_API.o \
			  tuner/si2147/Si2147_Properties_Strings.o \
			  tuner/si2147/Si2147_User_Properties.o  			  
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_si2147
endif

ifeq ($(CFG_HI_TUNER_TYPE_MXL203),y)
hi_tuner-y  +=  tuner/mxl203/mxl203.o \
                tuner/mxl203/MxL203RF.o \
                tuner/mxl203/MxL_User_Define.o \
                tuner/mxl203/MxL203RF_API.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_MXL203
endif

ifeq ($(CFG_HI_TUNER_TYPE_TMX7070X),y)
hi_tuner-y += tuner/tmx7070x.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TMX7070X
endif

ifeq ($(CFG_HI_TUNER_TYPE_MT2081),y)
hi_tuner-y +=   tuner/mt2081/mt2081.o \
                tuner/mt2081/mt2081_userdef.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_MT2081
endif

ifeq ($(CFG_HI_TUNER_TYPE_TDCC),y)
hi_tuner-y += tuner/tdcc.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TDCC
endif

ifeq ($(CFG_HI_TUNER_TYPE_R820C),y)
hi_tuner-y +=   tuner/R820C/I2C_Sys.o \
                tuner/R820C/R820C.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_R820C
endif

ifeq ($(CFG_HI_TUNER_TYPE_AV2011),y)
hi_tuner-y += tuner/av2011.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_AV2011
endif

ifeq ($(CFG_HI_TUNER_TYPE_AV2018),y)
hi_tuner-y += tuner/av2018.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_AV2018
endif

ifeq ($(CFG_HI_TUNER_TYPE_SHARP7903),y)
hi_tuner-y += tuner/sharp_qm1d1c004x.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_SHARP7903
endif

ifeq ($(CFG_HI_TUNER_TYPE_RDA5815),y)
hi_tuner-y += tuner/RDA5815.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_RDA5815
endif

ifeq ($(CFG_HI_TUNER_TYPE_M88TS2022),y)
hi_tuner-y += tuner/M88TS2022.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_M88TS2022
endif

ifeq ($(CFG_HI_TUNER_TYPE_MXL603),y)
hi_tuner-y +=   tuner/MXL603/MxL603_OEM_Drv.o \
                tuner/MXL603/MxL603_TunerApi.o \
                tuner/MXL603/MxL603_TunerCfg.o \
                tuner/MXL603/MxL603.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_MXL603
endif

ifeq ($(CFG_HI_TUNER_TYPE_MXL608),y)
hi_tuner-y +=   tuner/MXL608/MxL608_TunerSpurTable.o \
		tuner/MXL608/MxL608_OEM_Drv.o \
		tuner/MXL608/MxL608_TunerApi.o \
		tuner/MXL608/MxL608_TunerCfg.o \
		tuner/MXL608/MxL608.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_MXL608
endif

ifeq ($(CFG_HI_TUNER_TYPE_MXL683),y)
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_MXL683
endif

ifeq ($(CFG_HI_TUNER_TYPE_TDA182I5A),y)
hi_tuner-y  +=  tuner/tda182i5a/tmbslTDA182I5a.o \
                tuner/tda182i5a/tmbslTDA182I5a_Instance.o \
                tuner/tda182i5a/tmbslTDA182I5a_RegDef.o \
		tuner/tda182i5a/tda182i5a.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TDA182I5A
endif

ifeq ($(CFG_HI_TUNER_TYPE_SI2144),y)
hi_tuner-y  +=  tuner/Si2144/Silabs_L0_Connection.o \
                tuner/Si2144/Si2144_User_Properties.o \
                tuner/Si2144/Si2144_Properties_Strings.o \
		tuner/Si2144/Si2144_Properties_Strings.o \
		tuner/Si2144/Si2144_L2_API.o \
		tuner/Si2144/Si2144_L1_Properties.o \
		tuner/Si2144/Si2144_L1_Commands.o \
		tuner/Si2144/Si2144_L1_API.o \
		tuner/Si2144/Si2144.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_SI2144
endif

ifeq ($(CFG_HI_TUNER_TYPE_M88TC3800),y)
hi_tuner-y += tuner/m88tc3800/mt_fe_tn_tc3800.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_M88TC3800
endif


ifeq ($(CFG_HI_TUNER_TYPE_TDA6651),y)
hi_tuner-y += tuner/tda6651.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_TDA6651
endif

ifeq ($(CFG_HI_TUNER_TYPE_AV2026),y)
hi_tuner-y += tuner/AV2026/AV2026.o
EXTRA_CFLAGS += -DTUNER_DEV_TYPE_AV2026
endif

hi_tuner-y += lnbctrl/lnbctrl_common.o

ifeq ($(CFG_HI_LNB_CTRL_ISL9492),y)
hi_tuner-y += lnbctrl/isl9492.o
EXTRA_CFLAGS += -DLNB_CTRL_ISL9492
endif

ifeq ($(CFG_HI_LNB_CTRL_MPS8125),y)
hi_tuner-y += lnbctrl/mps8125.o
EXTRA_CFLAGS += -DLNB_CTRL_MPS8125

ifeq ($(CFG_HI_LNB_CTRL_GPIO),y)
EXTRA_CFLAGS += -DLNB_CTRL_GPIO

ifneq (${CFG_HI_LNB_EN_GPIO_NO},)
EXTRA_CFLAGS += -DHI_LNB_EN_GPIO_NO=${CFG_HI_LNB_EN_GPIO_NO}
endif

ifneq (${CFG_HI_LNB_HV_SEL_GPIO_NO},)
EXTRA_CFLAGS += -DHI_LNB_HV_SEL_GPIO_NO=${CFG_HI_LNB_HV_SEL_GPIO_NO}
endif
endif

ifeq ($(CFG_HI_LNB_CTRL_DEMOD),y)
EXTRA_CFLAGS += -DLNB_CTRL_DEMOD
endif
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
ifeq ($(HI_DEMOD_MXL254_MXL214),y)
	$(AT)cp -rf $(CURDIR)/demod/MXL254/MxL_HRCLS_FW.mbin $(HI_MODULE_DIR)/
	$(AT)cp -rf $(CURDIR)/demod/MXL254/nvram50.bin $(HI_MODULE_DIR)/
endif

uninstall:
	$(AT)rm -rf $(HI_MODULE_DIR)/$(MOD_NAME).ko
