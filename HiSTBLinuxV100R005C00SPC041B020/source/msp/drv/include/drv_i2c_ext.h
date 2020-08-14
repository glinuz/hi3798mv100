/*     extdrv/include/gpio_i2c.h for Linux .
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     03-Apr-2006 create this file
 *
 */

#ifndef _DRV_I2C_EXT_H
#define _DRV_I2C_EXT_H

#include "hi_type.h"
#ifdef __KERNEL__
#include "hi_drv_dev.h"
#endif
HI_S32 I2C_DRV_ModInit(HI_VOID);
HI_VOID I2C_DRV_ModExit(HI_VOID);

//typedef HI_VOID (*FN_I2C_OPEN)(HI_VOID);
//typedef HI_VOID (*FN_I2C_CLOSE)(HI_VOID);
typedef HI_S32 (*FN_I2C_WRITECONFIG)(HI_U32, HI_U8);
typedef HI_S32 (*FN_I2C_WRITE)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);
typedef HI_S32 (*FN_I2C_READ)(HI_U32, HI_U8, HI_U32, HI_U32, HI_U8 *, HI_U32);
typedef HI_S32 (*FN_I2C_SetRate)(HI_U32 , HI_U32 );
#ifdef __KERNEL__
typedef HI_S32 (*FN_I2C_Suspend)(PM_BASEDEV_S* , pm_message_t );
typedef HI_S32 (*FN_I2C_Resume)(PM_BASEDEV_S* );
#endif

typedef struct
{
    FN_I2C_WRITECONFIG pfnI2cWriteConfig;
    FN_I2C_WRITE       pfnI2cWrite;
    FN_I2C_READ        pfnI2cRead;
    FN_I2C_WRITE       pfnI2cWriteNostop;
    FN_I2C_READ        pfnI2cReadDirectly;
    FN_I2C_SetRate     pfnI2cSetRate;
#ifdef __KERNEL__
    FN_I2C_Suspend	   pfnI2cSuspend;
    FN_I2C_Resume	   pfnI2cResume;
#endif
} I2C_EXT_FUNC_S;

#endif
