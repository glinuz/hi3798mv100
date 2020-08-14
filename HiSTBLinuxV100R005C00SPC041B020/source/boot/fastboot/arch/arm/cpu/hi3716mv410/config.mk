PLATFORM_RELFLAGS += -fno-strict-aliasing -fno-common -ffixed-r8
#		     -msoft-float

# Make ARMv5 to allow more compilers to work, even though its v7a.
#PLATFORM_CPPFLAGS += -march=armv5
PLATFORM_CPPFLAGS += -march=armv7-a
# =========================================================================
#
# Supply options according to compiler version
#
# =========================================================================
PLATFORM_RELFLAGS +=$(call cc-option,-mshort-load-bytes,\
		    $(call cc-option,-malignment-traps,))
