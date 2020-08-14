###########################################################
# platform definition
###########################################################
LOCAL_CFLAGS    += $(TRUSTEDCORE_LOCAL_CFLAGS)
LOCAL_ASFLAGS   += $(TRUSTEDCORE_LOCAL_ASFLAGS)

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
objects :=  


COMMON_KRENEL_INCS := \
        -I$(TRUSTEDCORE_DIR)/include/kernel/ \
        -I$(TRUSTEDCORE_DIR)/include/config/ \
		-I$(TRUSTEDCORE_DIR)/include/TEE \
		-I$(HI_TEE_DRV_DIR)/include \
		-I$(HI_TEE_API_DIR)/include

###########################################################
PLAT_SYS_INCS := -I$(TRUSTEDCORE_DIR)/platform

ifeq ($(strip $(WITH_HISI_SEC_MMZ)), true)
objects += mem/sec_mmz
endif

ifeq ($(strip $(WITH_HISI_SMMU)), true)
objects += mem/hi_smmu
endif

ifeq ($(strip $(WITH_HISI_OTP)), true)
objects += otp
endif

ifeq ($(strip $(WITH_HISI_ADVCA)), true)
objects += advca
endif

ifeq ($(strip $(WITH_HISI_DMX)), true)
objects += demux
endif

ifeq ($(strip $(WITH_HISI_PVR)), true)
objects += pvr
endif

ifeq ($(strip $(WITH_HISI_AVPLAY)), true)
objects += avplay
endif

ifeq ($(strip $(WITH_HISI_CIPHER)), true)
objects += cipher
endif

ifeq ($(strip $(WITH_HISI_SEC_HDMI)), true)
objects += hdmi
endif

ifeq ($(strip $(WITH_HISI_VDP)), true)
objects += vdp
endif

ifeq ($(strip $(WITH_HISI_VFMW)), true)
objects += vfmw
endif

ifeq ($(strip $(WITH_HISI_SEC_TIMER)), true)
objects += timer
endif

ifeq ($(strip $(WITH_STB_BEIDOU)), true)
objects += beidou
endif

ifeq ($(strip $(WITH_STB_I2C_DRIVER)), true)
objects += i2c
endif

##########################################################
# project path definition
##########################################################
LOCAL_OUT = $(OUT)/$(notdir $(CURDIR))

#include $(TRUSTEDCORE_BUILD_RAW_EXECUTABLE)
# add for enum size may different in different toolchain
LOCAL_CFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables
LOCAL_ASFLAGS += -fno-short-enums -fno-exceptions -fno-unwind-tables

LOCAL_CFLAGS += -march=armv7-a

CFLAGS += $(COMMON_KRENEL_INCS)
CFLAGS += $(LOCAL_CFLAGS)
#LDFLAGS += $(LOCAL_LDFLAGS)

#export CFLAGS 

objects_clean:=$(addsuffix _clean,$(objects))
objects_install:=$(addsuffix _install,$(objects))
objects_uninstall:=$(addsuffix _uninstall,$(objects))

ifeq ($(WITH_ARMV8_SUPPORT) , true)
#ARMV8_POSIX_STR := 64bit
endif

#export ARMV8_POSIX_STR

LIBS := $(LOCAL_OUT)/libstbdrv.a
SUBLIBS := $(foreach sub,$(objects),$(LOCAL_OUT)/$(sub)/libsec_$(notdir $(sub)).a)

##########################################################
# build libstbdrv.a
##########################################################
.PHONY: all clean install uninstall $(LIBS)

#==================================================================
# build libstbdrv.a
#==================================================================

all: $(LIBS)

clean:  $(objects_clean)
	$(AT)rm -rf $(LIBS)
	$(AT)rm -rf ar.mak

install: $(objects_install)

uninstall: $(objects_uninstall)

$(LIBS): $(objects)
	rm -rf $@
	echo "CREATE $@" > $(LOCAL_OUT)/ar.mak
	$(foreach sublib,$(SUBLIBS),echo "ADDLIB $(sublib)" >> $(LOCAL_OUT)/ar.mak;)
	echo "SAVE" >> $(LOCAL_OUT)/ar.mak
	echo "END" >> $(LOCAL_OUT)/ar.mak
	$(AR2) -M < $(LOCAL_OUT)/ar.mak

#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(objects_install) $(objects_uninstall)

prepare:
	echo "prepare done!"
$(objects): | prepare
	make -C $@ all

$(objects_clean):
	make -C $(patsubst %_clean,%,$@) clean

$(objects_install):
	make -C $(patsubst %_clean,%,$@) install

$(objects_uninstall):
	make -C $(patsubst %_clean,%,$@) uninstall

