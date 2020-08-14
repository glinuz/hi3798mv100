#include <linux/delay.h>

#include "hi_type.h"

#include "sony_common.h"
#include "sony_i2c.h"
#include "hi_drv_i2c.h"
#include "drv_tuner_ioctl.h"

sony_result_t hi_i2c_read_sony(struct sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData, uint32_t size, uint8_t mode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TunerPort = *(HI_U32*)(pI2c->user);
    HI_U32 I2cNum = g_stTunerOps[u32TunerPort].enI2cChannel;
    s32Ret = HI_DRV_I2C_Read_sony(I2cNum, deviceAddress, 0, 0, pData, size, mode);

    if (HI_SUCCESS == s32Ret)
    {
        return SONY_RESULT_OK;
    }
    else
    {
        return SONY_RESULT_ERROR_I2C;
    }
}

sony_result_t hi_i2c_write_sony(struct sony_i2c_t* pI2c, uint8_t deviceAddress,  uint8_t *pData, uint32_t size, uint8_t mode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32TunerPort = *(HI_U32*)(pI2c->user);
    HI_U32 I2cNum = g_stTunerOps[u32TunerPort].enI2cChannel;
    s32Ret = HI_DRV_I2C_Write_sony(I2cNum, deviceAddress, 0, 0, pData, size, mode);

    if (HI_SUCCESS == s32Ret)
    {
        return SONY_RESULT_OK;
    }
    else
    {
        return SONY_RESULT_ERROR_I2C;
    }
}

