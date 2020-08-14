#!/bin/sh
###############################################################################

SELF=$(basename $0)
KEY=$(echo $1 | sed -e 's/=.*//g')
VALUE=$(echo $1 | sed -e "s/${KEY}=//g")
COMMENT=$2
CONFIG_FILE=$3

################################################################################

if [ $# != 3 ]; then
	echo "${SELF} {KEY=VALUE} {COMMENT} {CONFIG FILE}"
	exit 1
fi


if [ ss"${VALUE}" = ss"" ]; then
	{
		echo ;
		echo ;
		echo "/*"
		echo " * " "${COMMENT}"
		echo " * " "#define ${KEY}"
		echo " */"
	} >> ${CONFIG_FILE}
else
	{
		echo ;
		echo ;
		echo "/*"
		echo " * " "${COMMENT}"
		echo " * "
		echo " */"
		echo "#define ${KEY}	${VALUE}"
	} >> ${CONFIG_FILE}
fi
