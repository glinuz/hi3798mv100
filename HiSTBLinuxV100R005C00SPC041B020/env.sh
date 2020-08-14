#!/bin/bash
function get_make_command()
{
  echo command make
}

function make()
{
    local start_time=$(date +"%s")
    $(get_make_command) "$@"
    local ret=$?
    local end_time=$(date +"%s")
    local tdiff=$(($end_time-$start_time))
    local hours=$(($tdiff / 3600 ))
    local mins=$((($tdiff % 3600) / 60))
    local secs=$(($tdiff % 60))
    echo
    if [ $ret -eq 0 ] ; then
        echo -n -e "\033[1;32m #### make \""$@"\" completed successfully!"
    else
        echo -n -e "\033[1;31m #### make target \""$@"\" failed !!!"
    fi
    if [ $hours -gt 0 ] ; then
        printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
    elif [ $mins -gt 0 ] ; then
        printf "(%02g:%02g (mm:ss))" $mins $secs
    elif [ $secs -gt 0 ] ; then
        printf "(%s seconds)" $secs
    fi
    echo -e " ####\033[1;0m"
    echo
    return $ret
}
function setpaths()
{
    arm_histbv310_path=${PWD}/tools/linux/toolchains/arm-histbv310-linux/bin
    aarch64_histbv100_path=${PWD}/tools/linux/toolchains/aarch64-histbv100-linux/bin
    utils_path=${PWD}/tools/linux/utils/bin

    PATH=${arm_histbv310_path}:${PATH//${arm_histbv310_path}:/}
    PATH=${aarch64_histbv100_path}:${PATH//${aarch64_histbv100_path}:/}
    PATH=${utils_path}:${PATH//${utils_path}:/}
}

function check_bash()
{
    is_bash=`${SHELL} --version | grep "GNU bash"`
    is_bash=${is_bash:0:8}
    if [ "${is_bash}" != "GNU bash" ] ; then
        echo "The shell may NOT be a bash, we need a bash to build SDK, please use bash!";
        echo "To change your shell, you should be root first, and then set by steps followed:";
        echo "    rm -f /bin/sh";
        echo "    ln -s /bin/bash  /bin/sh";
        echo ""
    fi
}

check_bash

setpaths

export HISI_LINUX_ENV=ok

