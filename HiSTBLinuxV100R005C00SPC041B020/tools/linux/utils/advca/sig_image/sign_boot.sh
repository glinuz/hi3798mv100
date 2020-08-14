#! /bin/sh

if [ $# -ne 3 ]
then
	echo -e "usage: sign_boot.sh ca_type chip_type cfg_file" 
	echo -e "ca_type : conax | nagra | vmx | ... | other chip_type cfg_file"
	echo -e ""
	exit 1
fi

echo -e "NOTE:	This tool is only used for develop and self test!"
echo -e "	Cannot used for produce purpose!"

CA_NAME=$1
CHIP_TYPE=$2
CFG_FILE=$3

# nagra is not supported now!
if [ $CA_NAME == "nagra" ]
then
	echo -e "nagra is not supported now!"
	exit 1
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
#	rm -f $advca_cfgfile.bak
}

# advca programmer:
echo -e "sign advca programmer"
#BOOT_FILE=$(sed -n 's/BootFile=/BootFile=/p' Signboot_config.cfg | awk -F "BootFile=" '{print $2}')
#BOOT_INPUT_DIR=$(sed -n 's/InputFileDir=/InputFileDir=/p' Signboot_config.cfg | awk -F "InputFileDir=" '{print $2}')
#BOOT_OUTPUT_DIR=$(sed -n 's/OutputFileDir=/OutputFileDir=/p' Signboot_config.cfg | awk -F "OutputFileDir=" '{print $2}')

cp sign_advca_programmer.cfg  Signboot_config.cfg
if [ -f ./SignInput/advca_programmer.bin ];then
	process_cfg_file Signboot_config.cfg $CHIP_TYPE $CFG_FILE
	$SIGN_TOOL 0 Signboot_config.cfg 
	if [ $? -eq 0 ];then
		mv ./SignOutput/FinalBoot.bin ./SignOutput/advca_programmer.sig
		rm -f ./SignOutput/FinalBoot.bin
	else
		echo -e "sign advca programmer fail!"
	fi
else
	echo -e "./SignInput/advca_programmer.bin is not exist!"
fi

# sig boot
echo -e "sign boot image!"
cp sign_boot.cfg Signboot_config.cfg 
#BOOT_FILE=$(sed -n 's/BootFile=/BootFile=/p' Signboot_config.cfg | awk -F "BootFile=" '{print $2}')
#BOOT_INPUT_DIR=$(sed -n 's/InputFileDir=/InputFileDir=/p' Signboot_config.cfg | awk -F "InputFileDir=" '{print $2}')
#BOOT_OUTPUT_DIR=$(sed -n 's/OutputFileDir=/OutputFileDir=/p' Signboot_config.cfg | awk -F "OutputFileDir=" '{print $2}')
cp sign_boot.cfg  Signboot_config.cfg
if [ -f ./SignInput/fastboot-burn.bin ];then
	process_cfg_file Signboot_config.cfg $CHIP_TYPE $CFG_FILE
	$SIGN_TOOL 0 Signboot_config.cfg 
	if [ $? -ne 0 ];then
		echo -e "sign advca programmer fail!"
	fi
else
	echo -e "./SignInput/fastboot-burn.bin is not exist!"
fi

