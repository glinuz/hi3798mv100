#!/bin/bash

##################################
### code format tool    ##########
### version: 1.0        ##########
### create:wangdedong   ##########
### date:  2016.06.23   ##########
##################################

#filetype='\.c$|\.h$|\.S$|\.cpp$|\.java$|\.chh$|\.cc$|\.ph$|\.inc$'
filetype='\.c$|\.h$|\.cpp$|\.chh$|\.cc$|\.ph$|\.inc$'

scriptdir=${0%/*}
#echo "$scriptdir"


if (($# < 1));then
    echo "code format tool v1.0"
    echo "=================================================================="
    echo "author:wangdedong"
    echo "Usage: $0 [-d | -f] dir|file"
    echo "       -d format all the files of the special directory."
    echo "       -f format the special file, it is default, can be omitted."
    echo "=================================================================="
    exit 1
fi

if [ "$1"x = "-fx" ]; then
    echo "process the file $2"
    color=1
    obj=$2
elif [ "$1"x = "-dx" ]; then
    echo "process the dir $2"
    color=2
    obj=$2
else
    echo "process the file $1 default"
    color=1
    obj=$1
fi

if [ "$color" -eq "2" ];then




	echo -n "Do you sure to format the codes in this path? Warning: dont modify freetype/libpng/zlib, it'll be checkout. (y/n)"
	read content
	#echo $content
	if [ $content == "y" ];then
		find $obj -type f | grep -E $filetype |xargs $scriptdir/astyle --style=linux -np --indent=force-tab=8 --pad-header --unpad-paren --lineend=linux --align-pointer=name
		# --max-code-length=80
		#find . -type f | grep -E $filetype |xargs indent -ut -ts4
		#astyle --style=linux -rnp --exclude="git main2.c" "*"
		#if [ -d "TEE/tui/penpaint/freetype" ];then
		#	echo "checkout freetype"
		#	git checkout TEE/tui/penpaint/freetype
		#fi
		#if [ -d "TEE/tui/penpaint/libpng" ];then
		#	echo "checkout libpng"
		#	git checkout TEE/tui/penpaint/libpng
		#fi


	elif [ $content == "n" ];then
		echo "no format yet."
		exit
	else
		echo ":( error answer, do nothing."
		exit
	fi
elif [ "$color" -eq "1" ];then
	$scriptdir/astyle --style=linux -np --indent=force-tab=8 --pad-header --unpad-paren --lineend=linux --align-pointer=name $obj
fi

#exit
echo "process end"

#find . -type f | grep -E '\.c$|\.h$|\.S$|\.cpp$|\.java$|\.lds$|\.ld*|\.chh$|\.cc$|\.ph$|\.inc$'|xargs astyle -np --max-code-length=80
#--exclude=zlib --exclude=freetype --exclude=libpng
