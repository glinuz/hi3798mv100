DFB_SAMPLE_DIR=$(shell pwd)

ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../..
include $(SDK_DIR)/base.mak
endif

.PHONY: all clean install
all:
	cd ${DFB_SAMPLE_DIR}/src ; make -f Makefile-32 install
	cd ${DFB_SAMPLE_DIR}/src/df_neo ; make -f Makefile-32 install
	cd ${DFB_SAMPLE_DIR}/sample_new ; make -f Makefile-32 install
	cd ${DFB_SAMPLE_DIR}/src ; make -f Makefile-32  clean
	cd ${DFB_SAMPLE_DIR}/src/df_neo ; make -f Makefile-32 clean
	cd ${DFB_SAMPLE_DIR}/sample_new ; make -f Makefile-32 clean
ifneq ($(CFG_HI_AARCH64_TOOLCHAINS_NAME),)
	cd ${DFB_SAMPLE_DIR}/src ; make -f Makefile-64 install
	cd ${DFB_SAMPLE_DIR}/src/df_neo ; make -f Makefile-64 install
	cd ${DFB_SAMPLE_DIR}/sample_new ; make -f Makefile-64 install
endif
clean:
	cd ${DFB_SAMPLE_DIR}/src ; make -f Makefile-32  clean
	cd ${DFB_SAMPLE_DIR}/src/df_neo ; make -f Makefile-32 clean
	cd ${DFB_SAMPLE_DIR}/sample_new ; make -f Makefile-32 clean
ifneq ($(CFG_HI_AARCH64_TOOLCHAINS_NAME),)
	cd ${DFB_SAMPLE_DIR}/src ; make -f Makefile-64 clean
	cd ${DFB_SAMPLE_DIR}/src/df_neo ; make -f Makefile-64  clean
	cd ${DFB_SAMPLE_DIR}/sample_new ; make -f Makefile-64  clean
endif

install:all
