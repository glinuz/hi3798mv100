#!/bin/sh
################################################################################
#    Copyright (C) 2014 Hisilicon STB Development Dept
#    All rights reserved.
#####
#    Create by Cai Zhiyong
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#    http://www.apache.org/licenses/LICENSE-2.0
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
################################################################################

################################################################################
# $* "string"
function pr_debug()
{
	local NORMAL="\033[00m"
	local PURPLE="\033[35m"

	# echo -e ${PURPLE}"$*"${NORMAL}
}
################################################################################

OPTIONS=${1}
TGTCONFIG=${2}
DEFCONFIG=${3}
CONFIGINC=${4}
CMDLINE=$(printenv ${5})

if [ ! -f ${TGTCONFIG} ]; then
	exit 0;
fi

pr_debug "  CMDLINE=\"${CMDLINE}\""

MKFLAGS=$(echo ${CMDLINE} | grep " \-\- ")
if [ ss"${MKFLAGS}" = ss"" ]; then
	KEYWORD=$(echo "${CMDLINE}" | grep "=")
	if [ ss"${KEYWORD}" != ss"" ]; then
		KEYWORD=$(echo "${CMDLINE%%=*}" | grep "[ -]")
		if [ ss"${KEYWORD}" = ss"" ]; then
			MKFLAGS=${CMDLINE}
		fi
	fi
else
	MKFLAGS=$(echo ${CMDLINE} | sed -e 's/.*[ ]--[ ]//')
fi

pr_debug "  MKFLAGS=\"${MKFLAGS}\""

ALL_CONFIGS=""
PREFIX=config_

################################################################################
# $1 - AA=aa
function export_one_item()
{
	local LINE=${PREFIX}${1}
	local KEYWORD=${LINE%%=*}
	local VALUE=${LINE#*=}

	KEYWORD=$(echo ${KEYWORD})
	VALUE=$(echo ${VALUE})

	export ${KEYWORD}="${VALUE}"

	ALL_CONFIGS=${KEYWORD}:${ALL_CONFIGS}
}
################################################################################
# $*  AA=aa BB=bb CC=cc
function export_multi_items()
{
	while [ "$#" != '0' ]; do
		export_one_item ${1}
		shift
	done
}
################################################################################
# $1 - file to be export
function export_file_configs()
{
	local CONFIG_FILE=${1}

	while read -r LINE ; do
		LINE=$(echo ${LINE} | sed -e 's/\r//g')
		SKIP=${LINE:0:1}
		if [ ss"$SKIP" = ss"#" -o ss"$SKIP" = ss"" ]; then
			continue;
		fi
		export_one_item "${LINE}"
	done < ${CONFIG_FILE}
}
################################################################################
# $1 - a line input
function write_one_config()
{
	local KEYWORD=${1%%=*}

	if [ ss"${KEYWORD}" = ss"ALL_CONFIGS" ]; then
		return 0
	fi

	local TMP=$(echo ${ALL_CONFIGS} | grep "${KEYWORD}:")
	if [ ss"${TMP}" = ss"" ]; then
		return 0
	fi

	KEYWORD=$(echo ${KEYWORD} | sed -e "s/${PREFIX}//g")
	VALUE=$(printenv ${PREFIX}${KEYWORD})

	if [ s"${VALUE}" != s"" ]; then
		echo "${KEYWORD} = ${VALUE}" >> ${DEFCONFIG}
	fi
}
################################################################################
# $1 - base config file
function export_to_file()
{
	local CONFIG_FILE=$1

	ALL_CONFIGS=""
	export_file_configs ${CONFIG_FILE}
	export_multi_items ${MKFLAGS}

	local VARIBLES=$(printenv | grep "${PREFIX}")

	rm -f ${DEFCONFIG}
	echo "${VARIBLES}" | while read -r LINE ; do
		LINE=$(echo ${LINE} | sed -e 's/\r//g')
		write_one_config "${LINE}"
	done

	VARIBLES=$(sort ${DEFCONFIG})

	echo "# This file build from ${TGTCONFIG}" > ${DEFCONFIG}
	echo "${VARIBLES}" >> ${DEFCONFIG}

	save_include_file
}
################################################################################
# $* - all config be checked
function check_every_configs()
{
	while [ "$#" != '0' ]; do
		local LINE=${1}
		local KEYWORD=${LINE%%=*}

		# check add new config
		local TMP=$(echo ${ALL_CONFIGS} | grep "${PREFIX}${KEYWORD}:")
		if [ ss"${TMP}" = ss"" ]; then
			pr_debug check: ${KEYWORD}
			return 1
		fi

		# check a exist config modify
		export ${LINE}
		local NEW=$(printenv ${KEYWORD})
		local OLD=$(printenv ${PREFIX}${KEYWORD})
		if [ ss"${NEW}" != ss"${OLD}" ]; then
			pr_debug check: ${NEW} != ${OLD}
			return 1;
		fi

		shift
	done

	return 0
}
################################################################################

function check_mkflags_configs()
{
	ALL_CONFIGS=""

	export_file_configs ${DEFCONFIG}

	check_every_configs ${MKFLAGS}

	if [ "$?" != "0" ]; then
		export_to_file ${DEFCONFIG}
		pr_debug "  Check config. it is change, export a new config."
	else
		pr_debug "  Check, config has no change."
	fi
}
################################################################################
# $1 - a line input
function write_one_define()
{
	local LINE=${1}
	local SKIP=${LINE:0:1}

	if [ ss"${SKIP}" = ss"#" -o ss"${SKIP}" = ss"" ]; then
		return 0
	fi

	local KEYWORD=${LINE%%=*}
	if [ ${#KEYWORD} -gt 40 ]; then
		echo "Error: config name "\"${KEYWORD}\"" too longer."
		exit 1
	fi

	KEYWORD=${KEYWORD}"                                        "
	KEYWORD=${KEYWORD:0:40}
	VALUE=${LINE#*=}

	if [ s"${VALUE}" != s"" ]; then
		echo "#define ${KEYWORD} ${VALUE}" >> ${CONFIGINC}
	else
		echo "#undef ${KEYWORD}" >> ${CONFIGINC}
	fi

	return 0
}
################################################################################
#
function save_include_file()
{
	rm -f ${CONFIGINC}

	echo '/* This file is auto export, DO NOT modify. */
#ifndef _CONFIGH_
#define _CONFIGH_

#include "platform.h"

' >> ${CONFIGINC}

	while read -r LINE ; do
		LINE=$(echo ${LINE} | sed -e 's/\r//g' )
		write_one_define "${LINE}"
	done < ${DEFCONFIG}

	write_one_define "HOSTNAME = \"${HOSTNAME}\""
	write_one_define "USER = \"${USER}\""

	echo '
#endif
' >> ${CONFIGINC}
}
################################################################################

if [ ss"${OPTIONS}" = ss"export" ]; then
	pr_debug "  export config..."
	export_to_file ${TGTCONFIG}
	exit 0
fi
################################################################################

if [ ss"${OPTIONS}" = ss"check" ]; then
	if [ ss"${MKFLAGS}" = ss"" ]; then
		exit 0
	fi
	if [ ! -f ${DEFCONFIG} ]; then
		exit 0
	fi
	check_mkflags_configs
	exit 0
fi
