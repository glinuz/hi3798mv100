#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "hi_error_mpi.h"
#include "hi_module.h"
#include "hi_drv_struct.h"
#include "hi_unf_pwm.h"
#include "drv_pwm_ioctl.h"


static HI_S32            g_PwmDevFd    = -1;
static const HI_CHAR     g_PwmDevName[] ="/dev/"UMAP_DEVNAME_PWM;
static pthread_mutex_t   g_PwmMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_PWM_LOCK()       (void)pthread_mutex_lock(&g_PwmMutex);
#define HI_PWM_UNLOCK()     (void)pthread_mutex_unlock(&g_PwmMutex);

#define PWM_CHECK_INIT()        \
    do{                         \
        HI_PWM_LOCK();          \
        if (g_PwmDevFd < 0)     \
        {                       \
            HI_ERR_PWM("pwm is not init!\n");   \
            HI_PWM_UNLOCK();                    \
            return HI_ERR_PWM_NOT_INIT;         \
        }                       \
        HI_PWM_UNLOCK();        \
    }while(0)


static HI_S32 PWM_CheckVersion(HI_VOID)
{
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    return HI_ERR_PWM_UNSUPPORT;
#else
    return HI_SUCCESS;
#endif
}

#define PWM_CHECK_VERSION()                     \
    do{                                         \
        if (HI_SUCCESS != PWM_CheckVersion())   \
        {                                       \
            HI_ERR_PWM("pwm is unsupport!\n");  \
            return HI_ERR_PWM_UNSUPPORT;        \
        }                                       \
    }while(0)

HI_S32 HI_UNF_PWM_Init(HI_VOID)
{
    PWM_CHECK_VERSION();

    HI_PWM_LOCK();

    if (g_PwmDevFd > 0)
    {
        HI_PWM_UNLOCK();
        return HI_SUCCESS;
    }

    g_PwmDevFd = open(g_PwmDevName, O_RDWR | O_NONBLOCK, 0);
    if (g_PwmDevFd < 0)
    {
        HI_ERR_PWM("pwm device open err!\n");
        HI_PWM_UNLOCK();
        return HI_ERR_PWM_OPEN_ERR;
    }

    HI_PWM_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_DeInit(HI_VOID)
{
    HI_S32      Ret;

    PWM_CHECK_VERSION();

    HI_PWM_LOCK();

    if (g_PwmDevFd < 0)
    {
        HI_PWM_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_PwmDevFd);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PWM("pwm device close err!\n");
        HI_PWM_UNLOCK();
        return HI_ERR_PWM_CLOSE_ERR;
    }

    g_PwmDevFd = -1;

    HI_PWM_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_Open(HI_UNF_PWM_E enPWM)
{
    PWM_CHECK_VERSION();

    PWM_CHECK_INIT();

    if (enPWM >= HI_UNF_PWM_BUTT)
    {
        HI_ERR_PWM("ERR: invalid param!\n");
        return HI_ERR_PWM_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_Close(HI_UNF_PWM_E enPWM)
{
    PWM_CHECK_VERSION();

    PWM_CHECK_INIT();

    if (enPWM >= HI_UNF_PWM_BUTT)
    {
        HI_ERR_PWM("ERR: invalid param!\n");
        return HI_ERR_PWM_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_GetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr)
{
    HI_S32                  Ret;
    PWM_ATTR_CMD_PARA_S     stCmdPara;

    PWM_CHECK_VERSION();

    PWM_CHECK_INIT();

    if (enPWM >= HI_UNF_PWM_BUTT || HI_NULL == pstAttr)
    {
        HI_ERR_PWM("ERR: invalid param!\n");
        return HI_ERR_PWM_INVALID_PARA;
    }

    stCmdPara.enPwm = enPWM;
    stCmdPara.stPwmAttr = *pstAttr;

    Ret = ioctl(g_PwmDevFd, CMD_PWM_GETATTR, &stCmdPara);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PWM("ERR: get attr err, Ret=%#x!\n", Ret);
        return HI_ERR_PWM_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_SetAttr(HI_UNF_PWM_E enPWM, HI_UNF_PWM_ATTR_S *pstAttr)
{
    HI_S32                  Ret;
    PWM_ATTR_CMD_PARA_S     stCmdPara;

    PWM_CHECK_VERSION();

    PWM_CHECK_INIT();

    if (enPWM >= HI_UNF_PWM_BUTT || HI_NULL == pstAttr)
    {
        HI_ERR_PWM("ERR: invalid param!\n");
        return HI_ERR_PWM_INVALID_PARA;
    }

    stCmdPara.enPwm = enPWM;
    stCmdPara.stPwmAttr = *pstAttr;
    Ret = ioctl(g_PwmDevFd, CMD_PWM_SETATTR, &stCmdPara);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PWM("ERR: set attr err, Ret=%#x!\n", Ret);
        return HI_ERR_PWM_INVALID_OPT;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_PWM_SendSignal(HI_UNF_PWM_E enPWM, HI_U32 u32CarrierSigDurationUs, HI_U32 u32LowLevelSigDurationUs)
{
    HI_S32                      Ret;
    PWM_SIGNAL_CMD_PARA_S       stCmdPara;

    PWM_CHECK_VERSION();

    PWM_CHECK_INIT();

    if (enPWM >= HI_UNF_PWM_BUTT)
    {
        HI_ERR_PWM("ERR: invalid param!\n");
        return HI_ERR_PWM_INVALID_PARA;
    }

    stCmdPara.enPwm = enPWM;
    stCmdPara.u32CarrierSigDurationUs = u32CarrierSigDurationUs;
    stCmdPara.u32LowLevelSigDurationUs = u32LowLevelSigDurationUs;
    Ret = ioctl(g_PwmDevFd, CMD_PWM_SENDSIGNAL, &stCmdPara);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_PWM("ERR: send signal err, Ret=%#x!\n", Ret);
        return HI_ERR_PWM_INVALID_OPT;
    }

    return HI_SUCCESS;

}




