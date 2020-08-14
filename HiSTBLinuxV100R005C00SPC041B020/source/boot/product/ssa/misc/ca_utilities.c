/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : ca_utilities.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"



/*
** CRC32 Arithmetic
*/
#define     POLYNOMIAL 0x04c11db7L
static short _First = 1;                       /*Init _CrcTable[256] Flag, Only init once time*/
static unsigned long  _CrcTable[256];                  /*Calculate CRC32*/


static void GenCrcTable(void)
{
     int    i, j;
    unsigned long crc_accum;
    for( i = 0;  i < 256;  i++ )
    {
        crc_accum =  ( i << 24 );
        for( j = 0;  j < 8;  j++ )
        {
            if( crc_accum & 0x80000000L )
                crc_accum = ( crc_accum << 1 ) ^ POLYNOMIAL;
            else
                crc_accum = ( crc_accum << 1 );
        }
        _CrcTable[i] = crc_accum;
    }
    return;
}


unsigned long DVB_CRC32(unsigned char *buf, int length)
{
    unsigned long crc = 0xffffffff;

    if(_First)
    {
        GenCrcTable();
        _First = 0;
    }
    while( length-- )
    {
        crc = (crc << 8) ^ _CrcTable[((crc >> 24) ^ *buf++) & 0xff];
    }

    return crc;
}

