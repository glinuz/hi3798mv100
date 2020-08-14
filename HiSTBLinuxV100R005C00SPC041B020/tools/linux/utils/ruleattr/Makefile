################################################################################
#
#  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
#
#  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
#  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
#  others, published or used, in whole or in part, without the express prior
#  written permission of Hisilicon.
#
#  Create By Cai Zhiyong 2015/03/13
#
################################################################################

PREFIX ?= $(shell pwd)

AT  :=

CC  := gcc
AR  := ar

SRCS    := ruleattr.c ruleattr_offset64.c
OBJS    := $(addprefix $(PREFIX)/,$(SRCS:%.c=%.o))
LIB     := $(PREFIX)/libruleattr.a
CFLAGS  := -m32

.PHONY: all clean

all: $(LIB)

clean:
	$(AT)rm -rf $(LIB) $(OBJS)

$(LIB): $(OBJS)
	$(AT)$(AR) rcs $@ $^

$(PREFIX)/%.o : %.c
	$(AT)mkdir -p $(dir $@)
	$(AT)$(CC) $(CFLAGS) -o $@ -c $<
