#include "user.h"
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include "hi_drv_i2c.h"
#include "drv_tuner_ioctl.h"

//spinlock_t User_lock = SPIN_LOCK_UNLOCKED;
struct semaphore it9133_sem;

/**
 * Variable of critical section
 */

Dword User_memoryCopy (
    IN  Demodulator*    demodulator,
    IN  void*           dest,
    IN  void*           src,
    IN  Dword           count
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  memcpy(dest, src, (size_t)count);
     *  return (0);
     */
    memcpy(dest, src, (size_t)count);
    return (Error_NO_ERROR);
}

Dword User_delay (
    IN  Demodulator*    demodulator,
    IN  Dword           dwMs
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  delay(dwMs);
     *  return (0);
     */

    msleep_interruptible(dwMs);
    //mdelay(dwMs/**2*/);
    
    return (Error_NO_ERROR);
}


Dword User_enterCriticalSection (
    IN  Demodulator*    demodulator
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  return (0);
     */
    //spin_lock(&User_lock);
    HI_S32 s32Ret = 0; 
    s32Ret = down_interruptible (&it9133_sem);
    return (s32Ret);
}


Dword User_leaveCriticalSection (
    IN  Demodulator*    demodulator
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  return (0);
     */
    //spin_unlock(&User_lock);
    up(&it9133_sem);
    return (Error_NO_ERROR);
}


Dword User_mpegConfig (
    IN  Demodulator*    demodulator
) {
    /*
     *  ToDo:  Add code here
     *
     */
    return (Error_NO_ERROR);
}


Dword User_busTx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    IN  Byte*           buffer
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  short i;
     *
     *  start();
     *  write_i2c(uc2WireAddr);
     *  ack();
     *  for (i = 0; i < bufferLength; i++) {
     *      write_i2c(*(ucpBuffer + i));
     *      ack();
     *  }
     *  stop();
     *
     *  // If no error happened return 0, else return error code.
     *  return (0);
     */
    HI_S32 s32Ret = 0;
    DefaultDemodulator* demod = (DefaultDemodulator *)demodulator;
    HI_U8 I2cDevAddr = demod->demodAddr;
    HI_U32 I2cNum = ((TUNER_OPS_S *)(demod->userData))->enI2cChannel;

    if (4 >= I2cNum)
    {
        s32Ret = HI_DRV_I2C_Write(I2cNum, I2cDevAddr, 0, 0, buffer, bufferLength);
    }

    if (HI_SUCCESS == s32Ret)
        return (Error_NO_ERROR);
    else
        return (Error_WRITE_TUNER_FAIL);
}


Dword User_busRx (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    OUT Byte*           buffer
) {
    /*
     *  ToDo:  Add code here
     *
     *  //Pseudo code
     *  short i;
     *
     *  start();
     *  write_i2c(uc2WireAddr | 0x01);
     *  ack();
     *  for (i = 0; i < bufferLength - 1; i++) {
     *      read_i2c(*(ucpBuffer + i));
     *      ack();
     *  }
     *  read_i2c(*(ucpBuffer + bufferLength - 1));
     *  nack();
     *  stop();
     *
     *  // If no error happened return 0, else return error code.
     *  return (0);
     */
    HI_S32 s32Ret = 0;
    DefaultDemodulator* demod = (DefaultDemodulator *)demodulator;
    HI_U8 I2cDevAddr = demod->demodAddr;
    HI_U32 I2cNum = ((TUNER_OPS_S *)(demod->userData))->enI2cChannel;

    if (4 >= I2cNum)
    {
        s32Ret = HI_DRV_I2C_ReadDirectly(I2cNum, I2cDevAddr, 0, 0,  buffer, bufferLength);
    }

    if (HI_SUCCESS == s32Ret)
        return (Error_NO_ERROR);
    else
        return (Error_READ_TUNER_FAIL);
}


Dword User_busRxData (
    IN  Demodulator*    demodulator,
    IN  Dword           bufferLength,
    OUT Byte*           buffer
) {
    return (Error_NO_ERROR);
}
