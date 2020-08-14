#! /bin/sh
if [ $# -ne 3 ]
then
	echo -e "usage: ./vmxBuild.sh [image_file | all_image] sdk_dir advca_image_dir"
	echo -e "image_file: bootargs.bin hi_kernel.bin rootfs.squashfs apploader.bin"
	echo -e "all_image: should be defined at config.mk, which is located at root directory of 'tools/linux/utils/vmx'"
	exit 1
fi

key_file=
id_name=

sdk_dir=$2
advca_image_dir=$3
sign_output_dir="${advca_image_dir}/SignOutput"
sign_input_dir="${advca_image_dir}/SignInput"
sign_output_flash_dir="${sign_output_dir}/vmx_flash"
sign_output_upgrade_dir="${sign_output_dir}/vmx_upgrade"

vmx_dir="${sdk_dir}/tools/linux/utils/advca/vmx"
create_signature_sh="${vmx_dir}/create_signature.sh"
vmx_sh_temp_dir="${vmx_dir}/vmx_sh_temp_dir"
Align16="${vmx_dir}/source/Align16.c"
sample_ca_vmx_merge="${sdk_dir}/sample/advca/sample_ca_vmx_appsignsimulator.c"

vmx_init ()
{
	if [ ! -d ${sign_output_flash_dir} -o ! -d ${sign_output_upgrade_dir} ]
	then
		mkdir -p "${sign_output_flash_dir}"
		mkdir -p "${sign_output_upgrade_dir}"
	fi

	if [ ! -d ${sign_output_dir} ]
	then
		mkdir -p "${sign_output_dir}"
	fi

	mkdir -p ${vmx_sh_temp_dir}

	if [ ! -f "${vmx_dir}/source/Align16" ]
	then
		gcc ${Align16} -o ${vmx_dir}/source/Align16
		strip  ${vmx_dir}/source/Align16
	fi

	if [ ! -f "${vmx_dir}/sample_ca_vmx_merge" ]
	then
		gcc  ${sample_ca_vmx_merge} \
			-o ${vmx_dir}/sample_ca_vmx_merge
		strip  ${vmx_dir}/sample_ca_vmx_merge
	fi
}

software_signature ()
{
	mode=${2}
	if [ ${mode} -eq 0 ]
	then
		sign_file_dir="${sign_output_flash_dir}"
	elif [ ${mode} -eq 1 ]
	then
		sign_file_dir="${sign_output_upgrade_dir}"
	fi

	file_to_sign=${sign_input_dir}/${1}
	if [ ! -f "${file_to_sign}" ]
	then
		echo -e "no find file:'${file_to_sign}'"
		return 1
	fi

	signature_file=${vmx_sh_temp_dir}/$(echo "${1}" |awk -F "." '{print $1}')_sign.$(echo "${1}" |awk -F "." '{print $2}')
	signed_file=${vmx_sh_temp_dir}/$(echo "${1}" |awk -F "." '{print $1}')_encrypt.$(echo "${1}" |awk -F "." '{print $2}')
	final_signature_file=${sign_file_dir}/$(echo "${1}" |awk -F "." '{print $1}')_signature.$(echo "${1}" |awk -F "." '{print $2}')
	final_signature_head=${sign_file_dir}/$(echo "${1}" |awk -F "." '{print $1}')_head.$(echo "${1}" |awk -F "." '{print $2}')
	length_of_file=`stat -c %s ${file_to_sign}`
	length_of_signing=${length_of_file}

	Align=`expr 16 - $[ ${length_of_file} % 16 ]`
	if [ ${Align} -ne 16 ]
	then
		cp -f ${file_to_sign} ${vmx_sh_temp_dir}/${1}
		file_to_sign=${vmx_sh_temp_dir}/${1}
		${vmx_dir}/source/Align16 ${file_to_sign} $Align
	fi

	source ${create_signature_sh} ${length_of_file} ${length_of_signing} ${file_to_sign} ${signature_file} ${signed_file} ${key_file} ${mode} ${id_name}
	${vmx_dir}/sample_ca_vmx_merge ${signed_file} ${signature_file} ${final_signature_head} ${final_signature_file}
	rm -rf  ${final_signature_head}
	return 0
}

vmx_deinit ()
{
	rm -rf ${vmx_dir}/source/Align16
	rm -rf ${vmx_dir}/sample_ca_vmx_merge
	rm -rf ${vmx_sh_temp_dir}
	exit 1
}



vmx_init
if [ "$1" = "all_image" ]
then
	echo -e "************** start vmx signing **************"
	all_image_list="$(cat config.mk | awk -F "=" '{print $2}')"
	echo "image_list: ${all_image_list}"
	for device in ${all_image_list}
	do
		if [ -f "${sign_input_dir}/${device}" ]
		then
			## "mode" is defined by verimatrix, which means 0:flash, 1:upgrade
			software_signature ${device} 0
			software_signature ${device} 1
			echo "sign ${device} success!"
		else
			echo "file: ${sign_input_dir}/${device} not exist!"			
		fi
	done
	ls -l ${sign_output_flash_dir}
	ls -l ${sign_output_upgrade_dir}
	echo -e "************** end vmx signing **************\n"
	vmx_deinit
elif [ "$1" = "clean" ]
then
	if [ -d ${sign_output_flash_dir} ]
	then
		rm -rf ${sign_output_flash_dir}
	fi

	if [ -d ${sign_output_upgrade_dir} ]
	then
		rm -rf ${sign_output_upgrade_dir}
	fi

	vmx_deinit
else
	echo -e "************** start vmx signing **************"
	software_signature ${1} 0;
	if [ $? -ne 0 ]
	then
		echo -e "sign file:'${1}' failed"
		echo -e "************** end vmx signing **************\n"
		vmx_deinit
	fi

	software_signature ${1} 1
	if [ $? -ne 0 ]
	then
		echo -e "sign file:${1} failed"
		echo -e "************** end vmx signing **************\n"
		vmx_deinit
	fi

	echo "sign ${1} success!"
	ls -l ${sign_output_flash_dir}
	ls -l ${sign_output_upgrade_dir}
	echo -e "************** end vmx signing **************\n"

	vmx_deinit
fi

