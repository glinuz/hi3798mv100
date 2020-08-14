# Makefile for the TEE root scan client.
#
# Each configuration option enables a list of files.

obj-$(CONFIG_TEE_ANTIROOT_CLIENT)  += rootagent_api.o\
    rootagent.o rootagent_crypto.o rootagent_mem.o
obj-$(CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API) += rscan_dummy.o

ccflags-y +=	-Idrivers/hisi/tzdriver \
		-Idrivers/hisi/tzdriver/libhwsecurec

ccflags-$(CONFIG_TEE_ANTIROOT_CLIENT) +=\
    -Isecurity/hw_root_scan/include
