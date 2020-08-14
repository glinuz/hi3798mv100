#! /bin/sh
if [ $# -ne 5 ]
then
        echo -e "usage: $0 ca_type chip_type sdk_dir image_dir boot_reg_file_name" 
        echo -e "ca_type : CONAX | NAGRA | VERIMATRIX | ... | "
        echo -e "This tool is only used for develop and self test!Cannot used for produce perpose!"
        exit 1
fi

CA_NAME=$1
CHIP_TYPE=$2
SDK_DIR=$3
IMAGE_DIR=$4
BOOT_REG_NAME=$5
BOOT_DIR=${SDK_DIR}/source/boot
ADVCA_SIGN_CFG_NAME=${BOOT_REG_NAME/reg/cfg}
CA_TOOL_DIR=${SDK_DIR}/tools/linux/utils/advca
VMX_SIGN_TOOL_DIR=${CA_TOOL_DIR}/vmx

function prepare ()
{	
	cp -afr ${IMAGE_DIR}/*.bin ./SignInput/
	find ${IMAGE_DIR} -maxdepth 1 -name rootfs* | xargs -r cp -t ./SignInput/
	cp ${BOOT_DIR}/sysreg/${CHIP_TYPE}/${ADVCA_SIGN_CFG_NAME} ./SignInput/
	uname -a | grep -rnw "x86_64" > /dev/null
	if [ 0 -eq $? ] ; then 
		cp ${SDK_DIR}/tools/linux/utils/advca/CASignTool_Linux/64bit/bin/CASignTool ./
	else
		cp ${SDK_DIR}/tools/linux/utils/advca/CASignTool_Linux/32bit/bin/CASignTool ./
	fi	
	chmod a+x ./*
	chmod a+x ${VMX_SIGN_TOOL_DIR}/vmxBuild.sh	
}

function sig_image ()
{
	if [ $CA_NAME == "CONAX" ]; then
		./sig.sh conax ${CHIP_TYPE} ${ADVCA_SIGN_CFG_NAME}
		cp ./SignOutput/*.sig ${IMAGE_DIR}/
		if [ -e ./SignInput/fastboot-burn.bin ] ; then
		cp ./SignOutput/FinalBoot.bin ${IMAGE_DIR}/
		else
		echo "warning:./SignOutput/FinalBoot.bin does not exist!"
		fi
	fi
	
	if [ $CA_NAME == "VERIMATRIX" ]; then
		if [ -e ./SignInput/miniboot.bin ] ; then
			mv ./SignInput/miniboot.bin ./SignInput/fastboot-burn.bin
			./sign_boot.sh vmx ${CHIP_TYPE} ${ADVCA_SIGN_CFG_NAME}
		else
			echo "warning: miniboot.bin does not exist ! can not sig the boot"
		fi
		cd ${VMX_SIGN_TOOL_DIR}
		./vmxBuild.sh all_image ${SDK_DIR} ${CA_TOOL_DIR}/sig_image
		cd -
		if [ -e ./SignOutput/advca_programmer.sig ] && [ -e ./SignOutput/FinalBoot.bin ] ; then
			cp ./SignOutput/advca_programmer.sig ${IMAGE_DIR}/
			cp ./SignOutput/FinalBoot.bin ${IMAGE_DIR}/
		fi
		cp ./SignOutput/vmx_flash/* ${IMAGE_DIR}/
	fi
}

rm -fr ./SignOutput
if [ $CHIP_TYPE == "hi3798cv200" ] || [ $CHIP_TYPE == "hi3796mv200" ]; then
	echo "Createsignature not support " $CHIP_TYPE
	exit 0
fi
prepare
sig_image






