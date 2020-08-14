/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_fe_math.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/04/25
  Description   :
  History       :
  1.Date        : 2016/04/25
  Author        : w00313507
  Modification  : Created file
******************************************************************************/
#include "hi_type.h"
#include "hi_debug.h"

static HI_U32 u32SNRLogTable[]=
{
    0, 100,
    2, 104,
    4, 109,
    6, 114,
    8, 120,
    10, 125,
    12, 131,
    14, 138,
    16, 144,
    18, 151,
    20, 158,
    22, 166,
    24, 173,
    26, 182,
    28, 190,
    30, 199,
    32, 208
};

static HI_U32 LeftTable(int u32InValue)
{
    HI_U32 u32OutValue = 0;
    HI_U32 u32Iter = 0;
    HI_U32 u32TableSize = sizeof(u32SNRLogTable)/sizeof(int);

    for(u32Iter=0; u32Iter<u32TableSize; u32Iter+=2)
    {
        if((u32Iter +1) < u32TableSize)
        {
            if(u32InValue <= u32SNRLogTable[u32Iter+1])
            {
                u32OutValue = u32SNRLogTable[u32Iter];
                return u32OutValue;
            }
        }
    }

    u32OutValue = 32;
    return u32OutValue;
}

/*100 * lg(u32InValue)*/
HI_U32 tuner_get_100lg_value(HI_U32 u32InValue)
{
    HI_U8 u8Index = 0;
    HI_U32 u32Tmp = 1;
    HI_U32 u32InValueLeft = 1;
    HI_U32 u32LogValue;
    HI_U32 u32Dvalue = 0;

    if(u32InValue == 0)
    {
        return 0;
    }

    do
    {
        u32Tmp = u32Tmp << 1;
        if (u32InValue < u32Tmp)
        {
            break;
        }
    }
    while(++u8Index < 32);

    u32LogValue = 301*u8Index/10;

    u32Dvalue = (1<<u8Index);
    if(u32Dvalue != 0)
    {
        u32InValueLeft = u32InValue*100/u32Dvalue;
    }

    u32LogValue = u32LogValue + LeftTable(u32InValueLeft);

    //HI_PRINT(" u32InValue = 0x%x  log ret = %d\n", u32InValue,  u32LogValue);

    return u32LogValue;
}

/* a^b*/
HI_U32 tuner_get_pow(HI_U32 a, HI_U32 b)
{
    HI_U32 i = 0;
    HI_U32 u32Result = 1;

    for (i = 1; i <= b; i++)
    {
        u32Result *= a;
    }

    return u32Result;
}

