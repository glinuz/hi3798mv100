## Makefile.am for DirectFB/src/core/gfxcards/vmware
SDK_DIR=./../../../../../../pub
INCLUDES = \
	-I$(top_builddir)/include      \
	-I$(top_srcdir)/include	       \
	-I$(top_builddir)/lib          \
	-I$(top_srcdir)/lib	       \
	-I$(top_srcdir)/src    	       \
	-I$(top_srcdir)/systems        \
        -I$(SDK_DIR)/include

tde_LTLIBRARIES = libdirectfb_tde.la

if BUILD_STATIC
tde_DATA = $(tde_LTLIBRARIES:.la=.o)
endif

tdedir = $(MODULEDIR)/gfxdrivers

libdirectfb_tde_la_SOURCES =	\
	tde_2d.c		\
	tde_2d.h		\
	tde_gfxdriver.c	        \
	tde_gfxdriver.h         \
        tde_driver.c            \
        tde_driver.h

LIB=${SDK_DIR}/lib/static/libhi_common.a \
    ${SDK_DIR}/lib/static/libhi_mpi.a

libdirectfb_tde_la_LDFLAGS =    \
	-module			\
	-avoid-version		\
	$(DFB_LDFLAGS) -lm      \
        ${LIB}

libdirectfb_tde_la_LIBADD = \
	$(top_builddir)/lib/direct/libdirect.la \
	$(top_builddir)/src/libdirectfb.la


include $(top_srcdir)/rules/libobject.make
	
