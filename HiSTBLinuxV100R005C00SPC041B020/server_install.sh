#! /bin/sh
#============================================
#  Create for Hisilicon STB SDK configure
#============================================

################################################################################
#  args:
#    $0  - line number
#    $1  - error message
#    $3  - command for install tools
#
error()
{
	cmd=${3-0}
	echo;
	if [ "${cmd}" == "0" ];then
	{
		echo "error: ${0}(${1}): ${2}"
		exit 1;
	}
	fi
	echo "warning: ${0}(${1}): ${2}"
	${cmd}
	if [ $? != 0 ];then
	{
		echo "fail :${cmd}"
		exit 1;
	}
	fi
}

################################################################################
#
#  '<' 
#  '='
#  '>'
#
CMD_RET="="
#
#  args: $0 "xxx.xxx.xxx"
#        $1  ver0
#        $2  ver1
#        $3  ver2
#
cmp_version()
{
	src=${1}"."
	ver0=$2
	ver1=${3-0}
	ver2=${4-0}

	src_ver0=$(echo ${src} | sed 's/[.].*//')

	if [ ${src_ver0} -lt ${ver0} ]; then {
		CMD_RET="<";
		return;
	} elif [ ${src_ver0} -gt ${ver0} ]; then {
		CMD_RET=">";
		return
	} fi

	src=$(echo ${src} | sed "s/${src_ver0}[.]//")
	src_ver1=$(echo ${src} | sed 's/[.].*//')

	if [ ${src_ver1} -lt ${ver1} ]; then {
		CMD_RET="<";
		return;
	} elif [ ${src_ver1} -gt ${ver1} ];then {
		CMD_RET=">";
		return
	} fi

	src=$(echo ${src} | sed "s/${src_ver1}[.]//")
	if [ "s${src}" = "s" ]; then
		src_ver2=0
	else
		src_ver2=$(echo ${src} | sed 's/[.].*//')
	fi

	if [ ${src_ver2} -lt ${ver2} ]; then {
		CMD_RET="<";
		return;
	} elif [ ${src_ver2} -gt ${ver2} ];then {
		CMD_RET=">";
		return
	} fi

	CMD_RET="=";
}
################################################################################
#
#  $1 "command"
#  $2 "result"   format  "xxx.xxx.xxx"
#  $3 ">=" | "="
#  $4 $5 $6 "version"
#
check_tools()
{
	util=$1
	result=$2
	condition=$3
	ver0=$4
	ver1=${5-0}
	ver2=${6-0}
	
	echo -n "Check ${util}: "

	tmp=$(echo "${result}" | awk '/[0-9]\.[0-9]|[0-9][0-9]\.[0-9]/{print $1}')
	if [ "s${tmp}" = "s" ];then
	{
		error ${LINENO} "${util} is not exist, please install." "sudo apt-get install ${util}"
		return 0
	}
	fi

	echo -n "version:${result} "

	result=$(echo ${result} | sed 's/[^0-9\.]//g')

	cmp_version "${result}" ${ver0} ${ver1} ${ver2}

	if [ "${condition}" = ">=" -a "${CMD_RET}" = "<" ]; then
	{
		error ${LINENO} "${util} Version is too old, please update. Version ${ver0}.${ver1}.${ver2} or later is recommended"
		return 0
	}
	fi

	if [ "${condition}" = "=" -a "${CMD_RET}" != "=" ]; then
		error ${LINENO} "${util} Version is not support. Please install ${util} ${ver0}.${ver1}.${ver2}. You could download the package from the internet." 
	fi

	echo "... OK"
}
################################################################################
#
#  $1 "lib"
#
check_libs()
{
	util=$1

	echo -n "Check ${util}: "
	
	msg=$(dpkg -s ${util})
	if [ $? != 0 ]; then
	{
		error ${LINENO} "${util} is not exist, please install." "sudo apt-get install ${util}"
		msg=$(dpkg -s ${util})	
	}
	fi

	result=$(echo "${msg}" | awk '$0~/^Version:/{print $$0}')
	echo "${result} ... OK"
}
################################################################################
#  check linux version
################################################################################
echo "Check linux host:"

ubuntu_version_info=$(lsb_release -a)
if [ $? != 0 ]; then
	error ${LINENO} "Command lsb_release is not exist."
fi
echo -e "${ubuntu_version_info}"
result=$(echo "${ubuntu_version_info}" | grep "Ubuntu")
if [ "s${result}" = "s" ]; then
	error ${LINENO} "Only Support Ubuntu"
fi

result=$(lsb_release -sr)
cmp_version "${result}" 10 0
if [ "${CMD_RET}" = "<" ]; then
	error ${LINENO} "Ubuntu version is too old. Ubuntu 10 or later is recommended."
fi

echo;
################################################################################
#  check the type of shell
################################################################################
echo -n "Check bash: "
result_bash=$(bash --version)
if [ $? != 0 ];then
{
	error ${LINENO} "bash is not exist, Please install bash" "sudo aptget-install bash"
	result_bash=$(bash --version)
}
fi

result_sh=$(sh --version)
if [ "${result_bash}" != "${result_sh}" ];then
{
	error ${LINENO} "sh is not bash, Should modify it." "sudo rm -f /bin/sh"
	sudo ln -s /bin/bash /bin/sh
	echo "Modify Successfully!"
}
fi

echo "... OK"

################################################################################
#  check the necessary tools
################################################################################
result=$(gcc -dumpversion)
check_tools "gcc" "${result}" ">=" 4 4 0

result=$(make --version | awk '$0~/[0-9]\.[0-9]/{print $3}')
check_tools "make" "${result}" "=" 3 81 0

result=$(msgfmt --version | awk '$0~/[0-9]\.[0-9]/{print $4}')
check_tools "gettext" "${result}" ">=" 0 18 1

result=$(bison --version | awk '$0~/[0-9]\.[0-9]/{print $4}')
check_tools "bison" "${result}" ">=" 2 5 0

result=$(flex --version | awk '$0~/[0-9]\.[0-9]/{print $2}')
check_tools "flex" "${result}" ">=" 2 5 35

result=$(bc --version | awk '$0~/[0-9]\.[0-9]/{print $2}')
check_tools "bc" "${result}" ">=" 1 06 95

check_libs zlib1g-dev

check_libs libncurses5-dev

result=$(lzma --version | awk '$0~/lzma.[0-9]\.[0-9]/{print $2}')
check_tools "lzma" "${result}" ">=" 5 1 0

echo;
