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

//#include "hi_adp_boardcfg.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
 #define HI_INFO_I2C(format, arg...)
#else
 #define HI_INFO_I2C(format, arg...) printf( format, ## arg)
#endif

HI_S32 main(HI_S32 argc, HI_CHAR **argv)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_U8 *pData = HI_NULL;

    HI_U32 u32DeviceAddress = 0;
    HI_U32 u32I2cNum  = 0;
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegAddrCount = 0;
    HI_U32 u32ReadNumber = 0;

    HI_U32 u32Loop;

    if (argc != 6)
    {
        HI_INFO_I2C("Usage: i2c_read  i2c_channel  device_addr  register_addr  ");
        HI_INFO_I2C("register_addr_len  read_bytes_number\n");

        return -1;
    }

    u32I2cNum = strtol(argv[1], NULL, 0);
    u32DeviceAddress = strtol(argv[2], NULL, 0);
    u32RegAddr = strtol(argv[3], NULL, 0);
    u32RegAddrCount = strtol(argv[4], NULL, 0);
    if (u32RegAddrCount > 4)
    {
        HI_INFO_I2C("register address length is error!\n");
        return -1;
    }

    u32ReadNumber = strtol(argv[5], NULL, 0);

    s32Ret = HI_UNF_I2C_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_I2C("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
        return s32Ret;
    }

    pData = (HI_U8 *)malloc(u32ReadNumber);
    if (HI_NULL == pData)
    {
        HI_INFO_I2C("\n pReadData malloc() error!\n");
        HI_UNF_I2C_DeInit();

        return HI_FAILURE;
    }

    /* Read data from Device */
    s32Ret = HI_UNF_I2C_Read(u32I2cNum, u32DeviceAddress,
                             u32RegAddr, u32RegAddrCount, pData, u32ReadNumber);
    if (s32Ret != HI_SUCCESS)
    {
        HI_INFO_I2C("call HI_I2C_Read failed.\n");
        free(pData);
        HI_UNF_I2C_DeInit();

        return s32Ret;
    }

    HI_INFO_I2C("\ndata read:\n");

    for (u32Loop = 0; u32Loop < u32ReadNumber; u32Loop++)
    {
        HI_INFO_I2C("0x%02x ", pData[u32Loop]);
    }

    HI_INFO_I2C("\n\n");

    free(pData);

    HI_UNF_I2C_DeInit();

    return HI_SUCCESS;
}
