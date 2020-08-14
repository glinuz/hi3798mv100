#!/bin/sh

echo -e "This script is for setting the advca configuration \
	\nacording to the default cfg.mak in configs/hi37xx/ "
if [ $# -ne 3 ]
then	
	echo -e "\nusage: \
	\n./advca_env_set.sh  sdk_dir ca_type ca_funtcion \
	\nca_type:\
	\n	conax\
	\n	novel\
	\n	nagra\
	\n	suma\
	\n	cti\
	\n	verimatrix\
	\n	irdeto\
	\n	panaccess\
	\n	dcas\
	\n	other\
	\nca_funtcion:\
	\n	debug\
	\n	release\
	\n	final\
	\nfor example: ./advca_env_set.sh /home/v1r2/Code conax final"
	exit 1
fi

sdk_dir=$1
ca_type=$2
ca_type=`tr '[:lower:]' '[:upper:]' <<< $ca_type`

ca_funtcion=$3
CA_FUNCTION=`tr '[:lower:]' '[:upper:]' <<< $ca_funtcion`

kernel_suffix=-ca-${ca_funtcion}_defconfig
busybox_suffix=glibc_ca_root_${ca_funtcion}.config
cfg=$1/cfg.mak

if [ ! -e $cfg ];then
	echo error , $cfg not exist, may be you should first exec 'cp configs/hi37xx/xx cfg.mak', exit!
	exist 1
fi

echo "---------advca env set begin-------"
enable=" CFG_HI_LOADER_SUPPORT "
enable+=" CFG_HI_BOOT_COMPRESSED "
enable+=" CFG_HI_ADVCA_SUPPORT "
enable+=" CFG_LIB_JPEG_VERSION_9A "
enable+=" CFG_LIB_PNG_VERSION_1_6_16 "
enable+=" CFG_ADVCA_${ca_type} "
enable+=" CFG_FUNCTION_${CA_FUNCTION} "


disable=" CFG_HI_TOOL_DOSFS_SUPPORT "

if [ $ca_funtcion != "debug" ] ; then
	disable+=" CFG_HI_ETH_SUPPORT "
	# Flash Config
	disable+=" CFG_HI_SPI_SUPPORT "
	disable+=" CFG_HI_SPI_BLOCK_SIZE "
	disable+=" CFG_HI_TOOL_GDB_SUPPORT "
	disable+=" CFG_HI_TOOL_MSP_DEBUG_SUPPORT "
	disable+=" CFG_HI_TOOL_PQ_DEBUG_SUPPORT "
fi

# Board Tools Config
disable+=" CFG_HI_TOOL_DOSFS_SUPPORT "
disable+=" CFG_HI_TOOL_E2FSPROGS_SUPPORT "
disable+=" CFG_HI_TOOL_REG_SUPPORT "
disable+=" CFG_HI_TOOL_TOP_SUPPORT "
disable+=" CFG_HI_WIFI_SUPPORT" 
disable+=" CFG_HI_FAT_SUPPORT "
disable+=" CFG_HI_NTFS_SUPPORT "

# Caption Config
disable+=" CFG_HI_PLAYER_SUPPORT "
disable+=" CFG_HI_PLAYER_FULL_SUPPORT "

# Graphic Config
disable+=" CFG_LIB_JPEG_VERSION_6B "
disable+=" CFG_LIB_PNG_VERSION_1_4_16 "

disable+=" CFG_HI_TOOL_DOSFS_SUPPORT "
disable+=" CFG_HI_TOOL_DOSFS_SUPPORT "
disable+=" CFG_HI_TOOL_DOSFS_SUPPORT "
disable+=" CFG_HI_EXFAT_SUPPORT "

# SCI Config
disable+=" CFG_HI_VI_SUPPORT "


# Flash Config
delete=" CFG_HI_SPI_BLOCK_SIZE "
delete+=" CFG_HI_ROOTFS_JFFS2 "
delete+=" CFG_HI_WIFI_DEVICE_RTL8188EUS "
delete+=" CFG_HI_WIFI_MODE_STA "
delete+=" CFG_HI_WIFI_MODE_AP "
delete+=" CFG_HI_WIFI_DEVICE_RTL8188EUS "
delete+=" CFG_HI_NTFS_MODULE_SUPPORT "
delete+=" CFG_HI_NTFS_TOOLS_SUPPORT "

# Common
delete+=" CFG_HI_LOG_SUPPORT "
delete+=" CFG_HI_LOG_LEVEL "
delete+=" CFG_HI_LOG_NETWORK_SUPPORT "
delete+=" CFG_HI_LOG_UDISK_SUPPORT "
delete+=" CFG_HI_PROC_SUPPORT "

# SCI Config
delete+=" CFG_HI_VI_BT656_HIGH8 "

# PowerManagement Config
delete+=" CFG_HI_PM_POWERUP_MODE1_SUPPORT "

echo "add some configuration"
sed  -i "/CFG_HI_LOADER_SUPPORT/ 		a\# CFG_HI_BOOTLOADER_SUPPORT is not set\
						\nCFG_HI_APPLOADER_SUPPORT=y" $cfg

sed  -i "/CFG_HI_TOOL_REG_UART_SUPPORT/ 	a\CFG_HI_TOOL_SANDBOX_SUPPORT=y" 			$cfg
sed  -i "/CFG_HI_BUSYBOX_CFG_NAME/ 		a\CFG_HI_BUSYBOX_CFG_SANDBOX_NAME=glibc_ca_sandbox.config" $cfg

sed  -i "/CFG_HI_ADVCA_SUPPORT/ 		a\# CFG_ADVCA_CONAX is not set\
						\n# CFG_ADVCA_NOVEL is not set\
						\n# CFG_ADVCA_NAGRA is not set\
						\n# CFG_ADVCA_SUMA is not set\
						\n# CFG_ADVCA_CTI is not set\
						\n# CFG_ADVCA_VERIMATRIX is not set\
						\n# CFG_ADVCA_IRDETO is not set\
						\n# CFG_ADVCA_PANACCESS is not set\
						\n# CFG_ADVCA_DCAS is not set\
						\n# CFG_ADVCA_OTHER is not set\
						\nCFG_HI_ADVCA_TYPE=${ca_type}\
						\nCFG_ADVCA_FUNCTION=y\
						\n# CFG_FUNCTION_DEBUG is not set\
						\n# CFG_FUNCTION_RELEASE is not set\
						\n# CFG_FUNCTION_FINAL is not set\
						\nCFG_HI_ADVCA_FUNCTION=${CA_FUNCTION}\
						\nCFG_HI_STATIC_LIB_ONLY=y\
						\nCFG_HI_ADVCA_VERIFY_ENABLE=y\
						\nCFG_HI_ADVCA_USE_EXT1_RSA_KEY=y\
						\nCFG_HI_ADVCA_SANDBOX_SUPPORT=y\
						\nCFG_SANDBOX_CHROOT=y\
						\n# CFG_SANDBOX_LXC is not set\
						\nCFG_HI_ADVCA_SANDBOX_TYPE=CHROOT" $cfg

for ff in $enable ; do
	echo "enable"  $ff
	sed  -i "/$ff/ 		c\\$ff=y" ./cfg.mak
done

for ff in $disable ; do 
	echo "disable"  $ff
	sed  -i "/$ff/ 		c\# $ff is not set" ./cfg.mak
done

for ff in $delete ; do 
	echo "delete"  $ff
	sed  -i "/$ff/d" 	./cfg.mak
done

echo "change some configuration"
sed  -i "/CFG_HI_BOOT_ENV_STARTADDR/ 		c\CFG_HI_BOOT_ENV_STARTADDR=0x100000" 				$cfg
sed  -i "/CFG_HI_BOOT_ENV_STARTADDR/ 		c\CFG_HI_BOOT_ENV_STARTADDR=0x100000" 				$cfg
sed  -i "/CFG_HI_BOOT_ENV_SIZE/ 		c\CFG_HI_BOOT_ENV_SIZE=0x20000" 				$cfg
sed  -i "/CFG_HI_BOOT_ENV_RANGE/ 		c\CFG_HI_BOOT_ENV_RANGE=0x100000" 				$cfg
sed  -i "/CFG_HI_KERNEL_CFG/ 			s/_defconfig/${kernel_suffix}/" 				$cfg
sed  -i "/CFG_HI_BUSYBOX_CFG_NAME/ 		s/glibc.config/${busybox_suffix}/" 				$cfg
#sed  -i "/CFG_HI_BUSYBOX_CFG_NAME/ 		c\CFG_HI_BUSYBOX_CFG_NAME=glibc_ca_root_${ca_funtcion}.config" 	$cfg

echo "advca env set finished"
exit 
