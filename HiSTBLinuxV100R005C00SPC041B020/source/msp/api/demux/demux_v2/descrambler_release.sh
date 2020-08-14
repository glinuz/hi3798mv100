#!/bin/bash
# ./descrambler_release.sh　 parameter	// 0 删除解扰功能, 其他值是保留解扰功能

with_dscr=$1

msp_dir=$PWD/../../

if [ 0 -eq $with_dscr ] ; then
    rm -rf $msp_dir/include/hi_unf_descrambler.h

    rm -rf $msp_dir/api/include/hi_mpi_descrambler.h
    rm -rf $msp_dir/api/demux/mpi_descrambler.c
    rm -rf $msp_dir/api/demux/unf_descrambler.c

    rm -rf $msp_dir/drv/include/drv_descrambler_ioctl.h
    rm -rf $msp_dir/drv/include/hi_drv_descrambler.h
    rm -rf $msp_dir/drv/demux/descrambler

    mv $msp_dir/api/demux/Makefile_nodscr   $msp_dir/api/demux/Makefile
    mv $msp_dir/drv/demux/Makefile_nodscr   $msp_dir/drv/demux/Makefile
else
    rm -rf $msp_dir/api/demux/Makefile_nodscr
    rm -rf $msp_dir/drv/demux/Makefile_nodscr
fi

rm -rf $0