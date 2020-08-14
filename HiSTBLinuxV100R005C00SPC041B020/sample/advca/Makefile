#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR := $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

include $(SAMPLE_DIR)/base.mak

#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := sample_ca_tsplay \
    sample_ca_setjtag \
    sample_ca_opensecboot \
    sample_ca_setMarketID \
    sample_ca_getchipid \
    sample_ca_crypto \
    sample_ca_lockhardcw \
    sample_ca_blpk \
    sample_ca_writeRSAkey \
    sample_ca_setSTBSN \
    sample_ca_swpk_keyladder \
    sample_ca_aes_cbc_mac \
    sample_ca_isMarketIdSet \
    sample_ca_gettrustzonestatus \
    sample_ca_get_otp_fuse \
    sample_ca_set_otp_fuse \
    sample_ca_csa3_sp_tsplay \
    sample_ca_enabletrustzone

ifeq ($(CFG_HI_ADVCA_TYPE),CONAX)
SAMPLE_IMAGES += sample_ca_lpk \
    sample_product_crypto \
    sample_product_encrypt_boot \
    sample_product_burn_partition \
    sample_product_pv_stb \
    sample_product_add_cmac \
    sample_ca_conax_tsplay
else ifeq ($(CFG_HI_ADVCA_TYPE),SUMA)
SAMPLE_IMAGES += sample_ca_suma_tsplay
else ifeq ($(CFG_HI_ADVCA_TYPE),CTI)
SAMPLE_IMAGES += sample_ca_cti_tsplay
else ifeq ($(CFG_HI_ADVCA_TYPE),DCAS)
SAMPLE_IMAGES += sample_ca_dcas_tsplay
else ifeq ($(CFG_HI_ADVCA_TYPE),NOVEL)
SAMPLE_IMAGES += sample_ca_novel_tsplay \
    sample_ca_novel_crypto
else ifeq ($(CFG_HI_ADVCA_TYPE),IRDETO)
SAMPLE_IMAGES += sample_ca_irdeto_msr2.2
endif

ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
SAMPLE_IMAGES += sample_ca_dvbplay
ifeq ($(CFG_HI_ADVCA_TYPE),DCAS)
SAMPLE_IMAGES += sample_ca_dcas_dvbplay
else ifeq ($(CFG_HI_ADVCA_TYPE),VERIMATRIX)
SAMPLE_IMAGES += sample_ca_dvbplay_misc \
    sample_ca_dvbplay_sp
else ifeq ($(CFG_HI_ADVCA_TYPE),NOVEL)
SAMPLE_IMAGES += sample_ca_novel_dvbplay
endif
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
ifeq ($(CFG_HI_FRONTEND_SUPPORT),y)
SAMPLE_IMAGES += sample_ca_pvr_rec \
    sample_ca_pvr_play \
    sample_ca_pvr_timeshift
endif
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
LOCAL_OBJS := sample_ca_adp_pvr.o
endif

COMMON_OBJS := $(filter-out hi_adp_pvr.o,$(COMMON_SRCS:%.c=%.o))

DEPEND_LIBS := $(HI_LIBS)

include $(SAMPLE_DIR)/hi_sample_rules.mak
