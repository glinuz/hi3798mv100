#Makefile for mkyaffs
#
# NB this is not yet suitable for putting into the kernel tree.
# YAFFS: Yet another Flash File System. A NAND-flash specific file system.
#
# Copyright (C) 2002 Aleph One Ltd.
#   for Toby Churchill Ltd and Brightstar Engineering
#
# Created by Charles Manning <charles@aleph1.co.uk>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.

## Change or override  KERNELDIR to your kernel

AM_CFLAGS  = $(CFLAGS) -I/usr/include -O2 -Wall -DCONFIG_YAFFS_UTIL
AM_CFLAGS += -Wshadow -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -Wmissing-declarations
AM_CFLAGS += -Wmissing-prototypes -Wredundant-decls -Wnested-externs -Winline

AM_LDFLAGS = $(LDFLAGS) -lruleattr

CC = $(MAKETOOLS)gcc -m32

MKYAFFS2SOURCES = yaffs_ecc.c yaffs_packedtags2.c yaffs_tagsvalidity.c

ifeq ($(CFG_SPI_NAND_SUPPORT),y)
CFLAGS     += -DCFG_SPI_NAND_SUPPORT
endif

all : mkyaffs

MKYAFFS2IMAGEOBJS = $(MKYAFFS2SOURCES:.c=.o)

%.o: %.c
	$(CC) -c $(AM_CFLAGS) $< -o $@

mkyaffs: $(MKYAFFS2IMAGEOBJS) mkyaffs2image.o oob_config.o
	$(CC) -o $@ $(MKYAFFS2IMAGEOBJS) mkyaffs2image.o oob_config.o $(AM_LDFLAGS)

clean:
	rm -f $(MKYAFFS2IMAGEOBJS) mkyaffs mkyaffs2image*.o oob_config*.o
