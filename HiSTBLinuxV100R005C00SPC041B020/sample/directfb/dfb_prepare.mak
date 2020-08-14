#===============================================================================
# export variable
#===============================================================================
DFB_SAMPLE=$(shell pwd)

#===================================================================
#dfb example version
#==================================================================
DFB_SAMPLE_RES_DIR   := $(HI_OUT_DIR)/obj/sample/directfb/share/directfb-examples
DFB_SAMPLE_RES_DIR64 := $(HI_OUT_DIR)/obj64/sample/directfb/share/directfb-examples

DFB_EX_VER = 1.6.0

DIRECTFB_PREPARE = directfb_prepare
ifeq ($(HI_USER_SPACE_LIB),y)
DIRECTFB_PREPARE += directfb_prepare32
endif
ifeq ($(HI_USER_SPACE_LIB64),y)
DIRECTFB_PREPARE += directfb_prepare64
endif

#===================================================================
#compile
#==================================================================
.PHONY: all clean

all:$(DIRECTFB_PREPARE)
	$(AT)make -C bin
	$(AT)rm -rf bin/*.c
	$(AT)rm -rf bin/*.h
	$(AT)rm -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}

directfb_prepare:
	$(AT)test -d ${DFB_SAMPLE}/bin || mkdir -p ${DFB_SAMPLE}/bin
	$(AT)test -d ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER} || (tar xf $(OPEN_SOURCE_DIR)/DirectFB-examples-${DFB_EX_VER}.tar.gz -C ${DFB_SAMPLE})
	$(AT)cp -rf ${DFB_SAMPLE}/patch-for-DirectFB-Examples/DirectFB-examples-${DFB_EX_VER}.patch/* ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/
	$(AT)cp -rf ${DFB_SAMPLE}/patch-for-DirectFB-Examples/DirectFB-examples-new/*                 ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/
	$(AT)find ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER} -name "*.c" | xargs cp -t ${DFB_SAMPLE}/bin
	$(AT)find ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER} -name "*.h" | xargs cp -t ${DFB_SAMPLE}/bin

directfb_prepare32:directfb_prepare
	$(AT)test -d $(DFB_SAMPLE_RES_DIR) || mkdir -p $(DFB_SAMPLE_RES_DIR)
	$(AT)test -d $(DFB_SAMPLE_RES_DIR)/fonts || mkdir -p $(DFB_SAMPLE_RES_DIR)/fonts
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/res             $(DFB_SAMPLE_RES_DIR)
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/data/*          $(DFB_SAMPLE_RES_DIR)
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/data/df_neo/*   $(DFB_SAMPLE_RES_DIR)
	$(AT)mv $(DFB_SAMPLE_RES_DIR)/decker.dgiff                                $(DFB_SAMPLE_RES_DIR)/fonts
	$(AT)mv $(DFB_SAMPLE_RES_DIR)/decker.ttf                                  $(DFB_SAMPLE_RES_DIR)/fonts
	$(AT)find $(DFB_SAMPLE_RES_DIR) -name "Makefile*" | xargs rm -rf
	$(AT)rm -rf $(DFB_SAMPLE_RES_DIR)/df_neo

directfb_prepare64:directfb_prepare
	$(AT)test -d $(DFB_SAMPLE_RES_DIR64) || mkdir -p $(DFB_SAMPLE_RES_DIR64)
	$(AT)test -d $(DFB_SAMPLE_RES_DIR64)/fonts || mkdir -p $(DFB_SAMPLE_RES_DIR64)/fonts
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/res             $(DFB_SAMPLE_RES_DIR64)
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/data/*          $(DFB_SAMPLE_RES_DIR64)
	$(AT)cp -rf ${DFB_SAMPLE}/DirectFB-examples-${DFB_EX_VER}/data/df_neo/*   $(DFB_SAMPLE_RES_DIR64)
	$(AT)mv $(DFB_SAMPLE_RES_DIR64)/decker.dgiff                              $(DFB_SAMPLE_RES_DIR64)/fonts
	$(AT)mv $(DFB_SAMPLE_RES_DIR64)/decker.ttf                                $(DFB_SAMPLE_RES_DIR64)/fonts
	$(AT)find $(DFB_SAMPLE_RES_DIR64) -name "Makefile*" | xargs rm -rf
	$(AT)rm -rf $(DFB_SAMPLE_RES_DIR64)/df_neo
	
clean:
	$(AT)make -C bin clean
	$(AT)rm -rf bin/*.c
	$(AT)rm -rf bin/*.h
	$(AT)rm -rf $(HI_OUT_DIR)/obj/sample/directfb/share
	$(AT)rm -rf $(HI_OUT_DIR)/obj64/sample/directfb/share
