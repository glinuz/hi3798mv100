
CSRC  = $(wildcard *.c)
COBJS := $(patsubst %.c,%.o,$(CSRC) )

ASRC  = $(wildcard *.S)
AOBJS := $(patsubst %.S,%.o,$(ASRC) )

LIBS := libcommon.a		

include $(TOPDIR)/rules.mk
