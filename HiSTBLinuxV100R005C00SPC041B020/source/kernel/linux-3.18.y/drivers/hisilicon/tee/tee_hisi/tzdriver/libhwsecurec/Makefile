# Makefile for secure functions.
#
# Each configuration option enables a list of files.

obj-$(CONFIG_TEE_LIBHWSECUREC) += memcpy_s.o memset_s.o strcpy_s.o strncpy_s.o \
		memmove_s.o strcat_s.o strncat_s.o strtok_s.o \
		securecutil.o secureprintoutput_a.o snprintf_s.o output.o vsnprintf_s.o

ccflags-y += -Idrivers/hisilicon/tee/tee_hisi/tzdriver
