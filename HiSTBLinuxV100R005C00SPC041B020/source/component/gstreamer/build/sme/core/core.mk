###########################################################
#Copyright (C) 2016-2026 2012 Euler SME.
#core.mk
###########################################################

#not show this command
NS:=@

#ignore the result
NC:=-

TOP := $(shell pwd)/../..

ALL_MODULES :=
EXTRA_CLEAN_MODULES :=

.PHONY:help flysme
help:help-info

all:all_modules

include ./core/config.mk
include ./core/definitions.mk
include ./module-list.mk

all_modules:$(ALL_MODULES)
	@echo "all modules build success!"

prebuild:

#KCONFIG_EXE = target/host/bin/himonf
#KCONFIG_CFG = scripts/kconfig/mainKconfig.cfg

#menuconfig:himonf hiconf
#	$(KCONFIG_EXE) $(KCONFIG_CFG)
#	@chmod -R 777 cfg.mk
#	@cat .config > cfg.mk


.PHONY:show_modules
show_modules:
	@echo "Available sub-modules:"
	@echo "$(ALL_MODULES)" | \
	tr -s ' ' '\n' | sort -u | column

export_all_modules:
	@echo $(ALL_MODULES) > ~/.export_all_modules

tools:

#.PHONY:test
#test:rootbox_install

check:
	@echo "CROSS:$(CROSS)"
	@echo "TARGET_CC:$(TARGET_CC)"
	@echo "TARGET_CXX:$(TARGET_CXX)"
	@echo "TOP:$(TOP)"
	@echo "TOP_DIR:$(TOP_DIR)"
	@echo "OUTDIR:$(OUTDIR)"
	@echo "OBJ_DIR:$(TARGET_OUT_OBJ_DIR)"
	@echo "BIN_DIR:$(TARGET_OUT_BIN_DIR)"
	@echo "LIB_DIR:$(TARGET_OUT_LIB_DIR)"
	@echo "all_modules are :$(ALL_MODULES)"

extra_cleans:$(EXTRA_CLEAN_MODULES)
	@echo "extra modules clean done!"

clean:extra_cleans
	@rm -rf $(TARGET_OUTDIR)/*
	@rm -rf $(TOPDIR)/$(CFG_RT_SME_THIRDPARTY_MERGE_DIR)/*

help-info:
	@echo  "==============================================================================="
	@echo  "==========================     Makefile Target Help    ========================="
	@echo  "==============================================================================="
	@echo  "  Makefile targets:"
	@echo  "  clean                   - remove all generated files from all  modules"
	@echo  "  help (or blank)         - display this help"
	@echo  "  all		              - build all modules for this project"
	@echo  "  LOCAL_MODULE            - build module as requred,for example,use 'make libbinder_ipc' to compile ipc modules"
	@echo  "  LOCAL_MODULE-clean      - remove all generated files from module LOCAL_MODULE,for example,make libbinder-clean"
	@echo  "  LOCAL_MODULE-check      - check module flags and source,objs for LOCAL_MODULE,for example,make libbinder-check"
	@echo  "  show_modules            - show all the Available modules in the project"
	@echo  ""
