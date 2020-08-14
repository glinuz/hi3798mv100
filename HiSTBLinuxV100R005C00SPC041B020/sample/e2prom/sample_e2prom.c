/*****************************************
* this sample base on hisi demo board test e2prom.
*user board notice:
* e2prom mode/ i2c device address/i2c use which channel
*
*****************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hi_unf_i2c.h"
#include "hi_adp_boardcfg.h"

typedef struct hiE2PROM_ATTR_S
{
    HI_U32 u32I2cNum;
    HI_U32 u32E2promAddr;
    HI_U32 u32TotalSize;
    HI_U32 u32PageSize;
    HI_U32 u32AddrLen;
}HI_E2PROM_ATTR_S;

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define HI_INFO_E2PROM(format, arg ...)
#else
#define HI_INFO_E2PROM(format, arg ...) printf( format, ## arg)
#endif

HI_S32 main(HI_S32 argc, HI_CHAR **argv)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Loop;
    HI_U32 u32Address = 0;
    HI_U32 u32Length  = 0;
    HI_U32 tmp;
    HI_E2PROM_ATTR_S stE2promAttr;

    HI_U32 u32TmpLen;
    HI_U8 *pReadData  = HI_NULL;
    HI_U8 *pWriteData = HI_NULL;
    HI_U8 *pData  = HI_NULL;
    HI_U8 *pData1 = HI_NULL;
    HI_U32 u32InputLength  = 0;
    HI_U32 u32InputAddress = 0;

#if 0 //defined (BOARD_TYPE_hi3716mdmo3avera) || (BOARD_TYPE_hi3716mdmo3bvera)
    HI_INFO_E2PROM("Not support e2prom test in Hi3716DEMO3A board\n");
    return HI_SUCCESS;
#endif

    /* Select device type: */
    HI_INFO_E2PROM("please select e2prom device: \n");
    HI_INFO_E2PROM("0 -- AT24C01 or M24LC01 \n");
    HI_INFO_E2PROM("1 -- AT24C02 or M24LC02 \n");
    HI_INFO_E2PROM("2 -- AT24C04 or M24LC04 \n");
    HI_INFO_E2PROM("3 -- AT24C08 or M24LC08 \n");
    HI_INFO_E2PROM("4 -- AT24C16 or M24LC16 \n");
    HI_INFO_E2PROM("5 -- AT24C32 or M24LC32 \n");
    HI_INFO_E2PROM("6 -- AT24C64 or M24LC64 \n");
    HI_INFO_E2PROM("7 -- AT24C128 or M24LC128 \n");
    HI_INFO_E2PROM("8 -- AT24C256 or M24LC256 \n");
    scanf("%u", &tmp);

    switch (tmp)
    {
    case 0:
        stE2promAttr.u32TotalSize = 0x80;
        stE2promAttr.u32PageSize  = 8;
        stE2promAttr.u32AddrLen = 1;
        break;

    case 1:
        stE2promAttr.u32TotalSize = 0x100;
        stE2promAttr.u32PageSize  = 8;
        stE2promAttr.u32AddrLen = 1;
        break;

    case 2:
        stE2promAttr.u32TotalSize = 0x200;
        stE2promAttr.u32PageSize  = 16;
        stE2promAttr.u32AddrLen = 1;
        break;

    case 3:
        stE2promAttr.u32TotalSize = 0x400;
        stE2promAttr.u32PageSize  = 16;
        stE2promAttr.u32AddrLen = 1;
        break;

    case 4:
        stE2promAttr.u32TotalSize = 0x800;
        stE2promAttr.u32PageSize  = 16;
        stE2promAttr.u32AddrLen = 1;
        break;

    case 5:
        stE2promAttr.u32TotalSize = 0x1000;
        stE2promAttr.u32PageSize  = 32;
        stE2promAttr.u32AddrLen = 2;
        break;

    case 6:
        stE2promAttr.u32TotalSize = 0x2000;
        stE2promAttr.u32PageSize  = 32;
        stE2promAttr.u32AddrLen = 2;
        break;

    case 7:
        stE2promAttr.u32TotalSize = 0x4000;
        stE2promAttr.u32PageSize  = 64;
        stE2promAttr.u32AddrLen = 2;
        break;

    case 8:
        stE2promAttr.u32TotalSize = 0x8000;
        stE2promAttr.u32PageSize  = 64;
        stE2promAttr.u32AddrLen = 2;
        break;

    default:
        HI_INFO_E2PROM("Error:Do not support input device \n");
        return HI_FAILURE;
    }

    /* Select device address */
    HI_INFO_E2PROM("please input e2prom device address (from 0xA0 to 0xAF): \n");
    HI_INFO_E2PROM("default value : 0xA0 \n");
    scanf("%x", &tmp);

    if ((tmp > 0xAF) || (tmp < 0xA0))
    {
        HI_INFO_E2PROM("Error:Invaild device address \n");
        return HI_FAILURE;
    }
    else
    {
        stE2promAttr.u32E2promAddr = tmp;
    }

    /* Input used i2c number */
    HI_INFO_E2PROM("please input i2c channel number(0-2): \n");
    HI_INFO_E2PROM("default value : 2 \n");
    scanf("%u", &tmp);
    stE2promAttr.u32I2cNum = tmp;

    /* Input the start address */
    HI_INFO_E2PROM("please input the start address: \n");
    scanf("%u", &tmp);
    u32InputAddress = tmp;

    /* Input the start address */
    HI_INFO_E2PROM("please input read/write length: \n");
    scanf("%u", &tmp);
    u32InputLength = tmp;

    if ((u32InputAddress + u32InputLength) > stE2promAttr.u32TotalSize)
    {
        HI_INFO_E2PROM("access addr overflow.\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_I2C_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_E2PROM("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    pWriteData = (HI_U8 *)malloc(u32InputLength);
    if (HI_NULL == pWriteData)
    {
        HI_INFO_E2PROM("\n pWriteData malloc() error!\n");
        return HI_FAILURE;
    }

    pData = pWriteData;

    HI_INFO_E2PROM("data write:\n");
    for (u32Loop = 0; u32Loop < u32InputLength; u32Loop++)
    {
        *pData = u32Loop & 0xff;

        HI_INFO_E2PROM("0x%02x ", *pData);

        ++pData;
    }

    pData = pWriteData;
    u32Length  = u32InputLength;
    u32Address = u32InputAddress;

    /* Write data to E2PROM */
    while (u32Length > 0)
    {
        if ((u32Address > 0xff) && (stE2promAttr.u32AddrLen == 1))
        {
            stE2promAttr.u32E2promAddr |= (((u32Address >> 8) << 1) & 0xe);
        }

        if ((u32Address % stE2promAttr.u32PageSize) != 0) // maybe excute one time
        {
            u32TmpLen = stE2promAttr.u32PageSize - (u32Address % stE2promAttr.u32PageSize);

            u32TmpLen = (u32TmpLen > u32Length) ? u32Length : u32TmpLen;
        }
        else
        {
            if (u32Length >= stE2promAttr.u32PageSize) //excute many times
            {
                u32TmpLen = stE2promAttr.u32PageSize;
            }
            else // maybe excute one time
            {
                u32TmpLen = u32Length;
            }
        }

        s32Ret = HI_UNF_I2C_Write(stE2promAttr.u32I2cNum, stE2promAttr.u32E2promAddr,
                                  u32Address, stE2promAttr.u32AddrLen, pData, u32TmpLen);
        if (s32Ret != HI_SUCCESS)
        {
            HI_INFO_E2PROM("call HI_I2C_Write failed.\n");
            if (pWriteData)
            {
                free(pWriteData);
            }

            return s32Ret;
        }

#if 0 //it is necessary to do some delay.
        Ret = HI_DRV_I2C_WriteConfig(stE2promAttr.u32I2cNum, stE2promAttr.u32E2promAddr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_I2C_WriteConfig failed.\n");
            return Ret;
        }

#else
        usleep(10000);
#endif
        u32Address += u32TmpLen;
        pData += u32TmpLen;
        u32Length -= u32TmpLen;
    }

    usleep(10000);

    pReadData = (HI_U8 *)malloc(u32InputLength);
    if (HI_NULL == pReadData)
    {
        HI_INFO_E2PROM("\n pReadData malloc() error!\n");
        if (pWriteData)
        {
            free(pWriteData);
        }

        return HI_FAILURE;
    }

    pData = pReadData;
    u32Length  = u32InputLength;
    u32Address = u32InputAddress;

    /* Read data from E2PROM */
    while (u32Length > 0)
    {
        if ((u32Address > 0xff) && (stE2promAttr.u32AddrLen == 1))
        {
            stE2promAttr.u32E2promAddr |= (((u32Address >> 8) << 1) & 0xe);
        }

        if ((u32Address % stE2promAttr.u32PageSize) != 0) // maybe excute one time
        {
            u32TmpLen = stE2promAttr.u32PageSize - (u32Address % stE2promAttr.u32PageSize);

            u32TmpLen = (u32TmpLen > u32Length) ? u32Length : u32TmpLen;
        }
        else
        {
            if (u32Length >= stE2promAttr.u32PageSize) //excute many times
            {
                u32TmpLen = stE2promAttr.u32PageSize;
            }
            else // maybe excute one time
            {
                u32TmpLen = u32Length;
            }
        }

        s32Ret = HI_UNF_I2C_Read(stE2promAttr.u32I2cNum, stE2promAttr.u32E2promAddr,
                                 u32Address, stE2promAttr.u32AddrLen, pData, u32TmpLen);
        if (s32Ret != HI_SUCCESS)
        {
            HI_INFO_E2PROM("call HI_I2C_Read failed.\n");
            if (pWriteData)
            {
                free(pWriteData);
            }

            if (pReadData)
            {
                free(pReadData);
            }

            return s32Ret;
        }

        u32Address += u32TmpLen;
        pData += u32TmpLen;
        u32Length -= u32TmpLen;
    }

    HI_INFO_E2PROM("\ndata read:\n");
    pData = pReadData;
    for (u32Loop = 0; u32Loop < u32InputLength; u32Loop++)
    {
        HI_INFO_E2PROM("0x%02x ", *pData);
        ++pData;
    }

    HI_INFO_E2PROM("\n\n");

    pData  = pReadData;
    pData1 = pWriteData;

    /*check whether the read data is equal to the write data */
    for (u32Loop = 0; u32Loop < u32InputLength; u32Loop++)
    {
        if (*pData++ != *pData1++)
        {
            HI_INFO_E2PROM("Error:Buffer_r[%d] = %#x, Buffer_w[%d] = %#x\n", u32Loop, *pData, u32Loop,
                           *pData1);
            goto ERR1;
        }
    }

    if (pWriteData)
    {
        free(pWriteData);
    }

    if (pReadData)
    {
        free(pReadData);
    }

    HI_UNF_I2C_DeInit();

    HI_INFO_E2PROM("\nrun e2prom demo success\n");

    return HI_SUCCESS;

ERR1:
    if (pWriteData)
    {
        free(pWriteData);
    }

    if (pReadData)
    {
        free(pReadData);
    }

    HI_UNF_I2C_DeInit();

    HI_INFO_E2PROM("run e2prom demo failed\n");

    return s32Ret;
}
