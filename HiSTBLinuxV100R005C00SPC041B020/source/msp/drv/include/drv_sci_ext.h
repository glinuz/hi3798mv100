/*     extdrv/include/gpio_i2c.h for Linux .
 *
 *
 * This file declares functions for user.
 *
 * History:
 *     2013-01-07 create this file
 *
 */

#ifndef _DRV_SCI_EXT_H
#define _DRV_SCI_EXT_H

#include "hi_type.h"
#include "hi_drv_dev.h"

HI_S32 SCI_DRV_ModInit(HI_VOID);
HI_VOID SCI_DRV_ModExit(HI_VOID);

typedef HI_S32 (*FN_SCI_Suspend)(PM_BASEDEV_S* , pm_message_t );
typedef HI_S32 (*FN_SCI_Resume)(PM_BASEDEV_S* );

typedef struct
{
    FN_SCI_Suspend				   pfnSciSuspend;
    FN_SCI_Resume				   pfnSciResume;
} SCI_EXT_FUNC_S;

#endif
