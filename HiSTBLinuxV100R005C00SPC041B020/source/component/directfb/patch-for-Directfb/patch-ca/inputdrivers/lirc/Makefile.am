## Makefile.am for DirectFB/inputdrivers/lirc
SDK_DIR=./../../../../../../pub
LIB=${SDK_DIR}/lib/static/libhi_common.a \
    ${SDK_DIR}/lib/static/libhi_mpi.a
LIB+=-lrt

INCLUDES = \
	-I$(top_srcdir)/include	\
	-I$(top_builddir)/lib	\
	-I$(top_srcdir)/lib	\
	-I$(top_srcdir)/src	\
	-I$(top_builddir)/include \
        -I$(SDK_DIR)/include

lirc_LTLIBRARIES = libdirectfb_lirc.la

if BUILD_STATIC
lirc_DATA = $(lirc_LTLIBRARIES:.la=.o)
endif

lircdir = $(MODULEDIR)/inputdrivers

libdirectfb_lirc_la_SOURCES =	\
	lirc.c

libdirectfb_lirc_la_LDFLAGS = \
	-module			\
	-avoid-version		\
	$(DFB_LDFLAGS)          \
	$(LIB)

libdirectfb_lirc_la_LIBADD = \
	$(top_builddir)/lib/direct/libdirect.la \
	$(top_builddir)/src/libdirectfb.la


include $(top_srcdir)/rules/libobject.make
	
