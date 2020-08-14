#!/bin/bash

if [ x$1 = xuninstall ]; then
    make -f sme_tools_install.mak clean
else
    echo 'export PATH=$HOME/sme_tool/usr/bin:$PATH' >> $HOME/.profile
    echo 'export LD_LIBRARY_PATH=$HOME/sme_tool/usr/lib:$LD_LIBRARY_PATH' >> $HOME/.profile
    echo 'export SME_DEPS_BIN_PATH=$HOME/sme_tool/usr/bin' >> $HOME/.profile
    echo 'export export PERLLIB=~/sme_tool/usr/lib/perl/5.14/:~/sme_tool/usr/lib/perl/5.18/:$PERLLIB' >> $HOME/.profile
    source $HOME/.profile

    make -f sme_tools_install.mak all SME_TOOL_PKG_CONFIG=$HOME/sme_tool/usr/lib/pkgconfig
    if [ $? = 0 ];then
        echo "configure OK!"
    else
        echo "configure failed!"
    fi
fi
