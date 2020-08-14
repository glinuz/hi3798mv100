###########################################################
#Copyright (C) 2015-2020 Hisilicon
#opensource_pkg.mk
###########################################################


LOCAL_MODULE := $(strip $(LOCAL_MODULE))
ifeq ($(LOCAL_MODULE),)
  $(error $(LOCAL_PATH): LOCAL_MODULE is not defined)
endif

ALL_MODULES +=$(LOCAL_MODULE)


.PHONY:$(LOCAL_MODULE)

EXTRA_CLEAN_MODULES += $(LOCAL_MODULE)-clean
