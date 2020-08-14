#! /bin/sh
#args: sig.sh ca_type
if [ $# -ne 3 ]
then
	echo -e "usage: sig.sh ca_type chip_type cfg_file" 
	echo -e "ca_type : conax | nagra | vmx | ... | other chip_type cfg_file"
	echo -e ""
	exit 1
fi

echo -e "NOTE:	This tool is only used for develop and self test!"
echo -e "	Cannot used for produce purpose!"

CA_NAME=$1
CHIP_TYPE=$2
CFG_FILE=$3

# nagra signature process 
# nagra is not supported now!
if [ $CA_NAME == "nagra" ]
then
	echo -e "nagra is not supported now!"
	exit 1
fi

# connax and vmx use special sig type.
if [ $CA_NAME == "conax" -o $CA_NAME == "vmx" ]
then
SIG_TYPE=special
else
SIG_TYPE=common
fi

SIGN_TOOL=./CASignTool

#remove old files!
rm -fr ./SignOutput

function process_cfg_file ()
{
	local advca_cfgfile=$1
	local chip=$2
	local cfg_file=$3

	if [ $chip == "hi3716mv310" ] || [ $chip == "hi3110ev500" ] ; then
		chip=Hi3716CV200
	elif [ $chip == "hi3716mv410" ] || [ $chip == "hi3716mv420" ] ; then
		chip=Hi3716MV410
	fi

	chmod +w $advca_cfgfile
	cp $advca_cfgfile $advca_cfgfile.bak

	sed 's/^Chip_Type=.*/Chip_Type='$chip'/g'  $advca_cfgfile.bak > $advca_cfgfile

	sed 's/^CFG=.*/CFG='$cfg_file'/g'  $advca_cfgfile.bak > $advca_cfgfile

	rm -f $advca_cfgfile.bak
}

# advca programmer:
echo -e "sig advca programmer"
cp sign_advca_programmer.cfg  Signboot_config.cfg
if [ -f ./SignInput/advca_programmer.bin ];then
	process_cfg_file Signboot_config.cfg $CHIP_TYPE $CFG_FILE
	$SIGN_TOOL 0 Signboot_config.cfg 
	if [ $? -eq 0 ];then
		mv SignOutput/FinalBoot.bin SignOutput/advca_programmer.sig
		rm -f SignOutput/*.bin
	else
		echo -e "Warning: sig advca programmer fail!"
	fi
else
	echo -E "Warning: ./SignInput/advca_programmer.bin is not exist!"
fi

# special signature type process 
if [ $SIG_TYPE == "special" ]
then
	echo -e "sig type: special"

	# sig boot
	echo -e "sig boot image!"

	cp sign_boot.cfg Signboot_config.cfg 

	process_cfg_file Signboot_config.cfg $CHIP_TYPE $CFG_FILE

	$SIGN_TOOL 0 Signboot_config.cfg
	if [ $? -ne 0 ];then
		echo "sign boot fail!\n"
	fi

	# sig bootargs 
	echo -e "sig bootargs image!"
	$SIGN_TOOL 3 special_bootargs_config.cfg	

	# sig kernel and rootfs.
	echo -e "sig system image!"
	$SIGN_TOOL 3 special_system_config.cfg

	# sig apploader
	if [ -f ./SignInput/apploader.bin ];then
		echo -e "sig apploader image!"
		$SIGN_TOOL 3 special_apploader_config.cfg 
	else
		echo -e "Warning: apploader.bin is not exist in SignInput!"
	fi

	# sig stbid
	echo -e "sig stbid image!"
	if [ -f ./SignInput/stbid.bin ];then
		$SIGN_TOOL 3 special_stbid.cfg
	else
		echo -e "Warning: stbid.bin is not exist in SignInput!"
	fi
else
# common signature type process

	echo -e "sig type common!"
	
	# sig boot
	echo -e "sig boot image!"
	cp sign_boot.cfg Signboot_config.cfg 
	$SIGN_TOOL 0 Signboot_config.cfg

	# sig bootargs 
	echo -e "sig bootargs image!"
	$SIGN_TOOL 2 common_bootargs_config.cfg
	# sig  kernel and rootfs
	echo -e "sig kernel image!"
	$SIGN_TOOL 2 common_kernel_config.cfg

	echo -e "sig rootfs image!"
	$SIGN_TOOL 2 common_rootfs_config.cfg

	# sig apploader
	echo -e "sig apploader image!"
	if [ -f ./SignInput/apploader.bin ];then
		$SIGN_TOOL 2 common_apploader_config.cfg
	else
		echo -e "Warning: apploader.bin is not exist in SignInput!"
	fi

	# sig stbid
	echo -e "sig stbid image!"
	if [ -f ./SignInput/stbid.bin ];then
		$SIGN_TOOL 2 common_stbid.cfg
	else
		echo -e "Warning: stbid.bin is not exist in SignInput!"
	fi

fi

