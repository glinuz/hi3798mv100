#!/bin/bash

###############################################################################
#
#  Ceate By Czyong
#
###############################################################################

function usage ()
{
	echo "Usage: ${selfname} Pagesize Blocksize Dir Size"
	echo "  Pagesize      NAND page size. 2k/4k/8k."
	echo "  Blocksize     NAND block size. 128k/1M "
	echo "  Dir           the directory you want to make ubifs"
	echo "  Size          this ubifs partition size in NAND. 48M, ... 50M"
	echo ""
	echo "Example:"
	echo "  ${selfname} 2k 128k /home/rootfs 50M"
	echo ""
	exit 0
}
###############################################################################

function run ()
{
	local cmd=$1
	echo "${cmd}"
	msg=$(eval "${cmd}"); result=$?
	echo ${msg}
	[ ${result} == 0 ] || exit ${result}
}
###############################################################################

function hstrtol ()
{
	local hstr=$1
	local zoom=1
	local result=$(echo "${hstr}" | awk '{printf "%d",$0}')

	if [ "$(echo ${hstr} | grep '[Gg]')" == "${hstr}" ]; then
		zoom=1073741824
	elif [ "$(echo ${hstr} | grep '[Mm]')" == "${hstr}" ]; then
		zoom=1048576
	elif [ "$(echo ${hstr} | grep '[Kk]')" == "${hstr}" ]; then
		zoom=1024
	fi

	echo $((${result} * ${zoom}))
}
###############################################################################

selfname=$(basename $0)

if [ $# == 0 ]; then
	usage;
fi

if [ $# != 5 ] && [ $# != 7 ]; then
	echo "need many parameters."
	exit 1
fi

hpagesize=${1}
pagesize=$(hstrtol ${hpagesize})
#hblocksize=${2}
#blocksize=$(hstrtol ${hblocksize})
#blocksize=`echo "ibase=16;obase=10;${2}" | bc`
blocksize=${2}
rootdir=$(echo $(echo "${3} " | sed 's/\/ //'))
hpartsize=${4}
partsize=$(hstrtol ${hpartsize})

if [ ! -d ${rootdir} ]; then
	echo "Directory ${rootdir} not exist."
	exit 1;
fi

LEB=$((${blocksize} - ${pagesize} * 2))
MAX_LEB_CNT=$((${partsize} / ${blocksize}))
###############################################################################

ubifsimg=${rootdir}.ubifs
#ubiimg=${rootdir}_${hpagesize}_${hblocksize}_${hpartsize}.ubiimg
ubiimg=${5}
ubicfg=${rootdir}.ubicfg

MKUBIFS=mkfs.ubifs
MKUBI=ubinize

if [ $# == 7 ]; then
	RULEFILE=${6}
	RULEDBG=${7}
	run "${MKUBIFS} -F -d ${rootdir} -m ${pagesize} -o ${ubifsimg} -e ${LEB} -c ${MAX_LEB_CNT} -T ${RULEFILE} -G ${RULEDBG}"
else
	run "${MKUBIFS} -F -d ${rootdir} -m ${pagesize} -o ${ubifsimg} -e ${LEB} -c ${MAX_LEB_CNT}"
fi

{
	echo "[ubifs-volumn]"
	echo "mode=ubi"
	echo "image=${ubifsimg}"
	echo "vol_id=0"
	echo "vol_flags=autoresize"
	echo "vol_type=dynamic"
	echo "vol_name=ubifs"
	echo ""

} > ${ubicfg}

run "${MKUBI} -o ${ubiimg} -m ${pagesize} -p ${blocksize} ${ubicfg}"

rm -f ${ubifsimg} ${ubicfg}
