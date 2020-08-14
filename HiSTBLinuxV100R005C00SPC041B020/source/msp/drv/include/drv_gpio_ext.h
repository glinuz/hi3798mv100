/*     extdrv/include/gpio_i2c.h for Linux .
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     03-Apr-2006 create this file
 *
 */

#ifndef _DRV_GPIO_EXT_H
#define _DRV_GPIO_EXT_H

#include "hi_type.h"
#ifdef __KERNEL__
#include "hi_drv_dev.h"
#endif
#include "hi_drv_gpio.h"
#include "hi_unf_gpio.h"

HI_S32 GPIO_DRV_ModInit(HI_VOID);
HI_VOID GPIO_DRV_ModExit(HI_VOID);

typedef HI_S32 (*FN_GPIO_Get_Bit)(HI_U32, HI_U32*);
typedef HI_S32 (*FN_GPIO_Set_Bit)(HI_U32, HI_U32);
typedef HI_S32 (*FN_GPIO_Get_Num)(GPIO_GET_GPIONUM_S*);

typedef HI_S32 (*FN_GPIO_Register_Server_Func)(HI_U32, HI_VOID (*func)(HI_U32));
typedef HI_S32 (*FN_GPIO_UnRegister_Server_Func)(HI_U32);
typedef HI_S32 (*FN_GPIO_Set_Int_Type)(HI_U32, HI_UNF_GPIO_INTTYPE_E);
typedef HI_S32 (*FN_GPIO_Set_Int_Enable)(HI_U32, HI_BOOL);
typedef HI_S32 (*FN_GPIO_Clear_GroupInt)(HI_U32);
typedef HI_S32 (*FN_GPIO_Clear_BitInt)(HI_U32);
#ifdef __KERNEL__
typedef HI_S32 (*FN_GPIO_Suspend)(PM_BASEDEV_S* , pm_message_t );
typedef HI_S32 (*FN_GPIO_Resume)(PM_BASEDEV_S* );
#endif

typedef struct
{
    FN_GPIO_Get_Bit                pfnGpioDirGetBit;
    FN_GPIO_Set_Bit                pfnGpioDirSetBit;
    FN_GPIO_Get_Bit                pfnGpioReadBit;
    FN_GPIO_Set_Bit                pfnGpioWriteBit;
    FN_GPIO_Get_Num                pfnGpioGetNum;
    FN_GPIO_Register_Server_Func   pfnGpioRegisterServerFunc;
    FN_GPIO_UnRegister_Server_Func pfnGpioUnRegisterServerFunc;
    FN_GPIO_Set_Int_Type           pfnGpioSetIntType;
    FN_GPIO_Set_Int_Enable         pfnGpioSetIntEnable;
    FN_GPIO_Clear_GroupInt         pfnGpioClearGroupInt;
    FN_GPIO_Clear_BitInt           pfnGpioClearBitInt;
#ifdef __KERNEL__
    FN_GPIO_Suspend				   pfnGpioSuspend;
    FN_GPIO_Resume				   pfnGpioResume;
#endif

} GPIO_EXT_FUNC_S;

#endif
